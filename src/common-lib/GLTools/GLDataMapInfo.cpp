/**
 * @file GLDataMapInfo.cpp
 * @brief implements of GLDataMapInfo class
 *
 * @author S.Tanaka
 * @date 2008.01.21
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "GLDataMapInfo.h"

#include <math.h>
#include <algorithm>


using namespace kome::gl;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



#define GRAPH_WIDTH			100.0
#define SECTION				"3D Datamap"
#define INT_TYPE_KEY		   "INTENSITY_VIEW_TYPE"
#define INT_VALUE_KEY		  "ABSOLUTE_MAX_INTENSITY"
#define SYNCHRONIZED_KEY	   "SYNCHRONIZED_VIEW"
#define ABS_VAL				"absolute"
#define AXIS_DRAWING_KEY	   "AXIS_DRAWING"


// static member
std::set< GLDataMapInfo* > GLDataMapInfo::m_graphSet;
kome::core::Matrix GLDataMapInfo::m_commonView( 4, 4 );
kome::core::Matrix GLDataMapInfo::m_commonTransform( 4, 4 );
bool GLDataMapInfo::m_syncView = false;
bool GLDataMapInfo::m_axisDrawing = true;


// constructor
GLDataMapInfo::GLDataMapInfo( kome::objects::DataMapInfo& dataMap ) 
		: m_dataMap( dataMap ), m_transform( 4, 4 ), m_view( 4, 4 ) {
	// initialize
	m_maxInt = 0.0f;
	m_transform = kome::numeric::MatrixTool::getIdentityMatrix( 4 );
	initView( m_view );

	// create array
	if( m_dataMap.getRowSize() == 0 || m_dataMap.getColSize() == 0 ) {
		m_vertices = NULL;
		m_normals = NULL;
	}
	else {
		int size = m_dataMap.getRowSize() * m_dataMap.getColSize();

		m_vertices = new Vec4[ size ];
		m_normals = new Vec3[ size ];
	}

	// insert to map
	m_graphSet.insert( this );

	// sync view
	setSyncView();

	// draw axis
	setAxisDrawing();

	// update graph data
	update();
}

// destructor
GLDataMapInfo::~GLDataMapInfo() {
	// delete from set
	m_graphSet.erase( this );

	// delete array
	if( m_vertices != NULL ) {
		delete[] m_vertices;
	}
	if( m_normals != NULL ) {
		delete[] m_normals;
	}
}

// get data map
kome::objects::DataMapInfo& GLDataMapInfo::getDataMap() {
	return m_dataMap;
}

// get graph width
double GLDataMapInfo::getGraphWidth() {
	return GRAPH_WIDTH;
}

// get the number of rows
unsigned int GLDataMapInfo::getRowCount() {
	return m_dataMap.getRowSize();
}

// get the number of columns
unsigned int GLDataMapInfo::getColCount() {
	return m_dataMap.getColSize();
}

// update graph information
void GLDataMapInfo::update() {
	createVertices();
	createNormals();
}

// get coordinate
void GLDataMapInfo::getXYZ(
		const unsigned int row,
		const unsigned int col,
		float* x,
		float* y,
		float* z,
		float* t
) {
	// index
	int idx = row * m_dataMap.getColSize() + col;

	// get coordinate
	*x = m_vertices[ idx ][ 0 ];
	*y = m_vertices[ idx ][ 1 ];
	*z = m_vertices[ idx ][ 2 ];
	*t = m_vertices[ idx ][ 3 ];
}

// get normal
void GLDataMapInfo::getNormal(
		const unsigned int row,
		const unsigned int col,
		float* x,
		float* y,
		float* z
) {
	// index
	int idx = row * m_dataMap.getColSize() + col;

	*x = m_normals[ idx ][ 0 ];
	*y = m_normals[ idx ][ 1 ];
	*z = m_normals[ idx ][ 2 ];	
}

// get maximum intensity
double GLDataMapInfo::getMaxIntensity() {
	return m_maxInt;
}

// create vertices
void GLDataMapInfo::createVertices() {
	// ini
	kome::core::MsppManager& msppMgr = kome::core::MsppManager::getInstance();
	kome::core::IniFile* ini = msppMgr.getIniFile();

	// get range
	float minXYZ = - (float)GRAPH_WIDTH;
	float maxXYZ = (float)GRAPH_WIDTH;
	float xUnitRange = ( maxXYZ - minXYZ ) / (float)( m_dataMap.getColSize() - 1 );
	float yUnitRange = ( maxXYZ - minXYZ ) / (float)( m_dataMap.getRowSize() - 1 );

	float maxIntensity = std::max( fabs( m_dataMap.getMaxIntensity() ), fabs( m_dataMap.getMinIntensity() ) );
	if( ini != NULL ) {
		std::string intType = ini->getString( SECTION, INT_TYPE_KEY, "relative" );
		if( intType.compare( ABS_VAL ) == 0 ) {
			maxIntensity = (float)( ini->getDouble( SECTION, INT_VALUE_KEY, 15000.0 ) );
		}
	}
	float minIntensity = - maxIntensity;
	m_maxInt = maxIntensity;

	float intRange = maxIntensity - minIntensity;

	// get vertices
	for( unsigned int i = 0; i < m_dataMap.getRowSize(); i++ ) {
		// y
		float y = minXYZ + yUnitRange * (float)i;

		// create vertex array
		for( unsigned int j = 0; j < m_dataMap.getColSize(); j++ ) {
			// x, z
			float x = minXYZ + xUnitRange * (float)j;
			float t = ( m_dataMap.getIntensity( i, j ) - minIntensity ) / intRange;
			float z = minXYZ + t * ( maxXYZ - minXYZ );

			// index
			int idx = i * m_dataMap.getColSize() + j;

			// set
			m_vertices[ idx ][ 0 ] = x;
			m_vertices[ idx ][ 1 ] = y;
			m_vertices[ idx ][ 2 ] = z;
			m_vertices[ idx ][ 3 ] = t;
		}
	}
}

// create normals
void GLDataMapInfo::createNormals() {
	for( int i = 0; i < (int)m_dataMap.getRowSize(); i++ ) {
		for( int j = 0; j < (int)m_dataMap.getColSize(); j++ ) {
			// index
			int idx = i * m_dataMap.getColSize() + j;

			// normal
			kome::core::Vector n( 3 );
			n( 0 ) = 0.0;
			n( 1 ) = 0.0;
			n( 2 ) = 0.0;

			// x, y, z
			double x = m_vertices[ idx ][ 0 ];
			double y = m_vertices[ idx ][ 1 ];
			double z = m_vertices[ idx ][ 2 ];

			// get normal
			int im = i - 1;
			int ip = i + 1;
			int jm = j - 1;
			int jp = j + 1;

			int idxs[ 4 ][ 4 ] = {
				{ i, jm, im, j },
				{ i, jm, ip, j },
				{ i, jp, im, j },
				{ i, jp, ip, j }
			};

			for( int k = 0; k < 4; k++ ) {
				int i0 = idxs[ k ][ 0 ];
				int j0 = idxs[ k ][ 1 ];
				int i1 = idxs[ k ][ 2 ];
				int j1 = idxs[ k ][ 3 ];

				if( j0 >= 0 && j0 < (int)m_dataMap.getColSize()
						&& i1 >= 0 && i1 < (int)m_dataMap.getRowSize() ) {
					kome::core::Vector d0( 3 );
					int idx0 = i0 * m_dataMap.getColSize() + j0;
					d0( 0 ) = m_vertices[ idx0 ][ 0 ] - x;
					d0( 1 ) = m_vertices[ idx0 ][ 1 ] - y;
					d0( 2 ) = m_vertices[ idx0 ][ 2 ] - z;

					kome::core::Vector d1( 3 );
					int idx1 = i1 * m_dataMap.getColSize() + j1;
					d1( 0 ) = m_vertices[ idx1 ][ 0 ] - x;
					d1( 1 ) = m_vertices[ idx1 ][ 1 ] - y;
					d1( 2 ) = m_vertices[ idx1 ][ 2 ] - z;

					kome::core::Vector tmp = d0.cross( d1 );
					if( tmp( 2 ) < 0.0 ) {
						tmp *= -1.0;
					}
					tmp /= tmp.norm();

					n += tmp;
				}
			}

			// normalize
			n /= n.norm();

			// set
			m_normals[ idx ][ 0 ] = (float)n( 0 );
			m_normals[ idx ][ 1 ] = (float)n( 1 );
			m_normals[ idx ][ 2 ] = (float)n( 2 );
		}
	}
}

// get view matrix
kome::core::Matrix& GLDataMapInfo::getView() {
	if( m_syncView ) {
		return m_commonView;
	}
	return m_view;
}

// get transform matrix
kome::core::Matrix& GLDataMapInfo::getTransform() {
	if( m_syncView ) {
		return m_commonTransform;
	}
	return m_transform;
}

// init view
void GLDataMapInfo::initView() {
	// transform
	kome::core::Matrix& transform = getTransform();
	transform = kome::numeric::MatrixTool::getIdentityMatrix( 4 );

	// view
	kome::core::Matrix& view = getView();
	initView( view );
}

// set sync view
void GLDataMapInfo::setSyncView() {
	// ini
	kome::core::MsppManager& msppMgr = kome::core::MsppManager::getInstance();
	kome::core::IniFile* ini = msppMgr.getIniFile();
	if( ini == NULL ) {
		return;
	}

	// get flag
	bool sync = ini->getBool( SECTION, SYNCHRONIZED_KEY, false );

	// set
	setSyncView( sync, NULL );
}

// set sync view
void GLDataMapInfo::setSyncView( const bool sync, kome::objects::DataMapInfo* dataMap ) {
	// check
	if( m_syncView == sync ) {
		return;
	}

	// set view
	if( sync ) {
		// graph information
		GLDataMapInfo* g = NULL;
		if( dataMap != NULL ) {
			for( std::set< GLDataMapInfo* >::iterator it = m_graphSet.begin();
					it != m_graphSet.end() && g == NULL; it++ ) {
				GLDataMapInfo* tmp = *it;
				if( &( tmp->getDataMap() ) == dataMap ) {
					g = tmp;
				}
			}
		}
		if( g == NULL && m_graphSet.size() > 0 ) {
			g = *( m_graphSet.begin() );
		}

		// set view
		if( g == NULL ) {
			initView( m_commonView );
			m_commonTransform = kome::numeric::MatrixTool::getIdentityMatrix( 4 );
		}
		else {
			m_commonView = g->getView();
			m_commonTransform = g->getTransform();
		}
	}
	else {
		// set view
		for( std::set< GLDataMapInfo* >::iterator it = m_graphSet.begin(); it != m_graphSet.end(); it++ ) {
			GLDataMapInfo* g = *it;

			g->getView() = m_commonView;
			g->getTransform() = m_commonTransform;
		}
	}

	// set flag
	m_syncView = sync;
}

// get sync view
bool GLDataMapInfo::isSyncView() {
	return m_syncView;
}

// set axis drawing
void GLDataMapInfo::setAxisDrawing() {
	// ini
	kome::core::MsppManager& msppMgr = kome::core::MsppManager::getInstance();
	kome::core::IniFile* ini = msppMgr.getIniFile();
	if( ini == NULL ) {
		return;
	}

	// get flag
	bool drawing = ini->getBool( SECTION, AXIS_DRAWING_KEY, true );

	// set
	setAxisDrawing( drawing );
}

// set axis drawing
void GLDataMapInfo::setAxisDrawing( const bool axisDrawing ) {
	m_axisDrawing = axisDrawing;
}

// get axis drawing
bool GLDataMapInfo::isAxisDrawing() {
	return m_axisDrawing;
}

// init view
void GLDataMapInfo::initView( kome::core::Matrix& view ) {
	// eye
	kome::core::Vector eye( 3 );
	eye( 0 ) = 0.0;
	eye( 1 ) = -300.0;
	eye( 2 ) = 300.0;

	// look at
	kome::core::Vector lookAt( 3 );
	lookAt( 0 ) = 0.0;
	lookAt( 1 ) = 0.0;
	lookAt( 2 ) = 0.0;

	// direction
	kome::core::Vector dir = lookAt - eye;
	dir /= dir.norm();

	// up
	kome::core::Vector right( 3 );
	right( 0 ) = 1.0;
	right( 1 ) = 0.0;
	right( 2 ) = 0.0;

	kome::core::Vector up = right.cross( dir );
	up /= up.norm();

	// right
	right = dir.cross( up );
	right /= right.norm();

	// set to matrix
	view( 0, 0 ) = right( 0 );   view( 0, 1 ) = up( 0 );   view( 0, 2 ) = dir( 0 );   view( 0, 3 ) = eye( 0 );
	view( 1, 0 ) = right( 1 );   view( 1, 1 ) = up( 1 );   view( 1, 2 ) = dir( 1 );   view( 1, 3 ) = eye( 1 );
	view( 2, 0 ) = right( 2 );   view( 2, 1 ) = up( 2 );   view( 2, 2 ) = dir( 2 );   view( 2, 3 ) = eye( 2 );
	view( 3, 0 ) = 0.0;		  view( 3, 1 ) = 0.0;	   view( 3, 2 ) = 0.0;		view( 3, 3 ) = 1.0;
}

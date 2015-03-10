/**
 * @file Sample3DCanvas.cpp
 * @brief implements of Sample3DCanvas class
 *
 * @author S.Tanaka
 * @date 2008.01.21
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "Sample3DCanvas.h"
#include "Sample3DViewManager.h"

#include <GL/glu.h>
#include <wx/event.h>	// for EVT_KEY_DOWN, EVT_KEY_UP


using namespace kome::view;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



#define Z_NEAR					  1.0
#define Z_FAR				   1000.0
#define FOVY					 30.0
#define DELTA_THETA				  1.0
#define DELTA_MOVE				  1.0

#define RESET_MENU_ID			10000
#define TIMER_ID                654

#define GL_CALL_TYPE			"GL_DRAW_DATA_MAP"
#define DRAW_INFO_PARAM_NAME	"draw info"


BEGIN_EVENT_TABLE( Sample3DCanvas, wxGLCanvas )
	EVT_KEY_DOWN( Sample3DCanvas::onKeyUpDown )	
	EVT_KEY_UP( Sample3DCanvas::onKeyUpDown )	
	EVT_PAINT( Sample3DCanvas::onPaint )
	EVT_SIZE( Sample3DCanvas::onSize )
	EVT_MOUSE_EVENTS( Sample3DCanvas::onMouse )
	EVT_ERASE_BACKGROUND( Sample3DCanvas::onEraseBackground )
	EVT_TIMER( TIMER_ID, Sample3DCanvas::onTimer )
	EVT_SHOW( Sample3DCanvas::onDestroy )
	EVT_MENU( RESET_MENU_ID, Sample3DCanvas::onReset )
	EVT_MENU_RANGE( ID_POPUP_MENU_MIN, ID_POPUP_MENU_MAX, Sample3DCanvas::onMenuPopup )
END_EVENT_TABLE()


// static member
std::vector< Sample3DCanvas* > Sample3DCanvas::m_canvases;


// constructor
Sample3DCanvas::Sample3DCanvas( wxWindow* parent, kome::objects::DataMapInfo& dataMap )
		: wxGLCanvas(
			parent,
			wxID_ANY,
			wxDefaultPosition,
			parent->GetClientSize()
		),
		m_graphInfo( dataMap ) {
	// initialize
	m_init = false;

	// insert to set
	m_canvases.push_back( this );

	m_isLeftDown = false;

	// timer
	m_timer = new wxTimer( this, TIMER_ID );
	m_preparingAnimation = false;
}

// destructor
Sample3DCanvas::~Sample3DCanvas() {
	// search
	int idx = -1;
	for( int i = 0; i < (int)m_canvases.size() && idx < 0; i++ ) {
		if( this == m_canvases[ i ] ) {
			idx = i;
		}
	}

	// delete from canvas
	if( idx >= 0 ) {
		m_canvases.erase( m_canvases.begin() + idx );
	}

	// timer
	if( m_timer != NULL ) {
		if( m_timer->IsRunning() ) {
			m_timer->Stop();
		}
		delete m_timer;
		m_timer = NULL;
	}
}

// get graph infromation
kome::gl::GLDataMapInfo& Sample3DCanvas::getGraphInfo() {
	return m_graphInfo;
}

// init OpenGL
void Sample3DCanvas::initGL() {
	// clear
	glClearColor( 1.0, 1.0, 1.0, 1.0 );
	glClearDepth( 1.0 );
	glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );

	// depth test
	glDepthFunc( GL_LEQUAL );	
}

// render
void Sample3DCanvas::render() {
	// clear
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	// depath test
	glEnable( GL_DEPTH_TEST );

	// Model View
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();

	// look
	look();

	// light
	light();

	// set transform
	glPushMatrix();
	kome::core::Matrix& transform = m_graphInfo.getTransform();
	GLdouble m[ 16 ] = {
		transform( 0, 0 ),
		transform( 1, 0 ),
		transform( 2, 0 ),
		transform( 3, 0 ),
		transform( 0, 1 ),
		transform( 1, 1 ),
		transform( 2, 1 ),
		transform( 3, 1 ),
		transform( 0, 2 ),
		transform( 1, 2 ),
		transform( 2, 2 ),
		transform( 3, 2 ),
		transform( 0, 3 ),
		transform( 1, 3 ),
		transform( 2, 3 ),
		transform( 3, 3 )
	};
	glMultMatrixd( m );

	// draw graph
	drawGraph();

	// draw axis
	drawSurfaces();

	if( m_graphInfo.isAxisDrawing() ) {
		drawAxis();
	}

	// draw others
	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();

	kome::objects::Parameters params;
	kome::objects::Variant& drawInfo = params.getValue( DRAW_INFO_PARAM_NAME );
	drawInfo.type = kome::objects::Variant::OTHER;
	drawInfo.prim.pt = &m_graphInfo;

	plgMgr.callAllFunctions( &params, GL_CALL_TYPE );

	// pop matrix
	glPopMatrix();
}

// look at
void Sample3DCanvas::look() {
	// matrix
	kome::core::Matrix& view = m_graphInfo.getView();

	// look at
	kome::core::Vector lookAt( 3 );
	lookAt( 0 ) = view( 0, 3 ) + view ( 0, 2 );
	lookAt( 1 ) = view( 1, 3 ) + view ( 1, 2 );
	lookAt( 2 ) = view( 2, 3 ) + view ( 2, 2 );

	// set look
	gluLookAt(
		view( 0, 3 ), view( 1, 3 ), view( 2, 3 ),
		lookAt( 0 ),  lookAt( 1 ),  lookAt( 2 ),
		view( 0, 1 ), view( 1, 1 ), view( 2, 1 )
	);		
}

// set light
void Sample3DCanvas::light() {
	// light 0
	static GLfloat lightAmb0[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	static GLfloat lightDif0[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	static GLfloat lightSpe0[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	static GLfloat lightPos0[] = { 0.0f, -1.0f, 1.0f, 0.0f };

	glLightfv( GL_LIGHT0, GL_AMBIENT, lightAmb0 );
	glLightfv( GL_LIGHT0, GL_DIFFUSE, lightDif0 );
	glLightfv( GL_LIGHT0, GL_SPECULAR, lightSpe0 );
	glLightfv( GL_LIGHT0, GL_POSITION, lightPos0 );

	// enable
	glEnable( GL_LIGHTING );
	glEnable( GL_LIGHT0 );
}

// texture
void Sample3DCanvas::texture() {
	// parameter
	glTexParameterf( GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP );
	glTexParameterf( GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameterf( GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );

	// environment
	glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );

	// model
	glShadeModel( GL_SMOOTH );
}

// draw graph
void Sample3DCanvas::drawGraph() {
	// manager
	Sample3DViewManager& mgr = Sample3DViewManager::getInstance();

	// texture
	texture();

	// enable texture
	glEnable( GL_TEXTURE_1D );

	// material
	GLfloat materialAmb[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	GLfloat materialDif[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	GLfloat materialSpe[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	GLfloat materialEmi[] = { 0.3f, 0.3f, 0.3f, 1.0f };
	GLfloat materialShine = 50.0f;

	glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT, materialAmb );
	glMaterialfv( GL_FRONT_AND_BACK, GL_DIFFUSE, materialDif );
	glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR, materialSpe );
	glMaterialfv( GL_FRONT_AND_BACK, GL_EMISSION, materialEmi );
	glMaterialf( GL_FRONT_AND_BACK, GL_SHININESS, materialShine );

	// variables
	float x = float();
	float y = float();
	float z = float();
	float t = float();

	// draw
	glTexImage1D(
		GL_TEXTURE_1D,
		0,
		GL_RGB,
		mgr.getGraphTextureImageSize(),
		0,
		GL_RGB,
		GL_UNSIGNED_BYTE,
		mgr.getGraphTextureImage()
	);

	for( unsigned int i = 0; i < m_graphInfo.getRowCount() - 1; i++ ) {

		glBegin( GL_TRIANGLE_STRIP );

		for( unsigned int j = 0; j < m_graphInfo.getColCount(); j++ ) {
			// normal
			m_graphInfo.getNormal( i, j, &x, &y, &z );
			glNormal3f( x, y, z );

			// texture & coordinate
			m_graphInfo.getXYZ( i, j, &x, &y, &z, &t );
			glTexCoord1f( t );
			glVertex3f( x, y, z );

			// normal
			m_graphInfo.getNormal( i + 1, j, &x, &y, &z );
			glNormal3f( x, y, z );

			// texture & coordinate
			m_graphInfo.getXYZ( i + 1, j, &x, &y, &z, &t );
			glTexCoord1f( t );
			glVertex3f( x, y, z );
		}

		glEnd();
	}

	// disable texture
	glDisable( GL_TEXTURE_1D );
}

// draw axis
void Sample3DCanvas::drawSurfaces() {
	// matrix
	kome::core::Matrix& view = m_graphInfo.getView();
	kome::core::Matrix& transform = m_graphInfo.getTransform();

	// material
	GLfloat materialAmb0[] = { 0.8f, 0.8f, 0.8f, 1.0f };
	GLfloat materialDif0[] = { 0.1f, 0.1f, 0.1f, 1.0f };
	GLfloat materialSpe0[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	GLfloat materialEmi0[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	GLfloat materialShine0 = 10.0f;

	glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT, materialAmb0 );
	glMaterialfv( GL_FRONT_AND_BACK, GL_DIFFUSE, materialDif0 );
	glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR, materialSpe0 );
	glMaterialfv( GL_FRONT_AND_BACK, GL_EMISSION, materialEmi0 );
	glMaterialf( GL_FRONT_AND_BACK, GL_SHININESS, materialShine0 );

	// range
	float range = (float)m_graphInfo.getGraphWidth();

	// view direction
	kome::core::Vector dir( 3 );
	dir( 0 ) = view( 0, 2 );
	dir( 1 ) = view( 1, 2 );
	dir( 2 ) = view( 2, 2 );

	// mz - intensity surface
	kome::core::Vector mzn( 3 );
	mzn( 0 ) = transform( 0, 1 );
	mzn( 1 ) = transform( 1, 1 );
	mzn( 2 ) = transform( 2, 1 );

	float y0 = - range;
	float ny = -1.0f;
	if( dir.dot( mzn ) > 0.0 ) {
		y0 = -y0;
		ny = -ny;
	}
	glNormal3f( 0.0, ny, 0.0 );
	glBegin( GL_POLYGON );
		glVertex3f( -range, y0, -range );
		glVertex3f( range, y0, -range );
		glVertex3f( range, y0, range );
		glVertex3f( -range, y0, range );
	glEnd();

	// rt - intensity surface
	kome::core::Vector rtn( 3 );
	rtn( 0 ) = transform( 0, 0 );
	rtn( 1 ) = transform( 1, 0 );
	rtn( 2 ) = transform( 2, 0 );

	float x0 = - range;
	float nx = 1.0f;
	if( dir.dot( rtn ) > 0.0 ) {
		x0 = -x0;
		nx = -nx;
	}
	
	glNormal3f( nx, 0.0, 0.0 );
	glBegin( GL_POLYGON );
		glVertex3f( x0, -range, -range );
		glVertex3f( x0, range, -range );
		glVertex3f( x0, range, range );
		glVertex3f( x0, -range, range );
	glEnd();
}

// draw axis
void Sample3DCanvas::drawAxis() {
	// matrix
	kome::core::Matrix& view = m_graphInfo.getView();
	kome::core::Matrix& transform = m_graphInfo.getTransform();

	// prepare
	glDisable( GL_LIGHTING );
	glDisable( GL_DEPTH_TEST );
	glColor3f( 0.0f, 0.0f, 0.0f );

	// range
	float range = (float)m_graphInfo.getGraphWidth();

	// view direction
	kome::core::Vector dir( 3 );
	dir( 0 ) = view( 0, 2 );
	dir( 1 ) = view( 1, 2 );
	dir( 2 ) = view( 2, 2 );

	kome::core::Vector up( 3 );
	up( 0 ) = view( 0, 1 );
	up( 1 ) = view( 1, 1 );
	up( 2 ) = view( 2, 1 );

	// m/z axis position
	kome::core::Vector mzn( 3 );
	mzn( 0 ) = transform( 0, 1 );
	mzn( 1 ) = transform( 1, 1 );
	mzn( 2 ) = transform( 2, 1 );

	float ya = range + 3.0;
	float ys = - range;
	bool yp = false;
	if( dir.dot( mzn ) > 0.0 ) {
		ya = -ya;
		ys = -ys;
		yp = true;
	}

	// rt axis position
	kome::core::Vector rtn( 3 );
	rtn( 0 ) = transform( 0, 0 );
	rtn( 1 ) = transform( 1, 0 );
	rtn( 2 ) = transform( 2, 0 );

	float xa = range + 3.0;
	float xs = - range;
	bool xp = false;
	if( dir.dot( rtn ) > 0.0 ) {
		xa = -xa;
		xs = -xs;
		xp = true;
	}

	// intensity title position
	kome::core::Vector zdir( 3 );
	zdir( 0 ) = transform( 0, 2 );
	zdir( 1 ) = transform( 1, 2 );
	zdir( 2 ) = transform( 2, 2 );

	float za = range;
	float zs = - range;
	if( up.dot( zdir ) < 0.0 ) {
		za = - za;
		zs = - zs;
	}

	// axis
	glBegin( GL_LINES );
		glVertex3f( xa, ya, 0.0f );
		glVertex3f( xs, ya, 0.0f );
	glEnd();

	glBegin( GL_LINES );
		glVertex3f( xa, ya, 0.0f );
		glVertex3f( xa, ys, 0.0f );
	glEnd();

	// axis title
	drawAxisTitle( "rt", xa, ys, 0.0f );	
	drawAxisTitle( "m/z", xs, ya, 0.0f );

	// axis scale
	double mz1 = m_graphInfo.getDataMap().getMinMz();
	double mz2 = m_graphInfo.getDataMap().getMaxMz();
	double rt1 = m_graphInfo.getDataMap().getMinRt();
	double rt2 = m_graphInfo.getDataMap().getMaxRt();
	double intensity1 = - m_graphInfo.getMaxIntensity();
	double intensity2 = m_graphInfo.getMaxIntensity();
	
	int n = 20;
	double delta_mz = (mz2 - mz1) / n;
	double delta_rt = (rt2 - rt1) / n;
	double delta_int = (intensity2 - intensity1) / n;

	// y axis
	calcDrawAxisTitle( rt1, rt2, range, ya, ys, xa, yp, delta_rt, 0, xs, ys );

	// x axis 
	calcDrawAxisTitle( mz1, mz2, range, xa, xs, ya, xp, delta_mz, 1, xs, ys );

	// z axis
	double d1 = ( xp?(-1.0):(1.0) );
	double d2 = ( yp?(-1.0):(1.0) );
	double p = dir.dot( zdir );

	calcDrawAxisTitle( intensity1, intensity2, range, za, zs, -1.0, true, delta_int, 2, xs, ys, d1, d2, p );
}

// draw axis title
void Sample3DCanvas::drawAxisTitle(
		const char* s,
		const float x,
		const float y,
		const float z
) {
	// matrix
	kome::core::Matrix& view = m_graphInfo.getView();
	kome::core::Matrix& transform = m_graphInfo.getTransform();

	// vector
	kome::core::Vector vec( 3 );
	vec( 0 ) = transform( 0, 0 ) * x + transform( 0, 1 ) * y + transform( 0, 2 ) * z + transform( 0, 3 );
	vec( 1 ) = transform( 1, 0 ) * x + transform( 1, 1 ) * y + transform( 1, 2 ) * z + transform( 1, 3 );
	vec( 2 ) = transform( 2, 0 ) * x + transform( 2, 1 ) * y + transform( 2, 2 ) * z + transform( 2, 3 );
	
	kome::core::Vector right( 3 );
	right( 0 ) = view( 0, 0 );
	right( 1 ) = view( 1, 0 );
	right( 2 ) = view( 2, 0 );

	kome::core::Vector up( 3 );
	up( 0 ) = view( 0, 1 );
	up( 1 ) = view( 1, 1 );
	up( 2 ) = view( 2, 1 );

	// align
	kome::img::Graphics::TextHAlign hAlign = kome::img::Graphics::HALIGN_LEFT;
	if( vec.dot( right ) < 0 ) {
		hAlign = kome::img::Graphics::HALIGN_RIGHT;
	}

	kome::img::Graphics::TextVAlign vAlign = kome::img::Graphics::VALIGN_BOTTOM;
	if( vec.dot( up ) < 0 ) {
		vAlign = kome::img::Graphics::VALIGN_TOP;
	}

	// draw title
	drawText( s, x, y, z, hAlign, vAlign );
}

// draw text
void Sample3DCanvas::drawText(
		const char* s,
		const float x,
		const float y,
		const float z,
		const kome::img::Graphics::TextHAlign hAlign,
		const kome::img::Graphics::TextVAlign vAlign
) {
	// prepare memory device context
	wxMemoryDC memDc;
	wxBitmap bitmap( 1024, 32 );
	memDc.SelectObject( bitmap );
	memDc.SetBackground( *wxWHITE_BRUSH );
	memDc.Clear();

	// draw text on device context
	wxString str = NVL( s, "" );
	wxSize size = memDc.GetTextExtent( str );
	memDc.DrawText( str, 0, 0 );

	// create bitmap
	unsigned char bits[] = {
		0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01
	};

	wxColour color;
	int h = size.GetHeight();
	int w = size.GetWidth();

	// create map
	std::vector< unsigned char > strMap;
	for( int i = h - 1; i >= 0; i-- ) {
		for( int j = 0; j < w; j++ ) {
			if( j % 8 == 0 ) {
				strMap.push_back( 0 );
			}

			unsigned char b = strMap.back();
		
			memDc.GetPixel( j, i, &color );
			if( color.Red() != wxWHITE->Red()
					|| color.Green() != wxWHITE->Green()
					|| color.Blue() != wxWHITE->Blue()
			) {
				b += bits[ j % 8 ];
				strMap.back() = b;
			}
		}
	}

	// draw text
	float xo = 0.0;
	if( hAlign == kome::img::Graphics::HALIGN_RIGHT ) {
		xo = (float)w;
	}
	else if( hAlign == kome::img::Graphics::HALIGN_CENTER ) {
		xo = (float)w / 2.0f;
	}
	float yo = 0.0;
	if( vAlign == kome::img::Graphics::VALIGN_TOP ) {
		yo = (float)h;
	}
	else if( vAlign == kome::img::Graphics::VALIGN_MIDDLE ) {
		yo = (float)h / 2.0f;
	}
	glRasterPos3f( x, y, z );
	glBitmap( w, h, xo, yo, (float)( w + 1 ), 0.0, &strMap[ 0 ] );
}

// move eye
void Sample3DCanvas::moveEye( const int dx, const int dy ) {
	// matrix
	kome::core::Matrix& view = m_graphInfo.getView();

	// distance
	double d = sqrt( (double)( dx * dx + dy * dy ) ) * DELTA_MOVE;
	if( ABS( dx ) > ABS( dy ) ) {
		if( dx < 0 ) {
			d = -d;
		}
	}
	else {
		if( dy > 0 ) {
			d = -d;
		}
	}

	// near & far
	double n = 10.0;
	double f = Z_FAR * 0.75;

	// now distance
	kome::core::Vector pos( 3 );
	pos( 0 ) = view( 0, 3 );
	pos( 1 ) = view( 1, 3 );
	pos( 2 ) = view( 2, 3 );

	double dd = pos.norm();

	// new distance
	dd -= d;
	if( dd < n ) {
		dd = n;
	}
	if( dd > f ) {
		dd = f;
	}

	// new position
	view( 0, 3 ) = dd * ( - view( 0, 2 ) );
	view( 1, 3 ) = dd * ( - view( 1, 2 ) );
	view( 2, 3 ) = dd * ( - view( 2, 2 ) );

	// refresh
	kome::window::WindowTool::refresh();
}

// rotate
void Sample3DCanvas::rotate( const int dx, const int dy ) {
	// matrix
	kome::core::Matrix& view = m_graphInfo.getView();
	kome::core::Matrix& transform = m_graphInfo.getTransform();

	// check the parameter
	if( dx == 0 && dy == 0 ) {
		return;
	}

	// theta
	double theta = sqrt( (double)( dx * dx + dy * dy ) ) * DELTA_THETA;

	// axis
	kome::core::Vector rotDir( 3 );
	rotDir( 0 ) = view( 0, 0 ) * (double)dx + view( 0, 1 ) * (double)( -dy );
	rotDir( 1 ) = view( 1, 0 ) * (double)dx + view( 1, 1 ) * (double)( -dy );
	rotDir( 2 ) = view( 2, 0 ) * (double)dx + view( 2, 1 ) * (double)( -dy );
	rotDir /= rotDir.norm();

	kome::core::Vector z( 3 );
	z( 0 ) = view( 0, 2 );
	z( 1 ) = view( 1, 2 );
	z( 2 ) = view( 2, 2 );

	kome::core::Vector axis = rotDir.cross( z );
	axis /= axis.norm();

	// rotation matrix
	kome::core::Matrix rot = kome::numeric::Transformation3D::getRotationMatrix4( theta, axis );

	// transform
	transform = rot * transform;

	// refresh
	const unsigned int num = kome::window::ChildFrame::getNumberOfFrames();
	for( unsigned int i = 0; i < num; i++ ) {
		kome::window::ChildFrame* frame = kome::window::ChildFrame::getFrame( i );
		Sample3DCanvas* canvas = dynamic_cast< Sample3DCanvas* >( frame->getCanvas() );
		if( canvas != NULL ) {
			canvas->Refresh();
		}
	}
}

// rotate (with Euler angles)
void Sample3DCanvas::rotate2( const int dx, const int dy ) {
	// matrix
	kome::core::Matrix& view = m_graphInfo.getView();
	kome::core::Matrix& transform = m_graphInfo.getTransform();

	// z axis rotation
	double thetaZ = (double)dx * DELTA_THETA;
	kome::core::Vector zAxis( 3 );
	zAxis( 0 ) = transform( 0, 2 );
	zAxis( 1 ) = transform( 1, 2 );
	zAxis( 2 ) = transform( 2, 2 );
	kome::core::Matrix rotZ = kome::numeric::Transformation3D::getRotationMatrix4( thetaZ, zAxis );

	// x axis rotation
	double thetaX = (double)dy * DELTA_THETA;
	kome::core::Vector xAxis( 3 );
	xAxis( 0 ) = view( 0, 0 );
	xAxis( 1 ) = view( 1, 0 );
	xAxis( 2 ) = view( 2, 0 );
	kome::core::Matrix rotX = kome::numeric::Transformation3D::getRotationMatrix4( thetaX, xAxis );

	// rotation matrix
	kome::core::Matrix rot = rotX * rotZ;

	// transform
	transform = rot * transform;

	// refresh
	const unsigned int num = kome::window::ChildFrame::getNumberOfFrames();
	for( unsigned int i = 0; i < num; i++ ) {
		kome::window::ChildFrame* frame = kome::window::ChildFrame::getFrame( i );
		Sample3DCanvas* canvas = dynamic_cast< Sample3DCanvas* >( frame->getCanvas() );
		if( canvas != NULL ) {
			canvas->Refresh();
		}
	}
}

// kill timer
void Sample3DCanvas::killTimer() {
	if( m_timer != NULL ) {
		if( m_timer->IsRunning() ) {
			m_timer->Stop();
		}
	}
}

// set size
void Sample3DCanvas::setSize( unsigned int w, unsigned int h ) {
	// prepare
	wxGLContext* context = GetContext();
	if( context == NULL ) {
		return;
	}
	if( GetParent()->IsShown() ) {
		SetCurrent();
	}

	/** set viewing projection */
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	gluPerspective( FOVY, (double)w / (double)h, Z_NEAR, Z_FAR );

	// set viewport
	glMatrixMode( GL_MODELVIEW );
	glViewport( 0, 0, (GLint)w, (GLint)h );
}

// @date 2012/11/26 <Add> OKADA ------->
// popup menu
void Sample3DCanvas::showMenu( const int px, const int py ) {
	// 2010.10.12 Add M.Izumi
	if( GetCapture() == this ) {
        ReleaseMouse();
    }
	// point
	wxPoint pt( px, py );

	// create menu
	wxMenu* menu = createMenu();
	if( menu == NULL ) {
		return;
	}

	// popup menu
	PopupMenu( menu, pt.x, pt.y );
	delete menu;
}
// @date 2012/11/26 <Add> OKADA <-------


// on paint
void Sample3DCanvas::onPaint( wxPaintEvent& evt ) {
	// DC
	wxPaintDC dc(this);

	// prepare
	wxGLContext* context = GetContext();
	if( context == NULL ) {
		return;
	}
	SetCurrent();

	// init
	if( !m_init ) {
		// init OpenGL
		initGL();

		// get size
		int w = int();
		int h = int();
		GetClientSize( &w, &h );
		setSize( w, h );

		m_init = true;
	}

	// draw
	render();

	// show
	glFlush();
	SwapBuffers();
}

// on size
void Sample3DCanvas::onSize( wxSizeEvent& evt ) {
	setSize( evt.GetSize().GetWidth(), evt.GetSize().GetHeight() );
	Refresh();
}

// on mouse event
void Sample3DCanvas::onMouse( wxMouseEvent& evt ) {
	// point
	wxPoint pt = evt.GetPosition();

	// get button flag and function type
	if( evt.ButtonDown() ) {		// button down event
		if( m_timer->IsRunning() ) {
			m_timer->Stop();
			m_animationD.px = 0;
			m_animationD.py = 0;
		}
		if( evt.RightDown() ) {		// right button
			if( HasCapture() ) {
				ReleaseMouse();
			}

			showMenu( pt.x, pt.y );
		}
		else {
			// capture
			if( HasCapture() ) {	// @date 2013/03/13 <Add> OKADA	// SPEC 91597
				// this window has the current mouse capture.	// 通常ここは通らない	// @date 2013/03/13 <Add> OKADA	// SPEC 91597
				;					// @date 2013/03/13 <Add> OKADA	// SPEC 91597
			}else{					// @date 2013/03/13 <Add> OKADA	// SPEC 91597
				CaptureMouse();
			}						// @date 2013/03/13 <Add> OKADA	// SPEC 91597
		}
	}
	else if( evt.ButtonUp() ) {		// button up event
		// release
		if( HasCapture() ) {
			ReleaseMouse();
		}
	}
	else if( evt.Dragging() ) {		// dragging
		if( m_timer->IsRunning() ) {
			m_timer->Stop();
			m_animationD.px = 0;
			m_animationD.py = 0;
		}
		if( evt.LeftIsDown() ) {	// left button
			if( evt.ShiftDown() ) { // shift key
				moveEye( pt.x - m_prevPt.px, pt.y - m_prevPt.py );
			}
			else {  // etc
				rotate2( pt.x - m_prevPt.px, pt.y - m_prevPt.py );
			}
		}
	}
	else if( evt.Entering() ) {	// OpenGLでキーボードイベントを取得するためにフォーカスを設定	

	}
	else if( evt.Leaving() ) {
	
	}

	// animation
	if( m_preparingAnimation ) {
		int dx = pt.x - m_prevPt.px;
		int dy = pt.y - m_prevPt.py;

		if( dx != 0 || dy != 0 ) {
			if( abs( dy ) > abs( dx ) ) {
				m_animationD.px = 0;
				m_animationD.py = dy;
			}
			else {
				m_animationD.px = dx;
				m_animationD.py = 0;
			}

			m_timer->Start( 100, false );
		}

		m_preparingAnimation = false;
	}

	if( m_isLeftDown && !evt.ShiftDown() && !evt.LeftIsDown() ) {
		m_preparingAnimation = true;
	}
	else {
		m_preparingAnimation = false;
	}

	// set mouse point
	m_prevPt.px = pt.x;
	m_prevPt.py = pt.y;

	// @date 2012.01.17 <Mod> M.Izumi
	// マウス、キーの状態を保存
	m_isLeftDown = evt.LeftIsDown();
	m_isShiftDown = evt.ShiftDown();

	// マウスカーソル設定
	if( m_isLeftDown  ) { // 左ボタン押下 かつ shift key押下
		if( m_isShiftDown ){
			setMouseCursor( PLUGIN_NAME_3D_VIEW, ICON_NAME_3D_ZOOM );		// @date 2011/06/30 
		}else{
			setMouseCursor( PLUGIN_NAME_3D_VIEW, ICON_NAME_3D_DIRECTION );	// @date 2011/06/30 
		}

	}else{
		setMouseCursor( "", "" );	// @date 2011/06/30 
	}
}

// on erase background
void Sample3DCanvas::onEraseBackground( wxEraseEvent& evt ) {
}

// on timer
void Sample3DCanvas::onTimer( wxTimerEvent& evt ) {
	rotate2( m_animationD.px, m_animationD.py );
}

// on destroy
void Sample3DCanvas::onDestroy( wxShowEvent& evt ) {
	if( !evt.IsShown() ) {
		if( m_timer != NULL ) {
			if( m_timer->IsRunning() ) {
				m_timer->Stop();
			}
			delete m_timer;
			m_timer = NULL;
		}
	}
}

// on reset
void Sample3DCanvas::onReset( wxCommandEvent& evt ) {
	// initialize view
	m_graphInfo.initView();

	// refresh
	kome::window::WindowTool::refresh();
}

// get the number of canvases
unsigned int Sample3DCanvas::getNumberOfCanvases() {
	return m_canvases.size();
}

// get canvas
Sample3DCanvas* Sample3DCanvas::getCanvas( const unsigned int index ) {
	if( index >= m_canvases.size() ) {
		return NULL;
	}
	return m_canvases[ index ];
}

// get canvas
Sample3DCanvas* Sample3DCanvas::getCanvas( kome::objects::DataMapInfo* dataMap ) {
	// search
	Sample3DCanvas* canvas = NULL;
	for( unsigned int i = 0; i < m_canvases.size() && canvas == NULL; i++ ) {
		Sample3DCanvas* c = m_canvases[ i ];
		if( dataMap == &c->getGraphInfo().getDataMap() ) {
			canvas = c;
		}
	}

	return canvas;
}

// on key up down
void Sample3DCanvas::onKeyUpDown( wxKeyEvent& evt) 
{
	// @date 2012.01.17 <Mod> M.Izumi
	m_isShiftDown =  evt.ShiftDown();
	// マウスカーソル設定
	if( m_isLeftDown  ) { // 左ボタン押下 かつ shift key押下
		if( m_isShiftDown ){
			setMouseCursor( PLUGIN_NAME_3D_VIEW, ICON_NAME_3D_ZOOM );		// @date 2011/06/30 
		}else{
			setMouseCursor( PLUGIN_NAME_3D_VIEW, ICON_NAME_3D_DIRECTION );	// @date 2011/06/30 
		}

	}else{
		setMouseCursor( "", "" );	// @date 2011/06/30 
	}

	return;
}

// set mouse cursor
int Sample3DCanvas::setMouseCursor( const char* pluginName , const char* iconName )
{
	kome::window::ChildFrame* frame = kome::window::ChildFrame::getActiveFrame();
	wxWindow* acCanvas = ( frame == NULL ? NULL : frame->getCanvas() );

	do{
		{
			// グラフ表示内
			if( m_isLeftDown ){
				//マウスカーソル変更
				kome::plugin::PluginManager& pluginMgr = kome::plugin::PluginManager::getInstance();
				kome::plugin::PluginInfo* info;
				kome::plugin::PluginIcon* icon;

				if( pluginName == NULL ){
					break;	// while()文の後ろに脱出
				}
				info = pluginMgr.getPlugin( pluginName );
				if( info == NULL ){
					break;	// while()文の後ろに脱出
				}
				icon = info->getIcon( iconName );
				if( icon == NULL ){
					break;	// while()文の後ろに脱出
				}
			
				wxBitmap bitmap( wxT(icon->getPath()), wxBITMAP_TYPE_PNG );
				wxImage img( bitmap.ConvertToImage() );
				int width = img.GetWidth();
				int height = img.GetHeight();

				// hotspot(アイコン中心)の設定
				if( strcmp( iconName, ICON_NAME_3D_DIRECTION ) == 0 ){
					img.SetOption(wxIMAGE_OPTION_CUR_HOTSPOT_X, 13 );
					img.SetOption(wxIMAGE_OPTION_CUR_HOTSPOT_Y, 13 );
				}else if( strcmp( iconName, ICON_NAME_3D_ZOOM ) == 0 ){
					img.SetOption(wxIMAGE_OPTION_CUR_HOTSPOT_X, 4 );
					img.SetOption(wxIMAGE_OPTION_CUR_HOTSPOT_Y, 4 );
				}else{
					break;
				}
			
				// This check is necessary to prevent a crash when no canvas is active.
				if (acCanvas != NULL){
					acCanvas->SetCursor(wxCursor(img));
				}else{
					this->SetCursor(wxCursor(img));
				}
			}else{
				if (acCanvas != NULL){
					// This check is necessary to prevent a crash when no canvas is active.
					acCanvas->SetCursor(wxCursor(wxCURSOR_ARROW));	// 矢印
				}else{
					this->SetCursor(wxCursor(wxCURSOR_ARROW));		// 矢印
				}
			}

			return 0;
		}
	}while(false);
	
	if (acCanvas != NULL)
	{
		// This check is necessary to prevent a crash when no canvas is active.
		acCanvas->SetCursor(wxCursor(wxCURSOR_ARROW));	// 矢印
	}else{
		this->SetCursor(wxCursor(wxCURSOR_ARROW));		// 矢印
	}

	return 0;
}

// @date 2012/11/26 <Add> OKADA ------->
// creates menu
wxMenu* Sample3DCanvas::createMenu()
{
	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();
	kome::plugin::PluginMenu& plugin_menu = plgMgr.getDataMapViewMenu();	// @date 2012/12/12 <Mod> OKADA

	wxMenu* menu = kome::window::Canvas::createPopupMenu( &plugin_menu );

	return menu;
}

// on menu popup
void Sample3DCanvas::onMenuPopup( wxCommandEvent& evt )
{
	// ID
	unsigned int id = evt.GetId();

	// active object
	kome::objects::Variant obj;
	kome::window::ChildFrame* frame = kome::window::ChildFrame::getActiveFrame();
	if( frame == NULL ) {
		kome::objects::ActiveObjectsManager& aoMgr = kome::objects::ActiveObjectsManager::getInstance();
		kome::objects::Sample* acSample = aoMgr.getActiveSample();

		if( acSample != NULL ) {
			obj.type = kome::objects::Variant::DATA_GROUP_NODE;
			obj.prim.pt = acSample->getRootDataGroupNode();
		}// ※acSampleがNULLの場合はobj初期値を使用する
	}
	else {
		obj = frame->getActiveObject();
	}

	kome::plugin::PluginMenu* ptrMenu = kome::window::Canvas::m_contextMenuTreeMap[id];

	ptrMenu->execute( obj );

	return;
}
// @date 2012/11/26 <Add> OKADA <-------

// calc draw axis title
void Sample3DCanvas::calcDrawAxisTitle(
	double min,
	double max,
	float range, 
	float na, 
	float ns, 
	float n,
	bool bflg,
	double delta,
	int axis_n,
	float xs,
	float ys,
	double d1,
	double d2,
	double p
){	
	// 全体的に処理を見直します
	//   きりの良い数字で表示させるには、現行のロジックでは限界があるので、
	//   スケールを引きたい開始位置とステップをきりの良い数字で、
	//   動けるように直します
	//
	double minSpace = ( max - min ) / (double)range;
	int logUnit = (int)ceil( log10( minSpace ) );
	double unit = pow( 10.0, (double)logUnit );

	// １ポイントあたりの数値幅を算出
	double pointSize = abs( ( max - min ) / ( ns - na ) );

	// 開始値の設定
	double	startValue = 0.0;
	if( true == bflg )
	{
		startValue = max;
		// 開始位置から値が減っていく方向なので、切り捨て
		startValue = floor( startValue );
	}
	else
	{
		startValue = min;
		// 開始位置から値が増えていく方向なので、切り上げ
		startValue = ceil( startValue );
	}

	// unit数で開始位置を調整
	startValue = floor( startValue / unit );
	startValue = startValue * unit;

	// スケール描画数の調整
	//   新しい移動ステップで描画するスケール数を再算出します
	int	nIndex = 20;
	delta = ceil( delta );		// 小数点以下切り上げ
	if( delta < unit )
	{
		delta = unit;
	}
	nIndex = ( max - min ) / delta;

	// label format
	std::string fmt = FMT( "%%.%df", ( logUnit < 0 ) ? -logUnit : 0 );
	
	std::string	s;
	double	v;
	double	axis_scale_width = 1.0;
	for ( int i = 1 ; i < nIndex ; i++ )
	{
		// ループに伴う移動距離を算出
		double dStep = delta * i;
		// スケール位置の値を算出します
		v = startValue + ( dStep * ( bflg?(-1.0):(1.0) ) );
		s =  FMT( fmt.c_str(), v );

		// スケール位置の値からPoint位置を算出します
		double dTemp = ( axis_n != 2 ? (( bflg?(max):(min) ) - v ) : (min-v));
		dTemp = floor( dTemp / pointSize );
		double nAxis = ns - dTemp;
		
		bool bScale_z = false;
		// z軸のみ固定
		if( axis_n == 2 ){
			if( ( p < 0.0 && v > 0.0 ) || ( p > 0.0 && v < 0.0 ) ){
				bScale_z = true;
			}
		}

		if  ( i != nIndex )
		{
			if  ( ( i % 2 ) == 0 )
			{

				switch( axis_n ){
				case 0: // y axis
					drawAxisTitle( s.c_str(), n, nAxis, 0.0f );
					break;
				case 1: // x axis
					drawAxisTitle( s.c_str(), nAxis, n, 0.0f );
					break;
				case 2: // z axis
					{
						if( bScale_z ){
							drawAxisTitle( s.c_str(), xs, ys, nAxis );
							axis_scale_width = 2.5;
						}
					}
					break;
				default:
					break;
				}
				if( axis_n != 2 ){
					axis_scale_width = 1.0;
				}
			}
			else
			{
				if( axis_n != 2 ){
					axis_scale_width = 0.5;
				}else{
					axis_scale_width = 1.2;
				}
			}

			glBegin( GL_LINES );
				switch( axis_n ){
				case 0: // y axis
					{
						glVertex3f( n - axis_scale_width, nAxis, 0.0f );
						glVertex3f( n + axis_scale_width, nAxis, 0.0f );
					}
					break;
				case 1: // x axis
					{
						glVertex3f( nAxis, n - axis_scale_width, 0.0f );
						glVertex3f( nAxis, n + axis_scale_width, 0.0f );
					}
					break;
				case 2: // z axis
					{
						if( bScale_z ){
							glVertex3f( xs, ys, nAxis );
							glVertex3f( xs + axis_scale_width * d1, ys, nAxis );

							glVertex3f( xs, ys, nAxis );
							glVertex3f( xs, ys + axis_scale_width * d2, nAxis );
						}
					}
					break;
				default:
					break;
				}
				
			glEnd();
		}
	}	
}

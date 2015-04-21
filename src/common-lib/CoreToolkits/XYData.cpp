/**
 * @file XYData.cpp
 * @brief implements of XYData class
 *
 * @author S.Tanaka
 * @date 2006.08.30
 * @date 2011.03.02 <Del> M.Aihara
 *       delete static Timer
 * @date 2011.03.02 <Mod> M.Aihara
 *       modify m_cacheSize
 * @date 2011.03.02 <Mod> M.Aihara
 *       modify getCacheList() and cacheList.push_back()
 * @date 2011.03.02 <Add> M.Aihara
 *       add TBB mutex code
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "XYData.h"

#include "SearchTool.h"
#include "FileAccessor.h"
#include "Buffer.h"
#include "MsppManager.h"

#include <math.h>
#include <boost/bind.hpp>

// @date 2011.03.02 <Add> M.Aihara ->
// add TBB mutex code
#ifdef  ENABLE_MASSPP_PARALLEL_TBB_MODE
#include "tbb/spin_mutex.h"
using namespace tbb;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


namespace
{
	typedef spin_mutex XYDATA_ParallelMutexType;
	XYDATA_ParallelMutexType  XYDATA_ParallelMutex_01;
	XYDATA_ParallelMutexType  XYDATA_ParallelMutex_02;
}
#endif // ENABLE_MASSPP_PARALLEL_TBB_MODE
// <Add> M.Aihara <-

using namespace kome::core;

/// cache size
const unsigned int XYData::m_cacheSize = 10000;

/// version
unsigned long XYData::m_currentVersion = 0;

// constructor
XYData::XYData() {
	m_updated = false;
	m_released = true;

	m_minX = 0.0;
	m_maxX = 0.0;
	m_minY = 0.0;
	m_maxY = 0.0;
	m_version = 0;
}

// destructor
XYData::~XYData() {
	// manager
	MsppManager& msppMgr = MsppManager::getInstance();

	// delete data file
//#pragma omp critical ( deleteXYDataFromCache )
	{
// @date 2011.03.02 <Add> M.Aihara ->
// add TBB mutex code
#ifdef  ENABLE_MASSPP_PARALLEL_TBB_MODE
		XYDATA_ParallelMutexType::scoped_lock lock( XYDATA_ParallelMutex_01 );
#endif //  ENABLE_MASSPP_PARALLEL_TBB_MODE
// <Add> M.Aihara <-

		// delete object from cache list
		std::list< XYData* >& cacheList = getCacheList();
		std::list< XYData* >::iterator it = cacheList.begin();
		while( it != cacheList.end() ) {
			if( *it == this ) {
				cacheList.erase( it );
				it = cacheList.end();
			}
			else {
				it++;
			}
		}

		// delete data
		if( !m_tmpDataFile.empty() ) {
			// get path
			std::string filePath = getpath( msppMgr.getTmpDir(), m_tmpDataFile.c_str() );

			removefile( filePath.c_str() );
		}
	}
}

// get minimum x
double XYData::getMinX() {
	updateRange();
	return m_minX;
}

// get maximum x
double XYData::getMaxX() {
	updateRange();
	return m_maxX;
}

// get minimum y
double XYData::getMinY() {
	updateRange();
	return m_minY;
}

// get maximum y
double XYData::getMaxY() {
	updateRange();
	return m_maxY;
}

// clear points
void XYData::clearPoints() {
	// recover
	recoverData();

	// clear points
	onClearPoints();

	// flag
	m_updated = true;
}

// add point
void XYData::addPoint( const double x, const double y ) {
	// recover data
	recoverData();

	// add point
	onAddPoint( x, y );

	// flag
	m_updated = true;
}

// insert point
void XYData::insertPoint( const unsigned int index, const double x, const double y ) {
	// recover data
	recoverData();

	// insert point
	onInsertPoint( std::min( index, getLength() ), x, y );

	// flag
	m_updated = true;
}

// update point
void XYData::updatePoint( const unsigned int index, const double x, const double y ) {
	// recover data
	recoverData();

	// check the index
	if( index >= getLength() ) {
		return;
	}

	// delete
	deletePoint( index );

	// search
	int idx = searchIndex( x );
	if( idx < 0 ) {
		idx = - idx - 1;
	}

	// insert
	onInsertPoint( idx, x, y );

	// flag
	m_updated = true;
}

// delete point
void XYData::deletePoint( const unsigned int index ) {
	// recover data
	recoverData();

	// check the parameter
	if( index >= getLength() ) {
		return;
	}

	// on delete
	onDeletePoint( index );

	// flag
	m_updated = true;
}

// get the number of points
unsigned int XYData::getLength() {
	// recover data
	recoverData();

	return onGetLength();
}

// get x coordinate
double XYData::getX( const unsigned int index ) {
	// recover data
	recoverData();

	if( index >= getLength() ) {
		std::string msg = FMT(
			"The point index(%d) is larger than max index(%d).",
			index,
			getLength() - 1
		);

		LOG_WARN( msg );
		throw msg;
	}

	return onGetX( index );
}

// get y coordinate
double XYData::getY( const unsigned int index ) {
	// recover data
	recoverData();

	if( index >= getLength() ) {
		std::string msg = FMT(
			"The point index(%d) is larger than max index(%d).",
			index,
			getLength() - 1
		);

		LOG_WARN( msg );
		throw msg;
	}

	return onGetY( index );
}

// reserve
void XYData::reserve( const unsigned int num ) {
	// recover data
	recoverData();

	onReserve( num );
}

// get version
unsigned long XYData::getVersion() {
	updateRange();
	return m_version;
}

// search index
int XYData::searchIndex( const double x ) {
	// recover data
	recoverData();

	// comparator
	class SearchHelper {
	public:
		// compare to search
		static int compare( double x0, double x1 ) {
			if( x0 < x1 ) {
				return -1;
			}
			if( x0 > x1 ) {
				return 1;
			}
			return 0;
		}

		// get x
		static double getX( int index, XYData* xyData ) {
			return xyData->onGetX( index );
		}
	};

	// search
	int idx = -1;
	idx = SearchTool::binarySearch< double, double >(
		x,
		boost::bind( SearchHelper::getX, _1, this ),
		SearchHelper::compare,
		getLength()
	);

	return idx;
}

// get nearest index
int XYData::getNearestIndex( const double x ) {
	// length
	int len = (int)getLength();
	if( len == 0 ) {
		return -1;
	}

	// search
	int idx = searchIndex( x );

	if( idx >= 0 ) {
		return idx;
	}

	int idx0 = - idx - 2;
	int idx1 = - idx - 1;

	if( idx0 < 0 ) {
		return idx1;
	}
	if( idx1 >= len ) {
		return idx0;
	}

	// distance
	double d0 = fabs( getX( idx0 ) - x );
	double d1 = fabs( getX( idx1 ) - x );

	return ( d1 < d0 ? idx1 : idx0 );
}

// executes filter
void XYData::filter( const double absY, const double relY ) {
	// recover data
	recoverData();

	// length
	int len = (int)getLength();
	if( len <= 0 ) {
		return;
	}

	// threshold  
	double minY = std::max( fabs( m_minY ), fabs( m_maxY ) ) * relY / 100.0;
	minY = std::max( absY, minY );

	// filter
	for( int i = len - 1; i >= 0; i-- ) {
		double y = getY( i );
		if( y < minY ) {
			onDeletePoint( i );
		}
	}

	// flag
	m_updated = true;
}

// get points
void XYData::getPoints( std::vector< Point< double > >& points, const bool ySort, const bool desc ) {
	// recover data
	recoverData();

	// definition
	typedef Point< double > pt;

	// initialize
	points.clear();

	// set data
	unsigned int len = getLength();
	if( len == 0 ) {
		return;
	}
	points.resize( len );

	for( unsigned int i = 0; i < len; i++ ) {
		points[ i ].px = onGetX( i );
		points[ i ].py = onGetY( i );
	}
 
	// sort
	std::sort(
		points.begin(),
		points.end(),
		ySort ? ( desc ? pt::greaterY : pt::lessY ) : ( desc ? pt::greaterX : pt::lessX  )
	);
}

// import data
bool XYData::importData( boost::function< int ( void*, int ) > readFun ) {
	// clear
	onClearPoints();

	// import
	bool ret = onLoadData( readFun );
	
	return ret;
}

// export data
bool XYData::exportData( boost::function< int ( void*, int ) > writeFun ) {
	// update range
	updateRange();

	// export
	bool ret = onSaveData( writeFun );
	return ret;
}

// get the total time of loading temporary file
// @date 2011.03.02 <Del> M.Aihara ->
// double XYData::getInputTotalTime() {
//	return m_inputTimer.getTotalTime();
// }
// <Del> M.Aihara

// get the total time of saving temporary file
// @date 2011.03.02 <Del> M.Aihara ->
// double XYData::getOutputTotalTime() {
//	return m_outputTimer.getTotalTime();
// }
// <Del> M.Aihara <-

// udpate range
void XYData::updateRange() {
	// version
	if( m_updated || m_version == 0 ) {
		m_currentVersion = m_currentVersion + 1;
		m_version = m_currentVersion;
	}

	// check the flag
	if( !m_updated ) {
		return;
	}

	// initialize
	m_minX = 0.0;
	m_maxX = 0.0;
	m_minY = 0.0;
	m_maxY = 0.0;

	// set min/max values
	unsigned int len = getLength();
	for( unsigned int i = 0; i < len; i++ ) {
		const double x = getX( i );
		const double y = getY( i );

		if( i == 0 || x < m_minX ) {
			m_minX = x;
		}
		if( i == 0 || x > m_maxX ) {
			m_maxX = x;
		}
		if( i == 0 || y < m_minY ) {
			m_minY = y;
		}
		if( i == 0 || y > m_maxY ) {
			m_maxY = y;
		}
	}

	// set the flag
	m_updated = false;
}

// on load data
bool XYData::onLoadData( boost::function< int ( void*, int ) > readFun ) {
	// length
	unsigned long len = 0;
	readFun( &len, sizeof( len ) );

	// read data
	if( len > 0 ) {
		double* arr = new double[ len * 2 ];
		readFun( arr, sizeof( double ) * len * 2 );

		onReserve( len );
		for( unsigned int i = 0; i < len; i++ ) {
			onAddPoint( arr[ i * 2 ], arr[ i * 2 + 1 ] );
		}

		delete[] arr;
	}

	return true;
}

// on save data
bool XYData::onSaveData( boost::function< int ( void*, int ) > writeFun ) {
	// length
	unsigned long len = onGetLength();
	writeFun( &len, sizeof( len ) );

	// write data
	if( len > 0 ) {
		double* arr = new double[ len * 2 ];
		for( unsigned int i = 0; i < len; i++ ) {
			arr[ i * 2 ] = onGetX( i );
			arr[ i * 2 + 1 ] = onGetY( i );
		}

		writeFun( arr, sizeof( double ) * len * 2 );

		delete[] arr;
	}

	return true;
}

// release data
void XYData::releaseData() {
	// check the flag
	if( m_released ) {
		return;
	}

	// manager
	MsppManager& msppMgr = MsppManager::getInstance();

	// cache list
	getCacheList();

	// save data
	// @date 2011.03.02 <Del> M.Aihara
	// m_outputTimer.start();

	// create data
	Buffer buffer;
	exportData( boost::bind( &DataAccessor::write, &buffer, _1, _2 ) );

	static int tmpDirCnt = 0;

	if( m_tmpDataFile.empty() ) {	// create tmp file
		// directory
		std::string dir = getpath( "xy_data", FMT( "%02x", tmpDirCnt ).c_str() );
		tmpDirCnt = ( tmpDirCnt + 1 ) % 0x100;

		// file name
		std::string fileName = msppMgr.getTmpFileName(
			FMT( "xy_data%04d", ( 0xffff & (long long)this ) ).c_str(),
			".xyd",
			dir.c_str()
		);
		m_tmpDataFile = getpath( dir.c_str(), fileName.c_str() );
	}

	// write
	std::string path = getpath( msppMgr.getTmpDir(), m_tmpDataFile.c_str() );
	FILE* fp = fileopen( path.c_str(), "wb" );

	FileAccessor acc( fp );
	exportData( boost::bind( &DataAccessor::write, &acc, _1, _2 ) );

	fflush( fp );
	fclose( fp );

	// @date 2011.03.02 <Del> M.Aihara
	// m_outputTimer.stop();

	// flag
	m_released = true;

	// clear points
	onClearPoints();
}


// recover data
void XYData::recoverData() {
	// cache list
	// @date 2011.03.02 <Mod> M.Aihara ->
	// modify getCacheList() and cacheList.push_back()
	// std::list< XYData* >& cacheList = getCacheList();
	// <Mod> M.Aihara <-

	// check the flag
	if( !m_released ) {
		return;
	} 

	// flag
	m_released = false;

// @date 2011.03.02 <Add> M.Aihara ->
// add TBB mutex code
#ifdef  ENABLE_MASSPP_PARALLEL_TBB_MODE
	{
	XYDATA_ParallelMutexType::scoped_lock lock( XYDATA_ParallelMutex_01 );
#endif //  ENABLE_MASSPP_PARALLEL_TBB_MODE
// <Add> M.Aihara <-

	// @date 2011.03.02 <Mod> M.Aihara ->
	// modify getCacheList() and cacheList.push_back()
	// cache list
	std::list< XYData* >& cacheList = getCacheList();
	// <Mod> M.Aihara <-

	// release other data
//#pragma omp critical ( deleteXYDataFromCache )
	{
		while( (int)cacheList.size() > (int)m_cacheSize - 1 ) {
			cacheList.front()->releaseData();
			cacheList.pop_front();
		}
	}

	// @date 2011.03.02 <Mod> M.Aihara ->
	// modify getCacheList() and cacheList.push_back()
	cacheList.push_back( this );
	// <Mod> M.Aihara <-

// @date 2011.03.02 <Add> M.Aihara ->
// add TBB mutex code
#ifdef  ENABLE_MASSPP_PARALLEL_TBB_MODE
	}
#endif //  ENABLE_MASSPP_PARALLEL_TBB_MODE
// <Add> M.Aihara <-

	// recover
	// @date 2011.03.02 <Del> M.Aihara
	// m_inputTimer.start();

	if( !m_tmpDataFile.empty() ) {

// @date 2011.03.02 <Add> M.Aihara ->
// add TBB mutex code
#ifdef  ENABLE_MASSPP_PARALLEL_TBB_MODE
		XYDATA_ParallelMutexType::scoped_lock lock( XYDATA_ParallelMutex_02 );
#endif //  ENABLE_MASSPP_PARALLEL_TBB_MODE
// <Add> M.Aihara

		// data file path
		MsppManager& msppMgr = MsppManager::getInstance();
		std::string path = getpath( msppMgr.getTmpDir(), m_tmpDataFile.c_str() );

		// file open
		FILE* fp = fileopen( path.c_str(), "rb" );

		// import
		FileAccessor acc( fp );
		importData( boost::bind( &DataAccessor::read, &acc, _1, _2 ) );

		fclose( fp );
	}

	// @date 2011.03.02 <Mod> M.Aihara ->
	// modify getCacheList() and cacheList.push_back()
// #pragma omp critical ( deleteXYDataFromCache )
//	{
//		cacheList.push_back( this );
//	}
	// <Mod> M.Aihara <-

	// @date 2011.03.02 <Del> M.Aihara
	// m_inputTimer.stop();

}

// get cache list
std::list< XYData* >& XYData::getCacheList() {
	// create object
	static std::list< XYData* > cl;

	return cl;
}

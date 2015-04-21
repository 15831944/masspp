/**
 * @file Peaks.cpp
 * @brief implements of Peaks class
 *
 * @author S.Tanaka
 * @date 2006.10.11
 * @date 2011.03.03 <Mod> M.Aihara
 *       soft cache DataPoints
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "Peaks.h"

#include "PeakElement.h"


using namespace kome::objects;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



#define MAX_PEAK_NUM    10000

// constructor
Peaks::Peaks() {
	m_precursor = -1.0;
// @date 2011.03.03 <Mod> M.Aihara ->
// soft cache DataPoints
#ifdef DATAPOINTS_SOFT_CACHE_TBB
    m_dps = NULL;
#endif // DATAPOINTS_SOFT_CACHE_TBB
// <Mod> M.Aihara <-
}

// copy constructor
Peaks::Peaks( const Peaks& peaks ) : kome::core::XYData( peaks ) {
	// initialize
	m_title = peaks.m_title;
	m_precursor = peaks.m_precursor;
	for( unsigned int i = 0; i < peaks.m_charges.size(); i++ ) {
		m_charges.push_back( peaks.m_charges[ i ] );
	}

// @date 2011.03.03 <Mod> M.Aihara ->
// soft cache DataPoints
#ifdef DATAPOINTS_SOFT_CACHE_TBB
    if(peaks.m_dps){
        m_dps = new kome::core::DataPoints( kome::core::DataPoints::FLOAT );
        *m_dps = *(peaks.m_dps);
    }else{
        m_dps = NULL;
    }
#endif // DATAPOINTS_SOFT_CACHE_TBB
// <Mod> M.Aihara <-

	m_title = peaks.m_title;
	m_charges = peaks.m_charges;
	m_precursor = peaks.m_precursor;
	m_dataFile = peaks.m_dataFile;

	// copy peaks
	if( peaks.m_peaks.size() == 0 ) {
		return;
	}

	m_peaks.reserve( peaks.m_peaks.size() );
	for( unsigned int i = 0; i < peaks.m_peaks.size(); i++ ) {
		// source peak
		PeakElement* src = peaks.m_peaks[ i ];

		// copy peak
		PeakElement* peak = new PeakElement( this );
		m_peaks.push_back( peak );

		peak->setX( src->getX() );
		peak->setY( src->getY() );
		peak->setLeft( src->getLeftX(), src->getLeftY() );
		peak->setRight( src->getRightX(), src->getRightY() );
		peak->setApex( src->getApexX(), src->getApexY() );
	}
}

// destructor
Peaks::~Peaks() {
	// manager
	kome::core::MsppManager& msppMgr = kome::core::MsppManager::getInstance();

// @date 2011.03.03 <Mod> M.Aihara ->
// soft cache DataPoints
#ifdef DATAPOINTS_SOFT_CACHE_TBB
    if(m_dps) delete m_dps;
#endif // DATAPOINTS_SOFT_CACHE_TBB
// <Mod> M.Aihara <-

	// clear points
	onClearPoints();

	// delete profile data
	if( !m_dataFile.empty() ) {
		std::string oldDataPath = getpath( msppMgr.getTmpDir(), m_dataFile.c_str() );

		if( fileexists( oldDataPath.c_str() ) ) {
			removefile( oldDataPath.c_str() );
		}
	}
}

// substitution operator
Peaks& Peaks::operator=( const Peaks& other ) {
	// initialize
	m_title = other.m_title;
	m_precursor = other.m_precursor;
	for( unsigned int i = 0; i < other.m_charges.size(); i++ ) {
		m_charges.push_back( other.m_charges[ i ] );
	}

	// clear points
	clearPoints();
	// copy peaks
	if( other.m_peaks.size() == 0 ) {
		return *this;
	}

	// add peaks
	reserve( other.m_peaks.size() );
	for( unsigned int i = 0; i < other.m_peaks.size(); i++ ) {
		// source peak
		PeakElement* src = other.m_peaks[ i ];

		// copy peak
		PeakElement* peak = createPeak( src->getX(), src->getY() );

		peak->setLeft( src->getLeftX(), src->getLeftY() );
		peak->setRight( src->getRightX(), src->getRightY() );
		peak->setApex( src->getApexX(), src->getApexY() );
		peak->setArea( src->getArea() );
	}

	return *this;
}

// clear charge
void Peaks::clearCharge() {
	m_charges.clear();
}

// add charge
void Peaks::addCharge( const short charge ) {
	m_charges.push_back( charge );
}

// get number of charges
unsigned int Peaks::getNumberOfCharges() {
	return m_charges.size();
}

// get charge
short Peaks::getCharge( const unsigned int index ) {
	// check index
	if( index >= m_charges.size() ) {
		LOG_WARN(
			FMT(
				"Charge index must be less than the number of charges. [%d/%d] ",
				index,
				m_charges.size()
			)
		);
		return 0;
	}

	return m_charges[ index ];
}

// set precursor
void Peaks::setPrecursor( const double precursor ) {
	m_precursor = precursor;
}

// get precursor
double Peaks::getPrecursor() {
	return m_precursor;
}

// @date 2011.03.03 <Mod> M.Aihara ->
// soft cache DataPoints
#ifdef DATAPOINTS_SOFT_CACHE_TBB
void Peaks::calcArea() {

	// get data points
	if( m_dps == NULL ) {
		LOG_WARN( FMT( "Failed to calculate area. Data points object is not set up." ) );
		return;
	}

	if( m_dps->getLength() < 2 ) {
		return;
	}

	// peaks
	for( unsigned int i = 0; i < m_peaks.size(); i++ ) {
		PeakElement* peak = m_peaks[ i ];

		// area
		if( !peak->hasArea() ) {
			// area
			double area = peak->calcArea( *m_dps );
			peak->setArea( area );
		}

		peak->searchApex( *m_dps );
	}
}

#else // DATAPOINTS_SOFT_CACHE_TBB

// calculate area
void Peaks::calcArea() {
	// manager
	kome::core::MsppManager& msppMgr = kome::core::MsppManager::getInstance();

	// get data points
	if( m_dataFile.empty() ) {
		LOG_WARN( FMT( "Failed to calculate area. Data points object is not set up." ) );
		return;
	}

	// tmp file
	std::string tmpPath = getpath( msppMgr.getTmpDir(), m_dataFile.c_str() );
	FILE* fp = fileopen( tmpPath.c_str(), "rb" );
	if( fp == NULL ) {
		return;
	}

	// import
	kome::core::DataPoints dps( kome::core::DataPoints::FLOAT );
	kome::core::FileAccessor acc( fp );
	dps.importData( boost::bind( &kome::core::FileAccessor::read, &acc, _1, _2 ) );
	fclose( fp );
	if( dps.getLength() < 2 ) {
		return;
	}

	// peaks
	for( unsigned int i = 0; i < m_peaks.size(); i++ ) {
		PeakElement* peak = m_peaks[ i ];

		// area
		if( !peak->hasArea() ) {
			// area
			double area = peak->calcArea( dps );
			peak->setArea( area );
		}

		// apex
		peak->searchApex( dps );

		// fwhm
		double fwhm = peak->calcFwhm( dps );
		peak->setFwhm( fwhm );
	}
}
#endif // DATAPOINTS_SOFT_CACHE_TBB
// <Mod> M.Aihara <-

// @date 2011.03.03 <Mod> M.Aihara ->
// soft cache DataPoints
#ifdef DATAPOINTS_SOFT_CACHE_TBB
void Peaks::setDataPoints( kome::core::XYData& xyData ) {

	// init area
	for( unsigned int i = 0; i < m_peaks.size(); i++ ) {
		m_peaks[ i ]->setArea( -1.0 );
	}

	// delete previous data
    if(m_dps){
        delete m_dps;
        m_dps = NULL;
    }

    m_dps = new kome::core::DataPoints( kome::core::DataPoints::FLOAT );

	// copy data
	for( unsigned int i = 0; i < xyData.getLength(); i++ ) {
		m_dps->addPoint( xyData.getX( i ), xyData.getY( i ) );
	}
}

#else // DATAPOINTS_SOFT_CACHE_TBB

// set data points
void Peaks::setDataPoints( kome::core::XYData& xyData ) {

	// manager
	kome::core::MsppManager& msppMgr = kome::core::MsppManager::getInstance();

	// init area
	for( unsigned int i = 0; i < m_peaks.size(); i++ ) {
		m_peaks[ i ]->setArea( -1.0 );
	}

	// delete previous data
	if( !m_dataFile.empty() ) {
		std::string oldDataPath = getpath( msppMgr.getTmpDir(), m_dataFile.c_str() );

		if( fileexists( oldDataPath.c_str() ) ) {
			removefile( oldDataPath.c_str() );
		}
	}

	// data file path
	static int tmpDirCnt = 0;

	std::string dir = getpath( "peaks_data", FMT( "%02x", tmpDirCnt ).c_str() );
	tmpDirCnt = ( tmpDirCnt + 1 ) % 0x100;

	std::string tmpFileName = msppMgr.getTmpFileName( "profile_data", ".xyd", dir.c_str() );
	m_dataFile = getpath( dir.c_str(), tmpFileName.c_str() );
	std::string tmpFilePath = getpath( msppMgr.getTmpDir(), m_dataFile.c_str() );

	// open file
	FILE* fp = fileopen( tmpFilePath.c_str(), "wb" );
	if( fp == NULL ) {
		m_dataFile.clear();
		return;
	}

	// copy data
	kome::core::DataPoints dps( kome::core::DataPoints::FLOAT );
	for( unsigned int i = 0; i < xyData.getLength(); i++ ) {
		dps.addPoint( xyData.getX( i ), xyData.getY( i ) );
	}

	// save data
	kome::core::FileAccessor acc( fp );
	dps.exportData( boost::bind( &kome::core::FileAccessor::write, &acc, _1, _2 ) );

	fclose( fp );
}
#endif // DATAPOINTS_SOFT_CACHE_TBB
// <Mod> M.Aihara <-

// create peak
PeakElement* Peaks::createPeak( const double x, const double y ) {
	// add peak
	PeakElement* peak = new PeakElement( this, x, y );
	m_peaks.push_back( peak );

	// length
	unsigned int len = m_peaks.size();

	// set range
	if( x < m_minX || len == 0 ) {
		m_minX = x;
	}
	if( x > m_maxX || len == 0 ) {
		m_maxX = x;
	}
	if( y < m_minY || len == 0 ) {
		m_minY = y;
	}
	if( y > m_maxY || len == 0 ) {
		m_maxY = y;
	}

	m_updated = true;

	return peak;
}

// get peak
PeakElement* Peaks::getPeak( const unsigned int index ) {
	if( index >= m_peaks.size() ) {
		return NULL;
	}
	double x = getX( index );
	return m_peaks[ index ];
}

// on clear points
void Peaks::onClearPoints() {
	for( unsigned int i = 0; i < m_peaks.size(); i++ ) {
		delete m_peaks[ i ];
	}
	m_peaks.clear();
}

// sort peak
void Peaks::sort() {
	std::sort( m_peaks.begin(), m_peaks.end(), lessPeakX );
}

// arrange peaks
void Peaks::arrangePeaks() {
	// check the array size
	if( m_peaks.size() <= MAX_PEAK_NUM ) {
		return;
	}

	// remove small peaks
	std::vector< PeakElement* > peaks;
	for( unsigned int i = 0; i < m_peaks.size(); i++ ) {
		peaks.push_back( m_peaks[ i ] );
	}

	std::sort( peaks.begin(), peaks.end(), lessPeakY );

	m_peaks.clear();
	for( unsigned int i = 0; i < MAX_PEAK_NUM; i++ ) {
		m_peaks.push_back( peaks[ i ] );
	}
	sort();

	for( unsigned int i = MAX_PEAK_NUM; i < peaks.size(); i++ ) {
		delete peaks[ i ];
	}
}

// on load file
bool Peaks::onLoadData( boost::function< int ( void*, int ) > readFun ) {
	// length
	unsigned long len = 0;
	readFun( &len, sizeof( len ) );

	// read data
	if( len > 0 ) {
		double* arr = new double[ len * 10 ];
		readFun( arr, sizeof( double ) * len * 10 );

		int* charges = new int[ len ];
		readFun( charges, sizeof( int ) * len );

		m_peaks.reserve( len );
		for( unsigned int i = 0; i < len; i++ ) {
			int idx = i * 10;
			double x  = arr[ idx + 0 ];
			double y  = arr[ idx + 1 ];
			double lx = arr[ idx + 2 ];
			double ly = arr[ idx + 3 ];
			double rx = arr[ idx + 4 ];
			double ry = arr[ idx + 5 ];
			double ax = arr[ idx + 6 ];
			double ay = arr[ idx + 7 ];
			double ar = arr[ idx + 8 ];
			double wi = arr[ idx + 9 ];
			int c = charges[ i ];

			PeakElement* peak = new PeakElement( this, x, y );
			peak->setLeft( lx, ly );
			peak->setRight( rx, ry );
			peak->setApex( ax, ay );
			peak->setArea( ar );
			peak->setFwhm( wi );
			peak->setCharge( c );

			m_peaks.push_back( peak );
		}

		delete[] arr;
		delete[] charges;
	}

	return true;
}

// on write file
bool Peaks::onSaveData( boost::function< int ( void*, int ) > writeFun ) {
	// length
	unsigned long len = m_peaks.size();
	writeFun( &len, sizeof( len ) );

	// write data
	if( len > 0 ) {
		double* arr = new double[ len * 10 ];

		for( unsigned int i = 0; i < len; i++ ) {
			// peak
			PeakElement* peak = m_peaks[ i ];

			// array
			int idx = i * 10;
			arr[ idx + 0 ] = peak->getX();
			arr[ idx + 1 ] = peak->getY();
			arr[ idx + 2 ] = peak->getLeftX();
			arr[ idx + 3 ] = peak->getLeftY();
			arr[ idx + 4 ] = peak->getRightX();
			arr[ idx + 5 ] = peak->getRightY();
			arr[ idx + 6 ] = ( peak->hasApex() ? peak->getApexX() : -1.0 );
			arr[ idx + 7 ] = ( peak->hasApex() ? peak->getApexY() : -1.0 );
			arr[ idx + 8 ] = ( peak->hasArea() ? peak->getArea() : -1.0 );
			arr[ idx + 9 ] = ( peak->hasFwhm() ? peak->getFwhm() : -1.0 );
		}

		writeFun( arr, sizeof( double ) * len * 10 );

		delete[] arr;

		// charges
		int* charges = new int[ len ];
		for( unsigned int i = 0; i < len; i++ ) {
			// peak
			PeakElement* peak = m_peaks[ i ];

			charges[ i ] = peak->getCharge();
		}

		writeFun( charges, sizeof( int ) * len );

		delete[] charges;
	}

	return true;
}

// on add point
void Peaks::onAddPoint( const double x, const double y ) {
	createPeak( x, y );
}

// on insert point
void Peaks::onInsertPoint( const unsigned int index, const double x, const double y ) {
	// index
	int idx = std::min( (int)index, (int)m_peaks.size() );

	// insert
	PeakElement* peak = new PeakElement( this, x, y );
	m_peaks.insert( m_peaks.begin() + idx, peak );
}

// on delete point
void Peaks::onDeletePoint( const unsigned int index ) {
	delete m_peaks[ index ];
	m_peaks.erase( m_peaks.begin() + index );
}

// on get length
unsigned int Peaks::onGetLength() {
	arrangePeaks();
	return m_peaks.size();
}

// on get x
double Peaks::onGetX( const unsigned int index ) {
	arrangePeaks();
	return m_peaks[ index ]->getX();
}

// on get y
double Peaks::onGetY( const unsigned int index ) {
	arrangePeaks();
	return m_peaks[ index ]->getY();
}

// on reserve
void Peaks::onReserve( const unsigned int num ) {
	m_peaks.reserve( num );
}

// sort by x
bool Peaks::lessPeakX( PeakElement* p0, PeakElement* p1 ) {
	return ( p0->getX() < p1->getX() );
}

// sort by y
bool Peaks::lessPeakY( PeakElement* p0, PeakElement* p1 ) {
	return ( p0->getY() > p1->getY() );
}

// issue peak id
int Peaks::issueId( PeakElement* peakElement ){
	if( peakElement != NULL && peakElement->getId() > 0 ){
		return peakElement->getId()+1;
	}
	return 0;
}

// get peak by id
PeakElement* Peaks::getPeakById( int id ){
	arrangePeaks();

	for( unsigned int i=0; i < m_peaks.size(); i++ ){
		if( m_peaks[i]->getId() == id ){
			return m_peaks[i];
		}
	}
	return NULL;
}

/**
 * @file DataMapInfo.cpp
 * @brief implements of DataMapInfo class
 *
 * @author S.Tanaka
 * @date 2007.09.22
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "DataMapInfo.h"

#include "DataGroupNode.h"
#include "Spectrum.h"
#include "DataSet.h"
#include "DataManager.h"

#include <math.h>


using namespace kome::objects;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



#define DEFAULT_MZ_ZERO_RANGE		0.2
#define DEFAULT_RT_ZERO_RANGE		1.0


// constructor
DataMapInfo::DataMapInfo( DataGroupNode& group, const unsigned int row, const unsigned int col ) {
    // initialize
	m_row = row;
	m_group = &group;
	m_col = col;

	// create map
	unsigned int size = row * col;
	if( size == 0 ) {
		m_map = NULL;
	}
	else {
		m_map = new double[ size ];
	}

	// initialize
	m_minMz = double();
	m_maxMz = double();
	m_minRt = double();
	m_maxRt = double();
	m_maxIntensity = double();
	m_minIntensity = double();
	m_autoCommit = true;
	m_tmp = NULL;
	m_completed = false;
}

// destructor
DataMapInfo::~DataMapInfo() {
	// delete map
	if( m_map != NULL ) {
		delete[] m_map;
	}

	// delete data map
	deleteTmpData();

	// close
	DataManager::closeDataMap( this, true );
}

// get row size
unsigned int DataMapInfo::getRowSize() {
	return m_row;
}

// get column size
unsigned int DataMapInfo::getColSize() {
	return m_col;
}

// get spectrum group
DataGroupNode& DataMapInfo::getGroup() {
	return *m_group;
}

// get sample
Sample* DataMapInfo::getSample() {
	return m_group->getSample();
}

// get min m/z
double DataMapInfo::getMinMz() {
	return m_minMz;
}

// get max m/z
double DataMapInfo::getMaxMz() {
	return m_maxMz;
}

// get min RT
double DataMapInfo::getMinRt() {
	return m_minRt;
}

// get max RT
double DataMapInfo::getMaxRt() {
	return m_maxRt;
}

// get min intenstiy
double DataMapInfo::getMinIntensity() {
	return m_minIntensity;
}

// get max intensity
double DataMapInfo::getMaxIntensity() {
	return m_maxIntensity;
}

// get the intensity of the image map
double DataMapInfo::getIntensity( const unsigned int row, const unsigned int col ) {
	int idx = row * m_col + col;

	return m_map[ idx ];
}

// set completed flag
void DataMapInfo::setCompleted( const bool completed ) {
	m_completed = completed;
}

// get completed flag
bool DataMapInfo::isCompleted() {
	return m_completed;
}

// set intensity range
void DataMapInfo::setIntensityRange( const double minInt, const double maxInt ) {
	m_minIntensity = minInt;
	m_maxIntensity = maxInt;
}

// get spectrum array
void DataMapInfo::getSpecArray( const char* filterName, std::vector< kome::objects::Spectrum* >& tmpSpecArray ){
	// create temporary data
	createTmpData();

	// data set
	kome::objects::DataSet spectra;
	m_group->getDataSet( &spectra );
	spectra.sortSpectra();

	// create array
	int specNum = (int)spectra.getNumberOfSpectra();
	
	// MS
	if( strcmp( filterName, "ms" ) == 0 ){
		for( int i = 0; i < specNum; i++ ) {
			kome::objects::Spectrum* spec = spectra.getSpectrum( i );
			if( spec->getMsStage() == 1 && spec->hasChromatogram() ) {
				tmpSpecArray.push_back( spec );
			}
		}
	// MS/MS
	}else if( strcmp( filterName, "ms2" ) == 0 ){
		for( int i = 0; i < specNum; i++ ) {
			kome::objects::Spectrum* spec = spectra.getSpectrum( i );
			if( spec->getMsStage() >= 2 && spec->hasChromatogram() ) {
				tmpSpecArray.push_back( spec );
			}
		}
	// All
	}else if( strcmp( filterName, "all" ) == 0 ){
		for( int i=0; i< specNum; i++ ){
			kome::objects::Spectrum* spec = spectra.getSpectrum( i );
			if( spec->hasChromatogram() ){
				tmpSpecArray.push_back( spec );
			}
		}
	}
}

// set spectra
bool DataMapInfo::setRange(
		double minMz,
		double maxMz,
		double minRt,
		double maxRt,
		const char* filterName,
		kome::core::Progress& progress
) {
	// @date 2012.09.07 <Mod> M.Izumi ->
	std::vector< kome::objects::Spectrum* > tmpSpecArray;
	getSpecArray( filterName, tmpSpecArray );
	// @date 2012.09.07 <Mod> M.Izumi <-

	int specNum = (int)tmpSpecArray.size();

	// start index
	int startIdx = 0;
	if( minRt >= 0.0 ) {
		for( int i = 0; i < specNum; i++ ) {
			if( tmpSpecArray[ i ]->getRt() <= minRt ) {
				startIdx = i;
			}
		}
	}

	// end index
	int endIdx = specNum - 1;
	if( maxRt >= 0.0 ) {
		for( int i = specNum - 1; i >= 0; i-- ) {
			if( tmpSpecArray[ i ]->getRt() >= maxRt ) {
				endIdx = i;
			}
		}
	}

	int length = endIdx - startIdx + 1;

	// spectra
	std::vector< kome::objects::Spectrum* > specArray;
	for( int i = startIdx; i <= endIdx; i++ ) {
		specArray.push_back( tmpSpecArray[ i ] );
	}

	// check the data
	if( length <= 0 ) {		// There is no spectrum.
		// set value
		m_tmp->minMz = MAX( minMz, 0.0 );
		m_tmp->maxMz = MAX( maxMz, 0.1 );
		m_tmp->minRt = MAX( minRt, 0.0 );
		m_tmp->maxRt = MAX( maxRt, 0.1 );
		m_tmp->minIntensity = 0.0;
		m_tmp->maxIntensity = 0.0;

		// progress
		progress.setRange( 0, 1 );
		progress.setPosition( 1 );

		// commit
		if( m_autoCommit ) {
			commit();
		}

		// flag
		m_completed = true;

		return true;
	}

	// rt	
	m_tmp->minRt = minRt < 0.0 ? tmpSpecArray[ startIdx ]->getRt() : minRt;
	m_tmp->maxRt = maxRt < 0.0 ? tmpSpecArray[ endIdx ]->getRt() : maxRt;

	// progress range
	progress.setRange( 0, 6 * length );
	progress.setPosition( 0 );

	// min m/z
	if( minMz < 0.0 ) {
		minMz = tmpSpecArray[ startIdx ]->getMinX();

		for( int i = startIdx + 1; i <= endIdx && !progress.isStopped(); i++ ) {
			progress.setPosition( i - startIdx );
			minMz = MIN( minMz, tmpSpecArray[ i ]->getMinX() );
		}
	}
	progress.setPosition( length );

	// max m/z
	if( maxMz < 0.0 ) {
		maxMz = tmpSpecArray[ startIdx ]->getMaxX();

		for( int i = startIdx + 1; i < endIdx && !progress.isStopped(); i++ ) {
			progress.setPosition( length + i - startIdx );
			maxMz = MAX( maxMz, tmpSpecArray[ i ]->getMaxX() );
		}
	}

	if( !progress.isStopped() ) {
		progress.setPosition( 2 * length );

		m_tmp->minMz = MIN( maxMz, minMz );
		m_tmp->maxMz = MAX( maxMz, minMz );
		double midMz = ( m_tmp->minMz + m_tmp->maxMz ) / 2.0;
		m_tmp->minMz = std::min( midMz - 0.01, m_tmp->minMz );
		m_tmp->maxMz = std::max( midMz + 0.01, m_tmp->maxMz );
	}

	// create image map
	if( !progress.isStopped() ) {
		createImageMap( specArray, progress );
	}

	// restore
	if( progress.isStopped() ) {
		rollback();
	}

	// commit
	if( m_autoCommit ) {
		commit();
	}

	// flag
	m_completed = !progress.isStopped();

	return m_completed;
}

// set auto commit flag
void DataMapInfo::setAutoCommit( const bool autoCommit ) {
	m_autoCommit = autoCommit;
}

// get auto commit flag
bool DataMapInfo::isAutoCommit() {
	return m_autoCommit;
}

// commit
void DataMapInfo::commit() {
	// check the data
	if( m_tmp == NULL ) {
		return;
	}

	// set values
	m_minMz = m_tmp->minMz;
	m_maxMz = m_tmp->maxMz;
	m_minRt = m_tmp->minRt;
	m_maxRt = m_tmp->maxRt;
	m_minIntensity = m_tmp->minIntensity;
	m_maxIntensity = m_tmp->maxIntensity;

	// map
	int size = m_row * m_col;
	if( size > 0 ) {
		memcpy( m_map, m_tmp->intensities, sizeof( double ) * size );
	}

	// delete data
	deleteTmpData();
}

// rollback
void DataMapInfo::rollback() {
	deleteTmpData();
}

// save data map
bool DataMapInfo::saveDataMap( const char* path ) {
	// file open
	FILE* fp = fileopen( path, "wb" );
	if( fp == NULL ) {
		return false;
	}

	// array size
	const unsigned int size = m_row * m_col;

	// save
	fwrite( &m_row, sizeof( m_row ), 1, fp );
	fwrite( &m_col, sizeof( m_col ), 1, fp );
	fwrite( &m_minMz, sizeof( m_minMz ), 1, fp );
	fwrite( &m_maxMz, sizeof( m_maxMz ), 1, fp );
	fwrite( &m_minRt, sizeof( m_minRt ), 1, fp );
	fwrite( &m_maxRt, sizeof( m_maxRt ), 1, fp );
	fwrite( &m_minIntensity, sizeof( m_minIntensity ), 1, fp );
	fwrite( &m_maxIntensity, sizeof( m_maxIntensity ), 1, fp );

	if( size > 0 ) {
		fwrite( m_map, sizeof( double ), size, fp );
	}

	fflush( fp );
	fclose( fp );

	return true;
}

// load data
bool DataMapInfo::loadDataMap( const char* path ) {
	// file open
	FILE* fp = fileopen( path, "rb" );
	if( fp == NULL ) {
		return false;
	}

	// array size
	const unsigned int size = m_row * m_col;

	// load array size
	unsigned int row = 0;
	unsigned int col = 0;

	fread( &row, sizeof( row ), 1, fp );
	fread( &col, sizeof( col ), 1, fp );

	if( row != m_row || col != m_col ) {
		LOG_WARN( FMT( "Data Map size is different." ) );
		fclose( fp );
		return false;
	}

	fread( &m_minMz, sizeof( m_minMz ), 1, fp );
	fread( &m_maxMz, sizeof( m_maxMz ), 1, fp );
	fread( &m_minRt, sizeof( m_minRt ), 1, fp );
	fread( &m_maxRt, sizeof( m_maxRt ), 1, fp );
	fread( &m_minIntensity, sizeof( m_minIntensity ), 1, fp );
	fread( &m_maxIntensity, sizeof( m_maxIntensity ), 1, fp );

	if( size > 0 ) {
		fread( m_map, sizeof( double ), size, fp );
	}

	fclose( fp );

	return true;
}

// create image map
void DataMapInfo::createImageMap(
		std::vector< kome::objects::Spectrum* >& spectra,
		kome::core::Progress& progress
) {
	// intensities array
	double* prevInts = new double[ m_col ];
	double* nowInts = new double[ m_col ];

	// initialize
	m_tmp->minIntensity = 0.0;
	m_tmp->maxIntensity = 0.1;

	// unit range (RT)
	double unitRange = ( m_tmp->maxRt - m_tmp->minRt ) / (double)( m_row - 1 );
	double rtZeroRange = DEFAULT_RT_ZERO_RANGE;
	double prevRt = 0.0;
	if( m_group->getNumberOfSpectra() > 0 ) {
		prevRt = m_group->getSpectrum( 0 )->getRt() - rtZeroRange;
	}
	double prevScan = 0;
	for( unsigned int i = 0; i < m_group->getNumberOfSpectra(); i++ ) {
		kome::objects::Spectrum* spec = m_group->getSpectrum( i );
		rtZeroRange = std::max( rtZeroRange, fabs( spec->getRt() - prevRt ) );
		prevRt = spec->getRt();
		prevScan = spec->getScanNumber();
	}

	// length
	int length = (int)spectra.size();

	// create image map
	int prevIdx = -1;
	prevRt = -9999999.9;
	for( unsigned int i = 0; i < spectra.size() && !progress.isStopped() ; i++ ) {
		// spectrum 
		Spectrum* spec = spectra[ i ];

		progress.setStatus( FMT( "Getting spectrum data... [%s]", spec->getName() ).c_str() );
		progress.setPosition( 2 * length + 4 * i );

		// intensities
		getSpecIntensities( spec, nowInts, m_row );
		if( spectra.size() == 1 ) {
			prevRt = spec->getRt();
			memcpy( prevInts, nowInts, sizeof( double ) * m_col );
		}

		// intensity range
		for( unsigned int j = 0; j < m_col; j++ ) {
			if( nowInts[ j ] < m_tmp->minIntensity ) {
				m_tmp->minIntensity = nowInts[ j ];
			}
			if( nowInts[ j ] > m_tmp->maxIntensity ) {
				m_tmp->maxIntensity = nowInts[ j ];
			}
		}

		// get index
		int idx = (int)m_row - 1;
		if( spectra.size() > 1 ) {
			idx = roundnum( ( spec->getRt() - m_tmp->minRt ) / unitRange );
		}

		// set intensities
		if( idx >= 0 && idx < (int)m_row ) {
			if( idx == prevIdx ) {
				for( unsigned int j = 0; j < m_col; j++ ) {
					int ptIdx = idx * m_col + j;
					double prevInt = m_tmp->intensities[ ptIdx ];
					double nowInt = nowInts[ j ];

					if( fabs( nowInt ) > fabs( prevInt ) ) {
						m_tmp->intensities[ ptIdx ] = nowInt;
					}
				}
			}
			else {
				for( unsigned int j = 0; j < m_col; j++ ) {
					int ptIdx = idx * m_col + j;
					m_tmp->intensities[ ptIdx ] = nowInts[ j ];
				}
			}
		}

		// interpolation
		int sIdx = prevIdx + 1;
		sIdx = MAX( 0, sIdx );
		if( spec->getRt() <= prevRt + rtZeroRange ) { // linear interpolation
			for( int j = sIdx; j < idx && j < (int)m_row; j++ ) {
				double rt = m_tmp->minRt + (double)j * unitRange;

				double d1 = fabs( rt - prevRt );
				double d2 = fabs( spec->getRt() - rt );
				if( prevRt == spec->getRt() ) {
					d1 = 1.0;
					d2 = 0.0;
				}
				for( int k = 0; k < (int)m_col; k++ ) {
					int ptIdx = j * m_row + k;
					m_tmp->intensities[ ptIdx ] = ( d2 * prevInts[ k ] + d1 * nowInts[ k ] ) / ( d1 + d2 );
				}
			}
		}
		else {		 // insert zero
			double zeroRange = std::min( ( spec->getRt() - prevRt ) / 2.5, rtZeroRange );

			for( int j = sIdx; j < idx && j < (int)m_row; j++ ) {
				double rt = m_tmp->minRt + (double)j * unitRange;

				if( rt > spec->getRt() - zeroRange ) {
					double d = rt - ( spec->getRt() - zeroRange );

					for( int k = 0; k < (int)m_col; k++ ) {
						int ptIdx = j * m_col + k;
						m_tmp->intensities[ ptIdx ] = ( d * nowInts[ k ] ) / zeroRange;
					}
				}
				else if( rt < prevRt + zeroRange ) {
					double d = zeroRange - fabs( rt - prevRt );

					for( int k = 0; k < (int)m_col; k++ ) {
						int ptIdx = j * m_col + k;
						m_tmp->intensities[ ptIdx ] = ( d * prevInts[ k ] ) / zeroRange;
					}
				}
				else {
					for( int k = 0; k < (int)m_col; k++ ) {
						int ptIdx = j * m_col + k;
						m_tmp->intensities[ ptIdx ] = 0.0;
					}
				}
			}
		}

		// set previous
		prevIdx = idx;
		prevRt = spec->getRt();
		memcpy( prevInts, nowInts, sizeof( double ) * m_row );
	}

	// delete array
	delete[] prevInts;
	delete[] nowInts;

	// check progress
	if( progress.isStopped() ) {
		return;
	}

	// log
	LOG_DEBUG(
		FMT(
			"Data Map : mz=[%.2f, %.2f] RT=[%.2f, %.2f] Int=[%.2f, %.2f]",
			m_tmp->minMz,
			m_tmp->maxMz,
			m_tmp->minRt,
			m_tmp->maxRt,
			m_tmp->minIntensity,
			m_tmp->maxIntensity
		)
	);

	// progress
	progress.fill();
}

// get spectrum intensities
void DataMapInfo::getSpecIntensities(
		Spectrum* spec,
		double* intensities,
		int size
) {
	// variables
	kome::core::DataPoints pt;

	// initialize
	for( int i = 0; i < size; i++ ) {
		intensities[ i ] = 0.0;
	}

	// unit range (m/z)
	double unitRange = ( m_tmp->maxMz - m_tmp->minMz ) / (double)( size - 1 );
	double mzZeroRange = std::max( DEFAULT_MZ_ZERO_RANGE, spec->getResolution() );

	// get point index
	int mzStartIdx = 0;
	int mzEndIdx = 0;
	int length = 0;

	// get points
	pt.clearPoints();
	spec->getXYData( &pt, m_tmp->minMz - mzZeroRange, m_tmp->maxMz + mzZeroRange, false );

	length = (int)pt.getLength();
	mzStartIdx = pt.searchIndex( m_tmp->minMz );
	mzEndIdx = pt.searchIndex( m_tmp->maxMz );

	if( mzStartIdx < 0 ) {
		mzStartIdx = - mzStartIdx - 2;
		mzStartIdx = MAX( 0, mzStartIdx );
	}
	if( mzEndIdx < 0 ) {
		mzEndIdx = - mzEndIdx - 1;
		mzEndIdx = MIN( length - 1, mzEndIdx );
	}

	// get data points
	int prevIdx = -1;
	double prevMz = -9999999.9;
	double prevInt = 0.0;
	for( int i = mzStartIdx; i <= mzEndIdx + 1; i++ ) {
		// get point
		double x = 0.0;
		double y = 0.0;
		if( i <= mzEndIdx ) {
			x = (double)pt.getX( i );
			y = (double)pt.getY( i );
		}
		else {
			x = std::max( m_tmp->maxMz, prevMz + 1.0 );
		}

		// get index
		int idx = roundnum( (double)( x - m_tmp->minMz ) / unitRange );

		// set intensity
		if( idx >= 0 && idx < size ) {
			if( idx == prevIdx ) {
				if( fabs( y ) > fabs( intensities[ idx ] ) ) {
					intensities[ idx ] = y;
				}
			}
			else{
				intensities[ idx ] = y;
			}
		}

		// interpolation
		int sIdx = prevIdx + 1;
		sIdx = MAX( 0, sIdx );
		if( x <= prevMz + mzZeroRange ) {	// linear interpolation
			for( int j = sIdx; j < idx && j < size; j++ ) {
				double mz = m_tmp->minMz + (double)j * unitRange;

				double d1 = fabs( mz - prevMz );
				double d2 = fabs( x - mz );

				intensities[ j ] = ( d2 * prevInt + d1 * y ) / ( d1 + d2 );
			}
		}
		else {		// insert zero
			double zeroRange = std::min( ( x - prevMz ) / 2.5, mzZeroRange );

			for( int j = sIdx; j < idx && j < size; j++ ) {
				double mz = m_tmp->minMz + (double)j * unitRange;

				if( mz > x - zeroRange ) {
					double d = mz - ( x - zeroRange );

					intensities[ j ] = ( d * y ) / zeroRange;
				}
				else if( mz < prevMz + zeroRange ) {
					double d = zeroRange - fabs( mz - prevMz );

					intensities[ j ] = ( d * prevInt ) / zeroRange;
				}
				else {
					intensities[ j ] = 0.0;
				}
			}
		}

		// previous
		prevIdx = idx;
		prevMz = x;
		prevInt = y;
	}
}

// creates temporary data
void DataMapInfo::createTmpData() {
	// delete data
	deleteTmpData();

	// create
	m_tmp = (TmpData*)malloc( sizeof( TmpData ) );

	// set values
	m_tmp->minMz = double();
	m_tmp->maxMz = double();
	m_tmp->minRt = double();
	m_tmp->maxRt = double();
	m_tmp->minIntensity = double();
	m_tmp->maxIntensity = double();

	// array
	unsigned int size = m_row * m_col;
	if( size > 0 ) {
		m_tmp->intensities = new double[ size ];
		for( unsigned int i = 0; i < size; i++ ) {
			m_tmp->intensities[ i ] = 0.0;
		}
	}
	else {
		m_tmp->intensities = NULL;
	}
}

// delete temporary data
void DataMapInfo::deleteTmpData() {
	// check the member
	if( m_tmp == NULL ) {
		return;
	}

	// delete array
	if( m_tmp->intensities != NULL ) {
		delete[] m_tmp->intensities;
	}

	// delete data
	free( m_tmp );

	// set NULL
	m_tmp = NULL;
}

// compare to sort
bool DataMapInfo::lessSpec( Spectrum* spec0, Spectrum* spec1 ) {
	return ( spec0->getRt() < spec1->getRt() );
}

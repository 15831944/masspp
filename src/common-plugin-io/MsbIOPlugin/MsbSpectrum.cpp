/**
 * @file MsbSpectrum.cpp
 * @brief implements of MsbSpectrum class
 *
 * @author S.Tanaka
 * @date 2006.10.07
 * @data 2011.03.02 <Add> M.Aihara
 *       add TBB mutex code
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "MsbSpectrum.h"

#include "MsbSample.h"
#include "SpectrumPart.h"
#include "MsbManager.h"

#include <math.h>

#include <list>
#include <boost/function.hpp>
#include <boost/bind.hpp>

// @data 2011.03.02 <Add> M.Aihara ->
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
	typedef spin_mutex MSBSPEC_ParallelMutexType;
	MSBSPEC_ParallelMutexType  MSBSPEC_ParallelMutex;
}
#endif // ENABLE_MASSPP_PARALLEL_TBB_MODE
// <Add> M.Aihara

using namespace kome::io::msb;


// constructor
MsbSpectrum::MsbSpectrum( MsbSample* sample, const char* name, FILE* fp )
		: kome::objects::Spectrum( sample, name ) {
	// initialize
	m_fp = fp;
}

// destructor
MsbSpectrum::~MsbSpectrum() {
}

// get spectrum part array
SpectrumPartArray& MsbSpectrum::getParts() {
	return m_parts;
}

// add part
void MsbSpectrum::addPart( 
		const unsigned int length,
		const float start,
		const float end,
		const float totalIntensity,
		const float minIntensity,
		const float maxIntensity,
		const unsigned int msAlign,
		const unsigned int intAlign
) {
	// create object
	SpectrumPart* part = NULL;
	if( m_parts.getNumberOfParts() > 0 ) {
		part = m_parts.getPart( (int)m_parts.getNumberOfParts() - 1 );
	}


	// m/z
	float partRange = MsbManager::getPartRange();
	int idx = roundnum( ( start + end ) / ( 2.0f * partRange ) );
	float mz = partRange * (float)idx;

	if( part == NULL || fabs( mz - part->getMz() ) > 0.5 * partRange ) {	// add part
		SpectrumPart* part = &( m_parts.addPart() );

		// set parameters
		part->setMz( mz );
		part->setMinMz( start );
		part->setMaxMz( end );
		part->setTotalIntensity( totalIntensity );
		part->setMinIntensity( minIntensity );
		part->setMaxIntensity( maxIntensity );

		part->setLength( length );
		part->setMsAlign( msAlign );
		part->setIntAlign( intAlign );
	}
	else {	// merge part
		part->setMinMz( std::min( start, part->getMinMz() ) );
		part->setMaxMz( std::max( end, part->getMaxMz() ) );
		part->setTotalIntensity( part->getTotalIntensity() + totalIntensity );
		part->setMinIntensity( std::min( minIntensity, part->getMinIntensity() ) );
		part->setMaxIntensity( std::max( maxIntensity, part->getMaxIntensity() ) );

		part->setLength( length + part->getLength() );
	}
}

// get data points
void MsbSpectrum::onGetXYData(
		kome::core::XYData* const xyData,
		const double minX,
		const double maxX
) {
	// check member
	if( m_parts.getNumberOfParts() == 0 ) {
		return;
	}

	// index
	int startIdx = 0;
	if( minX >= 0.0 ) {
		startIdx = m_parts.getPartIndex( minX );
		if( startIdx < 0 ) {
			startIdx = - startIdx - 1;
		}
	}
	int endIdx = (int)m_parts.getNumberOfParts() - 1;
	if( maxX >= 0.0 ) {
		endIdx = m_parts.getPartIndex( maxX );
		if( endIdx < 0 ) {
			endIdx = - endIdx - 2;
		}
	}
	if( startIdx > endIdx ) {
		return;
	}

	// get align and length
	unsigned int mzAlign = m_parts.getPart( startIdx )->getMsAlign();
	unsigned int intAlign = m_parts.getPart( startIdx )->getIntAlign();
	unsigned length = 0;
	for( int i = startIdx; i <= endIdx; i++ ) {
		length += m_parts.getPart( i )->getLength();
	}
	if( length == 0 ) {
		return;
	}

	// create array
	float* mzArray = new float[ length ];
	float* intArray = new float[ length ];

	// read
//#pragma omp critical ( readMsbData )
	{

// @data 2011.03.02 <Add> M.Aihara ->
// add TBB mutex code
#ifdef  ENABLE_MASSPP_PARALLEL_TBB_MODE
		MSBSPEC_ParallelMutexType::scoped_lock lock( MSBSPEC_ParallelMutex );
#endif //  ENABLE_MASSPP_PARALLEL_TBB_MODE
// <Add> M.Aihara <-

		fseek( m_fp, mzAlign, SEEK_SET );
		fread( mzArray, sizeof( float ), length, m_fp );

		fseek( m_fp, intAlign, SEEK_SET );
		fread( intArray, sizeof( float ), length, m_fp );
	}

	// add
	xyData->reserve( length );
	for( unsigned int i = 0; i < length; i++ ) {
		// x, y
		double x = (double)mzArray[ i ];
		double y = (double)intArray[ i ];

		// add
		if( ( minX < 0.0 || x >= minX ) && ( maxX < 0.0 || x <= maxX ) ) {
			xyData->addPoint( x, y );
		}
	}

	// delete array
	delete[] mzArray;
	delete[] intArray;
}

// get x range
void MsbSpectrum::onGetXRange( double* minX, double* max ) {
	// already settled
}

// get total intensity
double MsbSpectrum::onGetTotalIntensity( const double minX, const double maxX ) {
	// total intensity
	double intensity = 0.0;

	// readPartsList
	std::set< int > readPartsSet;

	// index
	int startIdx = 0;
	if( minX > 0.0 ) {
		startIdx = m_parts.getPartIndex( minX );
		if( startIdx < 0 ) {
			startIdx = - startIdx - 1;
		}
		else {
			readPartsSet.insert( startIdx );
			startIdx++;
		}
	}

	int endIdx = (int)m_parts.getNumberOfParts() - 1;
	if( maxX > 0.0 ) {
		endIdx = m_parts.getPartIndex( maxX );
		if( endIdx < 0 ) {
			endIdx = - endIdx - 2;
		}
		else {
			readPartsSet.insert( endIdx );
			endIdx--;
		}
	}

	// get parts information
	for( int i = startIdx; i <= endIdx; i++ ) {
		intensity += m_parts.getPart( i )->getTotalIntensity();
	}

	// read
	for( std::set< int >::iterator it = readPartsSet.begin();
			it != readPartsSet.end(); it++ ) {
		// Spectrum Part
		SpectrumPart* part = m_parts.getPart( *it );

		// length
		unsigned int length = part->getLength();
		if( length > 0 ) {

			// create arrays
			float* mzArray = new float[ length ];
			float* intArray = new float[ length ];

			// read
#pragma omp critical ( readMsbData )
			{
				fseek( m_fp, part->getMsAlign(), SEEK_SET );
				fread( mzArray, sizeof( float ), length, m_fp );

				fseek( m_fp, part->getIntAlign(), SEEK_SET );
				fread( intArray, sizeof( float ), length, m_fp );
			}

			// add
			for( unsigned int i = 0; i < length; i++ ) {
				double x = (double)mzArray[ i ];
				double y = (double)intArray[ i ];

				if( ( minX < 0.0 || x >= minX ) && ( maxX < 0.0 || x <= maxX ) ) {
					intensity += y;
				}
			}

			// delete arrays
			delete[] mzArray;
			delete[] intArray;
		}
	}

	return intensity;
}

// get total intensity
double MsbSpectrum::onGetMaxIntensity( const double minX, const double maxX ) {
	// max intensity
	double intensity = 0.0;

	// readPartsList
	std::set< int > readPartsSet;

	// index
	int startIdx = 0;
	if( minX > 0.0 ) {
		startIdx = m_parts.getPartIndex( minX );
		if( startIdx < 0 ) {
			startIdx = - startIdx - 1;
		}
		else {
			readPartsSet.insert( startIdx );
			startIdx++;
		}
	}

	int endIdx = (int)m_parts.getNumberOfParts() - 1;
	if( maxX > 0.0 ) {
		endIdx = m_parts.getPartIndex( maxX );
		if( endIdx < 0 ) {
			endIdx = - endIdx - 2;
		}
		else {
			readPartsSet.insert( endIdx );
			endIdx--;
		}
	}

	// get parts information
	for( int i = startIdx; i <= endIdx; i++ ) {
		intensity = MAX( intensity, m_parts.getPart( i )->getMaxIntensity() );
	}

	// read
	for( std::set< int >::iterator it = readPartsSet.begin();
			it != readPartsSet.end(); it++ ) {
		// Spectrum Part
		SpectrumPart* part = m_parts.getPart( *it );
		unsigned int length = part->getLength();

		if( length > 0 ) {
			// create arrays
			float* mzArray = new float[ length ];
			float* intArray = new float[ length ];

			// read
#pragma omp critical ( readMsbData )
			{
				fseek( m_fp, part->getMsAlign(), SEEK_SET );
				fread( mzArray, sizeof( float ), length, m_fp );

				fseek( m_fp, part->getIntAlign(), SEEK_SET );
				fread( intArray, sizeof( float ), length, m_fp );
			}

			// add
			for( unsigned int i = 0; i < length; i++ ) {
				double x = (double)mzArray[ i ];
				double y = (double)intArray[ i ];

				if( ( minX < 0.0 || x >= minX ) && ( maxX < 0.0 || x <= maxX ) ) {
					intensity = MAX( intensity, y );
				}
			}

			// delete arrays
			delete[] mzArray;
			delete[] intArray;
		}
	}

	return intensity;
}

// >>>>>>	@Date:2013/09/05	<Add>	A.Ozaki
//
// set the flag of request load data (virtual)
void	MsbSpectrum::onSetRequestLoadData( void )
{
	return;
}

// set the flag of request load data (virtual)
void	MsbSpectrum::onResetRequestLoadData( void )
{
	return;
}

// check the flag of request load data (virtual)
bool	MsbSpectrum::onIsRequestLoadData( void )
{
	return	kome::objects::Spectrum::onIsRequestLoadData( );
}

// set the flag of first access (virtual)
void	MsbSpectrum::onSetFirstAccess( void )
{
	return;
}

// reset the flag of first access (virtual)
void	MsbSpectrum::onResetFirstAccess( void )
{
	return;
}

// check the flag of first access (virtual)
bool	MsbSpectrum::onIsFirstAccess( void  )
{
	return	kome::objects::Spectrum::onIsFirstAccess( );
}

// load data (virtual)
bool	MsbSpectrum::onLoadData( void )
{
	return	kome::objects::Spectrum::onLoadData( );
}

//
// <<<<<<	@Date:2013/09/05	<Add>	A.Ozaki


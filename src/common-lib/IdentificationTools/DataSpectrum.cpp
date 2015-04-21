/**
 * @file DataSpectrum.cpp
 * @brief implements of DataSpectrum class
 *
 * @author S.Tanaka
 * @date 2008.09.29
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "DataSpectrum.h"

#include "Substance.h"
#include "Hit.h"
#include "IdentificationManager.h"


using namespace kome::ident;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// static member
std::set< DataSpectrum* > DataSpectrum::m_specSet;


// constructor
DataSpectrum::DataSpectrum( const int peakId, const int matrixSampleId )
		:  kome::objects::Spectrum( NULL, "" ), m_peakId( peakId ), m_matrixSampleId( matrixSampleId ) {
	// initialize
	m_normalized = false;
	m_aligned = false;

	m_matrixPos = -1.0;
	m_specPos = -1.0;
	m_scale = 1.0;

	m_aligned = false;

	updateSpec();

	// add to set
	m_specSet.insert( this );
}

// destructor
DataSpectrum::~DataSpectrum() {
	// delete from set
	m_specSet.erase( this );
}

// get peak ID
int DataSpectrum::getMatrixPeakId() {
	return m_peakId;
}

// get matrix sample ID
int DataSpectrum::getMatrixSampleId() {
	return m_matrixSampleId;
}

// gep
double DataSpectrum::getGap() {
	double gap = 0.0;
	if( m_aligned && m_specPos >= 0.0 ) {
		gap = m_specPos - m_matrixPos;
	}
	return gap;
}

// get scale
double DataSpectrum::getScale() {
	double scale = 1.0;
	if( m_normalized && m_specPos >= 0.0 ) {
		scale = m_scale;
	}
	return scale;
}

// set peak left
void DataSpectrum::setPeakLeft( const double leftX, const double leftY ) {
	m_left.px = leftX;
	m_left.py = leftY;
}

// get peak left
kome::core::Point< double > DataSpectrum::getPeakLeft() {
	// gap, scale
	const double gap = getGap();
	const double scale = getScale();

	// left
	kome::core::Point< double > pt = m_left;
	pt.px -= gap;
	pt.py *= scale;

	return pt;
}

// set peak right
void DataSpectrum::setPeakRight( const double rightX, const double rightY ) {
	m_right.px = rightX;
	m_right.py = rightY;
}

// get peak right
kome::core::Point< double > DataSpectrum::getPeakRight() {
	// gap, scale
	const double gap = getGap();
	const double scale = getScale();

	// right
	kome::core::Point< double > pt = m_right;
	pt.px -= gap;
	pt.py *= scale;

	return pt;
}

// set normalized flag value
void DataSpectrum::setNormalized( const bool normalized ) {
	m_normalized = normalized;
}

// get normalized flag value
bool DataSpectrum::isNormalized() {
	return m_normalized;
}

// set aligned flag value
void DataSpectrum::setAligned( const bool aligned ) {
	m_aligned = aligned;
}

// get aligned flag value
bool DataSpectrum::isAligned() {
   return m_aligned;
}

// update
void DataSpectrum::updateSpec() {
	// manager
	IdentificationManager& identMgr = IdentificationManager::getInstance();

	// get peak position
	kome::objects::PeakElement peak( NULL );
	identMgr.getPeakInfo( m_peakId, m_matrixSampleId, &peak );
	m_matrixPos = identMgr.getMatrixPeakMz( m_peakId );

	// get intensity
	double intensity = identMgr.getPeakValue( m_peakId, m_matrixSampleId );
	double normalizedInt = identMgr.getNormalizedPeakValue( m_peakId, m_matrixSampleId );
	if( intensity > 0.0 ) {
		m_scale = normalizedInt / intensity;
	}
	else if( intensity < 0.0 ) {
		m_scale = -1.0;
	}
	else {
		m_scale = 1.0;
	}

	// get left & right
	m_specPos = peak.getX();
	m_left.px = ( m_specPos >= 0.0 ? peak.getLeftX() : -1.0 );
	m_left.py = ( m_specPos >= 0.0 ? peak.getLeftY() : -1.0 );
	m_right.px = ( m_specPos >= 0.0 ? peak.getRightX() : -1.0 );
	m_right.py = ( m_specPos >= 0.0 ? peak.getRightY() : -1.0 );
}

// on get xy data
void DataSpectrum::onGetXYData(
		kome::core::XYData* const xyData,
		const double minX,
		const double maxX
) {
    // manager
	IdentificationManager& identMgr = IdentificationManager::getInstance();

	// profile
	int profile = identMgr.getProfile( m_peakId, m_matrixSampleId );

	// check parameters
	if( profile < 0 || xyData == NULL
			|| ( m_normalized && m_scale < 0.0 ) ) {
		return;
	}

	// get data
	kome::core::DataPoints dps;
	identMgr.getProfile( profile, dps );

	// gap, scale
	const double gap = getGap();
	const double scale = getScale();

	// add data
	for( unsigned int i = 0; i < dps.getLength(); i++ ) {
		const double x = dps.getX( i ) - gap;
		const double y = dps.getY( i ) * scale;

		if( ( minX < 0.0 || x >= minX ) && ( maxX < 0.0 || x <= maxX ) ) {
			xyData->addPoint( x, y );
		}
	}
}

// on get x range
void DataSpectrum::onGetXRange( double* minX, double* maxX ) {
	// get data points
	kome::core::DataPoints dps;
	onGetXYData( &dps, -1.0, -1.0 );

	// range
	const double minMz = dps.getMinX();
	const double maxMz = dps.getMaxX();

	// store
	if( minX != NULL ) {
		*minX = minMz;
	}
	if( maxX != NULL ) {
		*maxX = maxMz;
	}
}

// on get total intensity
double DataSpectrum::onGetTotalIntensity( const double minX, const double maxX ) {
	// get points
	kome::core::DataPoints dps;
	onGetXYData( &dps, minX, maxX );

	// get total intensity
	double intensity = 0.0;
	for( unsigned int i = 0; i < dps.getLength(); i++ ) {
		intensity += dps.getY( i );
	}
	return intensity;
}

// on get max intensity
double DataSpectrum::onGetMaxIntensity( const double minX, const double maxX ) {
	// get points
	kome::core::DataPoints dps;
	onGetXYData( &dps, minX, maxX );

	// get max intensity
	double intensity = 0.0;
	for( unsigned int i = 0; i < dps.getLength(); i++ ) {
		const double tmp = dps.getY( i );
		if( tmp > intensity ) {
			intensity = tmp;
		}
	}
	return intensity;
}

// >>>>>>	@Date:2013/09/05	<Add>	A.Ozaki
//
// set the flag of request load data (virtual)
void	DataSpectrum::onSetRequestLoadData( void )
{
	return;
}

// set the flag of request load data (virtual)
void	DataSpectrum::onResetRequestLoadData( void )
{
	return;
}

// check the flag of request load data (virtual)
bool	DataSpectrum::onIsRequestLoadData( void )
{
	return	kome::objects::Spectrum::onIsRequestLoadData( );
}

// set the flag of first access (virtual)
void	DataSpectrum::onSetFirstAccess( void )
{
	return;
}

// reset the flag of first access (virtual)
void	DataSpectrum::onResetFirstAccess( void )
{
	return;
}

// check the flag of first access (virtual)
bool	DataSpectrum::onIsFirstAccess( void  )
{
	return	kome::objects::Spectrum::onIsFirstAccess( );
}

// load data (virtual)
bool	DataSpectrum::onLoadData( void )
{
	return	kome::objects::Spectrum::onLoadData( );
}

//
// <<<<<<	@Date:2013/09/05	<Add>	A.Ozaki

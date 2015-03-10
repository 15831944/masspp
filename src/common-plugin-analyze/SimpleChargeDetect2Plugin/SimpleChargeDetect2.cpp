/**
 * @file SimpleChargeDetect2.cpp
 * @brief interfaces of SimpleChargeDetect class (Rewrite Version)
 *
 * @author M.Fukuda
 * @date 2010.06.08
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#include <cmath>
#include "stdafx.h"
#include "SimpleChargeDetect2.h"

using namespace kome::charge::simple2;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


// constractor
SimpleChargeDetect::SimpleChargeDetect(kome::objects::Parameters* params) {
	// get spectrum
	m_spectrum = kome::plugin::PluginCallTool::getSpectrum( params );
	// get peaks object
	m_peaks = kome::plugin::PluginCallTool::getPeaks( params );

	// get setting parameters
	m_settings = kome::plugin::PluginCallTool::getSettingValues( params );

	//default
	m_peakDetector = NULL;
	m_peakSettings = NULL;
	m_mzTolerance = 0.1;
	m_precursorTol = 1.0;
	m_maxCharge = 4;
	m_N = 1.0;
}

// destractor
SimpleChargeDetect::~SimpleChargeDetect(){}

// checkstatus if brank, retrun false
bool SimpleChargeDetect::checkStatus(){

	if(m_spectrum == NULL) return false;
	if(m_peaks == NULL) return false;
	if(m_peakDetector == NULL ) return false;
	setStatus();
	return true;
}

// Setstatus
void SimpleChargeDetect::setStatus(){
	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();
	if(m_settings != NULL)  {
		const char* peaks = m_settings->getParameterValue( "peaks" );
		m_peakDetector = plgMgr.getFunctionItem( "spec_peak_picking", NVL( peaks, "" ) );
		m_peakSettings = m_settings->getSubParameterValues( "peaks", NVL( peaks, "" ) );
		m_mzTolerance = m_settings->getDoubleValue(MZ_TOLERANCE, 1.0);
		m_precursorTol = m_settings->getDoubleValue( "precursor_tolerance", 1.0 );
		m_maxCharge = m_settings->getIntValue(MAX_CHARGE, 1);
	}
}

// GetCharge
void SimpleChargeDetect::getCharge(){
	// peak detection
	kome::objects::Spectrum* parent = m_spectrum->getParentSpectrum();
	if( parent == NULL ) {
		LOG_WARN( FMT( "Failed to get the parent spectrum." ) );
		return;
	}
	kome::core::XYData* parentData = parent->getXYData();
	kome::core::DataPoints dps;
	if( parentData == NULL ) {
		parent->getXYData( &dps, -1.0, -1.0, true );
		parentData = &dps;
	}

	kome::objects::Peaks parentPeaks;

	kome::objects::Parameters params;
	kome::plugin::PluginCallTool::setPeaks( params, parentPeaks );
	kome::plugin::PluginCallTool::setXYData( params, *parentData );
	kome::plugin::PluginCallTool::setSpectrum( params, *parent );
	if( m_peakSettings != NULL ) {
		kome::plugin::PluginCallTool::setSettingValues( params, *m_peakSettings );
	}
	
	// get percursor
	const double precursor = m_spectrum->getPrecursor();
	int precursorIdx = parentPeaks.getNearestIndex( precursor );
	double precursorMz = ( precursorIdx < (int)parentPeaks.getLength() ? parentPeaks.getX( precursorIdx ) : -1.0 );

	// set precursor
	if( precursorMz > 0.0 && fabs( precursorMz - precursor ) <= m_precursorTol ) {
		m_peaks->setPrecursor( precursorMz );
	}

	int charge = -1;

	// Precursormz after search (Range : m_N)
	for (int i = precursorIdx + 1; i < (int)parentPeaks.getLength(); i++) {

		double x = parentPeaks.getX( i );

		if( x <= precursorMz + m_N + m_mzTolerance) {
			for( int z = m_maxCharge; z > 0; z--) {
				// | Z * (M_iso-M_mono) - N | < M_tol
				if(fabs((double)z * (x - precursorMz) - m_N) < m_mzTolerance) {
					charge = z;
					break;
				}
			}
		} else {
			break; // after index over range. So break
		}
	}

	// add charge
	if (charge > 0) m_peaks->addCharge(charge);

}

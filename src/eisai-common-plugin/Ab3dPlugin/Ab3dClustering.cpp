
/**
 * @file Ab3dClustering.cpp
 * @brief implements of Ab3dClustering class
 *
 * @author S.Tanaka
 * @date 2007.10.30
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "Ab3dClustering.h"


using namespace kome::ab3d;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



#define CHARGE1_PARAM_NAME				 "charge1"
#define CHARGE2_PARAM_NAME				 "charge2"
#define CHARGE3_PARAM_NAME				 "charge3"
#define CHARGE4_PARAM_NAME				 "charge4"

#define MZ_TOLERANCE_PARAM_NAME			"mz_tol"
#define RT_TOLERANCE_PARAM_NAME			"rt_tol"


// constructor
Ab3dClustering::Ab3dClustering( kome::objects::SettingParameterValues* settings ) {
	// settings
	m_settings = settings;

	// charge flags
	m_chargeFlags[ 0 ] = true;
	m_chargeFlags[ 1 ] = true;
	m_chargeFlags[ 2 ] = true;
	m_chargeFlags[ 3 ] = true;
	if( m_settings != NULL ) {
		m_chargeFlags[ 0 ] = m_settings->getBoolValue( CHARGE1_PARAM_NAME, true );
		m_chargeFlags[ 1 ] = m_settings->getBoolValue( CHARGE2_PARAM_NAME, true );
		m_chargeFlags[ 2 ] = m_settings->getBoolValue( CHARGE3_PARAM_NAME, true );
		m_chargeFlags[ 3 ] = m_settings->getBoolValue( CHARGE4_PARAM_NAME, true );
	}

	// RT tolerance
	m_rtTol = 0.5;
	if( m_settings != NULL ) {
		m_rtTol = m_settings->getDoubleValue( RT_TOLERANCE_PARAM_NAME, 0.5 );
	}

	// m/z tolerance
	m_mzTol = 0.1;
	if( m_settings != NULL ) {
		m_mzTol = m_settings->getDoubleValue( MZ_TOLERANCE_PARAM_NAME, 0.1 );
	}
}

// destructor
Ab3dClustering::~Ab3dClustering() {
}

// clustering
void Ab3dClustering::detectClusters( kome::objects::Peaks2D& peaks, kome::core::Progress& progress ) {
	// check paramters
	if( peaks.getNumberOfPeaks() == 0 ) {
		return;
	}

	// log
	std::string msg = "AB3D Clustering [charge=";
	for( unsigned int i = 0; i < 4; i++ ) {
		if( m_chargeFlags[ i ] ) {
			msg.append( FMT( "%d", ( i + 1 ) ) );
		}
	}
	msg.append( FMT( ", mz_tol=%.2f, rt_tol=%.2f]", m_mzTol, m_rtTol ) );
	LOG_INFO( msg );

	// clear
	peaks.clearClusters();
	for( unsigned int i = 0; i < peaks.getNumberOfPeaks(); i++ ) {
		peaks.getPeak( i )->setCluster( NULL );
	}

	// sort
	peaks.sortByMz( false );

	// progress
	progress.setRange( 0, peaks.getNumberOfPeaks() );

	// create clusters
	int pos = 0;
	for( unsigned int i = 0; i < peaks.getNumberOfPeaks() && !progress.isStopped(); i++ ) {
		// peak
		kome::objects::Peak2DElement* peak = peaks.getPeak( i );
		double mz = peak->getMz();

		// cluster
		kome::objects::PeaksCluster2D* cluster = peak->getCluster();
		if( cluster == NULL ) {
			cluster = peaks.createCluster();

			cluster->addPeak( peak );
			progress.setPosition( ++pos );

			// create cluster
			int charge = 4;
			bool chargeLoop = true;
			while( charge > 0 && chargeLoop ) {
				int iso = 0;
				bool isoLoop = m_chargeFlags[ charge - 1 ];

				// search isotope peak
				while( isoLoop ) {
					bool foundIso = false;
					for( unsigned int j = i + 1; j < peaks.getNumberOfPeaks(); j++ ) {
						kome::objects::Peak2DElement* tmp = peaks.getPeak( j );
						if( tmp->getCluster() == NULL ) {
							// diffrential
							double diffRt = fabs( tmp->getRt() - peak->getRt() );
							double diffMz = fabs( ( tmp->getMz() - peak->getMz() ) * (double)charge - (double)iso );

							// isotope peak
							if( diffRt <= m_rtTol && diffMz <= m_mzTol ) {
								foundIso = true;
								chargeLoop = false;
								cluster->addPeak( tmp );
								double mz = cluster->getMz();
								progress.setPosition( ++pos );

								if( cluster->getCharge() < 0 ) {
									cluster->setCharge( charge );
								}
							}
						}
					}

					// set m/z
					if( iso == 0 ) {
						cluster->setMz( cluster->getMz() );
					}

					// check
					if( iso == 0 || foundIso ) {
						iso++;
					}
					else {
						isoLoop = false;
					}
				}

				charge--;
			}
		}
	}

	// cluster intensity
	for( unsigned int i = 0; i < peaks.getNumberOfClusters() && !progress.isStopped(); i++ ) {
		// cluster
		kome::objects::PeaksCluster2D* cluster = peaks.getCluster( i );

		// intensity
		double intensity = 0.0;
		for( unsigned int j = 0; j < cluster->getNumberOfPeaks(); j++ ) {
			// peak
			kome::objects::Peak2DElement* peak = cluster->getPeak( j );

			if( peak->getIntensity() > intensity ) {
				intensity = peak->getIntensity();
			}
		}
		cluster->setIntensity( intensity );
	}
}

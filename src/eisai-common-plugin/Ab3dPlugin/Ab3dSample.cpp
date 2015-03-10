/**
 * @file Ab3dSample.cpp
 * @brief implement of Ab3dSample class
 *
 * @author S.Tanaka
 * @date 2008.01.17
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "Ab3dSample.h"

#include "Ab3dSampleSet.h"
#include "Ab3dSpectrum.h"
#include "Ab3dManager.h"

#include <utility>


using namespace kome::ab3d;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
Ab3dSample::Ab3dSample(
		Ab3dSampleSet* sampleSet,
		kome::operation::Alignment& alignment,
		kome::core::Progress& progress
) : kome::objects::Sample( sampleSet ),
		m_progress( progress ),
		m_ctrlChrom( sampleSet->getControlDataSet() ),
		m_trmtChrom( sampleSet->getTreatmentSpectra() ),
		m_alignment( alignment ) {
	m_sampleSet = sampleSet;
}

// destructor
Ab3dSample::~Ab3dSample() {
}

// get AB3D sample set
Ab3dSampleSet* Ab3dSample::getAb3dSampleSet() {
	return m_sampleSet;
}

// get control chromatogram
kome::objects::Chromatogram& Ab3dSample::getControlChromatogram() {
	return m_ctrlChrom;
}

// get treatment chromatogram
kome::objects::Chromatogram& Ab3dSample::getTreatmentChromatogram() {
	return m_trmtChrom;
}

// get treatment scale
double Ab3dSample::getTreatmentScale() {
	return m_treatmentScale;
}

// get the number of clusters
unsigned int Ab3dSample::getNumberOfClusters() {
	return m_clusters.size();
}

// get cluster
Ab3dCompositionCluster* Ab3dSample::getCluster( const unsigned int index ) {
	if( index >= m_clusters.size() ) {
		return NULL;
	}
	return &m_clusters[ index ];
}

// set properties
void Ab3dSample::setProperties() {
	// treatment properties
	kome::core::Properties trmtProps;
	kome::objects::DataGroupNode* group = m_trmtChrom.getGroup();
	while( group != NULL ) {
		// add properties
		kome::core::Properties& props = group->getProperties();
		for( unsigned int i = 0; i < props.getNumberOfProperties(); i++ ) {
			trmtProps.setValue( props.getKey( i ), props.getValue( i ) );
		}
		group = group->getParentGroup();
	}

	// control properties
	kome::core::Properties ctrlProps;
	group = m_ctrlChrom.getGroup();
	while( group != NULL ) {
		// add properties
		kome::core::Properties& props = group->getProperties();
		for( unsigned int i = 0; i < props.getNumberOfProperties(); i++ ) {
			ctrlProps.setValue( props.getKey( i ), props.getValue( i ) );
		}
		group = group->getParentGroup();
	}

	// set properties
	for( unsigned int i = 0; i < trmtProps.getNumberOfProperties(); i++ ) {
		// properties
		kome::core::Properties& props = getRootDataGroupNode()->getProperties();

		// key, value
		std::string key = trmtProps.getKey( i );
		std::string value = trmtProps.getValue( i );

		// set value
		if( ctrlProps.hasKey( key.c_str() ) && !props.hasKey( key.c_str() ) ) {
			if( strcmp( ctrlProps.getStringValue( key.c_str(), "" ), value.c_str() ) == 0 ) {
				props.setValue( key.c_str(), value.c_str() );
			}
		}
	}
}

// pick peaks
void Ab3dSample::pickPeaks( kome::objects::DataGroupNode* group, kome::objects::Peaks2D* peaks, kome::core::Progress* progress ) {
	// peak detector
	kome::plugin::PluginFunctionItem* peakDetector = m_sampleSet->getPeakDetector();

	// create parameters object
	kome::objects::Parameters params;

	kome::plugin::PluginCallTool::setGroup( params, *group );
	kome::plugin::PluginCallTool::setDataSet( params, *group );
	kome::plugin::PluginCallTool::setPeaks2D( params, *peaks );
	kome::plugin::PluginCallTool::setProgress( params, *progress );

	kome::objects::SettingParameterValues* settings = m_sampleSet->getPeakSettings();
	if( settings != NULL ) {
		kome::plugin::PluginCallTool::setSettingValues( params, *settings );
	}

	// peak detection
	peakDetector->getCall()->invoke( &params );
}

// clustering
void Ab3dSample::detectClusters( kome::objects::DataGroupNode* group, kome::objects::Peaks2D* peaks, kome::core::Progress* progress ) {
	// the number of peaks
	unsigned int num = peaks->getNumberOfPeaks();

	// check cluster detector
	kome::plugin::PluginFunctionItem* clusterDetector = m_sampleSet->getClusterDetector();
	if( clusterDetector == NULL ) {
		// range
		progress->setRange( 0, num );

		// default cluster
		for( unsigned int i = 0; i < num; i++ ) {
			kome::objects::PeaksCluster2D* cluster = peaks->createCluster();
			cluster->addPeak( peaks->getPeak( i ) );

			progress->setPosition( i + 1 );
		}
		return;
	}

	// create parameters object
	kome::objects::Parameters params;
   
	kome::plugin::PluginCallTool::setGroup( params, *group );
	kome::plugin::PluginCallTool::setDataSet( params, *group );
	kome::plugin::PluginCallTool::setPeaks2D( params, *peaks );
	kome::plugin::PluginCallTool::setProgress( params, *progress );

	kome::objects::SettingParameterValues* settings = m_sampleSet->getClusterSettings();
	if( settings != NULL ) {
		kome::plugin::PluginCallTool::setSettingValues( params, *settings );
	}

	// clustering
	clusterDetector->getCall()->invoke( &params );

	for( unsigned int i = 0; i < num; i++ ) {
		// peak
		kome::objects::Peak2DElement* peak = peaks->getPeak( i );

		if( peak->getCluster() == NULL ) {
			kome::objects::PeaksCluster2D* cluster = peaks->createCluster();
			cluster->addPeak( peak );
		}
	}
}

// create spectra
void Ab3dSample::createSpectra(
		kome::objects::Peaks2D* ctrlPeaks,
		kome::objects::Peaks2D* trmtPeaks,
		kome::core::Progress* progress
) {
	// parameters
	const double mzTol = m_sampleSet->getMzTolerance();
	const double rtTol = m_sampleSet->getRtTolerance();
	const Ab3dComposition::IntensityType intType = m_sampleSet->getIntensityType();
	const Ab3dComposition::Operation operation = m_sampleSet->getOperation();

	// copy peaks
	m_ctrlPeaks.clearClusters();
	m_ctrlPeaks.clearPeaks();
	for( unsigned int i = 0; i < ctrlPeaks->getNumberOfClusters(); i++ ) {
		// cluster
		kome::objects::PeaksCluster2D* ctrlCluster = ctrlPeaks->getCluster( i );

		// create cluster
		kome::objects::PeaksCluster2D* cluster = m_ctrlPeaks.createCluster();
		cluster->setRt( ctrlCluster->getRt() );
		cluster->setRtRange( ctrlCluster->getStartRt(), ctrlCluster->getEndRt() );
		cluster->setMz( ctrlCluster->getMz() );
		cluster->setMzRange( ctrlCluster->getStartMz(), ctrlCluster->getEndMz() );

		// add peaks
		for( unsigned int j = 0; j < ctrlCluster->getNumberOfPeaks(); j++ ) {
			kome::objects::Peak2DElement* peak = ctrlCluster->getPeak( j );
			kome::objects::Peak2DElement* newPeak = m_ctrlPeaks.createPeak(
				peak->getRt(),
				peak->getStartRt(),
				peak->getEndRt(),
				peak->getMz(),
				peak->getStartMz(),
				peak->getEndMz(),
				peak->getIntensity()
			);

			cluster->addPeak( newPeak );
		}
	}

	m_trmtPeaks.clearClusters();
	m_trmtPeaks.clearPeaks();
	for( unsigned int i = 0; i < trmtPeaks->getNumberOfClusters(); i++ ) {
		// cluster
		kome::objects::PeaksCluster2D* trmtCluster = trmtPeaks->getCluster( i );

		// create cluster
		kome::objects::PeaksCluster2D* cluster = m_trmtPeaks.createCluster();
		cluster->setRt( trmtCluster->getRt() );
		cluster->setRtRange( trmtCluster->getStartRt(), trmtCluster->getEndRt() );
		cluster->setMz( trmtCluster->getMz() );
		cluster->setMzRange( trmtCluster->getStartMz(), trmtCluster->getEndMz() );

		// add peaks
		for( unsigned int j = 0; j < trmtCluster->getNumberOfPeaks(); j++ ) {
			// peak
			kome::objects::Peak2DElement* peak = trmtCluster->getPeak( j );

			// new peak
			kome::objects::Peak2DElement* newPeak = m_trmtPeaks.createPeak(
				m_alignment.convertRt( peak->getRt(), peak->getMz() ),
				m_alignment.convertRt( peak->getStartRt(), peak->getMz() ),
				m_alignment.convertRt( peak->getEndRt(), peak->getMz() ),
				peak->getMz(),
				peak->getStartMz(),
				peak->getEndMz(),
				peak->getIntensity()
			);
			
			cluster->addPeak( newPeak );
		}
	}

	ctrlPeaks->clearClusters();
	ctrlPeaks->clearPeaks();
	trmtPeaks->clearClusters();
	trmtPeaks->clearPeaks();

	// clusters
	std::vector< kome::objects::PeaksCluster2D* > clusters;
	std::set< kome::objects::PeaksCluster2D* > clusterSet;
	for( unsigned int i = 0; i < m_ctrlPeaks.getNumberOfClusters(); i++ ) {
		kome::objects::PeaksCluster2D* cluster = m_ctrlPeaks.getCluster( i );
		if( cluster->getNumberOfPeaks() > 0 ) {
			clusters.push_back( cluster );
			clusterSet.insert( cluster );
		}
	}
	for( unsigned int i = 0; i < m_trmtPeaks.getNumberOfClusters(); i++ ) {
		kome::objects::PeaksCluster2D* cluster = m_trmtPeaks.getCluster( i );
		if( cluster->getNumberOfPeaks() > 0 ) {
			clusters.push_back( cluster );
			clusterSet.insert( cluster );
		}
	}
	int clusterNum = (int)clusters.size();

	// progress
	progress->setRange( 0, clusterNum );
	progress->setPosition( 0 );

	// create spectra
	for( unsigned int i = 0; i < clusters.size() && !progress->isStopped(); i++ ) {
		// cluster
		kome::objects::PeaksCluster2D* cluster = clusters[ i ];

		// create spectra
		if( clusterSet.find( cluster ) != clusterSet.end() ) {
			// add cluster
			m_clusters.resize( m_clusters.size() + 1 );
			m_clusters.back().setCluster( cluster );

			// control intensity
			double ctrlInt = 0.0;
			int ctrlCount = 0;
			for( unsigned int j = 0; j < m_ctrlPeaks.getNumberOfClusters(); j++ ) {
				kome::objects::PeaksCluster2D* tmp = m_ctrlPeaks.getCluster( j );

				if( tmp->getRt() >= cluster->getRt() - rtTol
						&& tmp->getRt() <= cluster->getRt() + rtTol
						&& tmp->getMz() >= cluster->getMz() - mzTol
						&& tmp->getMz() <= cluster->getMz() + mzTol
				) {
					ctrlCount++;
					ctrlInt += tmp->getIntensity();

					m_clusters.back().addCtrlCluster( tmp );
				}
			}
			if( ctrlCount > 0 ) {
				ctrlInt /= (double)ctrlCount;
			}

			// treatment intensity
			double trmtInt = 0.0;
			int trmtCount = 0;
			for( unsigned int j = 0; j < m_trmtPeaks.getNumberOfClusters(); j++ ) {
				kome::objects::PeaksCluster2D* tmp = m_trmtPeaks.getCluster( j );

				if( tmp->getRt() >= cluster->getRt() - rtTol
						&& tmp->getRt() <= cluster->getRt() + rtTol
						&& tmp->getMz() >= cluster->getMz() - mzTol
						&& tmp->getMz() <= cluster->getMz() + mzTol
				) {
					trmtCount++;
					trmtInt += tmp->getIntensity();

					m_clusters.back().addSmplCluster( tmp );

					clusterSet.erase( tmp );
				}
			}
			if( trmtCount > 0 ) {
				trmtInt /= (double)trmtCount;
			}

			// intensity
			double intensity = 0.0;
			if( operation == Ab3dComposition::OPERATION_MERGE ) { // merge
				intensity = ctrlInt + trmtInt * m_treatmentScale;
			}
			else if( operation == Ab3dComposition::OPERATION_RATIO ) {	// ration
				intensity = std::max( trmtInt * m_treatmentScale, 1.0 ) / std::max( ctrlInt, 1.0 );
			}
			else {  // subtraction
				intensity = trmtInt * m_treatmentScale - ctrlInt;
			}
			m_clusters.back().setIntensity( intensity );
			m_clusters.back().setCtrlIntensity( ctrlInt );
			m_clusters.back().setSmplIntensity( trmtInt );

			// add points
			if( fabs( intensity ) > 0.0 ) {
				// spectrum
				kome::objects::Spectrum* ctrlSpec = m_ctrlChrom.searchSpectrum( cluster->getRt() );
				kome::objects::Spectrum* trmtSpec = m_trmtChrom.searchSpectrum( cluster->getRt() );
				Ab3dSpectrum* spec = getSpectrum( ctrlSpec, trmtSpec );

				// add point
				spec->addPeakInfo( cluster->getRt(), cluster->getMz(), intensity );
			}
		}

		// progress
		progress->setPosition( clusterNum - (int)clusterSet.size() );
	}

	// sort
	kome::objects::DataGroupNode* group = getRootDataGroupNode();
	group->sortSpectra();

	for( unsigned int i = 0; i < group->getNumberOfSpectra(); i++ ) {
		Ab3dSpectrum* spec = (Ab3dSpectrum*)group->getSpectrum( i );
		spec->sort();
	}
}

// get spectrum
Ab3dSpectrum* Ab3dSample::getSpectrum( kome::objects::Spectrum* ctrlSpec, kome::objects::Spectrum* trmtSpec ) {
	// group
	kome::objects::DataGroupNode* group = getRootDataGroupNode();

	// search
	Ab3dSpectrum* spec = NULL;
	for( unsigned int i = 0; i < group->getNumberOfSpectra() && spec == NULL; i++ ) {
		Ab3dSpectrum* tmp = (Ab3dSpectrum*)group->getSpectrum( i );

		if( tmp->getCtrlSpectrum() == ctrlSpec && tmp->getSmplSpectrum() == trmtSpec ) {
			spec = tmp;
		}
	}

	// create
	if( spec == NULL ) {
		spec = new Ab3dSpectrum( *( this ), ctrlSpec, trmtSpec );
		group->addSpectrum( spec );
	}

	return spec;
}

// on open sample
bool Ab3dSample::onOpenSample( kome::objects::DataGroupNode* rootGroup, kome::core::Progress* progress ) {
	// check peak detector
	kome::plugin::PluginFunctionItem* peakDetector = m_sampleSet->getPeakDetector();
	if( peakDetector == NULL ) {
		LOG_ERROR_CODE( FMT( "Failed to get the peak detector." ), ERR_OTHER );
		return false;
	}

	// properties
	setProperties();

	// progress
	m_progress.createSubProgresses( 5 );

	// scale
	std::vector< double > ints;

	for( unsigned int i = 0; i < m_ctrlChrom.getMemberSpectra().getNumberOfSpectra(); i++ ) {
		kome::objects::Spectrum* spec = m_ctrlChrom.getMemberSpectra().getSpectrum( i );
		if( spec->getMsStage() == 1 ) {
			ints.push_back( spec->getTotalIntensity() );
		}
	}
	double cSd = 1.0;
	if( ints.size() > 0 ) {
		cSd = kome::numeric::Statistics::sd( &( ints[ 0 ] ), ints.size() );
	}
	if( cSd < 1.0 ) {
		cSd = 1.0;
	}

	ints.clear();
	for( unsigned int i = 0; i < m_trmtChrom.getMemberSpectra().getNumberOfSpectra(); i++ ) {
		kome::objects::Spectrum* spec = m_trmtChrom.getMemberSpectra().getSpectrum( i );
		if( spec->getMsStage() == 1 ) {
			ints.push_back( spec->getTotalIntensity() );
		}
	}
	double tSd = 1.0;
	if( ints.size() > 0 ) {
		tSd = kome::numeric::Statistics::sd( &( ints[ 0 ] ), ints.size() );
	}
	if( tSd < 1.0 ) {
		tSd = 1.0;
	}

	m_treatmentScale = sqrt( cSd ) / sqrt( cSd );


	// control peak picking
	kome::objects::Peaks2D ctrlPeaks;

	m_progress.setStatus( "Peak Detection (Control)" );
	pickPeaks( m_ctrlChrom.getGroup(), &ctrlPeaks, m_progress.getSubProgress( 0 ) );
	if( m_progress.isStopped() ) {
		return true;
	}
	m_progress.getSubProgress( 0 )->fill();

	// constrol clustering
	m_progress.setStatus( "Clustering (Control)" );
	detectClusters( m_ctrlChrom.getGroup(), &ctrlPeaks, m_progress.getSubProgress( 1 ) );
	if( m_progress.isStopped() ) {
		return true;
	}
	m_progress.getSubProgress( 1 )->fill();

	// treatment peak picking
	kome::objects::Peaks2D trmtPeaks;

	m_progress.setStatus( "Peak Detection (Treatment)" );
	pickPeaks( m_trmtChrom.getGroup(), &trmtPeaks, m_progress.getSubProgress( 2 ) );
	if( m_progress.isStopped() ) {
		return true;
	}
	m_progress.getSubProgress( 2 )->fill();

	// constrol clustering
	m_progress.setStatus( "Clustering (Treatment)" );
	detectClusters( m_trmtChrom.getGroup(), &trmtPeaks, m_progress.getSubProgress( 3 ) );
	if( m_progress.isStopped() ) {
		return true;
	}
	m_progress.getSubProgress( 3 )->fill();

	// operation
	const Ab3dComposition::Operation operation = m_sampleSet->getOperation();
	std::string status;
	if( operation == Ab3dComposition::OPERATION_MERGE ) {
		status = "Merge";
	}
	else if( operation == Ab3dComposition::OPERATION_RATIO ) {
		status = "Getting Ratio";
	}
	else {
		status = "Subtraction";
	}
	m_progress.setStatus( status.c_str() );
	createSpectra( &ctrlPeaks, &trmtPeaks, m_progress.getSubProgress( 4 ) );
	if( m_progress.isStopped() ) {
		return true;
	}
	m_progress.getSubProgress( 4 )->fill();

	return true;
}
 
// on close file
bool Ab3dSample::onCloseSample() {
	// clear peaks
	m_ctrlPeaks.clearClusters();
	m_ctrlPeaks.clearPeaks();

	m_trmtPeaks.clearClusters();
	m_trmtPeaks.clearPeaks();

	// clear clusters information
	m_clusters.clear();

	return true;
}

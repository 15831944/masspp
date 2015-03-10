/**
 * @file Ab3dSampleSet.cpp
 * @brief implement of Ab3dSampleSet class
 *
 * @author S.Tanaka
 * @date 2008.01.17
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "Ab3dSampleSet.h"

#include "Ab3dSample.h"
#include "Ab3dSpectrum.h"
#include "Ab3dManager.h"

#include <utility>


using namespace kome::ab3d;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



#define PEAK_DETECTOR_PARAM_NAME		   "peak_detector"
#define CLUSTER_DETECTOR_PARAM_NAME		"cluster_detector"
#define INTTYPE_PARAM_NAME				 "int_type"
#define OPERATION_PARAM_NAME			   "op"
#define SPEC_FILTER_PARAM_NAME			 "smoothing"
#define MZ_TOLERANCE_PARAM_NAME			"mz_tol"
#define RT_TOLERANCE_PARAM_NAME			"rt_tol"

#define LABELING_ITEM_NAME				 "2D_PEAK_PICKING"
#define CLUSTERING_ITEM_NAME			   "2D_PEAK_CLUSTERING"


// operation array
kome::core::ConversionElement< std::string, Ab3dComposition::IntensityType > g_ab3dIntensityTypeConversionArray[] = {
	{ "max",   Ab3dComposition::INTENSITY_MAX },
	{ "total", Ab3dComposition::INTENSITY_TOTAL }
};

kome::core::ConversionElement< std::string, Ab3dComposition::Operation > g_ab3dOperationConversionArray[] = {
	{ "subtraction", Ab3dComposition::OPERATION_SUBTRACTION },
	{ "merge",	   Ab3dComposition::OPERATION_MERGE },
	{ "ratio",	   Ab3dComposition::OPERATION_RATIO }
};


// constructor
Ab3dSampleSet::Ab3dSampleSet(
		kome::objects::DataSet& ctrlSpectra,
		kome::objects::DataSet& trmtSpectra,
		kome::objects::SettingParameterValues* settings,
		kome::operation::Alignment* alignment,
		kome::core::Progress& progress
) :  m_ctrlSpectra( ctrlSpectra ), m_trmtSpectra( trmtSpectra ), m_progress( progress ) {
	// managers
	Ab3dManager& mgr = Ab3dManager::getInstance();
	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();

	// add data
	mgr.addSampleSet( this );

	// variables
	m_settings = settings;
	m_alignment = alignment;
	if( m_alignment == NULL ) {
		m_alignment = &( kome::operation::Alignment::getDefaultAlignment() );
	}

	// string
	std::string s;

	// peak detector
	const char* peakDetector = NULL;
	if( m_settings != NULL ) {
		peakDetector = m_settings->getParameterValue( PEAK_DETECTOR_PARAM_NAME );
	}
	m_peakDetector = plgMgr.getFunctionItem( LABELING_ITEM_NAME, NVL( peakDetector, "ab3d" ) );
	if( m_peakDetector != NULL && m_settings != NULL ) {
		kome::objects::SettingParameterValues* peakSettings
			= m_settings->getSubParameterValues( PEAK_DETECTOR_PARAM_NAME, peakDetector );
		m_peakSettings = *peakSettings;
	}

	// cluster detector
	const char* clusterDetector = NULL;
	if( m_settings != NULL ) {
		clusterDetector = m_settings->getParameterValue( CLUSTER_DETECTOR_PARAM_NAME );
	}
	m_clusterDetector = plgMgr.getFunctionItem( CLUSTERING_ITEM_NAME, NVL( clusterDetector, "ab3d" ) );
	if( m_clusterDetector != NULL && m_settings != NULL ) {
		kome::objects::SettingParameterValues* clusterSettings
			= m_settings->getSubParameterValues( CLUSTER_DETECTOR_PARAM_NAME, clusterDetector );
		m_clusterSettings = *clusterSettings;
	}

	// intensity type
	const char* intType = NULL;
	if( m_settings != NULL ) {
		intType = m_settings->getParameterValue( INTTYPE_PARAM_NAME );
	}
	s = tolowercase( intType );
	m_intType = CONVERT_FROM_ARRAY( s, Ab3dComposition::INTENSITY_MAX, g_ab3dIntensityTypeConversionArray );

	// operation
	const char* operation = NULL;
	if( m_settings != NULL ) {
		operation = m_settings->getParameterValue( OPERATION_PARAM_NAME );
	}
	s = tolowercase( operation );
	m_operation = CONVERT_FROM_ARRAY( s, Ab3dComposition::OPERATION_SUBTRACTION, g_ab3dOperationConversionArray );

	// m/z tolerance
	m_mzTol = 1.0;
	if( m_settings != NULL ) {
		m_mzTol = m_settings->getDoubleValue( MZ_TOLERANCE_PARAM_NAME, 1.0 );
	}

	// RT tolerance
	m_rtTol = 3.0;
	if( m_settings != NULL ) {
		m_rtTol = m_settings->getDoubleValue( RT_TOLERANCE_PARAM_NAME, 3.0 );
	}

	// create data
	createData();
}

// destructor
Ab3dSampleSet::~Ab3dSampleSet() {
	// delete data
	Ab3dManager& mgr = Ab3dManager::getInstance();
	mgr.deleteSampleSet( this );
}

// get control spectra
kome::objects::DataSet& Ab3dSampleSet::getControlDataSet() {
	return m_ctrlSpectra;
}

// get treatment spectra
kome::objects::DataSet& Ab3dSampleSet::getTreatmentSpectra() {
	return m_trmtSpectra;
}

// get peak detector
kome::plugin::PluginFunctionItem* Ab3dSampleSet::getPeakDetector() {
	return m_peakDetector;
}

// get peak settings
kome::objects::SettingParameterValues* Ab3dSampleSet::getPeakSettings() {
	return &m_peakSettings;
}

// get cluster detector
kome::plugin::PluginFunctionItem* Ab3dSampleSet::getClusterDetector() {
	return m_clusterDetector;
}

// get cluster settings
kome::objects::SettingParameterValues* Ab3dSampleSet::getClusterSettings() {
	return &m_clusterSettings;
}

// get intensity type
Ab3dComposition::IntensityType Ab3dSampleSet::getIntensityType() {
	return m_intType;
}

// get operation
Ab3dComposition::Operation Ab3dSampleSet::getOperation() {
	return m_operation;
}

// get m/z tolerance
double Ab3dSampleSet::getMzTolerance() {
	return m_mzTol;
}

// get RT tolerance
double Ab3dSampleSet::getRtTolerance() {
	return m_rtTol;
}

// create data
void Ab3dSampleSet::createData() {
	// get name
	kome::objects::Sample* ctrlSample = m_ctrlSpectra.getSample();
	kome::objects::Sample* trmtSample = m_trmtSpectra.getSample();

	std::string dir = getdir( ctrlSample->getSampleSet()->getFilePath() );
	std::string name;
	if( ctrlSample == trmtSample ) {	// same treatment
		name = FMT( "%s_%s", m_ctrlSpectra.getGroup()->getName(), m_trmtSpectra.getGroup()->getName() );
	}
	else {  // difference treatment
		size_t extPos = 0;

		std::string trmtName = trmtSample->getSampleSet()->getFileName();
		if( ( extPos = trmtName.find_last_of( "." ) ) != trmtName.npos ) {
			trmtName = trmtName.substr( 0, extPos );
		}

		std::string ctrlName = ctrlSample->getSampleSet()->getFileName();
		if( ( extPos = ctrlName.find_last_of( "." ) ) != ctrlName.npos ) {
			ctrlName = ctrlName.substr( 0, extPos );
		}
		
		name = FMT( "%s_%s", ctrlName.c_str(), trmtName.c_str() );
	}

	if( m_operation == Ab3dComposition::OPERATION_MERGE ) {
		name.append( " (merge)" );
	}
	else if( m_operation == Ab3dComposition::OPERATION_RATIO ) {
		name.append( " (ratio)" );
	}
	else {
		name.append( " (subtraction)" );
	}

	// path
	std::string path = getabsolutepath( dir.c_str(), name.c_str() );

	// open file
	openFile( name.c_str(), &m_progress ); 
}

// on open file
bool Ab3dSampleSet::onOpenFile( const char* path, kome::core::Progress* progress ) { // add param @date 2014.07.08 <Mod> M.Izumi
	// treatment sample
	kome::objects::Sample* trmtSample = m_trmtSpectra.getSample();

	// name
	std::string name = getfilename( path );

	// sample
	Ab3dSample* sample = new Ab3dSample( this, *m_alignment, *progress ); // @date 2014.07.08 <Mod> M.Izumi
	addSample( sample );

	sample->setName( name.c_str() );
	sample->setInstrument( trmtSample->getInstrument() );
	return sample->openSample();
}

// on close file
bool Ab3dSampleSet::onCloseFile() {
	return true;
}

// on open sample
bool Ab3dSampleSet::onOpenSample( kome::objects::Sample* sample, kome::core::Progress* progress ) { // add param @date 2014.07.08 <Mod> M.Izumi
	return true;
}

// on close sample
bool Ab3dSampleSet::onCloseSample( kome::objects::Sample* sample ) {
	return true;
}

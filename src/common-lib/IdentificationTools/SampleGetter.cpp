/**
 * @file SampleGetter.cpp
 * @brief Sample Getter class
 *
 * @author M.Izumi
 * @date 2012.11.20
 * 
 * Copyright(C) 2012 Shimadzu Corporation. All rights reserved.
 */


#include "stdafx.h"
#include "SampleGetter.h"


using namespace kome::ident;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
SampleGetter::SampleGetter(){
	// initialize
	m_sample = NULL;
	m_manipulatedSample = NULL;
	m_matrixId = -1;
	m_index = -1;
	m_stdSample = NULL;
	m_readInfo = NULL;

	m_align = &( kome::operation::Alignment::getDefaultAlignment() );
}

// destructor
SampleGetter::~SampleGetter(){
	releaseSample();
}

// set matrix
void SampleGetter::setMatrix( int matrix ){
	// manager
	IdentificationManager& identMgr = IdentificationManager::getInstance();

	// release sample
	releaseSample();

	// set
	m_matrixId = matrix;
	m_index = -1;

	// samples
	m_samples.clear();

	std::vector< int > groups;
	identMgr.getGroups( groups, matrix );
	for( unsigned int i = 0; i < groups.size(); i++ ) {
		identMgr.getMatrixSamplesFromGroup( m_samples, groups[ i ] );
	}
}

// set filter call functions
void SampleGetter::setFilterCallFunctions( 
	kome::plugin::PluginFunctionItem* fun,
	kome::core::Progress& progress, 
	kome::objects::SettingParameterValues* settings,
	kome::objects::DataGroupNode* group,
	kome::objects::Peaks2D* peaks2d,
	kome::objects::Peaks2D& dst
){
	// spectrum peak -> 2d peak
	if( peaks2d == NULL || peaks2d->getNumberOfPeaks() == 0 ){
		kome::objects::PeaksManager& pkMgr = kome::objects::PeaksManager::getInstance();
		peaks2d = pkMgr.createPeaks( group );
		kome::objects::DataSet ds;
		group->getDataSet( &ds );
		for( unsigned int i=0; i < ds.getNumberOfSpectra(); i++ ){
			kome::objects::Spectrum* spec = ds.getSpectrum( i );
			if( spec != NULL ){
				kome::objects::PeaksManager& pkMgr = kome::objects::PeaksManager::getInstance();
				kome::objects::Peaks* peak = pkMgr.getPeaks( spec );

				if( peak != NULL ){	// @date 2013/08/06 <Add> OKADA
					for( unsigned int j=0; j < peak->getLength(); j++ ){
						kome::objects::PeakElement* elt = peak->getPeak( j );
					
						kome::objects::Peak2DElement* elt2d = peaks2d->createPeak( spec->getRt(), elt->getX(), elt->getY() );
						elt2d->setId( spec->getId() );// set spec id
						kome::objects::PeaksCluster2D* cluster = peaks2d->createCluster();
						cluster->setRt( spec->getRt() );
						cluster->setIntensity( elt->getY() );
						cluster->setMz( elt->getX() );
						if( elt2d != NULL ){
							elt2d->setCluster( cluster );
						}
					}
				}	// @date 2013/08/06 <Add> OKADA
			}
		}
	}
	if( fun == NULL ){// @date 2014.03.12 <Add> M.Izumi
		kome::plugin::PluginCallTool::onUpdate2DPeaks( *group );
		kome::plugin::PluginCallTool::onUpdateGroup( *group );
		return;
	}
	
	// get call
	kome::plugin::PluginCall* call = fun->getCall();
	
	// create parameters
	kome::objects::Parameters params;
	kome::plugin::PluginCallTool::setProgress( params, progress );
	kome::plugin::PluginCallTool::setSettingValues( params, *settings );
	kome::plugin::PluginCallTool::setGroup( params, *group );
	kome::plugin::PluginCallTool::setPeaks2D( params, *peaks2d );
	kome::plugin::PluginCallTool::setUpdatedPeaks2d( params, dst );

	if( call != NULL ){
		call->invoke( &params );
	}

	// update 
	kome::plugin::PluginCallTool::onUpdateGroup( *group );
}

// get next sample
kome::objects::Sample* SampleGetter::next(){
	// check the standard group
	_ASSERT( m_stdSample != NULL );    // Standard Group is not set.

	// managers
	IdentificationManager& identMgr = IdentificationManager::getInstance();
	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();

	// release
	releaseSample();

	// next sample
	m_index = m_index + 1;
	if( m_index >= (int)m_samples.size() ) {
		return NULL;
	}

	const int sample = identMgr.getMatrixSampleSample( m_samples[ m_index ] );
	m_readInfo = (SampleReadInfo*)malloc( sizeof( SampleReadInfo ) );
	m_sample = identMgr.openSample( sample, *m_readInfo );
	if( m_sample == NULL ) {
		releaseSample();
		return NULL;
	}

	// manipulated sample
	kome::objects::DataGroupNode* stdGroup = ( m_stdSample == NULL ? NULL : m_stdSample->getRootDataGroupNode() );

	m_manipulatedSample = new kome::operation::ManipulatedSample( *m_sample );

	// normalization
	double minInt = double();
	double maxInt = double();
	bool relFlg = bool();
	double minRt = double();
	double maxRt = double();
	double minMz = double();
	double maxMz = double();
	std::string method;
	std::string params;
	identMgr.getNormalizationFilter( m_matrixId, &minInt, &maxInt, &relFlg, &minRt, &maxRt, &minMz, &maxMz, method, params );

	std::vector<kome::ident::NormalizationMethodInfo > methods;
	identMgr.getNormalizationMethods( m_matrixId, methods );

	// TODO: normalization filter should be set.
	// progress
	kome::core::Progress* progress = &( kome::core::Progress::getIgnoringProgress() );
	
	kome::objects::PeaksManager& pkMgr = kome::objects::PeaksManager::getInstance();
	
	kome::plugin::PluginFunctionItem* peakDetect = plgMgr.getFunctionItem( "normaliz_peak_detect", method.c_str() );

	if( peakDetect != NULL ) {
		kome::plugin::SettingsPage* detectPage = ( peakDetect != NULL ? peakDetect->getSettingsPage() : NULL );
		kome::objects::SettingParameterValues detectSettings;
		if( detectPage != NULL ){
			detectPage->setParameterString( detectSettings, params.c_str() );
		}
		
		// create parameters
		kome::objects::Parameters param;
		kome::objects::DataSet ds;
		m_sample->getRootDataGroupNode()->getDataSet( &ds );// @date 2014.03.10 <Mod> M.Izumi
		
		kome::plugin::PluginCallTool::setDataSet( param, ds );
		kome::plugin::PluginCallTool::setControlGroup( param, *stdGroup );
		kome::plugin::PluginCallTool::setSettingValues( param, detectSettings );
		kome::plugin::PluginCallTool::setProgress( param, *progress );
		
		// get call
		kome::plugin::PluginCall* call = peakDetect->getCall();
		
		if( !progress->isStopped() ){
			if(  call != NULL  && call->invoke( &param ).prim.boolVal ){
				kome::objects::DataGroupNode* group = m_sample->getRootDataGroupNode();
				kome::objects::Peaks2D* peaks2d = pkMgr.getPeaks( group );
				kome::objects::Peaks2D* dst = pkMgr.createPeaks( group );
			
				bool bflg = false;
				if( minInt != -1.0 && maxInt != -1.0 ){
					kome::plugin::PluginFunctionItem* item =  plgMgr.getFunctionItem("normalization_filter","call_threshold");
				
					if( item != NULL ){
						// get call
						kome::plugin::SettingsPage* page = item->getSettingsPage();
					
						kome::objects::SettingParameterValues settings;
						
						settings.setValue( "type", relFlg == true ? "absolute": "relative" );
						settings.setValue( "threshold", FMT( "%f:%f" ).c_str() );				
											
						if( !progress->isStopped() ){
							setFilterCallFunctions( item, *progress, &settings, group, peaks2d, *dst );
						}
						
						bflg = true;
					}
				}			
			
				if( minRt != -1.0 && maxRt != -1.0 ){
					kome::plugin::PluginFunctionItem* item =  plgMgr.getFunctionItem("normalization_filter","call_rt_range");
					if( item != NULL ){
						// get call
						kome::plugin::SettingsPage* page = item->getSettingsPage();
					
						kome::objects::SettingParameterValues settings;
						
						settings.setValue("rt_range", FMT("%f:%f", minRt, maxRt).c_str() );
						if( !progress->isStopped() ){
							setFilterCallFunctions( item, *progress, &settings, group, peaks2d, *dst );
						}

						bflg = true;
					}
				}
			
				if( minMz != -1.0 && maxMz != -1.0 ){
					kome::objects::SettingParameterValues settings;
					kome::plugin::PluginFunctionItem* item =  plgMgr.getFunctionItem("normalization_filter","call_mz_range");
					if( item != NULL ){
						// get call
						kome::plugin::SettingsPage* page = item->getSettingsPage();
											
						settings.setValue( "mz_range", FMT("%f:%f", minMz, maxMz).c_str() );
						if( !progress->isStopped() ){
							setFilterCallFunctions( item, *progress, &settings, group, peaks2d, *dst );
						}

						bflg = true;
					
					}
				}			
				// @date 2014.03.06 <Add> M.Izumi -> 
				if( !bflg  ){					
					setFilterCallFunctions( NULL, *progress, NULL, group, peaks2d, *dst );
				}
				// @date 2014.03.06 <Add> M.Izumi <-

				for( unsigned int i = 0; i < methods.size() && !progress->isStopped(); i++ ) {
					NormalizationMethodInfo info = methods[ i ];
					kome::plugin::PluginFunctionItem* item = plgMgr.getFunctionItem( "normalization_method", info.method.c_str() );
					if( item != NULL ) {
						// parameter
						kome::objects::SettingParameterValues settings;
						kome::plugin::SettingsPage* page = item->getSettingsPage();
						if( page != NULL ) {
							page->setParameterString( settings, info.params.c_str() );
							m_manipulatedSample->setOperationInfo( item->getCall(), &settings, stdGroup );
						}
					}
				}
			}
		}
	}
		
	// alignment
	m_align->prepare( m_stdSample, m_manipulatedSample, NULL );

	return m_manipulatedSample;
}

// release sample
void SampleGetter::releaseSample() {
	// manager
	kome::ident::IdentificationManager& identMgr = kome::ident::IdentificationManager::getInstance();

	// release sample
	if( m_sample != NULL ) {
		identMgr.releaseSample( m_sample, *m_readInfo );
		m_sample = NULL;
	}

	if( m_readInfo != NULL ) {
		free( m_readInfo );
		m_readInfo = NULL;
	}

	// delete maniplulated sample
	delete m_manipulatedSample;
	m_manipulatedSample = NULL;
}

// set standard sample
void SampleGetter::setStdSample( kome::objects::Sample* sample ) {
	m_stdSample = sample;
}

// get standard sample
kome::objects::Sample* SampleGetter::getStdSample() {
	return m_stdSample;
}

// set alignment
void SampleGetter::setAlignment( kome::operation::Alignment* alignment ) {
	if( alignment == NULL ) {
		m_align = &( kome::operation::Alignment::getDefaultAlignment() );
	}
	else {
		m_align = alignment;
	}
}

// get alignment
kome::operation::Alignment* SampleGetter::getAlignment() {
	return m_align;
}

// align
void SampleGetter::align( const double rt, const double mz, double* alignedRt, double* alignedMz ) {
	if( alignedRt != NULL ) {
		*alignedRt = m_align->convertRt( rt, mz );
	}
	if( alignedMz != NULL ) {
		*alignedMz = m_align->convertMz( rt, mz );
	}
}

// number of samples
unsigned int SampleGetter::getNumberOfSamples() {
	return m_samples.size();
}

// get sample ID
int SampleGetter::getMatrixSampleId() {
	
	// sample ID
	if( m_index < 0 || m_index >= (int)m_samples.size() ) {
		return -1;
	}

	return m_samples[ m_index ];
}

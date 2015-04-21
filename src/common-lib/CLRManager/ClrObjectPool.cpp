/**
 * @file ClrObjectPool.cpp
 * @brief implements of CLRObjectPool class
 *
 * @author S.Tanaka
 * @date 2007.04.05
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "ClrObjectPool.h"

#include "ClrGraphics.h"
#include "ClrProgress.h"
#include "ClrScript.h"
#include "ClrSampleSet.h"
#include "ClrSample.h"
#include "ClrSampleReader.h"		// @date 2012/08/22 <Add> OKADA
#include "SampleReaderWrapper.h"	// @date 2012/08/22 <Add> OKADA
#include "ClrSpectrum.h"
#include "ClrChromatogram.h"
#include "ClrXYData.h"
#include "ClrXYDataOperation.h"
#include "ClrDataManager.h"
#include "ClrPluginFunction.h"

#include "SampleSetWrapper.h"
#include "SampleWrapper.h"
#include "DataGroupNodeWrapper.h"


using namespace kome::clr;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
ClrObjectPool::ClrObjectPool() {
}

// destructor
ClrObjectPool::~ClrObjectPool() {
	// delete graphics
	while( m_graphicsSet.size() > 0 ) {
		delete *( m_graphicsSet.begin() );
	}

	// delete progress
	while( m_progressSet.size() > 0 ) {
		delete *( m_progressSet.begin() );
	}

	// delete script
	while( m_scriptSet.size() > 0 ) {
		delete *( m_scriptSet.begin() );
	}
	
	// @date 2013/01/16 <Add> OKADA ------->
	// delete sample Reader set
	while( m_sampleReaderSet.size() > 0 ) {
		delete *( m_sampleReaderSet.begin() );
	}
	// @date 2013/01/16 <Add> OKADA <-------


	// delete sample set
	while( m_sampleSetSet.size() > 0 ) {
		delete *( m_sampleSetSet.begin() );
	}

	// delete sample
	while( m_sampleSet.size() > 0 ) {
		delete* ( m_sampleSet.begin() );
	}

	// delete spectrum
	while( m_spectrumSet.size() > 0 ) {
		delete *( m_spectrumSet.begin() );
	}

	// delete chromatogram
	while( m_chromatogramSet.size() > 0 ) {
		delete *( m_chromatogramSet.begin() );		
	}

	// delete xy data
	while( m_xyDataSet.size() > 0 ) {
		delete *( m_xyDataSet.begin() );
	}

	// delete operation 
	while( m_operationSet.size() > 0 ) {
		delete *( m_operationSet.begin() );
	}

	// delete data manager
	while( m_dataMgrSet.size() > 0 ) {
		delete *( m_dataMgrSet.begin() );
	}

	// delete plug-in functions
	while( m_functionSet.size() > 0 ) {
		delete *( m_functionSet.begin() );
	}

	// delete data set
	for( std::set< kome::objects::DataSet* >::iterator it = m_dataSetSet.begin();
			it != m_dataSetSet.end(); it++ ) {
		delete (*it);
	}
	m_dataSetSet.clear();
}

// create graphics object
ClrGraphics& ClrObjectPool::createGraphics() {
	ClrGraphics* g = new ClrGraphics();
	m_graphicsSet.insert( g );

	return *g;
}

// destroy graphics object
void ClrObjectPool::destroyGraphics( ClrGraphics& graphics ) {
	if( m_graphicsSet.find( &graphics ) != m_graphicsSet.end() ) {
		delete &graphics;
	}
}

// remove graphics object
void ClrObjectPool::removeGraphics( ClrGraphics& graphics ) {
	m_graphicsSet.erase( &graphics );
}

// create progress object
ClrProgress& ClrObjectPool::createProgress() {
	ClrProgress* progress = new ClrProgress();
	m_progressSet.insert( progress );

	return *progress;
}

// destroy progress object
void ClrObjectPool::destroyProgress( ClrProgress& progress ) {
	if( m_progressSet.find( &progress ) != m_progressSet.end() ) {
		delete &progress;
	}
}

// remove progress object
void ClrObjectPool::removeProgress( ClrProgress& progress ) {
	m_progressSet.erase( &progress );
}

// create script object
ClrScript& ClrObjectPool::createScript() {
	ClrScript* script = new ClrScript();
	m_scriptSet.insert( script );

	return *script;
}

// destroy script object
void ClrObjectPool::destroyScript( ClrScript& script ) {
	if( m_scriptSet.find( &script ) != m_scriptSet.end() ) {
		delete &script;
	}
}

// remove script object
void ClrObjectPool::removeScript( ClrScript& script ) {
	m_scriptSet.erase( &script );
}

// create sample set object
ClrSampleSet& ClrObjectPool::createSampleSet() {
	ClrSampleSet* sampleSet = new ClrSampleSet();
	m_sampleSetSet.insert( sampleSet );

	return *sampleSet;
}

// destroy sample set object
void ClrObjectPool::destroySampleSet( ClrSampleSet& sampleSet ) {
	if( m_sampleSetSet.find( &sampleSet ) != m_sampleSetSet.end() ) {
		delete &sampleSet;
	}
}

// remove sample set object
void ClrObjectPool::removeSampleSet( ClrSampleSet& sampleSet ) {
	m_sampleSetSet.erase( &sampleSet );
}

// create sample
ClrSample& ClrObjectPool::createSample( SampleSetWrapper^ sampleSet ) {
	ClrSample* sample = new ClrSample( sampleSet );
	m_sampleSet.insert( sample );

	return *sample;
}

// destroy sample object
void ClrObjectPool::destroySample( ClrSample& sample ) {
	if( m_sampleSet.find( &sample ) != m_sampleSet.end() ) {
		delete &sample;
	}
}

// remove sample object
void ClrObjectPool::removeSample( ClrSample& sample ) {
	m_sampleSet.erase( &sample );
}

// create spectrum object
ClrSpectrum& ClrObjectPool::createSpectrum( SampleWrapper^ sample, System::String^ name ) {
	ClrSpectrum* spectrum = new ClrSpectrum( sample, name );
	m_spectrumSet.insert( spectrum );

	return *spectrum;
}

// destroy spectrum object
void ClrObjectPool::destroySpectrum( ClrSpectrum& spectrum ) { 
	if( m_spectrumSet.find( &spectrum ) != m_spectrumSet.end() ) {
		delete &spectrum;
	}
}

// remove spectrum object
void ClrObjectPool::removeSpectrum( ClrSpectrum& spectrum ) {
	m_spectrumSet.erase( &spectrum );
}



// @date 2012/08/22 <Add> OKADA ------->
// create sample Reader
ClrSampleReader& ClrObjectPool::createSampleReader() {

	kome::clr::ClrSampleReader* clrSampleReader = new kome::clr::ClrSampleReader();
	m_sampleReaderSet.insert( clrSampleReader );

	return *clrSampleReader;
}

// destroy sample Reader object
void ClrObjectPool::destroySampleReader( ClrSampleReader& sampleReader ) {
	if( m_sampleReaderSet.find( &sampleReader ) != m_sampleReaderSet.end() ) {
		delete &sampleReader;
	}
}

// remove sample Reader object
void ClrObjectPool::removeSampleReader( ClrSampleReader& sampleReader ) {
	m_sampleReaderSet.erase( &sampleReader );
}
// @date 2012/08/22 <Add> OKADA <-------

// create chromatogram object
ClrChromatogram& ClrObjectPool::createChromatogram( SampleWrapper^ sample ) {
	ClrChromatogram* chromatogram = new ClrChromatogram( sample );
	m_chromatogramSet.insert( chromatogram );

	return *chromatogram;
}

// destroy chromatogram object
void ClrObjectPool::destroyChromatogram( ClrChromatogram& chromatogram ) { 
	if( m_chromatogramSet.find( &chromatogram ) != m_chromatogramSet.end() ) {
		delete &chromatogram;
	}
}

// remove chromatogram object
void ClrObjectPool::removeChromatogram( ClrChromatogram& chromatogram ) {
	m_chromatogramSet.erase( &chromatogram );
}

// create xy data object
ClrXYData& ClrObjectPool::createXYData() {
	ClrXYData* xyData = new ClrXYData();
	m_xyDataSet.insert( xyData );

	return *xyData;
}

// destroy xy data object
void ClrObjectPool::destroyXYData( ClrXYData& xyData ) {
	if( m_xyDataSet.find( &xyData ) != m_xyDataSet.end() ) {
		delete &xyData;
	}
}

// remove xy data object
void ClrObjectPool::removeXYData( ClrXYData& xyData ) {
	m_xyDataSet.erase( &xyData );
}

// create operation object
ClrXYDataOperation& ClrObjectPool::createOperation() {
	ClrXYDataOperation* operation = new ClrXYDataOperation();
	m_operationSet.insert( operation );

	return *operation;
}

// destroy operation object
void ClrObjectPool::destroyOperation( ClrXYDataOperation& operation ) {
	if( m_operationSet.find( &operation ) != m_operationSet.end() ) {
		delete &operation;
	}
}

// remove operation object
void ClrObjectPool::removeOperation( ClrXYDataOperation& operation ) {
	m_operationSet.erase( &operation );
}

// create data manager object
ClrDataManager& ClrObjectPool::createDataManager() {
	ClrDataManager* mgr = new ClrDataManager();
	m_dataMgrSet.insert( mgr );

	return *mgr;
}

// destroy data manager object
void ClrObjectPool::destroyDataManager( ClrDataManager& mgr ) {
	if( m_dataMgrSet.find( &mgr ) != m_dataMgrSet.end() ) {
		delete &mgr;
	}
}

// remove data manager object
void ClrObjectPool::removeDataManager( ClrDataManager& mgr ) {
	m_dataMgrSet.erase( &mgr );
}

// create plug-in function object
ClrPluginFunction& ClrObjectPool::createPluginFunction() {
	ClrPluginFunction* fun = new ClrPluginFunction();
	m_functionSet.insert( fun );

	return *fun;
}

// destroy plug-in function object
void ClrObjectPool::destroyPluginFunction( ClrPluginFunction& fun ) {
	if( m_functionSet.find( &fun ) != m_functionSet.end() ) {
		delete &fun;
	}
}

// remove plug-in function object
void ClrObjectPool::removePluginFunction( ClrPluginFunction& fun ) {
	m_functionSet.erase( &fun );
}

// create data set object
kome::objects::DataSet& ClrObjectPool::createDataSet( DataGroupNodeWrapper^ group ) {
	// group
	kome::objects::DataGroupNode* g = NULL;
	if( group != nullptr ) {
		g = &group->getDataGroupNode();
	}

	// data set
	kome::objects::DataSet* dataSet = new kome::objects::DataSet( g );
	m_dataSetSet.insert( dataSet );

	return *dataSet;
}

// destroy data set object
void ClrObjectPool::destroyDataSet( kome::objects::DataSet& dataSet ) {
	if( m_dataSetSet.find( &dataSet ) != m_dataSetSet.end() ) {
		m_dataSetSet.erase( &dataSet );
		delete &dataSet;
	}
}

// remove data set object
void ClrObjectPool::removeDataSet( kome::objects::DataSet& dataSet ) {
	m_dataSetSet.erase( &dataSet );
}

// get instance
ClrObjectPool& ClrObjectPool::getInstance() {
	// create object (This is the only object.)
	static ClrObjectPool pool;

	return pool;
}

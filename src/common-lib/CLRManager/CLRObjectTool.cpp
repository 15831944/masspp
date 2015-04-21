/**
 * @file ClrObjectTool.cpp
 * @brief implements of ClrObjectTool class
 *
 * @author S.Tanaka
 * @date 2006.10.18
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#include "stdafx.h"
#include "ClrObjectTool.h"

#include "PropertiesWrapper.h"
#include "NumberRestrictionWrapper.h"
#include "ProgressWrapper.h"
#include "XYDataWrapper.h"
#include "SampleSetWrapper.h"
#include "SampleWrapper.h"
#include "SpectrumWrapper.h"
#include "ChromatogramWrapper.h"
#include "DataSetWrapper.h"
#include "DataGroupNodeWrapper.h"
#include "ChromatogramWrapper.h"
#include "PeaksWrapper.h"
#include "PeakElementWrapper.h"
#include "Peaks2DWrapper.h"
#include "GraphicsWrapper.h"
#include "MouseEventWrapper.h"
#include "KeyEventWrapper.h"
#include "SizeEventWrapper.h"
#include "DataMapInfoWrapper.h"
#include "DataManagerWrapper.h"
#include "ClrVariant.h"
#include "ClrParameters.h"
#include "SettingParameterValuesWrapper.h"

#include "ClrProgress.h"
#include "ClrProgressBase.h"
#include "ClrXYData.h"
#include "ClrXYDataBase.h"
#include "ClrSampleSet.h"
#include "ClrSampleSetBase.h"
#include "ClrSample.h"
#include "ClrSampleBase.h"
#include "ClrSpectrum.h"
#include "ClrChromatogram.h"
#include "ClrSpectrumBase.h"
#include "ClrGraphics.h"
#include "ClrGraphicsBase.h"
#include "ClrDataManager.h"
#include "ClrDataManagerBase.h"


using namespace kome::clr;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// System::String^ -> std::string
std::string ClrObjectTool::convertString( gcroot< System::String^ > s ) {
	// Marshal
	using System::Runtime::InteropServices::Marshal;

	// check parameter
	System::String^ s2 = s;
	if( s2 == nullptr ) {
		return std::string( "" );
	}

	// convert
	const char* chars = (const char*)( Marshal::StringToHGlobalAnsi( s2 ) ).ToPointer();

	// create string
	std::string str( NVL( chars, "" ) );

	// free
	Marshal::FreeHGlobal( System::IntPtr( (void*)chars ) );

	return str;
}

// const char* -> System::String^
gcroot< System::String^ > ClrObjectTool::convertString( const char* s, const char* dfVal ) {
	// check parameters
	if( s == NULL && dfVal == NULL ) {
		return nullptr;
	}

	return gcnew System::String( NVL( s, dfVal ) );
}

// create PropertiesWrapper object
gcroot< PropertiesWrapper^ > ClrObjectTool::createPropertiesWrapper( kome::core::Properties* props ) {
	// check the parameter
	if( props == NULL ) {
		return nullptr;
	}

	return gcnew PropertiesWrapper( *props );
}

// get Properties object
kome::core::Properties* ClrObjectTool::getProperties( gcroot< PropertiesWrapper^ > props ) {
	// get properties
	PropertiesWrapper^ p = props;
	if( p == nullptr ) {
		return NULL;
	}

	return &p->getProperties();
}

// create NumberRestrictionWrapper object
gcroot< NumberRestrictionWrapper^ > ClrObjectTool::createNumberRestrictionWrapper( kome::core::NumberRestriction* restriction ) {
	// check the parameter
	if( restriction == NULL ) {
		return nullptr;
	}

	return gcnew NumberRestrictionWrapper( *restriction );
}

// get NumberRestriction object
kome::core::NumberRestriction* ClrObjectTool::getNumberRestriction( gcroot< NumberRestrictionWrapper^ > restriction ) {
	// get restriction
	NumberRestrictionWrapper^ r = restriction;
	if( r == nullptr ) {
		return NULL;
	}

	return &r->getNumberRestriction();
}

// create ProgressWrapper object
gcroot< ProgressWrapper^ > ClrObjectTool::createProgressWrapper( kome::core::Progress* progress ) {
	// check the parameter
	if( progress == NULL ) {
		return nullptr;
	}

	// cast
	ClrProgress* clrPgrs = dynamic_cast< ClrProgress* >( progress );
	if( clrPgrs != NULL ) {
		return clrPgrs->getBaseProgress();
	}

	return gcnew ProgressWrapper( *progress );
}

// get Progress object
kome::core::Progress* ClrObjectTool::getProgress( gcroot< ProgressWrapper^ > progress ) {
	// get progress
	ProgressWrapper^ p = progress;
	if( p == nullptr ) {
		return NULL;
	}

	return &p->getProgress();
}

// create XYDataWrapper object
gcroot< XYDataWrapper^ > ClrObjectTool::createXYDataWrapper( kome::core::XYData* xyData ) {
	// check the parameter
	if( xyData == NULL ) {
		return nullptr;
	}

	// cast
	ClrXYData* clrXYData = dynamic_cast< ClrXYData* >( xyData );
	if( clrXYData != NULL ) {
		return clrXYData->getBaseData();
	}

	return gcnew XYDataWrapper( *xyData );
}

// get XYData object
kome::core::XYData* ClrObjectTool::getXYData( gcroot< XYDataWrapper^ > xyData ) {
	// get xy data
	XYDataWrapper^ xy = xyData;
	if( xy == nullptr ) {
		return NULL;
	}

	return &xy->getXYData();
}

// create SampleSet object
gcroot< SampleSetWrapper^ > ClrObjectTool::createSampleSetWrapper( kome::objects::SampleSet* sampleSet ) {
	// check the parameter
	if( sampleSet == NULL ) {
		return nullptr;
	}

	// cast
	ClrSampleSet* clrSampleSet = dynamic_cast< ClrSampleSet* >( sampleSet );
	if( clrSampleSet != NULL ) {
		return clrSampleSet->getBaseSampleSet();
	}

	return gcnew SampleSetWrapper( *sampleSet );
}

// get SampleSet object
kome::objects::SampleSet* ClrObjectTool::getSampleSet( gcroot< SampleSetWrapper^ > sampleSet ) {
	// get sample set
	SampleSetWrapper^ ss = sampleSet;
	if( ss == nullptr ) {
		return NULL;
	}

	return &ss->getSampleSet();
}

// create Sample object
gcroot< SampleWrapper^ > ClrObjectTool::createSampleWrapper( kome::objects::Sample* sample ) {
	// check the parameter
	if( sample == NULL ) {
		return nullptr;
	}

	// cast
	ClrSample* clrSample = dynamic_cast< ClrSample* >( sample );
	if( clrSample != NULL ) {
		return clrSample->getBaseSample();
	}

	return gcnew SampleWrapper( *sample );
}

// get Sample object
kome::objects::Sample* ClrObjectTool::getSample( gcroot< SampleWrapper^ > sample ) {
	// get sample
	SampleWrapper^ s = sample;
	if( s == nullptr ) {
		return NULL;
	}

	return &s->getSample();
}

// create SpectrumWrapper object
gcroot< SpectrumWrapper^ > ClrObjectTool::createSpectrumWrapper( kome::objects::Spectrum* spec ) {
	// check the parameter
	if( spec == NULL ) {
		return nullptr;
	}

	// cast
	ClrSpectrum* clrSpec = dynamic_cast< ClrSpectrum* >( spec );
	if( clrSpec != NULL ) {
		return clrSpec->getBaseSpectrum();
	}

	return gcnew SpectrumWrapper( *spec );
}

// get Spectrum object
kome::objects::Spectrum* ClrObjectTool::getSpectrum( gcroot< SpectrumWrapper^ > spec ) {
	// get spectrum
	SpectrumWrapper^ s = spec;
	if( s == nullptr ) {
		return NULL;
	}

	return &s->getSpectrum();
}

// create ChromatogramWrapper object
gcroot< ChromatogramWrapper^ > ClrObjectTool::createChromatogramWrapper( kome::objects::Chromatogram* chrom ) {
	// check the parameter
	if( chrom == NULL ) {
		return nullptr;
	}

	// cast
	ClrChromatogram* clrChrom = dynamic_cast< ClrChromatogram* >( chrom );
	if( clrChrom != NULL ) {
		return clrChrom->getBaseChromatogram();
	}

	return gcnew ChromatogramWrapper( *chrom );
}

// get Chromatogram object
kome::objects::Chromatogram* ClrObjectTool::getChromatogram( gcroot< ChromatogramWrapper^ > chrom ) {
	// get chromatogram
	ChromatogramWrapper^ c = chrom;
	if( c == nullptr ) {
		return NULL;
	}

	return &c->getChromatogram();
}

// create DataSetWrapper object
gcroot< DataSetWrapper^ > ClrObjectTool::createDataSetWrapper( kome::objects::DataSet* dataSet ) {
	// check the parameter
	if( dataSet == NULL ) {
		return nullptr;
	}

	return gcnew DataSetWrapper( *dataSet );
}

// get DataSet object
kome::objects::DataSet* ClrObjectTool::getDataSet( gcroot< DataSetWrapper^ > spectra ) {
	// get spectra
	DataSetWrapper^ s = spectra;
	if( s == nullptr ) {
		return NULL;
	}

	return &s->getDataSet();
}

// create DataGroupNodeWrapper object
gcroot< DataGroupNodeWrapper^ > ClrObjectTool::createDataGroupNodeWrapper( kome::objects::DataGroupNode* group ) {
	// check the parameter
	if( group == NULL ) {
		return nullptr;
	}

	return gcnew DataGroupNodeWrapper( *group );
}

// get DataGroupNode object
kome::objects::DataGroupNode* ClrObjectTool::getDataGroupNode( gcroot< DataGroupNodeWrapper^ > group ) {
	// get spectrum group
	DataGroupNodeWrapper^ sg = group;
	if( sg == nullptr ) {
		return NULL;
	}

	return &sg->getDataGroupNode();
}

// create PeaksWrapper object
gcroot< PeaksWrapper^ > ClrObjectTool::createPeaksWrapper( kome::objects::Peaks* peaks ) {
	// check the parameter
	if( peaks == NULL ) {
		return nullptr;
	}

	return gcnew PeaksWrapper( *peaks );
}

// get Peaks object
kome::objects::Peaks* ClrObjectTool::getPeaks( gcroot< PeaksWrapper^ > peaks ) {
	// get peaks
	PeaksWrapper^ p = peaks;
	if( p == nullptr ) {
		return NULL;
	}

	return &p->getPeaks();
}

// create PeakElementWrapper object
gcroot< PeakElementWrapper^ > ClrObjectTool::createPeakElementWrapper( kome::objects::PeakElement* element ){
	// check the parameter
	if( element == NULL ){
		return nullptr;
	}

	return gcnew PeakElementWrapper( *element );
}

// get peak element object
kome::objects::PeakElement* ClrObjectTool::getPeakElement( gcroot< PeakElementWrapper^ > element ){
	PeakElementWrapper^ e = element;
	if( e == nullptr ){
		return NULL;
	}

	return &e->getPeakElement();
}

// create Peaks2DWrapper object
gcroot< Peaks2DWrapper^ > ClrObjectTool::createPeaks2DWrapper( kome::objects::Peaks2D* peaks ) {
	// check the parameter
	if( peaks == NULL ) {
		return nullptr;
	}

	return gcnew Peaks2DWrapper( *peaks );
}

// get Peaks2D object
kome::objects::Peaks2D* ClrObjectTool::getPeaks2D( gcroot< Peaks2DWrapper^ > peaks ) {
	// get peaks
	Peaks2DWrapper^ p = peaks;
	if( p == nullptr ) {
		return NULL;
	}

	return &p->getPeaks();
}

// create GraphicsWrapper object
gcroot< GraphicsWrapper^ > ClrObjectTool::createGraphicsWrapper( kome::img::Graphics* graphics ) {
	// check the parameter
	if( graphics == NULL ) {
		return nullptr;
	}

	// cast
	ClrGraphics* clrGraphics = dynamic_cast< ClrGraphics* >( graphics );
	if( clrGraphics != NULL ) {
		return clrGraphics->getBaseGraphics();
	}

	return gcnew GraphicsWrapper( *graphics );
}

// get Graphics object
kome::img::Graphics* ClrObjectTool::getGraphics( gcroot< GraphicsWrapper^ > graphics ) {
	// get graphics
	GraphicsWrapper^ g = graphics;
	if( g == nullptr ) {
		return NULL;
	}

	return &g->getGraphics();
}

// create MouseEventWrapper object
gcroot< MouseEventWrapper^ > ClrObjectTool::createMouseEventWrapper( kome::evt::MouseEvent* evt ) {
	// check the parameter
	if( evt == NULL ) {
		return nullptr;
	}

	return gcnew MouseEventWrapper( *evt );
}

// get MouseEvent object
kome::evt::MouseEvent* ClrObjectTool::getMouseEvent( gcroot< MouseEventWrapper^ > evt ) {
	// get mouse event
	MouseEventWrapper^ e = evt;
	if( e == nullptr ) {
		return NULL;
	}

	return &e->getMouseEvent();
}

// create KeyEventWrapper object
gcroot< KeyEventWrapper^ > ClrObjectTool::createKeyEventWrapper( kome::evt::KeyEvent* evt ) {
	// check the parameter
	if( evt == NULL ) {
		return nullptr;
	}

	return gcnew KeyEventWrapper( *evt );
}

// get KeyEvent object
kome::evt::KeyEvent* ClrObjectTool::getKeyEvent( gcroot< KeyEventWrapper^ > evt ) {
	// get key event
	KeyEventWrapper^ e = evt;
	if( e == nullptr ) {
		return NULL;
	}

	return &e->getKeyEvent();
}

// create SizeEventWrapper object
gcroot< SizeEventWrapper^ > ClrObjectTool::createSizeEventWrapper( kome::evt::SizeEvent* evt ) {
	// check the parameter
	if( evt == NULL ) {
		return nullptr;
	}

	return gcnew SizeEventWrapper( *evt );
}

// get SizeEvent object
kome::evt::SizeEvent* ClrObjectTool::getSizeEvent( gcroot< SizeEventWrapper^ > evt ) {
	// get size event
	SizeEventWrapper^ e = evt;
	if( e == nullptr ) {
		return NULL;
	}

	return &e->getSizeEvent();
}

// create DataMapInfoWrapper object
gcroot< DataMapInfoWrapper^ > ClrObjectTool::createDataMapInfoWrapper( kome::objects::DataMapInfo* dataMap ) {
	// check the parameter
	if( dataMap == NULL ) {
		return nullptr;
	}

	return gcnew DataMapInfoWrapper( *dataMap );
}

// get DataMapInfo object
kome::objects::DataMapInfo* ClrObjectTool::getDataMapInfo( gcroot< DataMapInfoWrapper^ > dataMap ) {
	// get data map
	DataMapInfoWrapper^ dm = dataMap;
	if( dm == nullptr ) {
		return NULL;
	}

	return &dm->getDataMapInfo();
}

// create DataManagerWrapper object
gcroot< DataManagerWrapper^ > ClrObjectTool::createDataManagerWrapper( kome::objects::DataManager* dataMgr ) {
	// check the parameter
	if( dataMgr == NULL ) {
		return nullptr;
	}

	// cast
	ClrDataManager* clrDataMgr = dynamic_cast< ClrDataManager* >( dataMgr );
	if( clrDataMgr != NULL ) {
		return clrDataMgr->getBaseDataManager();
	}

	return gcnew DataManagerWrapper( *dataMgr );
}

// get DataManager object
kome::objects::DataManager* ClrObjectTool::getDataManager( gcroot< DataManagerWrapper^ > dataMgr ) {
	// get data manager
	DataManagerWrapper^ dm = dataMgr;
	if( dm == nullptr ) {
		return NULL;
	}

	return &dm->getDataManager();
}

// create SettingParameterValuesWrapper object
gcroot< SettingParameterValuesWrapper^ > ClrObjectTool::createSettingsWrapper( kome::objects::SettingParameterValues* settings ) {
	// check the parameter
	if( settings == NULL ) {
		return nullptr;
	}

	// create
	SettingParameterValuesWrapper^ s = gcnew SettingParameterValuesWrapper( *settings );

	return s;
}

// get SettingParameterValues object
kome::objects::SettingParameterValues* ClrObjectTool::getSettings( gcroot< SettingParameterValuesWrapper^ > settings ) {
	// get settings object
	SettingParameterValuesWrapper^ s = settings;
	if( s == nullptr ) {
		return NULL;
	}

	return &s->getSettingParameterValues();
}


// Variant -> ClrVariant
void ClrObjectTool::convertVariantToCLR(
		gcroot< ClrVariant^ > var0,
		kome::objects::Variant* var1
) {
	// check parameters
	ClrVariant^ v0 = var0;
	if( v0 == nullptr || var1 == NULL ) {
		return;
	}

	// set value
	v0->setValue( *var1 );
}

// ClrVariant -> Variant
void ClrObjectTool::convertVariantFromCLR(
		kome::objects::Variant* var0,
		gcroot< ClrVariant^ > var1
) {
	// check parameters
	ClrVariant^ v1 = var1;
	if( var0 == NULL || v1 == nullptr ) {
		return;
	}

	// set value
	v1->transmit( *var0 );
}

// Parameters -> ClrParameters
void ClrObjectTool::convertParamsToCLR(
		gcroot< ClrParameters^ > params0,
		kome::objects::Parameters* params1
) {
	// check parameters
	ClrParameters^ p0 = params0;
	if( p0 == nullptr || params1 == NULL ) {
		return;
	}

	// set value
	p0->setParameters( *params1 );
}

// ClrParameters -> Parameters
void ClrObjectTool::convertParamsFromCLR(
		kome::objects::Parameters* params0,
		gcroot< ClrParameters^ > params1
) {
	// check parameters
	ClrParameters^ p1 = params1;
	if( params0 == NULL || p1 == nullptr ) {
		return;
	}

	// set value
	p1->getParameters( *params0 );
}

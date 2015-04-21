/**
 * @file ClrMsDataVariant.cpp
 * @brief implements of ClrMsDataVariant class
 *
 * @author S.Tanaka
 * @date 2008.12.25
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "ClrMsDataVariant.h"

#include "ClrVariant.h"
#include "SpectrumWrapper.h"
#include "ChromatogramWrapper.h"
#include "DataMapInfoWrapper.h"
#include "DataGroupNodeWrapper.h"
#include "SampleWrapper.h"
#include "SampleSetWrapper.h"


using namespace kome::clr;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
ClrMsDataVariant::ClrMsDataVariant() {
	m_var = gcnew ClrVariant();

	unsetObject();
}

// constructor
ClrMsDataVariant::ClrMsDataVariant( ClrVariant^ var ) {
	m_var = var;
}

// destructor
ClrMsDataVariant::~ClrMsDataVariant() {
}

// set variant
void ClrMsDataVariant::setVariant( ClrVariant^ var ) {
	if( var == nullptr ) {
		unsetObject();
	}
	else {
		m_var->type = var->type;
		m_var->obj = var->obj;
	}
}

// get variant object
ClrVariant^ ClrMsDataVariant::getVariant() {
	return m_var;
}

// unset object
void ClrMsDataVariant::unsetObject() {
	m_var->type = ClrVariant::DataType::OTHER;
	m_var->obj = nullptr;
}

// set spectrum
void ClrMsDataVariant::setSpectrum( SpectrumWrapper^ spec ) {
	if( spec == nullptr ) {
		unsetObject();
	}
	else {
		m_var->type = ClrVariant::DataType::SPECTRUM;
		m_var->obj = spec;
	}
}

// set chromatogram
void ClrMsDataVariant::setChromatogram( ChromatogramWrapper^ chrom ) {
	if( chrom == nullptr ) {
		unsetObject();
	}
	else {
		m_var->type = ClrVariant::DataType::CHROMATOGRAM;
		m_var->obj = chrom;
	}
}

// set data map
void ClrMsDataVariant::setDataMap( DataMapInfoWrapper^ dataMap ) {
	if( dataMap == nullptr ) {
		unsetObject();
	}
	else {
		m_var->type = ClrVariant::DataType::DATA_MAP;
		m_var->obj = dataMap;
	}
}

// set group
void ClrMsDataVariant::setGroup( DataGroupNodeWrapper^ group ) {
	if( group == nullptr ) {
		unsetObject();
	}
	else {
		m_var->type = ClrVariant::DataType::DATA_GROUP_NODE;
		m_var->obj = group;
	}
}

// set sample
void ClrMsDataVariant::setSample( SampleWrapper^ sample ) {
	if( sample == nullptr ) {
		unsetObject();
	}
	else {
		m_var->type = ClrVariant::DataType::SAMPLE;
		m_var->obj = sample;
	}
}

// set sample set
void ClrMsDataVariant::setSampleSet( SampleSetWrapper^ sampleSet ) {
	if( sampleSet == nullptr ) {
		unsetObject();
	}
	else {
		m_var->type = ClrVariant::DataType::SAMPLE_SET;
		m_var->obj = sampleSet;
	}
}

// get spectrum
SpectrumWrapper^ ClrMsDataVariant::getSpectrum() {
	SpectrumWrapper^ spec = nullptr;
	if( m_var->type == ClrVariant::DataType::SPECTRUM ) {
		spec = (SpectrumWrapper^)m_var->obj;
	}

	return spec;
}

// get chromatogram
ChromatogramWrapper^ ClrMsDataVariant::getChromatogram() {
	ChromatogramWrapper^ chrom = nullptr;
	if( m_var->type == ClrVariant::DataType::CHROMATOGRAM ) {
		chrom = (ChromatogramWrapper^)m_var->obj;
	}

	return chrom;
}

// get data map
DataMapInfoWrapper^ ClrMsDataVariant::getDataMap() {
	DataMapInfoWrapper^ dataMap = nullptr;
	if( m_var->type == ClrVariant::DataType::DATA_MAP ) {
		dataMap = (DataMapInfoWrapper^)m_var->obj;
	}

	return dataMap;
}

// get group
DataGroupNodeWrapper^ ClrMsDataVariant::getGroup() {
	DataGroupNodeWrapper^ group = nullptr;

	if( m_var->type == ClrVariant::DataType::DATA_GROUP_NODE ) {	// group
		group = (DataGroupNodeWrapper^)m_var->obj;
	}
	else {   // etc
		SpectrumWrapper^ spec = getSpectrum();
		ChromatogramWrapper^ chrom = getChromatogram();
		DataMapInfoWrapper^ dataMap = getDataMap();

		if( spec != nullptr ) {
			group = spec->getGroup();
		}
		else if( chrom != nullptr ) {
			group = chrom->getGroup();
		}
		else if( dataMap != nullptr ) {
			group = dataMap->getGroup();
		}
	}

	return group;
}

// get sample
SampleWrapper^ ClrMsDataVariant::getSample() {
	SampleWrapper^ sample = nullptr;

	if( m_var->type == ClrVariant::DataType::SAMPLE ) {	// sample
		sample = (SampleWrapper^)m_var->obj;
	}
	else {	// get sample from group
		DataGroupNodeWrapper^ group = getGroup();
		if( group != nullptr ) {
			sample = group->getSample();
		}
	}

	return sample;
}

// get sample set
SampleSetWrapper^ ClrMsDataVariant::getSampleSet() {
	SampleSetWrapper^ sampleSet = nullptr;

	if( m_var->type == ClrVariant::DataType::SAMPLE_SET ) {	// sample set
		sampleSet = (SampleSetWrapper^)m_var->obj;
	}
	else {	// get sample set from sample
		SampleWrapper^ sample = getSample();
		if( sample != nullptr ) {
			sampleSet = sample->getSampleSet();
		}
	}

	return sampleSet;
}

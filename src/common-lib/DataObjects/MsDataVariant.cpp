/**
 * @file MsDataVariant.cpp
 * @brief implements of MsDataVariant class
 *
 * @author S.Tanaka
 * @date 2008.12.24
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "MsDataVariant.h"

#include "Spectrum.h"
#include "Chromatogram.h"
#include "DataMapInfo.h"
#include "DataGroupNode.h"
#include "Sample.h"
#include "SampleSet.h"


using namespace kome::objects;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
MsDataVariant::MsDataVariant() {
}

// constructor
MsDataVariant::MsDataVariant( Variant& var ) : m_var( var ) {
}

// destructor
MsDataVariant::~MsDataVariant() {
}

// set variant object
void MsDataVariant::setVariant( Variant& var ) {
	m_var = var;
}

// get variant object
Variant& MsDataVariant::getVariant() {
	return m_var;
}

// unset object
void MsDataVariant::unsetObject() {
	m_var.type = Variant::OTHER;
	m_var.prim.pt = NULL;
}

// set spectrum
void MsDataVariant::setSpectrum( Spectrum* spec ) {
	if( spec == NULL ) {
		unsetObject();
	}

	m_var.type = Variant::SPECTRUM;
	m_var.prim.pt = spec;
}

// set chromatogram
void MsDataVariant::setChromatogram( Chromatogram* chrom ) {
	if( chrom == NULL ) {
		unsetObject();
	}

	m_var.type = Variant::CHROMATOGRAM;
	m_var.prim.pt = chrom;
}

// set data map
void MsDataVariant::setDataMap( DataMapInfo* dataMap ) {
	if( dataMap == NULL ) {
		unsetObject();
	}

	m_var.type = Variant::DATA_MAP;
	m_var.prim.pt = dataMap;
}

// set group
void MsDataVariant::setGroup( DataGroupNode* group ) {
	if( group == NULL ) {
		unsetObject();
	}

	m_var.type = Variant::DATA_GROUP_NODE;
	m_var.prim.pt = group;
}

// set sample
void MsDataVariant::setSample( Sample* sample ) {
	if( sample == NULL ) {
		unsetObject();
	}

	m_var.type = Variant::SAMPLE;
	m_var.prim.pt = sample;
}

// set sample set
void MsDataVariant::setSampleSet( SampleSet* sampleSet ) {
	if( sampleSet == NULL ) {
		unsetObject();
	}

	m_var.type = Variant::SAMPLE_SET;
	m_var.prim.pt = sampleSet;
}

// get spectrum
Spectrum* MsDataVariant::getSpectrum() {
	Spectrum* spec = NULL;
	if( m_var.type == Variant::SPECTRUM ) {
		spec = (Spectrum*)m_var.prim.pt;
	}

	return spec;
}

// get chromatogram
Chromatogram* MsDataVariant::getChromatogram() {
	Chromatogram* chrom = NULL;
	if( m_var.type == Variant::CHROMATOGRAM ) {
		chrom = (Chromatogram*)m_var.prim.pt;
	}

	return chrom;
}

// get data map
DataMapInfo* MsDataVariant::getDataMap() {
	DataMapInfo* dm = NULL;
	if( m_var.type == Variant::DATA_MAP ) {
		dm = (DataMapInfo*)m_var.prim.pt;
	}

	return dm;
}

// get group
DataGroupNode* MsDataVariant::getGroup() {
	DataGroupNode* group = NULL;

	if( m_var.type == Variant::DATA_GROUP_NODE ) {	// group
		group = (DataGroupNode*)m_var.prim.pt;
	}
	else {   // etc
		Spectrum* spec = getSpectrum();
		Chromatogram* chrom = getChromatogram();
		DataMapInfo* dm = getDataMap();

		if( spec != NULL ) {
			group = spec->getGroup();
		}
		else if( chrom != NULL ) {
			group = chrom->getGroup();
		}
		else if( dm != NULL ) {
			group = &( dm->getGroup() );
		}
	}

	return group;
}

// get sample
Sample* MsDataVariant::getSample() {
	Sample* sample = NULL;

	if( m_var.type == Variant::SAMPLE ) {	// sample
		sample = (Sample*)m_var.prim.pt;
	}
	else {	// get sample from group
		DataGroupNode* group = getGroup();
		if( group != NULL ) {
			sample = group->getSample();
		}
	}

	return sample;
}

// get sample set
SampleSet* MsDataVariant::getSampleSet() {
	SampleSet* sampleSet = NULL;

	if( m_var.type == Variant::SAMPLE_SET ) {	// sample set
		sampleSet = (SampleSet*)m_var.prim.pt;
	}
	else {	// get sample set from sample
		Sample* sample = getSample();
		if( sample != NULL ) {
			sampleSet = sample->getSampleSet();
		}
	}

	return sampleSet;
}

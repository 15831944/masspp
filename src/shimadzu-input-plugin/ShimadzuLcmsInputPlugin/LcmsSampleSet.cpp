/**
 * @file LcmsSampleSet.cpp
 * @brief implements of LcmsSampleSet class
 *
 * @author S.Tanaka
 * @date 2008.02.08
 * 
 * Copyright(C) 2006-2013 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "LcmsSampleSet.h"

#include "LcmsSample.h"


using namespace kome::io::lcms;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
LcmsSampleSet::LcmsSampleSet() {
	m_export = nullptr;
}

// destructor
LcmsSampleSet::~LcmsSampleSet() {
}

// get export object
TTFLDataExport::ITTFLDataExportEx^ LcmsSampleSet::getExport() {
	return m_export;
}

// on open file
bool LcmsSampleSet::onOpenFile( System::String^ path ) {	
	// logger
	kome::clr::ClrLogger^ logger = kome::clr::ClrLogger::getInstance();

	// export object
	if( m_export != nullptr ) {
		m_export->Close();
		m_export = nullptr;
	}

	// create object	
	m_export = gcnew TTFLDataExport::TTFLDataExportClass();
	m_export->Open( getFilePath() );

	// get sample information
	System::DateTime creationDate;
	System::String^ creationOperator = nullptr;
	System::DateTime modificationDate;
	System::String^ modificationOperator = nullptr;
	System::String^ sampleName = nullptr;
	System::String^ sampleId = nullptr;
	System::String^ trayName = nullptr;
	System::String^ vial = nullptr;
	System::String^ desc = nullptr;
	int plSize = int();
 
	m_export->GetSampleInfo(
		getFilePath(),
		creationDate,
		creationOperator,
		modificationDate,
		modificationOperator,
		sampleName,
		sampleId,
		trayName,
		vial
	);

	m_export->GetDescription( getFilePath(), plSize, desc );

	// sample object
	LcmsSample^ sample = gcnew LcmsSample( this );
	addSample( sample );

	// name
	sample->setName( sampleName );

	// properties
	sample->setCreationDate( %creationDate );
	sample->setCreationOperator( creationOperator );
	sample->setModificationDate( %modificationDate );
	sample->setModificationOperator( modificationOperator );
	sample->setSampleId( sampleId );
	sample->setTrayName( trayName );
	sample->setVial( vial );
	sample->setDesc( desc );

	return true;
}

// on close file
bool LcmsSampleSet::onCloseFile() {
	if( m_export != nullptr ) {
		m_export->Close();
		m_export = nullptr;
	}

	return true;
}

// on open sample
bool LcmsSampleSet::onOpenSample( kome::clr::SampleWrapper^ sample ) {	
	return true;
}

// on close sample
bool LcmsSampleSet::onCloseSample( kome::clr::SampleWrapper^ sample ) {
	return true;
}

/**
 * @file WatersSample.cpp
 * @brief implements of WatersSample class
 *
 * @author S.Tanaka
 * @date 2008.01.21
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "WatersSample.h"

#include "WatersSampleSet.h"
#include "WatersSpectrum.h"


using namespace kome::io::waters;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



#define INSTRUMENT_NAME	   "Q-TOF"
#define SOFTWARE_NAME		 "Masslynx"


// constructor
WatersSample::WatersSample( WatersSampleSet^ sampleSet, int function ) 
		: kome::clr::ClrSampleBase( sampleSet ) {
	m_sampleSet = sampleSet;
	m_func = function;

	setInstrument( INSTRUMENT_NAME );
	setSoftwareName( SOFTWARE_NAME );
	setParallelReadable( true );
}

// destructor
WatersSample::~WatersSample() {
}

// get sample set
WatersSampleSet^ WatersSample::getWatersSampleSet() {
	return m_sampleSet;
}

// get function number
int WatersSample::getFunction() {
	return m_func;
}

// set file properties
void WatersSample::setFileProperties( kome::clr::PropertiesWrapper^ props ) {
	// header object
	DACSERVERLib::DACHeader^ header = gcnew DACSERVERLib::DACHeader();
	header->GetHeader( m_sampleSet->getPath() );
	// set properties
	props->setValue( "Sample ID", header->SampleID );
	props->setValue( "Sample Description", header->SampleDesc );
	props->setValue( "Acquisition Name", header->AcquName );
	props->setValue( "Acquisition Date", header->AcquDate );
	props->setValue( "Acquisition Time", header->AcquTime );
	props->setIntValue( "Major Version", header->VersionMajor );
	props->setIntValue( "Minor Version", header->VersionMinor );
	props->setValue( "User Name", header->UserName );
	props->setValue( "Instrument Name", header->Instrument );
	props->setValue( "Instrument Type", header->InstrumentType );

	cli::array< float >^ offsets = (cli::array< float >^)header->AnalogOffset;
	for( int i = 0; i < offsets->Length; i++ ) {
		props->setDoubleValue( "Analog Offset " + i, (double)offsets[ i ] );
	}

	props->setIntValue( "Autosampler Type", header->AutosamplerType );
	props->setValue( "Bottle Number", header->BottleNumber );
	props->setValue( "Conditions", header->Conditions );
	props->setIntValue( "Encrypted", header->Encrypted );
	props->setValue( "Ionization Gas", header->GasName );
	props->setValue( "Job Code", header->JobCode );
	props->setValue( "Lab Name", header->LabName );
	props->setIntValue( "MUX Flag", header->MuxStream );
	props->setValue( "PEP File Name", header->PepFileName );
	props->setValue( "Plate Description", header->PlateDesc );
	props->setValue( "Process", header->Process );
	props->setIntValue( "Resolved", header->Resolved );
	props->setDoubleValue( "Solvent Delay", header->SolventDelay );
	props->setValue( "Submitter", header->Submitter );
	props->setValue( "Task Code", header->TaskCode );
}

// set function properties
void WatersSample::setFuncProperties( kome::clr::PropertiesWrapper^ props ) {
	// function object
	DACSERVERLib::DACFunctionInfoClass fun;
	fun.GetFunctionInfo( m_sampleSet->getPath(), m_func );

	// set properties
	props->setValue( "Function Type", fun.FunctionType );
	props->setDoubleValue( "Function Set Mass", fun.FunctionSetMass );
	props->setIntValue( "Function Number", m_func );
	props->setIntValue( "Process Number", 0 );
}

// on open sample
bool WatersSample::onOpenSample( kome::clr::DataGroupNodeWrapper^ rootGroup ) {

	using namespace System;
	using namespace System::IO;
	using namespace System::Collections;

	// logger
	kome::clr::ClrLogger^ logger = kome::clr::ClrLogger::getInstance();

	// set file properties
	kome::clr::PropertiesWrapper^ props = rootGroup->getProperties();
	setFileProperties( props );

	// set function properties
	setFuncProperties( props );

	// stage
	int stage = 1;
	System::String^ sampleName = getName();
	if( sampleName->IndexOf( "MSMSMS" ) >= 0 ) {  // MS3
		stage = 3;
	}
	else if( sampleName->IndexOf( "MSMS" ) >= 0
			|| sampleName->IndexOf( "Daughter" ) >= 0 ) { // MS2
		stage = 2;
	}

	// objects
	DACSERVERLib::DACFunctionInfoClass fun;
	DACSERVERLib::DACScanStatsClass scan;

	System::String^ extPath = m_sampleSet->getPath();
	extPath = System::String::Concat(extPath, "\\_extern.inf");
	int nPolarity = -1;
	if (System::IO::File::Exists(extPath)) {
		StreamReader^ objReader = gcnew StreamReader(extPath);
		String^ sLine = "";
		while((sLine = objReader->ReadLine()) != nullptr) {
			if (sLine->IndexOf("Polarity") >= 0) {
				int sLen = sLine->Length;
				if (sLine[sLen-1] == '+') {
					nPolarity = 10;
				} else if(sLine[sLen-1] == '-'){
					nPolarity = 1;
				} else {
					nPolarity = 0;
				}
				break;
			}
		}
		objReader->Close();
	}

	// scan
	fun.GetFunctionInfo( m_sampleSet->getPath(), m_func );
	int scanNum = fun.NumScans;
	logger->info( scanNum.ToString() + "Scans" );

	for( int i = 1; i <= scanNum; i++ ) {
		// scan info
		scan.GetScanStats( m_sampleSet->getPath(), m_func, 0, i );

		// spectrum object
		WatersSpectrum^ spec = gcnew WatersSpectrum( this, "Scan" + i.ToString() );
		rootGroup->addSpectrum( spec );

		// set properties
		spec->setScanNumber( i );
		spec->setMsStage( stage );
		spec->setRt( scan.RetnTime );
		spec->setXRange( (double)scan.LoMass, (double)scan.HiMass );
		spec->setTotalIntensity( scan.TIC );
		spec->setMaxIntensity( scan.BPI );
		spec->setBasePeakMass( scan.BPM );
		spec->setCentroidMode( false );

		// porality
		if (nPolarity > -1) {
			if (nPolarity == 10) {
				spec->setPolarity( kome::clr::SpectrumWrapper::Polarity::POSITIVE );
			} else if(nPolarity == 1) {
				spec->setPolarity( kome::clr::SpectrumWrapper::Polarity::NEGATIVE );
			} else {
				spec->setPolarity( kome::clr::SpectrumWrapper::Polarity::UNKNOWN);
			}
		}

		// precursor
		if( stage > 1 ) {
			DACSERVERLib::DACExScanStatsClass exScan;
			exScan.GetExScanStats( m_sampleSet->getPath(), m_func, 0, i );

			spec->setPrecursor( exScan.SetMass );
		}

		// title
		System::String^ title = spec->getName() + " rt=" + scan.RetnTime;
		if( stage > 1 ) {
			title += ( " precursor=" + spec->getPrecursor() );
		}
		spec->setTitle( title );
	}

	return true;
}

// on close sample
bool WatersSample::onCloseSample() {
	return true;
}

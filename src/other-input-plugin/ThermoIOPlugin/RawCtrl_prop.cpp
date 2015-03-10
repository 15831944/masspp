/**
 * @file RawCtrl_prop.cpp
 * @brief Thermo MSFileReader get Other Parameters to Props.
 *
 * @author M.Fukuda
 * @date 2012.04.16
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#include "stdafx.h"
#include "RawCtrl.h"

template<typename T> void setValue( PROPS& props, const char* key, T& val ) {
}

// Insert Props Overwrap
template<> void setValue( PROPS& props, const char* key, long& val ) {
	props.setIntValue(key, val);
	val = 0;
}

// Insert Props Overwrap
template<> void setValue( PROPS& props, const char* key, double& val ) {
	props.setDoubleValue(key, val);
	val = 0.0;
}

// Insert Props Overwrap
template<> void setValue( PROPS& props, const char* key, BSTR& val ) {
	_bstr_t orig(val);
	std::string tmp((char *)orig);
	props.setValue(key, tmp.c_str());
	SysFreeString(val);
	val = NULL;
}

const std::string BstrToString(BSTR& val) {
	_bstr_t orig(val);
	std::string tmp((char *)orig);
	SysFreeString(val);
	val = NULL;
	return tmp;
}

//const char* RawCtrl::getXRCtrlPropVal(HRESULT(*decorate)(BSTR*)) {
//	BSTR bsRst = NULL;
//	HRESULT hr = (*decorate)(&bsRst);
//	if (SUCCEEDED(hr)) {
//		return BstrToString(bsRst).c_str();
//	}
//	return NULL;
//}

//const char* RawCtrl::getInstrumentName() {
const STR RawCtrl::getInstrumentName() {
	BSTR bsRst = NULL;
	HRESULT hr = m_XRCtrl->GetInstName(&bsRst);
	if (SUCCEEDED(hr)) {
		return BstrToString(bsRst);
	}
	return "";
}

//const char* RawCtrl::getInstSoftwareVersion() {
const STR RawCtrl::getInstSoftwareVersion() {
	BSTR bsRst = NULL;
	HRESULT hr = m_XRCtrl->GetInstSoftwareVersion(&bsRst);
	if (SUCCEEDED(hr)) {
		return BstrToString(bsRst).c_str();
	}
	return "";
}

// Make Props
const bool RawCtrl::setSampleInfo( PROPS& props ) {

	BSTR bsRst = NULL;
	long nRst(0);
	double dRst(0.0);

	HRESULT hr = m_XRCtrl->GetCreatorID( &bsRst );
	setValue(props, "Creator ID", bsRst);

	hr = m_XRCtrl->GetVersionNumber( &nRst );
	setValue(props, "File Version", nRst );

	hr = m_XRCtrl->IsNewFile( &nRst );
	setValue(props, "Is NewFile", nRst);

	hr = m_XRCtrl->IsError( &nRst );
	setValue(props, "Is Error", nRst );

	hr = m_XRCtrl->GetErrorMessage( &bsRst );
	setValue(props, "Error Message", bsRst );

	hr = m_XRCtrl->GetWarningMessage( &bsRst );
	setValue(props, "Warning Message", bsRst );

	//--- Seq
	hr = m_XRCtrl->GetSeqRowNumber( &nRst );
	setValue(props, "Sequence Row Number", nRst );

	hr = m_XRCtrl->GetSeqRowSampleType( &nRst );
	setValue(props, "Sequence Row Sample Type", nRst );

	hr = m_XRCtrl->GetSeqRowRawFileName( &bsRst );
	setValue(props, "Sequence Row File Name", bsRst );
	
	hr = m_XRCtrl->GetSeqRowDataPath( &bsRst );
	setValue(props, "Sequence Row Data Path", bsRst );

	hr = m_XRCtrl->GetSeqRowSampleID( &bsRst );
	setValue(props, "Sequence Row Sample ID", bsRst );

	hr = m_XRCtrl->GetSeqRowSampleName( &bsRst );
	setValue(props, "Sequence Row Sample Name", bsRst );

	hr = m_XRCtrl->GetSeqRowComment( &bsRst );
	setValue(props, "Sequence Row Comment", bsRst );

	hr = m_XRCtrl->GetSeqRowLevelName( &bsRst );
	setValue(props, "Sequence Row Level Name", bsRst );

	for (long i = 0; i < 5; i++ ) {
		hr = m_XRCtrl->GetSeqRowUserLabel( i, &bsRst );
		std::string dmyStr = FMT("Sequence Row User Label[%d]", i);
		setValue(props,  dmyStr.c_str(), bsRst );
	}

	// Sequence Row Instrument Method
	hr = m_XRCtrl->GetSeqRowInstrumentMethod( &bsRst );
	setValue(props, "Sequence Row Instrument Method", bsRst );

	// Sequence Row Processing Method	
	hr = m_XRCtrl->GetSeqRowProcessingMethod( &bsRst );
	setValue(props, "Sequence Row Processing Method", bsRst );

	// Sequence Row Calibration File	
	hr = m_XRCtrl->GetSeqRowCalibrationFile( &bsRst );
	setValue(props, "Sequence Row Calibration File", bsRst );

	// Sequence Row Vial Position
	hr = m_XRCtrl->GetSeqRowVial( &bsRst );
	setValue(props, "Sequence Row Vial Position", bsRst );

	// Sequence Row Injection Volume
	hr = m_XRCtrl->GetSeqRowInjectionVolume( &dRst );
	setValue(props, "Sequence Row Injection Volume", dRst );

	// Sequence Row Sample Weight
	hr = m_XRCtrl->GetSeqRowSampleWeight( &dRst );
	setValue(props, "Sequence Row Sample Weight", dRst );

	// Sequence Row Sample Volume
	hr = m_XRCtrl->GetSeqRowSampleVolume( &dRst );
	setValue(props, "Sequence Row Sample Volume", dRst );

	// Sequence Row ISTD Amount
	hr = m_XRCtrl->GetSeqRowISTDAmount( &dRst );
	setValue(props, "Sequence Row ISTD Amount", dRst );

	// Sequence Row Dilution Factor
	hr = m_XRCtrl->GetSeqRowDilutionFactor( &dRst );
	setValue(props, "Sequence Row Dilution Factor", dRst );

	// File INFO
	// In Acquisition
	hr = m_XRCtrl->InAcquisition( &nRst );
	setValue(props, "In Acquisition", nRst );

		// Number of Spectra
	hr = m_XRCtrl->GetNumSpectra( &nRst );
	setValue(props, "Number of Spectra", nRst );

	// Number of Status Log Entries
	hr = m_XRCtrl->GetNumStatusLog( &nRst );
	setValue(props, "Number of Status Log Entries", nRst );

	// Number of Error Log Entries
	hr = m_XRCtrl->GetNumErrorLog( &nRst );
	setValue(props, "Number of Error Log Entries", nRst );

	// Number of Tune Data Entries
	hr = m_XRCtrl->GetNumTuneData( &nRst );
	setValue(props, "Number of Tune Data Entries", nRst );

	// Number of Trailer Extra Entries
	hr = m_XRCtrl->GetNumTrailerExtra( &nRst );
	setValue(props, "Number of Trailer Extra Entries", nRst );

	// Mass Resolution
	hr = m_XRCtrl->GetMassResolution( &dRst );
	setValue(props, "Mass Resolution", dRst );

	// Expected Run Time
	hr = m_XRCtrl->GetExpectedRunTime( &dRst );
	setValue(props, "Expected Run Time", dRst );

	// First Spectrum Number
	hr = m_XRCtrl->GetFirstSpectrumNumber( &nRst );
	setValue(props, "First Spectrum Number", nRst );

	// Last Spectrum Number
	hr = m_XRCtrl->GetLastSpectrumNumber( &nRst );
	setValue(props, "Last Spectrum Number", nRst );

	// Low Mass
	hr = m_XRCtrl->GetLowMass( &dRst );
	setValue(props, "Run Low Mass", dRst );

	// High Mass
	hr = m_XRCtrl->GetHighMass( &dRst );
	setValue(props, "Run High Mass", dRst );

	// Start Time
	hr = m_XRCtrl->GetStartTime( &dRst );
	setValue(props, "Start Time", dRst );

	// End Time
	hr = m_XRCtrl->GetEndTime( &dRst );
	setValue(props, "End Time", dRst );

	// Max Integrated Intensity
	hr = m_XRCtrl->GetMaxIntegratedIntensity( &dRst );
	setValue(props, "Max Integrated Intensity", dRst );

	// Max Intensity
	hr = m_XRCtrl->GetMaxIntensity( &nRst );
	setValue(props, "Max Intensity", nRst );

	// Instrument ID
	hr = m_XRCtrl->GetInstrumentID( &nRst );
	setValue(props, "Instrument ID", nRst );

	// Inlet ID
	hr = m_XRCtrl->GetInletID( &nRst );
	setValue(props, "Inlet ID", nRst );

	// Error Flag
	hr = m_XRCtrl->GetErrorFlag( &nRst );
	setValue(props, "Error Flag", nRst );

	// Sample Volume
	hr = m_XRCtrl->GetSampleVolume( &dRst );
	setValue(props, "Sample Volume", dRst );

	// Sample Amount
	hr = m_XRCtrl->GetSampleWeight( &dRst );
	setValue(props, "Sample Amount", dRst );

	// Vial Number
	hr = m_XRCtrl->GetVialNumber( &nRst );
	setValue(props, "Vial Number", nRst );

	// Injection Volume
	hr = m_XRCtrl->GetInjectionVolume( &dRst );
	setValue(props, "Injection Volume", dRst );

	// Flags
	hr = m_XRCtrl->GetFlags( &bsRst );
	setValue(props, "Flags", bsRst );

	// Acquisition File
	hr = m_XRCtrl->GetAcquisitionFileName( &bsRst );
	setValue(props, "Acquisition File", bsRst );

	// Instrument Description
	hr = m_XRCtrl->GetInstrumentDescription( &bsRst );
	setValue(props, "Instrument Description", bsRst );

	// Acquisition Date
	hr = m_XRCtrl->GetAcquisitionDate( &bsRst );
	setValue(props, "Acquisition Date", bsRst );

	// Operator
	hr = m_XRCtrl->GetOperator( &bsRst );
	setValue(props, "Operator", bsRst );

	// Comment 1
	hr = m_XRCtrl->GetComment1( &bsRst );
	setValue(props, "Comment 1", bsRst );

	// Comment 2
	hr = m_XRCtrl->GetComment2( &bsRst );
	setValue(props, "Comment 2", bsRst );

	// Sample Volume Units
	hr = m_XRCtrl->GetSampleVolumeUnits( &bsRst );
	setValue(props, "Sample Volume Units", bsRst );

	// Sample Amount Units
	hr = m_XRCtrl->GetSampleAmountUnits( &bsRst );
	setValue(props, "Sample Amount Units", bsRst );

	// Injection Volume Units
	hr = m_XRCtrl->GetInjectionAmountUnits( &bsRst );
	setValue(props, "Injection Volume Units", bsRst );

	// Instrument Name	
	hr = m_XRCtrl->GetInstName( &bsRst );
	setValue(props, "Instrument Name", bsRst );

	// Instrument Model	
	hr = m_XRCtrl->GetInstModel( &bsRst );
	setValue(props, "Instrument Model", bsRst );

	// Instrument Serial Number
	hr = m_XRCtrl->GetInstSerialNumber( &bsRst );
	setValue(props, "Instrument Serial Number", bsRst );

	// Instrument Hardware Rev
	hr = m_XRCtrl->GetInstHardwareVersion( &bsRst );
	setValue(props, "Instrument Hardware Version", bsRst );

	// Instrument Flags
	hr = m_XRCtrl->GetInstFlags( &bsRst );
	setValue(props, "Instrument Flags", bsRst );

	// Channel Labels
	long nNum = 0;
	hr = m_XRCtrl->GetInstNumChannelLabels( &nNum );
	for( long i = 0; i < nNum; i++ ) {
		hr = m_XRCtrl->GetInstChannelLabel( i, &bsRst );
		std::string sDmyStr = FMT("Channel %d Label", i );
		setValue(props, sDmyStr.c_str(), bsRst );
	}

	return true;
}


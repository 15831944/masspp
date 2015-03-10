/**
 * @file RawCtrl.cpp
 * @brief Thermo MSFileReader Control cpp file
 *
 * @author M.Fukuda
 * @date 2012.04.16
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#include "stdafx.h"
#include "RawCtrl.h"

// Data Point Struct
typedef struct _datapeak {
	double dX;         // Mass(M/Z) or RT
	double dIntensity; // Intensity
} DataPeak;

// Data Label Struct
typedef struct _datalabel {
	double datas[6]; //Mass, Intensity,Resonance, baseline, noise, charge
} DataLabel;

// Data Flag Struct
typedef struct _dataflag {
	unsigned char flags[6]; //Saturated, Flagmented, Merged, Exception, Reference, Modified
} DataFlag;

// Support Functions

// LPWSTRの形で、min-max のフォーマットになるように整形して返す。
LPWSTR range2LStr( double minX, double maxX ) {
	if ( minX < 0.0 || maxX < 0.0 ) {
		return NULL;
	}
	std::string sDmy = FMT("%lf-%lf",minX,maxX);
	size_t orgSize = sDmy.size() + 1;
	size_t dmySize;
	wchar_t* wsStr = new wchar_t[orgSize];
	mbstowcs_s( &dmySize, wsStr, orgSize, sDmy.c_str(), _TRUNCATE );
	return wsStr;
}

// Parse Getted Array
// varData に取得された値を XYDataに割り付ける
// varData, varFlagsに関してはここで解放される
void unPackArray( VARIANT& varData, VARIANT& varFlags, long nArraySize, kome::core::XYData* const xyData ) {
	if ( nArraySize ) {
		xyData->reserve( nArraySize );
		SAFEARRAY FAR* psa = varData.parray;
		DataPeak* pDataPeaks = NULL;
		SafeArrayAccessData(psa, (void**)(&pDataPeaks));
		for ( long i = 0; i < nArraySize; i++ ) {
			xyData->addPoint( pDataPeaks[i].dX, pDataPeaks[i].dIntensity );
		}
		SafeArrayUnaccessData( psa );
	}

	if ( varData.vt != VT_EMPTY ) {
		SAFEARRAY FAR* psa = varData.parray;
		varData.parray = NULL;
		SafeArrayDestroy(psa);
	}

	if ( varFlags.vt != VT_EMPTY ) {
		SAFEARRAY FAR* psa = varFlags.parray;
		varFlags.parray = NULL;
		SafeArrayDestroy(psa);
	}
}

// Constructor
RawCtrl::RawCtrl() : m_XRCtrl(NULL) {
	setupCOM();
}

// Destructor
RawCtrl::~RawCtrl(){
	m_XRCtrl.Release();
}

// Setup Com
const bool RawCtrl::setupCOM() {
	// Initialize COM interface
	HRESULT hr = m_XRCtrl.CreateInstance("MSFileReader.XRawfile.1");
	if (FAILED(hr)) {
		LOG_ERROR(FMT("Can not Read MSFileReader"));
		return false;
	}
	return true;
}

//-------------------------------------------------------
// File IO
//-------------------------------------------------------
const bool RawCtrl::openMyFile( const char* path ) {

	HRESULT hr = m_XRCtrl->Open(path);
	return JudgeRst(hr, FMT("Unable to open RAW file : %s", path));
}

const bool RawCtrl::closeMyFile() {
	m_XRCtrl->RefreshViewOfFile();
	m_XRCtrl->Close();
	return true;
}

// seacrh for SRM filter on Head Only
const bool RawCtrl::IsHaveSRMData( const long& nFstScanNum, bool* bFlag ) {
	STR dmyStr;
	bool ret = this->getFilterFromScanNum( nFstScanNum, dmyStr );
	*bFlag = (dmyStr.find("SRM") != std::string::npos);
	return ret;
}

const bool RawCtrl::IsCentroidScanForScanNum( const long& nScanNum, bool* bFlag) {
	long centMode(0);
	HRESULT hr = m_XRCtrl->IsCentroidScanForScanNum( nScanNum, &centMode );
	*bFlag = (centMode != 0);
	return SUCCEEDED(hr);
}

//---------------------------------------------------------
// Set Controls
//---------------------------------------------------------

const bool RawCtrl::SetCurrentController( long nType, long nCtrl ) {
	m_XRCtrl->SetCurrentController( nType, nCtrl );
	return true;
}

//---------------------------------------------------------
// Getters
//---------------------------------------------------------

const bool RawCtrl::getMSCtrlNum( long* ctrlNum ) {
	HRESULT hr = m_XRCtrl->GetNumberOfControllersOfType( 0, ctrlNum );
	return JudgeRst(hr, FMT("Error Getting number of ctrl of type MS"));
}

const bool RawCtrl::getResolution( double* dReso ) {
	HRESULT hr = m_XRCtrl->GetMassResolution(dReso);
	return SUCCEEDED(hr);
}

// get Spectrum Range
const bool RawCtrl::getSpecScanRange( long* nFst, long* nLast, long* nTotal) {
	long firstScanNumber = 0, lastScanNumber = 0;

	HRESULT hr = m_XRCtrl->GetFirstSpectrumNumber(nFst);
	if (FAILED(hr)) {
		fprintf(stdout, "ERROR: Unable to get first scan\n");
		return false;
	}
	m_XRCtrl->GetLastSpectrumNumber(nLast);
	*nTotal = (*nLast - *nFst) + 1;
	return true;
}

// get MSOrder as Stage
const bool RawCtrl::getMSOrder( const long& nScanNum, long* nMSOrder ) {
	HRESULT hr = m_XRCtrl->GetMSOrderForScanNum( nScanNum, nMSOrder );
	return SUCCEEDED(hr);
}

// get Precursor
const bool RawCtrl::getPrecursor( const long& nScanNum, long nMsOrder, double* dPrec ) {
	HRESULT hr = m_XRCtrl->GetPrecursorMassForScanNum( nScanNum, nMsOrder, dPrec );
	return SUCCEEDED(hr);
}

// get RT
const bool RawCtrl::getRTFromScanNum( const long& nScanNum, double* dRT ) {
	HRESULT hr = m_XRCtrl->RTFromScanNum( nScanNum, dRT );
	return SUCCEEDED(hr);
}

//---------------------------------------------------------
// Getters Filter
//---------------------------------------------------------
// get Filter
const bool RawCtrl::getFilterFromScanNum( const long& nScanNum, STR& sFilter ) {
	BSTR bsTmpStr = NULL;
	HRESULT hr = m_XRCtrl->GetFilterForScanNum( nScanNum, &bsTmpStr );
	if ( FAILED(hr) || bsTmpStr ==  NULL) {
		return false;
	}
	_bstr_t orig(bsTmpStr);
	STR basicstring((char *)orig);
	sFilter = basicstring;
	SysFreeString(bsTmpStr);
//	delete bsTmpStr;
	return true;
}

// get ALL Filters
const bool RawCtrl::getFiltes( std::vector<std::string>& sFilterArr ) {
	
	VARIANT varFilters;
	VariantInit(&varFilters);
	long nArraySize = 0;
	HRESULT hr = m_XRCtrl->GetFilters( &varFilters, &nArraySize );
	if (FAILED(hr) || !nArraySize || varFilters.vt != (VT_ARRAY | VT_BSTR ) ) {
		return false;
	}
	SAFEARRAY FAR* psa = varFilters.parray;
	varFilters.parray = NULL;
	BSTR* psbstrFilters = NULL;
	if ( FAILED(SafeArrayAccessData(psa, (void**)(&psbstrFilters)))) {
		SafeArrayUnaccessData(psa);
		SafeArrayDestroy(psa);
		return false;
	}
	for ( long i = 0; i < nArraySize; i++ ) {
		_bstr_t orig(psbstrFilters[i]);
		STR basicstring((char *)orig);
		sFilterArr.push_back(basicstring);
	}

	SafeArrayUnaccessData(psa);
	SafeArrayDestroy(psa);
	return true;
}

//---------------------------------------------------------
// Getters Spctrum
//---------------------------------------------------------
// get ScanNum's Header Info
const bool RawCtrl::getScanHeader(
			const long& nScanNum,   
			long* pnPackets,
			double* pdStartTime,   
			double* pdLowMass,
			double* pdHighMass,     
			double* pdTIC,
			double* pdBasePeakMass, 
			double* pdBasePeakIntensity,
			long* pnNumChannels,    
			long pbUniformTime,
			double* pdFrequency) 
{
	HRESULT hr = m_XRCtrl->GetScanHeaderInfoForScanNum( 
		nScanNum, pnPackets, pdStartTime,
		pdLowMass,pdHighMass,pdTIC,
		pdBasePeakMass,pdBasePeakIntensity, pnNumChannels, 
		&pbUniformTime, pdFrequency);
	return SUCCEEDED(hr);
}

// get Spectrum Datas
const bool RawCtrl::getSpecData(long nScanNum, XYDATA* const xyData) {

	LPCTSTR szFilter = NULL;			// No filter
	long nIntensityCutoffType = 0;		// No cutoff
	long nIntensityCutoffValue = 0;		// No cutoff
	long nMaxNumberOfPeaks = 0;			// 0 : return all data peaks
	bool bCentroidThisScan = false;		// No centroiding
	double dCentroidPeakWidth = 0;		// No centroiding

	VARIANT varMassList;
	VariantInit(&varMassList);
	VARIANT varPeakFlags;
	VariantInit(&varPeakFlags);
	long nArraySize(0);
	
	HRESULT hr = m_XRCtrl->GetMassListFromScanNum(
		&nScanNum,
		szFilter,
		nIntensityCutoffType,
		nIntensityCutoffValue,
		nMaxNumberOfPeaks,
		FALSE,
		&dCentroidPeakWidth,
		&varMassList,
		&varPeakFlags,
		&nArraySize);

	if (FAILED(hr)){ return false; }
	unPackArray( varMassList, varPeakFlags, nArraySize, xyData );

	return true;
}

// 範囲付きでXYDataを取得
const bool RawCtrl::getSpecDataWithRange(long nScanNum, const double dMinMz, const double dMaxMz,XYDATA* const xyData) {

	LPCTSTR szFilter = NULL;			// No filter
	long nIntensityCutoffType = 0;		// No cutoff
	long nIntensityCutoffValue = 0;		// No cutoff
	long nMaxNumberOfPeaks = 0;			// 0 : return all data peaks
	bool bCentroidThisScan = false;		// No centroiding
	double dCentroidPeakWidth = 0;		// No centroiding

	VARIANT varMassList;
	VariantInit(&varMassList);
	VARIANT varPeakFlags;
	VariantInit(&varPeakFlags);
	long nArraySize(0);	
	LPWSTR szMassRange1 = range2LStr(dMinMz, dMaxMz);

	HRESULT hr = m_XRCtrl->GetMassListRangeFromScanNum(
		&nScanNum,
		szFilter,
		nIntensityCutoffType,
		nIntensityCutoffValue,
		nMaxNumberOfPeaks,
		FALSE,
		&dCentroidPeakWidth,
		&varMassList,
		&varPeakFlags,
		szMassRange1,
		&nArraySize);

	delete szMassRange1;
	szMassRange1 = NULL;
	
	if (FAILED(hr)){ return false; }
	unPackArray( varMassList, varPeakFlags, nArraySize, xyData );

	return true;
}

// Thermo(MSFileReader)由来で取得できる Spectrumのピークデータを取得する。
const bool RawCtrl::getTgtScanLabelDatas(const long nScanNum) {
	long curScanN = nScanNum;
	VARIANT varLabels, *pvarLabels;
	VARIANT varFlags, *pvarFlags;
	pvarLabels = &varLabels;
	pvarFlags = &varFlags;

	HRESULT hr = m_XRCtrl->GetLabelData(pvarLabels, pvarFlags, &curScanN);

	if (FAILED(hr)) { return false;}
	SAFEARRAY FAR* psa = pvarLabels->parray;
	long lnum = psa->rgsabound[0].cElements;
	DataLabel* pDataLabels = NULL;
	SafeArrayAccessData(psa, (void**)(&pDataLabels));
	SafeArrayUnaccessData(psa);

	if (pvarFlags) {
		SAFEARRAY FAR* psa2 = pvarFlags->parray;
		DataFlag* pDataFlags = NULL;
		SafeArrayAccessData(psa2, (void**)(&pDataFlags));
		SafeArrayUnaccessData(psa2);

		ResizeLabelF_(lnum);
		for ( long i = 0; i < lnum; i++ ) {
			AddDataLabelF_(pDataLabels[i].datas, pDataFlags[i].flags);
		}
	}

	return true;
}

//---------------------------------------------------------
// Get Chrom Actions
//---------------------------------------------------------
const bool RawCtrl::getChromTIC( XYDATA* const xyData ) {
	long nChroType(1); // AS TIC
	return getChromData(nChroType, NULL, NULL, xyData);
}

const bool RawCtrl::getChromAllXIC(const char* filter, const char* szMassRange, XYDATA* const xyData) {
	long nChroType(0);
	return getChromData(nChroType, filter, szMassRange, xyData);
}

const bool RawCtrl::getChromData(
		const long& nChroType, 
		const char* filter, 
		const char* szMassRange,
		XYDATA* const xyData)
{

	SetCurrentController( 0, 1 ); // first MS controller

	long nChroOperator(0);
	long nChroType2(0);
	LPCTSTR szFilter = NULL;
	LPCTSTR szMassRanges1 = NULL;
	LPCTSTR szMassRanges2 = NULL;
	double dDelay = 0.0;
	double dStartTime = 0.0;
	double dEndTime = 0.0;
	long nSmoothingType(0);
	long nSmoothingValue(0);

	VARIANT varChroData;
	VariantInit(&varChroData);
	VARIANT varPeakFlags;
	VariantInit(&varPeakFlags);
	
	long nArraySize = 0;

	HRESULT hr = m_XRCtrl->GetChroData (
		nChroType,
		nChroOperator,
		nChroType2,
		filter,//szFilter,
		szMassRange,//szMassRanges1,
		szMassRanges2,
		dDelay,
		&dStartTime,
		&dEndTime,
		nSmoothingType,
		nSmoothingValue,
		&varChroData,
		&varPeakFlags,
		&nArraySize );

	if (FAILED(hr)){ return false; }
	unPackArray( varChroData, varPeakFlags, nArraySize, xyData );
	return true;
}

const bool RawCtrl::JudgeRst( HRESULT& hr, std::string& ErrMsg ) {
	if (FAILED(hr)) {
		LOG_ERROR(ErrMsg);
		return false;
	} else {
		return true;
	}
}

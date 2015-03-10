/**
 * @file RawCtrl.h
 * @brief Thermo MSFileReader Control header file
 *
 * @author M.Fukuda
 * @date 2012.04.16
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#ifndef __THERMO_LIBS__
#define __THERMO_LIBS__
	#ifdef WIN64
		#import "C:\Program Files\Thermo\MSFileReader\Xrawfile2_x64.dll"
	#else
		#import "C:\Program Files (x86)\Thermo\MSFileReader\Xrawfile2.dll"	
	#endif
#endif
#include <string>

// TypeDefs
typedef MSFileReaderLib::IXRawfile5Ptr XR_CTRL;
typedef std::string STR;
typedef std::vector<STR> vSTR;
typedef kome::core::XYData XYDATA;
typedef kome::core::Properties PROPS;

class RawCtrl {

public:
	RawCtrl();
	~RawCtrl();

public:
	/**
	 * @brief  Initialize COM interface("MSFileReader.XRawfile.1")
	 * @param  none.
	 * @return T/F create interface results
	 * @date 2012.04.16 M.Fukuda#Eisai Add
	 */
	const bool setupCOM();

	/**
	 * @brief  Open Raw File 
	 * @param  [in] char* file path
	 * @return T/F function results
	 * @date 2012.04.16 M.Fukuda#Eisai Add
	 */
	const bool openMyFile( const char* path );

	/**
	 * @brief  Close Raw File 
	 * @param  none.
	 * @return T/F function results
	 * @date 2012.04.16 M.Fukuda#Eisai Add
	 */
	const bool closeMyFile();

	/**
	 * @brief  Check Having SRM Data (Checking have Filter)
	 * @param  [in] Long& First scan number
	 * @param  [out] Bool having SRM, True.
	 * @return T/F function results
	 * @date 2012.04.16 M.Fukuda#Eisai Add
	 */
	const bool IsHaveSRMData( const long& nFstScanNum, bool* bFlag );

	/**
	 * @brief  Check and Set Is Centroid Scan
	 * @param  [in] Long& First scan number
	 * @param  [out] Bool Is CentroidMode, True.
	 * @return T/F function results
	 * @date 2012.04.16 M.Fukuda#Eisai Add
	 */
	const bool IsCentroidScanForScanNum( const long& nScanNum, bool* bIsCent);

	/**
	 * @brief  Set current controller to Thermo COM
	 * @param  [in] Long : controller type
	 * @param  [in] Long : controller number
	 * @return T/F function results
	 * @date 2012.04.16 M.Fukuda#Eisai Add
	 */
	const bool SetCurrentController( long nType, long nCtrl );

	/**
	 * @brief  get controller number (From COM)
	 * @param  [out] Long* : controller number
	 * @return T/F function results
	 * @date 2012.04.16 M.Fukuda#Eisai Add
	 */
	const bool getMSCtrlNum( long* ctrlNum );

	/**
	 * @brief  get Resolution (From keeping File)
	 * @param  [out] Long* : Resolution
	 * @return T/F function results
	 * @date 2012.04.16 M.Fukuda#Eisai Add
	 */
	const bool getResolution( double* dReso );

	/**
	 * @brief  get Sample Spectrm Scan number
	 * @param  [out] Long* : first scan number
	 * @param  [out] Long* : last scan number
	 * @param  [out] Long* : number of specta
	 * @return T/F function results
	 * @date 2012.04.16 M.Fukuda#Eisai Add
	 */
	const bool getSpecScanRange( long* nFst, long* nLast, long* nCnt);

	/**
	 * @brief  get MS Order (as Stage)
	 * @param  [in] Long* : target scan number
	 * @param  [out] Long* : MS Order
	 * @return T/F function results
	 * @date 2012.04.16 M.Fukuda#Eisai Add
	 */	
	const bool getMSOrder( const long& nScanNum, long* nMsOrder );

	/**
	 * @brief  get Precursor
	 * @param  [in] Long* : target scan number
	 * @param  [in] Long* : MS Order
	 * @param  [out] Long*: precursor (if nothing, -1.0);
	 * @return T/F function results
	 * @date 2012.04.16 M.Fukuda#Eisai Add
	 */
	const bool getPrecursor( const long& nScanNum, long nMsOrder, double* dPrec );

	/**
	 * @brief  get Retention Time
	 * @param  [in] Long* : target scan number
	 * @param  [out] Long*: Retention Time
	 * @return T/F function results
	 * @date 2012.04.16 M.Fukuda#Eisai Add
	 */
	const bool getRTFromScanNum( const long& nScanNum, double* dRT );

	/**
	 * @brief  get Filter
	 * @param  [in] Long* : target scan number
	 * @param  [out] String*: Filter
	 * @return T/F function results
	 * @date 2012.04.16 M.Fukuda#Eisai Add
	 */
	const bool getFilterFromScanNum( const long& nScanNum, STR& str );

	/**
	 * @brief  get all Filter
	 * @param  [out] Vector<String>& : all Filters
	 * @return T/F function results
	 * @date 2012.04.16 M.Fukuda#Eisai Add
	 */
	const bool getFiltes( vSTR& sFilterArr );

	/**
	 * @brief  get Spectrum header infomatrions
	 * @param  [in] Long* : target scan number
	 * @param  [out] Long*   : number of peaks
	 * @param  [out] double* : start time (as retention time)
	 * @param  [out] double* : low mass (minimun M/z)
	 * @param  [out] double* : high mass (maximun M/z)
	 * @param  [out] double* : total intensity
	 * @param  [out] double* : base peak mass
	 * @param  [out] double* : base peak intensity
	 * @param  [out] Long*   : numbur of channel (no use)
	 * @param  [out] Long*   : unifoem time (no use)
	 * @param  [out] double* : frequency (no use)
	 * @return T/F function results
	 * @date 2012.04.16 M.Fukuda#Eisai Add
	 */
	const bool getScanHeader( 
		const long& nScanNum,
		long* pnPeaks,
		double* pdStartTime,
		double* pdLowMass,
		double* pdHighMass,
		double* pdTIC,
		double* pdBasePeakMass,
		double* pdBasePaekIntensity,
		long* pnNumChannels,
		long pbUniformTime,
		double* pdFrequency);

	/**
	 * @brief  get Spectrum XY Point Datas
	 * @param  [in] Long : target scan number
	 * @param  [out] XYdata objects(kome::core::XYData)
	 * @return T/F function results
	 * @date 2012.04.16 M.Fukuda#Eisai Add
	 */
	const bool getSpecData(long nScanNum,  XYDATA* const xyData);

	/**
	 * @brief  get Spectrum XY Point Datas (Filterd Range)
	 * @param  [in] Long : target scan number
	 * @param  [in] double : range of start M/z
	 * @param  [in] double : range of end M/z
	 * @param  [out] XYdata objects(kome::core::XYData)
	 * @return T/F function results
	 * @note   if start ot end value less 0.0, get All Range (as getSpecData results)
	 * @date 2012.04.16 M.Fukuda#Eisai Add
	 */
	const bool getSpecDataWithRange(long nScanNum, const double dMinMz, const double dMaxMz, XYDATA* const xyData);

	const bool getTgtScanLabelDatas(const long nScanNum);

	/**
	 * @brief  get Chromatogram XY Point Datas (Filterd Range)
	 * @param  [out] XYdata objects(kome::core::XYData)
	 * @return T/F function results
	 * @date 2012.04.16 M.Fukuda#Eisai Add
	 */
	const bool getChromTIC(XYDATA* const xyData);

	/**
	 * @brief  get Chromatogram XY Point Datas (Filterd Range)
	 * @param  [in] Char* Filter
	 * @param  [in] Char* Mass Range
	 * @param  [out] XYdata objects(kome::core::)
	 * @return T/F function results
	 * @date 2012.04.16 M.Fukuda#Eisai Add
	 */
	const bool getChromAllXIC(const char* filter, const char* szMassRange, XYDATA* const xyData);

	/**
	 * @brief  set sample infomations
	 * @param  [out] Properties objects (kome::core::)
	 * @return T/F function results
	 * @note   write on prop.cpp
	 * @date 2012.04.16 M.Fukuda#Eisai Add
	 */
	const bool setSampleInfo(PROPS& props);
	const STR getInstrumentName();
	const STR getInstSoftwareVersion();

	template <typename RFunc> void set_ResizeLabelFunc(RFunc f) {ResizeLabelF_ = f;}
	template <typename AFunc> void set_AddDataLabelFunc(AFunc f) {AddDataLabelF_ = f;}
private:
//	const char* getXRCtrlPropVal(HRESULT (*decorate)(BSTR*));
	/**
	 * @brief  get Chromatogram XY Point Datas (Filterd Range)
	 * @param  [in] Long : Chromatogram Type(0:Mass Range, 1:TIC)
	 * @param  [in] Char* Filter
	 * @param  [in] Char* Mass Range
	 * @param  [out] XYdata objects(kome::core::)
	 * @return T/F function results
	 * @note   if other type of Raw File Extend it.
	 * @date 2012.04.16 M.Fukuda#Eisai Add
	 */
	const bool getChromData( 
		const long& nChroType, 
		const char* filter, 
		const char* szMassRange,
		XYDATA* const xyData);

private:

	/**
	 * @brief  Judge any functions Hresult
	 * @param  [in] HRESULT : return code
	 * @param  [in] String& : Error message
	 * @return T/F function results
	 * @note   If HRESULT is not Success, Call Log Error.
	 * @date 2012.04.16 M.Fukuda#Eisai Add
	 */	
	const bool JudgeRst( HRESULT& hr, std::string& ErrMsg );

	// My menbers
	XR_CTRL m_XRCtrl;

	boost::function<void(const unsigned int)> ResizeLabelF_;
	boost::function<void(const double[], const unsigned char[])> AddDataLabelF_;
};

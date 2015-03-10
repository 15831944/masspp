/**
 * @file ThermoSpectrum.h
 * @brief interfaces of Thermo Spectrum class header
 *
 * @author M.Fukuda
 * @date 2012.04.16
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#ifndef __KOME_IO_THERMO_SPECTRUM_H__
#define __KOME_IO_THERMO_SPECTRUM_H__

namespace kome {
namespace io {
namespace thermo {

class ThermoSample;
class ThermoSpectrum : public kome::objects::Spectrum {
private:

	// For XIC & BPC Data Struct
	struct stDataCtrl {
		double dIntensity;
		double dMzL;
		double dMzR;
	};

public:

	/**
	* @brief constructor
	* @param [in] ThermoSample objects : for matcing
	* @param [in] Char* : PC Name for (need specrum object)
	* @date 2012.04.16 M.Fukuda#Eisai Add
	*/
	ThermoSpectrum( ThermoSample* thermoSample, const char* pcName );

	virtual ~ThermoSpectrum();

	/**
	* @brief set spectrum status
	* @param [in] Long : scan number
	* @return none
	* @note  not get XYDatas.
	* @date 2012.04.16 M.Fukuda#Eisai Add
	*/
	void setSpecStatus(const long nSpecNum);

	const bool getVenderSpecturmPeakValues(kome::objects::Peaks* peaks);

protected:

	/**
	* @brief get XY Data Points(Use Range Filter)
	* @param [out] XYData (kome::core) XYData Set target.
	* @param [in] double : minimum Range M/Z
	* @param [in] double : maximum Range M/Z
	* @return if Faile get XYData, False.
	* @note either minX or maxX less Zero, get Full Range.
	* @date 2012.04.16 M.Fukuda#Eisai Add
	*/
	virtual void onGetXYData( kome::core::XYData* const xyData, const double minX, const double maxX );

	/**
	* @brief set Fileter Info to Spectrum object
	* @param [in] String : Filter string
	* @return none.
	* @date 2012.04.16 M.Fukuda#Eisai Add
	*/
	virtual void setFilter( const std::string& sFilter );

	/**
	* @brief get X Range (usuallry get M/z)
	* @param [out] Double : the pointer to store minimum x value
	* @param [out] Double : the pointer to store maximum x value
	* @return none.
	* @note This method is called by getMinX or getMaxX method. (abstract method)
	* @date 2012.04.16 M.Fukuda#Eisai Add
	*/
	virtual void onGetXRange( double* minX, double* maxX );

	/**
	* @brief get Total Intensity (Use Range Filter)
	* @param [in] double : minimum Range M/Z
	* @param [in] double : maximum Range M/Z
	* @return Dobule : having Total Intensity
	* @note call inner getXIC_BPC(this)
	* @note if call different range, reCalc Total Intensity.
	* @note This method is called by getTotalIntensity method. (abstract method)
	* @date 2012.04.16 M.Fukuda#Eisai Add
	*/
	virtual double onGetTotalIntensity( const double minX, const double maxX );

	/**
	* @brief get Max Intensity (Use Range Filter)
	* @param [in] double : minimum Range M/Z
	* @param [in] double : maximum Range M/Z
	* @return Dobule : having Max Intensity
	* @note call inner getXIC_BPC(this)
	* @note if call different range, reCalc Max Intensity.
	* @note This method is called by getMaxIntensity method. (abstract method)
	* @date 2012.04.16 M.Fukuda#Eisai Add
	*/
	virtual double onGetMaxIntensity( const double minX, const double maxX );

private:

	/**
	* @brief get Total/Max Intensity (Use Range Filter)
	* @param [in] double : minimum Range M/Z
	* @param [in] double : maximum Range M/Z
	* @param [in] stDataCtrl object(this) : XIC or BPC Data Struct
	* @param [in] Bool : if True, mean XIC mode.
	* @return Dobule : Total or Max Intensity (part of bXIC)
	* @note if call different range, Call reCalc.
	* @date 2012.04.16 M.Fukuda#Eisai Add
	*/
	const double getXIC_BPC( const double dMinX, const double dmaxX, const stDataCtrl& orgCtrl, bool bXIC );
	
	/**
	* @brief set Total/Max Intensity Status
	* @param [in] double : minimum Range M/Z
	* @param [in] double : maximum Range M/Z
	* @param [in] double : intensity
	* @param [in] Bool : if True, mean XIC mode.
	* @return none
	* @note set selected struct
	* @date 2012.04.16 M.Fukuda#Eisai Add
	*/	
	void setXICorBPC( const double dMzL, const double dMzR, const double dIntensity, bool bXic);

	/**
	* @brief ReCalc Total/Max Intensity (Use Range Filter)
	* @param [in] double : minimum Range M/Z
	* @param [in] double : maximum Range M/Z
	* @param [in] DataGroupNode (kome::objects) : for dataSearch Objects
	* @param [in] Bool : if True, mean XIC mode.
	* @return If Failed get New Points, False.
	* @date 2012.04.16 M.Fukuda#Eisai Add
	*/
	const bool ReCalcXICorBPC_DataWithRange( const double dMzL, const double dMzR, DGN* pDGN, bool bXic);

private:
	ThermoSample* m_sample;
	std::string m_sFilter;

	stDataCtrl m_stXic; //XICÇÃç\ë¢ëÃ
	stDataCtrl m_stBpc; //BPCÇÃç\ë¢ëÃ
};

} // end of NameSpace
}
}
#endif

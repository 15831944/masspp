/**
 * @file ThermoSample.h
 * @brief interfaces of ThermoSample class
 *
 * @author M.Fukuda
 * @date 2011.10.17
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#ifndef __KOME_IO_THERMO_SAMPLE_H__
#define __KOME_IO_THERMO_SAMPLE_H__

class RawCtrl;
namespace kome {
namespace io {
namespace thermo {

class ThermoSampleSet;

class ThermoSample : public kome::objects::Sample {

public:
	/**
 	 * @brief constructor
	 * @param [in] ThermoSampleSet object : for matching
	 * @param [in] Int controller type(From RawCtrl Object)
	 * @param [in] Int controller Number(From RawCtrl Object)
	 * @date 2012.04.16 M.Fukuda#Eisai Add
	 */
	ThermoSample( ThermoSampleSet* sampleSet, const int _type, const long _controller );

	virtual ~ThermoSample();

	/**
	 * @brief  get ThermoSampleSet object
	 * @param  none.
	 * @return ThermoSampleSet object
	 * @date 2012.04.16 M.Fukuda#Eisai Add
	*/
	ThermoSampleSet* getThermoSampleSet();

	/**
	 * @brief  get RawCtrl objects
	 * @param  none.
	 * @return RawCtrl objects
	 * @date 2012.04.16 M.Fukuda#Eisai Add
 	 */
	RawCtrl* getRawCtrl();

	/**
	 * @brief  get RawCtrl
	 * @param [out] RawCtrl object
	 * @return if success set params, True.
	 * @note get My RawCtrl, and set Current control my Status.
	 * @date 2012.04.16 M.Fukuda#Eisai Add
	*/
	const bool getRawCtrlwithMyTypes(RawCtrl* pRawCtrl_);

protected:
	/**
	* @brief open my Sample
	* @param [out] DataGroupNode (kome::objects)
	* @return If true, it succeeded to open this sample.
	* @note This method is called by openTreatment method. (override method)
	* @date 2012.04.16 M.Fukuda#Eisai Add
	*/
	virtual bool onOpenSample( kome::objects::DataGroupNode* rootGroup );

	/**
	* @brief clost my Sample
	* @return If true, it succeeded to close this sample.
	* @note This method is called by closeTreatment method. (override method)
	* @date 2012.04.16 M.Fukuda#Eisai Add
	*/
	virtual bool onCloseSample();

	virtual void onDetectPeaksByAPI( kome::objects::Spectrum* spec, kome::objects::Peaks* peaks );

private:

	/**
	* @brief Create ThermoChromatogram Objects
	* @param [in] RawCtrl Object
	* @param [out] DataGroupNode (kome::objects)
	* @return If Falied Creation, False (Now Fix True)
	* @date 2012.04.16 M.Fukuda#Eisai Add
	*/
	const bool createChrom(RawCtrl* rawCtrl, DGN* rootGroup);

	/**
	* @brief Create Spectrum Objects continuity
	* @param [in] RawCtrl Object
	* @param [in] Create Start Scan number
	* @param [in] Create End Scan number
	* @param [out] DataGroupNode (kome::objects)
	* @return If Falied Creation, False (Now Fix True)
	* @date 2012.04.16 M.Fukuda#Eisai Add
	*/
	const bool createSpec(RawCtrl* rawCtrl, const long nFstScan, const long nLastScan, DGN* rootGroup);

	// My Members -------------------
	ThermoSampleSet* m_sampleSet; // SampleSet
	int m_type; // Type ID
	long m_controller; // Contoler ID			
	std::map<long, std::string> sCtrlMap; // Control Map

};
}
}
}

#endif		// __KOME_IO_Thermo_SAMPLE_H__

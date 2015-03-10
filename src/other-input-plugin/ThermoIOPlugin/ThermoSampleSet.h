/**
 * @file ThermoSampleSetSampleSet.h
 * @brief interfaces of ThermoSampleSet class
 *
 * @author M.Fukuda
 * @date 2011.10.14
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#ifndef __KOME_IO_ThermoSampleSet_SAMPLE_SET_H__
#define __KOME_IO_ThermoSampleSet_SAMPLE_SET_H__

class RawCtrl;

namespace kome {
namespace io {
namespace thermo {

/**
* memo. SampleSet = File
* File(SampleSet) have Any Samples (usually has only one)
* so, File not equal Sample.
**/

class ThermoSampleSet : public kome::objects::SampleSet {
public:

	/**
	* @brief constructor
	* @note Create RawCtrl Object here (use new)
	* @date 2012.04.16 M.Fukuda#Eisai Add
	*/
	ThermoSampleSet();

	/**
	* @brief destructor
	* @note Delete RawCtrl Object here (use delete)
	* @date 2012.04.16 M.Fukuda#Eisai Add
	*/
	virtual ~ThermoSampleSet();

	/**
	* @brief get RawCtrl
	* @param none
	* @return my RawCtrl object
	* @date 2012.04.16 M.Fukuda#Eisai Add
	*/
	RawCtrl* getRawCtrl();

	/**
	* @brief Opem Sample
	* @param [in] Sample object(kome::objects) : open target sample
	* @return Fix True.
	* @note This method is called by openTreatment method. (override method)
	* @note No use now.
	* @date 2012.04.16 M.Fukuda#Eisai Add
	*/
	virtual bool onOpenSample( kome::objects::Sample* sample );

	/**
	* @brief Close Sample
	* @param [in] Sample object(kome::objects) close target sample
	* @return Fix True.
	* @note This method is called by closeTreatment method. (override method)
	* @note Check other sample is open same file, not any, Call onCloseRawFile
	* @date 2012.04.16 M.Fukuda#Eisai Add
	*/	
	virtual bool onCloseSample( kome::objects::Sample* sample );

protected:

	/**
	* @brief check can open target Raw File.
	* @param [in] Char : Sample Path
	* @return T/F if success open file, True.
	* @note This method is called by openFile method. (override method)
	* @note check and add sampleObject file types.
	* @date 2012.04.16 M.Fukuda#Eisai Add
	*/	
	virtual bool onOpenFile( const char* path );

	/**
	* @brief Close Sample File
	* @param [in] Sample object(kome::objects) : close target
	* @return Fix True.
	* @note This method is called by closeTreatment method. (override method)
	* @note inner Call onCloseRawFile
	* @date 2012.04.16 M.Fukuda#Eisai Add
	*/	
	virtual bool onCloseFile();

private:

	/**
	* @brief check can open target Raw File.(for Thermo)
	* @param [in] Char : File Path
	* @return T/F if success open file, True.
	* @note if open success, keep open flag.
	* @date 2012.04.16 M.Fukuda#Eisai Add
	*/
	bool onOpenRawFile( const char* path );

	/**
	* @brief close Raw File
	* @param [in] Char : File Path
	* @return Fix True
	* @note open flag to close.
	* @date 2012.04.16 M.Fukuda#Eisai Add
	*/
	bool onCloseRawFile();

	// My Members
	bool b_opend;
	RawCtrl* m_RawCtrl;

};

} // End of Namespace
}
}


#endif		// __KOME_IO_Thermo_SAMPLE_SET_H__

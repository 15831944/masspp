/**
 * @file ThermoChromatogram.h
 * @brief Thermo MSFileReader Chromatogram Objects header file
 *
 * @author M.Fukuda
 * @date 2012.04.16
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#ifndef __KOME_IO_THERMO_CHROMATOGRAM_H__
#define __KOME_IO_THERMO_CHROMATOGRAM_H__

namespace kome {
namespace io {
namespace thermo {

class ThermoSample;
			
class ThermoChromatogram : public kome::objects::Chromatogram {
public:

	/**
	 * @brief Constructor
	 * @param [in] ThermoSample Object
	 * @date 2012.04.16 M.Fukuda#Eisai Add
	 */
	ThermoChromatogram( ThermoSample* thermoSample );

	virtual ~ThermoChromatogram();

	/**
	 * @brief set MRM Status
	 * @param [in] Int : Range Number (Like a auto Index)
	 * @param [in] double : Q1
	 * @param [in] String : Q3 Range String Ex)[294.5-295.5]
	 * @return none
	 * @date 2012.04.16 M.Fukuda#Eisai Add
	 */
	void setMRMStatus( const int rangeX, const double& q1, const std::string& q3Str );

	/**
	 * @brief set Filter infomation
	 * @param [in] String& : Filter
	 * @return none
	 * @date 2012.04.16 M.Fukuda#Eisai Add
	 */
	void setFilter( const std::string& filter );

protected:

	/**
	 * @brief get XY Data Points
	 * @param [out] XYData (kome::core)
	 * @return none
	 * @note This method is called by getXYData method (override method)
	 * @date 2012.04.16 M.Fukuda#Eisai Add
	 */
	virtual void onGetXYData( kome::core::XYData* const xyData );
				
	/**
	 * @brief get Mass
	 * @param [out] DataSet (kome::objects)
	 * @param [in] double : Retenstion Time of Statr
	 * @param [in] double : Retenstion Time of End
	 * @param [in] SearchType (kome::objects) : start search
	 * @param [in] SearchType (kome::objects) : end search
	 * @return none
	 * @note This method is called by getSpectra method (override method)
	 * @date 2012.04.16 M.Fukuda#Eisai Add
	 */
	virtual void onGetSpectra(
		kome::objects::DataSet& dataSet,
		const double startRt,
		const double endRt,
		const kome::objects::SearchType startSearch,
		const kome::objects::SearchType endSearch
		){};

	/**
	 * @brief get Mass
	 * @param [in] UInt : index data index
	 * @return mass (fix 0.0)
	 * @note This method is called by getMass method (override method)
	 * @date 2012.04.16 M.Fukuda#Eisai Add
	 */
	virtual double onGetMass( const unsigned int index ){return 0.0;}

	/**
  	 * @brief get Ms Stage
	 * @param [in] Uint : index data index
	 * @return MS stage.(fix -1)
	 * @note This method is called by getMsStage method (override method)
	 * @date 2012.04.16 M.Fukuda#Eisai Add	 
	 */
	virtual int onGetMsStage( const unsigned int index ){ return -1;}

	/**
	 * @brief get precursor
	 * @param [in] Uint : index data index
	 * @return precursor mass (fix 0.0)
	 * @note virtual double onGetPrecursor( const unsigned int index )
	 * @date 2012.04.16 M.Fukuda#Eisai Add
	*/
	virtual double onGetPrecursor( const unsigned int index ){ return 0.0;}

	// My Members
	std::string mzRange;
	std::string m_filter;
	double m_mass;			// mass
	double m_precursor; 	// precursor
	ThermoSample* m_sample;
};

} // end of nameSpace
}
}

#endif

/**
 * @file DefaultDataManager.h
 * @brief interfaces of DefaultDataManager class
 *
 * @author S.Tanaka
 * @date 2008.10.23
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_OBJECTS_DEFAULT_DATA_MANAGER_H__
#define __KOME_OBJECTS_DEFAULT_DATA_MANAGER_H__


#include "DataManager.h"


namespace kome {
	namespace objects {

		class Sample;
		class Spectrum;
		class Chromatogram;
		class DataMapInfo;

		/**
		 * @class DefaultDataManager
		 * @brief default data manager
		 */
		class DATA_OBJECTS_CLASS DefaultDataManager : public DataManager {
		protected:
			/**
			 * @fn DefaultDataManager()
			 * @brief constructor
			 */
			DefaultDataManager();

			/**
			 * @fn virtual ~DefaultDataManager()
			 * @brief destructor
			 */
			virtual ~DefaultDataManager();

		protected:
			/**
			 * @fn virtual void onOpenSample( Sample* sample ) 
			 * @brief This method is called when a sample is opened. (override method)
			 * @param[in] sample sample object to be opened
			 */
			virtual void onOpenSample( Sample* sample ) ;

			/**
			 * @fn virtual void onCloseSample( Sample* sample, const bool deleting ) 
			 * @brief This method is called when a sample is closed. (override method)
			 * @param[in] sample sample object to be closed
			 * @param[in] deleting If true, the specified object is being deleted now.
			 */
			virtual void onCloseSample( Sample* sample, const bool deleting ) ;

			/**
			 * @fn virtual void onOpenSpectrum( Spectrum* spec ) 
			 * @brief This method is called when a spectrum is opened. (override method)
			 * @param[in] spec spectrum object to be opened
			 */
			virtual void onOpenSpectrum( Spectrum* spec ) ;

			/**
			 * @fn virtual void onCloseSpectrum( Spectrum* spec, const bool deleting ) 
			 * @brief This method is called when a spectrum is closed. (override method)
			 * @param[in] spec spectrum object to be opened
			 * @param[in] deleting If true, the specified object is being deleted now.
			 */
			virtual void onCloseSpectrum( Spectrum* spec, const bool deleting ) ;

			/**
			 * @fn virtual void onOpenChromatogram( Chromatogram* chrom ) 
			 * @brief This method is called when a chromatogram is opened. (override method)
			 * @param[in] chrom chromatogram object to be opened
			 */
			virtual void onOpenChromatogram( Chromatogram* chrom ) ;

			/**
			 * @fn virtual void onCloseChromatogram( Chromatogram* chrom, const bool deleting ) 
			 * @brief This method is called when a chromatogram is closed. (override method)
			 * @param[in] chrom chromatogram object to be closed
			 * @param[in] deleting If true, the specified object is being deleted now.
			 */
			virtual void onCloseChromatogram( Chromatogram* chrom, const bool deleting ) ;

			/**
			 * @fn virtual void onOpenDataMap( DataMapInfo* dataMap ) 
			 * @brief This method is called when a data map is opened. (override method)
			 * @param[in] dataMap data map object to be opened
			 */
			virtual void onOpenDataMap( DataMapInfo* dataMap ) ;

			/**
			 * @fn virtual void onCloseDataMap( DataMapInfo* dataMap, const bool deleting ) 
			 * @brief This method is called when a data map is closed. (override method)
			 * @param[in] dataMap data map object to be closed
			 * @param[in] deleting If true, the specified object is being deleted now.
			 */
			virtual void onCloseDataMap( DataMapInfo* dataMap, const bool deleting ) ;
		};
	}
}

#endif	// __KOME_OBJECTS_DEFAULT_DATA_MANAGER_H__


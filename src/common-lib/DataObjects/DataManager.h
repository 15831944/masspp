/**
 * @file DataManager.h
 * @brief interfaces of DataManager class
 *
 * @author S.Tanaka
 * @date 2006.10.04
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_OBJECTS_DATA_MANAGER_H__
#define __KOME_OBJECTS_DATA_MANAGER_H__


#include <vector>


namespace kome {
	namespace objects {

		class Sample;
		class Spectrum;
		class Chromatogram;
		class DataMapInfo;

		/**
		 * @class DataManager
		 * @brief data management abstract class
		 */
		class DATA_OBJECTS_CLASS DataManager {
		protected:
			/**
			 * @fn DataManager()
			 * @brief constructor
			 */
			DataManager();

			/**
			 * @fn virtual ~DataManager()
			 * @brief destructor
			 */
			virtual ~DataManager();

		protected:
			/** order */
			int m_order;

		protected:
			/** data manager set */
			static std::vector< DataManager* > m_dataMgrs;

			/** sorted flag */
			static bool m_sorted;

		protected:
			/**
			 * @fn virtual void onOpenSample( Sample* sample ) = 0
			 * @brief This method is called when a sample is opened. (abstract method)
			 * @param[in] sample sample object to be opened
			 */
			virtual void onOpenSample( Sample* sample ) = 0;

			/**
			 * @fn virtual void onCloseSample( Sample* sample, const bool deleting ) = 0
			 * @brief This method is called when a sample is closed. (abstract method)
			 * @param[in] sample sample object to be closed
			 * @param[in] deleting If true, the specified object is being deleted now.
			 */
			virtual void onCloseSample( Sample* sample, const bool deleting ) = 0;

			/**
			 * @fn virtual void onOpenSpectrum( Spectrum* spec ) = 0
			 * @brief This method is called when a spectrum is opened. (abstract method)
			 * @param[in] spec spectrum object to be opened
			 */
			virtual void onOpenSpectrum( Spectrum* spec ) = 0;

			/**
			 * @fn virtual void onCloseSpectrum( Spectrum* spec, const bool deleting ) = 0
			 * @brief This method is called when a spectrum is closed. (abstract method)
			 * @param[in] spec spectrum object to be opened
			 * @param[in] deleting If true, the specified object is being deleted now.
			 */
			virtual void onCloseSpectrum( Spectrum* spec, const bool deleting ) = 0;

			/**
			 * @fn virtual void onOpenChromatogram( Chromatogram* chrom ) = 0
			 * @brief This method is called when a chromatogram is opened. (abstract method)
			 * @param[in] chrom chromatogram object to be opened
			 */
			virtual void onOpenChromatogram( Chromatogram* chrom ) = 0;

			/**
			 * @fn virtual void onCloseChromatogram( Chromatogram* chrom, const bool deleting ) = 0
			 * @brief This method is called when a chromatogram is closed. (abstract method)
			 * @param[in] chrom chromatogram object to be closed
			 * @param[in] deleting If true, the specified object is being deleted now.
			 */
			virtual void onCloseChromatogram( Chromatogram* chrom, const bool deleting ) = 0;

			/**
			 * @fn virtual void onOpenDataMap( DataMapInfo* dataMap ) = 0
			 * @brief This method is called when a data map is opened. (abstract method)
			 * @param[in] dataMap data map object to be opened
			 */
			virtual void onOpenDataMap( DataMapInfo* dataMap ) = 0;

			/**
			 * @fn virtual void onCloseDataMap( DataMapInfo* dataMap, const bool deleting ) = 0
			 * @brief This method is called when a data map is closed. (abstract method)
			 * @param[in] dataMap data map object to be closed
			 * @param[in] deleting If true, the specified object is being deleted now.
			 */
			virtual void onCloseDataMap( DataMapInfo* dataMap, const bool deleting ) = 0;

		public:
			/**
			 * @fn static void openSample( Sample* sample )
			 * @brief This method is called when a sample is opened.
			 * @param[in] sample sample object to be opened
			 */
			static void openSample( Sample* sample );

			/**
			 * @fn static void closeSample( Sample* sample, const bool deleting )
			 * @brief This method is called when a sample is closed.
			 * @param[in] sample sample object to be closed
			 * @param[in] deleting If true, the specified object is being deleted now.
			 */
			static void closeSample( Sample* sample, const bool deleting );

			/**
			 * @fn static void openSpectrum( Spectrum* spec )
			 * @brief This method is called when a spectrum is opened.
			 * @param[in] spec spectrum object to be opened
			 */
			static void openSpectrum( Spectrum* spec );

			/**
			 * @fn static void closeSpectrum( Spectrum* spec, const bool deleting )
			 * @brief This method is called when a spectrum is closed.
			 * @param[in] spec spectrum object to be closed
			 * @param[in] deleting If true, the specified object is being deleted now.
			 */
			static void closeSpectrum( Spectrum* spec, const bool deleting );

			/**
			 * @fn static void openChromatogram( Chromatogram* chrom )
			 * @brief This method is called when a chromaogram is opened.
			 * @param[in] chrom chromatogram object to be opened
			 */
			static void openChromatogram( Chromatogram* chrom );

			/**
			 * @fn static void closeChromatogram( Chromatogram* chrom, const bool deleting )
			 * @brief This method is called when a chromatogram is closed.
			 * @param[in] chrom chromatogram object to be closed
			 * @param[in] deleting If true, the specified object is being deleted now.
			 */
			static void closeChromatogram( Chromatogram* chrom, const bool deleting );

			/**
			 * @fn static void openDataMap( DataMapInfo* dataMap )
			 * @brief This method is called when a data map is opened.
			 * @param[in] dataMap data map object to be opened
			 */
			static void openDataMap( DataMapInfo* dataMap );

			/**
			 * @fn static void closeDataMap( DataMapInfo* dataMap, const bool deleting )
			 * @brief This method is called when a data map is closed.
			 * @param[in] dataMap data map object to be closed
			 * @param[in] deleting If true, the specified object is being deleted now.
			 */
			static void closeDataMap( DataMapInfo* dataMap, const bool deleting );

		protected:
			/**
			 * @fn static bool lessManager( DataManager* dm0, DataManager* dm1 )
			 * @brief conmapred data managers to sort
			 * @param[in] dm0 data manager object to be compared
			 * @param[in] dm1 data manager object
			 */
			static bool lessManager( DataManager* dm0, DataManager* dm1 );

			/**
			 * @fn static void sortDataManagers()
			 * @brief sorts data managers
			 */
			static void sortDataManagers();
		};
	}
}

#endif	// __KOME_OBJECTS_DATA_MANAGER_H__


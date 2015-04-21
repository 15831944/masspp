/**
 * @file DataManagerWrapper.h
 * @brief interfaces of DataManagerWrapper class
 *
 * @author S.Tanaka
 * @date 2006.11.09
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_CLR_DATA_MANAGER_WRAPPER_H__
#define __KOME_CLR_DATA_MANAGER_WRAPPER_H__


namespace kome { 
	namespace clr {

		ref class SampleWrapper;
		ref class SpectrumWrapper;
		ref class ChromatogramWrapper;
		ref class DataMapInfoWrapper;

		/**
		 * @class DataManagerWrapper
		 * @brief data manager wrapper class to use on CLR environment
		 */
		public ref class DataManagerWrapper {
		public:
			/**
			 * @fn DataManagerWrapper( kome::objects::DataManager& dataMgr )
			 * @brief constructor
			 * @param[in] dataMgr data manager object
			 */
			DataManagerWrapper( kome::objects::DataManager& dataMgr );

			/**
			 * @fn virtual ~DataManagerWrapper()
			 * @brief destructor
			 */
			virtual ~DataManagerWrapper();

		protected:
			/** data manager object */
			kome::objects::DataManager& m_dataMgr;

		public:
			/**
			 * @fn kome::objects::DataManager& getDataManager()
			 * @brief gets data manager object
			 * @return data manager object
			 */
			kome::objects::DataManager& getDataManager();

		public:
			/**
			 * @fn static void openSample( SampleWrapper^ sample )
			 * @brief This method is called when a sample is opened.
			 * @param[in,out] sample sample object to be opened
			 */
			static void openSample( SampleWrapper^ sample );

			/**
			 * @fn static void closeSample( SampleWrapper^ sample, bool deleting )
			 * @brief This method is called when a sample is closed.
			 * @param[in,out] sample sample object to be closed
			 * @param[in] deleting If true, the specified object is being deleted now.
			 */
			static void closeSample( SampleWrapper^ sample, bool deleting );

			/**
			 * @fn static void openSpectrum( SpectrumWrapper^ spec )
			 * @brief This method is called when a spectrum is opened.
			 * @param[in,out] spec spectrum object to be opened
			 */
			static void openSpectrum( SpectrumWrapper^ spec );

			/**
			 * @fn static void closeSpectrum( SpectrumWrapper^ spec, bool deleting )
			 * @brief This method is called when a spectrum is closed.
			 * @param[in,out] spec spectrum object to be closed
			 * @param[in] deleting If true, the specified object is being deleted now.
			 */
			static void closeSpectrum( SpectrumWrapper^ spec, bool deleting );

			/**
			 * @fn static void openChromatogram( ChromatogramWrapper^ chrom )
			 * @brief This method is called when a chromaogram is opened.
			 * @param[in,out] chrom chromatogram object to be opened
			 */
			static void openChromatogram( ChromatogramWrapper^ chrom );

			/**
			 * @fn static void closeChromatogram( ChromatogramWrapper^ chrom, bool deleting )
			 * @brief This method is called when a chromatogram is closed.
			 * @param[in,out] chrom chromatogram object to be closed
			 * @param[in] deleting If true, the specified object is being deleted now.
			 */
			static void closeChromatogram( ChromatogramWrapper^ chrom, bool deleting );

			/**
			 * @fn static void openDataMap( DataMapInfoWrapper^ dataMap )
			 * @brief This method is called when a data map is opened.
			 * @param[in,out] dataMap data map object to be opened
			 */
			static void openDataMap( DataMapInfoWrapper^ dataMap );

			/**
			 * @fn static void closeDataMap( DataMapInfoWrapper^ dataMap, bool deleting )
			 * @brief This method is called when a data map is closed.
			 * @param[in,out] dataMap data map object to be closed
			 * @param[in] deleting If true, the specified object is being deleted now.
			 */
			static void closeDataMap( DataMapInfoWrapper^ dataMap, bool deleting );
		};
	}
}

#endif	// __KOME_CLR_DATA_MANAGER_WRAPPER_H__

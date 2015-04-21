/**
 * @file ClrDataManager.h
 * @brief interfaces of ClrDataManager class
 *
 * @author S.Tanaka
 * @date 2007.12.28
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_CLR_DATA_MANAGER_H__
#define __KOME_CLR_DATA_MANAGER_H__


namespace kome {
	namespace clr {

		ref class ClrDataManagerBase;

		/**
		 * @class ClrDataManager
		 * @brief DataManager class that has managed class object
		 */
		class ClrDataManager : public kome::objects::DataManager {
		public:
			/**
			 * @fn ClrDataManager()
			 * @brief constructor
			 */
			ClrDataManager();

			/**
			 * @fn virtual ~ClrDataManager()
			 * @brief destructor
			 */
			virtual ~ClrDataManager();

		protected:
			/** base spectrum object */
			gcroot< ClrDataManagerBase^ > m_baseDataManager;

		public:
			/**
			 * @fn void setBaseDataManager( ClrDataManagerBase^ baseDataManager )
			 * @brief sets base spectrum object
			 * @param[in] baseDataManager base spectrum object
			 */
			void setBaseDataManager( ClrDataManagerBase^ baseDataManager );

			/**
			 * @fn ClrDataManagerBase^ getBaseDataManager()
			 * @brief gets base spectrum object
			 * @return base spectrum object
			 */
			ClrDataManagerBase^ getBaseDataManager();

		protected:
			/**
			 * @fn virtual void onOpenSample( kome::objects::Sample* sample )
			 * @brief This method is called when a sample is opened. (override method)
			 * @param[in,out] sample sample object to be opened
			 */
			virtual void onOpenSample( kome::objects::Sample* sample );

			/**
			 * @fn virtual void onCloseSample( kome::objects::Sample* sample, const bool deleting )
			 * @brief This method is called when a sample is closed. (override method)
			 * @param[in,out] sample sample object to be closed
			 * @param[in] deleting If true, the specified object is being deleted now.
			 */
			virtual void onCloseSample( kome::objects::Sample* sample, const bool deleting );

			/**
			 * @fn virtual void onOpenSpectrum( kome::objects::Spectrum* spec )
			 * @brief This method is called when a spectrum is opened. (override method)
			 * @param[in,out] spec spectrum object to be opened
			 */
			virtual void onOpenSpectrum( kome::objects::Spectrum* spec );

			/**
			 * @fn virtual void onCloseSpectrum( kome::objects::Spectrum* spec, const bool deleting )
			 * @brief This method is called when a spectrum is closed. (override method)
			 * @param[in,out] spec spectrum object to be opened
			 * @param[in] deleting If true, the specified object is being deleted now.
			 */
			virtual void onCloseSpectrum( kome::objects::Spectrum* spec, const bool deleting );

			/**
			 * @fn virtual void onOpenChromatogram( kome::objects::Chromatogram* chrom )
			 * @brief This method is called when a chromatogram is opened. (override method)
			 * @param[in,out] chrom chromatogram object to be opened
			 */
			virtual void onOpenChromatogram( kome::objects::Chromatogram* chrom );

			/**
			 * @fn virtual void onCloseChromatogram( kome::objects::Chromatogram* chrom, const bool deleting )
			 * @brief This method is called when a chromatogram is closed. (override method)
			 * @param[in,out] chrom chromatogram object to be closed
			 * @param[in] deleting If true, the specified object is being deleted now.
			 */
			virtual void onCloseChromatogram( kome::objects::Chromatogram* chrom, const bool deleting );

			/**
			 * @fn virtual void onOpenDataMap( kome::objects::DataMapInfo* dataMap )
			 * @brief This method is called when a data map is opened. (override method)
			 * @param[in,out] dataMap data map object to be opened
			 */
			virtual void onOpenDataMap( kome::objects::DataMapInfo* dataMap );

			/**
			 * @fn virtual void onCloseDataMap( kome::objects::DataMapInfo* dataMap, const bool deleting )
			 * @brief This method is called when a data map is closed. (override method)
			 * @param[in,out] dataMap data map object to be closed
			 * @param[in] deleting If true, the specified object is being deleted now.
			 */
			virtual void onCloseDataMap( kome::objects::DataMapInfo* dataMap, const bool deleting );

		};
	}
}

#endif	// __KOME_CLR_DATA_MANAGER_H__

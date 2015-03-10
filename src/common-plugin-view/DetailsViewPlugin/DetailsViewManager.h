/**
 * @file DetailsViewManager.h
 * @brief interfaces of DetailsViewManager class
 *
 * @author S.Tanaka
 * @date 2007.02.13
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_VIEW_DETAILS_VIEW_MANAGER_H__
#define __KOME_VIEW_DETAILS_VIEW_MANAGER_H__


namespace kome {
	namespace view {

		class DetailsListCtrl;

		/**
		 * @class DetailsViewManager
		 * @brief details view management class
		 */
		class DetailsViewManager : public kome::objects::DefaultDataManager {
		protected:
			/**
			 * @fn DetailsViewManager()
			 * @brief constructor
			 */
			DetailsViewManager();

			/**
			 * @fn virtual ~DetailsViewManager()
			 * @brief destructor
			 */
			virtual ~DetailsViewManager();

		protected:
			/** details view */
			DetailsListCtrl* m_details;

			/** current group */
			kome::objects::DataGroupNode* m_currentGroup;

			/** current spectrum */
			kome::objects::Spectrum* m_currentSpectrum;

			/** current chromatogram */
			kome::objects::Chromatogram* m_currentChromatogram;

		public:
			/**
			 * @fn void createPropertiesView()
			 * @brief creates file details view
			 */
			void createPropertiesView();

			/**
			 * @fn void selectGroup( kome::objects::DataGroupNode* group )
			 * @brief selects spectrum group
			 * @param group selected spectrum group object
			 */
			void selectGroup( kome::objects::DataGroupNode* group );

			/**
			 * @fn void selectChrom( kome::objects::Chromatogram* chrom )
			 * @brief selects chromatogram
			 * @param chrom selected chromatogram object
			 */
			void selectChrom( kome::objects::Chromatogram* chrom );

			/**
			 * @fn void selectSpec( kome::objects::Spectrum* spectrum )
			 * @brief selects spectrum
			 * @param spectrum selected spectrum object
			 */
			void selectSpec( kome::objects::Spectrum* spectrum );

			/**
			 * @fn void updateSpectrum( kome::objects::Spectrum* spectrum )
			 * @brief updates spectrum
			 */
			void updateSpectrum( kome::objects::Spectrum* spectrum );

			/**
			 * @fn void updateChromatogram( kome::objects::Chromatogram* chromatogram )
			 * @brief updates chromatogram
			 */
			void updateChromatogram( kome::objects::Chromatogram* chromatogram );

			/**
			 * @fn void togglePropertiesVisibility()
			 * @brief toggles the visibility of the file details view
			 */
			void togglePropertiesVisibility();

			/**
			 * @fn bool isVisiblePropertiesView()
			 * @brief checks the visibility of the file details view
			 * @return If true, the file details view is visible.
			 */
			bool isVisiblePropertiesView();

			/**
			 * @fn void releaseView()
			 * @brief releases view
			 */
			void releaseView();

		protected:
			/**
			 * @fn virtual void onCloseSample( kome::objects::Sample* sample, const bool deleting )
			 * @brief This method is called when a sample is closed. (override method)
			 * @param sample sample object to be closed
			 * @param[in] deleting If true, the specified object is being deleted now.
			 */
			virtual void onCloseSample( kome::objects::Sample* sample, const bool deleting );

		public:
			/**
			 * @fn static DetailsViewManager& getInstance()
			 * @brief gets details view management object
			 * @return details view management object (This is the only object.)
			 */
			static DetailsViewManager& getInstance();
		};
	}
}


#endif		// __KOME_VIEW_DETAILS_VIEW_MANAGER_H__

/**
 * @file SpectraGridViewManager.h
 * @brief interfaces of SpectraGridViewManager class
 *
 * @author S.Tanaka
 * @date 2007.02.15
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#ifndef __KOME_VIEW_SPECTRA_GRID_VIEW_MANAGER_H__
#define __KOME_VIEW_SPECTRA_GRID_VIEW_MANAGER_H__


namespace kome {
	namespace view {

		class SpectraListCtrl;

		/**
		 * @class SpectraGridViewManager
		 * @brief spectra view management class
		 */
		class SpectraGridViewManager : public kome::objects::DefaultDataManager {
		protected:
			/**
			 * @fn SpectraGridViewManager()
			 * @brief constructor
			 */
			SpectraGridViewManager();

			/**
			 * @fn virtual ~SpectraGridViewManager()
			 * @brief destructor
			 */
			virtual ~SpectraGridViewManager();

		protected:
			/** spectra view */
			SpectraListCtrl* m_dataSetView;

			/** current sample */
			kome::objects::Sample* m_currentSample;

		public:
			/**
			 * @fn void createSpectraGridView()
			 * @brief creates spectra view
			 */
			void createSpectraGridView();

			/**
			 * @fn void selectSample( kome::objects::Sample* sample )
			 * @brief selects sample
			 * @param group selected sample
			 */
			void selectSample( kome::objects::Sample* sample );

			/**
			 * @fn kome::objects::Sasmple* getCurrentSample()
			 * @brief gets current sample
			 * @return current samle
			 */
			kome::objects::Sample* getCurrentSample();

			/**
			 * @fn void updateView( kome::objects::DataSet& dataSet )
			 * @brief updates spectra view
			 * @param spectra updated spectra
			 */
			void updateView( kome::objects::DataSet& dataSet );

			/**
			 * @fn void toggleSpectraGridViewVisibility()
			 * @brief toggles the visibility of the spectra view
			 */
			void toggleSpectraGridViewVisibility();

			/**
			 * @fn bool isVisibleSpectraGridView()
			 * @brief checks the visibility of the spectra view
			 * @return If true, the spectra view is visible.
			 */
			bool isVisibleSpectraGridView();

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
			 * @fn static SpectraGridViewManager& getInstance()
			 * @brief gets spectra view management object
			 * @return spectra view management object (This is the only object.)
			 */
			static SpectraGridViewManager& getInstance();
		};
	}
}


#endif		// __KOME_VIEW_SPECTRA_VIEW_MANAGER_H__

/**
 * @file DetailsViewManager.h
 * @brief interfaces of DetailsViewManager class
 *
 * @author H.Parry
 * @date 2011.04.20
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation. All rights reserved.
 */


#ifndef __KOME_VIEW_DETAILS_VIEW_MANAGER_H__
#define __KOME_VIEW_DETAILS_VIEW_MANAGER_H__


namespace kome {
	namespace properties {

		class DetailsListCtrl;
		class EditSampleUserPropertiesDialog;

		/**
		 * @class DetailsViewManager
		 * @brief details view management class
		 */
		class DetailsViewManager : public kome::objects::DefaultDataManager {
		public:
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
			void createPropertiesView(EditSampleUserPropertiesDialog* pParentDlg);

			/**
			 * @fn DetailsListCtrl* getListCtrl()
			 * @brief gets list ctrl
			 * @return the pointer to list ctrl
			 */
			DetailsListCtrl* getListCtrl();

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
			 * @fn void deleteItems()
			 * @brief deletes items
			 */
			void deleteItems();

			/**
			 * @fn void newItem()
			 * @brief adds a new item
			 */
			void newItem();

			/**
			 * @fn void updateItem()
			 * @brief updates an item
			 */
			void updateItem(const long lRow, const int iColumn, const char *s);

			/**
			 * @fn void updateSpectrum( kome::objects::Spectrum* spectrum )
			 * @brief updates spectrum
			 */
			void updateSpectrum( kome::objects::Spectrum* spectrum );

			/**
			 * @fn void updateItem()
			 * @brief saves spectrum user properties
			 */
			void saveSpectrumUserProperties( kome::objects::Spectrum* spectrum );

			/**
			 * @fn void updateItem()
			 * @brief saves chromatogram user properties
			 */
			void saveChromatogramUserProperties( kome::objects::Chromatogram* chromatogram );

			/**
			 * @fn void updateItem()
			 * @brief saves group user properties
			 */
			void saveGroupUserProperties( kome::objects::DataGroupNode* group );

		};
	}
}


#endif		// __KOME_VIEW_DETAILS_VIEW_MANAGER_H__

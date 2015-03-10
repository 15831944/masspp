/**
 * @file SpectraSearchDialog.h
 * @brief interfaces of SpectraSearchDialog class
 *
 * @author S.Tanaka
 * @date 2007.02.18
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_SEARCH_SPECTRA_SEARCH_DIALOG_H__
#define __KOME_SEARCH_SPECTRA_SEARCH_DIALOG_H__


#include "SpectraSearchFilter.h"
#include "SpectraSearchManager.h"

#include <wx/wx.h>
#include <wx/listctrl.h>


namespace kome {
	namespace search {

		/**
		 * @class SpectraSearchDialog
		 * @brief spectra search dialog class
		 */
		class SpectraSearchDialog : public kome::window::ParametersAndHelpDialog {
		public:
			/**
			 * @fn SpectraSearchDialog( wxWindow* parent, kome::objects::DataGroupNode* group )
			 * @brief constructor
			 * @param parent parent window
			 * @param[in] group group
			 */
			SpectraSearchDialog( wxWindow* parent, kome::objects::DataGroupNode* group );

			/**
			 * @fn SpectraSearchDialog( wxWindow* parent, kome::objects::DataSet* dataset )
			 * @brief constructor
			 * @param parent parent window
			 * @param[in] dataset dataset
			 * @param[in] bCallFlg
			 */
			SpectraSearchDialog( wxWindow* parent, kome::objects::DataSet* dataset, kome::objects::DataSet& dsp, bool bCallFlg );

			/**
			 * @fn virtual ~SpectraSearchDialog()
			 * @brief destructor
			 */
			virtual ~SpectraSearchDialog();

		protected:
			/** settings */
			kome::objects::SettingParameterValues m_settings;

			/** search type */
			SpectraSearchManager::SearchType m_type;

			/** filter */
			SpectraSearchFilter m_filter;

			/** params */
			kome::plugin::ParameterSettings m_params;

			/** group */
			kome::objects::DataGroupNode* m_group;

			/** data set object */
			kome::objects::DataSet* m_dataSet;
			/** data set object Œ‹‰Ê•Û‘¶—p */
			kome::objects::DataSet m_ds;

			/** call tyep "filter_spectra" flag */
			bool m_bCallFlg;
						
			/** operation descript */
			std::string m_strOptDescript;
			
			/** string range */
			std::string m_strRange;

		protected:
			/** search radio box */
			wxRadioBox* m_searchRadioBox;

			/** properties key text control */
			wxTextCtrl* m_propKeyText;

			/** properties value text control */
			wxTextCtrl* m_propValueText;

			/** properties restriction combo box */
			wxComboBox* m_propCombo;

			/** properties list */
			wxListBox* m_propList;

		public:
			/**
			 * @fn SpectraSearchFilter& getFilter()
			 * @brief gets spectra search filter
			 * @return spectra search filter
			 */
			SpectraSearchFilter& getFilter();

			/**
			 * @fn SpectraSearchManager::SearchType getType()
			 * @brief gets search type
			 * @return search type
			 */
			SpectraSearchManager::SearchType getType();
	
		protected:
			/**
			 * @fn virtual wxSizer* createMainSizer()
			 * @brief creates main sizer
			 * @return main sizer
			 */
			virtual wxSizer* createMainSizer();

			/**
			 * @fn virtual wxSizer* createButtonSizer()
			 * @brief creates button sizer
			 * @return button sizer
			 */
			virtual wxSizer* createButtonSizer();

		protected:
			/**
			 * @fn wxSizer* createSpectrumSizer()
			 * @brief creates spectrum sizer
			 * @return spectrum sizer
			 */
			wxSizer* createSpectrumSizer();

			/**
			 * @fn wxSizer* createPropertiesSizer()
			 * @brief creates properties sizer
			 * @return properties sizer
			 */
			wxSizer* createPropertiesSizer();
		
		protected:
			/**
			 * @fn void onAddProp( wxCommandEvent& evt )
			 * @brief This method is called when property filter "Add" button is clicked.
			 * @param evt command event
			 */
			void onAddProp( wxCommandEvent& evt );

			/**
			 * @fn void onDeleteProp( wxCommandEvent& evt )
			 * @brief This method is called when property filter "Delete" button is clicked.
			 * @param evt command event
			 */
			void onDeleteProp( wxCommandEvent& evt );

			/**
			 * @fn void onSearch( wxCommandEvent& evt )
			 * @brief This method is called when "Search" button is clicked.
			 * @param evt command event
			 */
			void onSearch( wxCommandEvent& evt );

		protected:
			/**
			 * @fn virtual bool TransferDataFromWindow()
			 * @brief transfer window control data to value
			 * @return If a transfer failed, this method returns false.
			 */
			virtual bool TransferDataFromWindow();

		private:
			DECLARE_EVENT_TABLE();
		};
	}
}

#endif		// __KOME_SEARCH_SPECTRA_SEARCH_DIALOG_H__

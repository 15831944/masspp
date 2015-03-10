/**
 * @file AlignmentDialog.h
 * @brief interfaces of AlignmentDialog class
 *
 * @author S.Tanaka
 * @date 2007.08.20
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_ALIGNMENT_DIALOG_H__
#define __KOME_ALIGNMENT_DIALOG_H__


#include <vector>

#include <wx/wx.h>
#include <wx/notebook.h>


namespace kome {
	namespace alignment {

		class PanelBase;

		class AlignmentHelpGetterPanel;

		/**
		 * @class AlignmentDialog
		 * @brief alignment subtract dialog
		 */
		class AlignmentDialog : public kome::window::ParametersAndHelpDialog {
		public:
			/**
			 * @fn AlignmentDialog( wxWindow* parent, kome::objects::SettingParameterValues* settings )
			 * @brief constructor
			 * @param parent parent window
			 * @param settings setting parameter values
			 */
			AlignmentDialog( wxWindow* parent, kome::objects::SettingParameterValues* settings );

			/**
			 * @fn virtual ~AlignmentDialog()
			 * @brief destructor
			 */
			virtual ~AlignmentDialog();

		protected:
			/** note book */
			wxNotebook* m_book;

			AlignmentHelpGetterPanel* m_helpGetterPanel;
			///** control group combobox */
			//wxComboBox* m_ctrlGrpCombo;

			///** control chromatogram combobox */
			//wxComboBox* m_ctrlChromCombo;

			///** treatment group combobox */
			//wxComboBox* m_trmtGrpCombo;

			///** treatment chromatogram combobox */
			//wxComboBox* m_trmtChromCombo;

		protected:
			/** settings */
			kome::objects::SettingParameterValues m_settings;

			/** parameters */
			kome::plugin::ParameterSettings m_params;

		protected:
			/** panels */
			std::vector< PanelBase* > m_panels;

		public:
			/**
			 * @fn kome::objects::SettingParameterValues& getParameters()
			 * @brief gets parameters
			 * @return parameters object
			 */
			kome::objects::SettingParameterValues& getParameters();

		protected:
			/**
			 * @fn virtual wxSizer* createMainSizer()
			 * @brief creates main sizer (override method)
			 * @return main sizer
			 */
			virtual wxSizer* createMainSizer();

			/**
			 * @fn virtual wxSizer* createButtonSizer()
			 * @brief creates button sizer (override method)
			 * @return button sizer
			 */
			virtual wxSizer* createButtonSizer();

		protected:
			///**
			// * @fn wxSizer* createChromSizer()
			// * @brief creates chromatogram sizer
			// * @return chromatogram sizer
			// */
			//wxSizer* createChromSizer();

			/**
			 * @fn wxSizer* createCorrectionSizer()
			 * @brief creates correction sizer
			 * @return correction sizer
			 */
			wxSizer* createCorrectionSizerScaling();     //  @date 2013/3/13 <Add> FUJITA

			/**
			 * @fn wxSizer* createCorrectionSizer()
			 * @brief creates correction sizer
			 * @return correction sizer
			 */
			wxSizer* createCorrectionSizerAlignment();

		

		protected:
			/**
			 * @fn void initNote()
			 * @brief initializes note book
			 */
			void initNote();

			///**
			// * @fn void updateChromCombos();
			// * @brief updates chromatogram combo boxes
			// */
			//void updateChromCombos();

			///**
			// * @fn void selectChromatograms()
			// * @brief selects chromatograms
			// */
			//void selectChromatograms();

		protected:
			/**
			 * @fn void startAlignment( wxCommandEvent& evt )
			 * @brief starts alignment
			 * @param evt event information
			 */
			void startAlignment( wxCommandEvent& evt );

			/**
			 * @fn void onGroupCombo( wxCommandEvent& evt )
			 * @brief This method is called when group combo selection is changed
			 * @param evt event information
			 */
			void onGroupCombo( wxCommandEvent& evt );

			/**
			 * @fn void onChromCombo( wxCommandEvent& evt )
			 * @brief This method is called when chromatogram combo seleciton is changed
			 * @param evt event information
			 */
			void onChromCombo( wxCommandEvent& evt );
		
		// @date 2011.02.07 <Add> M.Izumi ->
		protected:
			/**
			 * @fn virtual bool TransferDataFromWindow()
			 * @brief transfer window control data to value
			 * @return If a transfer failed, this method returns false.
			 */
			virtual bool TransferDataFromWindow();

			/**
			 * @fn void onOK( wxCommandEvent& evt )
			 * @brief This method is called when the OK button is clicked.
			 * @param evt button event
			 */
			void onOK( wxCommandEvent& evt );
		// <-
		private:
			DECLARE_EVENT_TABLE();
		};

		// --------------------------------------------------------------
		//  class Alignment HelpGetter Panel
		// --------------------------------------------------------------
		class AlignmentHelpGetterPanel : public kome::window::HelpGetterPanel {

		public:
			/**
			 * @fn
			 * @brief
			 */
			AlignmentHelpGetterPanel( wxWindow* parent );

			/**
			 * @fn
			 * @brief
			 */
			virtual ~AlignmentHelpGetterPanel();

		protected:
			/** control group combobox */
			wxComboBox* m_ctrlGrpCombo;

			/** control chromatogram combobox */
			wxComboBox* m_ctrlChromCombo;

			/** treatment group combobox */
			wxComboBox* m_trmtGrpCombo;

			/** treatment chromatogram combobox */
			wxComboBox* m_trmtChromCombo;
		protected:
			/**
			 * @fn virtual wxSizer* createMainSizer()
			 * @brief creates main sizer (override method)
			 * @return main sizer
			 */
			virtual wxSizer* createMainSizer();

			/**
			 * @fn virtual std::string getDescription( wxWindow* wnd )
			 * @brief method to get the description from a prameter control
			 * @param[in] wnd parameter control
			 * @return description
			 */
			virtual std::string getDescription( wxWindow* wnd );
		protected:
			/**
			 * @fn wxSizer* createChromSizer()
			 * @brief creates chromatogram sizer
			 * @return chromatogram sizer
			 */
			wxSizer* createChromSizer();

		protected:
			/**
			 * @fn wxComboBox* createGroupCombo( const int id )
			 * @brief creates group combo box
			 * @param id combo box ID
			 * @return group combo box
			 */
			wxComboBox* createGroupCombo( const int id );

	
		protected:
			/**
			 * @fn void updateChromCombos();
			 * @brief updates chromatogram combo boxes
			 */
			void updateChromCombos();

			/**
			 * @fn void selectChromatograms()
			 * @brief selects chromatograms
			 */
			void selectChromatograms();
		

		public:
			kome::objects::DataGroupNode* getSelGroup( const int id );
			kome::objects::Chromatogram* getSelChrom( const int id );
			
			bool ctrlGrpIsEnable(){				
				return	( m_ctrlGrpCombo != NULL ? m_ctrlGrpCombo->IsEnabled() : false );
			}
			/**
			 * @fn void onGroupCombo( wxCommandEvent& evt )
			 * @brief This method is called when group combo selection is changed
			 * @param evt event information
			 */
			void onGroupCombo( wxCommandEvent& evt );

			/**
			 * @fn void onChromCombo( wxCommandEvent& evt )
			 * @brief This method is called when chromatogram combo seleciton is changed
			 * @param evt event information
			 */
			void onChromCombo( wxCommandEvent& evt );
		private:
			DECLARE_EVENT_TABLE();
		};
	}
}

#endif		// __KOME_ALIGNMENT_DIALOG_H__

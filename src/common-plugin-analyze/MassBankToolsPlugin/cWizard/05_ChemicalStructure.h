/**
 * @file 05_..
 * @brief MassBank's create records
 *
 * @author M.Fukuda
 * @date 2013.02.20
 *
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#pragma once

namespace kome {
	namespace massbank {
		namespace wizard {

			// -- Forward declaration --
			class RecordElements;
			class ExtendSettingsPanel;
			class DBAccClass;
			//-----------------------------------------------
			//  Wizard Element Page
			//-----------------------------------------------
			class ChemicalStructureWizardPage : public kome::window::StandardWizardPage {

			public:
				ChemicalStructureWizardPage(wxWizard* wizParent, RecordElements*& rcElem);

				// destrouctor
				~ChemicalStructureWizardPage(){}

			protected:
				/**
				 * @fn virtual wxSizer* createMainSizer()
				 * @brief creates main sizer
				 * @return main sizer
				 */
				virtual wxSizer* createMainSizer();

			private:

				kome::objects::SettingParameterValues inSettings_;
				RecordElements* m_rcElem;

				DECLARE_EVENT_TABLE();
			};

			//-----------------------------------------------
			//  Form ParametersAndHelp
			//-----------------------------------------------
			class ChemicalStructurePanel : public kome::window::ParametersAndHelpPanel {


			public:
				ChemicalStructurePanel(
					wxWindow* parent, 
					RecordElements*& rcElem,
					kome::objects::SettingParameterValues& inSettings);
				
				~ChemicalStructurePanel();

			private:
				//** parameter definitions */
				kome::plugin::ParameterSettings inParams_;
				//** parameter values */
				kome::objects::SettingParameterValues& inSettings_;

				// My Record Elements Keepper
				RecordElements* m_rcElem;

				// DBAccƒNƒ‰ƒX‚Ì shared pointer
				std::shared_ptr<DBAccClass> dbsPtr_;

				/** button */
				wxButton* fileButton_;
				wxButton* linkButton_;
				wxButton* searchButton_;

				ExtendSettingsPanel* namePanel_;
				ExtendSettingsPanel* dbLinkPanel_;
				ExtendSettingsPanel* chemicalPanel_;

				bool linkUniqueChkFg_;

			protected:
				// -- inherited class methos.
				virtual wxSizer* createMainSizer();
				wxGridSizer* createButtons();

				virtual bool TransferDataFromWindow();

				// Events -----------
				// onReflesh My Panel
				void onSettingsPanel(kome::window::SettingsPanelEvent& evt);

				// on Clicked Button Event
				void onFileButtonClick(wxCommandEvent& evt);
				void onLinkButtonClick(wxCommandEvent& evt);
				void onSearchButtonClick(wxCommandEvent& evt);

				// on clicked DB Link, Change Event enables
				void onCheckDBLink();
				void onCheckNamePanel();

				// get & Check inner Values
				const bool onCheckDBLinkPanleStatus(const std::string& linkstr, bool fg = false);

//				const std::string getParamValue(const std::string& key);

				const bool checkAlreadyUsedChemicals();

				const bool setChemicalStructureInfo(
					const bool useExtDB,
					const std::string URL = "");

			private:
				DECLARE_EVENT_TABLE();
			};
		} // end of namespace wizard
	}
}

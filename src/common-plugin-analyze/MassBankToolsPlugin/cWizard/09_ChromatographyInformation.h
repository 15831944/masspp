/**
 * @file 09_..
 * @brief MassBank's create records09_ChromatographyInformation
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

			//-----------------------------------------------
			//  Wizard Element Page
			//-----------------------------------------------
			class ChromatographyInformationWizardPage : public kome::window::StandardWizardPage {

			public:
				ChromatographyInformationWizardPage(wxWizard* wizParent, RecordElements*& rcElem);

				// destrouctor
				~ChromatographyInformationWizardPage(){}

			protected:
				/**
				 * @fn virtual wxSizer* createMainSizer()
				 * @brief creates main sizer
				 * @return main sizer
				 */
				virtual wxSizer* createMainSizer();
			private:
				RecordElements* m_rcElem;
				kome::objects::SettingParameterValues inSettings_;

				DECLARE_EVENT_TABLE();
			};

			//-----------------------------------------------
			//  Form ParametersAndHelp
			//-----------------------------------------------
			class ChromatographyInformationPanel : public kome::window::ParametersAndHelpPanel {

			public:
				ChromatographyInformationPanel(
					wxWindow* parent, RecordElements*& rcElem,
					kome::objects::SettingParameterValues& inSettings);
				
				~ChromatographyInformationPanel();
			private:
				//** parameter definitions */
				kome::plugin::ParameterSettings inParams_;
				//** parameter values */
				kome::objects::SettingParameterValues& inSettings_;

				RecordElements* m_rcElem;
	
				// Extend Panel
				ExtendSettingsPanel* panelEx;

				std::vector<std::string> subtagArr_;
				std::vector<std::string> subtagngArr_;

				bool uniqueChkFg_;

			protected:			
				virtual wxSizer* createMainSizer();

				virtual bool TransferDataFromWindow();

				void onSettingsPanel(kome::window::SettingsPanelEvent& evt);

			private:
				void onCheckChromInfo();

			private:
				DECLARE_EVENT_TABLE();
			};
		} // end of namespace wizard
	}
}

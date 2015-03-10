/**
 * @file 08_..
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

			//-----------------------------------------------
			//  Wizard Element Page
			//-----------------------------------------------
			class SpectrumInformationWizardPage : public kome::window::StandardWizardPage {

			public:
				SpectrumInformationWizardPage(wxWizard* wizParent, RecordElements*& rcElem);
				// destrouctor
				~SpectrumInformationWizardPage(){}

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
			class SpectrumInformationPanel : public kome::window::ParametersAndHelpPanel {

			public:
				SpectrumInformationPanel(
					wxWindow* parent, RecordElements*& rcElem,
					kome::objects::SettingParameterValues& inSettings);
				~SpectrumInformationPanel();

			private:
				//** parameter definitions */
				kome::plugin::ParameterSettings inParams_;
				//** parameter values */
				kome::objects::SettingParameterValues& inSettings_;

				std::vector<std::string> subtagArr_;
				std::vector<std::string> subtagngArr_;

				RecordElements* m_rcElem;
				// Extend Panel
				ExtendSettingsPanel* panelEx;

				bool uniqueChkFg_;

			protected:			
				virtual wxSizer* createMainSizer();

				virtual bool TransferDataFromWindow();

				void onSettingsPanel(kome::window::SettingsPanelEvent& evt);
				
				void onCheckSpecAddInfo();

			private:
				DECLARE_EVENT_TABLE();
			};
		} // end of namespace wizard
	}
}

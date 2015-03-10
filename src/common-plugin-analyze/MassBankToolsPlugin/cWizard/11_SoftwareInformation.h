/**
 * @file 11_..
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
			class SoftwareInformationWizardPage : public kome::window::StandardWizardPage {

			public:
				SoftwareInformationWizardPage(wxWizard* wizParent, RecordElements*& rcElem);

				// destrouctor
				~SoftwareInformationWizardPage(){}

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
			class SoftwareInformationPanel : public kome::window::ParametersAndHelpPanel {

			public:
				SoftwareInformationPanel(
					wxWindow* parent,
					kome::objects::SettingParameterValues& inSettings,
					RecordElements*& rcElem);
				~SoftwareInformationPanel();

			protected:			
				virtual wxSizer* createMainSizer();

				virtual bool TransferDataToWindow();
				virtual bool TransferDataFromWindow();

			private:
				//** parameter definitions */
				kome::plugin::ParameterSettings inParams_;
				//** parameter values */
				kome::objects::SettingParameterValues& inSettings_;

				ExtendSettingsPanel* m_panel;

				RecordElements* m_rcElem;
			private:
				DECLARE_EVENT_TABLE();
			};
		} // end of namespace wizard
	}
}

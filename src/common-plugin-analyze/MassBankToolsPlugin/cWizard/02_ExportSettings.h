/**
 * @file 02_..
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

			//Forward declaration
			class RecordElements;
			class ExtendSettingsPanel;
			
			//-----------------------------------------------
			//  Wizard Element Page
			//-----------------------------------------------
			class ExportSettingsWizardPage : public kome::window::StandardWizardPage {

			public:
				// constructor
				ExportSettingsWizardPage(wxWizard* wizParent, RecordElements*& rcElem);

				// destructor
				~ExportSettingsWizardPage(){}

			protected:
				// -- inherited class methos.

				/**
				 * @fn virtual wxSizer* createMainSizer()
				 * @brief creates main sizer
				 * @return main sizer
				 */
				virtual wxSizer* createMainSizer();

			private:
				//** parameter values */
				kome::objects::SettingParameterValues inSettings_;

				//** Record Elements Keeper */
				RecordElements* m_rcElem;

			private:
				DECLARE_EVENT_TABLE();
			};

			//-----------------------------------------------
			//  Form ParametersAndHelp
			//-----------------------------------------------
			class ExportSettingsPanel : public kome::window::ParametersAndHelpPanel {

			public:
				// constructor
				ExportSettingsPanel(
					wxWindow* parent, RecordElements*& rcElem,
					kome::objects::SettingParameterValues& inSettings);

				// destructor
				~ExportSettingsPanel();

			protected:
				// -- inherited class methos.
				virtual wxSizer* createMainSizer();

				virtual bool TransferDataFromWindow();

				void onSettingsPanel(kome::window::SettingsPanelEvent& evt);

			protected:
				//** parameter definitions */
				kome::plugin::ParameterSettings inParams_;

				//** parameter values */
				kome::objects::SettingParameterValues& m_settings;

			private:
				RecordElements* m_rcElem;

				ExtendSettingsPanel* panelEx;
			private:
				DECLARE_EVENT_TABLE();
			};
		} // end of namespace wizard
	}
}

/**
 * @file 04_..
 * @brief MassBank's create records
 *
 * @author M.Fukuda
 * @date 2013.02.22
 *
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#pragma once

namespace kome {
	namespace massbank {
		namespace wizard {

			// -- Forward declaration --
			class RecordElements;

			//-----------------------------------------------
			//  Wizard Element Page
			//-----------------------------------------------
			class RecordSettingsWizardPage : public kome::window::StandardWizardPage {

			public:
				//constrctor
				RecordSettingsWizardPage(wxWizard* wizParent, RecordElements*& rcElem);

				//destrctor
				~RecordSettingsWizardPage(){}

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
			class RecordSettingsPanel : public kome::window::ParametersAndHelpPanel {

			public:
				RecordSettingsPanel(
					wxWindow* parent, RecordElements*& rcElem,
					kome::objects::SettingParameterValues& inSettings);
				
				~RecordSettingsPanel();

			protected:
				// -- inherited class methos.
				virtual wxSizer* createMainSizer();

				virtual bool TransferDataFromWindow();

			protected:			
				//** parameter definitions */
				kome::plugin::ParameterSettings inParams_;
				//** parameter values */
				kome::objects::SettingParameterValues& inSettings_;

			private:
				RecordElements* m_rcElem;
				
			private:
				DECLARE_EVENT_TABLE();
			};
		} // end of namespace wizard
	}
}

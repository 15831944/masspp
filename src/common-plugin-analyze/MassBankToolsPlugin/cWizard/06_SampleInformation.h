/**
 * @file 06_..
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
			class SampleInformationWizardPage : public kome::window::StandardWizardPage {

			public:
				SampleInformationWizardPage(wxWizard* wizParent, RecordElements*& rcElem);
				~SampleInformationWizardPage(){}
			protected:
				virtual wxSizer* createMainSizer();
			private:
				kome::objects::SettingParameterValues inSettings_;
				RecordElements* m_rcElem;

				DECLARE_EVENT_TABLE();
			};

			//-----------------------------------------------
			//  Form ParametersAndHelp
			//-----------------------------------------------
			class SampleInformationPanel : public kome::window::ParametersAndHelpPanel {

			public:
				SampleInformationPanel(
					wxWindow* parent, RecordElements*& rcElem,
					kome::objects::SettingParameterValues& inSettings);				

				~SampleInformationPanel();

			protected:
				virtual wxSizer* createMainSizer();

				virtual bool TransferDataFromWindow();

			protected:
				//** parameter definitions */
				kome::plugin::ParameterSettings inParams_;
				//** parameter values */
				kome::objects::SettingParameterValues& inSettings_;

				RecordElements* m_rcElem;
			private:
				DECLARE_EVENT_TABLE();
			};
		} // end of namespace wizard
	}
}

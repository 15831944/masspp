/**
 * @file 07_..
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

			//-----------------------------------------------
			//  Wizard Element Page
			//-----------------------------------------------
			class InstrumentWizardPage : public kome::window::StandardWizardPage {

			public:
				InstrumentWizardPage(wxWizard* wizParent, RecordElements*& rcElem);
				// destrouctor
				~InstrumentWizardPage(){}

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
			class InstrumentPanel : public kome::window::ParametersAndHelpPanel {

			public:
				InstrumentPanel(
					wxWindow* parent, RecordElements*& rcElem,
					kome::objects::SettingParameterValues& inSettings);
				
				~InstrumentPanel();

			protected:			
				virtual wxSizer* createMainSizer();

				virtual bool TransferDataFromWindow();

				RecordElements* m_rcElem;
				//** parameter definitions */
				kome::plugin::ParameterSettings inParams_;
				//** parameter values */
				kome::objects::SettingParameterValues& inSettings_;
			private:
				DECLARE_EVENT_TABLE();
			};
		} // end of namespace wizard
	}
}

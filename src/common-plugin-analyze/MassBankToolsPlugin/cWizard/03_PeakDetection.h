/**
 * @file 03_..
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
			class ExtendSettingsPanel;
			class RecordElements;

			//-----------------------------------------------
			//  Wizard Element Page
			//-----------------------------------------------
			class PeakDetectionWizardPage : public kome::window::StandardWizardPage {

			public:
				PeakDetectionWizardPage(wxWizard* wizParent, RecordElements*& rcElem);

				~PeakDetectionWizardPage(){}

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

			private:
				DECLARE_EVENT_TABLE();
			};

			//-----------------------------------------------
			//  Form ParametersAndHelp
			//-----------------------------------------------
			class PeakDetectionPanel : public kome::window::ParametersAndHelpPanel {

			public:
				PeakDetectionPanel(
					wxWindow* parent, RecordElements*& rcElem,
					kome::objects::SettingParameterValues& inSettings);
				
				~PeakDetectionPanel();

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

//				ExtendSettingsPanel* panelEx;

			private:
				DECLARE_EVENT_TABLE();
			};
		} // end of namespace wizard
	}
}

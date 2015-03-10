/**
 * @file 15_..
 * @brief MassBank's create records Final
 *
 * @author M.Fukuda
 * @date 2013.03.04
 *
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#pragma once

namespace kome {
	namespace massbank {
		namespace wizard {

			// -- Forward declaration --
			class RecordElements;
			class SpectraGridList;

			//-----------------------------------------------
			//  Wizard Element Page
			//-----------------------------------------------
			class FinalSequenceWizardPage : public kome::window::StandardWizardPage {

			public:
				// const
				FinalSequenceWizardPage(wxWizard* wizParent, RecordElements*& rcElem);

				// destrouctor
				~FinalSequenceWizardPage(){}

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

			private:
				DECLARE_EVENT_TABLE();
			};

			//-----------------------------------------------
			//  Form ParametersAndHelp
			//-----------------------------------------------
			class FinalSequencePanel : public kome::window::ParametersAndHelpPanel {

			public:
				FinalSequencePanel(
					wxWindow* parent, RecordElements*& rcElem,
					kome::objects::SettingParameterValues& inSettings);

				~FinalSequencePanel();
			private:
				kome::plugin::ParameterSettings inParams_;
				kome::objects::SettingParameterValues& m_settings;

				RecordElements* m_rcElem;

				SpectraGridList* m_listp;

				kome::window::SpectrumCanvas* m_canvas;
				wxTextCtrl* m_text;

			protected:
				void onSct(wxListEvent& evt);

				virtual wxSizer* createMainSizer();

				virtual bool TransferDataToWindow();

			private:
				void initSpecList();
				void setAverageSpectra();
				void showSelectedInfo(const int idx);

			private:
				DECLARE_EVENT_TABLE();
			};
		} // end of namespace wizard
	}
}

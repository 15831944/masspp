/**
 * @file 01_TargetSpectra
 * @brief MassBank's create records
 *
 * @author M.Fukuda
 * @date 2013.02.19
 *
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#pragma once

class SpectraCheckList;

namespace pane{
	class SpecCheckListPanel;
}

namespace elm{
	class  SpectrumElementSetter;
}

namespace kome {
	namespace massbank {
		namespace wizard {

			// -- Forward declaration --
			class RecordElements;
			class TargetSpectraPanel;
			class ExtendSettingsPanel;
			class AllSamplePanel;

			//-----------------------------------------------
			//  Wizard Element Page
			//-----------------------------------------------
			class TargetSpectraWizardPage : public kome::window::StandardWizardPage {

			public:
				TargetSpectraWizardPage(wxWizard* wizParent, RecordElements*& rcElem);
				~TargetSpectraWizardPage();
			protected:
				 //createMainSizer
				virtual wxSizer* createMainSizer();

			private:
				kome::objects::SettingParameterValues m_settings;
				TargetSpectraPanel* m_panel1;
				RecordElements* m_rcElem;

			private:
				DECLARE_EVENT_TABLE();
			};

			//-----------------------------------------------
			//  Form ParametersAndHelp
			//-----------------------------------------------
			class TargetSpectraPanel : public kome::window::ParametersAndHelpPanel {

			public:
				TargetSpectraPanel(
					wxWindow* parent, 
					RecordElements*& rcElem,
					kome::objects::SettingParameterValues& settings);

				~TargetSpectraPanel();

			protected:
				kome::plugin::ParameterSettings m_param;
				kome::objects::SettingParameterValues& inSettings_;
				std::vector<kome::objects::XYDataOperation*> opts_;

			private:
				pane::SpecCheckListPanel* sclpA_;
				pane::SpecCheckListPanel* sclpD_;
				AllSamplePanel* allPane_;
				RecordElements* m_rcElem;
				kome::window::RadioBoxBook* m_book;

			protected:
				virtual wxSizer* createMainSizer();
				virtual bool TransferDataFromWindow();
				virtual std::string getDescription(wxWindow* wnd);
			private:
				void createDisplayedPanel();
				void createAllOpenedPanel();
				void parseProcessing(kome::objects::Spectrum* dmyspec);

			private:
				DECLARE_EVENT_TABLE();
			};

			//------------------------------------------------------
			//
			//   ALL Samples Panel   -------------------------------
			//
			//------------------------------------------------------

			class AllSamplePanel : public kome::window::HelpGetterPanel {

			public:
				AllSamplePanel(
					wxWindow* parent,
					kome::objects::SettingParameterValues& settings
					);

				virtual ~AllSamplePanel();

			private:
				kome::plugin::ParameterSettings inParams_;
				kome::objects::SettingParameterValues& inSettings_;

				wxButton* filterOnButton_;
				wxButton* filterOffButton_;

				// inner values
				wxWindow* parent_;
				ExtendSettingsPanel* panelEx_;
				SpectraCheckList* scList_;
				pane::SpecCheckListPanel* sclpA_;
				bool cgfg_;

				//サンプル(群)を管理するアダプタ
				std::unique_ptr<elm::SpectrumElementSetter> sesPtr_;

				std::vector<std::vector<kome::objects::Spectrum*>> rstArr_;
				std::vector<kome::objects::Spectrum*> specArr_;

			protected:
				// overLs
				virtual wxSizer* createMainSizer();
				virtual bool TransferDataFromWindow();
				wxGridSizer* createFilterButtons();
				//Events
				void onSettingsPanel(kome::window::SettingsPanelEvent& evt);
				void onFilterONButton(wxCommandEvent& evt);
				void onFilterOFFButton(wxCommandEvent& evt);
				void onUPUI(wxUpdateUIEvent& evt);

			public:
				pane::SpecCheckListPanel* 
					getSpecCheckListPanelPointer() const {return sclpA_;}

				// for AveragedSpectrumStatus
				std::vector<std::vector<kome::objects::Spectrum*>>
					getSpecArr() const {return rstArr_;}

			private:
				const bool onSetAverageSpectrum();

				void updateSpecList();

				const bool spareteAveregeSpectrumElement(
					const std::string& src);

				const bool onGetCheckedSamplesAllSpectra(std::vector<kome::objects::Spectrum*>&);

			private:
				DECLARE_EVENT_TABLE();
			};



		} // end of namespace wizard
	}
}

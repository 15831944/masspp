/**
 * @file SpectrumLabelingDialog.h
 * @brief interfaces of SpectrumLabelingDialog class
 *
 * @author S.Tanaka
 * @date 2006.10.10
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_LABELING_SPECTRUM_LABELING_DIALOG_H__
#define __KOME_LABELING_SPECTRUM_LABELING_DIALOG_H__


#include "PeakPickingManager.h"
#include <wx/wx.h>


namespace kome {
	namespace labeling {

		class SpectrumLabelHelpGetterPanel;

		/**
		 * @class SpectrumLabelingDialog
		 * @brief auto labeling dialog class
		 */
		class SpectrumLabelingDialog : public kome::window::ParametersAndHelpDialog {
		public:
			/**
			 * @fn SpectrumLabelingDialog(
						wxWindow* parent,
						kome::objects::Spectrum* spec
					)
			 * @brief constructor
			 * @param parent parent window
			 * @param spec active spectrum
			 */
			SpectrumLabelingDialog(
				wxWindow* parent,
				kome::objects::Spectrum* spec
			);

			/**
			 * @fn SpectrumLabelingDialog( wxWindow* parent )
			 * @brief constructor
			 * @param[in] parent parent window
			 */
			SpectrumLabelingDialog( wxWindow* parent );

			/**
			 * @fn virtual ~SpectrumLabelingDialog()
			 * @brief destructor
			 */
			virtual ~SpectrumLabelingDialog();

		protected:
			/** active spectrum */
			kome::objects::Spectrum* m_spec;

			/** simple flag */
			bool m_simple;

			/** peak picking target */
			PeakPickingManager::PeakPickingTarget m_target;

			/** peak picking function item */
			kome::plugin::PluginFunctionItem* m_peakPickingFunction;

			/** charge detection function item */
			kome::plugin::PluginFunctionItem* m_chargeDetectFunction;

			/** peak picking settings */
			kome::objects::SettingParameterValues* m_peakPickingSettings;

			/** charge detect settings */
			kome::objects::SettingParameterValues* m_chargeDetectSettings;

			/** parameters */
			kome::plugin::ParameterSettings m_parameters;

			/** settings */
			kome::objects::SettingParameterValues m_settings;
				
			/** target radio box */
			wxRadioBox* m_targetBox;

			/** peak filter‚ÌSettingParameterValues•¶Žš—ñ */
			std::string m_peakFilter;

			/** help getter */
			SpectrumLabelHelpGetterPanel* m_helpGetter;
		public:
			/**
			 * @fn PeakPickingManager::PeakPickingTarget getTarget()
			 * @brief gets peak pickng target
			 * @return peak picking target
			 */
			PeakPickingManager::PeakPickingTarget getTarget();

			/**
			 * @fn kome::plugin::PluginFunctionItem* getPeakPickingFunction()
			 * @brief gets peak picking function
			 * @return peak picking function
			 */
			kome::plugin::PluginFunctionItem* getPeakPickingFunction();

			/**
			 * @fn kome::plugin::PluginFunctionItem* getChargeDetectFunction()
			 * @brief gets charge detect function
			 * @return charge detect function
			 */
			kome::plugin::PluginFunctionItem* getChargeDetectFunction();

			/**
			 * @fn kome::objects::SettingParameterValues* getPeakPickingSettings()
			 * @brief gets peak picking settings
			 * @return peak picking settings
			 */
			kome::objects::SettingParameterValues* getPeakPickingSettings();

			/**
			 * @fn kome::objects::SettingParameterValues* getChargeDetectSettings()
			 * @brief gets charge detect settings
			 * @return charge detect settings
			 */
			kome::objects::SettingParameterValues* getChargeDetectSettings();

			/**
			 * @fn const char* getPeakFilterParam()
			 * @brief Peak Filtering‚ÌParameter Value•¶Žš—ñŽæ“¾
			 * @return Parameter Value•¶Žš—ñ
			 */
			const char* getPeakFilterParam();

		protected:
			/**
			 * @fn virtual wxSizer* createMainSizer();
			 * @brief creates main sizer (override method)
			 * @return field sizer
			 */
			virtual wxSizer* createMainSizer();

			/**
			 * @fn virtual wxSizer* createButtonSizer()
			 * @brief creates button sizer (override method)
			 * @return button sizer
			 */
			virtual wxSizer* createButtonSizer();					

        protected:
			/**
			 * @fn wxSizer* createPeakSizer()
			 * @brief creates peak sizer
			 * @return peak sizer
			 */
			wxSizer* createPeakSizer();

			/**
			 * @fn wxSizer* createChargeSizer()
			 * @brief creates charge sizer
			 * @return charge sizer
			 */
			wxSizer* createChargeSizer();
						
			/**
			 * @fn wxSizer* createPeakFilteringSizer()
			 * @brief creates peak filtering sizer
			 * return peak filtering sizer
			 */
			wxSizer* createPeakFilteringSizer();
		
		protected:
			/**
			 * @fn PeakPickingManager::PeakPickingTarget getPeakPickingTarget()
			 * @brief gets peak picking target
			 * @return peak picking target
			 */
			PeakPickingManager::PeakPickingTarget getPeakPickingTarget();

		protected:
			/**
			 * @fn virtual bool TransferDataFromWindow()
			 * @brief transfer window control data to value
			 * @return If a transfer failed, this method returns false.
			 */
			virtual bool TransferDataFromWindow();

		protected:
			/**
			 * @fn void onTestButton( wxCommandEvent& evt )
			 * @brief This button is called when the test button is clicked.
			 * @param evt event information
			 */
			void onTestButton( wxCommandEvent& evt );

		private:
			DECLARE_EVENT_TABLE();
		};

		// -------------------------------------------------------------------------
		// class spectrum label help getter panel
		// -------------------------------------------------------------------------
		class SpectrumLabelHelpGetterPanel : public kome::window::HelpGetterPanel {
		public:
			/**
			 * @fn SpectrumLabelHelpGetterPanel( wxWindow* parent, kome::objects::Spectrum* spec )
			 * @brief constructor
			 * @param parent
			 * @param spec
			 */
			SpectrumLabelHelpGetterPanel( wxWindow* parent, kome::objects::Spectrum* spec );

			/**
			 * @fn virtual ~SpectrumLabelHelpGetterPanel();
			 * @brief destructor
			 */
			virtual ~SpectrumLabelHelpGetterPanel();
		protected:
			/** target radio box */
			wxRadioBox* m_targetBox;

			/** spectrum object */
			kome::objects::Spectrum* m_spec;

		protected:
			/**
			 * @fn virtual wxSizer* createMainSizer();
			 * @brief creates main sizer (override method)
			 * @return field sizer
			 */
			virtual wxSizer* createMainSizer();

			/**
			 * @fn virtual std::string getDescription( wxWindow* wnd )
			 * @brief method to get the description from a prameter control
			 * @param[in] wnd parameter control
			 * @return description
			 */
			virtual std::string getDescription( wxWindow* wnd );
		public:
			/**
			 * @fn int getTargetSel()
			 * @brief get target selection
			 * @return sel
			 */
			int getTargetSel(){
				return m_targetBox->GetSelection();
			}

			/**
			 * @fn int getCount()
			 * @brief get count
			 * @retrn count 
			 */
			int getCount(){
				return m_targetBox->GetCount();
			}
		private:
			DECLARE_EVENT_TABLE();
		};
	}
}

#endif	//  __KOME_LABELING_SPECTRUM_LABELING_DIALOG_H__

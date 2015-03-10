/**
 * @file GenelateChromatogramDialog.h
 * @brief interfaces of GenelateChromatogramDialog class
 * @author T.Okuno
 * @date 2012.02.24
 * Copyright (C) 2014 Shimadzu Corporation All rights reserved.
 */

#ifndef __KOME_OPERATION_SEL_RANGE_GENELATE_CHROMATOGRAM_DIALOG_H__
#define __KOME_OPERATION_SEL_RANGE_GENELATE_CHROMATOGRAM_DIALOG_H__


#include <wx/wx.h>

namespace kome {
	namespace operation {
		namespace sel_range {

		class GenelateChromHelpGetterDlg;
		// ----------------------
		// Genelate Chromatogram Dialog
		// ----------------------
		class GenelateChromatogramDialog : public kome::window::ParametersAndHelpDialog {
		public:
			/**
			 * @fn GenelateChromatogramDialog( wxWindow* parent, kome::objects::DataGroupNode* group, bool menuFlg )
			 * @brief constructor
			 * @param[in] parent parent window
			 * @param[in] spec spectrum object
			 * @param[in] menuFlg
			 */
			GenelateChromatogramDialog( wxWindow* parent, kome::objects::DataGroupNode* group, bool menuFlg );

			/**
			 * @fn virtual ~GenelateChromatogramDialog()
			 * @brief destructor
			 */
			virtual ~GenelateChromatogramDialog();

		protected:
		
			/** start position  */
			double m_startPos;

			/** end position  */
			double m_endPos;

			/** group  */
			kome::objects::DataGroupNode* m_group;

			/** parameters */
			kome::plugin::ParameterSettings m_params;

			/** settings */
			kome::objects::SettingParameterValues m_settings;

			/** flag */
			bool m_flg;

			/** parameter name */
			std::string	m_strParamName;
			/** menu flag */
			bool m_menuFlg;

			/** help panel */
			GenelateChromHelpGetterDlg* m_helpPanel;

			/** title */
			std::string m_title;

		protected:
			/**
			 * @fn virtual wxSizer* createMainSizer()
			 * @brief creates main sizer (override method)
			 * @return main sizer
			 */
			virtual wxSizer* createMainSizer();

			/**
			 * @fn virtual wxSizer* createButtonSizer()
			 * @brief creates main sizer (override method)
			 * @return button sizer
			 */
			virtual wxSizer* createButtonSizer();

			/**
			 * @fn virtual bool TransferDataFromWindow()
			 * @brief transfer window control data to value
			 * @return If a transfer failed, this method returns false.
			 */
			virtual bool TransferDataFromWindow();

			/**
			 * @fn virtual bool TransferDataToWindow()
			 * @brief transfer value data to window controls
			 * @return If a transfer failed, this method returns false.
			 */
			virtual bool TransferDataToWindow();

		
		public:
			/**
			 * @fn double getStartPos()
			 * @brief get start position
			 * return start position
			 */
			double getStartPos();

			/**
			 * @fn void double getStartPos()
			 * @brief set start position
			 */
			void setStartPos( double sPos );

			/**
			 * @fn double getEndPos()
			 * @brief get end position
			 * return end position
			 */
			double getEndPos();

			/**
			 * @fn void double getEndPos()
			 * @brief set end position
			 */
			void setEndPos( double ePos );

			/**
			 * @fn void setSpec( kome::objects::Spectrum* spec )
			 * @brief set spectrum
			 * @param spec spectrum
			 */
			void setSpec( kome::objects::Spectrum* spec );

			/**
			 * @fn const char* getStrParamName()
			 * @brief get str parameter name
			 * @return param name
			 */
			const char* getStrParamName(){ return m_strParamName.c_str(); }

			/**
			 * @fn const char* getTitle()
			 * @brief gets the title 
			 * return title
			 */
			const char* getTitle();
						
		private:
			/**
			 * @fn void setWindowSettings()
			 * @brief set Window settings
			 */
			void setWindowSettings();

		private:
			DECLARE_EVENT_TABLE();
		};

		// -----------------------------------------------------------------
		// -----------------------------------------------------------------
		class GenelateChromHelpGetterDlg : public kome::window::HelpGetterPanel {
		public:
			GenelateChromHelpGetterDlg( wxWindow* parent );
			virtual ~GenelateChromHelpGetterDlg();

		public:
			/** radio box  */
			wxRadioBox* m_operatRadioBox;


		protected:
			/**
			 * @fn virtual wxSizer* createMainSizer()
			 * @brief creates main sizer (override method)
			 * @return main sizer
			 */
			virtual wxSizer* createMainSizer();

			virtual std::string getDescription( wxWindow* wnd );

		public:
			std::string getParamName();
		
		private:
			DECLARE_EVENT_TABLE();
		};
		}
	}
}

#endif // __KOME_OPERATION_SEL_RANGE_GENELATE_CHROMATOGRAM_DIALOG_H__

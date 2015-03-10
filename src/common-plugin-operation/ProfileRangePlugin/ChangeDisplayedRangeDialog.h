/**
 * @file ChangeDisplayedRangeDialog.h
 * @brief interfaces of ChangeDisplayedRangeDialog class
 * @author T.Okuno
 * @date 2012.02.28
 * Copyright (C) 2014 Shimadzu Corporation All rights reserved.
 */

#ifndef __KOME_OPERATION_SEL_RANGE_CHANGE_DISPLAYED_RANGE_DIALOG_H__
#define __KOME_OPERATION_SEL_RANGE_CHANGE_DISPLAYED_RANGE_DIALOG_H__


#include <wx/wx.h>

namespace kome {
	namespace operation {
		namespace sel_range {
		// ----------------------
		// Change Displayed Range Dialog
		// ----------------------
		class ChangeDisplayedRangeDialog : public kome::window::ParametersAndHelpDialog {
		public:
			// dialog type
			enum DIALOG_TYPE{
				DIALOG_MZ	= 0,
				DIALOG_RT,
				DIALOG_TYPE_NUM
			};

		public:
			/**
			 * @fn ChangeDisplayedRangeDialog()
			 * @brief constructor
			 * @param[in] parent parent window
			 * @param[in] spec spectrum object
			 */
			ChangeDisplayedRangeDialog( wxWindow* parent, kome::objects::Spectrum* spec );

			/**
			 * @fn ChangeDisplayedRangeDialog()
			 * @brief constructor
			 * @param[in] parent parent window
			 * @param[in] chrom chromatogram object
			 */
			ChangeDisplayedRangeDialog( wxWindow* parent, kome::objects::Chromatogram* chrom );

			/**
			 * @fn virtual ~ChangeDisplayedRangeDialog()
			 * @brief destructor
			 */
			virtual ~ChangeDisplayedRangeDialog();

		protected:
			/** start position  */
			double m_startPos;

			/** end position  */
			double m_endPos;

			/** objects spectrum  */
			kome::objects::Spectrum* m_spec;

			/** objects chromatogram */
			kome::objects::Chromatogram* m_chrom;

			/** objects canvas */
			wxWindow* m_canvas;

			/** parameters */
			kome::plugin::ParameterSettings m_params;

			/** settings */
			kome::objects::SettingParameterValues m_settings;

			/** flag */
			bool m_flg;

			/** dialog type flag */
			int		m_nDialogType;

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
			 * @fn void setChrom( kome::objects::Chromatogram* chrom )
			 * @brief set Chromatogram
			 * @param chrom Chromatogram
			 */
			void setChrom( kome::objects::Chromatogram* chrom );

			/**
			 * @fn void setCanvas( wxWindow* canvas )
			 * @brief set canvas
			 * @param canvas canvas window
			 */
			void setCanvas( wxWindow* canvas );

		private:
			/**
			 * @fn void setWindowSettings()
			 * @brief set Window settings
			 */
			void setWindowSettings();

		protected:
			/**
			 * @fnvoid onOkButton( wxCommandEvent& evt )
			 * @brief This method is called when the OK button is clicked.
			 * @param[in] evt click event
			 */
			void onOkButton( wxCommandEvent& evt );

		private:
			DECLARE_EVENT_TABLE();
		};
		}
	}
}

#endif // __KOME_OPERATION_SEL_RANGE_CHANGE_DISPLAYED_RANGE_DIALOG_H__

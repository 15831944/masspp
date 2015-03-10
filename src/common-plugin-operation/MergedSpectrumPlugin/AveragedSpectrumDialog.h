/**
 * @file AveragedSpectrumDialog.h
 * @brief interfaces of AveragedSpectrumDialog class
 * @author T.Okuno
 * @date 2012.02.28
 * Copyright (C) 2014 Shimadzu Corporation All rights reserved.
 */

#ifndef __KOME_OPERATION_SEL_RANGE_AVERAGED_SPECTRUM_DIALOG_H__
#define __KOME_OPERATION_SEL_RANGE_AVERAGED_SPECTRUM_DIALOG_H__


#include <wx/wx.h>

namespace kome {
	namespace operation {
		namespace sel_range {
		// ----------------------
		// Averaged Spectrum Dialog
		// ----------------------
		class AveragedSpectrumDialog : public kome::window::StandardDialog {
		public:
			/**
			 * @fn AveragedSpectrumDialog()
			 * @brief constructor
			 * @param[in] parent parent window
			 * @param[in] chrom chromatogram object
			 */
			AveragedSpectrumDialog( wxWindow* parent, kome::objects::Chromatogram* chrom );

			/**
			 * @fn virtual ~AveragedSpectrumDialog()
			 * @brief destructor
			 */
			virtual ~AveragedSpectrumDialog();

		protected:
			/** start position  */
			double m_startPos;

			/** end position  */
			double m_endPos;

			/** objects chromatogram */
			kome::objects::Chromatogram* m_chrom;

			/** parameters */
			kome::plugin::ParameterSettings m_params;

			/** settings */
			kome::objects::SettingParameterValues m_settings;

			/** flag */
			bool m_flg;

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
			 * @fn void setChrom( kome::objects::Chromatogram* chrom )
			 * @brief set Chromatogram
			 * @param chrom Chromatogram
			 */
			void setChrom( kome::objects::Chromatogram* chrom );

		
		private:
			/**
			 * @fn void setWindowSettings()
			 * @brief set Window settings
			 */
			void setWindowSettings();

		
		private:
			DECLARE_EVENT_TABLE();
		};
		}
	}
}

#endif // __KOME_OPERATION_SEL_RANGE_AVERAGED_SPECTRUM_DIALOG_H__

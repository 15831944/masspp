/**
 * @file ExportPeaksDialog.h
 * @brief interfaces of ExportPeaksDialog class
 *
 * @author M.Izumi
 * @date 2013.02.12
 *
 * Copyright (C) 2014 Shimadzu Corporation All rights reserved.
 */

#ifndef __KOME_LABELING_EXPORT_PEAKS_DIALOG_H__
#define __KOME_LABELING_EXPORT_PEAKS_DIALOG_H__

// ‘I‘ðŽˆ‚ðkome.ini‚É•Û‘¶	// @date 2011/08/22 <Add> OKADA
#define EXPORT_SECTION		"EXPORT_SAMPLE"
#define TARGET_KEY	"TARGET"
#define FORMAT_KEY	"FORMAT"

namespace kome {
	namespace io {
		namespace peaks {
			
			/**
			 * class
			 */
			class ExportPeaksDialog : public kome::window::ParametersAndHelpDialog {
		
			public:
				ExportPeaksDialog( wxWindow* parent );

				virtual ~ExportPeaksDialog();
							
			   /* @typedef ExportTarget
				* @brief export target
				*/
				typedef enum {
					TARGET_SPECTRUM = 1,
					TARGET_FILTERED = 2,
					TARGET_SAMPLE = 4,
					TARGET_CHROMATOGRAM = 8
				} ExportTarget;
			protected:
			
				/** radio box */
				wxRadioBox* m_RadioBox;
			

				/** target */
				ExportTarget m_target;

				int m_selectRadioBox;
			protected:
				/**
				 * @fn virtual wxSizer* createMainSizer();
				 * @brief creates main sizer (override method)
				 * @return field sizer
				 */
				wxSizer* createMainSizer();
				
				/**
				* @fn virtual wxSizer* createButtonSizer()
				* @brief creates button sizer (override method)
				* @return button sizer
				*/
				wxSizer* createButtonSizer();
				
				/**
				 * @fn virtual bool TransferDataFromWindow()
				 * @brief transfers data from window (override method)
				 * @return true or false
				 */
				virtual bool TransferDataFromWindow();
			
			public:
				/**
				 * @fn int getSelTarget()
				 * @brief get the select target number
				 * @return select radio box
				 */
				int getSelTarget(){ return m_selectRadioBox; }
				
			private:
				DECLARE_EVENT_TABLE();
			};
		}
	}
}

#endif	// __KOME_LABELING_EXPORT_PEAKS_DIALOG_H__

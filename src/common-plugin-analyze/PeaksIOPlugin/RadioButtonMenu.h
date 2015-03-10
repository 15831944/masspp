/**
 * @file RadioButtonMenu.h
 * @brief radio button menu dialog box
 *
 * @author OKADA, H
 * @date 2011/05/31
 * 
 * Copyright(C) 2014 Shimadzu Corporation. All rights reserved.
 */



#ifndef __RADIO_BUTTON_MENU_H__
#define __RADIO_BUTTON_MENU_H__

#include <wx/wx.h>

enum{
	IMPORT_PEAKS_IMPORT,		//=100等の番号を指定してもかまわない,
	IMPORT_PEAKS_OVERLAP,
	IMPORT_PEAKS_DELETE_OVERLAP,
    IMPORT_PEAKS_NUMS		    // 項目数=IMPORT_PEAKS_NUMS-IMPORT_PEAKS_IMPORT
};

#define STR_IMPORT_PEAKS_IMPORT			"Import"
#define STR_IMPORT_PEAKS_OVERLAP		"Overlap"
#define STR_IMPORT_PEAKS_DELETE_OVERLAP	"Delete Overlap"

#ifndef _countof
	#define _countof(array)	sizeof(array)/sizeof((array)[0])
#else
#endif
                                           

#define USE_RADIO_BUTTON 1	/* 1:ラジオボタン 0:ラジオボックス */	/* ラジオボタンを枠で囲みたい場合は、0を定義する */

namespace kome {
	namespace labeling {
		/**
		 * @class Radio Button Menu Dialog
		 * @brief  Radio Button Menu dialog
		 */
		class RadioButtonMenuDialog : public kome::window::ParametersAndHelpDialog {
		
		public:
			/**
			 * @fn RadioButtonMenuDialog(
						wxWindow* parent,
						kome::objects::Variant acObj
					)
			 * @brief constructor
			 * @param parent parent window
			 * @param spec active spectrum
			 */
			RadioButtonMenuDialog(
				wxWindow* parent,
				kome::objects::Variant acObj
			);

			/**
			 * @fn virtual ~RadioButtonMenuDialog()
			 * @brief destructor
			 */
			virtual ~RadioButtonMenuDialog();
			
		protected:
			/** active object */
			kome::objects::Variant m_obj;

#if USE_RADIO_BUTTON
			int m_selectedRadioButtonNo;

			/** radio button menu text */
			wxString strMenu[IMPORT_PEAKS_NUMS - IMPORT_PEAKS_IMPORT];

			/** window id of radio button */
			wxWindowID idMenu[IMPORT_PEAKS_NUMS - IMPORT_PEAKS_IMPORT];
			
			wxRadioButton* m_tempRadioButton;

#else
			/** target radio box */
			wxRadioBox* m_targetBox;
#endif

		protected:
			/**
			 * @fn virtual wxSizer* createMainSizer();
			 * @brief creates main sizer (override method)
			 * @return field sizer
			 */
			virtual wxSizer* createMainSizer();
			
		protected:
#if USE_RADIO_BUTTON
			/**
			 * @fn wxRadioButton* createTargetBox()
			 * @brief creates target radio button
			 * @return target radio button
			 */
//			wxRadioButton* createTargetBox();
#else
			/**
			 * @fn wxRadioBox* createTargetBox()
			 * @brief creates target radio box
			 * @return target radio box
			 */
			wxRadioBox* createTargetBox();
#endif

		public:
			/**
			 * @fn int getSelection()
			 * @brief select value
			 * @return selected radio button No.
			 */
			int getSelection();

#if USE_RADIO_BUTTON
			void OnSelectRadioButton(wxCommandEvent& event);

		private:
			DECLARE_EVENT_TABLE();

#endif
	};
	}
}



#endif



/**
 * @file BackgroundSubtractDialog.h
 * @brief interfaces of BackgroundSubtractDialog class
 *
 * @author S.Tanaka
 * @date 2009.12.08
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_VIEW_BACKGROUND_SUBTRACT_DIALOG_H__
#define __KOME_VIEW_BACKGROUND_SUBTRACT_DIALOG_H__


#include <vector>
#include <map>
#include <wx/wx.h>

#include "BackgroundSpectrum.h"


namespace kome {
	namespace background {
		
		class BgSubScrollWin;
		/**
		 * @class BackgroundSubtractDialog
		 * @brief background subtract dialog
		 */
		class BackgroundSubtractDialog : public kome::window::StandardDialog {
		public:
			/**
			 * @fn BackgroundSubtractDialog( wxWindow* parent )
			 * @brief constructor
			 * @param parent parent window
			 */
			BackgroundSubtractDialog( wxWindow* parent );

			/**
			 * @fn virtual ~BackgroundSubtractDialog()
			 * @brief destructor
			 */
			virtual ~BackgroundSubtractDialog();

		protected:
			/** background spectrum */
			BackgroundSpectrum m_spec;

			/** BgsubScrollWind class  */
			BgSubScrollWin* m_scrWin;				// @date 2011.07.14 <Add> M.Izumi

			/** temp spectrum */
			kome::objects::Spectrum* m_tmpSpec;

			/** subtract button */
			wxButton* m_subBtn;
		public:
			/** 
			 * @fn wxButton* getSubtractButton()
			 * @brief get subtract button
			 * @return subtract button
			 */
			wxButton* getSubtractButton();
		protected:
			/**
			 * @fn virtual wxSizer* createMainSizer()
			 * @brief creates main sizer (override method)
			 * @return main sizer
			 */
			virtual wxSizer* createMainSizer();

			/**
			 * @fn virtual wxSizer* createButtonSizer()
			 * @brief creates button sizer (override method)
			 * @return button sizer
			 */
			virtual wxSizer* createButtonSizer();

		public:
			/**
			  * @fn kome::objects::Spectrum* getTmpSpec()
			  * @brief get temp Spectrum 
			  */
			kome::objects::Spectrum* getTmpSpec();	// @date 2011.07.14 <Add> M.Izumi

			/**
			 * @fn void AddSpectrumCanvas()
			 * @brief ダイアログにSingleのSpectrum Canvas追加
			 */
			void AddSpectrumCanvas();				// @date 2011.07.14 <Add> M.Izumi

			/**
			 * @fn void closeCanvas( kome::objects::Spectrums* spec )
			 * @brief Spectrum Canvasを閉じた時、ダイアログ内の一致するCanvasを閉じる
			 *        "Background"に指定されているCanvasを閉じると次にアクティブになるCanvasuをBackground"に指定する
			 * @param spec spectrum object
			 */
			void closeCanvas( kome::objects::Spectrum* spec );	// @date 2011.07.14 <Add> M.Izumi
						
		protected:
			/**
			 * @fn virtual bool onOK()
			 * @brief This method is called when the "OK" button is clicked and values in the window is valid.
			 * @return If true, this window is closed.
			 */
			virtual bool onOK();

			/**
			 * @fn virtual bool onCancel()
			 * @brief This method is called when the "Cancel" button is clicked. (override method)
			 * @return If true, this dialog is closed.
			 */
			virtual bool onCancel();

		protected:
			/**
			 * @fn void onSubtraction( wxCommandEvent& evt )
			 * @brief This method is called when the "Background Subtraction" button is clicked.
			 * @param evt event information
			 */
			void onSubtraction( wxCommandEvent& evt );

		private:
			DECLARE_EVENT_TABLE();
		};
	}
}

#endif		// __KOME_VIEW_BACKGROUND_SUBTRACT_DIALOG_H__

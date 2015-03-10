/**
 * @file OverlappingDialog.h
 * @brief interfaces of OverlappingDialog class
 *
 * @author S.Tanaka
 * @date 2008.04.19
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_WINDOW_OVERLAPPING_DIALOG_H__
#define __KOME_WINDOW_OVERLAPPING_DIALOG_H__


#include <wx/wx.h>
#include <wx/spinctrl.h>


namespace kome {
	namespace view {

		class kome::window::CanvasBase;
		class StandardPanel;

		/**
		 * @class OverlappingDialog
		 * @brief overlapping dialog class
		 */
		class OverlappingDialog : public kome::window::StandardPanel {
		public:
			/**
			 * @fn OverlappingDialog( wxWindow* parent )
			 * @brief constructor
			 */
			OverlappingDialog( wxWindow* parent );

			/**
			 * @fn virtual ~OverlappingDialog()
			 * @brief destructor
			 */
			virtual ~OverlappingDialog();

		protected:
			/** chromatogram flag */
			bool m_chromFlg;

			/** canvas */
			kome::window::CanvasBase* m_canvas;

			/** default color */
			wxColour m_defaultColor;

			/** list box */
			wxListBox* m_list;

			/** remove button */
			wxButton* m_removeButton;

			/** scale x text ctrl */
			wxTextCtrl* m_scXText;

			/** scale x spin button */
			wxSpinButton* m_scXSpin;

			/** translate x text ctrl */
			wxTextCtrl* m_trXText;

			/** translate x spin button */
			wxSpinButton* m_trXSpin;
						
			// @date 2012.02.21 sugo add -->
			/** Delete button */
			wxBitmapButton*	m_pOverlappingDeleteButton;
			/** Up button */
			wxBitmapButton*	m_pOverlappingUpButton;
			/** Down button */
			wxBitmapButton*	m_pOverlappingDownButton;
			/** Color button */
			wxBitmapButton*	m_pOverlappingColorButton;
			/** Update button */
			wxButton*	m_pOverlappingUpdateButton;
			/** Auto Y Scale check box */
			wxCheckBox* m_pOverlappingAutoYScalCheckbox;

			/** spin button down flag */
			bool m_bSpinDown;
			/** selected list flag */
			bool m_bListSelect;
			/** current offset */
			int m_offset;
			/** current angle */
			int m_angle;
			// @date 2012.02.21 <-- sugo 

		public:
			/**
			 * @fn void update()
			 * @brief updates dialog
			 */
			void update();

			/**
			 * @fn void select()
			 * @brief select spectra or chromatograms
			 */
			void select();

			/**
			 * @fn void setCanvas( CanvasBase* canvas )
			 * @brief sets canvas
			 * @param canvas selected canvas
			 */
			void setCanvas( kome::window::CanvasBase* canvas );

			/**
			 * @fn void SelectSpecOfCanvas( kome::objects::Spectrum* spec )
			 * @brief キャンバスで選択されたスペクトルをリストに反映	
			 * @param spec selected spectrum object
			 */
			void SelectSpecOfCanvas( kome::objects::Spectrum* spec );

			/**
			 * @fn void SelectChromOfCanvas( kome::objects::Chromatogram* chrom )
			 * @brief キャンバスで選択されたクロマトグラムをリストに反映
			 * @param chrom selected chromatogram object
			 */
			void SelectChromOfCanvas( kome::objects::Chromatogram* chrom );

		protected:
			/**
			 * @fn bool transform()
			 * @brief transform overlapping window
			 * @returns If false, any transform value is illegal.
			 */
			bool transform();

		   /**
			* @fn bool checkScaling( wxTextCtrl* text, double* val, bool* flg )
			* @brief checks scaling value
			* @param[in] text text control
			* @param[out] val the pointer to store scaling value
			* @param[out] flg the pointer to store update flag
			* @return If false, the text data is illegal.
			*/
			// @date 2012.02.21 sugo change
			bool checkScaling( wxTextCtrl* text, int* val, bool* flg );

		   /**
			* @fn bool checkTranslation( wxTextCtrl* text, double* val, bool* flg )
			* @brief checks translation value
			* @param[in] text text control
			* @param[out] val the pointer to store scaling value
			* @param[out] flg the pointer to store update flag
			* @return If false, the text data is illegal.
			*/
			// @date 2012.02.21 sugo change
			bool checkTranslation( wxTextCtrl* text, int* val, bool* flg );

		protected:
			/** 
			 * @fn wxSizer* createEditSizer()
			 * @brief creates edit sizer
			 * @return edit sizer
			 */
			wxSizer* createEditSizer();

			/**
			 * @fn wxSizer* createTransformSizer()
			 * @brief creates transform sizer
			 * @return transform sizer
			 */
			wxSizer* createTransformSizer();

			// @date 2012.02.21 sugo add createUpdateSizer
			/**
			 * @fn wxSizer* createUpdateSizer()
			 * @brief creates Auto Y Scaling and Update sizer
			 * @return Auto Y Scaling and Update sizer
			 */
			wxSizer* createUpdateSizer();

		protected:
				/**
			 * @fn virtual wxSizer* createMainSizer()
			 * @brief creates main sizer
			 * @return main sizer
			 */
			virtual wxSizer* createMainSizer();

		protected:
			/**
			 * @fn void onSelect( wxCommandEvent& evt )
			 * @brief This method is called when list items are selected
			 * @param[in] evt list box event information
			 */
			void onSelect( wxCommandEvent& evt );

			/**
			 * @fn void onUpdateButton( wxCommandEvent& evt )
			 * @brief This method is called when the udpate button is clicked.
			 * @param[in] evt event information
			 */
			void onUpdateButton( wxCommandEvent& evt );

			/**
			 * @fn void onRemoveButton( wxCommandEvent& evt )
			 * @brief This method is called when the remove button is clicked.
			 * @param[in] evt event information
			 */
			void onRemoveButton( wxCommandEvent& evt );

		public:
			/**
			 * @fn void onRemoveButton()
			 * @brief This method is called when the remove button( or Delete key ) is clicked.
			 * @param[in] evt event information
			 */
			void onRemoveButton( void );	// @date 2012/12/06 <Add> OKADA

		protected:
			/**
			 * @fn void onColorButton( wxCommandEvent& evt )
			 * @brief This method is called when the color button is clicked.
			 * @param[in] evt event information
			 */
			void onColorButton( wxCommandEvent& evt );

			/**
			 * @fn void onScaleXSpinUp( wxSpinEvent& evt )
			 * @brief This method is called when the scale x up spin is clicked.
			 * @param[in] evt envent information
			 */
			void onScaleXSpinUp( wxSpinEvent& evt );

			/**
			 * @fn void onScaleXSpinDown( wxSpinEvent& evt )
			 * @brief This method is called when the scale x down spin is clicked.
			 * @param[in] evt envent information
			 */
			void onScaleXSpinDown( wxSpinEvent& evt );

			/**
			 * @fn void onTranslateXSpinUp( wxSpinEvent& evt )
			 * @brief This method is called when the translate x up is clicked.
			 * @param[in] evt event information
			 */
			void onTranslateXSpinUp( wxSpinEvent& evt );
			
			/**
			 * @fn void onTranslateXSpinDown( wxSpinEvent& evt )
			 * @brief This method is called when the translate x down is clicked.
			 * @param[in] evt event information
			 */
			void onTranslateXSpinDown( wxSpinEvent& evt );


			// @date 2012.02.21 sugo add onUpButton
			/**
			 * @fn void onAutoYScaleCheck( wxCommandEvent& evt )
			 * @brief This method is called when the 'Auto Y Scale' check box is clicked.
			 * @param[in] evt event information
			 */
			void onAutoYScaleCheck( wxCommandEvent& evt );

			/**
			 * @fn void onUpButton( wxCommandEvent& evt )
			 * @brief This method is called when the up(BMP) button is clicked.
			 * @param[in] evt event information
			 */
			void onUpButton( wxCommandEvent& evt );

			// @date 2012.02.21 sugo add onUpButton
			/**
			 * @fn void onDownButton( wxCommandEvent& evt )
			 * @brief This method is called when the down(BMP) button is clicked.
			 * @param[in] evt event information
			 */
			void onDownButton( wxCommandEvent& evt );

			/**
			 * @fn void setButtonStatus()
			 * @brief ボタンの状態切替
			 */
			void setButtonStatus();

			/**
			 * @fn void onUnSelectAll( wxCommandEvent& evt )
			 * @brief listBoxとキャンバスの選択波形解除
			 * @param[in] evt envent information
			 */
			void onUnSelectAll( wxCommandEvent& evt );

			/**
			 * @fn void onDelete( wxCommandEvent& evt )
			 * @brief 選択された項目を削除します(onRemoveButtonと同様の処理)
			 * @param[in] evt envent information
			 */
			void onDelete( wxCommandEvent& evt );

			/**
			 * @fn void onColor( wxCommandEvent& evt )
			 * @brief 選択された項目の色設定ダイアログを表示します（onColorButtonと同様の処理）
			 * @param[in] evt envent information
			 */
			void onColor( wxCommandEvent& evt );

			/**
			 * @fn void onUpdateUIPopupMenu( wxUpdateUIEvent& evt )
			 * @brief sets the status of popup-menu
			 * @param[in] evt update UI event information
			 */
			void onUpdateUIPopupMenu( wxUpdateUIEvent& evt );

			/**
			 * @fn void onContextMenu( wxContextMenuEvent& evt )
			 * @brief create context Menu
			 */
			void onContextMenu( wxContextMenuEvent& evt );

		private:
			DECLARE_EVENT_TABLE();
		};
	}
}


#endif		// __KOME_WINDOW_OVERLAPPING_DIALOG_H__

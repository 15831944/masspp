/**
 * @file VerticalBoxDialog.h
 * @brief interfaces of VerticalBoxCanvas class
 *
 * @author A.Ozaki(SSD)
 * @date 2013.05.23
 * 
 * Copyright (C) 2014 Shimadzu Corporation All rights reserved.
 */


#ifndef __KOME_WINDOW_VERTICALBOX_DIALOG_H__
#define __KOME_WINDOW_VERTICALBOX_DIALOG_H__


#include <wx/wx.h>
#include <wx/spinctrl.h>


namespace kome
{
	namespace view
	{

		class	kome::window::CanvasBase;
		class	StandardPanel;

		/**
		 * @class VerticalBoxDialog
		 * @brief overlapping dialog class
		 */
		class	VerticalBoxDialog : public kome::window::StandardPanel
		{
		public:
			/**
			 * @fn VerticalBoxDialog( wxWindow* parent )
			 * @brief constructor
			 */
			VerticalBoxDialog( wxWindow* parent );

			/**
			 * @fn virtual ~VerticalBoxDialog()
			 * @brief destructor
			 */
			virtual ~VerticalBoxDialog();

		protected:
			/** chromatogram flag */
			bool	m_bChromFlg;

			/** canvas */
			kome::window::CanvasBase*	m_pCanvas;

			/** list box */
			wxListBox*	m_pList;

			/** Delete button */
			wxBitmapButton*	m_pBitmapButtonDelete;

			/** Up button */
			wxBitmapButton*	m_pBitmapButtonUp;

			/** Down button */
			wxBitmapButton*	m_pBitmapButtonDown;

		public:
			/**
			 * @fn void update( void )
			 * @brief updates dialog
			 */
			void	update( void );

			/**
			 * @fn void select( void )
			 * @brief select spectra or chromatograms
			 */
			void	select( void );

			/**
			 * @fn void setCanvas( CanvasBase* pCanvas )
			 * @brief sets canvas
			 * @param canvas selected canvas
			 */
			void	setCanvas( kome::window::CanvasBase* pCanvas );

			/**
			 * @fn void SelectSpecOfCanvas( kome::objects::Spectrum* pSpec )
			 * @brief reflecting on the listbox the spectrum that selected on the canvas
			 * @param spec selected spectrum object
			 */
			void	SelectSpecOfCanvas( kome::objects::Spectrum* pSpec );

			/**
			 * @fn void SelectChromOfCanvas( kome::objects::Chromatogram* pChrom )
			 * @brief reflecting on the listbox the chromatogram that selected on the canvas
			 * @param chrom selected chromatogram object
			 */
			void	SelectChromOfCanvas( kome::objects::Chromatogram* pChrom );


			/**
			 * @fn void closeSpectrum( kome::objects::Spectrum* pSpec )
			 * @brief close spectrum
			 * @param chrom selected chromatogram object
			 */
			void	closeSpectrum( kome::objects::Spectrum* pSpec );

			/**
			 * @fn void closeChromatgram( kome::objects::Chromatogram* pChrom )
			 * @brief close chromatogram
			 * @param chrom selected chromatogram object
			 */
			void	closeChromatgram( kome::objects::Chromatogram* pChrom );

		protected:
			/**
			 * @fn virtual wxSizer* createMainSizer( )
			 * @brief creates main sizer
			 * @return main sizer
			 */
			virtual wxSizer*	createMainSizer( );

		protected:
			/** 
			 * @fn wxSizer* createEditSizer( void )
			 * @brief creates edit sizer
			 * @return edit sizer
			 */
			wxSizer*	createEditSizer( void );

			/**
			 * @fn void onSelect( wxCommandEvent& evt )
			 * @brief This method is called when list items are selected
			 * @param[in] evt list box event information
			 */
			void	onSelect( wxCommandEvent& evt );

			/**
			 * @fn void onUpdateButton( wxCommandEvent& evt )
			 * @brief This method is called when the udpate button is clicked.
			 * @param[in] evt event information
			 */
			void	onUpdateButton( wxCommandEvent& evt );

			/**
			 * @fn void onRemoveButton( wxCommandEvent& evt )
			 * @brief This method is called when the remove button is clicked.
			 * @param[in] evt event information
			 */
			void	onRemoveButton( wxCommandEvent& evt );
			/**
			 * @fn void onUpButton( wxCommandEvent& evt )
			 * @brief This method is called when the up(BMP) button is clicked.
			 * @param[in] evt event information
			 */
			void	onUpButton( wxCommandEvent& evt );

			/**
			 * @fn void onDownButton( wxCommandEvent& evt )
			 * @brief This method is called when the down(BMP) button is clicked.
			 * @param[in] evt event information
			 */
			void	onDownButton( wxCommandEvent& evt );

			/**
			 * @fn void setButtonStatus( void )
			 * @brief set button status
			 */
			void	setButtonStatus( void );

			/**
			 * @fn void onDelete( wxCommandEvent& evt )
			 * @brief delete selected object
			 * @param[in] evt envent information
			 */
			void	onDelete( wxCommandEvent& evt );

			/**
			 * @fn void onUpdateUIPopupMenu( wxUpdateUIEvent& evt )
			 * @brief sets the status of popup-menu
			 * @param[in] evt update UI event information
			 */
			void	onUpdateUIPopupMenu( wxUpdateUIEvent& evt );

			/**
			 * @fn void onContextMenu( wxContextMenuEvent& evt )
			 * @brief create context Menu
			 */
			void	onContextMenu( wxContextMenuEvent& evt );

		public:
			/**
			 * @fn void onRemoveButton( void )
			 * @brief This method is called when the remove button( or Delete key ) is clicked.
			 * @param[in] evt event information
			 */
			void	onRemoveButton( void );	// @date 2012/12/06 <Add> OKADA

		private:
			DECLARE_EVENT_TABLE( );
		};
	}
}


#endif		// __KOME_WINDOW_VERTICALBOX_DIALOG_H__

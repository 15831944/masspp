/**
 * @file VerticalBoxSettingsViewManager.h
 * @brief interfaces of VerticalBoxSettingsViewManager class
 *
 * @author A.Ozaki
 * @date 2013.05.23
 * 
 * Copyright(C) 2006-2014 Shimadzu Corp., Ltd. All rights reserved.
 */


#ifndef __KOME_VIEW_VERTICALBOX_SETTINGS_VIEW_MANAGER_H__
#define __KOME_VIEW_VERTICALBOX_SETTINGS_VIEW_MANAGER_H__

namespace kome {
	namespace view {

		class VerticalBoxDialog;
		class kome::window::CanvasBase;
		class kome::objects::Spectrum;
		class kome::objects::Chromatogram;

		/**
		 * @class VerticalBoxSettingsViewManager
		 * @brief spectra view management class
		 */
		class VerticalBoxSettingsViewManager : public kome::objects::DefaultDataManager {
		protected:
			/**
			 * @fn VerticalBoxSettingsViewManager()
			 * @brief constructor
			 */
			VerticalBoxSettingsViewManager();

			/**
			 * @fn virtual ~VerticalBoxSettingsViewManager()
			 * @brief destructor
			 */
			virtual ~VerticalBoxSettingsViewManager();

		protected:
			/** VerticalBox dialog */
			VerticalBoxDialog*	m_insVerticalBoxSettingsView;

			/** current canvas */
			kome::window::CanvasBase* m_currentCanvas;

		public:
			/**
			 * @fn void createVerticalBoxSettingsView( void )
			 * @brief creates vertical box settings view
			 */
			void	createVerticalBoxSettingsView( void);

			/**
			 * @fn void selectCanvas( kome::window::CanvasBase* canvas )
			 * @brief selects canvas
			 * @param canvas selected canvas
			 */
			void	selectCanvas( kome::window::CanvasBase* canvas );

			/**
			 * @fn kome::window::CanvasBase* getCurrentCanvas( void )
			 * @brief gets current canvas
			 * @return current canvas
			 */
			kome::window::CanvasBase*	getCurrentCanvas( void );

			/**
			 * @fn void updateView( void )
			 * @brief updates spectra view
			 */
			void	updateView( void );

			/**
			 * @fn void toggleVerticalBoxSettingsViewVisibility( void )
			 * @brief toggles the visibility of the spectra view
			 */
			void	toggleVerticalBoxSettingsViewVisibility( void );

			/**
			 * @fn bool isVisibleVerticalBoxSettingsView( void )
			 * @brief checks the visibility of the spectra view
			 * @return If true, the spectra view is visible.
			 */
			bool	isVisibleVerticalBoxSettingsView( void );

			/**
			 * @fn void releaseView( void )
			 * @brief releases view
			 */
			void	releaseView( void );

			/**
			 * @fn virtual void onCloseSample( Sample* sample, const bool deleting ) 
			 * @brief This method is called when a sample is closed. (override method)
			 * @param[in] sample sample object to be closed
			 * @param[in] deleting If true, the specified object is being deleted now.
			 */
			virtual void	onCloseSample( kome::objects::Sample* sample, const bool deleting );

			/**
			 * @fn virtual void onOpenSpectrum( kome::objects::Spectrum* spec ) 
			 * @brief This method is called when a spectrum is opened. (override method)
			 * @param[in] spec spectrum object to be opened
			 */
			virtual void	onOpenSpectrum( kome::objects::Spectrum* spec );

			/**
			 * @fn virtual void onCloseSpectrum( kome::objects::Spectrum* spec, const bool deleting ) 
			 * @brief This method is called when a spectrum is closed. (override method)
			 * @param[in] spec spectrum object to be opened
			 * @param[in] deleting If true, the specified object is being deleted now.
			 */
			virtual void	onCloseSpectrum( kome::objects::Spectrum* spec, const bool deleting );

			/**
			 * @fn virtual void onOpenChromatogram( kome::objects::Chromatogram* chrom ) 
			 * @brief This method is called when a chromatogram is opened. (override method)
			 * @param[in] chrom chromatogram object to be opened
			 */
			virtual void	onOpenChromatogram( kome::objects::Chromatogram* chrom );

			/**
			 * @fn virtual void onCloseChromatogram( kome::objects::Chromatogram* chrom, const bool deleting ) 
			 * @brief This method is called when a chromatogram is closed. (override method)
			 * @param[in] chrom chromatogram object to be closed
			 * @param[in] deleting If true, the specified object is being deleted now.
			 */
			virtual void	onCloseChromatogram( kome::objects::Chromatogram* chrom, const bool deleting );

			/**
			 * @fn void setSelectionList( kome::objects::Spectrum* spec )
			 * @brief set selection spectrum object
			 * @param spec spectrum object
			 */
			void	setSelectionItem( kome::objects::Spectrum* spec );

			/**
			 * @fn void setSelectionItem( kome::objects::Chromatogram* chrom )
			 * @brief set selection chromatogarm object
			 * @param chrom chromatogram object
			 */
			void	setSelectionItem( kome::objects::Chromatogram* chrom );

			/**
			 * @fn void onSpecKeyUpMgr( kome::objects::Chromatogram* spec )
			 * @brief delete action on keyboard
			 * @param chrom chromatogram object
			 */
			void	onSpecKeyUpMgr( kome::objects::Spectrum* spec );
	

		public:
			/**
			 * @fn static VerticalBoxSettingsViewManager& getInstance( void )
			 * @brief gets spectra view management object
			 * @return spectra view management object (This is the only object.)
			 */
			static VerticalBoxSettingsViewManager&	getInstance( void );
		};
	}
}


#endif		// __KOME_VIEW_VERTICALBOX_SETTINGS_VIEW_MANAGER_H__

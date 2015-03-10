/**
 * @file OverlappingSettingsViewManager.h
 * @brief interfaces of OverlappingSettingsViewManager class
 *
 * @author H.Parry
 * @date 2011.07.14
 * 
 * Copyright(C) 2006-2014 Shimadzu Corp., Ltd. All rights reserved.
 */


#ifndef __KOME_VIEW_OVERLAPPING_SETTINGS_VIEW_MANAGER_H__
#define __KOME_VIEW_OVERLAPPING_SETTINGS_VIEW_MANAGER_H__

namespace kome {
	namespace view {

		class OverlappingDialog;
		class kome::window::CanvasBase;
		class kome::objects::Spectrum;
		class kome::objects::Chromatogram;

		/**
		 * @class OverlappingSettingsViewManager
		 * @brief spectra view management class
		 */
		class OverlappingSettingsViewManager : public kome::objects::DefaultDataManager {
		protected:
			/**
			 * @fn OverlappingSettingsViewManager()
			 * @brief constructor
			 */
			OverlappingSettingsViewManager();

			/**
			 * @fn virtual ~OverlappingSettingsViewManager()
			 * @brief destructor
			 */
			virtual ~OverlappingSettingsViewManager();

		protected:
			/** overlapping dialog */
			OverlappingDialog* m_overlappingSettingsView;

			/** current canvas */
			kome::window::CanvasBase* m_currentCanvas;

		public:
			/**
			 * @fn void createOverlappingSettingsView()
			 * @brief creates overlapping settings view
			 */
			void createOverlappingSettingsView();

			/**
			 * @fn void selectCanvas( kome::window::CanvasBase* canvas )
			 * @brief selects canvas
			 * @param canvas selected canvas
			 */
			void selectCanvas( kome::window::CanvasBase* canvas );

			/**
			 * @fn kome::window::CanvasBase* getCurrentCanvas()
			 * @brief gets current canvas
			 * @return current canvas
			 */
			kome::window::CanvasBase* getCurrentCanvas();

			/**
			 * @fn void updateView( kome::objects::DataSet& dataSet )
			 * @brief updates spectra view
			 */
			void updateView();

			/**
			 * @fn void toggleOverlappingSettingsViewVisibility()
			 * @brief toggles the visibility of the spectra view
			 */
			void toggleOverlappingSettingsViewVisibility();

			/**
			 * @fn bool isVisibleOverlappingSettingsView()
			 * @brief checks the visibility of the spectra view
			 * @return If true, the spectra view is visible.
			 */
			bool isVisibleOverlappingSettingsView();

			/**
			 * @fn void releaseView()
			 * @brief releases view
			 */
			void releaseView();

			/**
			 * @fn virtual void onCloseSample( Sample* sample, const bool deleting ) 
			 * @brief This method is called when a sample is closed. (override method)
			 * @param[in] sample sample object to be closed
			 * @param[in] deleting If true, the specified object is being deleted now.
			 */
			virtual void onCloseSample( kome::objects::Sample* sample, const bool deleting );

			/**
			 * @fn virtual void onOpenSpectrum( kome::objects::Spectrum* spec ) 
			 * @brief This method is called when a spectrum is opened. (override method)
			 * @param[in] spec spectrum object to be opened
			 */
			virtual void onOpenSpectrum( kome::objects::Spectrum* spec ) ;

			/**
			 * @fn virtual void onCloseSpectrum( kome::objects::Spectrum* spec, const bool deleting ) 
			 * @brief This method is called when a spectrum is closed. (override method)
			 * @param[in] spec spectrum object to be opened
			 * @param[in] deleting If true, the specified object is being deleted now.
			 */
			virtual void onCloseSpectrum( kome::objects::Spectrum* spec, const bool deleting ) ;

			/**
			 * @fn virtual void onOpenChromatogram( kome::objects::Chromatogram* chrom ) 
			 * @brief This method is called when a chromatogram is opened. (override method)
			 * @param[in] chrom chromatogram object to be opened
			 */
			virtual void onOpenChromatogram( kome::objects::Chromatogram* chrom ) ;

			/**
			 * @fn virtual void onCloseChromatogram( kome::objects::Chromatogram* chrom, const bool deleting ) 
			 * @brief This method is called when a chromatogram is closed. (override method)
			 * @param[in] chrom chromatogram object to be closed
			 * @param[in] deleting If true, the specified object is being deleted now.
			 */
			virtual void onCloseChromatogram( kome::objects::Chromatogram* chrom, const bool deleting ) ;

			/**
			 * @fn void setSelectionList( kome::objects::Spectrum* spec )
			 * @brief キャンバスで選択されたスペクトルを送る
			 * @param spec spectrum object
			 */
			void setSelectionItem( kome::objects::Spectrum* spec );

			/**
			 * @fn void setSelectionItem( kome::objects::Chromatogram* chrom )
			 * @brief キャンバスで選択されたクロマトグラムを送る
			 * @param chrom chromatogram object
			 */
			void setSelectionItem( kome::objects::Chromatogram* chrom );

			/**
			 * @fn void onSpecKeyUpMgr( kome::objects::Chromatogram* spec )
			 * @brief キーボードで[Delete]が押された場合の処理
			 * @param chrom chromatogram object
			 */
			void onSpecKeyUpMgr( kome::objects::Spectrum* spec );
	

		public:
			/**
			 * @fn static OverlappingSettingsViewManager& getInstance()
			 * @brief gets spectra view management object
			 * @return spectra view management object (This is the only object.)
			 */
			static OverlappingSettingsViewManager& getInstance();
		};
	}
}


#endif		// __KOME_VIEW_OVERLAPPING_SETTINGS_VIEW_MANAGER_H__

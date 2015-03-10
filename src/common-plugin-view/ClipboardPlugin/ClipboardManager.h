/**
 * @file ClipboardManager.h
 * @brief interfaces of ClipboardManager class
 *
 * @author S.Tanaka
 * @date 2007.05.23
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_CLIPBOARD_MANAGER_H__
#define __KOME_CLIPBOARD_MANAGER_H__

#include <vector>
#include <utility>

namespace kome {
	
	namespace clipboard {
		class Clipboard;
		class BaselineSpectrum;
		class ClipboardDialog;

		/**
		 * @class ClipboardManager
		 * @brief linear filter management class
		 */
		class ClipboardManager {
		protected:
			/**
			 * @fn ClipboardManager()
			 * @brief constructor
			 */
			ClipboardManager();

			/**
			 * @fn virtual ~ClipboardManager()
			 * @brief destructor
			 */
			virtual ~ClipboardManager();
		// @date 2011.06.02 <Add> M.Izumi ->
		protected:
			/** properties */
			std::vector< std::pair< std::string, std::string > > m_properties;
			
			/** title */
			std::string m_strTitle;

		// <-
		public:
			/**
			 * @fn void copyXyDataText()
			 * @brief copies xy data text to clipboard
			 * @param xyData xy data
			 */
			void copyXyDataText( kome::core::XYData& xyData );

			/**
			 * @fn void copyBitmap( kome::window::Canvas& canvas )
			 * @brief copies bitmap data to clipboard
			 * @param canvas active canvas
			 */
			void copyBitmap( kome::window::Canvas& canvas );

#ifdef _MSC_VER

			/**
			 * @fn void copyMetafile( kome::window::Canvas& canvas )
			 * @brief copies metafile data to clipboard
			 * @param canvas active canvas
			 */
			void copyMetafile( kome::window::Canvas& canvas );

#endif	// _MSC_VER

		public:
			/**
			 * @fn kome::window::Canvas* getActiveCanvas()
			 * @brief gets active canvas
			 * @return active canvas
			 */
			kome::window::Canvas* getActiveCanvas();

		// @date 2011.06.02 <Add> M.Izumi ->
		protected:
			/**
			 * @fn void selectObject( kome::window::Canvas* canvas )
			 * @brief select object オブジェクトを選択(spec, chrom, group)
			 * @param canvas active canvas
			 */
			void selectObject( kome::window::Canvas* canvas );

			/**
			 * @fn void selectSpec( kome::objects::Spectrum* spectrum )
			 * @brief select spectrum Spectrumのパラメータ情報取得
			 * @param spectrum spectrum object
			 */
			void selectSpec( kome::objects::Spectrum* spectrum );
			
			/**
			 * @fn void selectChrom( kome::objects::Chromatogram* chrom )
			 * @brief select Chromatogram Chromatogramのパラメータ情報取得
			 * @param spectrum Chromatogram object
			 */
			void selectChrom( kome::objects::Chromatogram* chrom );

			/**
			 * @fn void selectGroup( kome::objects::DataGroupNode* group )
			 * @brief select DataGroupNode DataGroupNodeのパラメータ情報取得
			 * @param spectrum DataGroupNode object
			 */
			void selectGroup( kome::objects::DataGroupNode* group );
			
			/**
			 * @fn void setProperties( kome::core::Properties& props, kome::core::Properties& userProps )
			 * @brief sets properties (including user properties)
			 * @param props properties object
			 */
			void setProperties( kome::core::Properties& props, kome::core::Properties& userProps );
		// <-

		public:
			/**
			 * @fn static ClipboardManager& getInstance()
			 * @brief gets linear filter manager
			 * @return linear filter manager class (This is the only object.)
			 */
			static ClipboardManager& getInstance();
		};
	}
}

#endif		// __KOME_CLIPBOARD_MANAGER_H__

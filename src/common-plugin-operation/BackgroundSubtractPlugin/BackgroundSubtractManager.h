/**
 * @file BackgroundSubtractManager.h
 * @brief interfaces of BackgroundSubtractManager class
 *
 * @author S.Tanaka
 * @date 2009.12.08
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_BACKGROUND_SUBTRACT_MANAGER_H__
#define __KOME_BACKGROUND_SUBTRACT_MANAGER_H__


namespace kome {
	namespace background {

		class BackgroundSubtraction;
		class BackgroundSubtractDialog;
		
		/**
		 * @class BackgroundSubtractManager
		 * @brief Internal Standard Correction management class
		 */
		class BackgroundSubtractManager {
		protected:
			/**
			 * @fn BackgroundSubtractManager()
			 * @brief constructor
			 */
			BackgroundSubtractManager();

			/**
			 * @fn virtual ~BackgroundSubtractManager()
			 * @brief destructor
			 */
			virtual ~BackgroundSubtractManager();

		protected:
			/** current background subtraction */
			BackgroundSubtraction* m_currBgSub;

			/** flag */
			bool m_flg;

			/** background subtract dialog */
			BackgroundSubtractDialog* m_dlg;

			/** background spectrum */
			kome::objects::Spectrum* m_spec;

		public:
			
			/**
			 * @fn void setBackground( kome::objects::Spectrum* spec )
			 * @brief sets background
			 * @param[in] spec background spectrum
			 */
			void setBackground( kome::objects::Spectrum* spec );

			/**
			 * @fn BackgroundSubtraction* getCurrentBackgroundSubtraction()
			 * @brief gets current background subtraction
			 * @return current background subtraction
			 */
			BackgroundSubtraction* getCurrentBackgroundSubtraction();				
		
			/**
			 * @fn void setBackgroundFlag()
			 * @brief sets background flag value
			 */
			void setBackgroundFlag();

			/**
			 * @fn void AddSpectrumCanvas()
			 * @brief add spectrum canvas
			 */
			void AddSpectrumCanvas();

			/**
			 * @fn kome::objects::Spectrum* getSpectrum()
			 * @brief add spectrum canvas
			 * @return spec spectrum
			 */
			kome::objects::Spectrum* getSpectrum();

			/**
			 * @fn void closeSpecCanvas(kome::objects::Spectrum* spec)
			 * @brief close Spectrum canvas
			 * @param[in] spec spectrum
			 */
			void closeSpecCanvas( kome::objects::Spectrum* spec );

			/**
			 * @fn void closeMainFrame()
			 * @brief close main frame
			 */
			void closeMainFrame();
		public:
			/**
			 * @fn void openDialog()
			 * @brief displays the background subtract dialog
			 */
			void openDialog();

		public:
			/**
			 * @fn void drawLabel( kome::objects::Spectrum& spec, kome::img::Graphics& g )
			 * @brief draws background label
			 * @param[out] spec spectrum object
			 * @param[out] g graphics object
			 */
			void drawLabel( kome::objects::Spectrum& spec, kome::img::Graphics& g );

		public:
			/**
			 * @fn static BackgroundSubtractManager& getInstance()
			 * @brief gets Internal Standard Correction manager
			 * @return Internal Standard Correction manager class (This is the only object.)
			 */
			static BackgroundSubtractManager& getInstance();
		};
	}
}

#endif		// __KOME_BACKGROUND_SUBTRACT_MANAGER_H__

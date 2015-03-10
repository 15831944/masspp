/**
 * @file BgSubScrollWin.h
 * @brief interfaces of BgSubScrollWin class
 *
 * @author M.Izumi
 * @date 2011.06.22
 * 
 * Copyright (C) 2014 Shimadzu Corporation All rights reserved.
 * <•â‘«à–¾>
 */

#ifndef __KOME_BACKGROUND_BG_SCROLL_WINDOW_H__
#define __KOME_BACKGROUND_BG_SCROLL_WINDOW_H__

#include <vector>
#include <wx/wx.h>
#include "BackgroundSpectrum.h"

namespace kome {
	namespace background {

		/**
		 * @class BgSubScrollWin
		 * @brief BgSubScrollWin class
		 */
		class BgSubScrollWin : public wxScrolledWindow{

		public:
			/**
			 * @fn BgSubScrollWin( wxWindow* parent, int selNo )
			 * @brief constructor
			 */
			BgSubScrollWin( wxWindow* parent, int selNo );

			/**
			 * @fn virtual ~BgSubScrollWin()
			 * @brief destructor
			 */
			virtual ~BgSubScrollWin();
		protected:
			/** spectrum onbjects */
			std::vector< kome::objects::Spectrum* > m_spectra;
			/** radio button */
			wxRadioButton* m_radioBtn;
			/** select radio button number */
			int m_selRadioBtnNo;
			/** select map */
			std::map< wxWindowID , unsigned int > m_selNoMap;
			/** canvas */
			std::vector<kome::window::SpectrumCanvas*> m_canvas;
		protected:
			/**
			 * @fn wxSizer* setMainSizer()
			 * @brief set mamin sizer
			 * @return sizer
			 */
			wxSizer* setMainSizer();

			/**
			 * @fn void onSelectRadioButton( wxCommandEvent& evt )
			 * @brief selected radio button
			 * @param evt information of wxCommandEvent
			 */
			void onSelectRadioButton( wxCommandEvent& evt );

			/**
			 * @fn void setSpectrums()
			 * @brief set spectrum
			 */
			void setSpectrums();
			
			/**
			 * @fn void setBackground()
			 * @brief set back ground
			 */
			void setBackground();

			/**
			 * @fn void removeSpecAndCanvas()
			 * @brief remove spectrum and canvas
			 */
			void removeSpecAndCanvas();
			
		public:
			/**
			 * @fn kome::objects::Spectrum* getSpectrum()
			 * @brief get spectrum
			 * @return spectrum
			 */
			kome::objects::Spectrum* getSpectrum();
			
			/**
			 * @fn int getSelRadioBtnNo()
			 * @brief get select radio button number
			 * @return select index
			 */
			int getSelRadioBtnNo();

		private:
			DECLARE_EVENT_TABLE()
		};

	}
}

#endif		//__KOME_BACKGROUND_BG_SCROLL_WINDOW_H__
 

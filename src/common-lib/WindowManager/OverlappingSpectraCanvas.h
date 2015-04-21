/**
 * @file OverlappingSpectraCanvas.h
 * @brief interfaces of OverlappingSpectraCanvas class
 *
 * @author S.Tanaka
 * @date 2008.04.28
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_WINDOW_OVERLAPPING_SPECTRA_CANVAS_H__
#define __KOME_WINDOW_OVERLAPPING_SPECTRA_CANVAS_H__


#include "SpectrumCanvas.h"

#include <vector>


namespace kome {

	namespace operation {
		class TransformedSpectrum;
		class TransformedChromatogram;
	}

	namespace window {

		/**
		 * @class OverlappingSpectraCanvas
		 * @brief drawing profile of Spectrum panel class
		 */
		class WINDOW_MANAGER_CLASS OverlappingSpectraCanvas : public SpectrumCanvas {
		public:
			/**
			 * @fn OverlappingSpectraCanvas( wxWindow* parent, const int width, const int height )
			 * @brief constructor
			 * @param[in] parent parent frame window
			 * @param[in] width window width
			 * @param[in] height window height
			 */
			OverlappingSpectraCanvas( wxWindow* parent, const int width, const int height );

			/**
			 * @fn virtual ~OverlappingSpectraCanvas()
			 * @brief destructor
			 */
			virtual ~OverlappingSpectraCanvas();
	
		protected:
			/**
			 * @struct SpecInfo
			 * @brief spectrum drawing information
			 */
			struct SpecInfo {
				kome::operation::TransformedSpectrum* spec;
				COLOR color;
				bool flg;
			};

			/** spectrum object */
			std::vector< SpecInfo > m_overlappingSpectra;

			/** dummy spectrum */
			kome::objects::OverlappingSpectrum m_dummy;

			/** focused spectrum */
			kome::objects::Spectrum* m_focusedSpec;

			/** selected spectra */
			std::vector< kome::objects::Spectrum* > m_selectedSpectra;

		protected:
			/** canvas set */
			static std::set< OverlappingSpectraCanvas* > m_canvasSet;

		public:
			/**
			 * @fn void clearSelectedSpectra()
			 * @brief clears selected spectra
			 */
			void clearSelectedSpectra();

			/**
			 * @fn void addSelectedSpec( kome::objects::Spectrum* spec )
			 * @brief adds selected spectrum
			 * @param[in] spec spectrum to be selected
			 */
			void addSelectedSpec( kome::objects::Spectrum* spec );

			/**
			 * @fn void removeSelectedSpec( kome::objects::Spectrum* spec )
			 * @brief removes selected spectrum
			 * @param[in] spec spectrum to be unselected
			 */
			void removeSelectedSpec( kome::objects::Spectrum* spec );

			/**
			 * @fn unsigned int getNumberOfSelectedSpectra()
			 * @brief gets the number of selected spectra
			 * @return the number of selected spectra
			 */
			unsigned int getNumberOfSelectedSpectra();

			/**
			* @fn kome::objects::Spectrum* getSelectedSpec( const unsigned int idx )
			* @brief gets selected spectrum
			* @param[in] idx selected spectrum index
			* @return selected spectrum
			*/
			kome::objects::Spectrum* getSelectedSpec( const unsigned int idx );

		public:
			/**
			 * @fn virtual kome::objects::Spectrum* getBaseSpectrum()
			 * @brief gets base spectrum (override method)
			 * @return base spectrum
			 */
			virtual kome::objects::Spectrum* getBaseSpectrum();

		protected:
			/**
			 * @fn void updateActiveObject()
			 * @brief updates active object
			 */
			void updateActiveObject();

			/**
			 * @fn int searchSelectedSpec( kome::objects::Spectrum* spec )
			 * @brief searches selected spectrum
			 * @param[in] spec spectrum
			 * @return selected spectrum index (If negative value, specified spectrum is not selected.)
			 */
			int searchSelectedSpec( kome::objects::Spectrum* spec );
					
			/**
			 * @fn void setCallFunctionSelectedSpectra( kome::objects::Spectrum* spec )
			 * @brief set call function ( selected spectra )
			 * @param spec spectrum object
			 */
			void setCallFunctionSelectedSpectra( kome::objects::Spectrum* spec );
		
			/**
			 * @fn virtual kome::objects::Spectrum* onAddSpectrum( kome::objects::Spectrum* spec )
			 * @brief This method is called when a spectrum is added
			 * @param[in] spec spectrum
			 * @return spectrum object to be added
			 */
			virtual kome::objects::Spectrum* onAddSpectrum( kome::objects::Spectrum* spec );

			/**
			 * @fn virtual void onRemoveSpectrum( kome::objects::Spectrum* spec )
			 * @brief This method is called when a spectrum is removed
			 * @param[in] spec spectrum
			 */
			virtual void onRemoveSpectrum( kome::objects::Spectrum* spec );

		public:
			/**
			 * @fn void setColor( kome::objects::Spectrum* spec, const COLOR color )
			 * @brief sets colorn
			 * @param[in] spec spectrum
			 * @param[in] color color
			 */
			void setColor( kome::objects::Spectrum* spec, const COLOR color );

			/**
			 * @fn COLOR getColor( const unsigned int index )
			 * @brief gets color
			 * @param[in] index spectrum index
			 * @return color
			 */				
			COLOR getColor( const unsigned int index );
				
			/**
			 * @fn void update()
			 * @brief updates canvas
			 */
			void update();

		protected:
			/**
			 * @fn virtual bool onMouseButtonDown( kome::evt::MouseEvent& evt )
			 * @brief This method is called when mouse button is down (override method)
			 * @param[in] evt mouse event information
			 * @return refresh flag value
			 */
			virtual bool onMouseButtonDown( kome::evt::MouseEvent& evt );

			/**
			 * @fn virtual bool onMouseCursorMove( kome::evt::MouseEvent& evt )
			 * @brief This method is called when mouse cursor is moved. (override method)
			 * @param[in] evt mouse event information
			 * @return refresh flag value
			 */
			virtual bool onMouseCursorMove( kome::evt::MouseEvent& evt );

			/**
			 * @fn virtual void onDrawProfile( kome::window::DCGraphics& g )
			 * @brief This method is called when drawing profile (override method)
			 * @param[in] g graphics
			 */
			virtual void onDrawProfile( kome::window::DCGraphics& g );
						
		protected:
			/**
			 * @fn virtual wxMenu* createMenu()
			 * @brief creates menu (override method)
			 * @return created menu
			 */
			virtual wxMenu* createMenu();

			/**
			 * @fn void onUnSelect( wxCommandEvent& evt )
			 * @brief unselect spectra
			 */
			void onUnSelect( wxCommandEvent& evt );

		private:
			DECLARE_EVENT_TABLE()
		};
	}
}

#endif	//  __KOME_WINDOW_OVERLAPPING_SPECTRA_CANVAS_H__

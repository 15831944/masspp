/**
 * @file OverlappingChromatogramsCanvas.h
 * @brief interfaces of OverlappingChromatogramsCanvas class
 *
 * @author S.Tanaka
 * @date 2008.04.28
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_WINDOW_OVERLAPPING_CHROMATOGRAMS_CANVAS_H__
#define __KOME_WINDOW_OVERLAPPING_CHROMATOGRAMS_CANVAS_H__


#include "ChromatogramCanvas.h"

#include <vector>


namespace kome {

	namespace operation {
		class TransformedSpectrum;
		class TransformedChromatogram;
	}

	namespace window {

		/**
		 * @class OverlappingChromatogramsCanvas
		 * @brief drawing Chromatogram profile panel class
		 */
		class WINDOW_MANAGER_CLASS OverlappingChromatogramsCanvas : public ChromatogramCanvas {
		public:
			/**
			 * @fn OverlappingChromatogramsCanvas( wxWindow* parent, const int width, const int height )
			 * @brief constructor
			 * @param[in] parent parent frame window
			 * @param[in] width window width
			 * @param[in] height window height
			 */
			OverlappingChromatogramsCanvas( wxWindow* parent, const int width, const int height );

			/**
			 * @fn virtual ~OverlappingChromatogramsCanvas()
			 * @brief destructor
			 */
			virtual ~OverlappingChromatogramsCanvas();

		protected:
			/** 
			 * @struct ChromInfo
			 * @brief chromatogram drawing information
			 */
			struct ChromInfo {
				kome::operation::TransformedChromatogram* chrom;				
				COLOR color;
				bool flg;
			};

			/** chromatograms */
			std::vector< ChromInfo > m_overlappingChroms;

			/** dummy chromatogram */
			kome::objects::OverlappingChromatogram m_dummy;

			/** focused chromatogram */
			kome::objects::Chromatogram* m_focusedChrom;

			/** selected chromatograms */
			std::vector< kome::objects::Chromatogram* > m_selectedChroms;

			/** canvas set */
			static std::set< OverlappingChromatogramsCanvas* > m_canvasSet;

		public:
			/**
			 * @fn void clearSelectedChroms()
			 * @brief clears selected chromatograms
			 */
			void clearSelectedChroms();

			/**
			 * @fn void addSelectedChrom( kome::objects::Chromatogram* chrom )
			 * @brief adds selected chromatogram
			 * @param[in] chrom chromatogram to be selected
			 */
			void addSelectedChrom( kome::objects::Chromatogram* chrom );

			/**
			 * @fn void removeSelectedChrom( kome::objects::Chromatogram* chrom )
			 * @brief removes selected chromatogram
			 * @param[in] chrom chromatogram to be unselected
			 */
			void removeSelectedChrom( kome::objects::Chromatogram* chrom );

			/**
			 * @fn unsigned int getNumberOfSelectedChroms()
			 * @brief gets the number of selected chromatograms
			 * @return the number of selected chromatograms
			 */
			unsigned int getNumberOfSelectedChroms();

			/**
			* @fn kome::objects::Chromatogram* getSelectedChrom( const unsigned int idx )
			* @brief gets selected chromatogram
			* @param[in] idx selected chromatogram index
			* @return selected chromatogram
			*/
			kome::objects::Chromatogram* getSelectedChrom( const unsigned int idx );

		public:
			/**
			 * @fn virtual kome::objects::Chromatogram* getBaseChromatogram()
			 * @brief gets base chromatogram (override method)
			 * @return base chromatogram
			 */
			virtual kome::objects::Chromatogram* getBaseChromatogram();

		protected:
			/**
			 * @fn void updateActiveObject()
			 * @brief updates active object
			 */
			void updateActiveObject();

			/**
			 * @fn int searchSelectedChrom( kome::objects::Chromatogram* chrom )
			 * @brief searches selected chromatogram
			 * @param[in] chrom chromatogram
			 * @return selected chromatogram index (If negative value, specified chromatogram is not selected.)
			 */
			int searchSelectedChrom( kome::objects::Chromatogram* chrom );
			
			/**
			 * @fn void setCallFunctionSelectedChromatogram( kome::objects::Chromatogram* chrom )
			 * @brief set call function ( selected chromatogram )
			 * @param[in] chrom chromatogram object.
			 */
			void setCallFunctionSelectedChromatogram( kome::objects::Chromatogram* chrom );
					
			/**
			 * @fn virtual kome::objects::Chromatogram* onAddChromatogram( kome::objects::Chromatogram* chrom )
			 * @brief This method is called when a chromatogram is added.
			 * @param[in] chrom chromatogram
			 * @return chromatogram object to be added
			 */
			virtual kome::objects::Chromatogram* onAddChromatogram( kome::objects::Chromatogram* chrom );

			/** 
			 * @fn virtual void onRemoveChromatogram( kome::objects::Chromatogram* chrom )
			 * @brief This method is called when a chromatogram is removed.
			 * @param[in] chrom chromatogram
			 */
			virtual void onRemoveChromatogram( kome::objects::Chromatogram* chrom );

		public:
			/**
			 * @fn void setColor( kome::objects::Chromatogram* chrom, const COLOR color )
			 * @brief sets color
			 * @param[in] chrom chromatogram
			 * @param[in] color color
			 */
			void setColor( kome::objects::Chromatogram* chrom, const COLOR color );

			/**
			 * @fn COLOR getColor( const unsigned int index )
			 * @brief gets color
			 * @param[in] index chromatogram index
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
			 * @param[in,out] g graphics
			 */
			virtual void onDrawProfile( kome::window::DCGraphics& g );

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

#endif	//  __KOME_WINDOW_OVERLAPPING_CHROMATOGRAMS_CANVAS_H__


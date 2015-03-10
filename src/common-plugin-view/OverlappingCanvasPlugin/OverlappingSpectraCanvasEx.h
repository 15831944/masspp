/**
 * @file OverlappingSpectraCanvasEx.h
 * @brief interfaces of OverlappingSpectraCanvasEx class
 *
 * @author S.Sugo
 * @date 2012.02.21
 * 
 * Copyright (C) 2014 Shimadzu Corporation All rights reserved.
 */

#ifndef __KOME_WINDOW_OVERLAPPING_SPECTRA_CANVAS_EX_H__
#define __KOME_WINDOW_OVERLAPPING_SPECTRA_CANVAS_EX_H__

#include <vector>

namespace kome {

	namespace operation {
		class TransformedSpectrum;
		class TransformedChromatogram;
	}

	namespace window {

		/**
		 * @class OverlappingSpectraCanvasEx
		 * @brief drawing profile of Spectrum panel class
		 */
		class OverlappingSpectraCanvasEx : public SpectrumCanvas {
		public:
			/**
			 * @fn OverlappingSpectraCanvasEx( wxWindow* parent, const int width, const int height )
			 * @brief constructor
			 * @param[in] parent parent frame window
			 * @param[in] width window width
			 * @param[in] height window height
			 */
			OverlappingSpectraCanvasEx( wxWindow* parent, const int width, const int height );

			/**
			 * @fn virtual ~OverlappingSpectraCanvasEx()
			 * @brief destructor
			 */
			virtual ~OverlappingSpectraCanvasEx();

		protected:
			/**
			 * @struct SpecInfo
			 * @brief spectrum drawing information
			 */
			struct SpecInfo {
				kome::operation::TransformedSpectrum* spec;
				COLOR color;
				bool flg;
				// @date 2012.02.22 sugo add -->
				int num;
				// @date 2012.02.22 <-- sugo 
			};

			/** spectrum object */
			std::vector< SpecInfo > m_overlappingSpectra;
			int m_index;
		protected:

			/** dummy spectrum */
			kome::objects::OverlappingSpectrum m_dummy;

			/** focused spectrum */
			kome::objects::Spectrum* m_focusedSpec;

			/** selected spectra */
			std::vector< kome::objects::Spectrum* > m_selectedSpectra;

		protected:
			/** canvas set */
			static std::set< OverlappingSpectraCanvasEx* > m_canvasSet;

			// @date 2012.02.22 sugo add -->
			/** current offset */
			int m_offset;

			/** current angle */
			int m_angle;

			/** current 'Auto Y Scale' */
			bool m_bAutoYScale;

			/** window size change flag */
			bool m_bChangeSize;

			/** length of the X-axis */
			int m_nLengthX;

			/** length of the Y-axis */
			int m_nLengthY;

			/** slant of the X-axis */
			int m_nSlantX;

			/** slant of the Y-axis */
			int m_nSlantY;

			/** zero position of the X-axis */
			int m_nZeroPointX;

			/** zero position of the Y-axis */
			int m_nZeroPointY;
			// @date 2012.02.22 <-- sugo 

			/** color index */
			int m_colorIdx;

			/** colors */
			std::vector< COLOR > m_colors;

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
			
			/**
			 * @fn void setIndex( int index )
			 * @brief set index
			 * @param index
			 */
			void setIndex( int index ){ m_index = index; }
			/**
			 * @fn int getIndex()
			 * @brief get index
			 * @return index
			 */
			int getIndex(){ return m_index; }

			/**
			 * @fn void setColorIdx( int index )
			 * @brief set color index
			 * @param index
			 */
			void setColorIdx( int index ){ m_colorIdx = index; }

			/**
			 * @fn int getColorIdx()
			 * @brief get color index
			 * @return color index
			 */
			int getColorIdx(){ return m_colorIdx; }
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
			 * @fn virtual kome::objects::Spectrum* getBaseSpectrum()
			 * @brief gets base spectrum (override method)
			 * @return base spectrum
			 */
			virtual kome::objects::Spectrum* getBaseSpectrum();

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

			// @date 2012.02.22 sugo add -->
			/**
			 * @fn virtual void onDrawGraphics( wxDC& dc )
			 * @brief this method is called when drawing
			 * @param dc device context to be drawed
			 */
			virtual void onDrawGraphics( wxDC& dc );

			/**
			 * @fn void onDrawWindowFg( kome::window::DCGraphics& g )
			 * @brief This method is called when drawing graph foreground (override method)
			 * @param g graphics
			 */
			virtual void onDrawWindowFg( kome::window::DCGraphics& g );

			/**
			 * @fn virtual void onDraw( wxDC& dc )
			 * @brief This method is called when draw event (override method)
			 * @param[in,out] dc device context
			 */
			virtual void onDraw( wxDC& dc );

			/**
			 * @fn virtual void onSize( const unsigned int width, const unsigned int height )
			 * @brief This method is called when window size is changed
			 * @param[in] width window width
			 * @param[in] height window height
			 */
			virtual void onSize( const unsigned int width, const unsigned int height );
			// @date 2012.02.22 <-- sugo

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

		// @date 2012.02.22 sugo add -->
		public:
			/**
			* @fn int GetOverlappingSpectrumNum( int nNum )
			* @brief gets spectrum number
			* @param[in] nNum positon of number
			* @return spectrum number
			*/
			int GetOverlappingSpectrumNum( unsigned int nNum );

			/**
			 * @fn void exchange(int nS,int ne)
			 * @brief exchange data
			 * @param[in] nS data exchange position 1
			 * @param[in] nE data exchange position 2
			 */
			void exchange(int nS,int nE);

			/**
			 * @fn void SetAutoYScale( bool scale )
			 * @brief set 'Auto Y Scale'
			 * @param[in] scale ON/OFF
			 */
			void SetAutoYScale( bool scale );

			/**
			 * @fn void SetOffset( int offset )
			 * @brief set offset
			 * @param[in] offset Offset(1-100%)
			 */
			void SetOffset( int offset );

			/**
			 * @fn void SetAngle( int angle )
			 * @brief set angle
			 * @param[in] angle Angle(1-180)
			 */
			void SetAngle( int angle );

			/**
			 * @fn int GetAutoYScale(  )
			 * @brief get 'Auto Y Scale'
			 * @return ON/OFF
			 */
			bool GetAutoYScale(  );

			/**
			 * @fn int GetOffset(  )
			 * @brief get offset
			 * @return Offset(1-100%)
			 */
			int GetOffset(  );

			/**
			 * @fn int SetAngle(  )
			 * @brief get angle
			 * @return Angle(1-180)
			 */
			int GetAngle(  );

			/**
			 * @fn double GetMaxY( kome::objects::Spectrum* spec )
			 * @brief get Max Y
			 * @param[in] spec spectrum
			 * @param[in] minX minimum x of range (If minX is negative number, minimum x value is unbounded.)
			 * @param[in] maxX maximum x of range (If maxX is negative number, maximum x value is unbounded.)
			 * @return Max Y
			 */
			double GetMaxY( kome::objects::Spectrum* spec, const double minX = -1.0, const double maxX = -1.0 );
		// @date 2012.02.22 <-- sugo 

		// @date 2012/11/21 <Add> OKADA
		public:
			
			/**
			 * @fn void onMouseRightDown( wxMouseEvent& evt )
			 * @brief for event handler
			 * @param[in] evt Mouse Event
			 */
			void onMouseRightDown( wxMouseEvent& evt );

			/**
			 * @fn void onMouseRightUp( wxMouseEvent& evt )
			 * @brief for event handler
			 * @param[in] evt Mouse Event
			 */
			void onMouseRightUp( wxMouseEvent& evt );

			/**
			 * @fn void onContextMenu( wxContextMenuEvent& evt )
			 * @brief for event handler
			 * @param[in] evt Mouse Event
			 */
			void onContextMenu( wxContextMenuEvent& evt );

			/**
			 * @fn void onContextMenu( wxMouseEvent& evt )
			 * @brief for event handler
			 * @param[in] evt Mouse Event
			 */
			void onContextMenu( wxMouseEvent& evt );

			/**
			 * @fn void onContextMenu( wxPoint ptScreen )
			 * @brief for event handler
			 * @param[in] location of right click point
			 */
			void onContextMenu( wxPoint ptScreen );

			/**
			 * @fn void onMenuPopup( wxCommandEvent& evt );
			 * @brief clicked popup menu
			 * @param[in] evt information of wxCommandEvent
			 */
			void onMenuPopup( wxCommandEvent& evt );

			/**
			 * @fn void onOverlappingSettings( wxUpdateUIEvent& evt )
			 * @brief Overlapping Settings ‚ªƒNƒŠƒbƒN‚³‚ê‚½‚Æ‚«ŒÄ‚Î‚ê‚é
			 * @param[in] evt Mouse Event
			 */
			void onOverlappingSettings( wxCommandEvent& evt );

			/**
			 * @fn void onUpdatePopupMenu( wxUpdateUIEvent& evt )
			 * @brief sets the status of popup-menu
			 * @param[in] evt update UI event information
			 */
			void onUpdatePopupMenu( wxUpdateUIEvent& evt );

		private:
			DECLARE_EVENT_TABLE()
		};
	}
}

#endif	//  __KOME_WINDOW_OVERLAPPING_SPECTRA_CANVAS_EX_H__

/**
 * @file OverlappingChromatogramsCanvasEx.h
 * @brief interfaces of OverlappingChromatogramsCanvasEx class
 *
 * @author S.Sugo
 * @date 2012.02.24
 * 
 * Copyright (C) 2014 Shimadzu Corporation All rights reserved.
 */


#ifndef __KOME_WINDOW_OVERLAPPING_CHROMATOGRAMS_CANVAS_EX_H__
#define __KOME_WINDOW_OVERLAPPING_CHROMATOGRAMS_CANVAS_EX_H__


#include <vector>


namespace kome {

	namespace operation {
		class TransformedSpectrum;
		class TransformedChromatogram;
	}

	namespace window {

		/**
		 * @class OverlappingChromatogramsCanvasEx
		 * @brief drawing Chromatogram profile panel class
		 */
		class OverlappingChromatogramsCanvasEx : public ChromatogramCanvas {
		public:
			/**
			 * @fn OverlappingChromatogramsCanvasEx( wxWindow* parent, const int width, const int height )
			 * @brief constructor
			 * @param[in] parent parent frame window
			 * @param[in] width window width
			 * @param[in] height window height
			 */
			OverlappingChromatogramsCanvasEx( wxWindow* parent, const int width, const int height );

			/**
			 * @fn virtual ~OverlappingChromatogramsCanvasEx()
			 * @brief destructor
			 */
			virtual ~OverlappingChromatogramsCanvasEx();

		protected:
			/** 
			 * @struct ChromInfo
			 * @brief chromatogram drawing information
			 */
			struct ChromInfo {
				kome::operation::TransformedChromatogram* chrom;
				COLOR color;
				// @date 2012.02.24 sugo add -->
				int num;
				// @date 2012.02.24 <-- sugo 
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
			static std::set< OverlappingChromatogramsCanvasEx* > m_canvasSet;

			// @date 2012.02.24 sugo add -->
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
			// @date 2012.02.24 <-- sugo 

			/** color index */
			int m_colorIdx;

			/** colors */
			std::vector< COLOR > m_colors;
			/** index */
			int m_index;

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
			 * @brief set corlor index
			 * @param index
			 */
			void setColorIdx( int index ){ m_colorIdx = index; }

			/**
			 * @fn int getColorIdx()
			 * @brief get color index
			 * @retrn color index
			 */
			int getColorIdx(){ return m_colorIdx; }

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
			 * @fn virtual kome::objects::Chromatogram* getBaseChromatogram()
			 * @brief gets base chromatogram (override method)
			 * @return base chromatogram
			 */
			virtual kome::objects::Chromatogram* getBaseChromatogram();
			
			/**
			 * @fn void setCallFunctionSelectedSpectra( kome::objects::Spectrum* spec )
			 * @brief set call function ( selected chromatogram )
			 * @param spec spectrum object
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

			// @date 2012.02.24 sugo add -->
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
			// @date 2012.02.24 <-- sugo

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

		// @date 2012.02.24 sugo add -->
		public:
			/**
			* @fn int GetOverlappingChromatogramNum( int nNum )
			* @brief gets chromatogram number
			* @param[in] nNum positon of number
			* @return chromatogram number
			*/
			int GetOverlappingChromatogramNum( unsigned int nNum );

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
			 * @fn double GetMaxY( kome::objects::Chromatogram* chrom )
			 * @brief get Max Y
			 * @param[in] chrom chromatogram to be selected
			 * @param[in] minX minimum x of range (If minX is negative number, minimum x value is unbounded.)
			 * @param[in] maxX maximum x of range (If maxX is negative number, maximum x value is unbounded.)
			 * @return Max Y
			 */
			double GetMaxY( kome::objects::Chromatogram* chrom, const double minX = -1.0, const double maxX = -1.0 );
		// @date 2012.02.24 <-- sugo

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

#endif	//  __KOME_WINDOW_OVERLAPPING_CHROMATOGRAMS_CANVAS_EX_H__

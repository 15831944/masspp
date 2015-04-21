/**
 * @file ChromatogramCanvas.h
 * @brief interfaces of ChromatogramCanvas class
 *
 * @author S.Tanaka
 * @date 2008.04.28
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_WINDOW_CHROMATOGRAM_CANVAS_H__
#define __KOME_WINDOW_CHROMATOGRAM_CANVAS_H__


#include "ProfileCanvas.h"

#include <vector>


namespace kome {
	namespace window {
		/**
		 * @class ChromatogramCanvas
		 * @brief drawing Chromatogram profile panel class
		 */
		class WINDOW_MANAGER_CLASS ChromatogramCanvas : public ProfileCanvas {
		public:
			/**
			 * @fn ChromatogramCanvas(
					wxWindow* parent,
					const int width,
					const int height,
					kome::objects::Chromatogram* chrom
				)
			 * @brief constructor
			 * @param[in] parent parent frame window
			 * @param[in] width window width
			 * @param[in] height window height
			 * @param[in] chrom chromatogram
			 */
			ChromatogramCanvas(
				wxWindow* parent,
				const int width,
				const int height,
				kome::objects::Chromatogram* chrom
			);

			/**
			 * @fn virtual ~ChromatogramCanvas()
			 * @brief destructor
			 */
			virtual ~ChromatogramCanvas();

		protected:
			/** canvas set */
			static std::set< ChromatogramCanvas* > m_canvasSet;

		public:
			/**
			 * @fn virtual kome::objects::Chromatogram* getBaseChromatogram()
			 * @brief gets base chromatogram
			 * @return base chromatogram
			 */
			virtual kome::objects::Chromatogram* getBaseChromatogram();

		protected:
			/**
			 * @fn virtual bool onMouseButtonDown( kome::evt::MouseEvent& evt )
			 * @brief This method is called when mouse button is down (override method)
			 * @param[in] evt mouse event information
			 * @return refresh flag value
			 */
			virtual bool onMouseButtonDown( kome::evt::MouseEvent& evt );

			/**
			 * @fn virtual bool onMouseButtonUp( kome::evt::MouseEvent& evt )
			 * @brief This method is called when mouse button is up (override method)
			 * @param[in] evt mouse event information
			 * @return refresh flag value
			 */
			virtual bool onMouseButtonUp( kome::evt::MouseEvent& evt );

			/**
			 * @fn virtual bool onMouseDoubleClick( kome::evt::MouseEvent& evt )
			 * @brief This method is called when mouse button is double clicked. (override method)
			 * @param[in] evt mouse event information
			 * @return refresh flag value
			 */
			virtual bool onMouseDoubleClick( kome::evt::MouseEvent& evt );

			/**
			 * @fn virtual bool onMouseCursorMove( kome::evt::MouseEvent& evt )
			 * @brief This method is called when mouse cursor is moved. (override method)
			 * @param[in] evt mouse event information
			 * @return refresh flag value
			 */
			virtual bool onMouseCursorMove( kome::evt::MouseEvent& evt );

			/**
			 * @fn virtual bool onMouseCursorEnter( kome::evt::MouseEvent& evt )
			 * @brief This method is called when mouse cursor enters. (override method)
			 * @param[in] evt mouse event information
			 * @return refresh flag value
			 */
			virtual bool onMouseCursorEnter( kome::evt::MouseEvent& evt );

			/**
			 * @fn virtual bool onMouseCursorLeave( kome::evt::MouseEvent& evt )
			 * @brief This method is called when mouse cursor leaves. (override method)
			 * @param[in] evt mouse event information
			 * @return refresh flag value
			 */
			virtual bool onMouseCursorLeave( kome::evt::MouseEvent& evt );

			/**
			 * @fn virtual bool onKeyDown( kome::evt::KeyEvent& evt )
			 * @brief This method is called when a key is down
			 * @param[in] evt key event information
			 * @return refresh flag value
			 */
			virtual bool onKeyDown( kome::evt::KeyEvent& evt );

			/**
			 * @fn virtual bool onKeyUp( kome::evt::KeyEvent& evt )
			 * @brief This method is called when a key is up
			 * @param[in] evt key event information
			 * @return refresh flag value
			 */
			virtual bool onKeyUp( kome::evt::KeyEvent& evt );

		protected:
			/**
			 * @fn virtual void onPrepareDraw( kome::window::DCGraphics& g )
			 * @brief this method is called when prepareing (override method)
			 * @param[in,out] g graphics object to draw
			 */
			virtual void onPrepareDraw( kome::window::DCGraphics& g );

			/**
			 * @fn virtual void onDrawWindowBg( kome::window::DCGraphics& g )
			 * @brief This method is called when drawing window background (override method)
			 * @param[in,out] g graphics
			 */
			virtual void onDrawWindowBg( kome::window::DCGraphics& g );

			/**
			 * @fn virtual void onDrawGraphBg( kome::window::DCGraphics& g )
			 * @brief This method is called when drawing graph backgorund (override method)
			 * @param[in,out] g graphics
			 */
			virtual void onDrawGraphBg( kome::window::DCGraphics& g );

			/**
			 * @fn virtual void onDrawProfile( kome::window::DCGraphics& g )
			 * @brief This method is called when drawing profile (override method)
			 * @param[in,out] g graphics
			 */
			virtual void onDrawProfile( kome::window::DCGraphics& g );

			/**
			 * @fn virtual void onDrawGraphFg( kome::window::DCGraphics& g )
			 * @brief This method is called when drawing graph foregorund (override method)
			 * @param[in,out] g graphics
			 */
			virtual void onDrawGraphFg( kome::window::DCGraphics& g );

			/**
			 * @fn virtual void onDrawWindowFg( kome::window::DCGraphics& g )
			 * @brief This method is called when drawing graph foreground (override method)
			 * @param[in,out] g graphics
			 */
			virtual void onDrawWindowFg( kome::window::DCGraphics& g );

			/** 
			 * @fn virtual void onDrawChromatogramProfile(
						kome::window::DCGraphics& g,
						kome::objects::Chromatogram& chrom
					)
			 * @brief draws spectrum profile
			 * @param[in,out] g graphics
			 * @param[in] chrom chromatogram
			 */
			virtual void onDrawChromatogramProfile(
					kome::window::DCGraphics& g,
					kome::objects::Chromatogram& chrom
			);

		protected:
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
			 * @fn static ChromatogramCanvas* getCanvas( kome::objects::Chromatogram* chrom )
			 * @brief gets canvas that has specified chromatogram
			 * @param[in] chrom chromatogram
			 * @return chromatogram canvas. (If NULL, there is no canvas that has specified chromatogram.)
			 */
			static ChromatogramCanvas* getCanvas( kome::objects::Chromatogram* chrom );

			/**
			 * @fn static void destroyAllCanvas()
			 * @brief destroys all canvas
			 */
			static void destroyAllCanvas();

		protected:
			/**
			 * @fn wxMenu* createMenu()
			 * @brief creates menu
			 * @return created menu
			 */
			wxMenu* createMenu();


		// Context Menu
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
			 * @param[in] ptScreen location of right click point
			 */
			void onContextMenu( wxPoint ptScreen );

			/**
			 * @fn void onMenuPopup( wxCommandEvent& evt )
			 * @brief for event handler
			 * @param[in] evt Mouse Event
			 */
			void onMenuPopup( wxCommandEvent& evt );

		private:
			DECLARE_EVENT_TABLE()
		};
	}
}

#endif	//  __KOME_WINDOW_CHROMATOGRAM_CANVAS_H__

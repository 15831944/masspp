/**
 * @file SpectrumCanvas.h
 * @brief interfaces of SpectrumCanvas class
 *
 * @author S.Tanaka
 * @date 2008.04.28
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_WINDOW_SPECTRUM_CANVAS_H__
#define __KOME_WINDOW_SPECTRUM_CANVAS_H__


#include "ProfileCanvas.h"

#include <vector>


namespace kome {
	namespace window {

		/**
		 * @class SpectrumCanvas
		 * @brief drawing profile of Spectrum panel class
		 */
		class WINDOW_MANAGER_CLASS SpectrumCanvas : public ProfileCanvas {
		public:
			/**
			 * @fn SpectrumCanvas(
					wxWindow* parent,
					const int width,
					const int height,
					kome::objects::Spectrum* spec
				)
			 * @brief constructor
			 * @param[in] parent parent frame window
			 * @param[in] width window width
			 * @param[in] height window height
			 * @param[in] spec spectrum
			 */
			SpectrumCanvas(
				wxWindow* parent,
				const int width,
				const int height,
				kome::objects::Spectrum* spec
			);

			/**
			 * @fn virtual ~SpectrumCanvas()
			 * @brief destructor
			 */
			virtual ~SpectrumCanvas();
	
		protected:
			/** canvas set */
			static std::set< SpectrumCanvas* > m_canvasSet;

		public:
			/**
			 * @fn virtual kome::objects::Spectrum* getBaseSpectrum()
			 * @brief gets base spectrum
			 * @return base spectrum
			 */
			virtual kome::objects::Spectrum* getBaseSpectrum();

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
			 * @fn virtual void onDrawGraphFg( kome::window::DCGraphics& g )
			 * @brief This method is called when drawing graph foreground (override method)
			 * @param[in,out] g graphics
			 */
			virtual void onDrawWindowFg( kome::window::DCGraphics& g );

			/** 
			 * @fn virtual void onDrawSpectrumProfile(
						kome::window::DCGraphics& g,
						kome::objects::Spectrum& spec
					)
			 * @brief draws spectrum profile
			 * @param[in,out] g graphics
			 * @param[in] spec spectrum
			 */
			virtual void onDrawSpectrumProfile(
				kome::window::DCGraphics& g,
				kome::objects::Spectrum& spec
			);

		protected:
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
			 * @fn static SpectrumCanvas* getCanvas( kome::objects::Spectrum* spec )
			 * @brief gets canvas that has specified spectrum
			 * @param[in] spec spectrum
			 * @return spectrum canvas. (If NULL, there is no canvas that has specified spectrum.)
			 */
			static SpectrumCanvas* getCanvas( kome::objects::Spectrum* spec );

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

#endif	//  __KOME_WINDOW_SPECTRUM_CANVAS_H__

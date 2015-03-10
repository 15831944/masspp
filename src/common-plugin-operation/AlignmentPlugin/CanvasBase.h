/**
 * @file CanvasBase.h
 * @brief interfaces of CanvasBase class
 *
 * @author S.Tanaka
 * @date 2007.08.27
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_ALIGNMENT_CANVAS_BASE_H__
#define __KOME_ALIGNMENT_CANVAS_BASE_H__


#include <wx/wx.h>


namespace kome {
	namespace alignment {

		/**
		 * @class CanvasBase
		 * @brief base panel class
		 */
		class CanvasBase : public kome::window::Canvas {
		public:
			/**
			 * @fn CanvasBase( wxWindow* parent, const int width, const int height )
			 * @brief constructor
			 * @param parent parent window
			 * @param width window width
			 * @param height window height
			 */
			CanvasBase( wxWindow* parent, const int width, const int height );

			/**
			 * @fn virtual ~CanvasBase()
			 * @brief destructor
			 */
			virtual ~CanvasBase();

		protected:
			/** graphics */
			kome::window::DCGraphics m_graphics;

			/** x label flag */
			bool m_xLabelFlg;
			/** y label flag */
			bool m_yLabelFlg;
			/** x grid flag */
			bool m_xGridFlg;
			/** y grid flag */
			bool m_yGridFlg;
			/** position display flag */
			bool m_posFlg;
			/** graph title */
			std::string m_title;

			/** base zero */
			bool m_baseZeroFlg;

		protected:
			/**
			 * @fn virtual void getChromatograms(
					std::vector< kome::objects::Chromatogram* >& chroms,
					std::vector< COLOR >& colors
				)
			 * @brief gets chromatograms
			 * @param chroms the array object to store chromatograms
			 * @param colors the array object to store colors to draw profile
			 */
			virtual void getChromatograms(
				std::vector< kome::objects::Chromatogram* >& chroms,
				std::vector< COLOR >& colors
			);

		protected:
			/**
			 * @fn void setGraphRange( std::vector< kome::objects::Chromatogram* >& chroms )
			 * @brief sets graph range
			 * @param chroms chromatograms
			 */
			void setGraphRange( std::vector< kome::objects::Chromatogram* >& chroms );

		protected:
			/**
			 * @fn virtual void onDraw( wxDC& dc )
			 * @brief This method is called when draw event (override method)
			 * @param dc device context
			 */
			virtual void onDraw( wxDC& dc );

			/**
			 * @fn virtual void onSize( const unsigned int width, const unsigned int height )
			 * @brief This method is called when window size is changed (override method)
			 * @param width window width
			 * @param height window height
			 */
			virtual void onSize( const unsigned int width, const unsigned int height );

			/**
			 * @fn virtual bool onMouseButtonDown( kome::evt::MouseEvent& evt )
			 * @brief This method is called when mouse button is down (override method)
			 * @param evt mouse event information
			 * @return refresh flag value
			 */
			virtual bool onMouseButtonDown( kome::evt::MouseEvent& evt );

			/**
			 * @fn virtual bool onMouseButtonUp( kome::evt::MouseEvent& evt )
			 * @brief This method is called when mouse button is up (override method)
			 * @param evt mouse event information
			 * @return refresh flag value
			 */
			virtual bool onMouseButtonUp( kome::evt::MouseEvent& evt );

			/**
			 * @fn virtual bool onMouseDoubleClick( kome::evt::MouseEvent& evt )
			 * @brief This method is called when mouse button is double clicked. (override method)
			 * @param evt mouse event information
			 * @return refresh flag value
			 */
			virtual bool onMouseDoubleClick( kome::evt::MouseEvent& evt );

			/**
			 * @fn virtual bool onMouseCursorMove( kome::evt::MouseEvent& evt )
			 * @brief This method is called when mouse cursor is moved. (override method)
			 * @param evt mouse event information
			 * @return refresh flag value
			 */
			virtual bool onMouseCursorMove( kome::evt::MouseEvent& evt );

			/**
			 * @fn virtual bool onMouseCursorEnter( kome::evt::MouseEvent& evt )
			 * @brief This method is called when mouse cursor enters. (override method)
			 * @param evt mouse event information
			 * @return refresh flag value
			 */
			virtual bool onMouseCursorEnter( kome::evt::MouseEvent& evt );

			/**
			 * @fn virtual bool onMouseCursorLeave( kome::evt::MouseEvent& evt )
			 * @brief This method is called when mouse cursor leaves. (override method)
			 * @param evt mouse event information
			 * @return refresh flag value
			 */
			virtual bool onMouseCursorLeave( kome::evt::MouseEvent& evt );

		private:
			DECLARE_EVENT_TABLE()
		};
	}
}

#endif		// __KOME_ALIGNMENT_CANVAS_BASE_H__

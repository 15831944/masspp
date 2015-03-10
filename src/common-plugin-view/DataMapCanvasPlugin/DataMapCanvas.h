/**
 * @file DataMapCanvas.h
 * @brief interfaces of DataMapCanvas class
 *
 * @author S.Tanaka
 * @date 2007.09.24
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_VIEW_DATA_MAP_CANVAS_H__
#define __KOME_VIEW_DATA_MAP_CANVAS_H__


#include <vector>
#include <stack>


namespace kome {
	namespace view {

		/**
		 * @class DataMapCanvas
		 * @brief drawing profile of DataMap panel class
		 */
		class DataMapCanvas : public kome::window::Canvas {
		public:
			/**
			 * @fn DataMapCanvas( wxWindow* parent, kome::objects::DataMapInfo& dataMap )
			 * @brief constructor
			 * @param parent parent frame window
			 * @param dataMap data map
			 */
			DataMapCanvas( wxWindow* parent, kome::objects::DataMapInfo& dataMap );

			/**
			 * @fn virtual ~DataMapCanvas()
			 * @brief destructor
			 */
			virtual ~DataMapCanvas();
	
		protected:
			/** canvas set */
			static std::set< DataMapCanvas* > m_canvasSet;

			/** graphics object to draw */
			kome::window::DCGraphics m_graphics;

		public:
			/**
			 * @fn void updatePoints( kome::objects::DataMapInfo* dataMap )
			 * @brief updates data points
			 * @param dataMap data map information
			 */
			void updatePoints( kome::objects::DataMapInfo* dataMap );

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

		protected:
			/**
			 * @fn virtual void onDrawGraphics( wxDC& dc )
			 * @brief this method is called when drawing (override method)
			 * @param dc device context to be drawed
			 */
			virtual void onDrawGraphics( wxDC& dc );

		protected:
			/**
			 * @fn virtual void onAddDataMap( kome::objects::DataMapInfo* dataMap )
			 * @brief This method is called when a data map is added.
			 * @param dataMap data map to be added
			 */
			virtual kome::objects::DataMapInfo* onAddDataMap( kome::objects::DataMapInfo* dataMap );

			/**
			 * @fn virtual void onRemoveDataMap( kome::objects::DataMapInfo* dataMap )
			 * @brief This method is called when a data map is removed
			 * @param dataMap data map to be removed
			 */
			virtual void onRemoveDataMap( kome::objects::DataMapInfo* dataMap );

		public:
			/**
			 * @fn static DataMapCanvas* getCanvas( kome::objects::DataMapInfo* dataMap )
			 * @brief gets canvas that has dataMapified data map
			 * @param dataMap data map
			 * @return data map canvas. (If NULL, there is no canvas that has dataMapified data map.)
			 */
			static DataMapCanvas* getCanvas( kome::objects::DataMapInfo* dataMap );

			/**
			 * @fn int convertPos2Data( double x, double y, double * p_dX, double * p_dY )
			 * @brief conbert dot to RT or m/z.
			 * @param double x holizontal pixel location on window
			 * @param double y vertical pixel location on window
			 * @param double * p_dX m/z
			 * @param double * p_dY RT
			 * @return 0:success other:error
			 */
			int convertPos2Data( double x, double y, double * p_dX, double * p_dY );

			/**
			 * @fn int convertData2Pos( double dMz, double dRt, double * p_dX, double * p_dY )
			 * @brief conbert RT or m/z to dot.
			 * @param double dMz m/z
			 * @param double dRt RT
			 * @param double * p_dX holizontal pixel location on window
			 * @param double * p_dY vertical pixel location on window
			 * @return 0:success other:error
			 */
			int convertData2Pos( double dMz, double dRt, double * p_dX, double * p_dY );

			/**
			 * @fn int getGraphPosition( int *top, int *bottom, int *left, int *right )
			 * @brief get graph area
			 * @param int *top top location
			 * @param int *bottom bottom location
			 * @param int *left left location 
			 * @param int *right right location
			 * @return 0:success other:error
			 */
			int getGraphPosition( int *top, int *bottom, int *left, int *right );

			/**
			 * @fn bool isTooSmall( void )
			 * @brief check graph size
			 * @return true:"too small" false:display OK
			 */
			bool isTooSmall( void );

		protected:
			/**
			 * @fn virtual wxMenu* createMenu()
			 * @brief creates menu
			 * @return created menu
			 */
			wxMenu* createMenu();

		private:
			DECLARE_EVENT_TABLE()
		};
	}
}

#endif	//  __KOME_VIEW_DATA_MAP_CANVAS_H__

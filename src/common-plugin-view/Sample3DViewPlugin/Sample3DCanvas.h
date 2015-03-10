/**
 * @file Sample3DCanvas.h
 * @brief interfaces of Sample3DCanvas class
 *
 * @author S.Tanaka
 * @date 2008.01.21
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_VIEW_RAW_DATA_3D_CANVAS_H__
#define __KOME_VIEW_RAW_DATA_3D_CANVAS_H__


#include <wx/glcanvas.h>
#include <wx/timer.h>


#define	PLUGIN_NAME_3D_VIEW		"Raw Data 3D View"

// plugin.xml内で定義されたアイコン名
#define	ICON_NAME_3D_DIRECTION	"3d_direction"	
#define	ICON_NAME_3D_ZOOM		"3d_zoom"


namespace kome {
	namespace view {

		/**
		 * @class Sample3DCanvas
		 * @brief drawing 3D view canvas class
		 */
		class Sample3DCanvas : public wxGLCanvas {
		public:
			/**
			 * @fn Sample3DCanvas( kome::window::ChildFrame* frame )
			 * @brief constructor
			 * @param frame parent frame window
			 */
			Sample3DCanvas( wxWindow* parent, kome::objects::DataMapInfo& dataMap );

			/**
			 * @fn virtual ~Sample3DCanvas()
			 * @brief destructor
			 */
			virtual ~Sample3DCanvas();

		protected:
			/** initialize flag */
			bool m_init;

			/** graph information */
			kome::gl::GLDataMapInfo m_graphInfo;

			/** previous mouse point */
			kome::core::Point< int > m_prevPt;

			/** timer */
			wxTimer* m_timer;

			/** preparing animation */
			bool m_preparingAnimation;

			/** animation rotation */
			kome::core::Point< int > m_animationD;

		protected:
			/** 3D canvases */
			static std::vector< Sample3DCanvas* > m_canvases;
			
		protected:		
			/** mouse left button status */
			bool m_isLeftDown;

			/** key borad shibt key down */
			bool m_isShiftDown;

		public:
			/**
			 * @fn kome::gl::GLDataMapInfo& getGraphInfo()
			 * @brief gets graph information
			 * @return graph information object
			 */
			kome::gl::GLDataMapInfo& getGraphInfo();

		protected:
			/**
			 * @fn void initGL()
			 * @brief initializes OpenGL
			 */
			void initGL();

			/**
			 * @fn void render()
			 * @brief renders 3D view
			 */
			void render();

			/**
			 * @fn void look()
			 * @brief sets the position and direction of camera
			 */
			void look();

			/**
			 * @fn void light()
			 * @brief sets light
			 */
			void light();

			/**
			 * @fn void texture()
			 * @brief sets texture
			 */
			void texture();

			/**
			 * @fn void drawGraph()
			 * @brief draws graph
			 */
			void drawGraph();

			/**
			 * @fn void drawSurfaces()
			 * @brief draws surfaces
			 */
			void drawSurfaces();

			/**
			 * @fn void drawAxis()
			 * @brief draws axis
			 */
			void drawAxis();

			/**
			 * @fn void drawText(
						const char* s,
						const float x,
						const float y,
						const float z,
						const kome::img::Graphics::TextHAlign hAlign,
						const kome::img::Graphics::TextVAlign vAlign
				)
			 * @brief draws text
			 * @param s character string
			 * @param x x element of text position
			 * @param y y element of text position
			 * @param z z element of text position
			 * @param hAlign horizontal align
			 * @param vAlign vertical align
			 */
			void drawText(
					const char* s,
					const float x,
					const float y,
					const float z,
					const kome::img::Graphics::TextHAlign hAlign,
					const kome::img::Graphics::TextVAlign vAlign
			);

			/**
			 * @fn void drawAxisTitle(
					const char* s,
					const float x,
					const float y,
					const float z
				)
			 * @brief draws axis title
			 * @param s axis title
			 * @param x x element of text position
			 * @param y y element of text position
			 * @param z z element of text position
			 */
			void drawAxisTitle(
				const char* s,
				const float x,
				const float y,
				const float z
			);

			/**
			 * @fn void setSize( unsigned int w, unsigned int h )
			 * @brief sets size
			 * @param w window width
			 * @param h window height
			 */
			void setSize( unsigned int w, unsigned int h );

		protected:
			/**
			 * @fn void moveEye( const int dx, const int dy )
			 * @brief moves eye
			 * @param dx x element of the mouse move
			 * @param dy y element of the mouse move
			 */
			void moveEye( const int dx, const int dy );

			/**
			 * @fn void rotate( const int dx, const int dy )
			 * @brief rotates graph (free rotation)
			 * @param dx x element of the mouse move
			 * @param dy y element of the mouse move
			 */
			void rotate( const int dx, const int dy );

			/**
			 * @fn void rotate2( const int dx, const int dy )
			 * @brief rotates graph
			 * @param dx x element of the mouse move
			 * @param dy y element of the mouse move
			 */
			void rotate2( const int dx, const int dy );

			/**
			 * @fn void showMenu( const int px, const int py )
			 * @brief shows menu
			 * @param px x element of menu position
			 * @param py y element of menu position
			 */
			void showMenu( const int px, const int py );
						
			/**
			 * @fn void calcDrawAxisTitle( 
					double min, 
					double max,
					float range,
					float na,
					float ns, 
					float n,
					bool bflg,
					double delta,
					int axis_n,
					float xs,
					float ys,
					double d1 = 0.0,
					double d2 = 0.0,
					double p = 0.0	)
			 * @brief 各軸の数値を計算して表示
			 * @param min 最小値
			 * @param max 最大値
			 * @param range　range
			 * @param na 各表示位置
			 * @param ns 範囲
			 * @param n  計算しない方の位置 
			 * @param bflg 各回転位置？
			 * @param delta delta
			 * @param axis_n 計算する軸
			 * @param xs -range
			 * @param ys -range
			 * @param d1 x軸の表示方向
			 * @param d2 y軸の表示方向
			 * @param p dir.dot( zdir )
			 */
			void calcDrawAxisTitle( 
				double min, 
				double max,
				float range,
				float na,
				float ns, 
				float n,
				bool bflg,
				double delta,
				int axis_n,
				float xs,
				float ys,
				double d1 = 0.0,
				double d2 = 0.0,
				double p = 0.0
			);

		public:
			/**
			 * @fn void killTimer()
			 * @brief kills timer
			 */
			void killTimer();

		protected:
			/**
			 * @fn void onPaint( wxPaintEvent& evt )
			 * @brief This method is called on drawing
			 * @param evt event information
			 */
			void onPaint( wxPaintEvent& evt );

			/**
			 * @fn void onSize( wxSizeEvent& evt )
			 * @brief This method is called when window size is changed.
			 * @param evt event information
			 */
			void onSize( wxSizeEvent& evt );

			/**
			 * @fn void onMouse( wxMouseEvent& evt )
			 * @brief This method is called on mouse event
			 * @param evt event information
			 */
			void onMouse( wxMouseEvent& evt );

			/**
			 * @fn void onEraseBackground( wxEraseEvent& evt )
			 * @brief this method is called by erase background event.
			 * @param evt erase background event information
			 */
			void onEraseBackground( wxEraseEvent& evt );

			/**
			 * @fn void  onTimer( wxTimerEvent& evt )
			 * @brief This method is called by timer event
			 * @parma[in] evt timer event information
			 */
			void  onTimer( wxTimerEvent& evt );

			/**
			 * @fn void onDestroy( wxShowEvent& evt )
			 * @brief This method is called by destroy event
			 * @param[in] evt destroy event information
			 */
			void onDestroy( wxShowEvent& evt );

		protected:			 
			/**
			 * @fn void onReset( wxCommandEvent& evt )
			 * @brief This method is called when reset menu is called
			 * @param evt command event information
			 */
			void onReset( wxCommandEvent& evt );

		public:
			/**
			 * @fn static unsigned int getNumberOfCanvases()
			 * @brief gets the number of canvases
			 * @return the number ofcanvases
			 */
			static unsigned int getNumberOfCanvases();

			/**
			 * @fn static Sample3DCanvas* getCanvas( const unsigned int index )
			 * @brief gets the canvas
			 * @param param index canvas index
			 * @return canvas object (If NULL, the index is illegal.)
			 */
			static Sample3DCanvas* getCanvas( const unsigned int index );

			/**
			 * @fn static Sample3DCanvas* getCanvas( kome::objects::DataMapInfo* dataMap )
			 * @brief gets the canvas that has specified sample
			 * @param dataMap data map
			 * @return the canvas that has specified data map
			 */
			static Sample3DCanvas* getCanvas( kome::objects::DataMapInfo* dataMap );

			/**
			 * @fn void onKeyUpDown( wxKeyEvent& event )
			 * @brief This method is keyboard down event
			 * @param evt event information
			 */
			void onKeyUpDown( wxKeyEvent& event );

		public:
			/**
			 * @fn int setMouseCursor( kome::img::Graphics& graphics, kome::evt::MouseEvent& evt, const char* pluginName , const char* iconName );
			 * @brief set mouse cursor
			 * @param g Graphics object 
			 * @param pluginName set plugin-name. define at <name> tag in plugin.xml. For example, in this project, use PLUGIN_NAME_3D_VIEW.
			 * @param iconName  set mouse-cursor-name. define at <icon-name> tag in plugin.xml. For example, in this project, use ICON_NAME_3D_DIRECTION or ICON_NAME_3D_ZOOM .
			 * @return
			 */
			int setMouseCursor( const char* pluginName , const char* iconName );

			/**
			 * @fn wxMenu* createMenu()
			 * @brief creates menu
			 * @return created menu
			 */
			wxMenu* Sample3DCanvas::createMenu();	// @date 2012/11/26 <Add> OKADA

			/**
			 * @fn void onMenuPopup( wxCommandEvent& evt );
			 * @brief clicked popup menu
			 * @param[in] evt information of wxCommandEvent
			 */
			void onMenuPopup( wxCommandEvent& evt );	// @date 2012/11/26 <Add> OKADA

		private:
			DECLARE_EVENT_TABLE()
		};
	}
}


#endif		// __KOME_VIEW_RAW_DATA_3D_CANVAS_H__

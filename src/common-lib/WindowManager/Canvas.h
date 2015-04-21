/**
 * @file Canvas.h
 * @brief interfaces of Canvas class
 *
 * @author S.Tanaka
 * @date 2007.03.04
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_WINDOW_CANVAS_H__
#define __KOME_WINDOW_CANVAS_H__


#include <vector>
#include <boost/function.hpp>

#include <wx/wx.h>
#include <wx/scrolwin.h>


class wxTimer;

// for context menu ‚Í ID 60201-‚ðŽg—p‚µ‚Ü‚·
#define ID_POPUP_MENU_MIN	60201	// for context menu
#define ID_POPUP_MENU_MAX	60399

namespace kome {

	namespace evt {
		class MouseEvent;
		class KeyEvent;
	}

	namespace window {

		class ChildFrame;

		/**
		 * @class Canvas
		 * @brief common canvas class
		 */
		class WINDOW_MANAGER_CLASS Canvas : public wxScrolledWindow  {
		public:
			/**
			 * @fn Canvas(
					wxWindow* parent,
					const int id,
					const int width,
					const int height
				)
			 * @brief constructor
			 * @param[in] parent parent frame
			 * @param[in] id window ID
			 * @param[in] width window width
			 * @param[in] height window height
			 */
			Canvas(
				wxWindow* parent,
				const int id,
				const int width,
				const int height
			);

			/**
			 * @fn virtual ~Canvas()
			 * @brief destructor
			 */
			virtual ~Canvas();

		protected:
			/** frame */
			ChildFrame* m_frame;

			/** window width */
			int m_width;

			/** window height */
			int m_height;

			/** mouse point */
			kome::core::Point< int > m_cursorPos;

			/** tool tip */
			std::string m_tooltip;

			/** displayed tool tip */
			std::string m_displayedTooltip;

			/** index */
			int m_index;

			/** push flag */
			double m_pushFlg;

		private:
			/** click timer */
			wxTimer* m_clickTimer;

			/** init size flag */
			bool m_initSize;

			/** mouse button down event */
			kome::evt::MouseEvent* m_btnDownEvt;

			/** mouse button up event */
			kome::evt::MouseEvent* m_btnUpEvt;

			/** mouse button up flag */
			bool m_btnUp;

		private:
			/** spectra */
			std::vector< kome::objects::Spectrum* > m_spectra;

			/** chromatograms */
			std::vector< kome::objects::Chromatogram* > m_chroms;

			/** datamaps */
			std::vector< kome::objects::DataMapInfo* > m_datamaps;

			/** active object */
			kome::objects::Variant* m_acObj;

		protected:
			/** canvases */
			static std::vector< Canvas* > m_canvases;

		private:
			/** sub view flag */
			bool m_flgSetSub;

		public:
			/**
			* @typedef enum CanvasStyle
			* @brief Canvas Style
			*/
			typedef enum {
				MULTIPLE_OBJECTS = 1,    // Stacked
				OVERLAPPING = 2          // Overlapping
			} CanvasStyle;

			/** canvas style */
			int m_canvasStyle;
			
			/**
			 * @fn int getCanvasStyle()
			 * @brief get canvas style
			 * @return CanvasStyle
			 */
			int getCanvasStyle();

		public:
			/**
			 * @fn void popupMenu( const int px, const int py )
			 * @brief display popup menu
			 * @param[in] px the x coordinate of popup position
			 * @param[in] py the y coordinate of popup position
			 */
			void popupMenu( const int px, const int py );

		public:
			/**
			 * @fn void clearObjects()
			 * @brief clear objects
			 */
			void clearObjects();

			/**
			 * @fn virtual void clearSpectra()
			 * @brief clears spectra
			 */
			virtual void clearSpectra();

			/**
			 * @fn virtual void addSpectrum( kome::objects::Spectrum* spec )
			 * @brief adds spectrum
			 * @param[in] spec spectrum
			 */
			virtual void addSpectrum( kome::objects::Spectrum* spec );

			/**
			 * @fn virtual void setSpectrum( kome::objects::Spectrum* spec )
			 * @brief sets spectrum
			 * @param[in] spec spectrum
			 */
			virtual void setSpectrum( kome::objects::Spectrum* spec );

			/**
			 * @fn virtual void removeSpectrum( kome::objects::Spectrum* spec )
			 * @brief removes spectrum
			 * @param[in] spec spectrum
			 */
			virtual void removeSpectrum( kome::objects::Spectrum* spec );

			/**
			 * @fn unsigned int getNumberOfSpectra()
			 * @brief gets the number of spectra
			 * @return the number of spectra
			 */
			unsigned int getNumberOfSpectra();

			/**
			 * @fn virtual kome::objects::Spectrum* getSpectrum( const unsigned int idx = 0 )
			 * @brief gets spectrum
			 * @param[in] idx spectrum index
			 * @return spectrum spectrum (If NULL, the index is illegal.)
			 */
			virtual kome::objects::Spectrum* getSpectrum( const unsigned int idx = 0 );

			/**
			 * @fn virtual void clearChromatograms()
			 * @brief clears chromatograms 
			 */
			virtual void clearChromatograms();

			/**
			 * @fn virtual void addChromatogram( kome::objects::Chromatogram* chrom )
			 * @brief adds chromatogram
			 * @param[in] chrom chromatogram
			 */
			virtual void addChromatogram( kome::objects::Chromatogram* chrom );

			/**
			 * @fn virtual void setChromatogram( kome::objects::Chromatogram* chrom )
			 * @brief sets chromatogram
			 * @param[in] chrom chromatogram
			 */
			virtual void setChromatogram( kome::objects::Chromatogram* chrom );

			/**
			 * @fn virtual void removeChromatogram( kome::objects::Chromatogram* chrom )
			 * @brief removes chromatogram
			 * @param[in] chrom chromatogram
			 */
			virtual void removeChromatogram( kome::objects::Chromatogram* chrom );

			/**
			 * @fn unsigned int getNumberOfChromatograms()
			 * @brief gets the number of chromatograms
			 * @return the number of chromatograms
			 */
			unsigned int getNumberOfChromatograms();

			/**
			 * @fn kome::objects::Chromatogram* getChromatogram( const unsigned int idx = 0 )
			 * @brief gets chromatogram
			 * @ param idx chromatogram index
			 * @return chromatogram (If NULL, the index is illegal.)
			 */
			kome::objects::Chromatogram* getChromatogram( const unsigned int idx = 0 );

			/**
			 * @fn virtual void clearDataMaps()
			 * @brief clears data maps
			 */
			virtual void clearDataMaps();

			/**
			 * @fn virtual void addDataMap( kome::objects::DataMapInfo* dataMap )
			 * @brief adds data map
			 * @param[in] dataMap data map
			 */
			virtual void addDataMap( kome::objects::DataMapInfo* dataMap );

			/**
			 * @fn virtual void setDataMap( kome::objects::DataMapInfo* dataMap )
			 * @brief sets data map
			 * @param[in] dataMap data map
			 */
			virtual void setDataMap( kome::objects::DataMapInfo* dataMap );

			/**
			 * @fn virtual void removeDataMap( kome::objects::DataMapInfo* dataMap )
			 * @brief removes data map
			 * @param[in] dataMap data map
			 */
			virtual void removeDataMap( kome::objects::DataMapInfo* dataMap );

			/**
			 * @fn unsigned int getNumberOfDataMaps()
			 * @brief gets the number of data maps
			 * @return the number of data maps
			 */
			unsigned int getNumberOfDataMaps();

			/**
			 * @fn kome::objects::DataMapInfo* getDataMap( const unsigned int idx = 0 )
			 * @brief gets data map
			 * @param[in] idx data map index
			 * @return data map (If NULL, the index is illegal.)
			 */
			kome::objects::DataMapInfo* getDataMap( const unsigned int idx = 0 );

			/**
			 * @fn virtual void getSpecDispCanvas( std::vector< kome::objects::Spectrum* >&spectra )
			 * @brief Gets the spectrum displayed on the canvas
			 * @param spectra spectrum of being displayed on the canvas
			 */
			virtual void getSpecDispCanvas( std::vector< kome::objects::Spectrum* >&spectra );
			
			/**
			 * @fn virtual void getChromDispCanvas( std::vector< kome::objects::Chromatogram* >& chroms )
			 * @brief Gets the chromatogram displayed on the canvas
			 * @param chroms Chromatogram of being displayed on the canvas
			 */
			virtual void getChromDispCanvas( std::vector< kome::objects::Chromatogram* >&chroms );

		public:
			/**
			 * @fn void setActiveObject( kome::objects::Variant& obj )
			 * @brief sets active object
			 * @param[in] obj active object
			 */
			void setActiveObject( kome::objects::Variant& obj );

			/**
			 * @fn kome::objects::Variant getActiveObject()
			 * @brief gets active object
			 * @return active object
			 */
			kome::objects::Variant getActiveObject();

		protected:
			/**
			 * @fn virtual wxMenu* createMenu()
			 * @brief creates menu
			 * @return created menu
			 */
			virtual wxMenu* createMenu();

		public:
			/**
			 * @fn void draw( wxDC& dc )
			 * @brief draws
			 * @param[in,out] dc device context
			 */
			void draw( wxDC& dc );

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

			/**
			 * @fn virtual kome::objects::DataMapInfo* onAddDataMap( kome::objects::DataMapInfo* dataMap )
			 * @brief This method is called when a data map is added.
			 * @param[in] dataMap data map to be added
			 */
			virtual kome::objects::DataMapInfo* onAddDataMap( kome::objects::DataMapInfo* dataMap );

			/**
			 * @fn virtual void onRemoveDataMap( kome::objects::DataMapInfo* dataMap )
			 * @brief This method is called when a data map is removed
			 * @param[in] dataMap data map to be removed
			 */
			virtual void onRemoveDataMap( kome::objects::DataMapInfo* dataMap );

		protected:
			/**
			 * @fn virtual void onDraw( wxDC& dc )
			 * @brief This method is called when draw event
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

			/**
			 * @fn virtual bool onMouseButtonDown( kome::evt::MouseEvent& evt )
			 * @brief This method is called when mouse button is down
			 * @param[in] evt mouse event information
			 * @return refresh flag value
			 */
			virtual bool onMouseButtonDown( kome::evt::MouseEvent& evt );

			/**
			 * @fn virtual bool onMouseButtonUp( kome::evt::MouseEvent& evt )
			 * @brief This method is called when mouse button is up
			 * @param[in] evt mouse event information
			 * @return refresh flag value
			 */
			virtual bool onMouseButtonUp( kome::evt::MouseEvent& evt );

			/**
			 * @fn virtual bool onMouseDoubleClick( kome::evt::MouseEvent& evt )
			 * @brief This method is called when mouse button is double clicked.
			 * @param[in] evt mouse event information
			 * @return refresh flag value
			 */
			virtual bool onMouseDoubleClick( kome::evt::MouseEvent& evt );

			/**
			 * @fn virtual bool onMouseRightDoubleClick( kome::evt::MouseEvent& evt )
			 * @brief This method is called when mouse button is double clicked.
			 * @param[in] evt mouse event information
			 * @return refresh flag value
			 */
			virtual bool onMouseRightDoubleClick( kome::evt::MouseEvent& evt );

			/**
			 * @fn virtual bool onMouseCursorMove( kome::evt::MouseEvent& evt )
			 * @brief This method is called when mouse cursor is moved.
			 * @param[in] evt mouse event information
			 * @return refresh flag value
			 */
			virtual bool onMouseCursorMove( kome::evt::MouseEvent& evt );

			/**
			 * @fn virtual bool onMouseCursorEnter( kome::evt::MouseEvent& evt )
			 * @brief This method is called when mouse cursor enters.
			 * @param[in] evt mouse event information
			 * @return refresh flag value
			 */
			virtual bool onMouseCursorEnter( kome::evt::MouseEvent& evt );

			/**
			 * @fn virtual bool onMouseCursorLeave( kome::evt::MouseEvent& evt )
			 * @brief This method is called when mouse cursor leaves.
			 * @param[in] evt mouse event information
			 * @return refresh flag value
			 */
			virtual bool onMouseCursorLeave( kome::evt::MouseEvent& evt );

		protected:
			/**
			 * @fn void click()
			 * @brief call mouse click event
			 */
			void click();
					
			/**
			 * @fn virtual void OnDraw( wxDC& dc )
			 * @brief This method is called by drawing event. (override method)
			 * @param[in,out] dc device context to be drawed
			 */
			virtual void OnDraw( wxDC& dc );
					
			/**
			 * @fn void onEraseBackgroundEvt( wxEraseEvent& evt )
			 * @brief This method is called by erase background event.
			 * @param[in] evt erase background event information
			 */
			void onEraseBackgroundEvt( wxEraseEvent& evt );

			/**
			 * @fn void onSizeEvt( wxSizeEvent& evt )
			 * @brief This method is called by size event.
			 * @param[in] evt size event information
			 */
			void onSizeEvt( wxSizeEvent& evt );

			/**
			 * @fn void onMouseEvt( wxMouseEvent& evt )
			 * @brief This method is called by mouse event.
			 * @param[in] evt mouse event information
			 */
			void onMouseEvt( wxMouseEvent& evt );

			/**
			 * @fn void onTimerEvt( wxTimerEvent& evt )
			 * @brief this method is called by timer event
			 * @param[in] evt timer event information
			 */
			void onTimerEvt( wxTimerEvent& evt );

		public:
			/**
			 * @fn static unsigned int getNumberOfCanvases()
			 * @brief gets the number of canvases
			 * @return the number of canvases
			 */
			static unsigned int getNumberOfCanvases();

			/**
			 * @fn static Canvas* getCanvas( const unsigned int idx )
			 * @brief gets the canvas
			 * @param[in] idx canvas index
			 * @return canvas (If NULl, the specified index is illegal.)
			 */
			static Canvas* getCanvas( const unsigned int idx );

			/**
			 * @fn int getIndex()
			 * @brief gets the index
			 * @return index
			 */
			int getIndex();

			/**
			 * @fn void setSubView( bool flgSetSub )
			 * @brief sets the sub view flag
			 * @param[in] flgSetSub true=sub view false=not sub view
			 */
			void setSubView( bool flgSetSub );

			/**
			 * @fn bool isSubView( void )
			 * @brief gets the sub view flag
			 * @return true=sub view false=not sub view
			 */
			bool isSubView( void );

			// context menu
		private:
		public:
			/** context menu map */
			static std::map< int, kome::plugin::PluginMenu* > m_contextMenuTreeMap;

			/** context menu map counter */
			static int m_uCntContextMenuTreeMap;

		public:
			/**
			 * @fn static wxMenu* createPopupMenu( kome::plugin::PluginMenu* parentInfo, wxMenu* ptrMenu=NULL );
			 * @brief create Popup Menu
			 * @param[in] parentInfo menu information
			 * @param[in] ptrMenu parent menu. This method add menu to this pointer
			 * @return pointer of wxMenu*
			 */
			static wxMenu* createPopupMenu( kome::plugin::PluginMenu* parentInfo, wxMenu* ptrMenu=NULL );

		private:
			/**
			 * @fn static void createPopupMenuRecursive( kome::plugin::PluginMenu* parentInfo, wxMenu* parentMenu );
			 * @brief create Popup Menu
			 * @param[in] parentInfo menu information
			 * @param[in] ptrMenu parent menu. This method add menu to this pointer
			 * @return pointer of wxMenu*
			 */
			static void createPopupMenuRecursive( kome::plugin::PluginMenu* parentInfo, wxMenu* parentMenu );

		public:
			/**
			 * @fn static void executePopupMenu( unsigned int nId )
			 * @brief execute Popup Menu
			 * @param[in] nId menu id
			 */
			static void executePopupMenu( unsigned int nId );

			/**
			 * @fn void onMenuPopup( wxCommandEvent& evt )
			 * @brief clicked popup menu
			 * @param[in] evt information of wxCommandEvent
			 */
			void onMenuPopup( wxCommandEvent& evt );

		private:
			DECLARE_EVENT_TABLE()
		};
	}
}


#endif		// __KOME_WINDOW_CANVAS_H__

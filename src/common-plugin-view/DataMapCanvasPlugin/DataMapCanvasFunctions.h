/**
 * @file DataMapCanvasFunctions.h
 * @brief interfaces of functions of DataMapCanvas plug-in
 *
 * @author S.Tanaka
 * @date 2007.09.24
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_VIEW_DATA_MAP_CANVAS_FUNCTIONS_H__
#define __KOME_VIEW_DATA_MAP_CANVAS_FUNCTIONS_H__

#define	PLUGIN_NAME_DATA_MAP		"DataMap Canvas"

// plugin.xml内で定義されたアイコン名
#define	ICON_NAME_HEATMAP_SELECT	"heatmap_select"

#ifdef __unix
	extern "C" {
#endif  // __unix


/**
 * @fn kome::objects::Variant createDataMapCanvas( kome::objects::Parameters* params )
 * @brief creates data map canvas
 * @param params parameters object
 * @return data map canvas
 */
kome::objects::Variant createDataMapCanvas( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant setDataMapStatus( kome::objects::Parameters* )
 * @brief sets data map status
 * @return This method always returns true. (boolean)
 */
kome::objects::Variant setDataMapStatus( kome::objects::Parameters* );

/**
 * @fn kome::objects::Variant setDataMapStatusOpen( kome::objects::Parameters* )
 * @brief set data map status and open data map info
 * @return This method always returns true. (boolean)
 */
kome::objects::Variant setDataMapStatusOpen( kome::objects::Parameters* );

/**
 * @fn kome::objects::Variant onDataMapMouseMoveStatusBar( kome::objects::Parameters* params )
 * @brief This function is called when mouse cursor is moved on a datamap window.
 * @param params parameters object. (This has mouse event and graphics object.)
 * @return refresh flag (boolean value)
 */
kome::objects::Variant onDataMapMouseMoveStatusBar( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant onDataMapMouseLeaveStatusBar( kome::objects::Parameters* params )
 * @brief This function is called when mouse cursor is leaved from a datamap window.
 * @param params parameters object. (This has mouse event and graphics object.)
 * @return refresh flag (boolean value)
 */
kome::objects::Variant onDataMapMouseLeaveStatusBar( kome::objects::Parameters* );

/**
 * @fn kome::objects::Variant openDataMapFrame( kome::objects::Parameters* params )
 * @brief open data map frame
 * @return open data map frame
 */
kome::objects::Variant openDataMapFrame( kome::objects::Parameters* params );

/**
  * @fn int setMouseCursor( kome::img::Graphics& graphics, kome::evt::MouseEvent& evt, const char* pluginName , const char* iconName );
  * @brief set mouse cursor
  * @param g Graphics object 
  * @param evt mouse event 
  * @param pluginName set plugin-name. define at <name> tag in plugin.xml. For example, in this project, use PLUGIN_NAME_DATA_MAP.
  * @param iconName  set mouse-cursor-name. define at <icon-name> tag in plugin.xml. For example, in this project, use ICON_NAME_HEATMAP_SELECT .
  * @return true:in area  false:out of area
  */
int setMouseCursor( kome::img::Graphics& graphics, kome::evt::MouseEvent& evt, const char* pluginName , const char* iconName );

/**
 * @fn kome::objects::Variant onDataMapMouseButtonDown( kome::objects::Parameters* params )
 * @brief this function is called  when mouse cursor is down on a datamap window.
 * @param params parameters object. (This has mouse event and graphics object.)
 * @return refresh flag (boolean value)
 */
kome::objects::Variant onDataMapMouseButtonDown( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant onDataMapMouseButtonUp( kome::objects::Parameters* params )
 * @brief this function is called  when mouse cursor is up on a datamap window.
 * @param params parameters object. (This has mouse event and graphics object.)
 * @return refresh flag (boolean value)
 */
kome::objects::Variant onDataMapMouseButtonUp( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant onDataMapMouseDblClick( kome::objects::Parameters* params )
 * @brief this function is called  when mouse cursor is double click on a datamap window.
 * @param params parameters object. (This has mouse event and graphics object.)
 * @return refresh flag (boolean value)
 */
kome::objects::Variant onDataMapMouseDblClick( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant getHeatMapCanvasOperation( kome::objects::Parameters* params )
 * @brief get heatmap canvas operation
 * @return operation
 */
kome::objects::Variant getHeatMapCanvasOperation( kome::objects::Parameters* );

#ifdef __unix
	}
#endif  // __unix



#endif// __KOME_VIEW_DATA_MAP_CANVAS_FUNCTIONS_H__

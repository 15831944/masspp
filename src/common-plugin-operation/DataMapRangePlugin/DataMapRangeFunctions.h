/**
 * @file DataMapRangeFunctions.h
 * @brief interfaces of DataMapRange plug-in functions
 *
 * @author S.Tanaka
 * @date 2009.01.07
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_OPERATION_RANGE_DATAMAP_RANGE_FUNCTIONS_H__
#define __KOME_OPERATION_RANGE_DATAMAP_RANGE_FUNCTIONS_H__


#ifdef __unix
	extern "C" {
#endif  // __unix


/**
 * @fn kome::objects::Variant createDataMap( kome::objects::Parameters* params )
 * @brief This function is called when a data map is opened.
 * @param params parameters object. (This has data map object.)
 * @return If true, it succeeded to prepare data map. (boolean)
 */
kome::objects::Variant createDataMap( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant prepareDataMap( kome::objects::Parameters* params )
 * @brief prepares to draw data map
 * @param params parameters object. (This has data map and graphics object.)
 * @return return value (This value has no meanings.)
 */
kome::objects::Variant prepareDataMap( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant closeDataMapRange( kome::objects::Parameters* params )
 * @brief This function is called when data map is closed.
 * @param params parameters object. (This has data map object.)
 * @return return value (This value has no meanings.)
 */
kome::objects::Variant closeDataMapRange( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant onDataMapInfoButtonDown( kome::objects::Parameters* params )
 * @brief This function is called when mouse button is down on a data map window.
 * @param params parameters object. (This has mouse event and graphics object.)
 * @return refresh flag (boolean value)
 */
kome::objects::Variant onDataMapInfoButtonDown( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant onDataMapInfoMouseMove( kome::objects::Parameters* params )
 * @brief This functino is called when mouse cursor is moved on a data map window.
 * @param params parameters object. (This has mouse event and graphics object.)
 * @return refresh flag (boolean value)
 */
kome::objects::Variant onDataMapInfoMouseMove( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant onDataMapInfoButtonUp( kome::objects::Parameters* params )
 * @brief This function is called when mouse button is up on a data map window
 * @param params parameters object. (This has mouse event and graphics object.)
 * @return refresh flag (boolean value)
 */
kome::objects::Variant onDataMapInfoButtonUp( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant onDrawDataMapFg( kome::objects::Parameters* params )
 * @brief draws data map foreground
 * @param params parameters object. (This has data map, graphics object.)
 * @return return value. (This value has no meanings.)
 */
kome::objects::Variant onDrawDataMapFg( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant setRangeStatus( kome::objects::Parameters* )
 * @brief sets data map range status
 * @return This method always returns true. (boolean)
 */
kome::objects::Variant setRangeStatus( kome::objects::Parameters* );

/**
 * @fn kome::objects::Variant resetZoomDatamapFromContextMenu( kome::objects::Parameters* )
 * @brief resets chromatogram range status at datamap
 * @return This method always returns true. (boolean)
 */
kome::objects::Variant resetZoomDatamapFromContextMenu( kome::objects::Parameters* );	// @date 2012/11/30 <Add> OKADA

/**
 * @fn kome::objects::Variant openChangeDisplayedRangeDlg( kome::objects::Parameters* )
 * @brief open charge displayed range dialog
 * @return This method always return true.(boolean)
 */
kome::objects::Variant openChangeDisplayedRangeDlg( kome::objects::Parameters* );

/**
 * @fn kome::objects::Variant onDataMapInfoDblClick( kome::objects::Parameters* params )
 * @brief This function is called when mouse button is double click on a data map window
 * @param params parameters object. (This has mouse event and graphics object.)
 * @return refresh flag (boolean value)
 */
kome::objects::Variant onDataMapInfoDblClick( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant getDataMapRangeOperation( kome::objects::Parameters* params )
 * @brief gets dataMapRange Operation
 * @return return value DataMapRangeOperation object
 */
kome::objects::Variant getDataMapRangeOperation( kome::objects::Parameters* params );


#ifdef __unix
	}
#endif  // __unix



#endif		// __KOME_OPERATION_RANGE_DATAMAP_RANGE_FUNCTIONS_H__

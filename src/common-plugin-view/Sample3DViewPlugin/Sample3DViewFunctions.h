/**
 * @file Sample3DViewFunctions.h
 * @brief interfaces of Sample3DView plug-in functions
 *
 * @author S.Tanaka
 * @date 2008.01.21
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_VIEW_RAW_DATA_3D_VIEW_FUNCTINOS_H__
#define __KOME_VIEW_RAW_DATA_3D_VIEW_FUNCTINOS_H__


#ifdef __unix
	extern "C" {
#endif  // __unix


/**
 * @fn kome::objects::Variant create3DCanvas( kome::objects::Parameters* params )
 * @brief creates 3D View canvas
 * @param params parameters object
 * @return 3D View window
 */
kome::objects::Variant create3DCanvas( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant updateGraphInfo( kome::objects::Parameters* params )
 * @brief updates graph information
 * @param params parameters object
 * @return return value (This value has no meanings.)
 */
kome::objects::Variant updateGraphInfo( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant updateSettings( kome::objects::Parameters* )
 * @brief updates settings
 * @param params parameters object
 * @return return value (This value has no meanings.)
 */
kome::objects::Variant updateSettings( kome::objects::Parameters* );

/**
 * @fn kome::objects::Variant setDataMapStatus( kome::objects::Parameters* )
 * @brief sets data map status
 * @return This function always returns true. (boolean)
 */
kome::objects::Variant setDataMapStatus( kome::objects::Parameters* );

/**
 * @fn kome::objects::Variant setDataMapStatusOpen( kome::objects::Parameters* )
 * @brief set data map status and open data map info
 * @return This method always returns true. (boolean)
 */
kome::objects::Variant setDataMapStatusOpen( kome::objects::Parameters* );
		
/**
* @fn kome::objects::Variant onDataMapMouseMoveMouseCursor( kome::objects::Parameters* params )
* @brief data map Mouse Move MouseCursor
* @return If true, It succeeded to datamap mouse move. (boolean)
*/
kome::objects::Variant onDataMapMouseMoveMouseCursor( kome::objects::Parameters* params);

/**
* @fn kome::objects::Variant openDataMapInfo( kome::objects::Parameters* )
* @brief opens data map
* @return If true, It succeeded to open data map. (boolean)
*/
kome::objects::Variant openDataMapInfo( kome::objects::Parameters* params );

kome::objects::Variant get3DCanvasOperation( kome::objects::Parameters* );

/**
 * @fn kome::objects::Variant killTimers( kome::objects::Parameters* )
 * @brief kill timers
 * @return This function always returns true
 */
kome::objects::Variant killTimers( kome::objects::Parameters* );

#ifdef __unix
	}
#endif  // __unix



#endif		// __KOME_VIEW_RAW_DATA_3D_VIEW_FUNCTINOS_H__

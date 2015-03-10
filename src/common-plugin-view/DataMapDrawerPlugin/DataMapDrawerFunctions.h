/**
 * @file DataMapDrawerFunctions.h
 * @brief interfaces of DataMapDrawer plug-in functions
 *
 * @author S.Tanaka
 * @date 2007.09.24
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#ifndef __KOME_VIEW_DATA_MAP_DRAWER_FUNCTIONS_H__
#define __KOME_VIEW_DATA_MAP_DRAWER_FUNCTIONS_H__


#ifdef __unix
	extern "C" {
#endif  // __unix


/**
 * @fn kome::objects::Variant drawColorMap( kome::objects::Parameters* params )
 * @brief draws color map
 * @param params parameters object
 * @return return value (This value has no meanings.)
 */
kome::objects::Variant drawColorMap( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant drawBar( kome::objects::Parameters* params )
 * @brief draws bar
 * @param params parameters object
 * @return return value (This value has no meanings.)
 */  
kome::objects::Variant drawBar( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant prepareDataMap( kome::objects::Parameters* params )
 * @brief prepares for drawing
 * @param params parameters object
 * @return return value (This value has no meanings.)  
 */
kome::objects::Variant prepareDataMap( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant deleteBitmap( kome::objects::Parameters* params )
 * @brief delete bitmap data
 * @param params parameters object
 * @return return value (This value has no meanings.)  
 */
kome::objects::Variant deleteBitmap( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant deleteSampleBitmap( kome::objects::Parameters* params )
 * @brief delete sample
 * @param params parameters object
 * @return return value (This value has no meanings.)  
 */
kome::objects::Variant deleteSampleBitmap( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant updateSettings( kome::objects::Parameters* )
 * @brief This function is called when settings value is updated
 * @return return value (This value has no meanings.)
 */
kome::objects::Variant updateSettings( kome::objects::Parameters* );

/**
 * @fn kome::objects::Variant setStatusValue( kome::objects::Parameters* )
 * @brief sets draw map status colormap
 * @return This function always returns true. (boolean)
 */ 
kome::objects::Variant setStatusValue( kome::objects::Parameters* );

#ifdef __unix
	}
#endif  // __unix


#endif//  __KOME_VIEW_DATA_MAP_DRAWER_FUNCTIONS_H__

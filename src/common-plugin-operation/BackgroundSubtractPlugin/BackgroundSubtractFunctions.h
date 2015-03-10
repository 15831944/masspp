/**
 * @file BackgroundSubtractFunctions.h
 * @brief interfaces of BackgroundSubtract plug-in functions
 *
 * @author S.Tanaka
 * @date 2009.12.08
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_BACKGROUND_SUBTRACT_FUNCTIONS_H__
#define __KOME_BACKGROUND_SUBTRACT_FUNCTIONS_H__


#ifdef __unix
	extern "C" {
#endif  // __unix


/**
 * @fn kome::objects::Variant openBackgroundDialog( kome::objects::Parameters* params )
 * @brief opens background subtract dialog
 * @param params parameters object
 * @return This function always returns true. (boolean)
 */
kome::objects::Variant openBackgroundDialog( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant drawBackgroundLabel( kome::objects::Parameters* params )
 * @brief draws background label
 * @param params parameters object
 * @return This value has no meanings.
 */
kome::objects::Variant drawBackgroundLabel( kome::objects::Parameters* params );


// @date 2011.07.14 <Add> M.Izumi ->
/**
 * @fn kome::objects::Variant openSpectrumCanvas( kome::objects::Parameters* params )
 * @brief draws background label
 * @param params parameters object
 * @return This function always returns true. (boolean)
 */
kome::objects::Variant openSpectrumCanvas( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant closeSpectrumCanvas( kome::objects::Parameters* params )
 * @brief draws background label
 * @param params parameters object
 * @return This value has no meanings.
 */
kome::objects::Variant closeSpectrumCanvas( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant closeAllSpectrumCanvas( kome::objects::Parameters* params )
 * @brief draws background label
 * @param params parameters object
 * @return This value has no meanings.
 */
kome::objects::Variant closeAllSpectrumCanvas( kome::objects::Parameters* params );
// @date 2011.07.14 <Add> M.Izumi <-

/**
 * @fn kome::objects::Variant getBackgroundOperation( kome::objects::Parameters* )
 * @brief gets background  operation
 * @return return value BackgroundOperation object
 */
kome::objects::Variant getBackgroundOperation( kome::objects::Parameters* );

/**
 * @fn kome::objects::Variant getSpecXYDataOperation( kome::objects::Parameters* params )
 * @brief get spec xyData operation
 * @return xydata operation
 */
kome::objects::Variant getSpecXYDataOperation( kome::objects::Parameters* params );



#ifdef __unix
	}
#endif  // __unix



#endif		// __KOME_BACKGROUND_SUBTRACT_FUNCTIONS_H__

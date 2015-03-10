/**
 * @file ProfileCanvasFunctions.h
 * @brief interfaces of functions of ProfileCanvas plug-in
 *
 * @author S.Tanaka
 * @date 2007.09.23
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#ifndef __KOME_VIEW_PROFILE_CANVAS_FUNCTIONS_H__
#define __KOME_VIEW_PROFILE_CANVAS_FUNCTIONS_H__


#ifdef __unix
	extern "C" {
#endif  // __unix


/**
 * @fn kome::objects::Variant createSpectrumCanvas( kome::objects::Parameters* params )
 * @brief creates spectrum canvas
 * @param params parameters object
 * @return spectrum canvas
 */
kome::objects::Variant createSpectrumCanvas( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant createChromatogramCanvas( kome::objects::Parameters* params )
 * @brief creates chromatogram canvas
 * @param params parameters object
 * @return chromatogram canvas
 */
kome::objects::Variant createChromatogramCanvas( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant setSpecAndChromStatus( kome::objects::Parameters* )
 * @brief sets chromatogram and spectrum status
 * @return This method always returns true. (boolean)
 */
kome::objects::Variant setSpecAndChromStatus( kome::objects::Parameters* );

/**
 * @fn kome::objects::Variant getSpecProfileCanvasOperation( kome::objects::Parameters* )
 * @brief gets spectrum profile canvas operation
 * @return ProfileCanvas Operation
 */
kome::objects::Variant getSpecProfileCanvasOperation( kome::objects::Parameters* );

/**
 * @fn kome::objects::Variant getChromProfileCanvasOperation( kome::objects::Parameters* )
 * @brief gets chromatogram profile canvas operation
 * @return ProfileCanvas Operation
 */
kome::objects::Variant getChromProfileCanvasOperation( kome::objects::Parameters* );

#ifdef __unix
	}
#endif  // __unix



#endif// __KOME_VIEW_PROFILE_CANVAS_FUNCTIONS_H__

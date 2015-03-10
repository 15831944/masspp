/**
 * @file SelectRangeFunctions.h
 * @brief interfaces of SelectRange plug-in functions
 *
 * @author S.Tanaka
 * @date 2007.03.02
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#ifndef __KOME_OPERATION_RANGE_SELECT_RANGE_FUNCTIONS_H__
#define __KOME_OPERATION_RANGE_SELECT_RANGE_FUNCTIONS_H__


#ifdef __unix
	extern "C" {
#endif  // __unix

/**
 * @fn kome::objects::Variant onCloseSpec( kome::objects::Parameters* params )
 * @brief This function is called when spectrum is closed.
 * @param params parameters object. (This has spectrum object.)
 * @return return value (This value has no meanings.)
 */
kome::objects::Variant onCloseSpec( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant onCloseChrom( kome::objects::Parameters* params )
 * @brief This function is called when chromatogram is closed.
 * @param params parameters object. (This has chromatogram object.)
 * @return return value (This value has no meanings.)
 */
kome::objects::Variant onCloseChrom( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant onSpecButtonDown( kome::objects::Parameters* params )
 * @brief This function is called when mouse button is down on the spectrum window.
 * @param params parameters object. (This has mouse event and graphics object.)
 * @return refresh flag (boolean value)
 */
kome::objects::Variant onSpecButtonDown( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant onChromButtonDown( kome::objects::Parameters* params )
 * @brief This function is called when mouse button is down on the chromatogram window.
 * @param params parameters object. (This has mouse event and graphics object.)
 * @return refresh flag (boolean value)
 */
kome::objects::Variant onChromButtonDown( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant onSpecMouseMove( kome::objects::Parameters* params )
 * @brief This function is called when mouse cursor is moved on the spectrum window.
 * @param params parameters object. (This has mouse event and graphics object.)
 * @return refresh flag (boolean value)
 */
kome::objects::Variant onSpecMouseMove( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant onChromMouseMove( kome::objects::Parameters* params )
 * @brief This function is called when mouse cursor is moved on the spectrum window.
 * @param params parameters object. (This has mouse event and graphics object.)
 * @return refresh flag (boolean value)
 */
kome::objects::Variant onChromMouseMove( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant onSpecButtonUp( kome::objects::Parameters* params )
 * @brief This function is called when mouse button is up on the spectrum window.
 * @param params parameters object. (This has mouse event and graphics object.)
 * @return refresh flag (boolean value)
 */
kome::objects::Variant onSpecButtonUp( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant onChromButtonUp( kome::objects::Parameters* params )
 * @brief This function is called when mouse button is up on the chromatogram window.
 * @param params parameters object. (This has mouse event and graphics object.)
 * @return refresh flag (boolean value)
 */
kome::objects::Variant onChromButtonUp( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant onSpecButtonDblClick( kome::objects::Parameters* params )
 * @brief This function is called when mouse is double clicked on the spectrum window.
 * @param params parameters object. (This has mouse event and graphics object.)
 * @return refresh flag (boolean value)
 */
kome::objects::Variant onSpecButtonDblClick( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant onChromButtonDblClick( kome::objects::Parameters* params )
 * @brief This function is called when mouse button is double clicked on the chromatogram window.
 * @param params parameters object. (This has mouse event and graphics object.)
 * @return refresh flag (boolean value)
 */
kome::objects::Variant onChromButtonDblClick( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant onDrawSpecFg( kome::objects::Parameters* params )
 * @brief draw spectrum foreground
 * @param params parameters object. (This has spectrum, data points and graphics object.)
 * @return return value. (This value has no meanings.)
 */
kome::objects::Variant onDrawSpecFg( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant onDrawChromFg( kome::objects::Parameters* params )
 * @brief draw chromatogram foreground
 * @param params parameters object. (This has chromatogram, data points and graphics object.)
 * @return return value. (This value has no meanings.)
 */
kome::objects::Variant onDrawChromFg( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant onDrawSpecBg( kome::objects::Parameters* params )
 * @brief draw spectrum background
 * @param params parameters object. (This has spectrum, data points and graphics object.)
 * @return return value. (This value has no meanings.)
 */
kome::objects::Variant onDrawSpecBg( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant onDrawChromBg( kome::objects::Parameters* params )
 * @brief draw chromatogram background
 * @param params parameters object. (This has chromatogram, data points and graphics object.)
 * @return return value. (This value has no meanings.)
 */
kome::objects::Variant onDrawChromBg( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant SetValStatusBarSpec( kome::objects::Parameters* params )
 * @brief 波形のX軸、Y軸制御領域にマウスカーソルがある場合、
          ステータスバーにマウスカーソル位置の座標を表示
 * @param params parameters information
 * @return This value has no meanings.
 */
kome::objects::Variant SetValStatusBarSpec( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant SetValStatusBarChrom( kome::objects::Parameters* params )
 * @brief 波形のX軸、Y軸制御領域にマウスカーソルがある場合、
          ステータスバーにマウスカーソル位置の座標を表示
 * @param params parameters information
 * @return This value has no meanings.
 */
kome::objects::Variant SetValStatusBarChrom( kome::objects::Parameters* params );


/**
 * @fn kome::objects::Variant onMouseLeave( kome::objects::Parameters* params )
 * @brief This function is called when mouse cursor is leaved from a spectrum and chromatogram window.
 * @param params parameters object. (This has mouse event and graphics object.)
 * @return refresh flag (boolean value)
 * @return This value has no meanings.
 */
kome::objects::Variant onMouseLeave( kome::objects::Parameters* params );

#ifdef __unix
	}
#endif  // __unix



#endif		// __KOME_OPERATION_RANGE_SELECT_RANGE_FUNCTIONS_H__

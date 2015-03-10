/**
 * @file ProfileRangeFunctions.h
 * @brief interfaces of ProfileRange plug-in functions
 *
 * @author S.Tanaka
 * @date 2009.01.07
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_OPERATION_RANGE_PROFILE_RANGE_FUNCTIONS_H__
#define __KOME_OPERATION_RANGE_PROFILE_RANGE_FUNCTIONS_H__


#ifdef __unix
	extern "C" {
#endif  // __unix


/**
 * @fn kome::objects::Variant prepareSpectrum( kome::objects::Parameters* params )
 * @brief prepares to draw spectrum
 * @param params parameters object. (This has spectrum, data points and graphics object.)
 * @return return value. (This value has no meanings.)
 */
kome::objects::Variant prepareSpectrum( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant prepareChromatogram( kome::objects::Parameters* params )
 * @brief prepares to draw chromatogram
 * @param params parameters object. (This has chromatogram, data points and graphics object.)
 * @return return value. (This value has no meanings.)
 */
kome::objects::Variant prepareChromatogram( kome::objects::Parameters* params );

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
 * @brief This function is called when mouse button is down on a spectrum window.
 * @param params parameters object. (This has mouse event and graphics object.)
 * @return refresh flag (boolean value)
 */
kome::objects::Variant onSpecButtonDown( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant onChromButtonDown( kome::objects::Parameters* params )
 * @brief This function is called when mouse button is down on a chromatogram window.
 * @param params parameters object. (This has mouse event and graphics object.)
 * @return refresh flag (boolean value)
 */
kome::objects::Variant onChromButtonDown( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant onSpecMouseMove( kome::objects::Parameters* params )
 * @brief This function is called when mouse cursor is moved on a spectrum window.
 * @param params parameters object. (This has mouse event and graphics object.)
 * @return refresh flag (boolean value)
 */
kome::objects::Variant onSpecMouseMove( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant onChromMouseMove( kome::objects::Parameters* params )
 * @brief This function is called when mouse cursor is moved on a spectrum window.
 * @param params parameters object. (This has mouse event and graphics object.)
 * @return refresh flag (boolean value)
 */
kome::objects::Variant onChromMouseMove( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant onSpecButtonUp( kome::objects::Parameters* params )
 * @brief This function is called when mouse button is up on a spectrum window.
 * @param params parameters object. (This has mouse event and graphics object.)
 * @return refresh flag (boolean value)
 */
kome::objects::Variant onSpecButtonUp( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant onChromButtonUp( kome::objects::Parameters* params )
 * @brief This function is called when mouse button is up on a chromatogram window.
 * @param params parameters object. (This has mouse event and graphics object.)
 * @return refresh flag (boolean value)
 */
kome::objects::Variant onChromButtonUp( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant onSpecButtonDblClick( kome::objects::Parameters* params )
 * @brief This function is called when mouse is double clicked on a spectrum window.
 * @param params parameters object. (This has mouse event and graphics object.)
 * @return refresh flag (boolean value)
 */
kome::objects::Variant onSpecButtonDblClick( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant onChromButtonDblClick( kome::objects::Parameters* params )
 * @brief This function is called when mouse button is double clicked on a chromatogram window.
 * @param params parameters object. (This has mouse event and graphics object.)
 * @return refresh flag (boolean value)
 */
kome::objects::Variant onChromButtonDblClick( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant onSpecKeyDown( kome::objects::Parameters* params )
 * @brief This function is called when Ane Keybord action on a spectrum window.
 * @param params parameters object. (This has mouse event and graphics object.)
 * @return refresh flag (boolean value)
 */
kome::objects::Variant onSpecKeyDown( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant onChromKeyDown( kome::objects::Parameters* params )
 * @brief This function is called when Ane Keybord action on a chromatogram window.
 * @param params parameters object. (This has mouse event and graphics object.)
 * @return refresh flag (boolean value)
 */
kome::objects::Variant onChromKeyDown( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant onDrawSpecFg( kome::objects::Parameters* params )
 * @brief draws spectrum foreground
 * @param params parameters object. (This has spectrum, data points and graphics object.)
 * @return return value. (This value has no meanings.)
 */
kome::objects::Variant onDrawSpecFg( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant onDrawChromFg( kome::objects::Parameters* params )
 * @brief draws chromatogram foreground
 * @param params parameters object. (This has chromatogram, data points and graphics object.)
 * @return return value. (This value has no meanings.)
 */
kome::objects::Variant onDrawChromFg( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant onDrawSpecBg( kome::objects::Parameters* params )
 * @brief draws spectrum background
 * @param params parameters object. (This has spectrum, data points and graphics object.)
 * @return return value. (This value has no meanings.)
 */
kome::objects::Variant onDrawSpecBg( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant onDrawChromBg( kome::objects::Parameters* params )
 * @brief draws chromatogram background
 * @param params parameters object. (This has chromatogram, data points and graphics object.)
 * @return return value. (This value has no meanings.)
 */
kome::objects::Variant onDrawChromBg( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant setChromRangeStatus( kome::objects::Parameters* )
 * @brief sets chromatogram range status
 * @return This method always returns true. (boolean)
 */
kome::objects::Variant setRangeStatus( kome::objects::Parameters* );

/**
 * @fn kome::objects::Variant resetZoomFromContextMenu( kome::objects::Parameters* )
 * @brief resets chromatogram range status
 * @return This method always returns true. (boolean)
 */
kome::objects::Variant resetZoomFromContextMenu( kome::objects::Parameters* );	// @date 2012/11/16 <Add> OKADA

/**
 * @fn kome::objects::Variant resetZoomDatamapFromContextMenu( kome::objects::Parameters* )
 * @brief resets chromatogram range status at datamap
 * @return This method always returns true. (boolean)
 */
//kome::objects::Variant resetZoomDatamapFromContextMenu( kome::objects::Parameters* );	// @date 2012/11/30 <Add> OKADA

/**
 * @fn kome::objects::Variant resetZoomSubSpecFromContextMenu( kome::objects::Parameters* )
 * @brief resets chromatogram range status
 * @return This method always returns true. (boolean)
 */
kome::objects::Variant resetZoomSubSpecFromContextMenu( kome::objects::Parameters* );	// @date 2012/11/30 <Add> OKADA

/**
 * @fn kome::objects::Variant changeSpecRange( kome::objects::Parameters* params )
 * @brief changes spectrum range
 * @param params parameters information
 * @return This value has no meanings.
 */
kome::objects::Variant changeSpecRange( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant changeChromRange( kome::objects::Parameters* params )
 * @brief changes chromatogram range
 * @param params parameters information
 * @return This value has no meanings.
 */
kome::objects::Variant changeChromRange( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant SetValStatusBarSpec( kome::objects::Parameters* params )
 * @brief 波形のX軸、Y軸制御領域にマウスカーソルがある場合、
          ステータスバーにマウスカーソル位置の座標を表示
 * @param params parameters information
 * @return This value has no meanings.
 */
kome::objects::Variant SetValStatusBar( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant onMouseLeave( kome::objects::Parameters*  )
 * @brief This function is called when mouse cursor is leaved from a spectrum and chromatogram window.
 * @param params parameters object. (This has mouse event and graphics object.)
 * @return refresh flag (boolean value)
 */
kome::objects::Variant onMouseLeave( kome::objects::Parameters*  );

/**
 * @fn kome::objects::Variant getSpecRangeOperation( kome::objects::Parameters* )
 * @brief gets spectrum range  operation
 * @return return value ProfileRangeOperation object
 */
kome::objects::Variant getSpecRangeOperation( kome::objects::Parameters* );

/**
 * @fn kome::objects::Variant getChromRangeOperation( kome::objects::Parameters* )
 * @brief gets chromatogram range  operation
 * @return return value ProfileRangeOperation object
 */
kome::objects::Variant getChromRangeOperation( kome::objects::Parameters* );

/**
 * @fn kome::objects::Variant showChangeSpectrumRange( kome::objects::Parameters* params )
 * @brief shows Change Displayed Range Dialog
 * @param params parameters information (This object has spectrum and m/z range informations.)
 * @return This value has no meanings.
 */
kome::objects::Variant showChangeSpectrumRange( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant showChangeChromatogramRange( kome::objects::Parameters* params )
 * @brief shows Change Displayed Range Dialog
 * @param params parameters information (This object has chromatogram and RT range informations.)
 * @return This value has no meanings.
 */
kome::objects::Variant showChangeChromatogramRange( kome::objects::Parameters* params );


#ifdef __unix
	}
#endif  // __unix



#endif		// __KOME_OPERATION_RANGE_PROFILE_RANGE_FUNCTIONS_H__

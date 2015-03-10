/**
 * @file OverlappingCanvasFunctions.h
 * @brief interfaces of functions of OverlappingCanvas plug-in
 *
 * @author S.Tanaka
 * @date 2008.03.12
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_VIEW_PROFILE_CANVAS_FUNCTIONS_H__
#define __KOME_VIEW_PROFILE_CANVAS_FUNCTIONS_H__


#ifdef __unix
	extern "C" {
#endif  // __unix

/**
 * @fn kome::objects::Variant initGui( kome::objects::Parameters* )
 * @brief initialize view
 * @return return value (This value has no meaning.)
 */
kome::objects::Variant initGui( kome::objects::Parameters* );

/**
 * @fn kome::objects::Variant changeActiveObject( kome::objects::Parameters* params )
 * @brief This function is called when active object is changed
 * @param params parameters object
 * @return return value (This value has no meanings.)
 */
kome::objects::Variant changeActiveObject( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant getSpectrumCanvas( kome::objects::Parameters* params )
 * @brief creates spectrum canvas
 * @param params parameters object
 * @return spectrum canvas
 */
kome::objects::Variant getSpectrumCanvas( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant getChromatogramCanvas( kome::objects::Parameters* params )
 * @brief creates chromatogram canvas
 * @param params parameters object
 * @return chromatogram canvas
 */
kome::objects::Variant getChromatogramCanvas( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant setSpecAndChromStatus( kome::objects::Parameters* )
 * @brief sets spactrum and chromatogram status
 * @return This method always returns true. (boolean)
 */
kome::objects::Variant setSpecAndChromStatus( kome::objects::Parameters* );

/**
 * @fnkome::objects::Variant toggleOverlappingDialog( kome::objects::Parameters* )
 * @brief toggles the visibility of the overlapping dialog
 * @return If true, It succeeded to toggle the the visibility of the overlapping dialog (boolean)
 */
kome::objects::Variant toggleOverlappingDialog( kome::objects::Parameters* );

/**
 * @fn kome::objects::Variant checkOverlappingDialog( kome::objects::Parameters* )
 * @brief judges whether the overlapping dialog is shown
 * @return If true, the overlapping dialog is shown
 */
kome::objects::Variant checkOverlappingDialog( kome::objects::Parameters* );

/**
 * @fn kome::objects::Variant updateSettings( kome::objects::Parameters* )
 * @brief This function is called when settings value is updated
 * @return return value (This value has no meanings.)
 */
kome::objects::Variant updateSettings( kome::objects::Parameters* );

/**
 * @fn kome::objects::Variant exitGui( kome::objects::Parameters* )
 * @brief releases details view
 * @return This value has no meaning.
 */
kome::objects::Variant exitGui( kome::objects::Parameters* );

/**
 * @fn kome::objects::Variant SelectCanvasOfSpectrum( kome::objects::Parameters* params )
 * @brief キャンバスで選択されたスペクトルを取得
 */
kome::objects::Variant SelectCanvasOfSpectrum( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant SelectCanvasOfChromatogram( kome::objects::Parameters* params )
 * @brief キャンバスで選択されたクロマトグラムを取得
 */
kome::objects::Variant SelectCanvasOfChromatogram( kome::objects::Parameters* params );

// @date 2012/12/06 <Add> OKADA
/**
 * @fn kome::objects::Variant onSpecKeyUp( kome::objects::Parameters* params )
 * @brief This function is called when Ane Keybord action on a spectrum window.
 * @param params parameters object. (This has mouse event and graphics object.)
 * @return refresh flag (boolean value)
 */
kome::objects::Variant onSpecKeyUp( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant getSpecOverlapCanvasOperation( kome::objects::Parameters* params )
 * @brief get spectrum overlap canvas operaton
 * @return operation
 */
kome::objects::Variant getSpecOverlapCanvasOperation( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant getChromOverlapCanvasOperation( kome::objects::Parameters* params )
 * @brief get chromatogram overlap canvas operation
 * @return operation
 */
kome::objects::Variant getChromOverlapCanvasOperation( kome::objects::Parameters* params );


#ifdef __unix
	}
#endif  // __unix



#endif// __KOME_VIEW_PROFILE_CANVAS_FUNCTIONS_H__

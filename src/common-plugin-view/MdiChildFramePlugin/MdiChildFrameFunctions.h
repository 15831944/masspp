/**
 * @file MdiChildFrameFunctions.h
 * @brief interfaces of functions of MdiChildFrame plug-in
 *
 * @author S.Tanaka
 * @date 2007.09.23
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_VIEW_MDI_CHILD_FRAME_FUNCTIONS_H__
#define __KOME_VIEW_MDI_CHILD_FRAME_FUNCTIONS_H__


#ifdef __unix
	extern "C" {
#endif  // __unix


/**
 * @fn kome::objects::Variant openSpectrumFrame( kome::objects::Parameters* params )
 * @brief open spectrum frame
 * @param params parameters information. (This objest has spectrum information.)
 * @return return value. (This value doesn't have the meanings.)
 */
kome::objects::Variant openSpectrumFrame( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant openChromatogramFrame( kome::objects::Parameters* params )
 * @brief open chromatogram frame
 * @param params parameters information. (This object has chromatogram information.)
 * @return return value. (This value doesn't have the meanings.)
 */
kome::objects::Variant openChromatogramFrame( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant openDataMapFrame( kome::objects::Parameters* params )
 * @brief open data map frame
 * @param params parameters information. (This object has data map information.)
 * @return return value. (This value doesn't have the meanings.)
 */
kome::objects::Variant openDataMapFrame( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant closeAllSpectrumFrames( kome::objects::Parameters* )
 * @brief closes all spectrum frames
 * @return return value. (This value doesn't have the meanings.)
 */
kome::objects::Variant closeAllSpectrumFrames( kome::objects::Parameters* );

/**
 * @fn kome::objects::Variant closeAllChromatogramFrames( kome::objects::Parameters* )
 * @brief closes all chromatogram frames
 * @return return value. (This value doesn't have the meanings.)
 */
kome::objects::Variant closeAllChromatogramFrames( kome::objects::Parameters* );

/**
 * @fn kome::objects::Variant closeAllDataMapFrames( kome::objects::Parameters* )
 * @brief closes all data map frames
 * @return return value. (This value doesn't have the meanings.)
 */
kome::objects::Variant closeAllDataMapFrames( kome::objects::Parameters* );

/**
 * @fn kome::objects::Variant toggleAutoArrangeMode( kome::objects::Parameters* ) 
 * @brief toggles the Auto Arrange Mode on/off.
 * @return This function always returns true. (bool)
 */
kome::objects::Variant toggleAutoArrangeMode( kome::objects::Parameters* );

/**
 * @fn kome::objects::Variant setMdiWindowStatus( kome::objects::Parameters* params )
 * @brief sets MDI window status
 * @param params parameters object
 * @return This function always returns true
 */
kome::objects::Variant setMdiWindowStatus( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant cascadeChildren( kome::objects::Parameters* )
 * @brief cascade
 * @return This function always returns true. (boolean)
 */
kome::objects::Variant cascadeChildren( kome::objects::Parameters* );

/**
 * @fn kome::objects::Variant tileChildrenVertically( kome::objects::Parameters* )
 * @brief tile children
 * @return This function always returns true. (boolean)
 */
kome::objects::Variant tileChildrenVertically( kome::objects::Parameters* );

/**
 * @fn kome::objects::Variant tileChildren( kome::objects::Parameters* )
 * @brief tile children
 * @return This function always returns true. (boolean)
 */
kome::objects::Variant tileChildren( kome::objects::Parameters* );

/**
 * @fn kome::objects::Variant arrangeIcons( kome::objects::Parameters* )
 * @brief arrange icons
 * @return This function always returns true. (boolean)
 */
kome::objects::Variant arrangeIcons( kome::objects::Parameters* );

/**
 * @fn kome::objects::Variant onMoveChildFrame( kome::objects::Parameters* params )
 * @brief This function is called when a child frame is moved.
 * @param[in] params parameters
 * @return This value has no meanings.
 */
kome::objects::Variant onMoveChildFrame( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant onMovingChildFrame( kome::objects::Parameters* params )
 * @brief This function is called when a child frame is beging moved.
 * @param[in] params parameters
 * @return This value has no meanings.
 */
kome::objects::Variant onMovingChildFrame( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant onClientSize( kome::objects::Parameters* params )
 * @brief This method is called when the client size is changed.
 * @param[in] params parameters
 * @return This value has no meanings.
 */
kome::objects::Variant onClientSize( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant onChangeSettings( kome::objects::Parameters* params )
 * @brief This method is called when the settings is changed.
 * @param[in] params parameters
 * @return This value has no meanings.
 */
kome::objects::Variant onChangeSettings( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant activateNextChild( kome::objects::Parameters* )
 * @brief activate next child
 * @return This function always returns true. (boolean)
 */
kome::objects::Variant activateNextChild( kome::objects::Parameters* );

/**
 * @fn kome::objects::Variant activatePreviousChild( kome::objects::Parameters* )
 * @brief activate previous child
 * @return This function always returns true. (boolean)
 */
kome::objects::Variant activatePreviousChild( kome::objects::Parameters* );

/**
 * @fn kome::objects::Variant isNotAutoArrange( kome::objects::Parameters* )
 * @brief checks the auto arrange window mode is off
 * @return If true, the auto arrange window mode is off.
 */
kome::objects::Variant isNotAutoArrange( kome::objects::Parameters* );

/**
 * @fn kome::objects::Variant layoutChildrenOnClose( kome::objects::Parameters* params )
 * @brief layout children when a child frame is closed.
 * @param[in] params parameters
 * @return This value has no meanings.
 */
kome::objects::Variant layoutChildrenOnClose( kome::objects::Parameters* params );


#ifdef __unix
	}
#endif  // __unix



#endif// __KOME_VIEW_MDI_CHILD_FRAME_FUNCTIONS_H__

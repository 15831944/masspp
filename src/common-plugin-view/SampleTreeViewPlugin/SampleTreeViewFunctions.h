/**
 * @file SampleTreeViewFunctions.h
 * @brief SampleTreeView plug-in function interfaces
 *
 * @author S.Tanaka
 * @date 2008.01.10
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_VIEW_RAW_DATA_TREE_VIEW_FUNCTIONS_H__
#define __KOME_VIEW_RAW_DATA_TREE_VIEW_FUNCTIONS_H__


#ifdef __unix
	extern "C" {
#endif  // __unix

/**
 * @fn kome::objects::Variant initView( kome::objects::Parameters* )
 * @brief initializes tree view
 * @return return value (This return value doesn't have the meanings)
 */
kome::objects::Variant initView( kome::objects::Parameters* );

/**
 * @fn kome::objects::Variant finalizeView( kome::objects::Parameters* )
 * @brief finalizes tree view
 * @return return value (This value has no meanings.)
 */
kome::objects::Variant finalizeView( kome::objects::Parameters* );

/**
 * @fn kome::objects::Variant toggleTreeView( kome::objects::Parameters* )
 * @brief toggle visibility of the tree view
 * @return always true (boolean)
 */
kome::objects::Variant toggleTreeView( kome::objects::Parameters* );

/**
 * @fn kome::objects::Variant isVisibleTreeView( kome::objects::Parameters* )
 * @brief checks the visibility of the tree view
 * @return If true, the tree view is visible. (boolean value)
 */
kome::objects::Variant isVisibleTreeView( kome::objects::Parameters* );

/**
 * @fn kome::objects::Variant addSampleNode( kome::objects::Parameters* params )
 * @brief adds sample node
 * @param params parameters object
 * @return return value (This value sha no meanings.)
 */
kome::objects::Variant addSampleNode( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant updateGroup( kome::objects::Parameters* params )
 * @brief update group
 * @param params parameters object
 * @return return value (This value has no meanings.)
 */
kome::objects::Variant updateGroup( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant updateSample( kome::objects::Parameters* params )
 * @brief update sample
 * @param params parameters object
 * @return return value (This value has no meaning.)
 */
kome::objects::Variant updateSample( kome::objects::Parameters* params );


/**
 * @fn kome::objects::Variant selectSamplePage( kome::objects::Parameters* params )
 * @brief select sample page
 * @param[in] params parameters
 * @return This value has no meanings.
 */
kome::objects::Variant selectSamplePage( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant updateSpectrum( kome::objects::Parameters* params )
 * @brief navigates to the spectrum on open
 * @param params parameters object (This object has spectrum information.)
 * @return This function always returns true. (boolean)
 */
kome::objects::Variant updateSpectrum( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant onActivateObject( kome::objects::Parameters* params )
 * @brief on activate object
 * @param[in] params parameters object
 * @return return value (This value has no meanings.)
 */
kome::objects::Variant onActivateObject( kome::objects::Parameters* params );


#ifdef __unix
	}
#endif  // __unix



#endif		//  __KOME_VIEW_RAW_DATA_TREE_VIEW_FUNCTIONS_H__

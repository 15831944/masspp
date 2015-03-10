/**
 * @file DetailsViewFunctions.h
 * @brief interfaces of DetailsView plug-in functions
 *
 * @author S.Tanaka
 * @date 2007.02.13
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_VIEW_DETAILS_VIEW_FUNCTINOS_H__
#define __KOME_VIEW_DETAILS_VIEW_FUNCTINOS_H__


#ifdef __unix
	extern "C" {
#endif  // __unix


/**
 * @fn kome::objects::Variant initPropertiesView( kome::objects::Parameters* )
 * @brief initialize file details view
 * @return return value (This value has no meanings.)
 */
kome::objects::Variant initPropertiesView( kome::objects::Parameters* );

/**
 * @fn kome::objects::Variant changeActiveObject( kome::objects::Parameters* params )
 * @brief This function is called when active object is changed
 * @param params parameters object
 * @return return value (This value has no meanings.)
 */
kome::objects::Variant changeActiveObject( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant updateSpecDetails( kome::objects::Parameters* params )
 * @brief updates spectrum details
 * @param params parameters object (This object has spectrum information.)
 * @return return value (This value has no meanings.)
 */
kome::objects::Variant updateSpecDetails( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant togglePropertiesView( kome::objects::Parameters* )
 * @brief toggles visibility of file details view
 * @return This value is always true. (boolean)
 */
kome::objects::Variant togglePropertiesView( kome::objects::Parameters* );

/**
 * @fn kome::objects::Variant isVisibleProperties( kome::objects::Parameters* )
 * @brief checks the visibility of the file details view
 * @return If true, the file details view is visible. (boolean value)
 */
kome::objects::Variant isVisibleProperties( kome::objects::Parameters* );

/**
 * @fn kome::objects::Variant isVisibleSpectrumDetails( kome::objects::Parameters* )
 * @brief checks the visibility of the spectrum details view
 * @return If true, the spectrum details view is visible. (boolean value)
 */
kome::objects::Variant isVisibleSpectrumDetails( kome::objects::Parameters* );

/**
 * @fn kome::objects::Variant exitGui( kome::objects::Parameters* )
 * @brief releases details view
 * @return This value has no meanings.
 */
kome::objects::Variant exitGui( kome::objects::Parameters* );

#ifdef __unix
	}
#endif  // __unix



#endif		// __KOME_VIEW_DETAILS_VIEW_FUNCTINOS_H__

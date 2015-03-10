/**
 * @file PeakSearchFunctions.h
 * @brief interface of Peak Search plug-in functions
 *
 * @author S.Tanaka
 * @date 2008.09.08
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_PEAK_SEARCH_FUNCTIONS_H__
#define __KOME_PEAK_SEARCH_FUNCTIONS_H__


#ifdef __unix
	extern "C" {
#endif  // __unix


/**
 * @fn kome::objects::Variant openPeaksDifferenceSearchDialog( kome::objects::Parameters* params )
 * @brief opens peak difference search dialog
 * @return If true, it succeeded to open peak difference search dialog. (boolean)
 */
kome::objects::Variant openPeaksDifferenceSearchDialog( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant togglePeakSearchDialog( kome::objects::Parameters* )
 * @brief toggle visibility of the Peak Search Result dialog.
 * @return This function always return true. (boolean)
 */
kome::objects::Variant togglePeakSearchDialog( kome::objects::Parameters* );

/**
 * @fn kome::objects::Variant togglePeakDifferenceSearchDialog( kome::objects::Parameters* )
 * @brief toggle visibility of the Peak Difference Search Result dialog.
 * @return This function always return true. (boolean)
 */
kome::objects::Variant togglePeakDifferenceSearchDialog( kome::objects::Parameters* );


/**
 * @fn kome::objects::Variant isVisiblePeakSearchDialog( kome::objects::Parameters* )
 * @brief gets the visibility of the Peak Search Result dialog.
 * @return If ture, the Peak Search Result dialog is visible. (boolean)
 */
kome::objects::Variant isVisiblePeakSearchDialog( kome::objects::Parameters* );

/**
 * @fn kome::objects::Variant isVisiblePeakDifferenceSearchDialog( kome::objects::Parameters* )
 * @brief gets the visibility of the Peak Difference Result dialog.
 * @return If true, the Peak Difference Result dialog is visible. (boolean)
 */
kome::objects::Variant isVisiblePeakDifferenceSearchDialog( kome::objects::Parameters* );

/**
 * @fn kome::objects::Variant updateSettings( kome::objects::Parameters* )
 * @brief update settings
 * @return This value has no meanings.
 */
kome::objects::Variant updateSettings( kome::objects::Parameters* );

/**
 * @fn kome::objects::Variant exitGui( kome::objects::Parameters* )
 * @brief This function is called on exiting GUI
 * @return This value has no meanings.
 */
kome::objects::Variant exitGui( kome::objects::Parameters* );

#ifdef __unix
	}
#endif  // __unix



#endif		// __KOME_PEAK_SEARCH_FUNCTIONS_H__

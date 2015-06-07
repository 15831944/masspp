/**
 * @file MascotPMFForLCMSFunctions.h
 * @brief interfaces of plug-in functions
 *
 * @author S.Tanaka
 * @date 2012.10.26
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation All rights reserved.
 */



#ifndef __KOME_MASCOT_PMF_FOR_LCMS_FUNCTIONS_H__
#define __KOME_MASCOT_PMF_FOR_LCMS_FUNCTIONS_H__


/**
 * @fn kome::objects::Variant getPmfSearchEngine( kome::objects::Parameters* params )
 * @brief gets the PMF search engine
 * @params[in] params parameters
 * @return PMF search engine
 */
kome::objects::Variant getPmfSearchEngine( kome::objects::Parameters* params );


/**
 * @fn kome::objects::Variant checkModifications( kome::objects::Parameters* params )
 * @brief check modifications
 * @param[in] params parameters
 * @return error message (string)
*/
kome::objects::Variant checkModifications( kome::objects::Parameters* params );


#endif __KOME_MASCOT_PMF_FOR_LCMS_FUNCTIONS_H__

/**
 * @file HelpFunctions.h
 * @brief interfaces of log view plug-in functions
 *
 * @author S.Tanaka
 * @date 2007.03.16
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_VIEW_HELP_FUNCTIONS_H__
#define __KOME_VIEW_HELP_FUNCTIONS_H__


#ifdef __unix
	extern "C" {
#endif  // __unix


/**
 * @fn kome::objects::Variant showHelp( kome::objects::Parameters* )
 * @brief shows help
 * @return If false, it failed to create help information. (boolean)
 */
kome::objects::Variant showHelp( kome::objects::Parameters* );

/**
 * @fn kome::objects::Variant initDllHelp( kome::objects::Parameters* )
 * @brief DLLÇåƒÇ—èoÇ∑ÇæÇØÇÃèàóù
 * @return return value. (This value has no meanings.)
 */
kome::objects::Variant initDllHelp( kome::objects::Parameters* );	// 2010/08/16 APPEND OKADA

#ifdef __unix
	}
#endif  // __unix



#endif		// __KOME_VIEW_HELP_FUNCTIONS_H__

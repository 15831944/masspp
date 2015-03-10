/**
 * @file XTandemSearchPluginFunctions.h
 * @brief interfaces of plug-in functions
 *
 * @author N.Tashiro
 * @date 2012.01.25
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation. All rights reserved.
 */



#ifndef __KOME_XTANDEM_SEARCH_PLUGIN_FUNCTIONS_H__
#define __KOME_XTANDEM_SEARCH_PLUGIN_FUNCTIONS_H__

/**
 * @fn kome::objects::Variant initialiseSearchEngine( kome::objects::Parameters* params )
 * @brief initialise the search engine, i.e. create an instance
 * @param params parameters information
 * @return pointer to the search engine object (base class is SearchEngine)
 */
kome::objects::Variant initialiseSearchEngine( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant checkModifications( kome::objects::Parameters* params )
 * @brief check modifications
 * @param[in] params parameters
 * @return error message (string)
*/
kome::objects::Variant checkModifications( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant getResiduesFileList( kome::objects::Parameters* params )
 * @brief gets residues file list
 * @param[in] params parameters
 * @return This value has no meanings.
 */
kome::objects::Variant getResiduesFileList( kome::objects::Parameters* params );


#endif    // __KOME_XTANDEM_SEARCH_PLUGIN_FUNCTIONS_H__

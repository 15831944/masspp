/**
 * @file MascotSearchPluginFunctions.h
 * @brief interfaces of plug-in functions
 *
 * @author Parry
 * @date 2012.01.16
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation All rights reserved.
 */



#ifndef __KOME_MASCOT_SEARCH_PLUGIN_FUNCTIONS_H__
#define __KOME_MASCOT_SEARCH_PLUGIN_FUNCTIONS_H__


/**
 * @fn kome::objects::Variant getPmfSearchEngine( kome::objects::Parameters* params )
 * @brief gets the PMF search engine
 * @params[in] params parameters
 * @return PMF search engine
 */
kome::objects::Variant getPmfSearchEngine( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant getMisSearchEngine( kome::objects::Parameters* params )
 * @brief gets the MIS search engine
 * @param[in] params parameters
 * @return PMF search engine
 */
kome::objects::Variant getMisSearchEngine( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant initialiseSearchEngine( kome::objects::Parameters* params )
 * @brief initialise the search engine, i.e. create an instance
 * @param params parameters information
 * @return pointer to the search engine object (base class is SearchEngine)
 */
kome::objects::Variant initialiseSearchEngine( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant onUpdateSettings( kome::objects::Parameters* )
 * @brief This function is called when settings is updated.
 * @return return value (This value has no meanings.)
 */
kome::objects::Variant onUpdateSettings( kome::objects::Parameters* );

/**
 * @fn closeAllFileskome::objects::Variant logOutOfMascotServer( kome::objects::Parameters* )
 * @brief log out of the Mascot server
 * @return return value (This value has no meanings.)
 */
kome::objects::Variant logOutOfMascotServer( kome::objects::Parameters* );

/**
 * @fn kome::objects::Variant getPmfDbs( kome::objects::Parameters* params )
 * @brief gets PMS databases
 * @param[in] params parameters
 * @return This value has no meanings
 */
kome::objects::Variant getPmfDbs( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant getPmfTaxonomies( kome::objects::Parameters* params )
 * @brief gets PMF taxonomies
 * @param[in] params parameters
 * @return This value has no meanings.
 */
kome::objects::Variant getPmfTaxonomies( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant getMisDbs( kome::objects::Parameters* params )
 * @brief get MIS databases
 * @param[in] params parameters
 * @return This value has no meanings.
 */
kome::objects::Variant getMisDbs( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant getMisTaxonomies( kome::objects::Parameters* params )
 * @brief gets MIS taxonomies
 * @param[in] params parameters
 * @return This value has no meanings.
 */
kome::objects::Variant getMisTaxonomies( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant checkModifications( kome::objects::Parameters* params )
 * @brief check modifications
 * @param[in] params parameters
 * @return error message (string)
*/
kome::objects::Variant checkModifications( kome::objects::Parameters* params );



#endif    // __KOME_MASCOT_SEARCH_PLUGIN_FUNCTIONS_H__

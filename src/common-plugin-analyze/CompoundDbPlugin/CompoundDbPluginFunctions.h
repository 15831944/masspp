/**
 * @file CompoundDbPluginFunctions.h
 * @brief interfaces of plug-in functions
 *
 * @author tanaka-s
 * @date 2013.08.06
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation All rights reserved.
 */



#ifndef __KOME_COMPOUND_DB_PLUGIN_FUNCTIONS_H__
#define __KOME_COMPOUND_DB_PLUGIN_FUNCTIONS_H__


/**
 * @fn kome::objects::Variant getCasDb( kome::objects::Parameters* )
 * @brief gets the CAS database
 * @return CAS database
 */
kome::objects::Variant getCasDb( kome::objects::Parameters* );

/**
 * @fn kome::objects::Variant getChebiDb( kome::objects::Parameters* )
 * @brief gets the ChEBI database
 * @return ChEBI database
 */
kome::objects::Variant getChebiDb( kome::objects::Parameters* );

/**
 * @fn kome::objects::Variant getKeggDb( kome::objects::Parameters* )
 * @brief gets the KEGG database
 * @return KEGG database
 */
kome::objects::Variant getKeggDb( kome::objects::Parameters* );

/**
 * @fn kome::objects::Variant getKnapsackDb( kome::objects::Parameters* )
 * @brief gets the KNApSAcK database
 * @return KNApSAcK database
 */
kome::objects::Variant getKnapsackDb( kome::objects::Parameters* );

/**
 * @fn kome::objects::Variant getLipidbankDb( kome::objects::Parameters* )
 * @brief gets the LipidBank database
 * @return LipidBank database
 */
kome::objects::Variant getLipidmapsDb( kome::objects::Parameters* );

/**
 * @fn kome::objects::Variant getLipidmapsDb( kome::objects::Parameters* )
 * @brief gets the LIPID MAPS database
 * @return LIPID MAPS database
 */
kome::objects::Variant getLipidmapsDb( kome::objects::Parameters* );

/**
 * @fn kome::objects::Variant getPubchemDb( kome::objects::Parameters* )
 * @brief gets the PubChem database
 * @return PubChem database
 */
kome::objects::Variant getPubchemDb( kome::objects::Parameters* );


#endif    // __KOME_COMPOUND_DB_PLUGIN_FUNCTIONS_H__

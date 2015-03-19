/**
 * @file IdentificationPluginFunctions.h
 * @brief interfaces of plug-in functions
 *
 * @author H. Parry
 * @date 2012.01.16
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation All rights reserved.
 */



#ifndef __KOME_IDENTIFICATION_PLUGIN_FUNCTIONS_H__
#define __KOME_IDENTIFICATION_PLUGIN_FUNCTIONS_H__


/**
 * @fn kome::objects::Variant identificationOnBatch( kome::objects::Parameters* params )
 * @brief executes identification for batch processing
 * @param params This object has filtered spectrum information
 * @return This function always returns true. (boolean)
 */
kome::objects::Variant identificationOnBatch( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant openMgf( kome::objects::Parameters* params )
 * @brief open mgf file
 * @param params This object has file path
 * @return MgfSampleSet
 */
kome::objects::Variant openMgf( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant saveMisMgf( kome::objects::Parameters* params )
 * @brief save peak list in mascot generic format (MIS)
 * @param params parameter information object. (This parameter has path, spectra and progress information.)
 * @return return value. (If it failed to save peak list, this method returns false. [bool value])
 */
kome::objects::Variant saveMisMgf( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant savePmfText( kome::objects::Parameters* params )
 * @brief save peak list in text file
 * @param params parameter information object. (This parameter has path, spectra and progress information.)
 * @return return value. (If it failed to save peak list, this method returns false. [bool value])
 */
kome::objects::Variant savePmfText( kome::objects::Parameters* params );

// @date 2013/03/21 <Add> OKADA
/**
 * @fn kome::objects::Variant savePmfTextChrom( kome::objects::Parameters* params )
 * @brief save peak list in text file for Chromatogram
 * @param params parameter information object. (This parameter has path, spectra and progress information.)
 * @return return value. (If it failed to save peak list, this method returns false. [bool value])
 */
kome::objects::Variant savePmfTextChrom( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant toggleSearchResultWindow( kome::objects::Parameters* )
 * @brief toggles the visibility of the search result window
 * @return This function always returns true. (boolean)
 */
kome::objects::Variant toggleSearchResultWindow( kome::objects::Parameters* );

/**
 * @fn kome::objects::Variant isVisibleSearchResult( kome::objects::Parameters* )
 * @brief check the visibility of the search result window
 * @return If true the search result window is shown.
 */
kome::objects::Variant isVisibleSearchResult( kome::objects::Parameters* );

/**
 * @fn kome::objects::Variant openIdentificationDialog( kome::objects::Parameters* params )
 * @brief opens the Identification dialog
 * @param[in] params parameters
 * @return If true, the Identification dialog is opened successfully. (boolean)
 */
kome::objects::Variant openIdentificationDialog( kome::objects::Parameters* params );
   
/**
 * @fn kome::objects::Variant identification( int iSearchEngineIndex, kome::plugin::PluginFunctionItem* funcItem, bool bComparativeId )
 * @brief executes search
 * @param iSearchEngineIndex. The search engine index.
 * @param funcItem. The plugin function item for the selected search engine.
 * @param bComparativeId. If true, show the Comparative Identification dialog, else the Identification dialog.
 * @return If true, search was finished successfully. (boolean)
 */
kome::objects::Variant identification( int iSearchEngineIndex, kome::plugin::PluginFunctionItem* funcItem, bool bComparativeId );

/**
 * @fn bool executeSearchIdentification( kome::objects::Parameters* )
 * @brief invokes the (Comparative) Identification dialog for a search engine.
 * @param[in] obj. The active object.
 * @param[in] id. The menu ID of the menu item selected.
 * @return If true, it succeeded. (boolean)
 */
bool executeSearchIdentification( kome::objects::Variant obj, int id );

/**
 * @fn bool enableSearchIdentification( kome::objects::Parameters* )
 * @brief enables the (Comparative) Identification menu item for a search engine.
 * @param[in] obj. The active object.
 * @param[in] id. The menu ID of the menu item selected.
 * @return If true, it succeeded. (boolean)
 */
bool enableSearchIdentification( kome::objects::Variant obj, int id );

/**
 * @fn kome::objects::Variant getDatabaseList( kome::objects::Prameters* params )
 * @brief gets the database list
 * @param[in] params parameters 
 * @return This value has no meaning.
 */
kome::objects::Variant getDatabaseList( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant getDbFileList( kome::objects::Parameters* params )
 * @brief gets the DB file list
 * @param[in] params parameters
 * @return This valkue has no meaning.
 */
kome::objects::Variant getDbFileList( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant getEnzymeList( kome::objects::Prameters* params )
 * @brief gets the enzyme list
 * @param[in] params parameters 
 * @return This value has no meaning.
 */
kome::objects::Variant getEnzymeList( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant getModificationList( kome::objects::Prameters* params )
 * @brief gets the modification list
 * @param[in] params parameters 
 * @return This value has no meaning.
 */
kome::objects::Variant getModificationList( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant getIdentificationPanel( kome::objects::Parameters* params )
 * @brief get identification panel
 * @return This function always returns true. (boolean)
 */
kome::objects::Variant getIdentificationPanel( kome::objects::Parameters* params );
/**
 * @fn kome::objects::Variant showResultViewFromSearchEngine( kome::objects::Parameters* )
 * @brief show the visibility of the search result window
 * @return This function always returns true. (boolean)
 */
kome::objects::Variant showResultViewFromSearchEngine( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant showSubstances( kome::objects::Parameters* )
 * @brief toggles the Substances window
 * @return This function always returns true. (boolean)
 */
kome::objects::Variant showSubstances( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant showIdentSpectrum( kome::objects::Parameters* )
 * @brief show the spectrum window
 * @return This function always returns true. (boolean)
 */
kome::objects::Variant showIdentSpectrum( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant testIdentSubstance( kome::objects::Parameters* )
 * @brief show the test window
 * @return This function always returns true. (boolean)
 */
kome::objects::Variant testIdentSubstance( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant execComparativeIdent( kome::objects::Parameters* params )
 * @brief add matrix sample
 * @param[in] params parameters object
 * @return If true, This function is succeeded. (boolean)
*/
kome::objects::Variant execComparativeIdent( kome::objects::Parameters* params );	// @date 2013/07/26 Add OKADA task:#22 (17)-comparative_ident


/**
 * @fn kome::objects::Variant exportPepXML( kome::objects::Parameters* params )
 * @brief writes pepXML file
 * @param[in] parameters
 * @return If true, writing pepXML file finished succussfully.
 */
kome::objects::Variant exportPepXML( kome::objects::Parameters* params );


kome::ident::SpectrumGetter* createSpectrumGetter( kome::ident::SearchEngine* engine, double rt_tol, double mz_tol );



#endif    // __KOME_IDENTIFICATION_PLUGIN_FUNCTIONS_H__

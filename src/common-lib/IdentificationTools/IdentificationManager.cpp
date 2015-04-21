/**
 * @file IdentificationManager.cpp
 * @brief implementation of IdentificationManager class
 *
 * @author H.Parry
 * @date 2012.01.25
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation. All rights reserved.
 */


#include "stdafx.h"
#include "Substance.h"
#include "Hit.h"
#include "SearchResult.h"
#include "SearchEngine.h"
#include "Enzyme.h"
#include "Modification.h"
#include "IdentificationManager.h"
#include "ModificationHandler.h"
#include "EnzymeHandler.h"
#include "LocalDbHandler.h"
#include "MgfManager.h"

#include "PeptideSubstance.h"
#include "ProteinSubstance.h"

#include <map>
#include <boost/bind.hpp>
#include <boost/filesystem.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>


using namespace kome::ident;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



#define IDENTIFICATION_TYPE		           "identification"

#define DB_VERSION                         9

#define TABLE_SAMPLE_SET                   "MSPP_SAMPLE_SET"
#define TABLE_SAMPLE                       "MSPP_SAMPLE"
#define TABLE_SPECTRUM                     "MSPP_SPECTRUM"
#define TABLE_VERSION                      "MSPP_DB_VERSION"
#define TABLE_DATABASE                     "MSPP_DATABASE"
#define TABLE_DATA_FORMAT                  "MSPP_PUBLIC_DATA_FORMAT"
#define TABLE_URL                          "MSPP_PUBLIC_DATA_URL"
#define TABLE_ENGINE                       "MSPP_SEARCH_ENGINE"
#define TABLE_SEARCH                       "MSPP_DB_SEARCH"
#define TABLE_SUBSTANCE                    "MSPP_SUBSTANCE"
#define TABLE_RELATIONSHIP                 "MSPP_SUBSTANCE_RELATIONSHIP"
#define TABLE_SUBSTANCE_ITEM               "MSPP_SUBSTANCE_ITEM"
#define TABLE_SUBSTANCE_VALUE              "MSPP_SUBSTANCE_VALUE"
#define TABLE_PEAK_DETECTION               "MSPP_PEAK_DETECTION"
#define TABLE_PEAK_PARAMS                  "MSPP_PEAK_DETECTION_PARAMETERS"
#define TABLE_HIT                          "MSPP_HIT"
#define TABLE_HIT_SPEC                     "MSPP_HIT_SPECTRUM"
#define TABLE_HIT_ITEM                     "MSPP_HIT_ITEM"
#define TABLE_HIT_VALUE                    "MSPP_HIT_VALUE"
#define TABLE_SEARCH_PARAMS                "MSPP_SEARCH_PARAMETER"
#define TABLE_SEARCH_VALUE                 "MSPP_SEARCH_PARAMETER_VALUE"
#define TABLE_SEARCH_SPEC                  "MSPP_SEARCH_SPECTRUM"
#define TABLE_MATRIX                       "MSPP_PEAK_MATRIX"
#define TABLE_GROUP                        "MSPP_PEAK_MATRIX_GROUP"
#define TABLE_MATRIX_SAMPLE                "MSPP_PEAK_MATRIX_SAMPLE"
#define TABLE_PEAK                         "MSPP_PEAK_MATRIX_PEAK"
#define TABLE_MATRIX_COL                   "MSPP_PEAK_MATRIX_APPENDED_COLUMN"
#define TABLE_MATRIX_VAL                   "MSPP_PEAK_MATRIX_APPENDED_VALUE"
#define TABLE_MATRIX_SPEC                  "MSPP_SPECTRUM_FOR_IDENTIFICATION"
#define TABLE_PROFILE                      "MSPP_PROFILE"
#define TABLE_PEAK_VAL                     "MSPP_PEAK_MATRIX_VALUE"
#define TABLE_ACCESSION                    "MSPP_RECORD_ACCESSION"
#define TABLE_STANDARD_SAMPLE              "MSPP_PEAK_MATRIX_STANDARD_SAMPLE"
#define TABLE_NORM_FILTER                  "MSPP_NORMALIZATION_FILTER"
#define TABLE_NORM_METHOD                  "MSPP_NORMALIZATION_METHOD"
#define TABLE_ALIGN                        "MSPP_RT_ALIGNMENT"
#define TABLE_ALT_SUBSTANCE                "MSPP_ALT_SUBSTANCE"
#define TABLE_PEAK_LIST                    "MSPP_PEAK_LIST"
#define TABLE_MOD                          "MSPP_MOD"
#define TABLE_SEARCH_MOD                   "MSPP_SEARCH_MOD"
#define TABLE_HIT_MOD                      "MSPP_HIT_MOD"
#define TABLE_ENZYME                       "MSPP_ENZYME"
#define TABLE_ENZYME_CUTTER                "MSPP_ENZYME_CUTTER"

#define COL_ID_SAMPLE_SET                  "ID"
#define COL_PATH_SAMPLE_SET                "PATH"
#define COL_ID_SAMPLE                      "ID"
#define COL_SAMPLE_SET_SAMPLE              "SAMPLE_SET"
#define COL_SAMPLE_ID_SAMPLE               "SAMPLE_ID"
#define COL_NAME_SAMPLE                    "NAME"
#define COL_ID_SPECTRUM                    "ID"
#define COL_SAMPLE_SPECTRUM                "SAMPLE"
#define COL_SPECTRUM_ID_SPECTRUM           "SPECTRUM_ID"
#define COL_NAME_SPECTRUM                  "NAME"
#define COL_STAGE_SPECTRUM                 "STAGE"
#define COL_RT_SPECTRUM                    "RT"
#define COL_PRECURSOR_SPECTRUM             "PRECURSOR"
#define COL_POLARITY_SPECTRUM              "POLARITY"
#define COL_VERSION_VERSION                "VERSION"
#define COL_ID_DATABASE                    "ID"
#define COL_NAME_DATABASE                  "NAME"
#define COL_URL_DATABASE                   "URL"
#define COL_ID_FORMAT                      "ID"
#define COL_NAME_FORMAT                    "NAME"
#define COL_ID_URL                         "ID"
#define COL_DB_URL                         "DB"
#define COL_FORMAT_URL                     "FORMAT"
#define COL_URL_URL                        "URL"
#define COL_ID_ENGINE                      "ID"
#define COL_NAME_ENGINE                    "NAME"
#define COL_VERSION_ENGINE                 "VERSION"
#define COL_ID_SEARCH                      "ID"
#define COL_NAME_SEARCH                    "NAME"
#define COL_COMMENT_SEARCH                 "COMMENT"
#define COL_DATE_SEARCH                    "DATE"
#define COL_ENGINE_SEARCH                  "SEARCH_ENGINE"
#define COL_DB_SEARCH                      "DATABASE"
#define COL_RESULT_SEARCH                  "RESULT_DATA"
#define COL_PREC_MASS_TYPE_SEARCH          "PRECURSOR_MASS_TYPE"
#define COL_FRAG_MASS_TYPE_SEARCH          "FRAGMENT_MASS_TYPE"
#define COL_ENZ_SEARCH                     "ENZ"
#define COL_BASE_NAME_SEARCH               "BASE_NAME"
#define COL_SEARCH_ID_SEARCH               "SEARCH_ID"
#define COL_STATUS_SEARCH                  "STATUS"
#define COL_ID_SUBSTANCE                   "ID"
#define COL_ACCESSION_SUBSTANCE            "ACCESSION"
#define COL_NAME_SUBSTANCE                 "NAME"
#define COL_DATABASE_SUBSTANCE             "DATABASE"
#define COL_TYPE_SUBSTANCE                 "TYPE"
#define COL_ID_RELATIONSHIP                "ID"
#define COL_A_RELATIONSHIP                 "SUBSTANCE_A"
#define COL_B_RELATIONSHIP                 "SUBSTANCE_B"
#define COL_RELATIONSHIP_RELATIONSHIP      "RELATIONSHIP"
#define COL_ID_SUBSTANCE_ITEM              "ID"
#define COL_NAME_SUBSTANCE_ITEM            "NAME"
#define COL_DB_SUBSTANCE_ITEM              "DATABASE"
#define COL_TYPE_SUBSTANCE_ITEM            "TYPE"
#define COL_ID_SUBSTANCE_VAL               "ID"
#define COL_ITEM_SUBSTANCE_VAL             "ITEM"
#define COL_SUBSTANCE_SUBSTANCE_VAL        "SUBSTANCE"
#define COL_VALUE_SUBSTANCE_VAL            "VALUE"
#define COL_ID_PEAK_DETECTION              "ID"
#define COL_METHOD_PEAK_DETECTION          "METHOD"
#define COL_ID_PEAK_PARAMS                 "ID"
#define COL_DETECTION_PEAK_PARAMS          "DETECTION"
#define COL_NAME_PEAK_PARAMS               "PARAM_NAME"
#define COL_VALUE_PEAK_PARAMS              "PARAM_VALUE"
#define COL_ID_HIT                         "ID"
#define COL_ID_HIT_SPEC                    "ID"
#define COL_HIT_HIT_SPEC                   "HIT"
#define COL_SPEC_HIT_SPEC                  "SPECTRUM"
#define COL_CHARGE_HIT_SPEC                "CHARGE"
#define COL_SUBSTANCE_HIT                  "SUBSTANCE"
#define COL_ID_HIT_ITEM                    "ID"
#define COL_NAME_HIT_ITEM                  "NAME"
#define COL_ENGINE_HIT_ITEM                "SEARCH_ENGINE"
#define COL_SUB_TYPE_HIT_ITEM              "SUBSTANCE_TYPE"
#define COL_VAL_TYPE_HIT_ITEM              "VALUE_TYPE"
#define COL_ID_HIT_VAL                     "ID"
#define COL_ITEM_HIT_VAL                   "ITEM"
#define COL_HIT_HIT_VAL                    "HIT"
#define COL_VAL_HIT_VAL                    "VALUE"
#define COL_ID_SEARCH_PARAMS               "ID"
#define COL_NAME_SEARCH_PARAMS             "NAME"
#define COL_ENGINE_SEARCH_PARAMS           "ENGINE"
#define COL_ID_SEARCH_VAL                  "ID"
#define COL_SEARCH_SEARCH_VAL              "SEARCH"
#define COL_ITEM_SEARCH_VAL                "PARAMETER"
#define COL_VALUE_SEARCH_VAL               "VALUE"
#define COL_ID_SEARCH_SPEC                 "ID"
#define COL_SEARCH_SEARCH_SPEC             "SEARCH"
#define COL_SPEC_SEARCH_SPEC               "SPECTRUM_ID"
#define COL_CHARGE_SEARCH_SPEC             "CHARGE_FLG"
#define COL_PRECURSOR_SEARCH_SPEC          "PRECURSOR"
#define COL_PEAKS_SEARCH_SPEC              "PEAKS"
#define COL_ID_MATRIX                      "ID"
#define COL_NAME_MATRIX                    "NAME"
#define COL_COMMENT_MATRIX                 "COMMENT"
#define COL_RT_MATRIX                      "RT_FLG"
#define COL_MZ_MATRIX                      "MZ_FLG"
#define COL_CHARGE_MATRIX                  "CHARGE_FLG"
#define COL_SELECTED_MATRIX                "SELECTED"
#define COL_ID_GROUP                       "ID"
#define COL_MATRIX_GROUP                   "MATRIX"
#define COL_NAME_GROUP                     "NAME"
#define COL_COLOR_GROUP                    "COLOR"
#define COL_ORDER_GROUP                    "GROUP_ORDER"
#define COL_ID_MATRIX_SAMPLE               "ID"
#define COL_SAMPLE_MATRIX_SAMPLE           "SAMPLE"
#define COL_GROUP_MATRIX_SAMPLE            "MATRIX_GROUP"
#define COL_NAME_MATRIX_SAMPLE             "NAME"
#define COL_COLOR_MATRIX_SAMPLE            "COLOR"
#define COL_ID_PEAK                        "ID"
#define COL_MATRIX_PEAK                    "MATRIX"
#define COL_RT_PEAK                        "RT"
#define COL_MZ_PEAK                        "MZ"
#define COL_CHARGE_PEAK                    "CHARGE"
#define COL_ID_MATRIX_COL                  "ID"
#define COL_MATRIX_MATRIX_COL              "MATRIX"
#define COL_NAME_MATRIX_COL                "NAME"
#define COL_TYPE_MATRIX_COL                "TYPE"
#define COL_ID_MATRIX_VAL                  "ID"
#define COL_COL_MATRIX_VAL                 "COL"
#define COL_PEAK_MATRIX_VAL                "PEAK"
#define COL_VALUE_MATRIX_VAL               "VALUE"
#define COL_ID_MATRIX_SPEC                 "ID"
#define COL_PEAK_MATRIX_SPEC               "MATRIX_PEAK"
#define COL_SPEC_MATRIX_SPEC               "SPECTRUM"
#define COL_ID_PROFILE                     "ID"
#define COL_CHROM_PROFILE                  "IS_CHROM"
#define COL_DATA_PROFILE                   "DATA"
#define COL_ID_PEAK_VAL                    "ID"
#define COL_PEAK_PEAK_VAL                  "MATRIX_PEAK"
#define COL_SAMPLE_PEAK_VAL                "SAMPLE"
#define COL_VALUE_PEAK_VAL                 "PEAK_VALUE"
#define COL_NORMALIZED_PEAK_VAL            "NORMALIZED_VALUE"
#define COL_POS_PEAK_VAL                   "PEAK_POSITION"
#define COL_LX_PEAK_VAL                    "LEFT_X"
#define COL_LY_PEAK_VAL                    "LEFT_Y"
#define COL_RX_PEAK_VAL                    "RIGHT_X"
#define COL_RY_PEAK_VAL                    "RIGHT_Y"
#define COL_PROFILE_PEAK_VAL               "PROFILE"
#define COL_ID_ACCESSION                   "ID"
#define COL_PREFIX_ACCESSION               "PREFIX"
#define COL_COUNT_ACCESSION                "ACCESSION_COUNT"
#define COL_ID_STANDARD                    "ID"
#define COL_MATRIX_STANDARD                "MATRIX"
#define COL_SAMPLE_STANDARD                "SAMPLE"
#define COL_ID_NORM_FILTER                 "ID"
#define COL_MATRIX_NORM_FILTER             "MATRIX"
#define COL_MIN_INT_NORM_FILTER            "MIN_INTENSITY"
#define COL_MAX_INT_NORM_FILTER            "MAX_INTENSITY"
#define COL_REL_FLG_NORM_FILTER            "REL_FLG"
#define COL_MIN_RT_NORM_FILTER             "MIN_RT"
#define COL_MAX_RT_NORM_FILTER             "MAX_RT"
#define COL_MIN_MZ_NORM_FILTER             "MIN_MZ"
#define COL_MAX_MZ_NORM_FILTER             "MAX_MZ"
#define COL_METHOD_NORM_FILTER             "METHOD"
#define COL_PARAMS_NORM_FILTER             "PARAMS"
#define COL_ID_NORM_METHOD                 "ID"
#define COL_FILTER_NORM_METHOD             "FILTER"
#define COL_ORDER_NORM_METHOD              "METHOD_ORDER"
#define COL_METHOD_NORM_METHOD             "METHOD"
#define COL_PARAMS_NORM_METHOD             "PARAMS"
#define COL_ID_ALIGN                       "ID"
#define COL_MATRIX_ALIGN                   "MATRIX"
#define COL_METHOD_ALIGN                   "METHOD"
#define COL_PARAMS_ALIGN                   "PARAMS"
#define COL_ID_ALT_SUBSTANCE               "ID"
#define COL_HIT_ALT_SUBSTANCE              "HIT"
#define COL_SUBSTANCE_ALT_SUBSTANCE        "SUBSTANCE"
#define COL_START_ALT_SUBSTANCE            "START_POS"
#define COL_END_ALT_SUBSTANCE              "END_POS"
#define COL_BEFORE_ALT_SUBSTANCE           "BEFORE_SEQ"
#define COL_AFTER_ALT_SUBSTANCE            "AFTER_SEQ"
#define COL_ID_PEAK_LIST                   "ID"
#define COL_PEAKS_PEAK_LIST                "PEAK_LIST"
#define COL_DETECTION_PEAK_LIST            "PEAK_DETECTION"
#define COL_ID_MOD                         "ID"
#define COL_NAME_MOD                       "NAME"
#define COL_TYPE_MOD                       "TYPE"
#define COL_MASS_MOD                       "MASS"
#define COL_HIDDEN_MOD                     "IS_HIDDEN"
#define COL_ID_SEARCH_MOD                  "ID"
#define COL_SEARCH_SEARCH_MOD              "SEARCH"
#define COL_MOD_SEARCH_MOD                 "MODIFICATION"
#define COL_VARIABLE_SEARCH_MOD            "IS_VARIABLE"
#define COL_ID_HIT_MOD                     "ID"
#define COL_HIT_HIT_MOD                    "HIT"
#define COL_MOD_HIT_MOD                    "SEARCH_MOD"
#define COL_POS_HIT_MOD                    "POSITION"
#define COL_ID_ENZYME                      "ID"
#define COL_NAME_ENZYME                    "NAME"
#define COL_HIDDEN_ENZYME                  "IS_HIDDEN"
#define COL_ID_ENZYME_CUTTER               "ID"
#define COL_ENZYME_ENZYME_CUTTER           "ENZYME"
#define COL_TERM_ENZYME_CUTTER             "TERM"
#define COL_CLEAVE_ENZYME_CUTTER           "CLEAVE"
#define COL_RESTRICT_ENZYME_CUTTER         "RESTRICT"

#define INDEX_ID_SAMPLE_SET                "IDX_ID_SAMPLE_SET"
#define INDEX_PATH_SAMPLE_SET              "IDX_PATH_SAMPLE_SET"
#define INDEX_ID_SAMPLE                    "IDX_ID_SAMPLE"
#define INDEX_SAMPLE_SET_SAMPLE            "IDX_SAMPLE_SET_SAMPLE"
#define INDEX_ID_SPECTRUM                  "IDX_ID_SPECTRUM"
#define INDEX_SAMPLE_SPECTRUM              "IDX_SAMPLE_SPECTRUM"
#define INDEX_STAGE_SPECTRUM               "IDX_STAGE_SPECTRUM"
#define INDEX_PRECURSOR_SPECTRUM           "IDX_PRECURSOR_SPECTRUM"
#define INDEX_ID_DATABASE                  "IDX_ID_DATABASE"
#define INDEX_NAME_DATABASE                "IDX_NAME_DATABASE"
#define INDEX_ID_FORMAT                    "IDX_ID_FORMAT"
#define INDEX_NAME_FORMAT                  "IDX_NAME_FORMAT"
#define INDEX_ID_URL                       "IDX_ID_URL"
#define INDEX_DB_FORMAT_URL                "IDX_DB_FORMAT_URL"
#define INDEX_ID_ENGINE                    "IDX_ID_ENGINE"
#define INDEX_NAME_ENGINE                  "IDX_NAME_ENGINE"
#define INDEX_ID_SEARCH                    "IDX_ID_SEARCH"
#define INDEX_DATE_SEARCH                  "IDX_DATE_SEARCH"
#define INDEX_ENGINE_SEARCH                "IDX_ENGINE_SEARCH"
#define INDEX_DB_SEARCH                    "IDX_DB_SEARCH"
#define INDEX_SEARCH_ID_SEARCH             "IDX_SEARCH_ID_SEARCH"
#define INDEX_ID_SUBSTANCE                 "IDX_ID_SUBSTANCE"
#define INDEX_ACCESSION_SUBSTANCE          "IDX_ACCESSION_SUBSTANCE"
#define INDEX_DB_SUBSTANCE                 "IDX_DB_SUBSTANCE"
#define INDEX_TYPE_SUBSTANCE               "IDX_TYPE"
#define INDEX_ID_RELATIONSHIP              "IDX_ID_RELATIONSHIP"
#define INDEX_A_RELATIONSHIP               "IDX_A_RELATIONSHIP"
#define INDEX_B_RELATIONSHIP               "IDX_B_RELATIONSHIP"
#define INDEX_RELATIONSHIP_RELATIONSHIP    "IDX_RELATIONSHIP_RELATIONSHIP"
#define INDEX_ID_SUBSTANCE_ITEM            "IDX_ID_SUBSTANCE_ITEM"
#define INDEX_DB_TYPE_SUBSTANCE_ITEM       "IDX_DB_SUBSTANCE_ITEM"
#define INDEX_ID_SUBSTANCE_VAL             "IDX_ID_SUBSTANCE_VALUE"
#define INDEX_ITEM_SUBSTANCE_VAL           "IDX_ITEM_SUBSTANCE_VALUE"
#define INDEX_SUBSTANCE_SUBSTANCE_VAL      "IDX_SUBSTANCE_SUBSTANCE_VALUE"
#define INDEX_ID_PEAK_PARAMS               "IDX_ID_PEAK_PARAMS"
#define INDEX_DETECTION_PEAK_PARAMS        "IDX_DETECTION_PEAK_PARAMS"
#define INDEX_ID_PEAK_DETECTION            "IDX_ID_PEAK_DETECTION"
#define INDEX_DETECT_PEAK_PARAMS           "IDX_DETECT_PEAK_PARAMS"
#define INDEX_ID_HIT                       "IDX_ID_HIT"
#define INDEX_SUBSTANCE_HIT                "IDX_SUBSTANCE_HIT"
#define INDEX_ID_HIT_SPEC                  "IDX_ID_HIT_SPECTRUM"
#define INDEX_HIT_HIT_SPEC                 "IDX_HIT_HIT_SPECTRUM"
#define INDEX_SPEC_HIT_SPEC                "IDX_SPECTRUM_HIT_SPECTRUM"
#define INDEX_ID_HIT_ITEM                  "IDX_ID_HIT_ITEM"
#define INDEX_ENGINE_HIT_ITEM              "IDX_SEARCH_ENGINE_HIT_ITEM"
#define INDEX_ID_HIT_VAL                   "IDX_ID_HIT_VALUE"
#define INDEX_ITEM_HIT_VAL                 "IDX_ITEM_HIT_VALUE"
#define INDEX_HIT_HIT_VAL                  "IDX_HIT_HIT_VALUE"
#define INDEX_ID_SEARCH_PARAMS             "IDX_ID_SEARCH_PARAM"
#define INDEX_ENGINE_SEARCH_PARAMS         "IDX_SEARCH_ENGINE_SEARCH_PARAMS"
#define INDEX_ID_SEARCH_VAL                "IDX_ID_SEARCH_VAL"
#define INDEX_SEARCH_SEARCH_VAL            "IDX_SEARCH_SEARCH_VAL"
#define INDEX_PARAM_SEARCH_VAL             "IDX_PARAMETERS_SEARCH_VAL"
#define INDEX_ID_SEARCH_SPEC               "IDX_ID_SEARCH_SPECTRUM"
#define INDEX_SEARCH_SEARCH_SPEC           "IDX_SEARCH_SEARCH_SPECTRUM"
#define INDEX_SPEC_SEARCH_SPEC             "IDX_SPECTRUM_SEARCH_SPECTRUM"
#define INDEX_ID_MATRIX                    "IDX_ID_MATRIX"
#define INDEX_SELECTED_MATRIX              "IDX_SELECTED_MATRIX"
#define INDEX_ID_GROUP                     "IDX_ID_GROUP"
#define INDEX_MATRIX_GROUP                 "IDX_MATRIX_GROUP"
#define INDEX_ID_MATRIX_SAMPLE             "IDX_ID_MATRIX_SAMPLE"
#define INDEX_SAMPLE_MATRIX_SAMPLE         "IDX_SAMPLE_MATRIX_SAMPLE"
#define INDEX_GROUP_MATRIX_SAMPLE          "IDX_GROUP_MATRIX_SAMPLE"
#define INDEX_GROUP_SAMPLE_MATRIX_SAMPLE   "IDX_GROUP_SAMPLE_MATRIX_SAMPLE"
#define INDEX_ID_PEAK                      "IDX_ID_PEAK"
#define INDEX_MATRIX_PEAK                  "IDX_MATRIX_PEAK"
#define INDEX_RT_PEAK                      "IDX_RT_PEAK"
#define INDEX_MZ_PEAK                      "IDX_MZ_PEAK"
#define INDEX_RT_MZ_PEAK                   "IDX_RT_MZ_PEAK"
#define INDEX_ID_MATRIX_COL                "IDX_ID_MATRIX_COLUMN"
#define INDEX_MATRIX_MATRIX_COL            "IDX_MATRIX_MATRIX_COLUMN"
#define INDEX_ID_MATRIX_VAL                "IDX_ID_MATRIX_VALUE"
#define INDEX_COL_MATRIX_VAL               "IDX_COL_MATRIX_VALUE"
#define INDEX_ID_MATRIX_SPEC               "IDX_ID_MATRIX_SPECTRUM"
#define INDEX_PEAK_MATRIX_SPEC             "IDX_PEAK_MATRIX_SPECTRUM"
#define INDEX_SPECTRUM_MATRIX_SPEC         "IDX_SPECTRUM_MATRIX_SPECTRUM"
#define INDEX_PEAK_SPECTRUM_MATRIX_SPEC    "IDX_PEAK_SPECTRUM_MATRIX_SPECTRUM"
#define INDEX_ID_PROFILE                   "IDX_ID_PROFILE"
#define INDEX_ID_PEAK_VAL                  "IDX_ID_PEAK_VALUE"
#define INDEX_PEAK_SAMPLE_PEAK_VAL         "IDX_PEAK_SAMPLE_PEAK_VALUE"
#define INDEX_PEAK_PEAK_VAL                "IDX_PEAK_PEAK_VALUE"
#define INDEX_SAMPLE_PEAK_VAL              "IDX_SAMPLE_PEAK_VALUE"
#define INDEX_ID_ACCESSION                 "IDX_ID_ACCESSION"
#define INDEX_PREFIX_ACCESSION             "IDX_PREFIX_ACCESSION"
#define INDEX_MATRIX_STANDARD              "IDX_MATRIX_STANDARD"
#define INDEX_ID_NORM_FILTER               "IDX_ID_NORM_FILTER"
#define INDEX_MATRIX_NORM_FILTER           "IDX_MATRIX_NORM_FILTER"
#define INDEX_ID_NORM_METHOD               "IDX_ID_NORM_METHOD"
#define INDEX_FILTER_NORM_METHOD           "IDX_FILTER_NORM_METHOD"
#define INDEX_ID_ALIGN                     "IDX_ID_ALIGN"
#define INDEX_MATRIX_ALIGN                 "IDX_MATRIX_ALIGN"
#define INDEX_SEARCH_SEARCH_MOD            "IDX_SEARCH_SEARCH_MOD"
#define INDEX_HIT_ALT_SUBSTANCE            "IDX_HIT_ALT_SUBSTANCE"
#define INDEX_SUBSTANCE_ALT_SUBSTANCE      "IDX_SUBSTANCE_ALT_SUBSTANCE"
#define INDEX_ID_PEAK_LIST                 "IDX_ID_PEAK_LIST"
#define INDEX_ID_MOD                       "IDX_ID_MOD"
#define INDEX_NAME_MOD                     "IDX_NAME_MOD"
#define INDEX_ID_SEARCH_MOD                "IDX_ID_SEARCH_MOD"
#define INDEX_SEARCH_SEARCH_MOD            "IDX_SEARCH_SEARCH_MOD"
#define INDEX_ID_HIT_MOD                   "IDX_ID_HIT_MOD"
#define INDEX_HIT_HIT_MOD                  "IDX_HIT_HIT_MOD"
#define INDEX_ID_ENZYME                    "IDX_ID_ENZYME"
#define INDEX_NAME_ENZYME                  "IDX_NAME_ENZYME"
#define INDEX_ID_ENZYME_CUTTER             "IDX_ID_ENZYME_CUTTER"
#define INDEX_ENZYME_ENZYME_CUTTER         "IDX_ENZYME_ENZYME_CUTTER"

#define ACTIVATE_PEAK_FUNC_TYPE            "ACTIVATE_MATRIX_PEAK"
#define PEAK_ID_PARAM_NAME                 "peak"
#define ALIGNMENT_FUNC_TYPE                "ALIGNMENT"


// constructor
IdentificationManager::IdentificationManager( void )
{
	// create insufficient title tags warning code
	m_nInsufficientTitleTagsWarningCode = errorcode("Some hit spectra in the search results file do not contain the required spectrum TITLE tags.\nOnly hits with sufficient spectrum TITLE tags have been registered in the results database.");
	m_nNoValidTitleTagsWarningCode = errorcode("No hit spectra in the search results file contain the required spectrum TITLE tags.\nNo hits have been registered in the results database.");
	m_nNoValidHitSpectraWarningCode = errorcode("There are no hit spectra in the search results.");
	m_nSearchResultAdditionWarningCode = errorcode("There is a problem with the Mass++ results database configuration and search results could not be added.\nCheck the database configuration (File -> Database…).");

	// tables
	m_db = NULL;
	createTables();

	// load modifications
	loadModifications();

	// load enzymes
	loadEnzymes();

	// load local database files
	loadLocalDbFiles();

	// Find all the installed search engines.
	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();
	for( unsigned int i = 0; i < plgMgr.getNumberOfFunctionItems(IDENTIFICATION_TYPE); i++ ) {

		// get identification item
		kome::plugin::PluginFunctionItem* fnItem = plgMgr.getFunctionItem( IDENTIFICATION_TYPE, i );

		// Invoke the function - this will automatically create a SearchEngine object and add
		// its pointer to the m_arrSrchEngines array.
		kome::objects::Variant var = fnItem->getCall()->invoke(NULL);
	}
}

// destructor
IdentificationManager::~IdentificationManager( void )
{
	// delete enzymes
	deleteEnzymes();

	// Must delete all Modification objects that have been created.
	deleteModifications();

	// Must delete all SearchEngine objects that have been created.
	SearchEngine* engine = NULL;
	while (!m_arrSrchEngines.empty())
	{
		engine = m_arrSrchEngines.back();
		m_arrSrchEngines.pop_back();
		if( engine != NULL ) {
			delete engine;
			engine = NULL;
		}
	}
}

// clear focused peaks
void IdentificationManager::clearFocusedPeaks() {
	m_focusedPeaks.clear();
}

// add focused peak
void IdentificationManager::addFocusedPeak( const int peakId ) {
	if( peakId >= 0 ) {
		m_focusedPeaks.insert( peakId );
	}
}

// set focused peak
void IdentificationManager::setFocusedPeak( const int peakId ) {
	m_focusedPeaks.clear();
	if( peakId >= 0 ) {
		m_focusedPeaks.insert( peakId );
	}
}

// activate peak
void IdentificationManager::activatePeak( const int peakId ) {
	// plug-in manager
	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();

	// open matrix
	kome::plugin::PluginCallTool::openMatrix();

	// parameter
	kome::objects::Parameters params;
	kome::plugin::PluginCallTool::setInt( params, PEAK_ID_PARAM_NAME, peakId );
	plgMgr.callAllFunctions( &params, ACTIVATE_PEAK_FUNC_TYPE );
}

// remove focused peak
void IdentificationManager::removeFocusedPeak( const int peakId ) {
	m_focusedPeaks.erase( peakId );
}

// get focused peaks
void IdentificationManager::getFocusedPeaks( std::vector< int >& peakIds ) {
	// add to the array
	for( std::set< int >::iterator it = m_focusedPeaks.begin(); it != m_focusedPeaks.end(); it++ ) {
		peakIds.push_back( *it );
	}

	// sort
	std::sort( peakIds.begin(), peakIds.end() );
}

// judge the specified peak is focused or not.
bool IdentificationManager::isFocusedPeak( const int peakId ) {
	return ( m_focusedPeaks.find( peakId ) != m_focusedPeaks.end() );
}

bool IdentificationManager::GetStringValue(FILE* fp, const char* tag, std::string& strValue)
{
	bool bSuccess = false;
	char line[ 2048 ];
	std::string strLine;

	if (fgets(line, 2048, fp) != NULL)
	{
		strLine = line;
		std::string::size_type iColonPos = strLine.find("=");
		if (iColonPos != std::string::npos)
		{
			std::string strTag = strLine.substr(0, iColonPos);
			if (strTag.compare(tag) == 0)
			{
				strValue = strLine.substr(iColonPos + 1, strLine.length() - 1 - iColonPos);
				// Trim the trailing newline character if there is one.
				if ((strValue.size() > 0) && (strValue[strValue.size() - 1] == 10))
				{
					strValue = strValue.substr(0, strValue.size() - 1);
				}
				bSuccess = true;
			}
		}
	}

	return bSuccess;
}

// set progress message according to current file position
void IdentificationManager::ShowFileReadingProgress(FILE* fp, size_t fileSize, const int nSpectraCount, kome::core::Progress& prog)
{
	size_t filePos = ftell(fp);
	int nPercentProgress = static_cast<int>(100.0 * (double)filePos/(double)fileSize);
	prog.setPosition(nPercentProgress);
	prog.setStatus( FMT( "Reading MGF File [%d%%] ... %d Spectra", nPercentProgress, nSpectraCount ).c_str() );
}

// write the specified search result to a pepXML file at the specified path
bool IdentificationManager::writePepXML( const char* path, SearchResult& srchResult, const bool forTpp, kome::core::Progress* progress )
{
	// manager
	MgfManager& mgfMgr = MgfManager::getInstance();

	// file 
	FILE* fp = fileopen( path, "w" );
	if( fp == NULL ) {
		return false;
	}

	// header
	fprintf( fp, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n" );
	fprintf( fp, "<msms_pipeline_analysis date=\"%s\" ", srchResult.getDate().c_str() );
	fprintf(
		fp,
		"xsi:schemaLocation=\"%s\" ",
		"http://regis-web.systemsbiology.net/pepXML http://sashimi.sourceforge.net/schema_revision/pepXML/pepXML_v114.xsd"
	);
	fprintf( fp, "xmlns:xsi=\"%s\" ", "http://www.w3.org/2001/XMLSchema-instance" );
	fprintf( fp, "xmlns=\"%s\">\n", "http://regis-web.systemsbiology.net/pepXML" );
		
	fprintf( fp, "  <msms_run_summary base_name=\"\" raw_data_type=\"\" raw_data=\"\">\n" );

	// enzyme
	kome::ident::Enzyme* enzyme = srchResult.getEnzyme();

	// specify enzyme to "trypsin" only to work TPP plug-in
	// fprintf( fp, "    <sample_enzyme name=\"%s\">\n", ( enzyme == NULL ? "N/A" : enzyme->getName().c_str() ) );
	fprintf( fp, "    <sample_enzyme name=\"trypsin\">\n" );

	fprintf( fp, "    </sample_enzyme>\n" );

	// search
	kome::ident::SearchEngine* engine = srchResult.getSearchEngine();
	fprintf(
		fp,
		"    <search_summary base_name=\"%s\" ",
		srchResult.getBaseName()
	);
	fprintf(
		fp,
		"search_engine=\"%s\" ",
		( engine == NULL ? "N/A" : engine->getName().c_str() )
	);
	fprintf(
		fp,
		"precursor_mass_type=\"%s\" fragment_mass_type=\"%s\" ",
		( srchResult.getPrecursorMassType() == kome::ident::SearchResult::MASS_AVERAGE ? "average" : "monoisotopic" ),
		( srchResult.getFragmentMassType() == kome::ident::SearchResult::MASS_AVERAGE ? "average" : "monoisotopic" )
	);
	fprintf(
		fp,
		"search_id=\"%d\">\n",
		1
	);

    for( unsigned int i = 0; i < srchResult.getSearchParameters().getNumberOfProperties(); i++ ) {
		fprintf(
			fp,
			"      <parameter name=\"%s\" value=\"%s\" />\n",
			srchResult.getSearchParameters().getKey( i ),
			srchResult.getSearchParameters().getValue( i )
		);
	}

	fprintf( fp, "    </search_summary>\n" );

	// collecting spectra
	std::map< SpecInfo*, std::vector< Hit* > > hitMap;
	for( int i = 0; i < srchResult.getNumberOfHits(); i++ ) {
		Hit* hit = srchResult.getHit( i );
		for( int j = 0; j < hit->getNumberOfSpectra(); j++ ) {
			SpecInfo* spec = hit->getSpectrum( j );
			hitMap[ spec ].push_back( hit );
		}
	}

	// progress
	if( progress == NULL ) {
		progress = &( kome::core::Progress::getIgnoringProgress() );
	}
	progress->setRange( 0, srchResult.getSpectrumInfoList().size() );
	progress->setPosition( 0 );

	// each spectra
	int index = 1;
	for( unsigned int i = 0; i < srchResult.getSpectrumInfoList().size() && !progress->isStopped(); i++ ) {
		SpecInfo* spec = srchResult.getSpectrumInfoList().at( i );

		for( unsigned int j = 0; j < spec->charges.size(); j++ ) {
			int charge = spec->charges.at( j );

			// hits
			std::vector< Hit* > hits;
			if( hitMap.find( spec ) != hitMap.end() ) {
				for( unsigned int k = 0; k < hitMap[ spec ].size(); k++ ) {
					Hit* hit = hitMap[ spec ].at( k );
					if( charge == hit->getCharge() ) {
						PeptideSubstance* peptide = dynamic_cast< PeptideSubstance* >( hit->getSubstance() );
						ProteinSubstance* protein = dynamic_cast< ProteinSubstance* >( peptide == NULL ? NULL : peptide->getParentSubstance() );

						if( peptide != NULL && protein != NULL
								&& ( !forTpp || hit->getRank() == 1) ) {   // protein & peptide
							hits.push_back( hit );
						}
					}
				}
			}

			if( hits.size() > 0 ) {
				std::string specTitle;
				if( forTpp ) {
					specTitle = FMT( "spectrum%d.%d.%d.%d", spec->id, spec->specId, spec->specId, charge );
				}
				else {
					specTitle = mgfMgr.getSpectrumTitle( spec, charge );
				}
				fprintf(
					fp,
					"    <spectrum_query spectrum=\"%s\" start_scan=\"%d\" end_scan=\"%d\" precursor_neutral_mass=\"%f\" assumed_charge=\"%d\" index=\"%d\">\n",
					specTitle.c_str(),
					spec->specId,
					spec->specId,
					spec->precursor,
					charge,
					index
				);
				index++;
				
				fprintf( fp, "      <search_result>\n" );
					
				for( unsigned int k = 0; k < hits.size(); k++ ) {
					Hit* hit = hits[ k ];
					PeptideSubstance* peptide = (PeptideSubstance*)hit->getSubstance();
					ProteinSubstance* protein = (ProteinSubstance*)peptide->getParentSubstance();
					std::string sequence = peptide->getSequence();
					std::string prevAa = peptide->getResidueBefore();
					if( !prevAa.empty() ) {
						prevAa = prevAa.substr( prevAa.length() - 1 );
					}
					std::string nextAa = peptide->getResidueAfter();
					if( !nextAa.empty() ) {
						nextAa = nextAa.substr( 0, 1 );
					}

					fprintf(
						fp,
						"        <search_hit hit_rank=\"%d\" ",
						hit->getRank()
					);
					fprintf(
						fp,
						"peptide=\"%s\" peptide_prev_aa=\"%s\" peptide_next_aa=\"%s\" ",
						sequence.c_str(),
						prevAa.c_str(),
						nextAa.c_str()
					);
					fprintf(
						fp,
						"protein=\"%s\" num_tot_proteins=\"%d\" ",
						protein->getAccession().c_str(),
						hit->getProteinCount()
					);
					fprintf(
						fp,
						"calc_neutral_pep_mass=\"%f\" protein_descr=\"%s\" ",
						hit->getCalculatedMR(),
						protein->getName().c_str()
					);
					fprintf(
						fp,
						( hit->getDelta() >= 0.0 ? "massdiff=\"+%f\" " : "massdiff=\"%f\" " ),
						hit->getDelta()
					);
					fprintf(
						fp,
						"num_tol_term=\"2\" num_missed_cleavages=\"%d\"",
						hit->getMissedCleavagesCount()
					);

					if( hit->getMatchedIonCount() >= 0 ) {
						fprintf(
							fp,
							" num_matched_ions=\"%d\"",
							hit->getMatchedIonCount()
						);
					}
					fprintf(
						fp,
						" hit_id=\"%d\"",
						hit->getID()
					);

					fprintf( fp, ">\n" );
					
					for( int l = 0; l < hit->getNumberOfAltSubstances(); l++ ) {
						kome::ident::ProteinSubstance* altProt = dynamic_cast< kome::ident::ProteinSubstance* >( hit->getAltSubstance( l ) );
						if( altProt != NULL ) {
							fprintf(
								fp,
								"          <alternative_protein protein=\"%s\" protein_descr=\"%s\" protein_mw=\"%f\" />\n",
								altProt->getAccession().c_str(),
								altProt->getName().c_str(),
								altProt->getMass()
							);
						}
					}
					
					for( int l = 0; l < hit->getNumberOfValues(); l++ ) {
						if( hit->getValueType( l ) == kome::ident::TYPE_SCORE || hit->getValueType( l ) == kome::ident::TYPE_EXPECTATION_VALUE ) {
							fprintf(
								fp,
								"          <search_score name=\"%s\" value=\"%s\" />\n",
								hit->getValueName( l ).c_str(),
								hit->getValue( l ).c_str()
							);
						}
					}

					fprintf( fp, "          <search_score name=\"%s\" value=\"%s\" />\n", "star", "0" );					
					fprintf( fp, "        </search_hit>\n" );
				}
					
				fprintf( fp, "      </search_result>\n" );
				fprintf( fp, "    </spectrum_query>\n" );
			}
		}

		progress->setPosition( i + 1 );
	}

	if( !progress->isStopped() ) {
		fprintf( fp, "  </msms_run_summary>\n" );
		fprintf( fp, "</msms_pipeline_analysis>\n" );

		progress->fill();
	}

    // close
	fflush( fp );
	fclose( fp );

	return true;
}

// write all information for substance of the specified type
void IdentificationManager::writeAllSubstances( FILE* fp, const SearchResult& srchResult, SubstanceType subType )
{
	int nSubstanceCount = 0;
	for (int iSubstanceIndex = 0; iSubstanceIndex < srchResult.getNumberOfSubstances(); iSubstanceIndex++)
	{
		int nHitCount = 0;
		if (srchResult.getSubstance(iSubstanceIndex)->getType() == subType)
		{
			for (int iHitIndex = 0; iHitIndex < srchResult.getNumberOfHits(); iHitIndex++)
			{
				if (srchResult.getHit(iHitIndex)->getSubstance() == srchResult.getSubstance(iSubstanceIndex))
				{
					// For the first substance hit, first of all, write the header row.
					if ((nSubstanceCount == 0) && (nHitCount == 0))
					{
						std::string sHeaderRow;

						switch (subType)
						{
							case TYPE_PEPTIDE:
								sHeaderRow = "Peptide";
								break;

							case TYPE_METABOLITE:
								sHeaderRow = "Metabolite";
								break;

							case TYPE_UNKNOWN:
							default:
								sHeaderRow = "Unknown";
								break;
						}

						sHeaderRow += "\tProtein\tAccession\tName";

						for (int iSubstancePropIndex = 0; iSubstancePropIndex < static_cast<int>(srchResult.getSubstance(iSubstanceIndex)->getProperties().getNumberOfProperties()); iSubstancePropIndex++)
						{
							sHeaderRow += "\t";
							sHeaderRow += srchResult.getSubstance(iSubstanceIndex)->getProperties().getKey(iSubstancePropIndex);
						}
						for (int iHitValueIndex = 0; iHitValueIndex < static_cast<int>(srchResult.getHit(iHitIndex)->getNumberOfValues()); iHitValueIndex++)
						{
							sHeaderRow += "\t";
							sHeaderRow += srchResult.getHit(iHitIndex)->getValueName(iHitValueIndex);
						}

						sHeaderRow += "\n";
						fprintf(fp, sHeaderRow.c_str());
					}

					std::string sValueRow = FMT("%d\t%s\t%s\t%s", nSubstanceCount + 1,
						(srchResult.getSubstance(iSubstanceIndex)->getParentSubstance() != NULL) ? srchResult.getSubstance(iSubstanceIndex)->getParentSubstance()->getAccession().c_str() : "",
						srchResult.getSubstance(iSubstanceIndex)->getAccession().c_str(), srchResult.getSubstance(iSubstanceIndex)->getName().c_str());

					for (int iSubstancePropIndex = 0; iSubstancePropIndex < static_cast<int>(srchResult.getSubstance(iSubstanceIndex)->getProperties().getNumberOfProperties()); iSubstancePropIndex++)
					{
						sValueRow += "\t";
						sValueRow += srchResult.getSubstance(iSubstanceIndex)->getProperties().getValue(iSubstancePropIndex);
					}
					for (int iHitValueIndex = 0; iHitValueIndex < srchResult.getHit(iHitIndex)->getNumberOfValues(); iHitValueIndex++)
					{
						sValueRow += "\t";
						sValueRow += srchResult.getHit(iHitIndex)->getValue(iHitValueIndex);
					}
					sValueRow += "\n";
					fprintf(fp, sValueRow.c_str());

					nHitCount++;
				}
			}
			
			nSubstanceCount++;
		}
	}
	if (nSubstanceCount > 0)
	{
		fprintf(fp, "\n");
	}
}

// write the specified search result to a TSV (tab-separated value) text file at the specified path
bool IdentificationManager::writeTsv( const char* path, const SearchResult& srchResult, kome::core::Progress* progress )
{
	// open the file
	FILE* fp = fileopen( path, "w" );
	if( fp == NULL ) {
		LOG_ERROR_CODE( FMT( "Failed to create the file. Check the file / folder permissions. [%s]", NVL( path, "" ) ), ERR_FILE_OPEN_FAILED );
		return false;
	}

	fprintf(fp, "Date: %s\n", srchResult.getDate().c_str());
	fprintf(fp, "Database: %s\n", srchResult.getDb().c_str());
	fprintf(fp, "\n");

	// progress
	if( progress == NULL ) {
		progress = &( kome::core::Progress::getIgnoringProgress() );
	}
	progress->setRange( 0, srchResult.getNumberOfSubstances() );
	progress->setPosition( 0 );

	// export
	int nProteinCount = 0;
	for (int iSubstanceIndex = 0; iSubstanceIndex < srchResult.getNumberOfSubstances() && !progress->isStopped(); iSubstanceIndex++)
	{
		if (srchResult.getSubstance(iSubstanceIndex)->getType() == TYPE_PROTEIN)
		{
			// For the first protein, first of all, write the header row.
			if (nProteinCount == 0)
			{
				std::string sHeaderRow = "Protein\tAccession\tName";
				for (int iSubstancePropIndex = 0; iSubstancePropIndex < static_cast<int>(srchResult.getSubstance(iSubstanceIndex)->getProperties().getNumberOfProperties()); iSubstancePropIndex++)
				{
					sHeaderRow += "\t";
					sHeaderRow += srchResult.getSubstance(iSubstanceIndex)->getProperties().getKey(iSubstancePropIndex);
				}
				sHeaderRow += "\n";
				fprintf(fp, sHeaderRow.c_str());
			}

			std::string sValueRow = FMT("%d\t%s\t%s", nProteinCount + 1, srchResult.getSubstance(iSubstanceIndex)->getAccession().c_str(), srchResult.getSubstance(iSubstanceIndex)->getName().c_str());
			for (int iSubstancePropIndex = 0; iSubstancePropIndex < static_cast<int>(srchResult.getSubstance(iSubstanceIndex)->getProperties().getNumberOfProperties()); iSubstancePropIndex++)
			{
				sValueRow += "\t";
				sValueRow += srchResult.getSubstance(iSubstanceIndex)->getProperties().getValue(iSubstancePropIndex);
			}
			sValueRow += "\n";
			fprintf(fp, sValueRow.c_str());
			
			nProteinCount++;
		}
		progress->setPosition( iSubstanceIndex + 1 );
	}
	if (nProteinCount > 0)
	{
		fprintf(fp, "\n");
	}

	writeAllSubstances(fp, srchResult, TYPE_PEPTIDE);
	writeAllSubstances(fp, srchResult, TYPE_METABOLITE);
	writeAllSubstances(fp, srchResult, TYPE_UNKNOWN);

	fflush( fp );
	fclose( fp );

	if( !progress->isStopped() ) {
		progress->fill();
	}

	return true;
}

// get the number of installed local databases
int IdentificationManager::getNumberOfLocalDbs( void ) const
{
	return (int)m_localDbs.size();
}

// get the specified local database name
std::string IdentificationManager::getLocalDb( const int index ) const
{
	// string
	std::string s;

	// check the index
	_ASSERT( (index >= 0) && (index < static_cast<int>(m_localDbs.size())) );

	// database name
	if( index >= 0 && index < (int)m_localDbs.size() ) {
		s = m_localDbs[ index ].name;
	}

	return s;
}

// get the number of files in the specified local database
int IdentificationManager::getNumberOfLocalDbFiles( const int index ) const
{
	// count
	int num = 0;

	if( index >= 0 && index < (int)m_localDbs.size() ) {
		num = (int)( m_localDbs[ index ].localDbs.size() );
	}

	return num;
}

// get the filename of the specified file in the specified local database
std::string IdentificationManager::getLocalDbFileName( const int dbIndex, const int fileIndex ) const
{
	// name
	std::string s;

	// check the index
	_ASSERT( (dbIndex >= 0) && (dbIndex < static_cast<int>(m_localDbs.size())) );

	if( dbIndex >= 0 && dbIndex < (int)m_localDbs.size() ) {
		if( fileIndex >= 0 && fileIndex < (int)m_localDbs[ dbIndex ].localDbs.size() ) {
			s = ( m_localDbs[ dbIndex ].localDbs )[ fileIndex ].name;
		}
	}

	return s;
}

// get the type of the specified file in the specified local database
int IdentificationManager::getLocalDbFileType( const int dbIndex, const int fileIndex ) const
{
	// type
	int type = 0;
	LocalDbFileInfo* info = NULL;

	// check the index
	_ASSERT( (dbIndex >= 0) && (dbIndex < static_cast<int>(m_localDbs.size())) );

	if( dbIndex >= 0 && dbIndex < (int)m_localDbs.size() ) {
		// check the index
		_ASSERT( (fileIndex >= 0) && (fileIndex < static_cast<int>(m_localDbs[ dbIndex ].localDbs.size())) );

		if( fileIndex >= 0 && fileIndex < (int)m_localDbs[ dbIndex ].localDbs.size() ) {
			info = const_cast< LocalDbFileInfo* >( &( m_localDbs[ dbIndex ].localDbs[ fileIndex ] ) );
		}
	}

	// type
	if( info != NULL ) {
		for( unsigned int i = 0; i < info->localFiles.size(); i++ ) {
			type |= info->localFiles[ i ].first;
		}
	}

	return type;
}

// get the file path of the specified file and type in the specified local database
std::string IdentificationManager::getLocalDbFilePath( const int dbIndex, const int fileIndex, DbFileType fileType ) const
{
	// file path
	std::string p;

	LocalDbFileInfo* info = NULL;

	// check the index
	_ASSERT( (dbIndex >= 0) && (dbIndex < static_cast<int>(m_localDbs.size())) );

	if( dbIndex >= 0 && dbIndex < (int)m_localDbs.size() ) {
		// check the index
		_ASSERT( (fileIndex >= 0) && (fileIndex < static_cast<int>(m_localDbs[ dbIndex ].localDbs.size())) );

		if( fileIndex >= 0 && fileIndex < (int)m_localDbs[ dbIndex ].localDbs.size() ) {
			info = const_cast< LocalDbFileInfo* >( &( m_localDbs[ dbIndex ].localDbs[ fileIndex ] ) );
		}
	}

	if( info != NULL ) {
		for( unsigned int i = 0; i < info->localFiles.size(); i++ ) {
			if( fileType == info->localFiles[ i ].first ) {
				p = info->localFiles[ i ].second;
			}
		}
	}

	return p;
}

// get the local db regex
std::string IdentificationManager::getAccessionRegx( const int index ) const
{
	// string
	std::string s;

	// check the index
	_ASSERT( (index >= 0) && (index < static_cast<int>(m_localDbs.size())) );

	// database name
	if( index >= 0 && index < (int)m_localDbs.size() ) {
		s = m_localDbs[ index ].regx;
	}

	return s;
}

// get residues file
std::string IdentificationManager::getResiduesFile( const int index ) const 
{
	// string
	std::string s;

	// check the index
	_ASSERT( (index >= 0) && (index < static_cast<int>(m_localDbs.size())) );

	// database name
	if( index >= 0 && index < (int)m_localDbs.size() ) {
		s = m_localDbs[ index ].residues;
	}

	return s;	
}

// get the local db description
std::string IdentificationManager::getLocalDbDesc( const int index ) const
{
	// string
	std::string s;

	// check the index
	_ASSERT( ( index >= 0 ) && ( index < static_cast<int>( m_localDbs.size() ) ) );

	// descritpion
	if( index >= 0 && index < (int)m_localDbs.size() ) {
		s = m_localDbs[ index ].description;
	}

	return s;
}



// add a public (protein, metabolite) database
int IdentificationManager::addPublicDb( const char* dbName, const char* url )
{
	// Note: New internal database is a prerequisite.
	int dbId = -1;
	_ASSERT((strlen(dbName) > 0) && (strlen(url) > 0));

	if ((strlen(dbName) > 0) && (strlen(url) > 0))
	{
		dbId = addDatabase( dbName, url );
	}

	return dbId;
}

// set the URL for a public (protein, metabolite) database
void IdentificationManager::setPublicUrl( const int dbId, const char* format, const char* url )
{
	// Note: New internal database is a prerequisite.
	_ASSERT(strlen(format) > 0);

	if (strlen(format) > 0)
	{
		setFormatUrl( dbId, format, url );
	}
}

// register a search result in the internal database
unsigned int IdentificationManager::registerResult( SearchResult& srchResult, kome::core::Progress& prog )
{
	// warning code
	unsigned int ret = 0;

	// db
	int dbId = getDatabaseId( srchResult.getDb().c_str() );
	if( dbId < 0 ) {
		dbId = addDatabase( srchResult.getDb().c_str(), "" );
	}

	// search engine
	int engineId = -1;
	SearchEngine* engine = srchResult.getSearchEngine();
	if( engine != NULL ) {
		engineId = addSearchEngine( engine->getName().c_str(), engine->getVersion().c_str() );
	}

	// search
	SearchInfo searchInfo;
	
	searchInfo.name = srchResult.getTitle();
	searchInfo.comment = srchResult.getComment();
	searchInfo.date = srchResult.getDate();
	searchInfo.engineId = engineId;
	searchInfo.db = srchResult.getDb();
	searchInfo.result = srchResult.getResultLocation();

	kome::core::Properties& props = srchResult.getSearchParameters();
	for( unsigned int i = 0; i < props.getNumberOfProperties(); i++ ) {
		searchInfo.searchProps.setValue( props.getKey( i ), props.getValue( i ) );
	}

	searchInfo.precMassType = (int)srchResult.getPrecursorMassType();
	searchInfo.fragMassType = (int)srchResult.getFragmentMassType();

	kome::ident::Enzyme* enzyme = srchResult.getEnzyme();
	searchInfo.enzyme = enzyme;
	searchInfo.baseName = srchResult.getBaseName();

	for( unsigned int i = 0; i < srchResult.getNumberOfFixedModifications(); i++ ) {
		Modification* modification = srchResult.getFixedModification( i );
		searchInfo.fixedMods.push_back( modification );
	}

	for( unsigned int i = 0; i < srchResult.getNumberOfVariableModifications(); i++ ) {
		Modification* modification = srchResult.getVariableModification( i );
		searchInfo.varMods.push_back( modification );
	}

	// add search
	int searchId = addSearch( searchInfo );
	if( searchId < 0 ) {
		return SearchResultAdditionWarningCode();
	}

	// Create 2 subprogresses
	prog.createSubProgresses(2);
	kome::core::Progress* subProgress = prog.getSubProgress( 0 );

	// This check is required to prevent a /0 error when there are no spectra.
	if (srchResult.getSpectrumInfoList().size() > 0)
	{
		subProgress->setRange( 0, srchResult.getSpectrumInfoList().size() );
		subProgress->setPosition( 0 );
	}

	// spectra
	int validCnt = 0;
	int invalidCnt = 0;
	std::vector< std::pair< SpecInfo*, int > > searchSpectra;
	std::map< int, std::vector< int > > idIndexMap;

	for( unsigned int i = 0; (i < srchResult.getSpectrumInfoList().size()) && !prog.isStopped(); i++ ) {
		// spectrum
		SpecInfo* spec = ( srchResult.getSpectrumInfoList() )[ i ];

		// peak list
		subProgress->setStatus( FMT( "Checking Spectrum Information before Results Registration [%d/%d] ... %s", ( i + 1 ), srchResult.getSpectrumInfoList().size(), spec->name.c_str() ).c_str() );
		subProgress->setPosition( i + 1 );

		if( spec->path.empty() || spec->specId < 0 || spec->sampleIndex < 0 ) {
			invalidCnt++;
		}
		else {
			validCnt++;
			int specId = addSpectrum( *spec );
			int searchSpecId = setSearchSpectrum( specId, searchId, spec->peaks, spec->precursor, spec->charges );
			for( unsigned int j = 0; j < spec->peakIds.size(); j++ ) {
				setMatrixPeakSpectrum( spec->peakIds.at( j ), searchSpecId );
			}

			int idx = (int)searchSpectra.size();
			searchSpectra.push_back( std::make_pair( spec, searchSpecId ) );
			idIndexMap[ spec->specId ].push_back( idx );
		}
	}

	// The operation was cancelled.
	if (prog.isStopped())
	{
		return ret;
	}

	subProgress->fill();
	subProgress = prog.getSubProgress( 1 );

	return registerResult( searchId, srchResult, *subProgress );
}

// register result
unsigned int IdentificationManager::registerResult( const int searchId, SearchResult& srchResult, kome::core::Progress& prog ) {
	// warning code
	unsigned int ret = 0;

	// This check is required to prevent a /0 error when there are no hits.
	if (srchResult.getNumberOfHits() > 0)
	{
		prog.setRange( 0, srchResult.getNumberOfHits() );
		prog.setPosition( 0 );
	}

	// search information
	SearchInfo searchInfo;
	getSearch( searchId, searchInfo );

	int dbId = getDatabaseId( searchInfo.db.c_str() );
	if( dbId < 0 ) {
		dbId = addDatabase( searchInfo.db.c_str(), "" );
	}

	// set result location
	setResultLocation( searchId, srchResult.getResultLocation().c_str() );

	// hits
	int validHitCnt = 0;
	int invalidHitCnt = 0;
	std::map< Substance*, int > substanceMap;
	for( int i = 0; (i < srchResult.getNumberOfHits()) && !prog.isStopped(); i++ ) {
		// hit
		Hit* hit = srchResult.getHit( i );
		Substance* substance = hit->getSubstance();
		std::vector< Substance* > substances;
		substances.push_back( substance );
		for( int j = 0; j < hit->getNumberOfAltSubstances(); j++ ) {
			substances.push_back( hit->getAltSubstance( j ) );
		}

		prog.setStatus( FMT( "Checking / Registering Search Hits [%d/%d] ... %s", ( i + 1 ), srchResult.getNumberOfHits(), hit->getSubstance()->getName().c_str() ).c_str() );
		prog.setPosition( i + 1 );

		// spectra
		std::vector< int > spectra;
		for( int j = 0; j < hit->getNumberOfSpectra(); j++ ) {
			SpecInfo* spec = hit->getSpectrum( j );
			int specId = addSpectrum( *spec );
			int searchSpecId = setSearchSpectrum( specId, searchId, spec->peaks, spec->precursor, spec->charges );

			if( searchSpecId < 0 ) {
				invalidHitCnt++;
			}
			else {
				spectra.push_back( searchSpecId );
				validHitCnt++;
			}
		}

		// substance
		if( spectra.size() > 0 ) {
			for( unsigned int j = 0; j < substances.size(); j++ ) {
				Substance* tmp = substances[ j ];
				std::list< Substance* > substanceList;
				while( tmp != NULL ) {
					substanceList.push_front( tmp );
					tmp = tmp->getParentSubstance();
				}

				int substanceId = -1;
				for( std::list< Substance* >::iterator it = substanceList.begin(); it != substanceList.end(); it++ ) {
					Substance* tmp = *it;

					if( substanceMap.find( tmp ) == substanceMap.end() ) {
						SubstanceInfo subInfo;
						subInfo.accession = tmp->getAccession();
						subInfo.name = tmp->getName();
						subInfo.dbId = dbId;
						subInfo.type = tmp->getType();
						subInfo.parentId = substanceId;
						
						kome::core::Properties& props = tmp->getProperties();
						for( unsigned int j = 0; j < props.getNumberOfProperties(); j++ ) {
							subInfo.props.setValue( props.getKey( j ), props.getValue( j ) );
						}
						
						substanceId = addSubstance( subInfo );
						substanceMap[ tmp ] = substanceId;
					}
					else {
						substanceId = substanceMap[ tmp ];
					}
				}
			}
		}

		// add Hit
		int hitId = -1;
		if( spectra.size() > 0 ) {
			HitInfo hitInfo;
			hitInfo.substance = substanceMap[ substance ];
			hitInfo.charge = hit->getCharge();
			for( int j = 0; j < hit->getNumberOfAltSubstances(); j++ ) {
				AltSubstance as;
				as.id = substanceMap[ hit->getAltSubstance( j ) ];
				as.start = hit->getAltSubstanceStartPos( j );
				as.end = hit->getAltSubstanceEndPos( j );
				as.before = hit->getAltSubstanceBefore( j );
				as.after = hit->getAltSubstanceAfter( j );

				hitInfo.altSubstances.push_back( as );
			}

			for( unsigned int j = 0; j < spectra.size(); j++ ) {
				hitInfo.spectra.push_back( spectra[ j ] );
			}

			for( int j = 0; j < hit->getNumberOfValues(); j++ ) {
				hitInfo.props.resize( hitInfo.props.size() + 1 );
				hitInfo.props.back().name = hit->getValueName( j );
				hitInfo.props.back().value = hit->getValue( j );
				hitInfo.props.back().type = hit->getValueType( j );
			}

			hitId = addHit( hitInfo );
		}
	}

	// The operation was cancelled.
	if (prog.isStopped())
	{
		return ret;
	}

	if( invalidHitCnt > 0 ) {
		if ( validHitCnt > 0 )
		{
			ret = InsufficientTitleTagsWarningCode();
		}
		else
		{
			ret = NoValidTitleTagsWarningCode();
		}
	}
	else if( validHitCnt == 0 ) {
		ret = NoValidHitSpectraWarningCode();
	}

	prog.fill();

	return ret;
}


// load enzymes
void IdentificationManager::loadEnzymes() {
	// delete
	deleteEnzymes();

	// database
	kome::db::Database* db = getDb();

	if( db != NULL ) {
		// enzymes
		std::map< int, Enzyme* > enzymeMap;
		
		// get from DB
		std::string sql = FMT(
			"select %s, %s, %s from %s;",
			COL_ID_ENZYME,
			COL_NAME_ENZYME,
			COL_HIDDEN_ENZYME,
			TABLE_ENZYME
		);
		
		kome::db::ResultSet* rs = db->executeQuery( sql.c_str() );
		if( rs != NULL ) {
			while( rs->next() ) {
				int id = rs->getInt( 0, -1 );
				std::string name = rs->getString( 1, "" );
				int hidden = rs->getInt( 2, 1 );
				
				if( !name.empty() && id >= 0 ) {
					Enzyme* enz = new Enzyme( name.c_str() );
					enz->setId( id );
					enz->setHidden( hidden != 0 );
					
					enzymeMap[ id ] = enz;
					m_arrEnzymes.push_back( enz );
				}
			}
			rs->close();
		}
		
		sql = FMT(
			"select %s, %s, %s, %s from %s;",
			COL_ENZYME_ENZYME_CUTTER,
			COL_TERM_ENZYME_CUTTER,
			COL_CLEAVE_ENZYME_CUTTER,
			COL_RESTRICT_ENZYME_CUTTER,
			TABLE_ENZYME_CUTTER
		);
		
		rs = db->executeQuery( sql.c_str() );
		if( rs != NULL ) {
			while( rs->next() ) {
				int enzyme = rs->getInt( 0, -1 );
				std::string term = rs->getString( 1, "" );
				std::string cleave = rs->getString( 2, "" );
				std::string restrict = rs->getString( 3, "" );
				
				Enzyme::TermType t = Enzyme::UNKNOWN;
				if( term.compare( "C" ) == 0 ) {
					t = Enzyme::C_TERM;
				}
				else if( term.compare( "N" ) == 0 ) {
					t = Enzyme::N_TERM;
				}
				
				if( enzymeMap.find( enzyme ) != enzymeMap.end() ) {
					Enzyme* enz = enzymeMap[ enzyme ];
					enz->addCutter( t, cleave.c_str(), restrict.c_str() );
				}
			}
			rs->close();
		}
	}

	// xml file
	if( m_arrEnzymes.empty() ) {
		kome::core::MsppManager& msppMgr = kome::core::MsppManager::getInstance();
		std::string xmlFile = getpath( msppMgr.getConfDir(), "enzyme.xml" );
		std::vector< Enzyme* > enzymes;

		if( !fileexists( xmlFile.c_str() ) ) {
			std::string confDir = getpath( msppMgr.getMsppDir(), "conf" );
			std::string orgFile = getpath( confDir.c_str(), "enzyme.xml" );
			copyfile( orgFile.c_str(), xmlFile.c_str() );
		}

		if( fileexists( xmlFile.c_str() ) ) {
			EnzymeHandler handler( enzymes );
			handler.parse( xmlFile.c_str() );
		}

		for( unsigned int i = 0; i < enzymes.size(); i++ ) {
			updateEnzyme( enzymes[ i ] );
			delete enzymes[ i ];
		}
		enzymes.clear();
	}
}

// get the number of enzymes
int IdentificationManager::getNumberOfEnzymes( void ) const
{
	return m_arrEnzymes.size();
}

// get the specified enzyme information
Enzyme* IdentificationManager::getEnzyme( const int index ) const
{
	Enzyme* ret = NULL;
	// check the index
	if( index < static_cast<int>(m_arrEnzymes.size()) ) {
		ret = m_arrEnzymes[ index ];
	}
	return ret;
}

// get enzyme form ID
Enzyme* IdentificationManager::getEnzymeFromId( const int id ) {
	Enzyme* ret = NULL;
	for( unsigned int i = 0; i < m_arrEnzymes.size() && ret == NULL; i++ ) {
		if( m_arrEnzymes[ i ]->getId() == id ) {
			ret = m_arrEnzymes[ i ];
		}
	}

	return ret;
}

// set all enzyme hidden
void IdentificationManager::setAllEnzymeHidden() {
	// set objects
	for( unsigned int i = 0; i < m_arrEnzymes.size(); i++ ) {
		m_arrEnzymes[ i ]->setHidden( true );
	}

	// set DB
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return;
	}

	std::string sql = FMT( "update %s set %s = ?;", TABLE_ENZYME, COL_HIDDEN_ENZYME );

	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindInteger( 1, 1 );
		stmt->execute();
		stmt->close();
	}
}

// update enzyme
int IdentificationManager::updateEnzyme( Enzyme* enzyme ) {
	// parameter check
	if( enzyme == NULL ) {
		return -1;
	}

	std::string name = enzyme->getName();
	if( name.empty() ) {
		return -1;
	}

	// database
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return -1;
	}

	// ID
	int id = searchEnzyme( name.c_str() );

	// update enzyme
	std::vector< int > cutterIds;
	if( id >= 0 ) {    // update
		Enzyme* enz = getEnzymeFromId( id );
		std::string sql = FMT(
			"select %s from %s where %s = ?;",
			COL_ID_ENZYME_CUTTER,
			TABLE_ENZYME_CUTTER,
			COL_ENZYME_ENZYME_CUTTER
		);

		kome::db::Statement* stmt = db->prepare( sql.c_str() );
		if( stmt != NULL ) {
			stmt->bindInteger( 1, id );
			kome::db::ResultSet* rs = stmt->executeQuery();
			if( rs != NULL ) {
				while( rs->next() ) {
					int cId = rs->getInt( 0, -1 );
					if( cId >= 0 ) {
						cutterIds.push_back( cId );
					}
				}
				rs->close();
			}
			stmt->close();
		}

		sql = FMT(
			"update %s set %s = ? where %s = ?;",
			TABLE_ENZYME,
			COL_HIDDEN_ENZYME,
			COL_ID_ENZYME
		);

		stmt = db->prepare( sql.c_str() );
		if( stmt != NULL ) {
			stmt->bindInteger( 1, ( enzyme->isHidden() ? 1 : 0 ) );
			stmt->bindInteger( 2, id );
			stmt->execute();
			stmt->close();
		}

		enz->setHidden( enzyme->isHidden() );
	}
	else {    // insert
		std::string sql = FMT(
			"insert into %s (%s, %s) values (?, ?);",
			TABLE_ENZYME,
			COL_NAME_ENZYME,
			COL_HIDDEN_ENZYME
		);

		kome::db::Statement* stmt = db->prepare( sql.c_str() );
		if( stmt != NULL ) {
			stmt->bindString( 1, name.c_str() );
			stmt->bindInteger( 2, ( enzyme->isHidden() ? 1 : 0 ) );
			stmt->execute();
			stmt->close();
		}

		id = db->getMaxValue( TABLE_ENZYME, COL_ID_ENZYME );

		Enzyme* enz = new Enzyme( name.c_str() );
		enz->setHidden( enzyme->isHidden() );
		enz->setId( id );
		m_arrEnzymes.push_back( enz );
	}

	// enzyme
	Enzyme* enz = getEnzymeFromId( id );
	if( enz != NULL ) {
		enz->clearCutters();
	}

	// cutter
	std::string sql = FMT(
		"update %s set %s = ?, %s = ?, %s = ?, %s = ? where %s = ?;",
		TABLE_ENZYME_CUTTER,
		COL_ENZYME_ENZYME_CUTTER,
		COL_TERM_ENZYME_CUTTER,
		COL_CLEAVE_ENZYME_CUTTER,
		COL_RESTRICT_ENZYME_CUTTER,
		COL_ID_ENZYME_CUTTER
	);
	kome::db::Statement* stmt0 = db->prepare( sql.c_str() );

	sql = FMT(
		"insert into %s (%s, %s, %s, %s) values (?, ?, ?, ?);",
		TABLE_ENZYME_CUTTER,
		COL_ENZYME_ENZYME_CUTTER,
		COL_TERM_ENZYME_CUTTER,
		COL_CLEAVE_ENZYME_CUTTER,
		COL_RESTRICT_ENZYME_CUTTER
	);
	kome::db::Statement* stmt1 = db->prepare( sql.c_str() );

	for( unsigned int i = 0; i < enzyme->getNumberOfCutters(); i++ ) {
		Enzyme::Cutter* cutter = enzyme->getCutter( i );
		std::string term = "-";
		if( cutter->term == Enzyme::N_TERM ) {
			term = "N";
		}
		else if( cutter->term == Enzyme::C_TERM ) {
			term = "C";
		}

		int cId = -1;
		if( i < cutterIds.size() ) {
			cId = cutterIds[ i ];
		}

		kome::db::Statement* stmt = ( cId < 0 ? stmt1 : stmt0 );
		if( stmt != NULL ) {
			stmt->resetBind();
			stmt->bindInteger( 1, id );
			stmt->bindString( 2, term.c_str() );
			stmt->bindString( 3, cutter->cleave.c_str() );
			stmt->bindString( 4, cutter->restrict.c_str() );

			if( cId >= 0 ) {
				stmt->bindInteger( 5, cId );
			}

			stmt->execute();
		}

		enz->addCutter( cutter->term, cutter->cleave.c_str(), cutter->restrict.c_str() );
	}

	if( stmt0 != NULL ) {
		stmt0->close();
	}

	if( stmt1 != NULL ) {
		stmt1->close();
	}

	sql = FMT( "delete from %s where %s = ?;", TABLE_ENZYME_CUTTER, COL_ID_ENZYME_CUTTER );
	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		for( unsigned int i = enzyme->getNumberOfCutters(); i < cutterIds.size(); i++ ) {
			int cId = cutterIds[ i ];
			stmt->resetBind();
			stmt->bindInteger( 1, cId );
			stmt->execute();
		}
		stmt->close();
	}

	return id;
}

// search enzyme
int IdentificationManager::searchEnzyme( const char* name ) {
	// return value
	int id = -1;

	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return id;
	}

	// search
	std::string sql = FMT(
		"select %s from %s where %s = ?;",
		COL_ID_ENZYME,
		TABLE_ENZYME,
		COL_NAME_ENZYME
	);

	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindString( 1, name );
		kome::db::ResultSet* rs = stmt->executeQuery();
		if( rs != NULL ) {
			if( rs->next() ) {
				id = rs->getInt( 0, -1 );
			}
			rs->close();
		}
		stmt->close();
	}

	return id;
}

// delete enzymes
void IdentificationManager::deleteEnzymes() {
	for( unsigned int i = 0; i < m_arrEnzymes.size(); i++ ) {
		delete m_arrEnzymes[ i ];
	}

	m_arrEnzymes.clear();
}


// load modification
void IdentificationManager::loadModifications() {
	// delete modifications
	deleteModifications();

	// DB
	kome::db::Database* db = getDb();

	if( db != NULL ) {
		// get modifications from db
		std::string sql = FMT(
			"select %s, %s, %s, %s, %s from %s;",
			COL_ID_MOD,
			COL_NAME_MOD,
			COL_TYPE_MOD,
			COL_MASS_MOD,
			COL_HIDDEN_MOD,
			TABLE_MOD
		);

		kome::db::ResultSet* rs = db->executeQuery( sql.c_str() );
		if( rs != NULL ) {
			while( rs->next() ) {
				int id = rs->getInt( 0, -1 );
				std::string name = rs->getString( 1, "" );
				std::string type = rs->getString( 2, "" );
				double mass = rs->getReal( 3, -1.0 );
				int hidden = rs->getInt( 4, 1 );
				
				if( id >= 0 && !name.empty() ) {
					Modification* m = new Modification( name.c_str(), mass, type.c_str(), ( hidden != 0 ) );
					m->setId( id );
					m_arrModifications.push_back( m );
				}
			}
			rs->close();
		}
	}

	// XML file
	if( m_arrModifications.empty() ) {
		kome::core::MsppManager& msppMgr = kome::core::MsppManager::getInstance();
		std::vector< Modification* > modifications;
		std::string xmlFile = getpath( msppMgr.getConfDir(), "unimod.xml" );
		if( !fileexists( xmlFile.c_str() ) ) {
			std::string confDir = getpath( msppMgr.getMsppDir(), "conf" );
			std::string orgFile = getpath( confDir.c_str(), "unimod.xml" );
			copyfile( orgFile.c_str(), xmlFile.c_str() );
		}
	
		if( fileexists( xmlFile.c_str() ) ) {
			ModificationHandler handler( modifications );
			handler.parse( xmlFile.c_str() );
		}

		for( unsigned int i = 0; i < modifications.size(); i++ ) {
			updateModification( modifications[ i ] );
			delete modifications[ i ];
		}

		modifications.clear();
	}
}


// get the number of modifications
int IdentificationManager::getNumberOfModifications( void ) const
{
	return m_arrModifications.size();
}

// get a pointer to the specified Modification object
Modification* IdentificationManager::getModification( const int index ) const
{
	Modification* ret = NULL;
	// check the index
	_ASSERT( (index >= 0) && (index < static_cast<int>(m_arrModifications.size())) );

	if( index < static_cast<int>(m_arrModifications.size()) ) {
		ret = m_arrModifications[ index ];
	}
	return ret;
}

// get modification from ID
Modification* IdentificationManager::getModificationFromId( const int id ) {
	Modification* m = NULL;
	for( unsigned int i = 0; i < m_arrModifications.size() && m == NULL; i++ ) {
		if( m_arrModifications[ i ]->getId() == id ) {
			m = m_arrModifications[ i ];
		}
	}

	return m;
}

// set all modifications hidden
void IdentificationManager::setAllModificationHidden() { 
	// set objects
	for( unsigned int i = 0; i < m_arrModifications.size(); i++ ) {
		m_arrModifications[ i ]->setHidden( true );
	}

	// database
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return;
	}

	std::string sql = FMT( "update %s set %s = ?;", TABLE_MOD, COL_HIDDEN_MOD );

	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindInteger( 1, 1 );
		stmt->execute();
		stmt->close();
	}
}

// update modification
int IdentificationManager::updateModification( Modification* modification ) {
	// parameter check
	if( modification == NULL ) {
		return -1;
	}

	std::string name = modification->getName();
	if( name.empty() ) {
		return -1;
	}

	// database
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return -1;
	}

	// ID
	int id = searchModification( name.c_str() );
	std::string residue = modification->getAminoAcidResidue();

	// update modification
	if( id >= 0 ) {    // update
		Modification* m = getModificationFromId( id );

		std::string sql = FMT(
			"update %s set %s = ?, %s = ?, %s = ? where %s = ?;",
			TABLE_MOD,
			COL_TYPE_MOD,
			COL_MASS_MOD,
			COL_HIDDEN_MOD,
			COL_ID_MOD
		);

		kome::db::Statement* stmt = db->prepare( sql.c_str() );
		if( stmt != NULL ) {
			stmt->resetBind();
			stmt->bindString( 1, residue.c_str() );
			stmt->bindReal( 2, modification->getMonoMass() );
			stmt->bindInteger( 3, ( modification->isHidden() ? 1 : 0 ) );
			stmt->bindInteger( 4, id );
			stmt->execute();
			stmt->close();
		}

		m->setAminoAcidResidue( residue.c_str() );
		m->setMonoMass( modification->getMonoMass() );
		m->setHidden( modification->isHidden() );
	}
	else {    // insert
		std::string sql = FMT(
			"insert into %s (%s, %s, %s, %s) values (?, ?, ?, ?);",
			TABLE_MOD,
			COL_NAME_MOD,
			COL_TYPE_MOD,
			COL_MASS_MOD,
			COL_HIDDEN_MOD
		);

		kome::db::Statement* stmt = db->prepare( sql.c_str() );
		if( stmt != NULL ) {
			stmt->resetBind();
			stmt->bindString( 1, name.c_str() );
			stmt->bindString( 2, residue.c_str() );
			stmt->bindReal( 3, modification->getMonoMass() );
			stmt->bindInteger( 4, ( modification->isHidden() ? 1 : 0 ) );
			stmt->execute();
			stmt->close();
		}

		id = db->getMaxValue( TABLE_MOD, COL_ID_MOD );

		Modification* m = new Modification(
			name.c_str(),
			modification->getMonoMass(),
			residue.c_str(),
			modification->isHidden()
		);

		m->setId( id );
		m_arrModifications.push_back( m );
	}

	return id;
}

// search modification
int IdentificationManager::searchModification( const char* name ) {
	// return value
	int id = -1;

	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return id;
	}

	// search
	std::string sql = FMT(
		"select %s from %s where %s = ?;",
		COL_ID_MOD,
		TABLE_MOD,
		COL_NAME_MOD
	);

	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindString( 1, name );
		kome::db::ResultSet* rs = stmt->executeQuery();
		if( rs != NULL ) {
			if( rs->next() ) {
				id = rs->getInt( 0, -1 );
			}
			rs->close();
		}
		stmt->close();
	}

	return id;
}

// delete modifications
void IdentificationManager::deleteModifications() {
	for( unsigned int i = 0; i < m_arrModifications.size(); i++ ) {
		delete m_arrModifications[ i ];
	}

	m_arrModifications.clear();
}

// add search modification
int IdentificationManager::addSearchModification( const int search, const int modification, const bool isVariable ) {
	// search mod ID
	int id = -1;

	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return id;
	}

	// add
	std::string sql = FMT(
		"insert into %s (%s, %s, %s) values (?, ?, ?);",
		TABLE_SEARCH_MOD,
		COL_SEARCH_SEARCH_MOD,
		COL_MOD_SEARCH_MOD,
		COL_VARIABLE_SEARCH_MOD
	);

	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindInteger( 1, search );
		stmt->bindInteger( 2, modification );
		stmt->bindInteger( 3, ( isVariable ? 1 : 0 ) );

		if( stmt->execute() ) {
			id = db->getMaxValue( TABLE_SEARCH_MOD, COL_ID_SEARCH_MOD );
		}
		stmt->close();
	}

	return id;
}

// get search modifications
void IdentificationManager::getSearchModifications( std::vector< int >& searchMods, const int searchId ) {
	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return;
	}

	// search modifications
	std::string sql = FMT(
		"select %s from %s where %s = ?;",
		COL_ID_SEARCH_MOD,
		TABLE_SEARCH_MOD,
		COL_SEARCH_SEARCH_MOD
	);

	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindInteger( 1, searchId );

		kome::db::ResultSet* rs = stmt->executeQuery();

		if( rs != NULL ) {
			while( rs->next() ) {
				int id = rs->getInt( 0, -1 );
				if( id >= 0 ) {
					searchMods.push_back( id );
				}
			}
			rs->close();
		}
		stmt->close();
	}
}

// get modification
int IdentificationManager::getModificationFromSearchModification( const int searchModId ) {
	// modification
	int modification = -1;

	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return modification;
	}

	// get
	std::string sql = FMT(
		"select %s from %s where %s = ?;",
		COL_MOD_SEARCH_MOD,
		TABLE_SEARCH_MOD,
		COL_ID_SEARCH_MOD
	);

	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindInteger( 1, searchModId );
		kome::db::ResultSet* rs = stmt->executeQuery();
		if( rs != NULL ) {
			if( rs->next() ) {
				modification = rs->getInt( 0, -1 );
			}
			rs->close();
		}
		stmt->close();
	}
   
	return modification;		
}

// get search from search modification
int IdentificationManager::getSearchFromSearchModification( const int searchModId ) {
	// search
	int search = -1;

	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return search;
	}

	// get
	std::string sql = FMT(
		"select %s from %s where %s = ?;",
		COL_SEARCH_SEARCH_MOD,
		TABLE_SEARCH_MOD,
		COL_ID_SEARCH_MOD
	);

	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindInteger( 1, searchModId );
		kome::db::ResultSet* rs = stmt->executeQuery();
		if( rs != NULL ) {
			if( rs->next() ) {
				search = rs->getInt( 0, -1 );
			}
			rs->close();
		}
		stmt->close();
	}

	return search;
}

// is variable modification
bool IdentificationManager::isVariableModification( const int searchModId ) {
	// variable 
	bool isVariable = false;

	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return isVariable;
	}

	// get 
	std::string sql = FMT(
		"select %s from %s where %s = ?;",
		COL_VARIABLE_SEARCH_MOD,
		TABLE_SEARCH_MOD,
		COL_ID_SEARCH_MOD
	);

	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindInteger( 1, searchModId );
		kome::db::ResultSet* rs = stmt->executeQuery();
		if( rs != NULL ) {
			if( rs->next() ) {
				isVariable = ( rs->getInt( 0, 0 ) > 0 );
			}
			rs->close();
		}
		stmt->close();
	}

	return isVariable;
}

// delete search modifications
void IdentificationManager::deleteSearchModifications( const int searchId ) {
	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return;
	}

	// delete
	std::string sql = FMT(
		"delete from %s where %s = ?;",
		TABLE_SEARCH_MOD,
		COL_SEARCH_SEARCH_MOD
	);

	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindInteger( 1, searchId );
		stmt->execute();
		stmt->close();
	}
}

// add hit modification
int IdentificationManager::addHitModification( const int hit, const int searchMod, const int pos ) {
	// ID
	int id = -1;

	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return id;
	}

	// add
	std::string sql = FMT(
		"insert into %s (%s, %s, %s) values (?, ?, ?);",
		TABLE_HIT_MOD,
		COL_HIT_HIT_MOD,
		COL_MOD_HIT_MOD,
		COL_POS_HIT_MOD
	);

	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindInteger( 1, hit );
		stmt->bindInteger( 2, searchMod );
		stmt->bindInteger( 3, pos );

		if( stmt->execute() ) {
			id = db->getMaxValue( TABLE_HIT_MOD, COL_ID_HIT_MOD );
		}
		stmt->close();
	}

	return id;
}

// get hit modifications
void IdentificationManager::getHitModifications( std::vector< int >& hitMods, const int hitId ) {
	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return;
	}

	// hit modifications
	std::string sql = FMT(
		"select %s from %s where %s = ?;",
		COL_ID_HIT_MOD,
		TABLE_HIT_MOD,
		COL_HIT_HIT_MOD
	);

	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindInteger( 1, hitId );
		kome::db::ResultSet* rs = stmt->executeQuery();
		if( rs != NULL ) {
			while( rs->next() ) {
				int id = rs->getInt( 0, -1 );
				if( id >= 0 ) {
					hitMods.push_back( id );
				}
			}
			rs->close();
		}
		stmt->close();
	}
}

// search modification from hit modification
int IdentificationManager::getSearchModificationFromHitModification( const int hitId ) {
	// modification
	int m = -1;

	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return m;
	}

	// get
	std::string sql = FMT(
		"select %s from %s where %s = ?;",
		COL_MOD_HIT_MOD,
		TABLE_HIT_MOD,
		COL_ID_HIT_MOD
	);

	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindInteger( 1, hitId );
		kome::db::ResultSet* rs = stmt->executeQuery();
		if( rs != NULL ) {
			if( rs->next() ) {
				m = rs->getInt( 0, -1 );
			}
			rs->close();
		}
		stmt->close();
	}

	return m;
}

// hit from hit modification
int IdentificationManager::getHitFromHitModification( const int hitMod ) {
	// hit
	int h = -1;

	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return h;
	}

	// get
	std::string sql = FMT(
		"select %s from %s where %s = ?;",
		COL_HIT_HIT_MOD,
		TABLE_HIT_MOD,
		COL_ID_HIT_MOD
	);

	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindInteger( 1, hitMod );
		kome::db::ResultSet* rs = stmt->executeQuery();
		if( rs != NULL ) {
			if( rs->next() ) {
				h = rs->getInt( 0, -1 );
			}
			rs->close();
		}
		stmt->close();
	}

	return h;
}

// get hit modification position
int IdentificationManager::getHitModPosition( const int hitMod ) {
	// position
	int pos = -1;

	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return pos;
	}

	// get
	std::string sql = FMT(
		"select %s from %s where %s = ?;",
		COL_POS_HIT_MOD,
		TABLE_HIT_MOD,
		COL_ID_HIT_MOD
	);

	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindInteger( 1, hitMod );
		kome::db::ResultSet* rs = stmt->executeQuery();
		if( rs != NULL ) {
			if( rs->next() ) {
				pos = rs->getInt( 0, -1 );
			}
			rs->close();
		}
		stmt->close();
	}

	return pos;
}

// delete hit modifications
void IdentificationManager::deleteHitModifications( const int hitId ) {
	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return;
	}

	// delete
	std::string sql = FMT(
		"delete from %s where %s = ?;"
		TABLE_HIT_MOD,
		COL_HIT_HIT_MOD
	);

	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindInteger( 1, hitId );
		stmt->execute();
		stmt->close();
	}
}

// parse the spectrum title string into a SpecInfo struct
bool IdentificationManager::ParseSpectrumTitle(const char* spectrumTitle, SpecInfo& spectrumInfo, int* charge)
{
	bool bSuccess = true;

	// Initialise SpecInfo object, so we can easily tell which values were not read.
		
	std::string strTitle = spectrumTitle;
	std::string strValue;
	bool bSearchingForValues = true;

	while (bSearchingForValues && bSuccess)
	{
		std::string::size_type iColonPos = strTitle.find(":");
		if (iColonPos != std::string::npos)
		{
			std::string strTag = trimstring( strTitle.substr(0, iColonPos).c_str() );
			std::string::size_type iCommaPos = strTitle.find(",");
			if (iCommaPos == std::string::npos)
			{
				iCommaPos = strTitle.find("---");
				bSearchingForValues = false;
			}
			if (iCommaPos != std::string::npos)
			{
				strValue = trimstring( strTitle.substr(iColonPos + 2, iCommaPos - iColonPos - 2).c_str() );

				if (strTag.compare("spec_id") == 0)
				{
					spectrumInfo.specId = atol(strValue.c_str());
				}
				else if (strTag.compare("sample_index") == 0)
				{
					spectrumInfo.sampleIndex = atol(strValue.c_str());
				}
				else if (strTag.compare("sample_name") == 0)
				{
					spectrumInfo.sampleName = strValue;
				}
				else if (strTag.compare("file_path") == 0)
				{
					spectrumInfo.path = strValue;
				}
				else if (strTag.compare("spec_rt") == 0)
				{
					spectrumInfo.retentionTime = atof(strValue.c_str());
				}
				else if (strTag.compare("spec_prec") == 0)
				{
					spectrumInfo.precursor = atof(strValue.c_str());
				}
				else if (strTag.compare("spec_stage") == 0)
				{
					spectrumInfo.stage = atol(strValue.c_str());
				}
				else if (strTag.compare("charge") == 0)
				{
					if( charge != NULL ) {
						*charge = atol( strValue.c_str() );
					}
				}
				else if (strTag.compare("polarity") == 0)
				{
					spectrumInfo.polarity = kome::objects::Spectrum::POLARITY_UNKNOWN;
					if( strValue.find( "+" ) != strValue.npos ) {
						spectrumInfo.polarity = kome::objects::Spectrum::POLARITY_POSITIVE;
					}
					else if( strValue.find( "-" ) != strValue.npos ) {
						spectrumInfo.polarity = kome::objects::Spectrum::POLARITY_NEGATIVE;
					}
				}
				else if (strTag.compare("peak_id") == 0)
				{
					std::vector< std::string > tokens;
					stringtoken( strValue.c_str(), "+", tokens );
					for( unsigned int i = 0; i < tokens.size(); i++ ) {
						int peakId = -1;
						if( isint( tokens[ i ].c_str(), 10, &peakId ) ) {
							if( peakId >= 0 ) {
								spectrumInfo.peakIds.push_back( peakId );
							}
						}
					}
				}

				strTitle = strTitle.substr(iCommaPos + 2, strTitle.size() - iCommaPos - 2);
				if (!bSearchingForValues)
				{	// After finding a hyphen, the remaining string is the spectrum name.
					spectrumInfo.name = strTitle;
				}
			}
			else
			{
				bSuccess = false;
			}
		}
		else {
			bSearchingForValues = false;
		}
	}
	
	return bSuccess;
}

// get database home directory
const char* IdentificationManager::getDbHome() {
	if( m_dbHomeDir.empty() ) {
		std::string dbHome = getenvariable( "MASSPP_DB_HOME" );
		if( !dbHome.empty() ) {
			m_dbHomeDir = absolutepath( dbHome.c_str() );
		}
	}

	return m_dbHomeDir.c_str();
}

// load local db files
void IdentificationManager::loadLocalDbFiles() {
	// clear
	m_localDbs.clear();

	// xml file
	kome::core::MsppManager& msppMgr = kome::core::MsppManager::getInstance();

	std::string dbHome = getDbHome();
	std::string orgFile = getpath( dbHome.c_str(), "database.xml" );
	std::string xmlFile = getpath( msppMgr.getConfDir(), "database.xml" );

	if( !copyFileIfNew( orgFile.c_str(), xmlFile.c_str() ) ) {
		xmlFile.clear();
	}

	// load
	if( !xmlFile.empty() ) {
		LocalDbHandler handler( m_localDbs );
		handler.parse( xmlFile.c_str() );
	}

	// register
	for( unsigned int i = 0; i < m_localDbs.size(); i++ ) {
		DbInfo& info = m_localDbs[ i ];

		// id
		const int dbId = addDatabase( info.name.c_str(), info.url.c_str() );

		// formats
		for( unsigned int j = 0; j < info.formats.size(); j++ ) {
			if( !info.formats[ j ].name.empty() && !info.formats[ j ].url.empty() ) {
				setFormatUrl( dbId, info.formats[ j ].name.c_str(), info.formats[ j ].url.c_str() );
			}
		}
	}

	commit();
}

// add a search engine
void IdentificationManager::addSearchEngine( SearchEngine* pSrchEngine )
{
	_ASSERT(pSrchEngine != NULL);

	if (pSrchEngine != NULL)
	{
		m_arrSrchEngines.push_back(pSrchEngine);
	}
}

// get the number of search engines
int IdentificationManager::getNumberOfSearchEngines( void ) const
{
	return m_arrSrchEngines.size();
}

// get the specified search engine
SearchEngine* IdentificationManager::getSearchEngine( const int index ) const
{
	SearchEngine* ret = NULL;
	// check the index
	_ASSERT( (index >= 0) && (index < static_cast<int>(m_arrSrchEngines.size())) );

	if( index < static_cast<int>(m_arrSrchEngines.size()) ) {
		ret = m_arrSrchEngines[ index ];
	}
	return ret;
}

// remove a search engine
bool IdentificationManager::removeSearchEngine( SearchEngine* pSrchEngine )
{
	bool bRemoved = false;

	_ASSERT(pSrchEngine != NULL);

	if (pSrchEngine != NULL)
	{
		for (int i = 0; (i < static_cast<int>(m_arrSrchEngines.size())) && !bRemoved; i++ )
		{
			if (m_arrSrchEngines[i] == pSrchEngine)
			{
				m_arrSrchEngines.erase(m_arrSrchEngines.begin() + i);
				bRemoved = true;
			}
		}
	}

	return bRemoved;
}

// get database
kome::db::Database* IdentificationManager::getDb() {
	// get DB
	kome::db::DbManager& dbMgr = kome::db::DbManager::getInstance();
	kome::db::Database* db = dbMgr.getDb();
	if( db == m_db ) {
		return db;
	}

	// null check
	m_db = db;
	if( db == NULL ) {
		LOG_ERROR_CODE( FMT( "There is a problem with the Mass++ results database configuration and a connection cannot be made.\nCheck the database configuration (File -> Database...)." ), ERR_OTHER );
		return NULL;
	}

	// tables
	for( unsigned int i = 0; i < m_tables.size(); i++ ) {
		kome::db::Table& table = m_tables[ i ];
		db->createTable( table );
	}

	return db;
}

// commit
void IdentificationManager::commit() {
	// db
	kome::db::Database* db = getDb();
	if( db != NULL ) {
		db->commit();
	}
}

// rollback
void IdentificationManager::rollback() {
	// db
	kome::db::Database* db = getDb();
	if( db != NULL ) {
		db->rollback();
	}
}

// create tables
void IdentificationManager::createTables() {
	m_tables.resize( 44 );

	createSampleSetTable( m_tables[ 0 ] );
	createSampleTable( m_tables[ 1 ] );
	createSpecTable( m_tables[ 2 ] );
	createVersionTable( m_tables[ 3 ] );
	createDbTable( m_tables[ 4 ] );
	createFormatTable( m_tables[ 5 ] );
	createUrlTable( m_tables[ 6 ] );
	createEngineTable( m_tables[ 7 ] );
	createSearchTable( m_tables[ 8 ] );
	createSubstanceTable( m_tables[ 9 ] );
	createRelationshipTable( m_tables[ 10 ] );
	createSubstanceItemTable( m_tables[ 11 ] );
	createSubstanceValueTable( m_tables[ 12 ] );
	createPeakDetectionTable( m_tables[ 13 ] );
	createPeakParamsTable( m_tables[ 14 ] );
	createHitTable( m_tables[ 15 ] );
	createHitSpecTable( m_tables[ 16 ] );
	createHitItemTable( m_tables[ 17 ] );
	createHitItemTable( m_tables[ 18 ] );
	createHitValueTable( m_tables[ 19 ] );
	createSearchParamsTable( m_tables[ 20 ] );
	createSearchValueTable( m_tables[ 21 ] );
	createSearchSpecTable( m_tables[ 22 ] );
	createMatrixTable( m_tables[ 23 ] );
	createGroupTable( m_tables[ 24 ] );
	createMatrixSampleTable( m_tables[ 25 ] );
	createPeakTable( m_tables[ 26 ] );
	createMatrixColumnTable( m_tables[ 27 ] );
	createMatrixValueTable( m_tables[ 28 ] );
	createMatrixSpectrumTable( m_tables[ 29 ] );
	createProfileTable( m_tables[ 30 ] );
	createPeakValueTable( m_tables[ 31 ] );
	createAccessionTable( m_tables[ 32 ] );
	createStandardTable( m_tables[ 33 ] );
	createNormalizationFilterTable( m_tables[ 34 ] );
	createNormalizationMethodTable( m_tables[ 35 ] );
	createRtAlignmentTable( m_tables[ 36 ] );
	createAltSubstanceTable( m_tables[ 37 ] );
	createPeakListTable( m_tables[ 38 ] );
	createModTable( m_tables[ 39 ] );
	createSearchModTable( m_tables[ 40 ] );
	createHitModTable( m_tables[ 41 ] );
	createEnzymeTable( m_tables[ 42 ] );
	createEnzymeCutterTable( m_tables[ 43 ] );
}

// create sample set table
void IdentificationManager::createSampleSetTable( kome::db::Table& table ) {
	// table name
	table.setName( TABLE_SAMPLE_SET );

	// ID
	kome::db::TableColumn* col = table.addColumn( COL_ID_SAMPLE_SET );
	col->setAutoIncrement( true );
	col->setNotNull( true );
	col->setDataType( kome::db::TableColumn::INTEGER );

	kome::db::TableIndex* idx = table.addIndex( INDEX_ID_SAMPLE_SET );
	idx->addColumn( col );

	// path
	col = table.addColumn( COL_PATH_SAMPLE_SET );
	col->setNotNull( true );
	col->setDataType( kome::db::TableColumn::VARCHAR );

	idx = table.addIndex( INDEX_PATH_SAMPLE_SET );
	idx->addColumn( col );
}

// create sample table
void IdentificationManager::createSampleTable( kome::db::Table& table ) {
	// table name
	table.setName( TABLE_SAMPLE );

	// ID
	kome::db::TableColumn* col = table.addColumn( COL_ID_SAMPLE );
	col->setAutoIncrement( true );
	col->setNotNull( true );
	col->setDataType( kome::db::TableColumn::INTEGER );

	kome::db::TableIndex* idx = table.addIndex( INDEX_ID_SAMPLE );
	idx->addColumn( col );

	// sample set
	col = table.addColumn( COL_SAMPLE_SET_SAMPLE );
	col->setNotNull( true );
	col->setDataType( kome::db::TableColumn::INTEGER );

	idx = table.addIndex( INDEX_SAMPLE_SET_SAMPLE );
	idx->addColumn( col );

	// sample ID
	col = table.addColumn( COL_SAMPLE_ID_SAMPLE );
	col->setNotNull( true );
	col->setDataType( kome::db::TableColumn::INTEGER );

	idx->addColumn( col );
	
	// name
	col = table.addColumn( COL_NAME_SAMPLE );
	col->setNotNull( true );
	col->setDataType( kome::db::TableColumn::VARCHAR );
}

// create spectra table
void IdentificationManager::createSpecTable( kome::db::Table& table ) {
	// table name
	table.setName( TABLE_SPECTRUM );

	// ID
	kome::db::TableColumn* col = table.addColumn( COL_ID_SPECTRUM );
	col->setAutoIncrement( true );
	col->setNotNull( true );
	col->setDataType( kome::db::TableColumn::INTEGER );

	kome::db::TableIndex* idx = table.addIndex( INDEX_ID_SPECTRUM );
	idx->addColumn( col );

	// sample
	col = table.addColumn( COL_SAMPLE_SPECTRUM );
	col->setNotNull( true );
	col->setDataType( kome::db::TableColumn::INTEGER );

	idx = table.addIndex( INDEX_SAMPLE_SPECTRUM );
	idx->addColumn( col ) ;

	// id
	col = table.addColumn( COL_SPECTRUM_ID_SPECTRUM );
	col->setNotNull( true );
	col->setDataType( kome::db::TableColumn::INTEGER );

	// name
	col = table.addColumn( COL_NAME_SPECTRUM );
	col->setNotNull( true );
	col->setDataType( kome::db::TableColumn::VARCHAR );

	// stage
	col = table.addColumn( COL_STAGE_SPECTRUM );
	col->setNotNull( true );
	col->setDataType( kome::db::TableColumn::INTEGER );

	idx = table.addIndex( INDEX_STAGE_SPECTRUM );
	idx->addColumn( col );

	// RT
	col = table.addColumn( COL_RT_SPECTRUM );
	col->setDataType( kome::db::TableColumn::REAL );

	// Precursor
	col = table.addColumn( COL_PRECURSOR_SPECTRUM );
	col->setDataType( kome::db::TableColumn::REAL );

	// Polarity
	col = table.addColumn( COL_POLARITY_SPECTRUM );
	col->setDataType( kome::db::TableColumn::INTEGER );

	idx = table.addIndex( INDEX_PRECURSOR_SPECTRUM );
	idx->addColumn( col );
}

// create version table
void IdentificationManager::createVersionTable( kome::db::Table& table ) {
	// name
	table.setName( TABLE_VERSION );

	// version
	kome::db::TableColumn* col = table.addColumn( COL_VERSION_VERSION );
	col->setNotNull( true );
	col->setDataType( kome::db::TableColumn::INTEGER );
}

// create db table
void IdentificationManager::createDbTable( kome::db::Table& table ) {
	// name
	table.setName( TABLE_DATABASE );

	// id
	kome::db::TableColumn* col = table.addColumn( COL_ID_DATABASE );
	col->setNotNull( true );
	col->setAutoIncrement( true );
	col->setDataType( kome::db::TableColumn::INTEGER );

	kome::db::TableIndex* idx = table.addIndex( INDEX_ID_DATABASE );
	idx->addColumn( col );

	// name
	col = table.addColumn( COL_NAME_DATABASE );
	col->setNotNull( true );
	col->setDataType( kome::db::TableColumn::VARCHAR );

	idx = table.addIndex( INDEX_NAME_DATABASE );
	idx->addColumn( col );

	// URL
	col = table.addColumn( COL_URL_DATABASE );
	col->setDataType( kome::db::TableColumn::VARCHAR );
}

// create format table
void IdentificationManager::createFormatTable( kome::db::Table& table ) {
	// name
	table.setName( TABLE_DATA_FORMAT );

	// ID
	kome::db::TableColumn* col = table.addColumn( COL_ID_FORMAT );
	col->setAutoIncrement( true );
	col->setNotNull( true );
	col->setDataType( kome::db::TableColumn::INTEGER );

	kome::db::TableIndex* idx = table.addIndex( INDEX_ID_FORMAT );
	idx->addColumn( col );

	// name
	col = table.addColumn( COL_NAME_FORMAT );
	col->setNotNull( true );
	col->setDataType( kome::db::TableColumn::VARCHAR );

	idx = table.addIndex( INDEX_NAME_FORMAT );
	idx->addColumn( col );
}

// create URL table
void IdentificationManager::createUrlTable( kome::db::Table& table ) {
	// set name
	table.setName( TABLE_URL );

	// ID
	kome::db::TableColumn* col = table.addColumn( COL_ID_URL );
	col->setAutoIncrement( true );
	col->setNotNull( true );
	col->setDataType( kome::db::TableColumn::INTEGER );

	kome::db::TableIndex* idx = table.addIndex( INDEX_ID_URL );
	idx->addColumn( col );

	// DB
	col = table.addColumn( COL_DB_URL );
	col->setNotNull( true );
	col->setDataType( kome::db::TableColumn::INTEGER );

	idx = table.addIndex( INDEX_DB_FORMAT_URL );
	idx->addColumn( col );

	// FORMAT
	col = table.addColumn( COL_FORMAT_URL );
	col->setNotNull( true );
	col->setDataType( kome::db::TableColumn::INTEGER );

	idx->addColumn( col );

	// URL
	col = table.addColumn( COL_URL_URL );
	col->setNotNull( true );
	col->setDataType( kome::db::TableColumn::VARCHAR );
}

// create engine table
void IdentificationManager::createEngineTable( kome::db::Table& table ) {
	// name
	table.setName( TABLE_ENGINE );

	// ID
	kome::db::TableColumn* col = table.addColumn( COL_ID_ENGINE );
	col->setAutoIncrement( true );
	col->setNotNull( true );
	col->setDataType( kome::db::TableColumn::INTEGER );

	kome::db::TableIndex* idx = table.addIndex( INDEX_ID_ENGINE );
	idx->addColumn( col );

	// name
	col = table.addColumn( COL_NAME_ENGINE );
	col->setNotNull( true );
	col->setDataType( kome::db::TableColumn::VARCHAR );

	idx = table.addIndex( INDEX_NAME_ENGINE );
	idx->addColumn( col );

	// version
	col = table.addColumn( COL_VERSION_ENGINE );
	col->setDataType( kome::db::TableColumn::VARCHAR );
}

// create search table
void IdentificationManager::createSearchTable( kome::db::Table& table ) {
	// name
	table.setName( TABLE_SEARCH );

	// ID
	kome::db::TableColumn* col = table.addColumn( COL_ID_SEARCH );
	col->setAutoIncrement( true );
	col->setNotNull( true );
	col->setDataType( kome::db::TableColumn::INTEGER );

	kome::db::TableIndex* idx = table.addIndex( INDEX_ID_SEARCH );
	idx->addColumn( col );

	// name
	col = table.addColumn( COL_NAME_SEARCH );
	col->setNotNull( true );
	col->setDataType( kome::db::TableColumn::VARCHAR );

	// comment
	col = table.addColumn( COL_COMMENT_SEARCH );
	col->setDataType( kome::db::TableColumn::VARCHAR );

	// date
	col = table.addColumn( COL_DATE_SEARCH );
	col->setNotNull( true );
	col->setDataType( kome::db::TableColumn::VARCHAR );

	idx = table.addIndex( INDEX_DATE_SEARCH );
	idx->addColumn( col );

	// engine
	col = table.addColumn( COL_ENGINE_SEARCH );
	col->setNotNull( true );
	col->setDataType( kome::db::TableColumn::INTEGER );

	idx = table.addIndex( INDEX_ENGINE_SEARCH);
	idx->addColumn( col );

	// DB
	col = table.addColumn( COL_DB_SEARCH );
	col->setNotNull( true );
	col->setDataType( kome::db::TableColumn::INTEGER );

	idx = table.addIndex( INDEX_DB_SEARCH );
	idx->addColumn( col );

	// result
	col = table.addColumn( COL_RESULT_SEARCH );
	col->setDataType( kome::db::TableColumn::VARCHAR );

	// precursor mass type
	col = table.addColumn( COL_PREC_MASS_TYPE_SEARCH );
	col->setDataType( kome::db::TableColumn::INTEGER );

	// frgment mass type
	col = table.addColumn( COL_FRAG_MASS_TYPE_SEARCH );
	col->setDataType( kome::db::TableColumn::INTEGER );

	// enzyme
	col = table.addColumn( COL_ENZ_SEARCH );
	col->setDataType( kome::db::TableColumn::INTEGER );

	// base name
	col = table.addColumn( COL_BASE_NAME_SEARCH );
	col->setDataType( kome::db::TableColumn::VARCHAR );

	// search ID
	col = table.addColumn( COL_SEARCH_ID_SEARCH );
	col->setDataType( kome::db::TableColumn::VARCHAR );

	idx = table.addIndex( INDEX_SEARCH_ID_SEARCH );
	idx->addColumn( col );

	// status
	col = table.addColumn( COL_STATUS_SEARCH );
	col->setDataType( kome::db::TableColumn::INTEGER );
}

// create substance table
void IdentificationManager::createSubstanceTable( kome::db::Table& table ) {
	// name
	table.setName( TABLE_SUBSTANCE );

	// ID
	kome::db::TableColumn* col = table.addColumn( COL_ID_SUBSTANCE );
	col->setAutoIncrement( true );
	col->setNotNull( true );
	col->setDataType( kome::db::TableColumn::INTEGER );

	kome::db::TableIndex* idx = table.addIndex( INDEX_ID_SUBSTANCE );
	idx->addColumn( col );

	// accession
	col = table.addColumn( COL_ACCESSION_SUBSTANCE );
	col->setDataType( kome::db::TableColumn::VARCHAR );

	idx = table.addIndex( INDEX_ACCESSION_SUBSTANCE );
	idx->addColumn( col );

	// name
	col = table.addColumn( COL_NAME_SUBSTANCE );
	col->setNotNull( true );
	col->setDataType( kome::db::TableColumn::VARCHAR );

	// DB
	col = table.addColumn( COL_DATABASE_SUBSTANCE );
	col->setDataType( kome::db::TableColumn::INTEGER );

	idx = table.addIndex( INDEX_DB_SUBSTANCE );
	idx->addColumn( col );

	// type
	col = table.addColumn( COL_TYPE_SUBSTANCE );
	col->setDataType( kome::db::TableColumn::INTEGER );

	idx = table.addIndex( INDEX_TYPE_SUBSTANCE );
	idx->addColumn( col );
}

// create substance relationship table
void IdentificationManager::createRelationshipTable( kome::db::Table& table ) {
	// name
	table.setName( TABLE_RELATIONSHIP );

	// ID
	kome::db::TableColumn* col = table.addColumn( COL_ID_RELATIONSHIP );
	col->setAutoIncrement( true );
	col->setNotNull( true );
	col->setDataType( kome::db::TableColumn::INTEGER );

	kome::db::TableIndex* idx = table.addIndex( INDEX_ID_RELATIONSHIP );
	idx->addColumn( col );

	// A
	col = table.addColumn( COL_A_RELATIONSHIP );
	col->setNotNull( true );
	col->setDataType( kome::db::TableColumn::INTEGER );

	idx = table.addIndex( INDEX_A_RELATIONSHIP );
	idx->addColumn( col );

	// B
	col = table.addColumn( COL_B_RELATIONSHIP );
	col->setNotNull( true );
	col->setDataType( kome::db::TableColumn::INTEGER );

	idx = table.addIndex( INDEX_B_RELATIONSHIP );
	idx->addColumn( col );

	// relationship
	col = table.addColumn( COL_RELATIONSHIP_RELATIONSHIP );
	col->setNotNull( true );
	col->setDataType( kome::db::TableColumn::INTEGER );

	idx = table.addIndex( INDEX_RELATIONSHIP_RELATIONSHIP );
	idx->addColumn( col );
}

// create substance item table
void IdentificationManager::createSubstanceItemTable( kome::db::Table& table ) {
	// name
	table.setName( TABLE_SUBSTANCE_ITEM );

	// ID
	kome::db::TableColumn* col = table.addColumn( COL_ID_SUBSTANCE_ITEM );
	col->setAutoIncrement( true );
	col->setNotNull( true );
	col->setDataType( kome::db::TableColumn::INTEGER );

	kome::db::TableIndex* idx = table.addIndex( INDEX_ID_SUBSTANCE_ITEM );
	idx->addColumn( col );

	// name
	col = table.addColumn( COL_NAME_SUBSTANCE_ITEM );
	col->setNotNull( true );
	col->setDataType( kome::db::TableColumn::VARCHAR );

	// database
	col = table.addColumn( COL_DB_SUBSTANCE_ITEM );
	col->setNotNull( true );
	col->setDataType( kome::db::TableColumn::INTEGER );

	idx = table.addIndex( INDEX_DB_TYPE_SUBSTANCE_ITEM );
	idx->addColumn( col );

	// type
	col = table.addColumn( COL_TYPE_SUBSTANCE_ITEM );
	col->setNotNull( true );
	col->setDataType( kome::db::TableColumn::INTEGER );

	idx->addColumn( col );
}

// create substance value table
void IdentificationManager::createSubstanceValueTable( kome::db::Table& table ) {
	// name
	table.setName( TABLE_SUBSTANCE_VALUE );

	// ID
	kome::db::TableColumn* col = table.addColumn( COL_ID_SUBSTANCE_VAL );
	col->setAutoIncrement( true );
	col->setNotNull( true );
	col->setDataType( kome::db::TableColumn::INTEGER );

	kome::db::TableIndex* idx = table.addIndex( INDEX_ID_SUBSTANCE_VAL );
	idx->addColumn( col );

	// item
	col = table.addColumn( COL_ITEM_SUBSTANCE_VAL );
	col->setNotNull( true );
	col->setDataType( kome::db::TableColumn::INTEGER );

	idx = table.addIndex( INDEX_ITEM_SUBSTANCE_VAL );
	idx->addColumn( col );

	// substance
	col = table.addColumn( COL_SUBSTANCE_SUBSTANCE_VAL );
	col->setNotNull( true );
	col->setDataType( kome::db::TableColumn::INTEGER );

	idx->addColumn( col );
	idx = table.addIndex( INDEX_SUBSTANCE_SUBSTANCE_VAL );
	idx->addColumn( col );

	// value
	col = table.addColumn( COL_VALUE_SUBSTANCE_VAL );
	col->setNotNull( true );
	col->setDataType( kome::db::TableColumn::VARCHAR );
}

// create peak detection table
void IdentificationManager::createPeakDetectionTable( kome::db::Table& table ) {
	// set name
	table.setName( TABLE_PEAK_DETECTION );

	// ID
	kome::db::TableColumn* col = table.addColumn( COL_ID_PEAK_DETECTION );
	col->setAutoIncrement( true );
	col->setNotNull( true );
	col->setDataType( kome::db::TableColumn::INTEGER );

	kome::db::TableIndex* idx = table.addIndex( INDEX_ID_PEAK_DETECTION );
	idx->addColumn( col );

	// method
	col = table.addColumn( COL_METHOD_PEAK_DETECTION );
	col->setNotNull( true );
	col->setDataType( kome::db::TableColumn::VARCHAR );
}

// create peak detection parameters table
void IdentificationManager::createPeakParamsTable( kome::db::Table& table ) {
	// set name
	table.setName( TABLE_PEAK_PARAMS );

	// ID
	kome::db::TableColumn* col = table.addColumn( COL_ID_PEAK_PARAMS );
	col->setAutoIncrement( true );
	col->setNotNull( true );
	col->setDataType( kome::db::TableColumn::INTEGER );

	kome::db::TableIndex* idx = table.addIndex( INDEX_ID_PEAK_PARAMS );
	idx->addColumn( col );

	// search
	col = table.addColumn( COL_DETECTION_PEAK_PARAMS );
	col->setNotNull( true );
	col->setDataType( kome::db::TableColumn::INTEGER );

	idx = table.addIndex( INDEX_DETECTION_PEAK_PARAMS );
	idx->addColumn( col );

	// name
	col = table.addColumn( COL_NAME_PEAK_PARAMS );
	col->setNotNull( true );
	col->setDataType( kome::db::TableColumn::VARCHAR );

	// value
	col = table.addColumn( COL_VALUE_PEAK_PARAMS );
	col->setNotNull( true );
	col->setDataType( kome::db::TableColumn::VARCHAR );
}

// create hit table
void IdentificationManager::createHitTable( kome::db::Table& table ) {
	// name
	table.setName( TABLE_HIT );

	// ID
	kome::db::TableColumn* col = table.addColumn( COL_ID_HIT );
	col->setAutoIncrement( true );
	col->setNotNull( true );
	col->setDataType( kome::db::TableColumn::INTEGER );

	kome::db::TableIndex* idx = table.addIndex( INDEX_ID_HIT );
	idx->addColumn( col );

	// substance
	col = table.addColumn( COL_SUBSTANCE_HIT );
	col->setNotNull( true );
	col->setDataType( kome::db::TableColumn::INTEGER );

	idx = table.addIndex( INDEX_SUBSTANCE_HIT );
	idx->addColumn( col );
}

// create hit spectrum table
void IdentificationManager::createHitSpecTable( kome::db::Table& table ) {
	// name
	table.setName( TABLE_HIT_SPEC );

	// ID
	kome::db::TableColumn* col = table.addColumn( COL_ID_HIT_SPEC );
	col->setAutoIncrement( true );
	col->setNotNull( true );
	col->setDataType( kome::db::TableColumn::INTEGER );

	kome::db::TableIndex* idx = table.addIndex( INDEX_ID_HIT_SPEC );
	idx->addColumn( col );

	// spectrum
	col = table.addColumn( COL_SPEC_HIT_SPEC );
	col->setNotNull( true );
	col->setDataType( kome::db::TableColumn::INTEGER );

	idx = table.addIndex( INDEX_SPEC_HIT_SPEC );
	idx->addColumn( col );

	// hit
	col = table.addColumn( COL_HIT_HIT_SPEC );
	col->setNotNull( true );
	col->setDataType( kome::db::TableColumn::INTEGER );

	idx = table.addIndex( INDEX_HIT_HIT_SPEC );
	idx->addColumn( col );

	// charge
	col = table.addColumn( COL_CHARGE_HIT_SPEC );
	col->setDataType( kome::db::TableColumn::INTEGER );
}

// create hit item table
void IdentificationManager::createHitItemTable( kome::db::Table& table ) {
	// name
	table.setName( TABLE_HIT_ITEM );

	// ID
	kome::db::TableColumn* col = table.addColumn( COL_ID_HIT_ITEM );
	col->setAutoIncrement( true );
	col->setNotNull( true );
	col->setDataType( kome::db::TableColumn::INTEGER );

	kome::db::TableIndex* idx = table.addIndex( INDEX_ID_HIT_ITEM );
	idx->addColumn( col );

	// name
	col = table.addColumn( COL_NAME_HIT_ITEM );
	col->setNotNull( true );
	col->setDataType( kome::db::TableColumn::VARCHAR );

	// engine
	col = table.addColumn( COL_ENGINE_HIT_ITEM );
	col->setNotNull( true );
	col->setDataType( kome::db::TableColumn::INTEGER );

	idx = table.addIndex( INDEX_ENGINE_HIT_ITEM );
	idx->addColumn( col );

	// substance type
	col = table.addColumn( COL_SUB_TYPE_HIT_ITEM );
	col->setNotNull( true );
	col->setDataType( kome::db::TableColumn::INTEGER );

	// value type
	col = table.addColumn( COL_VAL_TYPE_HIT_ITEM );
	col->setNotNull( true );
	col->setDataType( kome::db::TableColumn::INTEGER );
}

// create hit value table
void IdentificationManager::createHitValueTable( kome::db::Table& table ) {
	// name
	table.setName( TABLE_HIT_VALUE );

	// ID
	kome::db::TableColumn* col = table.addColumn( COL_ID_HIT_VAL );
	col->setAutoIncrement( true );
	col->setNotNull( true );
	col->setDataType( kome::db::TableColumn::INTEGER );

	kome::db::TableIndex* idx = table.addIndex( INDEX_ID_HIT_VAL );
	idx->addColumn( col );

	// item
	col = table.addColumn( COL_ITEM_HIT_VAL );
	col->setNotNull( true );
	col->setDataType( kome::db::TableColumn::INTEGER );

	idx = table.addIndex( INDEX_ITEM_HIT_VAL );
	idx->addColumn( col );

	// hit
	col = table.addColumn( COL_HIT_HIT_VAL );
	col->setNotNull( true );
	col->setDataType( kome::db::TableColumn::INTEGER );

	idx->addColumn( col );
	idx = table.addIndex( INDEX_HIT_HIT_VAL );
	idx->addColumn( col );

	// value
	col = table.addColumn( COL_VAL_HIT_VAL );
	col->setNotNull( true );
	col->setDataType( kome::db::TableColumn::VARCHAR );
}

// create search params table
void IdentificationManager::createSearchParamsTable( kome::db::Table& table ) {
	// name
	table.setName( TABLE_SEARCH_PARAMS );

	// ID
	kome::db::TableColumn* col = table.addColumn( COL_ID_SEARCH_PARAMS );
	col->setAutoIncrement( true );
	col->setNotNull( true );
	col->setDataType( kome::db::TableColumn::INTEGER );

	kome::db::TableIndex* idx = table.addIndex( INDEX_ID_SEARCH_PARAMS );
	idx->addColumn( col );

	// name
	col = table.addColumn( COL_NAME_SEARCH_PARAMS );
	col->setNotNull( true );
	col->setDataType( kome::db::TableColumn::VARCHAR );

	// engine
	col = table.addColumn( COL_ENGINE_SEARCH_PARAMS );
	col->setNotNull( true );
	col->setDataType( kome::db::TableColumn::INTEGER );

	idx = table.addIndex( INDEX_ENGINE_SEARCH_PARAMS );
	idx->addColumn( col );
}

// create search value table
void IdentificationManager::createSearchValueTable( kome::db::Table& table ) {
	// name
	table.setName( TABLE_SEARCH_VALUE );

	// ID
	kome::db::TableColumn* col = table.addColumn( COL_ID_SEARCH_VAL );
	col->setAutoIncrement( true );
	col->setNotNull( true );
	col->setDataType( kome::db::TableColumn::INTEGER );

	kome::db::TableIndex* idx = table.addIndex( INDEX_ID_SEARCH_VAL );
	idx->addColumn( col );

	// parameter
	col = table.addColumn( COL_ITEM_SEARCH_VAL );
	col->setNotNull( true );
	col->setDataType( kome::db::TableColumn::INTEGER );

	idx = table.addIndex( INDEX_PARAM_SEARCH_VAL );
	idx->addColumn( col );

	// search
	col = table.addColumn( COL_SEARCH_SEARCH_VAL );
	col->setNotNull( true );
	col->setDataType( kome::db::TableColumn::INTEGER );

	idx->addColumn( col );
	idx = table.addIndex( INDEX_SEARCH_SEARCH_VAL );
	idx->addColumn( col );

	// value
	col = table.addColumn( COL_VALUE_SEARCH_VAL );
	col->setNotNull( true );
	col->setDataType( kome::db::TableColumn::VARCHAR );
}

// create search spectrum table
void IdentificationManager::createSearchSpecTable( kome::db::Table& table ) {
	// name
	table.setName( TABLE_SEARCH_SPEC );

	// ID
	kome::db::TableColumn* col = table.addColumn( COL_ID_SEARCH_SPEC );
	col->setAutoIncrement( true );
	col->setNotNull( true );
	col->setDataType( kome::db::TableColumn::INTEGER );

	kome::db::TableIndex* idx = table.addIndex( INDEX_ID_SEARCH_SPEC );
	idx->addColumn( col );

	// search
	col = table.addColumn( COL_SEARCH_SEARCH_SPEC );
	col->setNotNull( true );
	col->setDataType( kome::db::TableColumn::INTEGER );

	idx = table.addIndex( INDEX_SEARCH_SEARCH_SPEC );
	idx->addColumn( col );

	// spectrum
	col = table.addColumn( COL_SPEC_SEARCH_SPEC );
	col->setNotNull( true );
	col->setDataType( kome::db::TableColumn::INTEGER );

	idx = table.addIndex( INDEX_SPEC_SEARCH_SPEC );
	idx->addColumn( col );

	// precursor
	col = table.addColumn( COL_PRECURSOR_SEARCH_SPEC );
	col->setDataType( kome::db::TableColumn::REAL );

	// peak list
	col = table.addColumn( COL_PEAKS_SEARCH_SPEC );
	col->setDataType( kome::db::TableColumn::INTEGER );

	// charge
	col = table.addColumn( COL_CHARGE_SEARCH_SPEC );
	col->setDataType( kome::db::TableColumn::INTEGER );
}

// create matrix table
void IdentificationManager::createMatrixTable( kome::db::Table& table ) {
	// name
	table.setName( TABLE_MATRIX );

	// ID
	kome::db::TableColumn* col = table.addColumn( COL_ID_MATRIX );
	col->setAutoIncrement( true );
	col->setNotNull( true );
	col->setDataType( kome::db::TableColumn::INTEGER );

	kome::db::TableIndex* idx = table.addIndex( INDEX_ID_MATRIX );
	idx->addColumn( col );

	// name
	col = table.addColumn( COL_NAME_MATRIX );
	col->setNotNull( true );
	col->setDataType( kome::db::TableColumn::VARCHAR );

	// comment
	col = table.addColumn( COL_COMMENT_MATRIX );
	col->setDataType( kome::db::TableColumn::VARCHAR );

	// rt flag
	col = table.addColumn( COL_RT_MATRIX );
	col->setNotNull( true );
	col->setDataType( kome::db::TableColumn::INTEGER );

	// m/z flag
	col = table.addColumn( COL_MZ_MATRIX );
	col->setNotNull( true );
	col->setDataType( kome::db::TableColumn::INTEGER );

	// charge flag
	col = table.addColumn( COL_CHARGE_MATRIX );
	col->setNotNull( true );
	col->setDataType( kome::db::TableColumn::INTEGER );

	// selected
	col = table.addColumn( COL_SELECTED_MATRIX );
	col->setNotNull( true );
	col->setDataType( kome::db::TableColumn::INTEGER );

	idx = table.addIndex( INDEX_SELECTED_MATRIX );
	idx->addColumn( col );
}

// create group table
void IdentificationManager::createGroupTable( kome::db::Table& table ) {
	// name
	table.setName( TABLE_GROUP );

	// ID
	kome::db::TableColumn* col = table.addColumn( COL_ID_GROUP );
	col->setAutoIncrement( true );
	col->setNotNull( true );
	col->setDataType( kome::db::TableColumn::INTEGER );

	kome::db::TableIndex* idx = table.addIndex( INDEX_ID_GROUP );
	idx->addColumn( col );

	// matrix
	col = table.addColumn( COL_MATRIX_GROUP );
	col->setNotNull( true );
	col->setDataType( kome::db::TableColumn::INTEGER );

	idx = table.addIndex( INDEX_MATRIX_GROUP );
	idx->addColumn( col );

	// name
	col = table.addColumn( COL_NAME_GROUP );
	col->setNotNull( true );
	col->setDataType( kome::db::TableColumn::VARCHAR );

	// color
	col = table.addColumn( COL_COLOR_GROUP );
	col->setNotNull( true );
	col->setDataType( kome::db::TableColumn::INTEGER );

	// order
	col = table.addColumn( COL_ORDER_GROUP );
	col->setNotNull( true );
	col->setDataType( kome::db::TableColumn::INTEGER );
}

// crate matrix sample table
void IdentificationManager::createMatrixSampleTable( kome::db::Table& table ) {
	// name
	table.setName( TABLE_MATRIX_SAMPLE );

	// ID
	kome::db::TableColumn* col = table.addColumn( COL_ID_MATRIX_SAMPLE );
	col->setAutoIncrement( true );
	col->setNotNull( true );
	col->setDataType( kome::db::TableColumn::INTEGER );

	kome::db::TableIndex* idx = table.addIndex( INDEX_ID_MATRIX_SAMPLE );
	idx->addColumn( col );

	// sample
	col = table.addColumn( COL_SAMPLE_MATRIX_SAMPLE );
	col->setNotNull( true );
	col->setDataType( kome::db::TableColumn::INTEGER );

	idx = table.addIndex( INDEX_SAMPLE_MATRIX_SAMPLE );
	idx->addColumn( col );

	kome::db::TableIndex* idx2 = table.addIndex( INDEX_GROUP_SAMPLE_MATRIX_SAMPLE );
	idx2->addColumn( col );

	// group
	col = table.addColumn( COL_GROUP_MATRIX_SAMPLE );
	col->setNotNull( true );
	col->setDataType( kome::db::TableColumn::INTEGER );

	idx = table.addIndex( INDEX_GROUP_MATRIX_SAMPLE );
	idx->addColumn( col );

	idx2->addColumn( col );

	// name
	col = table.addColumn( COL_NAME_MATRIX_SAMPLE );
	col->setNotNull( true );
	col->setDataType( kome::db::TableColumn::VARCHAR );

	// color
	col = table.addColumn( COL_COLOR_MATRIX_SAMPLE );
	col->setNotNull( true );
	col->setDataType( kome::db::TableColumn::INTEGER );
}

// create peak table
void IdentificationManager::createPeakTable( kome::db::Table& table ) {
	// name
	table.setName( TABLE_PEAK );

	// ID
	kome::db::TableColumn* col0 = table.addColumn( COL_ID_PEAK );
	col0->setAutoIncrement( true );
	col0->setNotNull( true );
	col0->setDataType( kome::db::TableColumn::INTEGER );

	kome::db::TableIndex* idx = table.addIndex( INDEX_ID_PEAK );
	idx->addColumn( col0 );

	// matrix
	kome::db::TableColumn* col1 = table.addColumn( COL_MATRIX_PEAK );
	col1->setNotNull( true );
	col1->setDataType( kome::db::TableColumn::INTEGER );

	idx = table.addIndex( INDEX_MATRIX_PEAK );
	idx->addColumn( col1 );

	// RT
	kome::db::TableColumn* col2 = table.addColumn( COL_RT_PEAK );
	col2->setDataType( kome::db::TableColumn::REAL );

	idx = table.addIndex( INDEX_RT_PEAK );
	idx->addColumn( col1 );
	idx->addColumn( col2 );

	// m/z
	kome::db::TableColumn* col3 = table.addColumn( COL_MZ_PEAK );
	col3->setDataType( kome::db::TableColumn::REAL );

	idx = table.addIndex( INDEX_MZ_PEAK );
	idx->addColumn( col1 );
	idx->addColumn( col3 );

	idx = table.addIndex( INDEX_RT_MZ_PEAK );
	idx->addColumn( col1 );
	idx->addColumn( col2 );
	idx->addColumn( col3 );

	// charge
	kome::db::TableColumn* col4 = table.addColumn( COL_CHARGE_PEAK );
	col4->setDataType( kome::db::TableColumn::INTEGER );
}

// create matrix column table
void IdentificationManager::createMatrixColumnTable( kome::db::Table& table ) {
	// name
	table.setName( TABLE_MATRIX_COL );

	// ID
	kome::db::TableColumn* col = table.addColumn( COL_ID_MATRIX_COL );
	col->setAutoIncrement( true );
	col->setNotNull( true );
	col->setDataType( kome::db::TableColumn::INTEGER );

	kome::db::TableIndex* idx = table.addIndex( INDEX_ID_MATRIX_COL );
	idx->addColumn( col );

	// matrix
	col = table.addColumn( COL_MATRIX_MATRIX_COL );
	col->setNotNull( true );
	col->setDataType( kome::db::TableColumn::INTEGER );

	idx = table.addIndex( INDEX_MATRIX_MATRIX_COL );
	idx->addColumn( col );

	// name
	col = table.addColumn( COL_NAME_MATRIX_COL );
	col->setNotNull( true );
	col->setDataType( kome::db::TableColumn::VARCHAR );

	// type
	col = table.addColumn( COL_TYPE_MATRIX_COL );
	col->setNotNull( true );
	col->setDataType( kome::db::TableColumn::INTEGER );
}

// create matrix value table
void IdentificationManager::createMatrixValueTable( kome::db::Table& table ) {
	// name
	table.setName( TABLE_MATRIX_VAL );

	// ID
	kome::db::TableColumn* col = table.addColumn( COL_ID_MATRIX_VAL );
	col->setAutoIncrement( true );
	col->setNotNull( true );
	col->setDataType( kome::db::TableColumn::INTEGER );

	kome::db::TableIndex* idx = table.addIndex( INDEX_ID_MATRIX_VAL );
	idx->addColumn( col );

	// column
	col = table.addColumn( COL_COL_MATRIX_VAL );
	col->setNotNull( true );
	col->setDataType( kome::db::TableColumn::INTEGER );

	idx = table.addIndex( INDEX_COL_MATRIX_VAL );
	idx->addColumn( col );

	// peak
	col = table.addColumn( COL_PEAK_MATRIX_VAL );
	col->setNotNull( true );
	col->setDataType( kome::db::TableColumn::INTEGER );

	idx->addColumn( col );

	// value
	col = table.addColumn( COL_VALUE_MATRIX_VAL );
	col->setNotNull( true );
	col->setDataType( kome::db::TableColumn::VARCHAR );
}

// create matrix spectrum table
void IdentificationManager::createMatrixSpectrumTable( kome::db::Table& table ) {
	// name
	table.setName( TABLE_MATRIX_SPEC );

	// ID
	kome::db::TableColumn* col = table.addColumn( COL_ID_MATRIX_SPEC );
	col->setAutoIncrement( true );
	col->setNotNull( true );
	col->setDataType( kome::db::TableColumn::INTEGER );

    kome::db::TableIndex* idx = table.addIndex( INDEX_ID_MATRIX_SPEC );
	idx->addColumn( col );

	// peak
	col = table.addColumn( COL_PEAK_MATRIX_SPEC );
	col->setNotNull( true );
	col->setDataType( kome::db::TableColumn::INTEGER );	

	idx = table.addIndex( INDEX_PEAK_MATRIX_SPEC );
	idx->addColumn( col );

	idx = table.addIndex ( INDEX_PEAK_SPECTRUM_MATRIX_SPEC );
	idx->addColumn( col );

	// spectrum
	col = table.addColumn( COL_SPEC_MATRIX_SPEC );
	col->setNotNull( true );
	col->setDataType( kome::db::TableColumn::INTEGER );	

	idx->addColumn( col );

	idx = table.addIndex( INDEX_SPECTRUM_MATRIX_SPEC );
	idx->addColumn( col );
}

// create profile table
void IdentificationManager::createProfileTable( kome::db::Table& table ) {
	// name
	table.setName( TABLE_PROFILE );

	// ID
	kome::db::TableColumn* col = table.addColumn( COL_ID_PROFILE );
	col->setAutoIncrement( true );
	col->setNotNull( true );
	col->setDataType( kome::db::TableColumn::INTEGER );

	kome::db::TableIndex* idx = table.addIndex( INDEX_ID_PROFILE );
	idx->addColumn( col );

	// chromatogram
	col = table.addColumn( COL_CHROM_PROFILE );
	col->setNotNull( true );
	col->setDataType( kome::db::TableColumn::INTEGER );

	// data
	col = table.addColumn( COL_DATA_PROFILE );
	col->setNotNull( true );
	col->setDataType( kome::db::TableColumn::BINARY );
}

// create peak value table
void IdentificationManager::createPeakValueTable( kome::db::Table& table ) {
	// name
	table.setName( TABLE_PEAK_VAL );

	// ID
	kome::db::TableColumn* col = table.addColumn( COL_ID_PEAK_VAL );
	col->setAutoIncrement( true );
	col->setNotNull( true );
	col->setDataType( kome::db::TableColumn::INTEGER );

	kome::db::TableIndex* idx = table.addIndex( INDEX_ID_PEAK_VAL );
	idx->addColumn( col );

	// peak
	col = table.addColumn( COL_PEAK_PEAK_VAL );
	col->setNotNull( true );
	col->setDataType( kome::db::TableColumn::INTEGER );

	idx = table.addIndex( INDEX_PEAK_PEAK_VAL );
	idx->addColumn( col );

	idx = table.addIndex( INDEX_PEAK_SAMPLE_PEAK_VAL );
	idx->addColumn( col );

	// sample
	col = table.addColumn( COL_SAMPLE_PEAK_VAL );
	col->setNotNull( true );
	col->setDataType( kome::db::TableColumn::INTEGER );

	idx->addColumn( col );

	idx = table.addIndex( INDEX_SAMPLE_PEAK_VAL );
	idx->addColumn( col );

	// value
	col = table.addColumn( COL_VALUE_PEAK_VAL );
	col->setDataType( kome::db::TableColumn::REAL );

	// normalized value
	col = table.addColumn( COL_NORMALIZED_PEAK_VAL );
	col->setDataType( kome::db::TableColumn::REAL );

	// position
	col = table.addColumn( COL_POS_PEAK_VAL );
	col->setDataType( kome::db::TableColumn::REAL );

	// left x
	col = table.addColumn( COL_LX_PEAK_VAL );
	col->setDataType( kome::db::TableColumn::REAL );

	// left y
	col = table.addColumn( COL_LY_PEAK_VAL );
	col->setDataType( kome::db::TableColumn::REAL );

	// right x
	col = table.addColumn( COL_RX_PEAK_VAL );
	col->setDataType( kome::db::TableColumn::REAL );

	// right y
	col = table.addColumn( COL_RY_PEAK_VAL );
	col->setDataType( kome::db::TableColumn::REAL );

	// profile
	col = table.addColumn( COL_PROFILE_PEAK_VAL );
	col->setDataType( kome::db::TableColumn::INTEGER );
}

// create accession table
void IdentificationManager::createAccessionTable( kome::db::Table& table ) {
	// name
	table.setName( TABLE_ACCESSION );

	// ID
	kome::db::TableColumn* col = table.addColumn( COL_ID_ACCESSION );
	col->setAutoIncrement( true );
	col->setNotNull( true );
	col->setDataType( kome::db::TableColumn::INTEGER );

	kome::db::TableIndex* idx = table.addIndex( INDEX_ID_ACCESSION );
	idx->addColumn( col );

	// prefix 
	col = table.addColumn( COL_PREFIX_ACCESSION );
	col->setNotNull( true );
	col->setDataType( kome::db::TableColumn::VARCHAR );

	idx = table.addIndex( INDEX_PREFIX_ACCESSION );
	idx->addColumn( col );

	// count
	col = table.addColumn( COL_COUNT_ACCESSION );
	col->setNotNull( true );
	col->setDataType( kome::db::TableColumn::INTEGER );
}

// create standard sample table
void IdentificationManager::createStandardTable( kome::db::Table& table ) {
	// name
	table.setName( TABLE_STANDARD_SAMPLE );

	// ID
	kome::db::TableColumn* col = table.addColumn( COL_ID_STANDARD );
	col->setAutoIncrement( true );
	col->setNotNull( true );
	col->setDataType( kome::db::TableColumn::INTEGER );

	// matrix
	col = table.addColumn( COL_MATRIX_STANDARD );
	col->setNotNull( true );
	col->setDataType( kome::db::TableColumn::INTEGER );

	kome::db::TableIndex* idx = table.addIndex( INDEX_MATRIX_STANDARD );
	idx->addColumn( col );

	// sample
	col = table.addColumn( COL_SAMPLE_STANDARD );
	col->setNotNull( true );
	col->setDataType( kome::db::TableColumn::INTEGER );
}

// create normalization filter table
void IdentificationManager::createNormalizationFilterTable( kome::db::Table& table ) {
	// name
	table.setName( TABLE_NORM_FILTER );

	// ID
	kome::db::TableColumn* col = table.addColumn( COL_ID_NORM_FILTER );
	col->setAutoIncrement( true );
	col->setNotNull( true );
	col->setDataType( kome::db::TableColumn::INTEGER );

	kome::db::TableIndex* idx = table.addIndex( INDEX_ID_NORM_FILTER );
	idx->addColumn( col );

	// matrix
	col = table.addColumn( COL_MATRIX_NORM_FILTER );
	col->setNotNull( true );
	col->setDataType( kome::db::TableColumn::INTEGER );

	idx = table.addIndex( INDEX_MATRIX_NORM_FILTER );
	idx->addColumn( col );

	// min intensity
	col = table.addColumn( COL_MIN_INT_NORM_FILTER );
	col->setDataType( kome::db::TableColumn::REAL );

	// max intensity
	col = table.addColumn( COL_MAX_INT_NORM_FILTER );
	col->setDataType( kome::db::TableColumn::REAL );

	// relative flag
	col = table.addColumn( COL_REL_FLG_NORM_FILTER );
	col->setDataType( kome::db::TableColumn::INTEGER );

	// min RT
	col = table.addColumn( COL_MIN_RT_NORM_FILTER );
	col->setDataType( kome::db::TableColumn::REAL );

	// max RT
	col = table.addColumn( COL_MAX_RT_NORM_FILTER );
	col->setDataType( kome::db::TableColumn::REAL );

	// min m/z
	col = table.addColumn( COL_MIN_MZ_NORM_FILTER );
	col->setDataType( kome::db::TableColumn::REAL );

	// max m/z
	col = table.addColumn( COL_MAX_MZ_NORM_FILTER );
	col->setDataType( kome::db::TableColumn::REAL );

	// method
	col = table.addColumn( COL_METHOD_NORM_FILTER );
	col->setDataType( kome::db::TableColumn::VARCHAR );
	col->setNotNull( true );

	// parameters
	col = table.addColumn( COL_PARAMS_NORM_FILTER );
	col->setDataType( kome::db::TableColumn::VARCHAR );
}

// create normalization method filter
void IdentificationManager::createNormalizationMethodTable( kome::db::Table& table ) {
	// name
	table.setName( TABLE_NORM_METHOD );

	// ID
	kome::db::TableColumn* col = table.addColumn( COL_ID_NORM_METHOD );
	col->setAutoIncrement( true );
	col->setNotNull( true );
	col->setDataType( kome::db::TableColumn::INTEGER );

	kome::db::TableIndex* idx = table.addIndex( INDEX_ID_NORM_METHOD );
	idx->addColumn( col );

	// filter
	col = table.addColumn( COL_FILTER_NORM_METHOD );
	col->setNotNull( true );
	col->setDataType( kome::db::TableColumn::INTEGER );

	idx = table.addIndex( INDEX_FILTER_NORM_METHOD );
	idx->addColumn( col );

	// order
	col = table.addColumn( COL_ORDER_NORM_METHOD );
	col->setNotNull(true );
	col->setDataType( kome::db::TableColumn::INTEGER );

	// method
	col = table.addColumn( COL_METHOD_NORM_METHOD );
	col->setNotNull( true );
	col->setDataType( kome::db::TableColumn::VARCHAR );

	// parameters
	col = table.addColumn( COL_PARAMS_NORM_METHOD );
	col->setDataType( kome::db::TableColumn::VARCHAR );
}

// create RT alignment table
void IdentificationManager::createRtAlignmentTable( kome::db::Table& table ) {
	// name
	table.setName( TABLE_ALIGN );

	// ID
	kome::db::TableColumn* col = table.addColumn( COL_ID_ALIGN );
	col->setAutoIncrement( true );
	col->setNotNull( true );
	col->setDataType( kome::db::TableColumn::INTEGER );

	kome::db::TableIndex* idx = table.addIndex( INDEX_ID_ALIGN );
	idx->addColumn( col );

	// matrix
	col = table.addColumn( COL_MATRIX_ALIGN );
	col->setNotNull( true );
	col->setDataType( kome::db::TableColumn::INTEGER );

	idx = table.addIndex( INDEX_MATRIX_ALIGN );
	idx->addColumn( col );

	// method
	col = table.addColumn( COL_METHOD_ALIGN );
	col->setNotNull( true );
	col->setDataType( kome::db::TableColumn::VARCHAR );

	// parameters
	col = table.addColumn( COL_PARAMS_ALIGN );
	col->setDataType( kome::db::TableColumn::VARCHAR );
}

// create alternative substance table
void IdentificationManager::createAltSubstanceTable( kome::db::Table& table ) {
	// name
	table.setName( TABLE_ALT_SUBSTANCE );

	// ID
	kome::db::TableColumn* col = table.addColumn( COL_ID_ALT_SUBSTANCE );
	col->setAutoIncrement( true );
	col->setNotNull( true );
	col->setDataType( kome::db::TableColumn::INTEGER );

	// hit
	col = table.addColumn( COL_HIT_ALT_SUBSTANCE );
	col->setNotNull( true );
	col->setDataType( kome::db::TableColumn::INTEGER );

	kome::db::TableIndex* idx = table.addIndex( INDEX_HIT_ALT_SUBSTANCE );
	idx->addColumn( col );

	// substance
	col = table.addColumn( COL_SUBSTANCE_ALT_SUBSTANCE );
	col->setNotNull( true );
	col->setDataType( kome::db::TableColumn::INTEGER );
	idx = table.addIndex( INDEX_SUBSTANCE_ALT_SUBSTANCE );
	idx->addColumn( col );

	// start position
	col = table.addColumn( COL_START_ALT_SUBSTANCE );
	col->setDataType( kome::db::TableColumn::INTEGER );

	// end position
	col = table.addColumn( COL_END_ALT_SUBSTANCE );
	col->setDataType( kome::db::TableColumn::INTEGER );

	// before sequence
	col = table.addColumn( COL_BEFORE_ALT_SUBSTANCE );
	col->setDataType( kome::db::TableColumn::VARCHAR );

	// before sequence
	col = table.addColumn( COL_AFTER_ALT_SUBSTANCE );
	col->setDataType( kome::db::TableColumn::VARCHAR );
}

// create peak list table
void IdentificationManager::createPeakListTable( kome::db::Table& table ) {
	// name
	table.setName( TABLE_PEAK_LIST );

	// ID
	kome::db::TableColumn* col = table.addColumn( COL_ID_PEAK_LIST );
	col->setDataType( kome::db::TableColumn::INTEGER );
	col->setAutoIncrement( true );
	col->setNotNull( true );

	kome::db::TableIndex* idx = table.addIndex( INDEX_ID_PEAK_LIST );
	idx->addColumn( col );

	// peaks
	col = table.addColumn( COL_PEAKS_PEAK_LIST );
	col->setDataType( kome::db::TableColumn::BINARY );

	// peak detection
	col = table.addColumn( COL_DETECTION_PEAK_LIST );
	col->setDataType( kome::db::TableColumn::INTEGER );
}

// create modification table
void IdentificationManager::createModTable( kome::db::Table& table ) {
	// name
	table.setName( TABLE_MOD );

	// ID
	kome::db::TableColumn* col = table.addColumn( COL_ID_MOD );
	col->setDataType( kome::db::TableColumn::INTEGER );
	col->setAutoIncrement( true );
	col->setNotNull( true );

	kome::db::TableIndex* index = table.addIndex( INDEX_ID_MOD );
	index->addColumn( col );

	// name
	col = table.addColumn( COL_NAME_MOD );
	col->setDataType( kome::db::TableColumn::VARCHAR );
	col->setNotNull( true );

	index = table.addIndex( INDEX_NAME_MOD );
	index->addColumn( col );

	// type
	col = table.addColumn( COL_TYPE_MOD );
	col->setDataType( kome::db::TableColumn::VARCHAR );
	col->setNotNull( true );

	// mass
	col = table.addColumn( COL_MASS_MOD );
	col->setDataType( kome::db::TableColumn::REAL );
	col->setNotNull( true );

	// hidden
	col = table.addColumn( COL_HIDDEN_MOD );
	col->setDataType( kome::db::TableColumn::INTEGER );
}

// create search modification table
void IdentificationManager::createSearchModTable( kome::db::Table& table ) {
	// name
	table.setName( TABLE_SEARCH_MOD );

	// ID
	kome::db::TableColumn* col = table.addColumn( COL_ID_SEARCH_MOD );
	col->setDataType( kome::db::TableColumn::INTEGER );
	col->setAutoIncrement( true );
	col->setNotNull( true );

	kome::db::TableIndex* idx = table.addIndex( INDEX_ID_SEARCH_MOD );
	idx->addColumn( col );

	// search
	col = table.addColumn( COL_SEARCH_SEARCH_MOD );
	col->setDataType( kome::db::TableColumn::INTEGER );
	col->setNotNull( true );

	idx = table.addIndex( INDEX_SEARCH_SEARCH_MOD );
	idx->addColumn( col );

	// modification
	col = table.addColumn( COL_MOD_SEARCH_MOD );
	col->setDataType( kome::db::TableColumn::INTEGER );
	col->setNotNull( true );

	// variable
	col = table.addColumn( COL_VARIABLE_SEARCH_MOD );
	col->setDataType( kome::db::TableColumn::INTEGER );
}

// create hit modification table
void IdentificationManager::createHitModTable( kome::db::Table& table ) {
	// name
	table.setName( TABLE_HIT_MOD );

	// ID
	kome::db::TableColumn* col = table.addColumn( COL_ID_HIT_MOD );
	col->setDataType( kome::db::TableColumn::INTEGER );
	col->setAutoIncrement( true );
	col->setNotNull( true );

	kome::db::TableIndex* idx = table.addIndex( INDEX_ID_HIT_MOD );
	idx->addColumn( col );

	// hit
	col = table.addColumn( COL_HIT_HIT_MOD );
	col->setDataType( kome::db::TableColumn::INTEGER );
	col->setNotNull( true );

	idx = table.addIndex( INDEX_HIT_HIT_MOD );
	idx->addColumn( col );

	//  search modification
	col = table.addColumn( COL_MOD_HIT_MOD );
	col->setDataType( kome::db::TableColumn::INTEGER );
	col->setNotNull( true );

	// position
	col = table.addColumn( COL_POS_HIT_MOD );
	col->setDataType( kome::db::TableColumn::INTEGER );
	col->setNotNull( true );
}

// create enzyme table
void IdentificationManager::createEnzymeTable( kome::db::Table& table ) {
	// name
	table.setName( TABLE_ENZYME );

	// ID
	kome::db::TableColumn* col = table.addColumn( COL_ID_ENZYME );
	col->setDataType( kome::db::TableColumn::INTEGER );
	col->setAutoIncrement( true );
	col->setNotNull( true );

	kome::db::TableIndex* idx = table.addIndex( INDEX_ID_ENZYME );
	idx->addColumn( col );

	// name
	col = table.addColumn( COL_NAME_ENZYME );
	col->setDataType( kome::db::TableColumn::VARCHAR );
	col->setNotNull( true );

	idx = table.addIndex( INDEX_NAME_ENZYME );
	idx->addColumn( col );

	// hidden
	col = table.addColumn( COL_HIDDEN_ENZYME );
	col->setDataType( kome::db::TableColumn::INTEGER );
}

// create enzyme cutter table
void IdentificationManager::createEnzymeCutterTable( kome::db::Table& table ) {
	// name
	table.setName( TABLE_ENZYME_CUTTER );

	// ID
	kome::db::TableColumn* col = table.addColumn( COL_ID_ENZYME_CUTTER );
	col->setDataType( kome::db::TableColumn::INTEGER );
	col->setAutoIncrement( true );
	col->setNotNull( true );

	kome::db::TableIndex* idx = table.addIndex( INDEX_ID_ENZYME_CUTTER );
	idx->addColumn( col );

	// enzyme
	col = table.addColumn( COL_ENZYME_ENZYME_CUTTER );
	col->setDataType( kome::db::TableColumn::INTEGER );
	col->setNotNull( true );

	idx = table.addIndex( INDEX_ENZYME_ENZYME_CUTTER );
	idx->addColumn( col );

	// term
	col = table.addColumn( COL_TERM_ENZYME_CUTTER );
	col->setDataType( kome::db::TableColumn::VARCHAR );
	col->setNotNull( true );

	// cleave
	col = table.addColumn( COL_CLEAVE_ENZYME_CUTTER );
	col->setDataType( kome::db::TableColumn::VARCHAR );

	// restrict
	col = table.addColumn( COL_RESTRICT_ENZYME_CUTTER );
	col->setDataType( kome::db::TableColumn::VARCHAR );
}

// add matrix
int IdentificationManager::addMatrix(
		const char* name, const char* comment, const bool rtFlg, const bool mzFlg, const bool chargeFlg
) {
	// DB
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return -1;
	}

	// selected matrix
	int selected = getSelectedMatrix();

	// add
	int id = -1;

	std::string sql = FMT( 
		"insert into %s (%s, %s, %s, %s, %s, %s) values(?, ?, ?, ?, ?, ?);",
		TABLE_MATRIX,
		COL_NAME_MATRIX,
		COL_COMMENT_MATRIX,
		COL_RT_MATRIX,
		COL_MZ_MATRIX,
		COL_CHARGE_MATRIX,
		COL_SELECTED_MATRIX
	);

	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	bool ret = false;
	if( stmt != NULL ) {
		stmt->bindString( 1, NVL( name, "" ) );
		stmt->bindString( 2, NVL( comment, "" ) );
		stmt->bindInteger( 3, ( rtFlg ? 1 : 0 ) );
		stmt->bindInteger( 4, ( mzFlg ? 1 : 0 ) );
		stmt->bindInteger( 5, ( chargeFlg ? 1 : 0 ) );
		stmt->bindInteger( 6, ( selected < 0 ? 1 : 0 ) );

		ret = stmt->execute();

		stmt->close();
	}

	// ID
	id = db->getMaxValue( TABLE_MATRIX, COL_ID_MATRIX );
	if( id < 0 ) {
		id = -1;
	}

	return id;
}

// delete matrix
void IdentificationManager::deleteMatrix( const int id ) {
	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return;
	}

	// delete groups
	std::vector< int > groups;
	getGroups( groups, id );
	for( unsigned int i = 0; i < groups.size(); i++ ) {
		deleteGroup( groups[ i ] );
	}

	// delete columns
	std::vector< int > columns;
	getMatrixColumns( columns, id );
	for( unsigned int i = 0; i < columns.size(); i++ ) {
		deleteMatrixColumn( columns[ i ] );
	}

	// delete peaks
	std::vector< int > peaks;
	getMatrixPeaks( peaks, id );
	for( unsigned int i = 0; i < peaks.size(); i++ ) {
		deleteMatrixPeak( peaks[ i ] );
	}

	// delete
	std::string sql = FMT( "delete from %s where %s = ?;", TABLE_MATRIX, COL_ID_MATRIX );

	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindInteger( 1, id );
		stmt->execute();
		stmt->close();
	}
}

// get selected matrix
int IdentificationManager::getSelectedMatrix() {
	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return -1;
	}

	// get the selected matrix
	int id = -1;

	std::string sql = FMT(
		"select %s from %s where %s > 0;",
		COL_ID_MATRIX,
		TABLE_MATRIX,
		COL_SELECTED_MATRIX
	);

	kome::db::ResultSet* rs = db->executeQuery( sql.c_str() );
	if( rs != NULL ) {
		if( rs->next() ) {
			id = rs->getInt( 0, -1 );
		}
		rs->close();
	}

	return id;
}

// select matrix
void IdentificationManager::selectMatrix( const int id ) {
	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return;
	}

	// select
	std::string sql = FMT( "update %s set %s = ?;", TABLE_MATRIX, COL_SELECTED_MATRIX );
	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindInteger( 1, 0 );
		
		if( !stmt->execute() ) {
			db->rollback();
			stmt->close();
			return;
		}
		stmt->close();
	}

	sql = FMT( "update %s set %s = ? where %s = ?;", TABLE_MATRIX, COL_SELECTED_MATRIX, COL_ID_MATRIX );
	stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindInteger( 1, 1 );
		stmt->bindInteger( 2, id );

		stmt->execute();
		stmt->close();
	}
}

// update matrix flags
void IdentificationManager::updateMatrixFlags( const int id, const bool rtFlg, const bool mzFlg, const bool chargeFlg ) {
	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return;
	}

	std::string sql = FMT(
		"update %s set %s = ?, %s = ?, %s = ? where %s = ?;",
		TABLE_MATRIX,
		COL_RT_MATRIX,
		COL_MZ_MATRIX,
		COL_CHARGE_MATRIX,
		COL_ID_MATRIX
	);

	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindInteger( 1, ( rtFlg ? 1 : 0 ) );
		stmt->bindInteger( 2, ( mzFlg ? 1 : 0 ) );
		stmt->bindInteger( 3, ( chargeFlg ? 1: 0 ) );
		stmt->bindInteger( 4, id );

		stmt->execute();
		stmt->close();
	}
}

// update matrix
void IdentificationManager::updateMatrix( const int id, const char* name, const char* comment ) {
	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return;
	}

	std::string sql = FMT(
		"update %s set %s = ?, %s = ? where %s = ?;",
		TABLE_MATRIX,
		COL_NAME_MATRIX,
		COL_COMMENT_MATRIX,
		COL_ID_MATRIX
	);

	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindString( 1, NVL( name, "" ) );
		stmt->bindString( 2, NVL( comment, "" ) );
		stmt->bindInteger( 3, id );

		stmt->execute();
		stmt->close();
	}
}

// get matrices
void IdentificationManager::getMatrices( std::vector< int >& matrices ) {
	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return;
	}

	// matrices
	std::string sql = FMT( "select %s from %s;", COL_ID_MATRIX, TABLE_MATRIX );
	kome::db::ResultSet* rs = db->executeQuery( sql.c_str() );
	if( rs != NULL ) {
		while( rs->next() ) {
			int id = rs->getInt( 0, -1 );
			if( id >= 0 ) {
				matrices.push_back( id );
			}
		}
		rs->close();
	}
}

// get matrix name
std::string IdentificationManager::getMatrixName( const int id ) {
	// string object
	std::string s;

	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return s;
	}

	// name
	std::string sql = FMT(
		"select %s from %s where %s = ?;",
		COL_NAME_MATRIX,
		TABLE_MATRIX,
		COL_ID_MATRIX
	);

	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindInteger( 1, id );

		kome::db::ResultSet* rs = stmt->executeQuery();
		if( rs != NULL ) {
			if( rs->next() ) {
				s = rs->getString( 0, "" );
			}
			rs->close();
		}
		stmt->close();
	}

	return s;
}

// get matrix comment
std::string IdentificationManager::getMatrixComment( const int id ) {
	// string object
	std::string s;

	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return s;
	}

	// name
	std::string sql = FMT(
		"select %s from %s where %s = ?;",
		COL_COMMENT_MATRIX,
		TABLE_MATRIX,
		COL_ID_MATRIX
	);

	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindInteger( 1, id );

		kome::db::ResultSet* rs = stmt->executeQuery();
		if( rs != NULL ) {
			if( rs->next() ) {
				s = rs->getString( 0, "" );
			}
			rs->close();
		}
		stmt->close();
	}

	return s;
}

// get matrix flags
void IdentificationManager::getMatrixFlags( const int id, bool* rtFlg, bool* mzFlg, bool* chargeFlg ) {
	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return;
	}

	// name
	std::string sql = FMT(
		"select %s, %s, %s from %s where %s = ?;",
		COL_RT_MATRIX,
		COL_MZ_MATRIX,
		COL_CHARGE_MATRIX,
		TABLE_MATRIX,
		COL_ID_MATRIX
	);

	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindInteger( 1, id );

		kome::db::ResultSet* rs = stmt->executeQuery();
		if( rs != NULL ) {
			if( rs->next() ) {
				if( rtFlg != NULL ) {
					*rtFlg = ( rs->getInt( 0, 1 ) > 0 );
				}
				if( mzFlg != NULL ) {
					*mzFlg = ( rs->getInt( 1, 1 ) > 0 );
				}
				if( chargeFlg != NULL ) {
					*chargeFlg = ( rs->getInt( 2, 0 ) > 0 );
				}
			}
			rs->close();
		}
		stmt->close();
	}
}

// add group
int IdentificationManager::addGroup( const char* name, const int matrix, const COLOR color ) {
	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return -1;
	}

	// order
	int order = 0;
	std::string sql = FMT(
		"select max(%s) from %s where %s = ?;",
		COL_ORDER_GROUP,
		TABLE_GROUP,
		COL_MATRIX_GROUP
	);

	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindInteger( 1, matrix );
		kome::db::ResultSet* rs = stmt->executeQuery();
		if( rs != NULL ) {
			if( rs->next() ) {
				order = rs->getInt( 0, -1 ) + 1;
			}
			rs->close();
		}
		stmt->close();
	}

	// insert
	sql = FMT(
		"insert into %s (%s, %s, %s, %s) values (?, ?, ?, ?);",
		TABLE_GROUP,
		COL_NAME_GROUP,
		COL_MATRIX_GROUP,
		COL_COLOR_GROUP,
		COL_ORDER_GROUP
	);

	stmt = db->prepare( sql.c_str() );
	bool ret = false;
	if( stmt != NULL ) {
		stmt->bindString( 1, NVL( name, "" ) );
		stmt->bindInteger( 2, matrix );
		stmt->bindInteger( 3, (int)color );
		stmt->bindInteger( 4, order );

		ret = stmt->execute();
		stmt->close();
	}

	// id
	int id = -1;

	if( ret ) {
		id = db->getMaxValue( TABLE_GROUP, COL_ID_GROUP );
		if( id < 0 ) {
			id = -1;
		}
	}

	return id;
}

// delete group
void IdentificationManager::deleteGroup( const int id ) {
	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return;
	}

	// delete matrix samples
	std::vector< int > samples;
	getMatrixSamplesFromGroup( samples, id );
	for( unsigned int i = 0; i < samples.size(); i++ ) {
		deleteMatrixSample( samples[ i ] );
	}

	// delete
	std::string sql = FMT( "delete from %s where %s = ?;", TABLE_GROUP, COL_ID_GROUP );
	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindInteger( 1, id );

		stmt->execute();
		stmt->close();
	}
}

// update group
void IdentificationManager::updateGroup( const int id, const char* name, const COLOR color ) {
	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return;
	}

	// update
	std::string sql = FMT(
		"update %s set %s = ?, %s = ? where %s = ?;",
		TABLE_GROUP,
		COL_NAME_GROUP,
		COL_COLOR_GROUP,
		COL_ID_GROUP
	);

	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindString( 1, NVL( name, "" ) );
		stmt->bindInteger( 2, (int)color );
		stmt->bindInteger( 3, id );

		stmt->execute();
		stmt->close();
	}
}

// set group order
void IdentificationManager::setGroupOrder( const int id, const int order ) {
	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return;
	}

	// update
	std::string sql = FMT(
		"update %s set %s = ? where %s = ?;",
		TABLE_GROUP,
		COL_ORDER_GROUP,
		COL_ID_GROUP
	);

	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindInteger( 1, order );
		stmt->bindInteger( 2, id );

		stmt->execute();
		stmt->close();
	}
}

// get groups
void IdentificationManager::getGroups( std::vector< int >& groups, const int matrixId ) {
	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return;
	}

	// groups
	std::string sql = FMT(
		"select %s from %s where %s = ? order by %s;",
		COL_ID_GROUP,
		TABLE_GROUP,
		COL_MATRIX_GROUP,
		COL_ORDER_GROUP
	);

	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindInteger( 1, matrixId );

		kome::db::ResultSet* rs = stmt->executeQuery();
		if( rs != NULL ) {
			while( rs->next() ) {
				int group = rs->getInt( 0, -1 ); 
				if( group >= 0 ) {
					groups.push_back( group );
				}
			}
			rs->close();
		}
		stmt->close();
	}
}

// get group matrix
int IdentificationManager::getGroupMatrix( const int id ) {
	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return -1;
	}

	// name
	int matrix = -1;
	std::string sql = FMT(
		"select %s from %s where %s = ?;",
		COL_MATRIX_GROUP,
		TABLE_GROUP,
		COL_ID_GROUP
	);

	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindInteger( 1, id );
		kome::db::ResultSet* rs = stmt->executeQuery();
		if( rs != NULL ) {
			if( rs->next() ) {
				matrix = rs->getInt( 0, -1 );
			}
			rs->close();
		}
		stmt->close();
	}

	return matrix;
}

// get group name
std::string IdentificationManager::getGroupName( const int id ) {
	// string
	std::string s;

	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return s;
	}

	// name
	std::string sql = FMT(
		"select %s from %s where %s = ?;",
		COL_NAME_GROUP,
		TABLE_GROUP,
		COL_ID_GROUP
	);

	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindInteger( 1, id );
		kome::db::ResultSet* rs = stmt->executeQuery();
		if( rs != NULL ) {
			if( rs->next() ) {
				s = rs->getString( 0, "" );
			}
			rs->close();
		}
		stmt->close();
	}

	return s;
}

// get group color
COLOR IdentificationManager::getGroupColor( const int id ) {
	// color 
	COLOR c = -1;

	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return c;
	}

	// name
	std::string sql = FMT(
		"select %s from %s where %s = ?;",
		COL_COLOR_GROUP,
		TABLE_GROUP,
		COL_ID_GROUP
	);

	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindInteger( 1, id );
		kome::db::ResultSet* rs = stmt->executeQuery();
		if( rs != NULL ) {
			if( rs->next() ) {
				c = (COLOR)rs->getInt( 0, -1 );
			}
			rs->close();
		}
		stmt->close();
	}

	return c;
}

// set standard sample
void IdentificationManager::setStandardSample( const int matrixId, const int matrixSampleId ) {
	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return;
	}

	// ID
	int id = -1;

	// get
	std::string sql = FMT(
		"select %s from %s where %s = ?;",
		COL_ID_STANDARD,
		TABLE_STANDARD_SAMPLE,
		COL_MATRIX_STANDARD
	);

	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindInteger( 1, matrixId );

		kome::db::ResultSet* rs = stmt->executeQuery();
		if( rs != NULL ) {
			if( rs->next() ) {
				id = rs->getInt( 0, -1 );
			}
			rs->close();
		}
		stmt->close();
	}

	// set the standard sample
	if( id < 0 ) {    // insert
		sql = FMT(
			"insert into %s (%s, %s) values(?, ?);",
			TABLE_STANDARD_SAMPLE,
			COL_MATRIX_STANDARD,
			COL_SAMPLE_STANDARD
		);

		stmt = db->prepare( sql.c_str() );
		if( stmt != NULL ) {
			stmt->bindInteger( 1, matrixId );
			stmt->bindInteger( 2, matrixSampleId );

			stmt->execute();
			stmt->close();
		}
	}
	else {    // update
		sql = FMT(
			"update %s set %s = ? where %s = ?;",
			TABLE_STANDARD_SAMPLE,
			COL_SAMPLE_STANDARD,
			COL_ID_STANDARD
		);

		stmt = db->prepare( sql.c_str() );
		if( stmt != NULL ) {
			stmt->bindInteger( 1, matrixSampleId );
			stmt->bindInteger( 2, id );

			stmt->execute();
			stmt->close();
		}
	}
}

// clears normalization 
void IdentificationManager::clearNormalization( const int matrix ) {
	// database
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return;
	}

	// SQL
	std::string sql;

	// get filter ID
	int filterId = getNormalizationFilterId( matrix );
	if( filterId >= 0 ) {
		// delete methods
		sql = FMT(
			"delete from %s where %s = ?;",
			TABLE_NORM_METHOD,
			COL_FILTER_NORM_METHOD
		);

		kome::db::Statement* stmt = db->prepare( sql.c_str() );
		if( stmt != NULL ) {
			stmt->bindInteger( 1, filterId );
			stmt->execute();
			stmt->close();
		}
	}

	// delete filter
	sql = FMT(
		"delete from %s where %s = ?;",
		TABLE_NORM_FILTER,
		COL_MATRIX_NORM_FILTER
	);

	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindInteger( 1, matrix );
		stmt->execute();
		stmt->close();
	}
}

// set normalization filter
void IdentificationManager::setNormalizationFilter(
		const int matrix,
		const double minInt,
		const double maxInt,
		const bool relFlg,
		const double minRt,
		const double maxRt,
		const double minMz,
		const double maxMz,
		const char* method,
		const char* params
) {
	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return;
	}

	// ID
	int id = -1;
	std::string sql = FMT(
		"select %s from %s where %s = ?;",
		COL_ID_NORM_FILTER,
		TABLE_NORM_FILTER,
		COL_MATRIX_NORM_FILTER
	);

	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindInteger( 1, matrix );
		kome::db::ResultSet* rs = stmt->executeQuery();
		if( rs != NULL ) {
			if( rs->next() ) {
				id = rs->getInt( 0, -1 );
			}
			rs->close();
		}
		stmt->close();
	}

	// set filter
	if( id < 0 ) {    // add
		sql = FMT(
			"insert into %s (%s, %s, %s, %s, %s, %s, %s, %s, %s, %s) values(?, ?, ?, ?, ?, ?, ?, ?, ?, ?);",
			TABLE_NORM_FILTER,
			COL_MIN_INT_NORM_FILTER,
			COL_MAX_INT_NORM_FILTER,
			COL_REL_FLG_NORM_FILTER,
			COL_MIN_RT_NORM_FILTER,
			COL_MAX_RT_NORM_FILTER,
			COL_MIN_MZ_NORM_FILTER,
			COL_MAX_MZ_NORM_FILTER,
			COL_METHOD_NORM_FILTER,
			COL_PARAMS_NORM_FILTER,
			COL_MATRIX_NORM_FILTER
		);
	}
	else {
		sql = FMT(
			"update %s set %s = ?, %s = ?, %s = ?, %s = ?, %s = ?, %s = ?, %s = ?, %s = ?, %s = ? where %s = ?;",
			TABLE_NORM_FILTER,
			COL_MIN_INT_NORM_FILTER,
			COL_MAX_INT_NORM_FILTER,
			COL_REL_FLG_NORM_FILTER,
			COL_MIN_RT_NORM_FILTER,
			COL_MAX_RT_NORM_FILTER,
			COL_MIN_MZ_NORM_FILTER,
			COL_MAX_MZ_NORM_FILTER,
			COL_METHOD_NORM_FILTER,
			COL_PARAMS_NORM_FILTER,
			COL_MATRIX_NORM_FILTER
		);
	}

	stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindReal( 1, minInt );
		stmt->bindReal( 2, maxInt );
		stmt->bindInteger( 3, ( relFlg ? 1 : 0 ) );
		stmt->bindReal( 4, minRt );
		stmt->bindReal( 5, maxRt );
		stmt->bindReal( 6, minMz );
		stmt->bindReal( 7, maxMz );
		stmt->bindString( 8, NVL( method, "" ) );
		stmt->bindString( 9, NVL( params, "" ) );
		stmt->bindInteger( 10, matrix );

		stmt->execute();
		stmt->close();
	}
}

// get normalization filter
bool IdentificationManager::getNormalizationFilter(
		const int matrix,
		double* minInt,
		double* maxInt,
		bool* relFlg,
		double* minRt,
		double* maxRt,
		double* minMz,
		double* maxMz,
		std::string& method,
		std::string& params
) {
	// return value
	bool ret = false;

	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return ret;
	}

	// get filter
	std::string sql = FMT(
		"select %s, %s, %s, %s, %s, %s, %s, %s, %s from %s where %s = ?;",
		COL_MIN_INT_NORM_FILTER,
		COL_MAX_INT_NORM_FILTER,
		COL_REL_FLG_NORM_FILTER,
		COL_MIN_RT_NORM_FILTER,
		COL_MAX_RT_NORM_FILTER,
		COL_MIN_MZ_NORM_FILTER,
		COL_MAX_MZ_NORM_FILTER,
		COL_METHOD_NORM_FILTER,
		COL_PARAMS_NORM_FILTER,
		TABLE_NORM_FILTER,
		COL_MATRIX_NORM_FILTER
	);

	kome::db::Statement* stmt = db->prepare( sql.c_str() );

	if( stmt != NULL ) {
		stmt->bindInteger( 1, matrix );

		kome::db::ResultSet* rs = stmt->executeQuery();
		if( rs != NULL ) {
			if( rs->next() ) {
				if( minInt != NULL ) {
					*minInt = rs->getReal( 0, -1.0 );
				}
				if( maxInt != NULL ) {
					*maxInt = rs->getReal( 1, -1.0 );
				}
				if( relFlg != NULL ) {
					int flg = rs->getInt( 2, 0 );
					if( relFlg != NULL ) {
						*relFlg = ( flg > 0 );
					}
				}
				if( minRt != NULL ) {
					*minRt = rs->getReal( 3, -1.0 );
				}
				if( maxRt != NULL ) {
					*maxRt = rs->getReal( 4, -1.0 );
				}
				if( minMz != NULL ) {
					*minMz = rs->getReal( 5, -1.0 );
				}
				if( maxMz != NULL ) {
					*maxMz = rs->getReal( 6, -1.0 );
				}

				method = rs->getString( 7, "" );
				params = rs->getString( 8, "" );

				ret = true;
			}
			rs->close();
		}
		stmt->close();
	}

	return ret;
}

// add normalization method
void IdentificationManager::addNormalizationMethod(
		const int matrix,
		const char* method,
		const char* params
) {
	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return;
	}

	// filter
	int filterId = getNormalizationFilterId( matrix );
	if( filterId < 0 ) {
		return;
	}

	// order
	int order = -1;
	std::string sql = FMT(
		"select max(%s) from %s where %s = ?;",
		COL_ORDER_NORM_METHOD,
		TABLE_NORM_METHOD,
		COL_FILTER_NORM_METHOD
	);

	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindInteger( 1, filterId );
		kome::db::ResultSet* rs = stmt->executeQuery();
		if( rs != NULL ) {
			if( rs->next() ) {
				order = rs->getInt( 0, -1 );
			}
			rs->close();
		}
		stmt->close();
	}
	order = order + 1;

	// add
	sql = FMT(
		"insert into %s (%s, %s, %s, %s) values(?, ?, ?, ?);",
		TABLE_NORM_METHOD,
		COL_FILTER_NORM_METHOD,
		COL_ORDER_NORM_METHOD,
		COL_METHOD_NORM_METHOD,
		COL_PARAMS_NORM_METHOD
	);

	stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindInteger( 1, filterId );
		stmt->bindInteger( 2, order );
		stmt->bindString( 3, NVL( method, "" ) );
		stmt->bindString( 4, NVL( params, "" ) );

		stmt->execute();
		stmt->close();
	}
}

// get normalization methods
void IdentificationManager::getNormalizationMethods( const int matrix, std::vector< NormalizationMethodInfo >& methods ) {
	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return;
	}

	// filter ID
	int filterId = getNormalizationFilterId( matrix );
	if( filterId < 0 ) {
		return;
	}

	// methods
	std::string sql = FMT(
		"select %s, %s from %s where %s = ? order by %s;",
		COL_METHOD_NORM_METHOD,
		COL_PARAMS_NORM_METHOD,
		TABLE_NORM_METHOD,
		COL_FILTER_NORM_METHOD,
		COL_ORDER_NORM_METHOD
	);

	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindInteger( 1, filterId );
		kome::db::ResultSet* rs = stmt->executeQuery();

		if( rs != NULL ) {
			while( rs->next() ) {
				methods.resize( methods.size() + 1 );
				methods.back().method = rs->getString( 0, "" );
				methods.back().params = rs->getString( 1, "" );
			}
			rs->close();
		}
		stmt->close();
	}
}

// get normalization filter method from matirx
int IdentificationManager::getNormalizationFilterId( const int matrix ) {
	// return value
	int id = -1;

	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return id;
	}

	// get filter ID
	std::string sql = FMT(
		"select %s from %s where %s = ?;",
		COL_ID_NORM_FILTER,
		TABLE_NORM_FILTER,
		COL_MATRIX_NORM_FILTER
	);

	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindInteger( 1, matrix );
		kome::db::ResultSet* rs = stmt->executeQuery();
		if( rs != NULL ) {
			if( rs->next() ) {
				id = rs->getInt( 0, -1 );
			}
			rs->close();
		}
		stmt->close();
	}

	return id;
}

// get standard sample
int IdentificationManager::getStandardSample( const int matrixId ) {
	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return -1;
	}

	// get
	std::string sql = FMT(
		"select %s from %s where %s = ?;",
		COL_SAMPLE_STANDARD,
		TABLE_STANDARD_SAMPLE,
		COL_MATRIX_STANDARD
	);

	int sample = -1;

	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindInteger( 1, matrixId );
		kome::db::ResultSet* rs = stmt->executeQuery();

		if( rs != NULL ) {
			if( rs->next() ) {
				sample = rs->getInt( 0, -1 );
			}
			rs->close();
		}
		stmt->close();
	}

	return sample;
}

// set alignment
void IdentificationManager::setAlignment( const int matrix, const char* method, const char* params ) {
	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return;
	}

	// get ID
	int id = -1;
	std::string sql = FMT(
		"select %s from %s where %s = ?;",
		COL_ID_ALIGN,
		TABLE_ALIGN,
		COL_MATRIX_ALIGN
	);

	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindInteger( 1, matrix );

		kome::db::ResultSet* rs = stmt->executeQuery();
		if( rs != NULL ) {
			if( rs->next() ) {
				id = rs->getInt( 0, -1 );
			}
			rs->close();
		}
		stmt->close();
	}

	// set alignment
	if( id < 0 ) {    // new
		sql = FMT(
			"insert into %s (%s, %s, %s) values (?, ?, ?);",
			TABLE_ALIGN,
			COL_MATRIX_ALIGN,
			COL_METHOD_ALIGN,
			COL_PARAMS_ALIGN
		);

		stmt = db->prepare( sql.c_str() );
		if( stmt != NULL ) {
			stmt->bindInteger( 1, matrix );
			stmt->bindString( 2, NVL( method, "" ) );
			stmt->bindString( 3, NVL( params, "" ) );

			stmt->execute();
			stmt->close();
		}
	}
	else {    // update
		sql = FMT(
			"update %s set %s = ?, %s = ? where %s = ?;",
			TABLE_ALIGN,
			COL_METHOD_ALIGN,
			COL_PARAMS_ALIGN,
			COL_ID_ALIGN
		);

		stmt = db->prepare( sql.c_str() );
		if( stmt != NULL ) {
			stmt->bindString( 1, NVL( method, "" ) );
			stmt->bindString( 2, NVL( params, "" ) );
			stmt->bindInteger( 3, id );

			stmt->execute();
			stmt->close();
		}
	}
}

// get alignment
kome::operation::Alignment* IdentificationManager::getAlignment( const int matrix ) {
	// return value
	kome::operation::Alignment* align = NULL;

	// manager
	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();

	// DB
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return align;
	}

	// get info
	std::string method;
	std::string params;

	std::string sql = FMT(
		"select %s, %s from %s where %s = ?",
		COL_METHOD_ALIGN,
		COL_PARAMS_ALIGN,
		TABLE_ALIGN,
		COL_MATRIX_ALIGN
	);

	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindInteger( 1, matrix );

		kome::db::ResultSet* rs = stmt->executeQuery();
		if( rs != NULL ) {
			if( rs->next() ) {
				method = rs->getString( 0, "" );
				params = rs->getString( 1, "" );
			}
				
			rs->close();
		}
		stmt->close();
	}

	// plug-in 
	kome::plugin::PluginFunctionItem* item = NULL;
	for( unsigned int i = 0; i < plgMgr.getNumberOfFunctionItems( ALIGNMENT_FUNC_TYPE ) && item == NULL; i++ ) {
		kome::plugin::PluginFunctionItem* tmp = plgMgr.getFunctionItem( ALIGNMENT_FUNC_TYPE, i );
		if( method.compare( tmp->getShortName() ) == 0 ) {
			item = tmp;
		}
	}

	if( item == NULL ) {
		return align;
	}

	// parameters
	kome::objects::SettingParameterValues settings;
	kome::plugin::SettingsPage* page = item->getSettingsPage();
	if( page != NULL ) {
		page->setParameterString( settings, params.c_str() );
	}

	// alignment
	kome::objects::Parameters prms;
	kome::plugin::PluginCallTool::setSettingValues( prms, settings );

	align = (kome::operation::Alignment*)( item->getCall()->invoke( &prms ).prim.pt );

	return align;
}

// add matrix column
int IdentificationManager::addMatrixColumn( const char* name, const ColumnDataType type, const int matrixId ) {
	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return -1;
	}

	// ID
	int id = -1;

	// get
	std::string sql = FMT(
		"select %s from %s where %s = ? and %s = ?;",
		COL_ID_MATRIX_COL,
		TABLE_MATRIX_COL,
		COL_NAME_MATRIX_COL,
		COL_MATRIX_MATRIX_COL
	);

	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindString( 1, NVL( name, "" ) );
		stmt->bindInteger( 2, matrixId );

		kome::db::ResultSet* rs = stmt->executeQuery();
		if( rs != NULL ) {
			if( rs->next() ) {
				id = rs->getInt( 0, -1 );
			}
			rs->close();
		}
		stmt->close();
	}
	if( id >= 0 ) {
		return id;
	}

	// add
	sql = FMT(
		"insert into %s (%s, %s, %s) values(?, ?, ?);",
		TABLE_MATRIX_COL,
		COL_MATRIX_MATRIX_COL,
		COL_NAME_MATRIX_COL,
		COL_TYPE_MATRIX_COL
	);

	bool ret = false;
	stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindInteger( 1, matrixId );
		stmt->bindString( 2, NVL( name, "" ) );
		stmt->bindInteger( 3, (int)type );

		ret = stmt->execute();
		stmt->close();
	}

	if( ret ) {
		id = db->getMaxValue( TABLE_MATRIX_COL, COL_ID_MATRIX_COL );
		if( id < 0 ) {
			id = -1;
		}
	}

	return id;
}

// delete matrix column
void IdentificationManager::deleteMatrixColumn( const int id ) {
	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return;
	}

	// delete values
	deleteAppendedValueFromMatrixColumn( id );

	// delete
	std::string sql = FMT( "delete from %s where %s = ?;", TABLE_MATRIX_COL, COL_ID_MATRIX_COL );
	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindInteger( 1, id );

		stmt->execute();
		stmt->close();
	}
}

// update matrix column
void IdentificationManager::updateMatrixColumn( const int id, const char* name, const ColumnDataType type ) {
	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return;
	}

	// update
	std::string sql = FMT(
		"update %s set %s = ?, %s = ? where %s = ?;",
		TABLE_MATRIX_COL,
		COL_NAME_MATRIX_COL,
		COL_TYPE_MATRIX_COL,
		COL_ID_MATRIX_COL
	);

	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindString( 1, NVL( name, "" ) );
		stmt->bindInteger( 2, (int)type );
		stmt->bindInteger( 3, id );

		stmt->execute();
		stmt->close();
	}
}

// get matrix columns
void IdentificationManager::getMatrixColumns( std::vector< int >& columns, const int matrixId ) {
	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return;
	}

	// get columns
	std::string sql = FMT(
		"select %s from %s where %s = ?;",
		COL_ID_MATRIX_COL,
		TABLE_MATRIX_COL,
		COL_MATRIX_MATRIX_COL
	);

	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindInteger( 1, matrixId );
		kome::db::ResultSet* rs = stmt->executeQuery();
		if( rs != NULL ) {
			while( rs->next() ) {
				int id = rs->getInt( 0, -1 );
				if( id >= 0 ) {
					columns.push_back( id );
				}
			}
			rs->close();
		}
		stmt->close();
	}
}

// get matrix column name
std::string IdentificationManager::getMatrixColumnName( const int id ) {
	// string
	std::string s;

	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return s;
	}

	// column name
	std::string sql = FMT(
		"select %s from %s where %s = ?;",
		COL_NAME_MATRIX_COL,
		TABLE_MATRIX_COL,
		COL_ID_MATRIX_COL
	);

	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindInteger( 1, id );
		kome::db::ResultSet* rs = stmt->executeQuery();
		if( rs != NULL ) {
			if( rs->next() ) {
				s = rs->getString( 0, "" );
			}
			rs->close();
		}
		stmt->close();
	}

	return s;
}

// get column data type
ColumnDataType IdentificationManager::getMatrixColumnType( const int id ) {
	// type
	ColumnDataType type = TYPE_STRING;

	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return type;
	}

	// column name
	std::string sql = FMT(
		"select %s from %s where %s = ?;",
		COL_TYPE_MATRIX_COL,
		TABLE_MATRIX_COL,
		COL_ID_MATRIX_COL
	);

	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindInteger( 1, id );
		kome::db::ResultSet* rs = stmt->executeQuery();
		if( rs != NULL ) {
			if( rs->next() ) {
				type = (ColumnDataType)rs->getInt( 0, (int)TYPE_STRING );
			}
			rs->close();
		}
		stmt->close();
	}

	return type;
}

// add matrix peak
int IdentificationManager::addMatrixPeak( const double rt, const double mz, const int charge, const int matrixId ) {
    // db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return -1;
	}

	// add
	std::string sql = FMT(
		"insert into %s (%s, %s, %s, %s) values(?, ?, ?, ?);",
		TABLE_PEAK,
		COL_RT_PEAK,
		COL_MZ_PEAK,
		COL_CHARGE_PEAK,
		COL_MATRIX_PEAK
	);

	bool ret = false;
	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		if( rt >= 0.0 ) {
			stmt->bindReal( 1, rt );
		}
		else {
			stmt->bindReal( 1, -1.0 );
		}

		if( mz >= 0.0 ) {
			stmt->bindReal( 2, mz );
		}
		else {
			stmt->bindReal( 2, -1.0 );
		}

		if( charge > 0 ) {
			stmt->bindInteger( 3, charge );
		}
		else {
			stmt->bindInteger( 3, -1 );
		}

		stmt->bindInteger( 4, matrixId );

		ret = stmt->execute();
		stmt->close();
	}

	// id
	int id = -1;
	if( ret ) {
		id = db->getMaxValue( TABLE_PEAK, COL_ID_PEAK );
		if( id < 0 ) {
			id = -1;
		}
	}

	return id;
}

// delete peak
void IdentificationManager::deleteMatrixPeak( const int id ) {
	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return;
	}

	// delete values
	deleteAppendedValueFromPeak( id );
	deletePeakValuesFromMatrixPeak( id );

	// delete spectra
	std::vector< int > spectra;
	getMatrixPeakSpectraFromPeak( spectra, id );
	for( unsigned int i = 0; i < spectra.size(); i++ ) {
		deleteMatrixPeakSpectrum( spectra[ i ] );
	}

	// delete
	std::string sql = FMT( "delete from %s where %s = ?;", TABLE_PEAK, COL_ID_PEAK );
	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindInteger( 1, id );
		stmt->execute();
		stmt->close();
	}
}

// update peak
void IdentificationManager::updateMatrixPeak( const int id, const double rt, const double mz, const int charge ) {
	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return;
	}

	// delete
	std::string sql = FMT(
		"update %s set %s = ?, %s = ?, %s = ? where %s = ?;",
		TABLE_PEAK,
		COL_RT_PEAK,
		COL_MZ_PEAK,
		COL_CHARGE_PEAK,
		COL_ID_PEAK
	);
	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		if( rt >= 0.0 ) {
			stmt->bindReal( 1, rt );
		}
		else {
			stmt->bindReal( 1, -1.0 );
		}

		if( mz >= 0.0 ) {
			stmt->bindReal( 2, mz );
		}
		else {
			stmt->bindReal( 2, -1.0 );
		}

		if( charge > 0 ) {
			stmt->bindInteger( 3, charge );
		}
		else {
			stmt->bindInteger( 3, -1 );
		}

		stmt->execute();
		stmt->close();
	}
}

// get matrix peaks
void IdentificationManager::getMatrixPeaks( std::vector< int >& peaks, const int matrixId ) {
	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return;
	}

	// matrix peaks
	std::string sql = FMT(
		"select %s from %s where %s = ?;",
		COL_ID_PEAK,
		TABLE_PEAK,
		COL_MATRIX_PEAK
	);

	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindInteger( 1, matrixId );
		kome::db::ResultSet* rs = stmt->executeQuery();
		if( rs != NULL ) {
			while( rs->next() ) {
				const int id = rs->getInt( 0, -1 );
				if( id >= 0 ) {
					peaks.push_back( id );
				}
			}
			rs->close();
		}
		stmt->close();
	}
}

// get matrix peak RT
double IdentificationManager::getMatrixPeakRt( const int id ) {
	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return -1.0;
	}

	// RT
	std::string sql = FMT(
		"select %s from %s where %s = ?;", 
		COL_RT_PEAK,
		TABLE_PEAK,
		COL_ID_PEAK
	);

	double rt = -1.0;
	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindInteger( 1, id );

		kome::db::ResultSet* rs = stmt->executeQuery();
		if( rs != NULL ) {
			if( rs->next() ) {
				rt = rs->getReal( 0, -1.0 );
			}
			rs->close();
		}
		stmt->close();
	}

	return rt;
}

// get matrix peak m/z
double IdentificationManager::getMatrixPeakMz( const int id ) {
	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return -1.0;
	}

	// m/z
	std::string sql = FMT(
		"select %s from %s where %s = ?;", 
		COL_MZ_PEAK,
		TABLE_PEAK,
		COL_ID_PEAK
	);

	double mz = -1.0;
	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindInteger( 1, id );

		kome::db::ResultSet* rs = stmt->executeQuery();
		if( rs != NULL ) {
			if( rs->next() ) {
				mz = rs->getReal( 0, -1.0 );
			}
			rs->close();
		}
		stmt->close();
	}

	return mz;
}

// get matrix peak charge
int IdentificationManager::getMatrixPeakCharge( const int id ) {
	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return -1;
	}

	// m/z
	std::string sql = FMT(
		"select %s from %s where %s = ?;", 
		COL_CHARGE_PEAK,
		TABLE_PEAK,
		COL_ID_PEAK
	);

	int charge = -1;
	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindInteger( 1, id );

		kome::db::ResultSet* rs = stmt->executeQuery();
		if( rs != NULL ) {
			if( rs->next() ) {
				charge = rs->getInt( 0, -1 );
			}
			rs->close();
		}
		stmt->close();
	}

	return charge;
}

// get sample set ID
int IdentificationManager::addSampleSet( const char* path ) {
	// DB
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return -1;
	}

	// absolute path
	std::string p = absolutepath( path );

	// get sample set ID
	int id = getSampleSet( p.c_str() );
	if( id >= 0 ) {
		return id;
	}

	// add
	std::string sql = FMT(
		"insert into %s (%s) values (?);",
		TABLE_SAMPLE_SET,
		COL_PATH_SAMPLE_SET
	);

	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	bool ret = false;
	if( stmt != NULL ) {
		stmt->bindString( 1, p.c_str() );
		ret = stmt->execute();
		stmt->close();
	}

	if( ret ) {
		id = db->getMaxValue( TABLE_SAMPLE_SET, COL_ID_SAMPLE_SET );
		if( id < 0 ) {
			id = -1;
		}
	}

	return id;		
}

// get sample set
int IdentificationManager::getSampleSet( const char* path ) {
	// DB
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return -1;
	}

	// get sample set ID
	if( path == NULL ) {
		return -1;
	}
	std::string p = absolutepath( path );

	// search
	int id = -1;
	std::string sql = FMT(
		"select %s from %s where %s = ?;",
		COL_ID_SAMPLE_SET,
		TABLE_SAMPLE_SET,
		COL_PATH_SAMPLE_SET
	);

	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindString( 1, p.c_str() );
		kome::db::ResultSet* rs = stmt->executeQuery();
		if( rs != NULL ) {
			if( rs->next() ) {
				id = rs->getInt( 0, -1 );
			}
			rs->close();
		}
		stmt->close();
	}

	return id;
}

// add sample
int IdentificationManager::addSample( kome::objects::Sample* sample ) {
	// sample set
	if( sample == NULL ) {
		return -1;
	}
	kome::objects::SampleSet* ss = sample->getSampleSet();
	if( ss == NULL ) {
		return -1;
	}

	// add
	return addSample( ss->getFilePath(), sample->getSampleIndex(), sample->getName() );
}

// add sample
int IdentificationManager::addSample( const char* path, const int sampleIndex, const char* name ) {
	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return -1;
	}

	// get sample
	int id = getSample( path, sampleIndex );
	if( id >= 0 ) {
		return id;
	}

	// sample set ID
	const int ssId = addSampleSet( path );
	if( ssId < 0 ) {
		return -1;
	}

	// add
	bool ret = false;
	std::string sql = FMT(
		"insert into %s (%s, %s, %s) values(?, ?, ?);",
		TABLE_SAMPLE,
		COL_SAMPLE_SET_SAMPLE,
		COL_SAMPLE_ID_SAMPLE,
		COL_NAME_SAMPLE
	);

	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindInteger( 1, ssId );
		stmt->bindInteger( 2, sampleIndex );
		stmt->bindString( 3, NVL( name, "" ) );

		ret = stmt->execute();
		stmt->close();
	}

	// get ID
	if( ret ) {
		id = db->getMaxValue( TABLE_SAMPLE, COL_ID_SAMPLE );
		if( id < 0 ) {
			id = -1;
		}
	}

	return id;
}

// get sample
int IdentificationManager::getSample( kome::objects::Sample* sample ) {
	// sample set
	if( sample == NULL ) {
		return -1;
	}
	kome::objects::SampleSet* ss = sample->getSampleSet();
	if( ss == NULL ) {
		return -1;
	}

	// add
	return getSample( ss->getFilePath(), sample->getSampleIndex() );
}

// get sample
int IdentificationManager::getSample( const char* path, const int sampleIndex ) {
	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return -1;
	}

	// sample set ID
	const int ssId = getSampleSet( path );
	if( ssId < 0 ) {
		return -1;
	}

	// search
	int id = -1;
	std::string sql = FMT(
		"select %s from %s where %s = ? and %s = ?;",
		COL_ID_SAMPLE,
		TABLE_SAMPLE,
		COL_SAMPLE_SET_SAMPLE,
		COL_SAMPLE_ID_SAMPLE
	);

	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindInteger( 1, ssId );
		stmt->bindInteger( 2, sampleIndex );

		kome::db::ResultSet* rs = stmt->executeQuery();
		if( rs != NULL ) {
			if( rs->next() ) {
				id = rs->getInt( 0, -1 );
			}
			rs->close();
		}
		stmt->close();
	}

	return id;
}

// delete sample
void IdentificationManager::deleteSample( const int id ) {
	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return;
	}

	// delete spectra
	std::vector< int > spectra;
	getSpectra( spectra, id );
	for( unsigned int i = 0; i < spectra.size(); i++ ) {
		deleteSpectrum( spectra[ i ] );
	}

	// delete matrix sample
	std::vector< int > samples;
	getMatrixSamplesFromSample( samples, id );
	for( unsigned int i = 0; i < samples.size(); i++ ) {
		deleteMatrixSample( samples[ i ] );
	}

	// delete
	std::string sql = FMT( "delete from %s where %s = ?;", TABLE_SAMPLE, COL_ID_SAMPLE );
	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindInteger( 1, id );

		stmt->execute();
		stmt->close();
	}
}

// get sample name
std::string IdentificationManager::getSampleName( const int sampleId ) {
	// name
	std::string s;

	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return s;
	}

	// sample name
	std::string sql = FMT(
		"select %s from %s where %s = ?;",
		COL_NAME_SAMPLE,
		TABLE_SAMPLE,
		COL_ID_SAMPLE
	);

	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindInteger( 1, sampleId );
		kome::db::ResultSet* rs = stmt->executeQuery();
		if( rs != NULL ) {
			if( rs->next() ) {
				s = rs->getString( 0, "" );
			}
			rs->close();
		}
		stmt->close();
	}

	return s;
}

// get sample file path
std::string IdentificationManager::getSampleFilePath( const int sampleId ) {
	// name
	std::string s;

	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return s;
	}

	// sample file path
	std::string sql = FMT(
		"select ss.%s from %s as ss, %s as s where ss.%s = s.%s and s.%s = ?;",
		COL_PATH_SAMPLE_SET,
		TABLE_SAMPLE_SET,
		TABLE_SAMPLE,
		COL_ID_SAMPLE_SET,
		COL_SAMPLE_SET_SAMPLE,
		COL_ID_SAMPLE
	);

	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindInteger( 1, sampleId );
		kome::db::ResultSet* rs = stmt->executeQuery();
		if( rs != NULL ) {
			if( rs->next() ) {
				s = rs->getString( 0, "" );
			}
			rs->close();
		}
		stmt->close();
	}

	return s;
}

// get sample index
int IdentificationManager::getSampleIndex( const int sampleId ) {
	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return -1;
	}

	// sample index
	std::string sql = FMT(
		"select %s from %s where %s = ?;",
		COL_SAMPLE_ID_SAMPLE,
		TABLE_SAMPLE,
		COL_ID_SAMPLE
	);

	int id = -1;
	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindInteger( 1, sampleId );
		kome::db::ResultSet* rs = stmt->executeQuery();
		if( rs != NULL ) {
			if( rs->next() ) {
				id = rs->getInt( 0, -1 );
			}
			rs->close();
		}
		stmt->close();
	}

	return id;
}

// open sample
kome::objects::Sample* IdentificationManager::openSample( const int id, SampleReadInfo& readInfo ) {
	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return NULL;
	}

	// manager
	kome::plugin::FileFactory& ff = kome::plugin::FileFactory::getInstance();

	// get ID and path
	std::string sql = FMT(
		"select ss.%s, s.%s from %s as ss, %s as s where ss.%s = s.%s and s.%s = ?;",
		COL_PATH_SAMPLE_SET,
		COL_SAMPLE_ID_SAMPLE,
		TABLE_SAMPLE_SET,
		TABLE_SAMPLE,
		COL_ID_SAMPLE_SET,
		COL_SAMPLE_SET_SAMPLE,
		COL_ID_SAMPLE
	);

	kome::objects::Sample* sample = NULL;
	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	readInfo.id = id;
	if( stmt != NULL ) {
		stmt->bindInteger( 1, id );
		kome::db::ResultSet* rs = stmt->executeQuery();
		if( rs != NULL ) {
			if( rs->next() ) {
				std::string path = rs->getString( 0, "" );
				const int sId = rs->getInt( 1, -1 );
				// --> @2014/3/3 <Add> by Y.Fujita  毎回openFileしていたので、既に開いているものは開かないようにした。
				kome::objects::SampleSet* ss =  ff.searchFile( path.c_str());
				readInfo.openSampleSet = ( ss == NULL );      
	    		if ( ss == NULL ) {
					ss = ff.openFile( path.c_str(), NULL );	// add param
				}		
				if( ss != NULL ) {
					sample = ss->getSample( sId );
					if( sample != NULL ) {
						readInfo.openSample = !sample->isOpened();
						if( readInfo.openSample) {
							sample->openSample();
						}
					}
				}
				//--< 
			}
			rs->close();
		}
		stmt->close();
	}

	return sample;
}

// release sample
void IdentificationManager::releaseSample( kome::objects::Sample* sample, SampleReadInfo& readInfo ) {
	// check parameters
	if( sample == NULL || readInfo.id < 0 ) {
		return;
	}

	// file factory
	kome::plugin::FileFactory& factory = kome::plugin::FileFactory::getInstance();

	// sample set
	kome::objects::SampleSet* ss = sample->getSampleSet();

	// close sample
	if( readInfo.openSample ) {
		sample->closeSample();
	}

	// close sample set
	if( readInfo.openSampleSet && ss != NULL ) {
		factory.closeFile( ss );
	}
}


// add matrix sample
int IdentificationManager::addMatrixSample(
		const int group,
		const int sample,
		const char* name,
		const COLOR color
) {
	// parameters
	if( group < 0 || sample < 0 ) {
		return -1;
	}

	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return -1;
	}

	// search
	std::string sql = FMT(
		"select %s from %s where %s = ? and %s = ?;",
		COL_ID_MATRIX_SAMPLE,
		TABLE_MATRIX_SAMPLE,
		COL_GROUP_MATRIX_SAMPLE,
		COL_SAMPLE_MATRIX_SAMPLE
	);

	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	int id = -1;
	if( stmt != NULL ) {
		stmt->bindInteger( 1, group );
		stmt->bindInteger( 2, sample );

		kome::db::ResultSet* rs = stmt->executeQuery();
		if( rs != NULL ) {
			if( rs->next() ) {
				id = rs->getInt( 0, -1 );
			}
			rs->close();
		}
		stmt->close();
	}

	if( id >= 0 ) {
		return id;
	}

	// add
	sql = FMT(
		"insert into %s (%s, %s, %s, %s) values(?, ?, ?, ?);",
		TABLE_MATRIX_SAMPLE,
		COL_GROUP_MATRIX_SAMPLE,
		COL_SAMPLE_MATRIX_SAMPLE,
		COL_NAME_MATRIX_SAMPLE,
		COL_COLOR_MATRIX_SAMPLE
	);

	stmt = db->prepare( sql.c_str() );
	bool ret = false;
	if( stmt != NULL ) {
		stmt->bindInteger( 1, group );
		stmt->bindInteger( 2, sample );
		stmt->bindString( 3, NVL( name, "" ) );
		stmt->bindInteger( 4, (int)color );

		ret = stmt->execute();
		stmt->close();
	}

	// ID
	if( ret ) {
		id = db->getMaxValue( TABLE_MATRIX_SAMPLE, COL_ID_MATRIX_SAMPLE );
		if( id < 0 ) {
			id = -1;
		}
	}

	return id;
}

// delete matrix sample
void IdentificationManager::deleteMatrixSample( const int id ) {
	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return;
	}

	// delete values
	deletePeakValuesFromMatrixSample( id );

	// delete
	std::string sql = FMT( "delete from %s where %s = ?;", TABLE_MATRIX_SAMPLE, COL_ID_MATRIX_SAMPLE );
	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindInteger( 1, id );

		stmt->execute();
		stmt->close();
	}
}

// update matrix sample
void IdentificationManager::updateMatrixSample( const int id, const char* name, const COLOR color ) {
	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return;
	}

	// update
	std::string sql = FMT(
		"update %s set %s = ?, %s = ? where %s = ?;",
		TABLE_MATRIX_SAMPLE,
		COL_NAME_MATRIX_SAMPLE,
		COL_COLOR_MATRIX_SAMPLE,
		COL_ID_MATRIX_SAMPLE
	);

	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindString( 1, NVL( name, "" ) );
		stmt->bindInteger( 2, (int)color );
		stmt->bindInteger( 3, id );

		stmt->execute();
		stmt->close();
	}
}

// get matrix samples from group
void IdentificationManager::getMatrixSamplesFromGroup( std::vector< int >& samples, const int group ) {
	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return;
	}

	// samples
	std::string sql = FMT(
		"select %s from %s where %s = ?;",
		COL_ID_MATRIX_SAMPLE,
		TABLE_MATRIX_SAMPLE,
		COL_GROUP_MATRIX_SAMPLE
	);

	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindInteger( 1, group );
		kome::db::ResultSet* rs = stmt->executeQuery();
		if( rs != NULL ) {
			while( rs->next() ) {
				const int id = rs->getInt( 0, -1 );
				if( id >= 0 ) {
					samples.push_back( id );
				}
			}
			rs->close();
		}
		stmt->close();
	}
}

// get matrix samples from sample
void IdentificationManager::getMatrixSamplesFromSample( std::vector< int >& samples, const int sample ) {
	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return;
	}

	// samples
	std::string sql = FMT(
		"select %s from %s where %s = ?;",
		COL_ID_MATRIX_SAMPLE,
		TABLE_MATRIX_SAMPLE,
		COL_SAMPLE_MATRIX_SAMPLE
	);

	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindInteger( 1, sample );
		kome::db::ResultSet* rs = stmt->executeQuery();
		if( rs != NULL ) {
			while( rs->next() ) {
				const int id = rs->getInt( 0, -1 );
				if( id >= 0 ) {
					samples.push_back( id );
				}
			}
			rs->close();
		}
		stmt->close();
	}
}

// get matrix sample sample
int IdentificationManager::getMatrixSampleSample( const int id ) {
	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return -1;
	}

	// sample
	int sample = -1;
	std::string sql = FMT( 
		"select %s from %s where %s = ?;",
		COL_SAMPLE_MATRIX_SAMPLE,
		TABLE_MATRIX_SAMPLE,
		COL_ID_MATRIX_SAMPLE
	);

	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindInteger( 1, id );
		kome::db::ResultSet* rs = stmt->executeQuery();
		if( rs != NULL ) {
			if( rs->next() ) {
				sample = rs->getInt( 0, -1 );
			}
			rs->close();
		}
		stmt->close();
	}

	return sample;
}

// get matrix sample group
int IdentificationManager::getMatrixSampleGroup( const int id ) {
	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return -1;
	}

	// group
	int group = -1;
	std::string sql = FMT( 
		"select %s from %s where %s = ?;",
		COL_GROUP_MATRIX_SAMPLE,
		TABLE_MATRIX_SAMPLE,
		COL_ID_MATRIX_SAMPLE
	);

	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindInteger( 1, id );
		kome::db::ResultSet* rs = stmt->executeQuery();
		if( rs != NULL ) {
			if( rs->next() ) {
				group = rs->getInt( 0, -1 );
			}
			rs->close();
		}
		stmt->close();
	}

	return group;
}

// get matrix sample name
std::string IdentificationManager::getMatrixSampleName( const int id ) {
	// string
	std::string s;

	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return s;
	}

	// group
	std::string sql = FMT( 
		"select %s from %s where %s = ?;",
		COL_NAME_MATRIX_SAMPLE,
		TABLE_MATRIX_SAMPLE,
		COL_ID_MATRIX_SAMPLE
	);

	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindInteger( 1, id );
		kome::db::ResultSet* rs = stmt->executeQuery();
		if( rs != NULL ) {
			if( rs->next() ) {
				s = rs->getString( 0, "" );
			}
			rs->close();
		}
		stmt->close();
	}

	return s;
}

// get matrix sample name
COLOR IdentificationManager::getMatrixSampleColor( const int id ) {
	// string
	COLOR c = -1;

	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return c;
	}

	// group
	std::string sql = FMT( 
		"select %s from %s where %s = ?;",
		COL_COLOR_MATRIX_SAMPLE,
		TABLE_MATRIX_SAMPLE,
		COL_ID_MATRIX_SAMPLE
	);

	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindInteger( 1, id );
		kome::db::ResultSet* rs = stmt->executeQuery();
		if( rs != NULL ) {
			if( rs->next() ) {
				c = (COLOR)rs->getInt( 0, -1 );
			}
			rs->close();
		}
		stmt->close();
	}

	return c;
}

// set appended value
void IdentificationManager::setAppendedValue( const int peak, const int col, const char* val ) {
	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return;
	}

	// string
	std::string s = NVL( val, "" );

	// set
	if( s.empty() ) {    // delete
		std::string sql = FMT(
			"delete from %s where %s = ? and %s = ?;",
			TABLE_MATRIX_VAL,
			COL_PEAK_MATRIX_VAL,
			COL_COL_MATRIX_VAL
		);

		kome::db::Statement* stmt = db->prepare( sql.c_str() );
		if( stmt != NULL ) {
			stmt->bindInteger( 1, peak );
			stmt->bindInteger( 2, col );

			stmt->execute();
			stmt->close();
		}
	}
	else {
		// search
		int id = -1;
		std::string sql = FMT( 
			"select %s from %s where %s = ? and %s = ?;",
			COL_ID_MATRIX_VAL,
			TABLE_MATRIX_VAL,
			COL_PEAK_MATRIX_VAL,
			COL_COL_MATRIX_VAL
		);

		kome::db::Statement* stmt = db->prepare( sql.c_str() );
		if( stmt != NULL ) {
			stmt->bindInteger( 1, peak );
			stmt->bindInteger( 2, col );

			kome::db::ResultSet* rs = stmt->executeQuery();
			if( rs != NULL ) {
				if( rs->next() ) {
					id = rs->getInt( 0, -1 );
				}
				rs->close();
			}
			stmt->close();
		}

		if( id >= 0 ) {    // update
			std::string sql = FMT(
				"update %s set %s = ?, %s = ?, %s = ? where %s = ?;",
				TABLE_MATRIX_VAL,
				COL_PEAK_MATRIX_VAL,
				COL_COL_MATRIX_VAL,
				COL_VALUE_MATRIX_VAL,
				COL_ID_MATRIX_VAL
			);

			kome::db::Statement* stmt = db->prepare( sql.c_str() );
			if( stmt != NULL ) {
				stmt->bindInteger( 1, peak );
				stmt->bindInteger( 2, col );
				stmt->bindString( 3, s.c_str() );
				stmt->bindInteger( 4, id );

				stmt->execute();
				stmt->close();
			}
		}
		else {    // insert
			std::string sql = FMT(
				"insert into %s (%s, %s, %s) values (?, ?, ?);",
				TABLE_MATRIX_VAL,
				COL_PEAK_MATRIX_VAL,
				COL_COL_MATRIX_VAL,
				COL_VALUE_MATRIX_VAL
			);

			kome::db::Statement* stmt = db->prepare( sql.c_str() );
			if( stmt != NULL ) {
				stmt->bindInteger( 1, peak );
				stmt->bindInteger( 2, col );
				stmt->bindString( 3, s.c_str() );

				stmt->execute();
				stmt->close();
			}
		}
	}
}

// get appended value
std::string IdentificationManager::getAppendedValue( const int peak, const int col ) {
	// string
	std::string s;

	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return s;
	}

	// get
	std::string sql = FMT(
		"select %s from %s where %s = ? and %s = ?;",
		COL_VALUE_MATRIX_VAL,
		TABLE_MATRIX_VAL,
		COL_PEAK_MATRIX_VAL,
		COL_COL_MATRIX_VAL
	);

	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindInteger( 1, peak );
		stmt->bindInteger( 2, col );

		kome::db::ResultSet* rs = stmt->executeQuery();
		if( rs != NULL ) {
			if( rs->next() ) {
				s = rs->getString( 0, "" );
			}
			rs->close();
		}
		stmt->close();
	}

	return s;
}

// delete appended values from peak
void IdentificationManager::deleteAppendedValueFromPeak( const int peak ) {
	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return;
	}

	// delete
	std::string sql = FMT(
		"delete from %s where %s = ?;",
		TABLE_MATRIX_VAL,
		COL_PEAK_MATRIX_VAL
	);

	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindInteger( 1, peak );
		stmt->execute();
		stmt->close();
	}
}

// delete appended values from column
void IdentificationManager::deleteAppendedValueFromMatrixColumn( const int col ) {
	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return;
	}

	// delete
	std::string sql = FMT(
		"delete from %s where %s = ?;",
		TABLE_MATRIX_VAL,
		COL_COL_MATRIX_VAL
	);

	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindInteger( 1, col );

		stmt->execute();
		stmt->close();
	}
}

// set profile
int IdentificationManager::setProfile( kome::core::XYData& data, const bool isChrom ) {
	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return -1;
	}

	// data points
	kome::core::DataPoints dps( kome::core::DataPoints::FLOAT );
	for( unsigned int i = 0; i < data.getLength(); i++ ) {
		const double x = data.getX( i );
		const double y = data.getY( i );
		dps.addPoint( x, y );
	}

	kome::core::Buffer buff;
	dps.exportData( boost::bind( &kome::core::Buffer::write, &buff, _1, _2 ) );

	// add
	std::string sql = FMT(
		"insert into %s (%s, %s) values(?, ?);",
		TABLE_PROFILE,
		COL_DATA_PROFILE,
		COL_CHROM_PROFILE
	);

	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	bool ret = false;
	if( stmt != NULL ) {
		stmt->bindData( 1, buff.getBuffer(), buff.getLength() );
		stmt->bindInteger( 2, ( isChrom ? 1 : 0 ) );

		ret = stmt->execute();
		stmt->close();
	}

	// ID
	int id = -1;
	if( ret ) {
		id = db->getMaxValue( TABLE_PROFILE, COL_ID_PROFILE );
		if( id < 0 ) {
			id = -1;
		}
	}

	return id;
}

// delete profile
void IdentificationManager::deleteProfile( const int id ) {
	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return;
	}

	// delete
	std::string sql = FMT( "delete from %s where %s = ?;", TABLE_PROFILE, COL_ID_PROFILE );
	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindInteger( 1, id );
		stmt->execute();
		stmt->close();
	}
}

// get profile
void IdentificationManager::getProfile( const int id, kome::core::XYData& data ) {
	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return;
	}

	// data
	kome::core::DataPoints dps( kome::core::DataPoints::FLOAT );

	std::string sql = FMT( 
		"select %s from %s where %s = ?;",
		COL_DATA_PROFILE,
		TABLE_PROFILE,
		COL_ID_PROFILE
	);

	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindInteger( 1, id );
		kome::db::ResultSet* rs = stmt->executeQuery();
		if( rs != NULL ) {
			if( rs->next() ) {
				void* addr = rs->getData( 0 );
				const unsigned int len = rs->getDataSize( 0 );
				
				if( addr != NULL && len >= 0 ) {
					kome::core::MemoryAccessor acc( addr, len );
					dps.importData( boost::bind( &kome::core::DataAccessor::read, &acc, _1, _2 ) );
				}
			}
			rs->close();
		}
		stmt->close();
	}

	// set point
	for( unsigned int i = 0; i < dps.getLength(); i++ ) {
		const double x = dps.getX( i );
		const double y = dps.getY( i );

		if( x >= 0.0 ) {
			data.addPoint( x, y );
		}
	}
}

// get chromatogram flag
bool IdentificationManager::isChromatogramProfile( const int id ) {
	// return value
	bool ret = false;

	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return ret;
	}

	// data
	kome::core::DataPoints dps( kome::core::DataPoints::FLOAT );

	std::string sql = FMT( 
		"select %s from %s where %s = ?;",
		COL_CHROM_PROFILE,
		TABLE_PROFILE,
		COL_ID_PROFILE
	);

	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindInteger( 1, id );
		kome::db::ResultSet* rs = stmt->executeQuery();
		if( rs != NULL ) {
			if( rs->next() ) {
				ret = ( rs->getInt( 0, 0 ) > 0 );
			}
			rs->close();
		}
		stmt->close();
	}

	return ret;
}

// set peak value
void IdentificationManager::setPeakValue(
		const int matrixPeak,
		const int matrixSample,
		const double intensity,
		kome::objects::PeakElement* peak,
		const int profile
) {
	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return;
	}

	// search
	std::string sql = FMT(
		"select %s from %s where %s = ? and %s = ?;",
		COL_ID_PEAK_VAL,
		TABLE_PEAK_VAL,
		COL_PEAK_PEAK_VAL,
		COL_SAMPLE_PEAK_VAL
	);

	int id = -1;

	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindInteger( 1, matrixPeak );
		stmt->bindInteger( 2, matrixSample );

		kome::db::ResultSet* rs = stmt->executeQuery();
		if( rs != NULL ) {
			if( rs->next() ) {
				id = rs->getInt( 0, -1 );
			}
			rs->close();
		}
		stmt->close();
	}

	// set value
	if( id >= 0 ) {    // update
		sql = FMT(
			"update %s set %s = ?, %s = ?, %s = ?, %s = ?, %s = ?, %s = ?, %s = ?, %s = ? where %s = ?;",
			TABLE_PEAK_VAL,
			COL_VALUE_PEAK_VAL,
			COL_NORMALIZED_PEAK_VAL,
			COL_POS_PEAK_VAL,
			COL_LX_PEAK_VAL,
			COL_LY_PEAK_VAL,
			COL_RX_PEAK_VAL,
			COL_RY_PEAK_VAL,
			COL_PROFILE_PEAK_VAL,
			COL_ID_PEAK_VAL
		);
	}
	else {    // insert
		sql = FMT(
			"insert into %s (%s, %s, %s, %s, %s, %s, %s, %s, %s, %s) values (?, ?, ?, ?, ?, ?, ?, ?, ?, ?);",
			TABLE_PEAK_VAL,
			COL_VALUE_PEAK_VAL,
			COL_NORMALIZED_PEAK_VAL,
			COL_POS_PEAK_VAL,
			COL_LX_PEAK_VAL,
			COL_LY_PEAK_VAL,
			COL_RX_PEAK_VAL,
			COL_RY_PEAK_VAL,
			COL_PROFILE_PEAK_VAL,
			COL_PEAK_PEAK_VAL,
			COL_SAMPLE_PEAK_VAL
		);
	}

	stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		if( intensity >= 0.0 ) {
			stmt->bindReal( 1, intensity );
			stmt->bindReal( 2, intensity );
		}
		else {
			stmt->bindReal( 1, -1.0 );
			stmt->bindReal( 2, -1.0 );
		}

		if( peak == NULL ) {
			stmt->bindReal( 3, -1.0 );
			stmt->bindReal( 4, -1.0 );
			stmt->bindReal( 5, -1.0 );
			stmt->bindReal( 6, -1.0 );
			stmt->bindReal( 7, -1.0 );
		}
		else {
			stmt->bindReal( 3, peak->getX() );
			stmt->bindReal( 4, peak->getLeftX() );
			stmt->bindReal( 5, peak->getLeftY() );
			stmt->bindReal( 6, peak->getRightX() );
			stmt->bindReal( 7, peak->getRightY() );
		}

		if( profile >= 0 ) {
			stmt->bindInteger( 8, profile );
		}
		else {
			stmt->bindInteger( 8, -1 );
		}

		if( id >= 0 ) {    // update
			stmt->bindInteger( 9, id );
		}
		else {    // insert
			stmt->bindInteger( 9, matrixPeak );
			stmt->bindInteger( 10, matrixSample );
		}

		stmt->execute();
		stmt->close();
	}
}

// set normalized intensity
void IdentificationManager::setNormalizedIntensity( const int matrixPeak, const int matrixSample, const double normalized ) {
	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return;
	}

	// search
	std::string sql = FMT(
		"select %s from %s where %s = ? and %s = ?;",
		COL_ID_PEAK_VAL,
		TABLE_PEAK_VAL,
		COL_PEAK_PEAK_VAL,
		COL_SAMPLE_PEAK_VAL
	);

	int id = -1;

	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindInteger( 1, matrixPeak );
		stmt->bindInteger( 2, matrixSample );

		kome::db::ResultSet* rs = stmt->executeQuery();
		if( rs != NULL ) {
			if( rs->next() ) {
				id = rs->getInt( 0, -1 );
			}
			rs->close();
		}
		stmt->close();
	}

	if( id < 0 ) {
		return;
	}

	// set value
	sql = FMT(
		"update %s set %s = ? where %s = ?;",
		TABLE_PEAK_VAL,
		COL_NORMALIZED_PEAK_VAL,
		COL_ID_PEAK_VAL
	);

	stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		if( normalized >= 0.0 ) {
			stmt->bindReal( 1, normalized );
		}
		else {
			stmt->bindReal( 1, -1.0 );
		}

		stmt->execute();
		stmt->close();
	}
}

// delete peak values from matrix peak
void IdentificationManager::deletePeakValuesFromMatrixPeak( const int matrixPeak ) {
	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return;
	}

	// profile
	std::string sql0 = FMT(
		"select distinct %s from %s where %s = ?;",
		COL_PROFILE_PEAK_VAL,
		TABLE_PEAK_VAL,
		COL_PEAK_PEAK_VAL
	);

	std::set< int > profileSet;
	kome::db::Statement* stmt = db->prepare( sql0.c_str() );
	if( stmt != NULL ) {
		stmt->bindInteger( 1, matrixPeak );
		kome::db::ResultSet* rs = stmt->executeQuery();
		if( rs != NULL ) {
			while( rs->next() ) {
				const int profile = rs->getInt( 0, -1 );
				if( profile >= 0 ) {
					profileSet.insert( profile );
				}
			}
			rs->close();
		}
		stmt->close();
	}

	// delete
	std::string sql = FMT(
		"delete from %s where %s = ?;",
		TABLE_PEAK_VAL,
		COL_PEAK_PEAK_VAL
	);

	stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindInteger( 1, matrixPeak );
		stmt->execute();
		stmt->close();
	}

	// delete profiles
	stmt = db->prepare( sql0.c_str() );
	if( stmt != NULL ) {
		stmt->bindInteger( 1, matrixPeak );
		kome::db::ResultSet* rs = stmt->executeQuery();
		if( rs != NULL ) {
			while( rs->next() ) {
				const int profile = rs->getInt( 0, -1 );
				if( profile >= 0 ) {
					profileSet.erase( profile );
				}
			}
			rs->close();
		}
		stmt->close();
	}

	for( std::set< int >::iterator it = profileSet.begin(); it != profileSet.end(); it++ ) {
		const int profile = *it;
		deleteProfile( profile );
	}
}

// delete peak values from matrix sample
void IdentificationManager::deletePeakValuesFromMatrixSample( const int matrixSample ) {
	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return;
	}

	// profile
	std::string sql0 = FMT(
		"select distinct %s from %s where %s = ?;",
		COL_PROFILE_PEAK_VAL,
		TABLE_PEAK_VAL,
		COL_SAMPLE_PEAK_VAL
	);

	std::set< int > profileSet;
	kome::db::Statement* stmt = db->prepare( sql0.c_str() );
	if( stmt != NULL ) {
		stmt->bindInteger( 1, matrixSample );
		kome::db::ResultSet* rs = stmt->executeQuery();
		if( rs != NULL ) {
			while( rs->next() ) {
				const int profile = rs->getInt( 0, -1 );
				if( profile >= 0 ) {
					profileSet.insert( profile );
				}
			}
			rs->close();
		}
		stmt->close();
	}

	// delete
	std::string sql = FMT(
		"delete from %s where %s = ?;",
		TABLE_PEAK_VAL,
		COL_SAMPLE_PEAK_VAL
	);

	stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindInteger( 1, matrixSample );

		stmt->execute();
	}

	// delete profiles
	stmt = db->prepare( sql0.c_str() );
	if( stmt != NULL ) {
		stmt->bindInteger( 1, matrixSample );
		kome::db::ResultSet* rs = stmt->executeQuery();
		if( rs != NULL ) {
			while( rs->next() ) {
				const int profile = rs->getInt( 0, -1 );
				if( profile >= 0 ) {
					profileSet.erase( profile );
				}
			}
			rs->close();
		}
		stmt->close();
	}

	for( std::set< int >::iterator it = profileSet.begin(); it != profileSet.end(); it++ ) {
		const int profile = *it;
		deleteProfile( profile );
	}
}

// get peak value
double IdentificationManager::getPeakValue( const int matrixPeak, const int matrixSample ) {
	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return -1.0;
	}

	// get value
	std::string sql = FMT(
		"select %s from %s where %s = ? and %s = ?;",
		COL_VALUE_PEAK_VAL,
		TABLE_PEAK_VAL,
		COL_PEAK_PEAK_VAL,
		COL_SAMPLE_PEAK_VAL
	);

	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	double val = -1.0;
	if( stmt != NULL ) {
		stmt->bindInteger( 1, matrixPeak );
		stmt->bindInteger( 2, matrixSample );

		kome::db::ResultSet* rs = stmt->executeQuery();
		if( rs != NULL ) {
			if( rs->next() ) {
				val = rs->getReal( 0, -1.0 );
			}
			rs->close();
		}
		stmt->close();
	}

	return val;
}

// get normalized peak value
double IdentificationManager::getNormalizedPeakValue( const int matrixPeak, const int matrixSample ) {
	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return -1.0;
	}

	// get value
	std::string sql = FMT(
		"select %s from %s where %s = ? and %s = ?;",
		COL_NORMALIZED_PEAK_VAL,
		TABLE_PEAK_VAL,
		COL_PEAK_PEAK_VAL,
		COL_SAMPLE_PEAK_VAL
	);

	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	double val = -1.0;
	if( stmt != NULL ) {
		stmt->bindInteger( 1, matrixPeak );
		stmt->bindInteger( 2, matrixSample );

		kome::db::ResultSet* rs = stmt->executeQuery();
		if( rs != NULL ) {
			if( rs->next() ) {
				val = rs->getReal( 0, -1.0 );
			}
			rs->close();
		}
		stmt->close();
	}

	return val;
}

// has blank value
bool IdentificationManager::hasBlankValue( const int matrixId ) {
	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return false;
	}

	// number of peaks
	std::string sql = FMT(
		"select count(*) from %s where %s = ?;",
		TABLE_PEAK,
		COL_MATRIX_PEAK
	);

	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	int pNum = 0;
	if( stmt != NULL ) {
		stmt->bindInteger( 1, matrixId );

		kome::db::ResultSet* rs = stmt->executeQuery();
		if( rs != NULL ) {
			if( rs->next() ) {
				pNum = rs->getInt( 0, 0 );
			}
			rs->close();
		}
		stmt->close();
	}

	// number of samples
	sql = FMT(
		"select count(*) from %s as s, %s as g where s.%s = g.%s and g.%s = ?;",
		TABLE_MATRIX_SAMPLE,
		TABLE_GROUP,
		COL_GROUP_MATRIX_SAMPLE,
		COL_ID_GROUP,
		COL_MATRIX_GROUP
	);

	stmt = db->prepare( sql.c_str() );
	int sNum = 0;
	if( stmt != NULL ) {
		stmt->bindInteger( 1, matrixId );

		kome::db::ResultSet* rs = stmt->executeQuery();
		if( rs != NULL ) {
			if( rs->next() ) {
				sNum = rs->getInt( 0, 0 );
			}
			rs->close();
		}
		stmt->close();
	}

	// get value
	sql = FMT(
		"select count(*) from %s as v, %s as p where v.%s = p.%s and p.%s = ? and v.%s >= 0.0;",
		TABLE_PEAK_VAL,
		TABLE_PEAK,
		COL_PEAK_PEAK_VAL,
		COL_ID_PEAK,
		COL_MATRIX_PEAK,
		COL_NORMALIZED_PEAK_VAL
	);

	stmt = db->prepare( sql.c_str() );
	bool ret = true;
	if( stmt != NULL ) {
		stmt->bindInteger( 1, matrixId );

		kome::db::ResultSet* rs = stmt->executeQuery();
		if( rs != NULL ) {
			if( rs->next() ) {
				ret = ( rs->getInt( 0, 0 ) < pNum * sNum );
			}
			rs->close();
		}
		stmt->close();
	}

	return ret;
}

// the column of the sample is empty or not
bool IdentificationManager::isEmptySample( const int matrixSample ) {
	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return true;
	}

	// get value
	std::string sql = FMT(
		"select count(*) from %s where %s = ? and %s >= 0.0;",
		TABLE_PEAK_VAL,
		COL_SAMPLE_PEAK_VAL,
		COL_PEAK_PEAK_VAL
	);

	bool ret = true;
	kome::db::Statement* stmt = db->prepare( sql.c_str() );

	if( stmt != NULL ) {
		stmt->bindInteger( 1, matrixSample );

		kome::db::ResultSet* rs = stmt->executeQuery();
		if( rs != NULL ) {
			if( rs->next() ) {
				ret = ( rs->getInt( 0, 0 ) <= 0 );
			}
			rs->close();
		}
		stmt->close();
	}

	return ret;
}

// get peak
void IdentificationManager::getPeakInfo( const int matrixPeak, const int matrixSample, kome::objects::PeakElement* peak ) {
	// parameters
	if( peak == NULL ) {
		return;
	}

	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return;
	}

	// get value
	std::string sql = FMT(
		"select %s, %s, %s, %s, %s from %s where %s = ? and %s = ?;",
		COL_POS_PEAK_VAL,
		COL_LX_PEAK_VAL,
		COL_LY_PEAK_VAL,
		COL_RX_PEAK_VAL,
		COL_RY_PEAK_VAL,
		TABLE_PEAK_VAL,
		COL_PEAK_PEAK_VAL,
		COL_SAMPLE_PEAK_VAL
	);

	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	double val = -1.0;
	if( stmt != NULL ) {
		stmt->bindInteger( 1, matrixPeak );
		stmt->bindInteger( 2, matrixSample );

		kome::db::ResultSet* rs = stmt->executeQuery();
		if( rs != NULL ) {
			if( rs->next() ) {
				const double pos = rs->getReal( 0, -1.0 );
				const double lx = rs->getReal( 1, -1.0 );
				const double ly = rs->getReal( 2, -1.0 );
				const double rx = rs->getReal( 3, -1.0 );
				const double ry = rs->getReal( 4, -1.0 );

				if( pos >= 0.0 ) {
					peak->setX( pos );
				}
				if( lx >= 0.0 && ly >= 0.0 ) {
					peak->setLeft( lx, ly );
				}
				if( rx >= 0.0 && ry >= 0.0 ) {
					peak->setRight( rx, ry );
				}
			}
			rs->close();
		}
		stmt->close();
	}
}

// get profile
int IdentificationManager::getProfile( const int matrixPeak, const int matrixSample ) {
	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return -1;
	}

	// get value
	std::string sql = FMT(
		"select %s from %s where %s = ? and %s = ?;",
		COL_PROFILE_PEAK_VAL,
		TABLE_PEAK_VAL,
		COL_PEAK_PEAK_VAL,
		COL_SAMPLE_PEAK_VAL
	);

	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	int val = -1;
	if( stmt != NULL ) {
		stmt->bindInteger( 1, matrixPeak );
		stmt->bindInteger( 2, matrixSample );

		kome::db::ResultSet* rs = stmt->executeQuery();
		if( rs != NULL ) {
			if( rs->next() ) {
				val = rs->getInt( 0, -1 );
			}
			rs->close();
		}
		stmt->close();
	}

	return val;
}

// get spectrum information
void IdentificationManager::getSpecInfo( kome::objects::Spectrum& spec, SpecInfo& specInfo ) {
	// sample
	kome::objects::Sample* smpl = spec.getSample();
	kome::objects::SampleSet* ss = ( smpl == NULL ? NULL : smpl->getSampleSet() );

	// structure
	specInfo.specId = spec.getId();
	specInfo.sampleIndex = ( smpl == NULL ? -1 : smpl->getSampleIndex() );
	specInfo.sampleName = ( smpl == NULL ? "" : smpl->getName() );
	specInfo.path = ( ss == NULL ? "" : ss->getFilePath() );
	specInfo.retentionTime = spec.getRt();
	specInfo.precursor = spec.getPrecursor();
	specInfo.name = spec.getName();
	specInfo.stage = spec.getMsStage();
	specInfo.polarity = spec.getPolarity();
}

// add spectrum
int IdentificationManager::addSpectrum( kome::objects::Spectrum* spec ) {
	// parameter
	if( spec == NULL ) {
		return -1;
	}

	// SpecInfo
	SpecInfo specInfo;
	getSpecInfo( *spec, specInfo );

	// add
	return addSpectrum( specInfo );
}

// add spectrum
int IdentificationManager::addSpectrum( SpecInfo& spec ) {
	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return -1;
	}

	// sample
	int sId = addSample( spec.path.c_str(), spec.sampleIndex, spec.sampleName.c_str() );
	if( sId < 0 ) {
		return -1;
	}

	// search
	std::string sql = FMT(
		"select %s from %s where %s = ? and %s = ?;",
		COL_ID_SPECTRUM,
		TABLE_SPECTRUM,
		COL_SAMPLE_SPECTRUM,
		COL_SPECTRUM_ID_SPECTRUM
	);

	int id = -1;
	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindInteger( 1, sId );
		stmt->bindInteger( 2, spec.specId );
		kome::db::ResultSet* rs = stmt->executeQuery();
		if( rs != NULL ) {
			if( rs->next() ) {
				id = rs->getInt( 0, -1 );
			}
			rs->close();
		}
		stmt->close();
	}
	if( id >= 0 ) {
		spec.id = id;
		return id;
	}

	// add
	bool ret = false;
	sql = FMT(
		"insert into %s (%s, %s, %s, %s, %s, %s, %s) values(?, ?, ?, ?, ?, ?, ?);",
		TABLE_SPECTRUM,
		COL_SAMPLE_SPECTRUM,
		COL_SPECTRUM_ID_SPECTRUM,
		COL_NAME_SPECTRUM,
		COL_STAGE_SPECTRUM,
		COL_RT_SPECTRUM,
		COL_PRECURSOR_SPECTRUM,
		COL_POLARITY_SPECTRUM
	);

	stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindInteger( 1, sId );
		stmt->bindInteger( 2, spec.specId );
		stmt->bindString( 3, spec.name.c_str() );
		stmt->bindInteger( 4, spec.stage );
		stmt->bindReal( 5, spec.retentionTime );
		stmt->bindReal( 6, spec.precursor );
		stmt->bindInteger( 7, (int)spec.polarity );

		ret = stmt->execute();
		stmt->close();
	}

	if( ret ) {
		id = db->getMaxValue( TABLE_SPECTRUM, COL_ID_SPECTRUM );
		if( id < 0 ) {
			id = -1;
		}
		spec.id = id;
	}

	return id;
}

// delete spectrum
void IdentificationManager::deleteSpectrum( const int id ) {
	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return;
	}

	// delete search spectra
	std::vector< int > spectra;
	getSearchSpectraFromSpectrum( spectra, id );
	for( unsigned int i = 0; i < spectra.size(); i++ ) {
		deleteSearchSpectrum( spectra[ i ] );
	}

	// delete
	std::string sql = FMT(
		"delete from %s where %s = ?;",
		TABLE_SPECTRUM,
		COL_ID_SPECTRUM
	);

	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindInteger( 1, id );

		stmt->execute();
		stmt->close();
	}
}

// get spectra
void IdentificationManager::getSpectra( std::vector< int >& spectra, const int sampleId ) {
	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return;
	}

	// get spectra
	std::string sql = FMT(
		"select %s from %s where %s = ?;",
		COL_ID_SPECTRUM,
		TABLE_SPECTRUM,
		COL_SAMPLE_SPECTRUM
	);

	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindInteger( 1, sampleId );
		kome::db::ResultSet* rs = stmt->executeQuery();
		if( rs != NULL ) {
			while( rs->next() ) {
				const int id = rs->getInt( 0, -1 );
				if( id >= 0 ) {
					spectra.push_back( id );
				}
			}
			rs->close();
		}
		stmt->close();
	}
}

// get spectrum
void IdentificationManager::getSpectrum( const int id, SpecInfo& spec ) {
	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return;
	}

	// set ID
	spec.id = id;

	// get spectra
	std::string sql = FMT(
		"select %s, %s, %s, %s, %s, %s, %s from %s where %s = ?;",
		COL_SAMPLE_SPECTRUM,
		COL_SPECTRUM_ID_SPECTRUM,
		COL_NAME_SPECTRUM,
		COL_STAGE_SPECTRUM,
		COL_RT_SPECTRUM,
		COL_PRECURSOR_SPECTRUM,
		COL_POLARITY_SPECTRUM,
		TABLE_SPECTRUM,
		COL_ID_SPECTRUM
	);

	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindInteger( 1, id );
		kome::db::ResultSet* rs = stmt->executeQuery();
		if( rs != NULL ) {
			if( rs->next() ) {
				const int sample = rs->getInt( 0, -1 );
				spec.path = getSampleFilePath( sample );
				spec.sampleIndex = getSampleIndex( sample );
				spec.sampleName = getSampleName( sample );
				spec.specId = rs->getInt( 1, -1 );
				spec.name = rs->getString( 2, "" );
				spec.stage = rs->getInt( 3, 0 );
				spec.retentionTime = rs->getReal( 4, -1.0 );
				spec.precursor = rs->getReal( 5, -1.0 );
				spec.polarity = (kome::objects::Spectrum::Polarity)rs->getInt( 6, 0 );
			}
			rs->close();
		}
		stmt->close();
	}
}

// add database
int IdentificationManager::addDatabase( const char* name, const char* url ) {
	// DB
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return -1;
	}

	// get database ID
	int id = getDatabaseId( name );

	// add
	if( id >= 0 ) {    // update
		std::string sql = FMT(
			"update %s set %s = ? where %s = ?;",
			TABLE_DATABASE,
			COL_URL_DATABASE,
			COL_ID_DATABASE
		);

		kome::db::Statement* stmt = db->prepare( sql.c_str() );
		if( stmt != NULL ) {
			stmt->bindString( 1, NVL( url, "" ) );
			stmt->bindInteger( 2, id );

			stmt->execute();
			stmt->close();
		}

		return id;
	}

	bool ret = false;
	std::string sql = FMT(
		"insert into %s (%s, %s) values(?, ?);",
		TABLE_DATABASE,
		COL_NAME_DATABASE,
		COL_URL_DATABASE
	);

	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindString( 1, NVL( name, "" ) );
		stmt->bindString( 2, NVL( url, "" ) );

		ret = stmt->execute();
		stmt->close();
	}

	if( ret ) {
		id = db->getMaxValue( TABLE_DATABASE, COL_ID_DATABASE );
		if( id < 0 ) {
			id = -1;
		}
	}

	return id;
}

// get database ID
int IdentificationManager::getDatabaseId( const char* name ) {
	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return -1;
	}

	// get ID
	std::string sql = FMT(
		"select %s from %s where %s = ?;",
		COL_ID_DATABASE,
		TABLE_DATABASE,
		COL_NAME_DATABASE
	);

	int id = -1;
	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindString( 1, NVL( name, "" ) );
		kome::db::ResultSet* rs = stmt->executeQuery();
		if( rs != NULL ) {
			if( rs->next() ) {
				id = rs->getInt( 0, -1 );
			}
			rs->close();
		}
		stmt->close();
	}

	return id;
}

// delete database
void IdentificationManager::deleteDatabase( const int id ) {
	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return;
	}

	// delete format url
	deleteFormatUrl( id );

	// delete substance items
	deleteSubstanceItem( id );

	// substances
	std::vector< int > substances;
	getSubstances( substances, id );
	for( unsigned int i = 0; i < substances.size(); i++ ) {
		deleteSubstance( substances[ i ] );
	}

	// searches
	std::vector< int > searches;
	getSearchesFromDb( searches, id );
	for( unsigned int i = 0; i < searches.size(); i++ ) {
		deleteSearch( searches[ i ] );
	}

	// delete
	std::string sql = FMT(
		"delete from %s where %s = ?;",
		TABLE_DATABASE,
		COL_ID_DATABASE
	);

	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindInteger( 1, id );

		stmt->execute();
		stmt->close();
	}
}

// get database IDs
void IdentificationManager::getDatabases( std::vector< int >& databases ) {
	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return;
	}

	// databases
	std::string sql = FMT( "select %s from %s;", COL_ID_DATABASE, TABLE_DATABASE );
	kome::db::ResultSet* rs = db->executeQuery( sql.c_str() );
	if( rs != NULL ) {
		while( rs->next() ) {
			const int id = rs->getInt( 0, -1 );
			if( id >= 0 ) {
				databases.push_back( id );
			}
		}
		rs->close();
	}
}

// get database name
std::string IdentificationManager::getDatabaseName( const int id ) {
	// name
	std::string s;

	// database
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return s;
	}

	std::string sql = FMT(
		"select %s from %s where %s = ?;",
		COL_NAME_DATABASE,
		TABLE_DATABASE,
		COL_ID_DATABASE
	);

	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindInteger( 1, id );
		kome::db::ResultSet* rs = stmt->executeQuery();
		if( rs != NULL ) {
			if( rs->next() ) {
				s = rs->getString( 0, "" );
			}
			rs->close();
		}
		stmt->close();
	}

	return s;
}

// get database URL
std::string IdentificationManager::getDatabaseUrl( const int id ) {
	// url
	std::string s;

	// database
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return s;
	}

	std::string sql = FMT(
		"select %s from %s where %s = ?;",
		COL_URL_DATABASE,
		TABLE_DATABASE,
		COL_ID_DATABASE
	);

	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindInteger( 1, id );
		kome::db::ResultSet* rs = stmt->executeQuery();
		if( rs != NULL ) {
			if( rs->next() ) {
				s = rs->getString( 0, "" );
			}
			rs->close();
		}
		stmt->close();
	}

	return s;
}

// get format ID
int IdentificationManager::getFormatId( const char* format ) {
	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return -1;
	}

	// string
	std::string f = tolowercase( format );

	// search
	int fmtId = -1;
	std::string sql = FMT(
		"select %s from %s where %s = ?;",
		COL_ID_FORMAT,
		TABLE_DATA_FORMAT,
		COL_NAME_FORMAT
	);

	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindString( 1, f.c_str() );
		kome::db::ResultSet* rs = stmt->executeQuery();
		if( rs != NULL ) {
			if( rs->next() ) {
				fmtId = rs->getInt( 0, -1 );
			}
			rs->close();
		}
		stmt->close();
	}

	return fmtId;
}

// set format URL
void IdentificationManager::setFormatUrl( const int dbId, const char* format, const char* url ) {
	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return;
	}

	// string
	std::string f = tolowercase( format );

	// search
	int fmtId = getFormatId( f.c_str() );

	// add format
	if( fmtId < 0 ) {
		std::string sql = FMT(
			"insert into %s (%s) values(?);",
			TABLE_DATA_FORMAT,
			COL_NAME_FORMAT
		);

		bool ret = false;
		kome::db::Statement* stmt = db->prepare( sql.c_str() );
		if( stmt != NULL ) {
			stmt->bindString( 1, f.c_str() );

			ret = stmt->execute();
			stmt->close();
		}

		if( ret ) {
			fmtId = db->getMaxValue( TABLE_DATA_FORMAT, COL_ID_FORMAT );
			if( fmtId < 0 ) {
				fmtId = -1;
			}
		}
	}

	if( fmtId < 0 ) {
		return;
	}

	// search URL
	std::string sql = FMT(
		"select %s from %s where %s = ? and %s = ?;",
		COL_ID_URL,
		TABLE_URL,
		COL_DB_URL,
		COL_FORMAT_URL
	);

	int id = -1;

	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindInteger( 1, dbId );
		stmt->bindInteger( 2, fmtId );

		kome::db::ResultSet* rs = stmt->executeQuery();
		if( rs != NULL ) {
			if( rs->next() ) {
				id = rs->getInt( 0, -1 );
			}
			rs->close();
		}
		stmt->close();
	}

	// add URL
	if( id < 0 ) {    // insert
		sql = FMT(
			"insert into %s (%s, %s, %s) values(?, ?, ?);",
			TABLE_URL,
			COL_DB_URL,
			COL_FORMAT_URL,
			COL_URL_URL
		);

		stmt = db->prepare( sql.c_str() );
		if( stmt != NULL ) {
			stmt->bindInteger( 1, dbId );
			stmt->bindInteger( 2, fmtId );
			stmt->bindString( 3, NVL( url, "" ) );

			stmt->execute();
			stmt->close();
		}
	}
	else {    // update
		sql = FMT(
			"update %s set %s = ? where %s = ?;", 
			TABLE_URL,
			COL_URL_URL,
			COL_ID_URL
		);

		stmt = db->prepare( sql.c_str() );
		if( stmt != NULL ) {
			stmt->bindString( 1, NVL( url, "" ) );
			stmt->bindInteger( 2, id );

			stmt->execute();
			stmt->close();
		}
	}
}

// delete format
void IdentificationManager::deleteFormat( const char* format ) {
	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return;
	}

	// format ID
	int fmtId = getFormatId( format );
	if( fmtId < 0 ) {
		return;
	}

	// delete from URL
	std::string sql = FMT(
		"delete from %s where %s = ?;"
		TABLE_URL,
		COL_FORMAT_URL
	);

	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindInteger( 1, fmtId );

		stmt->execute();
		stmt->close();
	}

	// delete from format
	sql = FMT(
		"delete from %s where %s = ?;",
		TABLE_DATA_FORMAT,
		COL_ID_FORMAT
	);

	stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindInteger( 1, fmtId );

		stmt->execute();
		stmt->close();
	}
}

// delete format
void IdentificationManager::deleteFormatUrl( const int dbId, const char* format ) {
	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return;
	}

	// format ID
	int fmtId = getFormatId( format );
	if( fmtId < 0 ) {
		return;
	}

	// delete from URL
	std::string sql = FMT(
		"delete from %s where %s = ? and %s = ?;",
		TABLE_URL,
		COL_DB_URL,
		COL_FORMAT_URL
	);

	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindInteger( 1, dbId );
		stmt->bindInteger( 2, fmtId );

		stmt->execute();
		stmt->close();
	}
}

// delete format
void IdentificationManager::deleteFormatUrl( const int dbId ) {
	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return;
	}

	// delete from URL
	std::string sql = FMT(
		"delete from %s where %s = ?;",
		TABLE_URL,
		COL_DB_URL,
		COL_FORMAT_URL
	);

	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindInteger( 1, dbId );

		stmt->execute();
		stmt->close();
	}
}

// get format URL
std::string IdentificationManager::getFormatUrl( const int dbId, const char* format ) {
	// string
	std::string s;
	std::string f = tolowercase( format );

	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return s;
	}

	// get URL
	std::string sql = FMT(
		"select u.%s from %s as u, %s as f where u.%s = f.%s and u.%s = ? and f.%s = ?;",
		COL_URL_URL,
		TABLE_URL,
		TABLE_DATA_FORMAT,
		COL_FORMAT_URL,
		COL_ID_FORMAT,
		COL_DB_URL,
		COL_NAME_FORMAT
	);

	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindInteger( 1, dbId );
		stmt->bindString( 2, f.c_str() );

		kome::db::ResultSet* rs = stmt->executeQuery();
		if( rs != NULL ) {
			if( rs->next() ) {
				s = rs->getString( 0, "" );
			}
			rs->close();
		}
		stmt->close();
	}

	return s;
}

// get format list
void IdentificationManager::getFormatList( std::vector< std::string >& formats, const int dbId ) {
	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return;
	}

	// formats
	std::string sql = FMT( 
		"select f.%s from %s as u, %s asf where u.%s = f.%s and u.%s = ?;",
		COL_NAME_FORMAT,
		TABLE_URL,
		TABLE_DATA_FORMAT,
		COL_FORMAT_URL,
		COL_ID_FORMAT,
		COL_DB_URL
	);

	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindInteger( 1, dbId );

		kome::db::ResultSet* rs = stmt->executeQuery();
		if( rs != NULL ) {
			while( rs->next() ) {
				std::string fmt = rs->getString( 0, "" );
				if( !fmt.empty() ) {
					formats.push_back( fmt );
				}
			}
			rs->close();
		}
		stmt->close();
	}
}

// add substance item
int IdentificationManager::addSubstanceItem( const int dbId, const char* name, const SubstanceType type ) {
	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return -1;
	}

	// search
	std::string sql = FMT(
		"select %s from %s where %s = ? and %s = ? and %s = ?;",
		COL_ID_SUBSTANCE_ITEM,
		TABLE_SUBSTANCE_ITEM,
		COL_DB_SUBSTANCE_ITEM,
		COL_NAME_SUBSTANCE_ITEM,
		COL_TYPE_SUBSTANCE_ITEM
	);

	int id = -1;
	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindInteger( 1, dbId );
		stmt->bindString( 2, NVL( name, "" ) );
		stmt->bindInteger( 3, (int)type );

		kome::db::ResultSet* rs = stmt->executeQuery();
		if( rs != NULL ) {
			if( rs->next() ) {
				id = rs->getInt( 0, -1 );
			}
			rs->close();
		}
		stmt->close();
	}

	if( id >= 0 ) {
		return id;
	}

	// add
	sql = FMT(
		"insert into %s (%s, %s, %s) values(?, ?, ?);",
		TABLE_SUBSTANCE_ITEM,
		COL_DB_SUBSTANCE_ITEM,
		COL_NAME_SUBSTANCE_ITEM,
		COL_TYPE_SUBSTANCE_ITEM

	);

	bool ret = false;
	stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindInteger( 1, dbId );
		stmt->bindString( 2, NVL( name, "" ) );
		stmt->bindInteger( 3, (int)type );

		ret = stmt->execute();
		stmt->close();
	}

	// id
	if( ret ) {
		id = db->getMaxValue( TABLE_SUBSTANCE_ITEM, COL_ID_SUBSTANCE_ITEM );
		if( id < 0 ) {
			id = -1;
		}
	}

	return id;
}

// delete substance item
void IdentificationManager::deleteSubstanceItem( const int dbId ) {
	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return;
	}

	// delete
	std::string sql = FMT(
		"delete from %s where %s = ?;",
		TABLE_SUBSTANCE_ITEM,
		COL_DB_SUBSTANCE_ITEM
	);

	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindInteger( 1, dbId );
		stmt->execute();
		stmt->close();
	}
}

// set substance value
void IdentificationManager::setSubstanceValue( const int substanceId, const int itemId, const char* val ) {
	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return;
	}

	// search
	std::string sql =FMT(
		"select %s from %s where %s = ? and %s = ?;",
		COL_ID_SUBSTANCE_VAL,
		TABLE_SUBSTANCE_VALUE,
		COL_SUBSTANCE_SUBSTANCE_VAL,
		COL_ITEM_SUBSTANCE_VAL
	);

	int id = -1;
	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindInteger( 1, substanceId );
		stmt->bindInteger( 2, itemId );

		kome::db::ResultSet* rs = stmt->executeQuery();
		if( rs != NULL ) {
			if( rs->next() ) {
				id = rs->getInt( 0, -1 );
			}
			rs->close();
		}
		stmt->close();
	}

	// add
	if( id < 0 ) {    // insert
		sql = FMT(
			"insert into %s (%s, %s, %s) values (?, ?, ?);",
			TABLE_SUBSTANCE_VALUE,
			COL_SUBSTANCE_SUBSTANCE_VAL,
			COL_ITEM_SUBSTANCE_VAL,
			COL_VALUE_SUBSTANCE_VAL
		);

		stmt = db->prepare( sql.c_str() );
		bool ret = false;
		if( stmt != NULL ) {
			stmt->bindInteger( 1, substanceId );
			stmt->bindInteger( 2, itemId );
			stmt->bindString( 3, NVL( val, "" ) );
			stmt->execute();

			stmt->close();
		}
	}
	else {    // update
		sql = FMT(
			"update %s set %s = ? where %s = ?;",
			TABLE_SUBSTANCE_VALUE,
			COL_VALUE_SUBSTANCE_VAL,
			COL_ID_SUBSTANCE_VAL
		);

		stmt = db->prepare( sql.c_str() );
		if( stmt != NULL ) {
			stmt->bindString( 1, NVL( val, "" ) );
			stmt->bindInteger( 2, id );
			stmt->execute();

			stmt->close();
		}
	}
}

// delete substance value
void IdentificationManager::deleteSubstanceValue( const int substanceId ) {
	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return;
	}

	// delete
	std::string sql = FMT(
		"delete from %s where %s = ?;",
		TABLE_SUBSTANCE_VALUE,
		COL_SUBSTANCE_SUBSTANCE_VAL
	);

	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindInteger( 1, substanceId );
		stmt->execute();
		stmt->close();
	}
}

// add substance
int IdentificationManager::addSubstance( SubstanceInfo& substance ) {
	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return -1;
	}

	// id
	int id = getSubstanceId(
		substance.dbId,
		substance.accession.c_str(),
		substance.name.c_str(),
		substance.parentId
	);

	// add
	if( id < 0 ) {    // insert
		std::string sql = FMT(
			"insert into %s (%s, %s, %s, %s) values(?, ?, ?, ?);",
			TABLE_SUBSTANCE,
			COL_ACCESSION_SUBSTANCE,
			COL_NAME_SUBSTANCE,
			COL_DATABASE_SUBSTANCE,
			COL_TYPE_SUBSTANCE
	    );

		kome::db::Statement* stmt = db->prepare( sql.c_str() );
		bool ret = false;
		if( stmt != NULL ) {
			stmt->bindString( 1, substance.accession.c_str() );
			stmt->bindString( 2, substance.name.c_str() );
			stmt->bindInteger( 3, substance.dbId );
			stmt->bindInteger( 4, (int)substance.type );

			ret = stmt->execute();
		}

		// id
		if( ret ) {
			id = db->getMaxValue( TABLE_SUBSTANCE, COL_ID_SUBSTANCE );
			if( id < 0 ) {
				id = -1;
			}
		}
	}
	else {
		std::string sql = FMT(
			"update %s set %s = ?, %s = ? where %s = ?;",
			TABLE_SUBSTANCE,
			COL_NAME_SUBSTANCE,
			COL_TYPE_SUBSTANCE,
			COL_ID_SUBSTANCE
		);

		kome::db::Statement* stmt = db->prepare( sql.c_str() );
		if( stmt != NULL ) {
			stmt->bindString( 1, substance.name.c_str() );
			stmt->bindInteger( 2, (int)substance.type );
			stmt->bindInteger( 3, id );

			stmt->execute();
			stmt->close();
		}
	}

	if( id < 0 ) {
		return id;
	}

	// relationship
	if( substance.parentId >= 0 ) {
		std::string sql = FMT(
			"select %s from %s where %s = ? and %s = ?;",
			COL_ID_RELATIONSHIP,
			TABLE_RELATIONSHIP,
			COL_A_RELATIONSHIP,
			COL_B_RELATIONSHIP
		);

		int relId = -1;
		kome::db::Statement* stmt = db->prepare( sql.c_str() );
		if( stmt != NULL ) {
			stmt->bindInteger( 1, substance.parentId );
			stmt->bindInteger( 2, id );

			kome::db::ResultSet* rs = stmt->executeQuery();
			if( rs != NULL ) {
				if( rs->next() ) {
					relId = rs->getInt( 0, -1 );
				}
				rs->close();
			}
			stmt->close();
		}

		if( relId < 0 ) {    // insert
			sql = FMT(
				"insert into %s (%s, %s, %s) values(?, ?, ?);",
				TABLE_RELATIONSHIP,
				COL_A_RELATIONSHIP,
				COL_B_RELATIONSHIP,
				COL_RELATIONSHIP_RELATIONSHIP
			);

			stmt = db->prepare( sql.c_str() );
			if( stmt != NULL ) {
				stmt->bindInteger( 1, substance.parentId );
				stmt->bindInteger( 2, id );
				stmt->bindInteger( 3, (int)RELATIONSHIP_PARENT );

				stmt->execute();
				stmt->close();
			}
		}
		else {
			sql = FMT(
				"update %s set %s = ? where %s = ?;",
				TABLE_RELATIONSHIP,
				COL_RELATIONSHIP_RELATIONSHIP,
				COL_ID_RELATIONSHIP
			);

			stmt = db->prepare( sql.c_str() );
			if( stmt != NULL ) {
				stmt->bindInteger( 1, (int)RELATIONSHIP_PARENT );
				stmt->bindInteger( 2, relId );

				stmt->execute();
				stmt->close();
			}
		}
	}

	// properties
	for( unsigned int i = 0; i < substance.props.getNumberOfProperties(); i++ ) {
		std::string n = substance.props.getKey( i );
		const char* v = substance.props.getValue( i );

		int itemId = addSubstanceItem( substance.dbId, n.c_str(), substance.type );
		setSubstanceValue( id, itemId, NVL( v, "" ) );
	}

	return id;
}

// get substance ID
int IdentificationManager::getSubstanceId(
		const int dbId,
		const char* accession,
		const char* name,
		const int parentId
) {
	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return -1;
	}

	// string
	std::string a = NVL( accession, "" );
	std::string n = NVL( name, "" );

	// ID
	int id = -1;
	if( a.empty() ) {    // empty accession
		std::string sql = FMT(
			"select s.%s from %s as s, %s as r where s.%s = r.%s and s.%s = ? and s.%s = ? and s.%s = ? and r.%s = ? and r.%s = ?;",
			COL_ID_SUBSTANCE,
			TABLE_SUBSTANCE,
			TABLE_RELATIONSHIP,
			COL_ID_SUBSTANCE,
			COL_B_RELATIONSHIP,
			COL_DATABASE_SUBSTANCE,
			COL_ACCESSION_SUBSTANCE,
			COL_NAME_SUBSTANCE,
			COL_A_RELATIONSHIP,
			COL_RELATIONSHIP_RELATIONSHIP
		);

		kome::db::Statement* stmt = db->prepare( sql.c_str() );
		if( stmt != NULL ) {
			stmt->bindInteger( 1, dbId );
			stmt->bindString( 2, a.c_str() );
			stmt->bindString( 3, n.c_str() );
			stmt->bindInteger( 4, parentId );
			stmt->bindInteger( 5, (int)RELATIONSHIP_PARENT );

			kome::db::ResultSet* rs = stmt->executeQuery();
			if( rs != NULL ) {
				if( rs->next() ) {
					id = rs->getInt( 0, -1 );
				}
				rs->close();
			}
			stmt->close();
		}
	}
	else {
		std::string sql = FMT(
			"select %s from %s where %s = ? and %s = ?;",
			COL_ID_SUBSTANCE,
			TABLE_SUBSTANCE,
			COL_DATABASE_SUBSTANCE,
			COL_ACCESSION_SUBSTANCE
		);

		kome::db::Statement* stmt = db->prepare( sql.c_str() );
		if( stmt != NULL ) {
			stmt->bindInteger( 1, dbId );
			stmt->bindString( 2, a.c_str() );

			kome::db::ResultSet* rs = stmt->executeQuery();
			if( rs != NULL ) {
				if( rs->next() ) {
					id = rs->getInt( 0, -1 );
				}
				rs->close();
			}
			stmt->close();
		}
	}

	return id;
}

// delete substance
void IdentificationManager::deleteSubstance( const int id ) {
	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return;
	}

	// delete values
	deleteSubstanceValue( id );

	// delete alternative substances
	deleteAltSubstancesBySubstance( id );

	// delete hits
	std::vector< int > hits;
	getHitsFromSubstance( hits, id );
	for( unsigned int i = 0; i < hits.size(); i++ ) {
		deleteHit( hits[ i ] );
	}

	// delete relationships
	std::string sql = FMT(
		"delete from %s where %s = ? or %s = ?;",
		TABLE_RELATIONSHIP,
		COL_A_RELATIONSHIP,
		COL_B_RELATIONSHIP
	);

	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindInteger( 1, id );
		stmt->bindInteger( 2, id );

		stmt->execute();
		stmt->close();
	}
}

// get substances
void IdentificationManager::getSubstances( std::vector< int >& substances, const int dbId ) {
	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return;
	}

	// get substances
	std::string sql = FMT(
		"select %s from %s where %s = ? order by %s;",
		COL_ID_SUBSTANCE,
		TABLE_SUBSTANCE,
		COL_DATABASE_SUBSTANCE,
		COL_NAME_SUBSTANCE
	);

	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindInteger( 1, dbId );

		kome::db::ResultSet* rs = stmt->executeQuery();
		if( rs != NULL ) {
			while( rs->next() ) {
				const int id = rs->getInt( 0, -1 );
				if( id >= 0 ) {
					substances.push_back( id );
				}
			}
			rs->close();
		}
		stmt->close();
	}
}

// get substances
void IdentificationManager::getSubstances( std::vector< int >& substances ) {
	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return;
	}

	// get substances
	std::string sql = FMT(
		"select %s from %s order by %s;",
		COL_ID_SUBSTANCE,
		TABLE_SUBSTANCE,
		COL_NAME_SUBSTANCE
	);

	kome::db::ResultSet* rs = db->executeQuery( sql.c_str() );
	if( rs != NULL ) {
		while( rs->next() ) {
			const int id = rs->getInt( 0, -1 );
			if( id >= 0 ) {
				substances.push_back( id );
			}
		}
		rs->close();
	}
}

// get substance
void IdentificationManager::getSubstance( const int id, SubstanceInfo& substance ) {
	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return;
	}

	// subnstance
	std::string sql = FMT(
		"select %s, %s, %s, %s from %s where %s = ?;",
		COL_ACCESSION_SUBSTANCE,
		COL_NAME_SUBSTANCE,
		COL_DATABASE_SUBSTANCE,
		COL_TYPE_SUBSTANCE,
		TABLE_SUBSTANCE,
		COL_ID_SUBSTANCE
	);

	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	bool ret = false;
	if( stmt != NULL ) {
		stmt->bindInteger( 1, id );
		kome::db::ResultSet* rs = stmt->executeQuery();
		if( rs != NULL ) {
			if( rs->next() ) {
				ret = true;
				substance.accession = rs->getString( 0, "" );
				substance.name = rs->getString( 1, "" );
				substance.dbId = rs->getInt( 2, -1 );
				substance.type = (SubstanceType)rs->getInt( 3, (int)TYPE_UNKNOWN );
			}
			rs->close();
		}
	}

	if( !ret ) {
		return;
	}

	// parent
	substance.parentId = -1;

	if( substance.type != kome::ident::TYPE_PROTEIN ) {
		sql = FMT(
			"select %s from %s where %s = ? and %s = ?;",
			COL_A_RELATIONSHIP,
			TABLE_RELATIONSHIP,
			COL_B_RELATIONSHIP,
			COL_RELATIONSHIP_RELATIONSHIP
		);

		stmt = db->prepare( sql.c_str() );
		if( stmt != NULL ) {
			stmt->bindInteger( 1, id );
			stmt->bindInteger( 2, (int)RELATIONSHIP_PARENT );
			
			kome::db::ResultSet* rs = stmt->executeQuery();
			if( rs != NULL ) {
				if( rs->next() ) {
					substance.parentId = rs->getInt( 0, -1 );
				}
				rs->close();
			}
			stmt->close();
		}
	}

	// properties
	sql = FMT(
		"select i.%s, v.%s from %s as i, %s as v where i.%s = v.%s and v.%s = ?;",
		COL_NAME_SUBSTANCE_ITEM,
		COL_VALUE_SUBSTANCE_VAL,
		TABLE_SUBSTANCE_ITEM,
		TABLE_SUBSTANCE_VALUE,
		COL_ID_SUBSTANCE_ITEM,
		COL_ITEM_SUBSTANCE_VAL,
		COL_SUBSTANCE_SUBSTANCE_VAL
	);

	stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindInteger( 1, id );
		kome::db::ResultSet* rs = stmt->executeQuery();
		if( rs != NULL ) {
			while( rs->next() ) {
				std::string n = rs->getString( 0, "" );
				std::string v = rs->getString( 1, "" );

				substance.props.setValue( n.c_str(), v.c_str() );
			}
			rs->close();
		}
		stmt->close();
	}
}

// gets substance object
Substance* IdentificationManager::getSubstanceObject( const int id, SearchResult& result, std::map< int, Substance* >& substanceMap ) {
	// check the ID
	if( id < 0 ) {
		return NULL;
	}

	// check the map
	if( substanceMap.find( id ) != substanceMap.end() ) {
		return substanceMap[ id ];
	}

	// substance 
	SubstanceInfo subInfo;
	getSubstance( id, subInfo );

	// parent
	Substance* parent = getSubstanceObject( subInfo.parentId, result, substanceMap );

	Substance* substance = NULL;
	if( subInfo.type == (int)kome::ident::TYPE_PROTEIN ) {
		substance = new ProteinSubstance( &result, subInfo.accession.c_str(), subInfo.name.c_str(), parent );
	}
	else if( subInfo.type == (int)kome::ident::TYPE_PEPTIDE ) {
		substance = new PeptideSubstance( &result, subInfo.accession.c_str(), subInfo.name.c_str(),  parent );
	}
	else {
		substance = new Substance( &result, subInfo.accession.c_str(), subInfo.name.c_str(), (kome::ident::SubstanceType)subInfo.type, parent );
	}
	result.addSubstance( substance );
	
	for( unsigned int i = 0; i < subInfo.props.getNumberOfProperties(); i++ ) {
		substance->getProperties().setValue( subInfo.props.getKey( i ), subInfo.props.getValue( i ) );
	}

	substanceMap[ id ] = substance;

	return substance;
}

// add search engine
int IdentificationManager::addSearchEngine( const char* name, const char* version ) {
	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return -1;
	}

	// search
	std::string sql = FMT(
		"select %s from %s where %s = ? and %s = ?;",
		COL_ID_ENGINE,
		TABLE_ENGINE,
		COL_NAME_ENGINE,
		COL_VERSION_ENGINE
	);

	int id = -1;
	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindString( 1, NVL( name, "" ) );
		stmt->bindString( 2, NVL( version, "" ) );

		kome::db::ResultSet* rs = stmt->executeQuery();
		if( rs != NULL ) {
			while( rs->next() ) {
				id = rs->getInt( 0, -1 );
			}
			rs->close();
		}
		stmt->close();
	}

	if( id >= 0 ) {
		return id;
	}

	// add
	sql = FMT(
		"insert into %s (%s, %s) values(?, ?);",
		TABLE_ENGINE,
		COL_NAME_ENGINE,
		COL_VERSION_ENGINE
	);

	stmt = db->prepare( sql.c_str() );
	bool ret = false;
	if( stmt != NULL ) {
		stmt->bindString( 1, NVL( name, "" ) );
		stmt->bindString( 2, NVL( version, "" ) );

		ret = stmt->execute();
		stmt->close();
	}

	// ID
	if( ret ) {
		id = db->getMaxValue( TABLE_ENGINE, COL_ID_ENGINE );
		if( id < 0 ) {
			id = -1;
		}
	}

	return id;
}

// delete search engine
void IdentificationManager::deleteSearchEngine( const int id ) {
	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return;
	}

	// delete searches
	std::vector< int > searches;
	getSearchesFromEngine( searches, id );
	for( unsigned int i = 0; i < searches.size(); i++ ) {
		deleteSearch( searches[ i ] );
	}

	// delete hit items
	deleteHitItem( id );

	// delete items
	deleteSearchItem( id );

	// delete
	std::string sql = FMT( "delete from %s where %s = ?;", TABLE_ENGINE, COL_ID_ENGINE );
	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindInteger( 1, id );
		stmt->execute();
		stmt->close();
	}
}

// get search engines
void IdentificationManager::getSearchEngines( std::vector< int >& engines ) {
	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return;
	}

	// engines
	std::string sql = FMT( "select %s = %s;", COL_ID_ENGINE, TABLE_ENGINE );
	kome::db::ResultSet* rs = db->executeQuery( sql.c_str() );
	if( rs != NULL ) {
		while( rs->next() ) {
			const int id = rs->getInt( 0, -1 );
			if( id >= 0 ) {
				engines.push_back( id );
			}
		}
		rs->close();
	}
}

// get search engine name
std::string IdentificationManager::getSearchEngineName( const int id ) {
	// string
	std::string s;

	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return s;
	}

	// name
	std::string sql = FMT(
		"select %s from %s where %s = ?;",
		COL_NAME_ENGINE,
		TABLE_ENGINE,
		COL_ID_ENGINE
	);

    kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindInteger( 1, id );
		kome::db::ResultSet* rs = stmt->executeQuery();
		if( rs != NULL ) {
			if( rs->next() ) {
				s = rs->getString( 0, "" );
			}
			rs->close();
		}
		stmt->close();
	}

	return s;
}

// get search engine version
std::string IdentificationManager::getSearchEngineVersion( const int id ) {
	// string
	std::string s;

	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return s;
	}

	// name
	std::string sql = FMT(
		"select %s = %s where %s = ?;",
		COL_VERSION_ENGINE,
		TABLE_ENGINE,
		COL_ID_ENGINE
	);

    kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindInteger( 1, id );
		kome::db::ResultSet* rs = stmt->executeQuery();
		if( rs != NULL ) {
			if( rs->next() ) {
				s = rs->getString( 0, "" );
			}
			rs->close();
		}
		stmt->close();
	}

	return s;
}

// add search item
int IdentificationManager::addSearchItem( const int engineId, const char* name ) {
	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return -1;
	}

	// search
	int id = -1;
	std::string sql = FMT(
		"select %s from %s where %s = ? and %s = ?;",
		COL_ID_SEARCH_PARAMS,
		TABLE_SEARCH_PARAMS,
		COL_ENGINE_SEARCH_PARAMS,
		COL_NAME_SEARCH_PARAMS
	);

	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindInteger( 1, engineId );
		stmt->bindString( 2, NVL( name, "" ) );

		kome::db::ResultSet* rs = stmt->executeQuery();
		if( rs != NULL ) {
			if( rs->next() ) {
				id = rs->getInt( 0, -1 );
			}
			rs->close();
		}
		stmt->close();
	}

	if( id >= 0 ) {
		return id;
	}

	// add
	sql = FMT(
		"insert into %s (%s, %s) values(?, ?);",
		TABLE_SEARCH_PARAMS,
		COL_ENGINE_SEARCH_PARAMS,
		COL_NAME_SEARCH_PARAMS
	);

	bool ret = false;
	stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindInteger( 1, engineId );
		stmt->bindString( 2, NVL( name, "" ) );

		ret = stmt->execute();
		stmt->close();
	}

	// id
	if( ret ) {
		id = db->getMaxValue( TABLE_SEARCH_PARAMS, COL_ID_SEARCH_PARAMS );
		if( id < 0 ) {
			id = -1;
		}
	}

	return id;
}

// delete search item
void IdentificationManager::deleteSearchItem( const int engineId ) {
	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return;
	}

	// delete
	std::string sql = FMT(
		"delete from %s where %s = ?;",
		TABLE_SEARCH_PARAMS,
		COL_ENGINE_SEARCH_PARAMS
	);

	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindInteger( 1, engineId );
		stmt->execute();
		stmt->close();
	}
}

// set search value
void IdentificationManager::setSearchValue( const int itemId, const int searchId, const char* val ) {
	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return;
	}

	// search
	int id = -1;
	std::string sql = FMT(
		"select %s from %s where %s = ? and %s = ?;",
		COL_ID_SEARCH_VAL,
		TABLE_SEARCH_VALUE,
		COL_ITEM_SEARCH_VAL,
		COL_SEARCH_SEARCH_VAL
	);

	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindInteger( 1, itemId );
		stmt->bindInteger( 2, searchId );
		kome::db::ResultSet* rs = stmt->executeQuery();
		if( rs != NULL ) {
			if( rs->next() ) {
				id = rs->getInt( 0, -1 );
			}
			rs->close();
		}
		stmt->close();
	}

	// add
	if( id < 0 ) {    // insert
		sql = FMT(
			"insert into %s (%s, %s, %s) values(?, ?, ?);",
			TABLE_SEARCH_VALUE,
			COL_ITEM_SEARCH_VAL,
			COL_SEARCH_SEARCH_VAL,
			COL_VALUE_SEARCH_VAL
		);

		stmt = db->prepare( sql.c_str() );
		if( stmt != NULL ) {
			stmt->bindInteger( 1, itemId );
			stmt->bindInteger( 2, searchId );
			stmt->bindString( 3, NVL( val, "" ) );
			stmt->execute();

			stmt->close();
		}
	}
	else {    // update
		sql = FMT(
			"update %s set %s = ? where %s = ?;",
			TABLE_SEARCH_VALUE,
			COL_VALUE_SEARCH_VAL,
			COL_ID_SEARCH_VAL
		);

		stmt = db->prepare( sql.c_str() );
		if( stmt != NULL ) {
			stmt->bindString( 1, NVL( val, "" ) );
			stmt->bindInteger( 2, id );
			stmt->execute();

			stmt->close();
		}
	}
}

// delete search value
void IdentificationManager::deleteSearchValue( const int searchId ) {
	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return;
	}

	// delete
	std::string sql = FMT(
		"delete from %s where %s = ?;",
		TABLE_SEARCH_VALUE,
		COL_SEARCH_SEARCH_VAL
	);

	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindInteger( 1, searchId );
		stmt->execute();
		stmt->close();
	}
}

// add peak detection
int IdentificationManager::addPeakDetection( const char* method ) {
	// ID
	int id = -1;

	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return id;
	}

	// add
	std::string sql = FMT(
		"insert into %s (%s) values (?);",
		TABLE_PEAK_DETECTION,
		COL_METHOD_PEAK_DETECTION
	);

	bool ret = false;

	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindString( 1, NVL( method, "" ) );
		ret = stmt->execute();
		stmt->close();
	}

	if( ret ) {
		id = db->getMaxValue( TABLE_PEAK_DETECTION, COL_ID_PEAK_DETECTION );
	}

	return id;		
}

// set search peak parameter
void IdentificationManager::setPeakParameter( const int detect, const char* name, const char* value ) {
	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return;
	}

	// search
	int id = -1;
	std::string sql = FMT(
		"select %s from %s where %s = ? and %s = ?;",
		COL_ID_PEAK_PARAMS,
		TABLE_PEAK_PARAMS,
		COL_DETECTION_PEAK_PARAMS,
		COL_NAME_PEAK_PARAMS
	);

	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindInteger( 1, detect );
		stmt->bindString( 2, NVL( name, "" ) );

		kome::db::ResultSet* rs = stmt->executeQuery();
		if( rs != NULL ) {
			if( rs->next() ) {
				id = rs->getInt( 0, -1 );
			}
			rs->close();
		}
		stmt->close();
	}

	// add
	if( id < 0 ) {    // insert
		sql = FMT(
			"insert into %s (%s, %s, %s) values(?, ?, ?);",
			TABLE_PEAK_PARAMS,
			COL_DETECTION_PEAK_PARAMS,
			COL_NAME_PEAK_PARAMS,
			COL_VALUE_PEAK_PARAMS
		);

		stmt = db->prepare( sql.c_str() );
		if( stmt != NULL ) {
			stmt->bindInteger( 1, detect );
			stmt->bindString( 2, NVL( name, "" ) );
			stmt->bindString( 3, NVL( value, "" ) );

			stmt->execute();
			stmt->close();
		}
	}
	else {    // update
		sql = FMT(
			"update %s set %s = ? where %s = ?;", 
			TABLE_PEAK_PARAMS,
			COL_VALUE_PEAK_PARAMS,
			COL_ID_PEAK_PARAMS
		);

		stmt = db->prepare( sql.c_str() );
		if( stmt != NULL ) {
			stmt->bindString( 1, NVL( value, "" ) );

			stmt->execute();
			stmt->close();
		}
	}
}

// delete peak detection
void IdentificationManager::deletePeakDetection( const int detect ) {
	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return;
	}

	// delete parameters
	std::string sql = FMT(
		"delete from %s where %s = ?;",
		TABLE_PEAK_PARAMS,
		COL_DETECTION_PEAK_PARAMS
	);

	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindInteger( 1, detect );
		stmt->execute();
		stmt->close();
	}

	// delete detection
	sql = FMT(
		"delete from %s where %s = ?;",
		TABLE_PEAK_DETECTION,
		COL_ID_PEAK_DETECTION
	);

	stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindInteger( 1, detect );
		stmt->execute();
		stmt->close();
	}
}

// get peak detection method
std::string IdentificationManager::getPeakDetectionMethod( const int detect ) {
	// return value
	std::string method;

	// DB
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return method;
	}

	// get method
	std::string sql = FMT(
		"select %s from %s where %s = ?;",
		COL_METHOD_PEAK_DETECTION,
		TABLE_PEAK_DETECTION,
		COL_ID_PEAK_DETECTION
	);

	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindInteger( 1, detect );
		kome::db::ResultSet* rs = stmt->executeQuery();

		if( rs != NULL ) {
			if( rs->next() ) {
				method = rs->getString( 0, "" );
			}
			rs->close();
		}
		stmt->close();
	}

	return method;
}

// get peak detection parameters
void IdentificationManager::getPeakDetectionParameters( const int detect, kome::core::Properties& props ) {
	// DB
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return;
	}

	// parameters
	std::string sql = FMT(
		"select %s, %s from %s where %s = ?;",
		COL_NAME_PEAK_PARAMS,
		COL_VALUE_PEAK_PARAMS,
		TABLE_PEAK_PARAMS,
		COL_DETECTION_PEAK_PARAMS
	);

	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindInteger( 1, detect );

		kome::db::ResultSet* rs = stmt->executeQuery();
		if( rs != NULL ) {
			while( rs->next() ) {
				std::string name = rs->getString( 0, "" );
				std::string val = rs->getString( 1, "" );

				props.setValue( name.c_str(), val.c_str() );
			}

			rs->close();
		}

		stmt->close();
	}
}

// delete unlinked peak detection
void IdentificationManager::deleteUnlinkedPeakDetection() {
	// DB
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return;
	}

	// peak detection set
	std::set< int > detections;

	std::string sql = FMT(
		"select distinct %s from %s;",
		COL_DETECTION_PEAK_LIST,
		TABLE_PEAK_LIST
	);

	kome::db::ResultSet* rs = db->executeQuery( sql.c_str() );
	if( rs != NULL ) {
		while( rs->next() ) {
			int detection = rs->getInt( 0, -1 );
			if( detection >= 0 ) {
				detections.insert( detection );
			}
		}
		rs->close();
	}

	// delete list
	std::vector< int > deleteDetections;

	sql = FMT(
		"select %s from %s;",
		COL_ID_PEAK_DETECTION,
		TABLE_PEAK_DETECTION
	);

	rs = db->executeQuery( sql.c_str() );
	if( rs != NULL ) {
		while( rs->next() ) {
			int detection = rs->getInt( 0, -1 );
			if( detection >= 0 && detections.find( detection ) == detections.end() ) {
				deleteDetections.push_back( detection );
			}
		}
		rs->close();
	}

	// delete
	for( unsigned int i = 0; i < deleteDetections.size(); i++ ) {
		deletePeakDetection( deleteDetections[ i ] );
	}
}

// set peaks
int IdentificationManager::setPeaks( kome::core::XYData* peaks, const int peakDetection ) {
	// peaks ID
	int id = -1;

	// parameters
	if( peaks == NULL ) {
		return id;
	}

	// DB
	kome::db::Database* db = getDb();	
	if( db == NULL ) {
		return id;
	}

	// peaks
	kome::core::DataPoints dps( kome::core::DataPoints::FLOAT );
	for( unsigned int i = 0; i < peaks->getLength(); i++ ) {
		dps.addPoint( peaks->getX( i ), peaks->getY( i ) );
	}

	kome::core::Buffer buff;
	dps.exportData( boost::bind( &kome::core::DataAccessor::write, &buff, _1, _2 ) );

	// set peaks
	std::string sql = FMT(
		"insert into %s (%s, %s) values (?, ?);",
		TABLE_PEAK_LIST,
		COL_PEAKS_PEAK_LIST,
		COL_DETECTION_PEAK_LIST
	);

	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt == NULL ) {
		return id;
	}

	stmt->bindData( 1, buff.getBuffer(), buff.getLength() );
	stmt->bindInteger( 2, peakDetection );

	bool ret = stmt->execute();
	stmt->close();

	if( !ret ) {
		return id;
	}

	// ID
	id = db->getMaxValue( TABLE_PEAK_LIST, COL_ID_PEAK_LIST );

	return id;
}

// get peaks
int IdentificationManager::getPeaks( const int peaksId, kome::core::XYData* peaks ) {
	// peak detection
	int detection = -1;

	// DB
	kome::db::Database* db = getDb();
	if( db == NULL || peaks == NULL ) {
		return detection;
	}

	// peaks
	std::string sql = FMT(
		"select %s, %s from %s where %s = ?;",
		COL_PEAKS_PEAK_LIST,
		COL_DETECTION_PEAK_LIST,
		TABLE_PEAK_LIST,
		COL_ID_PEAK_LIST
	);

	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindInteger( 1, peaksId );

		kome::db::ResultSet* rs = stmt->executeQuery();
		if( rs != NULL ) {
			if( rs->next() ) {
				detection = rs->getInt( 1, -1 );

				if( peaks != NULL ) {
					kome::core::DataPoints dps( kome::core::DataPoints::FLOAT );

					kome::core::MemoryAccessor acc( rs->getData( 0 ), rs->getDataSize( 0 ) );
					dps.importData( boost::bind( &kome::core::DataAccessor::read, &acc, _1, _2 ) );

					for( unsigned int i = 0; i < dps.getLength(); i++ ) {
						peaks->addPoint( dps.getX( i ), dps.getY( i ) );
					}
				}
			}
			rs->close();
		}
		stmt->close();
	}

	return detection;
}

// delete peaks
void IdentificationManager::deletePeaks( const int peaks ) {
	// DB
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return;
	}

	// delete
	std::string sql = FMT(
		"delete from %s where %s = ?;",
		TABLE_PEAK_LIST,
		COL_ID_PEAK_LIST
	);

	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindInteger( 1, peaks );
		stmt->execute();
		stmt->close();
	}
}


// delete unlinked peaks
void IdentificationManager::deleteUnlinkedPeaks() {
	// DB
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return;
	}

	// peak IDs
	std::set< int > peaksSet;

	std::string sql = FMT(
		"select distinct %s from %s;",
		COL_PEAKS_SEARCH_SPEC,
		TABLE_SEARCH_SPEC
	);

	kome::db::ResultSet* rs = db->executeQuery( sql.c_str() );
	if( rs != NULL ) {
		while( rs->next() ) {
			int peaks = rs->getInt( 0, -1 );
			if( peaks >= 0 ) {
				peaksSet.insert( peaks );
			}
		}
		rs->close();
	}

	// delete peaks
	std::vector< int > delPeaks;
	sql = FMT(
		"select %s from %s;",
		COL_ID_PEAK_LIST,
		TABLE_PEAK_LIST
	);

	rs = db->executeQuery( sql.c_str() );
	if( rs != NULL ) {
		while( rs->next() ) {
			int peaks = rs->getInt( 0, -1);
			if( peaks >= 0 && peaksSet.find( peaks ) == peaksSet.end() ) {
				delPeaks.push_back( peaks );
			}
		}
		rs->close();
	}

	for( unsigned int i = 0; i < delPeaks.size(); i++ ) {
		deletePeaks( delPeaks[ i ] );
	}
}

// add search
int IdentificationManager::addSearch( SearchInfo& search ) {
	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return -1;
	}

	// DB ID
	int dbId = getDatabaseId( search.db.c_str() );
	if( dbId < 0 ) {
		dbId = addDatabase( search.db.c_str(), "" );
	}

	// add
	std::string sql = FMT(
		"insert into %s (%s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s) values (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);",
		TABLE_SEARCH,
		COL_NAME_SEARCH,
		COL_COMMENT_SEARCH,
		COL_DATE_SEARCH,
		COL_ENGINE_SEARCH,
		COL_DB_SEARCH,
		COL_RESULT_SEARCH,
		COL_PREC_MASS_TYPE_SEARCH,
		COL_FRAG_MASS_TYPE_SEARCH,
		COL_ENZ_SEARCH,
		COL_BASE_NAME_SEARCH,
		COL_SEARCH_ID_SEARCH,
		COL_STATUS_SEARCH
	);

	bool ret = false;
	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindString( 1, search.name.c_str() );
		stmt->bindString( 2, search.comment.c_str() );
		stmt->bindString( 3, search.date.c_str() );
		stmt->bindInteger( 4, search.engineId );
		stmt->bindInteger( 5, dbId );
		stmt->bindString( 6, search.result.c_str() );
		stmt->bindInteger( 7, search.precMassType );
		stmt->bindInteger( 8, search.fragMassType );
		Enzyme* enzyme = search.enzyme;
		stmt->bindInteger( 9, ( enzyme == NULL ? -1 : enzyme->getId() ) );
		stmt->bindString( 10, search.baseName.c_str() );
		stmt->bindString( 11, search.searchId.c_str() );
		stmt->bindInteger( 12, (int)SearchEngine::STATUS_UNKNOWN );

		ret = stmt->execute();
		stmt->close();
	}

	if( !ret ) {
		return -1;
	}

	// ID
	int id = db->getMaxValue( TABLE_SEARCH, COL_ID_SEARCH );
	if( id < 0 ) {
		return -1;
	}

	// search parameters
	for( unsigned int i = 0; i < search.searchProps.getNumberOfProperties(); i++ ) {
		std::string n = search.searchProps.getKey( i );
		std::string v = search.searchProps.getValue( i );

		if( !n.empty() && !v.empty() ) {
			int itemId = addSearchItem( search.engineId, n.c_str() );
			if( itemId >= 0 ) {
				setSearchValue( itemId, id, v.c_str() );
			}
		}
	}

	// add spectra
	for( int i = 0; i < search.specNum; i++ ) {
		SpecInfo* spec = (search.spectra)[ i ];
		int specId = addSpectrum( *spec );
		int searchSpecId = setSearchSpectrum( specId, id, spec->peaks, spec->precursor, spec->charges );

		// matrix peaks
		for( unsigned int j = 0; j < spec->peakIds.size(); j++ ) {
			const int matrixPeakId = spec->peakIds.at( j );
			if( matrixPeakId >= 0 ) {
				setMatrixPeakSpectrum( matrixPeakId, searchSpecId );
			}
		}
	}

	// modifications
	for( unsigned int i = 0; i < search.fixedMods.size(); i++ ) {
		addSearchModification( id, search.fixedMods.at( i )->getId(), false );
	}
	for( unsigned int i = 0; i < search.varMods.size(); i++ ) {
		addSearchModification( id, search.varMods.at( i )->getId(), true );
	}

	return id;
}

// delete search
void IdentificationManager::deleteSearch( const int id ) {
	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return;
	}

	// delete values
	deleteSearchValue( id );

	// delete modifications
	deleteSearchModifications( id );

	// delete search spectra
	std::vector< int > spectra;
	getSearchSpectraFromSearch( spectra, id );
	for( unsigned int i = 0; i < spectra.size(); i++ ) {
		deleteSearchSpectrum( spectra[ i ] );
	}

	// delete search
	std::string sql = FMT(
		"delete from %s where %s = ?;",
		TABLE_SEARCH,
		COL_ID_SEARCH
	);

	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindInteger( 1, id );
		stmt->execute();
		stmt->close();
	}
}

// update search
void IdentificationManager::updateSearch( const int id, SearchInfo& search ) {
	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return;
	}

	// delete values
	deleteSearchValue( id );
	deleteSearchModifications( id );

	// DB ID
	int dbId = getDatabaseId( search.db.c_str() );
	if( dbId < 0 ) {
		dbId = addDatabase( search.db.c_str(), "" );
	}

	// update
	std::string sql = FMT( 
		"update %s set %s = ?, %s = ?, %s = ?, %s = ?, %s = ?, %s = ?, %s = ?, %s = ?, %s = ?, %s = ?, %s = ? where %s = ?;",
		TABLE_SEARCH,
		COL_NAME_SEARCH,
		COL_COMMENT_SEARCH,
		COL_DATE_SEARCH,
		COL_ENGINE_SEARCH,
		COL_DB_SEARCH,
		COL_RESULT_SEARCH,
		COL_PREC_MASS_TYPE_SEARCH,
		COL_FRAG_MASS_TYPE_SEARCH,
		COL_ENZ_SEARCH,
		COL_BASE_NAME_SEARCH,
		COL_SEARCH_ID_SEARCH,
		COL_ID_SEARCH
	);

	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindString( 1, search.name.c_str() );
		stmt->bindString( 2, search.comment.c_str() );
		stmt->bindString( 3, search.date.c_str() );
		stmt->bindInteger( 4, search.engineId );
		stmt->bindInteger( 5, dbId );
		stmt->bindString( 6, search.result.c_str() );
		stmt->bindInteger( 7, search.precMassType );
		stmt->bindInteger( 8, search.fragMassType );
		stmt->bindInteger( 9, search.enzyme->getId() );
		stmt->bindString( 10, search.baseName.c_str() );
		stmt->bindString( 11, search.searchId.c_str() );
		stmt->bindInteger( 13, id );

		stmt->execute();
		stmt->close();
	}

	// search parameters
	for( unsigned int i = 0; i < search.searchProps.getNumberOfProperties(); i++ ) {
		std::string n = search.searchProps.getKey( i );
		std::string v = search.searchProps.getValue( i );

		if( !n.empty() && !v.empty() ) {
			int itemId = addSearchItem( search.engineId, n.c_str() );
			if( itemId >= 0 ) {
				setSearchValue( itemId, id, v.c_str() );
			}
		}
	}

	// modifications
	for( unsigned int i = 0; i < search.fixedMods.size(); i++ ) {
		addSearchModification( id, search.fixedMods.at( i )->getId(), false );
	}
	for( unsigned int i = 0; i < search.varMods.size(); i++ ) {
		addSearchModification( id, search.varMods.at( i )->getId(), true );
	}
}

// set search status
void IdentificationManager::setSearchStatus( const int id, SearchEngine::SearchStatus status ) {
	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return;
	}

	// update
	std::string sql = FMT( 
		"update %s set %s = ? where %s = ?;",
		TABLE_SEARCH,
		COL_STATUS_SEARCH,
		COL_ID_SEARCH
	);

	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindInteger( 1, (int)status );
		stmt->bindInteger( 2, id );

		stmt->execute();
		stmt->close();
	}
}

// get search status
SearchEngine::SearchStatus IdentificationManager::getSearchStatus( const int id ) {
	// return value
	SearchEngine::SearchStatus status = SearchEngine::STATUS_UNKNOWN;

	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return status;
	}

	// get the status
	std::string sql = FMT(
		"select %s from %s where %s = ?;",
		COL_STATUS_SEARCH,
		TABLE_SEARCH,
		COL_ID_SEARCH
	);

	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindInteger( 1, id );

		kome::db::ResultSet* rs = stmt->executeQuery();
		if( rs != NULL ) {
			if( rs->next() ) {
				status = (SearchEngine::SearchStatus)rs->getInt( 0, (int)SearchEngine::STATUS_UNKNOWN );
			}
			rs->close();
		}
		stmt->close();
	}

	return status;
}

// set result location
void IdentificationManager::setResultLocation( const int id, const char* location ) {
	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return;
	}

	// update
	std::string sql = FMT( 
		"update %s set %s = ? where %s = ?;",
		TABLE_SEARCH,
		COL_RESULT_SEARCH,
		COL_ID_SEARCH
	);

	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindString( 1, NVL( location, "" ) );
		stmt->bindInteger( 2, id );

		stmt->execute();
		stmt->close();
	}
}

// get result location
std::string IdentificationManager::getResultLocation( const int id ) {
	// return value
	std::string ret;

	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return ret;
	}

	// get the status
	std::string sql = FMT(
		"select %s from %s where %s = ?;",
		COL_RESULT_SEARCH,
		TABLE_SEARCH,
		COL_ID_SEARCH
	);

	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindInteger( 1, id );

		kome::db::ResultSet* rs = stmt->executeQuery();
		if( rs != NULL ) {
			if( rs->next() ) {
				ret = rs->getString( 0, "" );
			}
			rs->close();
		}
		stmt->close();
	}

	return ret;
}

// get searches from engine
void IdentificationManager::getSearchesFromEngine( std::vector< int >& searches, const int engineId ) {
	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return;
	}

	// searches
	std::string sql = FMT(
		"select %s from %s where %s = ?;",
		COL_ID_SEARCH,
		TABLE_SEARCH,
		COL_ENGINE_SEARCH
	);

	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindInteger( 1, engineId );

		kome::db::ResultSet* rs = stmt->executeQuery();
		if( rs != NULL ) {
			while( rs->next() ) {
				const int id = rs->getInt( 0, -1 );
				if( id >= 0 ) {
					searches.push_back( id );
				}
			}
			rs->close();
		}
		stmt->close();
	}
}

// get searches from DB
void IdentificationManager::getSearchesFromDb( std::vector< int >& searches, const int dbId ) {
	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return;
	}

	// searches
	std::string sql = FMT(
		"select %s from %s where %s = ?;",
		COL_ID_SEARCH,
		TABLE_SEARCH,
		COL_DB_SEARCH
	);

	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindInteger( 1, dbId );

		kome::db::ResultSet* rs = stmt->executeQuery();
		if( rs != NULL ) {
			while( rs->next() ) {
				const int id = rs->getInt( 0, -1 );
				if( id >= 0 ) {
					searches.push_back( id );
				}
			}
			rs->close();
		}
		stmt->close();
	}
}

// get searches from matrix
void IdentificationManager::getSearchesFromMatrix( std::vector< int >& searches, const int matrixId ) {
	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return;
	}

	// searches
	std::string sql = FMT(
		"select distinct s.%s from %s as p, %s as i, %s as s where s.%s = i.%s and i.%s = p.%s and p.%s = ?;",
		COL_SEARCH_SEARCH_SPEC,
		TABLE_PEAK,
		TABLE_MATRIX_SPEC,
		TABLE_SEARCH_SPEC,
		COL_ID_SEARCH_SPEC,
		COL_SPEC_MATRIX_SPEC,
		COL_PEAK_MATRIX_SPEC,
		COL_ID_PEAK,
		COL_MATRIX_PEAK
	);

	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindInteger( 1, matrixId );
		kome::db::ResultSet* rs = stmt->executeQuery();
		if( rs != NULL ) {
			while( rs->next() ) {
				const int id = rs->getInt( 0, -1 );
				if( id >= 0 ) {
					searches.push_back( id );
				}
			}
			rs->close();
		}
		stmt->close();
	}
}

// get searches from sample
void IdentificationManager::getSearchesFromSample( std::vector< int >& searches, kome::objects::Sample* sample ) {
	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return;
	}

	// sample ID
	int sampleId = getSample( sample );
	if( sampleId < 0 ) {
		return;
	}

	// searches
	std::string sql = FMT(
		"select distinct s0.%s from %s as s0, %s as s1 where s0.%s = s1.%s and s1.%s = ?;",
		COL_SEARCH_SEARCH_SPEC,
		TABLE_SEARCH_SPEC,
		TABLE_SPECTRUM,
		COL_SPEC_SEARCH_SPEC,
		COL_ID_SPECTRUM,
		COL_SAMPLE_SPECTRUM
	);

	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindInteger( 1, sampleId );
		kome::db::ResultSet* rs = stmt->executeQuery();
		if( rs != NULL ) {
			while( rs->next() ) {
				const int id = rs->getInt( 0, -1 );
				if( id >= 0 ) {
					searches.push_back( id );
				}
			}
			rs->close();
		}
		stmt->close();
	}
}

// get searches
void IdentificationManager::getSearches( std::vector< int >& searches ) {
		// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return;
	}

	// searches
	std::string sql = FMT(
		"select %s from %s;", COL_ID_SEARCH, TABLE_SEARCH
	);

	kome::db::ResultSet* rs = db->executeQuery( sql.c_str() );
	if( rs != NULL ) {
		while( rs->next() ) {
			const int id = rs->getInt( 0, -1 );
			if( id >= 0 ) {
				searches.push_back( id );
			}
		}
		rs->close();
	}
}

// get search
void IdentificationManager::getSearch( const int id, SearchInfo& search ) {
	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return;
	}

	// get search
	std::string sql = FMT( 
		"select %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s from %s where %s = ?;",
		COL_NAME_SEARCH,
		COL_COMMENT_SEARCH,
		COL_DATE_SEARCH,
		COL_ENGINE_SEARCH,
		COL_DB_SEARCH,
		COL_RESULT_SEARCH,
		COL_PREC_MASS_TYPE_SEARCH,
		COL_FRAG_MASS_TYPE_SEARCH,
		COL_ENZ_SEARCH,
		COL_BASE_NAME_SEARCH,
		COL_SEARCH_ID_SEARCH,
		TABLE_SEARCH,
		COL_ID_SEARCH
	);


	int dbId = -1;
	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindInteger( 1, id );

		kome::db::ResultSet* rs = stmt->executeQuery();
		if( rs != NULL ) {
			if( rs->next() ) {
				search.name = rs->getString( 0, "" );
				search.comment = rs->getString( 1, "" );
				search.date = rs->getString( 2, "" );
				search.engineId = rs->getInt( 3, -1 );
				dbId = rs->getInt( 4, -1 );
				search.result = rs->getString( 5, "" );
				search.precMassType = rs->getInt( 6, -1 );
				search.fragMassType = rs->getInt( 7, -1 );
				search.enzyme = getEnzymeFromId( rs->getInt( 8, -1 ) );
				search.baseName = rs->getString( 9, "" );
				search.searchId = rs->getString( 10, "" );
			}
			rs->close();
		}
		stmt->close();
	}
	search.db = getDatabaseName( dbId );

	// search parameters
	sql = FMT(
		"select p.%s, v.%s from %s as p, %s as v where p.%s = v.%s and v.%s = ?;",
		COL_NAME_SEARCH_PARAMS,
		COL_VALUE_SEARCH_VAL,
		TABLE_SEARCH_PARAMS,
		TABLE_SEARCH_VALUE,
		COL_ID_SEARCH_PARAMS,
		COL_ITEM_SEARCH_VAL,
		COL_SEARCH_SEARCH_VAL
	);

	stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindInteger( 1, id );

		kome::db::ResultSet* rs = stmt->executeQuery();
		if( rs != NULL ) {
			while( rs->next() ) {
				std::string n = rs->getString( 0, "" );
				std::string v = rs->getString( 1, "" );
				search.searchProps.setValue( n.c_str(), v.c_str() );
			}
			rs->close();
		}
		stmt->close();
	}

	// modifications
	std::vector< int > searchMods;
	getSearchModifications( searchMods, id );

	for( unsigned int i = 0; i < searchMods.size(); i++ ) {
		int searchModId = searchMods[ i ];
		int modId = getModificationFromSearchModification( searchModId );
		bool isVar = isVariableModification( searchModId );

		Modification* m = getModificationFromId( modId );

		if( m != NULL ) {
			if( isVar ) {
				search.varMods.push_back( m );
			}
			else {
				search.fixedMods.push_back( m );
			}
		}
	}
}

// get search
void IdentificationManager::getSearch( const int id, SearchResult& result ) {
	// get
	SearchInfo info;
	getSearch( id, info );

	// initialize
	result.clear();

	// search engine
	SearchEngine* engine = NULL;
	std::string engineStr = getSearchEngineName( info.engineId );
	for( int i = 0; i < getNumberOfSearchEngines() && engine == NULL; i++ ) {
		SearchEngine* tmp = getSearchEngine( i );
		if( engineStr.compare( tmp->getName() ) == 0 ) {
			engine = tmp;
		}
	}
	result.setSearchEngine( engine );

	// title
	result.setTitle( info.name.c_str() );

	// comment
	result.setComment( info.comment.c_str() );

	// date
	result.setDate( info.date.c_str() );

	// database
	std::string db = info.db;
	result.setDb( db.c_str() );

	// result
	result.setResultLocation( info.result.c_str() );

	// mass type
	result.setPrecursorMassType( (SearchResult::MassType)info.precMassType );
	result.setFragmentMassType( (SearchResult::MassType)info.fragMassType );

	// enzyme
	result.setEnzyme( info.enzyme );

	// base name
	result.setBaseName( info.baseName.c_str() );

	// modifications
	for( unsigned int i = 0; i < info.fixedMods.size(); i++ ) {
		Modification* m = info.fixedMods.at( i );
		result.addFixedModification( m );
	}

	for( unsigned int i = 0; i < info.varMods.size(); i++ ) {
		Modification* m = info.varMods.at( i );
		result.addVariableModification( m );
	}

	// properties
	for( unsigned int i = 0; i < info.searchProps.getNumberOfProperties(); i++ ) {
		result.getSearchParameters().setValue( info.searchProps.getKey( i ), info.searchProps.getValue( i ) );
	}

	// spectra
	std::vector< int > spectra;
	std::map< int, SpecInfo* > specMap;
	getSearchSpectraFromSearch( spectra, id );

	for( unsigned int i = 0; i < spectra.size(); i++ ) {
		SpecInfo* spec = new SpecInfo();

		int specId = 0;
		int peaks = -1;
		double precursor = -1.0;
		getSearchSpectrum( spectra[ i ], &specId, NULL, &peaks, &precursor );
		getSpectrum( specId, *spec );
		getCharges( spectra[ i ], spec->charges );
		spec->peaks = peaks;
		spec->precursor = precursor;

		specMap[ spectra[ i ] ] = spec;
		result.addSpectrum( spec );
	}

	// hit
	std::vector< int > hits;
	std::map< int, Substance* > substanceMap;
	getHitsFromSearch( hits, id );

	for( unsigned int i = 0; i < hits.size(); i++ ) {
		HitInfo hitInfo;
		getHit( hits[ i ], hitInfo );

		// substance
		Substance* substance = getSubstanceObject( hitInfo.substance, result, substanceMap );

		// hit
		Hit* hit = result.addHit( substance );
		hit->setID(hitInfo.id);
		hit->setCharge( hitInfo.charge );
		for( unsigned int j = 0; j < hitInfo.altSubstances.size(); j++ ) {
			AltSubstance* as = &( hitInfo.altSubstances.at( j ) );
			Substance* altSub = getSubstanceObject( as->id, result, substanceMap );
			hit->addAltSubstance( altSub, as->before.c_str(), as->after.c_str(), as->start, as->end );
		}
		for( unsigned int j = 0; j < hitInfo.props.size(); j++ ) {
			hit->addValue( hitInfo.props.at( j ).name.c_str(), hitInfo.props.at( j ).value.c_str(), hitInfo.props.at( j ).type );
		}

		for( unsigned int j = 0; j < hitInfo.spectra.size(); j++ ) {
			int sId = hitInfo.spectra.at( j );
			if( specMap.find( sId ) != specMap.end() ) {
				hit->addSpectrum( specMap[ sId ] );
			}
		}

		for( unsigned int j = 0; j < hitInfo.mods.size(); j++ ) {
			hit->addModification( hitInfo.mods.at( j ).first, hitInfo.mods.at( j ).second );
		}
	}
}

// get search ID
int IdentificationManager::getSearch( const char* engine, const char* version, const char* searchId ) {
	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return false;
	}

	// search engine ID
	int engineId = addSearchEngine( engine, version );

	// search
	std::string sql = FMT(
		"select %s from %s where %s = ? and %s = ?;",
		COL_ID_SEARCH,
		TABLE_SEARCH,
		COL_ENGINE_SEARCH,
		COL_SEARCH_ID_SEARCH
	);


	int id = -1;
	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindInteger( 1, engineId );
		stmt->bindString( 2, searchId );

		kome::db::ResultSet* rs = stmt->executeQuery();
		if( rs != NULL ) {
			if( rs->next() ) {
				id = rs->getInt( 0, -1 );
			}
			rs->close();
		}
		stmt->close();
	}

	return id;
}

// get search ID
std::string IdentificationManager::getSearchId( const int id ) { 
	// return value
	std::string ret;

	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return false;
	}

	// search
	std::string sql = FMT(
		"select %s from %s where %s = ?;",
		COL_SEARCH_ID_SEARCH,
		TABLE_SEARCH,
		COL_ID_SEARCH
	);

	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindInteger( 1, id );

		kome::db::ResultSet* rs = stmt->executeQuery();
		if( rs != NULL ) {
			if( rs->next() ) {
				ret = rs->getString( 0, "" );
			}
			rs->close();
		}
		stmt->close();
	}

	return ret;
}

// set search spectrum
int IdentificationManager::setSearchSpectrum( const int specId, const int searchId, const int peaks, const double precursor, std::vector< int >& charges ) {
	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return -1;
	}

	// search
	int id = getSearchSpectrum( searchId, specId );

	// charge
	int chargeFlg = 0;
	for( unsigned int i = 0; i < charges.size(); i++ ) {
		int charge = charges[ i ];
		charge = CLAMP( charge, 1, 8 );
		chargeFlg |= ( 1 << ( charge - 1 ) );
	}

	// set
	std::string sql;
	if( id < 0 ) {    // insert
		sql = FMT(
			"insert into %s (%s, %s, %s, %s, %s) values(?, ?, ?, ?, ?);",
			TABLE_SEARCH_SPEC,
			COL_PEAKS_SEARCH_SPEC,
			COL_PRECURSOR_SEARCH_SPEC,
			COL_CHARGE_SEARCH_SPEC,
			COL_SPEC_SEARCH_SPEC,
			COL_SEARCH_SEARCH_SPEC
		);
	}
	else {
		sql = FMT(
			"update %s set %s = ?, %s = ?, %s = ? where %s = ?;",
			TABLE_SEARCH_SPEC,
			COL_PEAKS_SEARCH_SPEC,
			COL_PRECURSOR_SEARCH_SPEC,
			COL_CHARGE_SEARCH_SPEC,
			COL_ID_SEARCH_SPEC
		);
	}

	bool ret = false;
	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindInteger( 1, peaks );

		if( precursor >= 0.0 ) {
			stmt->bindReal( 2, precursor );
		}
		else {
			stmt->bindReal( 2, -1.0 );
		}

		stmt->bindInteger( 3, chargeFlg );
		
		if( id < 0 ) {
			stmt->bindInteger( 4, specId );
			stmt->bindInteger( 5, searchId );
		}
		else {
			stmt->bindInteger( 4, id );
		}

		ret = stmt->execute();

		stmt->close();
	}

	if( ret && id < 0 ) {
		id = db->getMaxValue( TABLE_SEARCH_SPEC, COL_ID_SEARCH_SPEC );
		if( id < 0 ) {
			id = -1;
		}
	}

	return id;
}

// delete search spectrum
void IdentificationManager::deleteSearchSpectrum( const int id ) {
	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return;
	}

	// delete matrix peak spectra
	std::vector< int > spectra;
	getMatrixPeakSpectraFromSpectrum( spectra, id );
	for( unsigned int i = 0; i < spectra.size(); i++ ) {
		deleteMatrixPeakSpectrum( spectra[ i ] );
	}

	// delete hit spectra
	deleteHitSpectraFromSpectrum( id );

	// delete
	std::string sql = FMT(
		"delete from %s where %s = ?;",
		TABLE_SEARCH_SPEC,
		COL_ID_SEARCH_SPEC
	);

	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindInteger( 1, id );
		stmt->execute();
		stmt->close();
	}
}

// get search spectrum
int IdentificationManager::getSearchSpectrum( const int searchId, const int specId ) {
	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return -1;
	}

	// ID
	int id = -1;
	std::string sql = FMT(
		"select %s from %s where %s = ? and %s = ?;",
		COL_ID_SEARCH_SPEC,
		TABLE_SEARCH_SPEC,
		COL_SPEC_SEARCH_SPEC,
		COL_SEARCH_SEARCH_SPEC
	);

	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindInteger( 1, specId );
		stmt->bindInteger( 2, searchId );

		kome::db::ResultSet* rs = stmt->executeQuery();
		if( rs != NULL ) {
			if( rs->next() ) {
				id = rs->getInt( 0, -1 );
			}
			rs->close();
		}
	}

	return id;
}

// get search spectra from search
void IdentificationManager::getSearchSpectraFromSearch( std::vector< int >& spectra, const int searchId ) {
	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return;
	}

	// spectra
	std::string sql = FMT(
		"select %s from %s where %s = ?;",
		COL_ID_SEARCH_SPEC,
		TABLE_SEARCH_SPEC,
		COL_SEARCH_SEARCH_SPEC
	);

	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindInteger( 1, searchId );

		kome::db::ResultSet* rs = stmt->executeQuery();
		if( rs != NULL ) {
			while( rs->next() ) {
				const int id = rs->getInt( 0, -1 );
				if( id >= 0 ) {
					spectra.push_back( id );
				}
			}
			rs->close();
		}
		stmt->close();
	}
}

// get search spectra from spectrum
void IdentificationManager::getSearchSpectraFromSpectrum( std::vector< int >& spectra, const int specId ) {
	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return;
	}

	// spectra
	std::string sql = FMT(
		"select %s from %s where %s = ?;",
		COL_ID_SEARCH_SPEC,
		TABLE_SEARCH_SPEC,
		COL_SPEC_SEARCH_SPEC
	);

	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindInteger( 1, specId );

		kome::db::ResultSet* rs = stmt->executeQuery();
		if( rs != NULL ) {
			while( rs->next() ) {
				const int id = rs->getInt( 0, -1 );
				if( id >= 0 ) {
					spectra.push_back( id );
				}
			}
			rs->close();
		}
		stmt->close();
	}
}

// get search spectra
void IdentificationManager::getSearchSpectra( std::vector< int >& spectra ) {
	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return;
	}

	// spectra
	std::string sql = FMT(
		"select %s from %s;", COL_ID_SEARCH_SPEC, TABLE_SEARCH_SPEC
	);

	kome::db::ResultSet* rs = db->executeQuery( sql.c_str() );
	if( rs != NULL ) {
		while( rs->next() ) {
			const int id = rs->getInt( 0, -1 );
			if( id >= 0 ) {
				spectra.push_back( id );
			}
		}
		rs->close();
	}
}

// get search spectrum
void IdentificationManager::getSearchSpectrum( const int id, int* specId, int* searchId, int* peaks, double* precursor ) {
	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return;
	}

	// search spectrum
	std::string sql = FMT(
		"select %s, %s, %s, %s from %s where %s = ?;",
		COL_SPEC_SEARCH_SPEC,
		COL_SEARCH_SEARCH_SPEC,
		COL_PRECURSOR_SEARCH_SPEC,
		COL_PEAKS_SEARCH_SPEC,
		TABLE_SEARCH_SPEC,
		COL_ID_SEARCH_SPEC
	);

	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindInteger( 1, id );
		kome::db::ResultSet* rs = stmt->executeQuery();
		if( rs != NULL ) {
			if( rs->next() ) {
				// spectrum
				if( specId != NULL ) {
					*specId = rs->getInt( 0, -1 );
				}

				// search
				if( searchId != NULL ) {
					*searchId = rs->getInt( 1, -1 );
				}

				// precursor
				if( precursor != NULL ) {
					*precursor = rs->getReal( 2, -1.0 );
				}

				// peaks
				if( peaks != NULL ) {
					*peaks = rs->getInt( 3, -1 );
				}
			}
			rs->close();
		}
		stmt->close();
	}
}

// get charges
void IdentificationManager::getCharges( const int searchSpecId, std::vector< int >& charges ) {
	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return;
	}

	// search spectrum
	std::string sql = FMT(
		"select %s from %s where %s = ?;",
		COL_CHARGE_SEARCH_SPEC,
		TABLE_SEARCH_SPEC,
		COL_ID_SEARCH_SPEC
	);

	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindInteger( 1, searchSpecId );

		kome::db::ResultSet* rs = stmt->executeQuery();
		if( rs != NULL ) {
			if( rs->next() ) {
				// charge
				int charge = rs->getInt( 0, 0 );
				for( unsigned int c = 1; c <= 8; c++ ) {
					unsigned flg = ( 1 <<  ( c - 1 ) );
					if( flg & charge ) {
						charges.push_back( c );
					}
				}
			}
			rs->close();
		}
		stmt->close();
	}
}

// set matrix peak spectrum
int IdentificationManager::setMatrixPeakSpectrum( const int peakId, const int specId ) {
	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return -1;
	}

	// search
	int id = -1;
	std::string sql = FMT( 
		"select %s from %s where %s = ? and %s = ?;",
		COL_ID_MATRIX_SPEC,
		TABLE_MATRIX_SPEC,
		COL_PEAK_MATRIX_SPEC,
		COL_SPEC_MATRIX_SPEC
	);

	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindInteger( 1, peakId );
		stmt->bindInteger( 2, specId );

		kome::db::ResultSet* rs = stmt->executeQuery();
		if( rs != NULL ) {
			if( rs->next() ) {
				id = rs->getInt( 0, -1 );
			}
			rs->close();
		}
		stmt->close();
	}

	if( id >= 0 ) {
		return id;
	}

	// add
	bool ret = false;
	sql = FMT(
		"insert into %s (%s, %s) values(?, ?);",
		TABLE_MATRIX_SPEC,
		COL_PEAK_MATRIX_SPEC,
		COL_SPEC_MATRIX_SPEC
	);

   stmt = db->prepare( sql.c_str() );
   if( stmt != NULL ) {
	   stmt->bindInteger( 1, peakId );
	   stmt->bindInteger( 2, specId );

	   ret = stmt->execute();
   }

   // ID
   if( ret ) {
	   id = db->getMaxValue( TABLE_MATRIX_SPEC, COL_ID_MATRIX_SPEC );
	   if( id < 0 ) {
		   id = -1;
	   }
   }

   return id;
}

// delete matrix peak spectrum
void IdentificationManager::deleteMatrixPeakSpectrum( const int id ) {
	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return;
	}

	// delete
	std::string sql = FMT(
		"delete from %s where %s = ?;",
		TABLE_MATRIX_SPEC,
		COL_ID_MATRIX_SPEC
	);

	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindInteger( 1, id );

		stmt->execute();
		stmt->close();
	}
}

// get matrix peak spectra from search spectrum
void IdentificationManager::getMatrixPeakSpectraFromSpectrum( std::vector< int >& spectra, const int searchSpecId ) {
	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return;
	}

	// spectra
	std::string sql = FMT(
		"select %s from %s where %s = ?;",
		COL_ID_MATRIX_SPEC,
		TABLE_MATRIX_SPEC,
		COL_SPEC_MATRIX_SPEC
	);

	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindInteger( 1, searchSpecId );
		kome::db::ResultSet* rs = stmt->executeQuery();
		if( rs != NULL ) {
			while( rs->next() ) {
				const int id = rs->getInt( 0, -1 );
				if( id >= 0 ) {
					spectra.push_back( id );
				}
			}
			rs->close();
		}
		stmt->close();
	}
}

// get matrix peak spectra from matrix peak
void IdentificationManager::getMatrixPeakSpectraFromPeak( std::vector< int >& spectra, const int peakId ) {
	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return;
	}

	// spectra
	std::string sql = FMT(
		"select %s from %s where %s = ?;",
		COL_ID_MATRIX_SPEC,
		TABLE_MATRIX_SPEC,
		COL_PEAK_MATRIX_SPEC
	);

	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindInteger( 1, peakId );
		kome::db::ResultSet* rs = stmt->executeQuery();
		if( rs != NULL ) {
			while( rs->next() ) {
				const int id = rs->getInt( 0, -1 );
				if( id >= 0 ) {
					spectra.push_back( id );
				}
			}
			rs->close();
		}
		stmt->close();
	}
}

// get matrix peak spectra from search and matrix peak
void IdentificationManager::getMatrixPeakSpectraFromSearchAndPeak( std::vector< int >& spectra, const int searchId, const int peakId ) {
	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return;
	}

	// spectra
	std::string sql = FMT(
		"select i.%s from %s as i, %s as s where i.%s = s.%s and i.%s = ? and s.%s = ?;",
		COL_ID_MATRIX_SPEC,
		TABLE_MATRIX_SPEC,
		TABLE_SEARCH_SPEC,
		COL_SPEC_MATRIX_SPEC,
		COL_ID_SEARCH_SPEC,
		COL_PEAK_MATRIX_SPEC,
		COL_SEARCH_SEARCH_SPEC
	);

	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindInteger( 1, peakId );
		stmt->bindInteger( 2, searchId );

		kome::db::ResultSet* rs = stmt->executeQuery();
		if( rs != NULL ) {
			while( rs->next() ) {
				const int id = rs->getInt( 0, -1 );
				if( id >= 0 ) {
					spectra.push_back( id );
				}
			}
			rs->close();
		}
		stmt->close();
	}

}

// get matrix peak spectra
void IdentificationManager::getMatrixPeakSpectra( std::vector< int >& spectra ) {
	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return;
	}

	// spectra
	std::string sql = FMT(
		"select %s from %s;", COL_ID_MATRIX_SPEC, TABLE_MATRIX_SPEC
	);

	kome::db::ResultSet* rs = db->executeQuery( sql.c_str() );
	if( rs != NULL ) {
		while( rs->next() ) {
			const int id = rs->getInt( 0, -1 );
			if( id >= 0 ) {
				spectra.push_back( id );
			}
		}
		rs->close();
	}
}

// get peak of matrix peak spectrum peak
int IdentificationManager::getMatrixPeakSpectrumPeak( const int id ) {
	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return -1;
	}

	// peak
	std::string sql = FMT(
		"select %s from %s where %s = ?;",
		COL_PEAK_MATRIX_SPEC,
		TABLE_MATRIX_SPEC,
		COL_ID_MATRIX_SPEC
	);

	int peak = -1;
	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindInteger( 1, id );
		kome::db::ResultSet* rs = stmt->executeQuery();
		if( rs != NULL ) {
			if( rs->next() ) {
				peak = rs->getInt( 0, -1 );
			}
			rs->close();
		}
		stmt->close();
	}

	return peak;
}

// get spectrum of matrix peak spectrum peak
int IdentificationManager::getMatrixPeakSpectrumSpectrum( const int id ) {
	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return -1;
	}

	// peak
	std::string sql = FMT(
		"select %s from %s where %s = ?;",
		COL_SPEC_MATRIX_SPEC,
		TABLE_MATRIX_SPEC,
		COL_ID_MATRIX_SPEC
	);

	int spec = -1;
	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindInteger( 1, id );
		kome::db::ResultSet* rs = stmt->executeQuery();
		if( rs != NULL ) {
			if( rs->next() ) {
				spec = rs->getInt( 0, -1 );
			}
			rs->close();
		}
		stmt->close();
	}

	return spec;
}

// add hit item
int IdentificationManager::addHitItem(
		const int engineId,
		const char* name,
		const SubstanceType subType,
		const ValueType valType
) {
	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return -1;
	}

	// search
	int id = -1;
	std::string sql = FMT(
		"select %s from %s where %s = ? and %s = ? and %s = ? and %s = ?;",
		COL_ID_HIT_ITEM,
		TABLE_HIT_ITEM,
		COL_NAME_HIT_ITEM,
		COL_ENGINE_HIT_ITEM,
		COL_SUB_TYPE_HIT_ITEM,
		COL_VAL_TYPE_HIT_ITEM
	);

	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindString( 1, NVL( name, "" ) );
		stmt->bindInteger( 2, engineId );
		stmt->bindInteger( 3, (int)subType );
		stmt->bindInteger( 4, (int)valType );

		kome::db::ResultSet* rs = stmt->executeQuery();
		if( rs != NULL ) {
			if( rs->next() ) {
				id = rs->getInt( 0, -1 );
			}
			rs->close();
		}
		stmt->close();
	}

	if( id >= 0 ) {
		return id;
	}

	// add
	bool ret = false;
	sql = FMT(
		"insert into %s (%s, %s, %s, %s) values(?, ?, ?, ?);",
		TABLE_HIT_ITEM,
		COL_NAME_HIT_ITEM,
		COL_ENGINE_HIT_ITEM,
		COL_SUB_TYPE_HIT_ITEM,
		COL_VAL_TYPE_HIT_ITEM
	);

	stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindString( 1, NVL( name, "" ) );
		stmt->bindInteger( 2, engineId );
		stmt->bindInteger( 3, (int)subType );
		stmt->bindInteger( 4, (int)valType );

		ret = stmt->execute();
		stmt->close();
	}

	if( ret ) {
		id = db->getMaxValue( TABLE_HIT_ITEM, COL_ID_HIT_ITEM );
		if( id < 0 ) {
			id = -1;
		}
	}

	return id;
}

// delete hit item
void IdentificationManager::deleteHitItem( const int engineId ) {
	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return;
	}

	// delete hit item
	std::string sql = FMT( "delete from %s where %s = ?;" TABLE_HIT_ITEM, COL_ENGINE_HIT_ITEM );
	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindInteger( 1, engineId );
		stmt->execute();
		stmt->close();
	}
}

// set hit value
void IdentificationManager::setHitValue( const int hitId, const int itemId, const char* val ) {
	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return;
	}

	// search
	int id = -1;
	std::string sql = FMT(
		"select %s from %s where %s = ? and %s = ?;",
		COL_ID_HIT_VAL,
		TABLE_HIT_VALUE,
		COL_HIT_HIT_VAL,
		COL_ITEM_HIT_VAL
	);

	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindInteger( 1, hitId );
		stmt->bindInteger( 2, itemId );

		kome::db::ResultSet* rs = stmt->executeQuery();
		if( rs != NULL ) {
			if( rs->next() ) {
				id = rs->getInt( 0, -1 );
			}
			rs->close();
		}
		stmt->close();
	}

	// set
	if( id < 0 ) {    // insert
		sql = FMT(
			"insert into %s (%s, %s, %s) values (?, ?, ?);",
			TABLE_HIT_VALUE,
			COL_HIT_HIT_VAL,
			COL_ITEM_HIT_VAL,
			COL_VAL_HIT_VAL
		);

		stmt = db->prepare( sql.c_str() );
		if( stmt != NULL ) {
			stmt->bindInteger( 1, hitId );
			stmt->bindInteger( 2, itemId );
			stmt->bindString( 3, NVL( val, "" ) );

			stmt->execute();
			stmt->close();
		}
	}
	else {    // update
		sql = FMT(
			"update %s set %s = ? where %s = ?;",
			TABLE_HIT_VALUE,
			COL_VAL_HIT_VAL,
			COL_ID_HIT_VAL
		);

		stmt = db->prepare( sql.c_str() );
		if( stmt != NULL ) {
			stmt->bindString( 1, NVL( val, "" ) );
			stmt->bindInteger( 2, id );

			stmt->execute();
			stmt->close();
		}
	}
}

// delete hit value
void IdentificationManager::deleteHitValue( const int hitId ) {
	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return;
	}

	// delete
	std::string sql = FMT(
		"delete from %s where %s = ?;",
		TABLE_HIT_VALUE,
		COL_HIT_HIT_VAL
	);

	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindInteger( 1, hitId );
		stmt->execute();
		stmt->close();
	}
}

// add hit spectrum
void IdentificationManager::addHitSpectrum( const int hitId, const int searchSpecId, const int charge ) {
	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return;
	}

	// search
	int id = -1;
	std::string sql = FMT(
		"select %s from %s where %s = ? and %s = ?;",
		COL_ID_HIT_SPEC,
		TABLE_HIT_SPEC,
		COL_HIT_HIT_SPEC,
		COL_SPEC_HIT_SPEC
	);

	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindInteger( 1, hitId );
		stmt->bindInteger( 2, searchSpecId );

		kome::db::ResultSet* rs = stmt->executeQuery();
		if( rs != NULL ) {
			if( rs->next() ) {
				id = rs->getInt( 0, -1 );
			}
			rs->close();
		}
		stmt->close();
	}

	// add
	if( id < 0 ) {
		sql = FMT(
			"insert into %s (%s, %s, %s) values (?, ?, ?);",
			TABLE_HIT_SPEC,
			COL_HIT_HIT_SPEC,
			COL_SPEC_HIT_SPEC,
			COL_CHARGE_HIT_SPEC
		);

		kome::db::Statement* stmt = db->prepare( sql.c_str() );
		if( stmt != NULL ) {
			stmt->bindInteger( 1, hitId );
			stmt->bindInteger( 2, searchSpecId );
			stmt->bindInteger( 3, charge );

			stmt->execute();
			stmt->close();
		}
	}
}

// delete hit spectra from hit
void IdentificationManager::deleteHitSpectraFromHit( const int hitId ) {
	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return;
	}

	// delete
	std::string sql = FMT(
		"delete from %s where %s = ?;",
		TABLE_HIT_SPEC,
		COL_HIT_HIT_SPEC
	);

	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindInteger( 1, hitId );

		stmt->execute();
		stmt->close();
	}
}

// delete hit spectra from spectrum
void IdentificationManager::deleteHitSpectraFromSpectrum( const int searchSpecId ) {
	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return;
	}

	// delete
	std::string sql = FMT(
		"delete from %s where %s = ?;",
		TABLE_HIT_SPEC,
		COL_SPEC_HIT_SPEC
	);

	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindInteger( 1, searchSpecId );

		stmt->execute();
		stmt->close();
	}
}

// add hit 
int IdentificationManager::addHit( HitInfo& hit ) {
	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return -1;
	}

	// add
	std::string sql = FMT(
		"insert into %s (%s) values(?);",
		TABLE_HIT,
		COL_SUBSTANCE_HIT
	);

	bool ret = false;
	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindInteger( 1, hit.substance );
		ret = stmt->execute();
		stmt->close();
	}

	if( !ret ) {
		return -1;
	}

	// ID
	int id = db->getMaxValue( TABLE_HIT, COL_ID_HIT );
	if( id < 0 ) {
		return -1;
	}

	// substance
	SubstanceInfo substance;
	getSubstance( hit.substance, substance );

	for( unsigned int i = 0; i < hit.altSubstances.size(); i++ ) {
		addAltSubstance(
			id,
			hit.altSubstances.at( i ).id,
			hit.altSubstances.at( i ).before.c_str(),
			hit.altSubstances.at( i ).after.c_str(),
			hit.altSubstances.at( i ).start,
			hit.altSubstances.at( i ).end
		);
	}

	// search, engine
	int searchId = -1;
	int engineId = -1;
	if( hit.spectra.size() > 0 ) {
		int spectrum = ( hit.spectra )[ 0 ];
		getSearchSpectrum( spectrum, NULL, &searchId, NULL, NULL );

		if( searchId >= 0 ) {
			SearchInfo search;
			getSearch( searchId, search );

			engineId = search.engineId;
		}
	}

	if( searchId < 0 || engineId < 0 ) {
		return id;
	}

	// spectra
	for( unsigned int i = 0; i < hit.spectra.size(); i++ ) {
		addHitSpectrum( id, ( hit.spectra )[ i ], hit.charge );
	}

	// hit values
	for( unsigned int i = 0; i < hit.props.size(); i++ ) {
		std::string n = hit.props.at( i ).name;
		std::string v = hit.props.at( i ).value;
		ValueType t = hit.props.at( i ).type;

		int itemId = addHitItem( engineId, n.c_str(), substance.type, t );
		setHitValue( id, itemId, v.c_str() );
	}

	// modifications
	std::vector< int > searchMods;
	std::map< int, int > searchModMap;
	getSearchModifications( searchMods, searchId );

	for( unsigned int j = 0; j < searchMods.size(); j++ ) {
		int modId = getModificationFromSearchModification( searchMods[ j ] );
		if( modId >= 0 ) {
			searchModMap[ modId ] = searchMods[ j ];
		}
	}

	for( unsigned int i = 0; i < hit.mods.size(); i++ ) {
		int modId = hit.mods.at( i ).first->getId();
		int pos = hit.mods.at( i ).second;

		if( pos >= 0 && searchModMap.find( modId ) != searchModMap.end() ) {
			addHitModification( id, searchModMap[ modId ], pos );
		}
	}

	return id;
}

// delete hit
void IdentificationManager::deleteHit( const int id ) {
	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return;
	}

	// delete values
	deleteHitValue( id );

	// delete spectra
	deleteHitSpectraFromHit( id );

	// delete alternative substances
	deleteAltSubstancesByHit( id );

	// delete hit modifications
	deleteHitModifications( id );

	// delete
	std::string sql = FMT(
		"delete from %s where %s = ?;",
		TABLE_HIT,
		COL_ID_HIT
	);

	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindInteger( 1, id );
		stmt->execute();
		stmt->close();
	}
}

// get hits from substance
void IdentificationManager::getHitsFromSubstance( std::vector< int >& hits, const int substanceId ) {
	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return;
	}

	// hits
	std::string sql = FMT(
		"select %s from %s where %s = ?;",
		COL_ID_HIT,
		TABLE_HIT,
		COL_SUBSTANCE_HIT
	);

	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindInteger( 1, substanceId );
		kome::db::ResultSet* rs = stmt->executeQuery();
		if( rs != NULL ) {
			while( rs->next() ) {
				const int id = rs->getInt( 0, -1 );
				if( id >= 0 ) {
					hits.push_back( id );
				}
			}
			rs->close();
		}
		stmt->close();
	}
}

// get hits from search
void IdentificationManager::getHitsFromSearch( std::vector< int >& hits, const int searchId ) {
	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return;
	}

	// hits
	std::string sql = FMT(
		"select distinct s.%s from %s as s, %s as t where s.%s = t.%s and t.%s = ?;",
		COL_HIT_HIT_SPEC,
		TABLE_HIT_SPEC,
		TABLE_SEARCH_SPEC,
		COL_SPEC_HIT_SPEC,
		COL_ID_SEARCH_SPEC,
		COL_SEARCH_SEARCH_SPEC
	);

	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindInteger( 1, searchId );

		kome::db::ResultSet* rs = stmt->executeQuery();
		if( rs != NULL ) {
			while( rs->next() ) {
				const int id = rs->getInt( 0, -1 );
				if( id >= 0 ) {
					hits.push_back( id );
				}
			}
			rs->close();
		}
		stmt->close();
	}
}

// get hits from spectrum
void IdentificationManager::getHitsFromSpectrum( std::vector< int >& hits, const int searchSpecId ) {
	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return;
	}

	// hit
	std::string sql = FMT(
		"select %s from %s where %s = ?;",
		COL_HIT_HIT_SPEC,
		TABLE_HIT_SPEC,
		COL_SPEC_HIT_SPEC
	);

	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindInteger( 1, searchSpecId );

		kome::db::ResultSet* rs = stmt->executeQuery();
		if( rs != NULL ) {
			while( rs->next() ) {
				const int id = rs->getInt( 0, -1 );
				if( id >= 0 ) {
					hits.push_back( id );
				}
			}
			rs->close();
		}
		stmt->close();
	}
}

// get all hits
void IdentificationManager::getHits( std::vector< int >& hits ) {
	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return;
	}

	// hits
	std::string sql = FMT(
		"select %s from %s;",
		COL_ID_HIT,
		TABLE_HIT
	);
	
	kome::db::ResultSet* rs = db->executeQuery( sql.c_str() );
	if( rs != NULL ) {
		while( rs->next() ) {
			const int id = rs->getInt( 0, -1 );
			if( id >= 0 ) {
				hits.push_back( id );
			}
		}
		rs->close();
	}
}

// get hit
void IdentificationManager::getHit( const int id, HitInfo& hit ) {
	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return;
	}
	hit.id = id;
	// substance
	int substance = -1;
	std::string sql = FMT(
		"select %s from %s where %s = ?;",
		COL_SUBSTANCE_HIT,
		TABLE_HIT,
		COL_ID_HIT
	);

	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindInteger( 1, id );

		kome::db::ResultSet* rs = stmt->executeQuery();
		if( rs != NULL ) {
			if( rs->next() ) {
				substance = rs->getInt( 0, -1 );
			}
			rs->close();
		}

		stmt->close();
	}

	hit.substance = substance;

	// spectra
	sql = FMT(
		"select %s, %s from %s where %s = ?;",
		COL_SPEC_HIT_SPEC,
		COL_CHARGE_HIT_SPEC,
		TABLE_HIT_SPEC,
		COL_HIT_HIT_SPEC
	);

	stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindInteger( 1, id );

		kome::db::ResultSet* rs = stmt->executeQuery();
		if( rs != NULL ) {
			while( rs->next() ) {
				const int spec = rs->getInt( 0, -1 );
				if( spec >= 0 ) {
					hit.spectra.push_back( spec );
				}
				const int charge = rs->getInt( 1, -1 );
				hit.charge = charge;
			}
			rs->close();
		}
		stmt->close();
	}

	// values
	sql = FMT(
		"select n.%s, n.%s, v.%s from %s as n, %s as v where n.%s = v.%s and v.%s = ?;",
		COL_NAME_HIT_ITEM,
		COL_VAL_TYPE_HIT_ITEM,
		COL_VAL_HIT_VAL,
		TABLE_HIT_ITEM,
		TABLE_HIT_VALUE,
		COL_ID_HIT_ITEM,
		COL_ITEM_HIT_VAL,
		COL_HIT_HIT_VAL
	);

	stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindInteger( 1, id );

		kome::db::ResultSet* rs = stmt->executeQuery();
		if( rs != NULL ) {
			while( rs->next() ) {
				hit.props.resize( hit.props.size() + 1 );
				hit.props.back().name = rs->getString( 0, "" );
				hit.props.back().type = (ValueType)rs->getInt( 1, (int)TYPE_PROPERTY );
				hit.props.back().value = rs->getString( 2, "" );
			}
			rs->close();
		}
		stmt->close();
	}

	// alternative substances
	getAltSubstances( id, hit.altSubstances );

	// modifications
	std::vector< int > hitMods;
	getHitModifications( hitMods, id );

	for( unsigned int i = 0; i < hitMods.size(); i++ ) {
		int hitMod = hitMods[ i ];
		int searchMod = getSearchModificationFromHitModification( hitMod );
		int modId = getModificationFromSearchModification( searchMod );
		int pos = getHitModPosition( hitMod );

		Modification* m = getModificationFromId( modId );

		if( m != NULL && pos >= 0 ) {
			hit.mods.push_back( std::make_pair( m, pos ) );
		}
	}
}

// add alternative substance
void IdentificationManager::addAltSubstance( const int hitId, const int substanceId, const char* before, const char* after, int start, int end ) {
	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return;
	}

	// check
	int id = -1;
	std::string sql = FMT(
		"select %s from %s where %s = ? and %s = ?;",
		COL_ID_ALT_SUBSTANCE,
		TABLE_ALT_SUBSTANCE,
		COL_HIT_ALT_SUBSTANCE,
		COL_SUBSTANCE_ALT_SUBSTANCE
	);

	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindInteger( 1, hitId );
		stmt->bindInteger( 2, substanceId );

		kome::db::ResultSet* rs = stmt->executeQuery();
		if( rs != NULL ) {
			if( rs->next() ) {
				id = rs->getInt( 0, -1 );
			}
			rs->close();
		}
		stmt->close();
	}

	if( id < 0 ) {
		sql = FMT( 
			"insert into %s (%s, %s, %s, %s, %s, %s) values (?, ?, ?, ?, ?, ?);",
			TABLE_ALT_SUBSTANCE,
			COL_HIT_ALT_SUBSTANCE,
			COL_SUBSTANCE_ALT_SUBSTANCE,
			COL_START_ALT_SUBSTANCE,
			COL_END_ALT_SUBSTANCE,
			COL_BEFORE_ALT_SUBSTANCE,
			COL_AFTER_ALT_SUBSTANCE
	   	);

		stmt = db->prepare( sql.c_str() );
		if( stmt != NULL ) {
			stmt->bindInteger( 1, hitId );
			stmt->bindInteger( 2, substanceId );
			stmt->bindInteger( 3, start );
			stmt->bindInteger( 4, end );
			stmt->bindString( 5, before );
			stmt->bindString( 6, after );

			stmt->execute();
			stmt->close();
		}
	}
}

// delete alternative substances by hit
void IdentificationManager::deleteAltSubstancesByHit( const int hitId ) {
	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return;
	}

	// delete
	std::string sql = FMT(
		"delete from %s where %s = ?;",
		TABLE_ALT_SUBSTANCE,
		COL_HIT_ALT_SUBSTANCE
	);

	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindInteger( 1, hitId );
		stmt->execute();
		stmt->close();
	}
}

// delete alternative substances by substance
void IdentificationManager::deleteAltSubstancesBySubstance( const int substanceId ) {
	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return;
	}

	// delete
	std::string sql = FMT(
		"delete from %s where %s = ?;",
		TABLE_ALT_SUBSTANCE,
		COL_SUBSTANCE_ALT_SUBSTANCE
	);

	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindInteger( 1, substanceId );
		stmt->execute();
		stmt->close();
	}
}

// get alternative substances
void IdentificationManager::getAltSubstances( const int hitId, std::vector< AltSubstance >& substances ) {
	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return;
	}

	// get
	std::string sql = FMT(
		"select %s, %s, %s, %s, %s from %s where %s = ?",
		COL_SUBSTANCE_ALT_SUBSTANCE,
		COL_START_ALT_SUBSTANCE,
		COL_END_ALT_SUBSTANCE,
		COL_BEFORE_ALT_SUBSTANCE,
		COL_AFTER_ALT_SUBSTANCE,
		TABLE_ALT_SUBSTANCE,
		COL_HIT_ALT_SUBSTANCE
	);

	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindInteger( 1, hitId );
		kome::db::ResultSet* rs = stmt->executeQuery();
		if( rs != NULL ) {
			while( rs->next() ) {
				AltSubstance as;
				as.id = rs->getInt( 0, - 1 );
				as.start = rs->getInt( 1, - 1 );
				as.end = rs->getInt( 2, -1 );
				as.before = rs->getString( 3, "" );
				as.after = rs->getString( 4, "" );
				if( as.id >= 0 && as.start >= 0 && as.end >= as.start ) {
					substances.push_back( as );
				}
			}
			rs->close();
		}
		stmt->close();
	}
}

// set accession number
void IdentificationManager::setAccessionNumber( const char* prefix, const int num ) {
	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return;
	}

	// search
	int id = -1;
	std::string sql = FMT(
		"select %s from %s where %s = ?;",
		COL_ID_ACCESSION,
		TABLE_ACCESSION,
		COL_PREFIX_ACCESSION
	);

	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindString( 1, NVL( prefix, "" ) );
		kome::db::ResultSet* rs = stmt->executeQuery();
		if( rs != NULL ) {
			if( rs->next() ) {
				id = rs->getInt( 0, -1 );
			}
			rs->close();
		}
		stmt->close();
	}

	// set
	if( id < 0 ) {    // insert
		sql = FMT(
			"insert into %s (%s, %s) values (?, ?);",
			TABLE_ACCESSION,
			COL_PREFIX_ACCESSION,
			COL_COUNT_ACCESSION
		);

		stmt = db->prepare( sql.c_str() );
		if( stmt != NULL ) {
			stmt->bindString( 1, NVL( prefix, "" ) );
			stmt->bindInteger( 2, MAX( 0, num ) );

			stmt->execute();
			stmt->close();
		}
	}
	else {    // update
		sql = FMT( 
			"update %s set %s = ? where %s = ?;",
			TABLE_ACCESSION,
			COL_COUNT_ACCESSION,
			COL_ID_ACCESSION
		);

		stmt = db->prepare( sql.c_str() );
		if( stmt != NULL ) {
			stmt->bindInteger( 1, num );
			stmt->bindInteger( 2, id );

			stmt->execute();
			stmt->close();
		}
	}
}

// get accession number
int IdentificationManager::getAccessionNumber( const char* prefix ) {
	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return -1;
	}

	// accession number
	int num = -1;
	std::string sql = FMT(
		"select %s from %s where %s = ?;",
		COL_COUNT_ACCESSION,
		TABLE_ACCESSION,
		COL_PREFIX_ACCESSION
	);

	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindString( 1, NVL( prefix, "" ) );
		kome::db::ResultSet* rs = stmt->executeQuery();
		if( rs != NULL ) {
			if( rs->next() ) {
				num = rs->getInt( 0, -1 );
			}
			rs->close();
		}
		stmt->close();
	}

	// set
	if( num < 0 ) {
		setAccessionNumber( prefix, 0 );
		num = 0;
	}

	return num;
}


// issue accession number
int IdentificationManager::issueAccessionNumber( const char* prefix ) {
	// accession number
	int num = getAccessionNumber( prefix );

	// count up
	setAccessionNumber( prefix, num + 1 );

	return num;
}

// get database schema version
int IdentificationManager::getDbVersion() {
	return DB_VERSION;
}

// get current  database schema version
int IdentificationManager::getCurrentDbVersion() {
	// database
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return -1;
	}

	// get version
	std::string sql = FMT( "select %s from %s;", COL_VERSION_VERSION, TABLE_VERSION );
	kome::db::ResultSet* rs = db->executeQuery( sql.c_str() );
	if( rs == NULL ) {
		return -1;
	}

	int version = -1;
	if( rs->next() ) {
		version = rs->getInt( 0, -1 );
	}
	rs->close();

	if( version >= 0 ) {
		return version;
	}

	// old table
	if( db->hasTable( "PKMTX_VERSION" ) ) {
		sql = "select VERSION from PKMTX_VERSION;";
		rs = db->executeQuery( sql.c_str() );
		if( rs == NULL ) {
			return version;
		}

		if( rs->next() ) {
			version = rs->getInt( 1, -1 );
		}
		rs->close();

		if( version >= 0 ) {
			return version;
		}
	}

	if( db->hasTable( "PKMTX_PROJECT" ) ) {
		version = -1;
	}

	// set version
	if( version < 0 ) {
		version = getDbVersion();
		setDbVersion( version );
	}

	return version;
}

// set db version
void IdentificationManager::setDbVersion( const int version ) {
	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return;
	}

	// init
	kome::db::Table table;
	createVersionTable( table );
	db->initTable( table );

	// set version
	std::string sql = FMT(
		"insert into %s (%s) values (?);",
		TABLE_VERSION,
		COL_VERSION_VERSION
	);

	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindInteger( 1, version );
		stmt->execute();
		stmt->close();
	}
}

// convert database version
bool IdentificationManager::convertDbSchema( kome::core::Progress& progress ) {
	// database
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return false;
	}

	// current version
	int version = getCurrentDbVersion();
	if( version < 0 ) {
		LOG_ERROR( FMT( "Current Db version is too old to convert" ) );
		return false;
	}
	version = MAX( 1, version );
	int num = getDbVersion() - version;
	if( num <= 0 ) {
		return true;
	}

	if( num > 1 ) {
		progress.createSubProgresses( num );
	}
	int cnt = 0;
	kome::core::Progress* prgs = NULL;

	// convert
	switch( version ) {
	case 1:
		if( !progress.isStopped() ) {
			prgs = ( num > 1 ? progress.getSubProgress( cnt ) : &progress );
			cnt++;
			convertVersion1ToVersion2( *prgs );
		}
	case 2:
		if( !progress.isStopped() ) {
			prgs = ( num > 1 ? progress.getSubProgress( cnt ) : &progress );
			cnt++;
			convertVersion2ToVersion3( *prgs );
		}
	case 3:
		if( !progress.isStopped() ) {
			prgs = ( num > 1 ? progress.getSubProgress( cnt ) : &progress );
			cnt++;
			convertVersion3ToVersion4( *prgs );
		}
	case 4:
		if( !progress.isStopped() ) {
			prgs = ( num > 1 ? progress.getSubProgress( cnt ) : &progress );
			cnt++;
			convertVersion4ToVersion5( *prgs );
		}
	case 5:
		if( !progress.isStopped() ) {
			prgs = ( num > 1 ? progress.getSubProgress( cnt ) : &progress );
			cnt++;
			convertVersion5ToVersion6( *prgs );
		}
	case 6:
		if( !progress.isStopped() ) {
			prgs = ( num > 1 ? progress.getSubProgress( cnt ) : &progress );
			cnt++;
			convertVersion6ToVersion7( *prgs );
		}
	case 7:
	    if( !progress.isStopped() ) {
			prgs = ( num > 1 ? progress.getSubProgress( cnt ) : &progress );
			cnt++;
			convertVersion7To8( *prgs );
		}
	case 8:		
		if( !progress.isStopped() ) {
			prgs = ( num > 1 ? progress.getSubProgress( cnt ) : &progress );
			cnt++;
			convertVersion8To9( *prgs );
		}
	}
	db->commit();

	if( progress.isStopped() ) {
		kome::db::Database* db = getDb();
		if( db != NULL ) {
			for( unsigned int i = 0; i < m_tables.size(); i++ ) {
				db->dropTable( m_tables[ i ].getName() );
			}
			db->commit();
		}
	}
	else {
		progress.fill();
	}

	return true;
}

// convert ver 1 -> ver 2
void IdentificationManager::convertVersion1ToVersion2( kome::core::Progress& progress ) {
	// database
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return;
	}

	// progress
	progress.createSubProgresses( 8 );

	// matrix
	progress.setStatus( "Converting Matrices..." );
	kome::core::Progress* prgs = progress.getSubProgress( 0 );
	std::string sql = "select count(*) from PKMTX_PROJECT;";
	kome::db::ResultSet* rs = db->executeQuery( sql.c_str() );
	int num = 0;
	if( rs != NULL ) {
		if( rs->next() ) {
			num = rs->getInt( 0, 0 );
		}
		rs->close();
	}

	int cnt = 0;
	prgs->setRange( 0, num );
	prgs->setPosition( 0 );

	std::map< int, int > matrixMap;
	sql = "select ID, NAME, RT_FLG, MZ_FLG, CHARGE_FLG from PKMTX_PROJECT;";
	rs = db->executeQuery( sql.c_str() );
	if( rs != NULL ) {
		while( rs->next() && !progress.isStopped() ) {
			int id = rs->getInt( 0, -1 );
			std::string name = rs->getString( 1, "" );
			bool rtFlg = ( rs->getInt( 2, 1 ) > 0 );
			bool mzFlg = ( rs->getInt( 3, 1 ) > 0 );
			bool chargeFlg = ( rs->getInt( 4, 0 ) > 0 );

			if( id >= 0 ) {
				matrixMap[ id ] = addMatrix( name.c_str(), "", rtFlg, mzFlg, chargeFlg );
			}
			cnt++;
			prgs->setPosition( cnt );
		}
		rs->close();
	}
	if( progress.isStopped() ) {
		db->rollback();
		return;
	}
	prgs->fill();

	// group
	progress.setStatus( "Converting Groups..." );
	prgs = progress.getSubProgress( 1 );
	sql = "select count(*) from PKMTX_CATEGORY;";
	rs = db->executeQuery( sql.c_str() );
	num = 0;
	if( rs != NULL ) {
		if( rs->next() ) {
			num = rs->getInt( 0, 0 );
		}
		rs->close();
	}

	cnt = 0;
	prgs->setRange( 0, num );
	prgs->setPosition( 0 );

	std::map< int, int > groupMap;
	sql = "select ID, PROJECT_ID, NAME, COLOR from PKMTX_CATEGORY;";
	rs = db->executeQuery( sql.c_str() );
	if( rs != NULL ) {
		while( rs->next() && !progress.isStopped() ) {
			int id = rs->getInt( 0, -1 );
			int project = rs->getInt( 1, -1 );
			std::string name = rs->getString( 2, "" );
			COLOR color = kome::img::ColorTool::getColor( rs->getString( 3, "" ).c_str() );

			if( id >= 0 && matrixMap.find( project ) != matrixMap.end() ) {
				groupMap[ id ] = addGroup( name.c_str(), matrixMap[ project ], color );
			}
			cnt++;
			prgs->setPosition( cnt );
		}
		rs->close();
	}
	if( progress.isStopped() ) {
		db->rollback();
		return;
	}
	prgs->fill();

	// column
	progress.setStatus( "Converting Columns..." );
	prgs = progress.getSubProgress( 2 );
	sql = "select count(*) from PKMTX_APPENDED_COLUMN;";
	rs = db->executeQuery( sql.c_str() );
	num = 0;
	if( rs != NULL ) {
		if( rs->next() ) {
			num = rs->getInt( 0, 0 );
		}
		rs->close();
	}

	cnt = 0;
	prgs->setRange( 0, num );
	prgs->setPosition( 0 );

	std::map< int, int > colMap;
	sql = "select ID, NAME, TYPE, PROJECT_ID from PKMTX_APPENDED_COLUMN;";
	rs = db->executeQuery( sql.c_str() );
	if( rs != NULL ) {
		while( rs->next() && !progress.isStopped() ) {
			int id = rs->getInt( 0, -1 );
			std::string name = rs->getString( 1, "" );
			int type = rs->getInt( 2, (int)TYPE_STRING );
			int project = rs->getInt( 3, -1 );

			if( id >= 0 && matrixMap.find( project ) != matrixMap.end() ) {
				colMap[ id ] = addMatrixColumn( name.c_str(), (ColumnDataType)type, matrixMap[ project ] );
			}
			cnt++;
			prgs->setPosition( cnt );
		}
		rs->close();
	}
	if( progress.isStopped() ) {
		db->rollback();
		return;
	}
	prgs->fill();

	// peaks
	progress.setStatus( "Converting Peaks..." );
	prgs = progress.getSubProgress( 3 );
	sql = "select count(*) from PKMTX_MATRIX_PEAK;";
	rs = db->executeQuery( sql.c_str() );
	num = 0;
	if( rs != NULL ) {
		if( rs->next() ) {
			num = rs->getInt( 0, 0 );
		}
		rs->close();
	}

	cnt = 0;
	prgs->setRange( 0, num );
	prgs->setPosition( 0 );

	std::map< int, int > peakMap;
	sql = "select ID, PROJECT_ID, RT, MZ, CHARGE from PKMTX_MATRIX_PEAK;";
	rs = db->executeQuery( sql.c_str() );
	if( rs != NULL ) {
		while( rs->next() && !progress.isStopped() ) {
			int id = rs->getInt( 0, -1 );
			int project = rs->getInt( 1, -1 );
			double rt = rs->getReal( 2, -1.0 );
			double mz = rs->getReal( 3, -1.0 );
			int charge = rs->getInt( 4, -1 );

			if( id >= 0 && matrixMap.find( project ) != matrixMap.end() ) {
				peakMap[ id ] = addMatrixPeak( rt, mz, charge, matrixMap[ project ] );
			}
			cnt++;
			prgs->setPosition( cnt );
		}
		rs->close();
	}
	matrixMap.clear();

	if( progress.isStopped() ) {
		db->rollback();
		return;
	}
	prgs->fill();

	// samples
	progress.setStatus( "Converting Samples..." );
	prgs = progress.getSubProgress( 4 );
	sql = "select count(*) from PKMTX_SPECTRUM_GROUP;";
	rs = db->executeQuery( sql.c_str() );
	num = 0;
	if( rs != NULL ) {
		if( rs->next() ) {
			num = rs->getInt( 0, 0 );
		}
		rs->close();
	}

	cnt = 0;
	prgs->setRange( 0, num );
	prgs->setPosition( 0 );

	std::map< int, int > sampleMap;
	sql = "select g.ID, g.GROUP_NAME, g.COLOR, s.SAMPLE_NUM, s.NAME, ss.PATH, g.CATEGORY_ID";
	sql.append( " from PKMTX_SPECTRUM_GROUP as g, PKMTX_SAMPLE as s, PKMTX_SAMPLE_SET as ss" );
	sql.append( " where g.SAMPLE_ID = s.ID and s.SAMPLE_SET_ID = ss.ID;" );
	rs = db->executeQuery( sql.c_str() );
	if( rs != NULL ) {
		std::set< long long > sampleGroupSet;
		while( rs->next() && !progress.isStopped() ) {
			int id = rs->getInt( 0, -1 );
			std::string name = rs->getString( 1, "" );
			std::string color = rs->getString( 2, "" );
			int sampleIndex = rs->getInt( 3, -1 );
			std::string sampleName = rs->getString( 4, "" );
			std::string path = rs->getString( 5, "" );
			int group = rs->getInt( 6, -1 );

			if( id >= 0 && groupMap.find( group ) != groupMap.end() ) {
				long long sample = (long long)addSample( path.c_str(), sampleIndex, sampleName.c_str() );
				group = groupMap[ group ];

				long long sampleGroup = ( ( sample << 32 ) + group );
				if( sampleGroupSet.find( sampleGroup ) == sampleGroupSet.end() ) {
					sampleGroupSet.insert( sampleGroup );

					sampleMap[ id ] = addMatrixSample(
						group,
						sample,
						name.c_str(),
						kome::img::ColorTool::getColor( color.c_str() )
					);
				}
			}
			cnt++;
			prgs->setPosition( cnt );
		}
		rs->close();
	}
	groupMap.clear();

	if( progress.isStopped() ) {
		db->rollback();
		return;
	}
	prgs->fill();

	// values
	progress.setStatus( "Converting Values..." );
	prgs = progress.getSubProgress( 5 );
	sql = "select count(*) from PKMTX_APPENDED_VALUE;";
	rs = db->executeQuery( sql.c_str() );
	num = 0;
	if( rs != NULL ) {
		if( rs->next() ) {
			num = rs->getInt( 0, 0 );
		}
		rs->close();
	}

	cnt = 0;
	prgs->setRange( 0, num );
	prgs->setPosition( 0 );

	sql = "select COL_ID, PEAK_ID, VALUE from PKMTX_APPENDED_VALUE;";
	rs = db->executeQuery( sql.c_str() );
	if( rs != NULL ) {
		while( rs->next() && !progress.isStopped() ) {
			int col = rs->getInt( 0, -1 );
			int peak = rs->getInt( 1, -1 );
			std::string val = rs->getString( 2, "" );

			if( colMap.find( col ) != colMap.end() && peakMap.find( peak ) != peakMap.end() ) {
				setAppendedValue( peakMap[ peak ], colMap[ col ], val.c_str() );
			}
			cnt++;
			prgs->setPosition( cnt );
		}
		rs->close();
	}
	colMap.clear();

	if( progress.isStopped() ) {
		db->rollback();
		return;
	}
	prgs->fill();

	// intensity
	progress.setStatus( "Converting Profiles..." );
	prgs = progress.getSubProgress( 6 );
	sql = "select count(*) from PKMTX_INTENSITY;";
	rs = db->executeQuery( sql.c_str() );
	num = 0;
	if( rs != NULL ) {
		if( rs->next() ) {
			num = rs->getInt( 0, 0 );
		}
		rs->close();
	}

	cnt = 0;
	prgs->setRange( 0, num );
	prgs->setPosition( 0 );

	sql = "select v.PEAK_ID, v.GROUP_ID, v.PEAK_VALUE, v.CORRECTED_VALUE, p.IS_CHROM, p.POSITION,";
	sql.append( " p.LEFT_X, p.LEFT_Y, p.RIGHT_X, p.RIGHT_Y, p.DATA" );
	sql.append( " from PKMTX_INTENSITY as v, PKMTX_PROFILE as p" );
	sql.append( " where v.PROFILE_ID = p.ID" );

	rs = db->executeQuery( sql.c_str() );
	if( rs != NULL ) {
		while( rs->next() && !progress.isStopped() ) {
			const int peak = rs->getInt( 0, -1 );
			const int group = rs->getInt( 1, -1 );
			const double val = rs->getReal( 2, -1.0 );
			const double normalized = rs->getReal( 3, -1.0 );
			const int isChrom = rs->getInt( 4, 0 );
			const double pos = rs->getReal( 5, -1.0 );
			const double lx = rs->getReal( 6, -1.0 );
			const double ly = rs->getReal( 7, -1.0 );
			const double rx = rs->getReal( 8, -1.0 );
			const double ry = rs->getReal( 9, -1.0 );
			void* data = rs->getData( 10 );
			const unsigned int size = rs->getDataSize( 10 );

			// add
			if( sampleMap.find( group ) != sampleMap.end() && peakMap.find( peak ) != peakMap.end() ) {
				// profile
				int profile = -1;
				if( data != NULL && size > 0 ) {
					kome::core::MemoryAccessor acc( data, size );
					kome::core::DataPoints dps( kome::core::DataPoints::FLOAT );
					dps.importData( boost::bind( &kome::core::DataAccessor::read, &acc, _1, _2 ) );

					if( dps.getLength() > 0 ) {
						profile = setProfile( dps, ( isChrom > 0 ) );
					}
				}

				// peak
				if( pos >= 0.0 ) {
					kome::objects::PeakElement pk( NULL );
					pk.setX( pos );
					if( lx >= 0.0 && ly >= 0.0 && rx >= 0.0 && ry >= 0.0 ) {
						pk.setLeft( lx, ly );
						pk.setRight( rx, ry );
					}

					setPeakValue( peakMap[ peak ], sampleMap[ group ], val, &pk, profile );
				}
				else {
					setPeakValue( peakMap[ peak ], sampleMap[ group ], val, NULL, profile );
				}

				setNormalizedIntensity( peakMap[ peak ], sampleMap[ group ], normalized );

				cnt++;
				prgs->setPosition( cnt );
			}
		}
		rs->close();
	}
	sampleMap.clear();
	peakMap.clear();

	if( progress.isStopped() ) {
		db->rollback();
		return;
	}
	prgs->fill();
	db->commit();

	// drop tables
	progress.setStatus( "Dropping old tables..." );
	prgs = progress.getSubProgress( 7 );

	const char* tables[] = {
		"PKMTX_INTENSITY",
		"PKMTX_PROFILE",
		"PKMTX_SPECTRUM_GROUP",
		"PKMTX_SAMPLE",
		"PKMTX_SAMPLE_SET",
		"PKMTX_CATEGORY",
		"PKMTX_VERSION",
		"PKMTX_APPENDED_VALUE",
		"PKMTX_CTRLPT",
		"PKMTX_MATRIX_PEAK",
		"PKMTX_APPENDED_COLUMN",
		"PKMTX_PROJECT",
	};

	num = sizeof( tables ) / sizeof( const char* );
	prgs->setRange( 0, num + 1 );
	prgs->setPosition( 0 );
	for( int i = 0; i < num; i++ ) {
		db->dropTable( tables[ i ] );
		prgs->setPosition( i + 1 );
	}

	// set version
	setDbVersion( 2 );
	prgs->fill();

	// commit
	db->commit();
	progress.fill();
}

// convert ver 2 -> ver 3
void IdentificationManager::convertVersion2ToVersion3( kome::core::Progress& progress ) {
	// DB
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return;
	}

	// search spectra
	std::vector< int > spectra;
	std::string sql = "select ID from MSPP_SEARCH_SPECTRUM;";
	kome::db::ResultSet* rs = db->executeQuery( sql.c_str() );
	if( rs != NULL ) {
		while( rs->next() ) {
			int id = rs->getInt( 0, -1 );
			if( id >= 0 ) {
				spectra.push_back( id );
			}
		}
		rs->close();
	}
	long num = (long)spectra.size();

	// progress
	progress.createSubProgresses( 2 );
	kome::core::Progress* prgs0 = progress.getSubProgress( 0 );
	kome::core::Progress* prgs1 = progress.getSubProgress( 1 );

	// peak file directory
	kome::core::MsppManager& msppMgr = kome::core::MsppManager::getInstance();

	// time
	time_t t = time( NULL );
	struct tm* t2 = localtime( &t );

	// directory
	std::string confDir = msppMgr.getConfDir();
	std::string identDir = getpath( confDir.c_str(), "identification" );
	if( !fileexists( identDir.c_str() ) ) {
		makedirectory( identDir.c_str() );
	}
	std::string peaksDir = getpath( identDir.c_str(), "peaks" );
	if( !fileexists( peaksDir.c_str() ) ) {
		makedirectory( peaksDir.c_str() );
	}
	std::string yearDir = getpath( peaksDir.c_str(), FMT( "%d", ( 1900 + t2->tm_year ) ).c_str() );
	if( !fileexists( yearDir.c_str() ) ) {
		makedirectory( yearDir.c_str() );
	}
	std::string dayDir = getpath( yearDir.c_str(), FMT( "%02d%02d", ( 1 + t2->tm_mon ), t2->tm_mday ).c_str() );
	if( !fileexists( dayDir.c_str() ) ) {
		makedirectory( dayDir.c_str() );
	}

	std::string peakFileDir = dayDir;

	// save peaks
	std::vector< long long > aligns;

	int cnt = 0;
	std::string dir = peakFileDir;
	std::string fileName;
	std::string path;
	do {
		fileName = FMT( "oldpeaks%d.data", cnt );
		cnt++;
		path = getpath( dir.c_str(), fileName.c_str() );
	} while( fileexists( path.c_str() ) );

	FILE* fp = fileopen( path.c_str(), "wb" );
	if( fp == NULL ) {
		LOG_ERROR( FMT( "Failed to open the file. [%s]", path.c_str() ) );
		db->rollback();
		return;
	}
	fwrite( &num, sizeof( num ), 1, fp );

	long long align = 0;
	fwrite( &align, sizeof( align ), num, fp );

	prgs0->setRange( 0, num );
	prgs0->setPosition( 0 );

	sql = "select PEAK_LIST from MSPP_SEARCH_SPECTRUM where ID = ?;";	
	kome::db::Statement* stmt = db->prepare( sql.c_str() );

	if( stmt != NULL ) {
		for( int i = 0; i < num && !progress.isStopped(); i++ ) {
			// progress
			std::string msg = FMT( "Getting Peaks... [%d/%d]", ( i + 1 ), num );
			progress.setStatus( msg.c_str() );

			// align
			align = filetell( fp );
			aligns.push_back( align );
			
			// peak list
			stmt->resetBind();
			stmt->bindInteger( 1, spectra[ i ] );

			rs = stmt->executeQuery();
			if( rs != NULL ) {
				if( rs->next() ) {
					void* addr = rs->getData( 0 );
					int size = rs->getDataSize( 0 );
					fwrite( addr, 1, size, fp );
				}
				rs->close();
			}

			// progress
			prgs0->setPosition( i + 1 );
		}
		stmt->close();
	}

	// write aligns
	fileseek( fp, sizeof( num ), SEEK_SET );
	for( int i = 0; i < num; i++ ) {
		align = aligns[ i ];
		fwrite( &align, sizeof( align ), 1, fp );
	}
	fflush( fp );
	fclose( fp );

	if( progress.isStopped() ) {
		db->rollback();
		return;
	}
	
	prgs0->fill();

	// change the table
	kome::db::Table table;
	kome::db::TableColumn* col = table.addColumn( "PEAKS" );
	col->setDataType( kome::db::TableColumn::VARCHAR );
	std::string colStr = db->getColumnString( *col );

	sql = FMT( "alter table MSPP_SEARCH_SPECTRUM add %s;", colStr.c_str() );
	db->execute( sql.c_str() );

	col = table.addColumn( "CHARGE" );
	col->setDataType( kome::db::TableColumn::INTEGER );
	colStr = db->getColumnString( *col );
	sql = FMT( "alter table MSPP_HIT_SPECTRUM add %s;", colStr.c_str() );
	db->execute( sql.c_str() );

	if( progress.isStopped() ) {
		db->rollback();
		return;
	}

	// set peaks
	prgs1->setRange( 0, num );
	prgs1->setPosition( 0 );

	sql = "update MSPP_SEARCH_SPECTRUM set PEAKS = ? where ID = ?;";
	stmt = db->prepare( sql.c_str() );

	if( stmt != NULL ) {
		for( int i = 0; i < num && !progress.isStopped(); i++ ) {
			std::string msg = FMT( "Register Peaks... [%d/%d]", ( i + 1 ), num );
			progress.setStatus( msg.c_str() );

			stmt->resetBind();
			std::string peakList = FMT( "%s:%d", path.c_str(), i );

			stmt->bindString( 1, peakList.c_str() );
			stmt->bindInteger( 2, spectra[ i ] );

			stmt->execute();

			// progress
			prgs1->setPosition( i + 1 );
		}
		stmt->close();
	}

	if( !progress.isStopped() ) {
		progress.fill();
	}

	// set version
	setDbVersion( 3 );

	// commit
	db->commit();
	progress.fill();
}

// convert ver 3 -> ver 4
void IdentificationManager::convertVersion3ToVersion4( kome::core::Progress& progress ) {
	// DB
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return;
	}

	// progress
	progress.setRange( 0, 1 );
	progress.setPosition( 0 );

	// alter
	kome::db::Table table;
	kome::db::TableColumn* col = table.addColumn( "HIDDEN" );
	col->setDataType( kome::db::TableColumn::INTEGER );
	std::string colStr = db->getColumnString( *col );

	std::string sql = FMT( "alter table MSPP_MODIFICATION add %s;", colStr.c_str() );
	db->execute( sql.c_str() );
	progress.setPosition( 1 );
	if( progress.isStopped() ) {
		db->rollback();
		return;
	}

	// set
	sql = FMT( "update MSPP_MODIFICATION set HIDDEN = 0;" );
	db->execute( sql.c_str() );

	// set version
	setDbVersion( 4 );

	// commit
	db->commit();
	progress.fill();
}

// convert ver 4 -> ver 5
void IdentificationManager::convertVersion4ToVersion5( kome::core::Progress& progress ) {
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return;
	}

	// progress
	progress.setRange( 0, 1 );
	progress.setPosition( 0 );

	// alter (search)
	kome::db::Table table;

	kome::db::TableColumn* col0 = table.addColumn( COL_PREC_MASS_TYPE_SEARCH );
	col0->setDataType( kome::db::TableColumn::INTEGER );
	std::string sql = FMT( "alter table %s add %s;", TABLE_SEARCH, db->getColumnString( *col0 ).c_str() );
	db->execute( sql.c_str() );

	kome::db::TableColumn* col1 = table.addColumn( COL_FRAG_MASS_TYPE_SEARCH );
	col1->setDataType( kome::db::TableColumn::INTEGER );
	sql = FMT( "alter table %s add %s;", TABLE_SEARCH, db->getColumnString( *col1 ).c_str() );
	db->execute( sql.c_str() );

	kome::db::TableColumn* col2 = table.addColumn( "ENZYME" );
	col2->setDataType( kome::db::TableColumn::VARCHAR );
	sql = FMT( "alter table %s add %s;", TABLE_SEARCH, db->getColumnString( *col2 ).c_str() );
	db->execute( sql.c_str() );

	kome::db::TableColumn* col3 = table.addColumn( COL_BASE_NAME_SEARCH );
	col3->setDataType( kome::db::TableColumn::VARCHAR );
	sql = FMT( "alter table %s add %s;", TABLE_SEARCH, db->getColumnString( *col3 ).c_str() );
	db->execute( sql.c_str() );

	// alter (spectrum)
	kome::db::TableColumn* col4 = table.addColumn( COL_POLARITY_SPECTRUM );
	col4->setDataType( kome::db::TableColumn::INTEGER );
	sql = FMT( "alter table %s add %s;", TABLE_SPECTRUM, db->getColumnString( *col4 ).c_str() );
	db->execute( sql.c_str() );

	progress.setPosition( 1 );
	if( progress.isStopped() ) {
		db->rollback();
		return;
	}

	// set
	sql = FMT(
		"update %s set %s = ?, %s = ?, %s = ?, %s = ?;",
		TABLE_SEARCH,
		COL_PREC_MASS_TYPE_SEARCH,
		COL_FRAG_MASS_TYPE_SEARCH,
		"ENZYME",
		COL_BASE_NAME_SEARCH
	);

	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindInteger( 1, (int)SearchResult::MASS_UNKNOWN );
		stmt->bindInteger( 2, (int)SearchResult::MASS_UNKNOWN );
		stmt->bindString( 3, "N/A" );
		stmt->bindString( 4, "" );

		stmt->execute();
		stmt->close();
	}

	// set version
	setDbVersion( 5 );

	// commit
	db->commit();
	progress.fill();
}

// convert ver 5 -> ver 6
void IdentificationManager::convertVersion5ToVersion6( kome::core::Progress& progress ) {
	// database
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return;
	}

	// progress
	progress.createSubProgresses( 3 );
	kome::core::Progress* prgs = progress.getSubProgress( 0 );
	prgs->setRange( 0, 2 );
	progress.setStatus( "Adding table columns..." );

	// alter table
	kome::db::Table table;
	kome::db::TableColumn* col0 = table.addColumn( "SEARCH_ID" );
	col0->setDataType( kome::db::TableColumn::VARCHAR );
	std::string sql = FMT( "alter table MSPP_DB_SEARCH add %s;", db->getColumnString( *col0 ).c_str() );
	db->execute( sql.c_str() );
	prgs->setPosition( 1 );

	kome::db::TableColumn* col1 = table.addColumn( "STATUS" );
	col1->setDataType( kome::db::TableColumn::INTEGER );
	sql = FMT( "alter table MSPP_DB_SEARCH add %s;", db->getColumnString( *col1 ).c_str() );
	db->execute( sql.c_str() );

	sql = "update MSPP_DB_SEARCH set STATUS = ?;";
	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindInteger( 1, (int)SearchEngine::STATUS_COMPLETED );
		stmt->execute();
		stmt->close();
	}
	prgs->setPosition( 2 );

	// peak list files
	struct SearchSpectrum {
		int id;
		std::string peaks;
	};
	std::vector< SearchSpectrum > spectra;

	sql = "select ID, PEAKS from MSPP_SEARCH_SPECTRUM";
	kome::db::ResultSet* rs = db->executeQuery( sql.c_str() );
	if( rs != NULL ) {
		while( rs->next() ) {
			int id = rs->getInt( 0, -1 );
			std::string peaks = rs->getString( 1, "" );

			if( id >= 0 && !peaks.empty() ) {
				spectra.resize( spectra.size() + 1 );
				spectra.back().id = id;
				spectra.back().peaks = peaks;
			}
		}
		rs->close();
	}

	// progress
	progress.setStatus( "Converting peak list..." );
	prgs = progress.getSubProgress( 1 );
	prgs->setRange( 0, spectra.size() );

	// alter table
	kome::db::TableColumn* col = table.addColumn( "PEAK_LIST" );
	col->setDataType( kome::db::TableColumn::BINARY );

	sql = FMT( "alter table MSPP_SEARCH_SPECTRUM add %s;", db->getColumnString( *col ).c_str() );
	db->execute( sql.c_str() );

	sql = "update MSPP_SEARCH_SPECTRUM set PEAK_LIST = ? where ID = ?;";
	stmt = db->prepare( sql.c_str() );

	// set peak list
	std::set< std::string > files;
	if( stmt != NULL ) {
		// set peak list
		for( unsigned int i = 0; i < spectra.size() && !progress.isStopped(); i++ ) {
			int id = spectra[ i ].id;
			std::string peaks = spectra[ i ].peaks;
			size_t pos = peaks.rfind( ":" );
			if( pos != peaks.npos ) {
				std::string filePath = peaks.substr( 0, pos );
				files.insert( filePath );
			}

			kome::core::DataPoints dps( kome::core::DataPoints::FLOAT );
			getPeakList( peaks.c_str(), dps );
				
			// set peak list
			kome::core::Buffer buff;
			dps.exportData( boost::bind( &kome::core::DataAccessor::write, &buff, _1, _2 ) );
					
			stmt->resetBind();
			stmt->bindData( 1, buff.getBuffer(), buff.getLength() );
			stmt->bindInteger( 2, id );
					
			stmt->execute();
					
			prgs->setPosition( i + 1 );
		}
		stmt->close();
	}

	if( progress.isStopped() ) {
		db->rollback();
		return;
	}
	prgs->fill();

	// delete files
	prgs = progress.getSubProgress( 2 );
	progress.setStatus( "Deleting old files..." );
	prgs->setRange( 0, files.size() );

	int cnt = 0;
	for( std::set< std::string >::iterator it = files.begin(); it != files.end(); it++ ) {
		std::string path = *it;
//		removefile( path.c_str() );
		cnt++;
		prgs->setPosition( cnt );
	}
	prgs->fill();

	// set version
	setDbVersion( 6 );

	// commit
	db->commit();
	progress.fill();
}

// convert ver 6 -> ver 7
void IdentificationManager::convertVersion6ToVersion7( kome::core::Progress& progress ) {
	// database
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return;
	}

	// table
	kome::db::Table table;

    // progress
	progress.setRange( 0, 4 );
	progress.setPosition( 0 );

	// searches
	std::vector< int > searches;

	std::string sql = "select ID from MSPP_DB_SEARCH;";
	kome::db::ResultSet* rs = db->executeQuery( sql.c_str() );

	if( rs != NULL ) {
		while( rs->next() ) {
			int search = rs->getInt( 0, -1 );
			if( search >= 0 ) {
				searches.push_back( search );
			}
		}
		rs->close();
	}

	if( progress.isStopped() ) {
		db->rollback();
		return;
	}

	// peak detection table
	progress.setStatus( "Creating Peak Detection Table..." );
	std::map< int, int > pkMap;

	sql = "select SEARCH, PARAM_NAME, PARAM_VALUE from MSPP_PEAK_DETECTION_PARAMETERS;";
	rs = db->executeQuery( sql.c_str() );

	if( rs != NULL ) {
		while( rs->next() ) {
			int search = rs->getInt( 0, -1 );
			std::string name = rs->getString( 1, "" );
			std::string val = rs->getString( 2, "" );

			if( search >= 0 && !name.empty() ) {
				if( name.compare( "Peak Detector" ) == 0 ) {
					int id = addPeakDetection( val.c_str() );

					pkMap[ search ] = id;
				}
			}
		}
		rs->close();
	}

	if( progress.isStopped() ) {
		db->rollback();
		return;
	}
	progress.setPosition( 1 );

	// peak parameters
	kome::db::TableColumn* col = table.addColumn( "PEAK_DETECTION" );
	col->setDataType( kome::db::TableColumn::INTEGER );

	sql = FMT( "alter table MSPP_PEAK_DETECTION_PARAMETERS add %s;", db->getColumnString( *col ).c_str() );
	db->execute( sql.c_str() );

	sql = "update MSPP_PEAK_DETECTION_PARAMETERS set PEAK_DETECTION = ? where SEARCH = ?;";
	kome::db::Statement* stmt = db->prepare( sql.c_str() );

	if( stmt != NULL ) {
		for( unsigned int i = 0; i < searches.size(); i++ ) {
			int search = searches[ i ];
			stmt->resetBind();
			int detection = -1;
			if( pkMap.find( searches[ i ] ) == pkMap.end() ) {
				detection = addPeakDetection( "" );
				pkMap[ search ] = detection;
			}
			else {
				detection = pkMap[ search ];
			}

			stmt->bindInteger( 1, detection );
			stmt->bindInteger( 2, search );

			stmt->execute();
		}
		stmt->close();
	}

	if( progress.isStopped() ) {
		db->rollback();
		return;
	}
	progress.setPosition( 2 );

	// peaks
	std::map< int, int > pkMap2;

	sql = "select ID, SEARCH, PEAK_LIST from MSPP_SEARCH_SPECTRUM;";
	rs = db->executeQuery( sql.c_str() );

	if( rs != NULL ) {
		while( rs->next() ) {
			int spectrum = rs->getInt( 0, -1 );
			int search = rs->getInt( 2, -1 );
			void* addr = rs->getData( 3 );
			int size = rs->getDataSize( 3 );

			if( spectrum >= 0 && search >= 0 && addr != NULL && size > 0 ) {
				kome::core::MemoryAccessor acc( addr, size );

				kome::core::DataPoints dps( kome::core::DataPoints::FLOAT );
				dps.importData( boost::bind( &kome::core::DataAccessor::read, &acc, _1, _2 ) );

				int detect = pkMap[ search ];
				int peaks = setPeaks( &dps, detect );

				pkMap2[ spectrum ] = peaks;
			}
		}
		rs->close();
	}

	if( progress.isStopped() ) {
		db->rollback();
		return;
	}
	progress.setPosition( 3 );

	// search spectrum
	col = table.addColumn( "PEAKS" );
	col->setDataType( kome::db::TableColumn::INTEGER );

	sql = FMT( "alter table MSPP_SEARCH_SPECTRUM add %s;", db->getColumnString( *col ).c_str() );
	db->execute( sql.c_str() );

	sql = "update MSPP_SEARCH_SPECTRUM set PEAKS = ? where ID = ?;";
	stmt = db->prepare( sql.c_str() );

	if( stmt != NULL ) {
		for( std::map< int, int >::iterator it = pkMap2.begin(); it != pkMap2.end(); it++ ) {
			int spec = (*it).first;
			int peaks = (*it).second;
			
			stmt->resetBind();
			stmt->bindInteger( 1, peaks );
			stmt->bindInteger( 2, spec );
			
			stmt->execute();
		}
		stmt->close();
	}

	if( progress.isStopped() ) {
		db->rollback();
		return;
	}
	progress.setPosition( 4 );

	progress.fill();
	db->commit();
}

// convert version 7 -> 8
void IdentificationManager::convertVersion7To8( kome::core::Progress& progress ) {
	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return;
	}

	// temporary table
	kome::db::Table table;

	// peak detection and enzyme column in search table
	kome::db::TableColumn* col = table.addColumn( "ENZ" );
	col->setDataType( kome::db::TableColumn::INTEGER );


	std::string sql = FMT(
		"alter table MSPP_DB_SEARCH add %s;",
		db->getColumnString( *col ).c_str()
	);

	db->execute( sql.c_str() );

	sql = "update MSPP_DB_SEARCH set ENZ = ?, PEAK_DETECTION = ?;";

	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindInteger( 1, -1 );
		stmt->bindInteger( 2, -1 );
		stmt->execute();
		stmt->close();
	}

	progress.fill();
}

// convert version 8 -> 9
void IdentificationManager::convertVersion8To9( kome::core::Progress& progress ) {
	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return;
	}

	// column object
	kome::db::Table table;

	kome::db::TableColumn* startCol = table.addColumn( "START_POS" );
	startCol->setDataType( kome::db::TableColumn::INTEGER );

	kome::db::TableColumn* endCol = table.addColumn( "END_POS" );
	endCol->setDataType( kome::db::TableColumn::INTEGER );

	kome::db::TableColumn* beforeCol = table.addColumn( "BEFORE_SEQ" );
	beforeCol->setDataType( kome::db::TableColumn::VARCHAR );

	kome::db::TableColumn* afterCol = table.addColumn( "AFTER_SEQ" );
	afterCol->setDataType( kome::db::TableColumn::VARCHAR );

	// update
	std::string sql = FMT(
		"alter table MSPP_ALT_SUBSTANCE add %s;",
		db->getColumnString( *startCol ).c_str()
	);
	db->execute( sql.c_str() );

	sql = FMT(
		"alter table MSPP_ALT_SUBSTANCE add %s;",
		db->getColumnString( *endCol ).c_str()
	);
	db->execute( sql.c_str() );

	sql = FMT(
		"alter table MSPP_ALT_SUBSTANCE add %s;",
		db->getColumnString( *beforeCol ).c_str()
	);
	db->execute( sql.c_str() );

	sql = FMT(
		"alter table MSPP_ALT_SUBSTANCE add %s;",
		db->getColumnString( *afterCol ).c_str()
	);
	db->execute( sql.c_str() );

	progress.fill();
}

// get peak list
void IdentificationManager::getPeakList( const char* peakList, kome::core::XYData& xyData ) {
	// file path & index
	int idx = -1;
	std::string path;

	std::string peaks = NVL( peakList, "" );
	std::string::size_type pos = peaks.rfind( ":" );
	if( pos != peaks.npos ) {
		path = peaks.substr( 0, pos );
		idx = toint( peaks.substr( pos + 1 ).c_str(), 10, -1 );
	}

	if( path.empty() || idx < 0 ) {
		return;
	}

	// file open
	FILE* fp = fileopen( path.c_str(), "rb" );
	if( fp == NULL ) {
		return;
	}

	// read
	kome::core::DataPoints dps( kome::core::DataPoints::FLOAT );
	fileseek( fp, sizeof( long ) + idx * sizeof( long long ), SEEK_SET );
	long long align = 0;
	fread( &align, sizeof( align ), 1, fp );
	fileseek( fp, align, SEEK_SET );
	kome::core::FileAccessor acc( fp );

	dps.importData( boost::bind( &kome::core::DataAccessor::read, &acc, _1, _2 ) );

	fclose( fp );

	// set
	for( unsigned int i = 0; i < dps.getLength(); i++ ) {
		xyData.addPoint( dps.getX( i ), dps.getY( i ) );
	}
}


// set Substance Property
void IdentificationManager::setSubstanceProperty( SubstanceProperty substProp ){
	m_substanceProperty = substProp;
}

// get Substance Property
void IdentificationManager::getSubstanceProperty( SubstanceProperty& substProp ){
	substProp = m_substanceProperty;
}

// set prepare db thread condition
void IdentificationManager::setPrepareDBThreadCondition( const bool bSet ) {
	m_bPrepareDBThread = bSet;
}

// get prepare db thread condition
bool IdentificationManager::getPrepareDBThreadCondition() {
	return m_bPrepareDBThread;
}

// copy file
bool IdentificationManager::copyFileIfNew( const char* srcFile, const char* dstFile ) {
	// return value
	bool ret = false;

	// check parameters
	if( srcFile == NULL || dstFile == NULL ) {
		return ret;
	}

	// newer check
	bool newer = true;
	if( fileexists( srcFile ) && fileexists( dstFile ) ) {
		boost::filesystem::path srcP( srcFile );
		boost::filesystem::path dstP( dstFile );

		std::time_t srcT = boost::filesystem::last_write_time( srcP );
		std::time_t dstT = boost::filesystem::last_write_time( dstP );

		if( dstT > srcT ) {
			newer = false;
			ret = true;
		}
	}

	// copy
	if( newer ) {
		ret = copyfile( srcFile, dstFile );
	}

	return ret;
}

// get identification manager object (This is the only object)
IdentificationManager& IdentificationManager::getInstance( void )
{
	// create object (This is the only object.)
	static IdentificationManager mgr;

	return mgr;
}

/**
 * @file GlycanAnalysisPluginFunctions.h
 * @brief interfaces of GlycanAnalysis plug-in function
 *
 * @author N.Tashiro
 * @date 2012.04.02
 * 
 * Copyright (C) 2014 Shimadzu Corporation All rights reserved.
 */

#ifndef __KOME_GLYCAN_ANALYSIS_PLUGIN_FUNCTIONS_H__
#define __KOME_GLYCAN_ANALYSIS_PLUGIN_FUNCTIONS_H__

#ifdef __unix
	extern "C" {
#endif  // __unix

/**
 * @fn kome::objects::Variant drawGlycanAnalysisSpectrum( kome::objects::Parameters* params )
 * @brief initialize glycan analysis
 * @param params parameters information
 * @return true only
 */
kome::objects::Variant initGlycanAnalysis( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant drawGlycanAnalysisSpectrum( kome::objects::Parameters* params )
 * @brief check for can be analyzed
 * @param params parameters information
 * @return if false, Glycan Analysis is Disable
 */
kome::objects::Variant isEnableGlycanAnalysis( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant openGlycanAnalysis( kome::objects::Parameters* params )
 * @brief open glycan analysis window
 * @param params parameters information
 * @return if completed, return true
 */
kome::objects::Variant openGlycanAnalysis( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant closeGlycanAnalysis( kome::objects::Parameters* params )
 * @brief close glycan analysis and Glycan Structure window
 * @param params parameters information
 * @return if completed, return true
 */
kome::objects::Variant closeGlycanAnalysis( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant drawGlycanAnalysisSpectrum( kome::objects::Parameters* params )
 * @brief draw annotations on spectrum
 * @param params parameters information
 * @return false only
 */
kome::objects::Variant drawGlycanAnalysisSpectrum( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant drawGlycanAnalysisSpectrum( kome::objects::Parameters* params )
 * @brief open glycan structure dialog
 * @param params parameters information
 * @return if completed, return true.
 */
kome::objects::Variant openGlycanStructureDialog( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant drawGlycanAnalysisSpectrum( kome::objects::Parameters* params )
 * @brief exit program
 * @param params parameters information
 * @return true only
 */
kome::objects::Variant exitGlycanAnalysis( kome::objects::Parameters* params );

#ifdef __unix
	}	
#endif  // __unix

#endif	// __KOME_GLYCAN_ANALYSIS_PLUGIN_FUNCTIONS_H__

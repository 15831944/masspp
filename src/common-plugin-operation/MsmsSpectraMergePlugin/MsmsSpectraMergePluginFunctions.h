/**
 * @file MsmsSpectraMergePluginFunctions.h
 * @brief interfaces of plug-in functions
 *
 * @author tanaka-s
 * @date 2013.01.16
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation All rights reserved.
 */



#ifndef __KOME_MSMS_SPECTRA_MERGE_PLUGIN_FUNCTIONS_H__
#define __KOME_MSMS_SPECTRA_MERGE_PLUGIN_FUNCTIONS_H__


/**
 * @fn kome::objects::Variant mergeMs2Spectra( kome::objects::Parameters* params )
 * @brief merges MS/MS spectra
 * @param[in] params parameters
 * @return If true, merging MS/MS spectra is succeeded. (boolean value)
 */
kome::objects::Variant mergeMs2Spectra( kome::objects::Parameters* params );


#endif    // __KOME_MSMS_SPECTRA_MERGE_PLUGIN_FUNCTIONS_H__

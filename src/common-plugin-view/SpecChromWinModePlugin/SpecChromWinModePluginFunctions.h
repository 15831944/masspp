/**
 * @file SpecChromWinModePluginFunctions.h
 * @brief interfaces of plug-in functions
 *
 * @author tanaka-s
 * @date 2013.05.21
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation All rights reserved.
 */



#ifndef __KOME_SPEC_CHROM_WIN_MODE_PLUGIN_FUNCTIONS_H__
#define __KOME_SPEC_CHROM_WIN_MODE_PLUGIN_FUNCTIONS_H__


/**
 * @fn kome::objects::Variant onClientSize( kome::objects::Parameters* params )
 * @brief set client size
 * @param[in] params paameters object
 * @return This value has no meanings.
 */
kome::objects::Variant onClientSize( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant setSpecChromWindowStatus( kome::objects::Parameters* )
 * @brief set the status
 * @return This function always returns true. (boolean)
 */
kome::objects::Variant setSpecChromWindowStatus( kome::objects::Parameters* );

/**
 * @fn kome::objects::Variant openChromatogramFrame( kome::objects::Parameters* params )
 * @brief opens chromatogram frame
 * @param[in] params parameters
 * @return This value has no meanings.
 */
kome::objects::Variant openChromatogramFrame( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant openSpectrumFrame( kome::objects::Parameters* params )
 * @brief opens spectrum frame
 * @param[in] params parameters
 * @return This value has no meanings.
 */
kome::objects::Variant openSpectrumFrame( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant openDataMapFrame( kome::objects::Parameters* params )
 * @brief opens data map frame
 * @param[in] params parameters
 * @return This value has no meanings.
 */
kome::objects::Variant openDataMapFrame( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant onChromKeyDown( kome::objects::Parameters* params )
 * @brief on chromatogram key down
 * @param[in] params parameters
 * @return return value
 */
kome::objects::Variant onChromKeyDown( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant closeAllChildFrames( kome::objects::Parameters* )
 * @brief close all child frames
 * @return This value has no meanings.
 */
kome::objects::Variant closeAllChildFrames( kome::objects::Parameters* );


#endif    // __KOME_SPEC_CHROM_WIN_MODE_PLUGIN_FUNCTIONS_H__

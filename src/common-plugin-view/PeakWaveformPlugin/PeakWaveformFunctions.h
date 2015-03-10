/**
 * @file PeakWaveformFunctions.h
 * @brief interfaces of RawWaveform plug-in functions
 *
 * @author M.Izumi
 * @data 2013.02.25
 *
 * Copyright(C) 2013 Shimadzu Corporation. All rights reserved.
 */

#ifndef __KOME_VIEW_PEAK_WAVEFORM_FUNCTIONS_H__
#define __KOME_VIEW_PEAK_WAVEFORM_FUNCTIONS_H__

#ifdef __unix
	extern "C" {
#endif  // __unix

/**
 * @fn kome::objects::Variant openDispPeaks( kome::objects::Parameters* params )
 * @brif open disp peaks
 * @param params parameters object
 * @return If true, It succeeded to toggle the the visibility of the overlapping dialog (boolean)
 */
kome::objects::Variant openDispPeaks( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant openSpectPeaksFromContextMenu( kome::objects::Parameters* params )
 * @brif open spectrum disp peaks
 * @param params parameters object
 * @return If true, It succeeded to toggle the the visibility of the overlapping dialog (boolean)
 */
kome::objects::Variant openSpectPeaksFromContextMenu( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant openChromPeaksFromContextMenu( kome::objects::Parameters* params )
 * @brif open chromatogram disp peaks
 * @param params parameters object
 * @return If true, It succeeded to toggle the the visibility of the overlapping dialog (boolean)
 */
kome::objects::Variant openChromPeaksFromContextMenu( kome::objects::Parameters* params );

#endif	// __KOME_VIEW_PEAK_WAVEFORM_FUNCTIONS_H__

/**
 * @file MergedSpectrumPeaksFunctions.h
 * @brief interfaces of MergedSpectrumPeaks plug-in functions
 *
 * @author S.Tanaka
 * @date 2009.07.30
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_POSITION_MERGED_SPECTRUM_PEAKS_FUNCTIONS_H__
#define __KOME_POSITION_MERGED_SPECTRUM_PEAKS_FUNCTIONS_H__


#ifdef __unix
	extern "C" {
#endif  // __unix


/**
 * @fn kome::objects::Variant detectPeakPosition( kome::objects::Parameters* params )
 * @brief detects peak position
 * @param params parameters information
 * @return return value (This value ha no meanings.)
 */
kome::objects::Variant detectPeakPosition( kome::objects::Parameters* params );

#ifdef __unix
	}
#endif  // __unix


#endif // __KOME_POSITION_MERGED_SPECTRUM_PEAKS_FUNCTIONS_H__

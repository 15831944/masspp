/**
 * @file NeighborPeaksFunctions.h
 * @brief interfaces of NeighborPeaks plug-in functions
 *
 * @author S.Tanaka
 * @date 2008.10.10
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_SEARCH_NEIGHBOR_PEAKS_FUNCTION_H__
#define __KOME_SEARCH_NEIGHBOR_PEAKS_FUNCTION_H__


#ifdef __unix
	extern "C" {
#endif  // __unix


/**
 * @fn kome::objects::Variant searchSpectrumPeak( kome::objects::Parameters* params )
 * @brief searchs spectrum peak
 * @param params parameters object
 * @return return value (This value has no meanings.)
 *
 kome::objects::Variant searchSpectrumPeak( kome::objects::Parameters* params );

/**
* @fn kome::objects::Variant searchChromatogramPeak( kome::objects::Parameters* params )
* @brief searches chromatogram peak
* @param params parameters object
* @return return value (This value has no meanings.)
*/
kome::objects::Variant searchChromatogramPeak( kome::objects::Parameters* params );

#ifdef __unix
	}
#endif  // __unix



#endif// __KOME_SEARCH_NEIGHBOR_PEAKS_FUNCTION_H__

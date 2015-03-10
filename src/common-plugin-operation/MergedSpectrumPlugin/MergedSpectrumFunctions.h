/**
 * @file MergedSpectrumFunctions.h
 * @brief interfaces of merged spectrum plug-in functions
 *
 * @author S.Tanaka
 * @date 2007.03.06
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_OPERATION_MERGED_SPECTRUM_FUNCTIONS_H__
#define __KOME_OPERATION_MERGED_SPECTRUM_FUNCTIONS_H__


#ifdef __unix
	extern "C" {
#endif  // __unix

/**
 * @fn kome::objects::Variant mergeSpectra( kome::objects::Parameters* )
 * @brief merges spectra
 * @return return value (If it succeeded to merge spectra, this function returns true.)
 */
kome::objects::Variant mergeSpectra( kome::objects::Parameters* );

/**
 * @fn kome::objects::Variant showAveragedSpectrum( kome::objects::Parameters* params )
 * @brief shows Averaged Spectrum Dialog
 * @param params parameters information (This object has chromatogram and RT range informations.)
 * @return This value has no meanings.
 */
kome::objects::Variant showAveragedSpectrum( kome::objects::Parameters* params );


/**
 * @fn kome::objects::Variant getAverageSpec( kome::objects::Parameters* params )
 * @brief get average spectrum
 * @param params
 * @return
 */
kome::objects::Variant getAverageSpec( kome::objects::Parameters* params );

#ifdef __unix
	}
#endif  // __unix



#endif		// __KOME_OPERATION_MERGED_SPECTRUM_FUNCTIONS_H__

/**
 * @file SpectraSearchFunctions.h
 * @brief interface of Spectra Search plug-in functions
 *
 * @author S.Tanaka
 * @date 2007.01.15
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_SEARCH_SPECTRA_SEARCH_FUNCTIONS_H__
#define __KOME_SEARCH_SPECTRA_SEARCH_FUNCTIONS_H__


#ifdef __unix
	extern "C" {
#endif  // __unix


/**
 * @fn kome::objects::Variant openSpectraSearchDialog( kome::objects::Parameters* params )
 * @brief searches spectra
 * @param params parameters object
 * @return This value is always true. (boolean value)
 */
kome::objects::Variant openSpectraSearchDialog( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant setSpectrumRtRange( kome::objects::Parameters* params )
 * @brief sets retention time range
 * @param params parameters information object
 * @return return value. (boolean)
 */
kome::objects::Variant setSpectrumRtRange( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant setSpectrumPrecursorRange( kome::objects::Parameters* params )
 * @brief sets precursor range
 * @param params parameters information object
 * @return return value. (boolean)
 */
kome::objects::Variant setSpectrumPrecursorRange( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant setSpectrumScanNumRange( kome::objects::Parameters* params )
 * @brief sets scan number range
 * @param params parameters information object
 * @return return value. (boolean)
 */
kome::objects::Variant setSpectrumScanNumRange( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant setSpectrumMsStageRange( kome::objects::Parameters* params )
 * @brief sets ms stage range
 * @param params parameters information object
 * @return return value. (boolean)
 */
kome::objects::Variant setSpectrumMsStageRange( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant setSpectrumPropFilter( kome::objects::Parameters* params )
 * @brief sets property filter
 * @param params parameters information object
 * @return return value. (boolean)
 */
kome::objects::Variant setSpectrumPropFilter( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant getFilterSpectraOperation( kome::objects::Parameters* )
 * @brief gets filter spectra  operation
 * @return return value FilterSpectraOperation object
 */
kome::objects::Variant getFilterSpectraOperation( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant filterSpectraFunc( kome::objects::Parameters* params )
 * @brief searches spectra
 * @param params parameters object
 * @return return value.
 */
kome::objects::Variant filterSpectraFunc( kome::objects::Parameters* params );

#ifdef __unix
	}
#endif  // __unix



#endif		//  __KOME_SEARCH_SPECTRA_SEARCH_FUNCTIONS_H__

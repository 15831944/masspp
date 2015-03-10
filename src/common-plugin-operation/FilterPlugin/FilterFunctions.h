/**
 * @file FilterFunctions.h
 * @brief interfaces of Filter plug-in functions
 *
 * @author S.Tanaka
 * @date 2007.08.16
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#ifndef __KOME_FILTER_FUNCTIONS_H__
#define __KOME_FILTER_FUNCTIONS_H__


#ifdef __unix
	extern "C" {
#endif  // __unix


/**
 * @fn kome::objects::Variant openFilterDialog( kome::objects::Parameters* params )
 * @brief opens filter subtract dialog
 * @return This function always returns true. (boolean)
 */
kome::objects::Variant openFilterDialog( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant getSpecFilterOperation( kome::objects::Parameters* )
 * @brief gets filter operation
 * @return return value FilterOperation object
 */
kome::objects::Variant getSpecFilterOperation( kome::objects::Parameters* );


/**
 * @fn kome::objects::Variant getSpecFilterOperation( kome::objects::Parameters* )
 * @brief gets filter operation
 * @return return value FilterOperation object
 */
kome::objects::Variant getChromFilterOperation( kome::objects::Parameters* );

/**
 * @fn kome::objects::Variant filterWaveformOnBatch( kome::objects::Parameters* params )
 * @brief applying waveform filter
 * @param[in] params parameters
 * @return If true, applying waveform filter is completed successfully. (boolean value)
 */
kome::objects::Variant filterWaveformOnBatch( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant getSpecXYDataOperation( kome::objects::Parameters* params )
 * @brief get spec xyData operation
 * @return xydata operation
 */
kome::objects::Variant getSpecXYDataOperation( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant getChromXYDataOperation( kome::objects::Parameters* params )
 * @brief get chrom xyData operation
 * @return xydata operation
 */
kome::objects::Variant getChromXYDataOperation( kome::objects::Parameters* params );

#ifdef __unix
	}
#endif  // __unix


#endif		// __KOME_FILTER_FUNCTIONS_H__

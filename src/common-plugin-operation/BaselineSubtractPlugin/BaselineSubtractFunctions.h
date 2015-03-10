/**
 * @file BaselineSubtractFunctions.h
 * @brief interfaces of BaselineSubtract plug-in functions
 *
 * @author S.Tanaka
 * @date 2007.03.12
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#ifndef __KOME_BASELINE_SUBTRACT_FUNCTIONS_H__
#define __KOME_BASELINE_SUBTRACT_FUNCTIONS_H__


#ifdef __unix
	extern "C" {
#endif  // __unix


/**
 * @fn kome::objects::Variant openBaselineDialog( kome::objects::Parameters* params )
 * @brief opens baseline subtract dialog
 * @param params parameters object
 * @return This function always returns true. (boolean)
 */
kome::objects::Variant openBaselineDialog( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant drawChromBaseline( kome::objects::Parameters* params )
 * @brief draws chromatogram baseline
 * @param params parameters object (This object has chromatogram and graphics information.)
 * @return retun value (This value has no meanings.)
 */
kome::objects::Variant drawChromBaseline( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant drawSpecBaseline( kome::objects::Parameters* params )
 * @brief draws spectrum baseline
 * @param params parameters object (This object has spectrum and graphics information.)
 * @return return value (This value has no meanings.)
 */
kome::objects::Variant drawSpecBaseline( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant getSpecBaselineOperation( kome::objects::Parameters* )
 * @brief gets baseline subtract operation
 * @return return value baseline operation object
 */
kome::objects::Variant getSpecBaselineOperation( kome::objects::Parameters* );

/**
 * @fn kome::objects::Variant getChromBaselineOperation( kome::objects::Parameters* )
 * @brief gets baseline subtract operation
 * @return return value baseline operation object
 */
kome::objects::Variant getChromBaselineOperation( kome::objects::Parameters* );

/**
 * @fn kome::objects::Variant execBaselineSubtract( kome::objects::Parameters* params )
 * @brief add matrix sample
 * @param[in] params parameters object
 * @return If true, This function is succeeded. (boolean)
*/
kome::objects::Variant execBaselineSubtract( kome::objects::Parameters* params );

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



#endif		// __KOME_BASELINE_SUBTRACT_FUNCTIONS_H__

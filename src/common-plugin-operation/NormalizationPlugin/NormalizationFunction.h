/**
 * @file NormalizationFunction.h
 * @brief interfaces of NormalizationManager plug-in functions
 *
 * @author M.Izumi
 * @date 2012.07.02
 * 
 * Copyright(C) 2012 Shimadzu Corporation. All rights reserved.
 */

#ifndef __KOME_NORMALIZATION_FUNCTIONS_H__
#define __KOME_NORMALIZATION_FUNCTIONS_H__


#ifdef __unix
	extern "C" {
#endif  // __unix

/**
 * @fn kome::objects::Variant openNormalizationDialog( kome::objects::Parameters* )
 * @brief opens normalization dialog
 * @return This function always returns true. (boolean)
 */
kome::objects::Variant openNormalizationDialog( kome::objects::Parameters* );

/**
 * @fn kome::objects::Variant NormalizFilterThreshold( kome::objects::Parameters* params )
 * @brief Normalization Filter( Heigth Threshold )
 * @return This function always returns true. (boolean)
 */
kome::objects::Variant NormalizFilterThreshold( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant NormalizFilterRtRange( kome::objects::Parameters* params )
 * @brief Normalization Filter( RT Range )
 * @return This function always returns true. (boolean)
 */
kome::objects::Variant NormalizFilterRtRange( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant NormalizFilterMzRange( kome::objects::Parameters* params )
 * @brief  Normalization Filter( m/z Range )
 * @return  his function always returns true. (boolean)
 */
kome::objects::Variant NormalizFilterMzRange( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant NormalizMethodInteMultiSample( kome::objects::Parameters* params )
 * @brief Normalization Method( Internal Standard(Multi Sample) )
 * @return return value (This value has no meanings.)
 */
kome::objects::Variant NormalizMethodInteMultiSample( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant NormalizMethodInteEachFract( kome::objects::Parameters* params )
 * @brief Normalization Method( Internal Standards in all Fractions(Each Fraction) )
 * @return return value (This value has no meanings.)
 */
kome::objects::Variant NormalizMethodInteEachFract( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant NormalizMethodIntegratEachFrect( kome::objects::Parameters* params )
 * @brief Normalization Method( Integration(Each Fraction) )
 * @return return value (This value has no meanings.)
 */
kome::objects::Variant NormalizMethodIntegratEachFrect( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant NormalizMethodIntegratMultiSample( kome::objects::Parameters* params )
 * @brief Normalization Method( Integration(Multi Sample) )
 * @return return value (This value has no meanings.)
 */
kome::objects::Variant NormalizMethodIntegratMultiSample( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant NormalizMethodDistributMaxIntentEachFract( kome::objects::Parameters* params )
 * @brief Normalization Method( Distribution(Max Intensity)(Each Fraction) )
 * @return return value (This value has no meanings.)
 */
kome::objects::Variant NormalizMethodDistributMaxIntentEachFract( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant NormalizMethodDistributMaxIntentMultSample( kome::objects::Parameters* params )
 * @brief Normalization Method( Distribution(Max Intensity)(Multi Sample) )
 * @return return value (This value has no meanings.)
 */
kome::objects::Variant NormalizMethodDistributMaxIntentMultSample( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant NormalizMethodDistributMedianEachFract( kome::objects::Parameters* params )
 * @brief Normalization Method( Distribution(Median)(Each Fraction) )
 * @return return value (This value has no meanings.)
 */
kome::objects::Variant NormalizMethodDistributMedianEachFract( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant NormalizMethodDistributMedianMultiSample( kome::objects::Parameters* params )
 * @brief Normalization Method( Distribution(Median)(Multi Sample) )
 * @return return value (This value has no meanings.)
 */
kome::objects::Variant NormalizMethodDistributMedianMultiSample( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant NormalizMethodDistributPrecentileEachFract( kome::objects::Parameters* params )
 * @brief Normalization Method( Distribution(Percentile)(Each Fraction) )
 * @return return value (This value has no meanings.)
 */
kome::objects::Variant NormalizMethodDistributPrecentileEachFract( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant NormalizMethodDistributPrecentileMultiSample( kome::objects::Parameters* params )
 * @brief Normalization Method( Distribution(Percentile)(Multi Sample) )
 * @return return value (This value has no meanings.)
 */
kome::objects::Variant NormalizMethodDistributPrecentileMultiSample( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant setNormStandard( kome::objects::Parameters* params )
 * @brief Set Normalization Standard Sample
 * @param[in] params parameters object
 * @return If true, This function is succeeded. (boolean)
*/
kome::objects::Variant setNormStandard( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant setNormFilterInt( kome::objects::Parameters* params )
 * @brief Set Normalization Intensity Filter
 * @param[in] params parameters object
 * @return If true, This function is succeeded. (boolean)
*/
kome::objects::Variant setNormFilterInt( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant setNormFilterRt( kome::objects::Parameters* params )
 * @brief Set Normalization RT Filter
 * @param[in] params parameters object
 * @return If true, This function is succeeded. (boolean)
*/
kome::objects::Variant setNormFilterRt( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant setNormFilterMz( kome::objects::Parameters* params )
 * @brief Set Normalization m/z Filter
 * @param[in] params parameters object
 * @return If true, This function is succeeded. (boolean)
*/
kome::objects::Variant setNormFilterMz( kome::objects::Parameters* params );
/**
 * @fn kome::objects::Variant execNormSpec( kome::objects::Parameters* params )
 * @brief Normalize samples using spectrum peaks
 * @param[in] params parameters object
 * @return If true, This function is succeeded. (boolean)
*/
kome::objects::Variant execNormSpec( kome::objects::Parameters* params );
/**
 * @fn kome::objects::Variant execNorm2d( kome::objects::Parameters* params )
 * @brief Normalize samples using 2D peaks
 * @param[in] params parameters object
 * @return If true, This function is succeeded. (boolean)
*/
kome::objects::Variant execNorm2d( kome::objects::Parameters* params );


/**
 * @fn kome::objects::Variant execNormalizationCommand( const char* peak, kome::objects::SettingParameterValues* settings )
 * @brief exec Normalization from Command Line
 * @param[in] peak peak
 * @param[in] settings settings
 * @param[in] methods methods
 * @return If true, This function is succeeded. (boolean)
*/
kome::objects::Variant execNormalizationCommand( const char* peak, kome::objects::SettingParameterValues* settings, const char* methods );


#ifdef __unix
	}
#endif  // __unix



#endif		// __KOME_NORMALIZATION_FUNCTIONS_H__

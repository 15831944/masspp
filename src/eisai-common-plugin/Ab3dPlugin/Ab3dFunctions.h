/**
 * @file Ab3dAlignmentChromFunctions.h
 * @brief interfaces of Filter plug-in functions
 *
 * @author S.Tanaka
 * @date 2007.08.20
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_AB3D_FUNCTIONS_H__
#define __KOME_AB3D_FUNCTIONS_H__


#ifdef __unix
	extern "C" {
#endif  // __unix

/**
 * @fn kome::objects::Variant executeAlignmentByTIC( kome::objects::Parameters* params )
 * @brief executes alignment by TIC fitting
 * @param params parameters object
 * @return If true, it succeeded to execute alignment (boolean)
 */
kome::objects::Variant executeAlignmentByTIC( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant executeAlignmentByMC( kome::objects::Parameters* params )
 * @brief executes alignment by XIC fitting
 * @param params parameters object
 * @return If true, it succeeded to execute alignment (boolean)
 */
kome::objects::Variant executeAlignmentByMC( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant executeAlignmentBySpec( kome::objects::Parameters* params )
 * @brief executes alignment by spectrum fitting
 * @param params parameters object
 * @return If true, it succeeded to execute alignment (boolean)
 */
kome::objects::Variant executeAlignmentBySpec( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant executeComposition( kome::objects::Parameters* params )
 * @brief executes composition
 * @param params parameters object
 * @return composition data object. (If NULL, it failed to get composition data.)
 */
kome::objects::Variant executeComposition( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant detect2dPeaks( kome::objects::Parameters* params )
 * @brief detects 2D peaks
 * @param params parameters object
 * @return return value (This value has no meanings.)
 */
kome::objects::Variant detect2dPeaks( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant detect2dClusters( kome::objects::Parameters* params )
 * @brief detects 2D clusters
 * @param params parameters object
 * @return return value (This value has no meanings.)
 */
kome::objects::Variant detect2dClusters( kome::objects::Parameters* params );



#ifdef __unix
	}
#endif  // __unix



#endif		// __KOME_AB3D_FUNCTIONS_H__

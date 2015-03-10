/**
 * @file ProfileDrawerFunctions.h
 * @brief interfaces of ProfileDrawer plug-in functions
 *
 * @author S.Tanaka
 * @date 2006.09.29
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#ifndef __KOME_VIEW_PROFILE_DRAWER_FUNCTIONS_H__
#define __KOME_VIEW_PROFILE_DRAWER_FUNCTIONS_H__


#ifdef __unix
	extern "C" {
#endif  // __unix


/**
 * @fn kome::objects::Variant drawSpectrumProfile( kome::objects::Parameters* params )
 * @brief draw spectrum profile
 * @param params parameter information object. (This object has spectrum, graphics and data points information.)
 * @return return value. (This value doesn't have the meanings.)
 */
kome::objects::Variant drawSpectrumProfile( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant drawSpectrumImpulse( kome::objects::Parameters* params )
 * @brief draw spectrum profile
 * @param params parameter information object. (This object has spectrum, graphics and data points information.)
 * @return return value. (This value doesn't have the meanings.)
 */
kome::objects::Variant drawSpectrumImpulse( kome::objects::Parameters* params );	// @date 2011.02.28 <Add> M.Izumi 

/**
 * @fn kome::objects::Variant updateSpectrum( kome::objects::Parameters* params )
 * @brief close spectrum
 * @param params parameters information. (This object has spectrum information)
 * @return return value (This value has no meanings.)
 */
kome::objects::Variant updateSpectrum( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant prepareDrawChromatogram( kome::objects::Parameters* params )
 * @brief prepare to draw spectrum profile
 * @param params parameter information object. (This object has spectrum, graphics and data points information.)
 * @return return value. (This value doesn't have the meanings.)
 */
kome::objects::Variant prepareDrawChromatogram( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant drawChromatogramProfile( kome::objects::Parameters* params )
 * @brief draw chromatogram profile
 * @param params parameter information object. (This object has chromatogram, graphics and data points information.)
 * @return return value. (This value doesn't have the meanings.)
 */
kome::objects::Variant drawChromatogramProfile( kome::objects::Parameters* params );


/**
 * @fn kome::objects::Variant drawChromatogramImpulse( kome::objects::Parameters* params )
 * @brief draw chromatogram profile
 * @param params parameter information object. (This object has chromatogram, graphics and data points information.)
 * @return return value. (This value doesn't have the meanings.)
 */
kome::objects::Variant drawChromatogramImpulse( kome::objects::Parameters* params );	// @date 2011.02.28 <Add> M.Izumi 

/**
 * @fn kome::objects::Variant updateChromatogram( kome::objects::Parameters* params )
 * @brief close chromatogram
 * @param params parameters information. (This object has chromatogram information)
 * @return return value (This value has no meanings.)
 */
kome::objects::Variant updateChromatogram( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant setChromatogramDrawingGraphStatusProfile( kome::objects::Parameters* )
 * @brief sets chromatogram graph status
 * @return This function returns true. (boolean value)
 */
kome::objects::Variant setChromatogramDrawingGraphStatusProfile( kome::objects::Parameters* );

/**
 * @fn kome::objects::Variant setChromatogramDrawingGraphStatusImpulse( kome::objects::Parameters* )
 * @brief sets chromatogram graph status
 * @return This function returns true. (boolean value)
 */
kome::objects::Variant setChromatogramDrawingGraphStatusImpulse( kome::objects::Parameters* );

/**
 * @fn kome::objects::Variant setSpectrumDrawingGraphStatusProfile( kome::objects::Parameters* )
 * @brief sets spectrum graph status
 * @return This function returns true. (boolean value)
 */
kome::objects::Variant setSpectrumDrawingGraphStatusProfile( kome::objects::Parameters* );

/**
 * @fn kome::objects::Variant setSpectrumDrawingGraphStatusImpulse( kome::objects::Parameters* )
 * @brief sets spectrum graph status
 * @return This function returns true. (boolean value)
 */
kome::objects::Variant setSpectrumDrawingGraphStatusImpulse( kome::objects::Parameters* );	 // @date 2011.02.28 <Add> M.Izumi 

#ifdef __unix
	}
#endif  // __unix



#endif//  __KOME_VIEW_PROFILE_DRAWER_FUNCTIONS_H__

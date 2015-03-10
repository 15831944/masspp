/**
 * @file ScaleDrawerFunctions.h
 * @brief interfaces of ScaleDrawer plug-in functions
 *
 * @author S.Tanaka
 * @date 2007.03.01
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#ifndef __KOME_VIEW_SCALE_DRAWER_FUNCTIONS_H__
#define __KOME_VIEW_SCALE_DRAWER_FUNCTIONS_H__


#ifdef __unix
	extern "C" {
#endif  // __unix


/**
 * @fn kome::objects::Variant prepareSpectrumScale( kome::objects::Parameters* params )
 * @brief prepare to draw spectrum scale
 * @param params parameter information object. (This object has spectrum, graphics and data points information.)
 * @return return value. (This value doesn't have the meanings.)
 */
kome::objects::Variant prepareSpectrumScale( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant drawSpectrumScale( kome::objects::Parameters* params )
 * @brief draw foreground of spectrum
 * @param params parameter information object. (This object has spectrum, graphics and data points information.)
 * @return return value. (This value doesn't have the meanings.)
 */
kome::objects::Variant drawSpectrumScale( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant prepareChromatogramScale( kome::objects::Parameters* params )
 * @brief prepare to draw chromatogram scale
 * @param params parameter information object. (This object has spectrum, graphics and data points information.)
 * @return return value. (This value doesn't have the meanings.)
 */
kome::objects::Variant prepareChromatogramScale( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant drawChromatogramScale( kome::objects::Parameters* params )
 * @brief draw foreground of chromatogram
 * @param params parameter information object. (This object has chromatogram, graphics and data points information.)
 * @return return value. (This value doesn't have the meanings.)
 */
kome::objects::Variant drawChromatogramScale( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant prepareDataMapScale( kome::objects::Parameters* params )
 * @brief prepare to draw data map scale
 * @param params parameter information object. (This object has spectrum, graphics and data points information.)
 * @return return value. (This value doesn't have the meanings.)
 */
kome::objects::Variant prepareDataMapScale( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant drawDataMapScale( kome::objects::Parameters* params )
 * @brief draw foreground of data map
 * @param params parameter information object. (This object has chromatogram, graphics and data points information.)
 * @return return value. (This value doesn't have the meanings.)
 */
kome::objects::Variant drawDataMapScale( kome::objects::Parameters* params );


/**
 * @fn kome::objects::Variant setChromatogramDrawingScaleStatus( kome::objects::Parameters* )
 * @brief sets chromatogram drawing scale status
 * @return This function always returs true. (boolean)
 */
kome::objects::Variant setChromatogramDrawingScaleStatus( kome::objects::Parameters* );

/**
 * @fn kome::objects::Variant setSpectrumDrawingScaleStatus( kome::objects::Parameters* )
 * @brief sets spectrum drawing scale status
 * @return This function always returs true. (boolean)
 */
kome::objects::Variant setSpectrumDrawingScaleStatus( kome::objects::Parameters* );

/**
 * @fn kome::objects::Variant setDatamapDrawingScaleStatus( kome::objects::Parameters* )
 * @brief sets datamap drawing scale status
 * @return This function always returs true. (boolean)
 */
kome::objects::Variant setDatamapDrawingScaleStatus( kome::objects::Parameters* );

#ifdef __unix
	}
#endif  // __unix



#endif//  __KOME_VIEW_SCALE_DRAWER_FUNCTIONS_H__

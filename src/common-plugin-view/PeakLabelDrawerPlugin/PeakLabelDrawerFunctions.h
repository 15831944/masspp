/**
 * @file PeakLabelDrawerFunctions.h
 * @brief interfaces of PeakLabelDrawer plug-in functions
 *
 * @author S.Tanaka
 * @date 2007.03.01
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_VIEW_PEAK_LABEL_DRAWER_FUNCTIONS_H__
#define __KOME_VIEW_PEAK_LABEL_DRAWER_FUNCTIONS_H__


#ifdef __unix
	extern "C" {
#endif  // __unix


/**
 * @fn kome::objects::Variant drawSpecPeaksLabel( kome::objects::Parameters* params )
 * @brief draws peak label
 * @param params parameters information object
 * @return return value (This value has no meanings.)
 */
kome::objects::Variant drawSpecPeaksLabel( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant setSpecGraphMargin( kome::objects::Parameters* params )
 * @brief get space to draw label
 * @param params parameters information object
 * @return return value (This value has no meanings.)
 */
kome::objects::Variant setSpecGraphMargin( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant deleteSpectrumPeakInfo( kome::objects::Parameters* params )
 * @brief deletes spectrum peak information
 * @param params parameters object
 * @return return value (This value has no meanings.)
 */
kome::objects::Variant deleteSpectrumPeakInfo( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant drawChromPeaksLabel( kome::objects::Parameters* params )
 * @brief draws peak label
 * @param params parameters information object
 * @return return value (This value has no meanings.)
 */
kome::objects::Variant drawChromPeaksLabel( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant setChromGraphMargin( kome::objects::Parameters* params )
 * @brief get space to draw label
 * @param params parameters information object
 * @return return value (This value has no meanings.)
 */
kome::objects::Variant setChromGraphMargin( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant deleteChromatogramPeakInfo( kome::objects::Parameters* params )
 * @brief deletes chromatogram peak information
 * @param  params parameters object
 * @return return value (This value has no meanings.)
 */
kome::objects::Variant deleteChromatogramPeakInfo( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant setChromatogramDrawingPeaksStatus( kome::objects::Parameters* )
 * @brief sets chromatogram drawing peaks status
 * @return This function always returs true. (boolean)
 */
kome::objects::Variant setChromatogramDrawingPeaksStatus( kome::objects::Parameters* );

/**
 * @fn kome::objects::Variant setSpectrumDrawingPeaksStatus( kome::objects::Parameters* )
 * @brief sets spectrum drawing peaks status
 * @return This function always returs true. (boolean)
 */
kome::objects::Variant setSpectrumDrawingPeaksStatus( kome::objects::Parameters* );

/**
 * @fn kome::objects::Variant updateDrawingLabelSeetings( kome::objects::Parameters* )
 * @brief updates drawing label settings
 * @return This function has no meanings.
 */
kome::objects::Variant updateDrawingLabelSeetings( kome::objects::Parameters* );

// @date 2011.03.29 <Add> M.Izumi ->
/**
 * @brief This function is called when mouse button is down on a spectrum window.
 * @param params parameters object. (This has mouse event and graphics object.)
 * @return refresh flag (boolean value)
 */
kome::objects::Variant onSpecButtonDown( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant onChromButtonDown( kome::objects::Parameters* params )
 * @brief This function is called when mouse button is down on a chromatogram window.
 * @param params parameters object. (This has mouse event and graphics object.)
 * @return refresh flag (boolean value)
 */
kome::objects::Variant onChromButtonDown( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant onSpecMouseMove( kome::objects::Parameters* params )
 * @brief This function is called when mouse cursor is moved on a spectrum window.
 * @param params parameters object. (This has mouse event and graphics object.)
 * @return refresh flag (boolean value)
 */
kome::objects::Variant onSpecMouseMobe( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant onChromMouseMove( kome::objects::Parameters* params )
 * @brief This function is called when mouse cursor is moved on a spectrum window.
 * @param params parameters object. (This has mouse event and graphics object.)
 * @return refresh flag (boolean value)
 */
kome::objects::Variant onChromMouseMobe( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant onSpecButtonUp( kome::objects::Parameters* params )
 * @brief This function is called when mouse button is up on a spectrum window.
 * @param params parameters object. (This has mouse event and graphics object.)
 * @return refresh flag (boolean value)
 */
kome::objects::Variant onSpecButtonUp( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant onSpecButtonUp( kome::objects::Parameters* params )
 * @brief This function is called when mouse button is up on a spectrum window.
 * @param params parameters object. (This has mouse event and graphics object.)
 * @return refresh flag (boolean value)
 */
kome::objects::Variant onChromButtonUp( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant onDrawSpecFg( kome::objects::Parameters* params )
 * @brief draws spectrum foreground
 * @param params parameters object. (This has spectrum, data points and graphics object.)
 * @return return value. (This value has no meanings.)
 */
kome::objects::Variant onDrawSpecFg( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant onDrawChromFg( kome::objects::Parameters* params )
 * @brief draws chromatogram foreground
 * @param params parameters object. (This has chromatogram, data points and graphics object.)
 * @return return value. (This value has no meanings.)
 */
kome::objects::Variant onDrawChromFg( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant onSpecDblClick( kome::objects::Parameters* params )
 * @brief This function is called when mouse is double clicked on a spectrum window.
 * @param params parameters object. (This has mouse event and graphics object.)
 * @return refresh flag (boolean value)
 */
kome::objects::Variant onSpecDblClick( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant onChromDblClick( kome::objects::Parameters* params )
 * @brief This function is called when mouse button is double clicked on a chromatogram window.
 * @param params parameters object. (This has mouse event and graphics object.)
 * @return refresh flag (boolean value)
 */
kome::objects::Variant onChromDblClick( kome::objects::Parameters* params );
// @date 2011.03.29 <Add> M.Izumi <-

/**
 * @fn kome::objects::Variant draw2dPeaks( kome::objects::Parameters* params )
 * @brief draws 2D peaks
 * @param params parameters object
 * @return return value (This value has no meanings.)
 */
kome::objects::Variant draw2dPeaks( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant draw2dClusters( kome::objects::Parameters* params )
 * @brief draws 2D clusters
 * @param params parameters object
 * @return return value (This value has no meanings.)
 */
kome::objects::Variant draw2dPeaks( kome::objects::Parameters* params );


#ifdef __unix
	}
#endif  // __unix



#endif//  __KOME_VIEW_PEAK_LABEL_DRAWER_FUNCTIONS_H__

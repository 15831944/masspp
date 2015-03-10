/**
 * @file PeakDetectionFilterFunctions.h
 * @brief interfaces of plug-in functions
 *
 * @author tanaka-s
 * @date 2014.09.25
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation All rights reserved.
 */


#ifndef __KOME_PEAK_DETECTION_FILTER_FUNCTIONS_H__
#define __KOME_PEAK_DETECTION_FILTER_FUNCTIONS_H__


/**
 * @fn kome::objects::Variant SpecPeakFilterNumberOfPeaks( kome::objects::Parameters* params )
 * @brief performs peak filtering when "Number of Peaks" is requested
 * @param params parameters object
 * @return If true, it succeeded to perform the peak filtering. (boolean)
 */
kome::objects::Variant SpecPeakFilterNumberOfPeaks( kome::objects::Parameters* params );

#endif    // __KOME_PEAK_DETECTION_FILTER_FUNCTIONS_H__

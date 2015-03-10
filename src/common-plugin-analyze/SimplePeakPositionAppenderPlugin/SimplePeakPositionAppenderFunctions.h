/**
 * @file SimplePeakPositionAppenderFunctions.h
 * @brief interfaces of SimplePeakPositionAppender plug-in functions
 *
 * @author S.Tanaka
 * @date 2009.04.01
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_SIMPLE_PEAK_POSITION_APPENDER_FUNCTIONS_H__
#define __KOME_SIMPLE_PEAK_POSITION_APPENDER_FUNCTIONS_H__


#ifdef __unix
	extern "C" {
#endif  // __unix


/**
 * @fn kome::objects::Variant detectPeakPosition( kome::objects::Parameters* params )
 * @brief detects peak position
 * @param params parameters information
 * @return return value (This value ha no meanings.)
 */
kome::objects::Variant detectPeakPosition( kome::objects::Parameters* params );

#ifdef __unix
	}
#endif  // __unix



#endif // __KOME_SIMPLE_PEAK_POSITION_APPENDER_FUNCTIONS_H__

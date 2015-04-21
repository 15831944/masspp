/**
 * @file TimeFunctions.h
 * @brief interfaces of function which deals with time
 *
 * @author S.Tanaka
 * @date 2009.08.31
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_CORE_TIME_FUNCTIONS_H__
#define __KOME_CORE_TIME_FUNCTIONS_H__


/**
 * @fn long long getcurrenttime()
 * @brief gets current time in miliseconds
 * @return the difference between the current time and midnight, Jan 1, 1970.
 */
long long getcurrenttime();

/**
 * @fn std::string datetostring( struct tm* date )
 * @brief date -> string
 * @param[in] date date object
 * @return the date formatted as a string in the format DD MONTH YEAR HH:MM:SS, e.g. 19 Mar 2012 09:51:30
 */
std::string datetostring( struct tm* date );

#endif	// __KOME_CORE_TIME_FUNCTIONS_H__

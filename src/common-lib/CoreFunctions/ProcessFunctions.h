/**
 * @file ProcessFunctions.h
 * @brief interfaces of function which deals with process
 *
 * @author S.Tanaka
 * @date 2009.08.27
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_CORE_PROCESS_FUNCTIONS_H__
#define __KOME_CORE_PROCESS_FUNCTIONS_H__


/**
 * @fn unsigned int getprocessid()
 * @brief gets procsess ID
 * @return process ID
 */
unsigned int getprocessid();

/**
 * @fn bool processexists( const unsigned int pid )
 * @brief judges whether specified process is exists or not.
 * @param[in] pid process ID
 * @return If true, specified process exists.
 */
bool processexists( const unsigned int pid );


#endif	// __KOME_CORE_PROCESS_FUNCTIONS_H__

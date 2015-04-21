/**
 * @file ProcessFunctions.cpp
 * @brief implements of function which deals with process
 *
 * @author S.Tanaka
 * @date 2009.08.27
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"

#ifdef _MSC_VER
	#include <process.h>
#else
	#include <sys/types.h>
	#include <unistd.h>
	#include <signal.h>
#endif	// _MSC_VER


// get process ID
unsigned int getprocessid() {

#ifdef _MSC_VER
	const unsigned int pid = _getpid();
#else
	const unsigned int pid = getpid();
#endif	// _MSC_VER

	return pid;
}


// judges whether specified process exists or not
bool processexists( const unsigned int pid ) {
	bool ret = false;

#ifdef _MSC_VER
	HANDLE handle = OpenProcess(
		SYNCHRONIZE,
		FALSE,
		(DWORD)pid
	);
// >>>>>>	@Date:2013/10/03	<Modified>	A.Ozaki
	if  ( (HANDLE)NULL != handle )
	{
		ret = true;
	}
	if  ( GetLastError( ) == ERROR_ACCESS_DENIED )
	{
		ret = true;
	}
// <<<<<<	@Date:2013/10/03	<Modified>	A.Ozaki

	if( handle != NULL ) {
		CloseHandle( handle );
	}

#else
	if( kill( pid, 0 ) == 0 ) {
		ret = true;
	}
#endif	// _MSC_VER

	return ret;
}

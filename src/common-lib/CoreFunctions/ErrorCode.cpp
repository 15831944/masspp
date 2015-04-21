/**
 * @file ErrorCode.cpp
 * @brief implements of error code function
 *
 * @author S.Tanaka
 * @date 2006.08.18
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "ErrorCode.h"

#include "CoreMacros.h"

#include <string>
#include <vector>

std::vector<std::pair<std::string, unsigned int>>* pErrorList;

// get error code
unsigned int errorcode( const char* errorName ) {
	// static variable
	static unsigned int maxErrorCode = 0;

	// create string object
	std::string s = std::string( NVL( errorName, "" ) );

	// get error code
	unsigned code = maxErrorCode;

	unsigned errorIndex = -1;

	if (pErrorList == NULL)
	{
		pErrorList = new std::vector<std::pair<std::string, unsigned int>>;
	}

	for (int i = 0; (i < static_cast<int>(pErrorList->size())) && (errorIndex == -1); i++)
	{
		if ((*pErrorList)[i].first.compare(errorName) == 0)
		{
			errorIndex = i;
		}
	}

	if( errorIndex == -1 ) {	// new error code
		pErrorList->push_back(std::pair<std::string, unsigned int>(errorName, code));
		maxErrorCode++;
	}
	else {	// already exists
		code = (*pErrorList)[errorIndex].second;
	}

	return code;
}

// get error string for an existing error
const char* errorstring( unsigned int errorCode ) {

	unsigned errorIndex = -1;

	// get error string
	for (int i = 0; (i < static_cast<int>(pErrorList->size())) && (errorIndex == -1); i++)
	{
		if ((*pErrorList)[i].second == errorCode)
		{
			errorIndex = i;
		}
	}

	if (errorIndex != -1)
	{
		return (*pErrorList)[errorIndex].first.c_str();
	}
	else
	{
		return "";
	}
}

// deallocate error strings object
void errorcleanup( void )
{
// >>>>>>	@Date:2013/10/18	<Modified>	A.Ozaki
	if  ( pErrorList )
	{
		pErrorList->clear();
		delete pErrorList;
	}
// <<<<<<	@Date:2013/10/18	<Modified>	A.Ozaki
	pErrorList = NULL;
}

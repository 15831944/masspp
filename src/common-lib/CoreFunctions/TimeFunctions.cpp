/**
 * @file TimeFunctions.h
 * @brief implements of function which deals with time
 *
 * @author S.Tanaka
 * @date 2009.08.31
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"

#ifdef _MSC_VER
	#include	<time.h>

	#define EPOCH_FILE_TIME	116444736000000000
#else
	#include <sys/time.h>
#endif	// _MSC_VER

#include <wx/datetime.h>
#include <string>
#include "StringFunctions.h"

// get current time
long long getcurrenttime() {
	long long t = 0;

// get time
#ifdef _MSC_VER
	FILETIME ft;
	LARGE_INTEGER lv;
	long long v64;

	GetSystemTimeAsFileTime( &ft );
	lv.LowPart = ft.dwLowDateTime;
	lv.HighPart = ft.dwHighDateTime;

	v64 = lv.QuadPart - EPOCH_FILE_TIME;
	t = v64 / 10000;
#else
	struct timeval tv;
	gettimeofday( &tv, NULL );

	t = tv.tv_sec * 1000 + tv.tv_usec / 1000;
#endif	// _MSC_VER

	return t;
}

// date -> string
std::string datetostring( struct tm* date ) {

	std::string strDate;

	if (date != NULL)
	{
		wxDateTime wxWidDate(*date);
		strDate =  FMT( "%02d %s %04d %02d:%02d:%02d", wxWidDate.GetDay(), wxDateTime::GetMonthName(wxWidDate.GetMonth(), wxDateTime::Name_Abbr).c_str(), wxWidDate.GetYear(),
															wxWidDate.GetHour(), wxWidDate.GetMinute(), wxWidDate.GetSecond());
	}
	else
	{
		strDate = "";
	}

	return strDate;
}

/**
 * @file getNowDate
 * @brief get Now Date
 *
 * @author M.Fukuda
 * @date 2013.02.18
 *
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#include "getNowDate.h"
#include <time.h>

namespace ext {

	// get Now Date (Secure) YYYY:MM:DD.
const std::string getNowDate() {
	time_t nowt, jpt;
	struct tm tmst;
	int year, month, day;

	time(&nowt);
	localtime_s(&tmst, &nowt);
	jpt  = mktime(&tmst);
	
	year = tmst.tm_year + 1900;
	month = tmst.tm_mon + 1;
	day = tmst.tm_mday;
	char s[20];
	sprintf_s(s, "%04d.%02d.%02d", year, month, day);
	const std::string timeStr(s);
	return timeStr;
}

} // end of namespace

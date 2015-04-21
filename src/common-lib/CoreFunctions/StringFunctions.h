/**
 * @file StringFunctions.h
 * @brief interfaces of string function
 *
 * @author S.Tanaka
 * @date 2006.06.26
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_CORE_STRING_FUNCTIONS_H__
#define __KOME_CORE_STRING_FUNCTIONS_H__


#include <string>
#include <boost/function.hpp>
#include <vector>


#define FMT strfmt


/**
 * @fn std::string strfmt( const char* fmt, ... )
 * @brief get formated string
 * @param[in] fmt format
 * @return formated string
 */
std::string strfmt( const char* fmt, ... );

/**
 * @fn std::string wstr2str( std::wstring& ws )
 * @brief convert wstring to string
 * @param[in] ws wstring
 * @return string
 */
std::string wstr2str( std::wstring& ws );

/**
 * @fn std::wstring str2wstr( std::string& s )
 * @brief convert string to wstring
 * @param[in] s string
 * @return wstring
 */
std::wstring str2wstr( std::string& s );

/**
 * @fn std::string tolowercase( const char* s )
 * @brief convert all of the characters in the string given to lower case
 * @param[in] s string before converted to lower case
 * @return the string, converted to lower case
 */
std::string tolowercase( const char* s );

/**
 * @fn std::string touppercase( const char* s )
 * @brief convert all of the characters int the string given to upper case
 * @param[in] s string before converted to upper case
 * @return the string, converted to upper case
 */
std::string touppercase( const char* s );

/**
 * @fn int compareignorecase( const char* s1, const char* s2 )
 * @brief compare two strings ignoring case considerations
 * @param[in] s1 the string to compare
 * @param[in] s2 the string to be compared
 * @return - If s1 sorts lexicographically after s2, this function returns greater than zero.
 *		 - If s1 sorts lexicographically before s2, this function returns less than zero.
 *		 - If two strings match, this function returns zero.
 */
int compareignorecase( const char* s1, const char* s2 );

/**
 * @fn unsigned int stringtoken(
			const char* s,
			const char* delim,
				std::vector<std::string>& tokens
	)
 * @brief get tokens from string
 * @param[in] s string
 * @param[in] delim delimiters
 * @param[out] tokens the array to store tokens
 * @return number of tokens
 */
unsigned int stringtoken(
		const char* s,
		const char* delim,
		std::vector<std::string>& tokens
);

/**
 * @fn unsigned int stringseparate(
		    const char* s,
		    const char* separator,
		    std::vector<std::string>& tokens
		)
 * @brief separates string
 * @param[in] s string
 * @param[in] separator separator
 * @param[out] tokens the array to store tokens
 * @return the number of string values
 */
unsigned int stringseparate(
		const char* s,
		const char* separator,
		std::vector<std::string>& tokens
);

/**
 * @fn bool isint( const char* s, const int radix = 10, int* val = NULL )
 * @brief judge whether sutisfied character string shows integer number.
 * @param[in] s character string
 * @param[in] radix radix
 * @param[out] val If sutisfied character string is integer number, the number is stored in val.
			  If val is NULL, the number is not stored.
 * @return If sutiffied character string shows integer number, this function returns true.
 */
bool isint( const char* s, const int radix = 10, int* val = NULL );

/**
 * @fn int toint( const char* s, const int radix = 10, const int dfval = 0 )
 * @brief convert string into integer
 * @param[in] s character string converted into integer.
 * @param[in] radix radix
 * @param[in] dfval default value
 * @return integer value acquired from character string. ( If character string is illegal, this function returns dfval. )
 */
int toint( const char* s, const int radix = 10, const int dfval = 0 );

/**
 * @fn bool isint64( const char* s, const int radix = 10, long long* val = NULL )
 * @brief judge whether sutisfied character string shows 64 bit integer number.
 * @param[in] s character string
 * @param[in] radix radix
 * @param[out] val If sutisfied character string is integer number, the number is stored in val.
			  If val is NULL, the number is not stored.
 * @return If sutiffied character string shows integer number, this function returns true.
 */
bool isint64( const char* s, const int radix = 10, long long* val = NULL );

/**
 * @fn long long toint64( const char* s, const int radix = 10, const long long dfval = 0 )
 * @brief convert string into 64 bit integer
 * @param[in] s character string converted into integer.
 * @param[in] radix radix
 * @param[in] dfval default value
 * @return integer value acquired from character string. ( If character string is illegal, this function returns dfval. )
 */
long long toint64( const char* s, const int radix = 10, const long long dfval = 0 );

/**
 * @fn bool isdouble( const char* s, double* val = NULL )
 * @brief judge whether sutisfied character string shows real number.
 * @param[in] s character string
 * @param[out] val If sutisfied characgter string is real number, the number is stored in val.
			  If val is NULL, the number is not stored.
 * @return If sutisfied character string shows real number, this function returns true.
 */
bool isdouble( const char* s, double* val = NULL );

/**
 * @fn double todouble( const char* s, const double dfval = 0.0 )
 * @brief convert string into double
 * @param[in] s character string converted into double
 * @param[in] dfval default value
 * @return double value acquired from character string. ( If character string is illegal, this function returns dfval. )
 */
double todouble( const char* s, const double dfval = 0.0 );

/**
 * @fn bool tobool( const char* s, bool dfVal = false )
 * @brief get true or false from character string. 
		  - If s equals "1", "true" or "on" this method returns true.
		  - If s equals "0", "false" or "off" this method returns false.
		  - Otherwise this method returns default value.
 * @param[in] s character string
 * @param[in] dfVal defaultvalue
 * @return boolean value
 */
bool tobool( const char* s, bool dfVal = false );

/**
 * @fn std::string trimstring( const char* s )
 * @brief remove white spaces from both ends of specified string
 * @param[in] s character string to be trimmed
 * @return trimmed character string
 */
std::string trimstring( const char* s );

/**
 * @fn bool startswith( const char* s, const char* prefix )
 * @brief judges whether there is a sub-string at the start of string
 * @param[in] s string to test
 * @param[in] prefix sub-string to be checked
 * @return If the sub-string is found at the start of string, this method returns true.
 */
bool startswith( const char* s, const char* prefix );

/**
 * @fn bool endswith( const char* s, const char* suffix )
 * @brief judge whether there is a sub-string at the end of string
 * @param[in] s string to test
 * @param[in] suffix sub-string to be checked
 * @return If the sub-string is found at the end of string, this method returns true.
 */
bool endswith( const char* s, const char* suffix );

/**
 * @fn std::string replacestring( const char* s, const char* oldStr, const char* newStr )
 * @brief replaces specified substring with another one
 * @param[in] s character string to be replaced substring
 * @param[in] oldStr character string to be replaced
 * @param[in] newStr character string to replace
 * @return replaced string object
 */
std::string replacestring( const char* s, const char* oldStr, const char* newStr );

/**
 * @fn std::string shortenstring( const char* s, const unsigned int len, const bool cutTail )
 * @brief shorten the specified string
 * @param[in] s source string
 * @param[in] len maximum the length of string
 * @param[in] cutTail If true, the tail of the string is cut, otherwise the head of the string is cut.
 * @return shortened string
 */
std::string shortenstring( const char* s, const unsigned int len, const bool cutTail = true );

/**
 * @fn std::string regexsearch( const char* s, const char* regex )
 * @brief search string using regular expression.
 * @param[in] s string
 * @param[in] regex regular expression
 * @return matching string
 */
std::string regexsearch( const char* s, const char* regex );


#endif	// __KOME_CORE_STRING_FUNCTIONS_H__

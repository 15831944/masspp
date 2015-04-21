/**
 * @file StringFunctions.cpp
 * @brief implements of string function
 *
 * @author S.Tanaka
 * @date 2006.06.26
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"

#include "StringFunctions.h"
#include "CoreMacros.h"

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <set>
#include <list>
#include <boost/regex.hpp>


#define TMP_STRING         "MaSs2pLus0PlUs1tMp1sTrInG"


// define
#ifdef _MSC_VER
#define STRING_BUFFER_SIZE 0x1000
#endif	// _MSC_VER


/// white spaces
const char* g_whiteSpaces = " \t\r\n";

/// white spaces set
std::set<char> g_whiteSpacesSet;


// get formated string
std::string strfmt( const char* fmt, ... ) {
	// definition
	char* s = NULL;
	va_list ap;

	// get string
	va_start( ap, fmt );

#ifdef _MSC_VER

	s = (char*)malloc( STRING_BUFFER_SIZE );
	vsprintf_s( s, STRING_BUFFER_SIZE, fmt, ap );

#else

	vasprintf( &s, fmt, ap );

#endif	// _MSC_VER

	va_end( ap );

	// create std::string object
	std::string s2 = std::string( s );

	// free
	free( s );

	return s2;
}

// wstring -> string
std::string wstr2str( std::wstring& ws ) {
	// get buffer size
	size_t bufferSize = ( ws.length() + 1 ) * MB_CUR_MAX;

	// create buffer
	char* buffer = new char[ bufferSize ];

	// convert
	size_t size = 0;

#ifdef _MSC_VER
	wcstombs_s( &size, buffer, bufferSize, ws.c_str(), ws.size() );
#else
	size = wcstombs( buffer, ws.c_str(), size );
#endif	// _MSC_VER

	std::string s = std::string( buffer );

	delete[] buffer;

	return s;
}

// string -> wstring
std::wstring str2wstr( std::string& s ) {
	// get buffer size
	size_t bufferSize = s.length() + 1;

	// create buffer
	wchar_t* buffer = new wchar_t[ bufferSize ];

	// convert
	size_t size = 0;

#ifdef _MSC_VER
	mbstowcs_s( &size, buffer, bufferSize, s.c_str(), s.size() );
#else
	size = mbstowcs( buffer, s.c_str(), bufferSize );
#endif	// _MSC_VER

	std::wstring ws = std::wstring( buffer );

	delete[] buffer;

	return ws;
}

// to lower case
std::string tolowercase( const char* s ) {
	// string object
	std::string str;
	if( s == NULL ) {
		return str;
	}

	// length
	unsigned int len = (unsigned int)strlen( s );

	// to lower case
	for( unsigned int i = 0; i < len; i++ ) {
		str.push_back( tolower( s[ i ] ) );
	}

	return str;
}

// to upper case
std::string touppercase( const char* s ) {
	// string object
	std::string str;
	if( s == NULL ) {
		return str;
	}

	// length
	unsigned int len = (unsigned int)strlen( s );

	// to upper case
	for( unsigned int i = 0; i < len; i++ ) {
		str.push_back( toupper( s[ i ] ) );
	}

	return str;
}

// compare ignore case
int compareignorecase( const char* s1, const char* s2 ) {
	return strcmp( tolowercase( s1 ).c_str(), tolowercase( s2 ).c_str() );
}

// get tokens
unsigned int stringtoken(
		const char* s,
		const char* delim,
		std::vector< std::string >& tokens
){
	// check
	std::string::size_type delimLen = strlen( delim );
	if( delimLen == 0 ) {
		tokens.push_back( s );
		return 1;
	}

	// string
	std::string str = NVL( s, "" );
	std::list< std::string > strList;

	// double quote
	std::string::size_type start = std::string::npos;
	std::string::size_type pos = 0;
	while( ( pos = str.find(  "\"", ( start == std::string::npos ? 0 : start + 1 ) ) ) != str.npos ) {
		if( start == std::string::npos ) {
			start = pos;
		}
		else {
			strList.push_back( str.substr( start + 1, pos - start - 1 ) );

			std::string tmp = str.substr( 0, start );
			tmp.append( TMP_STRING );
			tmp.append( str.substr( pos + 1 ) );
			str = tmp;

			start = std::string::npos;
		}
	}
	str = FMT( "%s%c", str.c_str(), delim[ 0 ] );

	// separate
	std::string::size_type tmpLen = strlen( TMP_STRING );
	std::string::size_type cnt = 0;
	start = 0;

	for( unsigned int i = 0; i < str.length(); i++ ) {
		// check the character
		char c = str[ i ];
		bool found = false;
		for( unsigned int j = 0; j < delimLen && !found; j++ ) {
			if( c == delim[ j ] ) {
				found = true;
			}
		}

		// add
		if( found ) {
			std::string val = str.substr( start, i - start );

			std::string::size_type rPos = 0;
			while( ( rPos = val.find( TMP_STRING ) ) != val.npos ) {
				std::string tmp = val.substr( 0, rPos );
				tmp.append( strList.front() );
				strList.pop_front();
				tmp.append( val.substr( rPos + tmpLen ) );

				val = tmp;
			}

			if( !val.empty() ) {
				tokens.push_back( val );
				cnt++;
			}

			start = i + 1;
		}
	}

	return (unsigned int)cnt;
}

// string separate
unsigned int stringseparate(
		const char* s,
		const char* separator,
		std::vector< std::string >& tokens
) {
	// check
	std::string::size_type sepLen = strlen( separator );
	if( sepLen == 0 ) {
		tokens.push_back( s );
		return 1; 
	}

	// string
	std::string str = NVL( s, "" );
	std::list< std::string > strList;

	// double quote
	std::string::size_type start = std::string::npos;
	std::string::size_type pos = 0;
	while( ( pos = str.find(  "\"", ( start == std::string::npos ? 0 : start + 1 ) ) ) != str.npos ) {
		if( start == std::string::npos ) {
			start = pos;
		}
		else {
			strList.push_back( str.substr( start + 1, (int)pos - start - 1 ) );

			std::string tmp = str.substr( 0, start );
			tmp.append( TMP_STRING );
			tmp.append( str.substr( pos + 1 ) );
			str = tmp;

			start = std::string::npos;
		}
	}
	str.append( separator );

	// separate
	std::string::size_type tmpLen = strlen( TMP_STRING );
	std::string::size_type cnt = 0;
	start = 0;
	pos = 0;

	while( ( pos = str.find( separator, start ) ) != str.npos ) {
		// value
		std::string val = str.substr( start, (int)pos - start );

		std::string::size_type rPos = 0;
		while( ( rPos = val.find( TMP_STRING ) ) != val.npos ) {
			std::string tmp = val.substr( 0, rPos );
			tmp.append( strList.front() );
			strList.pop_front();
			tmp.append( val.substr( rPos + tmpLen ) );
			
			val = tmp;
		}

		tokens.push_back( val );
		cnt++;

		start = (int)( pos + sepLen );
	}

	return (unsigned int)cnt;
}

// integer number or not
bool isint( const char* s, const int radix, int* val ) {
	// check parameter
	if( s == NULL ) {
		return false;
	}

	// convert
	char* endptr = NULL;
	int v = strtol( s, &endptr, radix );

	if( endptr == s ) {		// not integer
		return false;
	}

	// •ÏŠ·‚Å‚«‚È‚©‚Á‚½•¶Žš‚©‚çŒã‚Ì’·‚³‚ð’²‚×‚é	// SPEC 90640
	if( 0 < strlen( endptr ) ){
		// INT‚Å‚Í‚È‚¢‚Æ‚Ý‚È‚·
		return false;
	}
	
	// store value.
	if( val != NULL ) {
		*val = v;
	}
	return true;
}

// string -> int
int toint( const char* s, const int radix, const int dfval ) {
	// check parameter
	if( s == NULL ) {
		return dfval;
	}

	// convert
	char* endptr = NULL;
	int val = strtol( s, &endptr, radix );

	if( endptr == s ) {		// character string doesn't show integer number.
		val = dfval;
	}

	return val;
}

// integer number or not
bool isint64( const char* s, const int radix, long long* val ) {
	// check parameter
	if( s == NULL ) {
		return false;
	}

	// convert
	char* endptr = NULL;
	long long v = 0;

#ifdef _MSC_VER
	v = _strtoi64( s, &endptr, radix );
#else
	v = strtoll( s, &endptr, radix );
#endif		// _MSC_VER

	if( endptr == s ) {		// not integer
		return false;
	}

	// store value.
	if( val != NULL ) {
		*val = v;
	}
	return true;
}

// string -> int
long long toint64( const char* s, const int radix, const long long dfval ) {
	// check parameter
	if( s == NULL ) {
		return dfval;
	}

	// convert
	char* endptr = NULL;
	long long val = dfval;

#ifdef _MSC_VER
	val = _strtoi64( s, &endptr, radix );
#else
	val = strtoll( s, &endptr, radix );
#endif		// _MSC_VER

	if( endptr == s ) {		// character string doesn't show integer number.
		val = dfval;
	}

	return val;
}

// real number of not
bool isdouble( const char* s, double* val ) {
	// check parameter
	if( s == NULL ) {
		return false;
	}
		
	// convert
	char* endptr = NULL;
	double v = strtod( s, &endptr );
	if( endptr == s ) {		// not real number
		return false;
	}
	
	// •ÏŠ·‚Å‚«‚È‚©‚Á‚½•¶Žš‚©‚çŒã‚Ì’·‚³‚ð’²‚×‚é	// SPEC 90640
	if( 0 < strlen( endptr ) ){
		// double‚Å‚Í‚È‚¢‚Æ‚Ý‚È‚·
		return false;
	}

	// store value.
	if( val != NULL ) {
		*val = v;
	}
	return true;
}

// string -> double
double todouble( const char* s, const double dfval ) {
	// check parameter
	if( s == NULL ) {
		return dfval;
	}

	// convert
	char* endptr = NULL;
	double val = strtod( s, &endptr );
	if( endptr == s ) {		// character string doesn't sho real number.
		val = dfval;
	}

	return val;
}

// get boolean value
bool tobool( const char* s, bool dfVal ) {
	// convert to upper case
	std::string value = touppercase( s );

	// get value
	if( value.compare( "1" ) == 0
			|| value.compare( "TRUE" ) == 0 || value.compare( "ON" ) == 0 ) {
		return true;
	}
	if( value.compare( "0" ) == 0
			|| value.compare( "FALSE" ) == 0 || value.compare( "OFF" ) == 0 ) {
		return false;
	}
	return dfVal;
}

// trim string
std::string trimstring( const char* s ) {
	// string object
	std::string str;

	// check parameter
	if( s == NULL ) {
		return str;
	}

	// create white spaces set
	if( g_whiteSpacesSet.size() == 0 ) {
		size_t num = strlen( g_whiteSpaces );
		for( unsigned int i = 0; i < num; i++ ) {
			g_whiteSpacesSet.insert( g_whiteSpaces[i] );
		}
	}

	// create working buffer
	size_t length = strlen( s );
	size_t size = length + 1;
	char* buffer = new char[ size ];

	// copy character string to buffer
	memcpy( buffer, s, size );

	// trim the front end of string
	char* firstChar = buffer;
	while( g_whiteSpacesSet.find( *firstChar ) != g_whiteSpacesSet.end()
			&& *firstChar != '\0' ) {
		firstChar++;
	}

	// trim the back end of string
	char* lastChar = buffer + length;
	if( firstChar != lastChar ) {
		lastChar--;
	}
	while( lastChar != firstChar
			&& g_whiteSpacesSet.find( *lastChar ) != g_whiteSpacesSet.end() ) {
		*lastChar = '\0';
		lastChar--;
	}

	// create string object
	std::string trimmed( firstChar );

	// double quote
	if( trimmed.length() >= 2 && trimmed.front() == '\"' && trimmed.back() == '\"' ) {
		trimmed = trimmed.substr( 1, trimmed.length() - 2 );
	}

	delete[] buffer;

	return trimmed;
}

// starts with
bool startswith( const char* s, const char* prefix ) {
	// create string object
	std::string str( NVL( s, "" ) );
	std::string prfx( NVL( prefix, "" ) );

	// check
	std::string::size_type pos = str.find( prfx );

	return ( pos == 0 );
}

// ends with
bool endswith( const char* s, const char* suffix ) {
	// create string object
	std::string str( NVL( s, "" ) );
	std::string sffx( NVL( suffix, "" ) );

	// check
	size_t pos = str.rfind( sffx );

	return ( pos == ( str.length() - sffx.length() ) );
}

// replace string
std::string replacestring( const char* s, const char* oldStr, const char* newStr ) {
	// create string object
	std::string str;
	if( s == NULL ) {
		return str;
	}
	str = s;

	// check parameters
	if( oldStr == NULL || newStr == NULL ) {
		return str;
	}
	if( strcmp( oldStr, newStr ) == 0 ) {
		return str;
	}

	// find substring
	std::string::size_type pos = str.find( oldStr );
	if( pos == str.npos ) {
		return str;
	}

	// replace
	size_t length = strlen( oldStr );
	str = str.substr( 0, pos ) + std::string( newStr ) + replacestring( s + ( pos + length), oldStr, newStr );

	return str;
}

// shorten string
std::string shortenstring( const char* s, const unsigned int len, const bool cutTail ) {
	// string object
	std::string tmp = NVL( s, "" );

	// check the length
	if( tmp.length() <= len ) {
		return tmp;
	}

	if( len <= 3 ) {
		tmp = "...";
		return tmp;
	}

	// shorten
	if( cutTail ) {
		tmp = tmp.substr( 0, len - 3 );
		tmp.append( "..." );
	}
	else {
		tmp = tmp.substr( 3 );
		tmp = FMT( "...%s", tmp.c_str() );
	}

	return tmp;
}

// regex search
std::string regexsearch( const char* s, const char* regex ) {
	// return value
	std::string ret;

	boost::regex r( regex );
	boost::match_results< const char* > results;
	if( boost::regex_search( s, results, r ) ) {
		ret = results.str( 1 );
	}

	return ret;
}

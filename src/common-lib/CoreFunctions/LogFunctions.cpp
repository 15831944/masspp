/**
 * @file LogFunctions.cpp
 * @brief implements of log function
 *
 * @author S.Tanaka
 * @date 2006.06.26
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"

#include "LogFunctions.h"

#include "FileFunctions.h"
#include "StringFunctions.h"
#include "CoreMacros.h"

#include <map>
#include <list>
#include <utility>


/** log level */
int g_logLevel = LOG_LEVEL_WARN;

/** write log function */
void (*g_outputLogFun)( const char*, int ) = NULL;


/** error code */
int g_errorCode = 0;

/** log list size */
int g_logListSize = 0;

/** error message */
std::string g_errorMessage;

/** temporary error message */
std::string g_tmperrorMessage;

/** log list */
std::list< std::pair< int, std::string > > g_logList;

/** log headers */
static const char* LOG_HEADERS[7] = {
	"",
	"[Fatal] :",
	"[Error] :",
	"[Warn]  :",
	"[Info]  :",
	"[Debug] :",
	"[Trace] :"
};


/**
 * @fn void setErrorCode( int code, const char* msg )
 * @brief set error code and message
 * @param code error code
 * @param msg error message
 */
void setErrorCode( int code, const char* msg ) {
	g_errorCode = code;
	g_errorMessage = std::string( msg );
}

// set log level
void setloglevel( int level ) {
	// check parameter
	if( level < MIN_LOG_LEVEL ) {
		level = MIN_LOG_LEVEL;
	}
	if( level > MAX_LOG_LEVEL ) {
		level = MAX_LOG_LEVEL;
	}

	// set parameter
	g_logLevel = level;
}

// get log level
int getloglevel() {
	return g_logLevel;
}

// get last error code
int getlasterror() {
	return g_errorCode;
}

// get last error message
const char* getlasterrormessage() {

	g_tmperrorMessage = g_errorMessage;

	// Delete the error message after it has been used.
	seterrorcode(-1, NULL);

	return g_tmperrorMessage.c_str();
}

// output log
void outputlog( const char* msg, int level, const char* file, int line ) {
	// check log level and message
	level = CLAMP( level, MIN_LOG_LEVEL, MAX_LOG_LEVEL );
	if( g_logLevel < level ) {
		return;
	}

	// create message
	std::string s = std::string( LOG_HEADERS[ level ] );
	if( file != NULL ) {
		s += " ";
		s += getfilename( file );
	}
	if( line >= 0 ) {
		s += strfmt( "(%d)", line );
	}
	if( msg != NULL ) {
		if( file == NULL && line < 0 ) {
			s += strfmt( "%s", msg );
		}
		else {
			s += strfmt( " - %s ", msg );
		}
	}

	// output log
	if( g_outputLogFun != NULL ) {
		(*g_outputLogFun)( s.c_str(), level );
	}

	// add to list
	if( g_logListSize != 0 ) {
		std::pair< int, std::string > logInfo = std::make_pair( level, s );
		g_logList.push_back( logInfo );
		
		setloglistsize( g_logListSize );
	}
}

// set error code
void seterrorcode( int code, const char* msg ) {
	g_errorCode = code;

	if( msg == NULL ) {
		g_errorMessage.clear();
	}
	else {
		g_errorMessage = std::string( msg );
	}
}

// output fatal log
void logfatal( const char* msg, const char* file, int line ) {
	outputlog( msg, LOG_LEVEL_FATAL, file, line );
}

void logfatalcode( int code, const char* msg, const char* file, int line ) {
	seterrorcode( code, msg );
	logfatal( msg, file, line );
}

// output error log
void logerror( const char* msg, const char* file, int line ) {
	outputlog( msg, LOG_LEVEL_ERROR, file, line );
}

void logerrorcode( int code, const char* msg, const char* file, int line ) {
	seterrorcode( code, msg );
	logerror( msg, file, line );
}

// output warning log
void logwarn( const char* msg, const char* file, int line ) {
	outputlog( msg, LOG_LEVEL_WARN, file, line );
}

void logwarncode( int code, const char* msg, const char* file, int line ) {
	seterrorcode( code, msg );
	logwarn( msg, file, line );
}

// output information log
void loginfo( const char* msg, const char* file, int line ) {
	outputlog( msg, LOG_LEVEL_INFO, file, line );
}

void loginfocode( int code, const char* msg, const char* file, int line ) {
	seterrorcode( code, msg );
	loginfo( msg, file, line );
}

// output debug log
void logdebug( const char* msg, const char* file, int line ) {
	outputlog( msg, LOG_LEVEL_DEBUG, file, line );
}

void logdebugcode( int code, const char* msg, const char* file, int line ) {
	seterrorcode( code, msg );
	logdebug( msg, file, line );
}

// output trace log
void logtrace( const char* msg, const char* file, int line ) {
	outputlog( msg, LOG_LEVEL_TRACE, file, line );
}

void logtracecode( int code, const char* msg, const char* file, int line ) {
	seterrorcode( code, msg );
	logtrace( msg, file, line );
}

// set log list size
void setloglistsize( const int size ) {
	// set to variable
	g_logListSize = MAX( size, -1 );

	// check size
	if( size > 0 ) {
		while( (int)g_logList.size() > size ) {
			g_logList.pop_front();
		}
	}
}

// get log list size
int getloglistsize() {
	return g_logListSize;
}

// flush log
void logflush() {
	// output
	for( std::list< std::pair< int, std::string > >::iterator it = g_logList.begin();
			it != g_logList.end(); it++ ) {
		g_outputLogFun( (*it).second.c_str(), (*it).first );
	}

	// clear list
	g_logList.clear();
}

// set output log function
void setoutputlogfunction( void (*outputLogFun)( const char*, int ) ) {
	g_outputLogFun = outputLogFun;
}

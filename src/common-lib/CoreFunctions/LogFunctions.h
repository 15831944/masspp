/**
 * @file LogFunctions.h
 * @brief interfaces of log function
 *
 * @author S.Tanaka
 * @date 2006.06.26
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_CORE_LOG_FUNCTIONS_H__
#define __KOME_CORE_LOG_FUNCTIONS_H__


// define
#define LOG_LEVEL_NONE  0
#define LOG_LEVEL_FATAL 1
#define LOG_LEVEL_ERROR 2
#define LOG_LEVEL_WARN  3
#define LOG_LEVEL_INFO  4
#define LOG_LEVEL_DEBUG 5
#define LOG_LEVEL_TRACE 6

#define MIN_LOG_LEVEL   0
#define MAX_LOG_LEVEL   6

// macros
#ifdef _DEBUG
	#define OUTPUT_LOG( msg, level )	{				   \
		if( level <= getloglevel() ) {					  \
			outputlog( msg, level, __FILE__, __LINE__ );	\
		}												   \
	}

	#define LOG_FATAL_CODE( msg, code )	logfatalcode( code, msg.c_str(), __FILE__, __LINE__ )
	#define LOG_ERROR_CODE( msg, code )	logerrorcode( code, msg.c_str(), __FILE__, __LINE__ )
	#define LOG_WARN_CODE( msg, code )	 logwarncode( code, msg.c_str(), __FILE__, __LINE__ )
	#define LOG_INFO_CODE( msg, code )	 loginfocode( code, msg.c_str(), __FILE__, __LINE__ )
	#define LOG_DEBUG_CODE( msg, code )	logdebugcode( code, msg.c_str(), __FILE__, __LINE__ )
	#define LOG_TRACE_CODE( msg, code )	logtracecode( code, msg.c_str(), __FILE__, __LINE__ )
#else
	#define OUTPUT_LOG( msg, level )	{				   \
		if( level <= getloglevel() ) {					  \
			outputlog( msg, level, NULL, -1 );			  \
		}												   \
	}

	#define LOG_FATAL_CODE( msg, code )	logfatalcode( code, msg.c_str(), NULL, -1 )
	#define LOG_ERROR_CODE( msg, code )	logerrorcode( code, msg.c_str(), NULL, -1 )
	#define LOG_WARN_CODE( msg, code )	 logwarncode( code, msg.c_str(), NULL, -1 )
	#define LOG_INFO_CODE( msg, code )	 loginfocode( code, msg.c_str(), NULL, -1 )
	#define LOG_DEBUG_CODE( msg, code )	logdebugcode( code, msg.c_str(), NULL, -1 )
	#define LOG_TRACE_CODE( msg, code )	logtracecode( code, msg.c_str(), NULL, -1 )
#endif		// _DEBUG

#define LOG_FATAL( msg )			   OUTPUT_LOG( msg.c_str(), LOG_LEVEL_FATAL )
#define LOG_ERROR( msg )			   OUTPUT_LOG( msg.c_str(), LOG_LEVEL_ERROR )
#define LOG_WARN( msg )				OUTPUT_LOG( msg.c_str(), LOG_LEVEL_WARN )
#define LOG_INFO( msg )				OUTPUT_LOG( msg.c_str(), LOG_LEVEL_INFO )
#define LOG_DEBUG( msg )			   OUTPUT_LOG( msg.c_str(), LOG_LEVEL_DEBUG )
#define LOG_TRACE( msg )			   OUTPUT_LOG( msg.c_str(), LOG_LEVEL_TRACE )


/**
 * @fn void setloglevel( int level )
 * @brief set log level
 * @param[in] level log level
 */
void setloglevel( int level );

/**
 * @fn int getloglevel()
 * @brief get log level
 * @return log level
 */
int getloglevel();

/**
 * @fn int getlasterror()
 * @brief get last error code
 * @return error code
 */
int getlasterror();

/**
 * @fn const char* getlasterrormessage()
 * @brief get last error message
 * @return last error message
 */
const char* getlasterrormessage();

/**
 * @fn void seterrorcode( int code, const char* msg )
 * @brief set error code
 * @param[in] code error code
 * @param[in] msg message
 */
void seterrorcode( int code, const char* msg );

/**
 * @fn void outputlog( const char* msg, int level, const char* file = NULL, int line = -1)
 * @brief output log
 * @param[in] msg log message
 * @param[in] level log level
 * @param[in] file file name
 * @param[in] line line number
 */
void outputlog( const char* msg, int level, const char* file = NULL, int line = -1);

/**
 * @fn void logfatal( const char* msg, const char* file = NULL, int line = -1 )
 * @brief output fatal log
 * @param[in] msg log message
 * @param[in] file source file name
 * @param[in] line line number
 */
void logfatal( const char* msg, const char* file = NULL, int line = -1 );

/**
 * @fn void logfatalcode( int code, const char* msg, const char* file = NULL, int line = -1 )
 * @brief output fatal log and set error code
 * @param[in] code error code
 * @param[in] msg log message
 * @param[in] file source file name
 * @param[in] line line number
 */
void logfatalcode( int code, const char* msg, const char* file = NULL, int line = -1 );

/**
 * @fn void logerror( const char* msg, const char* file = NULL, int line = -1 )
 * @brief output error log
 * @param msg log message
 * @param file source file name
 * @param line line number
 */
void logerror( const char* msg, const char* file = NULL, int line = -1 );

/**
 * @fn void logerrorcode( int code, const char* msg, const char* file = NULL, int line = -1 )
 * @brief output error log and set error code
 * @param[in] code error code
 * @param[in] msg log message
 * @param[in] file source file name
 * @param[in] line line number
 */
void logerrorcode( int code, const char* msg, const char* file = NULL, int line = -1 );

/**
 * @fn void logwarn( const char* msg, const char* file = NULL, int line = -1 )
 * @brief output warning log
 * @param[in] msg log message
 * @param[in] file source file name
 * @param[in] line line number
 */
void logwarn( const char* msg, const char* file = NULL, int line = -1 );

/**
 * @fn void logwarncode( int code, const char* msg, const char* file = NULL, int line = -1 )
 * @brief output warning log and set error code
 * @param[in] code error code
 * @param[in] msg log message
 * @param[in] file source file name
 * @param[in] line line number
 */
void logwarncode( int code, const char* msg, const char* file = NULL, int line = -1 );

/**
 * @fn void loginfo( const char* msg, const char* file = NULL, int line = -1 )
 * @brief output information log
 * @param[in] msg log message
 * @param[in] file source file name
 * @param[in] line line number
 */
void loginfo( const char* msg, const char* file = NULL, int line = -1 );

/**
 * @fn void loginfocode( int code, const char* msg, const char* file = NULL, int line = -1 )
 * @brief output information log and set error code
 * @param[in] code error code
 * @param[in] msg log message
 * @param[in] file source file name
 * @param[in] line line number
 */
void loginfocode( int code, const char* msg, const char* file = NULL, int line = -1 );

/**
 * @fn void logdebug( const char* msg, const char* file = NULL, int line = -1 )
 * @brief output debug log
 * @param[in] msg log message
 * @param[in] file source file name
 * @param[in] line line number
 */
void logdebug( const char* msg, const char* file = NULL, int line = -1 );

/**
 * @fn void logdebugcode( int code, const char* msg, const char* file = NULL, int line = -1 )
 * @brief output debug log and set error code
 * @param[in] code error code
 * @param[in] msg log message
 * @param[in] file source file name
 * @param[in] line line number
 */
void logdebugcode( int code, const char* msg, const char* file = NULL, int line = -1 );

/**
 * @fn void logtrace( const char* msg, const char* file = NULL, int line = -1 )
 * @brief output trace log
 * @param[in] msg log message
 * @param[in] file source file name
 * @param[in] line line number
 */
void logtrace( const char* msg, const char* file = NULL, int line = -1 );

/**
 * @fn void logtracecode( int code, const char* msg, const char* file = NULL, int line = -1 )
 * @brief output trace log and set error code
 * @param[in] code error code
 * @param[in] msg log message
 * @param[in] file source file name
 * @param[in] line line number
 */
void logtracecode( int code, const char* msg, const char* file = NULL, int line = -1 );


/**
 * @fn void setloglistsize( const int size )
 * @brief sets log list size
 * @param[in] size log list size (If size is negative, log list is unbounded.)
 */
 void setloglistsize( const int size );

 /**
  * @fn int getloglistsize()
  * @brief gets log list size
  * @return log lost size (If return value is negative, log list is unbounded.)
  */
int getloglistsize();

/**
 * @fn void logflush()
 * @brief flushes logs to be stored in the list
 */
void logflush();

/**
 * @fn void setoutputlogfunction(  void (*outputLogFun)( const char*, int ) )
 * @brief set output log function
 * @param[in] outputLogFun the pointer to output log function.
		  Its arguments are log message (const char*) and log level (int)
 */
void setoutputlogfunction( void (*outputLogFun)( const char*, int ) );


#endif		// __KOME_CORE_LOG_FUNCTIONS_H__

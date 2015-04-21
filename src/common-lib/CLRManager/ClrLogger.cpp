/**
 * @file ClrLogger.cpp
 * @brief implements of ClrLogger class
 *
 * @author S.Tanaka
 * @date 2006.10.17
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#include "stdafx.h"
#include "ClrLogger.h"

#include "ClrObjectTool.h"


using namespace kome::clr;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
ClrLogger::ClrLogger() {
}

// destructor
ClrLogger::~ClrLogger() {
}

// set log level
void ClrLogger::setLogLevel( Level level ) {
	setloglevel( (int)level );
}

// get log level
ClrLogger::Level ClrLogger::getLogLevel() {
	return (Level)getloglevel();
}

// fatal
void ClrLogger::fatal( System::String^ msg ) {
	outputLog( msg, Level::FATAL );
}

// error
void ClrLogger::error( System::String^ msg ) {
	outputLog( msg, Level::ERROR );
}

// warning
void ClrLogger::warn( System::String^ msg ) {
	outputLog( msg, Level::WARN );
}

// information
void ClrLogger::info( System::String^ msg ) {
	outputLog( msg, Level::INFO );
}

// debug
void ClrLogger::debug( System::String^ msg ) {
	outputLog( msg, Level::DEBUG );
}

// trace
void ClrLogger::trace( System::String^ msg ) {
	outputLog( msg, Level::TRACE );
}

// fatal & error code
void ClrLogger::fatalCode( System::String^ msg, int code ) {
	outputLogCode( msg, code, Level::FATAL );
}

// error & error code
void ClrLogger::errorCode( System::String^ msg, int code ) {
	outputLogCode( msg, code, Level::ERROR );
}

// warning & error code
void ClrLogger::warnCode( System::String^ msg, int code ) {
	outputLogCode( msg, code, Level::WARN );
}

// information & error code
void ClrLogger::infoCode( System::String^ msg, int code ) {
	outputLogCode( msg, code, Level::INFO );
}

// debug & error code
void ClrLogger::debugCode( System::String^ msg, int code ) {
	outputLogCode( msg, code, Level::DEBUG );
}

// trace & error code
void ClrLogger::traceCode( System::String^ msg, int code ) {
	outputLogCode( msg, code, Level::TRACE );
}

// get last error code
int ClrLogger::getLastErrorCode() {
	return getlasterror();
}

// get last error message
System::String^ ClrLogger::getLastErrorMessage() {
	return ClrObjectTool::convertString( getlasterrormessage(), "" );
}

// get error code
int ClrLogger::getErrorCode( System::String^ name ) {
	return (int)errorcode( ClrObjectTool::convertString( name ).c_str() );
}

// output log
void ClrLogger::outputLog( System::String^ msg, Level level ) {
	// check log level
	int l = (int)level;
	if( l > getloglevel() ) {
		return;
	}

	// output log
	outputlog(
		ClrObjectTool::convertString( msg ).c_str(),
		l
	);
}

// output log and set error code
void ClrLogger::outputLogCode( System::String^ msg, int code, Level level ) {
	// set error code
	seterrorcode( code, ClrObjectTool::convertString( msg ).c_str() );

	// output log
	outputLog( msg, level );
}

// get instance
ClrLogger^ ClrLogger::getInstance() {
	// create object
	if( m_logger == nullptr ) {
		m_logger = gcnew ClrLogger();
	}

	return m_logger;
}

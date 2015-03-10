/**
 * @file PeaksIOPluginOperation.cpp
 * @brief Peaks IO Plugin Operation class
 *
 * @author M.Izumi
 * @date 2012.03.02
 * 
 * Copyright (C) 2014 Shimadzu Corporation All rights reserved.
 */


#include "stdafx.h"
#include "PeaksIOPluginOperation.h"
#include "PeaksIOManager.h"

using namespace kome::io::peaks;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


// constructor
PeaksIoOperation::PeaksIoOperation(){
}

// destructor
PeaksIoOperation::~PeaksIoOperation(){	
}

// execute
bool PeaksIoOperation::onExecute(){
	bool ret = true;
	return ret;
}

// on save condition
void PeaksIoOperation::onSaveCondition( boost::function< int ( void*, int ) > writeFun ){
}

// on load condition
void PeaksIoOperation::onLoadCondition( boost::function< int ( void*, int ) > readFun ){
}


// on get descrioption
std::string PeaksIoOperation::onGetDescription(){
	std::string s;

	return s;
}

// on get parameters string 
std::string PeaksIoOperation::onGetParametersString(){
	std::string s;

	return s;
}

// on set parameters string 
void PeaksIoOperation::onSetParametersString( const char* strParam ){

}

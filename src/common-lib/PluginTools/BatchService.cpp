/**
* @file BatchService.cpp
* @brief BatchService
* @author OKADA, H.
* @date 2012/02/07
* Copyright (C) 2012 Shimadzu Corporation All rights reserved.
*/

#include "BatchService.h"

using kome::plugin::BatchService;

// constructor
BatchService::BatchService() 
{
}

// destructor
BatchService::~BatchService() 
{
}

std::string BatchService::execute( kome::plugin::BatchInfo* ptrBatchInfo )
{
	return( onExecute(ptrBatchInfo) ); 
};

BatchService::BatchStatus BatchService::getStatus( const char* str )
{
	return( onGetStatus( str ) );  
};

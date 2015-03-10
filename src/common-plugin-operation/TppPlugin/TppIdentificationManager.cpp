/**
 * @file TppSearchGrid.cpp
 * @brief implements of TppSearchGrid class
 *
 * @author N.Tashiro
 * @date 2013.03.15
 * 
 * Copyright(C) 2013 Shimadzu. All rights reserved.
 */

#include "stdafx.h"
#include "TppIdentificationManager.h"

using namespace kome::Tpp;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


TppIdentificationManager::TppIdentificationManager(){

}

TppIdentificationManager* TppIdentificationManager::GetInstance(){
	IdentificationManager::getInstance();

}

TppIdentificationManager::~TppIdentificationManager(){

}

/**
 * @file TextManager.cpp
 * @brief implements of TextManager class
 *
 * @author S.Tanaka
 * @date 2010.01.08
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "TextManager.h"


using namespace kome::io::text;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



#include "TextSampleSet.h"


// constructor
TextManager::TextManager() {
}

// destructor
TextManager::~TextManager() {
	// delete sample sets
	while( m_fileSet.size() > 0 ) {
		delete *( m_fileSet.begin() );
	}
}

// add sample set
void TextManager::addSampleSet( TextSampleSet* sampleSet ) {
	if( sampleSet != NULL ) {
		m_fileSet.insert( sampleSet );
	}
}

// delete sample set
void TextManager::deleteSampleSet( TextSampleSet* sampleSet ) {
	m_fileSet.erase( sampleSet );
}

// get instance
TextManager& TextManager::getInstance() {
	// create object (This is the only object.)
	static TextManager mgr;

	return mgr;
}

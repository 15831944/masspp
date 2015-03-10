/**
 * @file TextSampleSet.cpp
 * @brief implements of TextSampleSet class
 *
 * @author S.Tanaka
 * @date 2010.01.08
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "TextSampleSet.h"

#include "TextSample.h"
#include "TextManager.h"


using namespace kome::io::text;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
TextSampleSet::TextSampleSet() {
	// initialize
	m_fp = NULL;

	// add to the manager
	TextManager& mgr = TextManager::getInstance();
	mgr.addSampleSet( this );
}

// destructor
TextSampleSet::~TextSampleSet() {
	// delete from the manager
	TextManager& mgr = TextManager::getInstance();
	mgr.deleteSampleSet( this );

	closeFile();
}

// get file descriptor
FILE* TextSampleSet::getFile() {
	// Spec No.93991 @date 2013.08.29 <Add> M.izumi ->
	if( m_fp == NULL ){
		openFile( getFilePath() );
	}
	// @date 2013.08.29 <Add> M.Izumi
	return m_fp;
}

// opne file
bool TextSampleSet::openFile( const char* path ){ 
			
	// open
	m_fp = fileopen( path, "rb" );
		
	if( m_fp == NULL ) {
		return false;
	}

	return true;
}

// close file
bool TextSampleSet::closeFile(){
	if( m_fp != NULL ) {
		fclose( m_fp );
		m_fp = NULL;	// @date 2011.03.17 <Add> M.Izumi
	}
	return true;
}

// on open file
bool TextSampleSet::onOpenFile( const char* path, kome::core::Progress* progress ) {	// add param @date 2014.07.08 <Mod> M.Izumi
	if( !openFile( path ) ){
		return false;
	}

	// create sample
	TextSample* sample = new TextSample( this );
	addSample( sample );

	closeFile();	// Spec No.93991 @date 2013.08.29 <Add> M.Izumi
	return true;
}

// on close file
bool TextSampleSet::onCloseFile() {
	if( !closeFile() ){
		return false;
	}
	return true;
}

// on open sample
bool TextSampleSet::onOpenSample( kome::objects::Sample* sample, kome::core::Progress* progress ) {		// @date 2014.07.08 <Mod> M.Izumi
	return true;
}

// on close sample
bool TextSampleSet::onCloseSample( kome::objects::Sample* sample ) {
	onCloseFile();		// @date 2011.03.17 <Add> M.Izumi
	return true;
}

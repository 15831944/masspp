/*
 * @file FileFactoryWrapper.cpp
 * @brief implements of FileFactoryWrapper class
 *
 * @author S.Tanaka
 * @date 2007.01.22
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "FileFactoryWrapper.h"

#include <boost/bind.hpp>
#include <vector>


using namespace kome::clr;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
FileFactoryWrapper::FileFactoryWrapper( kome::plugin::FileFactory& factory ) : m_factory( factory ) {
}

// destructor
FileFactoryWrapper::~FileFactoryWrapper() {
}

// get file factory object
kome::plugin::FileFactory& FileFactoryWrapper::getFileFactory() {
	return m_factory;
}

// open sample set file
SampleSetWrapper^ FileFactoryWrapper::openFile( System::String^ path ) { 
	// open
	kome::objects::SampleSet* ss = m_factory.openFile( ClrObjectTool::convertString( path ).c_str() ); 

	// sample set wrapper
	SampleSetWrapper^ sampleSet = ClrObjectTool::createSampleSetWrapper( ss );

	return sampleSet;
}

// close file
void FileFactoryWrapper::closeFile( SampleSetWrapper^ sampleSet ) {
	// sample set
	kome::objects::SampleSet* ss = ClrObjectTool::getSampleSet( sampleSet );

	// close
	m_factory.closeFile( ss );
}

// search sample set file
SampleSetWrapper^ FileFactoryWrapper::searchFile( System::String^ path ) {
	// open
	kome::objects::SampleSet* ss = m_factory.searchFile( ClrObjectTool::convertString( path ).c_str() );

	// sample set wrapper
	SampleSetWrapper^ sampleSet = ClrObjectTool::createSampleSetWrapper( ss );

	return sampleSet;
}

// get the number of file formats
unsigned int FileFactoryWrapper::getNumberOfFileFormats() {
	return m_factory.getNumberOfFileFormats();
}

// get format name
System::String^ FileFactoryWrapper::getFormatName( unsigned int index ) {
	// get format name
	const char* n = m_factory.getFormatName( index );

	// convert
	System::String^ name = ClrObjectTool::convertString( n, NULL );
	return name;
}

// get extensions
cli::array< System::String^ >^ FileFactoryWrapper::getExtensions( unsigned int index ) {
	// get extensions
	std::vector< std::string > extensions;
	m_factory.getExtensions(index,extensions);

	// create array
	if( extensions.size() == 0 ) {
		return nullptr;
	}
	
	cli::array< System::String^ >^ arr = gcnew cli::array< System::String^ >( extensions.size() );
	for( unsigned int i = 0; i < extensions.size(); i++ ) {
		arr[ i ] = ClrObjectTool::convertString( extensions[ i ].c_str(), NULL );
	}

	return arr;
}


// get file factory object
FileFactoryWrapper^ FileFactoryWrapper::getInstance() {
	kome::plugin::FileFactory& ff = kome::plugin::FileFactory::getInstance();
	FileFactoryWrapper^ factory = gcnew FileFactoryWrapper( ff );
	return factory;
}

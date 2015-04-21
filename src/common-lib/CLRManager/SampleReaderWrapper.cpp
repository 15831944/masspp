/**
* @file SampleReaderWrapper.cpp
* @brief Sample Reader Wrapper
* @author OKADA, H.
* @date 2012/07/18
* Copyright (C) 2014 Shimadzu Corporation All rights reserved.
* <補足説明>
*/


#include "SampleReaderWrapper.h"
#include "ClrObjectTool.h"

using kome::clr::SampleReaderWrapper;

// constructor
SampleReaderWrapper::SampleReaderWrapper( kome::objects::SampleReader& sample_reader ) : m_sampleReader(sample_reader) {
}


// destructor
SampleReaderWrapper::~SampleReaderWrapper(){
}


// sampleReaderを返します
kome::objects::SampleReader&  SampleReaderWrapper::getSampleReader(){
	return m_sampleReader;
}


// SampleReader オブジェクトの selectKeys を呼び出す。
System::String^ SampleReaderWrapper::selectKeys( void ){
	return ClrObjectTool::convertString( m_sampleReader.selectKeys().c_str(), "" );
};


// SampleReader オブジェクトの openData を呼び出す。
kome::clr::SampleWrapper^ SampleReaderWrapper::openData( System::String^ strKey ){
	return ClrObjectTool::createSampleWrapper(m_sampleReader.openData( ClrObjectTool::convertString( strKey )	));
};


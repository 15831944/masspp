/**
 * @file DataGroupNodeWrapper.cpp
 * @brief implements of DataGroupNodeWrapper class
 *
 * @author S.Tanaka
 * @date 2006.08.31
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#include "stdafx.h"
#include "DataGroupNodeWrapper.h"

#include "SpectrumWrapper.h"
#include "SampleWrapper.h"
#include "ChromatogramWrapper.h"
#include "ClrObjectTool.h"
#include "PropertiesWrapper.h"


using namespace kome::clr;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
DataGroupNodeWrapper::DataGroupNodeWrapper( kome::objects::DataGroupNode& group )
		: DataSetWrapper( group ), m_group( group ){
}

// destructor
DataGroupNodeWrapper::~DataGroupNodeWrapper() {
}

// get base object
kome::objects::DataGroupNode& DataGroupNodeWrapper::getDataGroupNode() {
	return m_group;
}

// set ID
void DataGroupNodeWrapper::setId( int id ) {
	m_group.setId( id );
}

// get ID
int DataGroupNodeWrapper::getId() {
	return m_group.getId();
}

// set name
void DataGroupNodeWrapper::setName( System::String^ name ) {
	m_group.setName( ClrObjectTool::convertString( name ).c_str() );
}

// get name
System::String^ DataGroupNodeWrapper::getName() {
	return gcnew System::String( m_group.getName() );
}

// set group index
void DataGroupNodeWrapper::setGroupIndex( int index ) {
	m_group.setGroupIndex( index );
}

// get group index
int DataGroupNodeWrapper::getGroupIndex() {
	return m_group.getGroupIndex();
}

// get parent spectrum group
DataGroupNodeWrapper^ DataGroupNodeWrapper::getParentGroup() {
	if( m_group.getParentGroup() == NULL ) {
		return nullptr;
	}
	return gcnew DataGroupNodeWrapper( *m_group.getParentGroup() );
}

// create new child
DataGroupNodeWrapper^ DataGroupNodeWrapper::createChildGroup( System::String^ name ) {
	// create child
	kome::objects::DataGroupNode* child
		= m_group.createChildGroup( ClrObjectTool::convertString( name ).c_str() );

	return gcnew DataGroupNodeWrapper( *child );
}

// remove child group
void DataGroupNodeWrapper::removeChildGroup( DataGroupNodeWrapper^ child ) {
	// group
	kome::objects::DataGroupNode* c = ClrObjectTool::getDataGroupNode( child );

	// remove
	m_group.removeChildGroup( c );
}

// get the number of child groups
unsigned int DataGroupNodeWrapper::getNumberOfChildren() {
	return m_group.getNumberOfChildren();
}

// get child
DataGroupNodeWrapper^ DataGroupNodeWrapper::getChild( unsigned int index ) {
	return gcnew DataGroupNodeWrapper( *m_group.getChild( index ) );
}

// get spectra
void DataGroupNodeWrapper::getDataSet( DataSetWrapper^ dataSet ) {
	m_group.getDataSet( &dataSet->getDataSet() );
}

// set auto scan number flag
void DataGroupNodeWrapper::setAutoScanNumber( bool autoScanNumber ) { 
	m_group.setAutoScanNumber( autoScanNumber );
}

// get auto scan number flag
bool DataGroupNodeWrapper::isAutoScanNumber() {
	return m_group.isAutoScanNumber();
}

// set spectrum x axis title
void DataGroupNodeWrapper::setSpecXTitle( System::String^ title ) {
	m_group.setSpecXTitle( ClrObjectTool::convertString( title ).c_str() );
}

// get spectrum x axis title
System::String^ DataGroupNodeWrapper::getSpecXTitle() {
	return ClrObjectTool::convertString( m_group.getSpecXTitle(), "" );
}

// set spectrum y axis title
void DataGroupNodeWrapper::setSpecYTitle( System::String^ title ) {
	m_group.setSpecYTitle( ClrObjectTool::convertString( title ).c_str() );
}

// get spectrum y axis title
System::String^ DataGroupNodeWrapper::getSpecYTitle() {
	return ClrObjectTool::convertString( m_group.getSpecYTitle(), "" );
}

// set chromatogram x axis title
void DataGroupNodeWrapper::setChromXTitle( System::String^ title ) {
	m_group.setChromXTitle( ClrObjectTool::convertString( title ).c_str() );
}

// get chromatogram x axis title
System::String^ DataGroupNodeWrapper::getChromXTitle() {
	return ClrObjectTool::convertString( m_group.getChromXTitle(), "" );
}

// set chromatogram y axis title
void DataGroupNodeWrapper::setChromYTitle( System::String^ title ) {
	m_group.setChromYTitle( ClrObjectTool::convertString( title ).c_str() );
}

// get chromatogram y axis title
System::String^ DataGroupNodeWrapper::getChromYTitle() {
	return ClrObjectTool::convertString( m_group.getChromYTitle(), "" );
}

// get level
unsigned int DataGroupNodeWrapper::getLevel() {
	return m_group.getLevel();
}

// get properties
PropertiesWrapper^ DataGroupNodeWrapper::getProperties() {
	return gcnew PropertiesWrapper( m_group.getProperties() );
}

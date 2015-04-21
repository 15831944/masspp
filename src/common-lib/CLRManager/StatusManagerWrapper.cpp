/**
 * @file StatusManagerWrapper.cpp
 * @brief implements of StatusManagerWrapper class
 *
 * @author S.Tanaka
 * @date 2007.01.25
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#include "stdafx.h"
#include "StatusManagerWrapper.h"

#include "ClrObjectTool.h"
#include "ClrIniFile.h"

#include <boost/bind.hpp>


using namespace kome::clr;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
StatusManagerWrapper::StatusManagerWrapper( kome::objects::StatusManager& statusMgr )
		: m_statusMgr( statusMgr ) {
}

// destrucotr
StatusManagerWrapper::~StatusManagerWrapper() {
}

// get status manger object
kome::objects::StatusManager& StatusManagerWrapper::getStatusManager() {
	return m_statusMgr;
}

// set value
void StatusManagerWrapper::setValue( System::String^ name, System::String^ value ) {
	m_statusMgr.setValue(
		ClrObjectTool::convertString( name ).c_str(),
		ClrObjectTool::convertString( value ).c_str()
	);
}

// get value
System::String^ StatusManagerWrapper::getValue( System::String^ name ) {
	return ClrObjectTool::convertString(
		m_statusMgr.getValue(
			ClrObjectTool::convertString( name ).c_str()
		),
		NULL
	);
}

// check having status
bool StatusManagerWrapper::hasStatus( System::String^ name ) {
	return m_statusMgr.hasStatus(
		ClrObjectTool::convertString( name ).c_str()
	);
}

// get the number of statuses
unsigned int StatusManagerWrapper::getNumberOfStatuses() {
	return m_statusMgr.getNumberOfStatuses();
}

// get status name
System::String^ StatusManagerWrapper::getStatusName( unsigned int index ) {
	return ClrObjectTool::convertString( m_statusMgr.getStatusName( index ), NULL );
}

// get status value
System::String^ StatusManagerWrapper::getStatusValue( unsigned int index ) {
	return ClrObjectTool::convertString( m_statusMgr.getStatusValue( index ), NULL );
}

// set parameters to ini file
void StatusManagerWrapper::setToIniFile( ClrIniFile^ ini, System::String^ section ) {
	// check parameters
	if( ini == nullptr || section == nullptr ) {
		return;
	}

	// set parameters
	m_statusMgr.setToIniFile(
		ini->getIniFile(),
		ClrObjectTool::convertString( section ).c_str()
	);
}

// get parameters from ini file
void StatusManagerWrapper::getFromIniFile( ClrIniFile^ ini, System::String^ section ) {
	// check parameters
	if( ini == nullptr || section == nullptr ) {
		return;
	}

	// set parameters
	m_statusMgr.getFromIniFile(
		ini->getIniFile(),
		ClrObjectTool::convertString( section ).c_str()
	);
}

// get status manager object
StatusManagerWrapper^ StatusManagerWrapper::getInstance() { 
	// create object
	if( m_mgr == nullptr ) {
		m_mgr = gcnew StatusManagerWrapper( kome::objects::StatusManager::getInstance() );
	}

	return m_mgr;
}

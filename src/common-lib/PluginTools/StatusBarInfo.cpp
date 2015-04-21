/**
* @file StatusBarInfo.cpp
* @brief Status Bar Info
* @author OKADA, H.
* @date 2011/01/12
* Copyright (C) 2014 Shimadzu Corporation All rights reserved.
* <補足説明>
*/

#include "Stdafx.h"
#include "PluginInfo.h"
#include "StatusBarInfo.h"


using namespace kome::plugin;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG

using kome::plugin::StatusBarInfo;

// constructor
StatusBarInfo::StatusBarInfo() {
	m_targetVal = VAL_SB_TGT_NONE;
	m_status_bar_name = "";
	m_vpPlugin.clear();
	m_status_bar_message = "";
	m_nOrder = 0;
	m_flgDisp = 1;
}

// destructor
StatusBarInfo::~StatusBarInfo() {
}

// clear data
void StatusBarInfo::clearStatusBarInfo( void )
{
	m_targetVal = VAL_SB_TGT_NONE;
	m_status_bar_name = "";
	m_vpPlugin.clear();
	m_status_bar_message = "";
	m_nOrder = 0;
}

// Set Target
void StatusBarInfo::setTargetNo( const StatusBarTargetNo target_no )
{
	unsigned int uiNo = (unsigned int)target_no;
	unsigned int uiValue = m_targetVal;

	switch( uiNo ){
	case STATUS_BAR_TARGET_NONE:			
		uiValue |= VAL_SB_TGT_NONE;			
		break;
	case STATUS_BAR_TARGET_SPECTRUM:		
		uiValue |= VAL_SB_TGT_SPECTRUM;		
		break;
	case STATUS_BAR_TARGET_CHROMATOGRAM:	
		uiValue |= VAL_SB_TGT_CHROMATOGRAM;	
		break;
	case STATUS_BAR_TARGET_DATAMAP:			
		uiValue |= VAL_SB_TGT_DATAMAP;			
		break;
	default:								
		// 値が異常(通常あり得ない)
		uiValue = VAL_SB_TGT_NONE;	
		break;
	}

	m_targetVal = (StatusBarTargetValue)uiValue;
}

// Set Target
void StatusBarInfo::setTargetVal( const StatusBarTargetValue target_val )
{
	unsigned long ulValueOrg = (unsigned long)m_targetVal;
	unsigned long ulValueAdd = (unsigned long)target_val;

	ulValueOrg |= ulValueAdd;

	m_targetVal = (StatusBarTargetValue)ulValueOrg;
}

void StatusBarInfo::setTarget( const char* target )
{
	// array
	static kome::core::ConversionElement< std::string, StatusBarTargetNo > strStatusBarTargetArray[] = {
		{ "none",			STATUS_BAR_TARGET_NONE },
		{ "spectrum",		STATUS_BAR_TARGET_SPECTRUM },
		{ "chromatogram",	STATUS_BAR_TARGET_CHROMATOGRAM },
		{ "datamap",		STATUS_BAR_TARGET_DATAMAP }
	};

	// create string object
	std::string t = tolowercase( target );

	// set target
	StatusBarTargetNo target_no;
	target_no = CONVERT_FROM_ARRAY( t, STATUS_BAR_TARGET_NONE, strStatusBarTargetArray );
	setTargetNo( target_no );

}

kome::plugin::StatusBarInfo::StatusBarTargetValue StatusBarInfo::getTargetVal( void )
{
	return m_targetVal;
}

void StatusBarInfo::setName( const char* name )
{
	m_status_bar_name = name;
	m_status_bar_message = "";	// 表示文字列を初期化
}

const char* StatusBarInfo::getName( void )
{
	return m_status_bar_name.c_str();
}

std::string StatusBarInfo::getNameString( void )
{
	return m_status_bar_name;
}

void StatusBarInfo::setOrder( int order )
{
	m_nOrder = order;
}

int StatusBarInfo::getOrder( void )
{
	return m_nOrder;
}

void StatusBarInfo::setFlgDisp( int flgDisp )
{
	if( strcmp( m_status_bar_name.c_str(), DEFAULT_STATUS_BAR_NAME ) == 0 ){
		// デフォルトのステータスバーは非表示に出来ない
		m_flgDisp = 1;
		return;
	}

	if( flgDisp == 0 ){
		m_flgDisp = 0;
	}else{
		m_flgDisp = 1;
	}
}

int StatusBarInfo::getFlgDisp( void )
{
	return m_flgDisp;
}

// set plug-in
void StatusBarInfo::setPlugin( PluginInfo* plugin ) {

	int iCountOfPluginInfo = m_vpPlugin.size();
	int iIcchi=0;	// 一致したら「1」

	for( int i=0; i<iCountOfPluginInfo; i++ ){
		if( m_vpPlugin[i] == plugin ){
			iIcchi = 1;
			break;	// 既に格納済み
		}
	}
	if( iIcchi == 0 ) {
		// 格納されていない
		m_vpPlugin.push_back( plugin );
	}
}

// get plug-in
PluginInfo* StatusBarInfo::getPlugin( void ) 
{
	return m_vpPlugin[0];
}

// get plug-in
PluginInfo* StatusBarInfo::getPlugin( int iCnt ) 
{
	int iCountOfPluginInfo = m_vpPlugin.size();

	if( iCountOfPluginInfo < iCnt ){
		// err
		return NULL;
	}

	return m_vpPlugin[iCnt];
}

void StatusBarInfo::setMessage( const char* message )
{
	m_status_bar_message = message;
}

const char* StatusBarInfo::getMessage( void )
{
	return m_status_bar_message.c_str();
}

bool StatusBarInfo::operator <(const StatusBarInfo& a)const{
	return ( m_nOrder < a.m_nOrder );
}

bool StatusBarInfo::operator <=(const StatusBarInfo& a)const{
	return ( m_nOrder <= a.m_nOrder );
}

// @date 2011.10.19 <Add> M.Izumi
int StatusBarInfo::getWidth(){
	return m_width;
}

void StatusBarInfo::setWidth( int width ){
	m_width = width;
}

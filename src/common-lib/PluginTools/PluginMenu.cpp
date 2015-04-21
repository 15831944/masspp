/**
 * @file PluginMenu.cpp
 * @brief implements of PluginMenu class
 *
 * @author S.Tanaka
 * @date 2006.08.10
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "PluginMenu.h"

#include "PluginManager.h"
#include "PluginCall.h"
#include "PluginCallTool.h"

#include <set>
#include <algorithm>
#include <boost/bind.hpp>


using namespace kome::plugin;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// static members
std::vector< PluginMenu::GroupOrder > PluginMenu::m_groupOrders;
std::map< std::string, int > PluginMenu::m_toolbarOrderMap;


static kome::core::ConversionElement< std::string, int > g_KeyCodeElements[] = {
	{ "tab", kome::core::key::KEY_TAB },
    { "enter", kome::core::key::KEY_RETURN },
    { "space", kome::core::key::KEY_SPACE },
    { "shift", kome::core::key::KEY_SHIFT },
    { "alt", kome::core::key::KEY_ALT },
    { "ctrl", kome::core::key::KEY_CONTROL },
    { "left", kome::core::key::KEY_LEFT },
    { "up", kome::core::key::KEY_UP },
    { "right", kome::core::key::KEY_RIGHT },
    { "down", kome::core::key::KEY_DOWN },
    { "f1", kome::core::key::KEY_F1 },
    { "f2", kome::core::key::KEY_F2 },
    { "f3", kome::core::key::KEY_F3 },
    { "f4", kome::core::key::KEY_F4 },
    { "f5", kome::core::key::KEY_F5 },
    { "f6", kome::core::key::KEY_F6 },
    { "f7", kome::core::key::KEY_F7 },
    { "f8", kome::core::key::KEY_F8 },
    { "f9", kome::core::key::KEY_F9 },
    { "f10", kome::core::key::KEY_F10 },
    { "f11", kome::core::key::KEY_F11 },
    { "f12", kome::core::key::KEY_F12 },
    { "esc", kome::core::key::KEY_ESCAPE }
};
	

// constructor
PluginMenu::PluginMenu() {
	// initialize
	m_order = -1;
	m_accelerator = 0;
	m_parent = NULL;
	m_target = TARGET_NONE;

	m_groupOrder = -1;

	m_children = NULL;

	m_execCall = NULL;
	m_enableCall = NULL;
	m_checkedCall = NULL;

	// menu ID
	PluginManager& plgMgr = PluginManager::getInstance();
	m_id = plgMgr.issueMenuId();

	m_ptPopupTypes = POPUP_NONE;	// Context Menu用に<popup>タグ内のPropertiesPageTypeを保存
	m_nPopupOrders = DEFAULT_ORDER;

}

// destructor
PluginMenu::~PluginMenu() {
	if( m_children != NULL ) { 
		for( unsigned int i = 0; i < m_children->size(); i++ ) {
			delete (*m_children)[i];
		}
		delete m_children;
		m_children = NULL;
	}
}

// set menu ID
void PluginMenu::setId( const unsigned int id ) {
	m_id = id;
}

// get menu ID
unsigned int PluginMenu::getId() {
	return m_id;
}

// set parent menu
void PluginMenu::setParentMenu( PluginMenu* parent ) {
	m_parent = parent;
}

// get parent menu
PluginMenu* PluginMenu::getParentMenu() {
	return m_parent;
}

// set menu name
void PluginMenu::setName( const char* name ) {
	m_name = std::string( NVL( name, "" ) );
}

// get menu name
const char* PluginMenu::getName() {
	return m_name.c_str();
}

// set menu name including accelerator information
void PluginMenu::setAccName( const char* name ) {
	if( name == NULL ) {
		setAccName( m_name.c_str() );
	}

	m_accName = std::string( name );
}

// get menu name including accelerator information
const char* PluginMenu::getAccName() {
	return m_accName.c_str();
}

// set menu order
void PluginMenu::setOrder( int order ) {
	if( order >= 0 ) {
		m_order = MIN( order, 9 );
	}
}

// get menu order
int PluginMenu::getOrder() {
	if( m_order < 0 ) {
		return DEFAULT_ORDER;
	}
	return m_order;
}

// set accelerator
void PluginMenu::setAccelerator( const char accelerator ) {
	// check parameter
	if( ( accelerator < 'a' || accelerator > 'z' ) 
			&& ( accelerator < 'A' || accelerator > 'Z' ) ) {
		m_accelerator = 0;
	}

	// to upper case
	m_accelerator = toupper( accelerator );
}

// get accelerator
char PluginMenu::getAccelerator() {
	return m_accelerator;
}

// clear shortcut key
void PluginMenu::clearShortcutKey() {
	m_shortcutKeys.clear();
}

// add shortcut key
void PluginMenu::addShortcutKey( const int code ) {
	m_shortcutKeys.push_back( code );
}

// add shortcut key
void PluginMenu::addShortcutKey( const char* key ) {
	// convert
	std::string k = tolowercase( key );
	int code = CONVERT_FROM_ARRAY( k, -1, g_KeyCodeElements );

	if( code < 0 && !k.empty() ) {
		code = (int)k[ 0 ];
	}

	if( code >= 0 ) {
		addShortcutKey( code );
	}
}

// gets the number of shortcut keys
unsigned int PluginMenu::getNumberOfShortcutKeys() {
	return m_shortcutKeys.size();
}

// get shortcut key
int PluginMenu::getShortcutKey( const unsigned int idx ) {
	if( idx >= m_shortcutKeys.size() ) {
		return -1;
	}
	return m_shortcutKeys[ idx ];
}

// get short cut key
std::string PluginMenu::getShortcutKey() {
	std::string str;

	for( unsigned int i = 0; i < m_shortcutKeys.size(); i++ ) {
		const int code = m_shortcutKeys[ i ];

		std::string k;
		k = INVERT_FROM_ARRAY( code, k, g_KeyCodeElements );

		if( k.empty() ) {
			k.push_back( (char)code );
		}

		if( !str.empty() ) {
			str.append( "+" );
		}

		k = touppercase( k.substr( 0, 1 ).c_str() ) + k.substr( 1 );
		str.append( k );
	}

	return str;
}

// set group information
void PluginMenu::setGroup( const char* name, int order ) {
	// set group name
	m_groupName = NVL( name, "" );
	if( m_groupName.empty() ) {
		return;
	}

	// search order information
	if( order < 0 ) {
		return;
	}
	GroupOrder* gOrder = NULL;
	for( unsigned int i = 0; i < m_groupOrders.size() && gOrder == NULL; i++ ) {
		GroupOrder* tmp = &( m_groupOrders[ i ] );

		if( tmp->parent == m_parent && tmp->name.compare( name ) == 0 ) {
			gOrder = tmp;
		}
	}

	// new order information
	if( gOrder == NULL ) {
		m_groupOrders.resize( m_groupOrders.size() + 1 );
		gOrder = &( m_groupOrders.back() );

		gOrder->name = m_groupName;
		gOrder->parent = m_parent;

		if( m_parent != NULL ) {
			for( unsigned int i = 0; i < m_parent->getNumberOfChildren(); i++ ) {
				m_parent->getChildMenu( i )->m_groupOrder = -1;
			}
		}
	}

	// update
	m_groupOrder = MIN( order, 9 );
	gOrder->order = m_groupOrder;	
}

// get group name
const char* PluginMenu::getGroupName() {
	if( m_groupName.empty() ) {
		return NULL;
	}
	return m_groupName.c_str();
}

// get group order
int PluginMenu::getGroupOrder() {
	// check the group
	if( m_groupName.empty() ) {
		return -1;
	}

	// search group order
	for( unsigned int i = 0; i < m_groupOrders.size() && m_groupOrder < 0; i++ ) {
		GroupOrder* tmp = &( m_groupOrders[ i ] );

		if( tmp->parent == m_parent && tmp->name.compare( m_groupName ) == 0 ) {
			m_groupOrder = tmp->order;
		}
	}
	if( m_groupOrder < 0 ) {
		m_groupOrder = DEFAULT_ORDER;
	}

	return m_groupOrder;
}

// set toolbar information
void PluginMenu::setToolbar( const char* name, int order ) {
	// set name
	m_toolbarName = NVL( name, "" );
	if( m_toolbarName.empty() ) {
		return;
	}
	
	// set order
	if( order < 0 ) {
		return;
	}
	m_toolbarOrderMap[ m_toolbarName ] = MIN( order, 9 );
}

// get toolbar name
const char* PluginMenu::getToolbarName() {
	if( m_toolbarName.empty() ) {
		return NULL;
	}
	return m_toolbarName.c_str();
}

// get toolbar order
int PluginMenu::getToolbarOrder() {
	// check name
	if( m_toolbarName.empty() ) {
		return -1;
	}

	// search
	std::map< std::string, int >::iterator it = m_toolbarOrderMap.find( m_toolbarName );
	if( it == m_toolbarOrderMap.end() ) {
		return DEFAULT_ORDER;
	}
	return (*it).second;
}

// set menu description
void PluginMenu::setDescription( const char* desc ) {
	m_description = NVL( desc, "" );
}

// get menu description
const char* PluginMenu::getDescription() {
	return m_description.c_str();
}

// set target
void PluginMenu::setTarget( const MenuTarget target ) {
	m_target = target;
}

void PluginMenu::setTarget( const char* target ) {
	// array
	static kome::core::ConversionElement< std::string, MenuTarget > strMenuTargetArray[] = {
		{ "none",		  TARGET_NONE },
		{ "sample",		  TARGET_SAMPLE },
		{ "group",		  TARGET_GROUP },
		{ "spectrum",	  TARGET_SPECTRUM },
		{ "chromatogram", TARGET_CHROMATOGRAM },
		{ "datamap",	  TARGET_DATAMAP },
		{ "spec_chrom",   TARGET_SPEC_CHROM },
		{ "not_null",	  TARGET_NOT_NULL },
		{ "tab_samples",  TARGET_TAB_SAMPLES },	// @date 2011.10.03 <Add> M.Izumi
		{ "spectra",	  TARGET_SPECTRA },		// @date 2011.10.03 <Add> M.Izumi
		{ "tab_sample",	  TARGET_TAB_SAMPLE }	// @date 2011.11.04 <Add> M.Izumi
	};

	// create string object
	std::string t = tolowercase( target );

	// set target
	m_target = CONVERT_FROM_ARRAY( t, TARGET_NONE, strMenuTargetArray );
}

// get target
PluginMenu::MenuTarget PluginMenu::getTarget() {
	return m_target;
}

// set checked status
void PluginMenu::setCheckedStatus( const char* name, const char* value ) {
	m_checkedStatusName = NVL( name, "" );
	m_checkedStatusValue = NVL( value, "" );
}

// get checked status name
const char* PluginMenu::getCheckedStatusName() {
	return m_checkedStatusName.c_str();
}

// get checked status value
const char* PluginMenu::getCheckedStatusValue() {
	return m_checkedStatusValue.c_str();
}

// set plug-in call (exe)
void PluginMenu::setCallExec( PluginCall* pluginCall ) {
	m_execCall = pluginCall;
}

// get plug-in call (exe)
PluginCall* PluginMenu::getCallExec() {
	return m_execCall;
}

// set plug-in call (enable)
void PluginMenu::setCallEnable( PluginCall* pluginCall ) {
	m_enableCall = pluginCall;
}

// get plug-in call (enable)
PluginCall* PluginMenu::getCallEnable() {
	return m_enableCall;
}

// set plug-in call (checked)
void PluginMenu::setCallChecked( PluginCall* pluginCall ) {
	m_checkedCall = pluginCall;
}

// get plug-in call (enable)
PluginCall* PluginMenu::getCallChecked() {
	return m_checkedCall;
}

// creates new child menu
PluginMenu* PluginMenu::createChildMenu( const char* name ) {
	// check properties
	std::string nameStr( NVL( name, "" ) );
	if( nameStr.empty() ) {
		LOG_WARN( FMT( "Child menu name is illegal." ) );
		return NULL;
	}

	// create array
	if( m_children == NULL ) {
		m_children = new std::vector<PluginMenu*>();
	}
	else {
		// check children
		PluginMenu* child = getChildMenu( name );
		if( child != NULL ) {
			return NULL;
		}
	}

	// create new menu
	PluginMenu* child = new PluginMenu();
	child->setName( name );
	child->setParentMenu( this );

	m_children->push_back( child );

	return child;
}

// gets number of child menus
unsigned int PluginMenu::getNumberOfChildren() {
	if( m_children == NULL ) {
		return 0;
	}
	return m_children->size();
}

// gets child menu
PluginMenu* PluginMenu::getChildMenu( unsigned int index ) {
	// check parameter and member
	if( m_children == NULL || index >= m_children->size() ) {
		return NULL;
	}

	return (*m_children)[index];	
}

// gets child menu
PluginMenu* PluginMenu::getChildMenu( const char* name ) {
	// string
	std::string nameStr( NVL( name, "" ) );

	// check parameter and member
	if( m_children == NULL || nameStr.empty() ) {
		return NULL;
	}

	// search
	for( unsigned int i = 0; i < m_children->size(); i++ ) {
		PluginMenu* child = ( *m_children )[ i ];

		if( nameStr.compare( child->getName() ) == 0 ) {
			return child;
		}
	}
	return NULL;
}

// execute menu command
bool PluginMenu::execute( kome::objects::Variant& acObj ) {
	// get call
	if( m_execCall == NULL ) {
		LOG_ERROR(
			FMT( "The function assigned to this menu item doesn't exist. [%s]", m_name.c_str() )
		);
		return false;
	}

	// execute
	kome::objects::Parameters params;
	PluginCallTool::setActiveObject( params, acObj );

	kome::objects::Variant ret = m_execCall->invoke( &params );

	return ( ret.type == kome::objects::Variant::BOOL && ret.prim.boolVal );
}

// judge whether it possible to execute command
bool PluginMenu::isEnabled( kome::objects::Variant& acObj ) {
	// return value
	bool enabled = false;

	// MS data
	kome::objects::MsDataVariant var( acObj );

	kome::objects::Sample* sample = var.getSample();
	kome::objects::DataGroupNode* group = var.getGroup();
	kome::objects::Spectrum* spec = var.getSpectrum();
	kome::objects::Chromatogram* chrom = var.getChromatogram();
	kome::objects::DataMapInfo* dm = var.getDataMap();
	
	// @date 2011.10.03 <Add> M.Izumi
	// SPEC 82541:Some plug-in dll is loaded even if it	
	kome::objects::ActiveObjectsManager& aoMgr = kome::objects::ActiveObjectsManager::getInstance();
	std::vector< kome::objects::Sample* > samples = aoMgr.getTabSamples();

	std::vector< kome::objects::Spectrum* > spectr = aoMgr.getOpenSpectra();
	bool bActive = false;
	for( unsigned int i=0; i < spectr.size(); i++ ){
		if( spectr[i] == spec ){
			bActive = true;
			break;
		}
	}

	// target
	switch( m_target ) {
	case TARGET_SAMPLE:
		enabled = ( sample != NULL );
		break;
	case TARGET_GROUP:		
		enabled = ( group != NULL && group->getNumberOfSpectra() > 0 );
		break;
	case TARGET_SPECTRUM:
		enabled = ( spec != NULL );
		break;
	case TARGET_CHROMATOGRAM:
		enabled = ( chrom != NULL );
		break;
	case TARGET_DATAMAP:
		enabled = ( dm != NULL );
		break;
	case TARGET_NOT_NULL:
		if( dm != NULL || spec != NULL || chrom != NULL ){
			enabled = true;
		}
		break;
		
	case TARGET_SPEC_CHROM:
		if( spec != NULL || chrom != NULL ) {
			enabled = true;
		}
		break;

	// @date 2011.10.03 <Add> M.Izumi ->
	// SPEC 82541:Some plug-in dll is loaded even if it 
	case TARGET_TAB_SAMPLES:
		if( samples.size() >= 2 ){
			enabled = true;
		}
		break;
	case TARGET_SPECTRA:
		if( spectr.size() >= 2 && bActive ){
			enabled = true;
		}
		break;
	// @date 2011.10.03 <Add> M.Izumi <-
	case TARGET_TAB_SAMPLE:		// @date 2011.11.04 <Add> M.Izumi
		if( samples.size() > 0 ){
			enabled = true;
		}
		break;
	default:
		enabled = true;
		break;
	}

	// get call
	if( enabled && m_enableCall != NULL ) {
		kome::objects::Parameters params;

		PluginCallTool::setActiveObject( params, acObj );

		kome::objects::Variant ret = m_enableCall->invoke( &params );
		if( ret.type == kome::objects::Variant::BOOL ) {
			enabled = ret.prim.boolVal;
		}
	}

	return enabled;
}

// judge whether the menu is checked
bool PluginMenu::isChecked( kome::objects::Variant& acObj ) {
	// check members
	if( m_checkedStatusName.empty() && m_checkedCall == NULL ) {
		return false;
	}

	// status
	kome::objects::StatusManager& statusMgr = kome::objects::StatusManager::getInstance();
	if( !m_checkedStatusName.empty()  ) {
		std::string status = NVL( statusMgr.getValue( m_checkedStatusName.c_str() ), "" );
		std::string status2 = FMT( "!%s", status.c_str() );

		if( status.compare( m_checkedStatusValue ) != 0 && status2.compare( m_checkedStatusValue ) != 0 ) {
			return false;
		}
	}

	// call
	if( m_checkedCall != NULL ) {
		// check status
		bool checked = false;
		for( unsigned int i = 0; i < m_checkedCall->getNumberOfStatuses(); i++ ) {
			std::string statusName = m_checkedCall->getStatusName( i );
			std::string statusValue = m_checkedCall->getStatusValue( i );

			const char* v = statusMgr.getValue( statusName.c_str() );
			std::string val = NVL( v, "" );
			std::string val2 = FMT( "!%s", val.c_str() );

			if( val.compare( statusValue ) == 0 || val2.compare( statusValue ) ) {
				checked = true;
			}
		}

		if( m_checkedCall->getNumberOfStatuses() > 0 && !checked ) {
			return false;
		}

		// call function
		kome::objects::Parameters params;
		PluginCallTool::setActiveObject( params, acObj );

		kome::objects::Variant ret = m_checkedCall->invoke( &params );
		if( ret.type != kome::objects::Variant::BOOL ) {
			ret.type = kome::objects::Variant::BOOL;
			ret.prim.pt = false;
		}
		if( !ret.prim.boolVal ) {
			return false;
		}
	}

	return true;
}

// judge whether the menu is checkable
bool PluginMenu::isCheckable() {
	return ( m_checkedCall != NULL || !m_checkedStatusName.empty() );
}

// sort menu
void PluginMenu::sortMenu() {
	// check members
	if( m_children == NULL ) {
		return;
	}

	// call chilren's sort method. (This method is called recursively.)
	for( unsigned int i = 0; i < m_children->size(); i++ ){
		(*m_children)[i]->sortMenu();
	}

	// sort the array
	std::sort(
		m_children->begin(),
		m_children->end(),
		lessMenu
	);

	// set acc name
	std::set<char> accSet;
	for( unsigned int i = 0; i < m_children->size(); i++ ) {
		PluginMenu* menu = (*m_children)[ i ];
		char uAcc = toupper( menu->getAccelerator() );
		char lAcc = tolower( menu->getAccelerator() );

		std::string name = menu->getName();
		std::string accName = name;

		if( accSet.find( uAcc ) == accSet.end() ) {    // new accelerator
			std::string::size_type pos = name.find( uAcc );
			if( pos == name.npos ) {
				pos = name.find( lAcc );
			}

			if( pos != name.npos ) {
				if( pos == 0 ) {
					accName = "&" + name;
				}
				else {
					accName = name.substr( 0, pos ) + "&" + name.substr( pos );
				}

				accSet.insert( uAcc );
			}
		}

		menu->setAccName( accName.c_str() );
	}
}

// compare to sort
bool PluginMenu::lessMenu( PluginMenu* menu0, PluginMenu* menu1 ) {
	// compare group
	int o0 = menu0->getGroupOrder();
	if( o0 < 0 ) {
		o0 = DEFAULT_ORDER;
	}
	int o1 = menu1->getGroupOrder();
	if( o1 < 0 ) {
		o1 = DEFAULT_ORDER;
	}

	if( o0 != o1 ) {
		return ( o0 < o1 );
	}

	std::string n0 = NVL( menu0->getGroupName(), "" );
	std::string n1 = NVL( menu1->getGroupName(), "" );
	int cmp = n0.compare( n1 );
	if( cmp != 0 ) {
		return ( cmp < 0 );
	}

	// compare menu
	o0 = menu0->getOrder();
	o1 = menu1->getOrder();
	if( o0 != o1 ) {
		return ( o0 < o1 );
	}

	n0 = NVL( menu0->getName(), "" );
	n1 = NVL( menu1->getName(), "" );

	return ( n0.compare( n1 ) < 0 );
}

// add Popup Type Order
void PluginMenu::addPopupTypeOrder( PopupType type, int order ){	// @date 2012/10/30 <Add> OKADA
	m_vectPopupTypes.push_back( type );	// Context Menu用に<popup>タグ内のPropertiesPageTypeを保存
	m_vectPopupOrders.push_back( order );
}

// get Popup Type
PopupType PluginMenu::getPopupType( int index ){
	
	// @date 2013/06/10 <Add> OKADA	------->
	if( index < 0 ){
		// error
		return POPUP_NONE;
	}
	// @date 2013/06/10 <Add> OKADA	<-------

	if( m_vectPopupTypes.size() <= (unsigned int)index ){	// @date 2013/06/10 <Mod> OKADA	
	
		// error
		return POPUP_NONE;
	}

	return m_vectPopupTypes[index];
}

// get Popup Order
int PluginMenu::getPopupOrder( int index ){
	// @date 2013/06/10 <Add> OKADA	------->
	if( index < 0 ){
		// error
		return 5;
	}
	// @date 2013/06/10 <Add> OKADA	<-------

	if( m_vectPopupOrders.size() <= (unsigned int)index ){	// @date 2013/06/10 <Mod> OKADA	
 		// error
 		return 5;
 	}
	return m_vectPopupOrders[index];
}

// get Number Of Popup
int PluginMenu::getNumberOfPopup( void ){

	return m_vectPopupTypes.size();
}


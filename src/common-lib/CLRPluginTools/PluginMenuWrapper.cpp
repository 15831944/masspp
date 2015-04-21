/**
 * @file PluginMenuWrapper.cpp
 * @brief implements of PluginMenuWrapper class
 *
 * @author S.Tanaka
 * @date 2006.10.18
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "PluginMenuWrapper.h"

#include "PluginCallWrapper.h"


using namespace kome::clr;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// menu target conversion array
kome::core::ConversionElement< kome::plugin::PluginMenu::MenuTarget, PluginMenuWrapper::MenuTarget > g_menuTargetConversionArray[] = {
	{ kome::plugin::PluginMenu::TARGET_NONE,		 PluginMenuWrapper::MenuTarget::NONE },
	{ kome::plugin::PluginMenu::TARGET_SAMPLE,	   PluginMenuWrapper::MenuTarget::SAMPLE },
	{ kome::plugin::PluginMenu::TARGET_GROUP,		PluginMenuWrapper::MenuTarget::GROUP },
	{ kome::plugin::PluginMenu::TARGET_SPECTRUM,	 PluginMenuWrapper::MenuTarget::SPECTRUM },
	{ kome::plugin::PluginMenu::TARGET_CHROMATOGRAM, PluginMenuWrapper::MenuTarget::CHROMATOGRAM },
	{ kome::plugin::PluginMenu::TARGET_DATAMAP,	  PluginMenuWrapper::MenuTarget::DATAMAP },
	{ kome::plugin::PluginMenu::TARGET_SPEC_CHROM,   PluginMenuWrapper::MenuTarget::SPEC_CHROM },
	{ kome::plugin::PluginMenu::TARGET_NOT_NULL,	 PluginMenuWrapper::MenuTarget::NOT_NULL }
};

// constructor
PluginMenuWrapper::PluginMenuWrapper( kome::plugin::PluginMenu& menu) 
		: m_menu( menu ) {
}

// destructor
PluginMenuWrapper::~PluginMenuWrapper() {
}

// get plug-in menu object
kome::plugin::PluginMenu& PluginMenuWrapper::getPluginMenu() {
	return m_menu;
}

// get ID
unsigned int PluginMenuWrapper::getId() {
	return m_menu.getId();
}

// set name
void PluginMenuWrapper::setName( System::String^ name ) {
	m_menu.setName( ClrObjectTool::convertString( name ).c_str() );
}

// get name
System::String^ PluginMenuWrapper::getName() {
	return ClrObjectTool::convertString( m_menu.getName(), NULL );
}

// get menu name icluding accelerator information
System::String^ PluginMenuWrapper::getAccName() {
	return ClrObjectTool::convertString( m_menu.getAccName(), NULL );
}

// set order
void PluginMenuWrapper::setOrder( int order ) {
	m_menu.setOrder( order );
}

// get order
int PluginMenuWrapper::getOrder() {
	return m_menu.getOrder();
}

// set accelerator
void PluginMenuWrapper::setAccelerator( char accelerator ) {
	m_menu.setAccelerator( accelerator );
}

// get accelerator
char PluginMenuWrapper::getAccelerator() {
	return m_menu.getAccelerator();
}

// clear shortcut key
void PluginMenuWrapper::clearShortcutKey() {
	m_menu.clearShortcutKey();
}

// add shortcut key
void PluginMenuWrapper::addShortcutKey( int code ) {
	m_menu.addShortcutKey( code );
}

// add shortcut key
void PluginMenuWrapper::addShortcutKey( System::String^ key ) {
	std::string k = ClrObjectTool::convertString( key );
	m_menu.addShortcutKey( k.c_str() );
}

// get number of shortcut keys
unsigned int PluginMenuWrapper::getNumberOfShortcutKeys() {
	return m_menu.getNumberOfShortcutKeys();
}

// get shortcut key
int PluginMenuWrapper::getShortcutKey( unsigned int idx ) {
	return m_menu.getShortcutKey( idx );
}

// get shortcut key
System::String^ PluginMenuWrapper::getShortcutKey() {
	std::string k = m_menu.getShortcutKey();

	return ClrObjectTool::convertString( k.c_str(), NULL );
}

// set group
void PluginMenuWrapper::setGroup( System::String^ name, int order ) {
	m_menu.setGroup(
		ClrObjectTool::convertString( name ).c_str(),
		order
	);
}

// get group name
System::String^ PluginMenuWrapper::getGroupName() {
	return ClrObjectTool::convertString( m_menu.getGroupName(), NULL );
}

// get group order
int PluginMenuWrapper::getGroupOrder() {
	return m_menu.getGroupOrder();
}

// set toolbar
void PluginMenuWrapper::setToolbar( System::String^ name, int order ) {
	m_menu.setToolbar(
		ClrObjectTool::convertString( name ).c_str(),
		order
	);
}

// get toolbar name
System::String^ PluginMenuWrapper::getToolbarName() {
	return ClrObjectTool::convertString( m_menu.getToolbarName(), NULL );
}

// get toolbar order
int PluginMenuWrapper::getToolbarOrder() {
	return m_menu.getToolbarOrder();
}

// set description
void PluginMenuWrapper::setDescription( System::String^ desc ) {
	m_menu.setDescription( ClrObjectTool::convertString( desc ).c_str() );
}

// get description
System::String^ PluginMenuWrapper::getDescription() {
	return ClrObjectTool::convertString( m_menu.getDescription(), NULL );
}

// set menu target
void PluginMenuWrapper::setTarget( MenuTarget target ) {
	// convert target
	kome::plugin::PluginMenu::MenuTarget t = INVERT_FROM_ARRAY( target, kome::plugin::PluginMenu::TARGET_NONE, g_menuTargetConversionArray );

	// set target
	m_menu.setTarget( t );
}

void PluginMenuWrapper::setTarget( System::String^ target ) {
	m_menu.setTarget( ClrObjectTool::convertString( target ).c_str() );
}

// get menu target
PluginMenuWrapper::MenuTarget PluginMenuWrapper::getTarget() {
	// convert target
	MenuTarget target = CONVERT_FROM_ARRAY( m_menu.getTarget(), MenuTarget::NONE, g_menuTargetConversionArray );

	return target;
}

// set checked status
void PluginMenuWrapper::setCheckedStatus( System::String^ name, System::String^ value ) {
	m_menu.setCheckedStatus(
		ClrObjectTool::convertString( name ).c_str(),
		ClrObjectTool::convertString( value ).c_str()
	);
}

// get checked status name
System::String^ PluginMenuWrapper::getCheckedStatusName() {
	return ClrObjectTool::convertString( m_menu.getCheckedStatusName(), NULL );
}

// get checked status value
System::String^ PluginMenuWrapper::getCheckedStatusValue() {
	return ClrObjectTool::convertString( m_menu.getCheckedStatusValue(), NULL );
}

// set execute function 
void PluginMenuWrapper::setCallExec( PluginCallWrapper^ pluginCall ) {
	if( pluginCall == nullptr ) {
		m_menu.setCallExec( NULL );
	}
	else {
		m_menu.setCallExec( &pluginCall->getPluginCall() );
	}
}

// get execute function
PluginCallWrapper^ PluginMenuWrapper::getCallExec() {
	if( m_menu.getCallExec() == NULL ) {
		return nullptr;
	}
	return gcnew PluginCallWrapper( *m_menu.getCallExec() );
}

// set enable function
void PluginMenuWrapper::setCallEnable( PluginCallWrapper^ pluginCall ) {
	if( pluginCall == nullptr ) {
		m_menu.setCallEnable( NULL );
	}
	else {
		m_menu.setCallEnable( &pluginCall->getPluginCall() );
	}
}

// get enable function
PluginCallWrapper^ PluginMenuWrapper::getCallEnable() {
	if( m_menu.getCallEnable() == NULL ) {
		return nullptr;
	}
	return gcnew PluginCallWrapper( *m_menu.getCallEnable() );
}

// set checked function
void PluginMenuWrapper::setCallChecked( PluginCallWrapper^ pluginCall ) {
	if( pluginCall == nullptr ) {
		m_menu.setCallChecked( NULL );
	}
	else {
		m_menu.setCallChecked( &pluginCall->getPluginCall() );
	}
}

// get checked function
PluginCallWrapper^ PluginMenuWrapper::getCallChecked() {
	if( m_menu.getCallChecked() == NULL ) {
		return nullptr;
	}
	return gcnew PluginCallWrapper( *m_menu.getCallChecked() );
}

// get parent menu
PluginMenuWrapper^ PluginMenuWrapper::getParentMenu() {
	kome::plugin::PluginMenu* parent = m_menu.getParentMenu();
	if( parent == NULL ) {
		return nullptr;
	}
	return gcnew PluginMenuWrapper( *parent );
}

// create child menu
PluginMenuWrapper^ PluginMenuWrapper::createChildMenu( System::String^ name ) {
	kome::plugin::PluginMenu* child
		= m_menu.createChildMenu( ClrObjectTool::convertString( name ).c_str() );
	if( child == NULL ) {
		return nullptr;
	}
	return gcnew PluginMenuWrapper( *child );
}

// get the number of child menus
unsigned int PluginMenuWrapper::getNumberOfChildren() {
	return m_menu.getNumberOfChildren();
}

// get child menu
PluginMenuWrapper^ PluginMenuWrapper::getChildMenu( unsigned int index ) {
	kome::plugin::PluginMenu* child = m_menu.getChildMenu( index );
	if( child == NULL ) {
		return nullptr;
	}
	return gcnew PluginMenuWrapper( *child );
}

// get child menu
PluginMenuWrapper^ PluginMenuWrapper::getChildMenu( System::String^ name ) {
	kome::plugin::PluginMenu* child
		= m_menu.getChildMenu( ClrObjectTool::convertString( name ).c_str() );
	if( child == NULL ) {
		return nullptr;
	}
	return gcnew PluginMenuWrapper( *child );
}

// execute
bool PluginMenuWrapper::execute( ClrVariant^ acObj ) {
	kome::objects::Variant var;
	ClrObjectTool::convertVariantFromCLR( &var, acObj );

	return m_menu.execute( var );
}

// enabled
bool PluginMenuWrapper::isEnabled( ClrVariant^ acObj ) {
	kome::objects::Variant var;
	ClrObjectTool::convertVariantFromCLR( &var, acObj );

	return m_menu.isEnabled( var );
}

// checked
bool PluginMenuWrapper::isChecked( ClrVariant^ acObj ) {
	kome::objects::Variant var;
	ClrObjectTool::convertVariantFromCLR( &var, acObj );

	return m_menu.isChecked( var );
}

// checkable
bool PluginMenuWrapper::isCheckable() {
	return m_menu.isCheckable();
}

// sort
void PluginMenuWrapper::sortMenu() {
	m_menu.sortMenu();
}

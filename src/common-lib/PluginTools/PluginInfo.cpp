/**
 * @file PluginInfo.cpp
 * @brief implements of PluginInfo class
 *
 * @author S.Tanaka
 * @date 2006.06.27
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"

#include "PluginInfo.h"

#include "PluginCall.h"
#include "PluginIcon.h"
#include "ParameterSettings.h"
#include "PluginHelp.h"
#include "SettingsPage.h"
#include "StatusBarInfo.h"
#include "PluginComponent.h"	// @Date:2013/08/21	<Add>	A.Ozaki

using namespace kome::plugin;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG




// constructor
PluginInfo::PluginInfo() {
	m_flgActiveEnable=false;	// @date 2013/05/24 <Add> OKADA
	m_mapActiveTable.clear();	// @date 2013/05/24 <Add> OKADA

	m_bHidden = false;			// @Date:2013/07/30	<Add>	A.Ozaki
	m_vpComponent.clear( );		// @Date:2013/07/30	<Add>	A.Ozaki
}

// destructor
PluginInfo::~PluginInfo() {
	clearIcons();
	clearCalls();
	clearOptions();
	clearParameters();
	clearHelps();
	clearStatusBarInfo();	// 2011/01/14 <Add> OKADA

// >>>>>>	@Date:2013/08/21	<Add>	A.Ozaki
//
	std::vector<PluginComponent *>::iterator	it;
	for ( it = m_vpComponent.begin( ) ; it != m_vpComponent.end( ) ; it++ )
	{
		PluginComponent	*pComponent = *it;
		if  ( (PluginComponent *)NULL != pComponent )
		{
			delete	pComponent;
		}
	}
//
// <<<<<<	@Date:2013/08/21	<Add>	A.Ozaki

	m_vpComponent.clear( );		// @Date:2013/07/30	<Add>	A.Ozaki
}

// sets directory path
void PluginInfo::setDir( const char* dir ) {
	m_dir = absolutepath( NVL( dir, "" ) );
}

// gets directory path
const char* PluginInfo::getDir() {
	return m_dir.c_str();
}

// sets plug-in name
void PluginInfo::setName( const char* name ) {
	m_name = std::string( NVL( name, "" ) );
}

// gets plug-in name
const char* PluginInfo::getName() {
	return m_name.c_str();
}

// sets plug-in version
void PluginInfo::setVersion( const char* version ) {
	m_version = std::string( NVL( version, "" ) );
}

// gets plug-in version
const char* PluginInfo::getVersion() {
	return m_version.c_str();
}

// sets plug-in provider name
void PluginInfo::setProvider( const char* provider ) {
	m_provider = std::string( NVL( provider, "" ) );
}

// gets plug-in provider name
const char* PluginInfo::getProvider() {
	return m_provider.c_str();
}

// gets link file path
void PluginInfo::setLink( const char* path ) {
	// create string object
	std::string link = path;

#ifdef __unix
	link = FMT( "lib%s.so", link.c_str() );
#else
	link.append( ".dll" );
#endif		// __unix

	m_link = link;
}

// sets link file path
const char* PluginInfo::getLink() {
	return m_link.c_str();
}

// clear the array of plugi-in icons
void PluginInfo::clearIcons() {
	for( unsigned int i = 0; i < m_icons.size(); i++ ) {
		delete m_icons[ i ];
	}
	m_icons.clear();
}

// create new plug-in icon object
PluginIcon* PluginInfo::createNewIcon() {
	// create object
	PluginIcon* obj = new PluginIcon( this );
	m_icons.push_back( obj );

	// clear map
	m_iconMap.clear();

	return obj;
}

// get the number of plug-in icon objects
unsigned int PluginInfo::getNumberOfIcons() {
	return m_icons.size();
}

// get plug-in icon object
PluginIcon* PluginInfo::getIcon( const unsigned int index ) {
	// check index
	if( index > m_icons.size() ) {
		return NULL;
	}

	return m_icons[ index ];
}

// get plug-in icon
PluginIcon* PluginInfo::getIcon( const char* name ) {
	// create map
	if( m_iconMap.size() == 0 ) {
		createIconMap();
	}

	// create string object
	std::string s( NVL( name, "" ) );

	// search map
	if( m_iconMap.find( s ) == m_iconMap.end() ) {
		LOG_WARN( FMT( "There isn't an icon with the specified name(%s)", s.c_str() ) );
		return NULL;
	}
	return m_iconMap[ s ];
}

// clear the array of  plug-in calls
void PluginInfo::clearCalls() {
	for( unsigned int i = 0; i < m_calls.size(); i++ ) {
		delete m_calls[ i ];
	}
	m_calls.clear();
}

// create new plug-in call object
PluginCall* PluginInfo::createNewCall() {
	// create object
	PluginCall* obj = new PluginCall( this );
	m_calls.push_back( obj );

	// clear map
	m_callMap.clear();

	return obj;
}

// get the number of plug-in call objects
unsigned int PluginInfo::getNumberOfCalls() {

	if( m_flgActiveEnable ){
		return m_mapActiveTable.size();
	}

	return m_calls.size();
}

// get plug-in call object
PluginCall* PluginInfo::getCall( const unsigned int index ) {

	if( m_flgActiveEnable ){
		// index check
		if( m_mapActiveTable.size() <= index ) {
			return NULL;
		}

		unsigned int uIndexOrg = m_mapActiveTable[index];

		// check parameter
		if( uIndexOrg >= m_calls.size() ) {
			return NULL;
		}

		return m_calls[ uIndexOrg ];
	}

	// check parameter
	if( index >= m_calls.size() ) {
		return NULL;
	}

	return m_calls[ index ];
}

// clear options
void PluginInfo::clearOptions() {
	for( unsigned int i = 0; i < m_options.size(); i++ ) {
		delete m_options[ i ];
	}
	m_options.clear();
}

// add options file
bool PluginInfo::addOptionsFile( const char* path ) {
	// check the path
	if( path == NULL ) {
		return false;
	}

	// options object
	ParameterSettings* settings = new ParameterSettings( this, true );
	bool ret = settings->loadSettingsFile( path );

	// add
	if( ret ) {
		m_options.push_back( settings );
	}
	else {
		delete settings;
	}

	return ret;
}

// get the number of options files
unsigned int PluginInfo::getNumberOfOptions() { 
	return m_options.size();
}

// get options
ParameterSettings* PluginInfo::getOptions( const unsigned int index ) {
	if( index >= m_options.size() ) {
		return NULL;
	}
	return m_options[ index ];
}

// clear parameters
void PluginInfo::clearParameters() {
	for( unsigned int i = 0; i < m_parameters.size(); i++ ) {
		delete m_parameters[ i ];
	}
	m_parameters.clear();
}

// add parameters file
bool PluginInfo::addParametersFile( const char* path ) {
	// check the path
	if( path == NULL ) {
		return false;
	}

	// parameters object
	ParameterSettings* parameters = new ParameterSettings( this, false );
	bool ret = parameters->loadSettingsFile( path );

	// add
	if( ret ) {
		m_parameters.push_back( parameters );
	}
	else {
		delete parameters;
	}

	return ret;
}

// get the number of parameters files
unsigned int PluginInfo::getNumberOfParameters() { 
	return m_parameters.size();
}

// get parameters
ParameterSettings* PluginInfo::getParameters( const unsigned int index ) {
	if( index >= m_parameters.size() ) {
		return NULL;
	}
	return m_parameters[ index ];
}

// get plug-in call object
PluginCall* PluginInfo::getCall( const char* name ) {
	// create map
	if( m_callMap.size() == 0 ) {
		createCallMap();
	}

	// create string object
	std::string s( NVL( name, "" ) );

	// search map
	if( m_callMap.find( s ) == m_callMap.end() ) {
		LOG_WARN( FMT( "There isn't a function with the specified name(%s)", s.c_str() ) );
		return NULL;
	}

	return m_callMap[ s ];
}

// clear plug-in helps
void PluginInfo::clearHelps() {
	for( unsigned int i = 0; i < m_helps.size(); i++ ) {
		delete m_helps[ i ];
	}
	m_helps.clear();
}

// create new plug-in help
PluginHelp* PluginInfo::createNewHelp() {
	// create object
	PluginHelp* help = new PluginHelp();
	help->setPlugin( this );

	// add to the array
	m_helps.push_back( help );

	return help;
}

// get the number of plug-in helps
unsigned int PluginInfo::getNumberOfHelps() {
	return m_helps.size();
}

// get plug-in help
PluginHelp* PluginInfo::getHelp( const unsigned int index ) {
	if( index >= m_helps.size() ) {
		return NULL;
	}
	return m_helps[ index ];
}

// get settings page
SettingsPage* PluginInfo::getPage( const char* name ) {
	for( unsigned int i = 0; i < m_parameters.size(); i++ ) {
		SettingsPage* page = m_parameters[ i ]->getPage( name );
		if( page != NULL ) {
			return page;
		}
	}

	for( unsigned int i = 0; i < m_options.size(); i++ ) {
		SettingsPage* page = m_options[ i ]->getPage( name );
		if( page != NULL ) {
			return page;
		}
	}

	return NULL;
}

// create icon map
void PluginInfo::createIconMap() {
	// clear
	m_iconMap.clear();

	// create map
	for( unsigned int i = 0; i < m_icons.size(); i++ ) {
		PluginIcon* icon = m_icons[ i ];
		if( icon != NULL ) {
			std::string name( icon->getName() );

			if( m_iconMap.find( name ) == m_iconMap.end() ) {	// insert object to map
				m_iconMap[ name ] = icon;
			}
			else{	// duplicated name
				LOG_WARN(
					FMT(
						"Plug-in icon name \"%s:%s\" is duplicated.",
						m_name.c_str(),
						name.c_str()
					)
				);
			}
		}
	}
}

// create call map
void PluginInfo::createCallMap() {
	// clear
	m_callMap.clear();

	// create map
	for( unsigned int i = 0; i < m_calls.size(); i++ ) {
		PluginCall* call = m_calls[ i ];
		if( call != NULL ) {
			std::string name( call->getName() );

			if( m_callMap.find( name ) == m_callMap.end() ) {	// insert object to map
				m_callMap[ name ] = call;
			}
			else {	// duplicated name
				LOG_WARN(
					FMT(
						"Plug-in call name \"%s:%s\" is duplicated.",
						m_name.c_str(),
						name.c_str()
					)
				);
			}
		}
	}
}



// Status Bar


// clear plug-in helps
void PluginInfo::clearStatusBarInfo() {
	for( unsigned int i = 0; i < m_statusBarInfo.size(); i++ ) {
		delete m_statusBarInfo[ i ];
	}
	m_statusBarInfo.clear();
}

// create new StatusBarInfo common
StatusBarInfo* PluginInfo::createCommonStatusBarInfo() {
	// create object
	StatusBarInfo* statusBarInfo = new StatusBarInfo();
	statusBarInfo->setPlugin( this );	// 何かに使うかもしれないので呼び出し元プラグインを保存しておく

	statusBarInfo->setName( DEFAULT_STATUS_BAR_NAME );

	// add to the array
	m_statusBarInfo.push_back( statusBarInfo );

	return statusBarInfo;
}

// create new StatusBarInfo
StatusBarInfo* PluginInfo::createNewStatusBarInfo() {
	// create object
	StatusBarInfo* statusBarInfo = new StatusBarInfo();
	statusBarInfo->setPlugin( this );	// 何かに使うかもしれないので呼び出し元プラグインを保存しておく

	// add to the array
	m_statusBarInfo.push_back( statusBarInfo );

	return statusBarInfo;
}

// get the number of plug-in helps
unsigned int PluginInfo::getNumberOfStatusBarInfos() {
	return m_statusBarInfo.size();
}

// get plug-in help
StatusBarInfo* PluginInfo::getStatusBarInfo( const unsigned int index ) {
	if( index >= m_statusBarInfo.size() ) {
		return NULL;
	}
	return m_statusBarInfo[ index ];
}

// add Java class path
void PluginInfo::addJavaClassPath( const char* path ) {
	m_javaClassPathes.push_back( NVL( path, "" ) );
}

// get the number of class pathes
unsigned int PluginInfo::getNumberOfJavaClassPathes() {
	return m_javaClassPathes.size();
}

// get Java class path
const char* PluginInfo::getJavaClassPath( const unsigned int idx ) {
	if( idx >= m_javaClassPathes.size() ) {
		return NULL;
	}

	return m_javaClassPathes[ idx ].c_str();
}

// update active lookup table
void PluginInfo::updateActiveLookupTable( void ){

	m_mapActiveTable.clear();

	unsigned int uNumberOfCalls = m_calls.size();

	unsigned int n=0;
	for( unsigned int i=0; i<uNumberOfCalls; i++ ){
		if( m_calls[i]->isActive() ){
			m_mapActiveTable.insert( std::map< unsigned int, unsigned int >::value_type( n, i ) );
			n++;
		}
		
		else{
			;
		}
	}
	return;
}

// >>>>>>	@Date:2013/07/29	<Add>	A.Ozaki
//

// set hidden
void	PluginInfo::setHidden( const bool bHidden )
{
	m_bHidden = bHidden;

	return;
}

// hidden plugin
bool	PluginInfo::isHidden( void )
{
	return	m_bHidden;
}

// add plugin-call
void	PluginInfo::addCall( PluginCall *pCall )
{
	// add Plugin call
	m_calls.push_back( pCall );

	// clear map
	m_callMap.clear( );

	return;
}

// delete plugin-call
void	PluginInfo::deleteCall( PluginCall *pCall )
{
	std::vector< PluginCall* >::iterator	it = m_calls.begin( );
	
	for ( it = m_calls.begin( ) ; it != m_calls.end() ; ++it )
	{
		if  ( *it == pCall )
		{
			delete *it;
			m_calls.erase( it );

			m_callMap.clear( );
			break;
		}
	}

	return;
}

// add component
void	PluginInfo::addComponent( PluginComponent *pComponent )
{
	m_vpComponent.push_back( pComponent );

	return;
}

//
// <<<<<<	@Date:2013/07/29	<Add>	A.Ozaki

// >>>>>>	@Date:2013/08/21	<Add>	A.Ozaki
//
// get component
PluginComponent	*PluginInfo::getComponent( const char *lpcstrComponentName )
{
	PluginComponent	*pComponent = (PluginComponent *)NULL;

	std::vector<PluginComponent *>::iterator	it;
	for ( it = m_vpComponent.begin( ) ; it != m_vpComponent.end( ) ; it++ )
	{
		pComponent = *it;
		if  ( 0 == strcmp( pComponent->getName( ), lpcstrComponentName ) )
		{
			break;
		}
		pComponent = (PluginComponent *)NULL;
	}

	return	pComponent;
}

// get the number of plugin components
unsigned int PluginInfo::getNumberOfComponents( void )
{
	return	m_vpComponent.size( );
}

//
// get component
PluginComponent	*PluginInfo::getComponent( const unsigned int nIndex )
{
	if  ( m_vpComponent.size( ) <= nIndex )
	{
		return	(PluginComponent *)NULL;
	}

	return	m_vpComponent[nIndex];
}

//
// <<<<<<	@Date:2013/08/21	<Add>	A.Ozaki


// @date 2013/08/22 <Add> OKADA ------->

// set enable component
int PluginInfo::setEnableComponent( const char *lpcstrComponentName )
{
	unsigned int uNumberOfComponents = getNumberOfComponents();

	for( unsigned int i=0; i<uNumberOfComponents; i++ ){
		if( 0 == strcmp( m_vpComponent[i]->getName(), lpcstrComponentName ) ){
			// 一致した
			return setEnableComponent( i );
		}
	}

	return -1;

	// 該当するcomponentのenable/disable設定をenableにする
	// 該当するcomponentが持っているfunctionをすべてactiveにする
}

// set enable component
int PluginInfo::setEnableComponent( const unsigned int nIndex ){

	PluginComponent* pComponent = getComponent( nIndex );
	if( pComponent == NULL ){
		return -1;
	}

	// 該当するcomponentのenable/disable設定をenableにする
	pComponent->setEnable();

	// 該当するcomponentが持っているfunctionをすべてactiveにする
	unsigned int uNumberOfCalls = pComponent->getNumberOfCalls();
	for( unsigned int i=0; i<uNumberOfCalls; i++ ){
		kome::plugin::PluginCall* pCall = pComponent->getCall( i );
		pCall->setActive( true );
	}

	return 0;
}

// reset enable component
int PluginInfo::resetEnableComponent( const char *lpcstrComponentName ){

	unsigned int uNumberOfComponents = getNumberOfComponents();

	for( unsigned int i=0; i<uNumberOfComponents; i++ ){
		if( 0 == strcmp( m_vpComponent[i]->getName(), lpcstrComponentName ) ){
			// 一致した
			return resetEnableComponent( i );
		}
	}

	return -1;
}

// reset enable component
int PluginInfo::resetEnableComponent( const unsigned int nIndex ){

	PluginComponent* pComponent = getComponent( nIndex );
	if( pComponent == NULL ){
		return -1;
	}

	// 該当するcomponentのenable/disable設定をdisableにする
	pComponent->resetEnable();

	// (1)該当するcomponentが持っているfunctionをすべて列挙する
	// (2)1で列挙したfunctionを、ほかのcomponentが持っているか調べる
	// (3)ほかのcomponentでfunctionを持っているなら、
	//    そのcomponentのenable/disable設定の論理和を求める。
	//    すべてdisableなら論理和は0、そうでなければ1。
	// (4)求めた論理和をfunctionにセットする。1ならactive、0ならun-active。
	unsigned int uNumberOfCalls = pComponent->getNumberOfCalls();
	for( unsigned int i=0; i<uNumberOfCalls; i++ ){	// (1)
		kome::plugin::PluginCall* pCall = pComponent->getCall( i );

		bool bActiveTotal = false;
		unsigned int uNumberOfComponents = getNumberOfComponents();
		for( unsigned int uIndexCompo=0; uIndexCompo<uNumberOfComponents; uIndexCompo++ ){
			if( uIndexCompo == nIndex ){
				continue;	// ほかのcomponentについて調べたいので……
			}
			bool bRet = hasFunction( uIndexCompo, pCall );	// (2)ほかのcomponentが持ってる?
			if( bRet ){
				bool bEnable = m_vpComponent[uIndexCompo]->isEnable();
				bActiveTotal = (bActiveTotal || bEnable);	// (3)論理和
			}
		}

		pCall->setActive( bActiveTotal );	// (4)
	}

	return 0;
}

// is enable component
bool PluginInfo::isEnableComponent( const char *lpcstrComponentName )
{
	unsigned int uNumberOfComponents = getNumberOfComponents();

	for( unsigned int i=0; i<uNumberOfComponents; i++ ){
		if( 0 == strcmp( m_vpComponent[i]->getName(), lpcstrComponentName ) ){
			// 一致した
			return isEnableComponent( i );
		}
	}

	return false;
}

// is enable component
bool PluginInfo::isEnableComponent( const unsigned int nIndex )
{
	PluginComponent* pComponent = getComponent( nIndex );
	if( pComponent == NULL ){
		return false;
	}

	return pComponent->isEnable();
}

// 該当するコンポーネントが、指定するfunctionを持っているか調べる
// has function
bool PluginInfo::hasFunction( const char *lpcstrComponentName, kome::plugin::PluginCall* pCall )
{
	unsigned int uNumberOfComponents = getNumberOfComponents();

	for( unsigned int i=0; i<uNumberOfComponents; i++ ){
		if( 0 == strcmp( m_vpComponent[i]->getName(), lpcstrComponentName ) ){
			// 一致した
			return hasFunction( i, pCall );
		}
	}

	return false;
}

// 該当するコンポーネントが、指定するfunctionを持っているか調べる
// has function
bool PluginInfo::hasFunction( const unsigned int nComponentIndex, kome::plugin::PluginCall* pCall )
{
	PluginComponent* pComponent = getComponent( nComponentIndex );
	if( pComponent == NULL ){
		return false;
	}

	unsigned int uNumberOfCalls = pComponent->getNumberOfCalls();
	for( unsigned int i=0; i<uNumberOfCalls; i++ ){
		kome::plugin::PluginCall* pCallTarget = pComponent->getCall( i );
		if( pCallTarget == pCall ){
			return true;
		}
	}

	return false;
}
// @date 2013/08/22 <Add> OKADA <-------

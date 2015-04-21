/**
 * @file PluginManager.cpp
 * @brief implements of PluginManager class
 *
 * @author S.Tanaka
 * @date 2006.06.28
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */



#include "stdafx.h"
#include "PluginManager.h"

#include "PluginInfo.h"
#include "PluginHandler.h"
#include "PluginCall.h"	
#include "ParameterSettings.h"
#include "SettingsValue.h"
#include "SettingsHandler.h"
#include "SettingsPage.h"
#include "PluginFunctionItem.h"

#include <list>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/sax2/SAX2XMLReader.hpp>
#include <xercesc/sax2/XMLReaderFactory.hpp>


using namespace kome::plugin;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



#define START_MENU_ID 10001


// constructor
PluginManager::PluginManager() {
	// initialize
	m_menu = NULL;
	m_pluginParser = NULL;
	m_settingsParser = NULL;


	m_settingsRootChapter.setName( "Settings" );
	m_paramsRootChapter.setName( "Parameters" );
	m_menuId = START_MENU_ID;

	// for Context Menu
	m_pSpectrumItem = NULL;
	m_pSpectrumView = NULL;
	m_pSpectrumSubView = NULL;
	m_pChromatogramItem = NULL;
	m_pChromatogramView = NULL;
	m_pChromatogramSubView = NULL;
	m_pGroupItem = NULL;
	m_pDataMapView = NULL;
	m_pDataMapSubView = NULL;
	
	m_vstrPackages.clear();// @date 2013/02/21 <Add> OKADA

	m_flgActiveEnable=false;	// @date 2013/05/24 <Add> OKADA
	m_mapActiveTable.clear();	// @date 2013/05/24 <Add> OKADA

}

// destructor
PluginManager::~PluginManager() {
	clearPlugins();
	if( m_menu != NULL ) {
		delete m_menu;
	}

	// Context Menu
	if( m_pSpectrumItem != NULL ) {
		delete m_pSpectrumItem;
	}
	m_pSpectrumItem = NULL;
	
	if( m_pSpectrumView != NULL ) {
		delete m_pSpectrumView;
	}
	m_pSpectrumView = NULL;
	
	if( m_pSpectrumSubView != NULL ) {
		delete m_pSpectrumSubView;
	}
	m_pSpectrumSubView = NULL;

	if( m_pChromatogramItem != NULL ) {
		delete m_pChromatogramItem;
	}
	m_pChromatogramItem = NULL;

	if( m_pChromatogramView != NULL ) {
		delete m_pChromatogramView;
	}
	m_pChromatogramView = NULL;

	if( m_pChromatogramSubView != NULL ) {
		delete m_pChromatogramSubView;
	}
	m_pChromatogramSubView = NULL;

	if( m_pGroupItem != NULL ) {
		delete m_pGroupItem;
	}
	m_pGroupItem = NULL;

	if( m_pDataMapView != NULL ) {
		delete m_pDataMapView;
	}
	m_pDataMapView = NULL;

	if( m_pDataMapSubView != NULL ) {
		delete m_pDataMapSubView;
	}
	m_pDataMapSubView = NULL;

}

// set xsd file
void PluginManager::setXMLSchema( const char* pluginXsd, const char* settingsXsd ) {
	m_pluginParser   = kome::xml::XercesTool::getParser( pluginXsd );
	m_settingsParser = kome::xml::XercesTool::getParser( settingsXsd );
}

// get plug-in menu
PluginMenu& PluginManager::getMenu() {
	if( m_menu == NULL ) {
		m_menu = new PluginMenu();
	}

	return *m_menu;
}

// get plug-in menu
PluginMenu& PluginManager::getSpectrumItemMenu(){
	if( m_pSpectrumItem == NULL ) {
		m_pSpectrumItem = new PluginMenu();
	}

	return *m_pSpectrumItem;
}

// get plug-in menu
PluginMenu& PluginManager::getSpectrumViewMenu(){
	if( m_pSpectrumView == NULL ) {
		m_pSpectrumView = new PluginMenu();
	}

	return *m_pSpectrumView;
}

// get plug-in menu
PluginMenu& PluginManager::getSpectrumSubViewMenu(){
	if( m_pSpectrumSubView == NULL ) {
		m_pSpectrumSubView = new PluginMenu();
	}

	return *m_pSpectrumSubView;
}

// get plug-in menu
PluginMenu& PluginManager::getChromatogramItemMenu(){
	if( m_pChromatogramItem == NULL ) {
		m_pChromatogramItem = new PluginMenu();
	}

	return *m_pChromatogramItem;
}

// get plug-in menu
PluginMenu& PluginManager::getChromatogramViewMenu(){
	if( m_pChromatogramView == NULL ) {
		m_pChromatogramView = new PluginMenu();
	}

	return *m_pChromatogramView;
}

// get plug-in menu
PluginMenu& PluginManager::getChromatogramSubViewMenu(){
	if( m_pChromatogramSubView == NULL ) {
		m_pChromatogramSubView = new PluginMenu();
	}

	return *m_pChromatogramSubView;
}

// get plug-in menu
PluginMenu& PluginManager::getGroupItemMenu(){
	if( m_pGroupItem == NULL ) {
		m_pGroupItem = new PluginMenu();
	}

	return *m_pGroupItem;
}

// get plug-in menu
PluginMenu& PluginManager::getDataMapViewMenu(){
	if( m_pDataMapView == NULL ) {
		m_pDataMapView = new PluginMenu();
	}

	return *m_pDataMapView;
}

// get plug-in menu
PluginMenu& PluginManager::getDataMapSubViewMenu(){
	if( m_pDataMapSubView == NULL ) {
		m_pDataMapSubView = new PluginMenu();
	}

	return *m_pDataMapSubView;
}

// get settings root chapter
SettingsChapter& PluginManager::getSettingsRootChapter() {
	return m_settingsRootChapter;
}

// get parameters root chapter
SettingsChapter& PluginManager::getParametersRootChapter() {
	return m_paramsRootChapter;
}

// load plug-in file
bool PluginManager::loadPluginFile( const char* xmlPath ) {
	// check the parser
	if( m_pluginParser == NULL ) {
		LOG_WARN( FMT( "The plug-in schema file is not loaded yet." ) );
		return false;
	}

	// log
	LOG_DEBUG( FMT( "Loading plug-in file... [%s]", NVL( xmlPath, "" ) ) );

	// file check
	if( !checkfile( xmlPath ) ) {
		LOG_ERROR( FMT( "Invalid file [%s]", NVL( xmlPath, "" ) ) );
		return false;
	}

	// load xml
	bool result = true;
	PluginInfo* plugin = new PluginInfo();
	plugin->setDir( getdir( xmlPath ).c_str() );

	try{
		// create handler
		PluginHandler handler( *plugin, getMenu() );
		m_pluginParser->setContentHandler( &handler );
		m_pluginParser->setErrorHandler( &handler );

		// parse
		m_pluginParser->parse( xmlPath );
		if( handler.isError() ) {
			LOG_ERROR(
				FMT(
					"Failed to load the plug-in file. [%s]",
					xmlPath
				)
			);
			result = false;
		}
	}
	catch( const xercesc::XMLException& e ) {
		std::string msg = kome::xml::XercesTool::transcode( e.getMessage() );
		LOG_ERROR(
			FMT(
				"Failed to load the plug-in file. [%s] (%s)",
				xmlPath,
				msg.c_str()
			)
		);
		result = false;
	}
	catch( const xercesc::SAXException& e ) {
		std::string msg = kome::xml::XercesTool::transcode( e.getMessage() );
		LOG_ERROR(
			FMT(
				"Failed to load the plug-in file. [%s] (%s)",
				xmlPath,
				msg.c_str()
			)
		);
		result = false;
	}
	catch( ... ) {
		LOG_ERROR(
			FMT(
				"Failed to load the plug-in file. [%s]",
				xmlPath
			)
		);
		result = false;
	}

	// add plug-in information to the array
	m_plugins.push_back( plugin );

	return result;
}

// load settings file
bool PluginManager::loadSettingsFile( const char* xmlPath, ParameterSettings& settings ) {
	// check the parser
	if( m_pluginParser == NULL ) {
		LOG_WARN( FMT( "The plug-in schema file is not loaded yet." ) );
		return false;
	}

	// log
	LOG_DEBUG( FMT( "Loading plug-in file... [%s]", NVL( xmlPath, "" ) ) );

	// file check
	if( !checkfile( xmlPath ) ) {
		LOG_ERROR( FMT( "Invalid file [%s]", NVL( xmlPath, "" ) ) );
		return false;
	}

	// load xml	
	bool result = true;

	try{
		// create handler
		SettingsHandler handler( settings );
		m_settingsParser->setContentHandler( &handler );
		m_settingsParser->setErrorHandler( &handler );

		// parse
		m_settingsParser->parse( xmlPath );

	}
	catch( const xercesc::XMLException& e ) {
		std::string msg = kome::xml::XercesTool::transcode( e.getMessage() );
		LOG_ERROR(
			FMT(
				"Failed to load the settings file. [%s] (%s)",
				xmlPath,
				msg.c_str()
			)
		);
		result = false;
	}
	catch( const xercesc::SAXException& e ) {
		std::string msg = kome::xml::XercesTool::transcode( e.getMessage() );
		LOG_ERROR(
			FMT(
				"Failed to load the settings file. [%s] (%s)",
				xmlPath,
				msg.c_str()
			)
		);
		result = false;
	}
	catch( ... ) {
		LOG_ERROR(
			FMT(
				"Failed to load the settings file. [%s]",
				xmlPath
			)
		);
		result = false;
	}

	return result;
}

// load paths
bool PluginManager::loadPath( const char* path, boost::function< bool ( const char* ) > filterFun, const bool createMap ) {
	// result variable
	bool result = false;

	// check the filter
	if( !filterFun( path ) ) {
		return result;
	}

	// load
	if( isdirectory( path ) ) { // directory
		// open directory
		LOG_TRACE( FMT( "Open directory. [%s]", path ) );

		boost::filesystem::path p( path, boost::filesystem::native );
		boost::filesystem::directory_iterator end;
		for( boost::filesystem::directory_iterator it( p );
				it != end; it++ ) {
			std::string fileName = (*it).path().leaf().string();
			std::string child = getpath( path, fileName.c_str() );
			bool tmpResult = loadPath( child.c_str(), filterFun, false );
			result = result || tmpResult;
		}
	}
	else {  // file
		result = loadPluginFile( path );
	}

	// create map
	if( createMap ) {
		createPluginMap();
		createCallMap();
	}

	return result;
}

// clear plug-in array
void PluginManager::clearPlugins() {
	// delete function item map
	for( std::map< std::string, std::vector< PluginFunctionItem* > >::iterator it = m_itemMap.begin();
			it != m_itemMap.end(); it++ ) {
		std::vector< PluginFunctionItem* >& items = (*it).second;
		for( unsigned int i = 0; i < items.size(); i++ ) {
			delete items[ i ];
		}
		items.clear();
	}
	m_itemMap.clear();

	// delete plug-in infomation
	for( unsigned int i = 0; i < m_plugins.size(); i++ ) {
		delete m_plugins[i];
	}
	m_plugins.clear();
	m_pluginMap.clear();
	m_callMap.clear();

	// close all link
	while( m_handleMap.size() > 0 ) {
		closeLink( ( *( m_handleMap.begin() ) ).first.c_str() );
	}
}

// create plug-in map
void PluginManager::createPluginMap() {
	// clear
	m_pluginMap.clear();

	// create map
	for( unsigned int i = 0; i < m_plugins.size(); i++ ) {
		PluginInfo* plugin = m_plugins[ i ];
		if( plugin != NULL ) {
			std::string name = std::string( plugin->getName() );
			if( m_pluginMap.find( name ) == m_pluginMap.end() ) {	// set object to map
				m_pluginMap[ name ] = plugin;
			}
			else {	// name is duplicated
				LOG_WARN( FMT( "Plug-in name \"%s\" is duplicated.", name.c_str() ) );
			}
		}
	}
}

// create plug-in call map
void PluginManager::createCallMap() {
	// clear
	m_callMap.clear();

	// classify call by the type
	for( unsigned int i = 0; i < m_plugins.size(); i++ ) {
		PluginInfo* plugin = m_plugins[ i ];

		for( unsigned int j = 0; j < plugin->getNumberOfCalls(); j++ ) {
			PluginCall* call = plugin->getCall( j );

			m_callMap[ call->getType() ].push_back( call );
		}
	}

	// sort
	for( std::map< std::string, std::vector< PluginCall* > >::iterator it = m_callMap.begin();
			it != m_callMap.end(); it++ ) {
		std::sort( (*it).second.begin(), (*it).second.end(), lessCall );
	}
}

// create plug-in function item map
void PluginManager::createFunctionItemMap( const char* type ) {
	// check the map
	if( m_itemMap.find( type ) != m_itemMap.end() ) {
		return;
	}

	// create array
	std::vector< PluginFunctionItem* >& items = m_itemMap[ type ];
	forEachCalls(
		type,
		boost::bind( addFunctionItem, _1, &items ),
		false
	);
}

// issues menu ID
unsigned int PluginManager::issueMenuId() {
	unsigned int id = m_menuId;
	m_menuId++;
	return id;
}

// get link
void* PluginManager::getLink( const char* path ) {
	// create path string
	std::string p = absolutepath( path );

	// check map
	if( m_handleMap.find( p ) == m_handleMap.end() ) {
		return NULL;
	}

	return m_handleMap[ p ];
}

// open dynamic link library
void* PluginManager::openLink( const char* path ) {
	// get handle
	std::string p = absolutepath( path );
	DL_HANDLE handle = (DL_HANDLE)getLink( p.c_str() );
	if( handle != NULL ) {
		LOG_TRACE( FMT( "The link is already opened. [%s]", p.c_str() ) );
		return handle;
	}

	// check file path
	if( !checkfile( p.c_str() ) ) {
		LOG_ERROR( FMT( "Specified path is illegal. [%s]", p.c_str() ) );
		return NULL;
	}

	// open
	LOG_INFO( FMT( "Loading library. [%s]", p.c_str() ) );

#ifdef _WIN32
	handle = LoadLibraryEx( p.c_str(), NULL, LOAD_WITH_ALTERED_SEARCH_PATH );
#else
	handle = dlopen( p.c_str(), RTLD_LAZY );
#endif	// _WIN32

	if( handle == NULL ) {
#ifdef _WIN32
		// error code
		int err = GetLastError();

		// error message
		LPVOID errBuf;
		FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			err,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(char*)( &errBuf ),
			0,
			NULL
		);

		// log
		LOG_ERROR(
			FMT(
				"Failed to load the library. [%s]  Error(code=%x): %s",
				p.c_str(),
				err,
				errBuf
			)
		);

		// free buffer
		LocalFree( errBuf );
#else
		LOG_ERROR(
			FMT(
				"Failed to load the library. [%s] (%s)",
				p.c_str(),
				dlerror()
			)
		);
#endif	//_WIN32

		return NULL;
	}

	// set to map
	m_handleMap[ p ] = handle;

	return handle;
}

// close dynamic link library
void PluginManager::closeLink( const char* path ) {
	// create path string
	std::string p = absolutepath( path );

	// check map
	if( m_handleMap.find( p ) == m_handleMap.end() ) {
		LOG_WARN( FMT( "The link has not been opened yet. [%s]", p.c_str() ) );
		return;
	}

	// close
	DL_HANDLE handle = (DL_HANDLE)m_handleMap[p];

#ifdef _WIN32
	FreeLibrary( handle );
#else
	dlclose( handle );
#endif	// _WIN32

	// remove from map
	m_handleMap.erase( p );
}

// get the number of plug-in informations
unsigned int PluginManager::getNumberOfPlugins() {

	if( m_flgActiveEnable ){
		return m_mapActiveTable.size();
	}

	return m_plugins.size();
}

// get plug-in information
PluginInfo* PluginManager::getPlugin( const unsigned int index ) {

	if( m_flgActiveEnable ){
		// index check
		if( m_mapActiveTable.size() <= index ) {
			return NULL;
		}

		unsigned int uIndexOrg = m_mapActiveTable[index];

		// check parameter
		if( uIndexOrg >= m_plugins.size() ) {
			return NULL;
		}

		return m_plugins[ uIndexOrg ];
	}

	// check parameter
	if( index >= m_plugins.size() ) {
		return NULL;
	}

	return m_plugins[index];
}

// get functions
void PluginManager::forEachCalls(
		const char* type,
		boost::function< void ( PluginCall* ) > eachFun,
		bool checkStatus
) {
	// to upper case
	std::string typeStr = touppercase( type );

	// check map
	if( m_callMap.find( typeStr ) == m_callMap.end() ) {
		return;
	}
	std::vector< PluginCall* >& calls = m_callMap[ typeStr ];

	// push functions
	for( unsigned int i = 0; i < calls.size(); i++ ) {
		if( !checkStatus || calls[ i ]->checkStatus() ) {   // check status
			eachFun( calls[ i ] );
		}
	}
}

// get the number of function items
unsigned int PluginManager::getNumberOfFunctionItems( const char* type ) {
	// to uppper case
	std::string t = touppercase( type );

	// create array
	createFunctionItemMap( t.c_str() );

	// get array
	std::vector< PluginFunctionItem* >& items = m_itemMap[ t ];
	return items.size();
}

// get plug-in function item
PluginFunctionItem* PluginManager::getFunctionItem( const char* type, const unsigned int index ) {
	// to uppper case
	std::string t = touppercase( type );

	// check
	if( t.empty() ) {
		return NULL;
	}

	// create array
	createFunctionItemMap( t.c_str() );

	// get array
	std::vector< PluginFunctionItem* >& items = m_itemMap[ t ];
	if( index >= items.size() ) {
		return NULL;
	}
	return items[ index ];
}

// get plug-in function item
PluginFunctionItem* PluginManager::getFunctionItem( const char* type, const char* name, kome::objects::SettingParameterValues* settings ) {
	// to uppper case
	std::string t = touppercase( type );

	// check
	if( t.empty() || name == NULL ) {
		return NULL;
	}

	// create array
	createFunctionItemMap( t.c_str() );

	// get array
	std::vector< PluginFunctionItem* >& items = m_itemMap[ t ];

	// name and parameters
	std::string n = NVL( name, "" );
	std::string p;
	std::string::size_type pos = n.find( "[" );
	if( pos != n.npos ) {
		p = n.substr( pos );
		n = n.substr( 0, pos );
	}

	// search
	PluginFunctionItem* item = NULL;
	for( unsigned int i = 0; i < items.size() && item == NULL; i++ ) {
		PluginFunctionItem* tmp = items[ i ];
		if( n.compare( tmp->getShortName() ) == 0 ) {
			item = tmp;
		}
	}

	// parameters
	if( item != NULL && !p.empty() && settings != NULL ) {
		SettingsPage* page = item->getSettingsPage();
		if( page != NULL ) {
			page->setParameterString( *settings, p.c_str() );
		}
	}

	return item;
}

// call all functions of the specified type
void PluginManager::callAllFunctions(
		kome::objects::Parameters* params,
		const char* type,
		const bool checkStatus
) {
	// to upper case
	std::string typeStr = touppercase( type );

	// check map
	if( m_callMap.find( typeStr ) == m_callMap.end() ) {
		return;
	}
	std::vector< PluginCall* >& calls = m_callMap[ typeStr ];

	// call functions
	for( unsigned int i = 0; i < calls.size(); i++ ) {
		// plug-in function
		PluginCall* call = calls[ i ];

		// check status
		if( !checkStatus || call->checkStatus() ) {
			
			// Plugin Manager		// @date 2013/04/17 <Add> OKADA ------->
			if( call->isActive() ){
				// OK
			}else{
				LOG_TRACE_CODE( FMT( "Function call was not invoked. FuncName = '%s' is not active.", call->getName() ), ERR_OTHER );
				continue;	// pass
			}						// @date 2013/04/17 <Add> OKADA <-------
			
			// execute
			calls[ i ]->invoke( params );
		}
	}
}

// get plugi-n information
PluginInfo* PluginManager::getPlugin( const char* name ) {
	// create string object
	std::string s( NVL( name, "" ) );

	// search map
	if( m_pluginMap.find( s ) == m_pluginMap.end() ) {
		LOG_WARN( FMT( "There isn't a plug-in with the specified name(%s)", s.c_str() ) );
		return NULL;
	}

	return m_pluginMap[ s ];
}

// add plug-in function item
void PluginManager::addFunctionItem( PluginCall* call, std::vector< PluginFunctionItem* >* items ) {
	// create item object
	PluginFunctionItem* item = new PluginFunctionItem();
	item->setInfo( call );

	items->push_back( item );
}

// compare to sort
bool PluginManager::lessCall( PluginCall* call0, PluginCall* call1 ) {
	return ( call0->getOrder() < call1->getOrder() );
}

// add Package
void PluginManager::addPackage( const char * pszPackage ){
	if( pszPackage == NULL ){
		return;
	}

	std::string strPackage = pszPackage;

	std::vector< std::string >::iterator cIter = find( m_vstrPackages.begin(),m_vstrPackages.end() , strPackage );

	if( cIter != m_vstrPackages.end() ){
		// m_vstrPackagesÇÕë∂ç›ÇµÇƒÇ‹Ç∑
	}else{
		// ñ≥Ç©Ç¡ÇΩÇÃÇ≈í«â¡
		m_vstrPackages.push_back( strPackage );
	}
}

// get number of Packages
int PluginManager::getNumberOfPackages( void ){
	int nNumberOfPackages = m_vstrPackages.size();
	
	return nNumberOfPackages;
}

// get Package
char * PluginManager::getPackage( int nIndex ){
	if( getNumberOfPackages() <= nIndex ){
		return NULL;
	}

	return (char *)m_vstrPackages[nIndex].c_str();
}

// update Plugins
void PluginManager::updatePlugins( void ){

}

// @date 2013/05/24 <Add> OKADA ------->
// set active enable
void PluginManager::setActiveEnable( bool flgActiveEnable ){	
	m_flgActiveEnable = flgActiveEnable;

	unsigned int uNumberOfPluginsFull = m_plugins.size();
	for( unsigned int i=0; i<uNumberOfPluginsFull; i++ ){
		m_plugins[i]->setActiveEnable( flgActiveEnable ); 
	}

}	

// set active enable
bool PluginManager::getActiveEnable( void ){	
	return m_flgActiveEnable;
}	

// update active lookup table
void PluginManager::updateActiveLookupTable( void ){

	m_mapActiveTable.clear();

	unsigned int uNumberOfPluginsFull = m_plugins.size();

	unsigned int n=0;
	for( unsigned int i=0; i<uNumberOfPluginsFull; i++ ){
		
		m_plugins[i]->updateActiveLookupTable();
		unsigned int uNumberOfCalls = m_plugins[i]->getNumberOfCalls();

		if( 0 < uNumberOfCalls ){
			// m_mapActiveTableÇ…ÅAactiveÇ»î‘çÜÅ®FullÇÃî‘çÜ ÇÃÉeÅ[ÉuÉãÇçÏê¨
			m_mapActiveTable.insert( std::map< unsigned int, unsigned int >::value_type( n, i ) );
			n++;
		}else{
			;
		}
	}

	//////////
	bool oldval = m_flgActiveEnable;

	m_flgActiveEnable = true;

	// create call map - createCallMap()Ç∆ìØìôÇÃèàóù
	{
		// clear
		m_callMap.clear();

		// classify call by the type
		for( unsigned int i = 0; i < m_mapActiveTable.size(); i++ ) 
		{
			PluginInfo* plugin = getPlugin( i );
			
			for( unsigned int j = 0; j < plugin->getNumberOfCalls(); j++ ) {
				PluginCall* call = plugin->getCall( j );

				m_callMap[ call->getType() ].push_back( call );
			}
		}

		// sort
		for( std::map< std::string, std::vector< PluginCall* > >::iterator it = m_callMap.begin();
				it != m_callMap.end(); it++ ) {
			std::sort( (*it).second.begin(), (*it).second.end(), lessCall );
		}
	}
	
	// @date 2013/06/13 <ADD> OKADA ------->
	// delete function item map
	for( std::map< std::string, std::vector< PluginFunctionItem* > >::iterator it = m_itemMap.begin();
			it != m_itemMap.end(); it++ ) {
		std::vector< PluginFunctionItem* >& items = (*it).second;
		for( unsigned int i = 0; i < items.size(); i++ ) {
			delete items[ i ];
		}
		items.clear();
	}
	// @date 2013/06/13 <ADD> OKADA <-------
	
	m_itemMap.clear();

	m_flgActiveEnable = oldval;
	//////////

	return;
}
// @date 2013/05/24 <Add> OKADA <-------

// >>>>>	@Date:2013/09/11	<Add>	A.Ozaki
//
// add active lookup table
void	PluginManager::addActiveLookupTable( PluginInfo *pInfo )
{
	if  ( (PluginInfo *)NULL == pInfo )
	{
		return;
	}

	// PluginInfoì‡ÇçXêV
	pInfo->updateActiveLookupTable( );

	// m_pluginMapÇçXêV
	std::string	strName = std::string( pInfo->getName( ) );
	if  ( m_pluginMap.find( strName ) == m_pluginMap.end( ) )
	{
		m_pluginMap[ strName ] = pInfo;
	}

	if  ( 0 >= pInfo->getNumberOfCalls( ) )
	{
		return;
	}

	// m_pluginsÇ…ìoò^Ç≥ÇÍÇƒÇ¢ÇÈà íuÇãÅÇﬂÇ‹Ç∑
	//
	unsigned int	unFound = m_plugins.size( );
	for ( unsigned int unIndex = 0 ; unIndex < m_plugins.size( ) ; unIndex++ )
	{
		if  ( pInfo == m_plugins[unIndex] )
		{
			unFound = unIndex;
			break;
		}
	}

	if  ( m_mapActiveTable.size( ) <= unFound )
	{
		// m_mapActiveTableÇ…ñ¢ìoò^Ç»ÇÃÇ≈ÅAí«â¡ÇµÇ‹Ç∑
		m_mapActiveTable.insert( std::map< unsigned int, unsigned int >::value_type( m_mapActiveTable.size( ), unFound ) );
	}

	// m_callMapÇ…í«â¡
	//
	for ( unsigned int unIndex = 0 ; unIndex < pInfo->getNumberOfCalls( ) ; unIndex++ )
	{
		PluginCall*	pCall = pInfo->getCall( unIndex );

		
		if  ( m_callMap.find( pCall->getType( ) ) == m_callMap.end( ) )
		{
			// ñ¢ìoò^Ç»ÇÃÇ≈ÅAìoò^ÇµÇ‹Ç∑
			m_callMap[ pCall->getType( ) ].push_back( pCall );
		}
		else
		{
			std::vector< PluginCall* >&	insCalls = m_callMap[ pCall->getType( ) ];

			for ( unsigned int unCalls = 0 ; unCalls < insCalls.size( ); unCalls++ )
			{
				std::string	strType = insCalls[unCalls]->getType( );
				if  ( 0 != strType.compare( pCall->getType( ) ) )
				{
					// ñ¢ìoò^Ç»ÇÃÇ≈ÅAìoò^ÇµÇ‹Ç∑
					m_callMap[ pCall->getType( ) ].push_back( pCall );
				}
			}
		}
	}

	return;
}
//
// <<<<<<	@Date:2013/09/11	<Add>	A.Ozaki

// get refer to PluginManager object
PluginManager& PluginManager::getInstance() {
	// create the only object
	static PluginManager plgMgr;

	return plgMgr;
}

// >>>>>>	@Date:2013/07/29	<Add>	A.Ozaki
//
PluginInfo	*PluginManager::getHiddenPlugin( const char *lpszName )
{
	// @date 2013.08.22 <Add> M.Izumi ->
	unsigned int inum = m_plugins.size();
	for( unsigned int i=0; i < inum; i++ )
	{		
		if( strcmp( m_plugins[i]->getName(), lpszName ) == 0 )
		{
			return m_plugins[i];
		}
	}
	// @date 2013.08.22 <Add> M.Izumi <-

	PluginInfo	*pInfo = new PluginInfo( );

	if  ( (PluginInfo *)NULL == pInfo )
	{
		return	pInfo;
	}

	// set hidden type
	pInfo->setHidden( true );

	// set name
	pInfo->setName( lpszName );

	// add plug-in information to the array
	m_plugins.push_back( pInfo );

	return	pInfo;
}

//
// <<<<<<	@Date:2013/07/29	<Add>	A.Ozaki

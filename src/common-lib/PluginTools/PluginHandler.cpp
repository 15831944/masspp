/**
 * @file PluginHandler.cpp
 * @brief implements of PluginHandler class
 *
 * @author S.Tanaka
 * @date 2006.08.10
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "PluginHandler.h"

#include "PluginInfo.h"
#include "PluginIcon.h"
#include "PluginMenu.h"
#include "PluginCall.h"
#include "PluginHelp.h"
#include "PluginComponent.h"	// @Date:2013/08/21	<Add>	A.Ozaki

#include "StatusBarInfo.h"
#include "PluginManager.h"

#include <list>
#include <boost/bind.hpp>

using namespace kome::plugin;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// define
#define PLUGIN_NAME_TAG				 "name"
#define PLUGIN_VERSION_TAG			 "version"
#define PLUGIN_PROVIDER_TAG			 "provider"
#define PLUGIN_LINK_TAG				 "link"
#define ICON_TAG					 "icon"
#define ICON_NAME_TAG				 "icon-name"
#define ICON_FILE_TAG				 "icon-file"
#define MENU_TAG					 "menu"
#define MENU_ITEM_TAG				 "menu-item"
#define MENU_ITEM_NAME_TAG			 "menu-name"
#define MENU_GROUP_TAG				 "menu-group"

#define MENU_ITEM_POPUP_TAG		     "popup"	// for Context Menu	// @date 2012/10/24 <Add> OKADA
#define MENU_POPUP_TYPE_TAG		     "type"	    // for Context Menu	// @date 2012/10/24 <Add> OKADA
#define MENU_POPUP_ORDER_TAG	     "order"	// for Context Menu	// @date 2012/10/24 <Add> OKADA

#define MENU_POPUP_TYPE_ATTR_SPEC_ITEM		"spec_item"		// スペクトルアイコン (Sample Tree View)		// @date 2012/10/30 <Add> OKADA
#define MENU_POPUP_TYPE_ATTR_CHROM_ITEM		"chrom_item"	// クロマトグラムアイコン (Sample Tree View)	// @date 2012/10/30 <Add> OKADA
#define MENU_POPUP_TYPE_ATTR_GROUP_ITEM		"group_item"	// グループアイコン (Sample Tree View)			// @date 2012/10/30 <Add> OKADA

#define MENU_POPUP_TYPE_ATTR_SPEC_VIEW			"spec_view"			// スペクトル波形ウィンドウ						// @date 2012/10/30 <Add> OKADA
#define MENU_POPUP_TYPE_ATTR_SPEC_SUB_VIEW		"spec_sub_view"		// スペクトル波形ウィンドウ(Sub)				// @date 2012/11/08 <Add> OKADA
#define MENU_POPUP_TYPE_ATTR_CHROM_VIEW			"chrom_view"		// クロマトグラム波形ウィンドウ					// @date 2012/10/30 <Add> OKADA
#define MENU_POPUP_TYPE_ATTR_CHROM_SUB_VIEW		"chrom_sub_view"	// クロマトグラム波形ウィンドウ(Sub)			// @date 2012/11/08 <Add> OKADA
#define MENU_POPUP_TYPE_ATTR_DATAMAP_VIEW		"datamap_view"		// ヒートマップ or 3D View						// @date 2012/10/30 <Add> OKADA
#define MENU_POPUP_TYPE_ATTR_DATAMAP_SUB_VIEW	"datamap_sub_view"	// ヒートマップ or 3D View(Sub)					// @date 2012/11/08 <Add> OKADA

#define TOOLBAR_TAG					 "toolbar"
#define MENU_DESCRIPTION_TAG		 "menu-description"
#define SHORTCUT_KEY_TAG             "shortcut-key"
#define STATUS_TAG					 "status"
#define STATUS_NAME_TAG				 "status-name"
#define STATUS_VALUE_TAG			 "status-value"
#define JAVA_TAG                     "java"
#define JAVA_CLASSPATH_TAG           "classpath"
#define CALL_TAG					 "call"
#define CALL_NAME_TAG				 "call-name"
#define CALL_FUNCTION_TAG			 "call-function"
#define CALL_METHOD_TAG				 "call-method"
#define CALL_CLASS_NAME_TAG			 "class-name"
#define CALL_METHOD_NAME_TAG		 "method-name"
#define CALL_PARAM_KEY_TAG			 "param-name"
#define CALL_PARAM_VALUE_TAG		 "param-value"
#define CALL_DESCRIPTION_TAG		 "call-description"
#define OPTIONS_FILE_TAG			 "settings-file"
#define PARAMETERS_FILE_TAG			 "parameters-file"
#define HELP_FILE_TAG				 "help-file"

#define ICON_ID_ATTR				 "id"
#define ICON_TYPE_ATTR				 "type"
#define ICON_WIDTH_ATTR				 "width"
#define ICON_HEIGHT_ATTR			 "height"
#define MENU_ITEM_ID_ATTR			 "id"
#define MENU_ITEM_ACCELERATOR_ATTR	 "accelerator"
#define MENU_ITEM_PARENT_ATTR		 "parent"
#define MENU_ITEM_ORDER_ATTR		 "order"
#define MENU_ITEM_TARGET_ATTR		 "target"
#define MENU_ITEM_CHECKED_ATTR		 "checked"
#define MENU_GROUP_ORDER_ATTR		 "order"
#define TOOLBAR_ORDER_ATTR			 "order"
#define SHORTCUT_KEY_ATTR            "key"
#define STATUS_ID_ATTR				 "id"
#define STATUS_DEFAULT_FLAG_ATTR	 "default"
#define CALL_TYPE_ATTR				 "type"
#define CALL_ICON_ATTR				 "icon"
#define CALL_ORDER_ATTR				 "order"
#define CALL_STATUS_ATTR			 "status"
#define CALL_METHOD_STATIC_ATTR		 "static"
#define CALL_IFLOADED_ATTR			 "ifloaded"
#define ARG_TYPE_TYPE_ATTR			 "type"
#define ARG_TYPE_MIN_INCLUSIVE_ATTR	 "minInclusive"
#define ARG_TYPE_MAX_INCLUSIVE_ATTR	 "maxInclusive"
#define ARG_TYPE_MIN_EXCLUSIVE_ATTR	 "minExclusive"
#define ARG_TYPE_MAX_EXCLUSIVE_ATTR	 "maxExclusive"
#define ARG_TYPE_COLON_ATTR			 "colon"
#define ARG_TYPE_DASH_ATTR			 "dash"
#define HELP_FILE_ORDER_ATTR		 "order"

#define MENU_CALL_PARAM_NAME		 "menu"

#define CALL_TYPE_EXECUTE			 "EXECUTE"
#define CALL_TYPE_ENABLE			 "ENABLE"
#define CALL_TYPE_CHECKED			 "CHECKED"

#define STATUS_BAR_GUI_TAG			 "gui"
#define STATUS_BAR_TAG				 "status-bar"
#define STATUS_BAR_TAG_TARGET		 "target"
#define STATUS_BAR_TAG_ATTRIBUTE	 "order"
#define STATUS_BAR_NAME				 "status-bar-name"

//#define PACKAGE_TAG			         "package"	// @date 2013/02/21 <Add> OKADA
#define PACKAGE_FIXED_ATTR	         "fixed"		// @date 2013/02/21 <Add> OKADA

// >>>>>>	@Date:2013/08/20	<Add>	A.Ozaki
#define	COMPONENTS_TAG					"components"
#define		COMPONENT_TAG					"component"
#define			COMPONENT_NAME_ATTR				"component-name"
#define			COMPONENT_DESC_ATTR				"component-desc"
#define			COMPONENT_CALLS_TAG				"component-calls"
#define				COMPONENT_CALL_ATTR				"component-call"
#define	PACKAGES_TAG					"packages"
#define		PACKAGE_TAG						"package"
#define			PACKAGE_NAME_ATTR				"package-name"
#define			PACKAGE_COMPONENTS_TAG			"package-components"
#define				PACKAGE_COMPONENT_ATTR			"package-component"
// <<<<<	@Date:2013/08/20	<Add>	A.Ozaki

/* 文字列を大文字に直して比較 */	// multiplebyte文字のことは考えていないので注意
static int capitalized_strcmp(const char *s1, const char *s2)
{
    while (toupper(*s1) == toupper(*s2)) {  /* 文字が等しい間ループする */
        if (*s1 == '\0'){                   /* 末尾まで等しければ */
            return (0);                     /* ０を返す */
		}
        s1++;                               /* 文字列の途中なので */
        s2++;                               /* インクリメントする */
    }    /* 等しくないので文字の値の差を返す */
    return (toupper(*s1) - toupper(*s2));
};

// constructor
PluginHandler::PluginHandler( PluginInfo& plugin, PluginMenu& menu )
		: m_plugin( plugin ), m_menu( menu ) {
}

// destructor
PluginHandler::~PluginHandler() {

}

// start document
void PluginHandler::onStartDocument() {
	m_currentIcon = NULL;
	m_currentMenu = NULL;
	m_currentArg = NULL;
	m_currentStatusbarInfo = m_plugin.createCommonStatusBarInfo();	// Status Bar Info
	m_inJava = false;

// >>>>>>	@Date:2013/08/21	<Add>	A.Ozaki
//
	m_pCurrentComponent = (PluginComponent *)NULL;
	m_mapCall.clear( );
	m_vecCalls.clear( );
	m_strCurrentPackageName.clear( );
//
// <<<<<<	@Date:2013/08/21	<Add>	A.Ozaki
}

// end document
void PluginHandler::onEndDocument() {

// >>>>>>	@Date:2013/08/21	<Add>	A.Ozaki
//

	for ( unsigned int unIndex = 0 ; unIndex < m_plugin.getNumberOfCalls( ) ; unIndex++ )
	{
		PluginCall	*pCall = m_plugin.getCall( unIndex );

		bool	bFound = false;
		std::vector<PluginCall *>::iterator	it;
		for ( it = m_vecCalls.begin( ) ; it != m_vecCalls.end( ) ; it++ )
		{
			if  ( *it == pCall )
			{
				bFound = true;
			}
		}
		if  ( false == bFound )
		{
			PluginComponent*	pComponent = new PluginComponent( );
			pComponent->setName( pCall->getName( ) );
			pComponent->setDesc( pCall->getDescription( ) );
			pComponent->addCall( pCall );

			m_plugin.addComponent( pComponent );
		}
	}

	m_mapCall.clear( );
	m_vecCalls.clear( );
//
// <<<<<<	@Date:2013/08/21	<Add>	A.Ozaki

	// clear
	m_iconMap.clear();
	m_menuMap.clear();
	m_menuMapTempMi.clear();
	m_statusMap.clear();
}

// start element
void PluginHandler::onStartElement( const char* name, kome::core::Properties& attrs ) {
	// each tags
	if( strcmp( name, ICON_TAG ) == 0 ) { // <icon>
		// create object
		m_currentIcon = m_plugin.createNewIcon();

		// ID
		int id = attrs.getIntValue( ICON_ID_ATTR, -1 );
		m_currentIcon->setId( id );

		// type
		m_currentIcon->setType( attrs.getStringValue( ICON_TYPE_ATTR, "" ) );

		// size
		int width = attrs.getIntValue( ICON_WIDTH_ATTR, 0 );
		int height = attrs.getIntValue( ICON_HEIGHT_ATTR, 0 );

		m_currentIcon->setSize( width, height );

		// set object to map
		m_iconMap[ m_currentIcon->getId() ] = m_currentIcon;
	}

	// <menu-item>
	else if( strcmp( name, MENU_ITEM_TAG ) == 0 ) {	// <menu-item>

		/*	タグ階層（正確な情報はスキーマ参照）
			menu-item		
				menu-name	
				menu-group	
				toolbar	
				shortcut	
					shortcut-key
				popup	
				menu-description	
		*/

		// 今回新設した変数の初期化処理を行う
		// <menu-item>タグ用一時変数の初期化処理を行う

		// <menu-item>
		m_nIdMenuItem = 0;						// m_menuInfoTempMi.id
		m_strAcceleratorMenuItem = "";	// std::string accelerator
		m_nParentMenuItem = 0;					// int parent
		m_nOrderMenuItem = 0;					// m_menuInfoTempMi.order
		m_strTargetMenuItem = "";		// m_menuInfoTempMi.target
		m_nCheckedIdMenuItem = 0;				// int checkedId 

		// </menu-name>
		m_strTextMenuName = "";

		// <menu-group>
		m_nOrderMenuGroup = 0;	// m_groupOrder

		// </menu-group>
		m_strTextMenuGroup = "";

		// <toolbar>
		m_nOrderToolbar = 0;

		// </toolbar>
		m_strTextToolbar = "";

		// <shortcut-key>
		m_v_strKeyShortcutKey.clear();
			
		// <popup>
		m_v_strTypePopup.clear();
		m_v_nOrderPopup.clear();
			
		// </menu-description>
		m_strTextMenuDescription = "";

		// 読み込んだタグを一時的に保存
		m_nIdMenuItem = attrs.getIntValue( MENU_ITEM_ID_ATTR, -1 );	// m_menuInfoTempMi.id
		m_strAcceleratorMenuItem = attrs.getStringValue( MENU_ITEM_ACCELERATOR_ATTR, "" );	// std::string accelerator
		m_nParentMenuItem = attrs.getIntValue( MENU_ITEM_PARENT_ATTR, -1 );			// int parent
		m_nOrderMenuItem = attrs.getIntValue( MENU_ITEM_ORDER_ATTR, -1 );			// m_menuInfoTempMi.order
		m_strTargetMenuItem = attrs.getStringValue( MENU_ITEM_TARGET_ATTR, "" );		// m_menuInfoTempMi.target
		m_nCheckedIdMenuItem = attrs.getIntValue( MENU_ITEM_CHECKED_ATTR, -1 );		// int checkedId 
	}

	// <menu-group>

	else if( strcmp( name, MENU_GROUP_TAG ) == 0 ) {	// <menu-group>
		// order
		m_nOrderMenuGroup = attrs.getIntValue( MENU_GROUP_ORDER_ATTR, -1 );
	}

	// <popup>
	else if( strcmp( name, MENU_ITEM_POPUP_TAG ) == 0 ) {	// <popup>

		std::string strTemp;
		int nTemp;

		strTemp = attrs.getStringValue( MENU_POPUP_TYPE_TAG, "" );	// type=
		nTemp = attrs.getIntValue( MENU_POPUP_ORDER_TAG, 5 );	// order=	// orderが無い場合は「5」とする

		m_v_strTypePopup.push_back( strTemp );
		m_v_nOrderPopup.push_back( nTemp );
	}

	// <toolbar>
	else if( strcmp( name, TOOLBAR_TAG ) == 0 ) {   // <toolbar>
		// order
		m_nOrderToolbar = attrs.getIntValue( TOOLBAR_ORDER_ATTR, -1 );
	}

	 // <shortcut-key>
	else if( strcmp( name, SHORTCUT_KEY_TAG ) == 0 ) {    // <shortcut-key>
		std::string key = attrs.getStringValue( SHORTCUT_KEY_ATTR, "" );
		m_v_strKeyShortcutKey.push_back( key );
	}

	else if( strcmp( name, STATUS_TAG ) == 0 ) {	// <status>
		m_statusInfo.id = attrs.getIntValue( STATUS_ID_ATTR, -1 );
		m_statusInfo.df = attrs.getBoolValue( STATUS_DEFAULT_FLAG_ATTR, false );

		// init
		m_statusInfo.name.clear();
		m_statusInfo.value.clear();
	}
	else if( strcmp( name, CALL_TAG ) == 0 ) {	// <call>
		// creating object
		m_currentCall = m_plugin.createNewCall();

		// set type
		m_currentCall->setType( attrs.getStringValue( CALL_TYPE_ATTR, "" ) );

		// set icon
		int icon = attrs.getIntValue( CALL_ICON_ATTR, -1 );
		if( m_iconMap.find( icon ) != m_iconMap.end() ) {
			m_currentCall->setIcon( m_iconMap[ icon ] );
		}

		// set order
		m_currentCall->setOrder( attrs.getIntValue( CALL_ORDER_ATTR, 5 ) );

		// set statuses
		std::vector< std::string > statusIdArray;
		stringtoken(
			attrs.getStringValue( CALL_STATUS_ATTR, "" ), ", \t\r\n", statusIdArray);
		for( unsigned int i = 0; i < statusIdArray.size(); i++ ) {
			int statusId = toint( statusIdArray[ i ].c_str(), 10, -1 );
			if( statusId >= 0 && m_statusMap.find( (unsigned int)statusId ) != m_statusMap.end() ) {
				std::pair< std::string, std::string >& statusPair = m_statusMap[ (unsigned int)statusId ];

				m_currentCall->addStatus( statusPair.first.c_str(), statusPair.second.c_str() );
			}
		}

		// set ifloaded
		bool ifloaded = attrs.getBoolValue( CALL_IFLOADED_ATTR, false );
		m_currentCall->setIfLoaded( ifloaded );
	}
	else if( strcmp( name, CALL_METHOD_TAG ) == 0 ) {	// <call-method>
		// static method or not
		m_methodInfo.isStatic = attrs.getBoolValue( CALL_METHOD_STATIC_ATTR, false );
	}
	else if( strcmp( name, HELP_FILE_TAG ) == 0 ) { // <help-file>
		// create help
		m_currentHelp = m_plugin.createNewHelp();

		// set order
		m_currentHelp->setOrder( attrs.getIntValue( HELP_FILE_ORDER_ATTR, 5 ) );
	}
	else if( strcmp( name, STATUS_BAR_TAG ) == 0 ) {	// <status-bar>

		// status bar タグ開始
		// targetの読み取り
		std::string str = attrs.getStringValue( STATUS_BAR_TAG_TARGET, "" );	// "target"
		m_tempStatusBarInfo.clearStatusBarInfo();
		m_tempStatusBarInfo.setTarget( (const char*)str.c_str() );

		// orderの処理
		int order = attrs.getIntValue( STATUS_BAR_TAG_ATTRIBUTE, 5 );
		m_tempStatusBarInfo.setOrder( order );

		int width = attrs.getIntValue( "width", -1);	// "width"
		m_tempStatusBarInfo.setWidth( width );
	}
	else if( strcmp( name, JAVA_TAG ) == 0 ) {    // <java>
		m_inJava = true;
	}
// >>>>>>	@Date:2013/08/20	<Add>	A.Ozaki
//
	else if  ( 0 == strcmp( name, COMPONENT_TAG ) )    // <component>
	{
		m_pCurrentComponent = new PluginComponent( );
	}
//
// <<<<<<	@Date:2013/08/20	<Add>	A.Ozaki

}


// <popup>タグのtypeの文字列をPopupTypeに変換する
PopupType popup_str2enum( const char * szPopupStr ){
	PopupType type  = POPUP_NONE;

	if( capitalized_strcmp( szPopupStr, MENU_POPUP_TYPE_ATTR_SPEC_ITEM ) == 0 ){		        // スペクトルアイコン (SAMPLE TREE VIEW)
		type = POPUP_SPEC_ITEM;
	}else if( capitalized_strcmp( szPopupStr, MENU_POPUP_TYPE_ATTR_CHROM_ITEM ) == 0 ){	        // クロマトグラムアイコン (SAMPLE TREE VIEW)
		type = POPUP_CHROM_ITEM;
	}else if( capitalized_strcmp( szPopupStr, MENU_POPUP_TYPE_ATTR_GROUP_ITEM ) == 0 ){	        // グループアイコン (SAMPLE TREE VIEW)
		type = POPUP_GROUP_ITEM;
	}else if( capitalized_strcmp( szPopupStr, MENU_POPUP_TYPE_ATTR_SPEC_VIEW ) == 0 ){			// スペクトル波形ウィンドウ
		type = POPUP_SPEC_VIEW;
	}else if( capitalized_strcmp( szPopupStr, MENU_POPUP_TYPE_ATTR_SPEC_SUB_VIEW ) == 0 ){		// スペクトル波形ウィンドウ(Sub)
		type = POPUP_SPEC_SUB_VIEW;
	}else if( capitalized_strcmp( szPopupStr, MENU_POPUP_TYPE_ATTR_CHROM_VIEW ) == 0 ){		    // クロマトグラム波形ウィンドウ
		type = POPUP_CHROM_VIEW;
	}else if( capitalized_strcmp( szPopupStr, MENU_POPUP_TYPE_ATTR_CHROM_SUB_VIEW ) == 0 ){	    // クロマトグラム波形ウィンドウ(Sub)
		type = POPUP_CHROM_SUB_VIEW;
	}else if( capitalized_strcmp( szPopupStr, MENU_POPUP_TYPE_ATTR_DATAMAP_VIEW ) == 0 ){		// ヒートマップ OR 3D VIEW
		type = POPUP_DATAMAP_VIEW;
	}else if( capitalized_strcmp( szPopupStr, MENU_POPUP_TYPE_ATTR_DATAMAP_SUB_VIEW ) == 0 ){	// ヒートマップ OR 3D VIEW(Sub)
		type = POPUP_DATAMAP_SUB_VIEW;
	}

	return type;
}

// end element
void PluginHandler::onEndElement( const char* name, const char* text ) {
	// each tags
	if( strcmp( name, PLUGIN_NAME_TAG ) == 0 ) {	// </name>
		m_plugin.setName( text );
	}
	else if( strcmp( name, PLUGIN_VERSION_TAG ) == 0 ) {	// </version>
		m_plugin.setVersion( text );
	}
	else if( strcmp( name, PLUGIN_PROVIDER_TAG ) == 0 ) {	// </provider>
		m_plugin.setProvider( text );
	}
	else if( strcmp( name, PLUGIN_LINK_TAG ) == 0 ) {	// </link>
		m_plugin.setLink( text );
	}
	else if( strcmp( name, ICON_TAG ) == 0 ) { // </icon>
		m_currentIcon = NULL;
	}
	else if( strcmp( name, ICON_NAME_TAG ) == 0 ) {	// </icon-name>
		if( m_currentIcon != NULL ) {
			m_currentIcon->setName( text );
		}
	}
	else if( strcmp( name, ICON_FILE_TAG ) == 0 ) { // </icon-file>
		if( m_currentIcon != NULL ) {
			m_currentIcon->setPath( getabsolutepath( m_plugin.getDir(), text ).c_str() );
		}
	}

	// </menu-item>
	else if( strcmp( name, MENU_ITEM_TAG ) == 0 ) {	// </menu-item>

		// ここで、popup-menuタグがあるかどうか判断して、処理を分岐する
		if( 0 < m_v_strTypePopup.size() ){
			// popupタグが含まれていた

			for( unsigned int pc=0; pc<m_v_strTypePopup.size(); pc++ )	// @date 2013/06/10 <Mod> OKADA	
			{

				////////////////////////////////////////////////////////////
				// <menu-item>
				// ID
				m_menuInfoTempMi.id = m_nIdMenuItem;

				// accelerator
				std::string accelerator = m_strAcceleratorMenuItem;
				if( !accelerator.empty() ) {
					m_menuInfoTempMi.accelerator = accelerator[0];
				}
				else{
					m_menuInfoTempMi.accelerator = '\0';
				}

				// parent
				int parent = m_nParentMenuItem;
				if( m_menuMapTempMi.find( parent ) == m_menuMapTempMi.end() ) {
					kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();
					
					std::string strPopup = m_v_strTypePopup[pc];
					PopupType ptype = popup_str2enum( strPopup.c_str() );

					switch( ptype ){
					case POPUP_NONE:						
						// 通常あり得ない
						break;
					case POPUP_SPEC_ITEM:		// スペクトルアイコン (SAMPLE TREE VIEW)
						m_currentMenuTempMi = &(plgMgr.getSpectrumItemMenu());
						break;
					case POPUP_CHROM_ITEM:		// クロマトグラムアイコン (SAMPLE TREE VIEW)
						m_currentMenuTempMi = &(plgMgr.getChromatogramItemMenu());
						break;
					case POPUP_GROUP_ITEM:		// グループアイコン (SAMPLE TREE VIEW)
						m_currentMenuTempMi = &(plgMgr.getGroupItemMenu());
						break;
					case POPUP_SPEC_VIEW:		// スペクトル波形ウィンドウ
						m_currentMenuTempMi = &(plgMgr.getSpectrumViewMenu());
						break;
					case POPUP_SPEC_SUB_VIEW:	// スペクトル波形ウィンドウ(Sub)
						m_currentMenuTempMi = &(plgMgr.getSpectrumSubViewMenu());
						break;
					case POPUP_CHROM_VIEW:		// クロマトグラム波形ウィンドウ
						m_currentMenuTempMi = &(plgMgr.getChromatogramViewMenu());
						break;
					case POPUP_CHROM_SUB_VIEW:	// クロマトグラム波形ウィンドウ(Sub)
						m_currentMenuTempMi = &(plgMgr.getChromatogramSubViewMenu());
						break;
					case POPUP_DATAMAP_VIEW:	// ヒートマップ OR 3D VIEW
						m_currentMenuTempMi = &(plgMgr.getDataMapViewMenu());
						break;
					case POPUP_DATAMAP_SUB_VIEW:// ヒートマップ OR 3D VIEW(Sub)
						m_currentMenuTempMi = &(plgMgr.getDataMapSubViewMenu());
						break;
					default:
						// 通常あり得ない
						break;
					}
				}
				else {
					m_currentMenuTempMi = m_menuMapTempMi[ parent ];
				}

				// order
				m_menuInfoTempMi.order = m_nOrderMenuItem;

				// target
				m_menuInfoTempMi.target = m_strTargetMenuItem;

				// status
				int checkedId = m_nCheckedIdMenuItem;
				if( checkedId >= 0 && m_statusMap.find( (unsigned int)checkedId ) != m_statusMap.end() ) {
					std::pair< std::string, std::string >& statusPair = m_statusMap[ (unsigned int)checkedId ];

					m_menuInfoTempMi.chkName = statusPair.first;
					m_menuInfoTempMi.chkValue = statusPair.second;
				}
				else {
					m_menuInfoTempMi.chkName.clear();
					m_menuInfoTempMi.chkValue.clear();
				}
				////////////////////////////////////////////////////////////
				// </menu-name>
				if( m_currentMenuTempMi != NULL && m_menuInfoTempMi.id >= 0 ) {
					// create child menu
					PluginMenu* m = m_currentMenuTempMi->createChildMenu( m_strTextMenuName.c_str() );
					if( m == NULL ) {
						m = m_currentMenuTempMi->getChildMenu( m_strTextMenuName.c_str() );
					}
					m_currentMenuTempMi = m;
				}

				if( m_currentMenuTempMi != NULL ) {
					// set properties
					if( m_menuInfoTempMi.order >= 0 ) {
						m_currentMenuTempMi->setOrder( m_menuInfoTempMi.order );
					}
					if( m_menuInfoTempMi.accelerator != '\0' ) {
						m_currentMenuTempMi->setAccelerator( m_menuInfoTempMi.accelerator );
					}
					if( !m_menuInfoTempMi.target.empty() ) {
						m_currentMenuTempMi->setTarget( m_menuInfoTempMi.target.c_str() );
					}
					if( !m_menuInfoTempMi.chkName.empty() ) {
						m_currentMenuTempMi->setCheckedStatus( m_menuInfoTempMi.chkName.c_str(), m_menuInfoTempMi.chkValue.c_str() );
					}

					// set to map
					m_menuMapTempMi[ m_menuInfoTempMi.id ] = m_currentMenuTempMi;
				}
				////////////////////////////////////////////////////////////
				// <menu-group>
				m_groupOrderTempMi = m_nOrderMenuGroup;

				////////////////////////////////////////////////////////////
				// </menu-group>
				if( m_currentMenuTempMi != NULL ) {
					m_currentMenuTempMi->setGroup( m_strTextMenuGroup.c_str(), m_groupOrderTempMi );
				}

				////////////////////////////////////////////////////////////
				// <toolbar>
				m_toolbarOrderTempMi = m_nOrderToolbar;

				////////////////////////////////////////////////////////////
				// </toolbar>
				if( m_currentMenuTempMi != NULL ) {
					m_currentMenuTempMi->setToolbar( m_strTextToolbar.c_str(), m_toolbarOrderTempMi );
				}

				////////////////////////////////////////////////////////////
				// <shortcut-key>
				if( m_currentMenuTempMi != NULL ) {
					for( int i=0; i<(int)m_v_strKeyShortcutKey.size(); i++ ){
						std::string key = m_v_strKeyShortcutKey[i];
						if( !key.empty() ) {
							m_currentMenuTempMi->addShortcutKey( key.c_str() );
						}
					}
				}

				////////////////////////////////////////////////////////////
				// <popup>
				{
					// type
					m_popupType = m_v_strTypePopup[pc];	// type=
					// order
					m_popupOrder = m_v_nOrderPopup[pc];	// order=	// orderが無い場合は「5」とする

					PopupType type  = POPUP_NONE;

					if( capitalized_strcmp( m_popupType.c_str(), MENU_POPUP_TYPE_ATTR_SPEC_ITEM ) == 0 ){			    // スペクトルアイコン (SAMPLE TREE VIEW)
						type = POPUP_SPEC_ITEM;
					}else if( capitalized_strcmp( m_popupType.c_str(), MENU_POPUP_TYPE_ATTR_CHROM_ITEM ) == 0 ){	    // クロマトグラムアイコン (SAMPLE TREE VIEW)
						type = POPUP_CHROM_ITEM;
					}else if( capitalized_strcmp( m_popupType.c_str(), MENU_POPUP_TYPE_ATTR_GROUP_ITEM ) == 0 ){	    // グループアイコン (SAMPLE TREE VIEW)
						type = POPUP_GROUP_ITEM;
					}else if( capitalized_strcmp( m_popupType.c_str(), MENU_POPUP_TYPE_ATTR_SPEC_VIEW ) == 0 ){			// スペクトル波形ウィンドウ
						type = POPUP_SPEC_VIEW;
					}else if( capitalized_strcmp( m_popupType.c_str(), MENU_POPUP_TYPE_ATTR_SPEC_SUB_VIEW ) == 0 ){		// スペクトル波形ウィンドウ(Sub)
						type = POPUP_SPEC_SUB_VIEW;
					}else if( capitalized_strcmp( m_popupType.c_str(), MENU_POPUP_TYPE_ATTR_CHROM_VIEW ) == 0 ){		// クロマトグラム波形ウィンドウ
						type = POPUP_CHROM_VIEW;
					}else if( capitalized_strcmp( m_popupType.c_str(), MENU_POPUP_TYPE_ATTR_CHROM_SUB_VIEW ) == 0 ){	// クロマトグラム波形ウィンドウ(Sub)
						type = POPUP_CHROM_SUB_VIEW;
					}else if( capitalized_strcmp( m_popupType.c_str(), MENU_POPUP_TYPE_ATTR_DATAMAP_VIEW ) == 0 ){		// ヒートマップ OR 3D VIEW
						type = POPUP_DATAMAP_VIEW;
					}else if( capitalized_strcmp( m_popupType.c_str(), MENU_POPUP_TYPE_ATTR_DATAMAP_SUB_VIEW ) == 0 ){	// ヒートマップ OR 3D VIEW(Sub)
						type = POPUP_DATAMAP_SUB_VIEW;
					}

					if( type == POPUP_NONE ){
						// error
					}else{
						if( m_currentMenuTempMi != NULL ) {
							m_currentMenuTempMi->addPopupTypeOrder( type, m_popupOrder );
						}
					}
				}

				////////////////////////////////////////////////////////////
				// </menu-description>
				if( m_currentMenuTempMi != NULL ) {
					m_currentMenuTempMi->setDescription( m_strTextMenuDescription.c_str() );
				}

				////////////////////////////////////////////////////////////
			}
		}
		else{

			////////////////////////////////////////////////////////////
			// <menu-item>
			// ID
			m_menuInfo.id = m_nIdMenuItem;

			// accelerator
			std::string accelerator = m_strAcceleratorMenuItem;
			if( !accelerator.empty() ) {
				m_menuInfo.accelerator = accelerator[0];
			}
			else{
				m_menuInfo.accelerator = '\0';
			}

			// parent
			int parent = m_nParentMenuItem;
			if( m_menuMap.find( parent ) == m_menuMap.end() ) {
				m_currentMenu = &m_menu;
			}
			else {
				m_currentMenu = m_menuMap[ parent ];
			}

			// order
			m_menuInfo.order = m_nOrderMenuItem;

			// target
			m_menuInfo.target = m_strTargetMenuItem;

			// status
			int checkedId = m_nCheckedIdMenuItem;
			if( checkedId >= 0 && m_statusMap.find( (unsigned int)checkedId ) != m_statusMap.end() ) {
				std::pair< std::string, std::string >& statusPair = m_statusMap[ (unsigned int)checkedId ];

				m_menuInfo.chkName = statusPair.first;
				m_menuInfo.chkValue = statusPair.second;
			}
			else {
				m_menuInfo.chkName.clear();
				m_menuInfo.chkValue.clear();
			}
			////////////////////////////////////////////////////////////
			// </menu-name>
			if( m_currentMenu != NULL && m_menuInfo.id >= 0 ) {
				// create child menu
				PluginMenu* m = m_currentMenu->createChildMenu( m_strTextMenuName.c_str() );
				if( m == NULL ) {
					m = m_currentMenu->getChildMenu( m_strTextMenuName.c_str() );
				}
				m_currentMenu = m;
			}

			if( m_currentMenu != NULL ) {
				// set properties
				if( m_menuInfo.order >= 0 ) {
					m_currentMenu->setOrder( m_menuInfo.order );
				}
				if( m_menuInfo.accelerator != '\0' ) {
					m_currentMenu->setAccelerator( m_menuInfo.accelerator );
				}
				if( !m_menuInfo.target.empty() ) {
					m_currentMenu->setTarget( m_menuInfo.target.c_str() );
				}
				if( !m_menuInfo.chkName.empty() ) {
					m_currentMenu->setCheckedStatus( m_menuInfo.chkName.c_str(), m_menuInfo.chkValue.c_str() );
				}

				// set to map
				m_menuMap[ m_menuInfo.id ] = m_currentMenu;
			}
			////////////////////////////////////////////////////////////
			// <menu-group>
			m_groupOrder = m_nOrderMenuGroup;

			////////////////////////////////////////////////////////////
			// </menu-group>
			if( m_currentMenu != NULL ) {
				m_currentMenu->setGroup( m_strTextMenuGroup.c_str(), m_groupOrder );
			}

			////////////////////////////////////////////////////////////
			// <toolbar>
			m_toolbarOrder = m_nOrderToolbar;

			////////////////////////////////////////////////////////////
			// </toolbar>
			if( m_currentMenu != NULL ) {
				m_currentMenu->setToolbar( m_strTextToolbar.c_str(), m_toolbarOrder );
			}

			////////////////////////////////////////////////////////////
			// <shortcut-key>
			if( m_currentMenu != NULL ) {
				for( int i=0; i<(int)m_v_strKeyShortcutKey.size(); i++ ){
					std::string key = m_v_strKeyShortcutKey[i];
					if( !key.empty() ) {
						m_currentMenu->addShortcutKey( key.c_str() );
					}
				}
			}

			////////////////////////////////////////////////////////////
			// <popup>
			for( int i=0; i<(int)m_v_strTypePopup.size(); i++ ){
				// type
				m_popupType = m_v_strTypePopup[i];	// type=
				// order
				m_popupOrder = m_v_nOrderPopup[i];	// order=	// orderが無い場合は「5」とする

				PopupType type  = POPUP_NONE;

				if( capitalized_strcmp( m_popupType.c_str(), MENU_POPUP_TYPE_ATTR_SPEC_ITEM ) == 0 ){			   // スペクトルアイコン (SAMPLE TREE VIEW)
					type = POPUP_SPEC_ITEM;
				}else if( capitalized_strcmp( m_popupType.c_str(), MENU_POPUP_TYPE_ATTR_CHROM_ITEM ) == 0 ){	   // クロマトグラムアイコン (SAMPLE TREE VIEW)
					type = POPUP_CHROM_ITEM;
				}else if( capitalized_strcmp( m_popupType.c_str(), MENU_POPUP_TYPE_ATTR_GROUP_ITEM ) == 0 ){	   // グループアイコン (SAMPLE TREE VIEW)
					type = POPUP_GROUP_ITEM;
				}else if( capitalized_strcmp( m_popupType.c_str(), MENU_POPUP_TYPE_ATTR_SPEC_VIEW ) == 0 ){			// スペクトル波形ウィンドウ
					type = POPUP_SPEC_VIEW;
				}else if( capitalized_strcmp( m_popupType.c_str(), MENU_POPUP_TYPE_ATTR_SPEC_SUB_VIEW ) == 0 ){		// スペクトル波形ウィンドウ(Sub)
					type = POPUP_SPEC_SUB_VIEW;
				}else if( capitalized_strcmp( m_popupType.c_str(), MENU_POPUP_TYPE_ATTR_CHROM_VIEW ) == 0 ){		// クロマトグラム波形ウィンドウ
					type = POPUP_CHROM_VIEW;
				}else if( capitalized_strcmp( m_popupType.c_str(), MENU_POPUP_TYPE_ATTR_CHROM_SUB_VIEW ) == 0 ){	// クロマトグラム波形ウィンドウ(Sub)
					type = POPUP_CHROM_SUB_VIEW;
				}else if( capitalized_strcmp( m_popupType.c_str(), MENU_POPUP_TYPE_ATTR_DATAMAP_VIEW ) == 0 ){		// ヒートマップ OR 3D VIEW
					type = POPUP_DATAMAP_VIEW;
				}else if( capitalized_strcmp( m_popupType.c_str(), MENU_POPUP_TYPE_ATTR_DATAMAP_SUB_VIEW ) == 0 ){	// ヒートマップ OR 3D VIEW(Sub)
					type = POPUP_DATAMAP_SUB_VIEW;
				}

				if( type == POPUP_NONE ){
					// error
				}else{
					if( m_currentMenu != NULL ) {
						m_currentMenu->addPopupTypeOrder( type, m_popupOrder );
					}
				}
			}

			////////////////////////////////////////////////////////////
			// </menu-description>
			if( m_currentMenu != NULL ) {
				m_currentMenu->setDescription( m_strTextMenuDescription.c_str() );
			}

			////////////////////////////////////////////////////////////
		}


		// </menu-item>
		m_currentMenu = NULL;
		m_groupOrder = -1;
		m_toolbarOrder = -1;
	}

	// </menu-name>
	else if( strcmp( name, MENU_ITEM_NAME_TAG ) == 0 ){	// </menu-name>
		m_strTextMenuName = text;
	}

	// </menu-group>
	else if( strcmp( name, MENU_GROUP_TAG ) == 0 ) {	// </menu-group>
		m_strTextMenuGroup = text;
	}

	// </toolbar>
	else if( strcmp( name, TOOLBAR_TAG ) == 0 ) {   // </toolbar>
		m_strTextToolbar = text;
	}

	// </menu-description>
	else if( strcmp( name, MENU_DESCRIPTION_TAG ) == 0 ) {		// </menu-description>
		m_strTextMenuDescription = text;
	}

	else if( strcmp( name, STATUS_TAG ) == 0 ) {	// </status>
		if( m_statusInfo.id >= 0 ) {
			// name & value
			std::string n = m_statusInfo.name;
			std::string v = m_statusInfo.value;


			// set default value
			if( m_statusInfo.df ) {
				kome::objects::StatusManager& statusMgr = kome::objects::StatusManager::getInstance();

				if( !statusMgr.hasStatus( m_statusInfo.name.c_str() ) ) {
					statusMgr.setValue( m_statusInfo.name.c_str(), m_statusInfo.value.c_str() );
				}

				v = FMT( "%s%s", "!", v.c_str() );
			}

			// set to map
			m_statusMap[ (unsigned int)m_statusInfo.id ] = std::make_pair( n, v );
		}
	}
	else if( strcmp( name, STATUS_NAME_TAG ) == 0 ) {		// </status-name>
		m_statusInfo.name = text;
	}
	else if( strcmp( name, STATUS_VALUE_TAG ) == 0 ) {		// </status-value>
		m_statusInfo.value = text;
	}
	else if( strcmp( name, CALL_TAG ) == 0 ) {	// </call>
		if( m_currentCall != NULL ) {
			// get menu
			std::list<int> menuIds;
			std::vector<int> values;
			m_currentCall->getProperties().getIntValues(MENU_CALL_PARAM_NAME, values);

			for(unsigned int i = 0; i < values.size(); i++) {

				int menuId = values[i];	// menu ID
				if( m_menuMap.find( menuId ) != m_menuMap.end() ) {	// check map
					PluginMenu* menu = m_menuMap[menuId];

					// set function information
					if( strcmp( m_currentCall->getType(), CALL_TYPE_EXECUTE ) == 0 ) {	    // EXECUTE
						menu->setCallExec( m_currentCall );
					}
					else if( strcmp( m_currentCall->getType(), CALL_TYPE_ENABLE ) == 0 ) {	// ENABLE
						menu->setCallEnable( m_currentCall );
					}
					else if( strcmp( m_currentCall->getType(), CALL_TYPE_CHECKED ) == 0 ) {	// CHECKED
						menu->setCallChecked( m_currentCall );
					}

				}

				//　↑ウインドウtopのメニューバー用処理

				//　↓Context Menu 用処理
				if( m_menuMapTempMi.find( menuId ) != m_menuMapTempMi.end() ) {	// check map
					PluginMenu* menuPopup = m_menuMapTempMi[menuId];

					int nNum = 0;
					if( menuPopup != NULL ){
						nNum = menuPopup->getNumberOfPopup();

						// set function information
						if( strcmp( m_currentCall->getType(), CALL_TYPE_EXECUTE ) == 0 ) {	    // EXECUTE
							menuPopup->setCallExec( m_currentCall );
						}
						else if( strcmp( m_currentCall->getType(), CALL_TYPE_ENABLE ) == 0 ) {	// ENABLE
							menuPopup->setCallEnable( m_currentCall );
						}
						else if( strcmp( m_currentCall->getType(), CALL_TYPE_CHECKED ) == 0 ) {	// CHECKED
							menuPopup->setCallChecked( m_currentCall );
						}
					}
				}
			}

			// log
			if( m_currentCall != NULL ) {
				LOG_DEBUG(
					FMT(
						"add call \"%s\" (type: %s)",
						NVL( m_currentCall->getName(), "" ),
						NVL( m_currentCall->getType(), "" )
					)
				);
			}

			m_currentCall = NULL;
		}
	}
	else if( strcmp( name, CALL_NAME_TAG ) == 0 ) {	// </call-name>
		if( m_currentCall != NULL ) {
			m_currentCall->setName( text );
// >>>>>>	＠Date:2013/08/21	<Add>	A.Ozaki
//
			std::string	strName = FMT( "%s", text );
			m_mapCall[strName] = m_currentCall;
//
// <<<<<<	@Date:2013/08/21	<Add>	A.Ozaki
		}
	}
	else if( strcmp( name, CALL_FUNCTION_TAG ) == 0 ) {	// </call-function>
		if( m_currentCall != NULL ) {
			m_currentCall->setFunction( text, NULL, true );
		}
	}
	else if( strcmp( name, CALL_METHOD_TAG ) == 0 ) {	// </call-method>
		if( m_currentCall != NULL ) {
			m_currentCall->setFunction(
				m_methodInfo.methodName.c_str(),
				m_methodInfo.className.c_str(),
				m_methodInfo.isStatic
			);
		}
	}
	else if( strcmp( name, CALL_CLASS_NAME_TAG ) == 0 ) {	// </class_name>
		m_methodInfo.className = text;
	}
	else if( strcmp( name, CALL_METHOD_NAME_TAG ) == 0 ) {	// </method-name>
		m_methodInfo.methodName = text;
	}
	else if( strcmp( name, CALL_PARAM_KEY_TAG ) == 0 ) {	// </param-name>
		m_paramKey = text;
	}
	else if( strcmp( name, CALL_PARAM_VALUE_TAG ) == 0 ) {	// </param-value>
		if( m_currentCall != NULL ) {
			m_currentCall->getProperties().setValue( m_paramKey.c_str(), text );
		}
	}
	else if( strcmp( name, CALL_DESCRIPTION_TAG ) == 0 ) {  // </call-description>
		if( m_currentCall != NULL ) {
			m_currentCall->setDescription( text );
		}
	}
	else if( strcmp( name, OPTIONS_FILE_TAG ) == 0 ) {		// </settings-file>
		m_plugin.addOptionsFile( text );
	}
	else if( strcmp( name, PARAMETERS_FILE_TAG ) == 0 ) {	// </parameters-file>
		m_plugin.addParametersFile( text );
	}
	else if( strcmp( name, HELP_FILE_TAG ) == 0 ) {         // </help-file>
		m_currentHelp->setHelpFile( text );
	}
	else if( strcmp( name, STATUS_BAR_NAME ) == 0 ){	    // </status-bar-name>
		m_tempStatusBarInfo.setName( text );

		// 得られたステータスバー名が既に登録されていないか調べる
		unsigned int numberOfStatusBarInfos = (m_currentStatusbarInfo->getPlugin())->getNumberOfStatusBarInfos();
		StatusBarInfo* p_tempStatusBarInfo;
		int flgIcchi = 0;	// 一致したら(既に登録されていれば)「１」

		for( unsigned int i=0; i<numberOfStatusBarInfos; i++ ){
			p_tempStatusBarInfo = (m_currentStatusbarInfo->getPlugin())->getStatusBarInfo( i );
			if ( strcmp( p_tempStatusBarInfo->getName(), text ) ){
				// 不一致
			}else{
				// 一致
				flgIcchi = 1;
				p_tempStatusBarInfo->setTargetVal( m_tempStatusBarInfo.getTargetVal() );	// 得られたステータスバーが既に登録されているので、Target値を追加する
				break;	// 検索終了
			}
		}
		if( flgIcchi == 0 ){
			// 一度も一致しなかった->新たに追加
			m_currentStatusbarInfo = m_plugin.createNewStatusBarInfo();
			m_currentStatusbarInfo->setTargetVal( m_tempStatusBarInfo.getTargetVal() );
			m_currentStatusbarInfo->setOrder( m_tempStatusBarInfo.getOrder() );
			m_currentStatusbarInfo->setName( m_tempStatusBarInfo.getName() );
			m_currentStatusbarInfo->setWidth( m_tempStatusBarInfo.getWidth() );		// @date 2011.10.19 <Add> M.Izumi
		}
	}
	else if( strcmp( name, JAVA_TAG ) == 0 ) {    // </java>
		m_inJava = false;
	}
	else if( strcmp( name, JAVA_CLASSPATH_TAG ) == 0 ) {    // </classpath>
		m_plugin.addJavaClassPath( text );
	}

// >>>>>>	＠Date:2013/08/21	<Delete>	A.Ozaki
//
	// @date 2013/02/21 <Add> OKADA ------->
//	else if( strcmp( name, PACKAGE_TAG ) == 0 ) {	// </package>	
//		if( m_currentCall != NULL ) {
//			m_currentCall->addPackage( text );
//			kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();
//			plgMgr.addPackage( text );
//		}
//	}
//
	// @date 2013/02/21 <Add> OKADA <-------
//
// <<<<<<	@Date:2013/08/21	<Delete>	A.Ozaki

	// >>>>>>	@Date:2013/08/21	<Add>	A.Ozaki
	//
	else if  ( strcmp( name, COMPONENT_NAME_ATTR ) == 0 )	// </component-name>
	{
		if  ( (PluginComponent *)NULL != m_pCurrentComponent )
		{
			m_pCurrentComponent->setName( text );
		}
	}
	else if  ( strcmp( name, COMPONENT_DESC_ATTR ) == 0 )	// </component-desc>
	{
		if  ( (PluginComponent *)NULL != m_pCurrentComponent )
		{
			m_pCurrentComponent->setDesc( text );
		}
	}
	else if  ( strcmp( name, COMPONENT_CALL_ATTR ) == 0 )	// </component-call>
	{
		std::string	strName = FMT( "%s", text );
		if  ( m_mapCall.find( strName ) != m_mapCall.end( ) )
		{
			PluginCall	*pCall = m_mapCall[strName];

			if  ( (PluginCall *)NULL != pCall )
			{
				if  ( (PluginComponent *)NULL != m_pCurrentComponent )
				{
					m_pCurrentComponent->addCall( pCall );
					m_vecCalls.push_back( pCall );
				}
			}
		}
	}
	else if  ( strcmp( name, COMPONENT_TAG ) == 0 )				// </component>
	{
		m_plugin.addComponent( m_pCurrentComponent );

		m_pCurrentComponent = (PluginComponent *)NULL;
	}
	else if  ( strcmp( name, PACKAGE_TAG ) == 0 )				// </package>
	{
		m_strCurrentPackageName.clear( );
	}
	else if  ( strcmp( name, PACKAGE_NAME_ATTR ) == 0 )			// </package-name>
	{
		m_strCurrentPackageName = FMT( "%s", text );
		if  ( false == m_strCurrentPackageName.empty( ) )
		{
			// regist package name to PluginManager
			kome::plugin::PluginManager	&insPluginManager = kome::plugin::PluginManager::getInstance( );
			insPluginManager.addPackage( m_strCurrentPackageName.c_str( ) );
		}
	}
	else if  ( strcmp( name, PACKAGE_COMPONENT_ATTR ) == 0 )	// </package-component>
	{
		if  ( false == m_strCurrentPackageName.empty( ) )
		{
			PluginComponent	*pComponent = m_plugin.getComponent( text );
			if  ( (PluginComponent *)NULL != pComponent )
			{
				pComponent->addPackage( m_strCurrentPackageName.c_str( ) );
			}
		}
	}
	//
	// <<<<<<	@Date:2013/08/21	<Add>	A.Ozaki

}

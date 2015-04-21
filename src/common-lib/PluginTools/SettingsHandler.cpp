/**
 * @file SettingsHandler.cpp
 * @brief implements of SettingsHandler class
 *
 * @author S.Tanaka
 * @date 2007.01.30
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "SettingsHandler.h"

#include "PluginManager.h"
#include "PluginInfo.h"
#include "ParameterSettings.h"
#include "SettingsValue.h"
#include "SettingsGroup.h"
#include "SettingsChapter.h"
#include "SettingsPage.h"
#include "SettingsForm.h"
#include "SettingsParam.h"
#include "PluginFunctionItem.h"


using namespace kome::plugin;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



#define CHAPTER_TAG					"chapter"
#define VALUE_TAG					"value"
#define VALUE_NAME_TAG              "value-name"
#define ENUM_TAG					"enum"
#define ENUM_SECTION_TAG            "enum-section"
#define ENUM_NAME_TAG				"enum-name"
#define ENUM_VALUE_TAG				"enum-value"
#define ENUM_DESC_TAG				"enum-desc"
#define FILTER_TAG					"filter"
#define FILTER_NAME_TAG				"filter-name"
#define FILTER_EXTENSION_TAG		"filter-extension"
#define FILTER_DESC_TAG				"filter-desc"
#define ITEM_TYPE_TAG				"item-type"
#define INI_TYPE_TAG                "enum-key"
#define FUN_TYPE_TAG                "enum-call"
#define EXCLUDE_TAG					"exclude"
#define GROUP_TAG					"group"
#define GROUP_NAME_TAG				"group-name"
#define PARAM_TAG					"param"
#define PARAM_NAME_TAG				"param-name"
#define PARAM_SECTION_TAG			"param-section"
#define PARAM_KEY_TAG				"param-key"
#define PARAM_PREFIX_TAG			"prefix"
#define PAGE_TAG					"page"
#define PAGE_NAME_TAG				"page-name"
#define PAGE_TITLE_TAG				"page-title"
#define CHECK_FUN_TAG               "check-fun"
#define FORM_TAG					"form"
#define FORM_TITLE_TAG				"form-title"
#define FORM_DESC_TAG				"form-desc"
#define FORM_LIST_TAG				"list-menu"
#define	FORM_LIST_EDIT_TAG	        "edit"
#define	FORM_LIST_ADD_TAG	        "add"
#define	FORM_LIST_UPDOWN_TAG	    "updown"
#define	FORM_LIST_DELETE_TAG	    "delete"
#define	FORM_LIST_COPY_TAG	        "copy"
#define	FORM_LIST_SAVE_TAG	        "save"
#define	FORM_LIST_SELECTALL_TAG	    "selectall"
#define	FORM_LIST_EDITNAME_TAG	    "editName"
#define	FORM_LIST_ADDNAME_TAG	    "addName"
#define	FORM_LIST_FILTER	        "filter"


#define PAGE_PROPERTIES_TAG			    "properties-page"	// for Context menu // @date 2012/10/24 <Add> OKADA
#define PAGE_PROPERTIE_TYPE_ATTR	    "type"	// for Context Menu	// @date 2012/10/24 <Add> OKADA

#define PAGE_PROPERTIES_SPEC_VIEW		"spec_view"		// スペクトル波形ウィンドウ		// @date 2012/10/30 <Add> OKADA
#define PAGE_PROPERTIES_CHROM_VIEW		"chrom_view"	// クロマトグラム波形ウィンドウ	// @date 2012/10/30 <Add> OKADA
#define PAGE_PROPERTIES_HEATMAP_VIEW	"heatmap_view"	// ヒートマップ View			// @date 2012/10/30 <Add> OKADA
#define PAGE_PROPERTIES_3D_VIEW			"3d_view"		// 3D View						// @date 2012/10/30 <Add> OKADA

#define CHAPTER_ID_ATTR				"id"
#define CHAPTER_PARENT_ATTR			"parent"
#define CHAPTER_ORDER_ATTR			"order"
#define VALUE_ID_ATTR				"id"
#define VALUE_TYPE_ATTR				"type"
#define VALUE_LIST_ATTR				"list"
#define VALUE_PARENT_ATTR           "parent"
#define VALUE_HIDEALLCHK_ATTR		"hideAllChk"
#define VALUE_DEFAULT_ATTR			"default"
#define VALUE_MIN_INCLUSIVE_ATTR	"minInclusive"
#define VALUE_MAX_INCLUSIVE_ATTR	"maxInclusive"
#define VALUE_MIN_EXCLUSIVE_ATTR	"minExclusive"
#define VALUE_MAX_EXCLUSIVE_ATTR	"maxExclusive"
#define VALUE_REQUIRED_ATTR			"required"
#define ENUM_VALUE_ATTR				"value"
#define PARAM_ID_ATTR				"id"
#define PARAM_VALUE_ATTR			"value"
#define PARAM_GROUP_ATTR			"group"
#define GROUP_ID_ATTR				"id"
#define PAGE_CHAPTER_ATTR			"chapter"
#define PAGE_ORDER_ATTR				"order"
#define PAGE_COMMON_ATTR			"common"
#define FORM_PARAM_ATTR				"param"
#define FORM_GROUP_ATTR				"group"
#define FORM_DESC_IMG_ATTR          "img"
 

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
SettingsHandler::SettingsHandler( ParameterSettings& settings ) : m_settings( settings ) {
	startDocument();
}

// destructor
SettingsHandler::~SettingsHandler() {
}

// start document
void SettingsHandler::onStartDocument() {
	m_currentChapter = NULL;
	m_chapterId = -1;
	m_currentValue = NULL;
	m_currentGroup = NULL;
	m_currentPage = NULL;
	m_currentParam = NULL;
	m_currentForm = NULL;

	m_valueMap.clear();
	m_groupMap.clear();
	m_paramMap.clear();
}

// end document
void SettingsHandler::onEndDocument() { 
	// clear maps
	m_valueMap.clear();
	m_groupMap.clear();
	m_paramMap.clear();
}

// start element
void SettingsHandler::onStartElement( const char* name, kome::core::Properties& attrs ) {
	// plug-in manager
	PluginManager& plgMgr = PluginManager::getInstance();

	// each element
	if( strcmp( name, CHAPTER_TAG ) == 0 ) {		// <chapter>
		// id
		m_chapterId = attrs.getIntValue( CHAPTER_ID_ATTR, -1 );

		// parent
		m_currentChapter = NULL;
		if( m_settings.isIni() ) {
			m_currentChapter = &( plgMgr.getSettingsRootChapter() );
		}
		else {
			m_currentChapter = &( plgMgr.getParametersRootChapter() );
		}
		int parentId = attrs.getIntValue( CHAPTER_PARENT_ATTR, -1 );
		if( m_chapterMap.find( parentId ) != m_chapterMap.end() ) {
			m_currentChapter = m_chapterMap[ parentId ];
		}

		// order
		m_chapterOrder = attrs.getIntValue( CHAPTER_ORDER_ATTR, -1 );
	}
	else if( strcmp( name, VALUE_TAG ) == 0 ) {  // <value>
		// id
		std::string idStr = attrs.getStringValue( VALUE_ID_ATTR, "" );
		int id = int();
		if( isint( idStr.c_str(), 10, &id ) ) {
			// add value
			m_currentValue = m_settings.addValue();
			m_valueMap[ id ] = m_currentValue;

			// set type
			m_currentValue->setType( attrs.getStringValue( VALUE_TYPE_ATTR, "" ) );

			// set list value flag
			int listVal = attrs.getIntValue( VALUE_LIST_ATTR, -1 );
			if( listVal < 0 ) {
				listVal = attrs.getIntValue( VALUE_PARENT_ATTR, -1 );
			}
			SettingsValue* parentValue = NULL;
			if( m_valueMap.find( listVal ) != m_valueMap.end() ) {
				parentValue = m_valueMap[ listVal ];
			}
			if( parentValue != NULL ) {
				parentValue->addChildValue( m_currentValue );
			}

			// set default value
			m_currentValue->setDefaultValue( attrs.getStringValue( VALUE_DEFAULT_ATTR, "" ) );

			if (strcmp(attrs.getStringValue( VALUE_HIDEALLCHK_ATTR, "" ), "true") == 0)
			{
				m_currentValue->setHideAllCheckbox(true);
			}
						
			// set required
			m_currentValue->setRequired( attrs.getBoolValue( VALUE_REQUIRED_ATTR, false ) );

			// set restriction
			std::string valStr;
			double v = double();
			
			valStr = attrs.getStringValue( VALUE_MIN_INCLUSIVE_ATTR, "" );
			if( isdouble( valStr.c_str(), &v ) ) {
				m_currentValue->getNumberRestriction().setInclusiveMinValue( v );
			}

			valStr = attrs.getStringValue( VALUE_MAX_INCLUSIVE_ATTR, "" );
			if( isdouble( valStr.c_str(), &v ) ) {
				m_currentValue->getNumberRestriction().setInclusiveMaxValue( v );
			}

			valStr = attrs.getStringValue( VALUE_MIN_EXCLUSIVE_ATTR, "" );
			if( isdouble( valStr.c_str(), &v ) ) {
				m_currentValue->getNumberRestriction().setExclusiveMinValue( v );
			}

			valStr = attrs.getStringValue( VALUE_MAX_EXCLUSIVE_ATTR, "" );
			if( isdouble( valStr.c_str(), &v ) ) {
				m_currentValue->getNumberRestriction().setExclusiveMaxValue( v );
			}
		}
	}
	else if( strcmp( name, ENUM_TAG ) == 0 ) {		// <enum>
		// clear enum
		m_enumSection.clear();
		m_enumName.clear();
		m_enumVal.clear();
		m_enumDesc.clear();
	}
	else if( strcmp( name, FILTER_TAG ) == 0 ) {		// <filter>
		// clear filter
		m_filterName.clear();
		m_filterExt.clear();
		m_filterDesc.clear();
	}
	else if( strcmp( name, PARAM_TAG ) == 0 ) {		// <param>
		// id
		std::string idStr = attrs.getStringValue( PARAM_ID_ATTR, "" );
		int id = int();
		if( isint( idStr.c_str(), 10, &id ) ) {
			// add parameter
			m_currentParam = m_settings.addParam();
			m_paramMap[ id ] = m_currentParam;

			// set value
			std::string valStr = attrs.getStringValue( PARAM_VALUE_ATTR, "" );
			int val = int();
			if( isint( valStr.c_str(), 10, &val ) ) {
				if( m_valueMap.find( val ) != m_valueMap.end() ) {
					m_currentParam->setValue( m_valueMap[ val ] );
				}
			}
		}
	}
	else if( strcmp( name, GROUP_TAG ) == 0 ) {		// <group>
		// id
		std::string idStr = attrs.getStringValue( GROUP_ID_ATTR, "" );
		int id = int();
		if( isint( idStr.c_str(), 10, &id ) ) {
			// add group
			m_currentGroup = m_settings.addGroup();
			m_groupMap[ id ] = m_currentGroup;
		}
	}
	else if( strcmp( name, PAGE_TAG ) == 0 ) {		// <page>
		// chapter
		int chapterId = attrs.getIntValue( PAGE_CHAPTER_ATTR, -1 );
		SettingsChapter* chapter = NULL;
		if( m_settings.isIni() ) {
			chapter = &( plgMgr.getSettingsRootChapter() );
		}
		else {
			chapter = &( plgMgr.getParametersRootChapter() );
		}
		if( m_chapterMap.find( chapterId ) != m_chapterMap.end() ) {
			chapter = m_chapterMap[ chapterId ];
		}

		// order
		int order = attrs.getIntValue( PAGE_ORDER_ATTR, -1 );

		// common page flag
		bool commonFlag = attrs.getBoolValue( PAGE_COMMON_ATTR, false );

		// create page
		m_currentPage = new SettingsPage( m_settings );

		// set attribute
		chapter->addPage( m_currentPage );
		if( order >= 0 ) {
			m_currentPage->setOrder( order );
		}
		m_currentPage->setCommon( commonFlag );
	}

	// @date 2012/10/24 <Add> OKADA
	else if( strcmp( name, PAGE_PROPERTIES_TAG ) == 0 ) {		// <properties-page>
		std::string m_popupType = attrs.getStringValue( PAGE_PROPERTIE_TYPE_ATTR, "" );	// type=

		PropertiesPageType	type = PROP_NONE;
		
		if( capitalized_strcmp( m_popupType.c_str(), PAGE_PROPERTIES_SPEC_VIEW ) == 0 ){
			type = PROP_SPEC_VIEW;
		}else if( capitalized_strcmp( m_popupType.c_str(), PAGE_PROPERTIES_CHROM_VIEW ) == 0 ){
			type = PROP_CHROM_VIEW;
		}else if( capitalized_strcmp( m_popupType.c_str(), PAGE_PROPERTIES_HEATMAP_VIEW ) == 0 ){
			type = PROP_HEATMAP_VIEW;
		}else if( capitalized_strcmp( m_popupType.c_str(), PAGE_PROPERTIES_3D_VIEW ) == 0 ){
			type = PROP_3D_VIEW;
		}

		m_currentPage->setPropertiesPageType( type );
	}
	// @date 2012/10/24 <Add> OKADA
	else if( strcmp( name, FORM_LIST_TAG ) == 0 ) {		// <list-menu>

		// @date 2012/10/31 <Del> テスト用コメントアウト
		bool m_listEdit = attrs.getBoolValue( FORM_LIST_EDIT_TAG, false );				// edit=
		bool m_listAdd = attrs.getBoolValue( FORM_LIST_ADD_TAG, false );				// add=
		bool m_listUpdown = attrs.getBoolValue( FORM_LIST_UPDOWN_TAG, false );			// updown=
		bool m_listDelete = attrs.getBoolValue( FORM_LIST_DELETE_TAG, false );			// delete=
		bool m_listCopy = attrs.getBoolValue( FORM_LIST_COPY_TAG, false );				// copy=
		bool m_listSave = attrs.getBoolValue( FORM_LIST_SAVE_TAG, false );				// save=
		bool m_listSelectall = attrs.getBoolValue( FORM_LIST_SELECTALL_TAG, false );	// selectall=
		bool m_listFilter = attrs.getBoolValue( FORM_LIST_FILTER, false );				// filter=

		std::string m_listEditname = attrs.getStringValue( FORM_LIST_EDITNAME_TAG, "" );	// editName=
		std::string m_listAddname = attrs.getStringValue( FORM_LIST_ADDNAME_TAG, "" );		// addName=

		m_currentPage->addForm();
		unsigned int unNum = m_currentPage->getNumberOfForms();

		if( 1 <= unNum  ){
			unsigned int index = unNum-1;
			SettingsForm* pSettingForm;
			pSettingForm = m_currentPage->getForm( index );

			pSettingForm->setEditMenuItem( m_listEdit );
			pSettingForm->setAddMenuItem( m_listAdd );
			pSettingForm->setUpDownMenuItem( m_listUpdown );
			pSettingForm->setDeleteMenuItem( m_listDelete );
			pSettingForm->setCopyMenuItem( m_listCopy );
			pSettingForm->setSaveAllMenuItem( m_listSave );
			pSettingForm->setSelectAllMenuItem( m_listSelectall );
			pSettingForm->setFilterMenuItem( m_listFilter );
			pSettingForm->setEditMenuName( m_listEditname );
			pSettingForm->setAddMenuName( m_listAddname );

		}else{
			// 通常あり得ない
		}
	}
	else if( strcmp( name, FORM_TAG ) == 0 ) {		// <form>
		// param
		int paramId = attrs.getIntValue( FORM_PARAM_ATTR, -1 );
		SettingsParam* param = NULL;
		if( m_paramMap.find( paramId ) != m_paramMap.end() ) {
			param = m_paramMap[ paramId ];
		}

		// group
		int groupId = attrs.getIntValue( FORM_GROUP_ATTR, -1 );
		SettingsGroup* group = NULL;
		if( m_groupMap.find( groupId ) != m_groupMap.end() ) {
			group = m_groupMap[ groupId ];
		}

		// add form
		if( m_currentPage != NULL && param != NULL ) {
			m_currentForm = m_currentPage->addForm();

			m_currentForm->setParam( param );
			m_currentForm->setGroup( group );
		}
	}
	else if( strcmp( name, FORM_DESC_TAG ) == 0 ) {    // <form-desc>
		kome::plugin::PluginInfo* plugin = m_settings.getPlugin();
		std::string img = attrs.getStringValue( FORM_DESC_IMG_ATTR, "" );
		if( !img.empty() ) {
			img = getabsolutepath( plugin->getDir(), img.c_str() );
		}
		m_currentForm->setImageFilePath( img.c_str() );
	}
}

// end element
void SettingsHandler::onEndElement( const char* name, const char* text ) {
	// each element
	if( strcmp( name, CHAPTER_TAG ) == 0 ) {		// </chapter>
		if( m_chapterId >= 0 ) {
			// create child chapter
			m_currentChapter = m_currentChapter->getChildChapter( text );
			if( m_chapterOrder >= 0 ) {
				m_currentChapter->setOrder( m_chapterOrder );
			}

			// set to map
			m_chapterMap[ m_chapterId ] = m_currentChapter;
		}
	}
	else if( strcmp( name, VALUE_TAG ) == 0 ) {  // </value>
		m_currentValue = NULL;
	}
	else if( strcmp( name, VALUE_NAME_TAG ) == 0 ) {    // </value-name>
		if( m_currentValue != NULL ) {
			m_currentValue->setName( text );
		}
	}
	else if( strcmp( name, ENUM_TAG ) == 0 ) {		// </enum>
		// add enumerate item
		if( m_currentValue != NULL ) {
			m_currentValue->addEnumItem(
				m_enumName.c_str(),
				m_enumVal.c_str(),
				m_enumDesc.c_str(),
				m_enumSection.c_str()
			);
		}
	}
	else if( strcmp( name, ENUM_SECTION_TAG ) == 0 ) {    // </enum-section>
		m_enumSection = NVL( text, "" );
	}
	else if( strcmp( name, ENUM_NAME_TAG ) == 0 ) {  // </enum-name>
		m_enumName = NVL( text, "" );
		m_enumVal = m_enumName;
		m_enumDesc = m_enumName;
	}
	else if( strcmp( name, ENUM_VALUE_TAG ) == 0 ) {	// </enum-value>
		m_enumVal = NVL( text, "" );
	}
	else if( strcmp( name, ENUM_DESC_TAG ) == 0 ) {	// </enum-desc>
		m_enumDesc = NVL( text, "" );
	}
	else if( strcmp( name, FILTER_TAG ) == 0 ) {		// </filter>
		// add filter item
		if( m_currentValue != NULL ) {
			m_currentValue->addFilterItem(
				m_filterName.c_str(),
				m_filterExt.c_str(),
				m_filterDesc.c_str()
			);
		}
	}
	else if( strcmp( name, FILTER_NAME_TAG ) == 0 ) {  // </filter-name>
		m_filterName = NVL( text, "" );
		m_filterDesc = m_filterName;
	}
	else if( strcmp( name, FILTER_EXTENSION_TAG ) == 0 ) {	// </filter-extension>
		m_filterExt = NVL( text, "" );
	}
	else if( strcmp( name, FILTER_DESC_TAG ) == 0 ) {	// </filter-desc>
		m_filterDesc = NVL( text, "" );
	}
	else if( strcmp( name, ITEM_TYPE_TAG ) == 0 ) { // </item-type>
		m_currentValue->setEnumType( SettingsValue::ENUM_PLUGIN );
		m_currentValue->setEnumKey( text );
	}
	else if( strcmp( name, INI_TYPE_TAG ) == 0 ) { // </enum-key>
		m_currentValue->setEnumType( SettingsValue::ENUM_INI );
		m_currentValue->setEnumKey( text );
	}
	else if( strcmp( name, FUN_TYPE_TAG ) == 0 ) {    // </enum-call>
		m_currentValue->setEnumType( SettingsValue::ENUM_FUNCTION );
		m_currentValue->setEnumKey( text );
	}
	else if( strcmp( name, EXCLUDE_TAG ) == 0 ) {   // </exclude>
		m_currentValue->addExcludeValue( text );
	}
	else if( strcmp( name, PARAM_TAG ) == 0 ) {		// </param>
		m_currentParam = NULL;
	}
	else if( strcmp( name, PARAM_NAME_TAG ) == 0 ) {		// </param-name>
		if( m_currentParam != NULL ) {
			m_currentParam->setName( text );
		}
	}
	else if( strcmp( name, PARAM_SECTION_TAG ) == 0 ) {		// </param-section>
		if( m_currentParam != NULL ) {
			m_currentParam->setSection( text );
		}
	}
	else if( strcmp( name, PARAM_KEY_TAG ) == 0 ) { // </param-key>
		if( m_currentParam != NULL ) {
			m_currentParam->setKey( text );
		}
	}
	else if( strcmp( name, PARAM_PREFIX_TAG ) == 0 ) {		// </prefix>
		if( m_currentParam != NULL ) {
			m_currentParam->setPrefix( text );
		}
	}
	else if( strcmp( name, GROUP_TAG ) == 0 ) {		// </group>
		m_currentGroup = NULL;
	}
	else if( strcmp( name, GROUP_NAME_TAG ) == 0 ) {		// </group-name>
		if( m_currentGroup != NULL ) {
			m_currentGroup->setName( text );
		}
	}
	else if( strcmp( name, PAGE_TAG ) == 0 ) {		// </page>
		m_currentPage = NULL;
	}
	else if( strcmp( name, PAGE_NAME_TAG ) == 0 ) { // </page-name>
		if( m_currentPage != NULL ) {
			m_currentPage->setName( text );
		}
	}
	else if( strcmp( name, PAGE_TITLE_TAG ) == 0 ) {		// </page-title>
		if( m_currentPage != NULL ) {
			m_currentPage->setTitle( text );
		}
	}
	else if( strcmp( name, CHECK_FUN_TAG ) == 0 ) {    // </check-fun>
		if( m_currentPage != NULL ) {
			PluginFunctionItem* fun = new PluginFunctionItem();
			fun->setShortName( text );
			fun->setLongName( "_dummy_" );
			m_currentPage->addCheck( fun );
		}
	}
	else if( strcmp( name, FORM_TAG ) == 0 ) {		// </form>
		m_currentForm = NULL;
	}
	else if( strcmp( name, FORM_TITLE_TAG ) == 0 ) {		// </form-title>
		if( m_currentForm != NULL ) {
			m_currentForm->setTitle( text );
		}
	}
	else if( strcmp( name, FORM_DESC_TAG ) == 0 ) {	// </form-desc>
		if( m_currentForm != NULL ) {
			m_currentForm->setDesc( text );
		}
	}
}

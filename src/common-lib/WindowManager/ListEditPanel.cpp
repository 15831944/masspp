/**
 * @file ListEditPanel.cpp
 * @brief implements of ListEditPanel class
 *
 * @author S.Tanaka
 * @date 2007.09.03
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "ListEditPanel.h"

#include "SettingsPageDialog.h"
#include "ListTool.h"
#include "RawDataFileDialog.h"
#include "SettingsPanel.h"
#include "WindowTool.h"

#include "list_add16x16.xpm"
#include "list_color16x16.xpm"
#include "list_del16x16.xpm"
#include "list_edit16x16.xpm"
#include "list_up16x16.xpm"
#include "list_down16x16.xpm"
#include "list_del_disabled16x16.xpm"
#include "setting_color_disabled16x16.xpm"
#include "list_edit_disabled16x16.xpm"
#include "list_up_disabled16x16.xpm"
#include "list_down_disabled16x16.xpm"


#include <set>
#include <wx/colordlg.h>


using namespace kome::window;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



#define BORDER_SIZE						   3
#define GRID_GAP						   3
#define ALL_BUTTON_CNT                     5
#define BUTTON_SPACE                      10

#define LIST_WIDTH                       200
#define LIST_HEIGHT                       80

#define ID_LIST_CTRL                      65
#define ID_ADD_BUTTON                     66
#define ID_EDIT_BUTTON                    67
#define ID_DEL_BUTTON                     68
#define ID_UP_BUTTON                      69
#define ID_DOWN_BUTTON                    70
#define ID_COLOR_BUTTON                   71
#define ID_IMPORT_BUTTON				  72
#define ID_EXPORT_BUTTON				  73

#define FILE_EXPORT_CAPTION				  "Save As"
#define FILE_IMPORT_CAPTION				  "Select Files"


BEGIN_EVENT_TABLE( ListEditPanel, HelpGetterPanel )
	EVT_MENU( ID_ADD_BUTTON, ListEditPanel::onAdd )
	EVT_MENU( ID_EDIT_BUTTON, ListEditPanel::onEdit )	// @date 2012/11/29 <Add> OKADA
	EVT_MENU( ID_UP_BUTTON, ListEditPanel::onUp )		// @date 2012/11/29 <Add> OKADA
	EVT_MENU( ID_DOWN_BUTTON, ListEditPanel::onDown )	// @date 2012/11/29 <Add> OKADA
	EVT_MENU( ID_DEL_BUTTON, ListEditPanel::onDelete )	// @date 2012.12.17 <Add> M.Izumi

	EVT_LIST_ITEM_ACTIVATED( wxID_ANY, ListEditPanel::onEditDoubleClick )

    EVT_BUTTON( ID_ADD_BUTTON, ListEditPanel::onAdd )
    EVT_BUTTON( ID_EDIT_BUTTON, ListEditPanel::onEdit )
    EVT_BUTTON( ID_DEL_BUTTON, ListEditPanel::onDelete )
	EVT_BUTTON( ID_UP_BUTTON, ListEditPanel::onUp )
	EVT_BUTTON( ID_DOWN_BUTTON, ListEditPanel::onDown )
	EVT_BUTTON( ID_COLOR_BUTTON, ListEditPanel::onColor )
	EVT_BUTTON( ID_IMPORT_BUTTON, ListEditPanel::onImport )
	EVT_BUTTON( ID_EXPORT_BUTTON, ListEditPanel::onExport )
	EVT_UPDATE_UI( ID_EDIT_BUTTON, ListEditPanel::onUpdateSelection )
	EVT_UPDATE_UI( ID_DEL_BUTTON, ListEditPanel::onUpdateSelection )
	EVT_UPDATE_UI( ID_UP_BUTTON, ListEditPanel::onUpdateSelection )
	EVT_UPDATE_UI( ID_DOWN_BUTTON, ListEditPanel::onUpdateSelection )
	EVT_UPDATE_UI( ID_EXPORT_BUTTON, ListEditPanel::onUpdateSelection )
	EVT_LIST_ITEM_SELECTED( ID_LIST_CTRL, ListEditPanel::onListItemsSelected )
	EVT_LIST_ITEM_DESELECTED( ID_LIST_CTRL, ListEditPanel::onListItemsSelected )
	EVT_LIST_ITEM_FOCUSED( ID_LIST_CTRL, ListEditPanel::onListItemsSelected )
	EVT_LIST_ITEM_ACTIVATED( ID_LIST_CTRL, ListEditPanel::onListItemDoubleClicked )
	EVT_CONTEXT_MENU( EditGridListCtrl::onContextMenu )
END_EVENT_TABLE()


// constructor
ListEditPanel::ListEditPanel(
		wxWindow* parent,
		const int id,
		kome::plugin::SettingsForm* form,
		kome::objects::SettingParameterValues* settings,
		const bool titleFlg,
		const bool singleSel,
		const bool hideAllCheckbox
) : HelpGetterPanel( parent, id ), m_params( NULL, false ) {
	// initialize
	m_form = form;
	m_settings = settings;
	m_titleFlg = titleFlg;
	m_singleSel = singleSel;
	m_hideAllCheckbox = hideAllCheckbox;
	m_valueType = VALUES_NORMAL;
	m_page = NULL;
	m_colorIdx = -1;
	m_list = NULL;
	m_color = kome::img::ColorTool::BLACK;
	
	m_addButton = NULL;
	m_editButton = NULL;
	m_delButton = NULL;
	m_upButton = NULL;
	m_downButton = NULL;
	m_colorButton = NULL;
	m_importButton = NULL;
	m_exportButton = NULL;
	m_iSelectedItem = -1;

	kome::plugin::SettingsParam* param = ( form == NULL ? NULL : form->getParam() );
	kome::plugin::SettingsValue* val = ( param  == NULL ? NULL : param->getValue() );
	val = ( val == NULL ? NULL : val->getChildValue( 0 ) );
	if( val != NULL ) {
		val->getEnumeration( m_enumeration );
	}

	// page
	createPage();

	// control
	createControls();
}

// destructor
ListEditPanel::~ListEditPanel() {
}


// get value
std::string ListEditPanel::getValue( const unsigned int listIdx, const unsigned int elmIdx ) {
	// return value
	std::string ret;

	// value
	kome::plugin::SettingsParam* param = ( m_form == NULL ? NULL : m_form->getParam() );
	kome::plugin::SettingsValue* value = ( param == NULL ? NULL : param->getValue() );
	
	// manager
	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();
	 
	// check the list index
	if( listIdx >= m_values.size() ) {
		return ret;
	}

	// separate
	std::vector< std::string > vals;
	kome::plugin::SettingsValue::separateListValue( m_values[ listIdx ].c_str(), vals );

	if( elmIdx < vals.size() ) {
		ret = vals[ elmIdx ];
	}

	ret = kome::plugin::SettingsValue::convertToDisplayingString( ret.c_str() );

	// function item
	if( value != NULL ) {
		kome::plugin::SettingsValue* subVal = value->getChildValue( elmIdx );
		if( subVal != NULL && subVal->getEnumType() == kome::plugin::SettingsValue::ENUM_PLUGIN ) {
			std::string itemType = subVal->getEnumKey();
			std::string itemName = ret;
			std::string subParams;

			std::string::size_type pos = ret.find( "[" );
			if( pos != ret.npos ) {
				itemName = ret.substr( 0, pos );
				subParams = ret.substr( pos );
				subParams = removeBrackets( subParams.c_str() );
				subParams = kome::plugin::SettingsValue::removeOuterBracket( subParams.c_str() );
			}

			// item
			kome::plugin::PluginFunctionItem* item = plgMgr.getFunctionItem( itemType.c_str(), itemName.c_str() );
			if( item != NULL ) {
				ret = item->getLongName();
				if( !subParams.empty() ) {
					ret = FMT( "%s [%s]", ret.c_str(),subParams.c_str() );
				}
			}
		}
	}

	return ret;
}

// get integer value
int ListEditPanel::getIntValue( const unsigned int listIdx, const unsigned int elmIdx ) {
	// string value
	std::string str = getValue( listIdx, elmIdx );

	// integer value
	int val = toint( str.c_str(), 10, -1 );

	return val;
}

// get double value
double ListEditPanel::getDoubleValue( const unsigned int listIdx, const unsigned elmIdx ) {
	// string value
	std::string str = getValue( listIdx, elmIdx );

	// double value
	double val = todouble(  str.c_str(), -1.0 );

	return val;
}

// get value
std::string ListEditPanel::getValue() {
	// get from the list
	std::vector< std::string > values;
	if( m_valueType & VALUES_ENUM ) {    // checked
		CheckGridListCtrl* checkedList = (CheckGridListCtrl*)m_list;
		for( unsigned int i = 0; i < checkedList->getNumberOfData(); i++ ) {
			long long data = checkedList->getData( i );
			if( checkedList->isSelected( data ) ) {
				std::string val = getEnumValue( data );
				values.push_back( val );
			}
		}
	}
	else {    // normal list
		for( unsigned int i = 0; i < m_list->getNumberOfData(); i++ ) {
			long long data = m_list->getData( i );
			values.push_back( m_values[ data ] );
		}
	}

	// return value
	std::string ret;
	for( unsigned int i = 0; i < values.size(); i++ ) {
		std::string val = values[ i ];
		val = kome::plugin::SettingsValue::removeOuterBracket( val.c_str() );

		if( !ret.empty() ) {
			ret.append( LIST_VALUES_DELIMITER );
		}

		if( val.find( "," ) != val.npos || val.find( "\\" ) != val.npos
				|| val.find( "[" ) != val.npos || val.find( "]" ) != val.npos
				|| val.find( ":" ) != val.npos || val.find( "$" ) != val.npos
				|| val.find( "\"" ) != val.npos ) {
			val = FMT( "[%s]", val.c_str() );
		}

		ret.append( val );
	}

	return ret;
}

// set value
void ListEditPanel::setValue( const char* value ) {
	// clear
	if( !m_values.empty() ) {
		m_values.clear();
	}

	std::string s( NVL(  value, "" ) );
		
	// seperate
	kome::plugin::SettingsValue::separateListValue( s.c_str(), m_values );
		
	// refresh
	if( m_list != NULL ) {		
		updateList();
	}
}

// get the number of values
unsigned int ListEditPanel::getNumberOfValues() {
	return m_values.size();
}

// get the number of columns 
unsigned int ListEditPanel::getNumberOfColumns() {
	return m_colNames.size();
}

// get the column name
const char* ListEditPanel::getColumnName( const unsigned int idx ) {
	if( idx >= m_colNames.size() ) {
		return NULL;
	}
	
	return m_colNames[ idx ].first.c_str();
}

// get column type
kome::plugin::SettingsValue::ValueType ListEditPanel::getColumnType( const unsigned int idx ) {
	// value
	kome::plugin::SettingsParam* param = ( m_form == NULL ? NULL : m_form->getParam() );
	kome::plugin::SettingsValue* value = ( param == NULL ? NULL : param->getValue() );
	if( value == NULL ) {
		return kome::plugin::SettingsValue::TYPE_UNKNOWN;
	}

	// type
	if( idx >= value->getNumberOfChildValues() ) {
		return kome::plugin::SettingsValue::TYPE_UNKNOWN;
	}

	return value->getChildValue( idx )->getType();
}

// get the enumeration size
unsigned int ListEditPanel::getEnumSize() {
	return m_enumeration.size();
}

// get the enumeration name
const char* ListEditPanel::getEnumName( const unsigned int idx ) {
	return m_enumeration[ idx ].name.c_str();
}

// get the enumeration value
const char* ListEditPanel::getEnumValue( const unsigned int idx ) {
	return m_enumeration[ idx ].value.c_str();
}

// get color index
int ListEditPanel::getColorIndex() {
	return m_colorIdx;
}

// get color
COLOR ListEditPanel::getColor( const unsigned int idx ) {
	// return value
	COLOR ret = kome::img::ColorTool::BLACK;
	if( m_colorIdx < 0 ) {
		return ret;
	}

	// get the color
	std::string str = getValue( idx, m_colorIdx );
	ret = kome::img::ColorTool::getColor( str.c_str() );

	return ret;
}

// create page
void ListEditPanel::createPage() {
	// initialize
	if( m_page != NULL ) {
		delete m_page;
	}
	m_page = NULL;
	m_colNames.clear();
	m_values.clear();

	// value
	kome::plugin::SettingsParam* param = ( m_form == NULL ? NULL : m_form->getParam() );
	kome::plugin::SettingsValue* value = ( param == NULL ? NULL : param->getValue() );
	if( value == NULL ) {
		return;
	}

	// set value
	const char* defaultVal = ( m_settings == NULL ? "" : m_settings->getParameterValue( param->getName() ) );
	if( defaultVal == NULL ) {
		defaultVal = value->getDefaultValue();
	}
	setValue( defaultVal );

	// get number of child
	const unsigned int num = value->getNumberOfChildValues();

	// type count
	int colorCnt = 0;
	int fileCnt = 0;
	int enumCnt = 0;
	for( unsigned int i = 0; i < num; i++ ) {
		kome::plugin::SettingsValue* child = value->getChildValue( i );

		if( child->getType() == kome::plugin::SettingsValue::TYPE_COLOR ) {
			colorCnt++;
			m_colorIdx = (int)i;
		}
		else if( child->getType() == kome::plugin::SettingsValue::TYPE_INPUT
				|| child->getType() == kome::plugin::SettingsValue::TYPE_RAWDATA ) {
			fileCnt++;
		}

		if( child->getType() == kome::plugin::SettingsValue::TYPE_BOOL && child->isEnumValue() ) {
			enumCnt++;
		}
	}

	// value type
	m_valueType = VALUES_NORMAL;

	if( colorCnt == 1 ) {
		m_valueType = VALUES_WITH_COLOR;
	}
	else {
		m_colorIdx = -1;
	}

	if( fileCnt == 1 && colorCnt + fileCnt == (int)num ) {
		m_valueType |= VALUES_FILE;
	}

	if( num == 1 && enumCnt == 1 ) {
		m_valueType = VALUES_ENUM;
	}

	// page
	m_page = new kome::plugin::SettingsPage( m_params );

	for( unsigned int i = 0; i < num; i++ ) {
		// child value
		kome::plugin::SettingsValue* child = value->getChildValue( i );

		// page of value
		kome::plugin::SettingsValue* val = m_params.addValue();
		val->setType( child->getType() );
		val->setEnumType( child->getEnumType() );
		val->setEnumKey( child->getEnumKey() );
		val->setRequired( child->isRequired() );

		std::vector< kome::plugin::SettingsValue::EnumItem > enumeration;
		child->getEnumeration( enumeration );

		for( unsigned int j = 0; j < enumeration.size(); j++ ) {
			val->addEnumItem( 
				enumeration[ j ].name.c_str(),
				enumeration[ j ].value.c_str(),
				enumeration[ j ].desc.c_str(),
				enumeration[ j ].section.c_str()
			);
		}

		kome::core::NumberRestriction& r0 = child->getNumberRestriction();
		kome::core::NumberRestriction& r1 = val->getNumberRestriction();

		if( r0.hasExclusiveMinValue() ) {
			r1.setExclusiveMinValue( r0.getExclusiveMinValue() );
		}
		if( r0.hasExclusiveMaxValue() ) {
			r1.setExclusiveMaxValue( r0.getExclusiveMaxValue() );
		}
		if( r0.hasInclusiveMinValue() ) {
			r1.setInclusiveMinValue( r0.getInclusiveMinValue() );
		}
		if( r0.hasInclusiveMaxValue() ) {
			r1.setInclusiveMaxValue( r0.getInclusiveMaxValue() );
		}

		// parameter
		kome::plugin::SettingsParam* param = m_params.addParam();

		std::string name = child->getName();
		if( name.empty() ) {
			name = FMT( "value%d", i );
		}

		std::string itemName;
		if( val->getEnumType() == kome::plugin::SettingsValue::ENUM_PLUGIN ) {
			itemName = val->getEnumKey();
		}
		m_colNames.push_back( std::make_pair( name, itemName ) );

		param->setName( name.c_str() );
		param->setValue( val );
		
		// form
		kome::plugin::SettingsForm* form = m_page->addForm();
		form->setParam( param );
		form->setTitle( name.c_str() );
	}
}

// update list
void ListEditPanel::updateList() {
	// update
	if( m_valueType & VALUES_ENUM ) {    // check list
		EditCheckGridListCtrl* checkList = (EditCheckGridListCtrl*)m_list;

		// set
		std::set< std::string > valSet;
		for( unsigned int i = 0; i < m_values.size(); i++ ) {
			valSet.insert( m_values[ i ] );
		}

		// select
		for( unsigned int i = 0; i < checkList->getNumberOfData(); i++ ) {
			long long data = checkList->getData( i );
			std::string val = getEnumValue( data );
			checkList->setSelected( data, ( valSet.find( val ) != valSet.end() ) );
		}

		checkList->Refresh();
	}
	else {    // normal list
		// clear
		m_list->clearData( false );
		m_list->clearDataColor();

		// add data
		for( int i = 0; i < (int)m_values.size(); i++ ) {
			if( m_values[i].compare( "none" ) != 0 ){
				m_list->addData( i, false );
				
				if( m_valueType & VALUES_WITH_COLOR ) {
					m_list->setDataColor( i, getColor( i ) );
				}
			}
		}
			
		// update
		m_list->updateList();
	}
}

// update enumeration
void ListEditPanel::updateEnumeration( const char* section ) {
	// get value
	m_values.clear();
	CheckGridListCtrl* checkList = dynamic_cast< CheckGridListCtrl* >( m_list );
	if( checkList != NULL ) {
		std::vector< long long > selectedData;
		checkList->getSelectedData( selectedData );
		for( unsigned int i = 0; i < selectedData.size(); i++ ) {
			m_values.push_back( m_enumeration[ selectedData[ i ] ].value );
		}
	}

	// value
	kome::plugin::SettingsParam* param = ( m_form == NULL ? NULL : m_form->getParam() );
	kome::plugin::SettingsValue* val = ( param  == NULL ? NULL : param->getValue() );
	val = ( val == NULL ? NULL : val->getChildValue( 0 ) );
	if( val != NULL ) {
		m_enumeration.clear();
		val->getEnumeration( m_enumeration, section );
	}

	// update list
	m_list->clearData( false );
	for( unsigned int i = 0; i < m_enumeration.size(); i++ ) {
		m_list->addData( i, false );
	}
	m_list->updateList();
	updateList();

	// refresh
	Refresh();
}

// process event
void ListEditPanel::processEvent() {
	// parent
	SettingsPanel* panel = dynamic_cast< SettingsPanel* >( GetParent() );

	// event
	if( panel != NULL ) {
		panel->processEvent( m_form );
	}
}

// set values
void ListEditPanel::setValues( const bool update ) {
	// manager
	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();

	// settings
	kome::plugin::SettingsParam* param = ( m_form == NULL ? NULL : m_form->getParam() );
	kome::plugin::SettingsValue* value = ( param == NULL ? NULL : param->getValue() );

	kome::objects::SettingParameterValues settings;
	m_page->setParameters( settings );

	// selected items
	std::vector< int > items;
	ListTool::getSelectedItems( m_list, items );

	// default value
	if( update ) {
		std::vector< std::string > defaultValues;
		defaultValues.resize( m_colNames.size() );
		
		std::string subParam;

		for( unsigned int i = 0; i < items.size(); i++ ) {
			int idx = m_list->getData( items[ i ] );

			for( unsigned int j = 0; j < m_colNames.size(); j++ ) {
				subParam = getValue( idx, j );
				std::string tmp = getEnumPluginName( idx, j );
				if( i == 0 ) {
					defaultValues[ j ] = tmp;
				}
				else if( tmp.compare( defaultValues[ j ] ) != 0 ) {
					defaultValues[ j ] = "";
				}
			}
		}
	

		for( unsigned int j = 0; j < m_colNames.size(); j++ ) {
			settings.setValue( m_colNames[ j ].first.c_str(), defaultValues[ j ].c_str() );
		}
	}

	// dialog
	SettingsPageDialog dlg( this, m_page, &settings );
	if( dlg.ShowModal() != wxID_OK ) {
		return;
	}

	// value
	std::string strVal;
	for( unsigned int j = 0; j < m_colNames.size(); j++ ) {
		// child value
		kome::plugin::SettingsValue* childVal = value->getChildValue( j );

		// separate
		if( j > 0 ) {
			strVal.append( ELEMENT_VALUES_DELIMITER );
		}

		std::string name = m_colNames[ j ].first;
		std::string itemName = m_colNames[ j ].second;

		const char* v = settings.getParameterValue( name.c_str() );
		std::string val = NVL( v, "" );
		bool bflg = false;
		if( !itemName.empty() ) {
			kome::plugin::PluginFunctionItem* item = plgMgr.getFunctionItem( itemName.c_str(), v );
			if( item != NULL ) {
				val = item->getShortName();
				kome::plugin::SettingsPage* page = item->getSettingsPage();
				if( page != NULL ) {
					kome::objects::SettingParameterValues* subSettings = settings.getSubParameterValues( name.c_str(), v );
					std::string subVal = page->getParameterString( *subSettings );

					val = FMT( "%s%s", val.c_str(), subVal.c_str() );
				}
			}
			bflg = true;
		}
		else {
			kome::plugin::SettingsValue::ValueType t = childVal->getType();
			if( t == kome::plugin::SettingsValue::TYPE_DIR || t == kome::plugin::SettingsValue::TYPE_EMAIL
				|| t == kome::plugin::SettingsValue::TYPE_INPUT || kome::plugin::SettingsValue::TYPE_OUTPUT
				|| t == kome::plugin::SettingsValue::TYPE_PASSWD || t == kome::plugin::SettingsValue::TYPE_RAWDATA 
				|| t == kome::plugin::SettingsValue::TYPE_STRING
			) {
				val = kome::plugin::SettingsValue::convertToSavingString( val.c_str() );
			}
		}

		// @date 2012.06.14 <Add> M.Izumi
		int index = val.find_first_of(ELEMENT_VALUES_DELIMITER);
		if( index != -1 && !bflg ){
			val.replace( index, 1, ":" );
		}

		strVal.append( FMT( "[%s]", val.c_str() ) );
	}

	// update
	if( update ) {    // edit
		for( unsigned int i = 0; i < items.size(); i++ ) {
			int idx = m_list->getData( items[ i ] );
			m_values[ idx ] = strVal;

			if( m_colorIdx >= 0 ) {
				m_list->setDataColor( idx, getColor( idx ) );
			}
		}
	}
	else {    // add
		int idx = (int)m_values.size();

		m_values.push_back( strVal );
		m_list->addData( idx, false );
		
		if( m_colorIdx >= 0 ) {
			m_list->setDataColor( idx, getColor( idx ) );
		}
	}
	
	// event
	processEvent();
}

// add files
void ListEditPanel::addFiles() {
	// file index
	int fileIdx = ( m_colorIdx == 0 ? 1 : 0 );

	// file map
	std::map< std::string, int > fileMap;
	for( unsigned int i = 0; i < m_values.size(); i++ ) {
		fileMap[ getValue( i, fileIdx ) ] = (int)i;
	}

	// value
	kome::plugin::SettingsParam* param = ( m_form == NULL ? NULL : m_form->getParam() );
	kome::plugin::SettingsValue* value = ( param == NULL ? NULL : param->getValue() );
	if( value == NULL ) {
		return;
	}

	// files
	std::set< std::string > fileSet;
	for( unsigned int i = 0; i < m_list->getNumberOfData(); i++ ) {
		int idx = m_list->getData( i );
		std::string path = getValue( idx, fileIdx );
		fileSet.insert( path );
	}

	// color
	std::string clStr;
	if( m_colorIdx >= 0 ) {
		clStr = kome::img::ColorTool::getString( m_color );
	}

	// dialog
	std::vector< std::string > paths;

	if( value->getChildValue( fileIdx )->getType() == kome::plugin::SettingsValue::TYPE_INPUT ) {
		wxFileDialog dlg(
			this,
			wxT( FILE_IMPORT_CAPTION ),
			wxEmptyString,
			wxEmptyString,
			wxT( "*.*" ),
			wxFD_OPEN | wxFD_FILE_MUST_EXIST | wxFD_MULTIPLE
		);

		if( dlg.ShowModal() == wxID_OK ) {
			// paths
			wxArrayString pathArray;
			dlg.GetPaths( pathArray );

			for( unsigned int i = 0; i < pathArray.Count(); i++ ) {
				paths.push_back( pathArray.Item( i ).c_str() );
			}
		}
	}
	else {
		RawDataFileDialog dlg( this );
		if( dlg.ShowModal() == wxID_OK ) {
			for( unsigned int i = 0; i < dlg.getNumberOfPaths(); i++ ) {
				std::string p = dlg.getPath( i );
				paths.push_back( kome::plugin::SettingsValue::convertToSavingString( p.c_str() ) );
			}
		}
	}

	for( unsigned int i = 0; i < paths.size(); i++ ) {
		std::string path = paths[ i ];
		if( fileSet.find( path ) == fileSet.end() ) {
			// search the map
			int idx = -1;
			if( fileMap.find( path ) != fileMap.end() ) {
				idx = fileMap[ path ];
			}

			// value
			std::string val = path;
			if( m_colorIdx == 0 ) {
				val = FMT( "%s%s[%s]", clStr.c_str(), ELEMENT_VALUES_DELIMITER, path.c_str() );
			}
			else if( m_colorIdx == 1 ) {
				val = FMT( "[%s]%s%s", path.c_str(), ELEMENT_VALUES_DELIMITER, clStr.c_str() );
			}

			// add
			if( idx < 0 ) {
				idx = (int)m_values.size();
				m_values.push_back( val );
			}
			else {
				m_values[ idx ] = val;
			}

			m_list->addData( idx, false );

			if( m_colorIdx >= 0 ) {
				m_list->setDataColor( idx, getColor( idx ) );
			}

			fileSet.insert( path );
		}
	}

	// event
	if( paths.size() > 0 ) {
		processEvent();
	}
}

// get selected items
void ListEditPanel::getSelectedValues( std::vector< std::string >& values ) {
	// get from the list
	if( m_valueType & VALUES_ENUM ) {    // checked
		CheckGridListCtrl* checkedList = (CheckGridListCtrl*)m_list;
		for( unsigned int i = 0; i < checkedList->getNumberOfData(); i++ ) {
			long long data = checkedList->getData( i );
			if( checkedList->isSelected( data ) ) {
				values.push_back( getEnumValue( data ) );
			}
		}
	}
	else {    // normal list
		for( unsigned int i = 0; i < m_list->getNumberOfData(); i++ ) {
			if( ListTool::isSelected( m_list, i ) ) {
				long long data = m_list->getData( i );
				values.push_back( m_values[ data ] );
			}
		}
	}
}

// on select item
void ListEditPanel::onSelectItem() {
	// Spec No. 86207 Mass++ crashes when adding the default charge in the Simple Charge Detect @date 2012.06.19 <Add> M.Izumi	
	// check the value type
	if( m_valueType & VALUES_ENUM ) {
		return;
	}
	
	const long lSelectedItem =  m_iSelectedItem;

	if( m_editButton != NULL ) {
		m_editButton->Enable();
	}
	
	if( m_delButton != NULL ) {
		m_delButton->Enable();
	}
			
	if( m_upButton != NULL ) {
		m_upButton->Enable( lSelectedItem > 0  );
	}

	if( m_downButton != NULL ) {
		m_downButton->Enable( lSelectedItem < (m_list->GetItemCount() - 1 ));
	}

	
	if( m_valueType == ( VALUES_FILE | VALUES_WITH_COLOR ) && m_colorButton != NULL ) {
		m_colorButton->Enable();		
	}
}

// on double-click item
void ListEditPanel::onDoubleClickItem() {
}

// create main sizer
wxSizer* ListEditPanel::createMainSizer() { 
	// sizer
	wxSizer* sizer = NULL;
	if( m_titleFlg ) {
        sizer = new wxStaticBoxSizer( wxVERTICAL, this, wxT( m_form == NULL ? "" : m_form->getTitle() ) );		
	}
	else {
		sizer = new wxBoxSizer( wxVERTICAL );
	}

	wxSizer* buttonSizer = createButtonSizer();

	wxFlexGridSizer* innerSizer = new wxFlexGridSizer( ( buttonSizer == NULL ? 1 : 2 ), 1, 0, 0 );
	innerSizer->AddGrowableRow( 0 );
	innerSizer->AddGrowableCol( 0 );

	// value
	kome::plugin::SettingsParam* param = ( m_form == NULL ? NULL : m_form->getParam() );
	kome::plugin::SettingsValue* value = ( param == NULL ? NULL : param->getValue() );
	if( value == NULL || m_colNames.size() == 0 ) {
		return sizer;
	}

	// list
	wxWindow* wnd = NULL;
	if( m_valueType == VALUES_ENUM ) {
		CheckGridListPanel* panel = NULL;
		if( getEnumSize() > ALL_BUTTON_CNT ) {
			if (m_hideAllCheckbox)
			{
				panel = new CheckGridListPanel( this, wxID_ANY, CheckGridListPanel::BOTTOM_ALL, "" );
			}
			else
			{
				panel = new CheckGridListPanel( this, wxID_ANY, CheckGridListPanel::BOTTOM_ALL );
			}
			panel->setDescription( m_form->getDesc() );
			panel->setImageFilePath( m_form->getImageFilePath() );
		}

		m_list = new EditCheckGridListCtrl( panel == NULL ? (wxWindow*)this : (wxWindow*)panel );
		wnd = ( panel == NULL ? (wxWindow*)m_list : (wxWindow*)panel );
	}
	else {
		// Context Menu
		m_listEditGridListCtrl = new EditGridListCtrl( this, m_singleSel );	
		m_list = (GridListCtrl*) m_listEditGridListCtrl;
		wnd = m_list;

		std::string strEditMenuName = m_form->getEditMenuName();							// Context Menu
		std::string strAddMenuName = m_form->getAddMenuName();
		bool flgUpDown = m_form->hasUpDownMenuItem();

		EditGridListCtrl* ptrEditGridListCtrl = (EditGridListCtrl*)m_listEditGridListCtrl;	// Context Menu

		int nPopupMenuFlags = 0;
		if( m_form->hasEditMenuItem() ){
			nPopupMenuFlags |= POPUP_EDIT;
			ptrEditGridListCtrl->setEditMenuName( strEditMenuName );
		}
		if( m_form->hasAddMenuItem() ){	
			nPopupMenuFlags |=	POPUP_ADD;	
			ptrEditGridListCtrl->setAddMenuName( strAddMenuName );
		}
		if( m_form->hasUpDownMenuItem() ){	
			ptrEditGridListCtrl->setUpDown( flgUpDown );
			nPopupMenuFlags |=	POPUP_UP;	// 必要？
			nPopupMenuFlags |=	POPUP_DOWN;	// 必要？
		}
		if( m_form->hasDeleteMenuItem() ){	
			nPopupMenuFlags |=	POPUP_DELETE;	
		}
		if( m_form->hasCopyMenuItem() ){	
			nPopupMenuFlags |=	POPUP_COPY;	
		}
		if( m_form->hasSaveAllMenuItem() ){	
			nPopupMenuFlags |=	POPUP_SAVE;	
		}
		if( m_form->hasSelectAllMenuItem() ){	
			nPopupMenuFlags |=	POPUP_SELECT;	
			nPopupMenuFlags |=	POPUP_DESELECT;	
		}
		if( m_form->hasFilterMenuItem() ){	
			nPopupMenuFlags |=	POPUP_FILTER;	
		}
		ptrEditGridListCtrl->setPopupMenuFlag( nPopupMenuFlags );
	}

	if( m_list != NULL ) {
		m_list->create();
	}

	innerSizer->Add( wnd, 1, wxALL | wxGROW, 0 );

	// button sizer
	if( buttonSizer != NULL ) {
		innerSizer->Add( buttonSizer, 0, wxBOTTOM | wxLEFT, BORDER_SIZE );
	}

	sizer->Add( innerSizer, 1, wxALL | wxGROW, 0 );

	return sizer;
}

// create button sizer
wxSizer* ListEditPanel::createButtonSizer() {
	// check the value type
	if( m_valueType & VALUES_ENUM ) {
		return NULL;
	}

	// sizer
	wxBoxSizer* sizer = new wxBoxSizer( wxHORIZONTAL );

	// add button
	wxBitmap addBmp( list_add16x16_xpm );
	m_addButton = new wxBitmapButton( this, ID_ADD_BUTTON, addBmp );
	m_addButton->SetToolTip("Add new item");
	sizer->Add( m_addButton, 0, wxALIGN_CENTER_VERTICAL, BORDER_SIZE );

	// edit button
	if( !( m_valueType & VALUES_FILE ) ) {
		wxBitmap editBmp( list_edit16x16_xpm );
		m_editButton = new wxBitmapButton( this, ID_EDIT_BUTTON, editBmp );
		m_editButton->SetToolTip("Edit selected item");
		wxBitmap editDisableBmp( item_edit_disabled16x16_xpm );
		m_editButton->SetBitmapDisabled( editDisableBmp );
		m_editButton->Enable( false );
		
		sizer->Add( m_editButton, 0, wxALIGN_CENTER_VERTICAL, BORDER_SIZE );		
	}

	// delete button
	wxBitmap delBmp( list_del16x16_xpm );
	m_delButton = new wxBitmapButton( this, ID_DEL_BUTTON, delBmp );
	m_delButton->SetToolTip("Delete selected item");
	wxBitmap delDisableBmp( list_del_disabled16x16_xpm );
	m_delButton->SetBitmapDisabled( list_del_disabled16x16_xpm );
	m_delButton->Enable( false );
	sizer->Add( m_delButton, 0, wxALIGN_CENTER_VERTICAL, BORDER_SIZE );

	sizer->AddSpacer( BUTTON_SPACE );

	// up button
	wxBitmap upBmp( list_up16x16_xpm );
	m_upButton = new wxBitmapButton( this, ID_UP_BUTTON, upBmp );
	m_upButton->SetToolTip("Move selected item up");
	wxBitmap upDisableBmp( list_up_disabled16x16_xpm );
	m_upButton->SetBitmapDisabled( upDisableBmp );
	m_upButton->Enable( false );
	sizer->Add( m_upButton, 0, wxALIGN_CENTER_VERTICAL, BORDER_SIZE );

	// down button
	wxBitmap dwnBmp( list_down16x16_xpm );
	m_downButton = new wxBitmapButton( this, ID_DOWN_BUTTON, dwnBmp );
	m_downButton->SetToolTip("Move selected item down");
	wxBitmap dwnDisableBmp( list_down_disabled16x16_xpm );
	m_downButton->SetBitmapDisabled( dwnDisableBmp );
	m_downButton->Enable( false );
	sizer->Add( m_downButton, 0, wxALIGN_CENTER_VERTICAL, BORDER_SIZE );

	// color button
	if( m_valueType == ( VALUES_FILE | VALUES_WITH_COLOR ) ) {
		sizer->AddSpacer( BUTTON_SPACE );

		wxBitmap clBmp( list_color16x16_xpm );
		m_colorButton = new wxBitmapButton( this, ID_COLOR_BUTTON, clBmp );
		wxBitmap clDisableBmp( setting_color_disabled16x16_xpm );
		m_colorButton->SetBitmapDisabled( clDisableBmp );
		m_colorButton->Enable( false );
		sizer->Add( m_colorButton, 0, wxALIGN_CENTER_VERTICAL, BORDER_SIZE );
	}
	
	// import button
	m_importButton = new wxButton( this, ID_IMPORT_BUTTON, wxT( "Import..." ) );
	sizer->Add( m_importButton, 0, wxALIGN_CENTER_VERTICAL, BORDER_SIZE );

	// export button
	m_exportButton = new wxButton( this, ID_EXPORT_BUTTON, wxT( "Export..." ) );
	sizer->Add( m_exportButton, 0, wxALIGN_CENTER_VERTICAL, BORDER_SIZE );

	return sizer;
}

// get help
std::string ListEditPanel::getDescription( wxWindow* wnd ) {
	// description
	std::string ret = ( m_form == NULL ? "" : m_form->getDesc() );

	return ret;
}

// get image file path
std::string ListEditPanel::getImageFilePath( wxWindow* wnd ) {
	std::string ret;
	return ret;
}

// data -> window
bool ListEditPanel::TransferDataToWindow() {
	// default
	if( !HelpGetterPanel::TransferDataToWindow() ) {
		return false;
	}

	// list
	if( m_valueType & VALUES_ENUM ) {    // check list
		// initialize
		CheckGridListCtrl* checkList = (CheckGridListCtrl*)m_list;
		checkList->deselectAll();

		// value
		kome::plugin::SettingsParam* param = ( m_form == NULL ? NULL : m_form->getParam() );
		kome::plugin::SettingsValue* value = ( param == NULL ? NULL : param->getValue() );
		value = ( value == NULL ? NULL : value->getChildValue( 0 ) );

		if( value != NULL ) {
			// value set
			std::set< std::string > valueSet;
			for( unsigned int i = 0; i < m_values.size(); i++ ) {
				valueSet.insert( m_values[ i ] );
			}

			// select
			for( unsigned int i = 0; i < m_enumeration.size(); i++ ) {
				std::string val = m_enumeration[ i ].value;
				checkList->setSelected( i, ( valueSet.find( val ) != valueSet.end() ) );
			}
		}
	}
	else {    // normal list
		updateList();
	}

	return true;
}

// window -> data
bool ListEditPanel::TransferDataFromWindow() {
	// default
	if( !HelpGetterPanel::TransferDataFromWindow() ) {
		return false;
	}

	// list
	if( m_valueType & VALUES_ENUM ) {    // check list
		CheckGridListCtrl* checkList = (CheckGridListCtrl*)m_list;

		// clear
		m_values.clear();

		// value
		kome::plugin::SettingsParam* param = ( m_form == NULL ? NULL : m_form->getParam() );
		kome::plugin::SettingsValue* value = ( param == NULL ? NULL : param->getValue() );
		value = ( value == NULL ? NULL : value->getChildValue( 0 ) );

		if( value != NULL ) {
			// get selection
			for( unsigned int i = 0; i < m_enumeration.size(); i++ ) {
				if( checkList->isSelected( i ) ) {
					m_values.push_back( m_enumeration[ i ].value );
				}
			}
		}
	}
	else {    // normal list
		// displayed data
		std::vector< std::string > values;

		for( unsigned int i = 0; i < m_list->getNumberOfData(); i++ ) {
			int idx = m_list->getData( i );
			values.push_back( m_values[ idx ] );
		}

		// update
		m_values.clear();

		for( unsigned int i = 0; i < values.size(); i++ ) {
			m_values.push_back( values[ i ] );
		}
	}

	return true;
}

// on add
void ListEditPanel::onAdd( wxCommandEvent& evt ) {
	// deselect all
	ListTool::unselectAll( m_list );

	// add
	if( m_valueType & VALUES_FILE ) {
		addFiles();
	}
	else {
		setValues( false );		
	}
	
	// update
	m_list->updateList();

	// Automatically select the last item added.
	if (m_list->getNumberOfData() > 0)
	{
		ListTool::selectItem( m_list, m_list->getNumberOfData() - 1 );
	}

	m_list->Refresh();
}

// on edit double click
void ListEditPanel::onEditDoubleClick( wxListEvent& evt ){
	 wxCommandEvent evtDummy;
	 onEdit( evtDummy );
}

// on edit
void ListEditPanel::onEdit( wxCommandEvent& evt ) {
	// edit
	setValues( true );

	// selected items
	std::vector< int > items;
	ListTool::getSelectedItems( m_list, items );
	ListTool::unselectAll( m_list );

	// update
	m_list->updateList();

	for( unsigned int i = 0; i < items.size(); i++ ) {
		ListTool::selectItem( m_list, items[ i ] );
	}

	m_list->Refresh();
}

// remove value
void ListEditPanel::removeValue( const long long data ){
	if( m_values.size() < data ){
		return;
	}
		
	m_values.erase( m_values.begin() + data ); // @date 2012.06.28 <Add> M.Izumi
}

// on delete
void ListEditPanel::onDelete( wxCommandEvent& evt ) {
	// selected items
	std::vector< int > items;
	ListTool::getSelectedItems( m_list, items );
	ListTool::unselectAll( m_list );
	
	int iItemToSelect = 999999;

	// delete
	for( int i = (int)items.size() - 1; i >= 0; i-- ) {

		// Set iItemToSelect to the index of the first item selected.
		if ( items[ i ] < iItemToSelect)
		{
			iItemToSelect = items[ i ];
		}

		m_list->deleteData( iItemToSelect, false );	// @date 2012.12.17 <Mod> M.Izumi
	}
	
	updateList();

	// After a deletion, select the item at the same position, or the last item.
	if (iItemToSelect > m_list->GetItemCount() - 1)
	{
		iItemToSelect = m_list->GetItemCount() - 1;
	}
	if (iItemToSelect >= 0)
	{
		ListTool::selectItem( m_list, iItemToSelect );
	}

	// update
	m_list->Refresh();

	// event
	processEvent();
}

// on up
void ListEditPanel::onUp( wxCommandEvent& evt ) {
	// get the selection
	std::vector< int > items;
	ListTool::getSelectedItems( m_list, items );
	m_list->deselectAll();

	// move
	int prev = (int)m_list->getNumberOfData();
	for( int i = (int)items.size() - 1; i >= 0; i-- ) {
		// index
		int idx0 = items[ i ];
		int idx1 = idx0 - 1;

		// move
		if( idx1 == prev ) {    // cannot move
			prev = idx0;
		}
		else {
			m_list->swapData( idx0, idx1, false );
			
			items[ i ] = idx1;
			prev = idx1;
		}
	}

	// update
	m_list->updateList();

	// selection
	for( unsigned int i = 0; i < items.size(); i++ ) {
		
		ListTool::selectItem( m_list, items[ i ] );
	}
	
	m_list->Refresh();

	// event
	processEvent();
}

// on down
void ListEditPanel::onDown( wxCommandEvent& evt ) {
	// get the selection
	std::vector< int > items;
	ListTool::getSelectedItems( m_list, items );
	m_list->deselectAll();

	// move
	int prev = (int)m_list->getNumberOfData();
	for( int i = (int)items.size() - 1; i >= 0; i-- ) {
		// index
		int idx0 = items[ i ];
		int idx1 = idx0 + 1;

		// move
		if( idx1 == prev ) {    // cannot move
			prev = idx0;
		}
		else {
			m_list->swapData( idx0, idx1, false );
			items[ i ] = idx1;
			prev = idx1;
		}
	}

	// update
	m_list->updateList();

	// selection
	for( unsigned int i = 0; i < items.size(); i++ ) {
		ListTool::selectItem( m_list, items[ i ] );
	}
	m_list->Refresh();

	// event
	processEvent();
}

// on color
void ListEditPanel::onColor( wxCommandEvent& evt ) {
	// get color
	wxColourDialog dlg( this );
	if( dlg.ShowModal() != wxID_OK ) {
		return;
	}
	wxColour colour = dlg.GetColourData().GetColour();
	m_color = kome::img::ColorTool::getColor( colour.Red(), colour.Green(), colour.Blue() );
	std::string clStr = kome::img::ColorTool::getString( m_color );

	// selected items
	std::vector< int > items;
	ListTool::getSelectedItems( m_list, items );
	ListTool::unselectAll( m_list );
	if( items.size() == 0 ) {
		return;
	}

	// column
	int clCol = m_colorIdx;
	int pathCol = ( clCol == 0 ? 1 : 0 );

	// edit value
	for( unsigned int i = 0; i < items.size(); i++ ) {
		int idx = m_list->getData( items[ i ] );

		std::string path = getValue( idx, pathCol );
		std::string value;
		if( clCol == 0 ) {
			value = FMT( "%s%s[%s]", clStr.c_str(), ELEMENT_VALUES_DELIMITER, path.c_str() );
		}
		else {
			value = FMT( "[%s]%s%s", path.c_str(), ELEMENT_VALUES_DELIMITER, clStr.c_str() );
		}

		m_values[ idx ] = value;

		m_list->setDataColor( idx, m_color );
	}

	// update
	m_list->updateList();
	for( unsigned int i = 0; i < items.size(); i++ ) {
		ListTool::selectItem( m_list, items[ i ] );
	}
	m_list->Refresh();

	// event
	processEvent();
}

// on import
void ListEditPanel::onImport( wxCommandEvent& evt ){
	
	// value
	kome::plugin::SettingsParam* param = ( m_form == NULL ? NULL : m_form->getParam() );
	kome::plugin::SettingsValue* value = ( param == NULL ? NULL : param->getValue() );
	if( value == NULL ) {
		return;
	}

	wxFileDialog dlg(
		this,
		wxT( FILE_IMPORT_CAPTION ),
		wxEmptyString,
		wxEmptyString,
		wxT( "*.txt" ),
		wxFD_OPEN | wxFD_FILE_MUST_EXIST | wxFD_MULTIPLE
	);
	

	std::vector< std::string > paths;
	if( dlg.ShowModal() == wxID_OK ){
		// paths
		wxArrayString pathArray;
		dlg.GetPaths( pathArray );

		for( unsigned int i = 0; i < pathArray.Count(); i++ ) {
			paths.push_back( pathArray.Item( i ).c_str() );
		}
	}

	std::string msg;
	for( unsigned int i = 0; i < paths.size() && msg.empty(); i++ ) {		
		FILE* fp = fileopen(  paths[ i ].c_str(), "r" );
		if( fp == NULL ){
			return;
		}

		// buffer
		char buff[ 4096 ];
		while( fgets( buff, sizeof(buff), fp ) != NULL && msg.empty() ){
			std::vector< std::string > tmp;
			stringtoken( buff, "\t", tmp );

			std::string v;

			for( unsigned int i = 0; i < value->getNumberOfChildValues(); i++ ) {
				kome::plugin::SettingsValue* childVal = value->getChildValue( i );
				std::string val = ( i < tmp.size() ? tmp[ i ] : "" );
				std::string name = childVal->getName();
				if( name.empty() ) {
					name = FMT( "Value%d", i );
				}

				msg = childVal->checkValue( val.c_str(), name.c_str(), childVal->isRequired() );

				if( !v.empty() ) {
					v.append( ":" );
				}
				val = FMT( "[%s]", val.c_str() );
				v.append( val );
			}

			if( msg.empty() ) {
				m_values.push_back( v );
			}
		}

		// close
		fclose( fp );
	}

	updateList();

	// error message
	if( !msg.empty() ) {
		kome::window::WindowTool::showInfo( msg.c_str() );
	}

	// event
	if( paths.size() > 0 ) {
		processEvent();
	}
}

// on export
void ListEditPanel::onExport( wxCommandEvent& evt ){
	wxFileDialog dialog(
		kome::window::WindowTool::getMainWindow(),
		wxT( FILE_EXPORT_CAPTION ),
		wxEmptyString,
		wxEmptyString,
		wxT( "*.txt" ),
		wxFD_SAVE | wxFD_OVERWRITE_PROMPT
	);

	if( dialog.ShowModal() == wxID_OK ) {
		// get path
		std::string strPath = dialog.GetPath();
		
		// save
		FILE* fp = fileopen( strPath.c_str(), "w" );
		if( fp == NULL ){
			return;
		}

		std::string str = kome::plugin::SettingsValue::removeOuterBracket( getValue().c_str() );
		std::vector< std::string > tokens;
		kome::plugin::SettingsValue::separateListValue( str.c_str(), tokens );

		std::string s;
		for( unsigned int i=0; i < tokens.size(); i++ ){
			std::string line = kome::plugin::SettingsValue::removeOuterBracket( tokens[ i ].c_str() );
			std::vector< std::string > elms;
			kome::plugin::SettingsValue::separateListValue( line.c_str(), elms );

			for( unsigned int j = 0; j < elms.size(); j++ ) {
				std::string elm = kome::plugin::SettingsValue::removeOuterBracket( elms[ j ].c_str() );

				if( j > 0 ) {
					fprintf( fp, "\t" );
				}
				fprintf( fp, "%s", elm.c_str() );
			}

			fprintf( fp, "\n" );
		}
		
		// close
		fflush( fp );
		fclose( fp );
	}

	// event
	processEvent();
}

// update selection
void ListEditPanel::onUpdateSelection( wxUpdateUIEvent& evt ) {
	// get the selection
	std::vector< int > items;
	ListTool::getSelectedItems( m_list, items );
	
	// update
	evt.Enable( items.size() > 0 );
}

// on list item selected
void ListEditPanel::onListItemsSelected( wxListEvent& evt ) {
	m_iSelectedItem = evt.GetIndex();
	// check the color
	if( m_colorIdx >= 0 ) {
		// selected items
		std::vector< int > items;
		ListTool::getSelectedItems( m_list, items );

		// get color
		COLOR color = kome::img::ColorTool::BLACK;
		bool flg = true;
		
		for( unsigned int i = 0; i < items.size() && flg; i++ ) {
			int idx = m_list->getData( items[ i ] );
			COLOR c = getColor( idx );

			if( i == 0 ) {
				color = c;
			}
			else if( flg && c != color ) {
				flg = false;
				color = kome::img::ColorTool::BLACK;
			}
		}

		// set the button color
		m_color = color;
	}

	// select
	onSelectItem();	

	// event
	processEvent();//@date 2013.03.28 <Add> M.Izumi 
	
}

// on list item double-clicked
void ListEditPanel::onListItemDoubleClicked( wxListEvent& evt ) {
	onDoubleClickItem();
}

// remove brackets
std::string ListEditPanel::removeBrackets( const char* s ) {
	// string
	std::string str = NVL( s, "" );

	// count
	int commaCnt = 0;
	for( unsigned int i = 0; i < str.length(); i++ ) {
		if( str[ i ] == ',' ) {
			commaCnt += 1;
		}
	}

	// separate
	str = kome::plugin::SettingsValue::removeOuterBracket( str.c_str() );

	std::vector< std::string > vals;
	kome::plugin::SettingsValue::separateListValue( str.c_str(), vals );

	int commaCnt2 = 0;
	for( unsigned int i = 0; i < vals.size(); i++ ) {
		std::string val = vals[ i ];
		for( unsigned int j = 0; j < val.length(); j++ ) {
			if( val[ j ] == ',' ) {
				commaCnt2 += 1;
			}
		}
	}

	// return value
	std::string ret ;
	for( unsigned int i = 0; i < vals.size(); i++ ) {
		if( !ret.empty() ) {
			if( commaCnt2 < commaCnt ) {
				ret.append( "," );
			}
			else {
				ret.append( ":" );
			}
		}

		std::string val = vals[ i ];
		if( vals.size() > 1 ) {
			val = removeBrackets( val.c_str() );
		}

		ret.append( val );
	}

	if( vals.size() > 1 ) {
		ret = FMT( "[%s]", ret.c_str() );
	}

	return ret;
}

// gets the enumeration plug-in name
std::string ListEditPanel::getEnumPluginName( const unsigned int listIdx, const unsigned int elmIdx ){
	// return value
	std::string ret;
	// value
	kome::plugin::SettingsParam* param = ( m_form == NULL ? NULL : m_form->getParam() );
	kome::plugin::SettingsValue* value = ( param == NULL ? NULL : param->getValue() );
		 
	// check the list index
	if( listIdx >= m_values.size() ) {
		return ret;
	}

	// separate
	std::vector< std::string > vals;
	kome::plugin::SettingsValue::separateListValue( m_values[ listIdx ].c_str(), vals );

	if( elmIdx < vals.size() ) {
		ret = vals[ elmIdx ];
	}

	// function item
	if( value != NULL ) {
		kome::plugin::SettingsValue* subVal = value->getChildValue( elmIdx );
		if( subVal != NULL && subVal->getEnumType() == kome::plugin::SettingsValue::ENUM_PLUGIN ) {
			std::string itemType = subVal->getEnumKey();
			std::string itemName = ret;

			std::string::size_type pos = ret.find( "[" );
			if( pos != ret.npos ) {
				itemName = ret.substr( 0, pos );
			}
			ret = itemName;
		}
	}
	
	return ret;
}




//
// EditGridListCtrl
//

BEGIN_EVENT_TABLE( EditGridListCtrl, GridListCtrl )
END_EVENT_TABLE()


// constructor
EditGridListCtrl::EditGridListCtrl( wxWindow* parent, const bool singleSel )
		: GridListCtrl( parent, ID_LIST_CTRL, LIST_WIDTH, LIST_HEIGHT, false, true, singleSel )  {
    // initialize
    m_panel = dynamic_cast< ListEditPanel* >( parent );

	// Context Menu
	m_strEditMenuName = "";	/** name of the Edit menu */
	m_strAddMenuName = "Add";	/** name of the Add menu */
	m_flgUpDown = false;	/** up-down menu exists or not */

}

// destructor
EditGridListCtrl::~EditGridListCtrl() {
}

// on create
void EditGridListCtrl::onCreate() {
	// columns
	int num = (int)( m_panel == NULL ? 0 : m_panel->getNumberOfColumns() );

	int colorIdx = m_panel->getColorIndex();

	for( int i = 0; i < num; i++ ) {
		if( i != colorIdx ) {
			// type
			kome::plugin::SettingsValue::ValueType valType = m_panel->getColumnType( i );
			GridListCtrl::ColumnDataType type = GridListCtrl::TYPE_STRING;
			if( valType == kome::plugin::SettingsValue::TYPE_INT ) {
				type = GridListCtrl::TYPE_UINT;
			}
			else if( valType == kome::plugin::SettingsValue::TYPE_DOUBLE ) {
				type = GridListCtrl::TYPE_UDOUBLE;
			}

			// add
			addColumn( m_panel->getColumnName( i ), type );
		}
	}
}

// get int value
int EditGridListCtrl::getInt( const long long data, const int column ) const {
	// return value
	int val = -1;

	// column number
	int colorIdx = m_panel->getColorIndex();
	int col = column;
	if( colorIdx >= 0 && column >= colorIdx ) {
		col = column + 1;
	}

	// value
	val = m_panel->getIntValue( data, col );

	return val;
}

// get double value
double EditGridListCtrl::getDouble( const long long data, const int column ) const {
	// return value
	double val = -1.0;

	// column number
	int colorIdx = m_panel->getColorIndex();
	int col = column;
	if( colorIdx >= 0 && column >= colorIdx ) {
		col = column + 1;
	}

	// value
	val = m_panel->getDoubleValue( data, col );

	return val;
}

// get string
std::string EditGridListCtrl::getString( const long long data, const int column ) const {
	// return value
	std::string val;

	// column number
	int colorIdx = m_panel->getColorIndex();
	int col = column;
	if( colorIdx >= 0 && column >= colorIdx ) {
		col = column + 1;
	}

	// value
	val = m_panel->getValue( data, col );

	return val;
}

// The name of the Edit menu is set. The Edit menu is not displayed when a null character is set. 
void EditGridListCtrl::setEditMenuName( std::string strEditMenuName ){
	m_strEditMenuName = strEditMenuName;
};

// This function gets the name of the Edit menu. 
std::string EditGridListCtrl::getEditMenuName( void ){
	return m_strEditMenuName;
};

// The name of the Add menu is set. The Add menu is not displayed when a null character is set. 
void EditGridListCtrl::setAddMenuName( std::string strAddMenuName ){
	m_strAddMenuName = strAddMenuName;
};

// This function gets the name of the Add menu. 
std::string EditGridListCtrl::getAddMenuName( void ){
	return m_strAddMenuName;
};

// This function set whether a up-down menu exists or not. 
void EditGridListCtrl::setUpDown( bool flgUpDown ){
	m_flgUpDown = flgUpDown;
};

// This function returns whether a up-down menu exists or not. 
bool EditGridListCtrl::canUpDown( void ){
	return m_flgUpDown;
};

// create menu
wxMenu* EditGridListCtrl::createMenu() {
	// menu
	wxMenu* menu = NULL;
	size_t uiInsertLocation = 0;

	int nSelectedItemCount = GetSelectedItemCount();	// リストで項目が何個選択されているかを返す
	// リストで項目が1個も選択されていない場合は Add に相当するメニューのみを表示する

	if(nSelectedItemCount == 0){
		// Add に相当するメニューのみを表示
		menu = new wxMenu();
	}else{
		menu = GridListCtrl::createMenu();	// 親クラスのcreateMenu()を呼び出し
	}

	bool flgMenuDisp=false;	// menuを1項目でもセットしたらtrue。SEPARATOR表示判別用。

	if(nSelectedItemCount == 0){
		// Add に相当するメニューのみを表示
	}else{
		if( getActivateName().empty() ){
			;	// 文字列が指定されていない
			uiInsertLocation = 0;
		}else{
			// 文字列が指定されています->先頭に特別menuがある
			uiInsertLocation = 2;
		}
	}

	if(nSelectedItemCount == 0){
		// Add に相当するメニューのみを表示
	}else{
		// EDIT
		if( getEditMenuName().empty() ){
			// EDIT menu 表示しない
		}else{
			menu->Insert( uiInsertLocation, ID_EDIT_BUTTON, wxT( getPopupMenuName(POPUP_EDIT).c_str() ) );
			uiInsertLocation++;
			flgMenuDisp = true;
		}

		// SEPARATOR //////////////////////////////
		if( flgMenuDisp ){
			menu->InsertSeparator( uiInsertLocation );
			uiInsertLocation++;
			flgMenuDisp = false;
		}
	}

	// Add or New
	if( getAddMenuName().empty() ){
		// ADD menu 表示しない
	}else{
		menu->Insert( uiInsertLocation, ID_ADD_BUTTON, wxT( getPopupMenuName(POPUP_ADD).c_str() ) );
		uiInsertLocation++;
		flgMenuDisp = true;
	}

	if(nSelectedItemCount == 0){
		// Add に相当するメニューのみを表示
	}else{
		// SEPARATOR //////////////////////////////
		if( flgMenuDisp ){
			menu->InsertSeparator( uiInsertLocation );
			uiInsertLocation++;
			flgMenuDisp = false;
		}

		// UP/DOWN
		if( canUpDown() ){
			// UP/DOWN menu 表示する
			menu->Insert( uiInsertLocation,  ID_UP_BUTTON, wxT( getPopupMenuName(POPUP_UP).c_str() ) );
			uiInsertLocation++;
			menu->Insert( uiInsertLocation,  ID_DOWN_BUTTON, wxT( getPopupMenuName(POPUP_DOWN).c_str() ) );
			uiInsertLocation++;
			flgMenuDisp = true;
		}else{
			;	// UP/DOWN menu 表示しない
		}
		// SEPARATOR //////////////////////////////
		if( flgMenuDisp ){
			menu->InsertSeparator( uiInsertLocation );
			uiInsertLocation++;
			flgMenuDisp = false;
		}
	}

	return menu;
}

// on context menu
void EditGridListCtrl::onContextMenu( wxContextMenuEvent& evt ){

	GridListCtrl::onContextMenu( evt );
}

// get popup menu name
std::string EditGridListCtrl::getPopupMenuName( PopupMenuFlg flgPopupMenu ){

	std::string strPopupMenuName;

	switch( flgPopupMenu ){
	case POPUP_DELETE:
		strPopupMenuName = "&Delete";
		break;
	case POPUP_COPY:
		strPopupMenuName = "&Copy";
		break;
	case POPUP_SAVE:
		strPopupMenuName = "&Save All...";
		break;
	case POPUP_SELECT:
		strPopupMenuName = "Select &All";
		break;
	case POPUP_DESELECT:
		strPopupMenuName = "&Deselect All";
		break;
	case POPUP_FILTER:
		strPopupMenuName = "&Filter...\t(Ctrl+F)";
		break;
	case POPUP_ADD:
		strPopupMenuName = getAddMenuName();
		break;
	case POPUP_UP:
		strPopupMenuName = "&Up";
		break;
	case POPUP_DOWN:
		strPopupMenuName = "&Down";
		break;
	case POPUP_EDIT:
		strPopupMenuName = getEditMenuName();
		break;
	default:
		break;
	};

	return strPopupMenuName;
};

// @date 2012/12/05 <Add> OKADA ------->
// Context menu	
// on delete data
bool EditGridListCtrl::onDeleteData( const long long data ) {	

	m_panel->removeValue( getData( data ) );
	m_panel->updateList();

	return false;	
}

// @date 2012/12/05 <Add> OKADA <-------



//
// EditCheckGridListCtrl
//

BEGIN_EVENT_TABLE( EditCheckGridListCtrl, CheckGridListCtrl )
END_EVENT_TABLE()


// constructor
EditCheckGridListCtrl::EditCheckGridListCtrl( wxWindow* parent )
		: CheckGridListCtrl( parent, ID_LIST_CTRL, LIST_WIDTH, LIST_HEIGHT, false )  {
    // initialize
	m_panel = NULL;
	if( parent != NULL ) {
		m_panel = dynamic_cast< ListEditPanel* >( parent );
		if( m_panel == NULL ) {
			m_panel = dynamic_cast< ListEditPanel* >( parent->GetParent() );
		}
	}
}

// destructor
EditCheckGridListCtrl::~EditCheckGridListCtrl() {
}

// on create
void EditCheckGridListCtrl::onCreate() {
	// column
	addColumn( m_panel->getColumnName( 0 ), GridListCtrl::TYPE_STRING );

	// data
	for( unsigned int i = 0; i < m_panel->getEnumSize(); i++ ) {
		addData( i, false );
	}
}

// get string
std::string EditCheckGridListCtrl::getString( const long long data, const int column ) const {
	// return value
	std::string val = m_panel->getEnumName( data );

	return val;
}


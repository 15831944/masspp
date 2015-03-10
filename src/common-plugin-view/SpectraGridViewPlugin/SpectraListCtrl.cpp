/**
 * @file SpectraListCtrl.cpp
 * @brief implements of SpectraListCtrl class
 *
 * @author S.Tanaka
 * @date 2007.02.15
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "SpectraListCtrl.h"

#include <boost/bind.hpp>


using namespace kome::view;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



#define INT_COLUMN_WIDTH				40
#define DOUBLE_COLUMN_WIDTH				45
#define STRING_COLUMN_WIDTH				85
#define ICON_WIDTH						16
#define ICON_HEIGHT						16


#define MS1_ICON_NAME					"MS1"
#define MS2_ICON_NAME					"MS2"
#define MS3_ICON_NAME					"MS3"
#define MS4_ICON_NAME					"MS4"
#define MS5_ICON_NAME					"MS5"
#define MS6_ICON_NAME					"MS6"
#define MS7_ICON_NAME					"MS7"
#define MSN_ICON_NAME					"MSn"


#define DELETE_MENU_ID				 (13579)
#define SET_PANEL_MENU_ID		   (13579+1)
#define ID_ACTIVE					  13350

// INIファイル関係	
#define DEF_KEY_SPECTRA_GRID_VIEW			"SPECTRA_GRID_VIEW_SECTION"
#define DEF_SECTION_SPECTRA_GRID_VIEW		"SpectraGridViewValue"
#define DEF_DEFAULT_SPECTRA_GRID_VIEW	    "RT,File Name,Stage,Scan Number,Precursor,TIC,BPI"
#define DEF_SPECIAL_SPECTRA_GRID_VIEW	    "Name"


BEGIN_EVENT_TABLE( SpectraListCtrl, GridListCtrl )
	EVT_LIST_COL_CLICK( wxID_ANY, SpectraListCtrl::onColClick )
	EVT_UPDATE_UI( DELETE_MENU_ID, SpectraListCtrl::onUpdateUI )
	EVT_MENU( SET_PANEL_MENU_ID, SpectraListCtrl::onSettingPanel )
	EVT_UPDATE_UI( SET_PANEL_MENU_ID, SpectraListCtrl::onUpdateUI )
END_EVENT_TABLE()


// constructor
SpectraListCtrl::SpectraListCtrl( wxWindow* parent ) 
		: kome::window::EditGridListCtrl(	(wxWindow*)parent, false ) {
	// initialize
	m_dataSet = NULL;
	m_sample = NULL;
	m_sort = -1;

	pParent = parent;	// @date 2011/03/29 <Add> OKADA

	// Context Menu
	setActivateName( "Open" );
}

// destructor
SpectraListCtrl::~SpectraListCtrl() {
}

// set group
void SpectraListCtrl::setSample( kome::objects::Sample* sample ) {
	// check
	if( sample == m_sample ) {
		return;
	}

	// set sample
	m_sample = sample;

	// set spectra
	m_dataSet = NULL;
	if( sample != NULL ) {
		kome::objects::ActiveObjectsManager& aoMgr = kome::objects::ActiveObjectsManager::getInstance();
		m_dataSet = aoMgr.getFilteredDataSet( sample );
	}

	// create list
	if( m_dataSet == NULL ) {
		clear();
	}
	else {
		setSpectra( *m_dataSet );
	}

	checkType();

	onCreate();
}

// updates title
void SpectraListCtrl::updateTitle() {
	// title
	if( m_dataSet == NULL ) {
		setTitle( "Filtered Spectra" );
	}
	else {
		setTitle( FMT( "Filtered Spectra (%d spectra)", m_dataSet->getNumberOfSpectra() ).c_str() );
	}
}

// get spectra object
kome::objects::DataSet* SpectraListCtrl::getSpectra() {
	return m_dataSet;
}

// update
void SpectraListCtrl::update() {
	if( m_dataSet == NULL ) {
		clear();
	}
	else {
		setSpectra( *m_dataSet );
	}

	// unselect all
	kome::window::ListTool::unselectAll( this );
}

// clear
void SpectraListCtrl::clear() {
	// clear spectra
	m_specArray.clear();
	SetItemCount( 0 );

	// Refresh
	Refresh();
}

// GlidListCtrlクラスのcreate()を走らせないための処理
void SpectraListCtrl::create() {
}

// set spectra
void SpectraListCtrl::setSpectra( kome::objects::DataSet& dataSet ) {
	// check the spectra
	if( dataSet.getNumberOfSpectra() == 0 ) {
		clear();
	}

	// get spectrum array
	m_specArray.clear();
	unsigned int numberOfSpectra = dataSet.getNumberOfSpectra();
	clearData( false );
	for( unsigned int i = 0; i < numberOfSpectra; i++ ) {
		m_specArray.push_back( dataSet.getSpectrum( i ) );
		addData( i, false );
	}

	// update list
	updateList();

	Refresh();
}

// get selected spectra
void SpectraListCtrl::getSelectedSpectra( std::vector< kome::objects::Spectrum* >& selectedSpectra ) {
	// get selected items
	std::vector< int > items;
	kome::window::ListTool::getSelectedItems( this, items );

	// get spectra
	for( unsigned int i = 0; i < items.size(); i++ ) {
		int item = items[ i ];
		if( item >= 0 && item < (int)m_specArray.size() ) {
			selectedSpectra.push_back( m_specArray[ item ] );			
		}
	}
}

// on column click
void SpectraListCtrl::onColClick( wxListEvent& evt ) {
	// column
	int col = evt.GetColumn();

	// check the array
	if( m_specArray.size() == 0 ) {
		return;
	}

	// sort
	std::sort(
		m_specArray.begin(),
		m_specArray.end(),
		boost::bind(
			&SpectraListCtrl::lessSpectrum,
			this,
			_1,
			_2,
			col
		)
	);

	// set sort info
	if( m_sort == col ) {
		m_sort = -1;
	}
	else {
		m_sort = col;
	}

	// refresh
	Refresh();
}

// double clicked
void SpectraListCtrl::onActivateData( const long long data ) {	// dataに選択されたモノが入る
	// get selected spectra
	std::vector< kome::objects::Spectrum* > specArray;
	getSelectedSpectra( specArray );

	// spectrum
	kome::objects::Spectrum* spec = specArray[ 0 ];

	// open
	if( kome::plugin::PluginCallTool::onOpenSpectrum( *spec ) ) {
		kome::plugin::PluginCallTool::openSpectrum( *spec );
	}		
}


// compare to sort
bool SpectraListCtrl::lessSpectrum( kome::objects::Spectrum* spec0, kome::objects::Spectrum* spec1, int col ) {
	// descending order flag
	bool desc = ( m_sort == col );

	// compare
	bool ret = false;
	if( col == 0 || col == 1 ) {	// compare string
		// get string
		std::string s0;
		std::string s1;

		if( col == 0 ) {	// Name
			s0 = NVL( spec0->getName(), "" );
			s1 = NVL( spec1->getName(), "" );
		}
		else if( col == 1 ) {		// File Name
			if( spec0->getSample() != NULL ) {
				s0 = NVL( spec0->getSample()->getSampleSet()->getFileName(), "" );
			}
			if( spec1->getSample() != NULL ) {
				s1 = NVL( spec1->getSample()->getSampleSet()->getFileName(), "" );
			}
		}

		// comare
		int cmp = s0.compare( s1 );
		ret = desc ? ( cmp > 0 ) : ( cmp < 0 );
	}
	else {  // compare number
		double v0 = 0.0;
		double v1 = 0.0;

		if( col == 2 ) {	// stage
			v0 = (double)spec0->getMsStage();
			v1 = (double)spec1->getMsStage();
		}
		else if( col == 3 ) {		// scan number
			v0 = (double)spec0->getScanNumber();
			v1 = (double)spec1->getScanNumber();
		}
		else if( col == 4 ) {		// RT
			v0 = spec0->getRt();
			v1 = spec1->getRt();
		}
		else if( col == 5 ) {		// Precursor
			v0 = spec0->getPrecursor();
			v1 = spec1->getPrecursor();
		}
		else if( col == 6 ) {		// TIC
			v0 = spec0->getTotalIntensity();
			v1 = spec1->getTotalIntensity();
		}
		else if( col == 7 ) {		// BPC
			v0 = spec0->getMaxIntensity();
			v1 = spec1->getMaxIntensity();
		}

		// check empty
		if( v0 < 0.0 && v1 >= 0.0 ) {
			return false;
		}
		if( v0 >= 0.0 && v1 < 0.0 ) {
			return true;
		}

		// compare
		ret = desc ? ( v0 > v1 ) : ( v0 < v1 );
	}

	return ret;
}

// delete selected spectra
void SpectraListCtrl::onDelete( wxCommandEvent& evt ) {
	// check the member
	if( m_dataSet == NULL ) {
		return;
	}

	// get selected spectra
	std::vector< kome::objects::Spectrum* > spectra;
	getSelectedSpectra( spectra );

	// vector -> array
	std::set< kome::objects::Spectrum* > deleteSpectra;
	for( unsigned int i = 0; i < spectra.size(); i++ ) {
		deleteSpectra.insert( spectra[ i ] );
	}
	spectra.clear();

	// spectra object -> vector
	for( unsigned int i = 0; i < m_dataSet->getNumberOfSpectra(); i++ ) {
		spectra.push_back( m_dataSet->getSpectrum( i ) );
	}

	// set spectra
	m_dataSet->clearSpectra();
	for( unsigned int i = 0; i < spectra.size(); i++ ) {
		kome::objects::Spectrum* spec = spectra[ i ];
		if( deleteSpectra.find( spec ) == deleteSpectra.end() ) {
			m_dataSet->addSpectrum( spec );
		}
	}

	// update
	kome::plugin::PluginCallTool::onUpdateDataSet( *m_dataSet );
}

// get item image
int SpectraListCtrl::OnGetItemImage(long item) const {
	// get spectrum
	kome::objects::Spectrum* spec = NULL;
	if( item >= 0 && item < (int)m_specArray.size() ) {
		spec = m_specArray[ item ];
	}
	if( spec == NULL ) {
		return -1;
	}

	// get icon
	kome::window::IconManager& iconMgr = kome::window::IconManager::getInstance();
	int icon = iconMgr.getIconIndex( spec->getIcon() );
	if( icon < 0 ) {
		const char* iconNames[] = {
			MS1_ICON_NAME,		
			MS2_ICON_NAME,
			MS3_ICON_NAME,
			MS4_ICON_NAME,
			MS5_ICON_NAME,
			MS6_ICON_NAME,
			MS7_ICON_NAME,
			MSN_ICON_NAME
		};
		
		// stage
		int stage = spec->getMsStage() - 1;
		stage = CLAMP( stage, 0, 7 );

		std::string iconName = iconNames[ stage ];

		// get icon
		icon = iconMgr.getIconIndex( iconName.c_str() );
		spec->setIcon( iconName.c_str() );
	}

	return icon;
}

// on update UI
void SpectraListCtrl::onUpdateUI( wxUpdateUIEvent& evt ) {
	evt.Enable( true );
}

// create menu
wxMenu* SpectraListCtrl::createMenu() {
	wxMenu* menu = new wxMenu();;
	
	menu->Insert( 0, ID_ACTIVE, wxT("Open") );
	menu->InsertSeparator( 1 );
	menu->Insert( 2, SET_PANEL_MENU_ID, wxT( "&Filtered Spectrum List Column..." ) );	// SPEC No.85760 @date 2012.04.19 <Mod> M.Izumi
	menu->InsertSeparator( 3 );

	return menu;
}

// delete selected spectra
void SpectraListCtrl::onSettingPanel( wxCommandEvent& evt ) {

	// オブジェクト準備
	kome::plugin::ParameterSettings params( NULL, true );
	kome::plugin::SettingsPage* page
		= new kome::plugin::SettingsPage( params );	
	page->setTitle( wxT("Filtered Spectrum List Column") );	// SPEC No.85760 @date 2012.04.19 <Mod> M.Izumi

	// アルゴリズム選択欄
	kome::plugin::SettingsValue* val  = params.addValue();
	val->setType( kome::plugin::SettingsValue::TYPE_LIST );
	val->setDefaultValue( wxT(DEF_DEFAULT_SPECTRA_GRID_VIEW) );

	kome::plugin::SettingsValue* childVal = params.addValue();
	childVal->setType( kome::plugin::SettingsValue::TYPE_STRING );
	childVal->setName( "Column" );
	val->addChildValue( childVal );
	val->setRequired( true );

	// コンボボックスに選択肢を設定
	int nCntOfSpecArray = (int)m_specArray.size();
	if( nCntOfSpecArray <= 0 ){
		return;	// まだ一件も読み込まれていない
	}

	// キーをm_keyTypeから取得
	std::map<std::string, int>::iterator it;
	it = m_keyType.begin();

	while ( it != m_keyType.end() ){
		std::string strKey;
		std::string strKeySpecial = DEF_SPECIAL_SPECTRA_GRID_VIEW;
		strKey = it->first;
		if( strKey == strKeySpecial ){
			// "Name"は項目に追加しない
		}else{
			childVal->addEnumItem( strKey.c_str(), strKey.c_str(), "" );
		}
		it++;
	}


	kome::plugin::SettingsParam* param = params.addParam();
	param->setValue( val );
	param->setName( "Column" );
	param->setSection( DEF_SECTION_SPECTRA_GRID_VIEW );
	param->setKey( DEF_KEY_SPECTRA_GRID_VIEW );
	
	kome::plugin::SettingsForm* form = page->addForm();
	form->setParam( param );
	form->setTitle( "Columns" );
	form->setEditMenuName( "Edit" );
	form->setAddMenuName( "Add" );

	// for Context Menu
	// [Filtered Spectrum List Column]画面
	// 項目が選択されているか?
	bool flgSelected = false;
	for( int i = 0; i < GetItemCount(); i++ ) {
		int nState = GetItemState( i, wxLIST_STATE_SELECTED );
		if( nState == wxLIST_STATE_SELECTED ){
			flgSelected = true;	// 選択されている
			break;
		}
	}
	if( flgSelected ){
		// 項目が選択されている場合のメニュー
		form->setEditMenuItem( true );
		form->setAddMenuItem( true );
		form->setUpDownMenuItem( true );
		form->setSelectAllMenuItem( true );
		form->setDeleteMenuItem( true );
		form->setCopyMenuItem( false );
		form->setSaveAllMenuItem( false );
		form->setFilterMenuItem(false );
		
	}else{
		// 何も選択されていない場合のメニュー
		form->setEditMenuItem( false );
		form->setAddMenuItem( true );
		form->setUpDownMenuItem( false );
		form->setDeleteMenuItem( false );
		form->setCopyMenuItem( false );
		form->setSaveAllMenuItem( false );
		form->setSelectAllMenuItem( false );
		form->setFilterMenuItem( false );
		
	}

	// ダイアログ表示
	kome::objects::SettingParameterValues values;
	kome::window::SettingsPageDialog dlg( pParent, page, &values );
	int ret = dlg.ShowModal();
	if( ret == wxID_OK ){
		onCreate();
	}

}

// on create
void SpectraListCtrl::onCreate() 
{

	// INIファイル読み込み
    kome::core::MsppManager& msppMgr = kome::core::MsppManager::getInstance();
	kome::core::IniFile* ini = msppMgr.getIniFile();

	std::string strSpectraGridViewValue = ini->getString( DEF_SECTION_SPECTRA_GRID_VIEW, DEF_KEY_SPECTRA_GRID_VIEW, DEF_DEFAULT_SPECTRA_GRID_VIEW );
	std::string strSpectraGridViewValueOrg = strSpectraGridViewValue;	// 元の文字列を保存しておく

	// strSpectraGridViewValue には、「Name」が含まれないので、先頭に追加する
	std::string top = DEF_SPECIAL_SPECTRA_GRID_VIEW;
	std::string separator = LIST_VALUES_DELIMITER;
	strSpectraGridViewValue = top + separator + strSpectraGridViewValueOrg;


	const char * p_szSpectraGridViewValue;
	const char* delim = LIST_VALUES_DELIMITER;
	p_szSpectraGridViewValue = strSpectraGridViewValue.c_str();

	// カラムは最初に全部クリア
	while( GetColumnCount() ){
		DeleteColumn(0);
	}

	// 各項目を切り出して配列に入れる
	m_vecStrSpecGrid.clear();
	int tokenCnt=0;
	ColumnDataType type;
    int format;
	int width;

    for(const char *token=strtok( (char *)p_szSpectraGridViewValue, delim );
        token;
        token = strtok( 0, delim )){
			m_vecStrSpecGrid.push_back( token );

			// Default value
			type = TYPE_STRING;			// string
			format = wxLIST_FORMAT_LEFT;// 左寄せ
			width = STRING_COLUMN_WIDTH;// 文字列幅

			std::map<std::string, int>::iterator it;
			it = m_keyType.find( token );
			if( it == m_keyType.end() ){
				// 見つからなかった(通常ない)
			}else{
				// キーが見つかった
				if( (*it).second == CD_TYPE_INT ){
					type = TYPE_INT;				// integer
					format = wxLIST_FORMAT_RIGHT;	// 右寄せ
					width = INT_COLUMN_WIDTH;		// double幅
				}else if( (*it).second == CD_TYPE_DOUBLE ){
					type = TYPE_DOUBLE;				// double
					format = wxLIST_FORMAT_RIGHT;	// 右寄せ
					width = DOUBLE_COLUMN_WIDTH;	// double幅
				}else{
					// 文字列
				}
			}

			// insert column
			long lVal = InsertColumn( tokenCnt, token, format, width );
			// set type
			setHeaderTitle( tokenCnt, token );
			setDataType( tokenCnt, type );

			tokenCnt++;
    }

	// image list
	kome::window::IconManager& iconMgr = kome::window::IconManager::getInstance();
	wxImageList* imageList = iconMgr.createImageList( ICON_WIDTH, ICON_HEIGHT );
	AssignImageList( imageList, wxIMAGE_LIST_SMALL );

	return;
}

// get string
std::string SpectraListCtrl::getString( const long long data, const int column ) const 
{
	std::string str;

	// column名を取得
	if( (int)m_vecStrSpecGrid.size() < column ){
		// error
		return "";
	}
	std::string title = m_vecStrSpecGrid[column];

	int sizeOfSpec = m_specArray.size();
	if( data < sizeOfSpec ){
		kome::core::Properties props;
		m_specArray[data]->getProperties( props );
		str = props.getStringValue( title.c_str(), "" ); // SPEC No.86058 @date 2012.06.11 <Mod> M.Izumi
	}else{
		// NG 
	}

	return str;
}

// get Integer
int SpectraListCtrl::getInt( const long long data, const int column ) const{
	int ret = INT_MIN;

	// column名を取得
	if( (int)m_vecStrSpecGrid.size() < column ){
		// error
		return ret;
	}
	std::string title = m_vecStrSpecGrid[column];

	int sizeOfSpec = m_specArray.size();
	if( data < sizeOfSpec ){
		kome::core::Properties props;
		m_specArray[data]->getProperties( props );
		ret = props.getIntValue( title.c_str(), INT_MIN );
	}else{
		// NG 
	}

	return ret;
}

// get double
double SpectraListCtrl::getDouble( const long long data, const int column ) const{
	double ret = - FLT_MAX;

	// column名を取得
	if( (int)m_vecStrSpecGrid.size() < column ){
		// error
		return ret;
	}
	std::string title = m_vecStrSpecGrid[column];

	int sizeOfSpec = m_specArray.size();
	if( data < sizeOfSpec ){
		kome::core::Properties props;
		m_specArray[data]->getProperties( props );
		ret = props.getDoubleValue( title.c_str(), INT_MIN );
	}else{
		// NG 
	}

	return ret;
}

// checkType :　各キーに対してすべての値をチェックし、キーがINTかdoubleかstringかを調べます
void SpectraListCtrl::checkType( void )
{
	kome::objects::Sample* sample = m_sample;
	kome::core::Properties props;

	if( sample != NULL ){
		for( unsigned int i = 0; i < sample->getNumberOfGroups(); i++ ) {
			kome::objects::DataGroupNode* group = sample->getGroup( i );
			props = group->getProperties();
			checkTypeProp( props );
		}
	}

	// m_specArrayのキーの取得
	int sizeOfSpec = m_specArray.size();
	for( int j=0; j<sizeOfSpec; j++ ){
		props = m_specArray[j]->getProperties();	// 親groupを検索しない
		checkTypeProp( props );
	}

	return;
}

// checkTypeProp
void SpectraListCtrl::checkTypeProp( kome::core::Properties & props )
{
	const char * szEmpty = "empty";
	int cdType;

	unsigned int cntProp = props.getNumberOfProperties();
	for ( unsigned int i=0; i<cntProp; i++ ){
		std::string strKey;
		std::string strVal;
		std::map<std::string, int>::iterator it;

		strKey = props.getKey( i );

		cdType = CD_TYPE_INT;

		it = m_keyType.find( strKey );
		if( it == m_keyType.end() ){
			// 見つからなかった
		}else{
			// キーが見つかった
			cdType = (*it).second;
			if( cdType == CD_TYPE_STRING ){
				continue;
			}
		}

		strVal = props.getStringValue( strKey.c_str(), szEmpty );

		bool blIsInt = isint( strVal.c_str() );
		if( (cdType==CD_TYPE_INT) && blIsInt ){
			// int
		}else if( isdouble( strVal.c_str() ) ) {
			// double
			cdType = CD_TYPE_DOUBLE;
		}else{
			cdType = CD_TYPE_STRING;	// loop out
		}

		it = m_keyType.find( strKey );
		if( it == m_keyType.end() ){
			// 見つからなかった
			m_keyType.insert( std::pair<std::string, int>(strKey, cdType) );
		}else{
			// キーが見つかった
			m_keyType[ strKey ] = cdType;
		}
	}
}

// get popup menu name
std::string SpectraListCtrl::getPopupMenuName( PopupMenuFlg flgPopupMenu ){

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

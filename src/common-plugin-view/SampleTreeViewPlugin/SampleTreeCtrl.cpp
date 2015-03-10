/**
 * @file SampleTreeCtrl.cpp
 * @brief implements of SampleTreeCtrl class
 *
 * @author S.Tanaka
 * @date 2008.01.10
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "SampleTreeCtrl.h"
#include "SampleTreeData.h"
#include "SampleTreeManager.h"	// @date 2012/11/08 <Add> OKADA	for Context Menu

#include <wx/notebook.h>		// @date 2012/11/08 <Add> OKADA	for Context Menu
#include <boost/bind.hpp>


using namespace kome::view;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// @date 2011.05.10 <Add> M.Izumi ->
#define STATUS_PARAM_NAME			 "WINDOW"

#define STATUS_PARAM_VALUE_SINGLE	 "Canvas"
#define PLUGIN_NAME_SINGLE			 "Profile Canvas"
#define ICON_NAME_SINGLE			 "single_cursor"

#define STATUS_PARAM_VALUE_OVERLAP	 "Overlapping"
#define PLUGIN_NAME_OVERLAP			 "Overlapping Canvas"
#define ICON_NAME_OVERLAP			 "overlapping_cursor"

#define STATUS_PARAM_VALUE_STACKED	 "vertical"
#define PLUGIN_NAME_STACKED			 "Vertical Box Canvas Plugin"
#define ICON_NAME_STACKED			 "vertical_box_cursor"
// @date 2011.05.10 <Add> M.Izumi <-

// for context menu
#define ID_POPUP_MENU_COLLAPSE		60001
#define ID_POPUP_MENU_EXPAND		60002

#define STR_POPUP_MENU_COLLAPSE		"Collapse"
#define STR_POPUP_MENU_EXPAND		"Expand"


// event
BEGIN_EVENT_TABLE( SampleTreeCtrl, wxTreeCtrl )
	EVT_LEFT_DCLICK( SampleTreeCtrl::onDblClicked )
	EVT_RIGHT_UP( SampleTreeCtrl::onMouseRightUp )	// for context menu
	EVT_RIGHT_DOWN( SampleTreeCtrl::onMouseRightDown ) //　@date 2012/11/07 <Comment> RIGHT_DOWNイベントを設定しないとRIGHT_UPイベントが発生しないようなので、こうした
	EVT_MOTION( SampleTreeCtrl::onMouseMove )
	EVT_MENU( ID_POPUP_MENU_COLLAPSE, SampleTreeCtrl::onCollapse )	// for context menu
	EVT_MENU( ID_POPUP_MENU_EXPAND, SampleTreeCtrl::onExpand )		// for context menu
	EVT_MENU_RANGE( ID_POPUP_MENU_MIN, ID_POPUP_MENU_MAX, SampleTreeCtrl::onMenuPopup )
	EVT_TREE_ITEM_GETTOOLTIP( wxID_ANY, SampleTreeCtrl::onTooltip )
	EVT_TREE_ITEM_EXPANDING( wxID_ANY, SampleTreeCtrl::onTreeExpanding )	// @Date:2013/07/16	<Add>	A.Ozaki
END_EVENT_TABLE()

// constructor
SampleTreeCtrl::SampleTreeCtrl( wxWindow* parent )
	: wxTreeCtrl( 
		parent,
		wxID_ANY,
		wxDefaultPosition,
		wxSize( 200, 300 ),
		wxTR_SINGLE | wxTR_ROW_LINES | wxTR_HAS_BUTTONS | wxTR_LINES_AT_ROOT
	) {

	m_menuContext = NULL;
}

// destructor
SampleTreeCtrl::~SampleTreeCtrl() {   
}

// on double clicked
void SampleTreeCtrl::onDblClicked( wxMouseEvent& ) {	// @date 2013/06/10 <Mod> OKADA
	// manager
	SampleTreeManager& mgr = SampleTreeManager::getInstance();

	// get selection
	wxTreeItemId id = GetSelection();
	if( !id.IsOk() ) {
		return;
	}

	// get data
	SampleTreeData* data = (SampleTreeData*)GetItemData( id );
	if( data == NULL ) {
		return;
	}

	// set flag
	mgr.setTreeActionFlag( true );

	// open
	if( data->getData().type == kome::objects::Variant::SPECTRUM ) {	// spectrum
		// get spectrum
		kome::objects::Spectrum* spec
			= (kome::objects::Spectrum*)data->getData().prim.pt;

		if( spec != NULL ) {			
			spec->setOperationFlag( true );	// set operation flag @date 2013.02.27 <Add> M.Izumi
			if( kome::plugin::PluginCallTool::onOpenSpectrum( *spec ) ) {
				kome::plugin::PluginCallTool::openSpectrum( *spec );
			}
		}
	}
	else if( data->getData().type == kome::objects::Variant::CHROMATOGRAM ) {	// chromatogram
		// get chromatogram
		kome::objects::Chromatogram* chrom
			= (kome::objects::Chromatogram*)data->getData().prim.pt;
		if( chrom != NULL ) {
			chrom->setOperationFlag( true );	// set operation flag @date 2013.02.27 <Add> M.Izumi
			if( kome::plugin::PluginCallTool::onOpenChromatogram( *chrom ) ) {
				kome::plugin::PluginCallTool::openChromatogram( *chrom );
			}
		}
	}
	else if( data->getData().type == kome::objects::Variant::DATA_GROUP_NODE ) {	// DATA_GROUP_NODE
		// data group
		bool flgExpand = getNowTree()->IsExpanded( id );
	
		// Spec No.93242 : Sample Tree内のMS2が全部開く  @date 2013.06.27 <Mod> M.Izumi ->
		if( flgExpand == true ){
			getNowTree()->Collapse( id ); // tree を収納
		}else{
			getNowTree()->Expand( id );  // tree を開く
		}
		// @date 2013.06.27 <Mod> M.Izumi <-
	}

	// restore flag
	mgr.setTreeActionFlag( false );

}

// mouse move
void SampleTreeCtrl::onMouseMove( wxMouseEvent& ) {	// @date 2013/06/10 <Mod> OKADA
	// manager object	
	kome::objects::StatusManager& statusMgr = kome::objects::StatusManager::getInstance();
	kome::plugin::PluginManager& pluginMgr = kome::plugin::PluginManager::getInstance();

	kome::plugin::PluginInfo* info;
	kome::plugin::PluginIcon* icon;

	std::string str = statusMgr.getValue( STATUS_PARAM_NAME );

	// single
	if( str.compare( STATUS_PARAM_VALUE_SINGLE ) == 0 ){		
		info = pluginMgr.getPlugin( PLUGIN_NAME_SINGLE );
		if( info != NULL ){
			icon = info->getIcon( ICON_NAME_SINGLE );
			if( icon != NULL ){
				wxBitmap bitmap( wxT(icon->getPath()), wxBITMAP_TYPE_PNG );
				wxImage img( bitmap.ConvertToImage() );
		
				// set cursor
				SetCursor(wxCursor(img));
			}
		}
	// overlapping
	}else if( str.compare( STATUS_PARAM_VALUE_OVERLAP ) == 0 ){
		info = pluginMgr.getPlugin( PLUGIN_NAME_SINGLE );
		if( info != NULL ){
			icon = info->getIcon( ICON_NAME_SINGLE );
			if( icon != NULL ){
				wxBitmap bitmap( wxT(icon->getPath()), wxBITMAP_TYPE_PNG );
				wxImage img( bitmap.ConvertToImage() );
		
				// set cursor
				SetCursor(wxCursor(img));
			}
		}
	// stacked
	}else if( str.compare( STATUS_PARAM_VALUE_STACKED ) == 0 ){
		info = pluginMgr.getPlugin( PLUGIN_NAME_SINGLE );
		if( info != NULL ){
			icon = info->getIcon( ICON_NAME_SINGLE );
			if( icon != NULL ){
				wxBitmap bitmap( wxT(icon->getPath()), wxBITMAP_TYPE_PNG );
				wxImage img( bitmap.ConvertToImage() );
		
				// set cursor
				SetCursor(wxCursor(img));
			}
		}
	}else{
		SetCursor(wxCursor(wxCURSOR_ARROW)); // 矢印
	}
}

void SampleTreeCtrl::onMouseRightDown( wxMouseEvent& evt )
{
	// Context Menu / 右クリック時、マウスカーソルがある位置を選択する
	wxPoint wxpRightClickPoint = evt.GetPosition();
	wxTreeItemId wxtId = wxTreeCtrl::HitTest( wxpRightClickPoint );
	wxTreeCtrl::SelectItem( wxtId );

	return;
}

void SampleTreeCtrl::onMouseRightUp( wxMouseEvent& evt )
{
	bool flgButtonUp = evt.ButtonUp();

	if( flgButtonUp == true ){
		;	// OK
	}else{
		return;	// UPイベントがほしいので、何もしない
	}

	onContextMenu( evt );

	return;
}


// context menu
void SampleTreeCtrl::onContextMenu( wxMouseEvent& evt ) {

	// get position
	wxPoint ptScreen = evt.GetPosition();
	wxPoint pt;

	pt = ptScreen;	

	// get selection
	wxTreeItemId id = GetSelection();
	if( !id.IsOk() ) {
		return;
	}

	// get data
	SampleTreeData* data = (SampleTreeData*)GetItemData( id );
	if( data == NULL ) {
		return;
	}

	// create menu
	wxMenu* menu = createMenu( data->getData().type );
	if( menu == NULL ) {
		return;
	}

	// popup menu
	PopupMenu( menu, pt.x, pt.y );
	delete menu;
}

wxTreeCtrl* SampleTreeCtrl::getNowTree(void){
	wxTreeCtrl* ptrTree = NULL;

	//// get managers
	kome::view::SampleTreeManager& treeMgr = SampleTreeManager::getInstance();
	
	wxNotebook* ptrBook = treeMgr.getNoteBook();

	int nSelectedPage = ptrBook->GetSelection();
	if( nSelectedPage == -1 ){
		// error
	}else{
		kome::objects::Sample* ptrSample = treeMgr.getSample( nSelectedPage );
		ptrTree = treeMgr.getTreeCtrl( ptrSample );
	}

	return ptrTree;
}

// creates menu
wxMenu* SampleTreeCtrl::createMenu( kome::objects::Variant::DataType type )
{
	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();

	// get selection
	wxTreeItemId id = GetSelection();
	if( !id.IsOk() ) {
		return NULL;
	}

	wxMenu* ptrMenu = NULL;

	if( type == kome::objects::Variant::SPECTRUM ){
		// SubView
		kome::plugin::PluginMenu& plugin_menu = plgMgr.getSpectrumItemMenu();
		m_menuContext = kome::window::Canvas::createPopupMenu( &plugin_menu, ptrMenu );
	}else if( type == kome::objects::Variant::CHROMATOGRAM ){
		// not sub view
		kome::plugin::PluginMenu& plugin_menu = plgMgr.getChromatogramItemMenu();
		m_menuContext = kome::window::Canvas::createPopupMenu( &plugin_menu, ptrMenu );
	}else if( type == kome::objects::Variant::DATA_GROUP_NODE ){
		// data group
		bool flgExpand = getNowTree()->IsExpanded( id );
		std::string strCollapseExpand;
		int nItemId = 0;
		if( flgExpand == true ){
			strCollapseExpand = STR_POPUP_MENU_COLLAPSE;
			nItemId = ID_POPUP_MENU_COLLAPSE;
		}else{
			strCollapseExpand = STR_POPUP_MENU_EXPAND;
			nItemId = ID_POPUP_MENU_EXPAND;
		}
		kome::plugin::PluginMenu& plugin_menu = plgMgr.getGroupItemMenu();
		ptrMenu = new wxMenu();
	
		ptrMenu->Append( nItemId, strCollapseExpand.c_str() );

		// Separator
		ptrMenu->AppendSeparator();

		m_menuContext = kome::window::Canvas::createPopupMenu( &plugin_menu, ptrMenu );


	}else{
		return NULL;
	}

	return m_menuContext;
}


// on collapse
void SampleTreeCtrl::onCollapse( wxCommandEvent& ){	// @date 2013/06/10 <Mod> OKADA
	// @date 2013.06.27 <Mod> M.Izumi
	// get selection
	wxTreeItemId id = GetSelection();
	if( !id.IsOk() ) {
		return;
	}

	getNowTree()->Collapse( id );

	return;
}

// on expand
void SampleTreeCtrl::onExpand( wxCommandEvent& ){	// @date 2013/06/10 <Mod> OKADA
	// @date 2013.06.27 <Mod> M.Izumi
	// get selection
	wxTreeItemId id = GetSelection();
	if( !id.IsOk() ) {
		return;
	}

	getNowTree()->Expand( id );

	return;
}

// on menu popup
void SampleTreeCtrl::onMenuPopup( wxCommandEvent& evt )
{
	// ID
	int nEventId = evt.GetId();

	wxTreeItemId id = GetSelection();
	if( !id.IsOk() ) {
		return;
	}

	// get data
	SampleTreeData* data = (SampleTreeData*)GetItemData( id );
	if( data == NULL ) {
		return;
	}

	kome::plugin::PluginMenu* ptrMenu = kome::window::Canvas::m_contextMenuTreeMap[nEventId];
	ptrMenu->execute( data->getData() );

	return;
}

// set tooltip
void SampleTreeCtrl::onTooltip( wxTreeEvent& evt ) {
	// get data
	wxTreeItemId id = evt.GetItem();
	SampleTreeData* data = (SampleTreeData*)GetItemData( id );
	if( data == NULL ) {
		return;
	}

	// tooltip
	std::string s;

	if( data->getData().type == kome::objects::Variant::SPECTRUM ) {
		// spectrum
		kome::objects::Spectrum* spec
			= (kome::objects::Spectrum*)data->getData().prim.pt;

		// name
		s = spec->getName();

		// RT & Precursor
		bool flg = false;
		if( spec->getRt() >= 0.0 ) {
			flg = true;
			s.append( FMT( " (RT=%f", spec->getRt() ) );
		}

		if( spec->getMsStage() > 1 && spec->getPrecursor() >= 0.0 ) {
			s.append( flg ? ", " : " (" );
			s.append( FMT( "Precursor=%f", spec->getPrecursor() ) );
			flg = true;
		}

		if( flg ) {
			s.append( ")" );
		}

		// title
		s.append( " : " );
		s.append( spec->getTitle() );
	}

	evt.SetToolTip( s.c_str() );
}

// >>>>>>	@Date:2013/07/16	<Add>	A.Ozaki
//
#define	MS1_ICON_NAME			 "MS1"
#define	MS2_ICON_NAME			 "MS2"
#define	MS3_ICON_NAME			 "MS3"
#define	MS4_ICON_NAME			 "MS4"
#define	MS5_ICON_NAME			 "MS5"
#define	MS6_ICON_NAME			 "MS6"
#define	MS7_ICON_NAME			 "MS7"
#define	MSN_ICON_NAME			 "MSn"

void SampleTreeCtrl::onTreeExpanding( wxTreeEvent& evt )
{
	wxTreeItemId	insId = evt.GetItem( );

	SampleTreeData*	pItemData = (SampleTreeData *)GetItemData( insId );
	if  ( ( SampleTreeData *)NULL == pItemData )
	{
		return;
	}

	// Treeが開くときだけに処理を限定するので、他の条件時は何も処理をしない
	//
	if  ( kome::objects::Variant::DATA_GROUP_NODE == pItemData->getData( ).type )
	{	// DATA_GROUP_NODE
		// data group
		unsigned int	unIndex = 0;
		kome::objects::DataGroupNode	*pGroupNode = (kome::objects::DataGroupNode *)pItemData->getData( ).prim.pt;

		for ( unIndex = 0 ; unIndex < pGroupNode->getNumberOfSpectra( ) ; unIndex++ )
		{
			kome::objects::Spectrum*	pSpec = pGroupNode->getSpectrum( unIndex );
			if  ( (kome::objects::Spectrum *)NULL != pSpec )
			{
				// 読込完了が終了しているかを確認して、読み込みしていない場合は読込を実行させます
				// （実際に動作するのはAXIMASpectrumだけです）
				//
				bool	bUpdate = false;
				if  ( true == pSpec->isRequestLoadData( ) )
				{
					bool	bRet = pSpec->loadData( );
					if  ( true == bRet )
					{
						bUpdate = true;
					}
				}
				else if  ( true == pSpec->isFirstAccess( ) )
				{
					bUpdate = true;
				}
				if  ( true == bUpdate )
				{
					// 読込が成功しているか最初のサンプルオープン時なので、Treeの表示情報を更新します
					//
					kome::view::SampleTreeManager&	insTreeMgr = SampleTreeManager::getInstance( );

					// メソッドないでicon名が既に登録されている場合には、新たに設定出来ないように
					// なっているため、ここで初期化します
					//
					pSpec->setIcon( "" );

					// Nodeの更新
					//
					insTreeMgr.updateNodes( pGroupNode );

					pSpec->resetFirstAccess( );
				}
			}
		}
	}

	return;
}

//
// <<<<<<	@Date:2013/07/16	<Add>	A.Ozaki

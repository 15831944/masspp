/**
 * @file VerticalBoxChromatogramCanvas.cpp
 * @brief VerticalBox Chromatogram Canvas
 * @author OKADA, H.
 * @date 2010.11
 * Copyright (C) 2014 Shimadzu Corporation All rights reserved.
 * 
 */

// >>>>>>	@Date:2013/05/23	<Modify>	A.Ozaki
//
// supported delete object
//
// 選択オブジェクトの削除、並び替えの機能を追加しました
// そのため、コード全般にわたり変更が発生したので、個別のコメントは入れていません
//   目障りなコメントアウトされたコードは、削除しました
//
// <<<<<<	@Date:2013/05/29	<Modify>	A.Ozaki

#include "stdafx.h"
#include "VerticalBoxChromatogramCanvas.h"
#include "VerticalBoxCanvasPluginFunctions.h"	// @date 2010/12/06 <Add> OKADA

#include <list>
#include <winbase.h>
#include <windows.h>
#include <wx/dcbuffer.h>	// for wxBufferedPaintDC
#include <wx/event.h>

using namespace kome::window;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


#define	PLUGIN_NAME								"VerticalBox Canvas"
#define	ICON_NAME								"verticalbox_dlg"

#define	UNSELECT_MENU_ID						( 10000 )

// for context menu
#define	ID_POPUP_MENU_VERTICALBOX_SETTINGS		( 60013 )
#define	STR_POPUP_MENU_VERTICALBOX_SETTINGS		"Stacked Settings"

#define CHROMATOGRAM_PROFILE_COLOR				kome::img::ColorTool::BLUE

#define DRAW_CHROM_FUNC_TYPE					"DRAW_CHROM"
#define VERTICAL_BOX_SIZE_DEFAULT				200		// スペクトル、クロマトグラムのグラフの縦方向の大きさのデフォルト値。View->Options画面のView->Window->Window Sizeで値を設定していない場合、この値を用いる

#define SECTION									"Drawing"
#define FONT_AXIS								"FONT_AXIS"

BEGIN_EVENT_TABLE( VerticalBoxChromatogramCanvas, ChromatogramCanvas )
	EVT_SCROLLWIN_TOP		( VerticalBoxChromatogramCanvas::onScrollWinTop )
	EVT_SCROLLWIN_BOTTOM	( VerticalBoxChromatogramCanvas::onScrollWinBottom )
	EVT_SCROLLWIN_LINEUP	( VerticalBoxChromatogramCanvas::onScrollWinLineup )
	EVT_SCROLLWIN_LINEDOWN	( VerticalBoxChromatogramCanvas::onScrollWinLinedown )
	EVT_SCROLLWIN_PAGEUP	( VerticalBoxChromatogramCanvas::onScrollWinPageup )
	EVT_SCROLLWIN_PAGEDOWN	( VerticalBoxChromatogramCanvas::onScrollWinPagedown )
	EVT_SCROLLWIN			( VerticalBoxChromatogramCanvas::onScroll )
	EVT_MOUSEWHEEL			( VerticalBoxChromatogramCanvas::onMouseWhell )	// マウスホイールに対応	// @date 2010/11/18 <Add> OKADA
	EVT_MENU				( ID_POPUP_MENU_VERTICALBOX_SETTINGS, VerticalBoxChromatogramCanvas::onVerticalboxSettings )
	EVT_UPDATE_UI			( ID_POPUP_MENU_VERTICALBOX_SETTINGS, VerticalBoxChromatogramCanvas::onUpdatePopupMenu )
END_EVENT_TABLE()

// static member
std::set< VerticalBoxChromatogramCanvas* >	VerticalBoxChromatogramCanvas::m_canvasSet;

// constructor
VerticalBoxChromatogramCanvas::VerticalBoxChromatogramCanvas( wxWindow* parent, const int width, const int height ) 
	: ChromatogramCanvas( parent, width, height, NULL )
{
	// initialize
	m_canvasSet.insert( this );

	m_parentLocal = parent;
	m_iHeightLocal = -1;

	m_iHeightLocal = getHeightLocal();

	m_iScrollPos = 0;

	// CanvasStyle 設定 @date 2010.12.10 <Add> M.Izumi
	Canvas::m_canvasStyle = MULTIPLE_OBJECTS;

	m_pSelectedChrom = NULL;

	m_chromPos = -1;

	m_nIndex = -1;
	m_nIndexDisp = -1;

	return;
}

// destructor
VerticalBoxChromatogramCanvas::~VerticalBoxChromatogramCanvas( )
{
	m_canvasSet.erase( this );

	for ( unsigned int i = 0 ; i < m_verticalBoxChroms.size( ); i++ )
	{
		delete m_verticalBoxChroms[ i ].chrom;
	}
	m_verticalBoxChroms.clear( );

	m_parentLocal = NULL;

	return;
}

// add chromatogram
kome::objects::Chromatogram* VerticalBoxChromatogramCanvas::onAddChromatogram( kome::objects::Chromatogram* chrom )
{
	// 垂直スクロールバーを設定
	int iHeightLocal = m_verticalBoxChroms.size( ) * m_iHeightLocal;

	SetScrollbarsVB( 1, iHeightLocal );
	
	// check the Chromatogram
	if  ( (kome::objects::Chromatogram *)NULL == chrom )
	{
		return	(kome::objects::Chromatogram *)NULL;
	}

	int idx = -1;

	// @date 2011/08/04 <Add> OKADA ->
	for ( unsigned int i = 0 ; i < getNumberOfChromatograms( ) && idx < 0 ; i++ )
	{
		if  ( ( chrom->getOrgChromatogram( ) ) == ( getChromatogram( i )->getOrgChromatogram( ) )
			&& chrom->getOperationFlag( ) )
		{
			idx = (int)i;
		}
	}
	// @date 2011/08/04 <Add> OKADA <-

	if  ( 0 <= idx)
	{
		SetScrollPosVB( idx * m_iHeightLocal );	// @date 2010/12/14 <Add> OKADA

		return	(kome::objects::Chromatogram *)NULL;	// すでに登録済み
	}

	// color
	COLOR	color = CHROMATOGRAM_PROFILE_COLOR;

	// add
	m_verticalBoxChroms.resize( m_verticalBoxChroms.size( ) + 1 );
	m_verticalBoxChroms.back( ).chrom = new kome::operation::TransformedChromatogram( *chrom, chrom->getOperationFlag( ) );
	m_verticalBoxChroms.back( ).insColor = color;
	m_verticalBoxChroms.back( ).bFlg = true;
	m_dummy.addChromatogram( m_verticalBoxChroms.back( ).chrom );
	m_verticalBoxChroms.back( ).nNum = getNumberOfChromatograms( );

	// 追加した場合、ダイアログ上の選択を解除する必要があるので、
	// m_nIndexに-1を設定します
	m_nIndex = -1;

	// set Displayed Range
	kome::window::CanvasBase::setDisplayedRange( getBaseChromatogram( ) );	// @date 2011.02.10 <Add> M.Izumi

	SetScrollPosVB( m_verticalBoxChroms.size( ) * m_iHeightLocal );	// @date 2010/12/14 <Add> OKADA

	// update
	update( );

	return	m_verticalBoxChroms.back().chrom;
}

// update 
void	VerticalBoxChromatogramCanvas::update( void )
{
	// update window
	m_dummy.deleteXYData();
	kome::plugin::PluginCallTool::onUpdateChromatogram( m_dummy );
	kome::window::WindowTool::refresh();

	return;
}

// on draw window foreground
// ここでグラフ軸線を描画
void	VerticalBoxChromatogramCanvas::onDrawWindowFg( kome::window::DCGraphics& g )
{
	// グラフタイトルを描画
	DispGraphTitle( g );

	kome::objects::Chromatogram*	transformedChrom = m_verticalBoxChroms[ m_nIndexDisp ].chrom;
	if  ( (kome::objects::Chromatogram *)NULL == transformedChrom )
	{
		return;
	}

	kome::objects::Chromatogram*	chrom = transformedChrom->getOrgChromatogram( );
	kome::plugin::PluginCallTool::drawChromatogramWindowFg( *chrom, g, this );

	// active
	if  ( -1 != m_nIndexDisp && m_nIndexDisp == m_nIndex )
	{
		g.getDC( )->SetPen( wxPen( *wxBLACK, 3 ) );
		int lx = g.getGraphPositionLeft( );
		int rx = g.getGraphPositionRight( );
		int ty = g.getGraphPositionTop( );
		int by = g.getGraphPositionBottom( );
		int lines[] = {
			lx, ty, lx, by, rx, by, rx, ty, lx, ty
		};

		g.drawLines( lines, sizeof( lines ) / sizeof( int ) / 2 );
	}

	return;
}

// on draw profile
void	VerticalBoxChromatogramCanvas::onDrawProfile( kome::window::DCGraphics& g )
{
	// draw profiles
	if  ( 0 > m_nIndexDisp )
	{
		// error
		return;
	}

	if  ( m_nIndexDisp < (int)m_verticalBoxChroms.size( ) )
	{
		ChromInfo*	info = (ChromInfo *)NULL;

		info = &( m_verticalBoxChroms[ m_nIndexDisp ] );

		if  ( true == info->bFlg && (ChromInfo *)NULL != info )
		{
			g.setColor( info->insColor );
			onDrawChromatogramProfile( g, *( info->chrom ), 0 );

			g.setScale( 1.0, 1.0 );
		}
	}

	return;
}

// resize for height
void	VerticalBoxChromatogramCanvas::refreshHeight(  )
{
	m_iHeightLocal = getHeightLocal( );

	// 垂直スクロールバー・スクロール位置を末尾に設定
	int iHeightLocal = m_verticalBoxChroms.size()*m_iHeightLocal;
	SetScrollbarsVB( 1, iHeightLocal );
	SetScrollPosVB( m_verticalBoxChroms.size()*m_iHeightLocal );

	onSize( 0, 0 );

	return;
}

// event action for onSize
void	VerticalBoxChromatogramCanvas::onSize( const unsigned int width, const unsigned int height )
{
	// ここで表示域高さを指定する
	if  ( 0 > m_iHeightLocal)
	{
		m_iHeightLocal = getHeightLocal( );
	}

	// ここで表示幅を指定する
	// get size
	int iWidthClient = 0;
	int iHeightClient = 0;
	GetClientSize( &iWidthClient, &iHeightClient );

	if  ( 0 <= iWidthClient && 0 <= m_iHeightLocal )
	{
		CanvasBase::onSize( iWidthClient, m_iHeightLocal );
	}

	// TOP MARGIN の設定
	m_graphics.setGraphPositionTop( VERTICAL_BOX_MARGIN_TOP );	// @date 2010/12/14 <Add> OKADA

	int iX, iY;
	GetViewStart( &iX, &iY );	// Scroll bar の先頭位置を取得

	int	iHeightLocalMax = m_verticalBoxChroms.size( ) * m_iHeightLocal - iHeightClient -1;
	if  ( ( 0 <= iHeightLocalMax ) && ( iHeightLocalMax < m_iScrollPos ) )
	{
		m_iScrollPos = iHeightLocalMax;
	}
	
	int iHeightTotal = m_verticalBoxChroms.size( ) * m_iHeightLocal;
	if  ( iHeightTotal < iHeightClient )
	{
		m_iScrollPos = 0;
	}

	return;
}

// Optionで設定した高さを取得
// 成功：高さ　失敗：負の数
int	VerticalBoxChromatogramCanvas::getHeightLocal( void )
{
	// ここで高さ情報を取得
	int iHeightLocal = -1;
	kome::core::MsppManager& msppMgr = kome::core::MsppManager::getInstance();
	if  ( NULL != &msppMgr )
	{
		kome::core::IniFile*	options = msppMgr.getIniFile( );
		if  ( options != NULL )
		{
			iHeightLocal = options->getInt( "Vertical Box Window Size", "VERTICAL_BOX_SIZE", VERTICAL_BOX_SIZE_DEFAULT );
		}
	}

	return	iHeightLocal;
}

// on draw Chromatogram profile
void VerticalBoxChromatogramCanvas::onDrawChromatogramProfile
(
	kome::window::DCGraphics&		g,
	kome::objects::Chromatogram&	chrom,
	int								iVBCnt
)
{
	// manager
	kome::plugin::PluginManager&	plgMgr = kome::plugin::PluginManager::getInstance( );

	// DC
	wxDC*	dc = g.getDC( );

	// memory device context
	wxSize	size = dc->GetSize( );
	wxBitmap	bitmap( size.GetWidth(), m_iHeightLocal );

	wxMemoryDC	memDc( bitmap );

	memDc.SetBackground( *wxWHITE_BRUSH );

	g.setDC( &memDc );

	// get functions
	std::vector< kome::plugin::PluginCall* >	calls;
	for ( unsigned int i = 0 ; i < plgMgr.getNumberOfPlugins( ) ; i++)
	{
		kome::plugin::PluginInfo*	plginfo = plgMgr.getPlugin( i );
		for ( unsigned int j = 0 ; j < plginfo->getNumberOfCalls( ) ; j++ )
		{
			kome::plugin::PluginCall*	plgcall = plginfo->getCall( j );
			if  ( strcmp( plgcall->getType( ), DRAW_CHROM_FUNC_TYPE ) == 0
				&& plgcall->checkStatus( ) )
			{
				calls.push_back( plgcall );
			}
		}
	}

	// parameter
	kome::objects::Parameters	params;
	kome::plugin::PluginCallTool::setChromatogram( params, chrom );
	kome::plugin::PluginCallTool::setCanvas( params, this );
	kome::plugin::PluginCallTool::setGraphics( params, g );

	// draw
	for ( unsigned int i = 0 ; i < calls.size( ) ; i++ )
	{
		memDc.Clear( );
		calls[ i ]->invoke( &params );

		// copy
		dc->Blit(
			0,
			0,
			size.GetWidth(),
			m_iHeightLocal,
			&memDc,
			0,
			0,
			wxCOPY
		);
	}

	// restore DC
	g.setDC( dc );

	return;
}

// drawing graphics
void	VerticalBoxChromatogramCanvas::onDrawGraphics( wxDC& dc )
{
	int	iHeightLocal = m_verticalBoxChroms.size( ) * m_iHeightLocal;

	int	iW, iH;
	GetClientSize( &iW, &iH );

	// buffered dc
	wxBitmap	wxBitmapTemp( iW, m_iHeightLocal );
	wxMemoryDC	memDcTemp( wxBitmapTemp );

	// erase
	wxColour	bgColor = GetBackgroundColour( );
	if  ( !bgColor.Ok( ) )
	{
		bgColor = wxSystemSettings::GetColour( wxSYS_COLOUR_3DFACE );
	}
	memDcTemp.SetBrush( wxBrush( bgColor ) );
	memDcTemp.SetPen( wxPen( bgColor, 1 ) );

	int	iX, iY;
	GetViewStart( &iX, &iY );	// Scroll bar の先頭位置を取得

	iY = m_iScrollPos;

	// スクロールバー位置設定
	wxRect windowRect( 0, 0, iW, m_iHeightLocal );	// 2010/11/05 DEBUG TEST OKADA
	CalcUnscrolledPosition( windowRect.x, windowRect.y, &windowRect.x, &windowRect.y );

	memDcTemp.DrawRectangle( windowRect );

	// draw
	m_tooltip.clear( );

	m_nIndexDisp = -1;

	// memory device context
	wxSize	size = dc.GetSize( );

	// buffered dc
	int	vw, vh;
	GetVirtualSize( &vw, &vh );
	wxBitmap wxBitmapTemp2( vw, vh );
	wxMemoryDC memDc2nd( wxBitmapTemp2 );
	memDc2nd.SetBackground( *wxWHITE_BRUSH );
	memDc2nd.Clear( );

	// erase
	bgColor = GetBackgroundColour( );
	if  ( !bgColor.Ok( ) )
	{
		bgColor = wxSystemSettings::GetColour( wxSYS_COLOUR_3DFACE );
	}
	memDc2nd.SetBrush( wxBrush( bgColor ) );
	memDc2nd.SetPen( wxPen( bgColor, 1 ) );

	int	iWidth2nd, iHeight2nd;					// for Debug
	memDc2nd.GetSize( &iWidth2nd, &iHeight2nd );// for Debug

	m_graphics.setDC( &memDcTemp );	// @date 2010/12/09 <Add> OKADA

	for ( unsigned int i = 0 ; i < m_verticalBoxChroms.size() ; i++ )
	{

		// 転送すべきか判定（window外にはみ出して表示されない物はcopy処理を行わない）
		int iYDrawPos = (m_iHeightLocal*(int)i)-(iY);
		if  ( iYDrawPos < -m_iHeightLocal )
		{
			// ウインドウの上端より上に描画されるので、描画しない
			continue;
		}
		if  ( size.GetHeight( ) < iYDrawPos )
		{
			// ウインドウの下端より下に描画されるので、描画しない
			continue;
		}

		m_nIndexDisp = i;
		ProfileCanvas::onDrawGraphics( memDcTemp );

		// copy
		bool bRet = memDc2nd.Blit(
			0,							// Destination device context x position. 転送先(このDC)のx座標。 
			iYDrawPos,					// Destination device context y position. 転送先y座標。 
			size.GetWidth( ),			// Width of source area to be copied. 転送領域の幅。 
			m_iHeightLocal,				// Height of source area to be copied. 転送領域の高さ。 
			&memDcTemp,					// Source device context. 転送元DC。 
			0,							// Source device context x position. 転送元x座標。 
			0,							// Source device context y position. 転送元y座標。 
			wxCOPY	// Logical function to use: see wxDC::SetLogicalFunction.適用する論理関数: wxDC::SetLogicalFunctionを見よ。 
		);
		memDcTemp.Clear( );
	}

	// 最後にまとめて転送
	// copy
	bool bRet = dc.Blit( 
		0,					// Destination device context x position. 転送先(このDC)のx座標。 
		0,					// Destination device context y position. 転送先y座標。 
		vw,					// Width of source area to be copied. 転送領域の幅。 
		vh,					// Height of source area to be copied. 転送領域の高さ。 
		&memDc2nd,			// Source device context. 転送元DC。 
		0,					// Source device context x position. 転送元x座標。 
		0,					// Source device context y position. 転送元y座標。 
		wxCOPY	// Logical function to use: see wxDC::SetLogicalFunction.適用する論理関数: wxDC::SetLogicalFunctionを見よ。 
	);

	m_chromPos = m_nIndexDisp;
	m_nIndexDisp = -1;

	return;
}

// event action for scroll
void	VerticalBoxChromatogramCanvas::onScroll( wxScrollWinEvent& evt )
{
	int iPos = -1;

	int iOri = evt.GetOrientation( );
	if  ( iOri == wxHORIZONTAL )
	{
		// 水平スクロールバー
	}
	else if  ( iOri == wxVERTICAL )
	{
		iPos = evt.GetPosition();
		m_iScrollPos = iPos;
	}
	else
	{
		// error
	}

	SetScrollPos( evt.GetOrientation( ), iPos );

	Refresh( );

	return;
}

// event action for scroll up on Window
void	VerticalBoxChromatogramCanvas::onScrollWinLineup( wxScrollWinEvent& evt )
{
	int iPos = -1;

	int iOri = evt.GetOrientation( );
	if  ( iOri == wxHORIZONTAL )
	{
		// 水平スクロールバー
	}
	else if  ( iOri == wxVERTICAL )
	{
		iPos = evt.GetPosition( );
	}
	else
	{
		// error
	}
	
	m_iScrollPos--;
	if  ( m_iScrollPos < 0 )
	{
		m_iScrollPos = 0;
	}
	SetScrollPos( evt.GetOrientation( ), m_iScrollPos );

	Refresh( );

	return;
}

// event action for scroll down on Window
void	VerticalBoxChromatogramCanvas::onScrollWinLinedown( wxScrollWinEvent& evt )
{
	int iPos = -1;

	int iOri = evt.GetOrientation( );
	if  ( iOri == wxHORIZONTAL )
	{
		// 水平スクロールバー
	}
	else if  ( iOri == wxVERTICAL )
	{
		iPos = evt.GetPosition( );
	}
	else
	{
		// error
	}
	
	// get size
	int iWidthClient = 0;
	int iHeightClient = 0;
	GetClientSize( &iWidthClient, &iHeightClient );
	int iHeightLocalMax = m_verticalBoxChroms.size( ) * m_iHeightLocal - iHeightClient -1;

	m_iScrollPos++;
	if  ( iHeightLocalMax < m_iScrollPos )
	{
		m_iScrollPos = iHeightLocalMax;
	}
	int iPosNew = iPos + 1;
	SetScrollPos( evt.GetOrientation( ), m_iScrollPos );

	Refresh( );

	return;
}

// event action for scroll top button on Window
void	VerticalBoxChromatogramCanvas::onScrollWinTop( wxScrollWinEvent& evt )
{
	SetScrollPos( evt.GetOrientation( ), m_iScrollPos );

	Refresh( );

	return;
}

// event action for scroll bottom button on Window
void	VerticalBoxChromatogramCanvas::onScrollWinBottom( wxScrollWinEvent& evt )
{
	SetScrollPos( evt.GetOrientation( ), m_iScrollPos );

	Refresh( );

	return;
}

// event action for page up scroll on Window
void	VerticalBoxChromatogramCanvas::onScrollWinPageup( wxScrollWinEvent& evt )
{
	m_iScrollPos -= m_iHeightLocal;
	if  ( m_iScrollPos < 0 )
	{
		m_iScrollPos = 0;
	}

	SetScrollPos( evt.GetOrientation( ), m_iScrollPos );

	Refresh( );

	return;
}

// event action for page down scroll on Window
void	VerticalBoxChromatogramCanvas::onScrollWinPagedown( wxScrollWinEvent& evt )
{
	m_iScrollPos += m_iHeightLocal;

	// get size
	int iWidthClient = 0;
	int iHeightClient = 0;
	GetClientSize( &iWidthClient, &iHeightClient );
	int iHeightLocalMax = m_verticalBoxChroms.size( ) * m_iHeightLocal - iHeightClient -1;
	
	if  ( iHeightLocalMax < m_iScrollPos )
	{
		m_iScrollPos = iHeightLocalMax;
	}

	SetScrollPos( evt.GetOrientation( ), m_iScrollPos );

	Refresh( );

	return;
}

// drawing foreground graphics
void	VerticalBoxChromatogramCanvas::onDrawGraphFg( kome::window::DCGraphics& g )
{
	kome::objects::Chromatogram*	transformedChrom;
	transformedChrom = getBaseChromatogram( );
	if  ( (kome::objects::Chromatogram *)NULL == transformedChrom )
	{
		return;
	}

	kome::objects::Chromatogram*	chrom = transformedChrom->getOrgChromatogram( );
	kome::plugin::PluginCallTool::drawChromatogramFg( *chrom, g, this );

	return;
}

// drawing background graphics
void	VerticalBoxChromatogramCanvas::onDrawGraphBg( kome::window::DCGraphics& g )
{
	kome::objects::Chromatogram*	transformedChrom;
	transformedChrom = getBaseChromatogram( );
	if  ( (kome::objects::Chromatogram *)NULL == transformedChrom )
	{
		return;
	}
	kome::objects::Chromatogram* chrom = transformedChrom->getOrgChromatogram( );
	kome::plugin::PluginCallTool::drawChromatogramBg( *chrom, g, this );		// @date 2010/11/26 <Add> OKADA

	return;
}

// on draw window background
void	VerticalBoxChromatogramCanvas::onDrawWindowBg( kome::window::DCGraphics& g )
{
	ProfileCanvas::onDrawWindowBg( g );
	ChromatogramCanvas::onDrawWindowBg( g );

	return;
}

//
void	VerticalBoxChromatogramCanvas::SetScrollbarsVB( int pixelsPerUnitY, int noUnitsY, int yPos, bool noRefresh )
{
	if  ( 0 > yPos )
	{
		yPos = m_iScrollPos;
	}
	
	SetScrollbars( 0, pixelsPerUnitY, 0, noUnitsY, 0, yPos, noRefresh );

	return;
}

//
void	VerticalBoxChromatogramCanvas::AdjustScrollbars( void )
{
	wxScrollHelper::AdjustScrollbars( );
	SetScrollPos( wxVERTICAL, m_iScrollPos );	// 2010/11/16 APPEND OKADA

	return;
}

kome::objects::Chromatogram*	VerticalBoxChromatogramCanvas::getBaseChromatogram( void )
{
	if  ( m_nIndexDisp < 0 )
	{
		return	&m_dummy;	// VerticalBoxWindowでの処理
	}
	else
	{
		if  ( m_nIndexDisp >= (int)m_verticalBoxChroms.size( ) )
		{
			return	&m_dummy;
		}
		int	unIndex = m_verticalBoxChroms[ m_nIndexDisp ].nNum;
		return	getChromatogram( unIndex );
	}
}

void	VerticalBoxChromatogramCanvas::onPrepareDraw( kome::window::DCGraphics& g )
{
	kome::objects::Chromatogram*	transformedChrom;
	transformedChrom = getBaseChromatogram( );
	if  ( (kome::objects::Chromatogram *)NULL == transformedChrom )
	{
		return;
	}
	kome::objects::Chromatogram*	chrom = transformedChrom->getOrgChromatogram( );
	kome::plugin::PluginCallTool::prepareDrawChromatogram( *chrom, g, this );

	return;
}

// mouse wheel action
void	VerticalBoxChromatogramCanvas::onMouseWhell( wxMouseEvent& evt )
{
	// (evt.m_wheelRotation/evt.m_wheelDelta)=スクロール方向, +1:上方向 -1:下方向
	// (m_iHeightLocal/3)=スクロール幅
	m_iScrollPos -= ( evt.m_wheelRotation / evt.m_wheelDelta ) * ( m_iHeightLocal / 3 );

	// get size
	int iWidthClient = 0;
	int iHeightClient = 0;
	GetClientSize( &iWidthClient, &iHeightClient );
	int iHeightLocalMax = m_verticalBoxChroms.size( ) * m_iHeightLocal - iHeightClient - 1;
	
	if  ( iHeightLocalMax < m_iScrollPos )
	{
		m_iScrollPos = iHeightLocalMax;
	}
	if  ( m_iScrollPos < 0 )
	{
		m_iScrollPos = 0;
	}
	if  ( m_verticalBoxChroms.size( ) <= 1 )
	{
		m_iScrollPos = 0;
	}

	SetScrollPos( wxVERTICAL, m_iScrollPos );

	Refresh( );

	return;
}

// on mouse button down
bool	VerticalBoxChromatogramCanvas::onMouseButtonDown( kome::evt::MouseEvent& evt )
{
	kome::evt::MouseEvent	insBackupEvt = evt;

	kome::objects::Chromatogram*	pChrom = onMouseCommonAction( evt );

	if  ( (kome::objects::Chromatogram *)NULL == pChrom )
	{
		return	false;
	}

	m_nIndex = getClickObjectNumber( insBackupEvt );

	if  ( 0 <= m_nIndex && m_nIndex < (int)getNumberOfChromatograms( ) )
	{
		// >>>>>	@Date:2013/05/17	<Add>	A.Ozaki
		setCallFunctionSelectedChromatogram( pChrom );
		// <<<<<	@Date:2013/05/07	<Add>	A.Ozaki

		setSelectedChrom( pChrom );
	}
	else
	{
		m_nIndex = -1;
		setCallFunctionSelectedChromatogram( NULL );

		setSelectedChrom( NULL );

		pChrom = m_pSelectedChrom;
	}

	return	kome::plugin::PluginCallTool::onChromatogramButtonDown( *pChrom, &m_graphics, evt, this );
}

// on mouse button up
bool	VerticalBoxChromatogramCanvas::onMouseButtonUp( kome::evt::MouseEvent& evt )
{
	kome::objects::Chromatogram*	pChrom = onMouseCommonAction( evt );

	if  ( (kome::objects::Chromatogram *)NULL == pChrom )
	{
		return	false;
	}

	if  ( kome::plugin::PluginCallTool::onChromatogramButtonUp( *pChrom, &m_graphics, evt, this ) )
	{
		return	true;
	}
	else if ( evt.rbutton( )
			&& evt.x( ) >= 0 && evt.x( ) <= m_width && evt.y( ) >= 0 && evt.y( ) <= m_height )
	{
		popupMenu( evt.x( ), evt.y( ) );
	}

	return false;
}

// on mouse button double click
bool	VerticalBoxChromatogramCanvas::onMouseDoubleClick( kome::evt::MouseEvent& evt )
{
	kome::objects::Chromatogram*	pChrom = onMouseCommonAction( evt );

	if  ( (kome::objects::Chromatogram *)NULL == pChrom )
	{
		return	false;
	}

	return kome::plugin::PluginCallTool::onChromatogramDoubleClick( *pChrom, &m_graphics, evt, this );
}

// on mouse cursor move
bool	VerticalBoxChromatogramCanvas::onMouseCursorMove( kome::evt::MouseEvent& evt )
{
	kome::objects::Chromatogram*	pChrom = onMouseCommonAction( evt );

	if  ( (kome::objects::Chromatogram *)NULL == pChrom )
	{
		return	false;
	}

	return kome::plugin::PluginCallTool::onChromatogramMouseMove( *pChrom, &m_graphics, evt, this );
}

// on remove chromatogram
void	VerticalBoxChromatogramCanvas::onRemoveChromatogram( kome::objects::Chromatogram* chrom )
{
	// search
	int	idx = -1;
	for ( int i = 0 ; i < (int)m_verticalBoxChroms.size( ) && idx < 0 ; i++ )
	{
		if  ( chrom == m_verticalBoxChroms[ i ].chrom
				|| m_verticalBoxChroms[ i ].chrom->getOrgChromatogram( ) == chrom->getOrgChromatogram( ) )
		{
			idx = i;
		}
	}

	// remove
	if  ( idx >= 0 )
	{
		// ActiveObjectに設定しているものが削除された場合、設定を解除します
		//
		if  ( NULL != m_pSelectedChrom &&
			m_pSelectedChrom->getOrgChromatogram( ) == chrom->getOrgChromatogram( ) )
		{
			clearSelectedChroms( );
			updateActiveObject( );
		}

		m_dummy.removeChromatogram( chrom );
		kome::plugin::PluginCallTool::onUpdateChromatogram( m_dummy );

		delete	m_verticalBoxChroms[ idx ].chrom;
		int	nStack = m_verticalBoxChroms[ idx ].nNum;
		m_verticalBoxChroms.erase( m_verticalBoxChroms.begin() + idx );

		// 削除されたのでm_verticalBoxChroms[].nNumを調整します
		for ( unsigned int unIndex = 0 ; unIndex < m_verticalBoxChroms.size( ) ; unIndex++ )
		{
			if  (  m_verticalBoxChroms[ unIndex ].nNum > nStack )
			{
				 m_verticalBoxChroms[ unIndex ].nNum =  m_verticalBoxChroms[ unIndex ].nNum - 1;
			}
		}
	}
	else
	{
		// 全てのリストがなくなる場合
		// 現状はのっぺらぼーのcanvasのまま放置してます
	}

	// 垂直スクロールバーを設定
	m_iScrollPos = 0;	// スクロールバー位置の調整	// @date 2010/12/07 <Add> 0にしてみた
	int iHeightLocal = m_verticalBoxChroms.size()*m_iHeightLocal;	// @date 2010/12/07 <Add> OKADA
	SetScrollbarsVB( 1, iHeightLocal-1 );							// @date 2010/12/07 <Add> OKADA
	SetScrollPos( wxVERTICAL, m_iScrollPos );	// スクロールバー位置の設定
	Refresh( );
	onSize( 0, 0 );

	// update
	update( );

	return;
}

// @date 2012.04.25 <Add> M.Izumi ->

// add selected chromatogram
void	VerticalBoxChromatogramCanvas::setSelectedChrom( kome::objects::Chromatogram* chrom )
{
	// check the parameter
	if  ( (kome::objects::Chromatogram* )NULL == chrom )
	{
		return;
	}

	if  ( m_pSelectedChrom != chrom )
	{
		m_pSelectedChrom = chrom;
	}

	// active object
	updateActiveObject( );

	return;
}
// <-

// update active object
void	VerticalBoxChromatogramCanvas::updateActiveObject( void )
{
	// active object
	kome::objects::Variant obj;

	// object initialized
	memset( (void *)&obj, 0x00, sizeof( obj ) );
	obj.type = kome::objects::Variant::UNKNOWN;
	obj.prim.pt = NULL;

	if  ( (kome::objects::Chromatogram *)NULL != m_pSelectedChrom )
	{
		if  ( (kome::objects::Chromatogram *)NULL != m_pSelectedChrom->getOrgChromatogram( ) )
		{
			for ( unsigned int unIndex = 0 ; unIndex < m_verticalBoxChroms.size( ) ; unIndex++ )
			{
				if  ( m_pSelectedChrom->getOrgChromatogram( ) ==
					m_verticalBoxChroms[unIndex].chrom->getOrgChromatogram( ) )
				{
					// 設定する場合は、kome::operation::TransformedChromatogram型で
					// ないとだめです
					obj.type = kome::objects::Variant::CHROMATOGRAM;

					// >>>>>>	@Date:2013/06/07	<Modify>	A.Ozaki
					// 設定するのは、m_pSelectedChrom->getOrgChromatogram( )とします
					// そうでないとPeakListが表示されません

					obj.prim.pt = m_pSelectedChrom->getOrgChromatogram( );

					// <<<<<<	@Date:2013/06/07	<Modify>	A.Ozaki
					break;
				}
			}
		}
	}

	setActiveObject( obj );

	kome::plugin::PluginCallTool::onActivate( getActiveObject( ) );

	return;
}

// iScrollPosの設定値に従ってスクロールします
void	VerticalBoxChromatogramCanvas::SetScrollPosVB( int iScrollPos )
{
	// 垂直スクロールバーを設定
	int iHeightLocal = m_verticalBoxChroms.size( ) * m_iHeightLocal;
	SetScrollbarsVB( 1, iHeightLocal );

	m_iScrollPos = iScrollPos;

	// m_iScrollPosの最大チェック
	int iWidthClient = 0;
	int iHeightClient = 0;
	GetClientSize( &iWidthClient, &iHeightClient );
	int iHeightLocalMax = m_verticalBoxChroms.size( ) * m_iHeightLocal - iHeightClient - 1;
	if  ( iHeightLocalMax < m_iScrollPos )
	{
		m_iScrollPos = iHeightLocalMax;
	}
	if  ( m_iScrollPos < 0 )
	{
		m_iScrollPos = 0;
	}

	// update
	update( );

	// 垂直スクロールバーを設定
	iHeightLocal = m_verticalBoxChroms.size( ) * m_iHeightLocal;
	SetScrollbars( 0, 1, 0, iHeightLocal-1 );

	SetScrollPos( wxVERTICAL, m_iScrollPos );
	Refresh( );
	onSize( 0, 0 );

	return;
}

// @date 2010/12/14 <Add> OKADA ------->
void	VerticalBoxChromatogramCanvas::DispGraphTitle( kome::window::DCGraphics& g )
{
	if  ( -1 == m_nIndexDisp )
	{
		// not request
		return;
	}

	std::string	title = m_verticalBoxChroms[ m_nIndexDisp ].chrom->getTitle( );
	kome::objects::DataGroupNode* group = m_verticalBoxChroms[ m_nIndexDisp ].chrom->getGroup( );
	while ( group != NULL )
	{
		title.append( FMT( " - %s", group->getName( ) ) );
		group = group->getParentGroup( );
	}
	const char * pszTitle = title.c_str( );

	// グラフ右上にタイトルを表示
	COLOR colOldColor = g.getColor( );
	COLOR colNewColor = RGB( 0x00, 0x00, 0x00 );	// BLACK
	g.setColor( colNewColor);
	kome::img::Font oldFont = g.getFont();
	kome::img::Font font = kome::window::FontTool::getFontFromIni( SECTION, FONT_AXIS );
	g.setFont( font );

	int iTitleWidth, iTitleHeight;
	g.getTextSize( pszTitle, &iTitleWidth, &iTitleHeight );

	int iGraphPositionTop = g.getGraphPositionTop();
	int iGraphPositionRight = g.getGraphPositionRight();

	g.setTextHAlign( kome::img::Graphics::HALIGN_RIGHT );

	g.drawText( pszTitle,
				iGraphPositionRight, // - iTitleWidth
				0,
				0 );

	g.setColor( colOldColor );	// 色を元にもどす
	g.setFont( oldFont );
	return;
}
// 2010/12/14 <Add> OKADA <-------

// Gets the chromatograms displayed on the canvas
void	VerticalBoxChromatogramCanvas::getChromDispCanvas( std::vector< kome::objects::Chromatogram* >& chroms )
{
	// get size
	int iWidthClient = 0;
	int iHeightClient = 0;
	GetClientSize( &iWidthClient, &iHeightClient );
		
	// Canvasに何個表示できるか
	int iCanvas = ceil( (double)iHeightClient / (double)m_iHeightLocal );
	// 何番目を表示しているか
	int pos = m_chromPos;

	std::vector<int> tmp;	
	for ( int i=0 ; i < iCanvas ; i++ )
	{
		// SPEC No.91660:Stacked View モードでTIC、MCをStackedで表示。Edit >Copy as >Metafileクリックで落ちる。
		// @date 2013.03.15 <Mod> M.Izumi
		if  ( pos >= 0 )
		{
			tmp.push_back( pos );
		}
		pos--;
	}
	
	std::sort( tmp.begin( ), tmp.end( ) );
	
	for ( unsigned int i = 0 ; i < tmp.size( ) ; i++ )
	{
		kome::objects::Chromatogram* chrom = m_verticalBoxChroms[tmp[i]].chrom;
		if  ( chrom != NULL )
		{
			chroms.push_back( chrom );
		}
	}

	return;
}

// creates menu
wxMenu*	VerticalBoxChromatogramCanvas::createMenu( )
{
	bool	flgSubView = isSubView( );
	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance( );

	wxMenu* menu = (wxMenu *)NULL;

	if  ( flgSubView )
	{
		// SubView
		kome::plugin::PluginMenu& plugin_menu = plgMgr.getChromatogramSubViewMenu( );
		menu = kome::window::Canvas::createPopupMenu( &plugin_menu );
	}
	else
	{
		// not SubView
		kome::plugin::PluginMenu& plugin_menu = plgMgr.getChromatogramViewMenu( );
		menu = kome::window::Canvas::createPopupMenu( &plugin_menu );
	}

	if  ( (wxMenu *)NULL == menu )
	{
		menu = new wxMenu( );
		if  ( (wxMenu *)NULL == menu )
		{
			// ここでNULLはエラーです
		}
	}

	bool	bCheckable = true;				// check可能なボタン
	wxMenuItem*	item = new wxMenuItem( menu,
									ID_POPUP_MENU_VERTICALBOX_SETTINGS,
									wxT( STR_POPUP_MENU_VERTICALBOX_SETTINGS ),
									wxT( "" ),
									bCheckable ? wxITEM_CHECK : wxITEM_NORMAL
									);
	item->SetCheckable( bCheckable );
	menu->Append( item );

	return	menu;
}

// Context Menu	
void	VerticalBoxChromatogramCanvas::onVerticalboxSettings( wxCommandEvent& evt )
{
	toggleVerticalBoxDialog( NULL );

	return;
}

// update for Setting in Popup menu
//
void	VerticalBoxChromatogramCanvas::onUpdatePopupMenu( wxUpdateUIEvent& evt )
{
	int	nEventId = evt.GetId( );

	if  ( ID_POPUP_MENU_VERTICALBOX_SETTINGS != nEventId )
	{
		return;			// event id が異なる
	}

	// active object
	kome::objects::Variant	obj;
	ChildFrame*	frame = ChildFrame::getActiveFrame( );
	if  ( NULL == frame )
	{
		kome::objects::ActiveObjectsManager&	aoMgr = kome::objects::ActiveObjectsManager::getInstance( );
		kome::objects::Sample*	acSample = aoMgr.getActiveSample( );

		if  ( NULL != acSample )
		{
			obj.type = kome::objects::Variant::DATA_GROUP_NODE;
			obj.prim.pt = acSample->getRootDataGroupNode( );
		}// ※acSampleがNULLの場合はobj初期値を使用する
	}
	else
	{
		obj = frame->getActiveObject( );
	}

	// StackedView Settingsが表示されているか否か調べる
	kome::objects::Variant	ret = checkVerticalBoxDialog( NULL );

	bool	checked = ret.prim.boolVal;
	evt.Check( checked );

	return;
}

// get chromatogram number
int	VerticalBoxChromatogramCanvas::GetVerticalBoxChromatogramNum( unsigned int nNum )
{
	int	nRet = -1;
	if  ( m_verticalBoxChroms.size() > nNum )
	{
		nRet = m_verticalBoxChroms[ nNum ].nNum;
	}

	return	nRet;
}

// get index number
void	VerticalBoxChromatogramCanvas::clearSelectedChroms( void )
{
	// clear
	m_pSelectedChrom = NULL;

	// active object
	updateActiveObject( );

	return;
}

// add selected chromatogram
void	VerticalBoxChromatogramCanvas::addSelectedChrom( kome::objects::Chromatogram* pChrom )
{
	// check the parameter
	if  ( (kome::objects::Chromatogram *)NULL == pChrom )
	{
		return;
	}

	m_pSelectedChrom = pChrom;

	// active object
	updateActiveObject( );

	return;
}

// remove selected chromatogram
void	VerticalBoxChromatogramCanvas::removeSelectedChrom( kome::objects::Chromatogram* pChrom )
{
	// check the parameter
	if  ( (kome::objects::Chromatogram *)NULL == pChrom )
	{
		return;
	}

	// remove
	int	nIdx = searchSelectedChrom( pChrom );
	if  ( 0 <= nIdx )
	{
		m_verticalBoxChroms.erase( m_verticalBoxChroms.begin() + nIdx );

		// 全て削除して並び替え
		m_dummy.clearChromatograms( );
		for ( unsigned int i = 0 ; i < m_verticalBoxChroms.size( ) ; i++ )
		{
			m_dummy.addChromatogram( m_verticalBoxChroms[i].chrom );
		}
	}

	// active object
	updateActiveObject( );

	return;
}

// get the number of selected chromatograms
unsigned int	VerticalBoxChromatogramCanvas::getNumberOfSelectedChroms( void )
{
	unsigned int	unRet = 0;
	if  ( (kome::objects::Chromatogram *)NULL != m_pSelectedChrom )
	{
		unRet = 1;
	}

	return	unRet;
}

// get selected chromatogram
kome::objects::Chromatogram*	VerticalBoxChromatogramCanvas::getSelectedChrom( const unsigned int unIdx )
{
	kome::objects::Chromatogram*	pChrom = (kome::objects::Chromatogram *)NULL;

	if  ( 0 <= unIdx && unIdx < m_verticalBoxChroms.size( ) )
	{
		pChrom = m_verticalBoxChroms[ unIdx ].chrom;
	}

	return	pChrom;
}

// set index number
void	VerticalBoxChromatogramCanvas::setIndex( const int nIndex )
{
	m_nIndex = nIndex;

	return;
}

// get index number
int	VerticalBoxChromatogramCanvas::getIndex( void )
{
	return	m_nIndex;
}

// search selected chromatogram
int VerticalBoxChromatogramCanvas::searchSelectedChrom( kome::objects::Chromatogram* pChrom )
{
	// search
	int	nIdx = -1;
	for ( int i = 0 ; i < (int)m_verticalBoxChroms.size( ) && nIdx < 0 ; i++ )
	{
		if  ( m_verticalBoxChroms[ i ].chrom == pChrom
				|| m_verticalBoxChroms[ i ].chrom->getOrgChromatogram( ) == pChrom->getOrgChromatogram( ) )
		{
			nIdx = i;
		}
	}

	return	nIdx;
}

// set call function ( selected chromatogram )
void	VerticalBoxChromatogramCanvas::setCallFunctionSelectedChromatogram( kome::objects::Chromatogram* pChrom )
{
	kome::plugin::PluginManager&	plgMgr = kome::plugin::PluginManager::getInstance( );
	kome::plugin::PluginFunctionItem*	item = plgMgr.getFunctionItem( "select_chromatogram", "verticalbox" );
	
	kome::objects::Parameters	params;
	kome::plugin::PluginCallTool::setChromatogram( params, *pChrom );
	
	if  ( item != NULL )
	{
		item->getCall( )->invoke( &params );
	}

	return;
}

// exchange data
void	VerticalBoxChromatogramCanvas::exchange( const int nS, const int nE )
{
	ChromInfo	insTemp;

	insTemp.chrom = m_verticalBoxChroms[nS].chrom;
	insTemp.insColor = m_verticalBoxChroms[nS].insColor;
	insTemp.bFlg = m_verticalBoxChroms[nS].bFlg;
	insTemp.nNum = m_verticalBoxChroms[nS].nNum;

	m_verticalBoxChroms[nS].chrom = m_verticalBoxChroms[nE].chrom;
	m_verticalBoxChroms[nS].insColor = m_verticalBoxChroms[nE].insColor;
	m_verticalBoxChroms[nS].bFlg = m_verticalBoxChroms[nE].bFlg;
	m_verticalBoxChroms[nS].nNum = m_verticalBoxChroms[nE].nNum;

	m_verticalBoxChroms[nE].chrom = insTemp.chrom;
	m_verticalBoxChroms[nE].insColor = insTemp.insColor;
	m_verticalBoxChroms[nE].bFlg = insTemp.bFlg;
	m_verticalBoxChroms[nE].nNum = insTemp.nNum;

	// restruct m_dummy
	m_dummy.clearChromatograms( );
	for ( unsigned int i = 0 ; i < m_verticalBoxChroms.size( ) ; i++ )
	{
		m_dummy.addChromatogram( m_verticalBoxChroms[i].chrom );
	}

	update( );

	return;
}

// common action for mouse event
kome::objects::Chromatogram*	VerticalBoxChromatogramCanvas::onMouseCommonAction( kome::evt::MouseEvent& evt )
{
	m_nIndexDisp = getClickObjectNumber( evt );
	kome::objects::Chromatogram*	pChrom = getTransformedChrom( );
	// reset
	m_nIndexDisp = -1;

	return	pChrom;
}

// マウスのボタン押下位置がどの表示オブジェクトの位置かを求めます
int		VerticalBoxChromatogramCanvas::getClickObjectNumber( kome::evt::MouseEvent& evt )
{
	// VBプラグイン用に、マウスクリック垂直位置をずらす
	//
	int		nPos_y = (evt).y();
	int		nPos_y_org = nPos_y;

	nPos_y += m_iScrollPos;

	while ( m_iHeightLocal < nPos_y )
	{
		nPos_y -= m_iHeightLocal;
	}
	(evt).y(nPos_y);		// 書き換えた値を保存
	
	// クリックされたスペクトルを与える処理
	// クリックされたのが何個目のスペクトルかを求める
	int		nRet = ( nPos_y_org + m_iScrollPos ) / m_iHeightLocal;

	return	nRet;
}

// get chromatogram object at m_nListDisp
kome::objects::Chromatogram*	VerticalBoxChromatogramCanvas::getTransformedChrom( )
{
	kome::objects::Chromatogram*	pTransformedChrom = (kome::objects::Chromatogram *)NULL;
	kome::objects::Chromatogram*	pChrom = (kome::objects::Chromatogram *)NULL;
	pTransformedChrom = getBaseChromatogram( );
	if  ( (kome::objects::Chromatogram *)NULL != pTransformedChrom )
	{
		pChrom = pTransformedChrom->getOrgChromatogram( );
	}

	return	pChrom;
}

// set index number on canvas
void	VerticalBoxChromatogramCanvas::setIndexDisp( const int nIndexDisp )
{
	m_nIndexDisp = nIndexDisp;

	return;
}

// get index number on canvas
int	VerticalBoxChromatogramCanvas::getIndexDisp( void )
{
	return	m_nIndexDisp;
}


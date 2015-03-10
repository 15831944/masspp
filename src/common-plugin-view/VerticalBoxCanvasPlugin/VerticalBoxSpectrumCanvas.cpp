/**
 * @file VerticalBoxSpectrumCanvas.cpp
 * @brief VerticalBox Spectrum Canvas
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
#include "VerticalBoxSpectrumCanvas.h"
#include "VerticalBoxCanvasPluginFunctions.h"	// @date 2010/12/06 <Add> OKADA

#include <list>
#include <winbase.h>
#include <windows.h>
#include <wx/event.h>
#include <wx/dcbuffer.h>	// for wxBufferedPaintDC
#include <math.h>

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
#define	ID_POPUP_MENU_VERTICALBOX_SETTINGS		( 60014 )
#define	STR_POPUP_MENU_VERTICALBOX_SETTINGS		"Stacked Settings"

#define SPECTRUM_PROFILE_COLOR	kome::img::ColorTool::RED

#define DRAW_SPEC_FUNC_TYPE						"DRAW_SPEC"
#define VERTICAL_BOX_SIZE_DEFAULT				( 200 )		// スペクトル、クロマトグラムのグラフの縦方向の大きさのデフォルト値。View->Options画面のView->Window->Window Sizeで値を設定していない場合、この値を用いる

#define SECTION									"Drawing"
#define FONT_AXIS								"FONT_AXIS"

BEGIN_EVENT_TABLE( VerticalBoxSpectrumCanvas, SpectrumCanvas )
	EVT_SCROLLWIN_TOP		( VerticalBoxSpectrumCanvas::onScrollWinTop )
	EVT_SCROLLWIN_BOTTOM	( VerticalBoxSpectrumCanvas::onScrollWinBottom )
	EVT_SCROLLWIN_LINEUP	( VerticalBoxSpectrumCanvas::onScrollWinLineup )
	EVT_SCROLLWIN_LINEDOWN	( VerticalBoxSpectrumCanvas::onScrollWinLinedown )
	EVT_SCROLLWIN_PAGEUP	( VerticalBoxSpectrumCanvas::onScrollWinPageup )
	EVT_SCROLLWIN_PAGEDOWN	( VerticalBoxSpectrumCanvas::onScrollWinPagedown )
	EVT_SCROLLWIN			( VerticalBoxSpectrumCanvas::onScroll )
	EVT_MOUSEWHEEL			( VerticalBoxSpectrumCanvas::onMouseWhell )	// マウスホイールに対応	// @date 2010/11/18 <Add> OKADA
	EVT_MENU				( ID_POPUP_MENU_VERTICALBOX_SETTINGS, VerticalBoxSpectrumCanvas::onVerticalboxSettings )
	EVT_UPDATE_UI			( ID_POPUP_MENU_VERTICALBOX_SETTINGS, VerticalBoxSpectrumCanvas::onUpdatePopupMenu )
END_EVENT_TABLE()

// static member
std::set< VerticalBoxSpectrumCanvas* >	VerticalBoxSpectrumCanvas::m_canvasSet;

// constructor
VerticalBoxSpectrumCanvas::VerticalBoxSpectrumCanvas( wxWindow* parent, const int width, const int height ) 
	: SpectrumCanvas( parent, width, height, NULL )
{
	// initialize
	m_canvasSet.insert( this );

	m_parentLocal = parent;
	m_iHeightLocal = -1;

	m_iHeightLocal = getHeightLocal();

	m_iScrollPos = 0;
	// CanvasStyle 設定 @date 2010.12.10 <Add> M.Izumi
	Canvas::m_canvasStyle = MULTIPLE_OBJECTS;

	m_pSelectedSpectrum = NULL;

	m_specPos = -1;

	m_nIndex = -1;
	m_nIndexDisp = -1;
}

// destructor
VerticalBoxSpectrumCanvas::~VerticalBoxSpectrumCanvas( )
{
	m_canvasSet.erase( this );

	for ( unsigned int i = 0; i < m_verticalBoxSpectra.size(); i++ )
	{
		delete m_verticalBoxSpectra[ i ].spec;
	}
	m_verticalBoxSpectra.clear( );

	m_parentLocal = NULL;

	return;
}

// add spectrum
kome::objects::Spectrum* VerticalBoxSpectrumCanvas::onAddSpectrum( kome::objects::Spectrum* spec )
{
	// 垂直スクロールバーを設定
	int iHeightLocal = m_verticalBoxSpectra.size( ) * m_iHeightLocal;
	SetScrollbarsVB( 1, iHeightLocal );
	
	// check the spectrum
	if  ( (kome::objects::Spectrum *)NULL == spec )
	{
		return	(kome::objects::Spectrum *)NULL;
	}

	int idx = -1;

	// @date 2011/08/04 <Add> OKADA ->
	// specの型は[kome::io::msb::MsbSpectrum]、getSpectrum()の型は[kome::operation::TransformedSpectrum]であり比較しても一致しないため、
	// getOrgSpectrum()関数で両者を同じ型[kome::io::msb::MsbSpectrum]に併せて比較するよう変更
	for ( unsigned int i = 0 ; i < getNumberOfSpectra( ) && idx < 0 ; i++ )
	{
		if  ( ( spec->getOrgSpectrum( ) ) == ((getSpectrum( i ))->getOrgSpectrum( ) )
			&& spec->getOperationFlag( ) )
		{
			idx = (int)i;
		}
	}
	// @date 2011/08/04 <Add> OKADA <-

	if  ( idx >= 0 )
	{
		// すでに表示されている
		SetScrollPosVB( idx *m_iHeightLocal );	// @date 2010/12/14 <Add> OKADA

		return	(kome::objects::Spectrum *)NULL;
	}

	// color
	COLOR	color = SPECTRUM_PROFILE_COLOR;

	// add
	m_verticalBoxSpectra.resize( m_verticalBoxSpectra.size( ) + 1 );
	m_verticalBoxSpectra.back( ).spec = new kome::operation::TransformedSpectrum( *spec, spec->getOperationFlag( ) );
	m_verticalBoxSpectra.back( ).insColor = color;
	m_verticalBoxSpectra.back( ).bFlg = true;
	m_verticalBoxSpectra.back( ).nNum = getNumberOfSpectra( );
	
	m_dummy.addSpectrum( m_verticalBoxSpectra.back( ).spec );

	// 追加した場合、ダイアログ上の選択を解除する必要があるので、
	// m_nIndexに-1を設定します
	m_nIndex = -1;

	// set Displayed Range
	kome::window::CanvasBase::setDisplayedRange( getBaseSpectrum( ) );	// @date 2011.02.10 <Add> M.Izumi

	SetScrollPosVB( m_verticalBoxSpectra.size( ) * m_iHeightLocal );	// @date 2010/12/14 <Add> OKADA

	// update
	update( );

	return	m_verticalBoxSpectra.back( ).spec;
}

// update
void	VerticalBoxSpectrumCanvas::update( void )
{
	// update window
	m_dummy.deleteXYData( );
	kome::plugin::PluginCallTool::onUpdateSpectrum( m_dummy );
	kome::window::WindowTool::refresh( );

	return;
}

// on draw window foreground
// ここでグラフ軸線を描画
void	VerticalBoxSpectrumCanvas::onDrawWindowFg( kome::window::DCGraphics& g )
{
	// グラフタイトルを描画
	DispGraphTitle( g );

	kome::objects::Spectrum*	transformedSpec = m_verticalBoxSpectra[ m_nIndexDisp ].spec;
	if  ( (kome::objects::Spectrum *)NULL == transformedSpec )
	{
		return;
	}

	kome::objects::Spectrum*	spec = transformedSpec->getOrgSpectrum( );
	kome::plugin::PluginCallTool::drawSpectrumWindowFg( *spec, g, this );

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
void	VerticalBoxSpectrumCanvas::onDrawProfile( kome::window::DCGraphics& g )
{
	// draw profiles
	if  ( 0 > m_nIndexDisp )
	{
		// error
		return;
	}

	if  ( m_nIndexDisp < (int)m_verticalBoxSpectra.size( ) )
	{
		SpecInfo*	info = (SpecInfo *)NULL;

		info = &( m_verticalBoxSpectra[ m_nIndexDisp ] );

		if  ( true == info->bFlg && (SpecInfo *)NULL != info )
		{
			g.setColor( info->insColor );
			onDrawSpectrumProfile( g, *( info->spec ), 0 );

			g.setScale( 1.0, 1.0 );
		}
	}

	return;
}

// 2011/03/09
// resize for height
void	VerticalBoxSpectrumCanvas::refreshHeight( void )
{
	m_iHeightLocal = getHeightLocal( );

	// 垂直スクロールバー・スクロール位置を末尾に設定
	int iHeightLocal = m_verticalBoxSpectra.size( ) * m_iHeightLocal;
	SetScrollbarsVB( 1, iHeightLocal );
	SetScrollPosVB( m_verticalBoxSpectra.size( ) * m_iHeightLocal );

	onSize( 0, 0 );

	return;
}

// event action for onSize
void	VerticalBoxSpectrumCanvas::onSize( const unsigned int width, const unsigned int height )
{
	// ここで表示域高さを指定する
	if  ( m_iHeightLocal < 0 )
	{
		m_iHeightLocal = getHeightLocal( );
	}

	// ここで表示幅を指定する
	// get size
	int iWidthClient = 0;
	int iHeightClient = 0;
	GetClientSize( &iWidthClient, &iHeightClient );

	if  ( iWidthClient >= 0 && m_iHeightLocal >= 0 )
	{
		CanvasBase::onSize( iWidthClient, m_iHeightLocal );
	}

	// TOP MARGIN の設定
	m_graphics.setGraphPositionTop( VERTICAL_BOX_MARGIN_TOP );	// @date 2010/12/14 <Add> OKADA

	int iX, iY;
	GetViewStart( &iX, &iY );	// Scroll bar の先頭位置を取得

	int iHeightLocalMax = m_verticalBoxSpectra.size()*m_iHeightLocal - iHeightClient -1;
	if  ( ( 0 <= iHeightLocalMax ) && ( iHeightLocalMax < m_iScrollPos ) )
	{
		m_iScrollPos = iHeightLocalMax;
	}
	
	int iHeightTotal = m_verticalBoxSpectra.size( ) * m_iHeightLocal;
	if  ( iHeightTotal < iHeightClient )
	{
		m_iScrollPos = 0;
	}

	return;
}

// Optionで設定した高さを取得
// 成功：高さ　失敗：負の数
int	VerticalBoxSpectrumCanvas::getHeightLocal( void )
{
	// ここで高さ情報を取得
	int iHeightLocal = -1;
	kome::core::MsppManager&	msppMgr = kome::core::MsppManager::getInstance();
	if  ( &msppMgr != NULL )
	{
		kome::core::IniFile* options = msppMgr.getIniFile( );
		if  ( options != NULL )
		{
			iHeightLocal = options->getInt( "Vertical Box Window Size", "VERTICAL_BOX_SIZE", VERTICAL_BOX_SIZE_DEFAULT );
		}
	}

	return iHeightLocal;
}

// on draw spectrum profile
void	VerticalBoxSpectrumCanvas::onDrawSpectrumProfile
(
	kome::window::DCGraphics&	g,
	kome::objects::Spectrum&	spec,
	int							iVBCnt
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
	for ( unsigned int i = 0 ; i < plgMgr.getNumberOfPlugins( ) ; i++ )
	{
		kome::plugin::PluginInfo*	plginfo = plgMgr.getPlugin( i );
		for ( unsigned int j = 0 ; j < plginfo->getNumberOfCalls( ) ; j++ )
		{
			kome::plugin::PluginCall*	plgcall = plginfo->getCall( j );
			if  ( strcmp( plgcall->getType( ), DRAW_SPEC_FUNC_TYPE ) == 0
				&& plgcall->checkStatus( ) )
			{
				calls.push_back( plgcall );
			}
		}
	}

	// parameter
	kome::objects::Parameters	params;
	kome::plugin::PluginCallTool::setSpectrum( params, spec );
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
void	VerticalBoxSpectrumCanvas::onDrawGraphics( wxDC& dc )
{
	int	iHeightLocal = m_verticalBoxSpectra.size( ) * m_iHeightLocal;

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
	wxRect	windowRect( 0, 0, iW, m_iHeightLocal );
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
	wxBitmap	wxBitmapTemp2( vw, vh );
	wxMemoryDC	memDc2nd( wxBitmapTemp2 );
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

	int	iWidth2nd, iHeight2nd;					
	memDc2nd.GetSize( &iWidth2nd, &iHeight2nd );

	// ProfileCanvas::onDrawGraphics()内でm_graphicsのm_dcを内部でwxMemopryDCで確保したmemDCに置き換え、
	// その後onDrawGraphics(memDcTemp)の引数 "memDcTemp" に戻す処理をしている。このため、ProfileCanvas::onDrawGraphics()内のonDrawWindowBg()関数内
	// で軸線の範囲選択部分の塗りつぶし処理を行うと memDcTemp ではなく VerticalBoxSpectrumCanvas::onDrawGraphics( dc )関数の引数 "dc" に対して行われ、
	// それ以降の描画処理は全て memDcTemp に行われた後に、 最終的に dc へ上書きされるので、1つめの範囲選択が行われない結果となっていた。
	m_graphics.setDC( &memDcTemp );	// @date 2010/12/09 <Add> OKADA

	for ( unsigned int i = 0 ; i < m_verticalBoxSpectra.size( ) ; i++ )
	{
		// 転送すべきか判定（window外にはみ出して表示されない物はcopy処理を行わない）
		int iYDrawPos = (m_iHeightLocal*(int)i)-(iY);
		if  ( iYDrawPos < -m_iHeightLocal )
		{
			// ウインドウの上端より上に描画されるので、描画しない
			continue;
		}
		if  ( size.GetHeight() < iYDrawPos )
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

	m_specPos = m_nIndexDisp;
	m_nIndexDisp = -1;

	return;
}

// event action for scroll
void	VerticalBoxSpectrumCanvas::onScroll( wxScrollWinEvent& evt )
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
void	VerticalBoxSpectrumCanvas::onScrollWinLineup( wxScrollWinEvent& evt )
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
void VerticalBoxSpectrumCanvas::onScrollWinLinedown( wxScrollWinEvent& evt )
{
	int iPos = -1;

	int iOri = evt.GetOrientation( );
	if  ( iOri == wxHORIZONTAL )
	{
		// 水平・Xクロールバー
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
	int iHeightLocalMax = m_verticalBoxSpectra.size( ) * m_iHeightLocal - iHeightClient - 1;

	// 垂直スクロールバーを設定

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
void	VerticalBoxSpectrumCanvas::onScrollWinTop( wxScrollWinEvent& evt )
{
	SetScrollPos( evt.GetOrientation( ), m_iScrollPos );

	Refresh( );

	return;
}

// event action for scroll bottom button on Window
void	VerticalBoxSpectrumCanvas::onScrollWinBottom( wxScrollWinEvent& evt )
{
	SetScrollPos( evt.GetOrientation( ), m_iScrollPos );

	Refresh( );

	return;
}

// event action for page up scroll on Window
void	VerticalBoxSpectrumCanvas::onScrollWinPageup( wxScrollWinEvent& evt )
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
void VerticalBoxSpectrumCanvas::onScrollWinPagedown( wxScrollWinEvent& evt )
{
	m_iScrollPos += m_iHeightLocal;

	// get size
	int iWidthClient = 0;
	int iHeightClient = 0;
	GetClientSize( &iWidthClient, &iHeightClient );
	int iHeightLocalMax = m_verticalBoxSpectra.size( ) * m_iHeightLocal - iHeightClient - 1;
	
	if  ( iHeightLocalMax < m_iScrollPos )
	{
		m_iScrollPos = iHeightLocalMax;
	}

	SetScrollPos( evt.GetOrientation( ), m_iScrollPos );

	Refresh( );

	return;
}

// drawing foreground graphics
void	VerticalBoxSpectrumCanvas::onDrawGraphFg( kome::window::DCGraphics& g )
{
	kome::objects::Spectrum* transformedSpec;							// @date 2010/11/26 <Add> OKADA
	transformedSpec = getBaseSpectrum( );								// @date 2010/11/26 <Add> OKADA
	if  ( (kome::objects::Spectrum *)NULL == transformedSpec )
	{
		return;
	}

	kome::objects::Spectrum*	spec = transformedSpec->getOrgSpectrum( );	// @date 2010/11/26 <Add> OKADA
	kome::plugin::PluginCallTool::drawSpectrumFg( *spec, g, this );			// @date 2010/11/26 <Add> OKADA

	return;
}

// drawing background graphics
void	VerticalBoxSpectrumCanvas::onDrawGraphBg( kome::window::DCGraphics& g )
{
	kome::objects::Spectrum*	transformedSpec;						// @date 2010/11/26 <Add> OKADA
	transformedSpec = getBaseSpectrum( );								// @date 2010/11/26 <Add> OKADA
	if  ( (kome::objects::Spectrum *)NULL == transformedSpec )
	{
		return;
	}

	kome::objects::Spectrum*	spec = transformedSpec->getOrgSpectrum( );	// @date 2010/11/26 <Add> OKADA
	kome::plugin::PluginCallTool::drawSpectrumBg( *spec, g, this );			// @date 2010/11/26 <Add> OKADA

	return;
}

// on draw window background
void	VerticalBoxSpectrumCanvas::onDrawWindowBg( kome::window::DCGraphics& g )
{
	ProfileCanvas::onDrawWindowBg( g );		// @date 2010/11/26 <Del> OKADA
	SpectrumCanvas::onDrawWindowBg( g );	// @date 2010/11/26 <Add> OKADA

	return;
}

//
void	VerticalBoxSpectrumCanvas::SetScrollbarsVB( int pixelsPerUnitY, int noUnitsY, int yPos, bool noRefresh)
{
	if  ( 0 > yPos )
	{
		yPos = m_iScrollPos;
	}

	SetScrollbars( 0, pixelsPerUnitY, 0, noUnitsY, 0, yPos, noRefresh );

	return;
}

//
void	VerticalBoxSpectrumCanvas::AdjustScrollbars( void )
{
	wxScrollHelper::AdjustScrollbars( );
	SetScrollPos( wxVERTICAL, m_iScrollPos );	// 2010/11/16 APPEND OKADA

	return;
}

kome::objects::Spectrum*	VerticalBoxSpectrumCanvas::getBaseSpectrum( void )
{
	if  ( m_nIndexDisp < 0 )
	{
		return	&m_dummy;	// VerticalBoxWindowでの処理
	}
	else
	{
		if  ( m_nIndexDisp >= (int)m_verticalBoxSpectra.size( ) )
		{
			return	&m_dummy;
		}
		int	unIndex = m_verticalBoxSpectra[ m_nIndexDisp ].nNum;
		return	getSpectrum( unIndex );
	}
}

void	VerticalBoxSpectrumCanvas::onPrepareDraw( kome::window::DCGraphics& g )
{
	kome::objects::Spectrum*	transformedSpec;
	transformedSpec = getBaseSpectrum( );
	if  ( (kome::objects::Spectrum *)NULL == transformedSpec )
	{
		return;
	}
	kome::objects::Spectrum*	spec = transformedSpec->getOrgSpectrum( );
	kome::plugin::PluginCallTool::prepareDrawSpectrum( *spec, g, this );

	return;
}

// mouse wheel action
void	VerticalBoxSpectrumCanvas::onMouseWhell( wxMouseEvent& evt )
{
	// (evt.m_wheelRotation/evt.m_wheelDelta)=スクロール方向, +1:上方向 -1:下方向
	// (m_iHeightLocal/3)=スクロール幅
	m_iScrollPos -= ( evt.m_wheelRotation / evt.m_wheelDelta ) * ( m_iHeightLocal / 3 );

	// get size
	int iWidthClient = 0;
	int iHeightClient = 0;
	GetClientSize( &iWidthClient, &iHeightClient );
	int iHeightLocalMax = m_verticalBoxSpectra.size( ) * m_iHeightLocal - iHeightClient - 1;
	
	if  ( iHeightLocalMax < m_iScrollPos )
	{
		m_iScrollPos = iHeightLocalMax;
	}
	if  ( m_iScrollPos < 0 )
	{
		m_iScrollPos = 0;
	}
	if  ( m_verticalBoxSpectra.size( ) <= 1 )
	{
		m_iScrollPos = 0;
	}

	SetScrollPos( wxVERTICAL, m_iScrollPos );

	Refresh( );

	return;
}

// on mouse button down
bool	VerticalBoxSpectrumCanvas::onMouseButtonDown( kome::evt::MouseEvent& evt )
{
	kome::evt::MouseEvent	insBackupEvt = evt;

	kome::objects::Spectrum*	pSpec = onMouseCommonAction( evt );

	if  ( (kome::objects::Spectrum *)NULL == pSpec )
	{
		return	false;
	}

	m_nIndex = getClickObjectNumber( insBackupEvt );

	if  ( 0 <= m_nIndex && m_nIndex < (int)getNumberOfSpectra( ) )
	{
		// >>>>>	@Date:2013/05/17	<Add>	A.Ozaki
		setCallFunctionSelectedSpectrum( pSpec );
		// <<<<<	@Date:2013/05/07	<Add>	A.Ozaki

		// >>>>>>	@Date:2013/06/07	<Modify>	A.Ozaki
		setSelectedSpec( pSpec );
	}
	else
	{
		m_nIndex = -1;
		setCallFunctionSelectedSpectrum( NULL );

		// >>>>>>	@Date:2013/06/07	<Modify>	A.Ozaki
		setSelectedSpec( NULL );
		// <<<<<<	@Date:2013/06/07	<Modify>	A.Ozaki
	}

	return	kome::plugin::PluginCallTool::onSpectrumButtonDown( *pSpec, &m_graphics, evt, this );
}

// on mouse button up
bool	VerticalBoxSpectrumCanvas::onMouseButtonUp( kome::evt::MouseEvent& evt )
{
	kome::objects::Spectrum*	pSpec = onMouseCommonAction( evt );

	if  ( (kome::objects::Spectrum *)NULL == pSpec )
	{
		return	false;
	}

	if  ( kome::plugin::PluginCallTool::onSpectrumButtonUp( *pSpec, &m_graphics, evt, this ) )
	{
		return	true;
	}
	else if ( evt.rbutton( )
			&& evt.x( ) >= 0 && evt.x( ) <= m_width && evt.y( ) >= 0 && evt.y( ) <= m_height )
	{
		popupMenu( evt.x( ), evt.y( ) );
	}

	return	false;
}

// on mouse button double click
bool	VerticalBoxSpectrumCanvas::onMouseDoubleClick( kome::evt::MouseEvent& evt )
{
	kome::objects::Spectrum*	pSpec = onMouseCommonAction( evt );

	if  ( (kome::objects::Spectrum *)NULL == pSpec )
	{
		return	false;
	}

	return	kome::plugin::PluginCallTool::onSpectrumDoubleClick( *pSpec, &m_graphics, evt, this );
}

// on mouse cursor move
bool	VerticalBoxSpectrumCanvas::onMouseCursorMove( kome::evt::MouseEvent& evt )
{
	kome::objects::Spectrum*	pSpec = onMouseCommonAction( evt );

	if  ( (kome::objects::Spectrum *)NULL == pSpec )
	{
		return	false;
	}
	
	return	kome::plugin::PluginCallTool::onSpectrumMouseMove( *pSpec, &m_graphics, evt, this );
}

// remove spectrum
void	VerticalBoxSpectrumCanvas::onRemoveSpectrum( kome::objects::Spectrum* spec )
{
	// search
	int idx = -1;
	for ( int i = 0 ; i < (int)m_verticalBoxSpectra.size( ) && idx < 0 ; i++ )
	{
		if  ( spec == m_verticalBoxSpectra[ i ].spec 
				|| m_verticalBoxSpectra[ i ].spec->getOrgSpectrum( ) == spec->getOrgSpectrum( ) ) {
			idx = i;
		}
	}

	// remove
	if  ( idx >= 0 )
	{
		// ActiveObjectに設定しているものが削除された場合、設定を解除します
		//
		if  ( NULL != m_pSelectedSpectrum &&
			m_pSelectedSpectrum->getOrgSpectrum( ) == spec->getOrgSpectrum( ) )
		{
			clearSelectedSpectrum( );
			updateActiveObject( );
		}

		m_dummy.removeSpectrum( spec );
		kome::plugin::PluginCallTool::onUpdateSpectrum( m_dummy );

		delete	m_verticalBoxSpectra[ idx ].spec;
		int	nStack = m_verticalBoxSpectra[ idx ].nNum;
		m_verticalBoxSpectra.erase( m_verticalBoxSpectra.begin() + idx );

		// 削除されたのでm_verticalBoxSpectra[].nNumを調整します
		for ( unsigned int unIndex = 0 ; unIndex < m_verticalBoxSpectra.size( ) ; unIndex++ )
		{
			if  (  m_verticalBoxSpectra[ unIndex ].nNum > nStack )
			{
				 m_verticalBoxSpectra[ unIndex ].nNum =  m_verticalBoxSpectra[ unIndex ].nNum - 1;
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
	int iHeightLocal = m_verticalBoxSpectra.size()*m_iHeightLocal;	// @date 2010/12/07 <Add> OKADA
	SetScrollbarsVB( 1, iHeightLocal-1 );							// @date 2010/12/07 <Add> OKADA
	SetScrollPos( wxVERTICAL, m_iScrollPos );	// スクロールバー位置の設定
	Refresh( );
	onSize( 0, 0 );

	// update
	update( );

	return;
}

// @date 2012.04.25 <Add> M.Izumi ->

// add selected spectrum
void VerticalBoxSpectrumCanvas::setSelectedSpec( kome::objects::Spectrum* spec )
{
	// check the parameter
	if  ( (kome::objects::Spectrum *)NULL == spec )
	{
		return;
	}

	if  ( m_pSelectedSpectrum != spec )
	{
		m_pSelectedSpectrum = spec;
	}

	// active object
	updateActiveObject( );

	return;
}
// <-

// update active object
void	VerticalBoxSpectrumCanvas::updateActiveObject( void )
{
	// object
	kome::objects::Variant obj;

	// object initialized
	obj.type = kome::objects::Variant::UNKNOWN;
	obj.prim.pt = NULL;

	if  ( (kome::objects::Spectrum *)NULL != m_pSelectedSpectrum )
	{
		if  ( (kome::objects::Spectrum *)NULL != m_pSelectedSpectrum->getOrgSpectrum( ) )
		{
			for ( unsigned int unIndex = 0 ; unIndex < m_verticalBoxSpectra.size( ) ; unIndex++ )
			{
				if  ( m_pSelectedSpectrum->getOrgSpectrum( ) ==	m_verticalBoxSpectra[unIndex].spec->getOrgSpectrum( ) )
				{
					// 設定する場合は、kome::operation::TransformedSpectrum型で
					// ないとだめです
					obj.type = kome::objects::Variant::SPECTRUM;
					// >>>>>>	@Date:2013/06/07	<Modify>	A.Ozaki
					// 設定するのは、m_pSelectedChrom->getOrgChromatogram( )とします
					// そうでないとPeakListが表示されません
					obj.prim.pt = m_pSelectedSpectrum->getOrgSpectrum( );
					break;
				}
			}
		}
	}

	setActiveObject( obj );
	kome::plugin::PluginCallTool::onActivate( getActiveObject( ) );

	return;
}

// @date 2010/12/09 <Add> OKADA ------->
// iScrollPosの設定値に従ってスクロールします
void	VerticalBoxSpectrumCanvas::SetScrollPosVB( int iScrollPos )
{
	// 垂直スクロールバーを設定
	int iHeightLocal = m_verticalBoxSpectra.size( ) * m_iHeightLocal;
	SetScrollbarsVB( 1, iHeightLocal );

	m_iScrollPos = iScrollPos;

	// m_iScrollPosの最大チェック
	int iWidthClient = 0;
	int iHeightClient = 0;
	GetClientSize( &iWidthClient, &iHeightClient );
	int iHeightLocalMax = m_verticalBoxSpectra.size( ) * m_iHeightLocal - iHeightClient - 1;
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
	iHeightLocal = m_verticalBoxSpectra.size() * m_iHeightLocal;
	SetScrollbars( 0, 1, 0, iHeightLocal-1 );

	SetScrollPos( wxVERTICAL, m_iScrollPos );
	Refresh( );
	onSize( 0, 0 );

	return;
}

void	VerticalBoxSpectrumCanvas::DispGraphTitle( kome::window::DCGraphics& g )
{
	if  ( -1 == m_nIndexDisp )
	{
		// not request
		return;
	}

	std::string	title = m_verticalBoxSpectra[ m_nIndexDisp ].spec->getTitle( );
	kome::objects::DataGroupNode* group = m_verticalBoxSpectra[ m_nIndexDisp ].spec->getGroup( );
	while ( group != NULL )
	{
		title.append( FMT( " - %s", group->getName( ) ) );
		group = group->getParentGroup( );
	}
	const char	*pszTitle = title.c_str( );

	// グラフ右上にタイトルを表示
	COLOR	colOldColor = g.getColor( );
	COLOR	colNewColor = RGB( 0x00, 0x00, 0x00 );	// BLACK
	g.setColor( colNewColor );
	kome::img::Font oldFont = g.getFont();
	kome::img::Font font = kome::window::FontTool::getFontFromIni( SECTION, FONT_AXIS );
	g.setFont( font );

	int	iTitleWidth, iTitleHeight;
	g.getTextSize( pszTitle, &iTitleWidth, &iTitleHeight );

	int iGraphPositionTop = g.getGraphPositionTop( );
	int iGraphPositionRight = g.getGraphPositionRight( );

	g.setTextHAlign( kome::img::Graphics::HALIGN_RIGHT );
	
	g.drawText( pszTitle,
				iGraphPositionRight,
				0,
				0 );

	g.setColor( colOldColor );	// 色を元にもどす
	g.setFont( oldFont );
	return;
}

// Gets the spectrum displayed on the canvas
void	VerticalBoxSpectrumCanvas::getSpecDispCanvas( std::vector< kome::objects::Spectrum* >& spectra )
{
	// get size
	int iWidthClient = 0;
	int iHeightClient = 0;
	GetClientSize( &iWidthClient, &iHeightClient );
		
	// Canvasに何個表示できるか
	int iCanvas = ceil( (double)iHeightClient / (double)m_iHeightLocal );
	// 何番目を表示しているか
	int pos = m_specPos;

	std::vector<int> tmp;	
	for  ( int i = 0 ; i < iCanvas ; i++ )
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
		kome::objects::Spectrum* spec = m_verticalBoxSpectra[tmp[i]].spec;
		if  ( spec != NULL )
		{
			spectra.push_back( spec );
		}
	}

	return;
}

// creates menu
wxMenu* VerticalBoxSpectrumCanvas::createMenu( )
{
	bool	flgSubView = isSubView( );
	kome::plugin::PluginManager&	plgMgr = kome::plugin::PluginManager::getInstance( );

	// ここで、Overlapping Settings を追加
	wxMenu*	menu = (wxMenu *)NULL;

	if  ( flgSubView )
	{
		// SubView
		kome::plugin::PluginMenu& plugin_menu = plgMgr.getSpectrumSubViewMenu( );
		menu = kome::window::Canvas::createPopupMenu( &plugin_menu );
	}
	else
	{
		// not sub view
		kome::plugin::PluginMenu& plugin_menu = plgMgr.getSpectrumViewMenu( );
		menu = kome::window::Canvas::createPopupMenu( &plugin_menu );
	}

	if  ( (wxMenu*)NULL == menu )
	{
		menu = new wxMenu( );
		if  ( (wxMenu*)NULL == menu )
		{
			// ここはエラー
		}
	}

	bool	bCheckable = true;			// check可能なボタン
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
void	VerticalBoxSpectrumCanvas::onVerticalboxSettings( wxCommandEvent& evt )
{
	toggleVerticalBoxDialog( NULL );

	return;
}

// update for Setting in Popup menu
//
void	VerticalBoxSpectrumCanvas::onUpdatePopupMenu( wxUpdateUIEvent& evt )
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

// get spectrum number
int	VerticalBoxSpectrumCanvas::GetVerticalBoxSpectrumNum( unsigned int nNum )
{
	int	nRet = -1;
	if  ( m_verticalBoxSpectra.size() > nNum )
	{
		nRet = m_verticalBoxSpectra[ nNum ].nNum;
	}

	return	nRet;
}

// clears selected spectra
void	VerticalBoxSpectrumCanvas::clearSelectedSpectrum( void )
{
	// clear
	m_pSelectedSpectrum = NULL;

	// active object
	updateActiveObject( );

	return;
}

// add selected spectrum
void	VerticalBoxSpectrumCanvas::addSelectedSpectrum( kome::objects::Spectrum* pSpec )
{
	// check the parameter
	if  ( (kome::objects::Spectrum *)NULL == pSpec )
	{
		return;
	}

	m_pSelectedSpectrum = pSpec;

	// active object
	updateActiveObject( );

	return;
}

// remove selected spectrum
void	VerticalBoxSpectrumCanvas::removeSelectedSpectrum( kome::objects::Spectrum* pSpec )
{
	// check the parameter
	if  ( (kome::objects::Spectrum *)NULL == pSpec )
	{
		return;
	}

	// remove
	int	nIdx = searchSelectedSpectrum( pSpec );
	if  ( 0 <= nIdx )
	{
		m_verticalBoxSpectra.erase( m_verticalBoxSpectra.begin() + nIdx );

		// 全て削除して並び替え
		m_dummy.clearSpectra( );
		for ( unsigned int i = 0 ; i < m_verticalBoxSpectra.size( ) ; i++ )
		{
			m_dummy.addSpectrum( m_verticalBoxSpectra[i].spec );
		}
	}

	// active object
	updateActiveObject( );

	return;
}

// get the number of selected spectra
unsigned int	VerticalBoxSpectrumCanvas::getNumberOfSelectedSpectrum( void )
{
	unsigned int	unRet = 0;
	if  ( (kome::objects::Spectrum *)NULL != m_pSelectedSpectrum )
	{
		unRet = 1;
	}

	return	unRet;
}

// get selected spectrum
kome::objects::Spectrum*	VerticalBoxSpectrumCanvas::getSelectedSpectrum( const unsigned int unIdx )
{
	kome::objects::Spectrum*	pSpec = (kome::objects::Spectrum *)NULL;

	if  ( 0 <= unIdx && unIdx < m_verticalBoxSpectra.size( ) )
	{
		pSpec = m_verticalBoxSpectra[ unIdx ].spec;
	}

	return	pSpec;
}

// set number of select list
void	VerticalBoxSpectrumCanvas::setIndex( const int nIndex )
{
	m_nIndex = nIndex;

	return;
}

// get number of select list
int VerticalBoxSpectrumCanvas::getIndex( void )
{
	return	m_nIndex;
}

// search selected spectrum
int VerticalBoxSpectrumCanvas::searchSelectedSpectrum( kome::objects::Spectrum* pSpec )
{
	// search
	int	nIdx = -1;
	for ( int i = 0 ; i < (int)m_verticalBoxSpectra.size( ) && nIdx < 0 ; i++ )
	{
		if  ( m_verticalBoxSpectra[ i ].spec == pSpec
				|| m_verticalBoxSpectra[ i ].spec->getOrgSpectrum( ) == pSpec->getOrgSpectrum( ) )
		{
			nIdx = i;
		}
	}

	return	nIdx;
}

// set call function ( selected spectrum )
void	VerticalBoxSpectrumCanvas::setCallFunctionSelectedSpectrum( kome::objects::Spectrum* pSpec )
{
	kome::plugin::PluginManager&	plgMgr = kome::plugin::PluginManager::getInstance( );
	kome::plugin::PluginFunctionItem*	item = plgMgr.getFunctionItem( "select_spectrum", "verticalbox" );
	
	kome::objects::Parameters	params;
	kome::plugin::PluginCallTool::setSpectrum( params, *pSpec );
	
	if  ( item != NULL )
	{
		item->getCall( )->invoke( &params );
	}

	return;
}

// exchange data
void	VerticalBoxSpectrumCanvas::exchange( const int nS, const int nE )
{
	SpecInfo	insTemp;

	insTemp.spec = m_verticalBoxSpectra[nS].spec;
	insTemp.insColor = m_verticalBoxSpectra[nS].insColor;
	insTemp.bFlg = m_verticalBoxSpectra[nS].bFlg;
	insTemp.nNum = m_verticalBoxSpectra[nS].nNum;

	m_verticalBoxSpectra[nS].spec = m_verticalBoxSpectra[nE].spec;
	m_verticalBoxSpectra[nS].insColor = m_verticalBoxSpectra[nE].insColor;
	m_verticalBoxSpectra[nS].bFlg = m_verticalBoxSpectra[nE].bFlg;
	m_verticalBoxSpectra[nS].nNum = m_verticalBoxSpectra[nE].nNum;

	m_verticalBoxSpectra[nE].spec = insTemp.spec;
	m_verticalBoxSpectra[nE].insColor = insTemp.insColor;
	m_verticalBoxSpectra[nE].bFlg = insTemp.bFlg;
	m_verticalBoxSpectra[nE].nNum = insTemp.nNum;

	// restruct m_dummy
	m_dummy.clearSpectra( );
	for ( unsigned int i = 0 ; i < m_verticalBoxSpectra.size( ) ; i++ )
	{
		m_dummy.addSpectrum( m_verticalBoxSpectra[i].spec );
	}

	update( );

	return;
}

// common action for mouse event
kome::objects::Spectrum*	VerticalBoxSpectrumCanvas::onMouseCommonAction( kome::evt::MouseEvent& evt )
{
	m_nIndexDisp = getClickObjectNumber( evt );
	kome::objects::Spectrum*	pSpec = getTransformedSpec( );
	// reset
	m_nIndexDisp = -1;

	return	pSpec;
}

// マウスのボタン押下位置がどの表示オブジェクトの位置かを求めます
int		VerticalBoxSpectrumCanvas::getClickObjectNumber( kome::evt::MouseEvent& evt )
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

// get spectrum object at m_nListDisp
kome::objects::Spectrum*	VerticalBoxSpectrumCanvas::getTransformedSpec( )
{
	kome::objects::Spectrum*	pTransformedSpec = (kome::objects::Spectrum *)NULL;
	kome::objects::Spectrum*	pSpec = (kome::objects::Spectrum *)NULL;
	pTransformedSpec = getBaseSpectrum( );
	if  ( (kome::objects::Spectrum *)NULL != pTransformedSpec )
	{
		pSpec = pTransformedSpec->getOrgSpectrum( );
	}

	return	pSpec;
}

// set index number on canvas
void	VerticalBoxSpectrumCanvas::setIndexDisp( const int nIndexDisp )
{
	m_nIndexDisp = nIndexDisp;

	return;
}

// get index number on canvas
int VerticalBoxSpectrumCanvas::getIndexDisp( void )
{
	return	m_nIndexDisp;
}

// >>>>>	@Date:2013/05/17	<Add>	A.Ozaki

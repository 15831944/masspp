/**
 * @file MdiChildFrameManager.cpp
 * @brief implements of MdiChildFrameManager class
 *
 * @author S.Tanaka
 * @date 2007.09.23
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "MdiChildFrameManager.h"

#include <boost/bind.hpp>


using namespace kome::view;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



#define MS1_ICON_NAME			 "MS1"
#define MS2_ICON_NAME			 "MS2"
#define MS3_ICON_NAME			 "MS3"
#define MS4_ICON_NAME			 "MS4"
#define MS5_ICON_NAME			 "MS5"
#define MS6_ICON_NAME			 "MS6"
#define MS7_ICON_NAME			 "MS7"
#define MSN_ICON_NAME			 "MSn"

#define SPEC_WINDOW_FUNC_TYPE		"WINDOW"
#define CHROM_WINDOW_FUNC_TYPE	    "WINDOW"
#define DATA_MAP_WINDOW_FUNC_TYPE	"DATA_MAP_WINDOW"

#define AUTO_ARRANGE_SECTION        "Auto Arrange Window"
#define AUTO_ARRANGE_NUM_KEY        "FIXED_NUM"
#define AUTO_ARRANGE_COL_ROW_KEY    "COL_ROW"

#define STANDARD_CHILD_STYLE         ( wxMINIMIZE_BOX | wxMAXIMIZE_BOX | wxTHICK_FRAME | wxSYSTEM_MENU | wxCAPTION )
#define AUTO_ARRANGE_CHILD_STYLE     ( wxSYSTEM_MENU | wxCAPTION )


// constructor
MdiChildFrameManager::MdiChildFrameManager() {
	m_order = -255;
	m_movingChild = NULL;
	m_movingPos.px = 0;
	m_movingPos.py = 0;
	m_movePos.px = 0;
	m_movePos.py = 0;
	m_startRect.left = 0;
	m_startRect.top = 0;
	m_startRect.right = 0;
	m_startRect.bottom = 0;
	m_clientSize.px = 0;
	m_clientSize.py = 0;

	m_flg = false;
	m_moving = false;
}

// destructor
MdiChildFrameManager::~MdiChildFrameManager() {
}

// open spectrum
void MdiChildFrameManager::openSpectrum( kome::objects::Spectrum* spectrum ) {

	// SPEC 91535
	// command line version ?
	if( kome::window::WindowTool::getMainWindow() == NULL ){
		// command line version
		return;
	}
	// ↓for GUI version

	// check parameter
	if( spectrum == NULL ) {
		return;
	}

	// icon manager object
	kome::window::IconManager& iconMgr = kome::window::IconManager::getInstance();

	// style
	kome::objects::StatusManager& statusMgr = kome::objects::StatusManager::getInstance();
	const char* s = statusMgr.getValue( AUTO_ARRANGE_STATUS_NAME );
	std::string ss = NVL( s, "" );

	bool autoArrange = ( ss.compare( AUTO_ARRANGE_STATUS_VALUE ) == 0 );
	int style = (  autoArrange ? AUTO_ARRANGE_CHILD_STYLE : STANDARD_CHILD_STYLE );
	int sPos = ( autoArrange ? 3000 : -1 );

	// create new child frame
	kome::window::ChildFrame* frame = new kome::window::ChildFrame( sPos, sPos, -1, -1, style );

	// icon
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

	wxIcon* icon = iconMgr.getIcon( spectrum->getIcon() );
	if( icon == NULL ) {
		// stage
		int stage = spectrum->getMsStage() - 1;
		stage = CLAMP( stage, 0, 7 );

		std::string iconName = iconNames[ stage ];

		// get icon
		spectrum->setIcon( iconName.c_str() );
		icon = iconMgr.getIcon( iconName.c_str() );
	}

	if( icon != NULL ) {
		frame->SetIcon( *icon );
	}

	// window
	wxWindow* wnd = kome::plugin::PluginCallTool::createSpectrumWindow( *spectrum, frame );
	kome::window::Canvas* canvas = dynamic_cast< kome::window::Canvas* >( wnd );

	if( wnd == NULL ) {
		LOG_WARN( FMT( "Failed to create spectrum window." ) );

		frame->Close();
		frame->Destroy();
		delete frame;
	}
	else {
		// get old frame
		kome::window::ChildFrame* oldFrame = getChildFrame( wnd );

		// show
		if( frame == oldFrame || oldFrame == NULL ) {
			if( canvas == NULL ) {
				frame->addSpectrum( spectrum );
				frame->setCanvas( wnd );
			}
			else {
				canvas->addSpectrum( spectrum );
			}
			frame->Show( true );
			frame->Activate();
			
			m_frameSet.insert( frame );
		}
		else {
			frame->Close();
			frame->Destroy();
			delete frame;

			if( canvas == NULL ) {
				oldFrame->addSpectrum( spectrum );
			}
			else {
				canvas->addSpectrum( spectrum );
			}
			oldFrame->Activate();
			oldFrame->Refresh();
		}
	}

	layoutChildren( NULL );
}

// open chromatogram
void MdiChildFrameManager::openChromatogram( kome::objects::Chromatogram* chrom ) {

	// command line version ?
	if( kome::window::WindowTool::getMainWindow() == NULL ){
		// command line version
		return;
	}
	// ↓for GUI version

	// check parameter
	if( chrom == NULL ) {
		return;
	}

	// icon manager object
	kome::window::IconManager& iconMgr = kome::window::IconManager::getInstance();

	// style
	kome::objects::StatusManager& statusMgr = kome::objects::StatusManager::getInstance();
	const char* s = statusMgr.getValue( AUTO_ARRANGE_STATUS_NAME );
	std::string ss = NVL( s, "" );

	bool autoArrange = ( ss.compare( AUTO_ARRANGE_STATUS_VALUE ) == 0 );
	int style = (  autoArrange ? AUTO_ARRANGE_CHILD_STYLE : STANDARD_CHILD_STYLE );
	int sPos = ( autoArrange ? 3000 : -1 );

	// create new child frame
	kome::window::ChildFrame* frame = new kome::window::ChildFrame( sPos, sPos, -1, -1, style );

	// icon
	std::string iconName = chrom->getIcon();
	wxIcon* icon = iconMgr.getIcon( iconName.c_str() );
	if( icon == NULL ) {
		icon = iconMgr.getIcon( "chromatogram" );
	}
	if( icon != NULL ) {
		frame->SetIcon( *icon );
	}

	// window
	wxWindow* wnd = kome::plugin::PluginCallTool::createChromatogramWindow( *chrom, frame );
	kome::window::Canvas* canvas = dynamic_cast< kome::window::Canvas* >( wnd );

	if( wnd == NULL ) {
		LOG_WARN( FMT( "Failed to create chromatogram window." ) );

		frame->Close();
		frame->Destroy();
		delete frame;
	}
	else {
		// get old frame
		kome::window::ChildFrame* oldFrame = getChildFrame( wnd );

		// show
		if( frame == oldFrame || oldFrame == NULL ) {
			if( canvas == NULL ) {
				frame->addChromatogram( chrom );
				frame->setCanvas( wnd );
			}
			else {
				canvas->addChromatogram( chrom );
			}
			frame->Show( true );
			frame->Activate();

			m_frameSet.insert( frame );
		}
		else {
			frame->Close();
			frame->Destroy();
			delete frame;

			if( canvas == NULL ) {
				oldFrame->addChromatogram( chrom );
			}
			else {
				canvas->addChromatogram( chrom );
			}
			oldFrame->Activate();
			oldFrame->Refresh();
		}
	}

	layoutChildren( NULL );
}

// open data map
void MdiChildFrameManager::openDataMap( kome::objects::DataMapInfo* dataMap ) {
	// check parameter
	if( dataMap == NULL ) {
		return;
	}

	// style
	kome::objects::StatusManager& statusMgr = kome::objects::StatusManager::getInstance();
	const char* s = statusMgr.getValue( AUTO_ARRANGE_STATUS_NAME );
	std::string ss = NVL( s, "" );

	bool autoArrange = ( ss.compare( AUTO_ARRANGE_STATUS_VALUE ) == 0 );
	int style = (  autoArrange ? AUTO_ARRANGE_CHILD_STYLE : STANDARD_CHILD_STYLE );
	int sPos = ( autoArrange ? 3000 : -1 );

	// create new child frame
	kome::window::ChildFrame* frame = new kome::window::ChildFrame( sPos, sPos, -1, -1, style );

	// window
	wxWindow* wnd = kome::plugin::PluginCallTool::createDataMapWindow( *dataMap, frame );
	kome::window::Canvas* canvas = dynamic_cast< kome::window::Canvas* >( wnd );

	if( wnd == NULL ) {
		LOG_WARN( FMT( "Failed to create data map window." ) );

		frame->Close();
		frame->Destroy();
		delete frame;
	}
	else {
		// get old frame
		kome::window::ChildFrame* oldFrame = getChildFrame( wnd );

		// show
		if( frame == oldFrame || oldFrame == NULL ) {
			if( canvas == NULL ) {
				frame->addDataMap( dataMap );
				frame->setCanvas( wnd );
			}
			else {
				canvas->addDataMap( dataMap );
			}
			frame->Show( true );
			frame->Activate();

			m_frameSet.insert( frame );
		}
		else {
			frame->Close();
			frame->Destroy();
			delete frame;

			if( canvas == NULL ) {
				oldFrame->addDataMap( dataMap );
			}
			else {
				canvas->addDataMap( dataMap );
			}
			oldFrame->Activate();
			oldFrame->Refresh();
		}
	}

	layoutChildren( NULL );
}

// close spectrum frames
void MdiChildFrameManager::closeAllSpecFrames() {
	// get close frame
	std::set< kome::window::ChildFrame* > frameSet;
	for( unsigned int i = 0; i < kome::window::ChildFrame::getNumberOfFrames(); i++ ) {
		// frame
		kome::window::ChildFrame* frame = kome::window::ChildFrame::getFrame( i );

		// check
		if( frame->getNumberOfSpectra() > 0 && m_frameSet.find( frame ) != m_frameSet.end() ) {
			frameSet.insert( frame );
		}
	}

	// close
	for( std::set< kome::window::ChildFrame* >::iterator it = frameSet.begin();
			it != frameSet.end(); it++ ) {
		(*it)->Close();
		(*it)->Destroy();
		delete (*it);
	}
}

// close chromatogram frames
void MdiChildFrameManager::closeAllChromFrames() {
	// get close frame
	std::set< kome::window::ChildFrame* > frameSet;
	for( unsigned int i = 0; i < kome::window::ChildFrame::getNumberOfFrames(); i++ ) {
		// frame
		kome::window::ChildFrame* frame = kome::window::ChildFrame::getFrame( i );

		// check
		if( frame->getNumberOfChromatograms() > 0 && m_frameSet.find( frame ) != m_frameSet.end() ) {
			frameSet.insert( frame );
		}
	}

	// close
	for( std::set< kome::window::ChildFrame* >::iterator it = frameSet.begin();
			it != frameSet.end(); it++ ) {
		(*it)->Close();
		(*it)->Destroy();
		delete (*it);
	}
}

// close data map frames
void MdiChildFrameManager::closeDataMapFrames() {
	// get close frame
	std::set< kome::window::ChildFrame* > frameSet;
	for( unsigned int i = 0; i < kome::window::ChildFrame::getNumberOfFrames(); i++ ) {
		// frame
		kome::window::ChildFrame* frame = kome::window::ChildFrame::getFrame( i );

		// check
		if( frame->getNumberOfDataMaps() > 0 && m_frameSet.find( frame ) != m_frameSet.end() ) {
			frameSet.insert( frame );
		}
	}

	// close
	for( std::set< kome::window::ChildFrame* >::iterator it = frameSet.begin();
			it != frameSet.end(); it++ ) {
		(*it)->Close();
		(*it)->Destroy();
		delete (*it);
	}
}

// get specified spectrum frame
kome::window::ChildFrame* MdiChildFrameManager::getSpectrumFrame( kome::objects::Spectrum* spec ) {
	// search
	for( std::set< kome::window::ChildFrame* >::iterator it = m_frameSet.begin();
			it != m_frameSet.end(); it++ ) {
		// frame
		kome::window::ChildFrame* frame = *it;

		// compare
		for( unsigned int i = 0; i < frame->getNumberOfSpectra(); i++ ) {
			kome::objects::Spectrum* tmp = frame->getSpectrum( i );
			if( tmp == spec ) {
				return frame;
			}
		}
	}

	return NULL;
}

// get specified chromatogram frame
kome::window::ChildFrame* MdiChildFrameManager::getChromatogramFrame( kome::objects::Chromatogram* chrom ) {
	// search
	for( std::set< kome::window::ChildFrame* >::iterator it = m_frameSet.begin();
			it != m_frameSet.end(); it++ ) {
		// frame
		kome::window::ChildFrame* frame = *it;

		// compare
		for( unsigned int i = 0; i < frame->getNumberOfChromatograms(); i++ ) {
			kome::objects::Chromatogram* tmp = frame->getChromatogram( i );
			if( tmp == chrom ) {
				return frame;
			}
		}
	}

	return NULL;
}

// get specified data map frame
kome::window::ChildFrame* MdiChildFrameManager::getDataMapFrame( kome::objects::DataMapInfo* dataMap ) {
	// search
	for( std::set< kome::window::ChildFrame* >::iterator it = m_frameSet.begin();
			it != m_frameSet.end(); it++ ) {
		// frame
		kome::window::ChildFrame* frame = *it;

		// compare
		for( unsigned int i = 0; (frame != NULL) && i < frame->getNumberOfDataMaps(); i++ ) {
			kome::objects::DataMapInfo* tmp = frame->getDataMap( i );
			if( dataMap == tmp ) {
				return frame;
			}
		}
	}
	return NULL;
}

// get child frame
kome::window::ChildFrame* MdiChildFrameManager::getChildFrame( wxWindow* childWnd ) {
	// search
	for( unsigned int i = 0; i < kome::window::ChildFrame::getNumberOfFrames(); i++ ) {
		// frame
		kome::window::ChildFrame* frame = kome::window::ChildFrame::getFrame( i );
		if( frame != NULL ){
			// children
			wxWindowList& children = frame->GetChildren();
			for( unsigned int i = 0; i < children.GetCount(); i++ ) {
				if( childWnd == children.Item( i )->GetData() ) {
					return frame;
				}
			}
		}
	}
	return NULL;
}

// update child frame style
void MdiChildFrameManager::updateStyle() {
	// style
	int style = wxDEFAULT_FRAME_STYLE;

	// status
	kome::objects::StatusManager& statusMgr = kome::objects::StatusManager::getInstance();
	const char* s = statusMgr.getValue( AUTO_ARRANGE_STATUS_NAME );
	std::string ss = NVL( s, "" );

	if( ss.compare( AUTO_ARRANGE_STATUS_VALUE ) == 0 ) {    // auto arrange window mode
		style = AUTO_ARRANGE_CHILD_STYLE;
	}
	else {    // standard
		style = STANDARD_CHILD_STYLE;
	}
	
	// set style
	const unsigned int num = kome::window::ChildFrame::getNumberOfFrames();
	for( unsigned int i = 0; i < num; i++ ) {
		kome::window::ChildFrame* child = kome::window::ChildFrame::getFrame( i );
		if( child != NULL ){
			child->SetWindowStyle( style );	
			child->UpdateWindowUI();
		}
	}

	kome::window::WindowTool::refresh();
}

// set moving position
void MdiChildFrameManager::setMovingPosition( kome::window::ChildFrame* child, const int px, const int py ) {
	// check the flag
	if( m_flg ) {
		return;
	}

	// position
	if( child == m_movingChild ) {
		if( m_movingPos.px == px && m_movingPos.py == py && child != NULL && !m_moving ) {    // swap the position
			m_flg = true;

			// get frame to swap
			int num = (int)kome::window::ChildFrame::getNumberOfFrames();
			kome::window::ChildFrame* dst = NULL;
			wxPoint mousePt = wxGetMousePosition();
			kome::window::MainFrame* mainFrame = (kome::window::MainFrame*)kome::window::WindowTool::getMainWindow();
			wxWindow* clientWnd = mainFrame->GetClientWindow();
			for( int i = 0; i < num && dst == NULL; i++ ) {
				kome::window::ChildFrame* tmp = kome::window::ChildFrame::getFrame( i );
				if( tmp != child ) {
					wxRect rect = tmp->GetRect();
					wxPoint pt( rect.GetX(), rect.GetY() );
					wxPoint screenPt = clientWnd->ClientToScreen( pt );
					rect.SetX( screenPt.x );
					rect.SetY( screenPt.y );
					
					if( rect.Contains( mousePt ) ) {
						dst = tmp;
					}
				}
			}

			// arrange windows
			if( dst == NULL ) {    // restore
				mainFrame->setTimer(
					boost::bind( &MdiChildFrameManager::moveChild, this, child, m_startRect.left, m_startRect.top ),
					100,
					true
				);
			}
			else {    // swap
				wxPoint dstPt = dst->GetPosition();
				wxSize dstSize = dst->GetSize();

				dst->setFixedSize( m_startRect.getWidth(), m_startRect.getHeight() );
				dst->SetPosition( wxPoint( m_startRect.left, m_startRect.top ) );

				child->setFixedSize( dstSize.x, dstSize.y );

				mainFrame->setTimer(
					boost::bind( &MdiChildFrameManager::moveChild, this, child, dstPt.x, dstPt.y ),
					100,
					true
				);
			}
			m_movingChild = NULL;

			m_flg = false;
		}
	}
	else {
		m_movingChild = child;

		if( child != NULL ) {
			wxPoint pt = child->GetPosition();
			wxSize size = child->GetSize();

			m_startRect.left = pt.x;
			m_startRect.top = pt.y;
			m_startRect.right = pt.x + size.GetWidth();
			m_startRect.bottom = pt.y + size.GetHeight();
		}
	}

	// set the position
	m_movingPos.px = px;
	m_movingPos.py = py;
	m_moving = true;
}

// set move position
void MdiChildFrameManager::setMovePosition( kome::window::ChildFrame* child, const int px, const int py ) {
	// check the flag
	if( m_flg ) {
		return;
	}

	// set the position
	m_movePos.px = px;
	m_movePos.py = py;
	m_moving = false;
}

// set client size
void MdiChildFrameManager::setClientSize( const int w, const int h ) {
	m_clientSize.px = w;
	m_clientSize.py = h;

	layoutChildren( NULL );
}

// layout children
void MdiChildFrameManager::layoutChildren( kome::window::ChildFrame* closingChild, const bool sortFlg ) {
	// flag
	m_flg = true;

	// status
	kome::objects::StatusManager& statusMgr = kome::objects::StatusManager::getInstance();
	const char* s = statusMgr.getValue( AUTO_ARRANGE_STATUS_NAME );
	std::string ss = NVL( s, "" );

	if( ss.compare( AUTO_ARRANGE_STATUS_VALUE ) == 0 ) {    // auto arrange window mode
		// child frame count
		int num = (int)kome::window::ChildFrame::getNumberOfFrames();

		// child frames
		std::vector< kome::window::ChildFrame* > frames;
		for( int i = 0; i < num; i++ ) {
			kome::window::ChildFrame* frame = kome::window::ChildFrame::getFrame( i );
			if( frame != closingChild ) {
				frames.push_back( kome::window::ChildFrame::getFrame( i ) );
			}
		}

		num = (int)frames.size();

		// column & row
		kome::core::MsppManager& msppMgr = kome::core::MsppManager::getInstance();
		kome::core::IniFile* ini = msppMgr.getIniFile();

		int col = 0;
		int row = 0;
		if( ini != NULL ) {
			std::string colRow = ini->getString( AUTO_ARRANGE_SECTION, AUTO_ARRANGE_COL_ROW_KEY, "col" );
			bool colFlg = ( colRow.compare( "row" ) != 0 );
			int fixedNum = ini->getInt( AUTO_ARRANGE_SECTION, AUTO_ARRANGE_NUM_KEY, 2 );

			if( colFlg ) {
				col = std::max( fixedNum, 1 );
				row = num / col;
				if( num % col > 0 ) {
					row += 1;
				}
			}
			else {
				row = std::max( fixedNum, 1 );
				col = num / row;
				if( num % row > 0 ) {
					col += 1;
				}
			}
		}

		if( frames.size() > 0 && sortFlg ) {
			std::sort( frames.begin(), frames.end(), lessChildY );
			for( int i = 0; i < row; i++ ) {
				int start = i * col;
				int end = col * ( i + 1 );
				if( end > num ) {
					end = num;
				}
				std::sort( frames.begin() + start, frames.begin() + end, lessChildX );
			}
		}

		// size
		int w = m_clientSize.px;
		int h = m_clientSize.py;

		// layout
		int prevY = 0;
		for( int i = 0; i < row; i++ ) {
			int nextY = prevY + ( h - prevY ) / ( row - i );
			int prevX = 0;
			int colNum = std::min( col, num - i * col );
			for( int j = 0; j < colNum; j++ ) {
				int nextX = prevX + ( w - prevX ) / ( colNum - j );

				int idx = i * col + j;
				if( idx < (int)frames.size() ) {
					kome::window::ChildFrame* child = frames[ idx ];
					child->setFixedSize( nextX - prevX, nextY - prevY );
					const wxPoint pt = wxPoint( prevX, prevY );
					try {
						child->SetPosition( pt );
					}
					catch( std::exception& e )
					{
						const char* szException = e.what();
					}catch( ... )
					{
						;
					}
				}
				prevX = nextX;
			}
			prevY = nextY;
		}

		// clear set
		m_freeFrameSet.clear();
	}
	else {    // standard
		// set fixed size
		const unsigned int num = kome::window::ChildFrame::getNumberOfFrames();
		for( unsigned int i = 0; i < num; i++ ) {
			kome::window::ChildFrame* child = kome::window::ChildFrame::getFrame( i );
// >>>>>>	@Date:2013/12/17	<Modified:SPEC96098>	A.Ozaki
// 

			// 最大化中にプロファイルウィンドウの大きさが、MainFrameのサイズ変更に
			// 追従しない不具合の対策
			//
			if  ( NULL != child )
			{
				if  ( child->IsMaximized( ) )
				{
					// 最大化中は、強制的にウィンドウサイズを変更します
					//
					child->setFixedSize( -1, -1 );
					wxSize size = child->GetSize();
					// サイズを変更すると、フレーム枠の太さ分が差し引かれたサイズで
					// 通知されるようで、固定値を加算しています
					// どこからから取得できれば良いのですが、今のところ不明です
					size.x = m_clientSize.px + 16;
					size.y = m_clientSize.py + 38;
					child->SetSize( size );
					child->UpdateWindowUI( );
				}
				else if  ( m_freeFrameSet.find( child ) == m_freeFrameSet.end( ) )
				{
					child->setFixedSize( -1, -1 );
					wxSize size = child->GetSize();
					size.x = size.x + 1;
					size.y = size.y + 1;
					child->SetSize( size );
					child->UpdateWindowUI();
					m_freeFrameSet.insert( child );
				}
			}
//
// <<<<<<	@Date:2013/12/17	<Modified:SPEC96098>	A.Ozaki
		}
	}

	m_flg = false;
}

// move child
void MdiChildFrameManager::moveChild( kome::window::ChildFrame* child, const int px, const int py ) {
	// check the parameter
	if( child == NULL ) {
		return;
	}

	m_flg = true;

	child->SetPosition( wxPoint( px, py ) );

	m_flg = false;
}

// tile vertically
void MdiChildFrameManager::tileVertically() {
	// frames
	std::vector< kome::window::ChildFrame* > frames;
	for( unsigned int i = 0; i < kome::window::ChildFrame::getNumberOfFrames(); i++ ) {
		kome::window::ChildFrame* frame = kome::window::ChildFrame::getFrame( i );
		if( ( frame != NULL )&& !frame->IsIconized() ) {
			if( frame->IsMaximized() ) {
				frame->Restore();
			}
			frames.push_back( frame );
		}
	}
	if( frames.empty() ) {
		return;
	}

	// tile
	int pos = 0;
	int num = (int)frames.size();

	for( unsigned int i = 0; i < frames.size(); i++ ) {
		kome::window::ChildFrame* frame = frames[ i ];
		int w = m_clientSize.px;
		int h = ( m_clientSize.py - pos ) / num;
		num -= 1;
		
		if( frame != NULL ){
			frame->SetSize( wxSize( w, h ) );
			moveChild( frame, 0, pos );
		}
		pos += h;
	}
}

// set auto arrange status
bool MdiChildFrameManager::setAutoArrangeStatus() {
	// page
	kome::plugin::ParameterSettings params( NULL, true );
	kome::plugin::SettingsPage* page = new kome::plugin::SettingsPage( params );

	// size
	kome::plugin::SettingsValue* val = params.addValue();
	val->setRequired( true );
	val->setType( kome::plugin::SettingsValue::TYPE_INT );
	val->getNumberRestriction().setInclusiveMinValue( 1.0 );
	val->getNumberRestriction().setInclusiveMaxValue( 10.0 );
	val->setDefaultValue( "2" );

	kome::plugin::SettingsParam*  param = params.addParam();
	param->setName( "num" );
	param->setSection( AUTO_ARRANGE_SECTION );
	param->setKey( AUTO_ARRANGE_NUM_KEY );
	param->setValue( val );

	kome::plugin::SettingsForm*  form = page->addForm();
	form->setParam( param );
	form->setTitle( "Fixed Num." );
	form->setDesc( "Set the fixed number.");

	// Columns / Rows
	val = params.addValue();
	val->setRequired( true );
	val->setType( kome::plugin::SettingsValue::TYPE_STRING );
	val->addEnumItem( "Columns", "col", "The number of columns is fixed." );
	val->addEnumItem( "Rows", "row", "The number of rows is fixed." );
	val->setDefaultValue( "col" );

	param = params.addParam();
	param->setName( "colrow" );
	param->setSection( AUTO_ARRANGE_SECTION );
	param->setKey( AUTO_ARRANGE_COL_ROW_KEY );
	param->setValue( val );

	form = page->addForm();
	form->setParam( param );
	form->setTitle( "Column / Row" );
	form->setDesc( "Select column or row." );

	// dialog
	kome::window::SettingsPageDialog dlg(
		kome::window::WindowTool::getMainWindow(),
		page,
		NULL,
		true,
		NULL,
		false
	);

	return ( dlg.ShowModal() == wxID_OK );
}

// on close sample
void MdiChildFrameManager::onCloseSample( kome::objects::Sample* sample, const bool deleting ) {
	// get close frame
	std::set< kome::window::ChildFrame* > frameSet;
	for( unsigned int i = 0; i < kome::window::ChildFrame::getNumberOfFrames(); i++ ) {
		// frame
		kome::window::ChildFrame* frame = kome::window::ChildFrame::getFrame( i );
		kome::window::Canvas* canvas = dynamic_cast< kome::window::Canvas* >( frame == NULL ? NULL : frame->getCanvas() );

		if( frame == NULL ){
			continue;
		}

	    if( frame->getSample() == NULL ) {
			for( int j = (int)frame->getNumberOfSpectra() - 1; j >= 0; j-- ) {
				kome::objects::Spectrum* spec = frame->getSpectrum( j );
				if( spec->getSample() == sample ) {
					if( canvas == NULL ) {
						frame->removeSpectrum( spec );
					}
					else {
						canvas->removeSpectrum( spec );
					}
				}
			}

			for( int j = (int)frame->getNumberOfChromatograms() - 1; j >= 0; j-- ) {
				kome::objects::Chromatogram* chrom = frame->getChromatogram( j );
				if( chrom->getSample() == sample ) {
					if( canvas == NULL ) {
						frame->removeChromatogram( chrom );
					}
					else {
						canvas->removeChromatogram( chrom );
					}
				}
			}

			for( int j = (int)frame->getNumberOfDataMaps() - 1; j >= 0; j-- ) {
				kome::objects::DataMapInfo* dataMap = frame->getDataMap( j );
				if( dataMap->getSample() == sample ) {
					if( canvas == NULL ) {
						frame->removeDataMap( dataMap );
					}
					else {
						canvas->removeDataMap( dataMap );
					}
				}
			}
		}
		else if( frame->getSample() == sample ) {
			frameSet.insert( frame );
		}
	}

	// close
	for( std::set< kome::window::ChildFrame* >::iterator it = frameSet.begin();
			it != frameSet.end(); it++ ) {
		kome::window::ChildFrame* frame = *it;
		if( frame != NULL ){ // NULL チェック @date 2013.06.19 <Add> M.Izumi
			frame->Close();
			frame->Destroy();
			delete frame;
		}
	}
}

// compare by x
bool MdiChildFrameManager::lessChildX( kome::window::ChildFrame* child0, kome::window::ChildFrame* child1 ) {
	// check parameters
	if( child0 == NULL ) {
		return false;
	}
	if( child1 == NULL ) {
		return true;
	}

	// x coordinates
	wxPoint pt0 = child0->GetPosition();
	wxPoint pt1 = child1->GetPosition();

	return ( pt0.x < pt1.x );
}

// compare by y
bool MdiChildFrameManager::lessChildY( kome::window::ChildFrame* child0, kome::window::ChildFrame* child1 ) {
	// check parameters
	if( child0 == NULL ) {
		return false;
	}
	if( child1 == NULL ) {
		return true;
	}

	// x coordinates
	wxPoint pt0 = child0->GetPosition();
	wxPoint pt1 = child1->GetPosition();

	return ( pt0.y < pt1.y );
}

// get instance
MdiChildFrameManager& MdiChildFrameManager::getInstance() {
	// create object
	static MdiChildFrameManager mgr;

	return mgr;
}

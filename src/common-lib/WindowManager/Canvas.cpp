/**
 * @file Canvas.cpp
 * @brief implements of Canvas class
 *
 * @author S.Tanaka
 * @date 2007.03.04
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */



#include "stdafx.h"
#include "Canvas.h"

#include "ChildFrame.h"

#include <wx/timer.h>
#include <wx/dcbuffer.h>
#include <wx/utils.h>

#include "MainFrame.h"		// @date 2011.04.20 <Add> M.Izumi


using namespace kome::window;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



#define ID_CLICK_TIMER		1000
#define CLICK_EVENT_INTERVAL   500

#define APP_NAME					  "Mass++"	// @date 2011.04.20 <Add> M.Izumi


BEGIN_EVENT_TABLE( Canvas, wxScrolledWindow )
	EVT_ERASE_BACKGROUND( Canvas::onEraseBackgroundEvt )
	EVT_SIZE( Canvas::onSizeEvt )
	EVT_MOUSE_EVENTS( Canvas::onMouseEvt )
	EVT_TIMER( ID_CLICK_TIMER, Canvas::onTimerEvt )
	EVT_MENU_RANGE( ID_POPUP_MENU_MIN, ID_POPUP_MENU_MAX, Canvas::onMenuPopup )
END_EVENT_TABLE()


// static member
std::vector< Canvas* > Canvas::m_canvases;

std::map< int, kome::plugin::PluginMenu* > Canvas::m_contextMenuTreeMap;
int Canvas::m_uCntContextMenuTreeMap;


// constructor
Canvas::Canvas( wxWindow* parent, const int id, const int width, const int height )
		: wxScrolledWindow(
			parent,
			id,
			wxDefaultPosition,
			wxSize( width, height ),
			wxSUNKEN_BORDER
		) {
	// initialize
	m_btnUp = false;
	m_initSize = false;
	m_width = width;
	m_height = height;
	m_pushFlg = true;

	m_cursorPos.px = -1;
	m_cursorPos.py = -1;
	m_acObj = NULL;

	m_frame = dynamic_cast< ChildFrame* >( parent );
	if( m_frame != NULL ) {
		m_frame->setCanvas( this );
	}
	m_canvasStyle = 0;

	// set background
	SetBackgroundColour( wxColour( 255, 255, 255 ) );

	// create timer
	m_clickTimer = new wxTimer( this, ID_CLICK_TIMER );

	// event object
	m_btnDownEvt = new kome::evt::MouseEvent();
	m_btnUpEvt = new kome::evt::MouseEvent();

	// add to the array
	m_canvases.push_back( this );

	// set index
	m_index = m_canvases.size()-1;

	// context menu
	Canvas::m_uCntContextMenuTreeMap = ID_POPUP_MENU_MIN;

	m_flgSetSub = false;
}

// destructor
Canvas::~Canvas() {
	// delete timer
	if( m_clickTimer->IsRunning() ) {
		m_clickTimer->Stop();
	}
	delete m_clickTimer;
	m_clickTimer = NULL;

	// delete event object
	delete m_btnDownEvt;
	delete m_btnUpEvt;

	// delete active object
	if( m_acObj != NULL ) {
		delete m_acObj;
		m_acObj = NULL;
	}

	// delete from the array
	for( int i = (int)m_canvases.size() - 1; i >= 0; i-- ) {
		if( m_canvases[ i ] == this ) {
			m_canvases.erase( m_canvases.begin() + i );
		}
	}
}

// clear objects
void Canvas::clearObjects() {
	clearSpectra();
	clearChromatograms();
	clearDataMaps();
}

// clear spectra
void Canvas::clearSpectra() {
	// on remove
	for( unsigned int i = 0; i < getNumberOfSpectra(); i++ ) {
		onRemoveSpectrum( getSpectrum( i ) );
	}

	// clear
	if( m_frame == NULL ) {
		for( unsigned int i = 0; i < m_spectra.size(); i++ ) {
			kome::objects::Spectrum* spec = m_spectra[ i ];
			if( spec != NULL ) {
				kome::plugin::PluginCallTool::onCloseSpectrum( *spec, this );
			}
		}

		m_spectra.clear();
	}
	else {
		m_frame->clearSpectra();
	}
}

// add spectrum
void Canvas::addSpectrum( kome::objects::Spectrum* spec ) {
	// spectrum
	spec = onAddSpectrum( spec );
	if( spec == NULL ) {
		return;
	}

	// set
	if( m_frame == NULL ) {
		for( unsigned int i = 0; i < m_spectra.size(); i++ ) {
			if( m_spectra[ i ] == spec ) {
				return;
			}
		}
		m_spectra.push_back( spec );
	}
	else {
		m_frame->addSpectrum( spec );
	}	
}

// set spectrum
void Canvas::setSpectrum( kome::objects::Spectrum* spec ) {
	clearObjects();
	addSpectrum( spec );
}

// remove spectrum
void Canvas::removeSpectrum( kome::objects::Spectrum* spec ) {
	// prepare
	onRemoveSpectrum( spec );

	// remove
	if( m_frame == NULL ) {
		bool flg = false;
		for( int i = (int)m_spectra.size() - 1; i >= 0; i-- ) {
			if( m_spectra[ i ] == spec ) {
				m_spectra.erase( m_spectra.begin() + i );
				flg = true;
			}
		}

		if( flg && spec != NULL ) {
			kome::plugin::PluginCallTool::onCloseSpectrum( *spec, this );
		}
	}
	else {
		m_frame->removeSpectrum( spec );
	}
}

// get the number of spectra
unsigned int Canvas::getNumberOfSpectra() {
	if( m_frame == NULL ) {
		return m_spectra.size();
	}

	return m_frame->getNumberOfSpectra();
}

// get spectrum
kome::objects::Spectrum* Canvas::getSpectrum( const unsigned int idx ) {
	if( m_frame == NULL ) {
		if( idx >= m_spectra.size() ) {
			return NULL;
		}
		return m_spectra[ idx ];
	}

	return m_frame->getSpectrum( idx );
}

// clears chromatogram
void Canvas::clearChromatograms() {
	// prepare
	for( unsigned int i = 0; i < getNumberOfChromatograms(); i++ ) {
		onRemoveChromatogram( getChromatogram( i ) );
	}

	// clear
	if( m_frame == NULL ) {
		for( unsigned int i = 0; i < m_chroms.size(); i++ ) {
			kome::objects::Chromatogram* chrom = m_chroms[ i ];
			if( chrom != NULL ) {
				kome::plugin::PluginCallTool::onCloseChromatogram( *chrom, this );
			}
		}

		m_chroms.clear();
	}
	else {
		m_frame->clearChromatograms();
	}
}

// add chromatogram
void Canvas::addChromatogram( kome::objects::Chromatogram* chrom ) {
	// chromatogram
	chrom = onAddChromatogram( chrom );
	if( chrom == NULL ) {
		return;
	}

	// add
	if( m_frame == NULL ) {
		for( unsigned int i = 0; i < m_chroms.size(); i++ ) {
			if( m_chroms[ i ] == chrom ) {
				return;
			}
		}
		m_chroms.push_back( chrom );
	}
	else {
		m_frame->addChromatogram( chrom );
	}
}

// set chromatogram
void Canvas::setChromatogram( kome::objects::Chromatogram* chrom ) {
	clearChromatograms();
	addChromatogram( chrom );
}

// remove chromatogram
void Canvas::removeChromatogram( kome::objects::Chromatogram* chrom ) {
	// prepare
	onRemoveChromatogram( chrom );

	// remove
	if( m_frame == NULL ) {
		bool flg = false;
		for( int i = (int)m_chroms.size() - 1; i >= 0; i-- ) {
			if( m_chroms[ i ] == chrom ) {
				m_chroms.erase( m_chroms.begin() + i );
				flg = true;
			}
		}

		if( flg && chrom != NULL ) {
			kome::plugin::PluginCallTool::onCloseChromatogram( *chrom, this );
		}
	}
	else {
		m_frame->removeChromatogram( chrom );
	}
}

// get the number of chromatograms
unsigned int Canvas::getNumberOfChromatograms() {
	if( m_frame == NULL ) {
		return m_chroms.size();
	}

	return m_frame->getNumberOfChromatograms();
}

// get chromatogram
kome::objects::Chromatogram* Canvas::getChromatogram( const unsigned int idx ) {
	if( m_frame == NULL ) {
		if( idx >= m_chroms.size() ) {
			return NULL;
		}
		return m_chroms[ idx ];
	}

	return m_frame->getChromatogram( idx );
}

// clear data maps
void Canvas::clearDataMaps() {
	// prepare
	for( unsigned int i = 0; i < getNumberOfDataMaps(); i++ ) {
		onRemoveDataMap( getDataMap( i ) );
	}

	// clear
	if( m_frame == NULL ) {
		for( unsigned int i = 0; i < m_datamaps.size(); i++ ) {
			kome::objects::DataMapInfo* dataMap = m_datamaps[ i ];
			if( dataMap != NULL ) {
				kome::plugin::PluginCallTool::onCloseDataMap( *dataMap, this );
			}
		}

		m_datamaps.clear();
	}
	else {
		m_frame->clearDataMaps();
	}
}

// add data map
void Canvas::addDataMap( kome::objects::DataMapInfo* dataMap ) {
	// data map
	dataMap = onAddDataMap( dataMap );
	if( dataMap == NULL ) {
		return;
	}

	// add
	if( m_frame == NULL ) {
		for( unsigned int i = 0; i < m_datamaps.size(); i++ ) {
			if( m_datamaps[ i ] == dataMap ) {
				return;
			}
		}
		m_datamaps.push_back( dataMap );
	}
	else {
		m_frame->addDataMap( dataMap );
	}
}

// set data map
void Canvas::setDataMap( kome::objects::DataMapInfo* dataMap ) {
	clearDataMaps();
	addDataMap( dataMap );
}

// remove data map
void Canvas::removeDataMap( kome::objects::DataMapInfo* dataMap ) {
	// prepare
	onRemoveDataMap( dataMap );

	// remove
	if( m_frame == NULL ) {
		bool flg = false;
		for( int i = (int)m_datamaps.size() - 1; i >= 0; i-- ) {
			if( m_datamaps[ i ] == dataMap ) {
				m_datamaps.erase( m_datamaps.begin() + i );
				flg = true;
			}
		}

		if( flg && dataMap != NULL ) {
			kome::plugin::PluginCallTool::onCloseDataMap( *dataMap, this );
		}
	}
	else {
		m_frame->removeDataMap( dataMap );
	}
}

// get the number of data maps
unsigned int Canvas::getNumberOfDataMaps() {
	if( m_frame == NULL ) {
		return m_datamaps.size();
	}

	return m_frame->getNumberOfDataMaps();
}

// get data map
kome::objects::DataMapInfo* Canvas::getDataMap( const unsigned int idx ) {
	if( m_frame == NULL ) {
		if( idx >= m_datamaps.size() ) {
			return NULL;
		}

		return m_datamaps[ idx ];
	}

	return m_frame->getDataMap( idx );
}

// set active object
void Canvas::setActiveObject( kome::objects::Variant& obj ) {
	if( m_frame == NULL ) {
		if( obj.prim.pt == NULL ) {    // unset
			if( m_acObj != NULL ) {
				delete m_acObj;				
				m_acObj = NULL;			
			}
		}
		else {    // set
			if( m_acObj == NULL ) {
				m_acObj = new kome::objects::Variant;
			}
			*m_acObj = obj;
		}
	}
	else {
		m_frame->setActiveObject( obj );
	}
}

// get active object
kome::objects::Variant Canvas::getActiveObject() {
	// get active object
	if( m_frame != NULL ) {
		return m_frame->getActiveObject();
	}

	if( m_acObj != NULL ) {
		return *m_acObj;
	}

	// create active object
	kome::objects::Variant ret;

	if( m_spectra.size() == 1 && m_chroms.size() == 0 && m_datamaps.size() == 0 ) {         // check spectra
		ret.type = kome::objects::Variant::SPECTRUM;
		ret.prim.pt = m_spectra[ 0 ];
	}
	else if( m_spectra.size() == 0 && m_chroms.size() == 1 && m_datamaps.size() == 0 ) {    // chck chromatograms
		ret.type = kome::objects::Variant::CHROMATOGRAM;
		ret.prim.pt = m_chroms[ 0 ];
	}
	else if( m_spectra.size() == 0 && m_chroms.size() == 0 && m_datamaps.size() == 1 ) {    // check datamaps
		ret.type = kome::objects::Variant::DATA_MAP;
		ret.prim.pt = m_datamaps[ 0 ];
	}
	else {
		std::set< kome::objects::DataGroupNode* > groupSet;
		std::set< kome::objects::Sample* > sampleSet;

		// groups
		for( unsigned int i = 0; i < m_spectra.size(); i++ ) {
			groupSet.insert( m_spectra[ i ]->getGroup() );
		}
		for( unsigned int i = 0; i < m_chroms.size(); i++ ) {
			groupSet.insert( m_chroms[ i ]->getGroup() );
		}
		for( unsigned int i = 0; i < m_datamaps.size(); i++ ) {
			groupSet.insert( &( m_datamaps[ i ]->getGroup() ) );
		}
		groupSet.erase( NULL );

		// samples
		for( std::set< kome::objects::DataGroupNode* >::iterator it = groupSet.begin();
				it != groupSet.end(); it++ ) {
			sampleSet.insert( (*it)->getSample() );
		}
		sampleSet.erase( NULL );

		if( groupSet.size() == 1 ) {
			ret.type = kome::objects::Variant::DATA_GROUP_NODE;
			ret.prim.pt = *( groupSet.begin() );
		}
		else if( sampleSet.size() == 1 ) {
			ret.type = kome::objects::Variant::SAMPLE;
			ret.prim.pt = *( sampleSet.begin() );
		}
	}

	return ret;
}

// get canvas style		
int Canvas::getCanvasStyle(){
	return m_canvasStyle;	
}

// get index
int Canvas::getIndex(){
	return m_index;
}

// popup menu
void Canvas::popupMenu( const int px, const int py ) {
	if( GetCapture() == this ) {
		ReleaseMouse();
    }

	// point
	wxPoint pt( px, py );

	// create menu
	wxMenu* menu = createMenu();
	if( menu == NULL ) {
		return;
	}

	
	// popup menu
	PopupMenu( menu, pt.x, pt.y );
	delete menu;
}

// create menu
wxMenu* Canvas::createMenu() {
	return NULL;
}

// draw
void Canvas::draw( wxDC& dc ) {
	onDraw( dc );
}

// on add spectrum
kome::objects::Spectrum* Canvas::onAddSpectrum( kome::objects::Spectrum* spec ) {
	return spec;
}

// on remove spectrum
void Canvas::onRemoveSpectrum( kome::objects::Spectrum* spec ) {
}

// on add chromatogram
kome::objects::Chromatogram* Canvas::onAddChromatogram( kome::objects::Chromatogram* chrom ) {
	return chrom;
}

// on remove chromatogram
void Canvas::onRemoveChromatogram( kome::objects::Chromatogram* chrom ) {
}

// on add data map
kome::objects::DataMapInfo* Canvas::onAddDataMap( kome::objects::DataMapInfo* dataMap ) {
	return dataMap;
}

// on remove 
void Canvas::onRemoveDataMap( kome::objects::DataMapInfo* dataMap ) {
}

// on draw
void Canvas::onDraw( wxDC& dc ) {
}

// on seize
void Canvas::onSize( const unsigned int width, const unsigned int height ) {
	m_width = width;
	m_height = height;
}

// on mouse button down
bool Canvas::onMouseButtonDown( kome::evt::MouseEvent& evt ) {
	return false;
}

// on mouse button up
bool Canvas::onMouseButtonUp( kome::evt::MouseEvent& evt ) {
	return false;
}

// on mouse button double click
bool Canvas::onMouseDoubleClick( kome::evt::MouseEvent& evt ) {
	return false;
}

// on mouse button double click
bool Canvas::onMouseRightDoubleClick( kome::evt::MouseEvent& evt ) {
	return false;
}

// on mouse cursor move
bool Canvas::onMouseCursorMove( kome::evt::MouseEvent& evt ) {
	return false;
}

// on mouse cursor enter
bool Canvas::onMouseCursorEnter( kome::evt::MouseEvent& evt ) {
	return false;
}

// on mouse cursor leave
bool Canvas::onMouseCursorLeave( kome::evt::MouseEvent& evt ) {
	return false;
}

// click
void Canvas::click() {
	// flag
	m_pushFlg = true;

	// button down event
	if( onMouseButtonDown( *m_btnDownEvt ) ) {
		WindowTool::refresh();
	}

	// button up event
	if( m_btnUp ) {
		if( onMouseButtonUp( *m_btnUpEvt ) ) {
			WindowTool::refresh();
		}
	}
}

// on draw
void Canvas::OnDraw( wxDC& dc ) {
	// init size
	if( !m_initSize ) {
		// get size
		int width = 0;
		int height = 0;
		GetSize( &width, &height );

		if( width >= 0 && height >= 0 ) {
			onSize( width, height );
		}
		m_initSize = true;
	}

	// buffered dc
	wxBufferedPaintDC memDc( this );

	// erase
	wxColour bgColor = GetBackgroundColour();
	if( !bgColor.Ok() ) {
		bgColor = wxSystemSettings::GetColour( wxSYS_COLOUR_3DFACE );
	}
	memDc.SetBrush( wxBrush( bgColor ) );
	memDc.SetPen( wxPen( bgColor, 1 ) );


	wxRect windowRect( wxPoint( 0, 0 ), GetClientSize() );

	CalcUnscrolledPosition( windowRect.x, windowRect.y, &windowRect.x, &windowRect.y );

	memDc.DrawRectangle( windowRect );

	// draw
	m_tooltip.clear();
	onDraw( memDc );

	if( m_displayedTooltip.compare( m_tooltip ) != 0 ) {
		m_displayedTooltip = m_tooltip;

		SetToolTip( wxT( m_tooltip.c_str() ) );
	}
}

// on erase background event
void Canvas::onEraseBackgroundEvt( wxEraseEvent& evt ) {
}

// on size
void Canvas::onSizeEvt( wxSizeEvent& evt ) {
	// set size
	onSize( evt.GetSize().GetWidth(), evt.GetSize().GetHeight() );
	ChildFrame* ch = ChildFrame::getActiveFrame();
	kome::window::MainFrame* main = (MainFrame*)kome::window::WindowTool::getMainWindow();
	
	if( ch != NULL ){
		kome::objects::Sample* sample = ch->getSample();
		if( sample != NULL ){
			kome::objects::SampleSet* sampleSet = sample->getSampleSet();
			if( sampleSet != NULL ){
				std::string strName = sampleSet->getFileName();
				if( !ch->IsMaximized() ){
					if (sample->isEdited())
					{
						main->SetTitle( FMT( "*%s - %s", strName.c_str(), APP_NAME ));
					}
					else
					{
						main->SetTitle( FMT( "%s - %s", strName.c_str(), APP_NAME ));
					}
				}else{
					main->SetTitle( FMT( "%s", APP_NAME ));
				}
			}
		}
	}else{
		main->SetTitle( FMT( "%s", APP_NAME ));
	}

	// refresh
	Refresh();
}

// on mouse
void Canvas::onMouseEvt( wxMouseEvent& evt ) {
	// set cursor position
	wxPoint pt = evt.GetPosition();
	m_cursorPos.px = pt.x;
	m_cursorPos.py = pt.y;

	// create object
	kome::evt::MouseEvent me;

	// set keybord flag
	me.shift( evt.ShiftDown() );
	me.ctrl( evt.ControlDown() );
	me.alt( evt.AltDown() );

	// get coordinates
	me.x( pt.x );
	me.y( pt.y );
	
	// get button flag and function type
	if( evt.ButtonDown() ) {		// button down event
		me.lbutton( evt.LeftDown() );
		me.mbutton( evt.MiddleDown() );
		me.rbutton( evt.RightDown() );

		// set timer
		*m_btnDownEvt = me;
		m_btnUp = false;
		m_clickTimer->Start( CLICK_EVENT_INTERVAL, true );

		if( HasCapture() ){	// @date 2013/03/13 <Add> OKADA
			;	// this window has the current mouse capture.	
		}else{	// @date 2013/03/13 <Add> OKADA
			// capture
			CaptureMouse();
		}	// @date 2013/03/13 <Add> OKADA
	}
	else if( evt.ButtonUp() ) {		// button up event
		// check the flag
		if( !m_pushFlg ) {

			// release	// @date 2013/03/13 <Add> OKADA for SPEC 91597
			if( HasCapture() ) {
				ReleaseMouse();
			}

			return;
		}

		// getn infromation
		me.lbutton( evt.LeftUp() );
		me.mbutton( evt.MiddleUp() );
		me.rbutton( evt.RightUp() );

		if( m_clickTimer->IsRunning() ) {
			// set timer information
			*m_btnUpEvt = me;
			m_btnUp = true;
		}
		else {
			// call event
			if( onMouseButtonUp( me ) ) {
				WindowTool::refresh();
			}
		}

		// restore flag
		m_pushFlg = false;

		// release
		if( HasCapture() ) {
			ReleaseMouse();
		}
	}
	// 右クリック
	else if( evt.RightDown() ){
		int ddd=0;
		int jjj=0;
	}
	else if( evt.RightUp() ){
		// 右クリックメニュー表示
		int ddd=0;
		int jjj=0;
	}
	else if( evt.ButtonDClick() ) { // double click event
		// stop click event
		if( m_clickTimer->IsRunning() ) {
			m_clickTimer->Stop();
		}

		// double click event
		me.lbutton( evt.LeftDClick() );
		me.mbutton( evt.MiddleDClick() );
		me.rbutton( evt.RightDClick() );

		// wait cursor
		wxBeginBusyCursor();

		// mouse click
		if( onMouseDoubleClick( me ) ) {
			WindowTool::refresh();
		}

		// restore cursor
		wxEndBusyCursor();

	}
	else {
		// call click event
		if( m_clickTimer->IsRunning() ) {
			if( evt.Moving() ) {
				return;
			}
			else {
				m_clickTimer->Stop();
				click();
			}
		}

		// call mouse event
		me.lbutton( evt.LeftIsDown() );
		me.mbutton( evt.MiddleIsDown() );
		me.rbutton( evt.RightIsDown() );

		if( evt.Moving() || evt.Dragging() ) {		// mouse move
			if( onMouseCursorMove( me ) ) {
				WindowTool::refresh();
			}
		}
		else if( evt.Leaving() ) {  // mouse leave
			if( onMouseCursorLeave( me ) ) {
				WindowTool::refresh();
			}
		}
		else if( evt.Entering() ) { // mouse enter
			if( onMouseCursorEnter( me ) ) {
				WindowTool::refresh();
			}
		}
	}
}

// on timer
void Canvas::onTimerEvt( wxTimerEvent& evt ) {
	if( evt.GetId() == ID_CLICK_TIMER ) {   // click timer event
		click();
	}
}

// gets the number of canvasees
unsigned int Canvas::getNumberOfCanvases() {
	return m_canvases.size();
}

// gets the canvas
Canvas* Canvas::getCanvas( const unsigned int idx ) {
	if( idx >= m_canvases.size() ) {
		return NULL;
	}

	return m_canvases[ idx ];
}


// sets the sub view flag
void Canvas::setSubView( bool flgSetSub ){
	m_flgSetSub = flgSetSub;
};

// gets the sub view flag
bool Canvas::isSubView( void ){
	return m_flgSetSub;

};

// create popup menu
wxMenu* Canvas::createPopupMenu( kome::plugin::PluginMenu* parentInfo, wxMenu* ptrMenu ){
	if( ptrMenu == NULL ){
		ptrMenu = new wxMenu();
	}

	Canvas::m_uCntContextMenuTreeMap = ID_POPUP_MENU_MIN;	// pop-upメニューの中で一意であれば良いので、毎回先頭番号を振り直す。
	Canvas::m_contextMenuTreeMap.clear();

	createPopupMenuRecursive( parentInfo, ptrMenu );

	return ptrMenu;
};

// create popup menu recursive
void Canvas::createPopupMenuRecursive( kome::plugin::PluginMenu* parentInfo, wxMenu* parentMenu ){
	// この中で、メニューを再帰的に呼び出す

	unsigned int uNumberOfChildren = parentInfo->getNumberOfChildren();

	std::vector< std::pair< double, int> > v_nOrderTable;
	std::vector< std::pair< double, int> >::iterator it; 
	
	for( unsigned int i=0; i<uNumberOfChildren; i++ ){
		kome::plugin::PluginMenu* ptrMenu = parentInfo->getChildMenu( i );
		if( ptrMenu != NULL ){
			v_nOrderTable.push_back( std::pair<double, int>(  ptrMenu->getPopupOrder(0), i ) );
		}
	}
	// orderの順にAppendする
	std::sort( v_nOrderTable.begin(), v_nOrderTable.end() );
	it = v_nOrderTable.begin();

	// group name
	std::string strPrevGroupName;

	for( unsigned int j=0; j<uNumberOfChildren; j++ )
	{
		int index;
		index = it->second;
		it++;
		
		kome::plugin::PluginMenu* menuInfo = parentInfo->getChildMenu( index );

		// description
		std::string description = menuInfo->getDescription();

		kome::plugin::PluginCall* call = menuInfo->getCallExec();	// @date 2013/02/21 <Add> OKADA
		// @date 2013/04/17 <Add> OKADA	------->
		if( call != NULL ){
			if( call->isActive() ){
				// OK
			}else{
				LOG_TRACE_CODE( FMT( "Function call was not invoked. FuncName = '%s' is not active.", call->getName() ), ERR_OTHER );
				continue;	// activeでない関数は呼び出さない
			}
		}
		// @date 2013/04/17 <Add> OKADA <-------

		if( menuInfo->getNumberOfChildren() == 0 ){
			// leaf
			std::string strMenuName = menuInfo->getName();

			// separator
			if( 0 < j ){
				if( strPrevGroupName.compare( NVL( menuInfo->getGroupName(), "" ) ) != 0 ){
					parentMenu->AppendSeparator();
				}
			}
			strPrevGroupName = std::string( NVL( menuInfo->getGroupName(), "" ) );

			// item
			m_contextMenuTreeMap.insert( std::map<int, kome::plugin::PluginMenu*>::value_type( Canvas::m_uCntContextMenuTreeMap, menuInfo ) );

			bool bCheckable = menuInfo->isCheckable();
			wxMenuItem* item = new wxMenuItem(
				parentMenu,
				Canvas::m_uCntContextMenuTreeMap,
				wxT( strMenuName.c_str() ),
				wxT( description.c_str() ),
				menuInfo->isCheckable() ? wxITEM_CHECK : wxITEM_NORMAL
			);
			item->SetCheckable( bCheckable );
			parentMenu->Append( item );

			Canvas::m_uCntContextMenuTreeMap++;

		}else{
			// internal node
			wxMenu* ptrSubMenu = new wxMenu();

			// call method recursively
			createPopupMenuRecursive( menuInfo, ptrSubMenu );

			// @date 2013/04/17 <Add> OKADA ------>
			size_t nMenuItemCnt = ptrSubMenu->GetMenuItemCount();

			if( nMenuItemCnt == 0 ){
				delete ptrSubMenu;
				continue;
			}
			// @date 2013/04/17 <Add> OKADA <-------

			// @date 2012/12/12 <Add> OKADA ------->
			// separator
			if( 0 < j ){
				if( strPrevGroupName.compare( NVL( menuInfo->getGroupName(), "" ) ) != 0 ){
					parentMenu->AppendSeparator();
				}
			}
			strPrevGroupName = std::string( NVL( menuInfo->getGroupName(), "" ) );
			// @date 2012/12/12 <Add> OKADA <-------

			parentMenu->Append(
				Canvas::m_uCntContextMenuTreeMap
				, wxT( menuInfo->getName() )
				, ptrSubMenu
			);

			Canvas::m_uCntContextMenuTreeMap++;
		}
	}

	unsigned int uNumberOfChildrenDebug = parentInfo->getNumberOfChildren();

	return;
};

// execute popup menu
void executePopupMenu( unsigned int nId ){

	return;
}

// on menu popup
void Canvas::onMenuPopup( wxCommandEvent& evt )
{
	// ID
	unsigned int id = evt.GetId();

	kome::plugin::PluginMenu* ptrMenu = m_contextMenuTreeMap[id];

	bool flgRet = ptrMenu->execute( this->getActiveObject() );

	if( flgRet ){
		WindowTool::refresh();
	}

	return;
}

// Gets the spectrum displayed on the canvas
void Canvas::getSpecDispCanvas( std::vector< kome::objects::Spectrum* >&spectra ){
}

// Gets the chromatogram displayed on the canvas
void Canvas::getChromDispCanvas( std::vector< kome::objects::Chromatogram* >&chroms ){
}

/**
 * @file ChildFrame.cpp
 * @brief implements of ChildFrame class
 *
 * @author S.Tanaka
 * @date 2006.09.11
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "ChildFrame.h"

#include "WindowTool.h"
#include "MainFrame.h"
#include "Canvas.h"

#include <boost/bind.hpp>
#include "StatusBar.h"


using namespace kome::window;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



#define UNSET_SPEC_TIME				  500
#define DOCK_WND_VERTICAL_GAP		  5
#define DOCK_WND_HORIZONTAL_GAP		  5

#define CHILD_FRAME_MOVE_PLUGIN_TYPE      "ON_CHILD_FRAME_MOVE"
#define CHILD_FRAME_MOVING_PLUGIN_TYPE    "ON_CHILD_FRAME_MOVING"


BEGIN_EVENT_TABLE( ChildFrame, wxMDIChildFrame )
	EVT_ACTIVATE( ChildFrame::onActivate )
	EVT_CLOSE( ChildFrame::onClose )
	EVT_MOVING( ChildFrame::onMoving )
	EVT_MOVE( ChildFrame::onMove )
	EVT_SIZE( ChildFrame::onSize )
END_EVENT_TABLE()


// static member
std::vector< ChildFrame* > ChildFrame::m_frames;

// constructor
ChildFrame::ChildFrame( const int px, const int py, const int w, const int h, const long style ) : wxMDIChildFrame( 
		dynamic_cast< MainFrame* >( WindowTool::getMainWindow() ),
		wxID_ANY,
		wxT( "" ),
		wxPoint( px, py ),
		wxSize( w, h ),
		style
) {
	// initialize
	m_canvas = NULL;

	m_sample = NULL;
	m_group = NULL;
	m_acObj = NULL;
	m_fixedSize.first = -1;
	m_fixedSize.second = -1;

	m_frames.push_back( this );

	// update window menu
	MainFrame* mainFrame = (MainFrame*)WindowTool::getMainWindow();
	mainFrame->updateChildWindowMenu();
}

// destructor
ChildFrame::~ChildFrame() {
	// delete from array
	int idx = -1;
	for( unsigned int i = 0; i < m_frames.size() && idx < 0; i++ ) {
		if( m_frames[ i ] == this ) {
			idx = i;
		}
	}
	if( idx >= 0 ) {
		m_frames.erase( m_frames.begin() + idx );
	}

	// delete active object
	if( m_acObj != NULL ) {
		delete m_acObj;
	}
	m_acObj = NULL;
	
}

// set canvas
void ChildFrame::setCanvas( wxWindow* canvas ) {
	m_canvas = canvas;
}

// get canvas
wxWindow* ChildFrame::getCanvas() {
	return m_canvas;
}

// set fixed size
void ChildFrame::setFixedSize( const int width, const int height ) {
	// set the attribute
	m_fixedSize.first = width;
	m_fixedSize.second = height;

	// on size
	wxSize size = GetSize();
	if( width > 0 && height > 0 ) {
		size.x = width;
		size.y = height;

		if( IsMaximized() ) {
			Maximize( false );
		}
		if( IsIconized() ) {
			Iconize( false );
		}
	}

	SetSize( size );
}

// get the width of the fixed size
int ChildFrame::getFixedWidth() {
	return m_fixedSize.first;
}

// get the height of the fixed size
int ChildFrame::getFixedHeight() {
	return m_fixedSize.second;
}

// clear active objects
void ChildFrame::clearObjects() {
	clearSpectra();
	clearChromatograms();
	clearDataMaps();
}

// clear spectra
void ChildFrame::clearSpectra() {
	// close
	for( unsigned int i = 0; i < m_spectra.size(); i++ ) {
		kome::objects::Spectrum* spec = m_spectra[ i ];
		if( spec != NULL ) {
			kome::plugin::PluginCallTool::onCloseSpectrum( *spec, m_canvas );
		}
	}

	// clear
	m_spectra.clear();

	// update
	updateSampleAndGroup();
}

// clear chromatogram
void ChildFrame::clearChromatograms() {
	// close
	for( unsigned int i = 0; i < m_chroms.size(); i++ ) {
		kome::objects::Chromatogram* chrom = m_chroms[ i ];
		if( chrom != NULL ) {
			kome::plugin::PluginCallTool::onCloseChromatogram( *chrom, m_canvas );
		}
	}

	// clear
	m_chroms.clear();

	// update
	updateSampleAndGroup();
}

// clear datamaps
void ChildFrame::clearDataMaps() {
	// close
	for( unsigned int i = 0; i < m_datamaps.size(); i++ ) {
		kome::objects::DataMapInfo* dataMap = m_datamaps[ i ];
		if( dataMap != NULL ) {
			kome::plugin::PluginCallTool::onCloseDataMap( *dataMap, m_canvas );
		}
	}

	// clear
	m_datamaps.clear();

	// update
	updateSampleAndGroup();
}

// set active spectrum
void ChildFrame::setSpectrum( kome::objects::Spectrum* spec ) {
	clearObjects();
	addSpectrum( spec );
}

// set active chromatogram
void ChildFrame::setChromatogram( kome::objects::Chromatogram* chrom ) {
	clearObjects();
	addChromatogram( chrom );
}

// set active data map
void ChildFrame::setDataMap( kome::objects::DataMapInfo* dataMap ) {
	clearObjects();
	addDataMap( dataMap );
}

// add active spectrum
void ChildFrame::addSpectrum( kome::objects::Spectrum* spec ) {
	// check spectrum
	if( spec == NULL ) {
		return;
	}
	for( unsigned int i = 0; i < m_spectra.size(); i++ ) {
		if( spec == m_spectra[ i ] ) {
			return;
		}
	}

	// update active objects
	m_spectra.push_back( spec );
	updateSampleAndGroup();
		
	// open
	kome::objects::DataManager::openSpectrum( spec );

	// add open spectra				@date 2011.10.03 <Add> M.Izumi
	kome::objects::ActiveObjectsManager& aoMgr = kome::objects::ActiveObjectsManager::getInstance();
	kome::window::Canvas* canvas = dynamic_cast< kome::window::Canvas* >( getCanvas() );
	int style = ( canvas == NULL ? 0 : canvas->getCanvasStyle() );
	if( !( style & kome::window::Canvas::OVERLAPPING ) && !(style & kome::window::Canvas::MULTIPLE_OBJECTS )){
		aoMgr.addOpenSpectra( spec );
	}
}

// add active chromatogram
void ChildFrame::addChromatogram( kome::objects::Chromatogram* chrom ) {
	// check chromatogram
	if( chrom == NULL ) {
		return;
	}
	for( unsigned int i = 0; i < m_chroms.size(); i++ ) {
		if( chrom == m_chroms[ i ] ) {
			return;
		}
	}

	// update active objects
	m_chroms.push_back( chrom );
	updateSampleAndGroup();

	// open
	kome::objects::DataManager::openChromatogram( chrom );
}

// add active data map
void ChildFrame::addDataMap( kome::objects::DataMapInfo* dataMap ) {
	// check data map
	if( dataMap == NULL ) {
		return;
	}
	for( unsigned int i = 0; i < m_datamaps.size(); i++ ) {
		if( dataMap == m_datamaps[ i ] ) {
			return;
		}
	}

	// update active objects
	m_datamaps.push_back( dataMap );
	updateSampleAndGroup();

	// open
	kome::objects::DataManager::openDataMap( dataMap );
}

// get the number of active spectra
unsigned int ChildFrame::getNumberOfSpectra() {
	return m_spectra.size();
}

// get active spectrum
kome::objects::Spectrum* ChildFrame::getSpectrum( const unsigned int index ) {
	if( index >= m_spectra.size() ) {
		return NULL;
	}
	return m_spectra[ index ];
}

// get the number of active chromatograms
unsigned int ChildFrame::getNumberOfChromatograms() {
	return m_chroms.size();
}

// get active chromatogram
kome::objects::Chromatogram* ChildFrame::getChromatogram( const unsigned int index ) {
	if( index >= m_chroms.size() ) {
		return NULL;
	}
	return m_chroms[ index ];
}

// get the number of active data maps
unsigned int ChildFrame::getNumberOfDataMaps() {
	return m_datamaps.size();
}

// get active data map
kome::objects::DataMapInfo* ChildFrame::getDataMap( const unsigned int index ) {
	if( index >= m_datamaps.size() ) {
		return NULL;
	}
	return m_datamaps[ index ];
}

// get active sample
kome::objects::Sample* ChildFrame::getSample() {
	return m_sample;
}

// get active spectrum group
kome::objects::DataGroupNode* ChildFrame::getGroup() {
	return m_group;
}

// remove active spectra	2010.11.02 M.Izumi
void ChildFrame::removeSpectrum( kome::objects::Spectrum* spec ) {
	// remove
	bool flg = false;
	for( int i = (int)m_spectra.size() - 1; i >= 0; i-- ) {
		if( m_spectra[ i ] == spec ) {
			m_spectra.erase( m_spectra.begin() + i );
			flg = true;
		}
	}

	// close spectrum
	if( flg && spec != NULL ) {
		kome::plugin::PluginCallTool::onCloseSpectrum( *spec, m_canvas );
	}

	// update
	updateSampleAndGroup();
}

// remove active chromatogram	2010.11.02 M.Izumi
void ChildFrame::removeChromatogram( kome::objects::Chromatogram* chrom ) {
	// remove
	bool flg = false;
	for( int i = (int)m_chroms.size() - 1; i >= 0; i-- ) {
		if( m_chroms[ i ] == chrom ) {
			m_chroms.erase( m_chroms.begin() + i );
			flg = true;
		}
	}

	// close chromatogram
	if( flg && chrom != NULL ) {
		kome::plugin::PluginCallTool::onCloseChromatogram( *chrom, m_canvas );
	}

	// update
	updateSampleAndGroup();
}

// remove active DataMap	2010.11.02 M.Izumi
void ChildFrame::removeDataMap( kome::objects::DataMapInfo* dataMap ) {
	// remove
	bool flg = false;
	for( int i = (int)m_datamaps.size() - 1; i >= 0; i-- ) {
		if( m_datamaps[ i ] == dataMap ) {
			m_datamaps.erase( m_datamaps.begin() + i );
			flg = true;
		}
	}

	if( flg && dataMap != NULL ) {
		kome::plugin::PluginCallTool::onCloseDataMap( *dataMap, m_canvas );
	}

	// update
	updateSampleAndGroup();
}

// set active object
void ChildFrame::setActiveObject( kome::objects::Variant& obj ) {
	if( obj.prim.pt == NULL ) {    // unset
		if( m_acObj != NULL ) {
			delete m_acObj;
			m_acObj = NULL;
		}
	}
	else {  // set
		if( m_acObj == NULL ) {
			m_acObj = new kome::objects::Variant;
		}
		*m_acObj = obj;
	}
}

// get active object
kome::objects::Variant ChildFrame::getActiveObject() {
	// check the member
	if( m_acObj != NULL ) {
		return *m_acObj;
	}

	// return value
	kome::objects::Variant ret;

	// set the active object
	if( m_spectra.size() == 1 && m_chroms.size() == 0 && m_datamaps.size() == 0 ) {  // check spectra
		ret.type = kome::objects::Variant::SPECTRUM;
		ret.prim.pt = m_spectra[ 0 ];
	}
	else if( m_spectra.size() == 0 && m_chroms.size() == 1 && m_datamaps.size() == 0 ) {  // chck chromatograms
		ret.type = kome::objects::Variant::CHROMATOGRAM;
		ret.prim.pt = m_chroms[ 0 ];
	}
	else if( m_spectra.size() == 0 && m_chroms.size() == 0 && m_datamaps.size() == 1 ) {    // check datamaps
		ret.type = kome::objects::Variant::DATA_MAP;
		ret.prim.pt = m_datamaps[ 0 ];
	}
	else if( m_group != NULL ) {    // check the group
		ret.type = kome::objects::Variant::DATA_GROUP_NODE;
		ret.prim.pt = m_group;
	}
	else if( m_sample != NULL ) {    // check the sample
		ret.type = kome::objects::Variant::SAMPLE;
		ret.prim.pt = m_sample;
	}

	return ret;
}

// update active sample and group
void ChildFrame::updateSampleAndGroup() {
	// initialize
	m_group = NULL;
	m_sample = NULL;

	// groups
	std::set< kome::objects::DataGroupNode* > groupSet;	
	std::set< kome::objects::Sample* > sampleSet;

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

	// set group and sample
	if( groupSet.size() == 1 ) {
		m_group = *( groupSet.begin() );
	}

	if( sampleSet.size() == 1 ) {
		m_sample = *( sampleSet.begin() );
	}

	// title
	setTitle();

	// activate
	if( getActiveFrame() == this ) {
		callActivate( true );
	}
}

// set frame title
void ChildFrame::setTitle() { 
	// active objects count
	unsigned int specCnt = m_spectra.size();
	unsigned int chromCnt = m_chroms.size();
	unsigned int dpCnt = m_datamaps.size();

	// group
	kome::objects::DataGroupNode* group = m_group;

	// suffix
	std::string suffix;
	if( m_sample != NULL ) {
		suffix = m_sample->getName();

		kome::objects::SampleSet* sampleSet = m_sample->getSampleSet();
		if( sampleSet != NULL ) {
			if( !suffix.empty() ) {
				suffix.append( " - " );
			}
			suffix.append( sampleSet->getFileName() );
		}
	}

	// title
	std::string title;
	if( specCnt == 1 && chromCnt == 0 && dpCnt == 0 ) {		        // spectrum title
		kome::objects::Spectrum* spec = m_spectra[ 0 ];
		title = spec->getTitle();
	}
	else if( specCnt == 0 && chromCnt == 1 && dpCnt == 0 ) {		// chromatogram title
		kome::objects::Chromatogram* chrom = m_chroms[ 0 ];
		title = chrom->getTitle();
	}
	else {
		if( specCnt == 0 && chromCnt == 0 && dpCnt == 1 ) {		    // data map title
			title = "";
		}
		else {		// title
			if( specCnt > 0 ) {
				title = FMT( "%d spectra", specCnt );
			}
			if( chromCnt > 0 ) {
				if( !title.empty() ) {
					title.append( ", " );
				}
				title.append( FMT( "%d chromatograms", chromCnt ) );
			}
			if( dpCnt > 0 ) {
				if( !title.empty() ) {
					title.append( ", " );
				}
				title.append( FMT( "%d data maps", dpCnt ) );
			}
		}

		// group
		if( group != NULL && group->getParentGroup() != NULL ) {
			if( title.empty() ) {
				title = group->getName();
			}
			else {
				title.append( FMT( " (%s)", group->getName() ) );
			}
		}
	}

	// title + suffix
	if( title.empty() ) {
		title = suffix;
	}
	else if( !suffix.empty() ) {
		title = FMT( "%s - %s", title.c_str(), suffix.c_str() );
	}

	// tag
	kome::objects::ActiveObjectsManager& aoMgr = kome::objects::ActiveObjectsManager::getInstance();
	std::string tag = NVL( aoMgr.getSampleTagName( m_sample ), "" );
	if( !tag.empty() ) {
		title = FMT( "%s %s", tag.c_str(), title.c_str() );
	}

	// set title
	SetTitle( wxT( title.c_str() ) );

	// update window menu
	kome::window::MainFrame* main
		= dynamic_cast< kome::window::MainFrame* >( kome::window::WindowTool::getMainWindow() );
	if( main != NULL ) {
		main->updateChildWindowMenu();
	}
}

// judges whether this frame is active
bool ChildFrame::isActive() {
	// get active frame
	ChildFrame* frame = getActiveFrame();

	return ( frame == NULL );
}

// on activate
void ChildFrame::onActivate( wxActivateEvent& evt ) {
	// active frame
	ChildFrame* acFrame = getActiveFrame();

	// canvas
	bool flg = false;
	if( evt.GetActive() ) {    // activate
		flg = true;
	}
	else {
		if( acFrame == this ) {
			flg = true;
		}
	}

	// change active canvas
	if( flg ) {
		if( evt.GetActive() ) {    // set
			callActivate( true );
		}
		else {    // unset
			// call plug-in functions
			WindowTool::setTimer(
				boost::bind( ChildFrame::callActivate, false ),
				UNSET_SPEC_TIME,
				true
			);
		}
	}
}

// on close
void ChildFrame::onClose( wxCloseEvent& evt ) {
	// veto & skip
	if( evt.CanVeto() ) {
		evt.Veto();
	}

	// close
	for( unsigned int i = 0; i < getNumberOfSpectra(); i++ ) {
		kome::objects::Spectrum* spec = getSpectrum( i );
		kome::plugin::PluginCallTool::onCloseSpectrum( *spec, getCanvas() );
		// @date 2011.10.03 <Add> M.Izumi
		kome::objects::ActiveObjectsManager& aoMgr = kome::objects::ActiveObjectsManager::getInstance();
		aoMgr.removeOpenSpectra( spec );
	}

	for( unsigned int i = 0; i < getNumberOfChromatograms(); i++ ) {
		kome::objects::Chromatogram* chrom = getChromatogram( i );
		kome::plugin::PluginCallTool::onCloseChromatogram( *chrom, getCanvas() );
	}

	for( unsigned int i = 0; i < getNumberOfDataMaps(); i++ ) {
		kome::objects::DataMapInfo* dm = getDataMap( i );
		kome::plugin::PluginCallTool::onCloseDataMap( *dm, getCanvas() );
	}

	// delete from the array
	for( int i = (int)m_frames.size() - 1; i >= 0; i-- ) {
		if( m_frames[ i ] == this ) {
			m_frames.erase( m_frames.begin() + i );
		}
	}

	// update main frame menu
	kome::window::MainFrame* mainFrame
		= (kome::window::MainFrame*)kome::window::WindowTool::getMainWindow();
	mainFrame->updateChildWindowMenu( this );

	// call plug-in functions
	WindowTool::setTimer(
		boost::bind( ChildFrame::callActivate, false ),
		UNSET_SPEC_TIME,
		true
	);

	evt.Skip();
}

// on moving
void ChildFrame::onMoving( wxMoveEvent& evt ) {
	// event
	kome::evt::MoveEvent e;
	wxPoint pos = evt.GetPosition();
	e.x( pos.x );
	e.y( pos.y );

	// parameters
	kome::objects::Parameters params;
	kome::plugin::PluginCallTool::setWindow( params, this );
	kome::plugin::PluginCallTool::setMoveEvent( params, e );

	// throw
	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();
	plgMgr.callAllFunctions( &params, CHILD_FRAME_MOVING_PLUGIN_TYPE );
}

// on move
void ChildFrame::onMove( wxMoveEvent& evt ) {
	// event
	kome::evt::MoveEvent e;
	wxPoint pos = evt.GetPosition();
	e.x( pos.x );
	e.y( pos.y );

	// parameters
	kome::objects::Parameters params;
	kome::plugin::PluginCallTool::setWindow( params, this );
	kome::plugin::PluginCallTool::setMoveEvent( params, e );

	// throw
	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();
	plgMgr.callAllFunctions( &params, CHILD_FRAME_MOVE_PLUGIN_TYPE );
}

// on size
void ChildFrame::onSize( wxSizeEvent& evt ) {
	// size
	int fw = m_fixedSize.first;
	int fh = m_fixedSize.second;
	int w = evt.GetSize().GetWidth();
	int h = evt.GetSize().GetHeight();

	if( ( fw < 0 || fw == w ) && ( fh < 0 || fh == h ) ) {    // valid size
		wxMDIChildFrame::OnSize( evt );
	}

	// change size
	if( fw >= 0 ) {
		w = fw;
	}
	if( fh >= 0 ) {
		h = fh;
	}

	SetSize( w, h );
	Refresh( false );
}

// call activate
void ChildFrame::callActivate( const bool activate ) {
	// manager
	kome::objects::ActiveObjectsManager& aoMgr = kome::objects::ActiveObjectsManager::getInstance();

	// call plug-in functions
	kome::objects::Variant obj;
	ChildFrame* frame = getActiveFrame();
	if( frame == NULL ) {
		kome::objects::Sample* sample = aoMgr.getActiveSample();

		if( sample != NULL ) {
			obj.type = kome::objects::Variant::DATA_GROUP_NODE;
			obj.prim.pt = sample->getRootDataGroupNode();
		}
	}
	else {
		obj = frame->getActiveObject();
		if( !activate ) {
			return;
		}
	}

	// call status bar function
	{
		kome::window::StatusBar& statusBar = kome::window::StatusBar::getInstance();
		statusBar.dispAuto();		
	}

	// broadcast
	kome::plugin::PluginCallTool::onActivate( obj );

	// set active sample
	kome::objects::MsDataVariant var( obj );
	aoMgr.setActiveSample( var.getSample() );
}

// get the number of frames
unsigned int ChildFrame::getNumberOfFrames() {
	return m_frames.size();
}

// get child frame
ChildFrame* ChildFrame::getFrame( const unsigned int index ) {
	if( index >= m_frames.size() ) {
		return NULL;
	}
	return m_frames[ index ];
}

// get active frame
ChildFrame* ChildFrame::getActiveFrame() {
	// main frame
	kome::window::MainFrame* mainFrame
		= (kome::window::MainFrame*)kome::window::WindowTool::getMainWindow();
	
	// active frame
	ChildFrame* frame = dynamic_cast< ChildFrame* >( ( mainFrame == NULL ? NULL : mainFrame->GetActiveChild()) );
	for( unsigned int i = 0; i < m_frames.size(); i++ ) {
		if( m_frames[ i ] == frame ) {
			return frame;
		}
	}

	return NULL;
}

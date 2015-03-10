/**
 * @file ClipboardManager.cpp
 * @brief implements of ClipboardManager class
 *
 * @author S.Tanaka
 * @date 2007.05.23
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "ClipboardManager.h"

#include <wx/clipbrd.h>
#include <wx/metafile.h>
#include <map>

using namespace kome::clipboard;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


// constructor
ClipboardManager::ClipboardManager() {
}

// destructor
ClipboardManager::~ClipboardManager() {
}

// copy text data
void ClipboardManager::copyXyDataText( kome::core::XYData& xyData ) {
	// manager
	kome::objects::ActiveObjectsManager& aoMgr = kome::objects::ActiveObjectsManager::getInstance();
	kome::objects::PointsManager& ptMgr = kome::objects::PointsManager::getInstance();

	// text data
	wxString str;

	for( unsigned int i = 0; i < xyData.getLength(); i++ ) {
		str.append( FMT( "%f\t%f%s", xyData.getX( i ), xyData.getY( i ), NEW_LINE ) );
	}

	// text data object	
	wxTextDataObject* text = new wxTextDataObject( str );

	// open clipboard
	if( !wxTheClipboard->Open() ) {
		return;
	}

	// set clipboard
	wxTheClipboard->SetData( text );

	// close
	wxTheClipboard->Close();
}

// copy bitmap
void ClipboardManager::copyBitmap( kome::window::Canvas& canvas ) {
	// create bitmap
	wxSize size = canvas.GetSize();
	wxBitmap bitmap( size.GetWidth(), size.GetHeight() );

	// create DC
	wxMemoryDC memDc;
	memDc.SelectObject( bitmap );
	memDc.SetBackground( *wxWHITE_BRUSH );
	memDc.Clear();

	// draw
	canvas.draw( memDc );

	// open clipboard
	if( !wxTheClipboard->Open() ) {
		return;
	}

	// set clipboard
	wxTheClipboard->SetData( new wxBitmapDataObject( bitmap ) );

	// close
	wxTheClipboard->Close();
}


#ifdef _MSC_VER

// copy metafile
void ClipboardManager::copyMetafile( kome::window::Canvas& canvas ) {
	wxSize size = canvas.GetSize();

	wxBitmap bitmap( size.GetWidth(), size.GetHeight() );

	// create DC
	wxMemoryDC memDc;
	memDc.SelectObject( bitmap );
	memDc.SetBackground( *wxWHITE_BRUSH );
	memDc.Clear();

	// draw
	canvas.draw( memDc );

	// metafile
	wxMetafileDC dc1;

	// draw
	if( !dc1.IsOk() ){
		return;
	}
	
	// select Object 2011.06.02 <Add> M.Izumi
	selectObject( &canvas );
	
	// プロパティ情報を表示 @date 2011.06.02 <Add> M.Izumi
	int x = size.GetWidth()+10; 
	int y = 10;
	for( unsigned int i=0; i < m_properties.size(); i++ ){
		std::string str; 
		str = m_properties[i].first + " : " + m_properties[i].second;
		dc1.DrawText( str, x, y);
		
		y = y+15;
	}
	
	// title
	dc1.DrawText( m_strTitle, 0, 0 );

	dc1.Blit( 0, 25,size.GetWidth(),size.GetHeight(), &memDc, 0,0, wxCOPY );
	dc1.SetFont(*wxSWISS_FONT);
	
	// metafile
	wxMetafile* mf = dc1.Close();
	if( mf != NULL ) {
		mf->SetClipboard( size.GetWidth(), size.GetHeight() );
			
		delete mf;
	}
}

#endif	// _MSC_VER

// get active canvas
kome::window::Canvas* ClipboardManager::getActiveCanvas() {
	// canvas
	kome::window::Canvas* canvas = NULL;

	// active frame
	kome::window::ChildFrame* frame = kome::window::ChildFrame::getActiveFrame();
	if( frame != NULL ) {
		// search children
		wxWindowList& wndList = frame->GetChildren();
		for( unsigned int i = 0; i < wndList.GetCount() && canvas == NULL; i++ ) {			
			canvas = dynamic_cast< kome::window::Canvas* >( wndList.Item( i )->GetData() );			
		}

		//タイトル取得 @date 2011.06.02 <Add> M.Izumi
		m_strTitle = frame->GetTitle();
	}

	// check
	if( canvas == NULL ) {
		LOG_WARN( FMT( "Failed to get the active canvas." ) );
	}

	return canvas;
}

// get instance
ClipboardManager& ClipboardManager::getInstance() {
	// create object (This is the only object.)
	static ClipboardManager mgr;

	return mgr;
}

// オブジェクト選択 (spectrum, chromatogram, group) @date 2011.06.02 <Add> M.Izumi
void ClipboardManager::selectObject( kome::window::Canvas* canvas ){
	// clear
	m_properties.clear();
	
	kome::objects::MsDataVariant obj;
	obj.setVariant( canvas->getActiveObject() );
	
	bool bflg = false;

	// spectrum
	std::vector< kome::objects::Spectrum* > s;
	canvas->getSpecDispCanvas( s );
	if( !s.empty() ){
		for( unsigned int i=0; i < s.size(); i++ ){
			kome::objects::Spectrum* spec = s[i];
			if( spec != NULL ){
				selectSpec( spec );
			}
		}
		bflg = true;
	}else{
		kome::objects::Spectrum* spec = obj.getSpectrum();
		if( spec != NULL ) {
			selectSpec( spec );
		}
		bflg = true;
	}

	// chromatogram
	std::vector< kome::objects::Chromatogram* >c;
	canvas->getChromDispCanvas( c );
	if( !c.empty() ){
		for( unsigned int i=0; i < c.size(); i++ ){
			kome::objects::Chromatogram* chrom = c[i];
			if( chrom != NULL ){
				selectChrom( chrom );
			}
		}
		bflg = true;
	}else{
		kome::objects::Chromatogram* chrom = obj.getChromatogram();
		if( chrom != NULL ){
			selectChrom( chrom );
		}
		bflg = true;
	}

	// group
	if( !bflg ){
		kome::objects::DataGroupNode* group = obj.getGroup();
		if( group != NULL ){
			selectGroup( group );
		}
	}

}

// 各オブジェクトのプロパティ情報取得 ->
// select spectrum
void ClipboardManager::selectSpec( kome::objects::Spectrum* spectrum ){
	// properties
	kome::core::Properties props;
	spectrum->getProperties( props );
	
	// user properties
	kome::core::Properties userProps;
	spectrum->getUserProperties( userProps );

	setProperties( props, userProps );
}

// select chromatogram
void ClipboardManager::selectChrom( kome::objects::Chromatogram* chrom ){
	// properties
	kome::core::Properties props;
	chrom->getProperties( props );
	// user properties
	kome::core::Properties userProps;
	chrom->getUserProperties( userProps );

	setProperties( props, userProps );
}

// select group
void ClipboardManager::selectGroup( kome::objects::DataGroupNode* group ){
	// properties
	kome::core::Properties props;
	// user properties
	kome::core::Properties userProps;
	while( group != NULL ) {
		// add properties
		for( unsigned int i = 0; i < group->getProperties().getNumberOfProperties(); i++ ) {
			props.setValue(
				group->getProperties().getKey( i ),
				group->getProperties().getValue( i )
			);
		}
		// add user properties
		for( unsigned int i = 0; i < group->getUserProperties().getNumberOfProperties(); i++ ) {
			userProps.setValue(
				group->getUserProperties().getKey( i ),
				group->getUserProperties().getValue( i )
			);
		}

		// parent group
		group = group->getParentGroup();
	}
	setProperties( props, userProps );
}
// <-

// set Properties 
void ClipboardManager::setProperties( kome::core::Properties& props, kome::core::Properties& userProps ) {
	
	// get properties
	m_properties.reserve( props.getNumberOfProperties() + userProps.getNumberOfProperties() );
	for( unsigned int i = 0; i < props.getNumberOfProperties(); i++ ) {
		m_properties.push_back( std::make_pair( props.getKey( i ), props.getValue( i ) ) );
	}
	for( unsigned int j = 0; j < userProps.getNumberOfProperties(); j++ ) {
		char sModifiedKey[200];
		_snprintf(sModifiedKey, 200, "*%s", userProps.getKey( j ));

		m_properties.push_back( std::make_pair( sModifiedKey, userProps.getValue( j ) ) );
	}
}

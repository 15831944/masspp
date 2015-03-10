/**
 * @file PeakListCtrl.cpp
 * @brief implements of PeakListCtrl class
 *
 * @author S.Tanaka
 * @date 2008.09.11
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "PeakListCtrl.h"   
#include "PeakSearch.h"


using namespace kome::search;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



BEGIN_EVENT_TABLE( PeakListCtrl, kome::window::GridListCtrl )
	EVT_LEFT_DCLICK( PeakListCtrl::onDblClick )
END_EVENT_TABLE()


// constructor
PeakListCtrl::PeakListCtrl( wxWindow* parent, const int width, const int height )
		: kome::window::GridListCtrl( parent, -1, width, height, false ) {

	// Context Menu
	setPopupMenuFlag( POPUP_SELECT | POPUP_DESELECT | POPUP_COPY | POPUP_FILTER | POPUP_SAVE );
}

// destructor
PeakListCtrl::~PeakListCtrl() {
}

// on create
void PeakListCtrl::onCreate() { 
	// manager
	PeakSearch& s = PeakSearch::getInstance();

	// add columns
	addColumn( "RT", kome::window::GridListCtrl::TYPE_UDOUBLE );
	addColumn( "Precursor", kome::window::GridListCtrl::TYPE_UDOUBLE );
	addColumn( "m/z", kome::window::GridListCtrl::TYPE_UDOUBLE );
	addColumn( "Intensity", kome::window::GridListCtrl::TYPE_UDOUBLE );

	// rows
	for( unsigned int i = 0; i < s.getNumberOfPeaks(); i++ ) {
		addData( i, false );
	}
}

// get double value
double PeakListCtrl::getDouble( const long long data, const int column ) const {
	// manager
	PeakSearch& s = PeakSearch::getInstance();

	// get value
	double v = -1.0;
	if( column == 0 ) {	// RT
		v = s.getPeakRt( data );
	}
	else if( column == 1 ) {    // Precursor
		kome::objects::Spectrum* spec = s.getPeakSpectrum( data );
		if( spec != NULL ) {
			v = spec->getPrecursor();
		}
	}
	else if( column == 2 ) {	// m/z
		v = s.getPeakMz( data );
	}
	else if( column == 3 ) {	// inntensity
		v = s.getPeakIntensity( data );
	}

	return v;
}

// on double-click
void PeakListCtrl::onDblClick( wxMouseEvent& evt ) {
	// search object
	PeakSearch& s = PeakSearch::getInstance();

	// get selected items
	std::vector< int > items;
	kome::window::ListTool::getSelectedItems( this, items );
	if( items.size() == 0 ) {
		return;
	}

	int item = items[ 0 ];
	if( item < 0 || item >= GetItemCount() ) {
		return;
	}

	// canvas
	kome::window::ChildFrame* frame = kome::window::ChildFrame::getActiveFrame();
	wxWindow* canvas = ( frame == NULL ? NULL : frame->getCanvas() );

	// spectrum
	int idx = m_data[ item ];
	kome::objects::Spectrum* spec = s.getPeakSpectrum( idx );
	if( spec == NULL ) {
		return;
	}

	double mz = s.getPeakMz( idx );

	double startMz = std::max( 0.0, mz - s.getSpecViewRange() / 2.0 );
	double endMz = mz + s.getSpecViewRange() / 2.0;

	// No.79884 Peak Searchèàóùå„ÇÃSpectrumâÊëúÇ™èâä˙ï\é¶Ç∆ÇªÇÃå„Ç≈ägëÂó¶Ç™àŸÇ»ÇÈïsãÔçáèCê≥
	kome::plugin::PluginCallTool::openSpectrum( *spec );	// @date 2011.02.15 <Add> M.Izumi
	kome::plugin::PluginCallTool::changeSpectrumRange( *spec, startMz, endMz, (long long)canvas );

	kome::window::WindowTool::refresh();
}

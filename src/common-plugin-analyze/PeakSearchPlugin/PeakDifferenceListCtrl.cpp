/**
 * @file PeakDifferenceListCtrl.cpp
 * @brief implements of PeakDifferenceListCtrl class
 *
 * @author S.Tanaka
 * @date 2008.10.17
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "PeakDifferenceListCtrl.h"      
#include "PeakSearch.h"
#include "PeakDifferenceSearch.h"


using namespace kome::search;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



BEGIN_EVENT_TABLE( PeakDifferenceListCtrl, kome::window::GridListCtrl )
	EVT_LEFT_DCLICK( PeakDifferenceListCtrl::onDblClick )
END_EVENT_TABLE()


// constructor
PeakDifferenceListCtrl::PeakDifferenceListCtrl( wxWindow* parent, const int width, const int height )
		: kome::window::GridListCtrl( parent, -1, width, height, false ) {
}

// destructor
PeakDifferenceListCtrl::~PeakDifferenceListCtrl() {
}

// on create
void PeakDifferenceListCtrl::onCreate() { 
	// search object
	PeakDifferenceSearch& s = PeakDifferenceSearch::getInstance();

	// add columns
	addColumn( "Scan Number", kome::window::GridListCtrl::TYPE_UINT );
	addColumn( "RT", kome::window::GridListCtrl::TYPE_UDOUBLE );
	addColumn( "Precursor", kome::window::GridListCtrl::TYPE_UDOUBLE );
	addColumn( "m/z (Low)", kome::window::GridListCtrl::TYPE_UDOUBLE );
	addColumn( "Intensity (Low)", kome::window::GridListCtrl::TYPE_UDOUBLE );
	addColumn( "m/z (High)", kome::window::GridListCtrl::TYPE_UDOUBLE );
	addColumn( "Intensity (High)", kome::window::GridListCtrl::TYPE_UDOUBLE );
	addColumn( "Difference", kome::window::GridListCtrl::TYPE_UDOUBLE );
	addColumn( "Ratio", kome::window::GridListCtrl::TYPE_UDOUBLE );

	// rows
	for( unsigned int i = 0; i < s.getNumberOfPeaks(); i++ ) {
		addData( i, false );
	}
}

// get int value
int PeakDifferenceListCtrl::getInt( const long long data, const int column ) const {
	// search object
	PeakDifferenceSearch& s = PeakDifferenceSearch::getInstance();

	// get value
	int v = -1;
	if( column == 0 ) {	// Scan Number
		kome::objects::Spectrum* spec = s.getPeakSpectrum( data );
		if( spec != NULL ) {
			v = spec->getScanNumber();
		}
	}

	return v;
}

// get double value
double PeakDifferenceListCtrl::getDouble( const long long data, const int column ) const {
	// search object
	PeakDifferenceSearch& s = PeakDifferenceSearch::getInstance();

	// get value
	double v = -1.0;
	if( column == 1 ) {	// RT
		v = s.getPeakRt( data );
	}
	else if( column == 2 ) {    // precursor
		kome::objects::Spectrum* spec = s.getPeakSpectrum( data );
		if( spec != NULL ) {
			v = spec->getPrecursor();
		}
	}
	else if( column == 3 ) {	// m/z (Low)
		v = s.getLowPeakMz( data );
	}
	else if( column == 4 ) {	// intensity (Low)
		v = s.getLowPeakIntensity( data );
	}
	else if( column == 5 ) {	// m/z (High)
		v = s.getHighPeakMz( data );
	}
	else if( column == 6 ) {	// intensity (High)
		v = s.getHighPeakIntensity( data );
	}
	else if( column == 7 ) {	// difference
		v = s.getPeakDiff( data );
	}
	else if( column == 8 ) {   // ratio
		double lv = s.getLowPeakIntensity( data );
		double hv = s.getHighPeakIntensity( data );
		if( hv < 0.0 || lv <= 0.0 ) {
			v = -1.0;
		}
		else {
			v = hv / lv;
		}
	}

	return v;
}

// on double-click
void PeakDifferenceListCtrl::onDblClick( wxMouseEvent& evt ) {
	// search object
	PeakSearch& ps = PeakSearch::getInstance();
	PeakDifferenceSearch&  ds = PeakDifferenceSearch::getInstance();

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
	kome::objects::Spectrum* spec = ds.getPeakSpectrum( idx );
	if( spec == NULL ) {
		return;
	}

	double startMz = std::max( 0.0, ds.getLowPeakMz( idx ) - ps.getSpecViewRange() / 2.0 );
	double endMz = ds.getHighPeakMz( idx ) + ps.getSpecViewRange() / 2.0;

	kome::plugin::PluginCallTool::openSpectrum( *spec );
	kome::plugin::PluginCallTool::changeSpectrumRange( *spec, startMz, endMz, (long long)canvas );

	kome::window::WindowTool::refresh();
}

/**
 * @file PeaksListCtrl.cpp
 * @brief implements of PeaksListCtrl class
 *
 * @author S.Tanaka
 * @date 2007.10.23
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "PeaksListCtrl.h"   
#include "Peaks2dManager.h"


using namespace kome::labeling;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



BEGIN_EVENT_TABLE( PeaksListCtrl, kome::window::GridListCtrl )
	EVT_LIST_ITEM_SELECTED( wxID_ANY, PeaksListCtrl::onSelectItem )
	EVT_LIST_ITEM_DESELECTED( wxID_ANY, PeaksListCtrl::onSelectItem )
	EVT_LIST_ITEM_FOCUSED( wxID_ANY, PeaksListCtrl::onSelectItem )
END_EVENT_TABLE()


// constructor
PeaksListCtrl::PeaksListCtrl( wxWindow* parent ) 
		: kome::window::GridListCtrl( parent, wxID_ANY, 200, 300, false ) {
	// initialize
	m_group = NULL;

	// create list
	create();
}

// destructor
PeaksListCtrl::~PeaksListCtrl() {
}

// set group
void PeaksListCtrl::setGroup( kome::objects::DataGroupNode* group ) {
	// clear
	SetItemCount( 0 );
	Peaks2dManager& mgr = Peaks2dManager::getInstance();
	mgr.clearSelectedPeaks();

	// set
	m_group = group;
	if( group == NULL ) {
		Refresh();
		return;
	}

	// initialize
	deselectAll();
	clearData( false );

	// set peaks
	kome::objects::PeaksManager& pkMgr = kome::objects::PeaksManager::getInstance();
	kome::objects::Peaks2D* peaks = pkMgr.getPeaks( group );
	if( peaks != NULL && peaks->getNumberOfPeaks() > 0 ) {
		for( unsigned int i = 0; i < peaks->getNumberOfPeaks(); i++ ) {
			addData( (long long)peaks->getPeak( i ), false );
		}
	}

	// update
	updateList();
}

// create list
void PeaksListCtrl::create() {
	// insert columns
	addColumn( "RT", TYPE_UDOUBLE );
	addColumn( "m/z", TYPE_UDOUBLE );
	addColumn( "Intensity", TYPE_UDOUBLE );
	addColumn( "RT (Start)", TYPE_UDOUBLE );
	addColumn( "RT (End)", TYPE_UDOUBLE );
	addColumn( "m/z (Start)", TYPE_UDOUBLE );
	addColumn( "m/z (End)", TYPE_UDOUBLE );
	addColumn( "Cluster RT", TYPE_UDOUBLE );
	addColumn( "Cluster m/z", TYPE_UDOUBLE );
	addColumn( "Cluster Intensity", TYPE_UDOUBLE );
	addColumn( "Cluster RT (Start)", TYPE_UDOUBLE );
	addColumn( "Cluster RT (End)", TYPE_UDOUBLE );
	addColumn( "Cluster m/z (Start)", TYPE_UDOUBLE );
	addColumn( "Cluster m/z (End)", TYPE_UDOUBLE );
}

// get peak value
double PeaksListCtrl::getDouble( long long data, int col ) const {
	// peak
	kome::objects::Peak2DElement* peak = (kome::objects::Peak2DElement*)data; 

	// check
	if( peak == NULL ) {
		return -1.0;
	}

	// value
	double val = -1.0;
	if( col < 7 ) {		// peak value
		if( col == 0 ) { // RT
			val = peak->getRt();
		}
		else if( col == 1 ) {	// m/z
			val = peak->getMz();
		}
		else if( col == 2 ) {	// intensity
			val = peak->getIntensity();
		}
		else if( col == 3 ) {	// start RT
			val = peak->getStartRt();
		}
		else if( col == 4 ) {	// end RT
			val = peak->getEndRt();
		}
		else if( col == 5 ) {	// start m/z
			val = peak->getStartMz();
		}
		else if( col == 6 ) {	// end m/z
			val = peak->getEndMz();
		}
	}
	else {  // cluster value
		kome::objects::PeaksCluster2D* cluster = peak->getCluster();
		if( cluster != NULL ) {
			if( col == 7 ) { // RT
				val = cluster->getRt();
			}
			else if( col == 8 ) {	// m/z
				val = cluster->getMz();
			}
			else if( col == 9 ) {	// intensity
				val = cluster->getIntensity();
			}
			else if( col == 10 ) {	// start RT
				val = cluster->getStartRt();
			}
			else if( col == 11 ) {	// end RT
				val = cluster->getEndRt();
			}
			else if( col == 12 ) {	// start m/z
				val = cluster->getStartMz();
			}
			else if( col == 13 ) {	// end m/z
				val = cluster->getEndMz();
			}
		}
	}

	return val;
}

// on select item
void PeaksListCtrl::onSelectItem( wxListEvent& evt ) {
	// clear
	Peaks2dManager& mgr = Peaks2dManager::getInstance();
	mgr.clearSelectedPeaks();

	// add selected peaks
	for( int i = 0; i < GetItemCount(); i++ ) {
		if( GetItemState( i, wxLIST_STATE_SELECTED ) > 0 ) {
			mgr.addSelectedPeaks( (kome::objects::Peak2DElement*)getListData( i ) );
		}
	}

	// refresh
	kome::window::WindowTool::refresh();
}

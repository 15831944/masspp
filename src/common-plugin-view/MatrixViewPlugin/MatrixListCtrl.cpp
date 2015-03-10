/**
 * @file MatrixListCtrl.cpp
 * @brief implements of MatrixListCtrl class
 *
 * @author S.Tanaka
 * @date 2008.02.26
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "MatrixListCtrl.h"
#include "QuantitationDialog.h"
#include "MatrixViewManager.h"		// @date 2013/08/14 <Add> OKADA

#include <float.h>

using namespace kome::matrix::view;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



#define ID_DEL_RESTORE					 998
#define ID_DEL							 999

#define BORDER_SIZE						 3


BEGIN_EVENT_TABLE( MatrixListCtrl, kome::window::GridListCtrl )
	EVT_LIST_ITEM_SELECTED( wxID_ANY, MatrixListCtrl::onSelectItem )
	EVT_LIST_ITEM_DESELECTED( wxID_ANY, MatrixListCtrl::onDselectItem )
END_EVENT_TABLE()


// constructor
MatrixListCtrl::MatrixListCtrl(
		wxWindow* parent,
		const unsigned int width,
		const unsigned int height,
		kome::window::DialogProgress* progress
) : kome::window::GridListCtrl( parent, wxID_ANY, width, height, false ) {
    // initialize	
	m_rtCol = 0;
	m_mzCol = 1;
	m_chargeCol = -1;
	m_substanceCol = -1;
	m_appendedCol = 2;

	// list attr
	m_focusedAttr.SetFont( wxFont( 10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD ) );

	// Context Menu
	setActivateName( "Open" );
	setPopupMenuFlag( POPUP_COPY | POPUP_FILTER | POPUP_SAVE );
	// @date 2014.07.08 <Add> M.Izumi ->
	if( progress != NULL ){				
		m_pProgress = progress;
	}else{
		m_pProgress = NULL;
	}
	// @date 2014.07.08 <Add> M.Izumi <-
}

// destructor
MatrixListCtrl::~MatrixListCtrl() {
}

// on create
void MatrixListCtrl::onCreate() {
	// manager
	kome::ident::IdentificationManager& identMgr = kome::ident::IdentificationManager::getInstance();
	kome::matrix::view::MatrixViewManager& mgr = MatrixViewManager::getInstance();	// @date 2013/08/14 <Add> OKADA

	// clear substance map
	m_substanceMap.clear();

	// selected project
	int project = identMgr.getSelectedMatrix();

	// header
	bool rtFlg = false;
	bool mzFlg = false;
	bool chargeFlg = false;

	m_rtCol = -1;
	m_mzCol = -1;
	m_chargeCol = -1;
	m_appendedCol = 0;

	identMgr.getMatrixFlags( project, &rtFlg, &mzFlg, &chargeFlg );
	if( rtFlg ) {
		mgr.addColumn( "RT", TYPE_UDOUBLE, this );	// @date 2013/08/13 <Add> OKADA
		m_rtCol = m_appendedCol;
		m_appendedCol += 1;
	}
	if( mzFlg ) {
		mgr.addColumn( "m/z", TYPE_UDOUBLE, this );	// @date 2013/08/13 <Add> OKADA
		m_mzCol = m_appendedCol;
		m_appendedCol += 1;
	}
	if( chargeFlg ) {
		mgr.addColumn( "Charge", TYPE_UINT, this );	// @date 2013/08/13 <Add> OKADA
		m_chargeCol = m_appendedCol;
		m_appendedCol += 1;
	}

	std::vector< int > searches;
	identMgr.getSearchesFromMatrix( searches, project );
	int searchId = -1;
	for( unsigned int i = 0; i < searches.size(); i++ ) {
		searchId = std::max( searchId, searches[ i ] );
	}
	if( searchId >= 0 ) {
		mgr.addColumn( "Substance", TYPE_STRING, this, 160 );	// @date 2013/08/13 <Add> OKADA
		m_substanceCol = m_appendedCol;
		m_appendedCol += 1;
	}

	// appended columns
	m_appendedCols.clear();
	if( project >= 0 ) {
		identMgr.getMatrixColumns( m_appendedCols, project );
	}

	for( unsigned int i = 0; i < m_appendedCols.size(); i++ ) {
		std::string name = identMgr.getMatrixColumnName( m_appendedCols[ i ] );
		kome::ident::ColumnDataType type = identMgr.getMatrixColumnType( m_appendedCols[ i ] );

		ColumnDataType t = TYPE_STRING;
		if( type == kome::ident::TYPE_INT ) {
				t = TYPE_INT;
		}
		else if( type == kome::ident::TYPE_UINT ) {
			t = TYPE_UINT;
		}
		else if( type == kome::ident::TYPE_DOUBLE ) {
			t = TYPE_DOUBLE;
		}
		else if( type == kome::ident::TYPE_UDOUBLE ) {
			t = TYPE_UDOUBLE;
		}

		mgr.addColumn( name.c_str(), t, this );	// @date 2013/08/13 <Add> OKADA
	}

	// groups
	m_samples.clear();

	std::vector< int > groups;
	identMgr.getGroups( groups, project );
	for( unsigned int i = 0; i < groups.size(); i++ ) {
		identMgr.getMatrixSamplesFromGroup( m_samples, groups[ i ] );
	}

	for( unsigned int i = 0; i < m_samples.size(); i++ ) {
		std::string name = identMgr.getMatrixSampleName( m_samples[ i ] );
		mgr.addColumn( name.c_str(), TYPE_UDOUBLE, this );	// @date 2013/08/13 <Add> OKADA
	}

	// add items
	std::vector< int > peaks;
	identMgr.getMatrixPeaks( peaks, project );

	// add progress 
	if( m_pProgress != NULL ){
		m_pProgress->setRange( 0, peaks.size() ); // @date 2014.07.08 <Add> M.Izumi
	}
	for( unsigned int i = 0; i < peaks.size(); i++ ) { // @date 2014.07.08 <Add> M.Izumi
		if( m_pProgress != NULL ){
			m_pProgress->setStatus( FMT("peak [%d/%d]", i+1, peaks.size()).c_str(), true );
		}
		
		// add peak
		const int peakId = peaks[ i ];
		mgr.addData( peakId, false, this );	// @date 2013/08/14 <Add> OKADA

		// hit spectra
		searches.clear();
		double score = -FLT_MAX;
		identMgr.getMatrixPeakSpectraFromSearchAndPeak( searches, searchId, peakId );
		for( unsigned int j = 0; j < searches.size(); j++ ) {
			const int specId = identMgr.getMatrixPeakSpectrumSpectrum( searches[ j ] );
			std::vector< int > hits;
			int substance = -1;
			identMgr.getHitsFromSpectrum( hits, specId );
			for( unsigned int k = 0; k < hits.size(); k++ ) {
				const int hitId = hits[ k ];
				kome::ident::HitInfo hit;
				identMgr.getHit( hitId, hit );
				bool loop = true;
				for( unsigned int l = 0; l < hit.props.size() && loop; l++ ) {
					if( hit.props.at( l ).type == kome::ident::TYPE_SCORE ) {
						double val = double();
						if( isdouble( hit.props.at( l ).value.c_str(), & val ) ) {
							if( val > score ) {
								substance = hit.substance;
								loop = false;
							}
						}
					}
				}
				if( substance < 0 ) {
					substance = hit.substance;
				}
			}

			kome::ident::SubstanceInfo subInfo;
			subInfo.parentId = substance;
			while( subInfo.parentId >= 0 ) {
				identMgr.getSubstance( subInfo.parentId, subInfo );
			}
			if( !subInfo.accession.empty() || !subInfo.name.empty() ) {
				std::string subName = subInfo.accession;
				if( !subInfo.name.empty() ) {
					if( !subName.empty() ) {
						subName.append( " : " );
					}
					subName.append( subInfo.name );
				}

				m_substanceMap[ peakId ] =subName;
			}
		}
		//add progress @date 2014.07.08 <Add> M.Izumi
		if( m_pProgress != NULL ){
			m_pProgress->setPosition( i, true );
		}
	}

	// add progress @date 2014.07.08 <Add> M.Izumi
	if( m_pProgress != NULL ){
		m_pProgress->fill();
		delete m_pProgress ;
	}
	m_pProgress = NULL;

}

// get int value
int MatrixListCtrl::getInt( const long long data, const int column ) const {
	// manager
	kome::ident::IdentificationManager& identMgr = kome::ident::IdentificationManager::getInstance();

	// sample begin column
	const int sampleBeginCol = m_appendedCol + (int)m_appendedCols.size();

	// get value
	int val = int();

	if( column == m_chargeCol ) {	// charge
		int charge = identMgr.getMatrixPeakCharge( data );
		val = charge;
	}
	else if( column >= m_appendedCol && column < sampleBeginCol ) {	// appended value
		int idx = column - m_appendedCol;
		std::string s = identMgr.getAppendedValue( data, m_appendedCols[ idx ] );
		if( !isint( s.c_str(), 10, &val ) ) {
			val = INT_MIN;
		}
	}

	return val;
}

// get double value
double MatrixListCtrl::getDouble( const long long data, const int column ) const {
	// manager
	kome::ident::IdentificationManager& identMgr = kome::ident::IdentificationManager::getInstance();

	// sample begin column
	const int sampleBeginCol = m_appendedCol + (int)m_appendedCols.size();

	// get value
	double val = double();

	if( column == m_rtCol ) {	// RT
		double rt = identMgr.getMatrixPeakRt( data );
		val = rt;
	}
	else if( column == m_mzCol ) {	// m/z
		double mz = identMgr.getMatrixPeakMz( data );
		val = mz;
	}
	else if( column >= m_appendedCol && column < sampleBeginCol ) {	// appended value
		int idx = column - m_appendedCol;
		std::string s = identMgr.getAppendedValue( data, m_appendedCols[ idx ] );
		if( !isdouble( s.c_str(), &val ) ) {
			val = - FLT_MAX;
		}
	}
	else if( column >= sampleBeginCol ) {	// intensity
		// group ID
		int id = -1;
		int idx = column - sampleBeginCol;
		if( idx < (int)m_samples.size() ) {
			id = m_samples[ idx ];
		}

		if( id >= 0 ) {
			val = identMgr.getNormalizedPeakValue( data, id );
		}
	}

	return val;
}

// get string value
std::string MatrixListCtrl::getString( const long long data, const int column ) const {
	// string
	std::string s;

	// manager
	kome::ident::IdentificationManager& identMgr = kome::ident::IdentificationManager::getInstance();

	// index
	int idx = column - m_appendedCol;

	if( column == m_substanceCol ) {
		if( m_substanceMap.find( data ) != m_substanceMap.end() ) {
			s = m_substanceMap.at( data );
		}
	}
	else if( idx >= 0 && idx < (int)m_appendedCols.size() ) {
		s = identMgr.getAppendedValue( data, m_appendedCols[ idx ] );
	}

	return s;
}

// create menu
wxMenu* MatrixListCtrl::createMenu() {
	// default
	wxMenu* menu = kome::window::GridListCtrl::createMenu();
	return menu;
}

// on activate
void MatrixListCtrl::onActivateData( const long long data ) {
	
	// parent
	QuantitationDialog* dlg = dynamic_cast< QuantitationDialog* >( GetParent() );
	if( dlg == NULL ) {
		return;
	}

	// peak ID
	int peakId = data;

	// open
	dlg->openPeak( peakId );
}



// @date 2011.11.07 <Add> M.Izumi ->
// リストの選択、選択解除時の処理
void MatrixListCtrl::onSelectItem( wxListEvent& evt ){	

	// parent
	QuantitationDialog* dlg = dynamic_cast< QuantitationDialog* >( GetParent() );
	if( dlg == NULL ) {
		return;
	}

	// selected item
	std::vector< int > items;
	kome::window::ListTool::getSelectedItems( this, items );

	int peakId = -1;
	if( items.size() == 1 ) {
		peakId = (int)getData( items[ 0 ] );
	}

	dlg->setPeakId( peakId );

}

void MatrixListCtrl::onDselectItem( wxListEvent& evt ){
	// parent
	QuantitationDialog* dlg = dynamic_cast< QuantitationDialog* >( GetParent() );
	if( dlg == NULL ) {
		return;
	}

	// selected item
	std::vector< int > items;
	kome::window::ListTool::getSelectedItems( this, items );

	int peakId = -1;
	dlg->setPeakId( peakId );
}
// @date 2011.11.07 <Add> M.Izumi <-

// gets list item attribute
wxListItemAttr* MatrixListCtrl::OnGetItemAttr( long item ) const {
	// manager
	kome::ident::IdentificationManager& identMgr = kome::ident::IdentificationManager::getInstance();

	// data
	long long data = -1;
	if( item < (int)m_data.size() ) {
		data = m_data[ item ];
	}

	// default
	wxListItemAttr* attr = GridListCtrl::OnGetItemAttr( item );

	// focused
	if( identMgr.isFocusedPeak( data ) ) {
		attr = const_cast< wxListItemAttr* >( &m_focusedAttr );
	}
	else {
		attr = const_cast< wxListItemAttr* >( &m_normalAttr );
	}

	return attr;
}

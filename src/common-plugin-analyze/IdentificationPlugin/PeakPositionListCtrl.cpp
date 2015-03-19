/**
 * @file PeakPositionListCtrl.cpp
 * @brief implemnets of PeakPositionListCtrl class
 *
 * @author S.Tanaka
 * @date 2008.10.14
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "PeakPositionListCtrl.h"


using namespace kome::ident;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



BEGIN_EVENT_TABLE( PeakPositionListCtrl, kome::window::CheckGridListCtrl )
END_EVENT_TABLE()


// constructor
PeakPositionListCtrl::PeakPositionListCtrl( wxWindow* parent, const int width, const int height )
		: kome::window::CheckGridListCtrl( parent, wxID_ANY, width, height, false ) {
	m_rtCol = 0;
	m_mzCol = 1;
	m_chargeCol = -1;
	m_appendedCol = 2;
}

// destructor
PeakPositionListCtrl::~PeakPositionListCtrl() {
}

// on create
void PeakPositionListCtrl::onCreate() {

	// manager
	kome::ident::IdentificationManager& identMgr = kome::ident::IdentificationManager::getInstance();

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
		addColumn( "RT", TYPE_UDOUBLE );
		m_rtCol = m_appendedCol;
		m_appendedCol += 1;
	}
	if( mzFlg ) {
		addColumn( "m/z", TYPE_UDOUBLE );
		m_mzCol = m_appendedCol;
		m_appendedCol += 1;
	}
	if( chargeFlg ) {
		addColumn( "Charge", TYPE_UINT );
		m_chargeCol = m_appendedCol;
		m_appendedCol += 1;
	}

	// appended columns
	m_appendedCols.clear();
	if( project >= 0 ) {
		identMgr.getMatrixColumns( m_appendedCols, project );
	}

	for( unsigned int i = 0; i < m_appendedCols.size(); i++ ) {
		std::string name;
		kome::ident::ColumnDataType type = identMgr.getMatrixColumnType( m_appendedCols[ i ]  );
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

		name = identMgr.getMatrixColumnName( m_appendedCols[ i ] );

		addColumn( name.c_str(), t );
	}

	// peaks
	clearData( false );

	std::vector< int > peaks;
	identMgr.getMatrixPeaks( peaks, project );
	for( unsigned int i = 0; i < peaks.size(); i++ ) {
		addData( peaks[ i ], false );
	}
}

// get double value
double PeakPositionListCtrl::getDouble( const long long data, const int column ) const {
	// manager
	kome::ident::IdentificationManager& identMgr = kome::ident::IdentificationManager::getInstance();

	// sample begin column
	const int sampleBeginCol = m_appendedCol + (int)m_appendedCols.size();

	// get value
	double val = double();

	if( column == m_rtCol ) {	// RT
		val = identMgr.getMatrixPeakRt( data );
	}
	else if( column == m_mzCol ) {	// m/z
		val = identMgr.getMatrixPeakMz( data );
	}
	else if( column >= m_appendedCol && column < sampleBeginCol ) {	// appended value
		int idx = column - m_appendedCol;
		std::string s = identMgr.getAppendedValue( data, m_appendedCols[ idx ] );
		if( !isdouble( s.c_str(), &val ) ) {
			val = - FLT_MAX;
		}
	}

	return val;
}

// get int value
int PeakPositionListCtrl::getInt( const long long data, const int column ) const {
	// manager
	kome::ident::IdentificationManager& identMgr = kome::ident::IdentificationManager::getInstance();

	// sample begin column
	const int sampleBeginCol = m_appendedCol + (int)m_appendedCols.size();

	// get value
	int val = int();

	if( column == m_chargeCol ) {	// charge
		val = identMgr.getMatrixPeakCharge( data );
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

// get string value
std::string PeakPositionListCtrl::getString( const long long data, const int column ) const {
	// string
	std::string s;

	// manager
	kome::ident::IdentificationManager& identMgr = kome::ident::IdentificationManager::getInstance();

	// index
	int idx = column - m_appendedCol;

	if( idx >= 0 && idx < (int)m_appendedCols.size() ) {
		s = identMgr.getAppendedValue( data, m_appendedCols[ idx ] );
	}

	return s;
}

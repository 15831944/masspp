/**
 * @file SpectraCheckList.cpp
 * @brief interfaces of Spectra CheckListCtrl class
 *
 * @author M.Izumi
 * @date 2013.01.08
 * 
 * Copyright(C) 2013 Shimadzu Corporation. All rights reserved.
 */

#include "stdafx.h"
#include "SpectraCheckList.h"

using namespace kome::mzfilter;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



BEGIN_EVENT_TABLE( SpectraCheckList, kome::window::CheckGridListCtrl )
END_EVENT_TABLE()

// constructor
SpectraCheckList::SpectraCheckList( wxWindow* parent, kome::objects::Spectrum* spec )
	: kome::window::CheckGridListCtrl( parent, wxID_ANY, 400, 300, false ), m_spec(spec){

	m_sample = NULL;
	m_listValInfo.clear();
	m_index = -1;
}

// destructor
SpectraCheckList::~SpectraCheckList(){
}

// on create
void SpectraCheckList::onCreate(){

	// columns
	addColumn( "Spectrum", kome::window::GridListCtrl::TYPE_STRING );
	addColumn( "RT", kome::window::GridListCtrl::TYPE_STRING );
	addColumn( "Stage",  kome::window::GridListCtrl::TYPE_STRING );
	addColumn( "Precursor", kome::window::GridListCtrl::TYPE_STRING );

	// manager
	kome::objects::ActiveObjectsManager& aoMgr = kome::objects::ActiveObjectsManager::getInstance();
	m_sample = aoMgr.getActiveSample();

	if( m_sample == NULL ){
		return;
	}
		
	kome::objects::DataSet s( m_sample->getRootDataGroupNode() );
	kome::objects::DataGroupNode* group = m_sample->getRootDataGroupNode();
	if( group == NULL ){
		return;
	}
	group->getDataSet( &s );
	
	// wait cursor
	wxBeginBusyCursor();
		
	// all spectra
	for( unsigned int i = 0; i < s.getNumberOfSpectra(); i++ ) {
		kome::objects::Spectrum* spec = s.getSpectrum( i );
		
		if( spec != NULL ){
			// set row data
			DataInfo tmp;
			tmp.strNo = FMT( "%s", spec->getName() );
			tmp.strRt = FMT( "%f", spec->getRt() );
			tmp.strStage = FMT( "%d", spec->getMsStage() );
			tmp.strPrecursor = FMT( "%f", spec->getPrecursor() );
			tmp.spec = spec;
			
			if( m_spec == spec ){
				m_index = i;
			}

			m_listValInfo.push_back( tmp );

			addData( i, false );
		}
	}
	// restore cursor
	wxEndBusyCursor();
}

// get data
std::string  SpectraCheckList::getString( const long long data, const int column ) const {
	std::string str;

	if( data >=0 && data < m_listValInfo.size() ){
		switch( column ){
		case 0:
			str = m_listValInfo[data].strNo;
			break;
		case 1:
			str = m_listValInfo[data].strRt;
			break;
		case 2:
			str = m_listValInfo[data].strStage;
			break;
		case 3:
			str = m_listValInfo[data].strPrecursor;
			break;
		default:
			break;
		}
	}

	return str;
}

// get select spectrum
kome::objects::Spectrum* SpectraCheckList::getSelSpectrum( int data ){
	kome::objects::Spectrum* spec = NULL;
	
	if( data >=0 && data < (int)m_listValInfo.size() ){
		spec = m_listValInfo[data].spec;
	}

	return spec;
}

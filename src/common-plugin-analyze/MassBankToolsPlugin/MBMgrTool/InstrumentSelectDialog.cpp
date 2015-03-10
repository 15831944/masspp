/**
 * @file InstrumentSelectDialog.cpp
 * @brief interfaces of MassBank Instrument Selecet Dialog
 *
 * @author M.Fukuda
 * @date 2011.08.30
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#include "../stdafx.h"

#include "InstrumentSelectDialog.h"
#include "InstrumentTypesMgr.h"
#include "../MassBankManager.h"

using namespace kome::massbank;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


// EVENT TABLE
BEGIN_EVENT_TABLE( InstrumentSelectDialog, kome::window::StandardDialog )
END_EVENT_TABLE()

BEGIN_EVENT_TABLE( InstrumentCheckList, CheckGridListCtrl )
END_EVENT_TABLE()


// constractor
InstrumentSelectDialog::InstrumentSelectDialog( wxWindow* parent ) 
	: kome::window::StandardDialog(parent, "Instrument Selector") 
{
	m_instMgr = MassBankManager::getInstance().getInstMgr();
}

// destractor
InstrumentSelectDialog::~InstrumentSelectDialog(){}

// create Main Sizer
wxSizer* InstrumentSelectDialog::createMainSizer()
{
	// sizer
	wxSizer* sizer = new wxBoxSizer( wxVERTICAL );

	// panel
	kome::window::CheckGridListPanel* panel = new kome::window::CheckGridListPanel( this );

	// check grid list
	my_list = new InstrumentCheckList(panel, m_instMgr);
	my_list->create();

	panel->setNumberOfSelectedItems(m_instMgr->numberOfCheckedRow());
	sizer->Add( panel, 1, wxALL | wxGROW );
	
	return sizer;
}

// on Click OK.
bool InstrumentSelectDialog::onOK() 
{
	std::vector<long long> selectDatas;
	my_list->getSelectedData(selectDatas);
	
	// if select spectra is empty 
	if ( selectDatas.empty() ) {
		std::string msg = "No instruments are selected. Select 1 or more instruments.";
		kome::window::WindowTool::showInfo(msg.c_str());
		return false;
	}
	m_instMgr->setInstrumentTypeShownFlagsWithIDArray( selectDatas );
	m_instMgr->saveCheckedStatus();

	return kome::window::StandardDialog::onOK();
}

// get selected data ids
void InstrumentSelectDialog::getMyListStatus(
	std::vector<long long>& selectDatas
){
	my_list->getSelectedData(selectDatas);
}

// #### InstrumentCheckList #####------------------------------------------------------------------------

// constructor
InstrumentCheckList::InstrumentCheckList(
	wxWindow* parent, 
	InstrumentMgr* instmgr
) :  kome::window::CheckGridListCtrl( parent, wxID_ANY, 300, 300, false )
, m_instMgr(instmgr)
{
}

// destructor
InstrumentCheckList::~InstrumentCheckList(){}

// on create List
void InstrumentCheckList::onCreate() 
{
	addColumn("Instrument", kome::window::GridListCtrl::TYPE_STRING);

	unsigned int idxEnd = m_instMgr->getNumberOfInstrumentTypes();
	m_orgInstList.clear();
	m_orgInstList.reserve(idxEnd);

	for (unsigned int i = 0; i < idxEnd; i++) {
		addData(i, false);
		setSelected(i, m_instMgr->isShown(i)); 
		m_orgInstList.push_back(m_instMgr->getInstrumentType(i));
	}
}

// get String
std::string InstrumentCheckList::getString(
	const long long data, 
	const int column ) const 
{
	if ( column == 0 ) {
		return m_orgInstList[data];
	}
	// if nothing
	return "";
}

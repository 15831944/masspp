/**
 * @file NameSelectToAccession.cpp
 * @brief 外部DBから名前検索した結果を選択するダイアログ 
 *
 * @author M.Fukuda
 * @date 2013.12.21
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#include "../rel_stdafx.h"
#include "NameSelectToAccession.h"

using namespace kome::massbank;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


#define TO_RIGHT_BT  5470
#define TO_LEFT_BT   5471

BEGIN_EVENT_TABLE(CandidatesList, GridListCtrl)
END_EVENT_TABLE()
CandidatesList::CandidatesList(
	wxWindow* parent,
	std::vector<Substance>& substs
	) : kome::window::GridListCtrl(parent, wxID_ANY, 500, 150, false, true,true)
	, substs_(substs)
{
}

CandidatesList::~CandidatesList()
{
}

void CandidatesList::onCreate()
{
	addColumn( "Name", kome::window::GridListCtrl::TYPE_STRING, 320);
	addColumn( "Database", kome::window::GridListCtrl::TYPE_STRING, 80);
	addColumn( "Accession", kome::window::GridListCtrl::TYPE_STRING, 100);
	reloadMyList();
}

void CandidatesList::reloadMyList() {
	m_allData.clear();
	m_data.clear();

	int idx = 0;
	for each(auto& sub in substs_) {
		if (!sub.onList) {
			this->addData(idx, false);
		}
		idx++;
	}
	this->updateList();
}

std::string CandidatesList::getString(
	const long long data, 
	const int column) const 
{
	int idx = static_cast<int>(data);
	Substance& sub = substs_[idx];
	if(column == 0) {    // NAME
		return sub.name;
	} else if(column == 1) {    // DB 
		return sub.db;
	} else if( column == 2 ) {    // Accession
		return sub.accession;
	}
	return "";
}

// ***+---------------------+*********
//    | Dialog              |
// ***+---------------------+*********
BEGIN_EVENT_TABLE(NameSelectToAccessionDialog, kome::window::StandardDialog)
END_EVENT_TABLE()

NameSelectToAccessionDialog::NameSelectToAccessionDialog(
	wxWindow* parent,
	const std::string& keyword,
	const std::vector<std::string>& vals
	) : kome::window::StandardDialog(parent, "select External Database")
	, keyword_(keyword)
	, substs_()
	, candlist_(nullptr)
	, selectedItem_(-1)
{
	const unsigned int vlnum = vals.size();
	for (unsigned int i = 0; i < vlnum; i = i + 3) {
		// DB - NAME - ACC の順に来る。
		// NAME - DB - ACC の順に入れ替える。
		substs_.push_back(Substance(vals[i+1], vals[i], vals[i+2]));
	}
}

NameSelectToAccessionDialog::~NameSelectToAccessionDialog()
{
}

wxSizer* NameSelectToAccessionDialog::createMainSizer()
{
	// Creation Spec Lists Sizer
	wxFlexGridSizer* SpecListSizer = new wxFlexGridSizer(2, 1, GRID_GAP, GRID_GAP);
	
	SpecListSizer->AddGrowableRow(1);
	SpecListSizer->AddGrowableCol(0);

	// Add Search Key Text Box
	wxStaticText* stxt = new wxStaticText(this, wxID_STATIC,
		wxT(FMT("Search Keyword : %s", keyword_.c_str())),
		wxDefaultPosition,
		wxDefaultSize,
		wxALIGN_LEFT);
	SpecListSizer->Add(stxt, 1, wxALL|wxGROW, 2);

	// Make List
	candlist_ = new CandidatesList(this, substs_);
	candlist_->create();

	SpecListSizer->Add( candlist_, 1, wxALL|wxGROW, BORDER_SIZE);
	return SpecListSizer;
}

const bool NameSelectToAccessionDialog::getLinkLists(
	std::vector<std::pair<std::string, std::string>>& rst
){
	for each(auto& sub in substs_) {
		if (sub.onList) {
			rst.push_back(std::make_pair(sub.db, sub.accession));
		}
	}
	if(rst.empty()) return false;
	return true;
}

void NameSelectToAccessionDialog::doReloadMyLists() 
{
	this->onReloadMyLists();
}

void NameSelectToAccessionDialog::onReloadMyLists() 
{
	candlist_->reloadMyList();
}

bool NameSelectToAccessionDialog::TransferDataFromWindow()
{
	selectedItem_ = 
		candlist_->GetNextItem(selectedItem_, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
	if (selectedItem_ == -1) return false;
	return true;
}

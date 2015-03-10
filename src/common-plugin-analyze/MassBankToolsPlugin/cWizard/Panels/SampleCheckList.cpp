/**
 * @file SampleCheckList.cpp
 * @brief sample check list(use Wizard's sample select)
 *
 * @author M.Fukuda
 * @date 2013.01.18
 *
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#include "../rel_stdafx.h"
#include "SampleCheckList.h"

kome::window::CheckGridListPanel* createSampleCheckList(
	wxWindow* parent,
	const std::vector<kome::objects::Sample*>& samples,
	SampleCheckList*& clist
){
	kome::window::CheckGridListPanel* panel = 
		new kome::window::CheckGridListPanel(parent);

	// check grid list
	clist = new SampleCheckList(panel,samples);
//	clist->create();
//	clist->selectAll();
	return panel;
}

//---------------------------------------------------------------
//
// Class Check List
//
//---------------------------------------------------------------
BEGIN_EVENT_TABLE(SampleCheckList, kome::window::CheckGridListCtrl)
END_EVENT_TABLE()

// constructor
SampleCheckList::SampleCheckList(
	wxWindow* parent,
	const std::vector<kome::objects::Sample*>& samples
) : kome::window::CheckGridListCtrl(parent, wxID_ANY, 500, 100, false)
	, sampleArr_(samples)
{
}

// destructor
SampleCheckList::~SampleCheckList(){}

// inner Creater
void SampleCheckList::onCreate() 
{
	addColumn("Sample", kome::window::GridListCtrl::TYPE_STRING);
}

void SampleCheckList::setupMe()
{
	this->create();
	// Setup Data
	for(unsigned int cnt = 0; cnt < sampleArr_.size(); cnt++) {
		addData(cnt,false);
	}
	selectAll();
	this->Refresh();
}

// inner Get String Value (virtual)
std::string SampleCheckList::getString(
	const long long dataidx, 
	const int column) const 
{
	std::string rst("");
	if(0 <= dataidx && dataidx < sampleArr_.size()) {
		rst = FMT("%s", sampleArr_[dataidx]->getName());
	}
	return rst;
}

// get selected spectra (from checked List Id)
const bool SampleCheckList::getSelectedSample(
	std::vector<kome::objects::Sample*>& rstArr
){
	std::vector<long long> sIds;
	this->getSelectedData(sIds);

	if (sIds.empty()) {
		return false;
	}
	for each(auto& idx in sIds) {
		rstArr.push_back(sampleArr_[idx]);
	}
	return true;
}

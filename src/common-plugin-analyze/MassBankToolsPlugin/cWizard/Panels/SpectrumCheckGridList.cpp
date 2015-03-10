/**
 * @file SpectrumCheckGridList.cpp
 * @brief Spectrum Select Panel
 *
 * @author M.Fukuda
 * @date 2013.02.07
 *
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#include "../rel_stdafx.h"
#include "SpectrumCheckGridList.h"
using namespace pane;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


//-----------------------------------------------
//  Form HelpGetterPanel (write by myself)
//-----------------------------------------------
BEGIN_EVENT_TABLE(SpecCheckListPanel, kome::window::HelpGetterPanel)
END_EVENT_TABLE()

// constructor
SpecCheckListPanel::SpecCheckListPanel(
	wxWindow* parent,
	const std::string& dsc,
	const bool isDisplayMode
) : kome::window::HelpGetterPanel(parent)
	,mydsc_(dsc)
	,isDisplayMode_(isDisplayMode)
{
// create controls
	createControls();
}

// destructor
SpecCheckListPanel::~SpecCheckListPanel() {}

// create main sizer
wxSizer* SpecCheckListPanel::createMainSizer() 
{
	wxFlexGridSizer* sizer = new wxFlexGridSizer(1, 1, GRID_GAP, GRID_GAP);
	
	panel_ = 
		new kome::window::CheckGridListPanel(this);

	// check grid list
	scList_ = new SpectraCheckList(panel_, 500, 200, isDisplayMode_);
	scList_->create();
	panel_->setDescription(mydsc_.c_str());

	sizer->Add(panel_, 1, wxALL | wxGROW );
	return sizer;
}

void SpecCheckListPanel::clearMyList()
{
	scList_->clearMyList();
//	panel_->Refresh();
}

void SpecCheckListPanel::setShowSpectra(
	const std::vector<kome::objects::Spectrum*>& src
){
	scList_->setShowSpectra(src);
	scList_->Refresh();
}

//Sample単位のSpectraをListに格納する
void SpecCheckListPanel::setBaseSpectra(
	const std::vector<kome::objects::Spectrum*>& specArr
){
	scList_->clearMyAllSpecArr();
	scList_->clearMyList();
	scList_->addBaseSpectra(specArr);
	scList_->setShowAll();
	scList_->Refresh();
}

// get spec data inner->mainPanel
const bool SpecCheckListPanel::getSelectedSpectra(
	std::vector<kome::objects::Spectrum*>& specArr,
	const bool finalFg
){
	return scList_->getSelectedSpectra(specArr,finalFg);
}

void SpecCheckListPanel::setSelectedElement(const bool finalFg)
{
	scList_->setSelectedElement(finalFg);
}


const std::string kSpec("Spectrum");
const std::string kRT("RT");
const std::string kFileName("File Name");
const std::string kStage("Stage");
const std::string kScanNo("Scan Number");
const std::string kPrecursor("Precursor");

//---------------------------------------------------------------
//
// Class Check List
//
// Note typeNo 
//   1. OpenedSample's All Specra.
//   2. Displyed       All Specra.
//---------------------------------------------------------------
BEGIN_EVENT_TABLE(SpectraCheckList, kome::window::CheckGridListCtrl)
END_EVENT_TABLE()

// constructor
SpectraCheckList::SpectraCheckList(
	wxWindow* parent,
	const unsigned int xwidth, 
	const unsigned int xheight,
	const bool isDisplayMode
	)
	: kome::window::CheckGridListCtrl(parent, wxID_ANY, xwidth, xheight, false)
	, isDisplayMode_(isDisplayMode)
{}

// destructor
SpectraCheckList::~SpectraCheckList(){}

// inner Creater
void SpectraCheckList::onCreate() 
{
	if(isDisplayMode_) {
		addColumn(kSpec.c_str(),      kome::window::GridListCtrl::TYPE_STRING);
	} else {
		// columns
		addColumn(kSpec.c_str(),      kome::window::GridListCtrl::TYPE_STRING);
		addColumn(kRT.c_str(),        kome::window::GridListCtrl::TYPE_DOUBLE);
		addColumn(kFileName.c_str(),  kome::window::GridListCtrl::TYPE_STRING);
		addColumn(kStage.c_str(),     kome::window::GridListCtrl::TYPE_INT);
		addColumn(kScanNo.c_str(),    kome::window::GridListCtrl::TYPE_INT);
		addColumn(kPrecursor.c_str(), kome::window::GridListCtrl::TYPE_DOUBLE);
	}
}

kome::objects::Spectrum* SpectraCheckList::onGetSpec(
	const long long dataidx) const
{
	if(-1 < dataidx && dataidx < allspecArr_.size()) {
		return allspecArr_[dataidx].spec;
	} else {
		return nullptr;
	}
}

// FileNameを取得する(表示の関連)
const char* getFileName(kome::objects::Spectrum* spec)
{
	return spec->getSample()->getSampleSet()->getFileName();
}

// inner Get String Value (virtual)
std::string SpectraCheckList::getString(
	const long long dataidx, 
	const int column) const 
{
	kome::objects::Spectrum* spec = onGetSpec(dataidx);
	std::string rst("");
	if(spec != nullptr) {
		if(isDisplayMode_) {
//			rst = FMT("%s : %s",spec->getName(), spec->getSample()->getName());
			rst = FMT("%s : %s",spec->getName(), getFileName(spec));
		} else {
			if(column == 0) {
				rst =  spec->getName();
			} else if(column == 2) {
				rst = getFileName(spec);
			}
		}
	}
	return rst;
}

// inner Get Double Value (virtual)
double SpectraCheckList::getDouble(
	const long long dataidx, 
	const int column) const 
{
	double val = -1.0;
	kome::objects::Spectrum* spec = onGetSpec(dataidx);
	if(spec != nullptr) {
		if(column == 1) { // as RT
			val = spec->getRt();
		} else if(column == 5) {
			val = spec->getPrecursor();
		}
	}
	return val;
}

// inner Get Integer Value (virtual)
int SpectraCheckList::getInt(
	const long long dataidx, 
	const int column) const 
{
	int val = -1;
	kome::objects::Spectrum* spec = onGetSpec(dataidx);
	if(spec != nullptr) {
		if(column == 3) { // as stage
			val = spec->getMsStage();
		} else if(column == 4) {
			val = spec->getScanNumber();
		}
	}
	return val;
}

// Clear Showing List Datas
void SpectraCheckList::clearMyList(){
//	specArr_.clear();
	showIdx_.clear();
	this->deselectAll();
	this->clearData(true);
}

// ADD All Spectra(Set For Master Data)
void SpectraCheckList::addBaseSpectra(
	const std::vector<kome::objects::Spectrum*>& specArr
){
	for each(auto p in specArr) {
		allspecArr_.push_back(specFlag(p));
	}
}

// Show All Master's Spectra
void SpectraCheckList::setShowAll()
{
	clearMyList();
	unsigned int anum = allspecArr_.size();
	for(unsigned int i =0; i < anum; i++) {
		showIdx_.push_back(i);
	}
	updateMySpectraList();
}

void SpectraCheckList::updateMySpectraList()
{
	for each(auto i in showIdx_) {
		addData(i,false);
		if(allspecArr_[i].checked) {
			m_selectedDataSet.insert(i);
		}
	}
	this->updateList();
}

void SpectraCheckList::setSelectedElement(const bool finalFg)
{
	onSetSelectedElement(finalFg);
}

// show src Spectrum
void SpectraCheckList::setShowSpectra(
	const std::vector<kome::objects::Spectrum*>& src
){
	// 直前の状態を確保する
	onSetSelectedElement(false);
	clearMyList();

	const unsigned int anum = allspecArr_.size();
	for each(auto& spec in src) {
		for(unsigned int i =0; i < anum; i++) {
			if(allspecArr_[i].spec == spec) {
				showIdx_.push_back(i);
				break;
			}
		}
	}
	updateMySpectraList();
}

// 表示されていたチェックの状態をMasterに書き込む
void SpectraCheckList::onSetSelectedElement(const bool finalFg)
{
	for each(auto i in showIdx_) {
		allspecArr_[i].checked = isSelected(i);
	}
}

// Trueだったものを配列に入れて返す。
const bool SpectraCheckList::getSelectedSpectra(
	std::vector<kome::objects::Spectrum*>& refArr,
	const bool finalFg
){
	if(finalFg) {
		for each(auto i in showIdx_) {
			if(isSelected(i)) {
				refArr.push_back(allspecArr_[i].spec);
			}
		}
	} else {
		for each(auto t in allspecArr_) {
			if(t.checked) {
				refArr.push_back(t.spec);
			}
		}
	}
	bool ret =! refArr.empty();
	return ret;
}

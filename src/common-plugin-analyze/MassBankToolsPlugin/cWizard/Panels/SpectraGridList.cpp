/**
 * @file SpectrumCheckListPanel.cpp
 * @brief Spectrum Select Panel
 *
 * @author M.Fukuda
 * @date 2013.02.07
 *
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#include "../rel_stdafx.h"
#include "SpectraGridList.h"

const std::string kSpec("Spectrum");
const std::string kRT("RT");
const std::string kFileName("File Name");
const std::string kStage("Stage");
const std::string kScanNo("Scan Number");
const std::string kPrecursor("Precursor");

namespace kome {
	namespace massbank {
		namespace wizard {

//---------------------------------------------------------------
// Class Check List
//
// Note typeNo 
//   0. ActiveSample's All Specta.
//   1. OpenedSample's All Specra.
//   2. Displyed       All Specra.
//---------------------------------------------------------------
BEGIN_EVENT_TABLE(SpectraGridList, kome::window::GridListCtrl)
END_EVENT_TABLE()

// constructor
SpectraGridList::SpectraGridList(
	wxWindow* parent,
	wxWindowID thisId,
	const unsigned int width,
	const unsigned int height
) : kome::window::GridListCtrl(parent, thisId, width, height, false) {}

// destructor
SpectraGridList::~SpectraGridList(){}

// inner Creater
void SpectraGridList::onCreate() 
{		
	// columns
	addColumn(kSpec.c_str(),      kome::window::GridListCtrl::TYPE_STRING);
	addColumn(kRT.c_str(),        kome::window::GridListCtrl::TYPE_DOUBLE);
	addColumn(kFileName.c_str(),  kome::window::GridListCtrl::TYPE_STRING);
//	addColumn(kStage.c_str(),     kome::window::GridListCtrl::TYPE_INT);
	addColumn(kStage.c_str(),     kome::window::GridListCtrl::TYPE_STRING);
	addColumn(kScanNo.c_str(),    kome::window::GridListCtrl::TYPE_INT);
	addColumn(kPrecursor.c_str(), kome::window::GridListCtrl::TYPE_DOUBLE);
}

// inner Get String Value (virtual)
std::string SpectraGridList::getString(const long long dataidx, const int column) const 
{
	if (dataidx < listArr_.size()) {
		if(column == 0) {
			return listArr_[dataidx].specname;
		} else if(column == 2) {
			return listArr_[dataidx].filename;
		} else if(column == 3) { // as stage
			return listArr_[dataidx].stage;
		}
	}
	return "";
}

// inner Get Double Value (virtual)
double SpectraGridList::getDouble(const long long dataidx, const int column) const 
{
	double val = -1.0;
	if (dataidx < listArr_.size()) {
		if(column == 1) { // as RT
			val = listArr_[dataidx].rt;
		} else if(column == 5) {
			val = listArr_[dataidx].precursor;
		}
	}
	return val;
}

// inner Get Integer Value (virtual)
int SpectraGridList::getInt(const long long dataidx, const int column) const 
{
	int val = -1;
	if (dataidx < listArr_.size()) {
		if(column == 4) {
			val = listArr_[dataidx].scannum;
		}
	}
	return val;
}

// Set SpecData inner List
void SpectraGridList::onSetSpecData(
	kome::objects::Spectrum* spec, 
	const std::string& tgtfilename, 
	const unsigned int listIdx
){
	if(spec != NULL){
		// set row data
		listDatas tmplist;
		tmplist.specname = FMT("%s", spec->getName());
		tmplist.rt = spec->getRt();
		tmplist.filename = tgtfilename;
		tmplist.stage = convMsType(spec->getMsStage());

		tmplist.scannum = spec->getScanNumber();
		tmplist.precursor = spec->getPrecursor();
		tmplist.spec = spec;
			
		listArr_.push_back(tmplist);
		addData(listIdx, false);
	}
}

std::string SpectraGridList::convMsType(const int type) const {
	if(type == 1) return "MS";
	if(type == 2) return "MS2";
	return "All";
}

// get selected spectra (from checked List Id)
void SpectraGridList::setSelectedSpectra(
	std::vector<kome::objects::Spectrum*>& specArr,
	const bool reset
){
	// wait cursor
	wxBeginBusyCursor();


	if(reset) {
		listArr_.clear();
		this->clearData(false);
	}
	unsigned int idx = listArr_.size();

	std::for_each(specArr.begin(), specArr.end(), 
		[&](kome::objects::Spectrum* spec) {
			std::string flname = spec->getSample()->getSampleSet()->getFileName();
			onSetSpecData(spec, flname, idx);
			idx++;}
	);
	this->updateList();
	wxEndBusyCursor();
}

kome::objects::Spectrum* SpectraGridList::getSelectSpectrum(const int& id) 
{
	// @date 2013/06/10 <Add> OKADA	------->
	if( id < 0 ){
		return nullptr;
	}
	// @date 2013/06/10 <Add> OKADA	<-------

	if (m_data.size() < (unsigned int)id){	// @date 2013/06/10 <Mod> OKADA	
		return nullptr;
	}
	const int myId = m_data[id];
	return listArr_[myId].spec;
}


}// end of namespace
}
}

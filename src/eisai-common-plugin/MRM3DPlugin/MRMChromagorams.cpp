/**********************************************************/
/*! @addtogroup MRM3DPlugin 
    @file       MRMChromatograms.cpp
    @brief      MRM Chromarogram Objects Class
    @date       2012.10.24
	@author     M.Fukuda
	@version    1.0.0
	
	Copyright(C) 2006-2013 Eisai Co., Ltd. 
	                       All rights reserved.
 **********************************************************
2012.10.24
MRM3D 独自のChromatogram管理クラス
*/
/**********************************************************/
#include "MRMChromagorams.h"

#include <boost/lexical_cast.hpp>
#include <boost/lambda/lambda.hpp>

#include "FilterParser.h"

using namespace kome::mrm3d;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


// Check MRM Type Filter ?
const bool MRMType::checkMRMType(const std::string& filterStr) {
	std::string q1s;
	std::vector<std::string> q3sArr;
	double tq1, tq3f, tq3c, tq3e;
	if ( ! MRMFilterParser(filterStr, q1s, q3sArr)) return false;
	if ( ! Q1_Cast(q1s, tq1)) return false;

	for each(const std::string& q3 in q3sArr) {
		if ( ! Q3Parser(q3,tq3c,tq3f,tq3e) ) {
			return false;
		}
	}
	return true;
}

// constructor
MRMChrom::MRMChrom(
	const std::string& _q1,
	const std::string& _q3
) : q1word(_q1), q3word(_q3) {
	// Cast Inner Settings
	SetInit();
}

// destroctor
MRMChrom::~MRMChrom(){
	std::for_each(dataPoints.begin(),dataPoints.end(),
		[](RTInt* rti){delete rti;});
}

// init
void MRMChrom::SetInit() {
	Q1_Cast(q1word, q1);
	Q3Parser(q3word, q3, q3f, q3e);
}

// judge in range
const int MRMChrom::isInRange(const double& tgt) {
	if (isEqual(this->q3f, tgt)) return 0;
	if (isEqual(this->q3e, tgt)) return 0;
	if (tgt < this->q3f) return -1;
	if (this->q3e < tgt) return 1;
	return 0;
}

// Add Points
void MRMChrom::setPoints(const double& rt, const double& intensity) {
	RTInt* dtp = new RTInt(rt, intensity);
	this->dataPoints.push_back(dtp);
}

void MRMChrom::sortDataPoints() {
	std::sort(dataPoints.begin(), dataPoints.end(),
		[](RTInt* x, RTInt* y){ return x->RT < y->RT; });
}

// Back Target XIC
void MRMChrom::addLastPoints(const double& _intensity) {
	this->dataPoints.back()->Intensity += _intensity;
}

// Target Function(Feed Back)
// Maybe Target is xydata->addpoint(x,y)
void MRMChrom::doAssignXYData() {
	reserveFunc(dataPoints.size());
	for each(const RTInt* rti in dataPoints) {
		addFunc(rti->RT, rti->Intensity);
	}
}

//---------------------------------------------------------
//---------------------------------------------------------
// constructor
MRMChromKeeper::MRMChromKeeper(const std::string& myFilter)
: myFilter_(myFilter) {}

// destructor
MRMChromKeeper::~MRMChromKeeper(){}

// call doInit
const bool MRMChromKeeper::doInit() {
	return onSetInit();
}

// Private....
const bool MRMChromKeeper::onSetInit() {

	std::string q1;
	std::vector<std::string> q3arr;
	if (!MRMFilterParser(myFilter_, q1, q3arr)) {
		return false;
	}
	this->chroms_.reserve(q3arr.size());
	for each(const std::string& q3 in q3arr) {
		tMRMChromPtr chm = std::make_shared<MRMChrom>(q1, q3);
		this->chroms_.push_back(chm);
	}
	sortMyChromsByQ3();
	return true;
}

// sort by Q3
void MRMChromKeeper::sortMyChromsByQ3() {
//	std::sort(chroms_.begin(), chroms_.end(), q3_sorter());
	std::sort(chroms_.begin(), chroms_.end(), []
	(const tMRMChromPtr& a, const tMRMChromPtr& b){ return a->getQ3() < b->getQ3();});
}

// Public
void MRMChromKeeper::addPointsBase(const double& rt) {
	// for my Init
	chromsIdx_ = 0;
	overRange_ = false;
	std::for_each(chroms_.begin(), chroms_.end(),
		[rt](tMRMChromPtr& chm) {chm->setPoints(rt, 0.0);});
}

// 対象のmzが配下のMRMChrom内(Q3f-Q3e)に入るか否か判定。
// 再帰的に連続して子を探す。
void MRMChromKeeper::findAndsetPoints(const double& mz, const double& intensity) {
	if (overRange_) return;

	int flgs = chroms_[chromsIdx_]->isInRange(mz);

	if (flgs < 0) return;
	else if (flgs == 0) {
		chroms_[chromsIdx_]->addLastPoints(intensity);
	} else {
		chromsIdx_++;
		overRange_ = ! canAddPoints();
		findAndsetPoints(mz, intensity);
	}
}

// My Child Chroms Size
const bool MRMChromKeeper::canAddPoints() const {
	return chromsIdx_ < chroms_.size();
}

// Marge My Child Chroms to allChroms
void MRMChromKeeper::margeToAll(tMRMVCT& allChroms) {
	allChroms.insert(allChroms.end(), chroms_.begin(), chroms_.end());
}

/**
 * @file SpecElemSetter.cpp
 * @brief Wizard matrial(Label,ImagePath,..) message declaration
 *
 * @author M.Fukuda
 * @date 2013.12.21
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#include "../rel_stdafx.h"
#include "SpecElmSetter.h"

// 表示されているSpectrumを配列にして渡す
const bool elm::SpectrumElementSetter::getDisPlayedSpectra(
	std::vector<kome::objects::Spectrum*>& specArr
) {
	using namespace kome::objects;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG

	ActiveObjectsManager& aoMgr = ActiveObjectsManager::getInstance();
	specArr = aoMgr.getOpenSpectra();
	bool ret = !specArr.empty();
	return ret;
}

// 開かれているSampleを内部的に格納する
// *Ver2.7.0
// 文字列化したsampleの情報をユニークに保てないので
// 初期化時にユニーク化してIDを発行し、それを受け取る事にする。
void elm::SpectrumElementSetter::initMe()
{
	using namespace kome::objects;
	ActiveObjectsManager& aoMgr = ActiveObjectsManager::getInstance();
	unsigned int activeSampleNum = aoMgr.getNumberOfOpenedSamples();

	for(unsigned int idx = 0; idx < activeSampleNum; idx++) {
		Sample* sp = aoMgr.getOpenedSample(idx);
		SampleSet* ssp = sp->getSampleSet();
//		std::string es = FMT("%s [%s]",sp->getName(), ssp->getFileName());
		std::string es = FMT("%s",ssp->getFileName());
		splMap_.insert(kSPair(es,sp));
	}
}

// 内部のSample情報を返す
const bool elm::SpectrumElementSetter::getOpendAllSamples(
	std::vector<kSPair>& rstArr
){
	initMe();
	for each(auto& m in splMap_) {
		rstArr.push_back(m);
	}
	bool ret = !rstArr.empty();
	return ret;
}

// Sampleの情報だけを返す
const bool elm::SpectrumElementSetter::getOpendAllSamples(
	std::vector<kome::objects::Sample*>& rstArr
){
	initMe();
	for each(auto& m in splMap_) {
		rstArr.push_back(m.second);
	}
	return true;
}

// SampleIDの情報から対応するSampleを返す
const bool elm::SpectrumElementSetter::getSamplesSpectra(
	const std::string& ids,
	std::vector<kome::objects::Spectrum*>& rstArr)
{
	std::vector<std::string> tokens;
	kome::plugin::SettingsValue::separateListValue(ids.c_str(), tokens);
	bool ret =! tokens.empty();
	if(ret) {
		for each(auto& s in tokens) {
			auto itr = splMap_.find(s);
			if(itr != splMap_.end()) {
				this->onGetSampleSpectra(itr->second, rstArr);
			}
		}
	}
	return ret;
}

const bool elm::SpectrumElementSetter::checkAverageSpectraStream(
	const std::string& src,
	std::string& errmsg
){
	std::vector<kome::objects::Spectrum*> dmyarr;
	return onCheckAverageSpectraStream(src, errmsg, dmyarr);
}

const bool elm::SpectrumElementSetter::getAverageSpecElemements(
	const std::string& src,
	std::string& errmsg,
	std::vector<kome::objects::Spectrum*>& ref
){
	return onCheckAverageSpectraStream(src, errmsg, ref);
}

const bool elm::SpectrumElementSetter::onCheckAverageSpectraStream(
	const std::string& src,
	std::string& errmsg,
	std::vector<kome::objects::Spectrum*>& refArr
){
	kome::objects::Sample* s;
	double rtlow, rthigh;
	int mstype;
	if(!parseGetParameters(src, s, rtlow, rthigh, mstype, errmsg)) {
		//パースする際に何かの事故が発生した。
		if(errmsg.empty()) {
			errmsg = "List Parameter Parse Error";
		}
		return false;
	}

	kome::objects::DataSet ds = onGetDataSet(s);

	// 検索範囲のチェック
	if(onCheckFindRTRange(ds,rtlow,rthigh,errmsg)) {
		// Averageを構成するSpectrumの有無をチェック（ない場合はエラー)
		if(onGetMatchSpectra(ds,rtlow,rthigh,mstype,refArr,errmsg)) {
			return true;
		}
	}
	errmsg = "AverageSpectrum parameter error\n can not set : " + errmsg;
	return false;
}

const bool elm::SpectrumElementSetter::parseGetParameters(
	const std::string& src,
	kome::objects::Sample*& s,
	double& rtlow,
	double& rthigh,
	int& mstype,
	std::string& errmsg
){
	std::vector<std::string> prms;
	kome::plugin::SettingsValue::separateListValue(src.c_str(), prms);

	if(prms.size() != 4) {return false;}

	std::string idStr = kome::plugin::SettingsValue::convertToDisplayingString(prms[0].c_str());
	auto itr = splMap_.find(idStr);
	if(itr != splMap_.end()) {
		s = itr->second;
		if(isdouble(prms[1].c_str(),&rtlow) && isdouble(prms[2].c_str(),&rthigh)){
			mstype = 0;
			if (prms[3] == "ms") mstype = 1;
			if (prms[3] == "ms2") mstype = 2;
			return true;
		}
	} else {
		errmsg = FMT("Not opend sample: %s:", idStr.c_str());
		return false;
	}
	return false;
}


const bool elm::SpectrumElementSetter::onCheckFindRTRange(
	kome::objects::DataSet& ds,
	double& rtlow,
	double& rthigh,
	std::string& errmsg
){
	unsigned int specNum = ds.getNumberOfSpectra();
	
	ds.sortSpectra();
	const double fRT = ds.getSpectrum(0)->getRt();
	const double eRT = ds.getSpectrum(specNum - 1)->getRt();
	return onCheckFindRange(fRT, eRT, rtlow, rthigh, errmsg);
}

const bool elm::SpectrumElementSetter::onCheckFindRange(
	const double fRT,
	const double eRT,
	double& rtlow,
	double& rthigh,
	std::string& errmsg
){
	// まず-1を補完する
	if (rtlow < 0) rtlow = fRT;
	if (rthigh < 0) rthigh = eRT;
	// 検索最小 > 検索最大
	if (rthigh < rtlow) {
		errmsg = FMT("Start RT[%f] > End RT[%f].",rtlow, rthigh);
	}
	// 実最大 < 検索最小
	if (eRT < rtlow) {
		errmsg = FMT("Start RT[%f].( > sample max RT[%f])",rtlow, eRT);
	}
	// 実最小 < 検索最大
	if (fRT > rthigh) {
		errmsg = FMT("End RT[%f]. ( < sample min RT[%f]).",rthigh, fRT);
	}
	//エラー無し：正常
	return errmsg.empty();
}

const bool elm::SpectrumElementSetter::onGetMatchSpectra(
	kome::objects::DataSet& ds,
	const double rtlow,
	const double rthigh,
	const int mstype,
	std::vector<kome::objects::Spectrum*>& refArr,
	std::string& errmsg
){
	bool mfg = true;
	const unsigned int specNum = ds.getNumberOfSpectra();
	for(unsigned int idx = 0; idx < specNum; idx++) {
		kome::objects::Spectrum* spec = ds.getSpectrum(idx);
		double rt = spec->getRt();
		if(mstype > 0) {
			mfg = (spec->getMsStage() == mstype);
		}
		if (mfg && rtlow <= rt && rt <= rthigh) {
			refArr.push_back(spec);
		}
	}
	return (!refArr.empty());
}


// DataSetを取得する
kome::objects::DataSet elm::SpectrumElementSetter::onGetDataSet(
	kome::objects::Sample* p
){
	using namespace kome::objects;
	unsigned int pn = p->getNumberOfGroups();
	unsigned int uun(0);
	for(unsigned int i = 0; i < pn; i++) {
		DataGroupNode* dgn = p->getGroup(i);
		if(dgn != nullptr) {
			unsigned int vf = dgn->getNumberOfSpectra();
			uun += vf;
		}
	}

	DataGroupNode* dgnode = p->getRootDataGroupNode();
	DataSet dset(dgnode);
	dgnode->getDataSet(&dset);
	return dset;
}

// sampleから対応するSpectrumを返す
const bool elm::SpectrumElementSetter::onGetSampleSpectra(
	kome::objects::Sample* tgtsample,
	std::vector<kome::objects::Spectrum*>& specArr
){
	if(tgtsample == nullptr){
		return false;
	}
	kome::objects::DataSet dset = onGetDataSet(tgtsample);
	// all spectra
	unsigned int specNum = dset.getNumberOfSpectra();
	dset.sortSpectra();
	for(unsigned int idx = 0; idx < specNum; idx++) {
		specArr.push_back(dset.getSpectrum(idx));
	}
	return true;
}

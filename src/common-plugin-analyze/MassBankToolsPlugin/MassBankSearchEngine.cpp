/**
 * @file MassBankSearchEngine.cpp
 * @brief implements of MassBankSearchEngine class
 *
 * @author M.Fukuda
 * @date 2009.06.24
 *
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#include "stdafx.h"
#include "MassBankSearchEngine.h"
#include "MassBankManager.h"

using namespace kome::massbank;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


// constrctor
MassBankSearchEngine::MassBankSearchEngine(
	) : kome::ident::SearchEngine("MassBank","",true)
	, queryStringsTags_()
{
}

// destrctor
MassBankSearchEngine::~MassBankSearchEngine(){}

// This method is called when peaks of a spectrum are detected. (abstract method)
bool MassBankSearchEngine::isValidSpectrum(kome::objects::Spectrum* spec)
{
	// MS and MS2 and MS3 is ok.
	if(spec != nullptr) {
		return true;
	} else {
		return false;
	}
}


//validate the parameter settings (abstract method)
std::string MassBankSearchEngine::validateSettings(
	kome::objects::SettingParameterValues* params
){
	if(params->getParameterValue("email") == nullptr) {
		// Emailが必要な旨返す。
	}

	// 成功時は何も返さない。
	return "";
}

// Peak Detector at Prepare.
bool MassBankSearchEngine::onPreparePeakDetection(
	kome::objects::SettingParameterValues* searchSettings,
	const char* peakDetector,
	kome::objects::SettingParameterValues* peakSettings,
	const char* chargeDetector,
	kome::objects::SettingParameterValues* chargeSettings,
	const char* peakFilter
) {
	// Reset.
	// ピークディテクターに不備がある場合は返す？
	// ここで責任を持たせる必要が有る？
	queryStringsTags_.clear();
	return true;
}

// on Peak Detect
void MassBankSearchEngine::onSpectrumPeaks(
	kome::objects::Spectrum* spec,
	kome::ident::SpecInfo* specInfo,
	kome::objects::Peaks* peaks,
	int* peakIds,
	const unsigned int peakIdsSize
){
	MassBankManager& mgr = MassBankManager::getInstance();
	std::string querystr = mgr.getQueryStringTag(spec->getName(), peaks);
	if (! querystr.empty()){
		queryStringsTags_.push_back(querystr);
	}
}

// Peaks Detecot at Finish.
bool MassBankSearchEngine::onFinishPeakDetection()
{
	//ピークが完全に空になる場合はここで失敗扱い。（検索・登録の無駄）
	if (queryStringsTags_.empty()) {
		return false;
	} else {
		return true;
	}
}

// This method is called from search method. (abstract method)
std::string MassBankSearchEngine::onSearch(
	const char* title,
	const char* comment,
	kome::ident::SpecInfo** spectra,
	const unsigned int specNum,
	kome::objects::SettingParameterValues* settings,
	kome::core::Progress& progress
){
	std::string queryTags("");
	for each(auto& tmp in queryStringsTags_) {
		queryTags += tmp;
	}
	
	MassBankManager& mgr = MassBankManager::getInstance();
	std::string ids = mgr.batchSearch(queryTags, settings);

	if(ids.empty()) {
	LOG_ERROR_CODE(FMT(
		"There was a problem submitting the batch search.\nCheck the MassBank URL and Proxy setting in the Tools -> Options dialog."), ERR_OTHER );
	}

	progress.fill();
	return ids;
}

// This method is called by getSearchStatus method. (abstract method)
kome::ident::SearchEngine::SearchStatus MassBankSearchEngine::onGetSearchStatus(const char* id)
{
	kome::ident::SearchEngine::SearchStatus status = kome::ident::SearchEngine::STATUS_ERROR;

	if(id == nullptr) {
		return status;
	}
	// JOBSTATUSに応じて切り替える。
	MassBankManager& mgr = MassBankManager::getInstance();
	const int rst = mgr.getJobStatus(id);

	switch(rst) {
	case 0:
		status = kome::ident::SearchEngine::STATUS_WAITING;
		break;
	case 1:
		status = kome::ident::SearchEngine::STATUS_RUNNING;
		break;
	case 2:
		status = kome::ident::SearchEngine::STATUS_COMPLETED;
		break;
	default:
		// error
		break;
	}
	return status;
}

// 結果を取得する。
void MassBankSearchEngine::onGetResult(
	kome::ident::SearchResult& result, 
	const char* jobId,
	kome::ident::SpecInfo** spectra,
	const unsigned int specNum,
	kome::core::Progress& progress
){
	MassBankManager& mgr = MassBankManager::getInstance();
	mgr.getBatchResults(jobId);
	result.setResultLocation(jobId);
	const unsigned int rsnum = mgr.getNumberOfResults();
	progress.setRange(1, rsnum);

	for(unsigned int id = 0; id < rsnum && !progress.isStopped(); id++) { // @date 2013.11.25 <Mod> M.Izumi
		onSetSubTracts(id, result, spectra, specNum);
		progress.setPosition(id);
		progress.setStatus(FMT("Getting Results.. [%d/%d]", id, rsnum).c_str());
	}

	progress.fill();
}

void MassBankSearchEngine::onSetSubTracts(
	const unsigned int id,
	kome::ident::SearchResult& result, 
	kome::ident::SpecInfo** spectra,
	const unsigned int specNum
) {
	MassBankManager::SearchResult* searchrst = 
		MassBankManager::getInstance().getResult(id);

	std::vector<std::string> tokens;
	stringtoken(searchrst->title.c_str(), ";", tokens);
	const unsigned int tknum = tokens.size();

	std::string name     = (tknum > 0) ? trimstring(tokens[0].c_str()) : "";
	std::string insttype = (tknum > 1) ? trimstring(tokens[1].c_str()) : "";
	std::string msstage  = (tknum > 2) ? trimstring(tokens[2].c_str()) : "";

	// 残りは細分化せずに統合しておく。
	std::string others("");
	if (tknum > 3) {
		for(unsigned int i  = 3; i < tknum; i++ ) {
			others += tokens[i];
		}
	}

	kome::ident::Substance* subst = 
		new kome::ident::Substance(
		&result, 
		searchrst->id.c_str(),
		name.c_str(),
		kome::ident::TYPE_UNKNOWN,
		NULL);
	
	result.addSubstance(subst);

	kome::core::Properties& prop = subst->getProperties();
	prop.setDoubleValue("Exact Mass", searchrst->mass);
	prop.setValue("Formula", searchrst->formula.c_str());
	prop.setValue("Instrument Type", insttype.c_str());
	prop.setValue("MS Stage", msstage.c_str());
	prop.setValue("OTHERS", others.c_str());

	// Hit
	kome::ident::Hit* hit = result.addHit(subst);
	std::string scorename = FMT("Score of %s", searchrst->id.c_str());
	hit->addValue(scorename.c_str(), FMT("%f", searchrst->score).c_str(), kome::ident::TYPE_SCORE);

	MassBankManager::queryVector& qv = searchrst->queryArr; 

	for each(auto& q in qv) {
		for(unsigned int i = 0; i < specNum; i++) {
			kome::ident::SpecInfo* sinfo = spectra[i];
			if (q.first.compare(sinfo->name) == 0) {
				hit->addSpectrum(sinfo);
				break;
			}
		}
	}
}

// display the search result using the specified result location (abstract method)
bool MassBankSearchEngine::onOpenResult(const char* resultLocation)
{
	MassBankManager& mgr = MassBankManager::getInstance();
	// get results
	wxBeginBusyCursor();
	bool ret = mgr.getBatchResults(resultLocation);
	wxEndBusyCursor();

	// show result dialog
	if(ret) {
		mgr.setBatchSearchTypeName();
		mgr.showMassBankSearchResultsPane();
	} else {
		LOG_ERROR_CODE(FMT(
		"This job id is not Found."), ERR_OTHER );
	}

	return ret;
}

// This method is called from the prepareSearch method. (abstract method)
bool MassBankSearchEngine::onPrepareSearch(kome::core::Progress& progress)
{
	return true;
}

// This method is called when identification is finished. (abstract method)
void MassBankSearchEngine::onEndSearch()
{
}

// gets the search information (abstract method)
void MassBankSearchEngine::onGetSearchInfo(
	kome::ident::SearchInfo& searchInfo, 
	kome::objects::SettingParameterValues* settings
){
	searchInfo.db = "MassBank";
	searchInfo.enzyme = NULL;
	searchInfo.precMassType = kome::ident::SearchResult::MASS_UNKNOWN;
	searchInfo.fragMassType = kome::ident::SearchResult::MASS_UNKNOWN;
}

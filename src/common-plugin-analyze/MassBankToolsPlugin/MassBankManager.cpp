/**
 * @file MassBankManager.cpp
 * @brief implements of MassBankManager class
 *
 * @author S.Tanaka
 * @date 2009.06.24
 * 
 * @modify M.Fukuda
 * @date 2011.08.15, 2012.04.13, 2013.08.09
 *
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#include "stdafx.h"
#include "MassBankManager.h"

#include <iostream>
#include <fstream>
#include <boost/bind.hpp>
#include "wx/dirdlg.h"

#include "XMLHandler\ALLHandlerHeaders.h"
#include "XMLHandler\XMLMaker.h"

#include "MBMgrTool/InstrumentTypesMgr.h"
#include "MBMgrTool/SearchDialog.h"
#include "MBMgrTool/NetWorkCtrl.h"

#include "SearchResults\SearchResultPanel.h"
#include "SearchResults\SearchResultDialog.h"
#include "SearchResults\RecordPeaksDialog.h"

#include "MassBankSearchEngine.h"
#include "SearchResults\OverlaySpectrumTool.h"

#include "cWizard\calcFunc\StrFunc.h"
#include "cWizard\ErrorCheck.h"
#include "cWizard\IdentMgrAcc.h"
#include "cWizard\createRecordDialog.h"
#include "cWizard\Panels\VenderInstrument.h"
#include "cWizard\Panels\templ.hpp"

#include "cWizard\CommonLicenseArray.h"
#include "cWizard\RecordElements.h"

#include "RDKit\RDKitAccesser.h"

#define RESULTLISTPANE				"RESULTLISTPANE"
#define SPECTRUM_SEARCH_RESULT		"MassBank Spectrum Search Results"
#define BATCH_SEARCH_RESULT			"Batch Search Results"
#define PEAK_SEARCH_RESULT			"MassBank Peak Search Results"
#define PEAK_DIFF_SEARCH_RESULT		"MassBank Peak Difference Search Results"

#define MASSBANK_URL	"http://www.massbank.jp/"
#define NO_INI_DATA		"NODATA"
#define MASSBANK_API	"api/services/MassBankAPI?wsdl"

// MassBankのネットワーク状態をBitFlag化
#define BIT(num) ((unsigned int)1 << (num))
#define X_DB		BIT(0)
#define X_INST		BIT(1)
#define X_NET		BIT(2)
#define X_NOTFORCE	BIT(3)

using namespace kome::massbank;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


// constructor
MassBankManager::MassBankManager() 
	: m_searchDlgPos(-1, -1)
	, m_resultDlgPos(-1, -1)
	, m_searchTypeName("MassBank Search Results")
	, activeInfoPtr_(nullptr)
	, m_resultDlg(NULL)
	, m_useOverlapSpec(false)
	, instMgr_(new InstrumentMgr())
	, xmlMaker_(new XMLMaker())
	, networkCtrl_(new NetWorkCtrl())
	, overlaytool_(new OverlaySpectrumTool())
	, search_result_dialog_show_flag_(0)
	, show_modal_ids_()
{
	// Set Others
	instMgr_->set_f(boost::bind(&MassBankManager::getInstrumentTypesFromMassBank, this, _1));
}

// destructor
MassBankManager::~MassBankManager()
{
//	RecoverBatchPrefix();
}

// --- MS Types Set ----------------------------------------------------------------------

// Search(Batch) Results Inform Setter/Getter

// set Batch Search Type Name
void MassBankManager::setBatchSearchTypeName() 
{
	m_searchTypeName = BATCH_SEARCH_RESULT;
}

// if Need Score Col which Search Type Name
const bool MassBankManager::needScoreCol() 
{
	bool ret = m_searchTypeName.find(SPECTRUM_SEARCH_RESULT) != std::string::npos;
	ret |= m_searchTypeName.find(BATCH_SEARCH_RESULT) != std::string::npos;
	return ret;
	//return (m_searchTypeName == SPECTRUM_SEARCH_RESULT ||
	//	m_searchTypeName == BATCH_SEARCH_RESULT);
}

// clear results
void MassBankManager::clearResults() 
{
	m_results.clear();
	record_info_map_.clear();
	activeInfoPtr_ = nullptr;
	m_useQuery = false;
}

// get the number of results
unsigned int MassBankManager::getNumberOfResults() 
{
	return m_results.size();
}

// get result
MassBankManager::SearchResult* MassBankManager::getResult(const unsigned int idx)
{
	if(idx >= m_results.size()) {
		return NULL;
	}
	return &(m_results[idx]);
}

// Control Active Recored ---------------------------------------------------------------------------

const bool MassBankManager::getSearchRecordDetailsFromMassBank(
	const int idx,
	const bool needDetailes
){
	if (isValidActiveId(idx)) {
		if(needDetailes) {
			setMoreRecordDetailestoActiveRecordInfo();
		}
		return true;
	}

	SearchResult* result = this->getResult(idx);

	if( result == NULL ) {
		LOG_ERROR(FMT("Cannot get the record information."));
		return false;
	}
	std::string title = result->id;
	if( !result->title.empty() ) {
		title.append( " : " );
		title.append( result->title );
	}

	// set record information
	wxBeginBusyCursor();

	std::string info;

	if(getRecordInfo(result->id.c_str(), info)) {
		setSearchRecordDetails(idx, result->id, title, info, needDetailes);
	} else {
		LOG_ERROR(FMT("Cannot get the record information."));
	}
	wxEndBusyCursor();
	return true;
}

void MassBankManager::setSearchRecordDetails(
	const unsigned int ctrlIdx,
	const std::string& recordId,
	const std::string& record_title_name,
	const std::string& recordInfo,
	const bool setDetaile
){
	if(recordId.empty()) {
		return;
	}
	RecordInfo& tgt = record_info_map_[ctrlIdx];
	// MAPにあれば、初期化されて id が空欄。
	if(tgt.id.empty()) {
		tgt.id = recordId;
		tgt.titlename = record_title_name;
		tgt.info = recordInfo;

		if(setDetaile) {
			setMoreRecordDetailes(tgt);
		}
	}
	activeInfoPtr_ = &tgt;
}

void MassBankManager::setMoreRecordDetailes(
	RecordInfo& tgt
){
	if(!tgt.detaileFg) {
		getPeakInfo(tgt.id.c_str(), tgt.dps);
		createChemicalFormulaPNG(tgt);
		tgt.detaileFg = true;
	}
}

void MassBankManager::setMoreRecordDetailestoActiveRecordInfo()
{
	setMoreRecordDetailes(*activeInfoPtr_);
}

const bool MassBankManager::createChemicalFormulaPNG(
	RecordInfo& tgt
){
	// Tempフォルダを取得し、RecordID + PNGの構造式ファイルを作成する。
	std::string tmpDir = kome::core::MsppManager::getInstance().getTmpDir();
	std::string tmpFile = tgt.id + ".PNG";
	std::string pngFullPath = getpath(tmpDir.c_str(), tmpFile.c_str());

	std::string key1("CH$SMILES: ");
	std::string key2("CH$IUPAC: ");
	size_t nd1 = tgt.info.find(key1);
	size_t nd2 = tgt.info.find(key2);

	if(nd1 != std::string::npos && nd2 != std::string::npos) {
		nd1 += key1.length();
		std::string tgtsmiles = tgt.info.substr(nd1,(nd2 - nd1));
		if(mol::drawSmiles(tgtsmiles, pngFullPath, 400, 300)) {
			tgt.png_path = pngFullPath;
			return true;
		}
	}
	return false;
}

// unset active record
void MassBankManager::unsetActiveRecord() 
{
	// initialize
	record_info_map_.clear();
	activeInfoPtr_ = nullptr;

	// refresh
	kome::window::WindowTool::refresh();
}

// get active record information
const char* MassBankManager::getActiveRecordInfo() 
{
	if(activeInfoPtr_ != nullptr) {
		return activeInfoPtr_->info.c_str();
	} 
	return NULL;
}

// 格納したDailog用のタイトルを返す
const char* MassBankManager::getActiveRecordTitleName()
{
	if(activeInfoPtr_ != nullptr) {
		return activeInfoPtr_->titlename.c_str();
	} 
	return NULL;

}

// 格納したPNGのパスを返す。無効であればNULLが返る
const std::string MassBankManager::getActiveRecordPngPath()
{
	if(activeInfoPtr_ != nullptr) {
		return activeInfoPtr_->png_path;
	} 
	return "";
}

// get active record peaks
kome::core::XYData* MassBankManager::getActiveRecordPeaks()
{
	if(activeInfoPtr_ != nullptr) {
		return &activeInfoPtr_->dps;
	} 
	return NULL;
}

// Search Result Dialog(Details)の表示状態を保存、更新する。
void MassBankManager::setSearchResultDialogShowFlag(const unsigned int flags)
{
	search_result_dialog_show_flag_ = flags;
	if (m_rsPeaksDlg != nullptr) {
		m_rsPeaksDlg->upDateInfo();
		m_rsPeaksDlg->Update();
		m_useOverlapSpec = ((flags & 4) == 0) ? true : false;
		kome::window::WindowTool::refresh();
	}
}

// 検索結果詳細のダイアログのON/OFFを返す
unsigned int  MassBankManager::getSearchResultDialogShowFlag(void) const
{
	return search_result_dialog_show_flag_;
}

const bool MassBankManager::isValidActiveId(const int idx) 
{
	auto it = record_info_map_.find(idx);
	if(it != record_info_map_.end()) {
		activeInfoPtr_ = &it->second;
		return true;
	}
	return false;
}

// 詳細から検索を行う
const bool MassBankManager::searchKeywordFromDetailes(
	const std::string& keyword
){
	std::vector<std::string> keywords;
	show_modal_ids_.clear();
	std::vector<unsigned int> foundIdxs;

	if (stringtoken(keyword.c_str(), " ", keywords) == 0) {
		return false;
	}
	const unsigned int knum = m_results.size();

	for(unsigned int idx = 0; idx < knum; idx++) {
		if(getSearchRecordDetailsFromMassBank(idx, false)) {
			std::string info = getActiveRecordInfo();
			bool foundFg = true;
			for each(auto& key in keywords) {
				if(info.find(key) == std::string::npos) {
					foundFg = false;
					break;
				}
			}
			if (foundFg) {
				show_modal_ids_.push_back(idx);
			}
		}
	}
	// 条件を満たすものが見つかった場合はダイアログを起動する
	if(!show_modal_ids_.empty()) {
		SearchResultListDialog dlg(kome::window::WindowTool::getMainWindow());
		dlg.ShowModal();
	}

	return true;
}

// 検索済みの件数を返す
const bool MassBankManager::getNumberOfSearchedResults(
	std::vector<unsigned int>& ids
){
	if(show_modal_ids_.empty()) return false;
	ids = show_modal_ids_;
	return true;
}

// --- Draw Peaks -------------------------------------------------------------------------------

// Draw Peaks If Checked Over Write Mode
bool MassBankManager::setToDrawToActiveRecord(
	kome::img::Graphics& g,
	kome::objects::Spectrum* spec
) {	
	kome::core::XYData* actXYdata = getActiveRecordPeaks();
	return overlaytool_->setToDrawToActiveRecord(g, spec, actXYdata);
}

void MassBankManager::drawRecordPeaks(kome::img::Graphics& g)
{
	kome::core::XYData* xyData = getActiveRecordPeaks();
	overlaytool_->drawProfilePeaks(g, xyData);
}


// --- MassBank connections XML --------------------------------------------------------------------------

// Batch Search Parameterの為に Vender を設定する (Instrument Name連携含む)
bool MassBankManager::getVenderArraytoEnumItems(
	std::vector<kome::plugin::SettingsValue::EnumItem>*& enumArr
){
	// Batchで呼ばれる命令の先頭部分.
	// 前回Batch処理があれば値を戻しておく

	obo::VenderArr varr;
	obo::getVenderArray(varr);
	const unsigned int vsize = varr.size();
	enumArr->resize(vsize + 1);

	kome::plugin::SettingsValue::EnumItem* em;

	for(unsigned int i = 0; i < vsize; i++) {
		em = &(*enumArr)[i];
		em->name = varr[i].name;
		em->value = varr[i].sname;
		em->desc = varr[i].description;
	}
	em = &(*enumArr)[vsize];
	em->name = "###";
	em->value = "";
	em->desc = "";
	return true;
}

// Batch Search Parameterの為に Instrument を設定する (Vender連携含む)
bool MassBankManager::getInstrumentArraytoEnumItems(
	std::vector<kome::plugin::SettingsValue::EnumItem>*& enumArr
){
	obo::InstArr instArr;
	obo::getInstrumentArray(instArr);

	const unsigned int vsize = instArr.size();
	enumArr->resize(vsize + 1);

	kome::plugin::SettingsValue::EnumItem* em;

	for(unsigned int i = 0; i < vsize; i++) {
		em = &(*enumArr)[i];
		em->name =  instArr[i].name;
		em->value = instArr[i].name;
		em->desc = instArr[i].description;
		em->section = instArr[i].svendername;
	}

	em = &(*enumArr)[vsize];
	em->name = "";
	em->value = "";
	em->desc = "";
	em->section = "###";

	return true;
}


// Batch Search Parameterの為に Licenseを設定する
bool MassBankManager::getLicenseArraytoEnumItems(
	std::vector<kome::plugin::SettingsValue::EnumItem>*& enumArr
){
	std::vector<std::string> cclArr, dscArr;
	CCL::getLisenceElum(cclArr, dscArr);
	unsigned int cnum = cclArr.size();
	enumArr->resize(cnum);

	for(unsigned int i = 0; i < cnum; i++) {
		(*enumArr)[i].name =  cclArr[i];
		(*enumArr)[i].value = dscArr[i];
	}

	return true;
}

// Batch Search Parameterの為に Instrument Typeを設定する
bool MassBankManager::getGettedInstrumentTypeArraytoEnumItems(
	std::vector<kome::plugin::SettingsValue::EnumItem>*& enumArr,
	const bool withFree
){
	if (! checkMySetupInfo(X_INST|X_NET|X_NOTFORCE)) {
		return false;
	}

	std::vector<std::string> insttypeArr;
	instMgr_->getAllInstrumentTypes(insttypeArr);

	const unsigned int vsize =insttypeArr.size();
	enumArr->resize(vsize);
	for(unsigned int i = 0; i < vsize; i++) {
		(*enumArr)[i].name =  insttypeArr[i];
		(*enumArr)[i].value = insttypeArr[i];
	}
	// FreeAreaを作る
	if(withFree) {
		enumArr->resize(vsize + 1);
		(*enumArr)[vsize].name = "###";
		(*enumArr)[vsize].value = "";
	}
	return true;
}

// Batch Search Parameterの為に Appended Values を設定する
bool MassBankManager::getAppendedValuesArraytoEnumItems(
	std::vector<kome::plugin::SettingsValue::EnumItem>*& enumArr
){
	kome::massbank::wizard::RecordElements rcElm;
	std::vector<std::string> names, vals;
	rcElm.getAppendedValuesForBatch(names, vals);

	const unsigned int nsize =names.size();
	enumArr->resize(nsize);
	for(unsigned int i = 0; i < nsize; i++) {
		(*enumArr)[i].name =  names[i];
		(*enumArr)[i].value = vals[i];
	}

	return true;
}

// Batchによる MassBank Recordの作成
bool MassBankManager::createMassBankRecordonBatchMode(
	kome::objects::Parameters* params
){
	if(params == nullptr){ return false;}
	kome::objects::SettingParameterValues* settings = 
		kome::plugin::PluginCallTool::getSettingValues(params);
	
	if(settings == nullptr){ return false;}

	kome::objects::DataSet* dataSet = kome::plugin::PluginCallTool::getDataSet(params);
	if( dataSet == NULL ) { return false; }

	std::string path = kome::plugin::PluginCallTool::getPath(params);
	if(path.empty()) { return false;}

	kome::core::Progress* progress = kome::plugin::PluginCallTool::getProgress(params);
	if( progress == NULL ) {
		progress = &kome::core::Progress::getIgnoringProgress();
	}

	kome::massbank::wizard::RecordElements rcElm;
	rcElm.Init();

	std::string prefix = settings->getParameterValue("prefix");
	prefix = ext::tranceLowerToUpper(prefix);
	
	int basenum = settings->getIntValue("prefixnum", -1);
	if(basenum < 0) {
		basenum = Ident::getPrefixNum(prefix);
	}

	std::string license = settings->getParameterValue("license");

	std::string vendor = settings->getParameterValue("vendor");
	std::string instname = settings->getParameterValue("Instrument");
	std::string insttype = settings->getParameterValue("InstrumentType");

	std::string apValue = settings->getParameterValue("AppendValue");
	std::string title = "hoge";
	
	const unsigned int specnum = dataSet->getNumberOfSpectra();
	kome::objects::Spectrum* spec = dataSet->getSpectrum(0);
	if (spec == nullptr) {
		LOG_WARN(FMT("Batch Export Mode: MassBank: noting Spectrum "));
		return false;
	} //Error!! return false;

	if(CHK::checkBaseNum(prefix, basenum, 1)) {
		rcElm.setAccessions(prefix,basenum);
		// AppendValueを受け取り、分解。適宜セット。
		rcElm.AttachAppendValues(title, license, vendor, instname, insttype, apValue);
		// 不足している値を補完する。
		std::string myacc = rcElm.createAccession(prefix, basenum);
		std::string rstStr = rcElm.makeMassBankRecord(spec, myacc);
		std::string errmsg("");
		// 適合化させてTextとして出力。
		if(rcElm.writeMassBankRecordtoText(rstStr, path, errmsg)) {
			settings->setValue("prefixnum", FMT("%d",(basenum + 1)).c_str());
			kome::plugin::PluginCallTool::setSettingValues(*params, *settings);
			return true;
		}
	}

	LOG_WARN(FMT("Batch Export Mode: MassBank: Can not create MassBankRecords"));
	return false;
}

const std::string MassBankManager::getQueryStringTag(
	const char* specname,
	kome::objects::Peaks* peaks
) {
	if (specname != nullptr && peaks != nullptr) {
		if (peaks->getLength() > 0) {
			return xmlMaker_->getQueryStringsTag(specname, peaks);
		}
	}
	return "";
}

// get instrument types
bool MassBankManager::getInstrumentTypesFromMassBank(
	std::vector<std::string>& instruments
) {
	xmlMaker_->setGetInstrumentTypes();
	std::string tmpFile = onGetMethodFromMassBank("instrument_types");

	if (!tmpFile.empty()) {
		// parse
		MassBankInstrumentsHandler handler;
		handler.parse(tmpFile.c_str());
		handler.getInValues(instruments);
		// delete temporary fileinstMgr_
		removefile(tmpFile.c_str());
	
		if (instruments.empty()) {
			std::string msg = FMT( "Failed to get instrument list." );
			kome::window::WindowTool::showInfo(msg.c_str());
		}
		return true;
	}
	return false;
}

// search similar spectra
bool MassBankManager::searchSimilarSpectra(
	kome::core::XYData& peaks,
	kome::objects::SettingParameterValues* settings
){
	// ひな形を作成
	std::string instArr = instMgr_->getAll_ConvertMassBankSorceStr();
	xmlMaker_->setSimilarSpectraXML(peaks, settings, instArr);
	// XMLを生成して実行
	std::string tmpFile = onGetMethodFromMassBank("similar_spectra");
	clearResults();

	if(!tmpFile.empty()) {
		// 結果をパース
		MassBankSearchResultHandler handler(m_results);
		handler.parse(tmpFile.c_str());
		removefile(tmpFile.c_str());
		if (handler.isOK()) {
			std::sort(m_results.begin(), m_results.end(), lessSearchResult);
			return true;
		}
	}
	return false;
}

// batch search
std::string MassBankManager::batchSearch(
	const std::string& queryTags,
	kome::objects::SettingParameterValues* settings
){
	xmlMaker_->setBatchSearchXML(queryTags, settings);
	// XMLを生成して実行
	std::string tmpFile = onGetMethodFromMassBank("batch_search");
	// 結果をパース
	if(!tmpFile.empty()) {
		MassBankInstrumentsHandler handler;
		std::string jobId = handler.doGetBatchId(tmpFile.c_str());
		return jobId;
	}
	// Error or can not get JobId.
	return "";
}

// get JobStatus
const int MassBankManager::getJobStatus(const char* jobId)
{
	// write request
	xmlMaker_->setGetJobStatus(jobId);
	std::string tmpFile = onGetMethodFromMassBank("job_status");

	if(!tmpFile.empty()) {
		// parse
		MassBankJobStatusHandler handler;
		handler.parse(tmpFile.c_str());
		removefile(tmpFile.c_str());
		return handler.getStatus();
	}
	return -1;
}

// get batch results
const bool MassBankManager::getBatchResults( const char* id ) 
{
	// clear results
	clearResults();

	// write request
//	const char* ggg = "4163922-d98e-428a-bd7a-a0a2a41291f7";
	xmlMaker_->setGetJobResult(id);
	std::string tmpFile = onGetMethodFromMassBank("batch_result");

	// parse
	if(!tmpFile.empty()) {
		MassBankSearchResultHandler handler(m_results);
		handler.parse(tmpFile.c_str());

		// delete temporary file
		removefile(tmpFile.c_str());

		if (handler.isOK()) {
			std::for_each(m_results.begin(), m_results.end(), sortSearchResultQuery);
			m_useQuery = true;
			std::sort(m_results.begin(), m_results.end(), lessSearchResult);
			return true;
		}
	}
	return false;
}

// search peaks
bool MassBankManager::searchPeaks(
	const bool diff,
	kome::objects::SettingParameterValues* settings
) {
	std::string instArr = instMgr_->getAll_ConvertMassBankSorceStr();

	if (!diff) {
		xmlMaker_->setSearchPeaksXML(settings, instArr);
	} else {
		xmlMaker_->setDiffSearchPeaksXML(settings, instArr);
	}
	std::string tmpFile = onGetMethodFromMassBank("peak_search");

	if (!tmpFile.empty()) {
		// parse
		MassBankSearchResultHandler handler(m_results);
		handler.parse(tmpFile.c_str());
		removefile(tmpFile.c_str());

		return true;
	}
	return false;
}

// get record information
bool MassBankManager::getRecordInfo(
	const char* id, 
	std::string& info
) {
	xmlMaker_->setGetRecordInfo(id);
	std::string tmpFile = onGetMethodFromMassBank("record_info");

	// parse
	MassBankRecordHandler handler;
	handler.doGetRecordInfo(tmpFile.c_str(), id, info);
	return true;
}

// get peaks information
bool MassBankManager::getPeakInfo(
	const char* id,
	kome::core::XYData& xyData
) {
	// write request
	xmlMaker_->setGetPeak(id);
	std::string tmpFile = onGetMethodFromMassBank("peak_info");

	// parse
	MassBankPeaksHandler handler;
	handler.doGetXYDatas(tmpFile.c_str(), id, xyData);
	return true;
}	

// XmlMakerからXMLを受け取って NetCtrlにAPIをよんでもらう。
// 結果をテンポラリのテキストに吐き出して、ファイル名を返す。
const std::string MassBankManager::onGetMethodFromMassBank(
	const char* tmpFileName
) {
	if (! onCheckMassBankConnect()) {
		return "";
	}

	// search binding
	std::string method = xmlMaker_->getMethod();
	std::string xmlStr = xmlMaker_->getXML();
	std::vector<char> res;
	int errcode;

	if (!networkCtrl_->execAPI(method, xmlStr, res, errcode)) {
		// get no result (But, connection is Green)
		return "";
	}

	// save to temporary file
	std::string tmpFile = onCreateTempFilePath(tmpFileName);

	//// forDebug
	//std::string xmlStr2("");
	//xmlStr2.resize(res.size());
	//for(unsigned int xx = 0; xx < res.size(); xx++) {
	//	xmlStr2[xx] = res[xx];
	//}

	FILE* fp = fileopen(tmpFile.c_str(),"wb");
	fwrite(&(res[0]), 1, res.size(), fp);
	fflush(fp);
	fclose(fp);

	return tmpFile;
}

// Creation Temp File Path
const std::string MassBankManager::onCreateTempFilePath(
	const char* tmpFileName
) {
	// save to temporary file
	kome::core::MsppManager& msppMgr = kome::core::MsppManager::getInstance();
	const char* dirName = "massbank";
	std::string tmpDir = getpath(msppMgr.getTmpDir(), dirName);
	if(!fileexists( tmpDir.c_str())) {
		makedirectory(tmpDir.c_str());
	}

	std::string fileName = msppMgr.getTmpFileName(tmpFileName, ".xml", dirName);
	return getpath(tmpDir.c_str(), fileName.c_str());
}

// compare search results
bool MassBankManager::lessSearchResult(
	const SearchResult& r0, 
	const SearchResult& r1
) {
	// get scores
	if ( r1.score < 0.0 ) {
		return true; // r0 > r1
	} else if ( r0.score < 0.0 ) {
		return false; // r1 > r0 ( r0 is not less 0 )
	} else {
		return r0.score > r1.score;
	}
}

void MassBankManager::sortSearchResultQuery( SearchResult& r0 )
{
	// Queryの数を確認する。10以下なら10以下にする。
	r0.querycount = r0.queryArr.size();
	unsigned tmpSize = (r0.querycount > 10) ? 10 : r0.queryArr.size();
	queryVector tmp(tmpSize);
	//　降順にソートしてTOP10に切り詰める。（この前のステップでVectorが昇順ソートのInsertであるため）
	std::partial_sort_copy (
		r0.queryArr.begin(), 
		r0.queryArr.end(), 
		tmp.begin(),
		tmp.end(),
		boost::bind(&queryPair::second, _1) > boost::bind(&queryPair::second, _2)
		);

	r0.queryArr = tmp;
	queryVector(r0.queryArr).swap(r0.queryArr);
}

MassBankSearchEngine* MassBankManager::getEngine() 
{
	if (!checkMySetupInfo(X_DB|X_INST) ) {
		return false;
	}

	MassBankSearchEngine* mse = new MassBankSearchEngine();
	return mse;
}

//
// --- Event From Each Dialogs -----------------------------------------------------
//

bool MassBankManager::callCreateMassBankRecord(
	kome::objects::DataGroupNode* group
){
	// BatchSearchの結果があれば反映する

	if (!checkGroupInfo(group)) {
		return false;
	}
	
	// DB, Inst Checker
	if (!checkMySetupInfo(X_DB|X_INST) ) {
		return false;
	}
	
	kome::massbank::createRecordsWizardDialog wizdlg(kome::window::WindowTool::getMainWindow());
	wizdlg.RunWizard(wizdlg.getFirstPage());
	return true;
}

// searches similar spectra
bool MassBankManager::callSimilarSpectraSearch(
	kome::objects::Spectrum* spec
) {
	// check the spectrum
	if( spec == NULL ) {
		LOG_ERROR( FMT( "A spectrum is not active." ));
		return false;
	}

	// DB, Inst, Net Checker
	if ( ! checkMySetupInfo(X_INST|X_NET)) {return false;}

	kome::objects::Peaks* peaks = onGetSpectrumPeak(spec);

	// detect error
	if (peaks == nullptr) { return false;}

	// Open Search dialog (Input Dialog)
	kome::objects::SettingParameterValues settings;
	if (!onAdptSearchDialogCall(0, settings,spec)) { 
		return true; 
	}

	// Call MassBank API
	wxBeginBusyCursor();

	const bool ret = searchSimilarSpectra(*peaks,&settings);
	m_searchTypeName = SPECTRUM_SEARCH_RESULT;
	m_searchTypeName += FMT(" ( %s )",spec->getName());
	wxEndBusyCursor();

	// open Result dialog
	showMassBankSearchResultsPane();

	return ret;
}

// search peaks
bool MassBankManager::callPeakSearch(
	const bool diff
){
	// DB, Inst, Net Checker
	if ( ! checkMySetupInfo(X_INST|X_NET)) { 
		return false;
	}

	kome::objects::SettingParameterValues settings;

	// pattern Select.
	if (!onAdptSearchDialogCall((diff ? 4 : 2), settings)) { 
		return true; 
	}

	clearResults();

	wxBeginBusyCursor();

	const bool ret = searchPeaks(diff, &settings);
	m_searchTypeName = diff ? PEAK_DIFF_SEARCH_RESULT : PEAK_SEARCH_RESULT;

	wxEndBusyCursor();

	// open Pane
	showMassBankSearchResultsPane();

	return ret;
}

// SearchDialogのアダプタ。番号で分岐して呼び出す。ID_OKならtrueを返す
const bool MassBankManager::onAdptSearchDialogCall(
	int Fg, 
	kome::objects::SettingParameterValues& settings,
	kome::objects::Spectrum* spec
){
	std::string dname;
	SearchDialog::SearchType stype;

	if (Fg == 0) {
		dname = "MassBank Spectrum Search";
		stype = SearchDialog::TYPE_SPECTRUM;
	} else if(Fg == 2) {
		dname = "MassBank Peak Search";
		stype = SearchDialog::TYPE_PEAKS;
	}else if(Fg == 4) {
		dname = "MassBank Peak Difference Search";
		stype = SearchDialog::TYPE_DIFF;
	} else {
		return false;
	}

	bool hasEx = networkCtrl_->hasEx();

	SearchDialog dlg(
		kome::window::WindowTool::getMainWindow(),
		stype, 
		settings, 
		dname.c_str(),
		spec);
	
	dlg.SetPosition(getDialogStandardPt(&m_searchDlgPos, 50));

	bool ret = (dlg.ShowModal() == wxID_OK);
	return ret;
}

// get DialogSet Position
//kome::core::Point< int >
wxPoint MassBankManager::getDialogStandardPt(
	kome::core::Point< int >* tgtPos, 
	const int& mgSize
) {
	int px = tgtPos->px;
	int py = tgtPos->py;

	if( px < 0 || py < 0 ) {
		px = kome::window::WindowTool::getMainWindow()->GetPosition().x + mgSize;
		py = std::max(0, kome::window::WindowTool::getMainWindow()->GetPosition().y - mgSize);
	}

	return wxPoint( px, py );
}

// ネットワーク接続時等、状態のチェックを行う。
// Flagsは
//		X_DB (DBを使う) X_INST (Instrument情報を使う)
//		X_NET(NetWorkを使う) X_NOTFORCE(NetWorkに未接続では使わない)
const bool MassBankManager::checkMySetupInfo(
	unsigned int flags
) {	
	if (flags & X_NET) {
		// NetWorkを使用するので状況を確認
		bool usenet = onCheckMassBankConnect();
		if ((!usenet) && (!(flags & X_NOTFORCE))) {
			// NetWorkに繋がっていない。
			// それでもネット処理をするか否かを問う。
			if(!callWarningDialog()){ 
				return false;
			}
		}
	}
	if ((flags & X_INST)
		&& ! instMgr_->checkHavingInstrumentTypes(m_bNetInfoChanged)) { 
			return false;
	}
	// all Clear
	return true;
}

// グループ情報の有無を獲得
const bool MassBankManager::checkGroupInfo(
	kome::objects::DataGroupNode* group,
	bool popWindow
){
	if(group == NULL) {
		LOG_ERROR(FMT("A group is not active."));
		return false;
	}
		
	if (group->getNumberOfSpectra() > 0) {
		return true;
	} else {
		std::string emsg("This function cannot be used because there are no spectra in the active sample. Open a data file containing spectra.");
		if (popWindow) {
			kome::window::WindowTool::showError(emsg.c_str());
		} else {
			LOG_ERROR_CODE(emsg, ERR_NULL_POINTER);
		}
	}
	return false;
}

kome::objects::Peaks* MassBankManager::onGetSpectrumPeak(
	kome::objects::Spectrum* spec
){
	kome::objects::Peaks* peaks = getHasPeaks(spec);
	// ピークが無いので再度取得し直し
	if (peaks == nullptr) {
		std::string ynmsg("There are no peaks.\nDo you want to perform peak detection now?");
		if (kome::window::WindowTool::showYesNo(ynmsg.c_str())) {
			kome::objects::DataSet spectra;
			spectra.addSpectrum(spec);
			kome::plugin::PluginCallTool::detectSpecPeaks(spectra, NULL);
			peaks = getHasPeaks(spec);
		}
	}
	return peaks;
}

kome::objects::Peaks* MassBankManager::getHasPeaks(
	kome::objects::Spectrum* spec
){
	// manager
	kome::objects::PeaksManager& pkMgr = kome::objects::PeaksManager::getInstance();
	if (!pkMgr.hasPeaks(spec)) {
		LOG_ERROR(FMT("Peak detection has not been done yet."));
		return NULL;
	}

	kome::objects::Peaks* peaks = pkMgr.getPeaks( spec );
	if (peaks->getLength() > 0) {
		return peaks;
	}
	LOG_ERROR(FMT("There are no peaks."));
	return NULL;
}

// 現在の情報に変更が無ければ現在の、
// 変更があれば更新後のネットワークのT/Fを返す。
const bool MassBankManager::onCheckMassBankConnect() 
{
	bool ret = networkCtrl_->checkMyConnect();
	if (ret) {
		xmlMaker_->setHasExFg(networkCtrl_->hasEx());
	}
	return ret;
}

// ----------------------------------------------------------------------------------------------------------
// Control Search Results Pane 
// ----------------------------------------------------------------------------------------------------------
// Create Search Result Pane
void MassBankManager::createMassBankSearchResultsPane() 
{	
	// create
	m_resultDlg = new SearchResultListPanel(kome::window::WindowTool::getMainWindow());
	
	kome::window::MainFrame* mainFrame = onGetMyFrame();

	if( mainFrame != NULL ) {
		mainFrame->appendPane(m_resultDlg, m_searchTypeName.c_str(), kome::window::WindowTool::DOCK_ALIGN_BOTTOM );
	}
}

// Force Show Search Result Pane
void MassBankManager::showMassBankSearchResultsPane() 
{
	if (m_resultDlg == NULL) {
		createMassBankSearchResultsPane();
	} else {
		if (!isVisibileMassBankSearchResultsPane()) {
			toggleMassBankSearchResultsPaneVisibility();
		}
	}
	m_resultDlg->upDateMe();
	m_resultDlg->Refresh();

	if(isVisibileMassBankRecordPane("not selected details")) {
		m_rsPeaksDlg->upDateInfo();
		m_rsPeaksDlg->Update();
	}
}


kome::window::MainFrame* MassBankManager::onGetMyFrame()
{
	wxWindow* wnd = kome::window::WindowTool::getMainWindow();
	return dynamic_cast< kome::window::MainFrame* >( wnd );
}

// toggle the visibility of the spectra view
void MassBankManager::toggleMassBankSearchResultsPaneVisibility() 
{
	// check the member
	if( m_resultDlg == NULL ) {
		createMassBankSearchResultsPane();
		return;
	}
	// frame
	kome::window::MainFrame* mainFrame = onGetMyFrame();
	if( mainFrame == NULL ) {
		return;
	}

	// toggle visibility
	mainFrame->toggleVisibleBar( m_resultDlg );
}

// check the visibility of the spectra view
bool MassBankManager::isVisibileMassBankSearchResultsPane() 
{
	// frame
	kome::window::MainFrame* mainFrame = onGetMyFrame();
	if( mainFrame == NULL ) {
		return false;
	}
	mainFrame->setPaneTitle( m_resultDlg, m_searchTypeName.c_str() );
	// check
	return mainFrame->isVisibleBar( m_resultDlg );
}

// release view
void MassBankManager::releaseMassBankSearchResultsPane() 
{
	// set status
	kome::objects::StatusManager& statusMgr = kome::objects::StatusManager::getInstance();

	if( m_resultDlg != NULL ) {
		statusMgr.setValue( RESULTLISTPANE, "off" );
	}

	// release
	m_resultDlg = NULL;
}

// ----------------------------------------------------------------------------------------------------------
// Control Record Pane 
// ----------------------------------------------------------------------------------------------------------
// Create Record Pane
void MassBankManager::createMassBankRecordPane(
	const char* titleStr
) {
	kome::window::MainFrame* mainFrame = onGetMyFrame();

	// create
	m_rsPeaksDlg = new RecordPeaksDialog(kome::window::WindowTool::getMainWindow());

	if( mainFrame != NULL ) {
		mainFrame->appendPane( m_rsPeaksDlg, titleStr, kome::window::WindowTool::DOCK_ALIGN_BOTTOM );
	}
}

// show Record Pane
void MassBankManager::showMassBankRecordPane(
	const char* titleStr
) {
	if ( m_rsPeaksDlg == NULL ) {
		createMassBankRecordPane(titleStr);
	} else {
		if ( ! isVisibileMassBankRecordPane (titleStr) ) {
			toggleMassBankRecordPaneVisibility();
		}
	}
	m_rsPeaksDlg->upDateInfo();
	m_rsPeaksDlg->Update();
}

// toggle Record Pane
void MassBankManager::toggleMassBankRecordPaneVisibility() 
{
	if( m_rsPeaksDlg == NULL ) {
		createMassBankRecordPane();
		return;
	}
	// frame
	kome::window::MainFrame* mainFrame = onGetMyFrame();
	if( mainFrame == NULL ) {
		return;
	}

	// toggle visibility
	mainFrame->toggleVisibleBar( m_rsPeaksDlg );
}

// isVisibile Record Pane
bool MassBankManager::isVisibileMassBankRecordPane(const char* titleStr) {

	if ( m_rsPeaksDlg == NULL ) {
		return false;
	}

	kome::window::MainFrame* mainFrame = onGetMyFrame();
	if( mainFrame == NULL ) {
		return false;
	}
	if ( titleStr != NULL ) {
		mainFrame->setPaneTitle( m_rsPeaksDlg, titleStr );
	}

	// check
	return mainFrame->isVisibleBar( m_rsPeaksDlg );
}

void MassBankManager::releaseMassBankRecordPane(){}


const bool MassBankManager::callWarningDialog()
{
	LOG_WARN_CODE(
		FMT(" - Connection Error - \n"
"This method cannot be used because the MassBank server cannot be contacted.\n"
"Check the MassBank URL "), ERR_OTHER);
	return false;
}

// Batch Search時にDataBaseにロックがかかってCommit出来ない為、
// 別のフェーズでの保存を考えたが、
// 完全に別プロセス扱いで呼ばれ、DBにロックがかかっているので
// 何れにしてもかなりの工夫が必要である・・。どうしよう？

// get instance
MassBankManager& MassBankManager::getInstance() {
	// create object (This is the only object.)
	static MassBankManager mgr;

	return mgr;
}

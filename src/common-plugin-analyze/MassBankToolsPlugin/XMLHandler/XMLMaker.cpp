/**
 * @file XMLMaker
 * @brief MassBankのXMLを生成する
 *
 * @author M.Fukuda
 * @date 2013.04.24
 *
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#include "../stdafx.h"
#include "XMLMaker.h"
#include "constXMLTagName.h"
#include "settingParamsKeywords.h"

using namespace kome::massbank;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


namespace XMLMakerMspp {
enum ETYPE {
	STR = 0,
	INT = -1,
	DBL = 1,
};
}

typedef XMLMakerMspp::ETYPE ETYPE;

// constructor
XMLMaker::XMLMaker() 
	: settings_(nullptr)
	, xml_("")
	, method_("")
	, hasExFg_(true)
{
}

// destructor
XMLMaker::~XMLMaker()
{}

// Get Instrument Types
void XMLMaker::setGetInstrumentTypes() 
{
	method_ = "getInstrumentTypes";
	xml_ = onSetMSBKFormat();
}

// Similar Spectra XML
void XMLMaker::setSimilarSpectraXML(
	kome::core::XYData& peaks,
	kome::objects::SettingParameterValues* settings,
	const std::string& insttypesTag
){
	std::string creq(""),tmpReq("");
	settings_ = settings;
	//Keep Omitter
	for( unsigned int i = 0; i < peaks.getLength(); i++ ) {
		creq += onAppendFixedDouble(XML::TAG::MZS, peaks.getX(i));
		tmpReq += onAppendFixedDouble(XML::TAG::INTENSITY, peaks.getY(i));
	}
	creq.append(tmpReq);

	creq += onAppendFixedTypes(ETYPE::STR, XML::TAG::UNIT,  CPRM::SPEC::UNIT);
	creq += onAppendFixedTypes(ETYPE::DBL, XML::TAG::TOL,   CPRM::SPEC::TOL);
	creq += onAppendFixedTypes(ETYPE::INT, XML::TAG::CUTOFF, CPRM::SPEC::CUTOFF);

	if(hasExFg_) {
		creq += onAppendFixedTypes(ETYPE::DBL, XML::TAG::PRECURSOR, CPRM::SPEC::PRECURSOR);
		creq += onAppendFixedTypes(ETYPE::STR, XML::TAG::MSTYPE, CPRM::SPEC::MSTYPE);
	}

	creq += insttypesTag;
	creq += onAppendFixedTypes(ETYPE::STR, XML::TAG::ION_MODE, CPRM::SPEC::IONMODE);
	creq += onAppendFixedTypes(ETYPE::INT, XML::TAG::MAX_NUM,  CPRM::SPEC::MAXNUM);

	if (hasExFg_) {
		method_ = "searchSpectrumEx";
//		creq = onMakeTestXML_searchSpectrum(true);
	} else {
		method_ = "searchSpectrum";
//		creq = onMakeTestXML_searchSpectrum(false);
	}

	xml_ = onSetMSBKFormat(creq);
}

// batch search XML
void XMLMaker::setBatchSearchXML(
	std::vector<kome::objects::Spectrum*> spectra,
	kome::objects::SettingParameterValues* settings,
	const std::string& insttypesTag
) {
	settings_ = settings;
	// managers
	std::string creq("");
	kome::objects::PeaksManager& pkMgr = kome::objects::PeaksManager::getInstance();

	// write request
	creq = "<type>1</type>\r\n";
	creq += onAppendFixedType(XML::TAG::MAIL, 0);

	std::for_each(spectra.begin(),spectra.end(), [&](kome::objects::Spectrum* spec){
		kome::objects::Peaks* peaks = pkMgr.getPeaks(spec);

		if(peaks->getLength() > 0) {
			std::string query = FMT("<queryStrings>Name: %s;", spec->getName());
			for(unsigned int j = 0; j < peaks->getLength(); j++) {
				const double x = peaks->getX( j );
				const double y = peaks->getY( j );
				if(y > 0.0) {
					query += FMT(" %.4f,%.1f;", x, y);
				}				
			}
			query += "</queryStrings>\r\n";
			creq += query;
		}
	});
	creq += insttypesTag;
	creq += onAppendFixedType(XML::TAG::ION_MODE, 0);

	// Call MassBank
	method_ = "execBatchJob";
	xml_ = onSetMSBKFormat(creq);
}

void XMLMaker::setBatchSearchXML(
	const std::string& specQuerys,
	kome::objects::SettingParameterValues* settings
) {
	settings_ = settings;
	// managers
	std::string creq("");
	kome::objects::PeaksManager& pkMgr = kome::objects::PeaksManager::getInstance();

	// write request
	creq = "<type>1</type>\r\n";
	creq += onAppendFixedType(XML::TAG::MAIL, 0);
	creq += specQuerys;
	creq += onGetInstrumentTypesTag();
	creq += onAppendFixedType(XML::TAG::ION_MODE, 0);

	// Call MassBank
	method_ = "execBatchJob";
	xml_ = onSetMSBKFormat(creq);
}

// Batchの為の QueryStringブロックを作って返す。
const std::string XMLMaker::getQueryStringsTag(
	const char* scanname,
	kome::objects::Peaks* peaks
){
	if(peaks->getLength() > 0) {
		std::string query = FMT("<queryStrings>Name: %s;", scanname);
		for(unsigned int j = 0; j < peaks->getLength(); j++) {
			const double x = peaks->getX(j);
			const double y = peaks->getY(j);
			if(y > 0.0) {
				query += FMT(" %.4f,%.1f;", x, y);
			}				
		}
		query += "</queryStrings>\r\n";
		return query;
	} else {
		return "";
	}
}

const std::string XMLMaker::onGetInstrumentTypesTag(
) {
	const char* insttypestr = settings_->getParameterValue("insttype");
	if (insttypestr == nullptr) {
		return "";
	}
	std::vector<std::string> insttypes;
	kome::plugin::SettingsValue::separateListValue(insttypestr, insttypes);
	return onGetInstrumentTypesTag(insttypes);
}

const std::string XMLMaker::onGetInstrumentTypesTag(
	const std::vector<std::string>& instrumenttypes
) {
	std::string insttypes("");
	for each(auto& x in instrumenttypes) {
		insttypes += onAppendFixedTypes(ETYPE::STR, XML::TAG::INSTTYPE, x, true);
	}
	return insttypes;
}

// search peaks
void XMLMaker::setSearchPeaksXML(
	kome::objects::SettingParameterValues* settings,
	const std::string& tgtinstruments
) {
	settings_ = settings;
	std::string creq = onSetMZSArray(CPRM::SRCH::MZLIST);
	creq += onAppendFixedTypes(ETYPE::INT, XML::TAG::RELINT, CPRM::SRCH::INTNSTY);
	creq += onAppendFixedTypes(ETYPE::DBL, XML::TAG::TOL, CPRM::SRCH::TOL);
	
	if(hasExFg_) {
		creq += onAppendFixedTypes(ETYPE::STR, XML::TAG::MSTYPE, CPRM::SRCH::MSTYPE);
	}

	creq += tgtinstruments;
	creq += onAppendFixedTypes(ETYPE::STR, XML::TAG::ION_MODE, CPRM::SRCH::IONMODE);
	creq += onAppendFixedTypes(ETYPE::INT, XML::TAG::MAX_NUM,  CPRM::SRCH::MAXNUM);

	// For Setting MassBank API XML Tags(Header etc..)
	if(hasExFg_) {
		method_ = "searchPeakEx";
	} else {
		method_ = "searchPeak";
	}
	xml_ = onSetMSBKFormat(creq);
}

// Diff Search
void XMLMaker::setDiffSearchPeaksXML(
	kome::objects::SettingParameterValues* settings,
	const std::string& tgtinstruments
){
	settings_ = settings;
	std::string creq = onSetMZSArray(CPRM::DIFF::VALLIST);
	creq += onAppendFixedTypes(ETYPE::INT, XML::TAG::RELINT, CPRM::DIFF::INTNSTY);
	creq += onAppendFixedTypes(ETYPE::DBL, XML::TAG::TOL, CPRM::DIFF::TOL);

	if(hasExFg_) {
		creq += onAppendFixedTypes(ETYPE::STR, XML::TAG::MSTYPE, CPRM::DIFF::MSTYPE);
	}

	creq += tgtinstruments;
	creq += onAppendFixedTypes(ETYPE::STR, XML::TAG::ION_MODE, CPRM::DIFF::IONMODE);
	creq += onAppendFixedTypes(ETYPE::INT, XML::TAG::MAX_NUM,  CPRM::DIFF::MAXNUM);
	// Call Mass Bank

	if(hasExFg_) {
		method_ = "searchPeakDiffEx";
		//creq = onMakeTestXML_searchPeakDiff(true);
	} else {
		method_ = "searchPeakDiff";
		//creq = onMakeTestXML_searchPeakDiff(false);
	}
	xml_ = onSetMSBKFormat(creq);
}

void XMLMaker::setGetJobStatus(const std::string& jobId) 
{
	this->onSetJobStatus("getJobStatus", jobId.c_str());
}

void XMLMaker::setGetJobResult(const char* jobId) 
{
	this->onSetJobStatus("getJobResult", jobId);
}

void XMLMaker::setGetRecordInfo(const char* jobId) 
{
	this->onSetJobStatus("getRecordInfo", jobId);
}

void XMLMaker::setGetPeak(const char* jobId)
{
	this->onSetJobStatus("getPeak", jobId);
}

void XMLMaker::onSetJobStatus(
	const std::string& method, 
	const char* jobId
) {
	std::string creq = onSetJB_ID(jobId);
	method_ = method;
	xml_ = onSetMSBKFormat(creq);
}

const std::string XMLMaker::onSetJB_ID(const char* jobId)
{
	return FMT( "<jobId>%s</jobId>\r\n", jobId);
}

// XML出力パターンが固定なので、入れ子にして省略。
const std::string XMLMaker::onAppendFixedType(
	const std::string& xtgt,
	const int sType
) {
	const char* tgt = xtgt.c_str();
	if (sType == 0) {
		const char* val = settings_->getParameterValue(tgt);
		return FMT("<%s>%s</%s>\r\n",tgt, val, tgt);
	} else if( sType == -1){
		const int nInt = settings_->getIntValue(tgt, 0);
		return FMT("<%s>%d</%s>\r\n",tgt, nInt, tgt);
	} else {
		const double nDbl = settings_->getDoubleValue(tgt,0.3);
		return FMT("<%s>%.3f</%s>\r\n",tgt, nDbl, tgt);
	}
}

const std::string XMLMaker::onAppendFixedTypes(
	const int sType,
	const std::string& xtag,
	const std::string& tgtStr,
	const bool noParamFg
) {
	const char* tag = xtag.c_str();
	if (noParamFg) {
		return FMT("<%s>%s</%s>\r\n",tag, tgtStr.c_str(), tag);
	}

	const char* tgt = tgtStr.c_str();
	
	if (sType == ETYPE::STR) {
		const char* val = settings_->getParameterValue(tgt);
		return FMT("<%s>%s</%s>\r\n",tag, val, tag);
	} else if(sType == ETYPE::INT){
		const int nInt = settings_->getIntValue(tgt, 0);
		return FMT("<%s>%d</%s>\r\n",tag, nInt, tag);
	} else {
		const double nDbl = settings_->getDoubleValue(tgt,0.3);
		return FMT("<%s>%.3f</%s>\r\n",tag, nDbl, tag);
	}
}

//Doubleの値を <tag>Val</tag>として出力する
const std::string XMLMaker::onAppendFixedDouble(
	const std::string& xtag,
	const double value
){
	const char* tag = xtag.c_str();
	return FMT("<%s>%.3f</%s>\r\n",tag, value, tag);
}

// Create MZS Tags
const std::string XMLMaker::onSetMZSArray(const std::string& listKey) 
{
	// results
	const char* mzs = settings_->getParameterValue(listKey.c_str());

	std::vector<std::string> mzStrArray;
	stringtoken(mzs, ", ", mzStrArray);
	std::string creq("");

	for( unsigned int i = 0; i < mzStrArray.size(); i++ ) {
		double mz = todouble(mzStrArray[i].c_str(), -1.0);
		if( mz >= 0.0 ) {
			creq += FMT("<mzs>%.3f</mzs>\r\n", mz);
		}
	}
	return creq;
}

//Create MassBankTag (Brank Mode)
const std::string XMLMaker::onSetMSBKFormat()
{
	return this->onSetMSBKFormat("");
}

// 共通のヘッダ、フッダ + (中身)を作成する
const std::string XMLMaker::onSetMSBKFormat(
	const std::string& contentReq)
{
	std::string req;
	req = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n";
	req += "<soapenv:Envelope xmlns:soapenv=\"http://schemas.xmlsoap.org/soap/envelope/\"\r\n";
	req += "  xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\"\r\n";
	req += "  xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\">\r\n";
	req += "  <soapenv:Body>\r\n";
	req += FMT("<ns:%s\r\n", method_.c_str());
	req += "soapenv:encodingStyle=\"http://www.w3.org/2001/12/soap-encoding\"\r\n";
	req += "xmlns:ns=\"http://api.massbank\">\r\n";
	
	if (!contentReq.empty()) {
		req.append(contentReq);
	}
	req += FMT("</ns:%s>\r\n", method_.c_str());
	req += "  </soapenv:Body>\r\n";
	req += "</soapenv:Envelope>\r\n";
	return req;
}

// Test用　XML作成。(search Spectrum) 2013/7/31 時点の例と合致。
const std::string XMLMaker::onMakeTestXML_searchSpectrum(const bool exFg)
{
	// タグ順序を守る必要有。
	// 例えば,InstrumentTypesをprecursorより先行させると、
	// Precursorの値がInstrumentTypesが指す値と入れ替わる模様。
	std::string creq("");
	creq += onAppendFixedDouble(XML::TAG::MZS, 73.096);
	creq += onAppendFixedDouble(XML::TAG::MZS, 89.086);
	creq += onAppendFixedDouble(XML::TAG::INTENSITY, 300);
	creq += onAppendFixedDouble(XML::TAG::INTENSITY, 300);
	creq += onAppendFixedTypes(ETYPE::STR, XML::TAG::UNIT, "", true);
	creq += onAppendFixedTypes(ETYPE::DBL, XML::TAG::TOL, "", true);
	creq += onAppendFixedTypes(ETYPE::INT, XML::TAG::CUTOFF,   "", true);
	if(exFg) {
		creq += onAppendFixedTypes(ETYPE::DBL, XML::TAG::PRECURSOR, "", true);
		creq += onAppendFixedTypes(ETYPE::STR, XML::TAG::MSTYPE, "MS2", true);
	}
	creq += onAppendFixedTypes(ETYPE::STR, XML::TAG::INSTTYPE, "all", true);
	creq += onAppendFixedTypes(ETYPE::STR, XML::TAG::ION_MODE, "Positive", true);
	creq += onAppendFixedTypes(ETYPE::INT, XML::TAG::MAX_NUM,  "5", true);

	return creq;
}

// Test用　XML作成。(search Peak) 2013/7/31 時点の例と合致。
const std::string XMLMaker::onMakeTestXML_searchPeak(const bool exFg)
{	
	std::string creq("");
	creq += onAppendFixedDouble(XML::TAG::MZS, 80);
	creq += onAppendFixedDouble(XML::TAG::MZS, 91);
	creq += onAppendFixedTypes(ETYPE::INT, XML::TAG::RELINT, "200", true);
	creq += onAppendFixedTypes(ETYPE::DBL, XML::TAG::TOL, "0.3", true);
	if(exFg) {
		creq += onAppendFixedTypes(ETYPE::STR, XML::TAG::MSTYPE, "MS2", true);
	}
	creq += onAppendFixedTypes(ETYPE::STR, XML::TAG::INSTTYPE, "all", true);
	creq += onAppendFixedTypes(ETYPE::STR, XML::TAG::ION_MODE, "Both", true);
	creq += onAppendFixedTypes(ETYPE::INT, XML::TAG::MAX_NUM, "0", true);

	return creq;
}


// Test用　XML作成。(search PeakDiff) 2013/7/31 時点の例と合致。
const std::string XMLMaker::onMakeTestXML_searchPeakDiff(const bool exFg)
{
	std::string creq("");
	creq += onAppendFixedDouble(XML::TAG::MZS, 101);
	creq += onAppendFixedTypes(ETYPE::INT, XML::TAG::RELINT, "100", true);
	creq += onAppendFixedTypes(ETYPE::DBL, XML::TAG::TOL, "0.3", true);
	if(exFg) {
		creq += onAppendFixedTypes(ETYPE::STR, XML::TAG::MSTYPE, "MS2", true);
	}
	creq += onAppendFixedTypes(ETYPE::STR, XML::TAG::INSTTYPE, "all", true);
	creq += onAppendFixedTypes(ETYPE::STR, XML::TAG::ION_MODE, "Negative", true);
	creq += onAppendFixedTypes(ETYPE::INT, XML::TAG::MAX_NUM, "10", true);

	return creq;

}

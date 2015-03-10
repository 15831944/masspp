/**
 * @file RecordElements
 * @brief Record Elements Data Keeper
 *
 * @author M.Fukuda
 * @date 2013.03.08
 *
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#pragma once
#include <string>
#include <vector>
#include <map>

// -- Forward declaration --
namespace kome {
	namespace objects {
		class Spectrum;
		class AveragedSpectrum;
		class SettingParameterValues;
		class Peaks;
		class XYDataOperation;
	}
	namespace plugin {
		class PluginFunctionItem;
	}
}

//---------------------------------------------------------
//
//  main functions
//
//---------------------------------------------------------
namespace kome {
	namespace massbank {
		namespace wizard {

// -- Forward declaration --
// inner Element srcs
struct subTags;
struct tagInfo;
struct subtagInfo;

// Interopのショートカット
typedef std::vector<subTags>::iterator SubItr;
typedef std::vector<tagInfo>::iterator TGInItr;
typedef std::vector<subtagInfo>::iterator STGInItr;

// Hold by Shared Pointer.
class XPeakDetector;
class AnnotationElement;

//-- Main------
class RecordElements {

public:
	// constrcutor
	RecordElements();
	// destrcutor
	~RecordElements();

private:

// keep values
// save folder
	std::string folderpath_;
// target spectrum
	std::vector<kome::objects::Spectrum*> specArr_;
	std::vector<std::vector<kome::objects::Spectrum*>> avespecElmArr_;
	std::vector<kome::objects::AveragedSpectrum*> avespecArr_;
	bool procFg_;

// Acceccion Element Prefix + num
	std::string prefix_;
	int basenum_;

// Peak Detection & Filter Info
	std::string peakDetectStream_;
	std::shared_ptr<XPeakDetector> pkPtr_;

	// Annotation Info Objects
	std::shared_ptr<AnnotationElement> anoPtr_;

// Hold Titles
	std::string Titles_;

// Elements
	std::vector<std::string> topTags_;
	std::vector<tagInfo> tagInfoArr_;
	std::vector<subtagInfo> subtagInfoArr_;

	// Extends(MS Type, Polariryだけ処理が特殊なので捌）
	tagInfo* tiMSType_;
	tagInfo* tiPolarity_;

private:
// SubTags(SettedElements);
	std::vector<std::string> subSpecArr_;
	std::vector<std::string> subSpecNGArr_;
	std::vector<std::string> subChromArr_;
	std::vector<std::string> subChromNGArr_;
	std::vector<std::string> subIonArr_;
	std::vector<std::string> subIonNGArr_;
	std::vector<std::string> IonArr_;

public:
	void Init();

	const std::string createAccession(
		const std::string& prefix, 
		int basenum);

	void getAppendedValuesForBatch(
		std::vector<std::string>& names,
		std::vector<std::string>& values);


	// Setter.....

	// Spectraをセットする
	void setSpectra(
		const std::vector<kome::objects::Spectrum*>& specArr
	) {
			specArr_ = specArr;
	}

	// Spectraを返す
	void getSpectra(
		std::vector<kome::objects::Spectrum*>& specArr
	) {
		specArr = specArr_;
	}
	
	// AverageSpectra(の要素)をセットする
	const bool setAveSpectra(const std::vector<std::vector<kome::objects::Spectrum*>>& aveSpecArr);

	//AverageSpectra(の要素)を返す
	std::vector<kome::objects::AveragedSpectrum*>& getAveSpectra() {return avespecArr_;}

	//------------------------------------

	// UseProcFlagのチェック値を受け取る
	void setUseProcFlg(const bool procFg) {procFg_ = procFg;}
	void setProcStream(const std::string&);
	void setOPT(std::vector<kome::objects::XYDataOperation*>& opts);

	// Folder の値を受け取る（これ以前でチェック済み）
	void setFolder(const std::string& folderPath) {folderpath_ = folderPath;}

	// Peak Detector＋Paramの情報を受け取る
	void setPkDtStr(const std::string& pkstr){peakDetectStream_ = pkstr;}

	unsigned int getSpecArrSize() const {return specArr_.size();}

	const std::string getPkDtStr(){return peakDetectStream_;}

	// Setter (pane2)
	void setAccessions(
		const std::string& prefix, 
		int basenum
		);

	// Setter (pane3)
	void setPeakDetector(
		const std::string& detectStr, 
		kome::objects::SettingParameterValues* param = nullptr
		);
	
	void setPeakFilter(const char* peakFilter);
	
	// Rec Tag(pane4)----
	void setRecordInfo(
		const std::string& _title,
		const std::string& _date,
		const std::string& _authors,
		const std::string& _license,
		const std::string& _copyright,
		const std::string& _publications
		);

	void setComments(const std::string& _comments);

	// CH$ (pane5)
	void setCHName(const std::string& _name);

	const bool setCategory(
		const std::string& _prodcuct, 
		const std::string& cname
		);

	void setLinks(const std::string& _link);

	void setChemcalStructures(
		const std::string& _formula, 
		const std::string& _exactmass, 
		const std::string& _similes, 
		const std::string& _inchl
		);

	// SP$ (Pane6)
	void setSPStatus(
		const std::string& _scentName, 
		const std::string& spDb, 
		const std::string& spAcc, 
		const std::string& sample
		);

	// AC$ (Pane7)
	const bool setACInstruments(
		const std::string& _vendor, 
		const std::string& _instName, 
		const std::string& _instType
		);

	// AC$ (Pane8)
	void setMSStage_Polarity(
		const std::string& _msstage,
		const std::string& _polarity
		);

	void setSubSpec(const std::string& valStr);

	// AC$ (Pane9)
	void setSubChrom(const std::string& valStr);

	// MS$ (Pane10)
	void setIonType(const std::string& val);
	void setPrcType(const std::string& val);
	void setSubFcIon(const std::string& valStr);

	// MS$ (Pane11)
	void setDataProc(const std::string& vals);

	void setSoft_Peaks(
		const std::string& soft, 
		const std::string& peakStr
		);

	// PK$ (Pane12)
	const bool setAnnoDatas(const std::string& val);

	// getter (pane2)
	// getter (pane10)
	
	//------------------------------------------------
	
	//------------------------------------------------

	void getACSpecSubTags (
		std::vector<std::string>& vals, 
		std::vector<std::string>& ngVals);

	void getACChromSubTags(
		std::vector<std::string>& vals, 
		std::vector<std::string>& ngVals);

	void getMSIonSubTags  (
		std::vector<std::string>& vals, 
		std::vector<std::string>& ngVals);

	void getMSIonElements (std::vector<std::string>& vals);

	//************************************************
	// Final Panelでレコードの確認時に使用。FileIOのチェックはしない。
	const std::string getMassBankPreTexts(kome::objects::Spectrum*);

	const bool getMassBankPreText(
		kome::objects::Spectrum* src,
		kome::objects::Spectrum*& dst,
		std::string& refRecordStr
	);


	// Wizard経由のMassBank Recordを作成する
	const bool makeMassBankRecords(std::string& errmsg);

	const std::string makeMassBankRecord(
		kome::objects::Spectrum* spec,
		std::string& myAccesstion);

	const bool writeMassBankRecordtoText(
		const std::string& recordDataStr,
		const std::string& writeFilePath,
		std::string& refErrMsg);

	const std::string getFirstAccession();

	void AttachAppendValues(
		const std::string& title,
		const std::string& license,
		const std::string& vendor,
		const std::string& instname,
		const std::string& insttype,
		const std::string& apvalue);

private:
	// set upper
	void onInit();

	const std::string onCreationAccession(
		const std::string& prefix,
		const unsigned int num);

	void onSetUpACSpecSubTags(
		std::vector<std::string>& okArr,
		std::vector<std::string>& ngArr);

	void onSetUpACChromSubTags(
		std::vector<std::string>& okArr,
		std::vector<std::string>& ngArr);

	void onSetUpMSIonSubTags(
		std::vector<std::string>& okArr,
		std::vector<std::string>& ngArr);

	void onSetUpMSIonElems(void);

	// Tags Elements
	void addTagInfo(
		const std::string& _toptag, 
		const std::string& _tag, 
		const bool _multi = false
		);

	void addSubTagInfo(
		const std::string& _toptag, 
		const std::string& _subtop
		);

	const bool setTagInfoVal(
		const std::string& _tag, 
		const std::string& val, 
		const std::string& defval
		);

	const bool setTagInfoVal(
		const std::string& _toptag,
		const std::string& _tag, 
		const std::string& val, 
		const std::string& defval
		);

	void addSubTagElem(
		const std::string& _toptag, 
		const std::string& _subtop, 
		const std::string& _tagname, 
		const std::string& _val, 
		const bool ishold = false
		);

	// SearchItems
	TGInItr onSearchTagInfoVal(
		const std::string& _tag, 
		const std::string& _top = "##"
		); 

	STGInItr onSearchSubTagInfoVal(
		const std::string& _top, 
		const std::string& _sub
		);

	SubItr onSearchSubTag(
		STGInItr& subtaginfoitr, 
		const std::string& _tag
		); 


	// Value Splitter ----------------------------------------
	void onSplitVal(
		const tagInfo& taginfo, 
		std::vector<std::string>& vals
		);

	void onSplitParams(
		const std::string& src, 
		std::vector<std::string>& vals
		);

	void onSplitSetSubtags(
		const std::string& _toptag, 
		const std::string& _subtop, 
		const std::string& src
		);

	const bool RecordElements::onSplitParamsToPair(
		const std::string& src,
		std::vector<std::pair<std::string, std::string>>& rst
		);


	// アノテーション情報を分割する
	const bool onSplitAnnotateInfo(const std::string& src);

	// SubTagの各要素をアルファベット順にソートする
	void onSortABCSubTags();

	// %XXX%の値の置換を試みる
	const std::string onTryGetSpecReplaceProp(
		kome::objects::Spectrum*,
		const std::string& src
		);

	// MassBank Record の通常フォーマットで文字列を作成する
	const std::string onGetMassBankLine(
		const std::string& _top, 
		const std::string& _tag, 
		const std::string& val
		);

	// TagInfoの要素を作成する
	const std::string onGetMassBankLine(
		const tagInfo& taginfo, 
		kome::objects::Spectrum* spec
		);

	// Subtagをもつ要素を作成する
	const std::string onGetMassBankLine(
		const subtagInfo& subtaginfo, 
		kome::objects::Spectrum* spec
		);

	// MassBank RecordのAccession要素を作成する
	const std::string onMakeAccession(kome::objects::Spectrum*);

	// MassBank RecordのRecord_Title 要素を作成する
	const std::string onMakeTitle(kome::objects::Spectrum* spec);

	// MassBank RecordのMSType 要素を作成する	
	const std::string onGetExtraMsType(kome::objects::Spectrum* spec);

	// MassBank Recordの ION Mode 要素を作成する	
	const std::string onGetExtraIonMode(kome::objects::Spectrum* spec);

	// MassBank Recordの Peak Picking後の値を作成する
	const std::string onGetPeakLines(
		kome::objects::Peaks* peaks, 
		std::vector<double>& mzArr
		);

	// MassBank Recordの Annotationの値を作成する
	const std::string onGetAnnotations(
		std::vector<double>& mzArr, 
		const double rt, 
		const double prec
		);

	// MassBank Recordの Peak Picking & Annotationを合成した値を作成する
	const std::string onGetPeak_MassBank_Line(kome::objects::Spectrum* spec);

	const std::string onCreateSpectrumMassBankRecord(
		const std::string& myAccession, 
		kome::objects::Spectrum* spec
		);

};

}}}

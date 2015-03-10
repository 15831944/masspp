/**
 * @file VenderInstrument.h
 * @brief MassBank's create records
 *
 * @author M.Fukuda
 * @date 2013.01.18
 *
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

// Vender ComboBoxの処理などを行う関数群

#include "VenderInstrument.h"

#include <string>

// 文字列相関測定関数を呼ぶ
#include "../calcFunc/levenshtein.h"

using namespace std;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG

using namespace boost;

namespace obo {

// MassBank Format (空白が-に置換されている等) に対応のオブジェクト
struct ToMBFormat {
	char operator()(char c) {
		if(isspace(c)) { return '-';} 
		if(isalpha(c)) { return toupper(c);}
		return c;
	}
};

// MassBank Format方式に文字列をいったん修正
const std::string transform_MassBankInstFormat(const std::string& instStr) {
	// orignal traceform
	std::string s(instStr);
	std::transform(s.begin(), s.end(), s.begin(), ToMBFormat());
	return s;
}

// サンプルのInstrument Nameと、内部のInstumentが最も近いInstrument,Venderを返す。
void searchNearName(
	const std::string keyInst,
	const InstArr& instArr,
	std::string& foundVender,
	std::string& foundInst
) {
	int tmponc, lowonc = 999;
	for(InstArr::const_iterator it = instArr.begin(), eit = instArr.end(); it != eit; ++it) {
		tmponc = ext::levenshtein(keyInst, transform_MassBankInstFormat(it->name));
		if (tmponc < lowonc) {
			lowonc = tmponc;
			foundVender = it->svendername;
			foundInst = it->sname;
			if(lowonc == 0) { break; }
		}
	}
}

void setupVenderInfo(VenderArr& venderArr);
void setupInstInfo(InstArr& instArr);

//Batch Searchの為に、Venderの情報を返す
void getVenderArray (
	VenderArr& venderArr
){
	setupVenderInfo(venderArr);
}

//Batch Searchの為に、Insturumentの情報を返す
void getInstrumentArray (
	InstArr& instArr
){
	setupInstInfo(instArr);
}

void getVenderAndInstInfo (
	const std::string& keyInst,
	VenderArr& venderArr, 
	InstArr& instArr,
	std::string& foundVender,
	std::string& foundInst
) {
	setupVenderInfo(venderArr);
	setupInstInfo(instArr);
	searchNearName(keyInst, instArr, foundVender, foundInst);
}


void setupVenderInfo(VenderArr& venderArr) {
venderArr.push_back(venderSt("AB SCIEX","absiex","The brand of instruments from the joint venture between Applied Biosystems and MDS Analytical Technologies (formerly MDS SCIEX). Previously branded as Applied Biosystems|MDS SCIEX."));
venderArr.push_back(venderSt("Bruker Daltonics","bruker",""));
venderArr.push_back(venderSt("Shimadzu","Shimadzu","Shimadzu corporation instrument model. "));
venderArr.push_back(venderSt("Waters","waters","Waters Corporation instrument model."));
venderArr.push_back(venderSt("Thermo Fisher Scientific","thermo","Thermo Fisher Scientific instrument model. The company has gone through several names including Thermo Finnigan, Thermo Scientific. "));
venderArr.push_back(venderSt("Hitachi","hitachi","Hitachi instrument model."));
venderArr.push_back(venderSt("Varian","varian","Varian instrument model. "));
venderArr.push_back(venderSt("Agilent","agilent","Agilent instrument model. "));
venderArr.push_back(venderSt("Dionex","dionex","Dionex instrument model. "));
venderArr.push_back(venderSt("Applied Biosystems","applied","Applied Biosystems instrument model. "));
venderArr.push_back(venderSt("LECO","leco","LECO instrument model. "));
}

void setupInstInfo(InstArr& instArr) {
	typedef instrumentSt instrument;
instArr.push_back(instrument("4000 QTRAP","4000 QTRAP","Applied Biosystems/MDS SCIEX Q 4000 TRAP MS. ","absiex"));
instArr.push_back(instrument("API 150EX","API 150EX","Applied Biosystems/MDS SCIEX API 150EX MS. ","absiex"));
instArr.push_back(instrument("API 150EX Prep","API 150EX Prep","Applied Biosystems/MDS SCIEX API 150EX Prep MS. ","absiex"));
instArr.push_back(instrument("API 2000","API 2000","Applied Biosystems/MDS SCIEX API 2000 MS. ","absiex"));
instArr.push_back(instrument("API 3000","API 3000","Applied Biosystems/MDS SCIEX API 3000 MS. ","absiex"));
instArr.push_back(instrument("API 4000","API 4000","Applied Biosystems/MDS SCIEX API 4000 MS. ","absiex"));
instArr.push_back(instrument("proteomics solution 1","proteomics solution 1","Applied Biosystems/MDS SCIEX Proteomics Solution 1 MS. ","absiex"));
instArr.push_back(instrument("Q TRAP","Q TRAP","Applied Biosystems/MDS SCIEX Q TRAP MS. ","absiex"));
instArr.push_back(instrument("QSTAR","QSTAR","Applied Biosystems/MDS SCIEX QSTAR MS. ","absiex"));
instArr.push_back(instrument("SymBiot I","SymBiot I","Applied Biosystems/MDS SCIEX SymBiot I MS. ","absiex"));
instArr.push_back(instrument("SymBiot XVI","SymBiot XVI","Applied Biosystems/MDS SCIEX SymBiot XVI MS. ","absiex"));
instArr.push_back(instrument("3200 QTRAP","3200 QTRAP","AB SCIEX or Applied Biosystems|MDS SCIEX QTRAP 3200. ","absiex"));
instArr.push_back(instrument("4800 Plus MALDI TOF/TOF","4800 Plus MALDI TOF/TOF","AB SCIEX or Applied Biosystems|MDS SCIEX 4800 Plus MALDI TOF-TOF Analyzer. ","absiex"));
instArr.push_back(instrument("API 3200","API 3200","AB SCIEX or Applied Biosystems|MDS SCIEX API 3200 MS. ","absiex"));
instArr.push_back(instrument("API 5000","API 5000","AB SCIEX or Applied Biosystems|MDS SCIEX API 5000 MS. ","absiex"));
instArr.push_back(instrument("QSTAR Elite","QSTAR Elite","AB SCIEX or Applied Biosystems|MDS SCIEX QSTAR Elite. ","absiex"));
instArr.push_back(instrument("QSTAR Pulsar","QSTAR Pulsar","Applied Biosystems|MDS SCIEX QSTAR Pulsar. ","absiex"));
instArr.push_back(instrument("QSTAR XL","QSTAR XL","Applied Biosystems|MDS SCIEX QSTAR XL. ","absiex"));
instArr.push_back(instrument("4000 QTRAP","4000 QTRAP","OBSOLETE AB SCIEX or Applied Biosystems|MDS SCIEX QTRAP 4000. ","absiex"));
instArr.push_back(instrument("QTRAP 5500","QTRAP 5500","Applied Biosystems|MDS SCIEX QTRAP 5500. ","absiex"));
instArr.push_back(instrument("TripleTOF 5600","TripleTOF 5600","AB SCIEX TripleTOF 5600, a quadrupole - quadrupole - time-of-flight mass spectrometer. ","absiex"));
instArr.push_back(instrument("TOF/TOF 5800","TOF/TOF 5800","AB SCIEX or Applied Biosystems|MDS Analytical Technologies AB SCIEX TOF/TOF 5800 Analyzer. ","absiex"));
instArr.push_back(instrument("HCT","HCT","Bruker Daltonics' HCT: ESI Q-TOF, Nanospray, APCI, APPI. ","bruker"));
instArr.push_back(instrument("HCTplus","HCTplus","Bruker Daltonics' HCTplus: ESI Q-TOF, Nanospray, APCI, APPI. ","bruker"));
instArr.push_back(instrument("HCTultra","HCTultra","Bruker Daltonics' HCTultra: ESI TOF, Nanospray, APCI, APPI. ","bruker"));
instArr.push_back(instrument("HCTultra PTM","HCTultra PTM","Bruker Daltonics' HCTultra PTM: ESI TOF, Nanospray, APCI, APPI, PTR. ","bruker"));
instArr.push_back(instrument("HCTultra ETD II","HCTultra ETD II","Bruker Daltonics' HCTultra ETD II: ESI Q-TOF, Nanospray, APCI, APPI, ETD. ","bruker"));
instArr.push_back(instrument("esquire 4000","esquire 4000","Bruker Daltonics' esquire 4000: linear ion trap, ESI, MALDI, Nanospray, APCI, APPI. ","bruker"));
instArr.push_back(instrument("esquire 6000","esquire 6000","Bruker Daltonics' esquire 6000: linear ion trap, ESI, MALDI, Nanospray, APCI, APPI. ","bruker"));
instArr.push_back(instrument("autoflex II","autoflex II","Bruker Daltonics' autoflex II: MALDI TOF. ","bruker"));
instArr.push_back(instrument("autoflex TOF/TOF","autoflex TOF/TOF","Bruker Daltonics' autoflex TOF/TOF MS: MALDI TOF. ","bruker"));
instArr.push_back(instrument("microflex","microflex","Bruker Daltonics' microflex: MALDI TOF. ","bruker"));
instArr.push_back(instrument("OmniFlex","OmniFlex","Bruker Daltonics' OmniFlex: MALDI TOF. ","bruker"));
instArr.push_back(instrument("ultraflex","ultraflex","Bruker Daltonics' ultraflex: MALDI TOF. ","bruker"));
instArr.push_back(instrument("ultraflex TOF/TOF","ultraflex TOF/TOF","Bruker Daltonics' ultraflex TOF/TOF: MALDI TOF. ","bruker"));
instArr.push_back(instrument("autoflex III smartbeam","autoflex III smartbeam","Bruker Daltonics' autoflex III smartbeam: MALDI TOF. ","bruker"));
instArr.push_back(instrument("microflex LT","microflex LT","Bruker Daltonics' microflex LT: MALDI TOF. ","bruker"));
instArr.push_back(instrument("ultraflex III TOF/TOF","ultraflex III TOF/TOF","Bruker Daltonics' ultraflex III TOF/TOF: MALDI TOF. ","bruker"));
instArr.push_back(instrument("microflex LRF","microflex LRF","Bruker Daltonics' microflex LRF: MALDI TOF. ","bruker"));
instArr.push_back(instrument("ultrafleXtreme","ultrafleXtreme","Bruker Daltonics' ultrafleXtreme: MALDI TOF. ","bruker"));
instArr.push_back(instrument("microflex II","microflex II","Bruker Daltonics' microflex II: MALDI TOF. ","bruker"));
instArr.push_back(instrument("autoflex II TOF/TOF","autoflex II TOF/TOF","Bruker Daltonics' autoflex II TOF/TOF: MALDI TOF. ","bruker"));
instArr.push_back(instrument("autoflex III TOF/TOF smartbeam","autoflex III TOF/TOF smartbeam","Bruker Daltonics' autoflex III TOF/TOF smartbeam: MALDI TOF. ","bruker"));
instArr.push_back(instrument("autoflex","autoflex","Bruker Daltonics' autoflex: MALDI TOF. ","bruker"));
instArr.push_back(instrument("BioTOF II","BioTOF II","Bruker Daltonics' BioTOF II: ESI TOF. ","bruker"));
instArr.push_back(instrument("BioTOF-Q","BioTOF-Q","Bruker Daltonics' BioTOF-Q: ESI Q-TOF. ","bruker"));
instArr.push_back(instrument("BioTOF","BioTOF","Bruker Daltonics' BioTOF: ESI TOF. ","bruker"));
instArr.push_back(instrument("BioTOF III","BioTOF III","Bruker Daltonics' BioTOF III: ESI TOF. ","bruker"));
instArr.push_back(instrument("UltroTOF-Q","UltroTOF-Q","Bruker Daltonics' UltroTOF-Q: ESI Q-TOF (MALDI optional). ","bruker"));
instArr.push_back(instrument("microTOF LC","microTOF LC","Bruker Daltonics' microTOF LC: ESI TOF, Nanospray, APCI, APPI. ","bruker"));
instArr.push_back(instrument("micrOTOF","micrOTOF","Bruker Daltonics' micrOTOF: ESI TOF, APCI, APPI. ","bruker"));
instArr.push_back(instrument("micrOTOF-Q","micrOTOF-Q","Bruker Daltonics' micrOTOF-Q: ESI Q-TOF, Nanospray, APCI, APPI. ","bruker"));
instArr.push_back(instrument("micrOTOF-Q II","micrOTOF-Q II","Bruker Daltonics' micrOTOF-Q II: ESI Q-TOF, Nanospray, APCI, APPI. ","bruker"));
instArr.push_back(instrument("micrOTOF II","micrOTOF II","Bruker Daltonics' micrOTOF II: ESI TOF, Nanospray, APCI, APPI. ","bruker"));
instArr.push_back(instrument("amaZon ETD","amaZon ETD","Bruker Daltonics' amaZon ETD: ESI quadrupole ion trap, Nanospray, APCI, APPI, ETD, PTR. ","bruker"));
instArr.push_back(instrument("amaZon X","amaZon X","Bruker Daltonics' amaZon X: ESI quadrupole ion trap, APCI, APPI, ETD, PTR. ","bruker"));
instArr.push_back(instrument("maXis","maXis","Bruker Daltonics' maXis: ESI Q-TOF, Nanospray, APCI, APPI. ","bruker"));
instArr.push_back(instrument("maXis impact","maXis impact","maXis impact. ","bruker"));
instArr.push_back(instrument("solariX","solariX","Bruker Daltonics' solariX: ESI, MALDI, Qh-FT_ICR. ","bruker"));
instArr.push_back(instrument("apex IV","apex IV","Bruker Daltonics' apex IV: ESI, MALDI, Nanospray, APCI, APPI, Qh-FT_ICR. ","bruker"));
instArr.push_back(instrument("apex Q","apex Q","Bruker Daltonics' apex Q: ESI, MALDI, Nanospray, APCI, APPI, Qh-FT_ICR. ","bruker"));
instArr.push_back(instrument("apex ultra","apex ultra","Bruker Daltonics' apex ultra: ESI, MALDI, Nanospray, APCI, APPI, Qh-FT_ICR. ","bruker"));
instArr.push_back(instrument("AXIMA CFR MALDI-TOF","AXIMA CFR MALDI-TOF","Shimadzu Biotech AXIMA CFR MALDI-TOF MS. ","Shimadzu"));
instArr.push_back(instrument("AXIMA-QIT","AXIMA-QIT","Shimadzu Biotech AXIMA-QIT MS. ","Shimadzu"));
instArr.push_back(instrument("AXIMA-CFR plus","AXIMA-CFR plus","Shimadzu Biotech AXIMA-CFR plus MS. ","Shimadzu"));
instArr.push_back(instrument("AXIMA Performance MALDI-TOF/TOF","AXIMA Performance MALDI-TOF/TOF","Shimadzu Biotech AXIMA Performance MALDI-TOF/TOF MS. ","Shimadzu"));
instArr.push_back(instrument("AXIMA Confidence MALDI-TOF","AXIMA Confidence MALDI-TOF","Shimadzu Biotech AXIMA Confidence MALDI-TOF (curved field reflectron) MS. ","Shimadzu"));
instArr.push_back(instrument("AXIMA Assurance Linear MALDI-TOF","AXIMA Assurance Linear MALDI-TOF","Shimadzu Biotech AXIMA Assurance Linear MALDI-TOF MS. ","Shimadzu"));
instArr.push_back(instrument("LCMS-IT-TOF","LCMS-IT-TOF","Shimadzu Scientific Instruments LCMS-IT-TOF MS. ","Shimadzu"));
instArr.push_back(instrument("LCMS-2010EV","LCMS-2010EV","Shimadzu Scientific Instruments LCMS-2010EV MS. ","Shimadzu"));
instArr.push_back(instrument("LCMS-2010A","LCMS-2010A","Shimadzu Scientific Instruments LCMS-2010A MS. ","Shimadzu"));
instArr.push_back(instrument("Auto Spec Ultima NT","Auto Spec Ultima NT","Waters AutoSpec Ultima NT MS. ","waters"));
instArr.push_back(instrument("GCT","GCT","Waters GCT MS. ","waters"));
instArr.push_back(instrument("IsoPrime","IsoPrime","Waters IsoPrime MS. ","waters"));
instArr.push_back(instrument("IsoProbe","IsoProbe","Waters IsoProbe MS. ","waters"));
instArr.push_back(instrument("IsoProbe T","IsoProbe T","Waters IsoProbe T MS. ","waters"));
instArr.push_back(instrument("M@LDI L","M@LDI L","Waters MALDI L MS. ","waters"));
instArr.push_back(instrument("M@LDI LR","M@LDI LR","Waters MALDI LR MS. ","waters"));
instArr.push_back(instrument("NG-5400","NG-5400","Waters NG-5400 MS. ","waters"));
instArr.push_back(instrument("Platform ICP","Platform ICP","Waters Platform ICP MS. ","waters"));
instArr.push_back(instrument("Q-Tof micro","Q-Tof micro","Waters Q-Tof micro MS. ","waters"));
instArr.push_back(instrument("Q-Tof ultima","Q-Tof ultima","Waters Q-Tof Ultima MS. ","waters"));
instArr.push_back(instrument("quattro micro","quattro micro","Waters Quattro micro MS. ","waters"));
instArr.push_back(instrument("Quattro UItima","Quattro UItima","Waters Quattro Uitima MS. ","waters"));
instArr.push_back(instrument("Q-Tof Premier","Q-Tof Premier","Waters Q-Tof Premier MS. ","waters"));
instArr.push_back(instrument("Acquity UPLC PDA","Acquity UPLC PDA","Acquity UPLC Photodiode Array Detector. ","waters"));
instArr.push_back(instrument("Acquity UPLC FLR","Acquity UPLC FLR","Acquity UPLC Fluorescence Detector. ","waters"));
instArr.push_back(instrument("ACQUITY UPLC","ACQUITY UPLC","Waters LC-system. ","waters"));
instArr.push_back(instrument("ACQUITY UPLC H-Class","ACQUITY UPLC H-Class","Waters LC-system. ","waters"));
instArr.push_back(instrument("ACQUITY UPLC H-Class Bio","ACQUITY UPLC H-Class Bio","Waters LC-system. ","waters"));
instArr.push_back(instrument("ACQUITY UPLC I-Class","ACQUITY UPLC I-Class","Waters LC-system. ","waters"));
instArr.push_back(instrument("ACQUITY UPLC Systems with 2D Technology","ACQUITY UPLC Systems with 2D Technology","Waters LC-system. ","waters"));
instArr.push_back(instrument("nanoACQUITY UPLC","nanoACQUITY UPLC","Waters LC-system. ","waters"));
instArr.push_back(instrument("nanoACQUITY UPLC System with Technology","nanoACQUITY UPLC System with Technology","Waters LC-system. ","waters"));
instArr.push_back(instrument("nanoACQUITY UPLC with HDX Technology","nanoACQUITY UPLC with HDX Technology","Waters LC-system. ","waters"));
instArr.push_back(instrument("TRIZAIC UPLC nanoTile","TRIZAIC UPLC nanoTile","Waters LC-system. ","waters"));
instArr.push_back(instrument("GCT Premier","GCT Premier","Waters oa-ToF based. ","waters"));
instArr.push_back(instrument("MALDI Synapt G2 HDMS","MALDI Synapt G2 HDMS","Waters oa-ToF based. ","waters"));
instArr.push_back(instrument("MALDI Synapt G2 MS","MALDI Synapt G2 MS","Waters oa-ToF based. ","waters"));
instArr.push_back(instrument("MALDI Synapt G2-S HDMS","MALDI Synapt G2-S HDMS","Waters oa-ToF based. ","waters"));
instArr.push_back(instrument("MALDI Synapt G2-S MS","MALDI Synapt G2-S MS","Waters oa-ToF based. ","waters"));
instArr.push_back(instrument("MALDI Synapt HDMS","MALDI Synapt HDMS","Waters oa-ToF based. ","waters"));
instArr.push_back(instrument("MALDI Synapt MS","MALDI Synapt MS","Waters oa-ToF based. ","waters"));
instArr.push_back(instrument("Synapt G2 HDMS","Synapt G2 HDMS","Waters oa-ToF based. ","waters"));
instArr.push_back(instrument("Synapt G2 MS","Synapt G2 MS","Waters oa-ToF based. ","waters"));
instArr.push_back(instrument("Synapt G2-S HDMS","Synapt G2-S HDMS","Waters oa-ToF based. ","waters"));
instArr.push_back(instrument("Synapt G2-S MS","Synapt G2-S MS","Waters oa-ToF based. ","waters"));
instArr.push_back(instrument("Synapt HDMS","Synapt HDMS","Waters oa-ToF based. ","waters"));
instArr.push_back(instrument("Synapt MS","Synapt MS","Waters oa-ToF based. ","waters"));
instArr.push_back(instrument("Xevo G2 Q-Tof","Xevo G2 Q-Tof","Waters oa-ToF based. ","waters"));
instArr.push_back(instrument("Xevo G2 Tof","Xevo G2 Tof","Waters oa-ToF based. ","waters"));
instArr.push_back(instrument("Xevo Q-Tof","Xevo Q-Tof","Waters oa-ToF based. ","waters"));
instArr.push_back(instrument("3100","3100","Waters quadrupole based. ","waters"));
instArr.push_back(instrument("Acquity SQD","Acquity SQD","Waters quadrupole based. ","waters"));
instArr.push_back(instrument("Acquity TQD","Acquity TQD","Waters quadrupole based. ","waters"));
instArr.push_back(instrument("Quattro micro GC","Quattro micro GC","Waters quadrupole based. ","waters"));
instArr.push_back(instrument("Xevo TQ MS","Xevo TQ MS","Waters quadrupole based. ","waters"));
instArr.push_back(instrument("Xevo TQD","Xevo TQD","Waters quadrupole based. ","waters"));
instArr.push_back(instrument("Xevo TQ-S","Xevo TQ-S","Waters quadrupole based. ","waters"));
instArr.push_back(instrument("DELTA plusAdvantage","DELTA plusAdvantage","ThermoFinnigan DELTA plusAdvantage MS. ","thermo"));
instArr.push_back(instrument("DELTAplusXP","DELTAplusXP","ThermoFinnigan DELTAplusXP MS. ","thermo"));
instArr.push_back(instrument("LCQ Advantage","LCQ Advantage","ThermoFinnigan LCQ Advantage MS. ","thermo"));
instArr.push_back(instrument("LCQ Classic","LCQ Classic","ThermoFinnigan LCQ Classic MS. ","thermo"));
instArr.push_back(instrument("LCQ Deca XP Plus","LCQ Deca XP Plus","ThermoFinnigan LCQ Deca XP Plus MS. ","thermo"));
instArr.push_back(instrument("neptune","neptune","ThermoFinnigan NEPTUNE MS. ","thermo"));
instArr.push_back(instrument("PolarisQ","PolarisQ","ThermoFinnigan PolarisQ MS. ","thermo"));
instArr.push_back(instrument("Surveyor MSQ","Surveyor MSQ","ThermoFinnigan Surveyor MSQ MS. ","thermo"));
instArr.push_back(instrument("TEMPUS TOF","TEMPUS TOF","ThermoFinnigan TEMPUS TOF MS. ","thermo"));
instArr.push_back(instrument("TRACE DSQ","TRACE DSQ","ThermoFinnigan TRACE DSQ MS. ","thermo"));
instArr.push_back(instrument("TRITON","TRITON","ThermoFinnigan TRITON MS. ","thermo"));
instArr.push_back(instrument("TSQ Quantum","TSQ Quantum","ThermoFinnigan TSQ Quantum MS. ","thermo"));
instArr.push_back(instrument("LCQ Deca","LCQ Deca","ThermoFinnigan LCQ Deca. ","thermo"));
instArr.push_back(instrument("GC Quantum","GC Quantum","GC Quantum. ","thermo"));
instArr.push_back(instrument("LCQ Fleet","LCQ Fleet","LCQ Fleet. ","thermo"));
instArr.push_back(instrument("DSQ","DSQ","ThermoFinnigan DSQ GC-MS. ","thermo"));
instArr.push_back(instrument("MAT253","MAT253","ThermoFinnigan MAT253 MS. ","thermo"));
instArr.push_back(instrument("MAT900XP","MAT900XP","ThermoFinnigan MAT900XP MS. ","thermo"));
instArr.push_back(instrument("MAT900XP Trap","MAT900XP Trap","ThermoFinnigan MAT900XP Trap MS. ","thermo"));
instArr.push_back(instrument("MAT95XP","MAT95XP","ThermoFinnigan MAT95XP MS. ","thermo"));
instArr.push_back(instrument("MAT95XP Trap","MAT95XP Trap","ThermoFinnigan MAT95XP Trap MS. ","thermo"));
instArr.push_back(instrument("SSQ 7000","SSQ 7000","ThermoFinnigan SSQ 7000 MS. ","thermo"));
instArr.push_back(instrument("TSQ 7000","TSQ 7000","ThermoFinnigan TSQ 7000 MS. ","thermo"));
instArr.push_back(instrument("TSQ","TSQ","ThermoFinnigan TSQ MS. ","thermo"));
instArr.push_back(instrument("LTQ","LTQ","Finnigan LTQ MS. ","thermo"));
instArr.push_back(instrument("LTQ FT","LTQ FT","Finnigan LTQ FT MS. ","thermo"));
instArr.push_back(instrument("LTQ Orbitrap","LTQ Orbitrap","Finnigan LTQ Orbitrap MS. ","thermo"));
instArr.push_back(instrument("LXQ","LXQ","Finnigan LXQ MS. ","thermo"));
instArr.push_back(instrument("LTQ Orbitrap Discovery","LTQ Orbitrap Discovery","LTQ Orbitrap Discovery. ","thermo"));
instArr.push_back(instrument("LTQ Orbitrap XL","LTQ Orbitrap XL","LTQ Orbitrap XL. ","thermo"));
instArr.push_back(instrument("LTQ FT Ultra","LTQ FT Ultra","LTQ FT Ultra. ","thermo"));
instArr.push_back(instrument("Surveyor PDA","Surveyor PDA","Surveyor PDA. ","thermo"));
instArr.push_back(instrument("Accela PDA","Accela PDA","Accela PDA. ","thermo"));
instArr.push_back(instrument("ITQ 700","ITQ 700","Thermo Scientific ITQ 700 GC-MS. ","thermo"));
instArr.push_back(instrument("ITQ 900","ITQ 900","Thermo Scientific ITQ 900 GC-MS. ","thermo"));
instArr.push_back(instrument("ITQ 1100","ITQ 1100","Thermo Scientific ITQ 1100 GC-MS. ","thermo"));
instArr.push_back(instrument("LTQ XL ETD","LTQ XL ETD","Thermo Scientific LTQ XL MS with ETD. ","thermo"));
instArr.push_back(instrument("LTQ Orbitrap XL ETD","LTQ Orbitrap XL ETD","Thermo Scientific LTQ Orbitrap XL MS with ETD. ","thermo"));
instArr.push_back(instrument("DFS","DFS","Thermo Scientific DFS HR GC-MS. ","thermo"));
instArr.push_back(instrument("DSQ II","DSQ II","Thermo Scientific DSQ II GC-MS. ","thermo"));
instArr.push_back(instrument("MALDI LTQ XL","MALDI LTQ XL","Thermo Scientific MALDI LTQ XL MS. ","thermo"));
instArr.push_back(instrument("MALDI LTQ Orbitrap","MALDI LTQ Orbitrap","Thermo Scientific MALDI LTQ Orbitrap MS. ","thermo"));
instArr.push_back(instrument("TSQ Quantum Access","TSQ Quantum Access","Thermo Scientific TSQ Quantum Access MS. ","thermo"));
instArr.push_back(instrument("Element XR","Element XR","Thermo Scientific Element XR HR-ICP-MS. ","thermo"));
instArr.push_back(instrument("Element 2","Element 2","Thermo Scientific Element 2 HR-ICP-MS. ","thermo"));
instArr.push_back(instrument("Element GD","Element GD","Thermo Scientific Element GD Glow Discharge MS. ","thermo"));
instArr.push_back(instrument("GC IsoLink","GC IsoLink","Thermo Scientific GC IsoLink Isotope Ratio MS. ","thermo"));
instArr.push_back(instrument("Exactive","Exactive","Thermo Scientific Exactive MS. ","thermo"));
instArr.push_back(instrument("TSQ Quantum Ultra AM","TSQ Quantum Ultra AM","Thermo Scientific TSQ Quantum Ultra AM. ","thermo"));
instArr.push_back(instrument("TSQ Quantum Ultra","TSQ Quantum Ultra","Thermo Scientific TSQ Quantum Ultra. ","thermo"));
instArr.push_back(instrument("LTQ XL","LTQ XL","Thermo Scientific LTQ XL MS. ","thermo"));
instArr.push_back(instrument("LTQ Velos","LTQ Velos","Thermo Scientific LTQ Velos MS. ","thermo"));
instArr.push_back(instrument("LTQ Velos ETD","LTQ Velos ETD","Thermo Scientific LTQ Velos MS with ETD. ","thermo"));
instArr.push_back(instrument("TSQ Vantage","TSQ Vantage","TSQ Vantage. ","thermo"));
instArr.push_back(instrument("LTQ Orbitrap Velos","LTQ Orbitrap Velos","Finnigan LTQ Orbitrap Velos MS. ","thermo"));
instArr.push_back(instrument("ISQ","ISQ","Thermo Scientific ISQ single quadrupole MS with the ExtractraBrite source. ","thermo"));
instArr.push_back(instrument("Velos Plus","Velos Plus","Thermo Scientific second generation Velos. ","thermo"));
instArr.push_back(instrument("LTQ Orbitrap Elite","LTQ Orbitrap Elite","Thermo Scientific second generation Velos and Orbitrap. ","thermo"));
instArr.push_back(instrument("Q Exactive","Q Exactive","Thermo Scientific Q Exactive. ","thermo"));
instArr.push_back(instrument("explorer","explorer","IonSpec Explorer MS. ","varian"));
instArr.push_back(instrument("HiRes ESI","HiRes ESI","IonSpec HiResESI MS. ","varian"));
instArr.push_back(instrument("HiRes MALDI","HiRes MALDI","IonSpec HiResMALDI MS. ","varian"));
instArr.push_back(instrument("OMEGA","OMEGA","IonSpec OMEGA MS. ","varian"));
instArr.push_back(instrument("OMEGA-2001","OMEGA-2001","IonSpec OMEGA-2001 MS. ","varian"));
instArr.push_back(instrument("ultima","ultima","IonSpec Ultima MS. ","varian"));
instArr.push_back(instrument("1200 series LC/MSD SL","1200 series LC/MSD SL","The 1200 Series LC/MSD SL ion trap belongs to the Agilent LC/MSD ion trap family. It provides fast polarity switching and multisignal data acquisition capabilities in a single run while also providing 5 stages of automated data dependent MS/MS and 11 stages of manual MS/MS. ","agilent"));
instArr.push_back(instrument("6110 Quadrupole LC/MS","6110 Quadrupole LC/MS","The 6110 Quadrupole LC/MS system is a Agilent liquid chromatography instrument combined with an entry level single quadrupole mass spectrometer from the 6100 Series of Agilent quadrupole mass spectrometers. 6110 Quadrupole mass spectrometer has m/z range of 10-1500 and 2500 u/s scan speed. It proves useful for wide range of SIM quantitative applications. ","agilent"));
instArr.push_back(instrument("6120 Quadrupole LC/MS","6120 Quadrupole LC/MS","The 6120 Quadrupole LC/MS system is a Agilent liquid chromatography instrument combined with a single quadrupole mass spectrometer from the 6100 Series of Agilent mass spectrometers. 6120 quadrupole mass spectrometer has m/z range of 10-1500, 2500 u/s scan speed and utilizes multiple signal acquisition. ","agilent"));
instArr.push_back(instrument("6130 Quadrupole LC/MS","6130 Quadrupole LC/MS","The 6130 Quadrupole LC/MS system is a Agilent liquid chromatography instrument combined with a single quadrupole mass spectrometer from the 6100 series of Agilent mass spectrometers. The 6130 quadrupole mass spectrometer has m/z range of 2-3000, 2500 u/s scan speed in standard mode and 5250 u/s speed in fast-scan mode. It also uses multiple signal acquisition. ","agilent"));
instArr.push_back(instrument("6140 Quadrupole LC/MS","6140 Quadrupole LC/MS","The 6140 Quadrupole LC/MS system is a Agilent liquid chromatography instrument combined with a single quadrupole mass spectrometer from the 6100 Series of Agilent quadrupole mass spectrometers. 6140 Quadrupole mass spectrometer has m/z range of 10-1350, 2500 u/s scan speed in standard mode and 10000 u/s speed in fast-scan mode. It also uses multiple signal acquisition. ","agilent"));
instArr.push_back(instrument("6210 Time-of-Flight LC/MS","6210 Time-of-Flight LC/MS","The 6210 Time-of-Flight LC/MS is a Agilent liquid chromatography instrument combined with a Agilent time of flight mass spectrometer. This time of flight mass spectrometer has a m/z range of 50-12000, mass accuracy of less than 2 ppm and resolution greater than 13,000 at m/z 2722. It has multiple ion sources and can be used with multimode ion sources. ","agilent"));
instArr.push_back(instrument("6310 Ion Trap LC/MS","6310 Ion Trap LC/MS","The 6310 Ion Trap LC/MS is a Agilent liquid chromatography instrument combined with a 6300 series Agilent ion trap. It has a mass range of 50-2200 between 0.6 to 0.35 resolution and mass range of 200-4000 with resolution of 3-4. The scan speed varies from 1650-27000 for the respective mass ranges. ","agilent"));
instArr.push_back(instrument("6320 Ion Trap LC/MS","6320 Ion Trap LC/MS","The 6320 Ion Trap LC/MS is a Agilent liquid chromatography instrument combined with a 6300 series Agilent ion trap. It has a mass range of 50-2200 between 0.6 to 0.25 resolution and mass range of 200-4000 with resolution of less than 3. The scan speed varies from 1650-27000 for the respective mass ranges. ","agilent"));
instArr.push_back(instrument("6330 Ion Trap LC/MS","6330 Ion Trap LC/MS","The 6330 Ion Trap LC/MS is a Agilent liquid chromatography instrument combined with a 6300 series Agilent ion trap. It has a mass range of 50-2200 between 0.6 to 0.25 resolution and mass range of 200-4000 with resolution of less than 3. The scan speed varies from 1650-27000 for the respective mass ranges. ","agilent"));
instArr.push_back(instrument("6340 Ion Trap LC/MS","6340 Ion Trap LC/MS","The 6340 Ion Trap LC/MS is a Agilent liquid chromatography instrument combined with a 6300 series Agilent ion trap. It has a mass range of 50-2200 between 0.6 to 0.25 resolution and mass range of 200-4000 with resolution of less than 3. The scan speed varies from 1650-27000 for the respective mass ranges. ","agilent"));
instArr.push_back(instrument("6410 Triple Quadrupole LC/MS","6410 Triple Quadrupole LC/MS","The 6410 Quadrupole LC/MS system is a Agilent liquid chromatography instrument combined with a Agilent triple quadrupole mass spectrometer. Mass range of the mass spectrometer is 15-1650 m/z, resolution is at three settings of 0.7 u (unit), 1.2 u (wide) and 2.5 u (widest). The mass accuracy for 6410 mass spectrometer is 0.1 across the mass range. The collision cell is a hexapole with linear acceleration. ","agilent"));
instArr.push_back(instrument("1200 series LC/MSD VL","1200 series LC/MSD VL","The LC/MSD VL ion trap is part of the family of Agilent ion trap mass spectrometers. It has ESI, APCI and APPI ion sources and is a useful ion trap when the amount of sample is not the limiting factor. ","agilent"));
instArr.push_back(instrument("6220 Time-of-Flight LC/MS","6220 Time-of-Flight LC/MS","The 6220 Time-of-Flight LC/MS is a Agilent liquid chromatography instrument combined with a Agilent time of flight mass spectrometer. This time of flight mass spectrometer has a m/z range of 50-12000, mass accuracy of less than 2 ppm and resolution greater than 13,000 at m/z 2722. It has multiple ion sources and can be used with multimode ion sources. ","agilent"));
instArr.push_back(instrument("6510 Quadrupole Time-of-Flight LC/MS","6510 Quadrupole Time-of-Flight LC/MS","The 6510 Quadrupole Time-of-Flight LC/MS is a Agilent liquid chromatography instrument combined with a Agilent time of flight mass spectrometer. This time of flight mass spectrometer has a m/z range of 50-12000, mass accuracy of less than 2 ppm and resolution greater than 13,000 at m/z 2722. It has multiple ion sources and can be used with multimode ion sources. ","agilent"));
instArr.push_back(instrument("6520 Quadrupole Time-of-Flight LC/MS","6520 Quadrupole Time-of-Flight LC/MS","The 6520 Quadrupole Time-of-Flight LC/MS is a Agilent liquid chromatography instrument combined with a Agilent time of flight mass spectrometer. This time of flight mass spectrometer has a m/z range of 50-12000, mass accuracy of less than 2 ppm and resolution greater than 26,000 at m/z 2722. It has multiple ion sources and can be used with multimode ion sources. ","agilent"));
instArr.push_back(instrument("4700 Proteomics Analyzer","4700 Proteomics Analyzer","Applied Biosystems/MDS SCIEX 4700 Proteomics Analyzer MS. ","applied"));
instArr.push_back(instrument("Voyager-DE PRO","Voyager-DE PRO","Applied Biosystems/MDS SCIEX Voyager-DE PRO MS. ","applied"));
instArr.push_back(instrument("Voyager-DE STR","Voyager-DE STR","Applied Biosystems/MDS SCIEX Voyager-DE STR MS. ","applied"));
instArr.push_back(instrument("4800 Proteomics Analyzer","4800 Proteomics Analyzer","Applied Biosystems|MDS SCIEX 4800 Proteomics Analyzer. ","applied"));
instArr.push_back(instrument("Pegasus HRT","Pegasus HRT","LECO high resolution time-of-flight GC mass spectrometer. ","leco"));
instArr.push_back(instrument("Citius HRT","Citius HRT","LECO high resolution time-of-flight LC mass spectrometer. ","leco"));
instArr.push_back(instrument("Pegasus","Pegasus","LECO GC time-of-flight mass spectrometer. ","leco"));
instArr.push_back(instrument("TruTOF","TruTOF","LECO bench-top GC time-of-flight mass spectrometer. ","leco"));
instArr.push_back(instrument("Pegasus 4D","Pegasus 4D","LECO nominal mass resolution time-of-flight GCxGC mass spectrometer. ","leco"));
}

}

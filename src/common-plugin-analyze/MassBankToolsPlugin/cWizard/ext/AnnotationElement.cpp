/**
 * @file AnnotationElement
 * @brief Annotation Element Control
 *
 * @author M.Fukuda
 * @date 2013.03.08
 *
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#include "AnnotationElement.h"
#include "../calcFunc/StrFunc.h"
#include "../rel_stdafx.h"

const std::string kErr_NoMatchAnno2 = 
	"No peaks match the specified annotations. \nDo you want to create MassBank spectrum records anyway?";

const std::string kErr_NoMatchAnno = 
	"No peaks match the specified annotations.";

using namespace kome::massbank::wizard;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


//		("m/z")("m/z Tol.")("m/z Tol.unit")("Start RT")("End RT")
//		("Precursor")("Precursor Tol.")("Precursor Tol.unit");
//	std::vector<std::string> pkCol2s = boost::assign::list_of("Type")("Formula");
// アノテーションとして受ける配列の意味付け。
enum ANNOKEY {
	aMZ,
	aMZtol,
	aMZunt,
	aRTSt,
	aRTEd,
	aPR,
	aPRtol,
	aPRunt,
	aType,
	aForm,
	//----------------------
	ANNOKEY_NUM,
};

// Stringとしてアノテーション情報を取得、分解する。
// checkonly (T/F) T : 入力規則のチェックだけ / F : チェックが通ったら登録。
const bool AnnotationElement::getSplitAnnoInfo(
	const std::string& src, 
	const bool checkonly
) {
	// あのてーしょんをくりあ
	annoArr_.clear();
	typedef std::vector<std::string> STRARR;
	STRARR lineTokens, innerTokens;

	kome::plugin::SettingsValue::separateListValue(src.c_str(), lineTokens);
	
	STRARR::iterator it = lineTokens.begin();
	STRARR::iterator eit = lineTokens.end();

	// 取得要素を行単位で調べる。
	for(; it != eit; ++it) {
		innerTokens.clear();
		kome::plugin::SettingsValue::separateListValue(it->c_str(), innerTokens);
		unsigned int tsize = innerTokens.size();

		if (tsize == ANNOKEY_NUM) {
			// MZ について
			double mz, mztol,lrt,hrt, prc,prctol;
			bool mzppmFg, prcppmFg = false;

			mz = mztol = -1.0;
			if(!innerTokens[aMZ].empty()){
				mz = ext::toDbl(innerTokens[aMZ]);
			}

			if(mz < 0.0) {
				// mzがマイナス。
				return false;
			}

			if(!innerTokens[aMZtol].empty()) {
				mztol = ext::toDbl(innerTokens[aMZtol]);
			}
			
			mzppmFg = (innerTokens[aMZunt] == "ppm") ? true : false; 

			// RT について
			lrt = hrt = -1.0;
			if(!innerTokens[aRTSt].empty()) {
				lrt = ext::toDbl(innerTokens[aRTSt]);
			}

			if(!innerTokens[aRTEd].empty()) {
				hrt = ext::toDbl(innerTokens[aRTEd]);
			}
			
			if (hrt < 0.0) {
				hrt = DBL_MAX;
			}

			if (lrt >= hrt) {
				// RTの範囲設定エラー。
				return false;
			}
			// Precursor について
			prc = prctol = -1.0;
			if(!innerTokens[aPR].empty()) {
				prc = ext::toDbl(innerTokens[aPR]);
			}

			if(prc > 0.0){
				if(!innerTokens[aPRtol].empty()) {
					prctol = ext::toDbl(innerTokens[aPRtol]);
				}
				prcppmFg = (innerTokens[aPRunt] == "ppm") ? true : false; 
			}
			if (!checkonly) {
				// PPMかABSにとってtolに与えるべき値が変わる。
				annoArr_.push_back(AnnoElem());
				AnnoElem& elm = annoArr_.back();
				toTolval(mz, mztol, elm.lmz, elm.mz, elm.hmz, mzppmFg);
				
				elm.lrt = lrt;
				elm.hrt = hrt;

				// PPMかABSにとってtolに与えるべき値が変わる。
				if (prc > 0.0) {
					toTolval(prc, prctol, elm.lprc, elm.prc, elm.hprc, prcppmFg);
				}
				elm.sFormula = innerTokens[aForm];
				elm.sType = innerTokens[aType];
			}
		}
	}
	return true;
}

// mzの配列、RT, Precursor をうけとる
// 内部のアノテーション情報と一致するか否か調べる。
// checkonly (T/F) T : 該当アノテーションの有無を調べる。/ F ＋MassBankRecordとして出力を得る。
const std::string AnnotationElement::checkFitAnnoInfo(
	const std::vector<double>& mzArr,
	const double rt,
	const double prc,
	const bool checkonly
){
	std::string rstStr("");
	// clean Inner count;
	std::vector<AnnoElem>::iterator it, eit;

	std::for_each(mzArr.begin(),mzArr.end(),[&](const double mz) 
	{
		std::string anoblock;
		int cnt = 0;

		for (it = annoArr_.begin(), eit = annoArr_.end(); it != eit; ++it) {
			// precursorを使う
			if(it->prc > 0.0) {
				if (prc >= 0.0) {
					if(prc < it->lprc || it->hprc < prc) continue;
					// 範囲外なら打ち切り
				}
			}
			// rtをつかう
			if(rt > 0.0) {
				// 範囲外なら打ち切り
				if(rt < it->lrt || it->hrt < rt) continue;
			}
			// Mzの範囲内外を調べる
			if (it->lmz <= mz && mz <= it->hmz) {
				// 範囲内なのでカウント
				it->nCnt++;
				if(!checkonly) {
					// 出力が必要なので準備
					cnt++;
					anoblock += onGetAnnotationBlock(mz,*it);
				}
			}
		}

		if (cnt > 0) {
			rstStr += FMT("  %f %d\n", mz, cnt);
			rstStr += anoblock;
		}
	});
	return rstStr;
}

void AnnotationElement::onClearCnt() {
	std::for_each(annoArr_.begin(), annoArr_.end(), [](AnnoElem& elm){elm.nCnt = 0;});
}

void AnnotationElement::doSpectrumFitAnnotation(
	const std::vector<kome::objects::Spectrum*>& specArr
) {
	onClearCnt();
	kome::objects::PeaksManager& pkMgr = kome::objects::PeaksManager::getInstance();
	std::vector<double> mzArr;
	kome::objects::Peaks* peaks = nullptr;

	std::for_each(specArr.begin(),specArr.end(),[&](kome::objects::Spectrum* spec) 
	{
		peaks = pkMgr.getPeaks(spec);
		if (spec != nullptr) 
		{
			unsigned int pnum = peaks->getLength();
			mzArr.clear();
			mzArr.reserve(pnum);
			for( unsigned int i = 0; i < pnum; i++ ) {
				mzArr.push_back(peaks->getX(i));
			}
			this->checkFitAnnoInfo(mzArr,spec->getRt(),spec->getPrecursor(),true);
		}
	});
}

const bool AnnotationElement::isNotUseAnnotationElements() 
{
	std::string rstStr;
	std::for_each(annoArr_.begin(), annoArr_.end(), 
		[&](const AnnoElem& elm){rstStr += onNotUsedInfo(elm);});
	if (!rstStr.empty()) {
		kome::window::WindowTool::showInfo(kErr_NoMatchAnno2.c_str());
		//if (! kome::window::WindowTool::showYesNo(kErr_NoMatchAnno.c_str(),
		//	"Yes", "No", "Create MassBank Spectrum Records", rstStr.c_str())){
		//	return false; // やり直し
		//}
	}
	return true;
}

void AnnotationElement::toTolval(
	const double src, 
	const double _tol, 
	double& dstL, 
	double& dstC, 
	double& dstH, 
	bool ppm
) {
	double tol = (!ppm) ? _tol : src * _tol * kPPMVal;
	dstC = src;
	dstL = src - tol;
	dstH = src + tol;
}

// アノテーション情報を作成する
const std::string AnnotationElement::onGetAnnotationBlock(
	const double mz, 
	const AnnoElem& elm
) {
	double errDa = mz - elm.mz;
	int errnum = static_cast<int>(floor(errDa * 1000000.0 / elm.mz + 0.5));
	return FMT("    %s %f %d %s\n", elm.sType.c_str(),elm.mz, errnum, elm.sFormula.c_str());
}

// 全ピーク中一度もヒットしなかったアノテーションの情報を作成する
const std::string AnnotationElement::onNotUsedInfo(const AnnoElem& elm) 
{
	std::string rst("");
	if (elm.nCnt == 0) {
		rst = ("\n No Match Annotation\n");
		rst.append(FMT("m/z: %f - %f\n", elm.lmz, elm.hmz));
		if (!(elm.lrt < 0.0 && elm.hrt == DBL_MAX)) {
			if (elm.lrt < 0.0) {
				rst.append(FMT("RT: - %f\n",  elm.hrt));
			} else if (elm.hrt == DBL_MAX) {
				rst.append(FMT("RT: %f - \n",  elm.lrt));
			} else {
				rst.append(FMT("RT: %f - %f\n",  elm.lrt, elm.hrt));
			}
		}
		if (elm.prc > 0.0) {
			rst.append(FMT("Prc: %f - %f\n", elm.lprc,elm.hprc));
		}
		rst.append(FMT("Type : %s\n",    elm.sType.c_str()));
		rst.append(FMT("Formula : %s\n", elm.sFormula.c_str()));
	}
	return rst;
}

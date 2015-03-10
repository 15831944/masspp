/**
 * @file AnnotationElement
 * @brief Annotation Element Control
 *
 * @author M.Fukuda
 * @date 2013.03.08
 *
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#pragma once

#include <string>
#include <vector>

// -- Forward declaration --
namespace kome {
	namespace objects {
		class Spectrum;
	}
}

// -----
namespace kome {
namespace massbank {
namespace wizard {

const double kPPMVal = 0.0000001;

class AnnotationElement {

	struct AnnoElem {
		double mz,rt,prc;
		double lmz,hmz,lrt,hrt,lprc,hprc;
		int nCnt; // 内部カウンタ。該当するたびにインクリメント。
		std::string sType,sFormula;
		AnnoElem() {
			mz = lmz = hmz = -1.0;
			rt = prc = -1.0;
			lrt = lprc = -1.0;    // nega infinity
			hrt = hprc = DBL_MAX; // true infinity
			nCnt = 0;
		}
	};
public:
	// constructor
	AnnotationElement(){}
	// destructor
	~AnnotationElement(){}

	// Stringとしてアノテーション情報を取得、分解する。
	// checkonly (T/F) T : 入力規則のチェックだけ / F : チェックが通ったら登録。
	const bool getSplitAnnoInfo(
		const std::string& src, 
		const bool checkonly
		);

	// mzの配列、RT, Precursor をうけとる
	// 内部のアノテーション情報と一致するか否か調べる。
	// checkonly (T/F) T : 該当アノテーションの有無を調べる。/ F ＋MassBankRecordとして出力を得る。
	const std::string checkFitAnnoInfo(
		const std::vector<double>& mzArr,
		const double rt, 
		const double prc, 
		const bool checkonly
		);

	// Spectraから、Annotationを適合させる
	void doSpectrumFitAnnotation(const std::vector<kome::objects::Spectrum*>& specArr);

	// 使われないアノテーションがあるかどうか調べる。
	// ある場合は情報を開示。問い合わせる。
	const bool isNotUseAnnotationElements();

private:

	// 内部カウンタをクリアする。
	void onClearCnt();
	// アノテーション情報(Formula等）の部分を生成して返す。
	const std::string onGetAnnotationBlock(
		const double rt, 
		const AnnoElem& elm
		);

	// dstL = src - tol, dstH = src + tol として返す。ppm = true時は tolをPPMの要素として扱う。
	void toTolval(
		const double src, 
		const double _tol, 
		double& dstL, 
		double& dstC, 
		double& dstH, 
		bool ppm = false
		);

	const std::string onNotUsedInfo(const AnnoElem& elm);

	std::vector<AnnoElem> annoArr_;
};

} // end of namespace
}
}

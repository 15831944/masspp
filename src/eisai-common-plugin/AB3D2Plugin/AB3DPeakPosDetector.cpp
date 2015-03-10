/*********************************************************/
/*!
 @addtoGroup AB3D
 @file AB3DPeakPosDetector.h
 @brief AB3D peak position detect Functions.
 
 @date 2013.07.01
 @author M.Fukuda
 @varsion 1.0.0

 -------------------------------------------------------
 Copyright(C) 2006-2014 
 Eisai Co., Ltd. All rights reserved.
 -------------------------------------------------------
 @note
 */
/*********************************************************/

// AB3DPeakDetectors....

#include "stdafx.h"
#include "AB3DPeakPosDetector.h"

// include AB3D Parts
#include "NewAB3DLabeling.h"

// include DB Controler
#include "DBTools\DbController.h"
#include "DBTools\tempContainer.h"

// include progress controler
#include "progress\progressMatrix.h"

#define AB3D_SHORT_NAME "ab3d2"

// -------------------------------------------------------*
//    Inner Structs
// -------------------------------------------------------*
// 最小/最大の閾値を保存する構造体
namespace AB3D{
struct tolerance {
	double lo;
	double hi;
	// デフォルトコンストラクタ
	tolerance(){}
	// コンストラクタで範囲を計算する
	tolerance(const double val, const double tol) 
		: lo(val - tol)
		, hi(val + tol){}
};

// Peaks Rowの値を保存する構造体
// *TmpPeakValはTempContiner.h　で記述。
struct stRows {
	// Rowに表示される
	double mz;
	double rt;
	int charge;
	// 閾値内外の比較時に使用
	tolerance mztol; 
	tolerance rttol; 
	// Peaks Rowの子の要素
	std::vector<AB3D::TmpPeakVal*> vals;

	//constructor
	stRows(){}

	//constructor
	stRows(
		const double _mz, 
		const double _rt, 
		const int _cg
		) : mz(_mz)
		, rt(_rt)
		, charge(_cg){}
};
}
// -------------------------------------------------------*
//    Main Class & Functions
// -------------------------------------------------------*
// constructor
Ab3dPeakPosDetect::Ab3dPeakPosDetect(
	const double mzppm,
	const double rttol,
	std::map<std::string, std::string>& setmap,
	kome::objects::SettingParameterValues* settings
) : kome::ident::PeakPositionsDetector(true, false)
	, mzppm_(mzppm)
	, rttol_(rttol)
	, setmap_(setmap)
	, settings_(settings)
	, peaksPtr_(nullptr)
{
}

// destructor
Ab3dPeakPosDetect::~Ab3dPeakPosDetect(){
}

int Ab3dPeakPosDetect::onDetectPeakPositions(
	kome::ident::SampleGetter& samplegetter,
	kome::ident::PeakPositions& peakPos,
	kome::core::Progress& progress
){
	// DB Object
	std::unique_ptr<AB3D::DbController> dbctrl(new AB3D::DbController);
	std::string errmsg = "";

	// check DB Status
//	if (dbctrl->checkDBStatus() && (dbctrl->setupTempTables() != true)){
	if (!dbctrl->setupTempTables()){
		// TempTableが作れないので準備できない
		errmsg = "Failed to access database.";
	}
	
	// AB3Dの実行
	int fx = executeAB3D(dbctrl.get(), samplegetter, progress, errmsg);

	// Matrix作成の実行
	fx = makePeakPosData(dbctrl.get(), peakPos, progress,errmsg);

	setLastError(errmsg.c_str());
	return (HAS_RT | HAS_MZ | HAS_CHARGE);
}

kome::objects::SettingParameterValues* Ab3dPeakPosDetect::getSettings()
{
	return settings_;
}

// ------------------------------------------------------*
//   Execute AB3D at Matrix Mode
// ------------------------------------------------------*
int Ab3dPeakPosDetect::executeAB3D(
	AB3D::DbController* dbctrl,
	kome::ident::SampleGetter& samplegetter,
	kome::core::Progress& progress,
	std::string& errmsg
){
	int ret = 0;//(正常)

	//AB3D object
	NewAB3DLabeling ab3d(setmap_);

	// Progress をスタート
	MatrixProg prog(&progress, samplegetter.getNumberOfSamples());

	kome::objects::Sample* sample = nullptr;
	kome::operation::Alignment* align = nullptr;

	while((sample = samplegetter.next()) != NULL && !progress.isStopped()) {

		const bool isSupportedAPI = sample->isSupportedAPIPeaks();

		kome::operation::Alignment* align = samplegetter.getAlignment();
		kome::objects::DataSet dataSet;
		kome::objects::DataGroupNode* dgn = sample->getRootDataGroupNode();
		// DataSetが不正な場合をチェックできるならここで行う。
		if(dgn != nullptr) {
			dgn->getDataSet(&dataSet);
		}
		
		unsigned int cnum = dgn->getNumberOfSpectra();
		for(unsigned int i = 0; i < cnum; i++) {
			kome::objects::Spectrum* spec = dgn->getSpectrum(i);
			double rt = spec->getRt();
			if(align != NULL) {
				double convertedRt = align->convertRt(rt,spec->getMinX());
				spec->setRt(convertedRt);
			}
		}

		// MatrixでのSampleIdを記録する
		dbctrl->setSampleId(samplegetter.getMatrixSampleId());

		// Progressをセットする
		kome::core::Progress* subProgress = prog.getSubProgress();
		ab3d.bindProgress(subProgress);

		// AB3Dを実行。
		ab3d.doAB3D(dataSet, dbctrl);
	}
	if(progress.isStopped()) {
		errmsg = "Progress stopped.";
		ret = 1;
	}

	prog.closeMe();

	return ret;
}

// Peak PositionData に　AB3Dの解を入力する
int Ab3dPeakPosDetect::makePeakPosData(
	AB3D::DbController* dbctrl,
	kome::ident::PeakPositions& peakPos,
	kome::core::Progress& progress,
	std::string& errmsg
){
	std::vector<AB3D::TmpPeakVal*> tmpPeaks;
	std::vector<AB3D::TmpPeakVal*> errPeaks;
	
	// Temp Tableから Charge > -2 のPeak情報を、Valueの降順で取得する。
	// TmpPeakValは内部で[New]されて戻る。
	if(dbctrl->getMatchDataFromTemp(tmpPeaks)) {

		std::vector<AB3D::stRows> rowArr; 
		// 答えを振り分ける
		for each(auto p in tmpPeaks) {
			if(!parsePeaksToMatrix(p, rowArr)) {
				errPeaks.push_back(p);
			}
		}
		peaksPtr_.reset(new kome::objects::Peaks);

		for each(auto sr in rowArr) {
			int peakIdx = peakPos.addPeakPosition(sr.rt, sr.mz, sr.charge);

			for each(auto p in sr.vals) {
				AB3D::TmpMXVal* mx = &p->mxval;
				// TempTableからXICのDATAPOINTを取得
				kome::core::DataPoints dps;
				dbctrl->getDataPointsFromTemp(mx->profId, dps);
				// Peak情報の生成
				kome::objects::PeakElement* peak = peaksPtr_->createPeak(mx->core.x,mx->core.y);
				peak->setLeft(mx->left.x, mx->left.y);
				peak->setRight(mx->right.x, mx->right.y);

				// Test Code
				kome::core::XYData* xydata = &dps;
				long dpsnum = dps.getLength();
				xydata->getMaxX();
				peakPos.setPeakValueInfo(peakIdx, p->sampleId, mx->value, *xydata ,peak,true);
				// 上の時点でDB@Profileに書き込まれるので dpsの寿命はここで尽きて良い
			}
		}
	}
	// NewされたオブジェクトをDeleteする。
	for each(auto tpv in tmpPeaks){delete tpv;}
	return true;
}

// AB3Dの結果を Row - Peak Valueの構造になるように振り分ける
const bool Ab3dPeakPosDetect::parsePeaksToMatrix(
	AB3D::TmpPeakVal* pt,
	std::vector<AB3D::stRows>& rowArr
){
	std::vector<AB3D::stRows>::iterator it = rowArr.begin();
	std::vector<AB3D::stRows>::iterator eit = rowArr.end();
	for(; it != eit; ++it) {
		// 登録済みPeakの閾値を判定
		if(isInRowTolerance(pt, *it)) {
			//閾値内に要素がある
			if(isSameIdInRow(pt->sampleId, *it)) {
				// 既に同じSampleIDがある:「登録しない」
				return false;
			} else {
				// まだ子に無いSampleID。子として「登録」
				it->vals.push_back(pt);
				return true;
			}
		}
	}
	//登録済みのROWと一致せず。新規要素として登録する。
	rowArr.push_back(createStRow(pt));
	return true;
}

// 対象が rowの閾値と同じか否かを返す
const bool Ab3dPeakPosDetect::isInRowTolerance(
	AB3D::TmpPeakVal* p,
	const AB3D::stRows& row
){
	if (p->charge == row.charge) {
		// m/zの閾値内
		if (row.mztol.lo <= p->mz && p->mz <= row.mztol.hi) {
			// rtの閾値内
			if (row.rttol.lo <= p->rt && p->rt <= row.rttol.hi) {
				return true;
			}
		}
	}
	return false;

	//// Chargeが
	//if (p->charge != row.charge) {
	//	return false;
	//}
	//// m/zの閾値から外れるかを判定
	//if (p->mz < row.mztol.lo || row.mztol.hi < p->mz) {
	//	return false;
	//}
	//// rtの閾値から外れるかを判定
	//if (p->rt < row.rttol.lo || row.rttol.hi < p->rt) {
	//	return false;
	//}
	//// 閾値内で且つChargeが同じ。故に一致。
	//return true;
}

//　既に同じSampleIDがあれば True
const bool Ab3dPeakPosDetect::isSameIdInRow(
	const int tgtId, 
	const AB3D::stRows& tgtRow
){
	for each(auto& t in tgtRow.vals) {
		if(t->sampleId == tgtId) return true;
	}
	return false;
}

// stRowを生成する
AB3D::stRows Ab3dPeakPosDetect::createStRow(
	AB3D::TmpPeakVal* p
){
	AB3D::stRows rst(p->mz, p->rt, p->charge);
	double mztol = p->mz * 0.000001 * mzppm_;
	// 閾値判定用の範囲を計算して保持する。
	rst.mztol = AB3D::tolerance(p->mz, mztol);
	rst.rttol = AB3D::tolerance(p->rt, rttol_);
	rst.vals.push_back(p);
	return rst;
}

/**********************************************************/
/*! @addtogroup MRM3DPlugin 
    @file       MRM3DPeakDetector.h
    @brief      Filter Parser Functions Header
    @date       2012.10.24
	@author     M.Fukuda
	@version    1.0.0
	
	Copyright(C) 2006-2013 Eisai Co., Ltd. 
	                       All rights reserved.
 **********************************************************
2012.10.24
MRM3D Peak Pickingのメインクラス
*/
/**********************************************************/
#pragma once

// 前方宣言
namespace mspp{
	namespace sprt{
		class PeakDetector;
		class PeakSmoother;
	}
}

namespace kome{
namespace mrm3d{
	
class MRMChromKeeper;
class MRMChrom;
class ParamParser;

/*!--------------------------------------------------------
    @name   MRM3DCore
    @breif 	MRM3DPeakPicking main Class
	@date   2012.10.23
	@par	ChromatogramとしてMRMデータを抽出
	        Smoothing, Peak Picking, Filteringを行い、
			Peaks2d　として　結果を返す。
-----------------------------------------------------------*/
class MRM3DCore {

private:
	/*! @emum
	 *  @brief MRM3D Step Number
	 */
	enum MRM3DStep {
		CONVERT,   // Convert step
		PEAKPICK,  // PeakPicking step
		STEP_NUM,
	};
	/*! type def */
	typedef std::shared_ptr<MRMChrom> tMRMChromPtr;
	typedef std::vector<tMRMChromPtr> tvMRMPtr; /* t(ypedef) v(ector) の接頭辞*/

	/*! my Parameters*/
	kome::objects::SettingParameterValues* m_settings;
	ParamParser* m_parser;

public:
	explicit MRM3DCore( kome::objects::SettingParameterValues* settings );
	~MRM3DCore();
private:
	// copyの類禁止・防止
	MRM3DCore(const MRM3DCore& m) {}
	MRM3DCore& operator=(const MRM3DCore& rhs){}

public:
	/*!
	 * @brief execute mrm3d Peak Picking
	 * @param[in]  kome::objects::DataSet& dataSet Objects
	 * @param[in]  kome::core::Progress& progress objects pointer
	 * @param[out] kome::objects::Peaks2D& results 2D Peaks
	 */
	void executeLabeling (
		kome::objects::DataSet& dataSet,
		kome::core::Progress& progress,
		kome::objects::Peaks2D& peaks);

private:

	/*!
	 * @brief check having mrm filter infomations.
	 * @param[in]  kome::objects::DataSet& dataSet Objects
	 * @retval true Is Having MRM Infomation
	 * @note check spectrum[0] having filter info only.
	 */
	const bool isHavingMRMType (kome::objects::DataSet& dataSet);

	/*!
	 * @brief get MRMData From DataObjects
	 * @param[in]  kome::objects::DataSet& サンプルのデータセット
	 * @param[in]  kome::core::Progress& 　getMRMDataフェーズのプログレスポインタ
	 * @param[out] std::vector<std::shared_ptr<MRMChrom>> MRMData取得結果のMRMChrom配列
	 * @retval true Success get MRM Infomation
	 * @par  各spectrumを取り出し、MRMChromにデータを集積する（下記関数で行う)
	 *       MRMChromの配列として返す。
	 */
	const bool getMRMDatas (
		kome::objects::DataSet& dataSet,
		kome::core::Progress& progress,
		tvMRMPtr& allChroms);

	/*!
	 * @brief divide xydata to any MRMchroms
	 * @param[in]     kome::objects::Spectrum* : parse Spectrum
	 * @param[in out] MRMChromKeeper*& : set MRMChrom XYData from Spectrum
	 * @par MRMChromKeeperの各MRMChromが持つ　Q3　の範囲内に
	 *      Spectrum が持つXYDataの該当ポイントがあれば和算する。
	 *      各MRMChromは SpecのRT : X, Q3範囲内の合計 intensity : Y となる。
	 */
	void divideXYDataPoints (
		kome::objects::Spectrum* spectrum,
		MRMChromKeeper*& stocker);

	/*!
	 * @brief Detect Chromatgram Peaks
	 * @param[in]  std::shared_ptr<MRMChrom>&
	 * @param[in]  mspp::sprt::PeakDetector*
	 * @param[in]  mspp::sprt::PeakSmoother*
	 * @param[out] kome::objects::Peaks2D& results 2D Peaks
	 * @par MRMChromからXYDataを構成し、
	 *      Smoothing, PeakPickingを行い 
	 *      Peaks2Dに結果を追加する。
	 */
	void detectAndConvertPeaks (
		tMRMChromPtr& chromPtr,
		mspp::sprt::PeakDetector* pkFunc,
		mspp::sprt::PeakSmoother* pksmth,
		kome::objects::Peaks2D& peaks2d);

	/*!
	 * @brief convert from MRMChromData to xydata
	 * @param[in]  std::shared_ptr<MRMChrom>&
	 * @param[out] kome::core::XYData*
	 * @retval true Success convert
	 * @note Mass++Pluginでsmoothing,PeakPickをする為、
	 *       標準のデータ形式にコンバートする。
	 */
	const bool convertToXYData (
		tMRMChromPtr& chromPtr,
		kome::core::XYData* xydata);

	/*!
	 * @brief convert from Peaks to Peaks2D
	 * @param[in]  std::shared_ptr<MRMChrom>&
	 * @param[in]  kome::core::XYData&
	 * @param[out] kome::objects::Peaks2D& results 2D Peaks
	 * @retval true Success convert
	 * @note Peaks(平面の結果)を　Q1,Q3の情報を加味して
	 *       2DPeaksのデータ形式にコンバートする。
	 */
	const bool convertTo2DPeaks (
		tMRMChromPtr& chromPtr,
		kome::objects::Peaks& tmpPeaks,
		kome::objects::Peaks2D& peaks2d);

	/*!
	 * @brief convert from Peaks to Peaks2D
	 * @param[in-out] kome::objects::Peaks2D& results 2D Peaks
	 * @retval true Success Filtered
	 */
	const bool FilterPeak2D (
		kome::objects::Peaks2D& peaks2d);

};


}
}

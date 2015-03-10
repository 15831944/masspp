/**********************************************************
/*! @addtogroup ANYPLUGINS(FUTURE)
    @file       AssistTool.h
    @brief      PluginCallTools Helper Functions Header
    @date       2012.10.16
	@author     M.Fukuda
	@version    1.0.0
	@note 
	
	Copyright(C) 2006-2013 Eisai Co., Ltd. 
	                       All rights reserved.
**********************************************************
	@par   
	Mass++のPluginCall処理様式をクラス・カプセル化する。
	半定型化しているPluginCall一連の処理を統合する為。

	クラスをParamNameと共に作成する。
	SetUpフェーズで settingを解析して内容を取得。
	実行の可否を判定する(コンストラクタ中ではすべきではない為)
	invokeで実行させる。
**********************************************************/
#pragma once
#include <string>

namespace mspp {
namespace sprt {

/*!--------------------------------------------------------
    @name   PeakDetector
    @breif 	PluginCallTool の　Peak Detector　の　簡略化支援クラス。
	@date   2012.10.23
	@par	pluginCallToolを使った半定型文を省略するクラス
	        Setup で SeetingParameterValuesの中身を切り出す。
			inovkeで 実行する。
			各々不適切であれば falseを返す。
-----------------------------------------------------------*/
/*! MASSPP UNIZON */
const std::string specFunc("SPEC_PEAK_PICKING");
const std::string chromFunc("CHROM_PEAK_PICKING");

class PeakDetector {
private:
	typedef kome::objects::SettingParameterValues tSetParam;

	const std::string paramName_; 	            /*! param name(From setting.xml data) */
	kome::plugin::PluginFunctionItem* m_func; 	/*! assigned peak detect function */
	tSetParam* m_setParams; 	                /*! peak detect function's param */
public:	
	/*! @breif constructor
	 *  @param[in] string : xml setting value name 
	 */
	explicit PeakDetector(const std::string& XMLparamname);

	/*! destructor */
	~PeakDetector();

	/*!
	 * @brief setup Class inner status.
	 * @param[in]  kome::objects::SettingParameterValues* : setting parameter object pointer
	 * @param[in]  bool : Call Peak Detector Type T/Chomatogram F/Spectrum
	 * @retval true Successed setup.
	 */
	const bool setUp(tSetParam* settings, bool isChrom);

	/*!
	 * @brief invoke set up peak detector
	 * @param[in]  kome::objects::XYData& : peaks XYData object
	 * @param[out] kome::ocjects::Peaks&  : detected result peaks objects  
	 * @retval true Successed peak picking
	 */
	const bool invokeMe(kome::core::XYData& xyData, kome::objects::Peaks& peaks);
};

/*!--------------------------------------------------------
    @name   PeakSmoother
    @breif 	Peak Smoothing　の　簡略化支援クラス。
	@date   2012.10.24
	@par	pluginCallToolを使った半定型文を省略するクラス
	        Setup で SeetingParameterValuesの中身を切り出す。
			inovkeで 実行する。
			各々不適切であれば falseを返す。
-----------------------------------------------------------*/
/*! MASSPP UNIZON */
const std::string kXYDataSmooth("XYDATA_FILTER");

class PeakSmoother {
private:
	typedef kome::objects::SettingParameterValues tSetParam;

	const std::string paramName_; 	            /*! param name(From setting.xml data) */
	kome::plugin::PluginFunctionItem* m_func; 	/*! assigned peak detect function */
	tSetParam* m_setParams; 	                /*! peak detect function's param */
public:
	/*! @breif constructor
	 *  @param[in] string : xml setting value name 
	 */
	explicit PeakSmoother(const std::string& XMLparamname);
	
	/*! destructor */
	~PeakSmoother();

	/*!
	 * @brief setup Class inner status.
	 * @param[in]  kome::objects::SettingParameterValues* : setting parameter object pointer
	 * @retval true Successed setup.
	 */
	const bool setUp(tSetParam* settings);

	/*!
	 * @brief invoke smoothing to XYData
	 * @param[in.out]  kome::objects::XYData& : peaks XYData object
	 * @retval true Successed smoothing
	 * @note replace orginal data.
	 */
	const bool invokeMe(kome::core::XYData*& srcXY);

	/*!
	 * @brief invoke smoothing from src to dst
	 * @param[in]  kome::objects::XYData& : src peaks XYData object
	 * @param[out] kome::ocjects::XYData& : dst peaks XYData object
	 * @retval true Successed smoothing
	 */
	const bool invokeMe(kome::core::XYData& srcXY, kome::core::XYData& dstXY);
};

/*!--------------------------------------------------------
    @name   progress Keeper
    @breif 	Progress　の　簡略化支援クラス。
	@date   2012.10.23
	@par	Renge作成する。
	        nextStep で　Position&Status変更する。
			デストラクタでProgressをFillして〆る。
-----------------------------------------------------------*/
class ProgressKeeper {
private:
	kome::core::Progress& progress_; /*! 管理するProgress */
	std::string status_;             /*! 表示メッセージ */
	unsigned int maxcount_;          /*! カウントの最大値 */
	unsigned int totalcount_;        /*! 総カウント数 */
	unsigned int clockcount_;        /*! サイクル単位のカウント */
	unsigned int clockbasecount_;    /*! サイクルの最大カウント */
	bool isEnd_;
public:
	ProgressKeeper (
		kome::core::Progress& progress, 
		unsigned int maxcount, 
		const std::string& statusMsg);
	// オブジェクト終了時にFILLしてクローズさせる
	~ProgressKeeper();
	// 次に進める。
	void showNext();
	// 強制終了。
	void doFinish();
};

} // end of namespace
}

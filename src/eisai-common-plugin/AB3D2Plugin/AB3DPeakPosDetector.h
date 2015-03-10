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

#pragma once

// ëOï˚êÈåæ
namespace AB3D {
	class DbController;
	struct TmpPeakVal;
	struct stRows;
}

class Ab3dPeakPosDetect : public kome::ident::PeakPositionsDetector {
public:
	/**
	* @fn MrmPeakPosDetect( boool bCalcFlg, bool bAlignmentFlg )
	* @brief constructor
	*/
	Ab3dPeakPosDetect(
		const double mzppm,
		const double rttol,
		std::map<std::string, std::string>& setmap,
		kome::objects::SettingParameterValues* settings
		);

	/**
	* @fn virtual ~MrmPeakPosDetect()
	* @brief destructor
	*/
	virtual ~Ab3dPeakPosDetect();

private:
	// Inner parameters
	double mzppm_;
	double rttol_;
	std::map<std::string, std::string> setmap_;

	std::unique_ptr<kome::objects::Peaks> peaksPtr_;

	kome::objects::SettingParameterValues* settings_;

public:
	int onDetectPeakPositions(
		kome::ident::SampleGetter& samplegetter, 
		kome::ident::PeakPositions& peakPos,
		kome::core::Progress& progress);

public:
	kome::objects::SettingParameterValues* getSettings();

private:
	int executeAB3D(
		AB3D::DbController* dbctrl,
		kome::ident::SampleGetter& samplegetter,
		kome::core::Progress& progress,
		std::string& errmsg);




	const bool parsePeaksToMatrix(
		AB3D::TmpPeakVal* pt,
		std::vector<AB3D::stRows>& rowArr);

	AB3D::stRows createStRow(
		AB3D::TmpPeakVal* pt);

	const bool isInRowTolerance(
		AB3D::TmpPeakVal* pt, 
		const AB3D::stRows& tgtRow);

	const bool isSameIdInRow(
		const int tgtId, 
		const AB3D::stRows& tgtRow);

	int makePeakPosData(
		AB3D::DbController* dbctrl,
		kome::ident::PeakPositions& peakPos,
		kome::core::Progress& progress,
		std::string& errmsg);

};

/**
 * @file OverlaySpectrumTool.h
 * @brief èdÇÀèëÇ´ï\é¶ÇÃä÷êî
 *
 * @author M.Fukuda
 * @date 2009.06.24
 *
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */
#pragma once

class OverlaySpectrumTool {
public :
	OverlaySpectrumTool(){}
	~OverlaySpectrumTool(){}

	/**
	* @fn bool setToDrawToActiveRecord( kome::img::Graphics& g, kome::objects::Spectrum* spec )
	* @brief draws Active Canvas to target Graphics
	* @param g graphics object to draw
	* @param called Spectrum object (to use calc scale )
	*/
	bool setToDrawToActiveRecord(
		kome::img::Graphics& g,
		kome::objects::Spectrum* spec,
		kome::core::XYData* activeSpecXYData
		);

	void drawProfilePeaks(
		kome::img::Graphics& g, 
		kome::core::XYData* activeSpecXYData);

private:
	/**
	* @brief draws record peaks
	* @param g graphics object to draw
	* @param scale the scale of y-axis
	*/
	void drawRecordPeaks(
		kome::img::Graphics& g, 
		kome::core::XYData* activeSpecXYData,
		const double scale);
};

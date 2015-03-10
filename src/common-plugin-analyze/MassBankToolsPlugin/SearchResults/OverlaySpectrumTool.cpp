/**
 * @file OverlaySpectrumTool.cpp
 * @brief d‚Ë‘‚«•\Ž¦‚ÌŠÖ”
 *
 * @author M.Fukuda
 * @date 2009.06.24
 *
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#include "stdafx.h"
#include "OverlaySpectrumTool.h"

// --- Draw Peaks -------------------------------------------------------------------------------

// Draw Peaks If Checked Over Write Mode
bool OverlaySpectrumTool::setToDrawToActiveRecord(
	kome::img::Graphics& g,
	kome::objects::Spectrum* spec,
	kome::core::XYData* activeSpecXYData
) {
	if ( spec == NULL ) {
		return false;
	}
	
	double scale = 1.0;
	kome::core::XYData* xyData = spec->getXYData();
	if( xyData != NULL && activeSpecXYData != NULL ) {
		scale = std::max(0.01, xyData->getMaxY()) / std::max(0.01, activeSpecXYData->getMaxY());
	}

	// color
	COLOR oldColor = g.getColor();
	if( !g.isUnusedDefaultColor() ){
		g.setColor( kome::img::GraphTool::getColorFromIni("Drawing", "COLOR_MASSBANK_SPEC", "red"));
	}
	// draw
	drawRecordPeaks(g, activeSpecXYData, scale);

	// Reset old color
	g.setColor( oldColor );

	return true;
}

void OverlaySpectrumTool::drawProfilePeaks(
	kome::img::Graphics& g, 
	kome::core::XYData* activeSpecXYData
){
	// color
	COLOR oldColor = g.getColor();
	if( !g.isUnusedDefaultColor() ){
		g.setColor( kome::img::GraphTool::getColorFromIni("Drawing", "COLOR_MASSBANK_SPEC", "red"));
	}

//	g.setColor( kome::img::ColorTool::RED );

	// draw peaks
	drawRecordPeaks(g,activeSpecXYData,1.0);
	// restore color
	g.setColor( oldColor );
}

// draw peaks
void OverlaySpectrumTool::drawRecordPeaks(
	kome::img::Graphics& g, 
	kome::core::XYData* xyData,
	const double scale
) {
	// xy data
	if( xyData == NULL ) {
		return;
	}

	// index
	int startIdx = xyData->searchIndex( g.getMinX() );
	if( startIdx < 0 ) {
		startIdx = - startIdx - 1;
	}

	int endIdx = xyData->searchIndex( g.getMaxX() );
	if( endIdx < 0 ) {
		endIdx = - endIdx - 2;
	}

	// draw peaks
	double refpx, refpy;
	int px,py;
	int gxleft = g.getGraphPositionLeft();
	int gxright = g.getGraphPositionRight();
	int gxbottom = g.getGraphPositionBottom();

	for( int i = startIdx; i <= endIdx; i++ ) {
		g.transformDataToPosition(xyData->getX(i), xyData->getY(i) * scale, &refpx, &refpy);

		px = roundnum(refpx);
		py = roundnum(refpy);

		if(px >= gxleft && px <= gxright && py <= gxbottom) {
			g.drawLine(px, py, px, gxbottom);
			g.drawLine(px+1, py, px+1, gxbottom);
		}
	}
}

/**
 * @file ResultCanvas.cpp
 * @brief implements of ResultCanvas class
 *
 * @author S.Tanaka
 * @date 2007.08.22
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "ResultCanvas.h"
#include "AlignmentManager.h"


using namespace kome::alignment;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



BEGIN_EVENT_TABLE( ResultCanvas, CanvasBase )
END_EVENT_TABLE()


// constructor
ResultCanvas::ResultCanvas( wxWindow* parent )
		: CanvasBase( parent, -1, -1 ) {
	m_xLabelFlg = true;
	m_yLabelFlg = false;
	m_xGridFlg = false;
	m_yGridFlg = false;
	m_posFlg = true;
	m_baseZeroFlg = true;
}

// destructor
ResultCanvas::~ResultCanvas() {
}

// get chromatogram
void ResultCanvas::getChromatograms(
		std::vector< kome::objects::Chromatogram* >& chroms,
		std::vector< COLOR >& colors
) {
	// manager
	AlignmentManager& mgr = AlignmentManager::getInstance();

	// control chromatogram
	kome::objects::Chromatogram* ctrlChrom = mgr.getResultControlChrom();
	if( ctrlChrom != NULL ) {
		chroms.push_back( ctrlChrom );
		colors.push_back( kome::img::ColorTool::GREEN );
	}

	// sample chromatogram
	kome::objects::Chromatogram* smplChrom = mgr.getResultTreatmentChrom();
	if( smplChrom != NULL ) {
		chroms.push_back( smplChrom );
		colors.push_back( kome::img::ColorTool::BLUE );
	}
}

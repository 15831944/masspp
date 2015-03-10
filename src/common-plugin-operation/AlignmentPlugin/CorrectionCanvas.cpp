/**
 * @file CorrectionCanvas.cpp
 * @brief implements of CorrectionCanvas class
 *
 * @author S.Tanaka
 * @date 2007.08.22
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "CorrectionCanvas.h"
#include "AlignmentManager.h"


using namespace kome::alignment;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG




BEGIN_EVENT_TABLE( CorrectionCanvas, CanvasBase )
END_EVENT_TABLE()


// constructor
CorrectionCanvas::CorrectionCanvas( wxWindow* parent )
		: CanvasBase( parent, -1, -1 ) {
	m_xLabelFlg = true;
	m_yLabelFlg = true;
	m_xGridFlg = true;
	m_yGridFlg = true;
	m_posFlg = false;
	m_baseZeroFlg = false;
}

// destructor
CorrectionCanvas::~CorrectionCanvas() {
}

// get chromatograms
void CorrectionCanvas::getChromatograms(
		std::vector< kome::objects::Chromatogram* >& chroms,
		std::vector< COLOR >& colors
) {
	// manager
	AlignmentManager& mgr = AlignmentManager::getInstance();

	// correction chromatogram
	kome::objects::Chromatogram* chrom = mgr.getCorrectionDummyChrom();

	if( chrom != NULL ) {
		chroms.push_back( chrom );
		colors.push_back( kome::img::ColorTool::RED );
	}
}

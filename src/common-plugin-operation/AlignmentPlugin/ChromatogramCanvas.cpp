/**
 * @file ChromatogramCanvas.cpp
 * @brief implement of ChromatogramCanvas class
 *
 * @author S.Tanaka
 * @date 2007.08.20
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "ChromatogramCanvas.h"
#include "AlignmentManager.h"


using namespace kome::alignment;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



BEGIN_EVENT_TABLE( ChromatogramCanvas, CanvasBase )
END_EVENT_TABLE()


// constructor
ChromatogramCanvas::ChromatogramCanvas( wxWindow* parent, const int width, const int height, const bool ctrl )
		: CanvasBase( parent, width, height ), m_ctrlFlg( ctrl ) {
	m_xLabelFlg = true;
	m_yLabelFlg = false;
	m_xGridFlg = false;
	m_yGridFlg = false;
	m_posFlg = true;
	m_baseZeroFlg = true;
	m_title = m_ctrlFlg ? "Control" : "Treatment";
}

// destructor
ChromatogramCanvas::~ChromatogramCanvas() {
}

// get chromatograms
void ChromatogramCanvas::getChromatograms(
		std::vector< kome::objects::Chromatogram* >& chroms,
		std::vector< COLOR >& colors
) {
	// manager
	AlignmentManager& mgr = AlignmentManager::getInstance();

	// control chromatogram
	kome::objects::Chromatogram* ctrlChrom = mgr.getControlChrom();
	if( m_ctrlFlg && ctrlChrom == NULL ) {
		return;
	}

	// sample chromatogram
	kome::objects::Chromatogram* smplChrom = mgr.getTreatmentChrom();
	if( !m_ctrlFlg && smplChrom == NULL ) {
		return;
	}

	// add
	if( ctrlChrom != NULL ) {
		chroms.push_back( ctrlChrom );
		colors.push_back( m_ctrlFlg ? kome::img::ColorTool::GREEN : kome::img::ColorTool::WHITE );
	}	
	if( smplChrom != NULL ) {
		chroms.push_back( smplChrom );
		colors.push_back( m_ctrlFlg ? kome::img::ColorTool::WHITE : kome::img::ColorTool::BLUE );
	}
}

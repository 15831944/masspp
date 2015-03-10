/**
 * @file PeakEditOperation.cpp
 * @brief Peak Edit Operation class
 *
 * @author M.Izumi
 * @date 2013.05.28
 * 
 * Copyright (C) 2014 Shimadzu Corporation All rights reserved.
 */

#include "stdafx.h"
#include "PeakEditOperation.h"
#include "PeakEditManager.h"

using namespace kome::peaks::edit;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
PeakEditOperation::PeakEditOperation(){
	m_spec = NULL;
	m_chrom = NULL;
	m_graphics = NULL; 
	m_mouseEvt = NULL;
	m_flgStartEdirSpec = -1;
}

// destructor
PeakEditOperation::~PeakEditOperation(){
}

// on execute
bool PeakEditOperation::onExecute(){
	bool bflg = false;

	std::string shortName = getShortName();

	if( shortName.compare( "spec_peak_edit" ) == 0 ){

		bflg = specEditPeaks();		

	}else if( shortName.compare("chrom_peak_edit") == 0 ){

		bflg = chromEditPeaks();
	}

	return bflg;
}

// on save condition
void PeakEditOperation::onSaveCondition( boost::function< int ( void*, int ) > writeFun ){
	// managers
	kome::objects::PeaksManager& pkMgr = kome::objects::PeaksManager::getInstance();

	kome::core::XYData* xyData = NULL;
	kome::objects::Peaks* peaks  = NULL;
	// spec
	if( m_spec != NULL ){
		int specId = m_spec->getOrgSpectrum()->getId();
		writeFun( &specId, sizeof( specId ) );
		xyData = m_spec->getXYData();

		peaks = pkMgr.getPeaks( m_spec );
	}

	// chrom
	if( m_chrom != NULL ){
		int chromId = m_chrom->getOrgChromatogram()->getId();
		writeFun( &chromId, sizeof( chromId ) );
		kome::core::XYData* xyData = m_chrom->getXYData();
		
		peaks = pkMgr.getPeaks( m_chrom );
	}
	
	if( xyData != NULL ){
		xyData->exportData( writeFun );
	}
	
	if( peaks != NULL ){
		peaks->exportData( writeFun );
	}
}

// on load condition
void PeakEditOperation::onLoadCondition( boost::function< int ( void*, int ) > readFun ){
	// managers
	kome::objects::PeaksManager& pkMgr = kome::objects::PeaksManager::getInstance();

	int id = int();
	readFun( &id, sizeof( id ) );

	kome::objects::Sample* sample = getTargetSample();
	kome::objects::Chromatogram* chrom = NULL;
	kome::objects::Spectrum* spec = NULL;

	if( sample != NULL ){
		chrom = sample->getChromatogramById( id );
	}

	// chromatogram
	if( chrom != NULL && m_chrom != NULL ){		
		if( chrom->getOrgChromatogram() == m_chrom->getOrgChromatogram() ){
		}
	}

	// spectrum
	spec = sample->getSpectrumById( id );
	if( spec != NULL && m_spec != NULL ){
		if( spec->getOrgSpectrum() == m_spec->getOrgSpectrum() ){
		}
	}
}

// on get description
std::string PeakEditOperation::onGetDescription(){
	std::string s;
	
	s.append( "Edit Peaks " );

	if( m_spec != NULL ){
		s.append( FMT("[%s]", m_spec->getName() ) );
	}
	if( m_chrom != NULL ){
		s.append( "[%s]", m_chrom->getName() );
	}

	return s;
}

// on get parameters string
std::string PeakEditOperation::onGetParametersString(){
	std::string s;

	return s;
}

// on set parameters string
void PeakEditOperation::onSetParametersString( const char* strParam ){
}

void PeakEditOperation::deletePeak( int call ){
	PeakEditManager& mgr = PeakEditManager::getInstance();

	// @date 2011/10/14 <Add> OKADA ------->
	if( mgr.isInDeleteArea( *m_graphics, *m_mouseEvt, call ) ){
		if( mgr.m_peakOnClick == mgr.getSelectedPeak() ){
			mgr.deletePeak(); // エリア削除処理
			mgr.m_flgRedrawMemory = true;
		}
	}
	mgr.m_peakOnClick = NULL;
	// @date 2011/10/14 <Add> OKADA <-------

}

// set spec edit peaks
bool PeakEditOperation::specEditPeaks(){
	PeakEditManager& mgr = PeakEditManager::getInstance();
	deletePeak( CALL_FROM_SPECTRUM );
	bool bflg = true; 
		
	if( mgr.getInAddFlag() == 1 ){
		mgr.setModeEdit();
		bflg = mgr.finishToAddPeak( *m_spec, *m_graphics, *m_mouseEvt );
		
		// update	// SPEC 84518
		if( m_spec != NULL ) {
			kome::plugin::PluginCallTool::onUpdateSpecPeaks( *m_spec );
		}

		// 幅0のピークの手動作成を抑制する	// SPEC 84519
		kome::objects::PeakElement* peak = mgr.getSelectedPeak();
		if( peak != NULL ){
			double dR = peak->getRightX();
			double dL = peak->getLeftX();
			double dWidth = dR - dL;
			int iii = 0;
			if( abs(dWidth) < 0.000001 ){
				// delete
				mgr.deletePeak();
			}
		}
		mgr.setInAddFlag( 2 );
	}

	if( m_flgStartEdirSpec == 1 ){
		m_flgStartEdirSpec = 0;

		// finish to edit
		bflg = mgr.finishToEditPeak( *m_spec, *m_graphics, *m_mouseEvt );
	}

	return bflg;
}

// chrom edit peaks
bool PeakEditOperation::chromEditPeaks(){
	PeakEditManager& mgr = PeakEditManager::getInstance();

	deletePeak( CALL_FROM_CHROMATOGRAM );
	bool bflg = true; 
		
	if( mgr.getInAddFlag() == 1 ){
		mgr.setModeEdit();
		bflg = mgr.finishToAddPeak( *m_chrom, *m_graphics, *m_mouseEvt );
	}

	if( m_flgStartEdirChrom == 1 ){
		m_flgStartEdirChrom = 0;

		// finish to edit
		bflg = mgr.finishToEditPeak( *m_chrom, *m_graphics, *m_mouseEvt );
	}

	return bflg;
}

// set spec
void PeakEditOperation::setSpec( kome::objects::Spectrum* spec ){
	m_spec = spec;
}

// get spec
kome::objects::Spectrum* PeakEditOperation::getSpec(){
	return m_spec;
}
			
// set chrom
void PeakEditOperation::setChrom( kome::objects::Chromatogram* chrom ){
	m_chrom = chrom;
}

// get chrom
kome::objects::Chromatogram* PeakEditOperation::getChrom(){
	return m_chrom;
}

// set graphics
void PeakEditOperation::setGraphics( kome::img::Graphics* g ){
	m_graphics = g;
}

// get graphics
kome::img::Graphics* PeakEditOperation::getGraphics(){
	return m_graphics;
}

// set mouse evt
void PeakEditOperation::setMouseEvt( kome::evt::MouseEvent* evt ){
	m_mouseEvt = evt;
}

// get mouse evt
kome::evt::MouseEvent* PeakEditOperation::getMouseEvt(){
	return m_mouseEvt;
}

// set flg start edit spec
void PeakEditOperation::setFlgStartEdirSpec( int flgStartEditSpec ){
	m_flgStartEdirSpec = flgStartEditSpec;
}

// get flg start edit spec
int PeakEditOperation::getFlgStartEditSpec(){
	return m_flgStartEdirSpec;
}

// set flg start edit chrom
void PeakEditOperation::setFlgStartEdirChrom( int flgStartEditChrom ){
	m_flgStartEdirChrom = flgStartEditChrom;
}

// get flg start edti chrom
int PeakEditOperation::getFlgStartEditChrom(){
	return m_flgStartEdirChrom;
}

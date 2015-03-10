/**
 * @file OverlappingCanvasOperation.cpp
 * @brief overlapping Canvas Operation class
 *
 * @author M.Izumi
 * @date 2012.02.27
 * 
 * Copyright (C) 2014 Shimadzu Corporation All rights reserved.
 */

#include "stdafx.h"
#include "OverlappingCanvasOperation.h"
#include "OverlappingSettingsViewManager.h"

#define	SPEC_SHORT_NAME		"spec_overlap"
#define CHROM_SHORT_NAME	"chrom_overlap"

using namespace kome::view;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


// constructor
OverlappingCanvasOperation::OverlappingCanvasOperation(){
	m_spec = NULL;
	m_chrom = NULL;
	m_parent = NULL;
	m_specCanvas = NULL;
	m_chromCanvas = NULL;

	m_colorIdx = -1;
}

// destructor
OverlappingCanvasOperation::~OverlappingCanvasOperation(){
}

// on execute
bool OverlappingCanvasOperation::onExecute(){
	bool ret = false;
	// manager
	OverlappingSettingsViewManager& mgr = OverlappingSettingsViewManager::getInstance();
	
	std::string strShortName = getShortName();
	if( strShortName.compare( SPEC_SHORT_NAME ) == 0 ){
		
		ret = createSpecCanvas();
		
	}else if( strShortName.compare( CHROM_SHORT_NAME ) == 0 ){

		ret = createChromCanvas();
	}
	
	return ret;
}

// on load condition
void OverlappingCanvasOperation::onLoadCondition( boost::function< int ( void*, int ) > readFun ){
	int colorIdx = int();
	readFun( &colorIdx, sizeof( colorIdx ) );

	std::string strShortName = getShortName();
	if( strShortName.compare( SPEC_SHORT_NAME ) == 0 ){
		m_specCanvas->setColorIdx( colorIdx );
		specLoadCondition();
		
	}else if( strShortName.compare( CHROM_SHORT_NAME ) == 0 ){
		m_chromCanvas->setColorIdx( colorIdx );
		chromLoadConditon();		
	}
	
	kome::window::WindowTool::refresh();
}

// on save condition
void OverlappingCanvasOperation::onSaveCondition( boost::function< int ( void*, int ) > writeFun  ){
	int colorIdx = int();
	if( m_specCanvas != NULL ){
		colorIdx = m_specCanvas->getColorIdx();
		writeFun( &colorIdx, sizeof( colorIdx ) );
	}
	if( m_chromCanvas != NULL ){
		colorIdx = m_chromCanvas->getColorIdx();
		writeFun( &colorIdx, sizeof( colorIdx ) );
	}
}

// on get description
std::string OverlappingCanvasOperation::onGetDescription(){
	std::string s;
	std::string strShortName = getShortName();
	
	s.append( "Open Overlapping Canvas : " );
	if( strShortName.compare( SPEC_SHORT_NAME ) == 0 ){
		s.append( ( m_spec != NULL ? m_spec->getName() : "" ) );
	}else if( strShortName.compare( CHROM_SHORT_NAME ) == 0 ){
		s.append( ( m_chrom != NULL ? m_chrom->getName() : "" ) );
	}
	
	return s;
}

// on get parameters string
std::string OverlappingCanvasOperation::onGetParametersString(){
	std::string s;
	
	// sample id, spec or chrom id
	s.append( FMT( "%d,", getTargetSample()->getSampleId() ) );
	std::string strShortName = getShortName();
	if( m_spec != NULL ){
		s.append( FMT( "%d", ( m_spec != NULL ? m_spec->getOrgSpectrum()->getId() : -1 ) ) );
	}
	if( m_chrom != NULL ){
		s.append( FMT( "%d", ( m_chrom != NULL ? m_chrom->getOrgChromatogram()->getId() : -1 ) ) );
	}

	return s;
}

// on set parameters string
void OverlappingCanvasOperation::onSetParametersString( const char* strParam ){
	// icon manager object
	kome::window::IconManager& iconMgr = kome::window::IconManager::getInstance();

	// sample id, spec or chrom id
	std::vector< std::string >tokens;
	stringtoken( strParam, ",", tokens );

	if( tokens.size() < 1 ){
		return;
	}

	kome::objects::Sample* sample = kome::objects::Sample::getSampleById( toint( tokens[0].c_str(), 10, -1 ) );
	if( sample == NULL ){
		return;
	}
	setTargetSample( sample );
	
	std::string strShortName = getShortName();

	wxIcon* icon = NULL;
	// spec
	if( strShortName.compare( SPEC_SHORT_NAME ) == 0 ){
		m_spec = sample->getSpectrumById( toint( tokens[ 1 ].c_str(), 10, -1 ) );
		if( m_spec != NULL ){
			icon = iconMgr.getIcon( m_spec->getIcon() );
			if( icon != NULL && m_parent != NULL ) {
				dynamic_cast<kome::window::ChildFrame*>(m_parent)->SetIcon( *icon );
			}
		}		
						
	// chrom
	}else if( strShortName.compare( CHROM_SHORT_NAME ) == 0 ){
		m_chrom = sample->getChromatogramById( toint( tokens[ 1 ].c_str(), 10, -1 ) );
		if( m_chrom != NULL ){
			icon = iconMgr.getIcon( m_chrom->getIcon() );
			if( icon != NULL && m_parent != NULL ) {
				dynamic_cast<kome::window::ChildFrame*>(m_parent)->SetIcon( *icon );
			}
		}
	}
	
	onExecute();
}

// get spectrum canvas
kome::window::OverlappingSpectraCanvasEx* OverlappingCanvasOperation::getSpecCanvas(){
	return m_specCanvas;
}

// get chromatogram canvas
kome::window::OverlappingChromatogramsCanvasEx* OverlappingCanvasOperation::getChromCanvas(){
	return m_chromCanvas;
}

// create spectrum canvas
bool OverlappingCanvasOperation::createSpecCanvas(){
			
	for( unsigned int i = 0; i < kome::window::ChildFrame::getNumberOfFrames() && ( m_specCanvas == NULL || m_chromCanvas == NULL ); i++ ) {
		kome::window::ChildFrame* frame = kome::window::ChildFrame::getFrame( i );
		kome::window::OverlappingSpectraCanvasEx* specTmp
			= dynamic_cast< kome::window::OverlappingSpectraCanvasEx* >( frame->getCanvas() );
		kome::window::OverlappingChromatogramsCanvasEx* chromTmp
			= dynamic_cast< kome::window::OverlappingChromatogramsCanvasEx* >( frame->getCanvas() );
		if( specTmp != NULL ) {
			m_specCanvas = specTmp;
		}
		if( chromTmp != NULL ) {
			m_chromCanvas = chromTmp;
		}
	}
			
	// create canvas
	if( m_specCanvas == NULL ) {
		if( m_parent == NULL ){
			m_parent = new kome::window::ChildFrame( -1, -1,-1, -1, wxMINIMIZE_BOX | wxMAXIMIZE_BOX | wxTHICK_FRAME | wxSYSTEM_MENU | wxCAPTION );
		}
		m_specCanvas = new kome::window::OverlappingSpectraCanvasEx(
			m_parent,
			m_parent->GetClientSize().GetWidth(),
			m_parent->GetClientSize().GetHeight()
		);
	}

	return true;
}

// create chromatogram canvas
bool OverlappingCanvasOperation::createChromCanvas(){
	for( unsigned int i = 0;
				i < kome::window::ChildFrame::getNumberOfFrames() && ( m_specCanvas == NULL || m_chromCanvas == NULL ); i++ ) {
		kome::window::ChildFrame* frame = kome::window::ChildFrame::getFrame( i );
		kome::window::OverlappingSpectraCanvasEx* specTmp
			= dynamic_cast< kome::window::OverlappingSpectraCanvasEx* >( frame->getCanvas() );
		kome::window::OverlappingChromatogramsCanvasEx* chromTmp
			= dynamic_cast< kome::window::OverlappingChromatogramsCanvasEx* >( frame->getCanvas() );
		if( specTmp != NULL ) {
			m_specCanvas = specTmp;
		}
		if( chromTmp != NULL ) {
			m_chromCanvas = chromTmp;
		}
	}
		
	// create canvas
	if( m_chromCanvas == NULL ) {
		if( m_parent == NULL ){
			m_parent = new kome::window::ChildFrame( -1, -1,-1, -1, wxMINIMIZE_BOX | wxMAXIMIZE_BOX | wxTHICK_FRAME | wxSYSTEM_MENU | wxCAPTION );
		}
		m_chromCanvas = new kome::window::OverlappingChromatogramsCanvasEx(
			m_parent,
			m_parent->GetClientSize().GetWidth(),
			m_parent->GetClientSize().GetHeight()
		);	

	}

	return true;
}

// spec load condition
void OverlappingCanvasOperation::specLoadCondition(){
	
	int idx = -1;
	if( m_spec == NULL ){
		return;
	}
	kome::operation::OperationManager& optMgr = kome::operation::OperationManager::getInstance();
	unsigned int inum = optMgr.getNumberOfOperations();
	for( unsigned int i=0; i < inum; i++ ){
		if( this == optMgr.getOperation( i ) ){
			idx = i;
			break;
		}
	}

	int index = -1;
	for( unsigned int i = 0; i < m_specCanvas->getNumberOfSpectra(); i++ ){
		if( m_spec->getOrgSpectrum() == m_specCanvas->getSpectrum( i )->getOrgSpectrum() ){
			index = i;
			break;
		}
	}
	
	int state = optMgr.getState( idx );
	if( state == 0 ){		
		if( index == -1 ){		
			m_specCanvas->addSpectrum( m_spec->getOrgSpectrum() );
		}
	}else{		
		m_specCanvas->removeSpectrum( m_specCanvas->getSpectrum( index ) );
	}
}

// chrom load condition
void OverlappingCanvasOperation::chromLoadConditon(){
		
	int idx = -1;
	if( m_chrom == NULL ){
		return;
	}
	
	kome::operation::OperationManager& optMgr = kome::operation::OperationManager::getInstance();
	unsigned int inum = optMgr.getNumberOfOperations();
	for( unsigned int i=0; i < inum; i++ ){
		if( this == optMgr.getOperation( i ) ){
			idx = i;
			break;
		}
	}
	int index = -1;
	for( unsigned int i=0; i < m_chromCanvas->getNumberOfChromatograms(); i++ ){
		if( m_chrom->getOrgChromatogram() == m_chromCanvas->getChromatogram( i )->getOrgChromatogram() ){
			index = i;
		}
	}

	int state = optMgr.getState( idx );
	if( state == 0 ){
		
		if( index == -1 ){			
			m_chromCanvas->addChromatogram( m_chrom->getOrgChromatogram() );
		}
	}else{
		m_chromCanvas->removeChromatogram( m_chromCanvas->getChromatogram( index ) );
	}
}

/**
 * @file StackedCanvasOperation.cpp
 * @brief stacked Canvas Operation class
 *
 * @author M.Izumi
 * @date 2013.07.05
 * 
 * Copyright (C) 2014 Shimadzu Corporation All rights reserved.
 */


#include "stdafx.h"
#include "StackedCanvasOperaton.h"
#include "VerticalBoxSettingsViewManager.h"


#define SPEC_SHORT_NAME		"spec_stack"
#define CHROM_SHORT_NAME	"chrom_stack"


using namespace kome::view;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
stackCanvasOperation::stackCanvasOperation(){
	m_spec = NULL;
	m_chrom = NULL;
	m_parent = NULL;

	m_specCanvas = NULL;
	m_chromCanvas = NULL;

}

// destructor
stackCanvasOperation::~stackCanvasOperation(){
}

bool stackCanvasOperation::onExecute(){
	bool ret = false;
	VerticalBoxSettingsViewManager& mgr = VerticalBoxSettingsViewManager::getInstance();

	std::string strShortName = getShortName();
	if( strShortName.compare( SPEC_SHORT_NAME ) == 0 ){
		
		ret = createSpecCanvas();

	}else if( strShortName.compare( CHROM_SHORT_NAME ) == 0 ){
		
		ret = createChromCanvas();
	}
		
	return ret;
}

void stackCanvasOperation::onLoadCondition( boost::function< int ( void*, int ) > readFun ){
	
	std::string strShortName = getShortName();
	if( strShortName.compare( SPEC_SHORT_NAME ) == 0 ){
		specLoadCondition();
		
	}else if( strShortName.compare( CHROM_SHORT_NAME ) == 0 ){
		chromLoadConditon();
		
	}
	
	kome::window::WindowTool::refresh();
}

void stackCanvasOperation::onSaveCondition( boost::function< int ( void*, int ) > writeFun  ){
}

std::string stackCanvasOperation::onGetDescription(){
	std::string s;
	std::string strShortName = getShortName();

	s.append( "Open Stacked Canvas : " );
	if( strShortName.compare( SPEC_SHORT_NAME ) == 0 ){
		s.append( ( m_spec != NULL ? m_spec->getName() : "" ) );
	}else if( strShortName.compare( CHROM_SHORT_NAME ) == 0 ){
		s.append( ( m_chrom != NULL ? m_chrom->getName() : "" ) );
	}

	return s;
}

// on get parameters string
std::string stackCanvasOperation::onGetParametersString(){
	std::string s;
	
	// sample id, spec or chrom id
	s.append( FMT( "%d,", getTargetSample()->getSampleId() ) );
	std::string strShortName = getShortName();

	s.append( FMT( "%d", ( m_spec != NULL ? m_spec->getOrgSpectrum()->getId() : m_chrom->getOrgChromatogram()->getId() ) ) );
	
	return s;
}

// on set parameters string
void stackCanvasOperation::onSetParametersString( const char* strParam ){
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

// create spec canvas
bool stackCanvasOperation::createSpecCanvas(){
	// >>>>>> support delete function	@Date:2013/05/23	<Refactor>	A.Ozaki
	// get canvas
	m_specCanvas = (kome::window::VerticalBoxSpectrumCanvas *)NULL;
	m_chromCanvas = (kome::window::VerticalBoxChromatogramCanvas *)NULL;

	for( unsigned int i = 0;
			i < kome::window::ChildFrame::getNumberOfFrames( ) && ( m_specCanvas == NULL || m_chromCanvas == NULL ) ; i++ )
	{
		kome::window::ChildFrame* frame = kome::window::ChildFrame::getFrame( i );
		kome::window::VerticalBoxSpectrumCanvas*	pSpecTmp
			= dynamic_cast< kome::window::VerticalBoxSpectrumCanvas* >( frame->getCanvas( ) );
		kome::window::VerticalBoxChromatogramCanvas*	pChromTmp
			= dynamic_cast< kome::window::VerticalBoxChromatogramCanvas* >( frame->getCanvas( ) );
		if  ( pSpecTmp != NULL )
		{
			m_specCanvas = pSpecTmp;
		}
		if  ( pChromTmp != NULL )
		{
			m_chromCanvas = pChromTmp;
		}
	}

	// create canvas
	if ( m_specCanvas == NULL ){
		
		if( m_parent == NULL ){
			m_parent = new kome::window::ChildFrame( -1, -1,-1, -1, wxMINIMIZE_BOX | wxMAXIMIZE_BOX | wxTHICK_FRAME | wxSYSTEM_MENU | wxCAPTION );
		}
	
		m_specCanvas = new kome::window::VerticalBoxSpectrumCanvas(
			m_parent, 
			m_parent->GetClientSize().GetWidth( ), 
			m_parent->GetClientSize().GetHeight( ) );

	}

	// <<<<<< support delete function	@Date:2013/05/23	<Refactor>	A.Ozaki
	return true;
}

bool stackCanvasOperation::createChromCanvas(){
	// >>>>>> support delete function	@Date:2013/05/23	<Refactor>	A.Ozaki
	m_specCanvas = NULL;
	m_chromCanvas = NULL;
		
	for ( unsigned int i = 0 ;
			i < kome::window::ChildFrame::getNumberOfFrames( ) && ( m_specCanvas == NULL || m_chromCanvas == NULL ) ; i++ )
	{
		kome::window::ChildFrame*	frame = kome::window::ChildFrame::getFrame( i );
		kome::window::VerticalBoxSpectrumCanvas*	pSpecTmp
			= dynamic_cast< kome::window::VerticalBoxSpectrumCanvas* >( frame->getCanvas( ) );
		kome::window::VerticalBoxChromatogramCanvas*	pChromTmp
			= dynamic_cast< kome::window::VerticalBoxChromatogramCanvas* >( frame->getCanvas( ) );
		if  ( pSpecTmp != NULL )
		{
			m_specCanvas = pSpecTmp;
		}
		if  ( pChromTmp != NULL )
		{
			m_chromCanvas = pChromTmp;
		}
	}

	// create canvas
	if ( m_chromCanvas == NULL )
	{
		
		if( m_parent == NULL ){
			m_parent = new kome::window::ChildFrame( -1, -1,-1, -1, wxMINIMIZE_BOX | wxMAXIMIZE_BOX | wxTHICK_FRAME | wxSYSTEM_MENU | wxCAPTION );
		}
	
		m_chromCanvas = new kome::window::VerticalBoxChromatogramCanvas( 
			m_parent, 
			m_parent->GetClientSize().GetWidth( ), 
			m_parent->GetClientSize().GetHeight( )
		);
	}

	return true;
}

kome::window::VerticalBoxSpectrumCanvas* stackCanvasOperation::getSpecCanvas(){
	return m_specCanvas;
}

kome::window::VerticalBoxChromatogramCanvas* stackCanvasOperation::getChromCanvas(){
	return m_chromCanvas;
}

// spec load condition
void stackCanvasOperation::specLoadCondition(){
	int idx = -1;
	if( m_spec == NULL ){
		return;
	}
	for( unsigned int i = 0; i < m_specCanvas->getNumberOfSpectra(); i++ ){
		if( m_spec->getOrgSpectrum() == m_specCanvas->getSpectrum( i )->getOrgSpectrum() ){
		
			idx = i;
		}
	}

	if( idx > -1  ){
		m_specCanvas->removeSpectrum( m_specCanvas->getSpectrum( idx ) );
	}else {
		
		m_specCanvas->addSpectrum( m_spec->getOrgSpectrum() );
		
	}
}

// chrom load condition
void stackCanvasOperation::chromLoadConditon(){
	int idx = -1;
	if( m_chrom == NULL ){
		return;
	}
	for( unsigned int i=0; i < m_chromCanvas->getNumberOfChromatograms(); i++ ){
		if( m_chrom->getOrgChromatogram() == m_chromCanvas->getChromatogram( i )->getOrgChromatogram() ){
			
			idx = i;
		}
	}

	if( idx > -1 ){
		m_chromCanvas->removeChromatogram( m_chromCanvas->getChromatogram( idx ) );
	}else{
		m_chromCanvas->addChromatogram( m_chrom->getOrgChromatogram() );
	}
}

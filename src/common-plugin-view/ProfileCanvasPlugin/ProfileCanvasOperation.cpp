/**
 * @file ProfileCanvasOperation.cpp
 * @brief Profile Canvas Operation class
 *
 * @author M.Izumi
 * @date 2012.02.27
 * 
 * Copyright (C) 2014 Shimadzu Corporation All rights reserved.
 */

#include "stdafx.h"
#include "ProfileCanvasOperation.h"
#include "ProfileCanvasManager.h"

#define SPEC_SHORT_NAME		"spec_profile"
#define CHROM_SHORT_NAME	"chrom_profile"

using namespace kome::view;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG

using namespace kome::window;

// constructor
ProfileCanvasOperation::ProfileCanvasOperation(){
	m_spec		  = NULL;
	m_chrom		  = NULL;
	m_parent	  = NULL;
	m_specCanvas  = NULL;
	m_chromCanvas = NULL;
}

// destructor
ProfileCanvasOperation::~ProfileCanvasOperation(){
}

// on execute
bool ProfileCanvasOperation::onExecute(){

// >>>>>>	@Date:2013/09/07	<Modify>	A.Ozaki
//
	bool ret = true;		// false -> true
		
	std::string strShortName = getShortName();

	if( strShortName.compare( SPEC_SHORT_NAME ) == 0 ){
		// create canvas
		kome::window::SpectrumCanvas* canvas = kome::window::SpectrumCanvas::getCanvas( m_spec );
		if( canvas == NULL ) {
			m_specCanvas = new kome::window::SpectrumCanvas(
				m_parent,
				m_parent->GetClientSize().GetWidth(),
				m_parent->GetClientSize().GetHeight(),
				m_spec
			);
			if  ( (kome::window::SpectrumCanvas *)NULL == m_specCanvas )
			{
				ret = false;
			}
		}
		else
		{
			m_specCanvas = canvas;
			if( m_specCanvas->GetParent() != NULL && m_specCanvas->GetParent()->IsShown() == false ){  // @date 2014.03.12 <Mod> M.Izumi
				m_parent = m_specCanvas->GetParent();
				m_parent->Show();
			}
		}
			
	}else if( strShortName.compare( CHROM_SHORT_NAME ) == 0 ){
		// create canvas
		kome::window::ChromatogramCanvas* canvas = kome::window::ChromatogramCanvas::getCanvas( m_chrom );
		if( canvas == NULL ) {
			m_chromCanvas = new kome::window::ChromatogramCanvas(
				m_parent,
				m_parent->GetClientSize().GetWidth(),
				m_parent->GetClientSize().GetHeight(),
				m_chrom
			);
			if  ( (kome::window::ChromatogramCanvas *)NULL == m_chromCanvas )
			{
				ret = false;
			}
		}
		else
		{
			m_chromCanvas = canvas;
			if( m_chromCanvas->GetParent() != NULL && m_chromCanvas->GetParent()->IsShown() == false ){	// @date 2014.03.12 <Mod> M.Izumi
				m_parent = m_chromCanvas->GetParent();
				m_parent->Show();
			}
		}
	}
//
// <<<<<<	@Date:2013/09/07	<Modify>	A.Ozaki

	return ret;
}

// on load condition
void ProfileCanvasOperation::onLoadCondition( boost::function< int ( void*, int ) > readFun ){
	if( m_parent == NULL ){
		return;
	}
	if( m_parent->Hide() ){
		m_parent->Show( false );
	}else{
		m_parent->Show( true );	
	}
}

// on save condition
void ProfileCanvasOperation::onSaveCondition( boost::function< int ( void*, int ) > writeFun  ){
}

// on get description
std::string ProfileCanvasOperation::onGetDescription(){
	std::string s;
	std::string strShortName = getShortName();

	s.append( "Open Profile Canvas : " );
	if( strShortName.compare( SPEC_SHORT_NAME ) == 0 ){
		s.append( ( m_spec != NULL ? m_spec->getName() : "" ) );
	}else if( strShortName.compare( CHROM_SHORT_NAME ) == 0 ){
		s.append( ( m_chrom != NULL ? m_chrom->getName() : "" ));
	}

	return s;
}

// on get parameters string
std::string ProfileCanvasOperation::onGetParametersString(){
	std::string s;
	
	// sample id, spec or chrom id, parent width, parent height
	s.append( FMT( "%d,", getTargetSample()->getSampleId() ) );
	if( m_spec ){
		s.append( FMT( "%d,", ( m_spec->getOrgSpectrum()->getId() == NULL ? m_spec->getOrgSpectrum()->getId() : m_spec->getId() )));
	}
	if( m_chrom != NULL ){
		s.append( FMT( "%d,", ( m_chrom->getOrgChromatogram() == NULL ? m_chrom->getOrgChromatogram()->getId() : m_chrom->getId() ) ) );
	}
	if( m_specCanvas != NULL ){
		s.append( FMT( "%d", m_specCanvas->getIndex() ) );
	}
	if( m_chromCanvas != NULL ){
		s.append( FMT( "%d", m_chromCanvas->getIndex() ) );
			
	}
	return s;
}

// on set parameters string
void ProfileCanvasOperation::onSetParametersString( const char* strParam ){
	// icon manager object
	kome::window::IconManager& iconMgr = kome::window::IconManager::getInstance();

	// sample id, spec or chrom id, parent width, parent height
	std::vector< std::string >tokens;
	stringtoken( strParam, ",", tokens );

	if( tokens.size() < 3 ){
		return;
	}
	
	kome::objects::Sample* sample = kome::objects::Sample::getSampleById( toint( tokens[0].c_str(), 10, -1 ) );
	if( sample == NULL ){
		return;
	}
	setTargetSample( sample );
		
	std::string strShortName = getShortName();
	
	wxIcon* icon = NULL;
	ProfileCanvasManager& mgr = ProfileCanvasManager::getInstance();

	kome::window::Canvas* canvas = NULL;
	canvas = kome::window::Canvas::getCanvas( toint( tokens[ 2 ].c_str(), 10, -1 ) );

	// spec
	if( strShortName.compare( SPEC_SHORT_NAME ) == 0 ){
		m_spec = sample->getSpectrumById( toint( tokens[ 1 ].c_str(), 10, -1 ) );
		if( m_spec == NULL ){
			m_spec = mgr.getSpec();
		}
		icon = iconMgr.getIcon( ( m_spec == NULL ? NULL : m_spec->getIcon() ) );
		if( icon != NULL && m_parent != NULL ) {
			dynamic_cast<kome::window::ChildFrame*>(m_parent)->SetIcon( *icon );
		}
		
		if( m_parent == NULL ){
			m_parent = new kome::window::ChildFrame( -1, -1, -1, -1, wxMINIMIZE_BOX | wxMAXIMIZE_BOX | wxTHICK_FRAME | wxSYSTEM_MENU | wxCAPTION );
		}		
		
	// chrom
	}else if( strShortName.compare( CHROM_SHORT_NAME ) == 0 ){
		m_chrom = sample->getChromatogramById( toint( tokens[ 1 ].c_str(), 10, -1 ) );
		if( m_chrom == NULL ){
			m_chrom = mgr.getChrom();
		}
		icon = iconMgr.getIcon( (m_chrom == NULL ? NULL :m_chrom->getIcon() ));
		if( icon != NULL && m_parent != NULL ) {
			dynamic_cast<kome::window::ChildFrame*>(m_parent)->SetIcon( *icon );
		}				
		
		if( m_parent == NULL ){
			m_parent = new kome::window::ChildFrame( -1, -1, -1, -1, wxMINIMIZE_BOX | wxMAXIMIZE_BOX | wxTHICK_FRAME | wxSYSTEM_MENU | wxCAPTION );
		}
		
	}	
	
	onExecute();

	// @date 2013.09.03 <Add> M.Izumi ->
	if( m_parent != NULL ){
		kome::window::ChildFrame* frame = dynamic_cast<kome::window::ChildFrame*>(m_parent);
		if( frame->getNumberOfChromatograms() == 0 && 
			frame->getNumberOfSpectra() == 0 &&
			frame->getNumberOfDataMaps() == 0 ){
				delete m_parent;
				m_parent = NULL;
		}
	}
	// @date 2013.09.03 <Add> M.Izumi <-

}

// get spectrum canvas
kome::window::SpectrumCanvas* ProfileCanvasOperation::getSpecCanvas(){
	return m_specCanvas;
}

// get chromatogram
kome::window::ChromatogramCanvas* ProfileCanvasOperation::getChromCanvas(){
	return m_chromCanvas;
}


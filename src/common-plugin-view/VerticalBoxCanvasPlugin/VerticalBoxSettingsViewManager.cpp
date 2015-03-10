/**
 * @file VerticalBoxSettingsViewManager.cpp
 * @brief implements of VerticalBoxSettingsViewManager class
 *
 * @author A.Ozaki
 * @date 2013.05.14
 * 
 * Copyright(C) 2006-2014 Shimadzu Corp., Ltd. All rights reserved.
 */


#include	"stdafx.h"
#include	"VerticalBoxSettingsViewManager.h"
#include	"VerticalBoxDialog.h"
#include	"VerticalBoxSpectrumCanvas.h"
#include	"VerticalBoxChromatogramCanvas.h"

using namespace		kome::view;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



#define VERTICALBOX_SETTINGS_VIEW_STATUS_NAME			 "VERTICALBOX_SETTINGS_VIEW"


// constructor
VerticalBoxSettingsViewManager::VerticalBoxSettingsViewManager( )
{
	m_insVerticalBoxSettingsView = NULL;
	m_currentCanvas = NULL;
	
	return;
}

// destructor
VerticalBoxSettingsViewManager::~VerticalBoxSettingsViewManager( )
{
	return;
}

// create vertical box settings view
void	VerticalBoxSettingsViewManager::createVerticalBoxSettingsView( void )
{
	// check the member
	if	( (VerticalBoxDialog *)NULL != m_insVerticalBoxSettingsView )
	{
		return;
	}

	// frame 
	wxWindow*	wnd = kome::window::WindowTool::getMainWindow( );
	kome::window::MainFrame*	mainFrame = dynamic_cast< kome::window::MainFrame* >( wnd );

	// create
	m_insVerticalBoxSettingsView = new VerticalBoxDialog( kome::window::WindowTool::getMainWindow( ) );
	if( (kome::window::MainFrame *)NULL != mainFrame )
	{
		mainFrame->appendPane( m_insVerticalBoxSettingsView, "Stacked Settings", kome::window::WindowTool::DOCK_ALIGN_RIGHT );
	}

	// select canvas base
	kome::window::ChildFrame*	frame = kome::window::ChildFrame::getActiveFrame( );
	wxWindow*	canvas = ( frame == NULL ? NULL : frame->getCanvas( ) );
	selectCanvas( dynamic_cast< kome::window::CanvasBase* >( canvas ) );

	return;
}

// select canvas
void	VerticalBoxSettingsViewManager::selectCanvas( kome::window::CanvasBase* canvas )
{
	// check the member
	if  ( m_currentCanvas == canvas || (VerticalBoxDialog*)NULL == m_insVerticalBoxSettingsView )
	{
		return;
	}

	// canvas
	m_currentCanvas = canvas;
	m_insVerticalBoxSettingsView->setCanvas( canvas );
}

// get current canvas
kome::window::CanvasBase*	VerticalBoxSettingsViewManager::getCurrentCanvas( void )
{
	return	m_currentCanvas;
}

// update view
void	VerticalBoxSettingsViewManager::updateView( void )
{

	// update
	if  ( (VerticalBoxDialog *)NULL != m_insVerticalBoxSettingsView )
	{
		m_insVerticalBoxSettingsView->update( );
	}

	return;
}

// toggle the visibility of the spectra view
void	VerticalBoxSettingsViewManager::toggleVerticalBoxSettingsViewVisibility( void )
{
	// check the member
	if  ( (VerticalBoxDialog *)NULL == m_insVerticalBoxSettingsView )
	{
		createVerticalBoxSettingsView( );

		return;
	}

	// frame
	wxWindow*	wnd = kome::window::WindowTool::getMainWindow( );
	kome::window::MainFrame*	mainFrame = dynamic_cast< kome::window::MainFrame* >( wnd );
	if  ( (kome::window::MainFrame *)NULL == mainFrame )
	{
		return;
	}

	m_insVerticalBoxSettingsView->update( );

	// toggle visibility
	mainFrame->toggleVisibleBar( m_insVerticalBoxSettingsView );

	return;
}

// check the visibility of the spectra view
bool	VerticalBoxSettingsViewManager::isVisibleVerticalBoxSettingsView( void )
{
	// frame
	wxWindow*	wnd = kome::window::WindowTool::getMainWindow( );
	kome::window::MainFrame*	mainFrame = dynamic_cast< kome::window::MainFrame* >( wnd );
	if  ( (kome::window::MainFrame *)NULL == mainFrame )
	{
		return	false;
	}

	// check
	return	mainFrame->isVisibleBar( m_insVerticalBoxSettingsView );
}

// release view
void	VerticalBoxSettingsViewManager::releaseView( void )
{
	// frame
	wxWindow*	wnd = kome::window::WindowTool::getMainWindow( );
	kome::window::MainFrame*	mainFrame = dynamic_cast< kome::window::MainFrame* >( wnd );

	// set status
	kome::objects::StatusManager&	statusMgr = kome::objects::StatusManager::getInstance( );

	if	( (VerticalBoxDialog *)NULL != m_insVerticalBoxSettingsView )
	{
		statusMgr.setValue( VERTICALBOX_SETTINGS_VIEW_STATUS_NAME, "off" );
	}

	// release
	m_insVerticalBoxSettingsView = (VerticalBoxDialog *)NULL;
	m_currentCanvas = NULL;

	return;
}

// on close sample
void	VerticalBoxSettingsViewManager::onCloseSample( kome::objects::Sample* sample, const bool deleting )
{
	// check sample
	if  ( (kome::objects::Sample *)NULL == sample
		|| (kome::window::CanvasBase *)NULL == m_currentCanvas
		|| (VerticalBoxDialog *)NULL == m_insVerticalBoxSettingsView )
	{
		return;
	}
	return;
}


// on open spectrum
void	VerticalBoxSettingsViewManager::onOpenSpectrum( kome::objects::Spectrum* spec )
{
	// select canvas base
	kome::window::ChildFrame*	frame = kome::window::ChildFrame::getActiveFrame( );
	wxWindow*	canvas = ( frame == NULL ? NULL : frame->getCanvas( ) );

	VerticalBoxSettingsViewManager& mgr = VerticalBoxSettingsViewManager::getInstance( );
	mgr.selectCanvas( dynamic_cast< kome::window::CanvasBase* >(canvas) );

	updateView( );

	return;
}

// on close spectrum
void	VerticalBoxSettingsViewManager::onCloseSpectrum( kome::objects::Spectrum* spec, const bool deleting )
{
	if	( (VerticalBoxDialog *)NULL != m_insVerticalBoxSettingsView )
	{
		m_insVerticalBoxSettingsView->closeSpectrum( spec );
	}
}

// on open chromatogram
void	VerticalBoxSettingsViewManager::onOpenChromatogram( kome::objects::Chromatogram* chrom )
{

	// select canvas base
	kome::window::ChildFrame*	frame = kome::window::ChildFrame::getActiveFrame( );
	wxWindow*	canvas = ( frame == NULL ? NULL : frame->getCanvas( ) );
	selectCanvas( dynamic_cast< kome::window::CanvasBase* >( canvas ) );

	updateView( );

	return;
}

// on close chromatogram
void	VerticalBoxSettingsViewManager::onCloseChromatogram( kome::objects::Chromatogram* chrom, const bool deleting )
{
	if	( (VerticalBoxDialog *)NULL != m_insVerticalBoxSettingsView )
	{
		m_insVerticalBoxSettingsView->closeChromatgram( chrom );
	}
	return;
}

// キャンバスで選択されたスペクトルを送る		@date 2011.10.26 <Add> M.Izumi
void	VerticalBoxSettingsViewManager::setSelectionItem( kome::objects::Spectrum* spec )
{
	if	( (VerticalBoxDialog *)NULL != m_insVerticalBoxSettingsView )
	{
		m_insVerticalBoxSettingsView->SelectSpecOfCanvas( spec );
	}
}

// キャンバスで選択されたクロマトグラムを送る		@date 2011.10.26 <Add> M.Izumi
void VerticalBoxSettingsViewManager::setSelectionItem( kome::objects::Chromatogram* chrom )
{
	if	( (VerticalBoxDialog *)NULL != m_insVerticalBoxSettingsView )
	{
		m_insVerticalBoxSettingsView->SelectChromOfCanvas( chrom );
	}
}

void	VerticalBoxSettingsViewManager::onSpecKeyUpMgr( kome::objects::Spectrum* spec )
{
//	m_overlappingSettingsView->onRemoveButton();	
	// ↑この処理だと、ウインドウにフォーカスが合っていなくても選択されていれば削除される可能性がある。要対処。
	// @date 2012/12/06 <Comment> OKADA	聡さんと相談の結果、DELETE等のキーボード処理は今回行わないことになった

	return;
}

// get instance
VerticalBoxSettingsViewManager&	VerticalBoxSettingsViewManager::getInstance( void )
{
	// create object (This is the only object.)
	static VerticalBoxSettingsViewManager	mgr;

	return	mgr;
}


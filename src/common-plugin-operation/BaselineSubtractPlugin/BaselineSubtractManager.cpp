/**
 * @file BaselineSubtractManager.cpp
 * @brief implements of BaselineSubtractManager class
 *
 * @author S.Tanaka
 * @date 2007.03.12
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "BaselineSubtractManager.h"
#include "BaselineChromatogram.h"
#include "BaselineSpectrum.h"
#include "BaselineSubtractDialog.h"
#include "BaselineOperation.h" 

#include <boost/bind.hpp>


using namespace kome::baseline;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
BaselineSubtractManager::BaselineSubtractManager() {
}

// destructor
BaselineSubtractManager::~BaselineSubtractManager() {
	// delete baseline objects
	for( std::map< kome::objects::Chromatogram*, BaselineChromatogram* >::iterator it = m_chromBaselineMap.begin();
			it != m_chromBaselineMap.end(); it++ ) {
		delete (*it).second;
	}
	m_chromBaselineMap.clear();

	for( std::map< kome::objects::Spectrum*, BaselineSpectrum* >::iterator it = m_specBaselineMap.begin();
				it != m_specBaselineMap.end(); it++ ) {
		delete (*it).second;
	}
	m_specBaselineMap.clear();
	
}

// set draw chromatogram baseline
void BaselineSubtractManager::setDrawBaseline(
		kome::objects::Chromatogram* chrom,
		kome::plugin::PluginFunctionItem* item,
		kome::objects::SettingParameterValues* settings
) {
	// check the parameter
	if( chrom == NULL ) {
		return;
	}

	// get function
	kome::plugin::PluginCall* fun = ( item == NULL ) ? NULL : item->getCall();
	if( fun == NULL ) {
		return;
	}

	// get baseline chromatogram
	BaselineChromatogram* baselineChrom = NULL;
	if( m_chromBaselineMap.find( chrom ) != m_chromBaselineMap.end() ) {	// close
		baselineChrom = m_chromBaselineMap[ chrom ];
		kome::plugin::PluginCallTool::onCloseChromatogram( *baselineChrom, NULL );

		delete baselineChrom;
	}

	// create baseline chromatogram
	baselineChrom = new BaselineChromatogram( *chrom, *fun, settings );
	m_chromBaselineMap[ chrom ] = baselineChrom;

	// get baseline
	kome::objects::Chromatogram* blChrom = baselineChrom;
	blChrom->getXYData();
	kome::plugin::PluginCallTool::onUpdateChromatogram( *blChrom );
}

// set draw spectrum baseline
void BaselineSubtractManager::setDrawBaseline(
		kome::objects::Spectrum* spec,
		kome::plugin::PluginFunctionItem* item,
		kome::objects::SettingParameterValues* settings
) {
	// check the parameter
	if( spec == NULL ) {
		return;
	}

	// get function
	kome::plugin::PluginCall* fun = ( item == NULL ) ? NULL : item->getCall();
	if( fun == NULL ) {
		return;
	}

	// get baseline spectrum
	BaselineSpectrum* baselineSpec = NULL;
	if( m_specBaselineMap.find( spec ) != m_specBaselineMap.end() ) {		// close
		baselineSpec = m_specBaselineMap[ spec ];

		kome::plugin::PluginCallTool::onCloseSpectrum( *spec, NULL );
		delete baselineSpec;
	}

	// create baseline spectrum
	baselineSpec = new BaselineSpectrum( *spec, *fun, settings );

	m_specBaselineMap[ spec ] = baselineSpec;

	// get baseline
	baselineSpec->setResolution( -1.0 );
	baselineSpec->getXYData();
	kome::plugin::PluginCallTool::onUpdateSpectrum( *baselineSpec );
}

//erase chromatogram baseline
void BaselineSubtractManager::eraseBaseline( kome::objects::Chromatogram* chrom ) {
	// check the map
	if( m_chromBaselineMap.find( chrom ) == m_chromBaselineMap.end() ) {
		return;
	}

	// delete
	kome::objects::Chromatogram* baselineChrom = m_chromBaselineMap[ chrom ];
	if( baselineChrom == NULL ) {
		return;
	}
	baselineChrom->deleteXYData();
	kome::plugin::PluginCallTool::onCloseChromatogram( *( baselineChrom ), NULL );

	delete baselineChrom;

	m_chromBaselineMap.erase( chrom );
}

// erase spectrum baseline
void BaselineSubtractManager::eraseBaseline( kome::objects::Spectrum* spec ) {
	// check the map
	if( m_specBaselineMap.find( spec ) == m_specBaselineMap.end() ) {
		return;
	}

	// delete
	BaselineSpectrum* baselineSpec = m_specBaselineMap[ spec ];
	if( baselineSpec == NULL ) {
		return;
	}
	baselineSpec->deleteXYData();
	kome::plugin::PluginCallTool::onCloseSpectrum( *( baselineSpec ), NULL );

	delete baselineSpec;

	m_specBaselineMap.erase( spec );
}

// subtract chromatogram baseline
void BaselineSubtractManager::subtractBaseline(
		kome::objects::Chromatogram* chrom,
		kome::plugin::PluginFunctionItem* item,
		kome::objects::SettingParameterValues* settings,
		int index
) {
	// check the parameter
	if( chrom == NULL ) {
		return;
	}

	// get function
	kome::plugin::PluginCall* fun = ( item == NULL ) ? NULL : item->getCall();
	if( fun == NULL ) {
		return;
	}

	// create baseline subtract object
	kome::operation::BaselineSubtract* bs = new kome::operation::BaselineSubtract();
	bs->setBaselineInfo( fun, settings );
	bs->setIndex( index );
	LOG_DEBUG( FMT( "Execute %s", bs->getName() ) );
		
	setValMatchOperationChrom( bs, item );

	// -----------------------------------------------------
	// update
	// -----------------------------------------------------
	kome::objects::PointsManager& ptMgr = kome::objects::PointsManager::getInstance();
	ptMgr.addOperation( chrom, bs );
	
	kome::plugin::PluginCallTool::onUpdateChromatogram( *chrom );

	// erase baseline
	eraseBaseline( chrom );
	

	// refresh
	kome::window::WindowTool::refresh();
}

// subtract spectrum baseline
void BaselineSubtractManager::subtractBaseline(
		kome::objects::Spectrum* spec,
		kome::plugin::PluginFunctionItem* item,
		kome::objects::SettingParameterValues* settings,
		int index
) {
	// check the parameter
	if( spec == NULL ) {
		return;
	}

	// get function
	kome::plugin::PluginCall* fun = ( item == NULL ) ? NULL : item->getCall();
	if( fun == NULL ) {
		return;
	}

	// create baseline subtract object
	kome::operation::BaselineSubtract* bs = new kome::operation::BaselineSubtract();
	bs->setBaselineInfo( fun, settings );
	bs->setIndex( index );
	LOG_DEBUG( FMT( "Execute %s", bs->getName() ) );
	
	setValMatchOperationSpec( bs, item );
	// -----------------------------------------------------
	// update
	// -----------------------------------------------------
	kome::objects::PointsManager& ptMgr = kome::objects::PointsManager::getInstance();
	ptMgr.addOperation( spec, bs );
	kome::plugin::PluginCallTool::onUpdateSpectrum( *spec );
		
	// erase baseline
	eraseBaseline( spec );

	// refresh
	kome::window::WindowTool::refresh();
}

// draw chromatogram baseline
void BaselineSubtractManager::drawBaseline(
		kome::objects::Chromatogram* chrom,
		kome::img::Graphics* g,
		wxWindow* canvas
) {
	// check the map
	if( this->m_chromBaselineMap.find( chrom ) == m_chromBaselineMap.end() ) {
		return;
	}

	// set style
	COLOR oldColor = g->getColor();
	kome::img::Graphics::DrawStyle oldStyle = g->getDrawStyle();
	bool unusedColor = g->isUnusedDefaultColor();
	bool unusedLine = g->isUnusedDefaultThickness();

	g->setColor( kome::img::ColorTool::BLACK );
	g->setDrawStyle( kome::img::Graphics::STYLE_DOT );
	g->setUnusedDefaultColor( true );
	g->setUnusedDefaultThickness( true );

	// baseline
	BaselineChromatogram* baselineChrom = m_chromBaselineMap[ chrom ];
	if( baselineChrom == NULL ) {
		return;
	}

	// draw
	kome::plugin::PluginCallTool::drawChromatogram( *baselineChrom, *g, canvas );

	// recover
	g->setColor( oldColor );
	g->setDrawStyle( oldStyle );
	g->setUnusedDefaultColor( unusedColor );
	g->setUnusedDefaultThickness( unusedLine );
}

// draw spectrum baseline
void BaselineSubtractManager::drawBaseline(
		kome::objects::Spectrum* spec,
		kome::img::Graphics* g,
		wxWindow* canvas
) {
	// check the map
	if( this->m_specBaselineMap.find( spec ) == m_specBaselineMap.end() ) {
		return;
	}

	// set style
	COLOR oldColor = g->getColor();
	kome::img::Graphics::DrawStyle oldStyle = g->getDrawStyle();
	bool unusedColor = g->isUnusedDefaultColor();
	bool unusedLine = g->isUnusedDefaultThickness();

	g->setColor( kome::img::ColorTool::BLACK );
	g->setDrawStyle( kome::img::Graphics::STYLE_DOT );
	g->setUnusedDefaultColor( true );
	g->setUnusedDefaultThickness( true );

	// baseline
	BaselineSpectrum* baselineSpec = m_specBaselineMap[ spec ];
	if( baselineSpec == NULL ) {
		return;
	}

	// draw
	kome::plugin::PluginCallTool::drawSpectrum( *baselineSpec, *g, canvas );

	// recover
	g->setColor( oldColor );
	g->setDrawStyle( oldStyle );
	g->setUnusedDefaultColor( unusedColor );
	g->setUnusedDefaultThickness( unusedLine );
}

// Set the value of the matching operation ( chromatogram )
void BaselineSubtractManager::setValMatchOperationChrom( kome::operation::BaselineSubtract* bs, kome::plugin::PluginFunctionItem* item ){
	kome::operation::OperationManager& optMgr = kome::operation::OperationManager::getInstance();
	int	inum = optMgr.getNumberOfOperations();
	for( int i=0; i < inum; i++ ){
		BaselineOperation* opt = (BaselineOperation*)optMgr.getOperation( i );
		if( ( opt != NULL ) && strcmp( opt->getShortName(), "chrom_baseline_subtract" ) == 0 ){
			// 一致したXYDataOperationにセット
			if( opt->getFuncItem() == item && opt->getxyDataOperation() != (kome::objects::XYDataOperation*)bs ){
				opt->setxyDataOperation( (kome::objects::XYDataOperation*)bs );
				return;
			}
		}		
	}
}

// Set the value of the matching operation ( spectrum )
void BaselineSubtractManager::setValMatchOperationSpec( kome::operation::BaselineSubtract* bs, kome::plugin::PluginFunctionItem* item ){
	kome::operation::OperationManager& optMgr = kome::operation::OperationManager::getInstance();
	int	inum = optMgr.getNumberOfOperations();
	for( int i=0; i < inum; i++ ){
		BaselineOperation* opt = (BaselineOperation*)optMgr.getOperation( i );
		if( ( opt != NULL ) && strcmp( opt->getShortName(), "spec_baseline_subtract" ) == 0 ){
			// 一致したXYDataOperationにセット
			if( opt->getFuncItem() == item && opt->getxyDataOperation() != (kome::objects::XYDataOperation*)bs ){
				opt->setxyDataOperation( (kome::objects::XYDataOperation*)bs );
				return;
			}
		}		
	}
}

// get instance
BaselineSubtractManager& BaselineSubtractManager::getInstance() {
	// create object (This is the only object.)
	static BaselineSubtractManager mgr;

	return mgr;
}


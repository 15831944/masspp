/**
 * @file AverageSampleOperation.cpp
 * @brief Average Sample Operation class
 *
 * @author M.Izumi
 * @date 2012.01.27
 * 
 * Copyright (C) 2014 Shimadzu Corporation All rights reserved.
 */

#include "stdafx.h"
#include "SampleFusionOperation.h"
#include "CompositionManager.h"

using namespace kome::composition;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


// constructor
SampleFusionOperation::SampleFusionOperation(){
}

// destructor
SampleFusionOperation::~SampleFusionOperation(){
}

// execute
bool SampleFusionOperation::onExecute(){
	bool ret = false;	
		
	// alignment
	kome::operation::Alignment* align = NULL;
	if( m_alignItem != NULL ){
		kome::objects::Parameters params;
		kome::plugin::PluginCallTool::setGroup( params, *m_trmtGroup );
		kome::plugin::PluginCallTool::setControlGroup( params, *m_ctrlGroup );
		if( &m_alignSettings != NULL ){
			kome::plugin::PluginCallTool::setSettingValues( params, m_alignSettings );
		}
		kome::objects::Variant a = m_alignItem->getCall()->invoke( &params );
		align = (kome::operation::Alignment*)( a.prim.pt );
	}

	if( align == NULL ) {
		align = &( kome::operation::Alignment::getDefaultAlignment() );
	}
	
	// progress
	kome::window::DialogProgress progress( kome::window::WindowTool::getMainWindow(), "Sample Fusion" );
	kome::core::Progress* prgs = &progress;

	progress.createSubProgresses( 2 );
	kome::core::Progress* subPrgs = progress.getSubProgress( 0 );
	align->prepare( m_ctrlGroup->getSample(), m_trmtGroup->getSample(), subPrgs );
	prgs = progress.getSubProgress( 1 );
	
	// ---------------------------------------
	// composition
	// ---------------------------------------
	if( !progress.isStopped() ) {
		kome::objects::Parameters params;

		kome::plugin::PluginCallTool::setGroup( params, *m_trmtGroup );
		kome::plugin::PluginCallTool::setControlGroup( params, *m_ctrlGroup );
		kome::plugin::PluginCallTool::setProgress( params, *prgs );

		kome::objects::SettingParameterValues settings = m_compositionSettings;
		kome::plugin::PluginCallTool::setSettingValues( params, settings );

		progress.createSubProgresses( 2 );
		kome::core::Progress* subPrgs = progress.getSubProgress( 1 );
		align->prepare( m_ctrlGroup->getSample(), m_trmtGroup->getSample(), subPrgs );		
			
		kome::objects::Variant& alignVar = params.getValue( "alignment" );
		alignVar.type = kome::objects::Variant::OTHER;
		alignVar.prim.pt = align;

		kome::objects::Variant var = m_compositionItem->getCall()->invoke( &params );
		if( var.type == kome::objects::Variant::SAMPLE ) {
			kome::objects::Sample* smpl = (kome::objects::Sample*)var.prim.pt;
			if( smpl != NULL ) {
				kome::plugin::PluginCallTool::onOpenSample( *smpl );
				// SPEC No.80038 Mass++ Ç™óéÇøÇÈ ïsãÔçáèCê≥ @date 2011.02.07 <Add> M.Izumi  ->
				kome::composition::CompositionManager& cmpMgr = kome::composition::CompositionManager::getInstance();
				cmpMgr.addData(smpl, m_ctrlGroup, m_trmtGroup);
				//<-
				setTargetSample( smpl );
			}
		}
	}

	progress.fill();
	if( !progress.isStopped() ) {
		ret = true;
	}
	
	return ret;
}

// on save condition
void SampleFusionOperation::onSaveCondition( boost::function< int ( void*, int ) > writeFun ){
}

// on load condition
void SampleFusionOperation::onLoadCondition( boost::function< int ( void*, int ) > readFun ){
}

// get the description
std::string SampleFusionOperation::onGetDescription(){
	std::string s;
	if( m_compositionItem != NULL ){
		s.append( FMT( "%s ", m_compositionItem->getLongName() ) );
	}
	if( m_trmtGroup != NULL && m_ctrlGroup != NULL ){
		s.append( FMT( "Control:%s Treatment:%s(",
			m_ctrlGroup->getName(),
			m_trmtGroup->getName()) );
	}

	if( &m_compositionSettings != NULL ){
		int inum = m_compositionSettings.getNumberOfParameters();
		for( int i=0; i < inum; i++ ){
			s.append( FMT("[%s=%s]", m_compositionSettings.getParameterName(i), m_compositionSettings.getParameterValue(i)) );
			// sub
			kome::objects::SettingParameterValues* sub = m_compositionSettings.getSubParameterValues( m_compositionSettings.getParameterName(i), m_compositionSettings.getParameterValue(i) );
			int subnum = sub->getNumberOfParameters();
			for( int j=0; j < subnum; j++ ){
				s.append( FMT("[%s=%s]", sub->getParameterName(j), sub->getParameterValue(j)) );
			}
		}
		s.append(")");
	}
	return s;
}

// gets the parameter string
std::string SampleFusionOperation::onGetParametersString(){
	std::string s;

	// control sample Id, control id, treatement sample id, treatment id, [param value].....
	// get function 
	kome::plugin::PluginCall* alignFun = ( m_alignItem == NULL ) ? NULL : m_alignItem->getCall();
	
	kome::plugin::PluginCall* compositionFun = ( m_compositionItem == NULL ) ? NULL : m_compositionItem->getCall();
	if( compositionFun == NULL ){
		return s;
	}

	if( m_trmtGroup != NULL ){
		s.append( FMT("%d,%d",
			m_trmtGroup->getSample()->getSampleId(),
			m_trmtGroup->getId())
		);
	}

	if( m_ctrlGroup != NULL ){
		s.append( FMT("%d,%d",
			m_ctrlGroup->getSample()->getSampleId(),
			m_ctrlGroup->getId())
		);
	}
	s.append( FMT( ",%s,%s,%s,%s",
		( alignFun == NULL ) ? " " : alignFun->getType(),
		( alignFun == NULL ) ? " " : alignFun->getProperties().getStringValue("short name",""),
		compositionFun->getType(),
		compositionFun->getProperties().getStringValue("short name", "" ) )
	);

	if( &m_alignSettings != NULL ){
		kome::plugin::SettingsPage* page = ( m_alignItem == NULL ) ? NULL : m_alignItem->getSettingsPage();
		if( page != NULL ){
			s.append( FMT(",%s:", page->getParameterString( m_alignSettings ).c_str()) );
		}
	}

	if( &m_compositionSettings != NULL ){
		kome::plugin::SettingsPage* page = (m_compositionItem == NULL ) ? NULL : m_compositionItem->getSettingsPage();
		if( page != NULL ){
			s.append( FMT(",%s", page->getParameterString( m_compositionSettings ).c_str()) );
		}
	}

	return s;
}

// sets the parameter string
void SampleFusionOperation::onSetParametersString( const char* strParam ){
	// control sample Id, control id, treatement sample id, treatment id, [param value].....
	std::vector<std::string> tokens;
	stringtoken( strParam, ",", tokens );

	if( tokens.size() < 5 ){
		return;
	}

	kome::objects::Sample* tsample = kome::objects::Sample::getSampleById( toint( tokens[0].c_str(), 10, -1 ) );
	if( tsample == NULL ){
		return;
	}
	
	m_trmtGroup = tsample->getGroupById( toint( tokens[1].c_str(), 10, -1 ) );
	
	kome::objects::Sample* csample = kome::objects::Sample::getSampleById( toint( tokens[2].c_str(), 10, -1 ) );
	if( csample == NULL ){
		return;
	}

	m_ctrlGroup = csample->getGroupById( toint( tokens[3].c_str(), 10, -1 ) );
	
	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();
	m_alignItem = plgMgr.getFunctionItem( tokens[4].c_str(), tokens[5].c_str() );
	m_compositionItem = plgMgr.getFunctionItem( tokens[6].c_str(), tokens[7].c_str() );

	std::vector<std::string> strSettings;
	
	int index = FMT( "%s", strParam ).find("[");
	std::string tmpParam = FMT( "%s", strParam ).substr(index).c_str();

	stringtoken( tmpParam.c_str(), ":", strSettings );

	// alignment settings
	kome::plugin::SettingsPage* page = ( m_alignItem == NULL ? NULL :m_alignItem->getSettingsPage() );
	if( page != NULL ){
		page->setParameterString( m_alignSettings, strSettings[0].c_str() );
	}

	// composition settings
	page = ( m_compositionItem == NULL ? NULL : m_compositionItem->getSettingsPage() );
	if( page != NULL ){
		std::string s = "";
		if( strSettings.size() > 1 ){
			s.append(strSettings[1].c_str());
		}
		page->setParameterString( m_compositionSettings, s.c_str() );
	}
}

// set trmt group
void SampleFusionOperation::setTrmtGroup( kome::objects::DataGroupNode* trmtGroup ){
	m_trmtGroup = trmtGroup;
}

// get trmt group
kome::objects::DataGroupNode* SampleFusionOperation::getTrmtGroup(){
	return m_trmtGroup;
}

// set ctrl group
void SampleFusionOperation::setCtrlGroup( kome::objects::DataGroupNode* ctrlGroup ){
	m_ctrlGroup = ctrlGroup;
}

// get ctrl group
kome::objects::DataGroupNode* SampleFusionOperation::getCtrlGroup(){
	return m_ctrlGroup;
}

// set func item
void SampleFusionOperation::setAlignmentFuncItem( kome::plugin::PluginFunctionItem* item ){
	m_alignItem = item;
}

// get func item
kome::plugin::PluginFunctionItem* SampleFusionOperation::getAlignmentFuncItem(){
	return m_alignItem;
}

// set alignment settings
void SampleFusionOperation::setAlignmentSettings( kome::objects::SettingParameterValues* paramValue ){
	m_alignSettings = *paramValue;
}

// get alignment settings
kome::objects::SettingParameterValues SampleFusionOperation::getAlignmentSettings(){
	return m_alignSettings;
}

// set composition item
void SampleFusionOperation::setCompositionItem( kome::plugin::PluginFunctionItem* item ){
	m_compositionItem = item;
}

// get composition item
kome::plugin::PluginFunctionItem* SampleFusionOperation::getCompositionItem(){
	return m_compositionItem;
}

// set composition settings
void SampleFusionOperation::setCompositionSettings( kome::objects::SettingParameterValues* settings ){
	m_compositionSettings = *settings;
}

// get composition settings
kome::objects::SettingParameterValues SampleFusionOperation::getCompositionSettings(){
	return 	m_compositionSettings;
}

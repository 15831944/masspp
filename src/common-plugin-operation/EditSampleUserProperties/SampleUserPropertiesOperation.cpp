/**
 * @file SampleUserPropertiesOperation.h
 * @brief interfaces of SampleUserPropertiesOperation class
 *
 * @author M.Izumi
 * @date 2012.05.13
 * 
 * Copyright (C) 2014 Shimadzu Corporation All rights reserved.
 */

#include "stdafx.h"
#include "SampleUserPropertiesOperation.h"
#include "DetailsViewManager.h"

#include <sstream>

using namespace kome::properties;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


// constructor
SampleUserPropertiesOperation::SampleUserPropertiesOperation(){
	m_spec = NULL;
	m_chrom = NULL;
	m_group = NULL;

	m_bIsEdited = false;
	m_bValidate = false;

	m_properties.clear();
}

// destructor
SampleUserPropertiesOperation::~SampleUserPropertiesOperation(){
}

// get the description
std::string SampleUserPropertiesOperation::onGetDescription(){
	std::string s;
		
	s.append( FMT( "Add User Properties" ));
	if( m_spec != NULL ){
		s.append( FMT(" %s", m_spec->getName()) ) ;
	}

	if( m_chrom != NULL ){
		s.append( FMT(" %s", m_chrom->getName()) );
	}

	if( m_group != NULL ){
		s.append( FMT(" %s", m_group->getName()) );
	}
	s.append( FMT("[") );
		
	for( unsigned int i=0; i < m_properties.size(); i++ ){
		if( i != 0 ){
			s.append( FMT(",") );
		}

		std::pair< std::string, std::string > prop;
		prop = m_properties[i];
		std::string key = prop.first.c_str();
		std::string val = prop.second.c_str();

		s.append( FMT( "%s : %s", key.c_str(), val.c_str() ));
	}
	
	s.append( FMT("]") );

	return s;
}

// execute
bool SampleUserPropertiesOperation::onExecute(){
	
	if (m_bIsEdited)
	{		
		if ( !m_bValidate )
		{
			return false;
		}

		SaveUserProperties();
	}

	return true;
}

// save the condition
void SampleUserPropertiesOperation::onSaveCondition( boost::function< int ( void*, int ) > writeFun  ){
	kome::core::Properties userProps;	
	// spec
	if( m_spec != NULL ){
		int specId = m_spec->getId();
		writeFun( &specId, sizeof( specId ) );

		m_spec->getUserProperties( userProps );

	}
	// chrom
	if( m_chrom != NULL ){
		int chromId = m_chrom->getId();
		writeFun( &chromId, sizeof( chromId ) );
		
		m_chrom->getUserProperties( userProps );
	}
	// group
	if( m_group != NULL ){
		int groupId = m_group->getId();
		writeFun( &groupId, sizeof( groupId ) );

		userProps = m_group->getUserProperties();
	}

	int len	= userProps.getNumberOfProperties();
	writeFun( &len, sizeof( len ) );
	
	for( int i=0; i < len; i++ ){
// >>>>>>	@Date:2013/09/07	<Modified>	A.Ozaki
//
//		std::pair< std::string, std::string > props;

//		props = m_properties[ i ];
//		std::string key = props.first.c_str();
//		std::string val = props.second.c_str();

		const char	*pKey = userProps.getKey( i );
		const char	*pValue = userProps.getValue( i );
//
// <<<<<<	@Date:2013/09/07	<Modified>	A.Ozaki

		std::string	key = FMT( "%s", ((const char *)NULL == pKey)?"":pKey );
		std::string val = FMT( "%s", ((const char *)NULL == pValue)?"":pValue );
		int keyLen = key.size();
		int valLen = val.size();
		writeFun( &keyLen, sizeof( keyLen ) );
		writeFun( const_cast<char*>( key.c_str() ), key.size() );
		writeFun( &valLen, sizeof( valLen ) );
		writeFun( const_cast<char*>( val.c_str() ), val.size() );
	}
}

// load the condition
void SampleUserPropertiesOperation::onLoadCondition( boost::function< int ( void*, int ) > readFun ) {
	int id = int();
	readFun( &id, sizeof( id ) );

	kome::objects::Sample* sample = getTargetSample();
	kome::objects::Chromatogram* chrom = NULL;
	kome::objects::Spectrum* spec = NULL;
	kome::objects::DataGroupNode* group = NULL;
	
	kome::core::Properties userProps;	
	
	kome::core::Properties props;
	if( sample != NULL ){
		if( getSpec() != NULL ){
			spec = sample->getSpectrumById( id );
			props = ( spec != NULL ? spec->getUserProperties() : userProps);
		}
		if( getChrom() != NULL ){
			chrom = sample->getChromatogramById( id );
			props = ( chrom != NULL ? chrom->getUserProperties() : userProps );
		}
		if( getGroup() != NULL ){
			group = sample->getGroupById( id );
			props = ( group != NULL ? group->getUserProperties() : userProps );
		}
	}
	
	int inum = props.getNumberOfProperties()-1;
	// clear
	while( inum > -1 ){
		props.deleteParameter( props.getKey(inum) );
		inum--;
	}

	int len = int();
	readFun( &len, sizeof( len ) );

	for( int i=0; i < len; i++ ){
		int keyLen = 0;
		readFun( &keyLen, sizeof( keyLen ) );
		char* buffKey = new char[ keyLen + 1 ];
		readFun( buffKey, keyLen );
		buffKey[ keyLen ] = '\0';

		int valLen = 0;
		readFun( &valLen, sizeof( valLen ) );
		char* buffVal = new char[ valLen + 1 ];
		readFun( buffVal, valLen );
		buffVal[ valLen ] = '\0';

		userProps.setValue( buffKey, buffVal );
	
		delete buffKey;
		delete buffVal;
	}
	
	// set user properties
	if( spec != NULL ){
		spec->getUserProperties() = userProps;
	}else if( chrom != NULL ){
		chrom->getUserProperties() = userProps;
	}else if( group != NULL ){
		group->getUserProperties() = userProps;
	}
	
	sample->setEdited( true );	
	kome::plugin::PluginCallTool::onUpdateSample( *sample );

	updatePropetiesView();

}

// gets the parameter string
std::string SampleUserPropertiesOperation::onGetParametersString(){
	std::string s;

	kome::objects::Sample* sample = getTargetSample();
	if( sample != NULL ){
		s.append( FMT( "%d," , sample->getSampleId() ) );
	}

	std::stringstream ss;
	std::string tmp;
	// spec
	if( m_spec != NULL ){
		ss << ( m_spec->getOrgSpectrum() == NULL ? m_spec->getId() : m_spec->getOrgSpectrum()->getId() );
		ss >> tmp;
	}else{ 
		ss << -1;
		ss >> tmp;
	}
	s.append( tmp );
	s.append( "," );
	
	// chrom
	tmp.clear();
	ss.clear();
	if( m_chrom != NULL ){
		ss << ( m_chrom->getOrgChromatogram() == NULL ? m_chrom->getId() : m_chrom->getOrgChromatogram()->getId() );
		ss >> tmp;
	}else{ 
		ss << -1;
		ss >> tmp;
	}
	s.append( tmp );
	s.append( "," );

	// group
	tmp.clear();
	ss.clear();
	if( m_group != NULL ){
		ss << m_group->getId();
		ss >> tmp;
	}else{ 
		ss << -1;
		ss >> tmp;
	}
	s.append( tmp );
		
	s.append( FMT(",%s,%s", 
		( m_bIsEdited == true ? "true" : "fasle" ), 
		( m_bValidate == true ? "true" : "false" ) )
	);

	unsigned int inum = m_properties.size();
	for( unsigned int i=0; i < inum; i++ ){
		std::pair< std::string, std::string > propetie;
		propetie = m_properties[ i ];

		s.append( FMT(",%s:%s", propetie.first.c_str() , propetie.second.c_str() ) ); 
	}	
		
	return s;
}

// sets the parameter string
void SampleUserPropertiesOperation::onSetParametersString( const char* strParam ){
	std::vector<std::string> tokens;
	stringtoken( strParam, ",", tokens );
	if( tokens.size() < 5 ){
		return;
	}

	kome::objects::Sample* sample = kome::objects::Sample::getSampleById( toint( tokens[0].c_str(), 10, -1 ) );
	if( sample == NULL ){
		return;
	}
	setTargetSample( sample );

	// spec
	m_spec = sample->getSpectrumById( toint( tokens[1].c_str(), 10, -1 ) );
	m_chrom = sample->getChromatogramById( toint( tokens[2].c_str(), 10, -1 ) );
	m_group = sample->getGroupById( toint( tokens[3].c_str(), 10, -1 ) );
	m_bIsEdited = tobool( tokens[4].c_str() );
	m_bValidate = tobool( tokens[5].c_str() );
	
	for( unsigned int i=6; i < tokens.size(); i++ ){
		std::vector< std::string > props;
		stringtoken( tokens[i].c_str(), ":", props );

		std::pair< std::string, std::string > propetie;

		propetie.first =  ( props.size() > 1 ? props[0].c_str() : "" );
		propetie.second =  ( props.size() > 1 ? props[1].c_str() : "" );

		m_properties.push_back( propetie );
		
	}
}

// set spec
void SampleUserPropertiesOperation::setSpec( kome::objects::Spectrum* spec ){
	m_spec = spec;
}

// get spec
kome::objects::Spectrum* SampleUserPropertiesOperation::getSpec(){
	return m_spec;
}

// set chrom
void SampleUserPropertiesOperation::setChrom( kome::objects::Chromatogram* chrom ){
	m_chrom = chrom;
}

// get chrom
kome::objects::Chromatogram* SampleUserPropertiesOperation::getChrom(){
	return m_chrom;
}

// set group
void SampleUserPropertiesOperation::setGroup( kome::objects::DataGroupNode* group ){
	m_group = group;
}

// get group
kome::objects::DataGroupNode* SampleUserPropertiesOperation::getGroup(){
	return m_group;
}

// save user properties
void SampleUserPropertiesOperation::SaveUserProperties()
{
	kome::core::Properties props;
		
	for( unsigned int i=0; i < m_properties.size(); i++ ){
		std::pair< std::string, std::string > tmp;
		tmp = m_properties[i];
		props.setValue( tmp.first.c_str(), tmp.second.c_str() );
	}

	if ( m_chrom != NULL ){
		m_chrom->getUserProperties() = props;
		m_chrom->getSample()->setEdited(true);

		kome::plugin::PluginCallTool::onUpdateChromatogram( *m_chrom );
			
		// Update the Sample Tree view to show that the sample has been modified
		kome::plugin::PluginCallTool::onUpdateSample( *(m_chrom->getSample()) );
	}
	else if ( m_spec != NULL ){
		
		m_spec->getUserProperties() = props;
		m_spec->getSample()->setEdited(true);

		kome::plugin::PluginCallTool::onUpdateSpectrum( *m_spec );

		// Update the Sample Tree view to show that the sample has been modified
		kome::plugin::PluginCallTool::onUpdateSample( *(m_spec->getSample()) );
	}
	else if ( m_group != NULL ){
		
		props = m_group->getUserProperties();
		m_group->getSample()->setEdited(true);
		// Update the Sample Tree view to show that the sample has been modified
		kome::plugin::PluginCallTool::onUpdateSample( *(m_group->getSample()) );
	}
		
	updatePropetiesView();
}

// Update the Properties view 
void SampleUserPropertiesOperation::updatePropetiesView(){
	
	// Check for the active canvas
	kome::window::ChildFrame* frame = kome::window::ChildFrame::getActiveFrame();

	kome::objects::MsDataVariant obj;
	if( frame != NULL ) {
		obj.setVariant( frame->getActiveObject() );
	}

	kome::objects::Spectrum* objSpec = obj.getSpectrum();
	kome::objects::Chromatogram* objChrom = obj.getChromatogram();

	// update
	if( objSpec != NULL ) {
		kome::plugin::PluginCallTool::onUpdateSpectrum( *objSpec );
	}
	if( objChrom != NULL ) {
		kome::plugin::PluginCallTool::onUpdateChromatogram( *objChrom );
	}
}

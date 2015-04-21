/**
 * @file BatchAnalysis.cpp
 * @brief implements of BatchAnalysis class
 *
 * @author S.Tanaka
 * @date 2014.10.27
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation All rights reserved.
 */


#include "stdafx.h"
#include "BatchAnalysis.h"

#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>


using namespace kome::operation;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// static members
const char* BatchAnalysis::FILE_NAME_PROP_NAME = "File Name";
const char* BatchAnalysis::DIR_PARAM_NAME      = "dir";


// constructor
BatchAnalysis::BatchAnalysis( const char* analysisName ) : m_params( NULL, false ) {
	// initialize
	m_name = NVL( analysisName, "" );
	m_filesPage = NULL;
	m_parametersPage = NULL;

	// add file name property
	addFileProperty( FILE_NAME_PROP_NAME );
}

// destructor
BatchAnalysis::~BatchAnalysis() {
}

// clear file property
void BatchAnalysis::clearFileProperties() {
	m_fileProperties.clear();
}

// add file property
int BatchAnalysis::addFileProperty( const char* prop ) {
	int idx = (int)m_fileProperties.size();
	m_fileProperties.push_back( NVL( prop, "" ) );
	return idx;
}

// get number of file properties
unsigned int BatchAnalysis::getNumberOfFileProperties() {
	return m_fileProperties.size();
}

// get file property
const char* BatchAnalysis::getFileProperty( unsigned int idx ) {
	if( idx < 0 || idx >= (int)m_fileProperties.size() ) {
		return NULL;
	}
	return m_fileProperties[ idx ].c_str();
}

// get files page
kome::plugin::SettingsPage* BatchAnalysis::getFilesPage() {
	// create page
	if( m_filesPage == NULL ) {
		m_filesPage = new kome::plugin::SettingsPage( m_params );
		onCreateFilesPage( m_params, *m_filesPage );
	}

	return m_filesPage;
}

// get files
void BatchAnalysis::getFiles( kome::objects::SettingParameterValues* settings, std::vector< std::pair< std::string, kome::core::Properties > >& files ) {
	onGetFiles( settings, files );
}

// get parameters page
kome::plugin::SettingsPage* BatchAnalysis::getParametersPage() {
	// create page
	if( m_parametersPage == NULL ) {
		m_parametersPage = new kome::plugin::SettingsPage( m_params );
		onCreateParametersPage( m_params, *m_parametersPage );
	}

	return m_parametersPage;
}

// create file page
void BatchAnalysis::onCreateFilesPage( kome::plugin::ParameterSettings& params, kome::plugin::SettingsPage& page ) {
	// value
	kome::plugin::SettingsValue* value = params.addValue();
	value->setType( kome::plugin::SettingsValue::TYPE_DIR );
	value->setRequired( true );

	// parameter
	kome::plugin::SettingsParam* param = params.addParam();
	param->setSection( FMT( "Batch Analysis (%s)", m_name.c_str() ).c_str() );
	param->setKey( "DIR" );
	param->setName( DIR_PARAM_NAME );

	// form
	kome::plugin::SettingsForm* form = page.addForm();
	form->setTitle( "Directory" );
	form->setParam( param );
	form->setDesc( "Select the data folder." );
}

// get files
void BatchAnalysis::onGetFiles( kome::objects::SettingParameterValues* settings, std::vector< std::pair< std::string, kome::core::Properties > >& files ) {
	// directory
	std::string dir = settings->getParameterValue( DIR_PARAM_NAME );
	if( dir.empty() ) {
		return;
	}

	// get files
	getFiles( dir.c_str(), files );
}

// analyze
void BatchAnalysis::analyze( std::vector< std::string >& paths, kome::core::Progress& progress ) {
	// subprogresses
	progress.createSubProgresses( paths.size() + 2 );

	// preparation
	int progressIdx = 0;
	kome::core::Progress* subProgress = progress.getSubProgress( progressIdx );
	progressIdx++;
	progress.setStatus( "Preparation..." );
	onPrepareAnalysis( paths, *subProgress );
	if( progress.isStopped() ) {
		return;
	}
	subProgress->fill();

	// analyze each file
	for( unsigned int i = 0; i < paths.size() && !progress.isStopped(); i++ ) {    // each file
		// progress
		std::string message = FMT( "Analyzing ... [%d/%d]", ( i + 1 ), paths.size() );
		progress.setStatus( message.c_str() );
		subProgress = progress.getSubProgress( progressIdx );
		progressIdx++;

		// analyze
		onAnalyze( paths[ i ].c_str(), *subProgress );
		subProgress->fill();
	}
	if( progress.isStopped() ) {
		return;
	}

	// post analysis
	subProgress = progress.getSubProgress( progressIdx );
	progress.setStatus( "Finalizing..." );
	onFinishAnalysis( *subProgress );

	if( !progress.isStopped() ) {
		progress.fill();
	}
}

// shows analysis result
void BatchAnalysis::showResult() {
	onShowResult();
}

// create parameters page
void BatchAnalysis::onCreateParametersPage( kome::plugin::ParameterSettings& params, kome::plugin::SettingsPage& page ) {
	LOG_WARN( FMT( "This method is underconstruction. Please implement onCreateParametersPage method in the subclass of BatchAnalysis." ) );
}

// get paths
void BatchAnalysis::getFiles( const char* dir, std::vector< std::pair< std::string, kome::core::Properties > >& files ) {
	// path
	boost::filesystem::path p( dir, boost::filesystem::native );

	// store
	if( boost::filesystem::is_directory( p ) ) {    // directory
		boost::filesystem::directory_iterator end;
		for( boost::filesystem::directory_iterator it( p ); it != end; it++ ) {    // each child
			boost::filesystem::path child = (*it);
			getFiles( boost::filesystem::absolute( child ).string().c_str(), files );
		}
	}
	else {    // file
		files.resize( files.size() + 1 );
		files.back().first = boost::filesystem::absolute( p ).string();
		files.back().second.setValue( FILE_NAME_PROP_NAME, p.leaf().string().c_str() );
	}
}

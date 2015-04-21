/**
 * @file FileFactory.cpp
 * @brief implements of FileFactory class
 *
 * @author S.Tanaka
 * @date 2006.08.17
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"

#include "FileFactory.h"

#include "PluginManager.h"
#include "PluginInfo.h"
#include "PluginCall.h"
#include "PluginCallTool.h"

#include <string>
#include <list>
#include <algorithm>


using namespace kome::plugin;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



#define FUNC_TYPE_OPEN			"OPEN_FILE"
#define FUNC_TYPE_SUPPORTED	   "SUPPORTED_FILE"

#define OPEN_EXT_PARAM_NAME	   "ext"
#define OPEN_FILE_PARAM_NAME	  "name"


// constructor
FileFactory::FileFactory() {
	m_order = 255;
}

// destructor
FileFactory::~FileFactory() {
}

// create function arrays
void FileFactory::createFunctionArrays() {
	// log
	LOG_TRACE( FMT( "Searching open plug-ins" ) );

// >>>>>>	@Date:2013/06/17	<Add>	A.Ozaki
// createFunctionArraysは何回も呼び出されます
// 初期化される部分が存在してませんが、ここで初期化するようにします。
// そうでないと呼び出される毎に、リストが増殖して同じ呼び出しを繰り返し行ってしまいます
//
	m_openFunctions.clear( );
// <<<<<<	@Date:2013/06/17	<Add>	A.Ozaki

	// get plug-in manager object
	PluginManager& plgMgr = PluginManager::getInstance();

	// get plug-in informations
	for( unsigned int i = 0; i < plgMgr.getNumberOfPlugins(); i++ ) {
		PluginInfo* plugin = plgMgr.getPlugin( i );

		OpenInfo* openInfo = NULL;

		// get plug-in functions
		for( unsigned int j = 0; j < plugin->getNumberOfCalls(); j++ ) {
			PluginCall* plgCall = plugin->getCall( j );

			// add
			if( strcmp( plgCall->getType(), FUNC_TYPE_OPEN ) == 0 ) {	// OPEN
				LOG_TRACE( FMT( "There is an open plug-in in %s", plugin->getLink() ) );

				// create structure
				if( openInfo == NULL ) {
					m_openFunctions.resize( m_openFunctions.size() + 1 );
					openInfo = &m_openFunctions.back();
					openInfo->openCall = NULL;
					openInfo->supportedCall = NULL;
					openInfo->name = plgCall->getProperties().getStringValue( OPEN_FILE_PARAM_NAME, "" );
				}

				// set open function
				openInfo->openCall = plgCall;

				// set extensions
				std::string ext( plgCall->getProperties().getStringValue( OPEN_EXT_PARAM_NAME, "" ) );

				if( !ext.empty() ) {
					if(ext[0] == '.' ) ext = ext.substr( 1 );
					ext = tolowercase( ext.c_str() );
					openInfo->exts.push_back( ext );
				}
			}
			else if( strcmp( plgCall->getType(), FUNC_TYPE_SUPPORTED ) == 0 ) {	// SUPPORTED
				// create structure
				if( openInfo == NULL ) {
					m_openFunctions.resize( m_openFunctions.size() + 1 );
					openInfo = &m_openFunctions.back();
					fillzero( openInfo, sizeof( OpenInfo ) );
				}

				// set supported function
				openInfo->supportedCall = plgCall;
			}
		}
	}

	// sort the array
	if( m_openFunctions.size() > 0 ) {
		std::sort( m_openFunctions.begin(), m_openFunctions.end(), lessOpenInfo );
	}
}

// get the number of file formats
unsigned int FileFactory::getNumberOfFileFormats() {
	createFunctionArrays();
	return m_openFunctions.size();
}

// get file format name
const char* FileFactory::getFormatName( const unsigned int index ) {
	// check member
	if( index >= m_openFunctions.size() ) {
		return NULL;
	}

	return m_openFunctions[index].name.c_str();
}

// get file extensions
void FileFactory::getExtensions(
		const unsigned int index,
		std::vector<std::string> &extentions
) {
	// check member
	if( index >= m_openFunctions.size() ) {
		return;
	}

	// store extensions
	OpenInfo* info = &m_openFunctions[index];
	for( std::list< std::string >::iterator it = info->exts.begin();
			it != info->exts.end(); it++ ) {
				extentions.push_back( (*it).c_str() );
	}
}

// open file
kome::objects::SampleSet* FileFactory::openFile( const char* path, kome::core::Progress* progress ) {	// add param @date 2014.0.08 <Mod> M.Izumi 
	kome::objects::SampleSet* ss = NULL;
	ss = onOpenFile( path, progress );	// add param

	return ss;
}

// close file
void FileFactory::closeFile( kome::objects::SampleSet* sampleSet ) {
	onCloseFile( sampleSet );
}

// search file
kome::objects::SampleSet* FileFactory::searchFile( const char* path ) {
	// path
	std::string p = absolutepath( path );

	// search map
	if( m_sampleSetMap.find( p ) != m_sampleSetMap.end() ) {
		return m_sampleSetMap[ p ];
	}

	return NULL;
}

// on open file
kome::objects::SampleSet* FileFactory::onOpenFile( const char* path, kome::core::Progress* progress ) {		// add param @date 2014.0.08 <Mod> M.Izumi 
	// path
	std::string p = absolutepath( path );

	// check map
	kome::objects::SampleSet* sampleSet = searchFile( p.c_str() );
	if( sampleSet != NULL ) {
		LOG_DEBUG( FMT( "The file is already opened. [%s]", p.c_str() ) );
		return sampleSet;
	}

	// check path
	if( !fileexists( p.c_str() ) ) {
		LOG_ERROR_CODE( FMT( "The file cannot be found. Specify an existing file. [%s]", p.c_str() ), ERR_FILE_NOT_FOUND );
		return NULL;
	}

	// file name
	std::string fileName = tolowercase( getfilename( path ).c_str() );

	// getting sample set
	int cnt = 0;
	createFunctionArrays();
	for( unsigned int i = 0; i < m_openFunctions.size() && sampleSet == NULL; i++ ) {
		// open information
		OpenInfo* info = &( m_openFunctions[ i ] );
		LOG_DEBUG( FMT( "Checking ... %s", info->name.c_str() ) );

		// extension
		bool supported = ( info->exts.size() == 0 );
		for( std::list< std::string >::iterator it = info->exts.begin();
				it != info->exts.end() && !supported; it++ ) {
			std::string ext = ".";
			if( (*it).empty() ) {
				ext.clear();
			}
			else {
				ext.append( *it );
			}

			if( endswith( fileName.c_str(), ext.c_str() ) ) {
				supported = true;
			}
		}

		// check supportted
		if( supported && info->supportedCall != NULL ) {
			// parameters
			kome::objects::Parameters params;
			PluginCallTool::setPath( params, p.c_str() );

			// judge
			kome::objects::Variant ret = info->supportedCall->invoke( &params );
			supported = ( ret.type != kome::objects::Variant::BOOL || ret.prim.boolVal );
		}

		// check open function
		if( supported ) {
			if( info->openCall == NULL ) {
				LOG_WARN( FMT( "File open function is not found. [%s]", info->name.c_str() ) );
			}
		}

		// open
		if( supported ) {
			// parameters
			kome::objects::Parameters params;
			kome::plugin::PluginCallTool::setPath( params, p.c_str() );

			// get object
			kome::objects::Variant ret = info->openCall->invoke( &params );
			sampleSet = NULL;
			if( ret.type == kome::objects::Variant::SAMPLE_SET ) {
				sampleSet = (kome::objects::SampleSet*)ret.prim.pt;
			}

			// open
			if( sampleSet == NULL ) {
				LOG_ERROR( FMT( "Failed to get a sample set object." ) );
			}
			else {
				const bool success = sampleSet->openFile( p.c_str(), progress );	// add param

				if( success ) {
// >>>>>>	@Date:2013/06/17	<Modify>	A.Ozaki
// 読込に成功した後でm_sampleSetMapにsampleSetを設定しないと、
// 読込に失敗したファイルは２度と開けません
					m_sampleSetMap[ p ] = sampleSet;
// <<<<<<	@Date:2013/06/17	<Modify>	A.Ozaki
					if( sampleSet->getNumberOfSamples() == 0 ) {	// no sample
						LOG_ERROR_CODE( FMT( "There are no samples in the specified data file. Specify a data file containing at least one sample. [%s]", path ), ERR_OTHER );
					}
				}
				else {
					LOG_ERROR_CODE(
						FMT( "Failed to open the file. [%s]", path ),
						ERR_FILE_OPEN_FAILED
					);

					sampleSet = NULL;
				}
			}

			cnt++;
		}
	}

	// check the count
	if( cnt == 0 ) {
		LOG_ERROR_CODE(
			FMT( "There are no plug-ins that support the specified file." ),
			ERR_FILE_OPEN_FAILED
		);
	}

	return sampleSet;
}

// close file
void FileFactory::onCloseFile( kome::objects::SampleSet* sampleSet ) {
	// get sample sets
	std::set< kome::objects::SampleSet* > sampleSets;

	if( sampleSet == NULL ) {		// If sample set is NULL, closes all sample sets
		for( std::map< std::string, kome::objects::SampleSet* >::iterator it = m_sampleSetMap.begin();
				it != m_sampleSetMap.end(); it++ ) {
			kome::objects::SampleSet* ss = (*it).second;
			sampleSets.insert( ss );
		}
	}
	else {
		sampleSets.insert( sampleSet );
	}

	// close
	for( std::set< kome::objects::SampleSet* >::iterator it = sampleSets.begin();
			it != sampleSets.end(); it++ ) {
		// sample set
		kome::objects::SampleSet* ss = *it;

		// broadcast
		for( unsigned int i = 0; i < ss->getNumberOfSamples(); i++ ) {
			kome::objects::Sample* sample = ss->getSample( i );
			PluginCallTool::onCloseSample( *sample );
		}

		// remove from map
		std::string path = absolutepath( ss->getFilePath() );
		m_sampleSetMap.erase( path );

		// close
		ss->closeFile();
		delete ss;
	}
}

// on  close sample
void FileFactory::onCloseSample( kome::objects::Sample* sample, const bool deleting ) {
	// check the map
	if( sample == NULL ) {
		return;
	}

	// sample set
	kome::objects::SampleSet* sampleSet = sample->getSampleSet();
	bool opened = false;
	if( sampleSet == NULL ) {
		return;
	}
	for( unsigned int i = 0; i < sampleSet->getNumberOfSamples() && !opened; i++ ) {
		kome::objects::Sample* tmp = sampleSet->getSample( i );
		if( tmp != NULL && tmp->isOpened() ) {
			opened = true;
		}
	}
	if( opened ) {
		return;
	}

	// search map
	std::string path;
	for( std::map< std::string, kome::objects::SampleSet* > ::iterator it = m_sampleSetMap.begin();
			it != m_sampleSetMap.end() && path.empty(); it++ ) {
		if( (*it).second == sampleSet ) {
			path = (*it).first;
		}
	}
	if( path.empty() ) {
		return;
	}

	// close
	if( deleting ) {
		sampleSet->closeFile();
		delete sampleSet;
	}

	m_sampleSetMap.erase( path );
}

// get instance
FileFactory& FileFactory::getInstance() {
	// create object (This is the only object.)
	static FileFactory factory;

	return factory;
}

// compare to sort
bool FileFactory::lessOpenInfo( const OpenInfo& oi0, const OpenInfo& oi1 ) {
	// check open function
	if( oi0.openCall != NULL && oi1.openCall == NULL ) {
		return true;
	}
	else if( oi0.openCall == NULL && oi1.openCall != NULL ) {
		return false;
	}

	// check support function
	if( oi0.supportedCall != NULL && oi1.supportedCall == NULL ) {
		return true;
	}
	else if( oi0.supportedCall == NULL && oi1.supportedCall != NULL ) {
		return false;
	}

	// compare name
	return ( compareignorecase( oi0.name.c_str(), oi1.name.c_str()) < 0 );
}

/**
 * @file TandemSearchEngine.h
 * @brief interfaces of TandemSearchEngine class
 *
 * @author S.Tanaka
 * @date 2012.08.10
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation. All rights reserved.
 */


#include "stdafx.h"
#include "TandemSearchEngine.h"

#include "XTandemXMLManager.h"
#include "XTandemXMLHandler.h"
#include "XtandemManager.h"

using namespace kome::xtandem;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
TandemSearchEngine::TandemSearchEngine() : kome::ident::SearchEngine( "X! Tandem", "", true ) {
	m_fp = NULL;
}

// destructor
TandemSearchEngine::~TandemSearchEngine() {
}


// valid spectrum or not
bool TandemSearchEngine::isValidSpectrum( kome::objects::Spectrum* spec ) {
	if( spec == NULL ) {
		return false;
	}

	return ( spec->getMsStage() > 1 );
}

// on spectrum peaks
void TandemSearchEngine::onSpectrumPeaks(
		kome::objects::Spectrum* spec,
		kome::ident::SpecInfo* specInfo,
		kome::objects::Peaks* peaks,
		int* peakIds,
		const unsigned int peakIdsSize
) {
	// managers
	kome::ident::MgfManager& mgfMgr = kome::ident::MgfManager::getInstance();

	// peak list
	if( specInfo->stage > 1 && specInfo->precursor > 0.0 && peaks->getLength() > 0 ) {
		mgfMgr.writeMisPeaks( m_fp, specInfo, peaks );
	}
}


// on finish peak detection
bool TandemSearchEngine::onFinishPeakDetection() {
	if( m_fp != NULL ) {
		fclose( m_fp );
		m_fp = NULL;
	}

	return true;
}

// validate settings
std::string TandemSearchEngine::validateSettings( kome::objects::SettingParameterValues* paramSettings ) {
	std::string strValidationError = checkParams( paramSettings );
	return strValidationError;
}

// on search
std::string TandemSearchEngine::onSearch(
		const char* title,
		const char* comment,
		kome::ident::SpecInfo** spectra,
		const unsigned int specNum,
		kome::objects::SettingParameterValues* settings,
		kome::core::Progress& progress
) {
	// managers
	kome::core::MsppManager& msppMgr = kome::core::MsppManager::getInstance();
	XTandemXMLManager& xmlMgr = XTandemXMLManager::getInstance();
	kome::ident::IdentificationManager& identMgr = kome::ident::IdentificationManager::getInstance();
	XTandemManager& mgr = XTandemManager::getInstance();

	// check the database
	if( identMgr.getNumberOfLocalDbs() == 0 ) {
		LOG_ERROR_CODE(
			FMT( "There are no database information. Install Mass++ proteome database and restart Mass++." ),
			ERR_OTHER
		);
		return false;
	}

	// progress
	progress.setRange( 0, 2 );
	progress.setPosition( 0 );
	progress.setStatus( "Creating parameters file..." );

	// result file
	std::string resultPath = getpath( mgr.getResultDir(), m_resultFile.c_str() );

	// residues
	const char* dbName = settings->getParameterValue( "db" );
	std::string residuesFile;
	for( unsigned int i = 0; i < identMgr.getNumberOfLocalDbs(); i++ ) {
		std::string tmpDb = identMgr.getLocalDb( i );
		if( tmpDb.compare( dbName ) == 0 ) {
			residuesFile = trimstring( identMgr.getResiduesFile( i ).c_str() );
		}
	}
	
	//create XML files
	std::string taxonomyPath = getpath( m_workingDir.c_str(), "taxonomy.xml" );
	xmlMgr.createTaxonomyListXML(dbName, taxonomyPath.c_str() );

	std::string defaultFile = "default_input.xml";
	std::string defaultPath = getpath( m_workingDir.c_str(), defaultFile.c_str() );
	std::string orgDefaultFile = getpath( msppMgr.getMsppDir(), "opt/tandem/bin/default_input.xml" );
	copyfile( orgDefaultFile.c_str(), defaultPath.c_str() );

	std::string inputPath = getpath( m_workingDir.c_str(), "input.xml" );
	xmlMgr.createParameterXML(
		settings,
		inputPath.c_str(),
		defaultFile.c_str(),
		m_peaksPath.c_str(),
		residuesFile.c_str(),
		resultPath.c_str()
	);
	progress.setPosition( 1 );

	// search
	progress.setStatus( "Performing X! Tandem Search..." );
	std::string exeFile = getpath( msppMgr.getMsppDir(), "opt/tandem/bin/tandem.exe" );
	if( residuesFile.find( "xml" ) != residuesFile.npos ) {
		exeFile = getpath( msppMgr.getMsppDir(), "opt/tandem/bin/tandem2010.exe" );
	}
		
	std::string command = FMT( "\"%s\" \"%s\"", exeFile.c_str(), inputPath.c_str() );

	if( !fileexists( exeFile.c_str() ) ) {
		LOG_ERROR_CODE( FMT( "X! Tandem Binary not found.[%s]", exeFile.c_str() ), ERR_OTHER );
		return false;
	}

	// create process
	STARTUPINFO si = { sizeof( STARTUPINFO ) };
	PROCESS_INFORMATION pi = {};//
	size_t	szLine = command.length() + 1;
	char	*pLine = new char[ szLine ];
	bool bSuccess = false;

	sprintf( pLine, "%s", command.c_str() );

	if (CreateProcess( NULL, pLine, NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, m_workingDir.c_str(), &si, &pi))
	{
		// Wait until child process exits.
		DWORD r = WaitForSingleObject( pi.hProcess, INFINITE );
		if( r == WAIT_FAILED ) {
			LOG_ERROR( FMT( "Process Failed" ) );
		}
		else if( r == WAIT_ABANDONED ) {
			LOG_ERROR( FMT( "Process Abandoned" ) );
		}
		else if( r == WAIT_TIMEOUT ) {
			LOG_ERROR( FMT( "Process Timeout" ) );
		}
		else if( r == WAIT_OBJECT_0 ) {
			bSuccess = true;
		}
		else {
			LOG_ERROR( FMT( "Process Error (code=%d)", r ) );
		}
	}
	if  ( (char *)NULL != pLine )
	{
		delete[]	pLine;
	}

	if( !bSuccess ) {
		return "";
	}

	// @date 2013.11.25 <Add> M.Izumi
	progress.fill();

	return m_resultFile;
}

// prepare for peak detection
bool TandemSearchEngine::onPreparePeakDetection(
		kome::objects::SettingParameterValues* searchSttings,
		const char* peakDetector,
		kome::objects::SettingParameterValues* peakSttings,
		const char* chargeDetector,
		kome::objects::SettingParameterValues* chargeSttings,
		const char* peakFilter
) {
	// managers
	kome::core::MsppManager& msppMgr = kome::core::MsppManager::getInstance();
	kome::ident::MgfManager& mgfMgr = kome::ident::MgfManager::getInstance();
	XTandemManager& mgr = XTandemManager::getInstance();

	// directoies
	std::string tandemDir = getpath( msppMgr.getTmpDir(), "tandem" );
	if( !fileexists( tandemDir.c_str() ) ) {
		makedirectory( tandemDir.c_str() );
	}

	m_workingDir = getpath( tandemDir.c_str(), FMT( "%lld", getcurrenttime() ).c_str() );
	if( !fileexists( m_workingDir.c_str() ) ) {
		makedirectory( m_workingDir.c_str() );
	}

	m_resultFile = FMT( "%lld.xml", getcurrenttime() );

	// files
	std::string cssPath = getpath( mgr.getResultDir(), "tandem-style.css" );
	if( !fileexists( cssPath.c_str() ) ) {
		std::string orgPath = getpath( msppMgr.getMsppDir(), "opt\\tandem\\bin\\tandem-style.css" );
		copyfile( orgPath.c_str(), cssPath.c_str() );
	}

	std::string xslPath = getpath( mgr.getResultDir(), "tandem-style.xsl" );
	if( !fileexists( xslPath.c_str() ) ) {
		std::string orgPath = getpath( msppMgr.getMsppDir(), "opt\\tandem\\bin\\tandem-style.xsl" );
		copyfile( orgPath.c_str(), xslPath.c_str() );
	}

	// peaks file
	m_peaksPath = getpath( m_workingDir.c_str(), "peaks.mgf" );
	m_fp = fileopen( m_peaksPath.c_str(), "w" );

	mgfMgr.writeMisHeader( m_fp );
	fflush( m_fp );

	return true;
}

// on get search status
kome::ident::SearchEngine::SearchStatus TandemSearchEngine::onGetSearchStatus( const char* id ) {
	// return value
	kome::ident::SearchEngine::SearchStatus status = kome::ident::SearchEngine::STATUS_ERROR;

	// result file path
	XTandemManager& mgr = XTandemManager::getInstance();
	std::string path = getpath( mgr.getResultDir(), trimstring( id ).c_str() );

	if( fileexists( path.c_str() ) ) {
		status = kome::ident::SearchEngine::STATUS_COMPLETED;
	}

	return status;
}

// on get result
void TandemSearchEngine::onGetResult(
		kome::ident::SearchResult& result,
		const char* id,
		kome::ident::SpecInfo** spectra,
		const unsigned int specNum,
		kome::core::Progress& progress
) {
	// manager
	XTandemManager& mgr = XTandemManager::getInstance();
	kome::ident::IdentificationManager& identMgr = kome::ident::IdentificationManager::getInstance();

	// result location
	std::string resultLocation = trimstring( id );
	result.setResultLocation( resultLocation.c_str() );

	// DB
	int searchId = identMgr.getSearch( getName().c_str(), getVersion().c_str(), id );
	if( searchId < 0 ) {
		return;
	}
	kome::ident::SearchInfo searchInfo;
	identMgr.getSearch( searchId, searchInfo );

	// result path
	std::string resultPath = getpath( mgr.getResultDir(), resultLocation.c_str() );

	// parse
	kome::xtandem::XTandemXMLHandler xmlHandler( result, searchInfo, spectra, specNum );
	xmlHandler.parse( resultPath.c_str() );

	progress.fill();
}

// on open result
bool TandemSearchEngine::onOpenResult( const char* resultLocation ) {
	// return value
	bool bSuccess = true;

	// result Path
	XTandemManager& mgr = XTandemManager::getInstance();
	std::string resultPath = getpath( mgr.getResultDir(), resultLocation );

	HINSTANCE ret = ShellExecute(NULL, "open", resultPath.c_str(), 0, 0, SW_SHOWNORMAL);
	if ((int) ret <= 32)
	{
		bSuccess = false;
		LOG_ERROR_CODE( FMT( "Failed to launch browser to display X! Tandem Search results." ), ERR_OTHER );
	}

	return bSuccess;	
}

// on prepare search
bool TandemSearchEngine::onPrepareSearch( kome::core::Progress& progress ) {
	return true;
}

// on end search
void TandemSearchEngine::onEndSearch() {
}

// on get search information
void TandemSearchEngine::onGetSearchInfo( kome::ident::SearchInfo& searchInfo, kome::objects::SettingParameterValues* settings ) {
	// check the parameter
	if( settings == NULL ) {
		return;
	}

	// managers
	XTandemManager& mgr = XTandemManager::getInstance();
	kome::ident::IdentificationManager& identMgr = kome::ident::IdentificationManager::getInstance();

	// database
	const char* dbName = settings->getParameterValue( "db" );
	searchInfo.db = NVL( dbName, "" );

	// mass type
	searchInfo.precMassType = kome::ident::SearchResult::MASS_MONOISOTOPIC;
	searchInfo.fragMassType = kome::ident::SearchResult::MASS_MONOISOTOPIC;

	// modifications
	std::map< std::string, kome::ident::Modification* > modMap;
	for( int i = 0; i < identMgr.getNumberOfModifications(); i++ ) {
		kome::ident::Modification* m = identMgr.getModification( i );
		modMap[ m->getName() ] = m;
	}

	std::vector< std::string > mods;
	kome::plugin::SettingsValue::separateListValue( settings->getParameterValue( COMPLETE_MODIFICATIONS_PARAM_NAME ), mods );
	for( unsigned int i = 0; i < mods.size(); i++ ) {
		std::string tmp = trimstring( mods[ i ].c_str() );
		if( modMap.find( tmp ) != modMap.end() ) {
			searchInfo.fixedMods.push_back( modMap[ tmp ] );
		}
	}

	mods.clear();
	std::set< kome::ident::Modification* > varMods;
	kome::plugin::SettingsValue::separateListValue( settings->getParameterValue( POTENTIAL_MODIFICATIONS_PARAM_NAME ), mods );
	for( unsigned int i = 0; i < mods.size(); i++ ) {
		std::string tmp = trimstring( mods[ i ].c_str() );
		if( modMap.find( tmp ) != modMap.end() ) {
			kome::ident::Modification* m = modMap[ tmp ];
			searchInfo.varMods.push_back( m );
			varMods.insert( m );
		}
	}

	const char* varMods2[] = {
		"Acetyl (N-term)",
		"Gln->pyro-Glu (N-term Q)", 
		"Glu->pyro-Glu (N-term E)", 
		"Gln->pyro-Glu (Q)", 
		"Glu->pyro-Glu (E)"
	};

	unsigned int mod2N = sizeof( varMods2 ) / sizeof( const char* );

	for( unsigned int i = 0; i < mod2N; i++ ) {
		std::string tmpName = varMods2[ i ];
		if( modMap.find( tmpName ) != modMap.end() ) {
			kome::ident::Modification* m = modMap[ tmpName ];

			if( varMods.find( m ) == varMods.end() ) {
				searchInfo.varMods.push_back( m );
				varMods.insert( m );
			}
		}
	}

	// enzyme
	const char* enzyme = settings->getParameterValue( ENZYME_PARAM_NAME );
	kome::ident::Enzyme* enz = NULL;
	if( enzyme != NULL ) {
		for( int i = 0; i < identMgr.getNumberOfEnzymes() && enz == NULL; i++ ) {
			kome::ident::Enzyme* e = identMgr.getEnzyme( i );
			if( e->getName().compare( enzyme ) == 0 ) {
				enz = e;
			}
		}
	}
	searchInfo.enzyme = enz;

	// base name
	std::string resultPath = getpath( mgr.getResultDir(), m_resultFile.c_str() );
	searchInfo.baseName = resultPath;
}

// check parameters
std::string TandemSearchEngine::checkParams( kome::objects::SettingParameterValues* settings ) {
	// return value
	std::string ret;

	// modifications
	bool bMatchFound = false;
	std::vector<std::string> completeModsArray;
	std::vector<std::string> potentialModsArray;
	kome::plugin::SettingsValue::separateListValue( settings->getParameterValue(COMPLETE_MODIFICATIONS_PARAM_NAME), completeModsArray );
	kome::plugin::SettingsValue::separateListValue( settings->getParameterValue(POTENTIAL_MODIFICATIONS_PARAM_NAME), potentialModsArray );
	for (int i = 0; (i < static_cast<int>(completeModsArray.size())) && (!bMatchFound); i++)
	{
		std::string completeMod = completeModsArray[i];
		
		for (int j = 0; (j < static_cast<int>(potentialModsArray.size())) && (!bMatchFound); j++)
		{
			std::string potentialMod = potentialModsArray[j];
			
			if (completeMod == potentialMod)
			{
				bMatchFound = true;
			}
		}
	}
	if (bMatchFound)
	{
		ret = "Selecting the same modification in Complete and Potential Modifications is not allowed.";
	}


	// Validate that both Neutral Loss Mass and Window has no values or both has values. 
	bool bNullNewtralWindow = (settings->getDoubleValue(NEUTRAL_LOSS_WINDOW_PARAM_NAME, -1) == -1);
	bool bNullNewtralMass = (settings->getDoubleValue(NEUTRAL_LOSS_MASS_PARAM_NAME, -1) == -1);

	if(bNullNewtralWindow ^ bNullNewtralMass){
		// Different State.
		ret = "Neutral Loss Mass and Neutral Loss Window should either both be blank or both contain a value.";
	}

	return ret;
}

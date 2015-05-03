/**
 * @file MascotSearchEngineBase.cpp
 * @brief implements of MascotSearchEngineBase class
 *
 * @author S.Tanaka
 * @date 2012.08.09
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation. All rights reserved.
 */


#include "stdafx.h"
#include "MascotSearchEngineBase.h"         
#include "MascotManager.h"

#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>


#ifdef DUPLICATE
#undef DUPLICATE
#endif    // DUPLICATE


#include <msparser.hpp>
#include <ms_mascotresfile.hpp>


using namespace kome::mascot;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
MascotSearchEngineBase::MascotSearchEngineBase( const char* name, const char* search, const bool multipleSpec )
		: kome::ident::SearchEngine( name, NULL, multipleSpec ) {
	m_searchStr = NVL( search, "" );
}

// destructor
MascotSearchEngineBase::~MascotSearchEngineBase() {
}

// clear databases
void MascotSearchEngineBase::clearDatabases() {
	m_dbs.clear();
	m_taxonomies.clear();
}

// get the number of databases
unsigned int MascotSearchEngineBase::getNumberOfDatabases() {
	return m_dbs.size();
}

// get the database
const char* MascotSearchEngineBase::getDatabase( const unsigned int idx ) {
	if( idx >= m_dbs.size() ) {
		return NULL;
	}

	return m_dbs[ idx ].c_str();
}

// get the number of taxonomies
unsigned int MascotSearchEngineBase::getNumberOfTaxonomies() {
	return m_taxonomies.size();
}

// get the taxonomy
const char* MascotSearchEngineBase::getTaxonomy( const unsigned int idx ) {
	if( idx >= m_taxonomies.size() ) {
		return NULL;
	}

	return m_taxonomies[ idx ].c_str();
}

// import the result files into a SearchResult object
bool MascotSearchEngineBase::onImportResults(
		const std::vector<std::string>& fileList,
		kome::ident::SearchResult& result,
		kome::ident::SpecInfo** spectra,
		const unsigned int specNum,		
		kome::core::Progress& prog,
		bool usePeptideSummary,
		const double minProb
) {
	// check the parameter
	if( fileList.empty() ) {
		return false;
	}

	// managers
	MascotManager& mgr = MascotManager::getInstance();
	kome::ident::IdentificationManager& identMgr = kome::ident::IdentificationManager::getInstance();

	// dat file
	matrix_science::ms_mascotresfile file( fileList[ 0 ].c_str() );
	if( !file.isValid() ) {
		LOG_ERROR( FMT( "The file [%s] has an error. [Error Code: %d]", fileList[ 0 ].c_str(), (int)file.getErrorHandler() ) );
	}
	for( unsigned int i = 1; i < fileList.size(); i++ ) {
		if( !file.appendResfile( fileList[ i ].c_str() ) ) {
			LOG_ERROR( FMT( "Failed to add the .dat file" ) );
			return false;
		}
	}
	const double dMinProbability     = minProb;
	const bool   bUsePeptideSummary  = true;
	const int    nMaxHitsToReport    = 1000;
	const int    nFlags              = matrix_science::ms_mascotresults::MSRES_GROUP_PROTEINS
										| matrix_science::ms_mascotresults::MSRES_SHOW_SUBSETS;

	// parameters
	matrix_science::ms_searchparams p( file );

	// modification map
	std::map< std::string, kome::ident::Modification* > modMap;
	for( int i = 0; i < identMgr.getNumberOfModifications(); i++ ) {
		kome::ident::Modification* m = identMgr.getModification( i );
		std::string modName = m->getName();
		modMap[ modName ] = m;
	}

	// Hit
	kome::ident::Hit* currHit = NULL;
	std::map< std::string, kome::ident::SpecInfo* > specMap;
	for( unsigned int i = 0; i < specNum; i++ ) {
		kome::ident::SpecInfo* tmp = spectra[ i ];
		std::string key = FMT(
			"%d:%d:%s",
			tmp->specId,
			tmp->sampleIndex,
			tmp->path.c_str()
		);

		specMap[ key ] = tmp;
	}

	// fix modifications
	std::map< std::string, kome::ident::Modification* > fixModMap;

	std::vector< std::string > tokens;
	stringtoken( p.getMODS().c_str(), ",", tokens );
	for( unsigned int i = 0; i < tokens.size(); i++ ) {
		std::string modName = trimstring( tokens[ i ].c_str() );
		if( modMap.find( modName ) != modMap.end() ) {
			kome::ident::Modification* m = modMap[ modName ];
			fixModMap[ m->getAminoAcidResidue() ] = m;
		}
	}

	// variable modifications
	std::vector< kome::ident::Modification* > varMods;

	tokens.clear();
	stringtoken( p.getIT_MODS().c_str(), ",", tokens );
	for( unsigned int i = 0; i < tokens.size(); i++ ) {
		std::string modName = trimstring( tokens[ i ].c_str() );
		if( modMap.find( modName ) != modMap.end() ) {
			kome::ident::Modification* m = modMap[ modName ];
			varMods.push_back( m );
		}
	}

	// summary
	matrix_science::ms_mascotresults* results = NULL;
	if( bUsePeptideSummary ) {
		results = new matrix_science::ms_peptidesummary( file, nFlags, dMinProbability, nMaxHitsToReport, 0, 0 );
	}
	else {
		results = new matrix_science::ms_proteinsummary( file, nFlags, dMinProbability, nMaxHitsToReport, 0, 0 );
	}

	if( file.getLastError() ) {
		LOG_ERROR( FMT( "%s", file.getLastErrorString().c_str() ) );
	}

	// maps
	std::map< std::string, kome::ident::ProteinSubstance* > proteinMap;
	std::map< std::string, kome::ident::PeptideSubstance* > peptideMap;

	// protein
	int h = 1;
	matrix_science::ms_protein* prot = results->getHit( h );
	while( prot ) {    // each protein
		// protein
		std::string acc = prot->getAccession();
		std::string name = results->getProteinDescription( acc.c_str() );

		kome::ident::ProteinSubstance* protein = NULL;
		if( !acc.empty() ) {
			if( proteinMap.find( acc ) == proteinMap.end() ) {   // new protein
				protein =  new kome::ident::ProteinSubstance( &result, acc.c_str(), name.c_str(), NULL );
				proteinMap[ acc ] = protein;

				result.addSubstance( protein );
			}
			else {
				protein = proteinMap[ acc ];
			}
		}

		double proteinMass = results->getProteinMass( acc.c_str() );
		double proteinScore = prot->getScore();
		int proteinCoverage = prot->getCoverage();
		std::string unmatched = prot->getUnmatchedMasses( file );

		protein->setMass( proteinMass );

		// modification map
		std::map< std::string, kome::ident::Modification* > modMap;

		int pepCnt = prot->getNumDisplayPeptides();
		for( int pepIdx = 1; pepIdx <= pepCnt; pepIdx++ ) {    // each peptide
			int query = prot->getPeptideQuery( pepIdx );
			int p = prot->getPeptideP( pepIdx );

			if( p != -1 && query != -1 ) {
				matrix_science::ms_peptide* pep;
				if( results->getPeptide( query, p, pep )) {
					const double pepIonsScore = pep->getIonsScore();
					const double expectationValue = results->getPeptideExpectationValue( pepIonsScore, query );

//					if( pep->getAnyMatch() && expectationValue >= dMinProbability ) {
					if( pep->getAnyMatch() ) {
						// peptide
						std::string pepString = pep->getPeptideStr();
						char pepResidueBefore = prot->getPeptideResidueBefore( pepIdx );
						if( !isalpha( pepResidueBefore ) ) {
							pepResidueBefore = '-';
						}
						char pepResidueAfter = prot->getPeptideResidueAfter( pepIdx );
						if( !isalpha( pepResidueAfter ) ) {
							pepResidueAfter = '-';
						}
						std::string pepVarMods = results->getReadableVarMods( query, pep->getRank() );
						int pepStart = prot->getPeptideStart(pepIdx);
						int pepEnd = prot->getPeptideEnd(pepIdx);

						// modifications
						std::vector< kome::ident::PeptideSubstance::Modification > mods;
						for( unsigned int i = 0; i < pepString.length(); i++ ) {
							std::string aa = FMT( "%c", pepString[ i ] );
							if( fixModMap.find( aa ) != fixModMap.end() ) {
								mods.resize( mods.size() + 1 );
								kome::ident::Modification* m = fixModMap[ aa ];
								mods.back().mod = m->getName();
								mods.back().pos = (int)i;
								modMap[ m->getName() ] = m;
							}
						}

						std::string varModsStr = pep->getVarModsStr();
						for( unsigned int i = 0; i < varModsStr.length(); i++ ) {
							std::string varMod = FMT( "%c", varModsStr[ i ] );
							int index = toint( varMod.c_str(), 33, -1 ) - 1;
							if( index >= 0 && index < (int)varMods.size() ) {
								mods.resize( mods.size() + 1 );
								kome::ident::Modification* m = varMods[ index ];
								mods.back().mod = m->getName();
								mods.back().pos = (int)i - 1;
								modMap[ m->getName() ] = m;
							}
						}

						// peptide
						std::string peptideName = kome::ident::PeptideSubstance::createPeptideName(
							pepString.c_str(),
							FMT( "%c", pepResidueBefore ).c_str(),
							FMT( "%c", pepResidueAfter ).c_str(),
							mods
						);

						std::string key = FMT(
							"%s : %s",
							( protein == NULL ? "-" : protein->getAccession().c_str() ),
							peptideName.c_str()
						);

						kome::ident::PeptideSubstance* peptide = NULL;
						if( peptideMap.find( key ) == peptideMap.end() ) {
							peptide = new kome::ident::PeptideSubstance( &result, "", peptideName.c_str(), protein );
							peptideMap[ key ] = peptide;

							peptide->setSequence( pepString.c_str() );
							peptide->setResidueBefore( FMT( "%c", pepResidueBefore ).c_str() );
							peptide->setResidueAfter( FMT( "%c", pepResidueAfter ).c_str() );
							peptide->setStart( pepStart );
							peptide->setEnd( pepEnd );
							for( unsigned int i = 0; i < mods.size(); i++ ) {
								peptide->addModification( mods[ i ].mod.c_str(), mods[ i ].pos );
							}

							result.addSubstance( peptide );
						}
						else {
							peptide = peptideMap[ key ];
						}


						// alters
						std::vector< int > alterStart;
						std::vector< int > alterEnd;
						std::vector< std::string > alterPre;
						std::vector< std::string > alterPost;
						std::vector< int > alterFrame;
						std::vector< int > alterMulti;
						std::vector< int > alterDb;
						std::vector< std::string > alters = results->getAllProteinsWithThisPepMatch(
							query, p, alterStart, alterEnd, alterPre, alterPost, alterFrame, alterMulti, alterDb
						);

						// hit
						matrix_science::ms_inputquery inputQuery( file, query );
						std::string specTitle = inputQuery.getStringTitle( true );

						int charge = 0;
						kome::ident::SpecInfo* spec = new kome::ident::SpecInfo();
						identMgr.ParseSpectrumTitle( specTitle.c_str(), *spec, &charge );

						std::string specKey = FMT( "%d:%d:%s", spec->specId, spec->sampleIndex, spec->path.c_str() );
						if( specMap.find( specKey ) == specMap.end() ) {
							result.addSpectrum( spec );
							specMap[ specKey ] = spec;
						}
						else {
							delete spec;
							spec = specMap[ specKey ];
						}

						kome::ident::Hit* hit = result.addHit( peptide );
						hit->addSpectrum( spec );
						hit->setCharge( charge );
						hit->setRank( pep->getRank() );
						hit->setMissedCleavagesCount( pep->getMissedCleavages() );
						hit->setCalculatedMR( pep->getMrCalc() );
						hit->setDelta( pep->getDelta() );
						hit->setPeptideScore( pepIonsScore );
						hit->addValue(
							"homologyscore",
							FMT( "%d", results->getHomologyThreshold( query, 20 ) ).c_str(),
							kome::ident::TYPE_SCORE
						);
						hit->addValue(
							"identityscore",
							FMT( "%d", results->getPeptideIdentityThreshold( query, 20 ) ).c_str(),
							kome::ident::TYPE_SCORE
						);
						hit->setPeptideExpect( expectationValue );

						hit->addValue( "Observed", FMT( "%f", pep->getObserved() ).c_str(), kome::ident::TYPE_PROPERTY );
						hit->setProteinCount( alters.size() );
						hit->setMatchedIonCount( pep->getNumIonsMatched() );

						hit->addValue( "Unmatched", unmatched.c_str(), kome::ident::TYPE_PROPERTY );
						hit->setProteinScore( proteinScore );
						hit->addValue( "Percent Coverage", FMT( "%d", proteinCoverage ).c_str(), kome::ident::TYPE_PROPERTY );

						// modification
						for( unsigned int i = 0; i < mods.size(); i++ ) {
							hit->addModification( modMap[ mods[ i ].mod ], mods[ i ].pos );
						}

						// alter proteins
						for( unsigned int i = 0; i < alters.size(); i++ ) {
							std::string alterAcc = alters[ i ];
							std::string alterName = results->getProteinDescription( alterAcc.c_str() );
							const double alterMass = results->getProteinMass( alterAcc.c_str() );

							kome::ident::ProteinSubstance* alterProtein = NULL;
							if( proteinMap.find( alterAcc ) == proteinMap.end() ) {
								alterProtein = new kome::ident::ProteinSubstance( &result, alterAcc.c_str(), alterName.c_str(), NULL );
								result.addSubstance( alterProtein );
								alterProtein->setMass( alterMass );
								proteinMap[ alterAcc ] = alterProtein;
							}
							else {
								alterProtein = proteinMap[ alterName ];
							}

							if( alterProtein != NULL ) {
								hit->addAltSubstance( alterProtein, alterPre[ i ].c_str(), alterPost[ i ].c_str(), alterStart[ i ], alterEnd[ i ] );
							}
						}

						// on hit
						onHit( hit );
					}
				}
			}					
		}

		// next protein
		h++;
		prot = results->getHit( h );
	}

	delete results;

	return true;
}

// get enzyme
kome::ident::Enzyme* MascotSearchEngineBase::getEnzyme( kome::objects::SettingParameterValues* settings ) {
	// check the parameter
	if( settings == NULL ) {
		return NULL;
	}

	// manager
	kome::ident::IdentificationManager& identMgr = kome::ident::IdentificationManager::getInstance();

	// enzyme
	kome::ident::Enzyme* enzyme = NULL;

	const char* enzymeStr = settings->getParameterValue( ENZYME_PARAM_NAME );
	if( enzymeStr != NULL ) {
		for( int i = 0; i < identMgr.getNumberOfEnzymes() && enzyme == NULL; i++ ) {
			kome::ident::Enzyme* tmp = identMgr.getEnzyme( i );
			if( tmp->getName().compare( enzymeStr ) == 0 ) {
				enzyme = tmp;
			}
		}
	}

	return enzyme;
}

// get result file path
std::string MascotSearchEngineBase::getResultFilePath( int index ) {
	// manager
	kome::core::MsppManager& msppMgr = kome::core::MsppManager::getInstance();

	// folder
	if( m_resultDir.empty() ) {
		time_t now = time( NULL );
		tm* t = localtime( &now );

		std::string confDir = msppMgr.getConfDir();

		std::string mascotDir = getpath( confDir.c_str(), "mascot" );
		if( !fileexists( mascotDir.c_str() ) ) {
			makedirectory( mascotDir.c_str() );
		}

		std::string typeDir = getpath( mascotDir.c_str(), m_searchStr.c_str() );
		if( !fileexists( typeDir.c_str() ) ) {
			makedirectory( typeDir.c_str() );
		}

		std::string yearDir = getpath( typeDir.c_str(), FMT( "%d", ( t->tm_year + 1900 ) ).c_str() );
		if( !fileexists( yearDir.c_str() ) ) {
			makedirectory( yearDir.c_str() );
		}

		std::string monthDir = getpath( yearDir.c_str(), FMT( "%d", ( t->tm_mon + 1 ) ).c_str() );
		if( !fileexists( monthDir.c_str() ) ) {
			makedirectory( monthDir.c_str() );
		}

		std::string dayDir = getpath( monthDir.c_str(), FMT( "%d", t->tm_mday ).c_str() );
		if( !fileexists( dayDir.c_str() ) ) {
			makedirectory( dayDir.c_str() );
		}

		m_searchId = FMT( "%lld", getcurrenttime() );
		std::string timeDir = getpath( dayDir.c_str(), m_searchId.c_str() );
		if( !fileexists( timeDir.c_str() ) ) {
			makedirectory( timeDir.c_str() );
		}

		m_resultDir = timeDir;
	}

	// file name
	std::string fileName = "result.dat";
	if( index >= 0 ) {
		fileName = FMT( "result-%d.dat", index );
	}

	// path
	std::string p = getpath( m_resultDir.c_str(), fileName.c_str() );

	return p;
}

// get result directory
std::string MascotSearchEngineBase::getResultDir( const char* searchId, const char* baseDir ) {
	// return value
	std::string ret;
	if( searchId == NULL ) {
		return ret;
	}

	// base directory
	std::string dir = trimstring( baseDir );
	if( dir.empty() ) {
		kome::core::MsppManager& msppMgr = kome::core::MsppManager::getInstance();
		dir = getpath( msppMgr.getConfDir(), "mascot" );
		dir = getpath( dir.c_str(), m_searchStr.c_str() );
	}

	// each directories
	boost::filesystem::path p( dir.c_str(), boost::filesystem::native );
	boost::filesystem::directory_iterator end;

	for( boost::filesystem::directory_iterator it( p ); it != end && ret.empty(); it++ ) {
		std::string name = (*it).path().leaf().string();
		std::string path = getpath( dir.c_str(), name.c_str() );

		if( isdirectory( path.c_str()) ) {
			if( name.compare( searchId ) == 0 ) {
				ret = path;
			}
			else {
				ret = getResultDir( searchId, path.c_str() );
			}
		}
	}

	return ret;
}

// perform search
std::string MascotSearchEngineBase::performSearch(
		std::vector< std::string >& peakFiles,
		const char* title,
		kome::objects::SettingParameterValues* settings,
		kome::core::Progress& progress
) {
	// return value
	std::string ret;

	// manager
	MascotManager& mgr = MascotManager::getInstance();

	// check parameters
	if( peakFiles.size() == 0 ) {
		return ret;
	}

	// progress
	progress.createSubProgresses( peakFiles.size() );

	// list file
	std::string resultFile = getResultFilePath( -1 );
	std::string dir = getdir( resultFile.c_str() );
	std::string listFile = getpath( dir.c_str(), "list.txt" );

	FILE* fp = fileopen( listFile.c_str(), "w" );
	if( fp == NULL ) {
		return ret;
	}

	// min probability
	double minProb = 0.05;
	if( settings != NULL ) {
		minProb = settings->getDoubleValue( "min_probability", 0.05 );
	}
	fprintf( fp, "%f\n", minProb );

	// use peptide summary
	bool usePeptideSummary = false;
	if( settings != NULL ) {
		usePeptideSummary = settings->getBoolValue( "use_peptidesummary", false );
	}
	fprintf( fp, "%d\n", usePeptideSummary ? 1 : 0 );

	// search
	for( unsigned int i = 0; i < peakFiles.size() && !progress.isStopped(); i++ ) {
		// progress
		kome::core::Progress* prgs = progress.getSubProgress( i );

		// result file path
		resultFile = getResultFilePath( i );

		std::string resultLocation = mgr.executeSubSearch(
				m_searchStr.c_str(),
				title,
				peakFiles[ i ].c_str(),
				settings,
				*prgs,
				resultFile.c_str()
		);

		if( !resultLocation.empty() ) {
			fprintf( fp, "%s\t%s\n", resultLocation.c_str(), resultFile.c_str() );
		}
				
		if( !prgs->isStopped() ){
			prgs->fill();
		}
	}

	// close file
	fflush( fp );
	fclose( fp );

// >>>>>>	@Date:2013/09/07	<Add>	A.Ozaki
//  Process has been aborted, the progress dialog does not close automatically
//
	progress.fill( );
//
// <<<<<<	@Date:2013/09/07	<Add>	A.Ozaki

	if( progress.isStopped() ) {
		return ret;
	}

	// ID
	ret = m_searchId;

	return ret;
}

// on hit
void MascotSearchEngineBase::onHit( kome::ident::Hit* hit ) {
}

// on open result
bool MascotSearchEngineBase::onOpenResult( const char* resultLocation ) {
	// return value
	bool bSuccess = true;

	if (strlen(resultLocation) > 0)
	{
		MascotManager& mgr = MascotManager::getInstance();
		std::string baseUrl = mgr.getUrl();
		baseUrl.append( "cgi/master_results.pl?file=" );

		// Parse the resultLocation string, as it may contain multiple result substrings.
		std::string strResultLocation = resultLocation;
		std::vector<std::string> resultSubStringList;

		stringtoken( strResultLocation.c_str(), ":", resultSubStringList );

		// Open the Mascot results for each result substring in a browser
		for (int i = 0; (i < static_cast<int>(resultSubStringList.size())) && bSuccess; i++)
		{
			std::string url = baseUrl + resultSubStringList[i];
			HINSTANCE ret = ShellExecute(NULL, "open", url.c_str(), 0, 0, SW_SHOWNORMAL);
			if ((int) ret <= 32)
			{
				bSuccess = false;
				LOG_ERROR_CODE( FMT( "Failed to launch browser to display Mascot Search results." ), ERR_OTHER );
			}
			else if ( (i == 0) && (resultSubStringList.size() > 1) )
			{// If we have just displayed the first result and there are multiple results, wait for 5 seconds to give the web browser time to open.
				Sleep(5000);
			}
		}
	}
	else
	{
		bSuccess = false;
		LOG_ERROR_CODE( FMT( "Cannot display Mascot Search results for imported results." ), ERR_OTHER );
	}

	return bSuccess;
}

// validate settings
std::string MascotSearchEngineBase::validateSettings( kome::objects::SettingParameterValues* paramSettings ) {
	std::string strValidationError = checkMods( paramSettings );
	return strValidationError;
}

// on prepare search
bool MascotSearchEngineBase::onPrepareSearch( kome::core::Progress& progress ) {
	// manager
	MascotManager& mgr = MascotManager::getInstance();

	//
	m_resultDir.clear();

	// get the information
	MascotManager::SearchPageInfo info;

	if( !mgr.getPageInfo( m_searchStr.c_str(), info, progress ) ) {
		return false;
	}

	// copy
	clearDatabases();
	for( unsigned int i = 0; i < info.dbs.size(); i++ ) {
		m_dbs.push_back( info.dbs.at( i ) );
	}
	for( unsigned int i = 0; i < info.taxonomies.size(); i++ ) {
		m_taxonomies.push_back( info.taxonomies.at( i ) );
	}

	return true;
}

// on get status
kome::ident::SearchEngine::SearchStatus MascotSearchEngineBase::onGetSearchStatus( const char* id ) {
	// status
	kome::ident::SearchEngine::SearchStatus status = kome::ident::SearchEngine::STATUS_ERROR;

	// resuld directory
	std::string resultDir = getResultDir( id );
	if( resultDir.empty() ) {
		return status;
	}

	// list file
	std::string listFile = getpath( resultDir.c_str(), "list.txt" );
	if( fileexists( listFile.c_str() ) ) {
		status = kome::ident::SearchEngine::STATUS_COMPLETED;
	}

	return status;
}

// get result
void MascotSearchEngineBase::onGetResult(
		kome::ident::SearchResult& result,
		const char* id,
		kome::ident::SpecInfo** spectra,
		const unsigned int specNum,
		kome::core::Progress& progress
) {
	// result directory
	std::string resultDir = getResultDir( id );
	if( resultDir.empty() ) {
		return;
	}

	// manager
	MascotManager& mgr = MascotManager::getInstance();

	// dat files
	std::string listFile = getpath( resultDir.c_str(), "list.txt" );
	FILE* fp = fileopen( listFile.c_str(), "r" );
	if( fp == NULL ) {
		return;
	}

	double minProb = 0.05;
	std::vector< std::pair< std::string, std::string > > datFiles;
	char buff[ 4096 ];
	if( fgets( buff, 4096, fp ) != NULL ) {
		minProb = todouble( buff, minProb );
	}

	bool usePeptideSummary = false;
	if( fgets( buff, 4096, fp ) != NULL ) {
		if( toint( buff, 10, 0 ) > 0 ) {
			usePeptideSummary = true;
		}
	}

	while( fgets( buff, 4096, fp ) != NULL ) {
		std::vector< std::string > tokens;
		stringtoken( buff, "\t\r\n", tokens );
		if( tokens.size() >= 2 ) {
			datFiles.push_back( std::make_pair( tokens[ 0 ], tokens[ 1 ] ) );
		}
	}

	fclose( fp );

	if( datFiles.size() == 0 ) {
		return;
	}

	// result location
	std::string resultLocation;
	for( unsigned int i = 0; i < datFiles.size(); i++ ) {
		if( !resultLocation.empty() ) {
			resultLocation.append( ":" );
		}
		resultLocation.append( datFiles[ i ].first );
	}

	// parse
	std::vector< std::string > fileList;
	for( unsigned int i = 0; i < datFiles.size(); i++ ) {
		fileList.push_back( datFiles[ i ].second );
	}

	std::string searchType = trimstring( tolowercase( m_searchStr.c_str() ).c_str() );

	onImportResults(
		fileList,
		result,
		spectra,
		specNum,
		progress,
		usePeptideSummary,
		minProb
	);

	result.setResultLocation( resultLocation.c_str() );
}

// on get search info
void MascotSearchEngineBase::onGetSearchInfo( kome::ident::SearchInfo& searchInfo, kome::objects::SettingParameterValues* settings ) {
	// check the parameter
	if( settings == NULL ) {
		return;
	}

	// DB
	searchInfo.db = settings->getParameterValue( DB_PARAM_NAME );

	// enzyme
	kome::ident::Enzyme* enzyme = getEnzyme( settings );
	if( enzyme != NULL ) {
		searchInfo.enzyme = enzyme;
	}

	// mass type
	const char* massTypeStr = settings->getParameterValue( MASS_PARAM_NAME );
	kome::ident::SearchResult::MassType massType = kome::ident::SearchResult::MASS_UNKNOWN;
	if( massTypeStr != NULL ) {
		if( strcmp( massTypeStr, "Monoisotopic" ) == 0 ) {
			massType = kome::ident::SearchResult::MASS_MONOISOTOPIC;
		}
		else if( strcmp( massTypeStr, "Average" ) == 0 ) {
			massType = kome::ident::SearchResult::MASS_AVERAGE;
		}
	}
	searchInfo.precMassType = massType;
	searchInfo.fragMassType = massType;

	// modification map
	kome::ident::IdentificationManager& identMgr = kome::ident::IdentificationManager::getInstance();
	std::map< std::string, kome::ident::Modification* > modMap;
	for( int i = 0; i < identMgr.getNumberOfModifications(); i++ ) {
		kome::ident::Modification* m = identMgr.getModification( i );
		std::string modName = m->getName();
		modMap[ modName ] = m;
	}

	// fixed modifications
	std::vector< std::string > mods;
	kome::plugin::SettingsValue::separateListValue( settings->getParameterValue( FIXEDMODS_PARAM_NAME ), mods );
	for( unsigned int i = 0; i < mods.size(); i++ ) {
		std::string modName = mods[ i ];
		if( modMap.find( modName ) != modMap.end() ) {
			searchInfo.fixedMods.push_back( modMap[ modName ] );
		}
	}

	// variable modifications
	mods.clear();
	kome::plugin::SettingsValue::separateListValue( settings->getParameterValue( VARIABLE_PARAM_NAME ), mods );
	for( unsigned int i = 0; i < mods.size(); i++ ) {
		std::string modName = mods[ i ];
		if( modMap.find( modName ) != modMap.end() ) {
			searchInfo.varMods.push_back( modMap[ modName ] );
		}
	}

	// base name
	searchInfo.baseName = getResultFilePath( -1 );
}

// check modificatinos
std::string MascotSearchEngineBase::checkMods( kome::objects::SettingParameterValues* settings ) {
	// return value
	std::string ret;

	// Validate that the same modification is not contained in both
	// the Fixed Modifications and Variable Modifications lists.
	// manager
	MascotManager& mgr = MascotManager::getInstance();

	bool bMatchFound = false;
	std::vector<std::string> fixedModsArray;
	std::vector<std::string> variableModsArray;
	kome::plugin::SettingsValue::separateListValue( settings->getParameterValue(FIXEDMODS_PARAM_NAME), fixedModsArray );
	kome::plugin::SettingsValue::separateListValue( settings->getParameterValue(VARIABLE_PARAM_NAME), variableModsArray );

	for (int i = 0; (i < static_cast<int>(fixedModsArray.size())) && (!bMatchFound); i++)
	{
		std::string fixedMod = fixedModsArray[i];

		for (int j = 0; (j < static_cast<int>(variableModsArray.size())) && (!bMatchFound); j++)
		{
			std::string varMod = variableModsArray[j];

			if (fixedMod == varMod)
			{
				bMatchFound = true;
			}
		}
	}

	if (bMatchFound)
	{
		ret = "Selecting the same modification in Fixed and Variable Modifications is not allowed.";
	}

	return ret;
}

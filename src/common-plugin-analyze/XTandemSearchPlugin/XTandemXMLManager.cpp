/**
 * @file XTandemXMLManager.cpp
 * @brief create XML functions
 *
 * @author N.Tashiro
 * @date 2012.01.25
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation. All rights reserved.
 */


#include "stdafx.h"
#include "XTandemXMLManager.h"

#include "XtandemManager.h"


#include <fstream>


using namespace kome::xtandem;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


// constructor
XTandemXMLManager::XTandemXMLManager() {
}

// destructor
XTandemXMLManager::~XTandemXMLManager() {

}

//write default Parameter File for X! Tandem 
bool XTandemXMLManager::createTaxonomyListXML(const char* dbName, const char* path ){
		// managers
	kome::core::MsppManager& msppMgr = kome::core::MsppManager::getInstance();
	kome::ident::IdentificationManager& identMgr = kome::ident::IdentificationManager::getInstance();

	// file open
	std::ofstream ofs( path, std::ios::out | std::ios::trunc);
	if (ofs.fail()) {
		// fail failed open
		LOG_ERROR(FMT("Failed to create Default Parameter xml file.[Failed to open]"));
		return false;
	}

	// write string array
	std::string strTemp;
	std::vector<std::string> strWriteArray;

	// create start
	strTemp = FMT(XTANDEM_XML_VERSION);
	strWriteArray.push_back(strTemp);
	strTemp = FMT(XTANDEM_XML_BIOML_START);
	strWriteArray.push_back(strTemp);

	int nNumberOfDbs = identMgr.getNumberOfLocalDbs();
	for(int i = 0; i < nNumberOfDbs; i++){
		if(strcmp(identMgr.getLocalDb(i).c_str(),dbName) == 0){
			int nNumberOfDbFiles = identMgr.getNumberOfLocalDbFiles(i);
			for(int j = 0; j < nNumberOfDbFiles;j++){
				std::string strLocalDbFileName = identMgr.getLocalDbFileName(i,j);
				strTemp = FMT(XTANDEM_XML_TAXON_START, replacestring( strLocalDbFileName.c_str(), "\\", "/" ).c_str() );
				strWriteArray.push_back(strTemp);
				kome::ident::DbFileType nLocalDbFileType = (kome::ident::DbFileType)identMgr.getLocalDbFileType(i,j);
				if((nLocalDbFileType & (kome::ident::DbFileType::TYPE_FASTA)) > 0){
					strTemp = FMT(XTANDEM_XML_TAXON_FILE,identMgr.getLocalDbFilePath(i,j,(kome::ident::DbFileType::TYPE_FASTA)).c_str());
					strWriteArray.push_back(strTemp);
				}
				strTemp = FMT(XTANDEM_XML_TAXON_END);
				strWriteArray.push_back(strTemp);
			}
		}
	}

	strTemp = FMT(XTANDEM_XML_BIOML_END);
	strWriteArray.push_back(strTemp);

	// write xml
	for ( unsigned int i = 0; i < strWriteArray.size(); i++ ) {
		ofs << strWriteArray.at( i ) << std::endl;
	}

	ofs.close();
	return true;
}

const char* XTandemXMLManager::translateTFtoYN(bool target){
	return target?"yes":"no";
}

bool XTandemXMLManager::createParameterXML(
		kome::objects::SettingParameterValues* paramSettings,
		const char* path,
		const char* defaultFile,
		const char* peaksFile,
		const char* residuesFile,
		const char* resultFile
){
	// managers
	kome::core::MsppManager& msppMgr = kome::core::MsppManager::getInstance();
	kome::ident::IdentificationManager& identMgr = kome::ident::IdentificationManager::getInstance();
	XTandemManager& mgr = XTandemManager::getInstance();

	// file open
	std::ofstream ofs(path, std::ios::out | std::ios::trunc);
	if (ofs.fail()) {
		// fail failed open
		LOG_ERROR(FMT("Failed to create Default Parameter xml file.[Failed to open]"));
		return false;
	}

	// write string array
	std::string strTemp;
	std::vector<std::string> strWriteArray;

	// create start
	strTemp = FMT(XTANDEM_XML_VERSION);
	strWriteArray.push_back(strTemp);
	strTemp = FMT(XTANDEM_XML_BIOML_START);
	strWriteArray.push_back(strTemp);
	strTemp = FMT(XTANDEM_XML_PATH_HEAD);
	strWriteArray.push_back(strTemp);
	strTemp = FMT(XTANDEM_XML_PATH_DEFAULT, replacestring( defaultFile, "\\", "/" ).c_str() );
	strWriteArray.push_back(strTemp);
	if( residuesFile != NULL && strlen( residuesFile ) > 0 ) {
		strTemp = FMT( XTANDEM_XML_PATH_RESIDUES, replacestring( residuesFile, "\\", "/" ).c_str() );
        strWriteArray.push_back(strTemp);
	}
	strTemp = FMT(XTANDEM_XML_SPEC_HEAD);
	strWriteArray.push_back(strTemp);
	strTemp = FMT(XTANDEM_XML_SPEC_FRAG_ERR, paramSettings->getDoubleValue(FRAGMENT_MASS_ERROR_PARAM_NAME, 0.4));
	strWriteArray.push_back(strTemp);
	strTemp = FMT(XTANDEM_XML_SPEC_FRAG_UNIT, NVL(paramSettings->getParameterValue(FRAGMENT_MASS_ERROR_UNIT_PARAM_NAME),"Da"));
	strWriteArray.push_back(strTemp);
	strTemp = FMT(XTANDEM_XML_SPEC_PARENT_PLUS, paramSettings->getDoubleValue(PARENT_MONOISOTOPIC_MASS_ERROR_PLUS_PARAM_NAME, 100));
	strWriteArray.push_back(strTemp);
	strTemp = FMT(XTANDEM_XML_SPEC_PARENT_MINUS, paramSettings->getDoubleValue(PARENT_MONOISOTOPIC_MASS_ERROR_MINUS_PARAM_NAME, 100));
	strWriteArray.push_back(strTemp);
	strTemp = FMT(XTANDEM_XML_SPEC_PARENT_UNIT, NVL(paramSettings->getParameterValue(PARENT_MONOISOTOPIC_MASS_ERROR_UNIT_PARAM_NAME),"ppm"));
	strWriteArray.push_back(strTemp);
	strTemp = FMT(XTANDEM_XML_SPEC_PATH, replacestring( peaksFile, "\\", "/" ).c_str() );
	strWriteArray.push_back(strTemp);
	if(strcmp(paramSettings->getParameterValue(REMOVE_REDUNDANT_ANGLE_PARAM_NAME),"") == 0){
		strTemp = FMT(XTANDEM_XML_SPEC_USE_ANGLE, "no");
		strWriteArray.push_back(strTemp);
	}
	else{
		strTemp = FMT(XTANDEM_XML_SPEC_USE_ANGLE, "yes");
		strWriteArray.push_back(strTemp);
		strTemp = FMT(XTANDEM_XML_SPEC_ANGLE, paramSettings->getDoubleValue(PARENT_MONOISOTOPIC_MASS_ERROR_MINUS_PARAM_NAME, 0));
		strWriteArray.push_back(strTemp);
	}
	
	if(strcmp(paramSettings->getParameterValue(NEUTRAL_LOSS_MASS_PARAM_NAME),"") == 0){
		strTemp = FMT(XTANDEM_XML_SPEC_USE_ANGLE, "no");
		strWriteArray.push_back(strTemp);
	}
	else{
		strTemp = FMT(XTANDEM_XML_SPEC_USE_NEUTRAL, "yes");
		strWriteArray.push_back(strTemp);
		strTemp = FMT(XTANDEM_XML_SPEC_NEUTRAL_WINDOW, paramSettings->getDoubleValue(NEUTRAL_LOSS_WINDOW_PARAM_NAME, 0));
		strWriteArray.push_back(strTemp);
		strTemp = FMT(XTANDEM_XML_SPEC_NEUTRAL_MASS, paramSettings->getDoubleValue(NEUTRAL_LOSS_MASS_PARAM_NAME, 0));
		strWriteArray.push_back(strTemp);
	}

	strTemp = FMT(XTANDEM_XML_RESI_HEAD);
	strWriteArray.push_back(strTemp);

	const char* mods = paramSettings->getParameterValue(COMPLETE_MODIFICATIONS_PARAM_NAME);
	std::string modifications = getModificationString( mods );
	if( !modifications.empty() ) {
		strTemp = FMT( XTANDEM_XML_RESI_MODIFICATION, modifications.c_str() );
		strWriteArray.push_back(strTemp);
	}

	mods = paramSettings->getParameterValue(POTENTIAL_MODIFICATIONS_PARAM_NAME);
	modifications = getModificationString( mods );
	if( !modifications.empty() ) {
		strTemp = FMT(XTANDEM_XML_RESI_POTENTIAL, modifications.c_str() );
		strWriteArray.push_back(strTemp);
	}

	strTemp = FMT(XTANDEM_XML_PROT_HEAD);
	strWriteArray.push_back(strTemp);
	strTemp = FMT(XTANDEM_XML_PROT_ANNOTATION, translateTFtoYN(paramSettings->getBoolValue(USE_ANNOTATION_PARAM_NAME,false)));
	strWriteArray.push_back(strTemp);
	
	std::string strTaxonomy = paramSettings->getParameterValue(TARGET_TAXONOMY_PARAM_NAME);
	strTaxonomy = replacestring( strTaxonomy.c_str(), "[", "" );
	strTaxonomy = replacestring( strTaxonomy.c_str(), "]", "" );

	while(strTaxonomy.find(':',0) != std::string::npos){
		strTaxonomy.replace(strTaxonomy.find(':',0),1,",");
	}

	strTemp = FMT(XTANDEM_XML_PROT_TAXON, strTaxonomy.c_str());
	strWriteArray.push_back(strTemp);
		
	int nNumberOfEnzymes = identMgr.getNumberOfEnzymes();
	kome::ident::Enzyme* enzyme = NULL;
	std::string enzymeStr;
	for(int i = 0; i < nNumberOfEnzymes && enzyme == NULL; i++){
		kome::ident::Enzyme* tmp = identMgr.getEnzyme( i );
		if( strcmp( tmp->getName().c_str(), paramSettings->getParameterValue( ENZYME_PARAM_NAME ) ) == 0 ) {
			enzyme = tmp;
		}
	}

	if( enzyme != NULL ) {
		if( enzyme->getName().compare( "None" ) == 0 ) {
			enzymeStr = "[X]|[X]";
		}
		else {
			for( unsigned int i = 0; i < enzyme->getNumberOfCutters(); i++ ) {
				kome::ident::Enzyme::Cutter* cutter = enzyme->getCutter( i );
				std::string strCleavage = "[X]";
				if( !cutter->cleave.empty() ) {
					strCleavage = FMT( "[%s]", cutter->cleave.c_str() );
				}
				std::string strNonCleavage = "[X]";
				if( !cutter->restrict.empty() ) {
					strNonCleavage = FMT( "{%s}", cutter->restrict.c_str() );
				}
				std::string cutterStr;
				if( cutter->term == kome::ident::Enzyme::C_TERM ) {
					cutterStr = FMT( "%s|%s", strCleavage.c_str(), strNonCleavage.c_str() );
				}
				else if( cutter->term == kome::ident::Enzyme::N_TERM ) {
					cutterStr = FMT( "%s|%s", strNonCleavage.c_str(), strCleavage.c_str() );
				}
				
				if( !cutterStr.empty() ) {
					if( !enzymeStr.empty() ) {
						enzymeStr.append( "," );
					}
					enzymeStr.append( cutterStr );
				}
			}
		}
	}

	strTemp = FMT( XTANDEM_XML_PROT_CLEAVAGE_SITE, enzymeStr.c_str() );
	strWriteArray.push_back(strTemp);
			
	strTemp = FMT(XTANDEM_XML_PROT_CLEAVAGE_SEMI, translateTFtoYN(paramSettings->getBoolValue(SEMI_STYLE_CLEAVAGE_PARAM_NAME,false)));
	strWriteArray.push_back(strTemp);

	strTemp = FMT(XTANDEM_XML_SCOR_HEAD);
	strWriteArray.push_back(strTemp);
	if((std::string(paramSettings->getParameterValue(IONS_PARAM_NAME)).find('a',0) != std::string::npos)){
		strTemp = FMT(XTANDEM_XML_SCOR_IONS, "a","yes");
	}
	else{
		strTemp = FMT(XTANDEM_XML_SCOR_IONS, "a","no");
	}
	strWriteArray.push_back(strTemp);
	if((std::string(paramSettings->getParameterValue(IONS_PARAM_NAME)).find('b',0) != std::string::npos)){
		strTemp = FMT(XTANDEM_XML_SCOR_IONS, "b","yes");
	}
	else{
		strTemp = FMT(XTANDEM_XML_SCOR_IONS, "b","no");
	}
	strWriteArray.push_back(strTemp);
	if((std::string(paramSettings->getParameterValue(IONS_PARAM_NAME)).find('c',0) != std::string::npos)){
		strTemp = FMT(XTANDEM_XML_SCOR_IONS, "c","yes");
	}
	else{
		strTemp = FMT(XTANDEM_XML_SCOR_IONS, "c","no");
	}
	strWriteArray.push_back(strTemp);
	if((std::string(paramSettings->getParameterValue(IONS_PARAM_NAME)).find('x',0) != std::string::npos)){
		strTemp = FMT(XTANDEM_XML_SCOR_IONS, "x","yes");
	}
	else{
		strTemp = FMT(XTANDEM_XML_SCOR_IONS, "x","no");
	}
	strWriteArray.push_back(strTemp);
	if((std::string(paramSettings->getParameterValue(IONS_PARAM_NAME)).find('y',0) != std::string::npos)){
		strTemp = FMT(XTANDEM_XML_SCOR_IONS, "y","yes");
	}
	else{
		strTemp = FMT(XTANDEM_XML_SCOR_IONS, "y","no");
	}
	strWriteArray.push_back(strTemp);
	if((std::string(paramSettings->getParameterValue(IONS_PARAM_NAME)).find('z',0) != std::string::npos)){
		strTemp = FMT(XTANDEM_XML_SCOR_IONS, "z","yes");
	}
	else{
		strTemp = FMT(XTANDEM_XML_SCOR_IONS, "z","no");
	}
	strWriteArray.push_back(strTemp);

	int missedCleavages = paramSettings->getIntValue( MAX_MISSED_CLEAVAGES, 1 );
	strTemp = FMT( XTANDEM_XML_SCOR_MAX_CLEAVAGE, missedCleavages );
	strWriteArray.push_back( strTemp );

	strTemp = FMT(XTANDEM_XML_REFI_HEAD);
	strWriteArray.push_back(strTemp);
	// @date 2013.07.17 <Mod> M.Izumi -> use refinement
	bool buseRef = paramSettings->getBoolValue( USE_REFINEMENT, true );
	strTemp = FMT(XTANDEM_XML_REFI_REFINE, (buseRef== true ? "yes" : "no"));
	strWriteArray.push_back(strTemp);

	if( buseRef ) {
		strTemp = FMT(XTANDEM_XML_REFI_MAX, paramSettings->getDoubleValue(VALID_EXPECTION_VALUE_PARAM_NAME,100));
		strWriteArray.push_back(strTemp);
		strTemp = FMT(XTANDEM_XML_REFI_ANNOTATION, translateTFtoYN(paramSettings->getBoolValue(REFINEMENT_USE_ANNOTATION_PARAM_NAME,false)));
		strWriteArray.push_back(strTemp);
		strTemp = FMT(XTANDEM_XML_REFI_CLEAVAGE_SEMI, translateTFtoYN(paramSettings->getBoolValue(REFIMENT_SEMI_STYLE_CLEAVAGE_PARAM_NAME,false)));
		strWriteArray.push_back(strTemp);

		mods = paramSettings->getParameterValue(REFINEMENT_POTENTIAL_MODIFICATIONS_PARAM_NAME);
		modifications = getModificationString( mods );
		if( !modifications.empty() ) {
			strTemp = FMT(XTANDEM_XML_REFI_POTENTIAL, modifications.c_str() );
			strWriteArray.push_back(strTemp);
		}

		strTemp = FMT(XTANDEM_XML_REFI_MUTATION, translateTFtoYN(paramSettings->getBoolValue(CHECK_POINT_MUTATIONS_PARAM_NAME,false)));
		strWriteArray.push_back(strTemp);
	}
	
	strTemp = FMT(XTANDEM_XML_OUT_HEAD);
	strWriteArray.push_back(strTemp);
	strTemp = FMT(XTANDEM_XML_OUT_PATH, replacestring(resultFile, "\\", "/" ).c_str() );
	strWriteArray.push_back(strTemp);
	strTemp = FMT(XTANDEM_XML_OUT_PATH_HASHING, "no");
	strWriteArray.push_back(strTemp);
	strTemp = FMT(XTANDEM_XML_OUT_RESULTS, "valid");
	strWriteArray.push_back(strTemp);

	double maxValidExpectationValue = 0.1;
	maxValidExpectationValue = paramSettings->getDoubleValue( MAX_VALID_EXPECTION_VALUE_PARAM_NAME, 0.1 );
	strTemp = FMT(XTANDEM_XML_OUT_MAX_VALID, maxValidExpectationValue );
	strWriteArray.push_back(strTemp);
	strTemp = FMT(XTANDEM_XML_BIOML_END);
	strWriteArray.push_back(strTemp);
		
	// write xml
	for ( unsigned int i = 0; i < strWriteArray.size(); i++ ) {
		ofs << strWriteArray.at( i ) << std::endl;
	}

	ofs.close();
	return true;
}

// modification string
std::string XTandemXMLManager::getModificationString( const char* name ) {
	// return value
	std::string ret;

	// manager
	kome::ident::IdentificationManager& identMgr = kome::ident::IdentificationManager::getInstance();

	// modification map
	std::map< std::string, kome::ident::Modification* > modMap;
	for( int i = 0; i < identMgr.getNumberOfModifications(); i++ ) {
		kome::ident::Modification* modification = identMgr.getModification( i );
		modMap[ modification->getName() ] = modification;
	}

	// separate
	std::vector< std::string > tokens;
	kome::plugin::SettingsValue::separateListValue( name, tokens );

	// make string
	for( unsigned int i = 0; i < tokens.size(); i++ ) {
		std::string token = tokens[ i ];
		if( modMap.find( token ) != modMap.end() ) {
			kome::ident::Modification* modification = modMap[ token ];
			std::string site =  modification->getAminoAcidResidue().c_str();
			std::string site_lc = tolowercase( site.c_str() );
			if( site_lc.find( "n-term" ) != site_lc.npos || site_lc.find( "nterm" ) != site_lc.npos ) {
				site = "[";
			}
			else if( site_lc.find( "c-term" ) != site_lc.npos || site_lc.find( "cterm" ) != site_lc.npos ) {
				site = "]";
			}

			if( !ret.empty() ) {
				ret.append( "," );
			}
			ret.append( FMT( "%.6f@%s", modification->getMonoMass(), site.c_str() ) );
		}
	}

	return ret;
}

// get instance
XTandemXMLManager& XTandemXMLManager::getInstance() {
	// create object (This is the only object.)
	static XTandemXMLManager mgr;

	return mgr;
}

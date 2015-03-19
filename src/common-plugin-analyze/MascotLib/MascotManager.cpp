/**
 * @file MascotManager.cpp
 * @brief implements of MascotManager class
 *
 * @author S.Tanaka
 * @date 2011.01.13
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "MascotManager.h"

#include <string>


#ifdef DUPLICATE
#undef DUPLICATE
#endif    // DUPLICATE


#include <msparser.hpp>


using namespace kome::mascot;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG

using namespace kome::ident;


// constructor
MascotManager::MascotManager() {
	m_sSession = "";
	m_sUsername = "";
	m_sUserID = "";
	m_sLoggedInURL = "";
	m_bLoggedInUseProxy = false;
}

// destructor
MascotManager::~MascotManager() {
}

// login to / logout from server
bool MascotManager::ServerLoginLogout(const bool bLogin)
{
	// managers
	kome::core::MsppManager& msppMgr = kome::core::MsppManager::getInstance();
	kome::net::NetworkManager& netMgr = kome::net::NetworkManager::getInstance();

	// tmp dir
	std::string tmpDir = getpath( msppMgr.getTmpDir(), "mascot" );
	if( !fileexists( tmpDir.c_str() ) ) {
		if( !makedirectory( tmpDir.c_str() ) ) {
			LOG_ERROR_CODE( FMT( "Failed to create the directory. [%s]", tmpDir.c_str() ), ERR_OTHER );
			return false;
		}
	}

	std::string url;
	bool bUseProxy = false;

	if (bLogin)
	{
		// When logging in, we use the settings from Tools -> Options...
		url = getUrl();
		bUseProxy = useProxy();

		// Save the URL and use proxy settings for logout.
		m_sLoggedInURL = url;
		m_bLoggedInUseProxy = bUseProxy;
	}
	else
	{
		// When logging out, we use the settings used when logging in.
		url = m_sLoggedInURL;
		bUseProxy = m_bLoggedInUseProxy;

		// Those settings are no longer needed, so are re-initialised.
		m_sLoggedInURL = "";
		m_bLoggedInUseProxy = false;
	}

	// connection
	kome::net::HttpConnection* con = netMgr.getHttpConnection();
	if( con == NULL ) {
		LOG_ERROR_CODE( FMT( "Failed to get the HTTP connection." ), ERR_NULL_POINTER);
		return false;
	}

	con->setMethod( kome::net::HttpConnection::METHOD_MULTI_PART );

	// Login to / logout from the Mascot server.

	// URL
	url.append( "cgi/login.pl" );

	// parameters
	if (bLogin)
	{
		con->setParameter( "action", "login" );
		con->setParameter( "username", getUsername().c_str() );
		con->setParameter( "password", getPassword().c_str() );
		con->setParameter( "display", "logout_prompt" );
		con->setParameter( "savecookie", "1" );
		con->setParameter( "onerrdisplay", "login_prompt" );
	}
	else
	{
		// set the cookie header if necessary.
		setCookieHeader(con);

		con->setParameter( "action", "logout" );
		con->setParameter( "onerrdisplay", "nothing" );
	}

	// open
	if( !con->openUrl( url.c_str() ) ) {
		LOG_ERROR_CODE( FMT( "Failed to connect to the Mascot server. [%s]", url.c_str() ), ERR_OTHER );
		return false;
	}
	con->sendRequest();

	// save HTM
	kome::core::Buffer buff;
	con->getResponse( buff );

	std::string tmpFile = msppMgr.getTmpFileName( "login", ".txt", "mascot" );
	std::string tmpPath = getpath( tmpDir.c_str(), tmpFile.c_str() );

	saveHeader( (char*)buff.getBuffer(), buff.getLength(), tmpPath.c_str() );

	// close
	con->closeConnection();
	con->release();

	char line[ 2048 ];
	FILE* fp = fileopen( tmpPath.c_str(), "r" );

	if (fp != NULL)
	{
		// Find and read database list
		while( (fgets( line, 2048, fp ) != NULL) ) {
			if( ( strstr( line, "Set-Cookie" ) ) != NULL ) {

				char* tmpString = NULL;			
				if( ( tmpString = strstr( line, "MASCOT_" ) ) != NULL ) {

					std::string mascotString = ( tmpString + 7 );
					std::string::size_type pos = mascotString.find( ";" );
					mascotString = mascotString.substr( 0, pos );

					std::vector<std::string> tokens;
					stringseparate(mascotString.c_str(), "=", tokens);

					if (tokens[0].compare("SESSION") == 0)
					{
						m_sSession = tokens[1];
					}
					else if (tokens[0].compare("USERNAME") == 0)
					{
						m_sUsername = tokens[1];
					}
					else if (tokens[0].compare("USERID") == 0)
					{
						m_sUserID = tokens[1];
					}
				}
			}
		}

		fclose( fp );
	}
	else
	{
		LOG_ERROR_CODE( FMT( "Failed to open the response file. [%s]", tmpPath.c_str() ), ERR_OTHER );
		return false;
	}

	return true;
}

// login to server
bool MascotManager::login()
{
	bool bSuccess = true;

	// If a username has been specified but no login session has been set, try to login.
	if ((!getUsername().empty()) && (m_sSession.empty()))
	{
		bSuccess = ServerLoginLogout(true);
		bSuccess = ((bSuccess && !m_sSession.empty()) && (!m_sUsername.empty() && !m_sUserID.empty()));

		if (!bSuccess)
		{
			LOG_ERROR_CODE( FMT( "Failed to login to the Mascot server." ), ERR_OTHER );
		}
	}

	return bSuccess;
}

// logout from server
bool MascotManager::logout()
{
	bool bSuccess = true;

	if (!m_sSession.empty())
	{
		bSuccess = ServerLoginLogout(false);
		bSuccess = ((bSuccess && m_sSession.empty()) && (m_sUsername.empty() && m_sUserID.empty()));

		if (!bSuccess)
		{
			LOG_ERROR_CODE( FMT( "Failed to logout from the Mascot server." ), ERR_OTHER );
		}
	}

	return bSuccess;
}

// set cookie header
void MascotManager::setCookieHeader(kome::net::HttpConnection* con)
{
	// If we are logged into the Mascot server, set a "Cookie" header for the connection.
	if (!m_sSession.empty())
	{
		std::string sCookieString = FMT("MASCOT_SESSION=%s; MASCOT_USERNAME=%s; MASCOT_USERID=%s", m_sSession.c_str(), m_sUsername.c_str(), m_sUserID.c_str());
		con->setHeader("Cookie", sCookieString.c_str());
	}
}

// get parameters
bool MascotManager::getMascotParameters(std::string& strPath, const char* search) {

	// Login to the Mascot server if necessary.
	login();

	// managers
	kome::core::MsppManager& msppMgr = kome::core::MsppManager::getInstance();
	kome::net::NetworkManager& netMgr = kome::net::NetworkManager::getInstance();

	// tmp dir
	std::string tmpDir = getpath( msppMgr.getTmpDir(), "mascot" );
	if( !fileexists( tmpDir.c_str() ) ) {
		if( !makedirectory( tmpDir.c_str() ) ) {
			LOG_ERROR_CODE( FMT( "Failed to create the directory. [%s]", tmpDir.c_str() ), ERR_OTHER );
			return false;
		}
	}

	// connection
	kome::net::HttpConnection* con = netMgr.getHttpConnection();
	if( con == NULL ) {
		LOG_ERROR_CODE( FMT( "Failed to get the HTTP connection." ), ERR_NULL_POINTER);
		return false;
	}

	con->setMethod( kome::net::HttpConnection::METHOD_GET );

	// URL
	std::string url = getUrl();
	url.append( "cgi/search_form.pl" );

	// set the cookie header if necessary.
	setCookieHeader(con);

	// parameters
	con->setParameter( "SEARCH", NVL( search, "" ) );
	con->setParameter( "FORMVER", "2" );

	// open
	if( !con->openUrl( url.c_str() ) ) {
		LOG_ERROR_CODE( FMT( "Failed to connect to the Mascot server. [%s]", url.c_str() ), ERR_OTHER );
		return false;
	}
	con->sendRequest();

	// save HTML
	kome::core::Buffer buffer;
	con->getResponse( buffer );

	std::string tmpFile = msppMgr.getTmpFileName( "parameters", ".html", "mascot" );
	std::string tmpPath = getpath( tmpDir.c_str(), tmpFile.c_str() );

	saveBody( (char*)buffer.getBuffer(), buffer.getLength(), tmpPath.c_str() );
	strPath = tmpPath;

	// close
	con->closeConnection();
	con->release();

	// Check for an error message in the response.

	// First, look for a "Fatal Error".
	// This can occur when unrecognised / invalid form data has been sent.
	std::string errorMessage;
	char line[ 2048 ];
	FILE* fp = fileopen( tmpPath.c_str(), "r" );
	if (fp != NULL)
	{
		bool bErrorFound = false;
		while( fgets( line, 2048, fp ) != NULL && !bErrorFound ) {
			char* error = NULL;			
			if( ( error = strstr( line, "Fatal Error" ) ) != NULL ) {

				bErrorFound = true;
			}
		}

		if (bErrorFound)
		{
			while( errorMessage.empty() && (fgets( line, 2048, fp ) != NULL) ) {
				char* error = NULL;			
				if( ( error = strstr( line, "<B>" ) ) != NULL ) {
					errorMessage = ( error + 3 );
					std::string::size_type pos = errorMessage.find( "<" );
					errorMessage = errorMessage.substr( 0, pos );
				}
			}
		}

		fclose(fp);
	}

	if (errorMessage.empty())
	{	// No error found yet.
		// Second, look for a "<TITLE>ERROR: ".
		// This can occur when a webhost cannot be found
		// (e.g. access denied, incorrect URL, proxy problem).
		fp = fileopen( tmpPath.c_str(), "r" );
		if (fp != NULL)
		{
			while( errorMessage.empty() && (fgets( line, 2048, fp ) != NULL) ) {
				char* error = NULL;			
				if( ( error = strstr( line, "<TITLE>ERROR: " ) ) != NULL ) {

					errorMessage = ( error + 14 );
					std::string::size_type pos = errorMessage.find( "<" );
					errorMessage = errorMessage.substr( 0, pos );
				}
			}

			fclose(fp);
		}
	}

	if (!errorMessage.empty())
	{
		LOG_ERROR_CODE( FMT( "Contacting Mascot Server failed. [%s]", errorMessage.c_str() ), ERR_OTHER );
		return false;
	}

	return true;
}

// indicate whether to split the peak list for searching
bool MascotManager::splitPeakList()
{
	std::string url = getUrl();
	// Split the peak list if the Mascot URL is www.matrixscience.com
	return (url.find("www.matrixscience.com") != std::string::npos);
}

// execute Mascot search for a peaks file
std::string MascotManager::executeSubSearch(
		const char* search,
		const char* title,
		const char* peaksFile,
		kome::objects::SettingParameterValues* settings,
		kome::core::Progress& progress,
		const char* outFile
) {
	// return value
	std::string ret;

	// search type
	std::string type = NVL( search, "" );

	// Create subprogresses
	progress.createSubProgresses(3);

	// managers
	kome::net::NetworkManager& netMgr = kome::net::NetworkManager::getInstance();
	kome::ident::IdentificationManager& identMgr = kome::ident::IdentificationManager::getInstance();
	kome::core::MsppManager& msppMgr = kome::core::MsppManager::getInstance();

	// connection
	kome::net::HttpConnection* con = netMgr.getHttpConnection();
	if( con == NULL ) {
		LOG_ERROR_CODE( FMT( "Failed to get the HTTP connection." ), ERR_NULL_POINTER);
		return ret;
	}
	con->setMethod( kome::net::HttpConnection::METHOD_MULTI_PART );

	// URL
	std::string url = getUrl();
	url.append( "cgi/nph-mascot.exe?1" );

	// set the cookie header if necessary.
	setCookieHeader(con);

	if (progress.isStopped())
	{
		return ret;
	}

	kome::core::Progress* subProgress = progress.getSubProgress( 0 );
	subProgress->setStatus("Executing Mascot Search...");

	// hidden parameters
	con->setParameter( "INTERMEDIATE", "" );
	con->setParameter( "FORMVER", "1.01" );
	con->setParameter( "SEARCH", type.c_str() );
	con->setParameter( "PEAK", "AUTO" );

	con->setParameter( "ErrTolRepeat", "0" );
	con->setParameter( "SHOWALLMODS", "0" );

	// common parameters
	con->setParameter( "USERNAME", settings->getParameterValue(USERNAME_PARAM_NAME) );
	con->setParameter( "USEREMAIL", settings->getParameterValue(EMAIL_PARAM_NAME) );
	con->setParameter( "COM", title );
	con->setParameter( "DB", settings->getParameterValue(DB_PARAM_NAME) );
	con->setParameter( "CLE", settings->getParameterValue(ENZYME_PARAM_NAME) );
	con->setParameter( "PFA", settings->getParameterValue(ALLOWED_MISSED_CLEAVAGES_PARAM_NAME) );
	con->setParameter( "TAXONOMY", settings->getParameterValue(TAXONOMY_PARAM_NAME) );
	con->setParameter( "TOL", settings->getParameterValue(PEPTIDE_TOL_PARAM_NAME) );
	con->setParameter( "TOLU", settings->getParameterValue(PEPTIDE_UNIT_PARAM_NAME) );
	con->setParameter( "MASS", settings->getParameterValue(MASS_PARAM_NAME) );
	con->setParameter( "FILE", peaksFile, true );
	con->setParameter( "REPORT", settings->getParameterValue(REPORTNUM_PARAM_NAME) );

	std::vector<std::string> fixedModsArray;
	kome::plugin::SettingsValue::separateListValue(settings->getParameterValue(FIXEDMODS_PARAM_NAME), fixedModsArray);
	for (int i = 0; i < static_cast<int>(fixedModsArray.size()); i++)
	{
		con->setParameter( "MODS", fixedModsArray[i].c_str() );
	}
	std::vector<std::string> variableModsArray;
	kome::plugin::SettingsValue::separateListValue(settings->getParameterValue(VARIABLE_PARAM_NAME), variableModsArray);
	for (int i = 0; i < static_cast<int>(variableModsArray.size()); i++)
	{
		con->setParameter( "IT_MODS", variableModsArray[i].c_str() );
	}

	// each parameters
	if( type.compare( "PMF" ) == 0 ) {    // PMF
		con->setParameter( "REPTYPE", "concise" );
		con->setParameter( "DECOY", ( settings->getBoolValue( DECOY_PARAM_NAME, false ) ? "1" : "0" ) );
		con->setParameter( "SEG", settings->getParameterValue( PROTEIN_MASS_PARAM_NAME ) );
		con->setParameter( "CHARGE", settings->getParameterValue( CHARGE_PARAM_NAME ) );
	}
	else if( type.compare( "MIS" ) == 0 ) {    // MIS
		con->setParameter( "REPTYPE", "peptide" );
		con->setParameter( "CHARGE", convertPeptideChargeString(settings->getParameterValue(PEPTIDE_CHARGE_PARAM_NAME)).c_str() );
		con->setParameter( "QUANTITATION", settings->getParameterValue(QUANTITATION_PARAM_NAME) );
		con->setParameter( "PEP_ISOTOPE_ERROR", settings->getParameterValue(PEPTIDE_ISOTOPE_ERROR_PARAM_NAME) );
		con->setParameter( "ITOL", settings->getParameterValue(MSMS_TOL_PARAM_NAME) );
		con->setParameter( "ITOLU", settings->getParameterValue(MSMS_UNIT_PARAM_NAME) );
		con->setParameter( "FORMAT", "Mascot generic" );	
		con->setParameter( "PRECURSOR", "" );
		con->setParameter( "ERRORTOLERANT", strcmp(settings->getParameterValue(ERROR_TOL_DECAY_PARAM_NAME), "Error Tolerant") ? "0" : "1" );
		con->setParameter( "DECOY", strcmp(settings->getParameterValue(ERROR_TOL_DECAY_PARAM_NAME), "Decoy") ? "0" : "1" );
		con->setParameter( "INSTRUMENT", settings->getParameterValue(INSTRUMENT_PARAM_NAME) );
	}

	// open
	if( !con->openUrl( url.c_str() ) ) {
		LOG_ERROR_CODE( FMT( "Failed to connect to the Mascot server. [%s]", url.c_str() ), ERR_OTHER );
		return ret;
	}
	con->sendRequest();

	// response
	std::string tmpDir = getpath( msppMgr.getTmpDir(), "mascot" );
	std::string tmpFile = msppMgr.getTmpFileName( "response", ".txt", "mascot" );
	std::string tmpPath = getpath( tmpDir.c_str(), tmpFile.c_str() );
	FILE* fp = fileopen( tmpPath.c_str(), "wb" );
	kome::core::FileAccessor acc( fp );
	con->getResponse( acc );

	// close
	fclose( fp );
	con->closeConnection();
	con->release();

	// dat file path
	std::string datFileURL;
	std::string datFilePath;
	char line[ 2048 ];
	fp = fileopen( tmpPath.c_str(), "r" );
	while( fgets( line, 2048, fp ) != NULL && datFilePath.empty() ) {
		char* result = NULL;			
		char* equal = NULL;			
		if( ( result = strstr( line, "master_results" ) ) != NULL ) {
			equal = strstr( result, "=" );
			if( equal != NULL ) {
				datFilePath = ( equal + 1 );
				std::string::size_type pos = datFilePath.find( "\"" );
				datFilePath = datFilePath.substr( 0, pos );
				datFileURL = result + 14; // Next character after "master_results"
				datFileURL = datFileURL.substr(0, equal - result - 14);
				datFileURL += "=";
				datFileURL += datFilePath;
			}
		}
	}

	if (!datFilePath.empty())
	{
		if (progress.isStopped())
		{
			return ret;
		}
		
		subProgress->fill();
		subProgress = progress.getSubProgress( 1 );
		subProgress->setStatus("Confirming Mascot Search Results...");

		// get result
		con = netMgr.getHttpConnection();
		url = getUrl();
		url.append( "cgi/master_results.pl" );

		// set the cookie header if necessary.
		setCookieHeader(con);

		con->setMethod( kome::net::HttpConnection::METHOD_GET );

		con->setParameter( "file", datFilePath.c_str() );

		if( !con->openUrl( url.c_str() ) ) {
			LOG_ERROR_CODE( FMT( "Failed to connect to the Mascot server. [%s]", url.c_str() ), ERR_OTHER );
			return ret;
		}
		con->sendRequest();

		tmpFile = msppMgr.getTmpFileName( "result", ".txt", "mascot" );
		tmpPath = getpath( tmpDir.c_str(), tmpFile.c_str() );
		fp = fileopen( tmpPath.c_str(), "wb" );

		kome::core::FileAccessor acc( fp );
		con->getResponse( acc );

		fclose( fp );
		con->closeConnection();
		con->release();

		// Check for an error message in the response.

		// First, look for a "Fatal error".
		// This can occur when an invalid file has been specified.
		std::string errorMessage;
		char line[ 2048 ];
		FILE* fp = fileopen( tmpPath.c_str(), "r" );
		if (fp != NULL)
		{
			bool bErrorFound = false;
			while( fgets( line, 2048, fp ) != NULL && !bErrorFound ) {
				char* error = NULL;			
				if( ( error = strstr( line, "Fatal error" ) ) != NULL ) {

					bErrorFound = true;
				}
			}

			if (bErrorFound)
			{
				while( errorMessage.empty() && (fgets( line, 2048, fp ) != NULL) ) {
					char* error = NULL;			
					if( ( error = strstr( line, "<p>" ) ) != NULL ) {
						errorMessage = ( error + 3 );
						std::string::size_type pos = errorMessage.find( "<" );
						errorMessage = errorMessage.substr( 0, pos );
					}
				}
			}

			fclose(fp);
		}

		if (errorMessage.empty())
		{	// No error found yet.
			// Second, look for a "Fatal Error".
			fp = fileopen( tmpPath.c_str(), "r" );
			if (fp != NULL)
			{
				bool bErrorFound = false;
				while( fgets( line, 2048, fp ) != NULL && !bErrorFound ) {
					char* error = NULL;			
					if( ( error = strstr( line, "Fatal Error" ) ) != NULL ) {

						bErrorFound = true;
					}
				}

				if (bErrorFound)
				{
					while( errorMessage.empty() && (fgets( line, 2048, fp ) != NULL) ) {
						char* error = NULL;			
						if( ( error = strstr( line, "<B>" ) ) != NULL ) {
							errorMessage = ( error + 3 );
							std::string::size_type pos = errorMessage.find( "<" );
							errorMessage = errorMessage.substr( 0, pos );
						}
					}
				}

				fclose(fp);
			}
		}

		if (!errorMessage.empty())
		{
			LOG_ERROR_CODE( FMT( "Failed to confirm Mascot search results. [%s]", errorMessage.c_str() ), ERR_OTHER );
			return ret;
		}

		if (progress.isStopped())
		{
			return ret;
		}
		
		subProgress->fill();
		subProgress = progress.getSubProgress( 2 );
		subProgress->setStatus("Getting Mascot Search Results DAT File...");

		// get dat
		con = netMgr.getHttpConnection();

		url = getUrl();
		url.append( "cgi/export_dat_2.pl" );

		// set the cookie header if necessary.
		setCookieHeader(con);

		con->setMethod( kome::net::HttpConnection::METHOD_GET );

		con->setParameter( "file", datFilePath.c_str() );
		con->setParameter( "do_export", "1" );
		con->setParameter( "prot_hit_num", "1" );
		con->setParameter( "prot_acc", "1" );
		con->setParameter( "pep_query", "1" );
		con->setParameter( "pep_rank", "1" );
		con->setParameter( "pep_isbold", "1" );
		con->setParameter( "pep_isunique", "1" );
		con->setParameter( "pep_exp_mz", "1" );
		con->setParameter( "export_format", "MascotDAT" );

		if( !con->openUrl( url.c_str() ) ) {
			LOG_ERROR_CODE( FMT( "Failed to connect to the Mascot server. [%s]", url.c_str() ), ERR_OTHER );
			return datFilePath;
		}
		con->sendRequest();

		tmpPath = NVL( outFile, "" );

		kome::core::Buffer buffer;
		con->getResponse( buffer );
		saveBody( (char*)buffer.getBuffer(), buffer.getLength(), tmpPath.c_str() );
		con->closeConnection();
		con->release();
				
		subProgress->fill();

		// Check for an error message in the response.
		// If there is an error message on the first line, it is NOT a valid DAT file and
		// should not be used / copied.
		errorMessage.clear();
		fp = fileopen( tmpPath.c_str(), "r" );
		if (fp != NULL)
		{
			fgets( line, 2048, fp );
			char* error = NULL;			
			if( ( error = strstr( line, "Software error" ) ) != NULL ) {
				if (fgets( line, 2048, fp ) != NULL)
				{
					error = strstr( line, "<pre>" );
					if (error != NULL)
					{
						errorMessage = (error + 5);
						std::string::size_type pos = errorMessage.find( "<" );
						errorMessage = errorMessage.substr( 0, pos );
					}
				}
			}
			fclose(fp);
		}

		if (!errorMessage.empty())
		{
			LOG_ERROR_CODE( FMT( "Saving Mascot Results DAT file failed. [%s]", errorMessage.c_str() ), ERR_OTHER );
			return datFilePath;
		}
	}
	else
	{
		// An error has occurred.
		// Get the error string from the response.
		std::string errorMessage;
		char line[ 2048 ];
		fp = fileopen( tmpPath.c_str(), "r" );
		if (fp != NULL)
		{
			while( fgets( line, 2048, fp ) != NULL && errorMessage.empty() ) {
				char* error = NULL;			
				if( ( error = strstr( line, "Sorry, your search could not be performed due to the following mistake entering data." ) ) != NULL ) {
					if (fgets( line, 2048, fp ) != NULL)
					{
						errorMessage = line;
						std::string::size_type pos = errorMessage.find( "<" );
						errorMessage = errorMessage.substr( 0, pos );
					}
				}
			}
			fclose(fp);
		}

		if (errorMessage.empty())
		{	// No error found yet.
			// Second, look for "HTTP"
			fp = fileopen( tmpPath.c_str(), "r" );
			if (fp != NULL)
			{
				fgets( line, 2048, fp );
				char* error = NULL;			
				if( ( error = strstr( line, "HTTP" ) ) != NULL ) {

					errorMessage = error;
				}
				fclose(fp);
			}
		}

		if (!errorMessage.empty())
		{
			LOG_ERROR_CODE( FMT( "Mascot Search failed. [%s]", errorMessage.c_str() ), ERR_OTHER );
		}
		else
		{
			LOG_ERROR_CODE( FMT( "Mascot Search failed. [Failed to get the error message]" ), ERR_OTHER );
		}
		return datFilePath;
	}

	return datFilePath;
}

// get URL
const char* MascotManager::getUrl() {
	if( m_url.empty() ) {
		// default url
		m_url = "http://www.matrixscience.com/";

		// ini
		kome::core::MsppManager& msppMgr = kome::core::MsppManager::getInstance();
		kome::core::IniFile* ini = msppMgr.getIniFile();

		if( ini != NULL ) {
			m_url = ini->getString( SECTION, URL_KEY, m_url.c_str() );
			if( !m_url.empty() ) {
				char c = m_url[ (int)m_url.length() - 1 ];
				if( c != '/' ) {
					m_url.append( "/" );
				}
			}
		}
	}

	return m_url.c_str();
}

// using proxy or not
bool MascotManager::useProxy() {
	// ini
	kome::core::MsppManager& msppMgr = kome::core::MsppManager::getInstance();
	kome::core::IniFile* ini = msppMgr.getIniFile();
	if( ini == NULL ) {
		return false;
	}

	// value
	const bool val = ini->getBool( SECTION, PROXY_KEY, false );
	return val;
}

// get username
std::string MascotManager::getUsername() {
	// username
	std::string username;

	// ini
	kome::core::MsppManager& msppMgr = kome::core::MsppManager::getInstance();
	kome::core::IniFile* ini = msppMgr.getIniFile();
	if( ini == NULL ) {
		return username;
	}

	// value
	username = ini->getString( SECTION, USERNAME_KEY, "" );

	if (username.compare("none") == 0)
	{
		username.clear();
	}

	return username;
}

// get password
std::string MascotManager::getPassword() {
	// password
	std::string password;

	// ini
	kome::core::MsppManager& msppMgr = kome::core::MsppManager::getInstance();
	kome::core::IniFile* ini = msppMgr.getIniFile();
	if( ini == NULL ) {
		return password;
	}

	// value
	password = ini->getString( SECTION, PASSWORD_KEY, "" );

	if (password.compare("none") == 0)
	{
		password.clear();
	}

	return password;
}

// save header
void MascotManager::saveHeader( char* res, const unsigned int size, const char* file ) {
	// create body
	std::vector< char > header;
	int step = 0;
	for( unsigned int i = 0; (i < size) && (step <= 2); i++ ) {
		char c = res[ i ];

		if( step == 0 || step == 2 ) {
			if( c == '\r' ) {
				step++;
			}
			else {
				step = 0;
			}
		}
		else if( step == 1 ) {
			if( c == '\n' ) {
				step++;
			}
			else {
				step = 0;
			}
		}

		if ( c != '\r' )
		{
			header.push_back( c );
		}
	}

	// save
	FILE* fp = fileopen( file, "wb" );
	if( fp == NULL ) {
		return;
	}

	if( header.size() > 0 ) {
		fwrite( &( header[ 0 ] ), 1, header.size(), fp );
	}        

	// close
	fclose( fp );
}

// save body
void MascotManager::saveBody( char* res, const unsigned int size, const char* file ) {
	// create body
	std::vector< char > body;
	int step = 0;
	for( unsigned int i = 0; i < size; i++ ) {
		char c = res[ i ];

		if( step >= 4 ) {
			if( c != '\r' ) {
				body.push_back( c );
			}
		}
		else if( step == 0 || step == 2 ) {
			if( c == '\r' ) {
				step++;
			}
			else {
				step = 0;
			}
		}
		else if( step == 1 || step == 3 ) {
			if( c == '\n' ) {
				step++;
			}
			else {
				step = 0;
			}
		}
	}

	// save
	FILE* fp = fileopen( file, "wb" );
	if( fp == NULL ) {
		return;
	}

	if( body.size() > 0 ) {
		fwrite( &( body[ 0 ] ), 1, body.size(), fp );
	}        

	// close
	fclose( fp );
}

// convert the peptide charge string from one using colon separators to commas and "and".
std::string MascotManager::convertPeptideChargeString( const char* peptideChargeValue )
{
	std::string commaDelimString;

	for (int i = 0; i < static_cast<int>(strlen(peptideChargeValue)); i++)
	{
		if (peptideChargeValue[i] != ':')
		{
			commaDelimString += peptideChargeValue[i];
		}
		else
		{
			if (strchr(peptideChargeValue + i + 1, ':') == NULL)
			{	// No other : is found after the current one
				commaDelimString += " and ";
			}
			else
			{	// Another colon is found after the current one
				commaDelimString += ", ";
			}
		}
	}

	return commaDelimString;
}

// get page information
bool MascotManager::getPageInfo( const char* search, SearchPageInfo& info, kome::core::Progress& progress ) {
	// string
	std::string s = trimstring( search );

	// get the structure
	SearchPageInfo* org = &( m_pageMap[ s ] );

	// get the data
	if( org->dbs.empty() && org->taxonomies.empty() ) {
		// progress
		progress.setRange( 0, 3 );
		progress.setPosition( 0 );

		// In the case of Mascot, must connect to the internet to get this information.
		// Http is used to get the databases and taxonomy information.
		// manager
		MascotManager& mgr = MascotManager::getInstance();
		std::string tmpParamPath;

		// parameters
		if( !mgr.getMascotParameters(tmpParamPath, s.c_str()) ) {
			return false;
		}

		bool bReadingDatabaseList = false;
		bool bReadingTaxonomyList = false;
		char line[ 2048 ];
		FILE* fp = fileopen( tmpParamPath.c_str(), "r" );
		if( fp == NULL ) {
			LOG_ERROR_CODE( FMT( "Failed to open the response file. [%s]", tmpParamPath.c_str() ), ERR_OTHER );
			return false;
		}

		// Find and read database list
		while( (!bReadingDatabaseList) && (fgets( line, 2048, fp ) != NULL) ) {
			char* database = NULL;
			if( ( database = strstr( line, "Database(s)" ) ) != NULL ) {
				// Ignore 2 lines.
				fgets( line, 2048, fp );
				fgets( line, 2048, fp );
				bReadingDatabaseList = true;
			}
		}

		if (bReadingDatabaseList)
		{
			std::string strDatabase;
			while( (bReadingDatabaseList) && (fgets( line, 2048, fp ) != NULL) ) {
				char* database = NULL;
				if( ( database = strstr( line, "<OPTION" ) ) != NULL ) {
					database = strstr( database, ">" );
					if( database != NULL ) {
						strDatabase = ( database + 1 );
						std::string::size_type pos = strDatabase.find( "<" );
						strDatabase = strDatabase.substr( 0, pos );
						org->dbs.push_back( strDatabase );
					}
				}
				else
				{
					bReadingDatabaseList = false;
				}
			}
		}

		// Find and read taxonomy list
		while( (!bReadingTaxonomyList) && (fgets( line, 2048, fp ) != NULL) ) {
			char* taxonomy = NULL;
			if( ( taxonomy = strstr( line, "Taxonomy" ) ) != NULL ) {
				// Ignore 2 lines.
				fgets( line, 2048, fp );
				fgets( line, 2048, fp );
				bReadingTaxonomyList = true;
			}
		}
		if (bReadingTaxonomyList)
		{
			std::string strTaxonomy;
			while( (bReadingTaxonomyList) && (fgets( line, 2048, fp ) != NULL) ) {
				char* taxonomy = NULL;
				if( ( taxonomy = strstr( line, "<OPTION" ) ) != NULL ) {
					taxonomy = strstr( taxonomy, ">" );
					if( taxonomy != NULL ) {
						strTaxonomy = ( taxonomy + 1 );
						std::string::size_type pos = strTaxonomy.find( "<" );
						strTaxonomy = strTaxonomy.substr( 0, pos );
						org->taxonomies.push_back( strTaxonomy );
					}
				}
				else
				{
					bReadingTaxonomyList = false;
				}
			}
		}
		
		fclose( fp );
		progress.setPosition( 1 );
		if( progress.isStopped() ) {
			return true;
		}

		// manager
		kome::ident::IdentificationManager& identMgr = kome::ident::IdentificationManager::getInstance();

		// Only continue and try to get modifications if we have successfully got the database and taxonomy information.
		// MascotParser is used to get the modifications information, via the MascotHelper console application.

		// modifications
		if( !updateModifications() ) {
			return false;
		}

			
		if (identMgr.getNumberOfModifications() == 0)
		{
			LOG_ERROR_CODE( FMT( "Failed to get Mascot modifications information.\nCheck your internet connection settings; confirm that you can view the Mascot website in your internet browser."), ERR_OTHER );
			return false;
		}
		progress.setPosition( 2 );

		// enzymes
		if( !updateEnzymes() ) {
			return false;
		}
		if( identMgr.getNumberOfEnzymes() == 0 ) {
			LOG_ERROR_CODE( FMT( "Failed to get Mascot enzymes information.\nCheck your internet connection settings; confirm that you can view the Mascot website in your internet browser." ), ERR_OTHER );
			return false;
		}

// >>>>>> @Date:2014/01/27	<Modified>	A.Ozaki
// fixed setting value. (NULL)->(3)
		progress.setPosition( 3 );

		// If, although the Mascot server was successfully connected to, but the database list is empty
		// cannot allow the user to use the (Comparative) Identification dialogs.
		if (org->dbs.size() == 0 )
		{
			LOG_ERROR_CODE( FMT( "Failed to get Mascot databases information. [Unknown error]"), ERR_OTHER );
		}
	}

	// copy
	for( unsigned int i = 0; i < org->dbs.size(); i++ ) {
		info.dbs.push_back( org->dbs.at( i ) );
	}

	for( unsigned int i = 0; i < org->taxonomies.size(); i++ ) {
		info.taxonomies.push_back( org->taxonomies.at( i ) );
	}

	progress.fill();
	return true;
}

// update modifications
bool MascotManager::updateModifications() {
	// managers
	kome::core::MsppManager& msppMgr = kome::core::MsppManager::getInstance();
	kome::ident::IdentificationManager& identMgr = kome::ident::IdentificationManager::getInstance();
	MascotManager& mgr = MascotManager::getInstance();

	// ini
	kome::core::IniFile* ini = msppMgr.getIniFile();

	// login
	if( m_sSession.empty() ) {
		login();
	}

	// connection
	matrix_science::ms_connection_settings cs;
	if( !m_sSession.empty() ) {
		cs.setSessionID( m_sSession );
	}

	std::string url = mgr.getUrl();
	if( url.empty() ) {
		LOG_ERROR( FMT( "Failed to get the Mascot server url." ) );
		return false;
	}
	url.append( "cgi" );

	// modifications
	matrix_science::ms_masses masses;
//	matrix_science::ms_modfile modFile( url.c_str(), &masses, false, &cs );
	matrix_science::ms_modfile modFile( "http://www.matrixscience.com/cgi", &masses, false, &cs );

	int num = modFile.getNumberOfModifications();
	for( int i = 0; i < num; i++ ) {
		const matrix_science::ms_modification* modification = modFile.getModificationByNumber( i );
		std::string title = modification->getTitle();
		double delta = modification->getDelta( matrix_science::MASS_TYPE_MONO );
		bool hidden = modification->isHidden();

		std::string residue;
		int iStartBracketPos = title.rfind("(");
		int iEndBracketPos = title.rfind(")");
		if ((iStartBracketPos != std::string::npos) && (iEndBracketPos != std::string::npos))
		{
			residue = title.substr(iStartBracketPos + 1, iEndBracketPos - iStartBracketPos - 1);
		}


		kome::ident::Modification m( title.c_str(), delta, residue.c_str(), hidden );
		identMgr.updateModification( &m );
	}

	return true;
}

// update enzymes
bool MascotManager::updateEnzymes() {
	// managers
	kome::core::MsppManager& msppMgr = kome::core::MsppManager::getInstance();
	kome::ident::IdentificationManager& identMgr = kome::ident::IdentificationManager::getInstance();
	MascotManager& mgr = MascotManager::getInstance();

	// ini
	kome::core::IniFile* ini = msppMgr.getIniFile();

	// login
	if( m_sSession.empty() ) {
		login();
	}

	// connection
	std::string url = mgr.getUrl();
	url.append( "cgi" );

	matrix_science::ms_connection_settings cs;
	if( !m_sSession.empty() ) {
		cs.setSessionID( m_sSession );
	}

	// enzyme
	matrix_science::ms_enzymefile file( url.c_str(), &cs );
	int num = file.getNumberOfEnzymes();

	for( int i = 0; i < num; i++ ) {
		const matrix_science::ms_enzyme* enzyme = file.getEnzymeByNumber( i );
		std::string title = enzyme->getTitle();

		kome::ident::Enzyme e( trimstring( title.c_str() ).c_str() );

		for( int j = 0; j < enzyme->getNumberOfCutters(); j++ ) {
			matrix_science::ms_enzyme::cuttertype term = enzyme->getCutterType( j );
			std::string cleave = trimstring( enzyme->getCleave( j ).c_str() );;
			std::string restrict = trimstring( enzyme->getRestrict( j ).c_str() );

			kome::ident::Enzyme::TermType t = kome::ident::Enzyme::UNKNOWN;
			if( term == matrix_science::ms_enzyme::CTERM_CUTTER ) {
				t = kome::ident::Enzyme::C_TERM;
			}
			else if( term == matrix_science::ms_enzyme::NTERM_CUTTER ) {
				t = kome::ident::Enzyme::N_TERM;
			}

			e.addCutter( t, cleave.c_str(), restrict.c_str() );
		}

		identMgr.updateEnzyme( &e );
	}

	return true;
}

// get instance
MascotManager& MascotManager::getInstance() {
	// create object (This is the only object.)
	static MascotManager mgr;

	return mgr;
}

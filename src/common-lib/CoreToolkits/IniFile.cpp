/**
 * @file IniFile.cpp
 * @brief implements of IniFile class
 *
 * @author S.Tanaka
 * @date 2006.08.18
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "IniFile.h"

#include <list>


#define INI_LINE_BUFFER_SIZE	0x1000

// >>>>>>	暗号化	@Date:2013/10/18	<Add>	A.Ozaki
//
//#define _CRT_SECURE_NO_DEPRECATE	// 重複していると外します
#define	CRYPTOPP_DEFAULT_NO_DLL
#define	CRYPTOPP_ENABLE_NAMESPACE_WEAK 1

// ここからは「おまじない」
#pragma	push_macro( "new" )
#undef	new
#include	"dll.h"
#pragma	pop_macro( "new" )
// 「おまじない」おわり

#pragma comment( lib, "cryptlib.lib" )	// ライブラリの指定
USING_NAMESPACE(CryptoPP)
typedef unsigned char       BYTE;
//
// <<<<<<	暗号化	@Date:2013/10/18	<Add>	A.Ozaki

using namespace kome::core;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
IniFile::IniFile() {
}

// destructor
IniFile::~IniFile() {
}

// get file path
const char* IniFile::getFilePath() {
	return m_filePath.c_str();
}

// set string value
void IniFile::setString( const char* section, const char* key, const char* value ) {
	// create string object
	std::string sec = NVL( section, "" );

	// set section
	if( m_sectionMap.find( sec ) == m_sectionMap.end() ) {
		m_sections.push_back( sec );
	}

	// set value
	m_sectionMap[ sec ].setValue( key, value );
}

// get string value
const char* IniFile::getString( const char* section, const char* key, const char* defaultValue ) {
	// create string object
	std::string sec = NVL( section, "" );

	// search section
	if( m_sectionMap.find( sec ) == m_sectionMap.end() ) {
		return defaultValue;
	}

	// get parameter
	return m_sectionMap[ sec ].getStringValue( key, defaultValue );
}

bool	IniFile::decryptPassword( const char* pcPassword, char* pcDecrypt, const size_t szDecrypt )
{
	if  ( (char *)NULL == pcPassword || (char *)NULL == pcDecrypt )
	{
		return	false;
	}

	std::string	strVal = pcPassword;
	if  ( -1 != strVal.find( "@@@" ) )
	{
		try
		{
			byte	bKey[AES::DEFAULT_KEYLENGTH];	// 共通鍵
			byte	bIv[AES::BLOCKSIZE];			// IV

			// キーとIVに適当な値設定します
			//   暗号化・符号化に同じ値を設定する必要があります
			memset( (void *)bKey, 0x01, AES::DEFAULT_KEYLENGTH );
			memset( (void *)bIv, 0x01, AES::BLOCKSIZE );

			//------------------------------------------------
			//                復号化
			//------------------------------------------------
			int i = strVal.find_last_of( "@@@" );
			strVal = strVal.substr( i + 1,  ( strVal.length( ) - i ) );

			std::string strInputDec;
			HexDecoder	insHexDecode( new StringSink( strInputDec ) );
			insHexDecode.Put( (BYTE*)( strVal.c_str( ) ), strVal.size( ) );
			insHexDecode.MessageEnd( );

			// 復号化オブジェクト作成
			CTR_Mode<AES>::Decryption	insDecryption;
			insDecryption.SetKeyWithIV( bKey, sizeof( bKey ), bIv );	// キーとIVを復号化オブジェクトに設定

			// 変換フィルタを作成し、暗号文と復号後の出力先バッファを渡す
			std::string	strOutputDec;
			StreamTransformationFilter	insStfDec( insDecryption, new StringSink( strOutputDec ) );
			insStfDec.Put( (BYTE*)strInputDec.c_str( ), strInputDec.size( ) );
			insStfDec.MessageEnd( );

			strVal = strOutputDec;
		}
		catch ( ... )
		{
		}
	}

	memset( (void *)pcDecrypt, 0x00, szDecrypt );
	if  ( szDecrypt <= strVal.length( ) )
	{
		return	false;
	}
	memcpy( (void *)pcDecrypt, (char *)strVal.c_str( ), strVal.length( ) );

	return	true;
}

bool	IniFile::encryptPassword( const char* pcPassword, char* pcEncrypt, const size_t szEncrypt )
{
	if  ( (char *)NULL == pcPassword || (char *)NULL == pcEncrypt )
	{
		return	false;
	}

	std::string	strVal = pcPassword;
	try
	{
		byte	bKey[AES::DEFAULT_KEYLENGTH];	// 共通鍵
		byte	bIv[AES::BLOCKSIZE];			// IV

		// キーとIVに適当な値設定します
		//   暗号化・符号化に同じ値を設定する必要があります
		memset( (void *)bKey, 0x01, AES::DEFAULT_KEYLENGTH );
		memset( (void *)bIv, 0x01, AES::BLOCKSIZE );

		//------------------------------------------------
		//                暗号化
		//------------------------------------------------
		// 暗号化オブジェクト作成
		CTR_Mode<AES>::Encryption	insEncryptor;
		insEncryptor.SetKeyWithIV( bKey, sizeof( bKey ), bIv );		// キーとIVを暗号化オブジェクトに設定

		// 変換フィルタを作成し、平文と変換後の出力先バッファを渡します
		std::string	strInputEnc = strVal;
		std::string	strOutputEnc;
		StreamTransformationFilter insStfEnc( insEncryptor, new StringSink( strOutputEnc ) );
		insStfEnc.Put( (BYTE*)( strInputEnc.c_str( ) ), strInputEnc.size( ) );
		insStfEnc.MessageEnd( );

		// 暗号化されたモノをHex形式に変換します
		std::string	strHex;
		HexEncoder	insHexEncode( new StringSink( strHex ) );
		insHexEncode.Put( (BYTE*)( strOutputEnc.c_str( ) ), strOutputEnc.size( ) );
		insHexEncode.MessageEnd( );

		strVal.clear( );
		strVal.append( "@@@" );
		strVal.append( strHex );
	}
	catch ( ... )
	{
	}

	memset( (void *)pcEncrypt, 0x00, szEncrypt );
	if  ( szEncrypt <= strVal.length( ) )
	{
		return	false;
	}
	memcpy( (void *)pcEncrypt, (char *)strVal.c_str( ), strVal.length( ) );

	return	true;

}

// set boolean value
void IniFile::setBool( const char* section, const char* key, const bool value ) {
	// create string object
	std::string sec = NVL( section, "" );

	// set section
	if( m_sectionMap.find( sec ) == m_sectionMap.end() ) {
		m_sections.push_back( sec );
	}

	// set value
	m_sectionMap[ sec ].setValue( key, ( value ? "true" : "false" ) );
}

// get boolean value
bool IniFile::getBool( const char* section, const char* key, const bool defaultValue ) {
	// create string object
	std::string sec = NVL( section, "" );

	// search section
	if( m_sectionMap.find( sec ) == m_sectionMap.end() ) {
		return defaultValue;
	}

	// get parameter
	return m_sectionMap[ sec ].getBoolValue( key, defaultValue );
}

// set integer value
void IniFile::setInt( const char* section, const char* key, const int value ) {
	// create string object
	std::string sec = NVL( section, "" );

	// set section
	if( m_sectionMap.find( sec ) == m_sectionMap.end() ) {
		m_sections.push_back( sec );
	}

	// set value
	m_sectionMap[ sec ].setValue( key, FMT( "%d", value ).c_str() );
}

// get integer value
int IniFile::getInt( const char* section, const char* key, const int defaultValue ) {
	// create string object
	std::string sec = NVL( section, "" );

	// search section
	if( m_sectionMap.find( sec ) == m_sectionMap.end() ) {
		return defaultValue;
	}

	// get parameter
	return m_sectionMap[ sec ].getIntValue( key, defaultValue );
}

// set double value
void IniFile::setDouble( const char* section, const char* key, const double value ) {
	// create string object
	std::string sec = NVL( section, "" );

	// set section
	if( m_sectionMap.find( sec ) == m_sectionMap.end() ) {
		m_sections.push_back( sec );
	}

	// set value
	m_sectionMap[ sec ].setValue( key, FMT( "%f", value ).c_str() );
}

// delete value
void IniFile::deleteValue( const char* section, const char* key ) {
	// create string object
	std::string sec = NVL( section, "" );

	// check the map
	if( m_sectionMap.find( sec ) == m_sectionMap.end() ) {
		return;
	}

	// delete
	m_sectionMap[ sec ].deleteParameter( key );
	if( m_sectionMap[ sec ].getNumberOfProperties() == 0 ) {
		m_sectionMap.erase( sec );

		for( std::vector< std::string >::iterator it = m_sections.begin(); it != m_sections.end(); it++ )
		{
			if (!it->compare( sec ))
			{
				m_sections.erase(it);
				break;
			}
		}
	}
}

// get properties
Properties* IniFile::getProperties( const char* section ) {
	// check the parameter
	if( section == NULL  ) {
		return NULL;
	}

	// check the map
	if( m_sectionMap.find( section ) == m_sectionMap.end() ) {
		return NULL;
	}

	// get properties
	return &m_sectionMap[ section ];
}

// get double value
double IniFile::getDouble( const char* section, const char* key, const double defaultValue ) {
	// create string object
	std::string sec = NVL( section, "" );

	// search section
	if( m_sectionMap.find( sec ) == m_sectionMap.end() ) {
		return defaultValue;
	}

	// get parameter
	return m_sectionMap[ sec ].getDoubleValue( key, defaultValue );
}

// get the number of sections
unsigned int IniFile::getNumberOfSections() {
	return m_sections.size();
}

// get section
const char* IniFile::getSection( const unsigned int index ) {
	// check the parameter
	if( index >= m_sections.size() ) {
		return NULL;
	}

	return m_sections[ index ].c_str();
}

// get the number of parameters
unsigned int IniFile::getNumberOfParameters( const char* section ) {
	// get properties
	Properties* properties = getProperties( section );

	// get size
	if( properties == NULL ) {
		return 0;
	}
	return properties->getNumberOfProperties();
}

// get parameter name
const char* IniFile::getParameterName( const char* section, const unsigned int index ) {
	// get properties
	Properties* properties = getProperties( section );

	// get parameter name
	if( properties == NULL ) {
		return NULL;
	}
	return properties->getKey( index );
}

// get parameter value
const char* IniFile::getParameterValue( const char* section, const unsigned int index ) {
	// get properties
	Properties* properties = getProperties( section );

	// get parameter name
	if( properties == NULL ) {
		return NULL;
	}
	return properties->getValue( index );
}

// load the file
bool IniFile::load( const char* path ) {
	// log
	LOG_INFO( FMT( "Loading INI file... [%s]", NVL( path, "" ) ) );

	// check file
	if( !checkfile( path ) ) {
		LOG_WARN( FMT( "Illegal path [%s]", NVL( path, "" ) ) );
		return false;
	}

	// clear
	m_sections.clear();
	m_sectionMap.clear();
	m_filePath.clear();

	// open
	std::string p = absolutepath( path );
	FILE* fp = fileopen( p.c_str(), "r" );
	if( fp == NULL ) {
		LOG_WARN( FMT( "Failed to open the file. [%s]", NVL( path, "" ) ) );
		return false;
	}
	m_filePath = p;

	// read
	char line[ INI_LINE_BUFFER_SIZE ];
	Properties* params = NULL;
	std::string section;

	while( fgets( line, INI_LINE_BUFFER_SIZE, fp ) != NULL ) {	// read line
		// read line
		line[ INI_LINE_BUFFER_SIZE - 1 ] = '\0';

		if( line[ 0 ] == '#' || line[ 0 ] == ';' ) {	// comment
			continue;
		}

		// trim
		std::string l = trimstring( line );
		memcpy( line, l.c_str(), l.length() );
		line[ l.length() ] = '\0';
		
		if( line[ 0 ] == '[' ) {	// section
			char* c = strstr( line, "]" );
			if( c != NULL ) {
				*c = '\0';
			}
			c = strstr( line, "[" );
			if( c != NULL ) {
				*c = '\0';
			}

			section = std::string( line + 1 );
			params = NULL;
		}
		else {
			char* key = line;
			char* value = strstr( line, "=" );
			if( value != NULL ) {
				*value = '\0';
				value++;

				if( params == NULL ) {
					params = &m_sectionMap[ section ];
					m_sections.push_back( section );
				}
				// >>>>>>	@Date:2014/02/07	<Add>	A.Ozaki
				//
				// パスワードの符号化処理
				//   アプリケーション内は、符号化された内容を保有するように変更
				//   符号化対象は、
				//     key:"PASSWORD"/"COMMON_DB_PASSWORD"
				//     value:先頭３文字が"@@@"で始まる
				//
				std::string	strKey = tolowercase( key );
				std::string strValue = value;
				if  ( ( 0 == strKey.compare( "password" ) ||
					0 == strKey.compare( "common_db_password" ) )
					&& -1 != strValue.find( "@@@" ) )
				{
					char	cBuff[256];

					if  ( true == decryptPassword( strValue.c_str( ), cBuff, sizeof( cBuff ) ) )
					{
						strValue = cBuff;
					}
				}
//				params->setValue( key, value );
				params->setValue( key, strValue.c_str( ) );
				//
				// <<<<<<	@Date:2014/02/07	<Add>	A.Ozaki
			}
		}
	}

	// close
	fclose( fp );

	return true;
}

// save
bool IniFile::save( const char* path ) {
	// absolutepath 
	std::string p = absolutepath( NVL( path, m_filePath.c_str() ) );

	// log
	LOG_INFO( FMT( "Saving INI file... [%s]", p.c_str() ) );

	// file open
	FILE* fp = fileopen( p.c_str(), "w" );
	if( fp == NULL ) {
		LOG_ERROR(
			FMT(
				"Failed to open the file. [%s]",
				p.c_str()
			)
		);
		return false;
	}

	// write to file
	for( unsigned int i = 0; i < m_sections.size(); i++ ) {
		// write section
		std::string section = m_sections[ i ];

		if( !section.empty() ) {
			fprintf( fp, "[%s]\n", section.c_str() );
		}

		// write Properties
		Properties* params = &m_sectionMap[ section ];
		for( unsigned int i = 0; i < params->getNumberOfProperties(); i++ ) {
			// >>>>>>	@Date:2014/02/07	<Add>	A.Ozaki
			//
			// パスワードの暗号化処理
			//   iniファイルに出力するときに暗号化します
			//   暗号化対象は、
			//     key:"PASSWORD"/"COMMON_DB_PASSWORD"
			//     value:"none"以外
			//
			std::string	strKey = tolowercase( params->getKey( i ) );
			std::string strValue = params->getValue( i );
			if  ( ( 0 == strKey.compare( "password" ) || 0 == strKey.compare( "common_db_password" ) )
				&& ( 0 != strValue.compare( "none" ) ) )
			{
				char	cBuff[256];

				if  ( true == encryptPassword( params->getValue( i ), cBuff, sizeof( cBuff ) ) )
				{
					strValue = cBuff;
				}
			}
//			fprintf( fp, "%s=%s\n", params->getKey( i ), params->getValue( i ) );
			fprintf( fp, "%s=%s\n", params->getKey( i ), strValue.c_str( ) );

			//
			// <<<<<<	@Date:2014/02/07	<Add>	A.Ozaki
		}

		fprintf( fp, "\n" );
	}

	// close
	fflush( fp );
	fclose( fp );

	return true;
}

// load ini file
IniFile* IniFile::loadIniFile( const char* path ) { 
	// manager
	IniFileManager& mgr = IniFileManager::getInstance();

	// absolute path
	std::string p = absolutepath( path );

	// get object from map
	IniFile* iniFile = mgr.getFile( path );
	if( iniFile == NULL ) {	// create new object
		iniFile = new IniFile();
		if( iniFile->load( p.c_str() ) ) {	// success
			mgr.setFile( iniFile );
		}
		else{	// fail
			delete iniFile;
			iniFile = NULL;
		}
	}

	return iniFile;
}

// get ini file information object
IniFile* IniFile::getIniFile( const char* path ) {
	// manager
	IniFileManager& mgr = IniFileManager::getInstance();

	// absolutepath
	std::string p = absolutepath( path );

	// get object from map
	IniFile* iniFile = mgr.getFile( p.c_str() );
	if( iniFile == NULL ) {
		if( fileexists( p.c_str() ) ) {	// load file
			iniFile = loadIniFile( p.c_str() );
		}
		else {	// create new file
			iniFile = new IniFile();
			iniFile->m_filePath = p;
			mgr.setFile( iniFile );
		}
	}

	return iniFile;
}

// save all ini files
void IniFile::saveAllIniFiles() {
	// manager
	IniFileManager& mgr = IniFileManager::getInstance();

	// save
	mgr.save();
}



// IniFileManager --------------------------------------------------------

// constructor
IniFile::IniFileManager::IniFileManager() {
}

// destructor
IniFile::IniFileManager::~IniFileManager() {
	// delete ini file
	for( std::map< std::string, IniFile* >::iterator it = m_fileMap.begin();
			it != m_fileMap.end(); it++ ) {
		IniFile* ini = (*it).second;
		delete ini;
	}
	m_fileMap.clear();
}

// set ini file information to map
void IniFile::IniFileManager::setFile( IniFile* file ) {
	// check parameter
	if( file == NULL ) {
		return;
	}

	// absolutepath
	std::string path = absolutepath( file->getFilePath() );

	// search map
	if( m_fileMap.find( path ) != m_fileMap.end() ) {
		delete m_fileMap[ path ];
	}

	// set to map
	m_fileMap[ path ] = file;
}

// get ini file information from map
IniFile* IniFile::IniFileManager::getFile( const char* path ) {
	// absolute path
	std::string p = absolutepath( path );

	// search map
	if( m_fileMap.find( p ) == m_fileMap.end() ) {
		return NULL;
	}

	return m_fileMap[ p ];
}

// save all ini file
void IniFile::IniFileManager::save() {
	// delete ini file
	for( std::map< std::string, IniFile* >::iterator it = m_fileMap.begin();
			it != m_fileMap.end(); it++ ) {
		IniFile* ini = (*it).second;
		ini->save();
	}
}

// get instance
IniFile::IniFileManager& IniFile::IniFileManager::getInstance() {
	// create object
	static IniFileManager mgr;

	return mgr;
}

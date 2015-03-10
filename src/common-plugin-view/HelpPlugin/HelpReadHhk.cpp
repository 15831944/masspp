/**
 * @file HelpReadHhk.cpp
 * @brief read HHK file class
 * @author Y.Murata
 * @date 2011.12.21
 * Copyright (C) 2014 Shimadzu Corporation All rights reserved.
 */

#include "stdafx.h"
#include "HelpReadHhk.h"
#include "HelpMacros.h"

#include <fstream>
#include <wx\html\htmlpars.h>
#include <wx\html\m_templ.h>

using namespace kome::view;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


// constructor
HelpReadHhk::HelpReadHhk( std::vector< ST_HELP_KEY_OBJECT >* pvctHelpKey/* = NULL*/ )
{
	// initialize
	m_pvctHelpKey = pvctHelpKey;
}

// destructor
HelpReadHhk::~HelpReadHhk()
{
}

// Create an HHK file from the specified keyword.
bool HelpReadHhk::createKeywordInfo( const char* szHhkFileName, const char* szHtmlFullPath )
{
	std::ifstream ifHtmlFile;
	std::string   strRead;
	std::string   strNameFirst;
	std::string   strNameSecond;
	std::string   strLocal;
	char          szRead[READ_LINE_NUM_MAX];
	bool          bObjFlg  = false;
	int           nNameCnt = 0;

	/* Check pointer */
	if( m_pvctHelpKey == NULL ){
		LOG_ERROR( FMT("ST_HELP_KEY_OBJECT pointer is NULL." ) );
		return false;
	}

	/* File open */
	ifHtmlFile.open( szHhkFileName, std::ios::in );
	if( !ifHtmlFile ){
		/* Error */
		LOG_ERROR( FMT("Hhk File Open ERROR" ) );
		LOG_ERROR( FMT("File Name = %s", szHhkFileName ) );
		return false;
	}

	/* Read to the end of file. */
	for( int line=0; !ifHtmlFile.eof();line++ ){
		ifHtmlFile.getline( szRead, READ_LINE_NUM_MAX * sizeof( char ) );
		strRead = szRead;

		if( strRead.find( "<LI>", 0) != std::string::npos ){
			/* When you find the LI tag processing. */
			bObjFlg = true; /* Starting the Configuration object. */
		}
		
		if( strRead.find("<param",0) != std::string::npos ){
			/* When you find the param tag processing. */
			if( bObjFlg == true ){
				int offset = 0;

				/* To hold the contents of the param tags only for the start of the configuration object. */
				if( strRead.find("\"Name\"",0) != std::string::npos ){
					offset = strRead.find("value", 0);
					int startOffset = 0;
					int lastOffset  = 0;
					startOffset = strRead.find("\"",offset);
					lastOffset  = strRead.find("\"",startOffset+1);

					if( nNameCnt == 0 ){
						strNameFirst = "";
						for( int i = (startOffset+1); i<lastOffset; i++ ){
							strNameFirst += strRead.at(i);
						}
						nNameCnt++;
					}
					else{
						strNameSecond = "";
						for( int i = (startOffset+1); i<lastOffset; i++ ){
							strNameSecond += strRead.at(i);
						}
					}
				}
				else if( strRead.find("\"Local\"",0) != std::string::npos ){
					offset = strRead.find("value", 0);
					int startOffset = 0;
					int lastOffset  = 0;
					startOffset = strRead.find("\"",offset);
					lastOffset  = strRead.find("\"",startOffset+1);
					strLocal = "";
					for( unsigned int i = (startOffset+1); i<lastOffset; i++ ){
						strLocal += strRead.at(i);
					}
				}
				else{
					/* No treatment. */
				}
			}
		}
		if( strRead.find("</LI>",0) != std::string::npos ){
			/* Configuration object at the end of treatment. */
			bObjFlg  = false; /* End of Configuration Object. */
			nNameCnt = 0;
			/* Save up to create the best keywords. */
			ST_HELP_KEY_OBJECT  stHelpKeyObj;
			stHelpKeyObj.strIdxName = strNameFirst;
			stHelpKeyObj.strKeyName = strNameSecond;
			stHelpKeyObj.strLocal   = szHtmlFullPath;
			stHelpKeyObj.strLocal  += "help\\"; // Add a help folder path.
			stHelpKeyObj.strLocal  += strLocal;
			m_pvctHelpKey->push_back( stHelpKeyObj );
			
			int offset = strRead.find("</LI>",0);
			if( strRead.find("<LI>",offset) != std::string::npos )
			{ /* To check whether there is <LI> the same line. */
				bObjFlg = true;
			}
		}
	}
	/* File close */
	ifHtmlFile.close();

	return true;
}

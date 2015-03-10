/**
 * @file HelpReadHhc.cpp
 * @brief read HHK file class
 * @author Y.Murata
 * @date 2011.12.21
 * Copyright (C) 2014 Shimadzu Corporation All rights reserved.
 */

#include "stdafx.h"
#include "HelpReadHhc.h"
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
HelpReadHhc::HelpReadHhc( std::vector< ST_HELP_OBJECT >* pvctHelp/* = NULL*/ )
{
	// initialize
	m_pvctHelp = pvctHelp;
	m_nId = 0;

	if( m_pvctHelp != NULL ){
		/* Create a route */
		ST_HELP_OBJECT stHelpObj;
		stHelpObj.nID       = m_nId;
		stHelpObj.nParentID = -1;
		m_pvctHelp->push_back(stHelpObj);
	}

	/* Set the root information. */
	m_vctParentId.clear();
	m_vctParentId.push_back(0);
}

// destructor
HelpReadHhc::~HelpReadHhc()
{
	m_vctParentId.clear();
}

// Create an HHC file from the specified chapters.
bool HelpReadHhc::createChapterInfo( const char* szHhkFileName, const char* szHtmlFullPath )
{
	std::ifstream ifHtmlFile;
	std::string   strRead;
	std::string   strName;
	std::string   strLocal;
	char          szRead[READ_LINE_NUM_MAX];
	int           nTagUL  = 0;
	bool          bObjFlg = false;
	bool          bObjEndFlg   = false;
	bool          bULStartFlg  = false;
	int           nPreParentID = 0;

	/* Check pointer */
	if( m_pvctHelp == NULL ){
		LOG_ERROR( FMT("ST_HELP_OBJECT pointer is NULL." ) );
		return false;
	}

	/* File open */
	ifHtmlFile.open( szHhkFileName, std::ios::in );
	if( !ifHtmlFile ){
		/* Error */
		LOG_ERROR( FMT("Hhc File Open ERROR") );
		LOG_ERROR( FMT("File Name = %s", szHhkFileName ) );
		return false;
	}

	/* Read to the end of file. */
	for( int line=0; !ifHtmlFile.eof();line++ ){
		ifHtmlFile.getline( szRead, READ_LINE_NUM_MAX * sizeof( char ) );
		strRead = szRead;
		if( strRead.find("</UL>",0) != std::string::npos ){
			nTagUL--; /* Decrement the position of the paragraph. */
			m_vctParentId.pop_back();
			std::string::size_type nULoffset = strRead.find("</UL>",0)+1;
			/* Check if there is </UL> the same line. */
			while( strRead.find("</UL>",nULoffset) != std::string::npos )
			{
				nTagUL--;
				m_vctParentId.pop_back();
				nULoffset = strRead.find("</UL>",nULoffset) + 1;
			}
		}
	    else if( strRead.find( "<UL>", 0) != std::string::npos ){
			/* When you find a <UL>. */
			nTagUL++; /* Increment the position of the paragraph. */
			m_vctParentId.push_back(nPreParentID);
		}
		else {
			/* No action */
		}

		if( strRead.find( "<LI>", 0) != std::string::npos ){
			/* When you find a <LI>. */
			bObjFlg = true; /* Starting the Configuration object. */
			bObjEndFlg = false;
		}
		
		if( strRead.find("<param",0) != std::string::npos ){
			/* When you find the param tag processing. */
			if( bObjFlg == true ){
				std::string::size_type offset = 0;

				/* To hold the contents of the param object only when the starting configuration. */
				if( strRead.find("\"Name\"",0) != std::string::npos ){
					offset = strRead.find("value", 0);
					std::string::size_type startOffset = 0;
					std::string::size_type lastOffset  = 0;
					startOffset = strRead.find("\"",offset);
					lastOffset  = strRead.find("\"",startOffset+1);
					strName = "";
					for( unsigned int i = (startOffset+1); i<lastOffset; i++ ){
						strName += strRead.at(i);
					}
				}
				else if( strRead.find("\"Local\"",0) != std::string::npos ){
					offset = strRead.find("value", 0);
					int startOffset = 0;
					int lastOffset  = 0;
					startOffset = strRead.find("\"",offset);
					lastOffset  = strRead.find("\"",startOffset+1);
					strLocal = "";
					for( int i = (startOffset+1); i<lastOffset; i++ ){
						strLocal += strRead.at(i);
					}
				}
				else{
					/* No action */
				}
			}
		}
		if( strRead.find("</LI>",0) != std::string::npos ){
			/* Configuration object at the end of treatment. */
			bObjFlg = false; /* End of Configuration Object. */
			ST_HELP_OBJECT stHelpObj;
			bool bCheckFlg = true;

			/* To add information chapters. */
			stHelpObj.strName   = strName;
			stHelpObj.strLocal  = szHtmlFullPath;
			stHelpObj.strLocal  += "help\\";
			stHelpObj.strLocal  += strLocal;
			stHelpObj.nParentID = m_vctParentId[ nTagUL ];
			if( stHelpObj.nParentID >= 0 ){
				for( unsigned int Idx=0;Idx<(*m_pvctHelp)[ stHelpObj.nParentID ].nChildId.size();Idx++ ){
					int  nChildId = (*m_pvctHelp)[ stHelpObj.nParentID ].nChildId[Idx];
					if( stHelpObj.strName == (*m_pvctHelp)[ nChildId ].strName ) 
					{
						/* What Happens When wearing the "Name" */
						bCheckFlg = false;
						nPreParentID = (*m_pvctHelp)[ nChildId ].nID;
						break;
					}
				}
			}
			
			if( bCheckFlg == true ){
				/* What Happens When You suffered a "Name". */
				m_nId++;
				nPreParentID = m_nId;
				stHelpObj.nID       = m_nId;
				stHelpObj.nParentID = m_vctParentId[ nTagUL ];
				(*m_pvctHelp)[ stHelpObj.nParentID ].nChildId.push_back(nPreParentID);
				m_pvctHelp->push_back( stHelpObj );
			}
		}
	}
	/* File close */
	ifHtmlFile.close();

	m_vctParentId.clear();
	m_vctParentId.push_back(0);

	return true;
}

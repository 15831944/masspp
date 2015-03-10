/**
 * @file HelpManager.cpp
 * @brief implements of HelpManager class
 *
 * @author S.Tanaka
 * @date 2007.03.16
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "HelpManager.h"
#include "HelpMacros.h"
#include "HelpReadHhk.h"
#include "HelpReadHhc.h"

#include <vector>
#include <algorithm>
#include <wx/html/helpctrl.h>

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
HelpManager::HelpManager() {
	m_controller = NULL;
}

// destructor
HelpManager::~HelpManager() {
	if( m_controller != NULL ) {
		delete m_controller;
		m_controller = NULL;
	}
}

// show help
bool HelpManager::showHelpWindow() {
	// create help
	if( m_controller == NULL ) {
		createHelp();
	}
	if( m_controller == NULL ) {
		return false;
	}

	// show help
	m_controller->DisplayContents();

	return true;
}

// create help
void HelpManager::createHelp() {
	// get help
	std::vector< kome::plugin::PluginHelp* > helps;

	//Handling Policy 1
	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();
	for( unsigned int i = 0; i < plgMgr.getNumberOfPlugins(); i++ ) {
		// get plug-in 
		kome::plugin::PluginInfo* plugin = plgMgr.getPlugin( i );

		// get helps
		for( unsigned int j = 0; (plugin!=NULL) && j < plugin->getNumberOfHelps(); j++ ) { // NULL チェック @date 2013.06.19 <Mod> M.Izumi
			helps.push_back( plugin->getHelp( j ) );
		}
	}

	/* The order, file name, you get the absolute path. */
	int nOrder    = 0;
	int nPreOrder = 10;
	bool bOrderInfo = false;
	bool bResult;
	std::string helpFile;
	HelpManager::m_vctHtmlFullPath.clear();
	m_vctHtmlPath.clear();

	/* Plug-sort the information. */
	std::sort( helps.begin(), helps.end(), lessHelp );

	for( unsigned int j=0;j<helps.size();j++ ){
		nOrder = helps.at(j)->getOrder();
		helpFile = helps.at(j)->getHelpFile();
		std::string path;
		std::string dir;
		if( helps.at(j)->getPlugin() == NULL ) {
			path = helps.at(j)->getHelpFile();
		}
		else {
			path = getpath( helps.at(j)->getPlugin()->getDir(), helps.at(j)->getHelpFile() );
		}
		dir = helps.at(j)->getPlugin()->getDir();
		dir += '\\';
		HelpManager::m_vctHtmlFullPath.push_back(dir);
		if( nOrder < nPreOrder ){
			nPreOrder = nOrder;
			bOrderInfo = true;
		}
		else{
			bOrderInfo = false;
		}
		/* Handling Policy 2 */
		bResult = getFromHppFileToHTMLPath( path.c_str(), j, bOrderInfo );
		if( bResult != true ){
			return;
		}
		/* Order value and the output path. */
		LOG_TRACE_CODE( FMT( "FileName = %s", path.c_str() ), ERR_OTHER );
		LOG_TRACE_CODE( FMT( "Order    = %d", nOrder ), ERR_OTHER );       
	}

	/* Variable initialization section information */
	m_HelpChapterInfo.clear();
	HelpReadHhc	hhcRead( &m_HelpChapterInfo );
	/* Variable initialization Keyword information */
	m_HelpKeywordInfo.clear();
	HelpReadHhk	hhkRead( &m_HelpKeywordInfo );
	/* Information from the HTML files for each plug-in section, to get the best keywords. */
	for(unsigned int i = 0;i<m_vctHtmlFullPath.size();i++){
		std::string pathHhc;
		std::string pathHhk;
		pathHhc = m_vctHtmlFullPath[i];
		pathHhk = m_vctHtmlFullPath[i];
		pathHhc += "help\\toc.hhc";  //Add the following to help pass
		pathHhk += "help\\index.hhk";//Add the following to help pass
		/* Section to obtain information. */
		bResult = hhcRead.createChapterInfo(pathHhc.c_str(), m_vctHtmlFullPath[i].c_str());
		if( bResult != true ){
			return;
		}
		/* To obtain information about the kyword. */
		bResult = hhkRead.createKeywordInfo(pathHhk.c_str(), m_vctHtmlFullPath[i].c_str());
		if( bResult != true ){
			return;
		}
	}

	//2012/01/13 add by Y.Murata -> Sort by the menu item.
	unsigned int	i, j;
	std::string		strMenu;
	std::string		strItem;
	kome::plugin::PluginMenu&	plgMenu = plgMgr.getMenu();
	kome::plugin::PluginMenu*	pplgChild;

	// Sort of the first hierarchy.
	for( i = 0; i < plgMenu.getNumberOfChildren(); ++i ){
		pplgChild = plgMenu.getChildMenu( i );
		if( pplgChild != NULL ){
			strMenu = getDotCutName( pplgChild->getName() );

			// Search of the item in unison with menu item.
			for( j = 0; j < m_HelpChapterInfo[ 0 ].nChildId.size(); ++j ){
				int		nIndex = m_HelpChapterInfo[ 0 ].nChildId[ j ];

				strItem = m_HelpChapterInfo[ nIndex ].strName;
				if( strItem.at( 0 ) == '[' ){
					strItem.erase( 0, 1 );
					std::string::size_type nPos = strItem.find( "]" );
					if( nPos != std::string::npos ){
						strItem.erase( nPos );

						// When there is an according item
						if( compareignorecase( strMenu.c_str(), strItem.c_str() ) == 0 ){
							// Sorts less than the item.
							sortHelpChapterByMenu( nIndex, pplgChild );

							// It travels the item at the end.
							m_HelpChapterInfo[ 0 ].nChildId.erase( m_HelpChapterInfo[ 0 ].nChildId.begin() + j );
							m_HelpChapterInfo[ 0 ].nChildId.push_back( nIndex );
							break;
						}
					}
				}
			}
		}
	}
	//2012/01/13 add by Y.Murata <- Sort by the menu item.

	/* Create a variety of temporary files. */
	bResult = createTemporaryHppFile(); //HHP file
	if( bResult != true ){
		return;
	}
	bResult = createTemporaryHhcFile(); //HHC file
	if( bResult != true ){
		return;
	}
	bResult = createTemporaryHhkFile(); //HHK file
	if( bResult != true ){
		return;
	}

	// create controller
	int style = wxHF_TOOLBAR | wxHF_FLAT_TOOLBAR | wxHF_CONTENTS
		| wxHF_INDEX | wxHF_SEARCH | wxHF_PRINT //| wxHF_ICONS_BOOK
		| wxHF_ICONS_FOLDER | wxHF_MERGE_BOOKS | wxHF_ICONS_BOOK_CHAPTER;
	m_controller = new wxHtmlHelpController( style );

	m_controller->AddBook( wxT(m_strHhpFilePath.c_str()) );

	/* clear */
	m_HelpChapterInfo.clear();
	m_HelpKeywordInfo.clear();
}

// compare help to sort
bool HelpManager::lessHelp( kome::plugin::PluginHelp* help0, kome::plugin::PluginHelp* help1 ) {
	return ( help0->getOrder() < help1->getOrder() );
}

// get instance
HelpManager& HelpManager::getInstance() {
	// create object (This is the only object)
	static HelpManager mgr;

	return mgr;
}

// get from hpp file to html path
bool HelpManager::getFromHppFileToHTMLPath( const char* szHppFileName, int nIdx, bool bOrderInfo ) {
	std::ifstream ifHppFile;
	std::string   strRead;
	std::string   strBufFullPath;
	char          szRead[READ_LINE_NUM_MAX];
	bool          bHtmlFlag = false;
	bool          bFirstIdx = true;

	/* To check the parameters. */
	if( nIdx < 0 ){
		return false;
	}

	strBufFullPath = m_vctHtmlFullPath[nIdx];
	ifHppFile.open( szHppFileName, std::ios::in );
	/* Whether opende files. */
	if( !ifHppFile ){
		/* Returns an error */
		LOG_ERROR( FMT( "Hhp File Open ERROR" ) );
		LOG_ERROR( FMT( "File Name = %s", szHppFileName ) );
		return false;
	}

	/* Read to the end of the File. */
	for( int line=0; !ifHppFile.eof();line++ ){
		/* Read one line */
		ifHppFile.getline( szRead, READ_LINE_NUM_MAX * sizeof( char ) );
		strRead = szRead;
		
		if( (bHtmlFlag == true) && (strRead != "") ){
			/* [FILES]found only after the processing is performed. */
			strBufFullPath += "help\\"; // plus the path of the help folder crowded.
			strBufFullPath += strRead;
			/* Holds the full path to HTML file. */
			m_vctHtmlPath.push_back(strBufFullPath);
			if( (bFirstIdx == true) && (bOrderInfo==true) ){
				m_strDefaultFilePath = strBufFullPath;
				bFirstIdx = false;
			}
			/* Update the following path. */
			strBufFullPath = m_vctHtmlFullPath[nIdx];
		}

		/* [FILES] Check */
		if( strRead == "[FILES]" ){
			bHtmlFlag = true;
		}
	}
	ifHppFile.close();

	return true;
}

//HHP file to create a temporary.
bool HelpManager::createTemporaryHppFile( void )
{
	std::ofstream ofHppFile;
	std::string   strWrite;

	// Manager
	kome::core::MsppManager& msppMgr = kome::core::MsppManager::getInstance();

	// help Folder
	std::string folderName = "help";
	std::string helpFolder = getpath( msppMgr.getTmpDir(), folderName.c_str() );
	if( !fileexists( helpFolder.c_str() ) ) {
		makedirectory( helpFolder.c_str() );
	}

	// file name (not path)
	std::string hhpFile = msppMgr.getTmpFileName( "htmlhelp", ".hhp", folderName.c_str() );
	std::string hhcFile = msppMgr.getTmpFileName( "toc", ".hhc", folderName.c_str() );
	std::string hhkFile = msppMgr.getTmpFileName( "index", ".hhk", folderName.c_str() );

	std::string hhpPath = getpath( helpFolder.c_str(), hhpFile.c_str() );
	std::string hhcPath = getpath( helpFolder.c_str(), hhcFile.c_str() );
	std::string hhkPath = getpath( helpFolder.c_str(), hhkFile.c_str() );

	m_strHhpFilePath = hhpPath; /* HHP temporary file to hold the path. */
	m_strHhcFilePath = hhcPath; /* HHC temporary file to hold the path. */
	m_strHhkFilePath = hhkPath; /* HHK temporary file to hold the path. */

	/* File open */
	ofHppFile.open( hhpPath, std::ios::out );
	/* Whether the file is successfully opened. */
	if( !ofHppFile ){
		/* Error */
		LOG_ERROR( FMT( "Temporary Hhp File Open ERROR" ) );
		return false;
	}

	/* And outputs a fixed portion of the HHP file described. */
	strWrite = _T("[OPTIONS]\n");
	ofHppFile.write( strWrite.c_str(), strWrite.size() );

	strWrite = _T("Contents file=");
	strWrite += m_strHhcFilePath;
	strWrite += "\n";
	ofHppFile.write( strWrite.c_str(), strWrite.size() );

	strWrite = _T("Default topic=");
	strWrite += m_strDefaultFilePath;
	strWrite += "\n";
	ofHppFile.write( strWrite.c_str(), strWrite.size() );

	strWrite = _T("Index file=");
	strWrite += m_strHhkFilePath;
	strWrite += "\n";
	ofHppFile.write( strWrite.c_str(), strWrite.size() );

	strWrite = _T("\n");
	ofHppFile.write( strWrite.c_str(), strWrite.size() );

	strWrite = _T("[FILES]\n");
	ofHppFile.write( strWrite.c_str(), strWrite.size() );

	/* To output the full path. */
	for( unsigned int idx = 0; idx<m_vctHtmlPath.size(); idx++ ){
		strWrite = m_vctHtmlPath[idx];
		strWrite += "\n";
		ofHppFile.write( strWrite.c_str(), strWrite.size() );
	}

	/* File close */
	ofHppFile.close();

	return true;
}

// Recursively writes the contents of the UL tag.
void HelpManager::createULTag( int nId, std::ofstream& ofHppFile){
	std::string   strWrite = "";

	strWrite = "<LI><OBJECT type=\"text/sitemap\">";
	strWrite += "\n";
	ofHppFile.write( strWrite.c_str(), strWrite.size() );
	strWrite = "<param name=\"Name\" value=\"";
	strWrite += m_HelpChapterInfo[nId].strName;
	strWrite += "\">\n";
	ofHppFile.write( strWrite.c_str(), strWrite.size() );
	strWrite = "<param name=\"Local\" value=\"";
	strWrite += m_HelpChapterInfo[nId].strLocal;
	strWrite += "\"></OBJECT></LI>\n";
	ofHppFile.write( strWrite.c_str(), strWrite.size() );
	strWrite = "";

	if( m_HelpChapterInfo[nId].nChildId.size() <= 0 ){
		return;
	}

	strWrite = "<UL>";
	strWrite += "\n";
	ofHppFile.write( strWrite.c_str(), strWrite.size() );

	/* Toprint the chapters. */
	std::string strBuf;
	strBuf = "";
	for( unsigned int i=0;i<m_HelpChapterInfo[nId].nChildId.size();i++ ){	
		createULTag(m_HelpChapterInfo[nId].nChildId[i],ofHppFile);
	}

	strWrite = "</UL>\n";
	ofHppFile.write( strWrite.c_str(), strWrite.size() );
}

// It is deleted '...' from the end of the character string.
std::string HelpManager::getDotCutName( const char* szMenu )
{
	std::string	strRet = "";

	if( szMenu != NULL ){
		const char*		szDot = "...";
		unsigned int	nDotSize = strlen( szDot );

		strRet = szMenu;
		if( strRet.length() > nDotSize ){
			unsigned int	nPos = strRet.length() - nDotSize;

			if( strRet.rfind( szDot, nPos ) == nPos ){
				strRet.erase( nPos, nDotSize );
			}
		}
	}

	return strRet;
}

// Sorts less than the second hierarchy.
void HelpManager::sortHelpChapterByMenu( int nChildId, kome::plugin::PluginMenu* pplgMenu )
{
	// check param.
	if( nChildId < 0 ){
		LOG_ERROR( FMT( "ID of HHC file object structures ERROR.[%d]", nChildId ) );
		return;
	}
	else if( pplgMenu == NULL ){
		LOG_ERROR( FMT( "PluginMenu object pointer is NULL." ) );
		return;
	}

	unsigned int	i, j;
	unsigned int	nChildCount = pplgMenu->getNumberOfChildren();
	std::string		strMenu;
	kome::plugin::PluginMenu*	pplgChild;

	// Sort of the child node.
	for( i = 0; i < nChildCount; ++i ){
		pplgChild = pplgMenu->getChildMenu( nChildCount - 1 - i );

		if( pplgChild != NULL ){
			strMenu = getDotCutName( pplgChild->getName() );

			// Search of the item in unison with menu item.
			for( j = 0; j < m_HelpChapterInfo[ nChildId ].nChildId.size(); ++j ){
				int		nIndex = m_HelpChapterInfo[ nChildId ].nChildId[ j ];

				// When there is an according item
				if( compareignorecase( strMenu.c_str(),
									   m_HelpChapterInfo[ nIndex ].strName.c_str() ) == 0 ){
					// Sorts less than the item.
					sortHelpChapterByMenu( nIndex, pplgChild );

					// It travels the item at the first.
					m_HelpChapterInfo[ nChildId ].nChildId.erase( m_HelpChapterInfo[ nChildId ].nChildId.begin() + j );
					m_HelpChapterInfo[ nChildId ].nChildId.insert( m_HelpChapterInfo[ nChildId ].nChildId.begin(), nIndex );
					break;
				}
			}
		}
	}
}

// HHC file to create a temporary.
bool HelpManager::createTemporaryHhcFile( void )
{	
	std::ofstream ofHppFile;
	std::string   strWrite = "";
	ofHppFile.open(m_strHhcFilePath.c_str(), std::ios::out );
	if( !ofHppFile ){
		LOG_ERROR( FMT("Temporary HhcFile Open ERROR") );
		return false;
	}

	/* And outputs a fixed portion of the HHC file described. */
	strWrite =  "<HTML>";
	strWrite += "\n";
	ofHppFile.write( strWrite.c_str(), strWrite.size() );
	strWrite = "<HEAD><meta http-equiv=\"Content-Type\" content=\"text/html; charset=windows-1252\"></HEAD>";
	strWrite += "\n";
	ofHppFile.write( strWrite.c_str(), strWrite.size() );
	strWrite = "<BODY>";
	strWrite += "\n";
	ofHppFile.write( strWrite.c_str(), strWrite.size() );
	strWrite = "<OBJECT type=\"text/site properties\">";
	strWrite += "\n";
	ofHppFile.write( strWrite.c_str(), strWrite.size() );
	strWrite = "<param name=\"ImageType\" value=\"Folder\">";
	strWrite += "\n";
	ofHppFile.write( strWrite.c_str(), strWrite.size() );
	strWrite = "</OBJECT>";
	strWrite += "\n";
	ofHppFile.write( strWrite.c_str(), strWrite.size() );

	strWrite = "<UL>";
	strWrite += "\n";
	ofHppFile.write( strWrite.c_str(), strWrite.size() );

	/* Recursively create chapters. */
	for( unsigned int i=0;i<m_HelpChapterInfo[0].nChildId.size();i++ ){	
		createULTag(m_HelpChapterInfo[0].nChildId[i],ofHppFile);
	}
	strWrite = "</UL>\n";
	ofHppFile.write( strWrite.c_str(), strWrite.size() );

	strWrite = 	"</BODY></HTML>\n";
	ofHppFile.write( strWrite.c_str(), strWrite.size() );

	/* File close */
	ofHppFile.close();

	return true;
}

// HHK file to create a temporary.
bool HelpManager::createTemporaryHhkFile( void )
{
	std::ofstream ofHppFile;
	std::string   strWrite;

	/* File open */
	ofHppFile.open(m_strHhkFilePath.c_str(), std::ios::out );
	/* Whether the file is successfully opened. */
	if( !ofHppFile ){
		/* ERROR */
		LOG_ERROR( FMT( "Hhk File Open ERROR" ) );
		LOG_ERROR( FMT( "File Name = %s", m_strHhkFilePath.c_str() ) );
		return false;
	}

	/* And outputs a fixed portion of the HHC file described. */
	strWrite =  "<!DOCTYPE html PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\" \"http://www.w3.org/TR/html4/loose.dtd\">";
	strWrite += "\n";
	ofHppFile.write( strWrite.c_str(), strWrite.size() );
	strWrite =  "<HTML>";
	strWrite += "\n";
	ofHppFile.write( strWrite.c_str(), strWrite.size() );
	strWrite = "<HEAD>\n<meta name=\"GENERATOR\" content=\"Microsoft&reg; HTML Help Workshop 4.1\">";
	strWrite += "\n";
	ofHppFile.write( strWrite.c_str(), strWrite.size() );
	strWrite = "</HEAD><BODY>";
	strWrite += "\n";
	ofHppFile.write( strWrite.c_str(), strWrite.size() );
	strWrite = "<OBJECT type=\"text/site properties\">";
	strWrite += "\n";
	ofHppFile.write( strWrite.c_str(), strWrite.size() );
	strWrite = "</OBJECT>";
	strWrite += "\n";
	ofHppFile.write( strWrite.c_str(), strWrite.size() );
	strWrite = "<UL>";
	strWrite += "\n";
	ofHppFile.write( strWrite.c_str(), strWrite.size() );

	/* Absolute path to enumeration. */
	strWrite = "";
	for( unsigned int idx = 0; idx<m_HelpKeywordInfo.size(); idx++ ){
		strWrite = "<LI><OBJECT type=\"text/sitemap\">";
		strWrite += "\n";
		ofHppFile.write( strWrite.c_str(), strWrite.size() );
		strWrite = "<param name=\"Name\" value=\"";
		strWrite += m_HelpKeywordInfo[idx].strIdxName;
		strWrite += "\">\n";
		ofHppFile.write( strWrite.c_str(), strWrite.size() );
		strWrite = "<param name=\"Name\" value=\"";
		strWrite += m_HelpKeywordInfo[idx].strKeyName;
		strWrite += "\">\n";
		ofHppFile.write( strWrite.c_str(), strWrite.size() );
		strWrite = "<param name=\"Local\" value=\"";
		strWrite += m_HelpKeywordInfo[idx].strLocal;
		strWrite += "\">\n";
		ofHppFile.write( strWrite.c_str(), strWrite.size() );
		strWrite = "</OBJECT></LI>";
		ofHppFile.write( strWrite.c_str(), strWrite.size() );
	}
	/* The end tag */
	strWrite = "</UL></BODY></HTML>";
	strWrite += "\n";
	ofHppFile.write( strWrite.c_str(), strWrite.size() );

	/* File close */
	ofHppFile.close();
	
	return true;
}

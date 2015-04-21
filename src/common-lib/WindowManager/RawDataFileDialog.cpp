/**
 * @file RawDataFileDialog.cpp
 * @brief implements of RawDataFileDialog class
 *
 * @author S.Tanaka
 * @date 2009.07.03
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "RawDataFileDialog.h"

#include "ListTool.h"
#include "WindowTool.h"

#include <wx/splitter.h>
#include <boost/bind.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>

// for registry
#include <wx/msw/registry.h>	// @date 2011/09/07 <Add> OKADA
#define SUB_KEY		"HKEY_LOCAL_MACHINE\\SOFTWARE\\Mass++2"	// @date 2011/09/07 <Add> OKADA
#define KEY_VALUE	"InstallDir"		// @date 2011/09/07 <Add> OKADA


using namespace kome::window;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



#define SECTION					 "File IO"
#define DEFAULT_DIR_KEY			 "DEFAULT_DIR"


#define GRID_GAP					 3
#define BORDER_SIZE				  3

#define ID_DIR_CTRL				  17
#define ID_FILE_CTRL				 18
#define ID_TEXT_CTRL				 19
#define ID_FILTER_CTRL			   20


BEGIN_EVENT_TABLE( RawDataFileDialog, StandardDialog )
	EVT_TREE_SEL_CHANGED( wxID_TREECTRL, RawDataFileDialog::onChangeDir )
	EVT_LIST_ITEM_SELECTED( ID_FILE_CTRL, RawDataFileDialog::onSelectIcon )
	EVT_LIST_ITEM_DESELECTED( ID_FILE_CTRL, RawDataFileDialog::onSelectIcon )
	EVT_LIST_ITEM_ACTIVATED( ID_FILE_CTRL, RawDataFileDialog::onDoubleClickIcon )
	EVT_COMBOBOX( ID_FILTER_CTRL, RawDataFileDialog::onSelectFilter )
	EVT_BUTTON( wxID_OK, RawDataFileDialog::onOK )
END_EVENT_TABLE()


// mark combo
RawDataFileDialog::RawDataFileDialog( wxWindow* parent, const bool singleSelect )
		: StandardDialog( parent, "Select Files" ) {
	// initialize
	m_dirCtrl = NULL;
	m_fileCtrl = NULL;
	m_text = NULL;
	m_filterCombo = NULL;
	m_singleSelect = singleSelect;

	// file types
	getFileTypes();

	// directory
	std::string dir = getDefaultDir();
	if( !isdirectory( dir.c_str() ) ) {
		dir = "";
	}

	m_dir = dir;

	m_errorFlg = false;
}

// destructor
RawDataFileDialog::~RawDataFileDialog() {
}

// get the number of paths
unsigned int RawDataFileDialog::getNumberOfPaths() {
	return m_paths.size();
}

// get path
const char* RawDataFileDialog::getPath( const unsigned int idx ) {
	if( idx >= m_paths.size() ) {
		return NULL;
	}
	return m_paths[ idx ].c_str();
}

// get file types
void RawDataFileDialog::getFileTypes() {
	// file factory
	kome::plugin::FileFactory& factory = kome::plugin::FileFactory::getInstance();

	// initialize
	m_types.clear();

	// all files
	m_types.resize( 2 );
	m_types[ 0 ].name = "All Files";

	m_types[ 1 ].name = "All MS Files";
	std::set< std::string > extSet;

	// file types
	unsigned int num = factory.getNumberOfFileFormats();
	for( unsigned int i = 0; i < num; i++ ) {
		m_types.resize( m_types.size() + 1 );
		m_types.back().name = factory.getFormatName( i );

		factory.getExtensions(i,m_types.back().extensions);

		for( unsigned int j = 0; j < m_types.back().extensions.size(); j++ ) {
			std::string ext = tolowercase( m_types.back().extensions[ j ].c_str() );
			if( extSet.find( ext ) == extSet.end() ) {
				m_types[ 1 ].extensions.push_back( ext );
				extSet.insert( ext );
			}
		}
	}

	// extensions
	for( unsigned int i = 0; i < m_types.size(); i++ ) {
		std::string exts;
		FileType* type = &( m_types[ i ] );
		for( unsigned int j = 0; j < type->extensions.size(); j++ ) {
			std::string ext = ( type->extensions )[ j ];

			if( !exts.empty() ) {
				exts.append( ";" );
			}
			exts.append( FMT( "*.%s", ext.c_str() ) );
			type->extSet.insert( ext );
		}

		if( exts.empty() ) {
			type->name = FMT( "%s (*.*)", type->name.c_str() );
		}
		else {
			type->name = FMT( "%s (%s)", type->name.c_str(), exts.c_str() );
		}
	}
}

// on select directory
void RawDataFileDialog::onSelectDir( const char* dir ) {
	// set directory
	m_dir = absolutepath( dir );

	// clear list
	m_fileCtrl->ClearAll();
	m_fileCtrl->InsertItem( 0, wxT( ".." ), wxFileIconsTable::folder );

	// empty input
	m_text->SetValue( wxEmptyString );

	// check the path
	if( !isdirectory( m_dir.c_str() ) ) {
		return;
	}

	// file type
	int sel = m_filterCombo->GetSelection();
	FileType* type = (FileType*)m_filterCombo->GetClientData( sel );

	wxBeginBusyCursor();	// @date 2012/05/08 <Add> OKADA	//　SPEC 83777

	// get files
	try {
		boost::filesystem::path p( m_dir.c_str(), boost::filesystem::native );
		boost::filesystem::directory_iterator end;
		for( boost::filesystem::directory_iterator it( p ); it != end; it++ ) {
			// file
			std::string name = (*it).path().leaf().string();
			std::string ext = tolowercase( getext( name.c_str() ).c_str() );
			std::string path = getpath( m_dir.c_str(), name.c_str() );
			const bool isDir = isdirectory( path.c_str() );

			if( isDir || type->extSet.size() == 0
					|| type->extSet.find( ext ) != type->extSet.end() ) {	// check extensions
				// icon
				int icon = wxFileIconsTable::file;
				if( isDir ) {
					icon = wxFileIconsTable::folder;
				}
				else {
					icon = wxTheFileIconsTable->GetIconID( wxT( ext.c_str() ) );
				}

				// add
				m_fileCtrl->InsertItem( m_fileCtrl->GetItemCount(), wxT( name.c_str() ), icon );
			}
		}
	}
	catch( ... ) {
		if( m_errorFlg ) {
			std::string message = FMT( "Cannot access this folder. Check folder permissions or specify a different folder. [%s]", dir );
			kome::window::WindowTool::showError( message.c_str() );
		}
	}

	wxEndBusyCursor();	// @date 2012/05/08 <Add> OKADA	//　SPEC 83777

}

// applies paths
void RawDataFileDialog::applyPaths( const bool dirChange ) {
	// the array of paths
	std::vector< std::string > paths;

	// manager
	kome::core::MsppManager& msppMgr = kome::core::MsppManager::getInstance();

	// create buffer
	wxString val = m_text->GetValue();
	char* buff = new char[ val.length() + 1 ];
	sprintf( buff, "%s", val.c_str() );
	buff[ val.length() ] = '\0';

	// get paths
	char* fileName = strstr( buff, "\"" );
	if( fileName == NULL ) {
		std::string path = trimstring( buff );
		if( !path.empty() ) {
			path = getabsolutepath( m_dir.c_str(), path.c_str() );
			paths.push_back( path );
		}
	}
	else {
		while( fileName != NULL ) {
			fileName += 1;
			char* end = strstr( fileName, "\"" );

			std::string path = fileName;
			if( end != NULL ) {
				path = std::string( fileName, end );
			}
			path = getabsolutepath( m_dir.c_str(), trimstring( path.c_str() ).c_str() );
			paths.push_back( path );

			fileName = ( end == NULL ? NULL : strstr( end + 1, "\"" ) );
		}
	}

	delete[] buff;

	// action
	if( paths.size() == 0 ) {
		return;
	}
	if( paths.size() == 1 && dirChange ) {
		std::string path = paths[ 0 ];
		if( isdirectory( path.c_str() ) ) {	// change directory
			m_errorFlg = false;
			m_dirCtrl->ExpandPath( wxT( path.c_str() ) );
			m_dirCtrl->SetPath( wxT( path.c_str() ) );
			m_errorFlg = true;
			onSelectDir( path.c_str() );
			return;
		}
	}

	// check files
	for( unsigned int i = 0; i < paths.size(); i++ ) {
		std::string path = paths[ i ];
		path = getabsolutepath( m_dir.c_str(), path.c_str() );

		if( !fileexists( path.c_str() ) ) {	// file does not exist
			m_paths.clear();

			std::string msg = FMT(
				"File not found. [%s]\nCheck the file name and try again.",
				path.c_str()
			);
			WindowTool::showError( msg.c_str() );

			return;
		}

		m_paths.push_back( path );
	}

	// save default dialog
	kome::core::IniFile* ini = msppMgr.getIniFile();
	if( ini != NULL ) {
		ini->setString( SECTION, DEFAULT_DIR_KEY, m_dir.c_str() );
	}

	// end dialog
	EndDialog( wxID_OK );
}

// create file sizer
wxSizer* RawDataFileDialog::createFileSizer() {
	// sizer
	wxBoxSizer* sizer = new wxBoxSizer( wxVERTICAL );

	// splitter
	wxSplitterWindow* splitter = new wxSplitterWindow( this, wxID_ANY, wxDefaultPosition, wxSize( 400, 400 ), wxSP_3D );
	sizer->Add( splitter, 1, wxALL | wxGROW, BORDER_SIZE );

	// directory control
	m_dirCtrl = new wxGenericDirCtrl(
		splitter,
		ID_DIR_CTRL,
		wxEmptyString,
		wxDefaultPosition,
		wxSize( -1, 300 ),
		wxDIRCTRL_3D_INTERNAL | wxDIRCTRL_DIR_ONLY,
		wxEmptyString,
		0
	);

	// file control
	int style = wxLC_LIST | wxLC_NO_HEADER | wxLC_HRULES;
	if( m_singleSelect ) {
		style |= wxLC_SINGLE_SEL;
	}

	m_fileCtrl = new wxListCtrl(
		splitter,
		ID_FILE_CTRL,
		wxDefaultPosition,
		wxSize( -1, 300 ),
	    style	
	);
	m_fileCtrl->SetImageList(
		wxTheFileIconsTable->GetSmallImageList(),
		wxIMAGE_LIST_SMALL
	);

	// initialize splitter
	splitter->Initialize( m_dirCtrl );
	splitter->SetSashGravity( 0.3 );
	splitter->SplitVertically( m_dirCtrl, m_fileCtrl );

	return sizer;
}

// create input sizer
wxSizer* RawDataFileDialog::createInputSizer() {
	// sizer
	wxFlexGridSizer* sizer = new wxFlexGridSizer( 1, 2, GRID_GAP, GRID_GAP );
	sizer->AddGrowableRow( 0 );
	sizer->AddGrowableCol( 0 );

	// text
	m_text = new wxTextCtrl(
		this,
		ID_TEXT_CTRL,
		wxEmptyString,
		wxDefaultPosition,
		wxSize( 200, -1 ),
		0
	);
	sizer->Add( m_text, 1, wxGROW | wxALL | wxALIGN_CENTER_VERTICAL, BORDER_SIZE );

	// filter combo
	m_filterCombo = new wxComboBox(
		this,
		ID_FILTER_CTRL,
		wxEmptyString,
		wxDefaultPosition,
		wxSize( 300, -1 ),
		0,
		NULL,
		wxCB_SIMPLE | wxCB_READONLY
	);

	for( unsigned int i = 0; i < m_types.size(); i++ ) {
		FileType* type = &( m_types[ i ] );
		m_filterCombo->Append( wxT( type->name.c_str() ), type );
	}
	m_filterCombo->Select( 1 );

	sizer->Add( m_filterCombo, 0, wxGROW | wxALL | wxALIGN_CENTER_VERTICAL, BORDER_SIZE );

	return sizer;
}

// create main sizer
wxSizer* RawDataFileDialog::createMainSizer() {
	// sizer
	wxFlexGridSizer* sizer = new wxFlexGridSizer( 2, 1, GRID_GAP, GRID_GAP );
	sizer->AddGrowableRow( 0 );
	sizer->AddGrowableCol( 0 );

	// file sizer
	wxSizer* fileSizer = createFileSizer();
	sizer->Add( fileSizer, 1, wxALL | wxGROW, BORDER_SIZE );

	// input sizer
	wxSizer* inputSizer = createInputSizer();
	sizer->Add( inputSizer, 1, wxALL | wxGROW, BORDER_SIZE );

	// select directory
	if( !m_dir.empty() ) {
		wxBeginBusyCursor();	// @date 2011/12/20 <Add> OKADA

		m_errorFlg = false;
		m_dirCtrl->ExpandPath( wxT( m_dir.c_str() ) );
		m_dirCtrl->SetPath( wxT( m_dir.c_str() ) );
		m_errorFlg = true;
		onSelectDir( m_dir.c_str() );

		wxEndBusyCursor();		// @date 2011/12/20 <Add> OKADA
	}

	return sizer;
}

// create button sizer
wxSizer* RawDataFileDialog::createButtonSizer() {
	// sizer
	wxSizer* sizer = new wxBoxSizer( wxVERTICAL );
	wxSizer* innerSizer = new wxBoxSizer( wxHORIZONTAL );

	// buttons
	wxButton* button = new wxButton( this, wxID_OK, wxT( "Open" ) );
	innerSizer->Add( button, 0, wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT, BORDER_SIZE );

	button = new wxButton( this, wxID_CANCEL, wxT( "Cancel" ) );
	innerSizer->Add( button, 0, wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT, BORDER_SIZE );

	sizer->Add( innerSizer, 0, wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT, BORDER_SIZE );

	return sizer;
}

// on change directory
void RawDataFileDialog::onChangeDir( wxTreeEvent& evt ) {
	// check the member
	if( m_dirCtrl == NULL ) {
		return;
	}

	// set dir
	wxString path = m_dirCtrl->GetPath();
	onSelectDir( path.c_str() );
}

// on select icon
void RawDataFileDialog::onSelectIcon( wxListEvent& evt ) {
	// selected items
	std::vector< int > items;
	ListTool::getSelectedItems( m_fileCtrl, items );

	if( items.size() == 0 ) {
		m_text->SetValue( wxEmptyString );
	}
	else if( items.size() == 1 ) {
		m_text->SetValue( m_fileCtrl->GetItemText( items[ 0 ] ) );
	}
	else {
		std::string value;
		for( unsigned int i = 0; i < items.size(); i++ ) {
			if( !value.empty() ) {
				value.append( " " );
			}
			value.append( FMT( "\"%s\"", m_fileCtrl->GetItemText( items[ i ] ).c_str() ) );
		}
		m_text->SetValue( wxT( value.c_str() ) );
	}
}

// on file icon
void RawDataFileDialog::onDoubleClickIcon( wxListEvent& evt ) {
	applyPaths( true ); 
}

// on select filter
void RawDataFileDialog::onSelectFilter( wxCommandEvent& evt ) {
	onSelectDir( m_dir.c_str() );
}

// on OK
void RawDataFileDialog::onOK( wxCommandEvent& evt ) {
	applyPaths( false );
}

// get default directory
std::string RawDataFileDialog::getDefaultDir() {
	// manager
	kome::core::MsppManager& msppMgr = kome::core::MsppManager::getInstance();

	// get from ini file
	std::string defaultDir;
	kome::core::IniFile* ini = msppMgr.getIniFile();
	if( ini != NULL ) {
		defaultDir = ini->getString( SECTION, DEFAULT_DIR_KEY, "" );
	}

	// home directory
	if( defaultDir.empty() ) {
		defaultDir = gethomedir();
		
// @date 2011/09/07 <Add> OKADA ------->
		wxString strTemp;
		wxString strKeyValue = KEY_VALUE;
		const wxString wxstrSubKey = SUB_KEY;
		const wxChar* wxc_szSubKey = KEY_VALUE;

		// レジストリからインストール日付("YYYYMMDD"形式)を読み込む
		wxRegKey wxrRegKey( wxstrSubKey );

		bool bRet=false;

		if( wxrRegKey.Exists() ){
			
			// キーが存在します
			bRet = wxrRegKey.QueryValue( wxc_szSubKey, strTemp );

			if( bRet ){

				// レジストリに保存されたインストールディレクトリを取得する
				wxChar szInstallDir[MAX_PATH] = { '\0' };
				strncpy( szInstallDir, strTemp.GetData(), MAX_PATH-1 );

				// インストールディレクトリの直下のdataディレクトリのpathを得る
				std::string strDir = getpath( szInstallDir, "data" );

				if( strDir.empty() ){
					;
				}else{
					defaultDir = strDir;
				}
			}

			bRet = wxrRegKey.Close();
		}

// @date 2011/09/07 <Add> OKADA <-------

	}

	return defaultDir;
}

// get wild card
std::string RawDataFileDialog::getWildCard() {
	// get file factory object
	kome::plugin::FileFactory& factory = kome::plugin::FileFactory::getInstance();

	// initialize
	std::string s = "All Files (*.*)|*.*";

	// get file formats
	std::string specExtensions;
	std::string formats;
	std::set<std::string> extSet;

	for( unsigned int i = 0; i < factory.getNumberOfFileFormats(); i++ ) {
		// format name
		std::string name = factory.getFormatName( i );

		// extensions
		std::list< std::string > extList;
		std::vector<std::string> extStrs;
		factory.getExtensions(i,extStrs);

		std::string extensions;
		for (unsigned int i = 0; i < extStrs.size(); i++) {
			std::string ext = extStrs[i];
			if( extSet.find( ext ) == extSet.end() ) {
				extSet.insert( ext );
				if( specExtensions.length() > 0 ) {
					specExtensions.append( ";" );
				}
				specExtensions.append( FMT( "*.%s", ext.c_str() ) );
			}

			if( extensions.length() > 0 ) {
				extensions.append( ";" );
			}
			extensions.append( FMT( "*.%s", ext.c_str() ) );
		}

		// add
		if( !extensions.empty() ) {
			formats.append(
				FMT(
					"|%s (%s)|%s",
					name.c_str(),
					extensions.c_str(),
					extensions.c_str()
				)
			);
		}
	}

	if( !specExtensions.empty() ) {
		s.append(
			FMT(
				"|%s (%s)|%s",
				"All MS Files",
				specExtensions.c_str(),
				specExtensions.c_str()
			)
		);
	}
	if( !formats.empty() ) {
		s.append( formats );
	}

	return s;
}

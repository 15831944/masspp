/**
 * @file OutputFormatsDlg.cpp
 * @brief implementsation of OutputFormatsDlg class
 *
 * @author OKADA, H
 * @date 2012-03-06
 * 
 * Copyright(C) 2012 Shimadzu Corporation. All rights reserved.
 */


#include "stdafx.h"
#include "OutputFormatsDlg.h"
#include "BatchButtonDef.h"
#include "BatchServicePluginManager.h"

using namespace kome::batchservice;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


#define GRID_GAP		3
#define BORDER_SIZE		3


BEGIN_EVENT_TABLE( OutputFormatsDlg,			kome::window::StandardDialog )
	
	EVT_BUTTON( wxOK, OutputFormatsDlg::onOkButton )

END_EVENT_TABLE()

// チェックボックスの文字列
#define STR_CHKBX_MIS_TXT	"Mascot Generic Format (MIS)"
#define STR_CHKBX_MSB		"Mass Spectrum Bimary File"
#define STR_CHKBX_MZML		"mzML file"
#define STR_CHKBX_MZXML		"mzXML File"
#define STR_CHKBX_PNG		"PNG File (Spectrum)"
#define STR_CHKBX_TSV_TXT	"Tab Separated Values File"

// チェックボックス右側の拡張子
#define STR_EXT_CHKBX_MIS_TXT	"*.txt"
#define STR_EXT_CHKBX_MSB		"*.msb"
#define STR_EXT_CHKBX_MZML		"*.mzML"
#define STR_EXT_CHKBX_MZXML		"*.mzXML"
#define STR_EXT_CHKBX_PNG		"*.png"
#define STR_EXT_CHKBX_TSV_TXT	"*.txt"


// constructor
OutputFormatsDlg::OutputFormatsDlg( wxWindow* parent )
	: kome::window::StandardDialog(	parent,	"Output Formats")
{
	m_vectCheckBox.clear();
	m_vectOutputFormatInf.clear();
}

// destructor
OutputFormatsDlg::~OutputFormatsDlg() {
}


// create main sizer
wxSizer* OutputFormatsDlg::createMainSizer() {

	BatchServicePluginManager &mgr = BatchServicePluginManager::getInstance();
	unsigned int uiNum = mgr.getIOFormats( &m_vectOutputFormatInf );

	wxFlexGridSizer* fieldSizer = new wxFlexGridSizer( uiNum, 2, GRID_GAP, GRID_GAP );
	fieldSizer->AddGrowableCol( 0 );
	fieldSizer->AddGrowableCol( 1 );
	for( unsigned int i=0; i<uiNum; i++ ){
		fieldSizer->AddGrowableRow( i );
	}

	wxCheckBox* pCheckBoxTemp;
	m_vectCheckBox.clear();

	for( unsigned int i=0; i<uiNum; i++ ){
		pCheckBoxTemp = new wxCheckBox(this, ID_CHKBX_TOP+i, m_vectOutputFormatInf[i].strLongName );
		m_vectCheckBox.push_back( pCheckBoxTemp );
	}

	wxStaticText* pStaticTextTemp;
	std::vector<wxStaticText*> vpStaticTextTemp;

	for( unsigned int i=0; i<uiNum; i++ ){
		pStaticTextTemp = new wxStaticText( this, ID_TEXT_FMT_TOP+i,	m_vectOutputFormatInf[i].strExt.c_str() );
		vpStaticTextTemp.push_back( pStaticTextTemp );
	}

	for( unsigned int i=0; i<uiNum; i++ ){
		fieldSizer->Add(m_vectCheckBox[i], 0, wxALIGN_LEFT | wxALIGN_CENTRE_VERTICAL | wxALL, 5);	
		fieldSizer->Add(vpStaticTextTemp[i], 0, wxALIGN_LEFT | wxALIGN_CENTRE_VERTICAL | wxALL, 5);
	}
	// INIファイルから読み込む

	for( unsigned int i=0; i<uiNum; i++ ){
		m_vectCheckBox[i]->SetValue( m_vectOutputFormatInf[i].flgCheck );
	}

	return fieldSizer;
}


// create Button sizer
wxSizer* OutputFormatsDlg::createButtonSizer() {
	return CreateButtonSizer( wxOK | wxCANCEL );
}


void OutputFormatsDlg::getOutputFormat( std::vector<output_format_inf> & vect_stTempOutFrm){

	int nSize = m_vectOutputFormatInf.size();

	for( int i=0; i<nSize; i++ ){
		bool bVal = m_vectCheckBox[i]->GetValue();
		m_vectOutputFormatInf[i].flgCheck = bVal;
	}

	vect_stTempOutFrm = m_vectOutputFormatInf;

	return;
}


void OutputFormatsDlg::onOkButton( wxCommandEvent& evt ) {
	StandardDialog::onOkButton( evt );
}

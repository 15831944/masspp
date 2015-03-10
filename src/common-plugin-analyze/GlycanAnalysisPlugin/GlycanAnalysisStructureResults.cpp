/**
 * @file GlycanAnalysisStructureResults.cpp
 * @brief interfaces of Glycan Analysis Predicted Structures
 *
 * @author N.Tashiro
 * @date 2012.04.02
 * 
 * Copyright (C) 2014 Shimadzu Corporation All rights reserved.
 */

#include "stdafx.h"

#include "GlycanAnalysisStructureResults.h"
#include "GlycanAnalysisStructureCanvas.h"
#include "GlycanAnalysisManager.h"

#include <wx/clipbrd.h>

/** Menu ID (Copy)*/
#define BASE_MENU_ID_COPY			13579

/** Menu ID (Copy All)*/
#define BASE_MENU_ID_COPY_ALL		13580

/** Menu ID (Link to KEGG)*/
#define BASE_MENU_ID_LINK_TO_KEGG	13581

/** Layout */
// 2014/02/24 Change by nakaya Change of a selective state ->
//#define MARGIN 5	// margin of buttons and figures
#define MARGIN 0	// margin of buttons and figures
// 2014/02/24 Change by nakaya Change of a selective state <-

/** Menu Text (Copy)*/
#define GLYCAN_MENU_COPY "Copy"

/** Menu Text (Copy All)*/
#define GLYCAN_MENU_COPY_ALL "Copy All"

/** Menu Text (Link to KEGG)*/
#define GLYCAN_MENU_ACCESS_KEGG "Link to KEGG"


#define RESULT_FOLDER	"GLYCAN_ANALYSIS"


/** Sizer width*/
// 2014/02/20 Change by nakaya The addition of a display item  ->
//#define SIZER_WIDTH 400
#define SIZER_WIDTH 650
// 2014/02/20 Change by nakaya The addition of a display item  <-

/** Sizer height*/
#define SIZER_HEIGHT 200

using namespace kome::glycan;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


BEGIN_EVENT_TABLE(GlycanAnalysisStructureResults, wxScrolledWindow)
	EVT_RIGHT_UP(GlycanAnalysisStructureResults::onRightClick)
	EVT_RADIOBUTTON(wxID_ANY,GlycanAnalysisStructureResults::onRadioButton)
	EVT_MENU( BASE_MENU_ID_COPY , GlycanAnalysisStructureResults::copyFigure )
	EVT_MENU( BASE_MENU_ID_COPY_ALL , GlycanAnalysisStructureResults::copyAllFigures )
	EVT_MENU( BASE_MENU_ID_LINK_TO_KEGG , GlycanAnalysisStructureResults::accessKEGG )
	EVT_UPDATE_UI_RANGE( BASE_MENU_ID_COPY, BASE_MENU_ID_LINK_TO_KEGG, GlycanAnalysisStructureResults::onUpdateUI )
END_EVENT_TABLE()

// constructor
GlycanAnalysisStructureResults::GlycanAnalysisStructureResults( wxDialog* parent ):
//wxScrolledWindow(parent, wxID_ANY, wxDefaultPosition, wxSize(400,200), wxVSCROLL | wxALL | wxGROW | wxBORDER_STATIC){
// 2014/02/20 Change by nakaya The addition of a display item  ->
//wxScrolledWindow(parent, wxID_ANY, wxDefaultPosition, wxSize(400,200), wxVSCROLL | wxBORDER_STATIC){
wxScrolledWindow(parent, wxID_ANY, wxDefaultPosition, wxSize(SIZER_WIDTH,SIZER_HEIGHT), wxVSCROLL | wxBORDER_STATIC){
// 2014/02/20 Change by nakaya The addition of a display item  <-

	// menu
	m_menu = NULL;
	m_menu = new wxMenu();
	m_menu->Append( BASE_MENU_ID_COPY , wxT( GLYCAN_MENU_COPY ) );
	m_menu->Append( BASE_MENU_ID_COPY_ALL , wxT( GLYCAN_MENU_COPY_ALL ) );
	m_menu->Append( BASE_MENU_ID_LINK_TO_KEGG , wxT( GLYCAN_MENU_ACCESS_KEGG ) );
	
// 2014/02/20 Change by nakaya The addition of a display item  ->
	//// sizer
	//m_sizer = new wxBoxSizer( wxHORIZONTAL );
	//m_sizer->SetMinSize(400,200);
	//m_rbtnSizer = NULL;
	//m_canvasSizer = NULL;	

	//m_rbtnSizer = new wxBoxSizer( wxVERTICAL );
	//m_canvasSizer = new wxBoxSizer( wxVERTICAL );
	//// create dialog
	//m_sizer->Add( m_rbtnSizer, 0, wxLEFT | wxTOP | wxGROW, MARGIN );
	//m_sizer->Add( m_canvasSizer, 0, wxLEFT | wxTOP | wxRIGHT | wxGROW, MARGIN );
	//SetSizer(m_sizer);
	//SetScrollbars(0, 20, 0, 50,0, 0, false);
	//SetScrollRate(0, 50);
	// sizer
	m_sizer = new wxBoxSizer( wxHORIZONTAL );
	m_sizer->SetMinSize(SIZER_WIDTH,SIZER_HEIGHT);
	m_canvasSizer = NULL;
	m_rankSizer = NULL;
	m_assignedSizer = NULL;
	m_scoreSizer = NULL;

	m_canvasSizer = new wxBoxSizer( wxVERTICAL );
	m_rankSizer = new wxBoxSizer( wxVERTICAL );
	m_assignedSizer = new wxBoxSizer( wxVERTICAL );
	m_scoreSizer = new wxBoxSizer( wxVERTICAL );

	// create dialog
	m_sizer->Add( m_rankSizer, 0, wxLEFT | wxTOP | wxGROW, MARGIN );
	m_sizer->Add( m_canvasSizer, 0, wxLEFT | wxTOP | wxRIGHT | wxGROW, MARGIN );
	m_sizer->Add( m_assignedSizer, 0, wxLEFT | wxTOP | wxGROW, MARGIN );
	m_sizer->Add( m_scoreSizer, 0, wxLEFT | wxTOP | wxGROW, MARGIN );

	SetSizer(m_sizer);
	SetScrollbars(0, 20, 0, 50,0, 0, false);
	SetScrollRate(0, 50);	
// 2014/02/20 Change by nakaya The addition of a display item  <-
}

// destructor
GlycanAnalysisStructureResults::~GlycanAnalysisStructureResults(){
	clearResult();

	if(m_canvasSizer != NULL){
		m_sizer->Detach(m_canvasSizer);
		delete m_canvasSizer;
	}
// 2014/02/24 Delete by nakaya Change of a selective state ->
	//if(m_rbtnSizer != NULL){
	//	m_sizer->Detach(m_rbtnSizer);
	//	delete m_rbtnSizer;
	//}
// 2014/02/24 Delete by nakaya Change of a selective state <-

// 2014/02/20 Add by nakaya The addition of a display item  ->
	if(m_rankSizer != NULL){
		m_sizer->Detach(m_rankSizer);
		delete m_rankSizer;
	}
	if(m_assignedSizer != NULL){
		m_sizer->Detach(m_assignedSizer);
		delete m_assignedSizer;
	}
	if(m_scoreSizer != NULL){
		m_sizer->Detach(m_scoreSizer);
		delete m_scoreSizer;
	}
// 2014/02/20 Add by nakaya The addition of a display item  <-
	if(m_menu != NULL){
		delete m_menu;
	}
	SetSizer(NULL);
	delete m_sizer;
}

// add a glycan to results array
bool GlycanAnalysisStructureResults::addResult(GLYCAN_STRUCTURE* pGlycanStructure, Annotation* pAnnotation, const char* strLabel, const char* strAnion){

	// parameter check
	if(pGlycanStructure == NULL){
		return false;
	}
	this->SetMinSize(this->GetSize());

	wxSizerFlags flags = wxSizerFlags(0).Align(wxCENTER).Border(wxALL, MARGIN);
	
	// create canvas & add to sizer
	GlycanAnalysisStructureCanvas* canvas = new GlycanAnalysisStructureCanvas(
		this,pGlycanStructure,1,strLabel,true,strAnion,pAnnotation);
	m_canvasList.push_back(canvas);
	m_canvasSizer->Add( canvas  , flags);

// 2014/02/24 Delete by nakaya Change of a selective state ->
	//// create Radiobutton & add to sizer
	//wxRadioButton* radioBtn = new wxRadioButton(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(-1,GLYCAN_CANVAS_HEIGHT));
	//m_radioBtnList.push_back(radioBtn);
	//m_rbtnSizer->Add( radioBtn  , flags);
// 2014/02/24 Delete by nakaya Change of a selective state <-

// 2014/02/20 Add by nakaya The addition of a display item  ->
	TextCanvas* rankCanvas = new TextCanvas(this, pGlycanStructure, pGlycanStructure->strRank.c_str(), RANK_CANVAS_WIDTH, RANK_CANVAS_HEIGHT);
	m_rankCanvasList.push_back(rankCanvas);
	m_rankSizer->Add( rankCanvas  , flags);

	TextCanvas* assigendCanvas = new TextCanvas(this, pGlycanStructure, pGlycanStructure->strAssigned.c_str(), ASSIGNED_CANVAS_WIDTH, ASSIGNED_CANVAS_HEIGHT);
	m_assignedCanvasList.push_back(assigendCanvas);
	m_assignedSizer->Add( assigendCanvas  , flags);

	TextCanvas* scoreCanvas = new TextCanvas(this, pGlycanStructure, pGlycanStructure->strScore.c_str(), SCORE_CANVAS_WIDTH, SCORE_CANVAS_HEIGHT);
	m_scoreCanvasList.push_back(scoreCanvas);
	m_scoreSizer->Add( scoreCanvas  , flags);
// 2014/02/20 Add by nakaya The addition of a display item  <-

	return true;
}

// clear Predicted Structures
bool GlycanAnalysisStructureResults::clearResult(){

// 2014/02/20 Change by nakaya The addition of a display item  ->
	//if(m_rbtnSizer != NULL){
	//	m_rbtnSizer->Clear();
	//}
	//if(m_canvasSizer != NULL){
	//	m_canvasSizer->Clear();
	//}

	//m_sizer->Remove(m_rbtnSizer);
	//m_sizer->Remove(m_canvasSizer);
	//SetSizer(NULL);
	//m_sizer = new wxBoxSizer( wxHORIZONTAL );
	//m_sizer->SetMinSize(SIZER_WIDTH,SIZER_HEIGHT);

	//m_rbtnSizer = new wxBoxSizer( wxVERTICAL );
	//m_canvasSizer = new wxBoxSizer( wxVERTICAL );

	//for(unsigned int i = 0;i < m_canvasList.size();i++){
	//	delete m_canvasList.at(i);
	//}
	//m_canvasList.clear();

	//for(unsigned int i = 0;i < m_radioBtnList.size();i++){
	//	delete m_radioBtnList.at(i);
	//}
	//m_radioBtnList.clear();
	//m_sizer->Add( m_rbtnSizer, 0, wxLEFT | wxTOP | wxGROW, MARGIN );
	//m_sizer->Add( m_canvasSizer, 0, wxLEFT | wxTOP | wxRIGHT | wxGROW, MARGIN );
	
	if(m_canvasSizer != NULL){
		m_canvasSizer->Clear();
	}
	if(m_rankSizer != NULL){
		m_rankSizer->Clear();
	}
	if(m_assignedSizer != NULL){
		m_assignedSizer->Clear();
	}
	if(m_scoreSizer != NULL){
		m_scoreSizer->Clear();
	}

	m_sizer->Remove(m_canvasSizer);
	m_sizer->Remove(m_rankSizer);
	m_sizer->Remove(m_assignedSizer);
	m_sizer->Remove(m_scoreSizer);
	
	SetSizer(NULL);
	m_sizer = new wxBoxSizer( wxHORIZONTAL );
	m_sizer->SetMinSize(SIZER_WIDTH,SIZER_HEIGHT);

	m_canvasSizer = new wxBoxSizer( wxVERTICAL );
	m_rankSizer = new wxBoxSizer( wxVERTICAL );
	m_assignedSizer = new wxBoxSizer( wxVERTICAL );
	m_scoreSizer = new wxBoxSizer( wxVERTICAL );

	for(unsigned int i = 0;i < m_canvasList.size();i++){
		delete m_canvasList.at(i);
	}
	m_canvasList.clear();

	for(unsigned int i = 0;i < m_rankCanvasList.size();i++){
		delete m_rankCanvasList.at(i);
	}
	m_rankCanvasList.clear();

	for(unsigned int i = 0;i < m_assignedCanvasList.size();i++){
		delete m_assignedCanvasList.at(i);
	}
	m_assignedCanvasList.clear();

	for(unsigned int i = 0;i < m_scoreCanvasList.size();i++){
		delete m_scoreCanvasList.at(i);
	}
	m_scoreCanvasList.clear();

	m_sizer->Add( m_rankSizer, 0, wxLEFT | wxTOP | wxGROW, MARGIN );
	m_sizer->Add( m_canvasSizer, 0, wxLEFT | wxTOP | wxRIGHT | wxGROW, MARGIN );
	m_sizer->Add( m_assignedSizer, 0, wxLEFT | wxTOP | wxGROW, MARGIN );
	m_sizer->Add( m_scoreSizer, 0, wxLEFT | wxTOP | wxGROW, MARGIN );
// 2014/02/20 Change by nakaya The addition of a display item  <-

	return true;

}

// call openMenu() and skip event
void GlycanAnalysisStructureResults::onRightClick(wxMouseEvent& evt){
	evt.Skip();
	openMenu();
	return;
}

// when Radio button clicked, call this function
void GlycanAnalysisStructureResults::onRadioButton(wxCommandEvent& evt){
	GlycanAnalysisManager& gaManager = GlycanAnalysisManager::getInstance();	
	gaManager.refreshSpectrum();
}

// open the right click menu
void GlycanAnalysisStructureResults::openMenu(){

// 2014/02/24 Change by nakaya Change of a selective state ->
	//unsigned int unCount =  m_radioBtnList.size();
	GlycanAnalysisManager& gaManager = GlycanAnalysisManager::getInstance();
	std::vector<NGlycan>* pNGlycans = gaManager.getGlycans();
	if ( pNGlycans != NULL ) {
		if (0 < pNGlycans->size() && 0 < m_canvasList.size() ) {
			((wxDialog*)this)->PopupMenu( m_menu );
		}
	}
// 2014/02/24 Change by nakaya Change of a selective state <-

	return;
}

// on update UI
void GlycanAnalysisStructureResults::onUpdateUI( wxUpdateUIEvent& evt ) {
	
	evt.Enable( true );
}

// copy the selected figure
void GlycanAnalysisStructureResults::copyFigure(wxCommandEvent& evt){

	// Check to make sure.
	if(m_canvasList.size() > 0){
		unsigned int unSelectedID;
		bool bResult = getSelectedID(&unSelectedID);
		if(!bResult){
			LOG_ERROR(FMT("[GlycanAnalysisStructureResults] There is no selected radiobutton."));
			return;
		}
		wxTheClipboard->SetData( new wxBitmapDataObject( m_canvasList.at(unSelectedID)->m_bitmap ) );
	}
	return;
}

// save all figures in predicted structures
void GlycanAnalysisStructureResults::saveAllFigures(){
	// Check to make sure.
	/*
	for(unsigned int i = 0; i < m_canvasList.size(); i++){
		// wxTheClipboard->SetData( new wxBitmapDataObject( m_canvasList.at(i)->m_bitmap ) );
		wxString dir = 
		m_canvasList.at(i)->m_bitmap.SaveFile();
	}
	*/
	kome::core::MsppManager& msppMgr = kome::core::MsppManager::getInstance();
	std::string strFolderPath = FMT("%s\\%s", 
		msppMgr.getConfDir(), 
		RESULT_FOLDER
		);


	wxString imagePath = strFolderPath + wxT("\\test.bmp");
	// bool b = m_canvasList.at(0)->m_bitmap.SaveFile(imagePath, wxBITMAP_TYPE_BMP);

	return;
}

// copy all figures in predicted structures
void GlycanAnalysisStructureResults::copyAllFigures(wxCommandEvent& evt){
	
	int nCount = m_canvasList.size();

	// Check to make sure.
	if(nCount > 0){
		wxMemoryDC dc;
		wxMemoryDC dc_work;
		wxBitmap bmp(GLYCAN_CANVAS_WIDTH,nCount * GLYCAN_CANVAS_HEIGHT);
		dc.SelectObject(bmp);
		for(int i=0;i < nCount;i++){
			dc_work.SelectObject(m_canvasList.at(i)->m_bitmap);
			dc.Blit(0,i * GLYCAN_CANVAS_HEIGHT,GLYCAN_CANVAS_WIDTH,GLYCAN_CANVAS_HEIGHT,&dc_work,0,0);
		}
		wxTheClipboard->SetData( new wxBitmapDataObject(bmp) );

		dc.SelectObject( wxNullBitmap );
		dc_work.SelectObject( wxNullBitmap );
	}
	return;
}

// open the browser to access KEGG Glycan
void GlycanAnalysisStructureResults::accessKEGG(wxCommandEvent& evt){

	// Check to make sure.
	if(m_canvasList.size() > 0){
		unsigned int unSelectedID;
		bool bResult = getSelectedID(&unSelectedID);
		if(!bResult){
			LOG_ERROR(FMT("[GlycanAnalysisStructureResults] There is no selected radiobutton."));
			return;
		}
		wxString	strBrowser = FMT( "explorer \"%s%s\"", KEGG_GLYCAN_URL, m_canvasList.at(unSelectedID)->getGNumber() );
		::wxExecute( strBrowser, wxEXEC_MAKE_GROUP_LEADER );
	}
	return;
}

// get ID of selected radiobutton
bool GlycanAnalysisStructureResults::getSelectedID(unsigned int* selectedID){

	*selectedID = 0;
	bool bResult = false;
// 2014/02/24 Change by nakaya Change of a selective state ->
	//for(unsigned int i = 0;i < m_radioBtnList.size();i++){
	//	if(m_radioBtnList.at(i)->GetValue()){
	//		*selectedID = i;
	//		bResult = true;
	//	}
	//}
	for(unsigned int i = 0;i < m_canvasList.size();i++){
		if(m_canvasList.at(i)->m_bSelect){
			*selectedID = i;
			bResult = true;
		}
	}
// 2014/02/24 Change by nakaya Change of a selective state <-
	return bResult;
}

// select target radiobutton
void GlycanAnalysisStructureResults::select(unsigned int unID){
// 2014/02/24 Change by nakaya Change of a selective state ->
	//if(unID < m_radioBtnList.size()){
	//	m_radioBtnList.at(unID)->SetValue(true);
	//}
	if(unID < m_canvasList.size()){
		for(unsigned int i = 0; i < m_canvasList.size(); i++ ){
			if(i == unID){
				m_canvasList[i]->m_bSelect = true;	
				m_rankCanvasList[i]->m_bSelect = true;
				m_assignedCanvasList[i]->m_bSelect = true;
				m_scoreCanvasList[i]->m_bSelect = true;
			}
			else
			{
				m_canvasList[i]->m_bSelect = false;
				m_rankCanvasList[i]->m_bSelect = false;
				m_assignedCanvasList[i]->m_bSelect = false;
				m_scoreCanvasList[i]->m_bSelect = false;
			}
		}
	}

	GlycanAnalysisManager& gaManager = GlycanAnalysisManager::getInstance();	
	gaManager.refreshSpectrum();
// 2014/02/24 Change by nakaya Change of a selective state <-
}

// update predicted structures.
void GlycanAnalysisStructureResults::refreshResult(){
	int x = GetSize().GetWidth();
	int y = GetSize().GetHeight();
	SetSizerAndFit(m_sizer, false);
	SetScrollbars(0, 20, 0, 50,0, 0, false);
	SetScrollRate(0, 50);
	SetSize(x,y);
	this->Refresh();
}


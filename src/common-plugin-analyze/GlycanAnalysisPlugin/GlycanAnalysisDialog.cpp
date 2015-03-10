/**
 * @file GlycanAnalysisDialog.cpp
 * @brief Dialog class of GlycanAnalysisPlugin
 *
 * @author N.Tashiro
 * @date 2012.04.02
 * 
 * Copyright (C) 2014 Shimadzu Corporation All rights reserved.
 */


#include "stdafx.h"
#include "GlycanAnalysisDialog.h"
#include "GlycanAnalysisManager.h"
#include "GlycanAnalysisStructureDialog.h"

/** Layout (Border Size)*/
#define BORDER_SIZE		3

/** Layout (Grid Gap)*/
#define GRID_GAP		3

/** Layout (UI size)*/
#define PARAMATER_SIZE	156

/** parameters for get settings (section)*/
#define SECTION								"Auto Peak Picking"

/** parameters for get settings (peak algorithm)*/
#define PEAK_ALGORITHM_KEY					"SPEC_PEAK_ALGORITHM"

/** parameters for get settings (picking function)*/
#define SPECTRUM_PEAK_PICKING_FUNC_TYPE		"SPEC_PEAK_PICKING"

/** parameters for get settings (label)*/
#define LABELING_NAME						"Peak Detector"

#define RESULT_FOLDER	"GLYCAN_ANALYSIS"
#define RESULT_NAME	"result.html"

/** default tolerance */
#define TOLERANCE_DEFAULT "0.5"

/** dialog width */
#define DIALOG_WIDTH 900

/** dialog height */
#define DIALOG_HEIGHT 700

/** input char size */
#define INPUT_CHAR_SIZE 2048

using namespace kome::glycan;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG

using namespace kome::window;

BEGIN_EVENT_TABLE( GlycanAnalysisDialog, kome::window::StandardDialog )
	EVT_INIT_DIALOG( GlycanAnalysisDialog::onInitDialog )
	EVT_BUTTON( ID_BUTTON_DEFAULT, GlycanAnalysisDialog::onDefaultButton )
	EVT_BUTTON( ID_BUTTON_APPLY, GlycanAnalysisDialog::onApplyButton )
	EVT_BUTTON( ID_BUTTON_CLOSE, GlycanAnalysisDialog::onCloseButton )
// 2014/02/24 Add by nakaya Change of a selective state ->
	EVT_TIMER( ID_DRAW_TIMER, GlycanAnalysisDialog::OnTimer )
// 2014/02/24 Add by nakaya Change of a selective state <-
END_EVENT_TABLE()

// constructor
GlycanAnalysisDialog::GlycanAnalysisDialog( wxWindow* parent )
: m_parameters( NULL, false ),
	kome::window::StandardDialog(parent,"Glycan Analysis") {
	

//	wxSizer* mainSizer = createMainSizer();
//	SetSizer( mainSizer );
	createControls();

// 2014/02/24 Add by nakaya Change of a selective state ->
	// initial display
	InitDisplay();

	m_timer = new wxTimer( this, ID_DRAW_TIMER );
// 2014/02/24 Add by nakaya Change of a selective state <-	
}

// destructor
GlycanAnalysisDialog::~GlycanAnalysisDialog() {
// 2014/02/24 Add by nakaya Change of a selective state ->
	// kill timer
	if ( m_timer != NULL ) {
		m_timer->Stop();
		delete m_timer;
		m_timer = NULL;
	}
// 2014/02/24 Add by nakaya Change of a selective state <-
	
	delete m_pResultList;
	SetSizer(NULL);
}

// 2014/02/24 Add by nakaya Change of a selective state ->
// timer
void GlycanAnalysisDialog::OnTimer( wxTimerEvent& event ) {
	if ( m_pResultList ) {
		m_pResultList->Refresh();
	}
}

// 2014/02/24 Add by nakaya Change of a selective state ->
// initial display
void GlycanAnalysisDialog::InitDisplay() {
	if ( m_pResultList ) {
		m_pResultList->clearResult();
		
		GLYCAN_STRUCTURE glycanStructure;
		for ( unsigned int i = 0; i < 4; i++ ) {
			m_pResultList->addResult( &glycanStructure, NULL, "", "" );
		}
		m_pResultList->refreshResult();
	}
}
// 2014/02/24 Add by nakaya Change of a selective state <-

// close this dialog(from cancel button)
bool GlycanAnalysisDialog::onCancel(){
	// have to delete annotations and result
	GlycanAnalysisManager& gaManager = GlycanAnalysisManager::getInstance();
	gaManager.closeStructureDialog();
	gaManager.clearAnnotations();
	gaManager.refreshSpectrum();
	kome::window::StandardDialog::onCancel();
	return true;
}

// set label list
bool GlycanAnalysisDialog::setLabelList(std::vector< GLYCAN_LABEL > *labelList){
	if(labelList == NULL){
		return false;
	}

	m_pLabelingList->Clear();

	unsigned int unSize = labelList->size();

	if(unSize == 0){
		return false;
	}

	for(unsigned int i = 0; i < unSize; i++){
		m_pLabelingList->Append(wxString(labelList->at(i).strLabelName.c_str()));
	}

	m_pLabelingList->SetStringSelection(GLYCAN_ANALYSIS_LABELING_DEFAULT);

	return true;
}

// set anion list
bool GlycanAnalysisDialog::setAnionList(std::vector< GLYCAN_ANION > *anionList){

	if(anionList == NULL){
		return false;
	}

	m_pAnionList->Clear();

	unsigned int unSize = anionList->size();

	if(unSize == 0){
		return false;
	}

	for(unsigned int i = 0; i < unSize; i++){
		m_pAnionList->Append(wxString(anionList->at(i).strAnionName.c_str()));
	}

	m_pAnionList->SetStringSelection(GLYCAN_ANALYSIS_ANION_DEFAULT);

	return true;
}

// create main sizer
wxSizer* GlycanAnalysisDialog::createMainSizer()
{
	// create Sizer
	/*
	wxFlexGridSizer* mainSizer = new wxFlexGridSizer( 1, 2, 10, 10 );
	mainSizer->AddGrowableCol( 0 );
	mainSizer->AddGrowableRow( 0 );
	*/

	wxFlexGridSizer* subSizer = new wxFlexGridSizer( 3, 1, 10, 10 );
	subSizer->AddGrowableCol( 0 );
	subSizer->AddGrowableRow(2);

	// add sizers to main sizer
	subSizer->SetHGap(100);
	subSizer->Add( createParameterSizer(), 1, wxALL | wxGROW, 10 );
	subSizer->Add( createNewButton(), 0, wxALL | wxGROW, 10 );
	subSizer->Add( createResultSizer(), 1, wxALL | wxGROW, 10 );

	// mainSizer->Add(subSizer, 1, wxALL | wxGROW, 10);
	// mainSizer->Add(createDetailSizer(), 1, wxALL | wxGROW, 10);

	// return mainSizer;
	return subSizer;
}


wxSizer* GlycanAnalysisDialog::createDetailSizer(){
	wxFlexGridSizer* detailSizer = new wxFlexGridSizer( 2, 1, 0, 0 );

	detailSizer->AddGrowableRow(1);
	m_pstcDetailTitle = new wxStaticText(this, ID_STATIC_TITLE_LABELING, wxT("detail"));
	detailSizer->Add(m_pstcDetailTitle);
	m_pDetailHtml = new wxHtmlWindow(this, -1, wxDefaultPosition, wxSize(380, 350), wxHW_SCROLLBAR_NEVER);
	// m_pDetailHtml->SetSize(400, 200);
	// m_pDetailHtml->SetRelatedStatusBar(0);

	
	kome::core::MsppManager& msppMgr = kome::core::MsppManager::getInstance();
	std::string strFolderPath = FMT("%s\\%s", 
		msppMgr.getConfDir(), 
		RESULT_FOLDER
		);
	std::string strHtmlPath = FMT("%s\\%s\\%s", 
		msppMgr.getConfDir(), 
		RESULT_FOLDER,
		RESULT_NAME
		);
	/*
	wxFileName htmlfile = wxFileName("C:\\Users\\morimoto.KTLAST\\.mspp\\GLYCAN_ANALYSIS\\result.html");
	
	wxString dir = wxPathOnly("C:\\Users\\morimoto.KTLAST\\.mspp\\GLYCAN_ANALYSIS\\result.html");
	wxString filename = wxFileNameFromPath("C:\\Users\\morimoto.KTLAST\\.mspp\\GLYCAN_ANALYSIS\\result.html");
	wxFileName htmlfile = wxFileName(dir, filename);
	*/
	// m_pDetailHtml->LoadPage("C:\\Users\\morimoto.KTLAST\\.mspp\\GLYCAN_ANALYSIS\\result.html");
	// bool i = m_pDetailHtml->LoadFile(htmlfile);
	// wxString imagePath=wxGetCwd() + wxT("\\a.bmp");


	wxFileSystem::AddHandler(new wxMemoryFSHandler);
	wxString imagePath;
	wxImage image;
	wxBitmap bitmap;

	imagePath = strFolderPath + wxT("\\G00199.bmp");
	if(!image.LoadFile(imagePath)) {
		wxMessageBox(wxT("failed to load a figure"), wxT("wxMemoryFSHandler"), wxICON_ERROR);
		return detailSizer;
	}
	bitmap = wxBitmap(image);
	if(!bitmap.Ok()) {
		wxMessageBox(wxT("failed to load a figure"), wxT("wxMemoryFSHandler"), wxICON_ERROR);
		return detailSizer;
	}
	wxMemoryFSHandler::AddFile(_T("G00199.bmp"), bitmap, wxBITMAP_TYPE_BMP);
	imagePath = strFolderPath + wxT("\\G02019.bmp");
	if(!image.LoadFile(imagePath)) {
		wxMessageBox(wxT("failed to load a figure"), wxT("wxMemoryFSHandler"), wxICON_ERROR);
		return detailSizer;
	}
	bitmap = wxBitmap(image);
	if(!bitmap.Ok()) {
		wxMessageBox(wxT("failed to load a figure"), wxT("wxMemoryFSHandler"), wxICON_ERROR);
		return detailSizer;
	}
	wxMemoryFSHandler::AddFile(_T("G02019.bmp"), bitmap, wxBITMAP_TYPE_BMP);

	std::string strHtmlCode;
	// read HTML file
	if(!fileexists(strHtmlPath.c_str())) {
		LOG_ERROR(FMT("Html file is not found.[%s]",strHtmlPath.c_str()) );
		return detailSizer;
	}
	FILE *fp;
	fp = fileopen(strHtmlPath.c_str(), "r");
	char line[INPUT_CHAR_SIZE];
	if (NULL == fp) {
		LOG_ERROR(FMT("Html file is not opened.[%s]",strHtmlPath.c_str()) );
		return detailSizer;
	}
	while( fgets( line, INPUT_CHAR_SIZE, fp ) != NULL) {
		strHtmlCode += line;
	}
	fclose(fp);
	wxMemoryFSHandler::AddFile(_T("result.html"), _T(strHtmlCode.c_str()));

	m_pDetailHtml->SetBorders(0);
	m_pDetailHtml->LoadPage(_T("memory:result.html"));
	m_pDetailHtml->SetSize(m_pDetailHtml->GetInternalRepresentation()->GetWidth(), 
                  m_pDetailHtml->GetInternalRepresentation()->GetHeight());






	

	detailSizer->Add(m_pDetailHtml, 0, wxALIGN_CENTER_VERTICAL | wxALIGN_CENTER_HORIZONTAL | wxGROW);

	return detailSizer;
}

// create param sizer
wxSizer* GlycanAnalysisDialog::createParameterSizer(){
	wxFlexGridSizer* paramSizer = new wxFlexGridSizer( 4, 2, 0, 0 );

	m_pstcPeakDetectorTitle	= new wxStaticText(this,ID_STATIC_TITLE_PEAK_DETECTOR,wxT(LABELING_NAME));
	paramSizer->Add(m_pstcPeakDetectorTitle,0,wxALIGN_CENTER_VERTICAL | wxALL,15);

	// page
	kome::plugin::SettingsPage* page = new kome::plugin::SettingsPage( m_parameters );
	
	// peak detector
	kome::plugin::SettingsValue* value = m_parameters.addValue();
	value->setType( kome::plugin::SettingsValue::TYPE_STRING );
	value->setEnumType( kome::plugin::SettingsValue::ENUM_PLUGIN );
	value->setEnumKey( SPECTRUM_PEAK_PICKING_FUNC_TYPE );
	value->setRequired( false );

	kome::plugin::SettingsParam* param = m_parameters.addParam();
	param->setName( LABELING_NAME );
	param->setValue( value );
	param->setSection( SECTION );
	param->setKey( PEAK_ALGORITHM_KEY );	

	kome::plugin::SettingsForm* form = page->addForm();
	
	form->setParam( param );

	// panel
	kome::window::SettingsPanel* panel
		= new kome::window::SettingsPanel( this, page, false, &m_settings, true );
	panel->SetSize(-1,-1);
	paramSizer->Add( panel, 1);

	m_pstcLabelingTitle		= new wxStaticText(this,ID_STATIC_TITLE_LABELING,wxT("Labeling"));
	paramSizer->Add(m_pstcLabelingTitle,2,wxALIGN_CENTER_VERTICAL | wxALL,15);
	m_pLabelingList			= new wxChoice(this,ID_LIST_LABELING,wxDefaultPosition,wxDefaultSize);

	m_pLabelingList->SetSizeHints(PARAMATER_SIZE,-1);
	paramSizer->Add(m_pLabelingList,3,wxALIGN_CENTER_VERTICAL | wxALL,15);
	
	m_pstcToleranceTitle	= new wxStaticText(this,ID_STATIC_TITLE_TOLERANCE,wxT("Tolerance (Da)"));
	paramSizer->Add(m_pstcToleranceTitle,4,wxALIGN_CENTER_VERTICAL | wxALL,15);
	m_pedtTolerance			= new wxTextCtrl(this, ID_STATIC_TOLERANCE,TOLERANCE_DEFAULT,wxDefaultPosition,wxDefaultSize,0L,wxTextValidator(wxFILTER_NUMERIC));
	m_pedtTolerance->SetSizeHints(PARAMATER_SIZE,-1);
	paramSizer->Add(m_pedtTolerance,5,wxALIGN_CENTER_VERTICAL | wxALL,15);

	m_pstcAnionTitle		= new wxStaticText(this,ID_STATIC_TITLE_ANION,wxT("Anion"));
	paramSizer->Add(m_pstcAnionTitle,6,wxALIGN_CENTER_VERTICAL | wxALL,15);
	m_pAnionList			= new wxChoice(this,ID_LIST_ANION,wxDefaultPosition,wxDefaultSize);
	m_pAnionList->SetSizeHints(PARAMATER_SIZE,-1);
	paramSizer->Add(m_pAnionList,7,wxALIGN_CENTER_VERTICAL | wxALL,15);
	
	return paramSizer;

}

// create button sizer
wxSizer* GlycanAnalysisDialog::createButtonSizer(){
	wxFlexGridSizer* buttonSizer = new wxFlexGridSizer( 1, 4, 0, 0 );
	return buttonSizer;
}

wxSizer* GlycanAnalysisDialog::createNewButton(){
	wxFlexGridSizer* buttonSizer = new wxFlexGridSizer( 1, 4, 0, 0 );
	buttonSizer->AddGrowableCol(0);
	wxSize buttonSize = wxSize(-1,-1);
	buttonSizer->AddStretchSpacer();
	m_pDefaultButton	= new wxButton(this,ID_BUTTON_DEFAULT,wxT("Default"));
	buttonSizer->Add(m_pDefaultButton);
	m_pApplyButton		= new wxButton(this,ID_BUTTON_APPLY,wxT("Analyze"));
	buttonSizer->Add(m_pApplyButton);
	m_pCloseButton		= new wxButton(this,ID_BUTTON_CLOSE,wxT("Close"));
	buttonSizer->Add(m_pCloseButton);
	return buttonSizer;
}

// create result sizer
wxSizer* GlycanAnalysisDialog::createResultSizer(){
// 2014/02/20 Change by nakaya The addition of a display item  ->
	//wxFlexGridSizer* resultSizer = new wxFlexGridSizer( 2, 1, 0, 0 );

	//resultSizer->AddGrowableRow(1);
	//// resultSizer->AddGrowableCol(0);
	//m_pstcResultTitle	= new wxStaticText(this,ID_STATIC_TITLE_LABELING,wxT("Predicted Structures"));
	//resultSizer->Add(m_pstcResultTitle);
	//m_pResultList		= new GlycanAnalysisStructureResults(this);
	//resultSizer->Add(m_pResultList,1,wxALL | wxGROW );
	wxStaticBoxSizer* resultGroupSizer = new wxStaticBoxSizer( wxVERTICAL, this, wxT( "Predicted Structures" ) );

	wxFlexGridSizer* resultSizer = new wxFlexGridSizer( 2, 1, 0, 0 );

	wxFlexGridSizer* listTitleSizer = new wxFlexGridSizer( 1, 1, 0, 0 );
	
	std::string title = "";
	for(int i = 0; i < 7; i++){
		title += " ";
	}
	title += "Rank";

	for(int i = 0; i < 37; i++){
		title += " ";
	}
	title += "Structure";

	for(int i = 0; i < 35; i++){
		title += " ";
	}
	title += "Assigned / Theoretical";

	for(int i = 0; i < 14; i++){
		title += " ";
	}
	title += "Score";

	m_pstcStructureTitle	= new wxStaticText(this,ID_STATIC_STRUCTURE_TITLE,title.c_str());
	listTitleSizer->Add(m_pstcStructureTitle,1,wxALL | wxGROW );

	m_pResultList		= new GlycanAnalysisStructureResults(this);
	resultSizer->Add(listTitleSizer,1,wxALL | wxGROW );
	resultSizer->Add(m_pResultList,1,wxALL | wxGROW );

	resultGroupSizer->Add( resultSizer, 1, wxGROW | wxALL, BORDER_SIZE );

// 2014/02/20 Change by nakaya The addition of a display item  <-
	return resultGroupSizer;
}

// show this dialog
bool GlycanAnalysisDialog::Show( bool show ) {
	// show
	/*
	if ( show == true ) {
		// set dialog min size
		wxSize size;
		size.SetWidth( DIALOG_WIDTH );
		size.SetHeight( DIALOG_HEIGHT );
		SetMinSize(size);
		size.SetHeight( -1 );
		SetMaxSize(size);

		// set dialog size
		SetSize( -1 , -1 );

		// set position
		wxPoint point;
		point.x = -1;
		point.y = -1;
		SetPosition( point );

		// set maximize
		Maximize( false );

		if ( IsIconized() ) {
			Iconize( false );
		}
	}
	// hide
	else {

	}
	*/
// 2014/02/24 Add by nakaya Change of a selective state ->
	if ( show == true ) {
		m_timer->Start( INTERVAL_DRAW_TIMER, false );
	}
	else {
		m_timer->Stop();
	}
// 2014/02/24 Add by nakaya Change of a selective state <-

	return kome::window::StandardDialog::Show( show );
}

// initialize this dialog
void GlycanAnalysisDialog::onInitDialog( wxInitDialogEvent& evt ) {
	
}

// set parameter to default
void GlycanAnalysisDialog::onDefaultButton( wxCommandEvent& evt ){
	
	m_pLabelingList->SetStringSelection(GLYCAN_ANALYSIS_LABELING_DEFAULT);
	m_pAnionList->SetStringSelection(GLYCAN_ANALYSIS_ANION_DEFAULT);
	m_pedtTolerance->SetValue(wxString(TOLERANCE_DEFAULT));
}

// start calculate
void GlycanAnalysisDialog::onApplyButton( wxCommandEvent& evt ){
	TransferDataFromWindow();

	m_pResultList->clearResult();


	GlycanAnalysisManager& gaManager = GlycanAnalysisManager::getInstance();

	gaManager.clearAnnotations();

	kome::objects::Spectrum* spec = gaManager.getSpectrum();
	if(spec == NULL){
		return;
	}
	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();
	kome::objects::PeaksManager& pkMgr = kome::objects::PeaksManager::getInstance();

	kome::objects::Peaks* peaks = pkMgr.getPeaks( spec );
	if(peaks == NULL){
		return;
	}
	kome::plugin::PluginFunctionItem* peakDetector;

	// <tashiro> fixed:120518 SPEC:85989
	kome::ident::IdentificationManager& identMgr = kome::ident::IdentificationManager::getInstance();
	const char* s = m_settings.getParameterValue( LABELING_NAME );
	if(strcmp(s,"none")!=0){
		peakDetector = plgMgr.getFunctionItem( SPECTRUM_PEAK_PICKING_FUNC_TYPE, s );
		kome::objects::SettingParameterValues* m_peakSettings = NULL;
		if(peakDetector != NULL){
			m_peakSettings = m_settings.getSubParameterValues(
				LABELING_NAME,
				peakDetector->getShortName()
			);
		}
	}
	else{
		peakDetector = NULL;
	}
	// </tashiro>
	
	// begin busy cursor on GlycanAnalysis dialog
	wxBeginBusyCursor();

	// create peaks list
	// objects
	kome::core::DataPoints dps;
	kome::core::DataPoints parentDps;
	kome::objects::Peaks specPeaks;
	kome::objects::Peaks parentPeaks;

	kome::objects::Parameters peakParams;

	// data points
	specPeaks.setPrecursor( spec->getPrecursor() );
	spec->getXYData( &dps, false );
	specPeaks.setDataPoints( dps );
	
	// peak detection
	kome::plugin::PluginCallTool::setSpectrum( peakParams, *spec );
	kome::plugin::PluginCallTool::setXYData( peakParams, dps );
	kome::plugin::PluginCallTool::setPeaks( peakParams, specPeaks );
	kome::plugin::PluginCallTool::setSettingValues( peakParams, m_settings );

	// <tashiro> fixed:120518 SPEC:85989
	if(peakDetector != NULL){
		peakDetector->getCall()->invoke( &peakParams );
	}
	else{
		specPeaks = *peaks;
	}
	// </tashiro>

// 2014/02/24 Change by nakaya Change of a selective state ->
	//GlycanParams params;
	//m_pedtTolerance->GetValue().ToDouble(&(params.dTolerance));
	//params.strAnion = m_pAnionList->GetStringSelection();
	//params.strLabel = m_pLabelingList->GetStringSelection();

	//gaManager.setParams(&params);
	//if(!gaManager.calculate(&specPeaks,&params)){

	//	// After calling getlasterrormessage once, the error is deleted.
	//	// We want to use it twice, so make a copy first.
	//	std::string lasterrormessage = getlasterrormessage();
	//	if (lasterrormessage.size() > 0)
	//	{
	//		kome::window::WindowTool::showInfo( lasterrormessage.c_str() );	
	//	}
	//}

	//std::vector<NGlycan>* nGlycans = gaManager.getGlycans();

	//GLYCAN_STRUCTURE glycanStructure;

	//for(unsigned int i = 0; i < nGlycans->size();i++){
	//	gaManager.parseKCFFile(nGlycans->at(i).gNumber,&glycanStructure);
	//	glycanStructure.strScore = FMT("%d / %d", nGlycans->at(i).unMatched, nGlycans->at(i).unTheoretical);
	//	m_pResultList->addResult(&glycanStructure,NULL,params.strLabel.c_str(),params.strAnion.c_str());
	//}

	//// save bmp file
	//m_pResultList->saveAllFigures();

	//m_pResultList->select(0);
	//gaManager.refreshSpectrum();
	//m_pResultList->refreshResult();

	GlycanParams params;
	m_pedtTolerance->GetValue().ToDouble(&(params.dTolerance));
	params.strAnion = m_pAnionList->GetStringSelection();
	params.strLabel = m_pLabelingList->GetStringSelection();

	gaManager.setParams(&params);
	if ( !gaManager.calculate(&specPeaks,&params) ) {
		InitDisplay();

		// After calling getlasterrormessage once, the error is deleted.
		// We want to use it twice, so make a copy first.
		std::string lasterrormessage = getlasterrormessage();
		if (lasterrormessage.size() > 0)
		{
			kome::window::WindowTool::showInfo( lasterrormessage.c_str() );	
		}
	}
	else {
		std::vector<NGlycan>* nGlycans = gaManager.getGlycans();

		GLYCAN_STRUCTURE glycanStructure;

		for(unsigned int i = 0; i < nGlycans->size();i++){
			gaManager.parseKCFFile(nGlycans->at(i).gNumber,&glycanStructure);
			glycanStructure.strRank = FMT("%d", i + 1);
			glycanStructure.strAssigned = FMT("%d / %d", nGlycans->at(i).unMatched, nGlycans->at(i).unTheoretical);
			glycanStructure.strScore = FMT("%.2f", nGlycans->at(i).dRatio);
			m_pResultList->addResult(&glycanStructure,NULL,params.strLabel.c_str(),params.strAnion.c_str());
		}

		// save bmp file
		m_pResultList->saveAllFigures();

		m_pResultList->select(0);		
		m_pResultList->refreshResult();
	}
	gaManager.refreshSpectrum();
// 2014/02/24 Change by nakaya Change of a selective state <-

	wxEndBusyCursor();
}

// close this dialog
void GlycanAnalysisDialog::onCloseButton( wxCommandEvent& evt ){
	
	this->Close();

}

// transfer data from window
bool GlycanAnalysisDialog::TransferDataFromWindow() {
	// default
	if( !kome::window::StandardDialog::TransferDataFromWindow() ) {
		return false;
	}

	return true;
}

// get anion text
const char* GlycanAnalysisDialog::getAnion(){
	return m_pAnionList->GetStringSelection();
}

// get selected figure ID
bool GlycanAnalysisDialog::getSelectedResult(unsigned int* unID){
	return m_pResultList->getSelectedID(unID);
}

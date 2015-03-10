/**
 * @file PeakDetectionPanel.cpp
 * @brief interfaces of PeakDetectionPanel class
 *
 * @author M.Izumi
 * @date 2013.12.10
 * 
 * Copyright(C) 2013 Shimadzu Corporation. All rights reserved.
 */

#include "stdafx.h"
#include "PeakDetectionPanel.h"
#include "CreatePeakMatrixDialog.h"
#include "MatrixViewManager.h"
#include "PeakMatrixManager.h"

#include "matrix_flow05.xpm"

#include <sstream>


using namespace kome::matrix::view;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


#define GRID_GAP						3
#define BORDER_SIZE						3

#define PEAK_POSITION_PLUGIN_TYPE		"QUANT_PEAK_POS"

#define PEAK_POSITION_PARAM_NAME		"peak position"
#define INIT_FLG_PARAM_NAME				"initialize"

#define GPP_STANDARD_SAMPLE_PARAM_KEY	"GET_PEAK_POSITIONS_STANDARD_SAMPLE"
#define PEAK_POSITION_PARAM_KEY			"PEAK_POSITIONS"
#define INIT_FLG_PARAM_KEY				"INIT_POSITIONS"
#define PEAK_SEARCH_PLUGIN_TYPE			"QUANT_PEAK_VAL"

#define SEARCH_PARAM_NAME				"search"
#define RT_TOLERANCE_PARAM_NAME			"rt tol"
#define MZ_TOLERANCE_PARAM_NAME			"mz tol"
#define INIT_FLAG_PARAM_NAME			"init flag"

#define FM_STANDARD_SAMPLE_PARAM_KEY	"FILL_MATRIX_STANDARD_SAMPLE"
#define FM_TARGET_SAMPLES_PARAM_KEY		"FILL_MATRIX_TARGET_SAMPLES"
#define SEARCH_PARAM_KEY				"PEAK_SEARCHING_METHOD"
#define RT_TOLERANCE_PARAM_KEY			"RT_TOLERANCE"
#define MZ_TOLERANCE_PARAM_KEY			"MZ_TOLERANCE"
#define INIT_FLAG_PARAM_KEY				"INIT_FLAG"

BEGIN_EVENT_TABLE( PeakDetectionPanel, kome::window::PeakMatrixSettingsPageBase )
	EVT_KOME_SETTINGS_PANEL(wxID_ANY, PeakDetectionPanel::onSettingChanged)
END_EVENT_TABLE()

// constructor
PeakDetectionPanel::PeakDetectionPanel( wxWizard* pParent )
	 : kome::window::PeakMatrixSettingsPageBase( pParent, "Peak Detection" ){

	// 初期化
	m_helpPanel = NULL;
	m_peakPos = NULL;
	m_peakPosSettings = NULL;
	m_search = NULL;
	m_searchSettings = NULL;

	m_pParentDlg = dynamic_cast<CreatePeakMatrixDialog*>( pParent );
	
	createControls();
}

// destructor
PeakDetectionPanel::~PeakDetectionPanel(){
}

// Validate panel settings 
bool PeakDetectionPanel::IsValid()
{
	return m_helpPanel->Validate();
}

// Perform Apply-time validation checks (i.e. checks that must not be done in IsValid function)
bool PeakDetectionPanel::PreApplyValidationCheck(std::string& sError)
{
	bool bSuccess = true;

	std::ostringstream errorString;
		
	sError = errorString.str();

	return bSuccess;
}

// save panel settings
bool PeakDetectionPanel::SavePanelSettings( kome::core::Progress *pProgress )
{
	if( !TransferDataFromWindow() ) {
		return false;
	}
	// Have to return whether save has succeeded or failed, as this affects the status of the edited flag.
	bool bSuccess = false;
	
	pProgress->setStatus(GetPanelName().c_str());

	std::ostringstream infoString;

	// manager
	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();
	kome::ident::IdentificationManager& identMgr = kome::ident::IdentificationManager::getInstance();

	// selected project
	int project = GetActiveMatrix();

	// getting peak position method
	std::string fucName = m_helpPanel->getPeakPos();
	m_peakPos = plgMgr.getFunctionItem( PEAK_POSITION_PLUGIN_TYPE, fucName.c_str() ); // quant_peak_pos
	
	if( m_peakPos == NULL && !m_pParentDlg->getbEdit() ) {
		kome::window::WindowTool::showInfo( "Select a peak position method." );
		return false;
	}
		
	infoString << "Peak Positions: ";
	if( m_peakPos != NULL ){
		infoString << m_peakPos->getLongName() << "\n";

		m_peakPosSettings = m_helpPanel->getPeakPosSettings( PEAK_POSITION_PARAM_NAME, fucName.c_str() ); 
	
		for( unsigned int iParamIndex = 0; iParamIndex < m_peakPosSettings->getNumberOfParameters(); iParamIndex++ ){
			infoString << m_peakPosSettings->getParameterName(iParamIndex) << ": ";
			infoString << m_peakPosSettings->getParameterValue(iParamIndex) << "\n";
		}
	}

	// initialize flag
	bool binitFlg = m_helpPanel->getFlgParamName();

	infoString << "Initialize Positions: " << (binitFlg ? "true" : "false");

	m_pParentDlg->GetDialogProgressPtr()->setInformation(GetPanelName().c_str(), infoString.str().c_str());

	// standard group ID
	int stdGroupId = -1;

	std::vector< int > matrixGroups;
	std::vector< int > samples;
	identMgr.getGroups( matrixGroups, project );
	for( unsigned int i = 0; i < matrixGroups.size(); i++ ) {
		identMgr.getMatrixSamplesFromGroup( samples, matrixGroups[ i ] );
	}

	stdGroupId = identMgr.getStandardSample( project );

		
	// Peak Positions
	kome::ident::PeakPositions* peakPos = new kome::ident::PeakPositions();
	// peaks
	std::vector< int > peaks;
	identMgr.getMatrixPeaks( peaks, project );

	for( unsigned int i = 0; i < peaks.size(); i++ ) {
		double rt = identMgr.getMatrixPeakRt( peaks[i] );
		double mz = identMgr.getMatrixPeakMz( peaks[i] );
		int charge = identMgr.getMatrixPeakCharge( peaks[i] );

		peakPos->addPeakPosition( rt, mz, charge );
	}

	// clear peaks
	if( binitFlg ) {
		for( unsigned int i = 0; i < peaks.size(); i++ ) {
			identMgr.deleteMatrixPeak( peaks[ i ] );
		}
		if( m_peakPos != NULL ){
			delete peakPos;
			peakPos = new kome::ident::PeakPositions();
		}
	}
	
	// Always delete the added columns, so that previous Peak Analysis results are deleted.
	std::vector< int > columns;
	identMgr.getMatrixColumns( columns, project );
	for( unsigned int i = 0; i < columns.size(); i++ ) {
		identMgr.deleteMatrixColumn( columns[ i ] );
	}

	// standard group
	kome::ident::SampleReadInfo readInfo;
	kome::objects::Sample* stdSample = NULL;
	int stdSampleId = identMgr.getMatrixSampleSample( stdGroupId );
	if( stdSampleId >= 0 ) {
		stdSample = identMgr.openSample( stdSampleId, readInfo );
	}
	
	// create sample getter
	kome::ident::SampleGetter* getter = new kome::ident::SampleGetter();
	getter->setMatrix( project );
	kome::operation::Alignment* alignment = identMgr.getAlignment( project );
	getter->setAlignment( alignment );
	getter->setStdSample( stdSample );		
	
	// parameters
	kome::objects::Parameters params;
	if( m_peakPosSettings != NULL ) {
		kome::plugin::PluginCallTool::setSettingValues( params, *m_peakPosSettings );
	}

	// Peak Postion Detector object
	kome::ident::PeakPositionsDetector* peakPosDetect = NULL;
	int peak = -1;

	if( m_peakPos != NULL ){
		peakPosDetect = (kome::ident::PeakPositionsDetector*)( m_peakPos->getCall()->invoke( &params ).prim.pt );
	
		if( peakPosDetect == NULL ){ // NULL チェック
			
			identMgr.releaseSample( stdSample, readInfo );

			if( peakPos != NULL ){
				delete peakPos;
			}
			if( getter != NULL ){
				delete getter;
			}
			return bSuccess;
		}
		
		peak = peakPosDetect->detectPeakPositions( *getter, *peakPos, *pProgress );  

		// エラーメッセージ表示
		std::string errMsg = peakPosDetect->getLastError();
		if( !errMsg.empty() ){
			kome::window::WindowTool::showError( errMsg.c_str() );
			return bSuccess;
		}
	
	}	

	bool brt = false;
	bool bmz = false;
	bool bch = false;
	
	std::map< std::string, int > colMap;

	// ピーク情報取得
	int numPeak = peakPos->getNumberOfPeaks();
	std::map< int, int > peakIdMap;
	for( int i=0; i < numPeak; i++ ){
		
		double rt =  peakPos->getPeakPositionRt( i );
		double mz =  peakPos->getPeakPositionMz( i );
		int charge = peakPos->getPeakPositonCharge( i ) ;

		if( rt > 0.0 ){
			brt = true;
		}
		if( mz > 0.0 ){
			bmz = true;
		}
		if( charge > 0.0 ){
			bch = true;
		}
		
		// add peak
		int matrixPeakId = identMgr.addMatrixPeak( rt, mz, charge, project );
		peakIdMap[ i ] = matrixPeakId;
		
		int numProp = peakPos->getNumberOfProperties();
		for( int j=0; j < numProp; j++ ){
			std::string colName = peakPos->getPropertyName( j );
			int colType = peakPos->getPropertyType( j );
			std::string colVal = peakPos->getPropertyValue( i, colName.c_str() );
			
			// column ID
			int colId = -1;
			if( colMap.find( colName ) == colMap.end() ) {    // new column
				colId = identMgr.addMatrixColumn( colName.c_str(), (kome::ident::ColumnDataType)colType, project );
				if( colId >= 0 ) {
					colMap[ colName ] = colId;
				}
			}
			else {
				colId = colMap[ colName ];
			}

			// set value
			if( colId >= 0 ) {
				identMgr.setAppendedValue( matrixPeakId, colId, colVal.c_str() );
			}
		}
		
		// peak values
		int numSample = peakPos->getNumberOfMatrixSamples();
		for( int j=0; j < numSample; j++ ){

			int matrixSampleId = peakPos->getMatrixSampleId( j );

			kome::objects::PeakElement* pk = peakPos->getPeak( i,  matrixSampleId );
			
			int profile = peakPos->getProfile( i, matrixSampleId );			
			double peakVal = peakPos->getPeakValue( i, matrixSampleId );
			if( matrixSampleId >= 0 ){			
				identMgr.setPeakValue( matrixPeakId, matrixSampleId, peakVal, pk, profile );
			}
		}
	}
		
	if( peak > 0 ){
		identMgr.updateMatrixFlags( project, brt, bmz, bch );
	}		
	

	MatrixViewManager& viewMgr = MatrixViewManager::getInstance();
	viewMgr.updateMatrixList();

	delete getter;
	getter = NULL;
		
	// peak value caluc
	peakValueCalc( project, pProgress, peakPos, peakIdMap );
	
	delete peakPos;
	peakPos = NULL;

	delete peakPosDetect;
	peakPosDetect = NULL;

	// close group
	identMgr.releaseSample( stdSample, readInfo );
	
	bSuccess = true;
	return (bSuccess);
}

// Update standard samples on this panel
void PeakDetectionPanel::updateStandardSamples(bool bReadFromINIfile){
}

// on a setting changing in the settings panel
void PeakDetectionPanel::onSettingChanged(kome::window::SettingsPanelEvent& evt)
{
	SetEdited();
	
	// ここでフラグ検索
	if( !TransferDataFromWindow() ) {
		return;
	}
		
	if( m_helpPanel != NULL ){
		m_helpPanel->setPeakValStatus();
	}
}

// Refresh panel contents for matrix
void PeakDetectionPanel::RefreshPanelContentsForMatrix()
{
	updateStandardSamples();

	updateSettings();
}

// transfer data from window
bool PeakDetectionPanel::TransferDataFromWindow(){
	// default
	if( !PeakMatrixSettingsPageBase::TransferDataFromWindow() ) {
		return false;
	}
	
	const char* peakPosName = m_helpPanel->getPeakPos();
	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();
	kome::plugin::PluginFunctionItem* item = plgMgr.getFunctionItem( PEAK_POSITION_PLUGIN_TYPE, peakPosName );
	std::string s = FMT( "- %s(%s)\n" , m_sPanelName.c_str(), (item != NULL ? item->getLongName() : "" ) );
	setSettingParam( s.c_str() );
		
	return true;
}

// create inner sizer
void PeakDetectionPanel::createInnerSizer(wxStaticBoxSizer* InnerSizer){
	
	// set bitmap
	wxBitmap addBmp( matrix_flow05_xpm );

	wxBoxSizer* bmtSizer = new wxBoxSizer( wxVERTICAL );
	bmtSizer->Add(  new wxStaticBitmap( this, wxID_ANY, addBmp ), 1, (wxALL & ~wxRIGHT) | wxGROW, BORDER_SIZE );
	wxSizer* gHoriznSizer = new wxBoxSizer( wxHORIZONTAL);
	gHoriznSizer->Add( bmtSizer, 1, (wxALL & ~wxRIGHT) | wxGROW | wxALIGN_CENTER_VERTICAL, BORDER_SIZE );
	
	InnerSizer->Add( gHoriznSizer );

	m_helpPanel = new PeakDetectionHelpPanel( this, GetINISectionNameForMatrix(), m_pParentDlg->getbEdit() );

	InnerSizer->Add( m_helpPanel, 1, wxALL | wxGROW | wxALIGN_CENTER_VERTICAL, BORDER_SIZE );
}

// update settings
void PeakDetectionPanel::updateSettings(){
	
	m_helpPanel->updateSettings();	// 2014.07.15 <Add> M.Izumi

	// Load values from the INI file
	// Update the settings in the UI
	TransferDataToWindow();
}

bool PeakDetectionPanel::peakValueCalc( int project, kome::core::Progress *pProgress, kome::ident::PeakPositions* peakPos, std::map< int, int >& peakIdMap ){
	
	pProgress->setStatus(GetPanelName().c_str());
	std::ostringstream infoString;

	// Have to return whether save has succeeded or failed, as this affects the status of the edited flag.
	bool bSuccess = false;
	
	// manager
	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();
	kome::ident::IdentificationManager& identMgr = kome::ident::IdentificationManager::getInstance();

	//-----------------------------------
	// peak value caluc
	//-----------------------------------
	// peak search
	const char* search = m_helpPanel->getSearch();
	m_search = plgMgr.getFunctionItem( PEAK_SEARCH_PLUGIN_TYPE, search );

	if( m_search != NULL ){
		infoString << "Peak Search: ";
		infoString << m_search->getLongName() << "\n";

		m_searchSettings = m_helpPanel->getSearchSettings( search );

		for( unsigned int iParamIndex = 0; iParamIndex < m_searchSettings->getNumberOfParameters(); iParamIndex++ ){
			infoString << m_searchSettings->getParameterName( iParamIndex ) << ": ";
			infoString << m_searchSettings->getParameterValue( iParamIndex ) << "\n";
		}
	}
		
	// RT tolerance
	double rtTol = m_helpPanel->getTol( RT_TOLERANCE_PARAM_NAME, 0.5 );

	infoString << "RT Tolerance: " << rtTol << "\n";

	// m/z tolerance
	double mzTol = m_helpPanel->getTol( MZ_TOLERANCE_PARAM_NAME, 0.5 ); 

	infoString << "m/z Tolerance: " << mzTol << "\n";

	m_pParentDlg->GetDialogProgressPtr()->setInformation(GetPanelName().c_str(), infoString.str().c_str());
			
	// groups
	std::vector< int > matrixGroups;
	std::vector< int > samples;
	identMgr.getGroups( matrixGroups, project );
	for( unsigned int i = 0; i < matrixGroups.size(); i++ ) {
		identMgr.getMatrixSamplesFromGroup( samples, matrixGroups[ i ] );
	}
	
	// standard group ID
	int stdGroupId = -1;

	// manager
	PeakMatrixManager& mgr = PeakMatrixManager::getInstance();
	stdGroupId = identMgr.getStandardSample( project );
	
	bool initFlg = m_helpPanel->getPeakValParamName();

/*
	// Always delete the added columns, so that previous Peak Analysis results are deleted.
	std::vector< int > columns;
	identMgr.getMatrixColumns( columns, project );
	for( unsigned int i = 0; i < columns.size(); i++ ) {
		identMgr.deleteMatrixColumn( columns[ i ] );
	}
*/
	
	// @date 2013/08/28 <Add> FUJITA -->
	// FillMatrixHelpPanelクラスのm_settings(Fill Matrix上のコントロールの情報一式)を読んでくる
	kome::objects::SettingParameterValues temp = m_helpPanel->getSettings();
	//  "supplement"ドロップダウンリストで選択された文字列を読む
	const char* supplement_method = temp.getParameterValue( "supplement" );
	// @date 2013/08/28 <Add> FUJITA <--

	// search
	bSuccess = mgr.fillMatrix(
		m_search,
		m_searchSettings,
		rtTol,
		mzTol,
		initFlg,
		samples,
		stdGroupId,
		pProgress,
		supplement_method,
		peakPos,
		m_helpPanel->getPeakValStatus(),
		peakIdMap
	);

	if( !pProgress->isStopped() && bSuccess ) {
		pProgress->fill();

		MatrixViewManager& viewMgr = MatrixViewManager::getInstance();
		viewMgr.updateMatrixList();
	}

	if (!bSuccess && !pProgress->isStopped())
	{
		kome::window::WindowTool::showError( "An error occurred while filling the matrix." );
	}

	return (bSuccess) && !pProgress->isStopped();
	
}

// -------------------------------------------------------------------------------------
// class Peak Detection help pnale
// -------------------------------------------------------------------------------------
BEGIN_EVENT_TABLE( PeakDetectionHelpPanel, kome::window::ParametersAndHelpPanel )	
END_EVENT_TABLE()

// constructor
PeakDetectionHelpPanel::PeakDetectionHelpPanel( wxWindow* parent, const char* section, bool bEdit )
	: kome::window::ParametersAndHelpPanel( parent ), m_params( NULL, false ){
	m_section = section;
	m_bEdit = bEdit;

	m_peakPosBmp = NULL;
	m_peakPosValBmp = NULL;
	m_peakPosValBmp2 = NULL;
	m_posPanel = NULL;
	m_valPanel = NULL;

	m_bpeakVal = false;
	

	// create controls
	createControls();
}

// destructor
PeakDetectionHelpPanel::~PeakDetectionHelpPanel(){
}

// create main sizer
wxSizer* PeakDetectionHelpPanel::createMainSizer(){
	wxBoxSizer* pVerticalSizer = new wxBoxSizer( wxVERTICAL );
	
	pVerticalSizer->AddSpacer( 20 );
	pVerticalSizer->Add( new wxStaticText( this, wxID_ANY, wxT("Select the peak detection method.\n"
		"Determinating peak positions method makes peak matrix rows.\n"
		"And Calculating peak values method fills peak matrix." )) );
	
	pVerticalSizer->AddSpacer( 20 );

	wxBoxSizer* box = new wxStaticBoxSizer(wxVERTICAL, this, wxT("Peak Positions Determination"));	

	// page
	kome::plugin::SettingsPage* page = new kome::plugin::SettingsPage( m_params );
	page->setName( "alignment" );

	// peak position
	kome::plugin::SettingsValue* val = m_params.addValue();
	val->setType( kome::plugin::SettingsValue::TYPE_STRING );
	val->setEnumKey( PEAK_POSITION_PLUGIN_TYPE );
	val->setEnumType( kome::plugin::SettingsValue::ENUM_PLUGIN );
	if( !m_bEdit ){
		val->setRequired( true );
	}

	kome::plugin::SettingsParam* param = m_params.addParam();
	param->setName( PEAK_POSITION_PARAM_NAME );
	param->setValue( val );
	param->setSection( m_section.c_str() );
	param->setKey( PEAK_POSITION_PARAM_KEY );
	
	kome::plugin::SettingsForm* form = page->addForm();
	form->setParam( param );
	form->setTitle( "Peak Positions" ); 
	form->setDesc( "Select peak positions determination method." );
	
	// panel
	m_posPanel = new kome::window::SettingsPanel( this, page, false, &m_settings );
	box->Add( m_posPanel, 0, wxALIGN_CENTRE_VERTICAL | wxALL, BORDER_SIZE  );
		
	wxBoxSizer* iconSizer = new wxBoxSizer( wxHORIZONTAL );
	// icon manager object
	kome::window::IconManager& iconMgr = kome::window::IconManager::getInstance();
	wxBitmap addPosBmp( *iconMgr.getIcon( "peak_pos" ) );
	m_peakPosBmp = new wxStaticBitmap( this, wxID_ANY, addPosBmp );
	iconSizer->Add( m_peakPosBmp, 0, wxALIGN_CENTRE_VERTICAL | wxALL, BORDER_SIZE );

	wxBitmap addValBmp( *iconMgr.getIcon( "peak_val" ) );
	m_peakPosValBmp = new wxStaticBitmap( this, wxID_ANY, addValBmp );
	iconSizer->Add( m_peakPosValBmp, 0, wxALIGN_CENTRE_VERTICAL | wxALL, BORDER_SIZE );
	box->Add( iconSizer, 0, wxGROW | wxALL, BORDER_SIZE );

	pVerticalSizer->Add( box, 0, wxGROW | wxALL, BORDER_SIZE );
	pVerticalSizer->AddSpacer( 20 );

	// peak value
	wxBoxSizer* box2 = new wxStaticBoxSizer(wxVERTICAL, this, wxT("Peak Values Calculation"));	
	page = new kome::plugin::SettingsPage( m_params );
	page->setName( "peak search" );

	// peak search
	val = m_params.addValue();
	val->setType( kome::plugin::SettingsValue::TYPE_STRING );
	val->setEnumKey( PEAK_SEARCH_PLUGIN_TYPE );
	val->setEnumType( kome::plugin::SettingsValue::ENUM_PLUGIN );
	val->setDefaultValue("chromatogram");
	val->setRequired( true );

	param = m_params.addParam();
	param->setName( SEARCH_PARAM_NAME );
	param->setValue( val );
	param->setSection( m_section.c_str() );
	param->setKey( SEARCH_PARAM_KEY );
	
	form = page->addForm();
	form->setParam( param );
	form->setTitle( "&Peak Search" );
	form->setDesc( "Select peak values calculation method." );

	m_valPanel = new kome::window::SettingsPanel( this, page, false, &m_settings );
	box2->Add( m_valPanel, 0, wxALIGN_CENTRE_VERTICAL | wxALL, BORDER_SIZE  );

	wxBoxSizer* iconSizer2 = new wxBoxSizer( wxHORIZONTAL );
	
	wxBitmap addValBmp2( *iconMgr.getIcon( "peak_val" ) );
	m_peakPosValBmp2 = new wxStaticBitmap( this, wxID_ANY, addValBmp2 );
	
	iconSizer2->Add( m_peakPosValBmp2, 0, wxALIGN_CENTRE_VERTICAL | wxALL, BORDER_SIZE );
	box2->Add( iconSizer2, 0, wxGROW | wxALL, BORDER_SIZE );

	pVerticalSizer->Add( box2, 1, wxGROW | wxALL, BORDER_SIZE ); 
		
	// area peak
	page = new kome::plugin::SettingsPage( m_params );
	page->setName( "area peak" );
	// @date 2013/08/28 <Add> FUJITA -->
	// セルの値が0である場合に、補完するか、補完するならどの値で補完するかを選択するドロップダウンリスト追加
	kome::plugin::SettingsValue* Supplementing_val = m_params.addValue();
	Supplementing_val->setType( kome::plugin::SettingsValue::TYPE_STRING );
	Supplementing_val->addEnumItem( "(None)", "(None)", "(none)" );   // 選択肢(1)　空欄にする
	Supplementing_val->addEnumItem( "0", "0", "0" );                  // 選択肢(2)　0を入れる
	Supplementing_val->addEnumItem( "Area", "Area", "area" );         // 選択肢(3)　適切な面積を計算する
	Supplementing_val->setRequired( true );
	Supplementing_val->setDefaultValue( "(None)" );

	kome::plugin::SettingsParam* Supplementing_param = m_params.addParam();
	Supplementing_param->setName( "supplement" );                     // 本コントロールの名前。
	Supplementing_param->setValue( Supplementing_val );               // 上記で設定した選択肢名などを、本コントロールに設定する。
	Supplementing_param->setSection( "Supplementing Cell Value" );
	Supplementing_param->setKey( "Supplementing Cell Value" );

	kome::plugin::SettingsForm* Supplementing_form = page->addForm(); // 本panel上の全コントロールを示す"page"に、一つコントロールを追加する。
	Supplementing_form->setParam( Supplementing_param );              // 本コントロールをpage上に追加
	Supplementing_form->setTitle( "Supplementing Cell Value Method" );
	Supplementing_form->setDesc( "Select a supplementing-cell-value method to fill cells which can not be calculated peak value.\n" 
		"If \"(None)\" is set, the cells are filled with blank.\n"   // 本コントロールの説明
		"If \"0\" is set, the cells are filled with 0.\n"
		"If \"Area\" is set, the cells are filled with spectrum area within m/z or RT tolerance." );
 	// @date 2013/08/28 <Add> FUJITA <--

	// 新規作成時、表示しない
	if( m_bEdit ){
		// initialize position
		val = m_params.addValue();
		val->setType( kome::plugin::SettingsValue::TYPE_BOOL );
		val->setDefaultValue( "false" );
		val->setRequired( true );

		param = m_params.addParam();
		param->setName( INIT_FLG_PARAM_NAME );
		param->setValue( val );
		param->setSection( m_section.c_str() );
		param->setKey( INIT_FLG_PARAM_KEY );
	
		form = page->addForm();
		form->setParam( param );
		form->setTitle( "&Initialize Positions" );
		form->setDesc( "If you want to initialize peak rows, check this form." );

		// init flag
		val = m_params.addValue();
		val->setType( kome::plugin::SettingsValue::TYPE_BOOL );
		val->setDefaultValue( "false" );
		val->setRequired( true );

		param = m_params.addParam();
		param->setName( INIT_FLAG_PARAM_NAME );
		param->setValue( val );
		param->setSection( m_section.c_str() );
		param->setKey( INIT_FLAG_PARAM_KEY );
	
		form = page->addForm();
		form->setParam( param );
		form->setTitle( "&Initialize Peak Values" );
		form->setDesc( "If you want to initialize peak values, check this form." );
	}
		 
	setPeakValStatus(); // ビットマップの切り替え

	kome::window::SettingsPanel* pane = new kome::window::SettingsPanel( this, page, false, &m_settings );
	pVerticalSizer->Add( pane, 1, wxGROW | wxALL, BORDER_SIZE );

	return pVerticalSizer;
}

// transer data from window
bool PeakDetectionHelpPanel::TransferDataFromWindow(){
	// default
	if( !kome::window::ParametersAndHelpPanel::TransferDataFromWindow() ) {
		return false;
	}
	
	return true;
}

// update settings
void PeakDetectionHelpPanel::updateSettings(){
	
	// First of all, update the section for all parameters.
	for (unsigned int i = 0; i < m_params.getNumberOfParams(); i++)
	{
		kome::plugin::SettingsParam* param = m_params.getParam(i);
		param->setSection(m_section.c_str());
	}

	// Refresh the parameters for all pages - this is necessary so that
	// the values are updated in the UI when a different matrix is selected.
	for (unsigned int j = 0; j < m_params.getNumberOfPages(); j++)
	{
		kome::plugin::SettingsPage* page = m_params.getPage(j);
		page->setParameters(m_settings);
	}
	
	// Load values from the INI file
	// Update the settings in the UI
	TransferDataToWindow();
}

// get peak pos
const char* PeakDetectionHelpPanel::getPeakPos(){
	return m_settings.getParameterValue( PEAK_POSITION_PARAM_NAME );
}

// get peak pos settings
kome::objects::SettingParameterValues* PeakDetectionHelpPanel::getPeakPosSettings( const char* paramName, const char* peakPos ){
	return m_settings.getSubParameterValues( paramName, peakPos );
}

// get flag param name
bool PeakDetectionHelpPanel::getFlgParamName(){
	return m_settings.getBoolValue( INIT_FLG_PARAM_NAME, false );
}

bool PeakDetectionHelpPanel::getPeakValParamName(){
	return m_settings.getBoolValue( INIT_FLAG_PARAM_NAME, false );
}

// get search
const char* PeakDetectionHelpPanel::getSearch(){
	return m_settings.getParameterValue( SEARCH_PARAM_NAME );
}

// get m_settings 
kome::objects::SettingParameterValues PeakDetectionHelpPanel::getSettings(){ // @date 2013/08/28 <Add> FUJITA
	return m_settings;
}

// get search settings
kome::objects::SettingParameterValues* PeakDetectionHelpPanel::getSearchSettings( const char* search ){
	return m_settings.getSubParameterValues( SEARCH_PARAM_NAME, search );
}

// get tol
double PeakDetectionHelpPanel::getTol( const char* paramName, double def ){
	return m_settings.getDoubleValue( paramName, def );
}

// set peak value status
void PeakDetectionHelpPanel::setPeakValStatus(){
	// getting peak position method
	std::string peakPos = getPeakPos();
		
	if( !peakPos.empty() ){
		if( peakPos.compare( "spec_peaks" ) == 0 || peakPos.compare( "ab3dpos" ) == 0 ||
			peakPos.compare( "mrm" ) == 0 )
		{
			m_bpeakVal = true;
		}else{
			m_bpeakVal = false;
		}
	}else{
		m_bpeakVal = false;
	}
		
	// icon manager object
	kome::window::IconManager& iconMgr = kome::window::IconManager::getInstance();

	if( m_bpeakVal ){
		wxBitmap addValBmp( *iconMgr.getIcon( "peak_val_disable" ) );
		m_peakPosValBmp->SetBitmap( addValBmp );
		m_peakPosValBmp2->SetBitmap( addValBmp );
	}else{
		wxBitmap addValBmp( *iconMgr.getIcon( "peak_val" ) );
		m_peakPosValBmp->SetBitmap( addValBmp );
		m_peakPosValBmp2->SetBitmap( addValBmp );
	}

	if( m_valPanel != NULL ){
		wxWindow* win = dynamic_cast< wxWindow* >( m_valPanel );
		if( win != NULL ){
			win->Enable( !m_bpeakVal );
		}
	}
}

// get peak value status
bool PeakDetectionHelpPanel::getPeakValStatus(){
	return	m_bpeakVal;
}

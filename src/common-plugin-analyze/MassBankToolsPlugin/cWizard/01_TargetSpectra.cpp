/**
 * @file 01_TargetSpectra
 * @brief MassBank's create records
 *
 * @author M.Fukuda
 * @date 2013.02.19
 *
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 *
 */

#include "rel_stdafx.h"
#include "01_TargetSpectra.h"

// Element Box
#include "RecordElements.h"

// Define String
#include "WizardConstHeaders.h"

// Error Check
#include "ErrorCheck.h"

// AuxialTools
#include "Panels\CreatePanelTool.h"
#include "Panels\AuxiliaryToolOfCreatingPanel.h"
#include "Panels\ExtendSettingsPanel.h"
#include "Panels\PanelFactory.h"
#include "Panels\templ.hpp"

// W01 Unique Elements
#include "ext\SpecElmSetter.h"
//#include "Panels\SampleCheckList.h"
#include "Panels\SpectrumCheckGridList.h"

using namespace kome::massbank::wizard;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


#define FILTER_ON_BUTTON   9330
#define FILTER_OFF_BUTTON   9331

//-----------------------------------------------
//  Wizard Element Page
//-----------------------------------------------
BEGIN_EVENT_TABLE(TargetSpectraWizardPage, kome::window::StandardWizardPage)
END_EVENT_TABLE()

// constructor
TargetSpectraWizardPage::TargetSpectraWizardPage(
	wxWizard* wparent, 
	RecordElements*& rcElem
) : kome::window::StandardWizardPage(wparent)
, m_rcElem(rcElem)
{
	m_bitmap = getWizImg(MTR::W01::IMG);
	createControls();
}

// destructor
TargetSpectraWizardPage::~TargetSpectraWizardPage(){}

// create main sizer
wxSizer* TargetSpectraWizardPage::createMainSizer() 
{
	// sizer
	wxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	m_panel1 = new TargetSpectraPanel(this, m_rcElem, m_settings);
	sizer->Add(m_panel1, 1, wxALL|wxGROW|wxALIGN_CENTER, BORDER_SIZE);
	return sizer;
}

//-----------------------------------------------
//  Form ParametersAndHelp
//-----------------------------------------------
BEGIN_EVENT_TABLE(TargetSpectraPanel, kome::window::ParametersAndHelpPanel)
END_EVENT_TABLE()

// constructor
TargetSpectraPanel::TargetSpectraPanel(
	wxWindow* parent, 
	RecordElements*& rcElem,
	kome::objects::SettingParameterValues& inSettings
	) : kome::window::ParametersAndHelpPanel(parent)
	, m_param(NULL,false)
	, inSettings_(inSettings)
	, m_rcElem(rcElem)
{
	createControls();
}
				
// destructor
TargetSpectraPanel::~TargetSpectraPanel(){}

// createMainSizer
wxSizer* TargetSpectraPanel::createMainSizer() 
{
	using namespace kome::plugin;
	// create top sizer
	wxFlexGridSizer* sizer = pfc::createHeaders(this, 1, MTR::W01::TITLE);
	sizer->AddGrowableRow(2);

	// param access
	ParamIniAcc pia(kMassBank, kSctSmpNo);
	int oldidx = pia.getParam<int>();

	// Radio Box(Selector)
	m_book = new kome::window::RadioBoxBook(this);
	// All OpendSamplePanel
	createAllOpenedPanel();
	// DisplayedPanel
	createDisplayedPanel();
	
	sizer->Add(m_book, 1, wxALL|wxGROW);
	m_book->SetSelection(oldidx);

	return sizer;
}

// Open Smaple のパネルを作成
void TargetSpectraPanel::createAllOpenedPanel(
){
	// AllSamplePanel
	allPane_ = new AllSamplePanel(m_book, inSettings_);
	// Set Connection
	sclpA_ = allPane_->getSpecCheckListPanelPointer();
	m_book->AddPage(allPane_, wxT(LBL::W01::ALLSAMPLE));
}

// Display Panel のパネルを作成
void TargetSpectraPanel::createDisplayedPanel(
){
//	using namespace kome::plugin;

	sclpD_ = new pane::SpecCheckListPanel(m_book, HLP::WIZ::SPECLIST, true);
	std::vector<kome::objects::Spectrum*> specarr;
	elm::SpectrumElementSetter ses;
	ses.getDisPlayedSpectra(specarr);
	sclpD_->setBaseSpectra(specarr);

	m_book->AddPage(sclpD_, wxT(LBL::W01::DISPLAYED));
}

// Transfer
bool TargetSpectraPanel::TransferDataFromWindow()
{
	// default
	if( !kome::window::ParametersAndHelpPanel::TransferDataFromWindow()) {
		return false;
	}

	std::vector<kome::objects::Spectrum*> specArr;
	int bookselectId = m_book->GetSelection();

	for(unsigned int ix = 0; ix < opts_.size(); ix++) {
		delete opts_[ix];
	}
	opts_.clear();

	if(bookselectId == 1) {
		// SpectrumArrをセットする（DisplayedVersion)
		m_rcElem->setUseProcFlg(true);
		sclpD_->getSelectedSpectra(specArr,true);
		if(specArr.empty()){
			return CHK::showErrorNoSpecSelect();
		}
	} else {
		// SpectrumArrをセットする（ALl Sample Version)
		m_rcElem->setUseProcFlg(false);
		sclpA_->getSelectedSpectra(specArr,true);
		bool aveF =! m_rcElem->setAveSpectra(allPane_->getSpecArr());

		if(aveF && specArr.empty()){
			return CHK::showErrorNoSpecSelect();
		}
		std::vector<kome::objects::AveragedSpectrum*>& aves = m_rcElem->getAveSpectra();
		if(specArr.empty()) {
			parseProcessing(aves[0]);
		} else{
			// Proseeingをセット(中身のあるSpectrumが必要)
			parseProcessing(specArr[0]);
		}

		// Processingをセット
		m_rcElem->setOPT(opts_);
	}
	// Record Element にセット
	m_rcElem->setSpectra(specArr);
	
	//typenoを保存
	ParamIniAcc pia(kMassBank, kSctSmpNo);
	pia.setParam<int>(bookselectId);

	return true;
}

// get description
std::string TargetSpectraPanel::getDescription(wxWindow* wnd) 
{
	return "Check spectra to be export as spectrum records.\
[Sample Raw Data]\
 Selecting spectra contained in opened samples.\
[Displayed Data]\
 Selecting spectra from displayed spectra.";
}

void TargetSpectraPanel::parseProcessing(kome::objects::Spectrum* spec)
{
	std::string val;
	bool bflg = false;
	for( unsigned int i=0; i < inSettings_.getNumberOfParameters() && !bflg; i++ ){
		std::string strName = inSettings_.getParameterName(i);
		if( strName.compare(LBL::W01::PROCCED) == 0 ){
		//if( strName.compare( "data_manipulate" ) == 0 ){
			val = inSettings_.getParameterValue( strName.c_str() );
			bflg = true;
		}
	}

	// manager
	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();
	std::vector< std::string > arry;
	kome::plugin::SettingsValue::separateListValue( val.c_str(), arry );

	for( unsigned int j=0; j < arry.size(); j++ ){
		std::string name = ( arry[j].c_str() );
		int ifind = name.find_first_of( "[" );
		std::string strItem = name.substr( 0, ifind );
		std::string strParam = name.substr( ifind );

		kome::plugin::PluginFunctionItem* item = plgMgr.getFunctionItem( "spec_manipulation", strItem.c_str() );
		if( item != NULL ){
			kome::objects::SettingParameterValues manipulatSetting;
			kome::plugin::SettingsPage* page = ( item == NULL ? NULL : item->getSettingsPage() );
			if( page != NULL ){
				page->setParameterString( manipulatSetting, strParam.c_str());
				kome::objects::Parameters params;
				kome::plugin::PluginCallTool::setSettingValues(params, manipulatSetting);
				kome::plugin::PluginCallTool::setSpectrum( params, *spec );
				kome::objects::XYDataOperation* opt = NULL;
				opt = (kome::objects::XYDataOperation*)item->getCall()->invoke( &params ).prim.pt;
				if( opt != NULL ){
					opts_.push_back(opt);
				}
			}
		}
	}
}

// SampleListに表示するコンボボックス属性の作成
void getSpecComboElm(
	std::unique_ptr<elm::SpectrumElementSetter>& ses,
	pfc::ComboElm& cbm
){
	std::vector<std::pair<std::string,kome::objects::Sample*>> splArr;
	ses->getOpendAllSamples(splArr);
	for each(auto& p in splArr) {
		cbm.setVal(p.first, p.first, "");
	}
}

// MsTypeのコンボボックス属性の作成
void getSpecTypeComboElm(pfc::ComboElm& cbm) {
	cbm.setVal("All","all","all");
	cbm.setVal("MS","ms","ms");
	cbm.setVal("MS/MS","ms2","ms2");
}

BEGIN_EVENT_TABLE(AllSamplePanel, kome::window::HelpGetterPanel)
	EVT_KOME_SETTINGS_PANEL(wxID_ANY, AllSamplePanel::onSettingsPanel )
	EVT_UPDATE_UI(wxID_ANY, AllSamplePanel::onUPUI)
	EVT_BUTTON(FILTER_ON_BUTTON,  AllSamplePanel::onFilterONButton)
	EVT_BUTTON(FILTER_OFF_BUTTON, AllSamplePanel::onFilterOFFButton)
END_EVENT_TABLE()

// constructor
AllSamplePanel::AllSamplePanel(
	wxWindow* parent,
	kome::objects::SettingParameterValues& settings
) : kome::window::HelpGetterPanel(parent)
	, inSettings_(settings)
	, inParams_(NULL,false)
	, parent_(parent)
	, sesPtr_(new elm::SpectrumElementSetter())
	, cgfg_(false)
{
	createControls();
}

// destructor
AllSamplePanel::~AllSamplePanel() {}

// create main sizer
wxSizer* AllSamplePanel::createMainSizer() 
{
	// sizer
	wxFlexGridSizer* sizer = new wxFlexGridSizer(4, 1, GRID_GAP, GRID_GAP);

	// Set up ComboBox Elements
	pfc::ComboElm cbm, cbm2;
	getSpecComboElm(sesPtr_,cbm);
	getSpecTypeComboElm(cbm2);

	// Add Sample CheckBox Panel
	pfc::CreatePanelTool pane1(inParams_);
	pane1.setCheckListBox("Sample", cbm);
	pane1.makeThis(LBL::W01::SAMPLELIST, HLP::WIZ::SAMPLELIST, false);
	panelEx_ = pane1.generateExtendPanel(this, inSettings_);
	sizer->Add(panelEx_, 1, wxALL|wxGROW);

	// Add Spectrum Panel
	sclpA_ = new pane::SpecCheckListPanel(this, HLP::WIZ::SPECLIST, false);
	sizer->Add(sclpA_, 1, wxALL | wxGROW );
	
	// Add Filter Buttons
	sizer->Add(createFilterButtons(),1, wxALL);

	// Add Average Spectrum List
	pfc::CreatePanelTool pane3(inParams_);
	pane3.setListBox();
	pane3.setListElmCombo(cbm, "Sample");
	pane3.setListElmDouble("Start_RT");
	pane3.setListElmDouble("END_RT");
	pane3.setListElmCombo(cbm2, "Types");
	pane3.makeThis("SCAVE",LBL::W01::AVERAGE, HLP::WIZ::AVERAGELIST, true);
	kome::window::SettingsPanel* panel3 = pane3.generateSettingPanel(this, inSettings_);
	sizer->Add(panel3, 1, wxALL|wxGROW);

	// Add Procceds.

	//kome::plugin::SettingsPage* page = new kome::plugin::SettingsPage(inParams_);
	//// Data Manipulation
	//kome::plugin::SettingsValue* val = inParams_.addValue();
	//val->setType( kome::plugin::SettingsValue::TYPE_LIST );
	//val->setRequired( false );

	//kome::plugin::SettingsValue* childVal0 = inParams_.addValue();
	//childVal0->setType( kome::plugin::SettingsValue::TYPE_STRING );
	//childVal0->setEnumType( kome::plugin::SettingsValue::ENUM_PLUGIN );
	//childVal0->setEnumKey( "spec_manipulation" );
	//childVal0->setRequired( true );
	//childVal0->setName( "Data Manipulation" );
	//
	//val->addChildValue( childVal0 );

	//kome::plugin::SettingsParam* param = inParams_.addParam();
	//param->setName( "data_manipulate" );
	//param->setSection( kMassBank.c_str() );
	//param->setKey( "DATA_MANIPULATE" );
	//param->setValue( val );

	//kome::plugin::SettingsForm* form = page->addForm();
	//form->setTitle("Data Manipulation");
	//form->setParam( param );
	//form->setDesc( "Set basic data manipulation methods before making peak list." ); // @date 2014.01.23 <Add> M.Izumi

	//kome::window::SettingsPanel* panel = new kome::window::SettingsPanel( this, page, false, &inSettings_, true );
	//sizer->Add(panel, 1, wxALL|wxGROW);

// 時間が有ったら下記の書式に直す
//
	pfc::CreatePanelTool auxi4(inParams_);
	auxi4.setEnumListBox("Data Manipulation","spec_manipulation", true, "");
	auxi4.makeThis(LBL::W01::PROCCED, HLP::WIZ::PROCESSINGLIST, true);
	ExtendSettingsPanel* panel4 = auxi4.generateExtendPanel(this, inSettings_);
//	kome::window::SettingsPanel* panel4 = auxi4.generateSettingPanel(this, inSettings_);
	sizer->Add(panel4, 1, wxALL|wxGROW);

	return sizer;
}

// Filterのボタンを追加
wxGridSizer* AllSamplePanel::createFilterButtons(
){
	filterOffButton_ = new wxButton(this, FILTER_OFF_BUTTON, wxT("Clear Filter"));
	filterOnButton_ = new wxButton(this, FILTER_ON_BUTTON, wxT("Filter..."));
	filterOffButton_->Enable(false);
	filterOnButton_->Enable(false);
	
	wxGridSizer* btsizer = new wxGridSizer(1, 2, 3, 3);
	btsizer->Add(filterOffButton_);
	btsizer->Add(filterOnButton_);
	return btsizer;
}

// Transfer
bool AllSamplePanel::TransferDataFromWindow(
){
	if( !kome::window::HelpGetterPanel::TransferDataFromWindow()) {
		return false;
	}

	sclpA_->setSelectedElement();
	// Averaged Spectrumの分割
	bool ret = onSetAverageSpectrum();
	return ret;
}

const bool AllSamplePanel::onSetAverageSpectrum()
{
	std::string aveStr = inSettings_.getParameterValue("SCAVE");
	return spareteAveregeSpectrumElement(aveStr);
}

// Filter 実行ボタンクリック
void AllSamplePanel::onFilterONButton(
	wxCommandEvent& evt
){
	// 仮想フィルタ
	sclpA_->setSelectedElement();
	std::vector<kome::objects::Spectrum*> src, dst;
	kome::objects::DataSet srcds, dstds;

	if(onGetCheckedSamplesAllSpectra(src)) {
		for each(auto p in src) {
			srcds.addSpectrum(p);
		}
	}

	kome::objects::Parameters param;
	kome::plugin::PluginCallTool::setDataSet(param, srcds);
	kome::plugin::PluginCallTool::setSpectraFilter(param, dstds);
	kome::plugin::PluginCallTool::setBool( param, "RETURN_VALUE", false );

	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();
	plgMgr.callAllFunctions( &param, "FILTER_SPECTRA");

	if (kome::plugin::PluginCallTool::getBool(&param, "RETURN_VALUE", false)) {
		unsigned int pn = dstds.getNumberOfSpectra();

		for(unsigned int n = 0; n < pn; n++) {
			dst.push_back(dstds.getSpectrum(n));
		}
		sclpA_->setShowSpectra(dst);

		filterOffButton_->Enable(true);
	}
}

// Filter 解除ボタンクリック
void AllSamplePanel::onFilterOFFButton(
	wxCommandEvent& evt
){
	// チェック済みのものをバックアップ
	std::vector<kome::objects::Spectrum*> src;
	if(onGetCheckedSamplesAllSpectra(src)) {
		sclpA_->setSelectedElement();
		sclpA_->setShowSpectra(src);
	}
	filterOffButton_->Enable(false);
}

// 表示を更新時
void AllSamplePanel::onUPUI(
	wxUpdateUIEvent& evt
){
	if(cgfg_) {
		// SampleListに変更があった
		updateSpecList();
		cgfg_=false;
	}
}

// update Spectrum List
void AllSamplePanel::updateSpecList()
{
	std::vector<kome::objects::Spectrum*> specarr;
	if(onGetCheckedSamplesAllSpectra(specarr)) {
		filterOnButton_->Enable(true);
	} else {
		filterOnButton_->Enable(false);
	}
	filterOffButton_->Enable(false);
	sclpA_->setBaseSpectra(specarr);
}

const bool AllSamplePanel::onGetCheckedSamplesAllSpectra(
	std::vector<kome::objects::Spectrum*>& ref
){
	panelEx_->Update();
	std::string samplelists = panelEx_->getValue(LBL::W01::SAMPLELIST);
	return sesPtr_->getSamplesSpectra(samplelists, ref);
}

// get Panel Events
void AllSamplePanel::onSettingsPanel(
	kome::window::SettingsPanelEvent& evt
) {
	kome::plugin::SettingsForm* form = evt.getForm();
	if(form == NULL) {
		return;
	}
	std::string title = form->getTitle();
	if(title.compare(LBL::W01::SAMPLELIST) == 0) {
		cgfg_ = true;
	}
}

// Average List Ctrl を分割する
const bool AllSamplePanel::spareteAveregeSpectrumElement(
	const std::string& src
){
	std::string errmsg;
	std::vector<kome::objects::Spectrum*> rsts;
	rstArr_.clear();

	std::vector<std::string> arr;
	kome::plugin::SettingsValue::separateListValue(src.c_str(), arr);
	for each(auto& s in arr) {
		rsts.clear();
		if(sesPtr_->getAverageSpecElemements(s, errmsg, rsts)){
			rstArr_.push_back(rsts);
		} else {
			// AverageSpecに不正があった
			kome::window::WindowTool::showInfo(errmsg.c_str(), "Input Error");
			return false;
		}
	}	
	return true;
}

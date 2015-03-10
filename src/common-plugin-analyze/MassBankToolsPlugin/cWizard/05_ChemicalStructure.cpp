/**
 * @file 05_..
 * @brief MassBank's create records
 *
 * @author M.Fukuda
 * @date 2013.02.20
 *
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#include "rel_stdafx.h"
#include "05_ChemicalStructure.h"

using namespace kome::massbank::wizard;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


// Element Box
#include "RecordElements.h"

// Panel Support Headers
#include "Panels\CreatePanelTool.h"
#include "Panels\AuxiliaryToolOfCreatingPanel.h"
#include "Panels\ExtendSettingsPanel.h"
#include "Panels\templ.hpp"
#include "ExtDB\LinkDBAccessClass.h"

#include "ErrorCheck.h"

#include "WizardConstHeaders.h"

// suport functions
#include "..\RDKit\RDKitAccesser.h"

// Define String
const std::string kCategory("Category");
const std::string kProduct("Product");
const std::string kClassName("Class Name");

//## Extend ##
const std::string kBtnSrch("Search...");
const std::string kBtnMol("Import from mol file...");
const std::string kBtnDB("Import from external DB...");

#define MOLFILEBUTTON   9390
#define MOLLINKBUTTON   9391
#define MOLSEARCHBUTTON 9392

//-----------------------------------------------
//  Wizard Element Page
//-----------------------------------------------
//#include "images\massbank_flow05.xpm"
BEGIN_EVENT_TABLE(ChemicalStructureWizardPage, kome::window::StandardWizardPage)
END_EVENT_TABLE()

// constructor
ChemicalStructureWizardPage::ChemicalStructureWizardPage(
	wxWizard* wparent, 
	RecordElements*& rcElem
) : kome::window::StandardWizardPage(wparent)
	, m_rcElem(rcElem)
{	
	m_bitmap = getWizImg(MTR::W05::IMG);
	createControls();
}

// create main sizer
wxSizer* ChemicalStructureWizardPage::createMainSizer()
{
	// sizer
	wxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	ChemicalStructurePanel* panel1 = new ChemicalStructurePanel(this, m_rcElem, inSettings_);
	sizer->Add(panel1, 1, wxALL|wxGROW|wxALIGN_CENTER, BORDER_SIZE);
	return sizer;
}

//-----------------------------------------------
//  inner panel code
//-----------------------------------------------
BEGIN_EVENT_TABLE(ChemicalStructurePanel, kome::window::ParametersAndHelpPanel)
	EVT_BUTTON(MOLFILEBUTTON,  ChemicalStructurePanel::onFileButtonClick)
	EVT_BUTTON(MOLLINKBUTTON,  ChemicalStructurePanel::onLinkButtonClick)
	EVT_BUTTON(MOLSEARCHBUTTON,ChemicalStructurePanel::onSearchButtonClick)
    EVT_KOME_SETTINGS_PANEL(wxID_ANY, ChemicalStructurePanel::onSettingsPanel )
END_EVENT_TABLE()

// constructor
ChemicalStructurePanel::ChemicalStructurePanel(
	wxWindow* parent, 
	RecordElements*& rcElem, 
	kome::objects::SettingParameterValues& inSettings)
	: kome::window::ParametersAndHelpPanel(parent)
	, inParams_(NULL, false)
	, inSettings_(inSettings)
	, m_rcElem(rcElem)
	, dbsPtr_(new DBAccClass)
	, fileButton_(nullptr)
	, linkButton_(nullptr)
	, searchButton_(nullptr)
	, namePanel_(nullptr)
	, dbLinkPanel_(nullptr)
	, chemicalPanel_(nullptr)
	, linkUniqueChkFg_(false)
{
	createControls();
}

// destructor
ChemicalStructurePanel::~ChemicalStructurePanel(){}


// MsTypeのコンボボックス属性の作成
void getProductElm(pfc::ComboElm& cbm) {
	cbm.setVal(kUnknown);
	cbm.setVal(kNaturalProduct);
	cbm.setVal(kNonNaturalProduct);
}


// create sizer (virtual)
wxSizer* ChemicalStructurePanel::createMainSizer() 
{
	// sizer
	wxFlexGridSizer* sizer = pfc::createHeaders(this, 4, MTR::W05::TITLE);

	// combobox Element
	pfc::ComboElm prcCbm, exdbCbm;
	getProductElm(prcCbm);
	
	unsigned int dbnum = dbsPtr_->getMyNum();
	for(unsigned int idx=0; idx < dbnum; idx++) {
		exdbCbm.setVal(dbsPtr_->getNames(idx), dbsPtr_->getsURL(idx));
	}

	// Add Product Panel(Product & Class)
	pfc::CreatePanelTool auxi1(inParams_);
	auxi1.setGroup(kCategory);
	auxi1.setComboBox(prcCbm, "");
	auxi1.makeThis(kProduct, HLP::WIZ::N_PRODUCT, true);
	auxi1.makeStr(kClassName, HLP::WIZ::CLASSNAME, false,"",true);

	// CH Name List --------------------------
	pfc::CreatePanelTool auxi2(inParams_);
	auxi2.setListBox();
	auxi2.setListElmString(kName,true);
	auxi2.makeThis(kName, HLP::WIZ::CH_NAME, true);

	// DB Link List --------------------------
	pfc::CreatePanelTool auxi3(inParams_);
	auxi3.setListBox();
	auxi3.setListElmCombo(exdbCbm, kDB, true);
	auxi3.setListElmString(kAccession, true);
	auxi3.makeThis(kLink, HLP::WIZ::EX_LINK, true);

	// Chemical Area
	pfc::CreatePanelTool auxi4(inParams_);
	auxi4.setGroup(kChmStr);
	auxi4.makeStr(kFormula,   HLP::WIZ::FORMULA,  false, "", true);
	auxi4.makeStr(kExactMass, HLP::WIZ::EXACTMASS,false, "", true);
	auxi4.makeStr(kSMILES,    HLP::WIZ::SMILES,   false, "", true);
	auxi4.makeStr(kInchKey,   HLP::WIZ::INCHIKEY, false, "", true);

	// Panelの生成
	kome::window::SettingsPanel* panel1 = auxi1.generateSettingPanel(this, inSettings_);
	namePanel_ = auxi2.generateExtendPanel(this, inSettings_);
	dbLinkPanel_ = auxi3.generateExtendPanel(this, inSettings_);
	chemicalPanel_ = auxi4.generateExtendPanel(this, inSettings_);

	// Create Chemic Buttons
	wxGridSizer* buttonsSizer = createButtons();

	// Sizerに適合
	sizer->Add(panel1, 1, wxALL|wxGROW, BORDER_SIZE);
	sizer->Add(namePanel_, 1, wxALL|wxGROW, 0);
	sizer->Add(dbLinkPanel_, 1, wxALL|wxGROW, 0);
	sizer->Add(chemicalPanel_, 1, wxALL|wxGROW, BORDER_SIZE);
	sizer->Add(buttonsSizer, 1, wxALL|wxGROW, 0);

	return sizer;
}

// Buttonを作成する
wxGridSizer* ChemicalStructurePanel::createButtons()
{
	// button
	fileButton_ = new wxButton(this, MOLFILEBUTTON, wxT(kBtnMol.c_str()));
	linkButton_ = new wxButton(this, MOLLINKBUTTON, wxT(kBtnDB.c_str()));
	searchButton_ = new wxButton(this, MOLSEARCHBUTTON, wxT(kBtnSrch.c_str()));

	// judge Name Status	
	searchButton_->Enable(true);

	// judge Link Status
	ParamIniAcc pialink(kMassBank, kLink);
	std::string tmp = pialink.getParam<std::string>();
	if (tmp.empty() || tmp.compare("none") == 0) {
		linkButton_->Enable(false);
	} else {
		linkButton_->Enable(true);
	}

	wxGridSizer* buttonsSizer = new wxGridSizer(1, 3, 3, 3);

	buttonsSizer->Add(fileButton_, 1, wxALL| wxGROW, BORDER_SIZE);
	buttonsSizer->Add(linkButton_, 1, wxALL| wxGROW, BORDER_SIZE);
	buttonsSizer->Add(searchButton_, 1, wxALL| wxGROW, BORDER_SIZE);
	return buttonsSizer;
}

// on Click File Button
void ChemicalStructurePanel::onFileButtonClick(wxCommandEvent& evt) 
{
	setChemicalStructureInfo(false);
}

// on Click Link Button
void ChemicalStructurePanel::onLinkButtonClick(wxCommandEvent& evt) 
{
	// get LinkDBArr from Panels
	std::string linkstr = dbLinkPanel_->getValue(kLink);

	// get URL from combobox Selecter
	std::string URL = dbsPtr_->setListArrStrs(linkstr);
	setChemicalStructureInfo(true, URL);
}

const bool updateExp(
	ExtendSettingsPanel* p, const std::string& key, const std::string& newval)
{
	bool ret = false;
	std::string val = p->getValue(key);
	if(!val.empty()) {
		val = val + "," + newval;
		ret = true;
	} else {
		val = newval;
	}
	p->setValue(key, val);
	p->Update();
	return ret;
}

// Search Button Click時
void ChemicalStructurePanel::onSearchButtonClick(
	wxCommandEvent& evt
) {
	std::string shnameval(""),shlinksval(""),shURL("");

	if(dbsPtr_->getKeyWordSearchResults(shnameval, shlinksval, shURL)) {

		bool replaceChemFg = checkAlreadyUsedChemicals(); 
		bool multiFg = false;
		multiFg  |= updateExp(namePanel_, kName, shnameval);
		multiFg  |= updateExp(dbLinkPanel_, kLink, shlinksval);

		if (!multiFg) {
			setChemicalStructureInfo(true, shURL);
		} else {
			if (!replaceChemFg) {
				wxMessageDialog dlg(NULL, 
					wxT("Substitute Chemical Structure Informations?"),
					wxT("Informations"),
					wxNO_DEFAULT|wxYES_NO|wxICON_QUESTION);
				if(dlg.ShowModal() == wxID_YES) {
					setChemicalStructureInfo(true, shURL);
				}
			}
		}		
		onCheckDBLink();
	}
}

// Chemical Structureに情報をセットする
const bool ChemicalStructurePanel::setChemicalStructureInfo(
	const bool useExtDB,
	const std::string URL
){
	std::string mass, formula, smiles, inchi;
	bool ret(false);
	if (useExtDB) {
		if(!URL.empty()) {
			ret = mol::getMolDataFromURL(URL, mass, formula, smiles, inchi);
		}
	} else {
		ret = mol::getMolDataFromFileSelect(mass, formula, smiles, inchi);
	}

	if(ret) {
		//取得できたので値をセットする
		chemicalPanel_->setValue(kExactMass, mass);
		chemicalPanel_->setValue(kFormula,formula);
		chemicalPanel_->setValue(kSMILES,smiles);
		chemicalPanel_->setValue(kInchKey,inchi);
	}
	return ret;
}

//　構造式関連のTextBoxが空であれば　True　を返す
const bool ChemicalStructurePanel::checkAlreadyUsedChemicals()
{
	ExtendSettingsPanel* p = chemicalPanel_;
	bool ret = p->getValue(kExactMass).empty() 
				&& p->getValue(kFormula).empty() 
				&& p->getValue(kSMILES).empty()
				&& p->getValue(kInchKey).empty();
	return ret;
}

// get Panel Events
void ChemicalStructurePanel::onSettingsPanel(kome::window::SettingsPanelEvent& evt) 
{
	kome::plugin::SettingsForm* form = evt.getForm();
	if(form == NULL) {
		return;
	}
	std::string formtitle = form->getTitle();
	// input link panel
	if (kLink.compare(formtitle) == 0) {
		onCheckDBLink();
	}

	if (kName.compare(formtitle) == 0) {
		onCheckNamePanel();
	}
}

// 名前のパネルのチェック
void ChemicalStructurePanel::onCheckNamePanel()
{
	searchButton_->Enable(true);
}

// if Link is nothing, linkButton is disabled.
void ChemicalStructurePanel::onCheckDBLink() 
{
	std::string linkstr = dbLinkPanel_->getValue(kLink);
	if (!linkstr.empty()) {
		bool ufg = !linkUniqueChkFg_;
		bool fg = onCheckDBLinkPanleStatus(linkstr, ufg);
		linkButton_->Enable(fg);
		//正常系に戻ったらユニークチェックを戻す
		linkUniqueChkFg_ = !fg;
	} else {
		linkButton_->Enable(false);
	}
}

const bool ChemicalStructurePanel::onCheckDBLinkPanleStatus(
	const std::string& linkstr, bool fg
){
	std::vector<std::string> vst;
	CHK::SplitTopParams(linkstr, vst);
	// 重複があるとTrueが返ってくるので不可とする。
	return (!CHK::isOverlap(vst, 1, fg));
}

// Trancefer From Window (virtual)
bool ChemicalStructurePanel::TransferDataFromWindow() 
{
	// default
	if(!kome::window::ParametersAndHelpPanel::TransferDataFromWindow()) {
		return false;
	}
	pfc::spvAdpt sp(inSettings_);

	std::string product   = sp.val(kProduct);
	std::string classname = sp.val(kClassName);
	std::string names     = sp.val(kName);
	std::string links     = sp.val(kLink);
	std::string formula   = sp.val(kFormula);
	std::string exactmass = sp.val(kExactMass);
	std::string smiles    = sp.val(kSMILES);
	std::string inchi     = sp.val(kInchKey);

	if(!onCheckDBLinkPanleStatus(links, true)) {
		// 重複があるから
		return false;
	}

	m_rcElem->setCHName(names);
	m_rcElem->setCategory(product, classname);
	m_rcElem->setLinks(links);
	m_rcElem->setChemcalStructures(formula, exactmass, smiles, inchi);

	return true;
}

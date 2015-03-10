/**
 * @file 02_..
 * @brief MassBank's create records
 *
 * @author M.Fukuda
 * @date 2013.02.20
 *
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#include "rel_stdafx.h"
#include "02_ExportSettings.h"

#include "WizardConstHeaders.h"

// Element Box
#include "RecordElements.h"

// Panel Support Headers
#include "Panels\AuxiliaryToolOfCreatingPanel.h"
#include "Panels\ExtendSettingsPanel.h"
//#include "Panels\PanelFactory.h"

// suport functions
#include "ErrorCheck.h"
#include "calcFunc\StrFunc.h"

#include "IdentMgrAcc.h"

// wxwidget use only.
#include "wx/statbox.h"

using namespace kome::massbank::wizard;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


//-----------------------------------------------
//  Wizard Element Page
//-----------------------------------------------
BEGIN_EVENT_TABLE(ExportSettingsWizardPage, kome::window::StandardWizardPage)
END_EVENT_TABLE()

// constructor
ExportSettingsWizardPage::ExportSettingsWizardPage(
	wxWizard* wparent, 
	RecordElements*& rcElem
	) : kome::window::StandardWizardPage(wparent)
	, m_rcElem(rcElem)
{
	m_bitmap = getWizImg(MTR::W02::IMG);
	createControls();
}

// create main sizer
wxSizer* ExportSettingsWizardPage::createMainSizer()
{
	// sizer
	wxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	ExportSettingsPanel* panel1 = new ExportSettingsPanel(this,m_rcElem, inSettings_);
	sizer->Add(panel1, 1, wxALL|wxGROW|wxALIGN_CENTER, BORDER_SIZE);
	return sizer;
}

//-----------------------------------------------
//  inner panel code
//-----------------------------------------------
BEGIN_EVENT_TABLE(ExportSettingsPanel, kome::window::ParametersAndHelpPanel)
    EVT_KOME_SETTINGS_PANEL(wxID_ANY, ExportSettingsPanel::onSettingsPanel )
END_EVENT_TABLE()

// constrctor
ExportSettingsPanel::ExportSettingsPanel(
	wxWindow* parent, 
	RecordElements*& rcElem,
	kome::objects::SettingParameterValues& inSettings)
	: kome::window::ParametersAndHelpPanel(parent)
	, inParams_(NULL,false)
	, m_settings(inSettings)
	, m_rcElem(rcElem)
{
	createControls();

	// createControlで実体が作られないと有効にできない
	std::string preval = m_settings.getParameterValue(kPrefix.c_str());
	std::string tmpPrefix = ext::tranceLowerToUpper(preval);
	std::string basenum = Ident::getPrefixNum_Str(tmpPrefix);
	m_settings.setValue(kBaseNum.c_str(), basenum.c_str());
}
				
// destrctor
ExportSettingsPanel::~ExportSettingsPanel(){}

// create sizer (virtual)
wxSizer* ExportSettingsPanel::createMainSizer() 
{
	// sizer
	wxFlexGridSizer* sizer = pfc::createHeaders(this, 2, MTR::W02::TITLE);

	using namespace kome::plugin;

	// page
	kome::plugin::SettingsPage* page = new kome::plugin::SettingsPage(inParams_);
	
	// Prefix Text Box
	SettingsValue* val = inParams_.addValue();
	val->setType(SettingsValue::TYPE_STRING);
	val->setRequired(true);
	pfc::setParamAndFormAdpt(inParams_, val, page, kPrefix, kPrefix, HLP::WIZ::PREFIX, true);

	// Base Num Int Box (with Spin)
	val = inParams_.addValue();
	val->setType(kome::plugin::SettingsValue::TYPE_INT);
	val->getNumberRestriction().setInclusiveMinValue(0);
	pfc::setParamAndFormAdpt(inParams_, val, page, kBaseNum, kBaseNum, HLP::WIZ::BASENUM);

	// Extend Settings Panels (For Multi Action)
	panelEx = new ExtendSettingsPanel(this, page, false, &m_settings, true);

	// make Accession Group
	wxStaticBox* stBox = new wxStaticBox(this, wxID_STATIC, wxT(kAccession.c_str()));
	wxStaticBoxSizer* stBoxSizer = new wxStaticBoxSizer(stBox, wxVERTICAL);
	//accession msg
	wxStaticText* accmsg = new wxStaticText(this, wxID_STATIC, LBL::W02::ACCETION);
	accmsg->Wrap(-1);
	stBoxSizer->Add(accmsg, 0, wxALL, BORDER_SIZE);
	stBoxSizer->Add(panelEx, 1, wxALL|wxGROW, BORDER_SIZE);

	sizer->Add(stBoxSizer, 1, wxALL|wxGROW, BORDER_SIZE);

	// File Folder ------------------------------------------------------
	val = inParams_.addValue();
	val->setType(SettingsValue::TYPE_DIR);
	val->setRequired(true);
	SettingsPage* page2 = new SettingsPage(inParams_);

	pfc::setParamAndFormAdpt(inParams_, val, page2, kFolder, kFolder, HLP::WIZ::OUT_FOLDER,true);
	kome::window::SettingsPanel* panel2 
		= new kome::window::SettingsPanel(this, page2, false, &m_settings, true);

	// Last input
	sizer->Add(panel2, 1, wxALL|wxGROW, BORDER_SIZE);
	
	return sizer;
}

// Panels Event (virtual)
void ExportSettingsPanel::onSettingsPanel(kome::window::SettingsPanelEvent& evt)
{	
	kome::plugin::SettingsForm* form = evt.getForm();
	if( form == NULL ) {
		return;
	}
	std::string formtitle = form->getTitle();
	if (kPrefix.compare(formtitle) == 0) {
		// Prefixの入力時のチェック
		std::string preVal = panelEx->getValue(kPrefix);
		if(CHK::checkInputedPrefix(preVal) && CHK::checkPrefix(preVal)) {
			// 2<=X<=3 且つ文字列を満たしている。
			// 大文字にしてDBからNumを探す。
			std::string tmpPrefix = ext::tranceLowerToUpper(preVal);
			std::string numVal = Ident::getPrefixNum_Str(tmpPrefix);
			// BaseNumに反映。
			panelEx->setValue(kBaseNum, numVal);
		} else {
		}
	} else if(kBaseNum.compare(formtitle) == 0) {
		std::string preVal = panelEx->getValue(kPrefix);
		std::string NumVal = panelEx->getValue(kBaseNum);
		// BaseNumを含めた結果、オーバーしないかを確認。
		// この後追加される要素も含む。
		CHK::checkBaseNum_s(preVal,NumVal,0);
	}
	return;
}

// Transfer (virtual)
bool ExportSettingsPanel::TransferDataFromWindow() 
{
	// default
	if( !kome::window::ParametersAndHelpPanel::TransferDataFromWindow()) {
		return false;
	}
	std::string preval = m_settings.getParameterValue(kPrefix.c_str());

	if(CHK::checkInputedPrefix(preval) && CHK::checkPrefix(preval,true)) {
		// Ok Prefix 2<= X <=3 & All Aphabets.
		int basenum = m_settings.getIntValue(kBaseNum.c_str(), -1);

		// Ver 2.6.0, Numが空欄でも対応。
		if (basenum < 0) {
			basenum = Ident::getPrefixNum(preval);
		}

		if(CHK::checkBaseNum(preval, basenum, m_rcElem->getSpecArrSize())) {
			// OK Prefix + BaseNum Length <= 8
			m_rcElem->setAccessions(preval, basenum);
			std::string folderPath = m_settings.getParameterValue(kFolder.c_str());

			if(CHK::checkSaveFolderPath(folderPath)) {
				// OK SaveDir can use.(OK Write)
				m_rcElem->setFolder(folderPath);
				return true;
			}
		}
	}
	return false;
}

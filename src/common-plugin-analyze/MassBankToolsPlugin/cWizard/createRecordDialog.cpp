/**
 * @file createRecordsWizardDialog.cpp
 * @brief MassBank's create records Dialog
 *
 * @author M.Fukuda
 * @date 2013.01.18
 *
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#include "rel_stdafx.h"
#include "createRecordDialog.h"
#include "RecordElements.h"

// Wizard Page Elements
#include "01_TargetSpectra.h"
#include "02_ExportSettings.h"
#include "03_PeakDetection.h"
#include "04_RecordSettings.h"
#include "05_ChemicalStructure.h"
#include "06_SampleInformation.h"
#include "07_Instrument.h"
#include "08_SpectrumInformation.h"
#include "09_ChromatographyInformation.h"
#include "10_PrecurosrMolecularIon.h"
#include "11_SoftwareInformation.h"
#include "12_PeakAnnotation.h"

// -- Final ---
#include "15_FinalSequence.h"

const wxString kWizImg00(wxT("\\images\\massbank_flow00.png"));

// ref_stadfx で定義してある。
wxBitmap getWizImg(const wxString& imgPath)
{
	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();
	kome::plugin::PluginInfo* info = plgMgr.getPlugin("MassBank Tools");
	const char* dd = info->getDir();
	wxString fullPath = wxT(dd) + imgPath;
	return wxBitmap(fullPath, wxBITMAP_TYPE_PNG);
}

wxBitmap getWizImg(const std::string& imgPath)
{
	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();
	kome::plugin::PluginInfo* info = plgMgr.getPlugin("MassBank Tools");
	const char* dd = info->getDir();
	wxString fullPath = wxT(dd) + wxT(imgPath);
	return wxBitmap(fullPath, wxBITMAP_TYPE_PNG);
}

using namespace kome::massbank::wizard;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG

using namespace kome::massbank;

BEGIN_EVENT_TABLE(createRecordsWizardDialog, kome::window::Wizard)
    EVT_WIZARD_FINISHED(wxID_ANY, createRecordsWizardDialog::OnWizardFinished)
END_EVENT_TABLE()

// constructor
createRecordsWizardDialog::createRecordsWizardDialog(wxWindow* parent) 
	: kome::window::Wizard(parent, "Create Wizard", getWizImg(kWizImg00))
	, inParams_(NULL,true) , rcElmPtr_(new RecordElements)
{
	bool finished = false;
	wizard::RecordElements* m_rcElem = &*rcElmPtr_;
	m_rcElem->Init();

	m_page1 = new TargetSpectraWizardPage(this, m_rcElem);
	m_page2 = new ExportSettingsWizardPage(this, m_rcElem);
	m_page3 = new PeakDetectionWizardPage(this, m_rcElem);
	m_page4 = new RecordSettingsWizardPage(this, m_rcElem);
	m_page5 = new ChemicalStructureWizardPage(this, m_rcElem);
	m_page6 = new SampleInformationWizardPage(this, m_rcElem);
	m_page7 = new InstrumentWizardPage(this, m_rcElem);
	m_page8 = new SpectrumInformationWizardPage(this, m_rcElem);
	m_page9 = new ChromatographyInformationWizardPage(this, m_rcElem);
	m_page10 = new PrecurosrMolecularIonWizardPage(this, m_rcElem);
	m_page11 = new SoftwareInformationWizardPage(this, m_rcElem);
	m_page12 = new PeakAnnotationWizardPage(this, m_rcElem);
	m_final = new FinalSequenceWizardPage(this, m_rcElem);

	wxWizardPageSimple::Chain(m_page1, m_page2);
	wxWizardPageSimple::Chain(m_page2, m_page3);
	wxWizardPageSimple::Chain(m_page3, m_page4);
	wxWizardPageSimple::Chain(m_page4, m_page5);
	wxWizardPageSimple::Chain(m_page5, m_page6);
	wxWizardPageSimple::Chain(m_page6, m_page7);
	wxWizardPageSimple::Chain(m_page7, m_page8);
	wxWizardPageSimple::Chain(m_page8, m_page9);
	wxWizardPageSimple::Chain(m_page9, m_page10);
	wxWizardPageSimple::Chain(m_page10, m_page11);
	wxWizardPageSimple::Chain(m_page11, m_page12);
	wxWizardPageSimple::Chain(m_page12, m_final);

	GetPageAreaSizer()->Add(m_page1);
}

// destructor
createRecordsWizardDialog::~createRecordsWizardDialog(){
}

wxWizardPageSimple* createRecordsWizardDialog::getFirstPage(){
	return m_page1;
}


// Finish ボタンクリック時
void createRecordsWizardDialog::OnWizardFinished(wxWizardEvent& event)
{
	std::string errmsg;
	bool ret = rcElmPtr_->makeMassBankRecords(errmsg);
	if (ret) {
		//　正常終了。
		std::string msg = FMT("Spectrum records were completed successfully.");
		kome::window::WindowTool::showInfo(msg.c_str());
		finishedFg = true;
//		return true;
	} else {
		kome::window::WindowTool::showError(errmsg.c_str());
		finishedFg = false;
//		return false;
	}
}

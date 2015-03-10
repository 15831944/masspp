/**
 * @file Call Vender&Instrument Select Combo Box
 * @brief Vender&Instrument Select Combo Box Informations
 *
 * @author M.Fukuda
 * @date 2013.01.18
 *
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#include "InstrumentAccessPanel.h"
#include "VenderInstrument.h"
#include "AuxiliaryToolOfCreatingPanel.h"
#include "../objectsName.h"

#include "../../MassBankManager.h"
#include "../../MBMgrTool/InstrumentTypesMgr.h"

// Instrument Mgrから、Instrument Typeを取得する
void getInstumentTypeFromMgr(std::vector<std::string>& instTypeArr) {
	using namespace kome::massbank;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG

	MassBankManager& mgr = MassBankManager::getInstance();
	mgr.getInstMgr()->getAllInstrumentTypes(instTypeArr);
}

//- Called From Panel_Factory....
kome::window::SettingsPanel*
	createVenderInstrumentComboPanel(
		wxWindow* parent,
		kome::plugin::ParameterSettings& params,
		kome::objects::SettingParameterValues& settings,
		const std::string& keyStr,
		const std::string& vendorHelp,
		const std::string& instruHelp
) {
	using namespace kome::plugin;

	obo::VenderArr venderarr;
	obo::InstArr instarr;
	
	//機器情報をセット
//	std::string keyStr = samplesInstrument;
	//現在のInstrumentから情報を入手
	std::string findVender,findInst;
	
	obo::getVenderAndInstInfo(keyStr, venderarr, instarr,findVender,findInst);

	// Vender Set
	SettingsValue* val0 = params.addValue();
	val0->setType(SettingsValue::TYPE_STRING);
	
	for each(auto& tmp in venderarr) {
		val0->addEnumItem(tmp.name.c_str(), tmp.sname.c_str(), tmp.description.c_str());
	}
	//add Free Word
	val0->addEnumItem("###","FreeVender","Free");
	val0->setDefaultValue(findVender.c_str());
	val0->setRequired(true);

	// Instrument Name Set
	SettingsValue* val1 = params.addValue();
	val1->setType(SettingsValue::TYPE_STRING);
	for each(auto& tmp in instarr) {
		val1->addEnumItem(tmp.name.c_str(), tmp.name.c_str(), tmp.description.c_str(), tmp.svendername.c_str());
	}
	//Add Free Word
	val1->addEnumItem("###","FreeInst","Free","FreeVender");

	val1->setRequired(true);
	val1->setDefaultValue(findInst.c_str());

	val0->addChildValue( val1 );

	// page
	SettingsPage* page = new kome::plugin::SettingsPage(params);
	pfc::setParamAndFormAdpt(params, val0, page, kVender, kVender,vendorHelp);
	pfc::setParamAndFormAdpt(params, val1, page, kInstrumentName,kInstrumentName, instruHelp);

	kome::window::SettingsPanel* panel = new kome::window::SettingsPanel(parent, page, false, &settings, false);
	return panel;
}

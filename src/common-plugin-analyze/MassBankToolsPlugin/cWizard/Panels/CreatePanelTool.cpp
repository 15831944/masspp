/**
 * @file CreatePanelTool.h
 * @brief Panel Create Support
 *
 * @author M.Fukuda
 * @date 2013.02.22
 *
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#include "../rel_stdafx.h"
#include "CreatePanelTool.h"
#include "AuxiliaryToolOfCreatingPanel.h"
#include "ExtendSettingsPanel.h"

// --------------------------------------------------------
//   ComboElements
// --------------------------------------------------------
void pfc::ComboElm::setVal(
	const std::string& val, 
	const bool nodsc
){
	if(!nodsc) {
		cmbElms_.push_back(CmbElm(val,val,val));
	} else {
		cmbElms_.push_back(CmbElm(val,val,""));
	}
}

// ShownameとValueが同じパターン
void pfc::ComboElm::setVal(
	const std::string& nameval,
	const std::string& dsc
) {
	cmbElms_.push_back(CmbElm(nameval, nameval, dsc));
}

// 全て異なるパターン
void pfc::ComboElm::setVal(
	const std::string& name,
	const std::string& val,
	const std::string& dsc
){
	cmbElms_.push_back(CmbElm(name, val, dsc));
}

// 全要素をVetctor配列で受け取るパターン。
// 要素数が違うとアウト。
void pfc::ComboElm::setVals(
	const std::vector<std::string>& vals,
	const std::vector<std::string>& shows,
	const std::vector<std::string>& dscs
){
	for(unsigned int i = 0; i < vals.size(); i++) {
		this->setVal(vals[i], shows[i], dscs[i]);
	}
}


// --------------------------------------------------------
//   CreatePanelTool
// --------------------------------------------------------
pfc::CreatePanelTool::CreatePanelTool(
	kome::plugin::ParameterSettings& org
) : inParams_(org)
	, page_(nullptr)
	, val_(nullptr)
	, gp_(nullptr)
{
	page_ = new kome::plugin::SettingsPage(inParams_);
}

void pfc::CreatePanelTool::setGroup(
	const std::string& gpName
){
	gp_ = inParams_.addGroup();
	gp_->setName(gpName.c_str());
}

// Set Text Box
void pfc::CreatePanelTool::makeStr(
	const std::string& sname, 
	const std::string& dsc, 
	const bool req, 
	const std::string& def,
	const bool savefg
){
	setString(req, def);
	makeThis(sname, dsc, savefg);
}

// Set String
void pfc::CreatePanelTool::setString(
	const bool req, 
	const std::string& def
) {
	val_ = inSetStr("", req, def);
}


// Set Combo Box
void pfc::CreatePanelTool::setComboBox(
	ComboElm& arr,
	const bool req,
	const std::string& def
){
	val_ = inSetComboBox(&arr, "", req, def);
}

void pfc::CreatePanelTool::setEnumComboBox(
	const std::string& ekey,
	const std::string& colname,
	const bool req,
	const std::string& def
){
	val_ = inSetEnumComboBox(ekey, colname, req, def);
}


// Set ListBox
void pfc::CreatePanelTool::setListBox(
	const bool req,
	const std::string& def
){
	val_ = inParams_.addValue();
	val_->setType(kome::plugin::SettingsValue::TYPE_LIST);
	val_->setDefaultValue(def.c_str());
}

// Set ListString
void pfc::CreatePanelTool::setListElmString(
	const std::string& colname,
	const bool req
){
	val_->addChildValue(inSetStr(colname,req,""));
}

// Set List Double
void pfc::CreatePanelTool::setListElmDouble(
	const std::string& colname,
	const bool req
){
	val_->addChildValue(inSetDbl(nullptr,nullptr,colname,req,""));
}

// Set List Combo
void pfc::CreatePanelTool::setListElmCombo(
	ComboElm& arr,
	const std::string& colname,
	const bool req
){
	val_->addChildValue(inSetComboBox(&arr,colname,req,""));
}

// Set CheckList Box
void pfc::CreatePanelTool::setCheckListBox(
	const std::string& colname,
	ComboElm& arr,
	const bool req,
	const std::string& def
){
	this->setListBox();
	val_->addChildValue(inSetCheckList(&arr, colname, req, def));
}

// Set EnumList Box
void pfc::CreatePanelTool::setEnumListBox(
	const std::string& colname,
	const std::string& enumKey,
	const bool req,
	const std::string& def
){
	this->setListBox();
	val_->addChildValue(inSetEnumComboBox(enumKey, colname, req, def));
}

// Assign Page,Forms, Params. so on
void pfc::CreatePanelTool::makeThis(
	const std::string& sname, 
	const std::string& dsc, 
	const bool savefg
){
	this->makeThis(sname,sname,dsc,savefg);
}

void pfc::CreatePanelTool::makeThis(
	const std::string& sname, 
	const std::string& title, 
	const std::string& dsc, 
	const bool savefg)
{
	if(gp_ != nullptr) {
		pfc::setParamAndFormAdptGroup(inParams_, val_, page_, gp_, sname, title, dsc, savefg);
	} else {
		pfc::setParamAndFormAdpt(inParams_, val_, page_, sname, title, dsc, savefg);
	}

}

// Generater
kome::window::SettingsPanel* 
	pfc::CreatePanelTool::generateSettingPanel(
	wxWindow* parent, 
	kome::objects::SettingParameterValues& spv
){
	kome::window::SettingsPanel* panel = 
		new kome::window::SettingsPanel(parent, page_, false, &spv, true);
	return panel;
}

// Generater
kome::massbank::wizard::ExtendSettingsPanel* 
	pfc::CreatePanelTool::generateExtendPanel(
	wxWindow* parent, 
	kome::objects::SettingParameterValues& spv
){
	using namespace kome::massbank::wizard;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG

	ExtendSettingsPanel* panel = 
		new ExtendSettingsPanel(parent, page_, false, &spv, true);
	return panel;
}

// #----------------------------------------#
//   Private ----
// #----------------------------------------#

// Set String Type
kome::plugin::SettingsValue* pfc::CreatePanelTool::inSetStr(
	const std::string& name
){
	return onSetBase(name, kome::plugin::SettingsValue::TYPE_STRING);
}

// Set String Type (Required flag, Default value)
kome::plugin::SettingsValue* pfc::CreatePanelTool::inSetStr(
	const std::string& name,
	const bool req,
	const std::string& def
){
	kome::plugin::SettingsValue* val = inSetStr(name);
	onAddReqDef(val, req, def);
	return val;
}

kome::plugin::SettingsValue* pfc::CreatePanelTool::inSetInt(
	CStr& name
){
	return onSetBase(name, kome::plugin::SettingsValue::TYPE_INT);
}

kome::plugin::SettingsValue* pfc::CreatePanelTool::inSetInt(
	const int* min,
	const int* max,
	const std::string& colname,
	const bool req,
	const std::string& def
){
	kome::plugin::SettingsValue* val = inSetInt(colname);
	if(min != nullptr) {
		val->getNumberRestriction().setInclusiveMinValue(*min);
	}
	if(max != nullptr) {
		val->getNumberRestriction().setInclusiveMaxValue(*min);
	}
	onAddReqDef(val, req, def);
	return val;
}

// Set Double Type ()
kome::plugin::SettingsValue* pfc::CreatePanelTool::inSetDbl(
	const std::string& name
){
	return onSetBase(name, kome::plugin::SettingsValue::TYPE_DOUBLE);
}

// Set Double Type (Required flag, Default value)
kome::plugin::SettingsValue* pfc::CreatePanelTool::inSetDbl(
	const double* min,
	const double* max,
	const std::string& name,
	const bool req,
	const std::string& def
){
	kome::plugin::SettingsValue* val = inSetDbl(name);
	onAddReqDef(val, req, def);
	if(min != nullptr) {
		val->getNumberRestriction().setInclusiveMaxValue(*min);
	}
	if(max != nullptr) {
		val->getNumberRestriction().setInclusiveMaxValue(*min);
	}
	return val;
}

// Set Bool Type
kome::plugin::SettingsValue* pfc::CreatePanelTool::inSetBool(
	const std::string& name
){
	return onSetBase(name, kome::plugin::SettingsValue::TYPE_BOOL);
}

// Set Bool Type (Required flag, Default value)
kome::plugin::SettingsValue* pfc::CreatePanelTool::inSetBool(
	const std::string& name,
	const bool req,
	const std::string& def
){
	kome::plugin::SettingsValue* val = inSetBool(name);
	onAddReqDef(val, req, def);
	return val;
}

// Set ComboBox ()
kome::plugin::SettingsValue* pfc::CreatePanelTool::inSetComboBox(
	pfc::ComboElm* arr,
	const std::string& name
){
	kome::plugin::SettingsValue* val = inSetStr(name);
	inComboElmSet(arr, val);
	return val;
}

// Set ComboBox (Required flag, Default value)
kome::plugin::SettingsValue* pfc::CreatePanelTool::inSetComboBox(
	pfc::ComboElm* arr,
	const std::string& name,
	const bool req,
	const std::string& def
){
	kome::plugin::SettingsValue* val = inSetStr(name, req, def);
	inComboElmSet(arr, val);
	return val;
}

kome::plugin::SettingsValue* pfc::CreatePanelTool::inSetEnumComboBox(
	const std::string& key,
	const std::string& name,
	const bool req,
	const std::string& def
){
	kome::plugin::SettingsValue* val = inSetStr(name, req, def);
	val->setEnumType(kome::plugin::SettingsValue::ENUM_PLUGIN);
	val->setEnumKey(key.c_str());
	return val;
}

// Set CheckList ()
kome::plugin::SettingsValue* pfc::CreatePanelTool::inSetCheckList(
	ComboElm* arr,
	const std::string& name
){
	kome::plugin::SettingsValue* val = inSetBool(name);
	this->inComboElmSet(arr, val);
	return val;
}

// Set CheckList (Required flag, Default value)
kome::plugin::SettingsValue* pfc::CreatePanelTool::inSetCheckList(
	ComboElm* arr,
	const std::string& name,
	const bool req,
	const std::string& def
){
	kome::plugin::SettingsValue* val = inSetBool(name, req, def);
	inComboElmSet(arr, val);
	return val;
}

//-----------------------------

// Setup Base SettingValue
kome::plugin::SettingsValue* pfc::CreatePanelTool::onSetBase(
	const std::string& name,
	kome::plugin::SettingsValue::ValueType tp
){
	kome::plugin::SettingsValue* val = inParams_.addValue();
	if(!name.empty()) {
		val->setName(name.c_str());
	}
	val->setType(tp);
	return val;
}

// Add Combo EnumItems with ComboElm* 
void pfc::CreatePanelTool::inComboElmSet(
	ComboElm* arr, 
	kome::plugin::SettingsValue*& val)
{
	for each(auto& p in *arr->Elms()) {
		val->addEnumItem(p.val.c_str(), p.show.c_str(), p.dsc.c_str());
	}
}

// Add Required flag, and Default value
void pfc::CreatePanelTool::onAddReqDef(
	kome::plugin::SettingsValue*& srcVal,
	const bool req,
	const std::string& def
){
	if(req) {
		srcVal->setRequired(true);
	}
	if(!def.empty()) {
		srcVal->setDefaultValue(def.c_str());
	}
}

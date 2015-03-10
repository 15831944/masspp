/**
 * @file CreatePanelToolOfCreatePanel.cpp
 * @brief Panel Create Support
 *
 * @author M.Fukuda
 * @date 2013.02.22
 *
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#include "../rel_stdafx.h"
#include "AuxiliaryToolOfCreatingPanel.h"
#include "../objectsName.h"
#include "wx/statline.h"

namespace pfc {

wxFlexGridSizer* createHeaders(
	wxWindow* parent, 
	const int addrownum, 
	const wxString& headLabel
) {
	int fullRowcnt = addrownum + 2;
	wxFlexGridSizer* sizer = new wxFlexGridSizer(fullRowcnt, 1, GRID_GAP, GRID_GAP);
	sizer->AddGrowableCol(0);

	// Header
	wxStaticText* headStr = new wxStaticText(parent, wxID_STATIC, headLabel); 
	sizer->Add(headStr, 1, wxALL|wxGROW|wxALIGN_LEFT);

	// Static Line
	wxStaticLine* stLine = new wxStaticLine(parent, wxID_STATIC, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL); 
	sizer->Add(stLine, 1, wxALL|wxGROW);
	return sizer;
}

//SettingsParamへの定義部を作成する
//Param[in] sName  : パラメータの名前。
//Param[in] holdfg : MassBank Section & sNameのアサインをする
kome::plugin::SettingsParam* onSetParamAdpt (
	kome::plugin::ParameterSettings& inParams,
	kome::plugin::SettingsValue*& val,
	const std::string& sName,
	bool holdfg
){
	kome::plugin::SettingsParam* param = inParams.addParam();
	param->setValue(val);
	param->setName(sName.c_str());
	// 保存フラグがある場合は、MassBankのセクションと名前をキーにして割り当てる
	if(holdfg) {
		param->setSection(kMassBank.c_str());
		param->setKey(sName.c_str());
	}
	return param;
}

//SettingsFormへの定義部を作成する
//Param[in] sTitle  : フォーム名
//Param[in] sDesc   : フォームの説明文
kome::plugin::SettingsForm* onSetFormAdpt(
	kome::plugin::SettingsPage*& page,
	kome::plugin::SettingsParam*& param,
	const std::string& sTitle,
	const std::string& sDesc
){
	kome::plugin::SettingsForm* form = page->addForm();
	form->setParam(param);
	form->setTitle(sTitle.c_str());
	form->setDesc(sDesc.c_str());
	return form;
}

void addEnumItem(
	kome::plugin::SettingsValue*& val,
	const std::string& str1
){
	val->addEnumItem(str1.c_str(),str1.c_str(),str1.c_str());
}

void addEnumItem(
	kome::plugin::SettingsValue*& val,
	const std::string& str1,
	const std::string& help1
){
	val->addEnumItem(str1.c_str(), str1.c_str(), help1.c_str());
}


//定型化する
//ParameterSettingsへ、val,page等を元に割り付ける。
//  Param[in] inParams : paramの元になるparam
//  Param[in] val      : paramに割り当てるval
//  Param[in] page     : formの元になるpage
//  Param[in] sName    : パラメータの名前
//  Param[in] sTitle   : フォーム名
//  Param[in] sDesc    : フォームの説明文
//  Param[in] holdfg   : 保存用にMassBank Sectionを作るか否か
void setParamAndFormAdpt (
	kome::plugin::ParameterSettings& inParams,
	kome::plugin::SettingsValue*& val,
	kome::plugin::SettingsPage*& page,
	const std::string& sName,
	const std::string& sTitle,
	const std::string& sDesc,
	bool holdfg
){
	kome::plugin::SettingsParam* param = inParams.addParam();
	param->setValue(val);
	param->setName(sName.c_str());
	// 保存が必要な場合はセクションとキーを定義する
	if(holdfg) {
		param->setSection(kMassBank.c_str());
		param->setKey(sName.c_str());
	}
	kome::plugin::SettingsForm* form = page->addForm();
	
	form->setParam(param);
	form->setTitle(sTitle.c_str());
	form->setDesc(sDesc.c_str());
}

//定型化する
//ParameterSettingsへ、val,page等を元に割り付ける。
//  Param[in] inParams : paramの元になるparam
//  Param[in] val      : paramに割り当てるval
//  Param[in] page     : formの元になるpage
//  Param[in] sgroup   : formの属するグループ
//  Param[in] sName    : パラメータの名前
//  Param[in] sTitle   : フォーム名
//  Param[in] sDesc    : フォームの説明文
//  Param[in] holdfg   : 保存用にMassBank Sectionを作るか否か
void setParamAndFormAdptGroup (
	kome::plugin::ParameterSettings& inParams,
	kome::plugin::SettingsValue*& val,
	kome::plugin::SettingsPage*& page,
	kome::plugin::SettingsGroup* sgroup,
	const std::string& sName,
	const std::string& sTitle,
	const std::string& sDesc,
	bool holdfg
){
	kome::plugin::SettingsParam* param = onSetParamAdpt(inParams, val, sName, holdfg);
	kome::plugin::SettingsForm* form = onSetFormAdpt(page, param, sTitle, sDesc);
	if (sgroup != nullptr) {
		form->setGroup(sgroup);
	}
}

// subtag の箱を作る
kome::plugin::SettingsValue* setSubTagListValue (
	kome::plugin::ParameterSettings& inParams,
	const std::vector<std::string>& subtagArr,
	const std::vector<std::string>* subtagNGArr
){
	using namespace kome::plugin;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


	SettingsValue* val = inParams.addValue();
	val->setType(SettingsValue::TYPE_LIST);

	// subTag の項目を作成する
	SettingsValue* childVal 
		= setListChildStringValue(inParams, kSubTag, &subtagArr, true);
	val->addChildValue(childVal);

	// Valueの項目を作成する
	childVal = setListChildStringValue(inParams, kValue, nullptr, true);
	val->addChildValue(childVal);

	return val;
}

kome::plugin::SettingsValue* setListChildStringValue (
	kome::plugin::ParameterSettings& inParams,
	const std::string& childname,
	const std::vector<std::string>* childvals,
	const bool required,
	const bool hasChecked
) {
	using namespace kome::plugin;
	// subTag の項目を作成する
	SettingsValue* childVal = inParams.addValue();
	if (hasChecked) {
		childVal->setType(SettingsValue::TYPE_BOOL);
	} else {
		childVal->setType(SettingsValue::TYPE_STRING);
	}
	childVal->setName(childname.c_str());

	if (childvals != nullptr) {
		std::for_each(childvals->begin(), childvals->end(),
			[&](const std::string& s){
				childVal->addEnumItem(s.c_str(),s.c_str(),s.c_str());});
	}
	childVal->setRequired(required);
	return childVal;
}

// Integer の Value 部分を作成する
//  Param[in] inParams : paramの元になるparam
//  Param[in] nMin     : 最小値。使わない場合は knNoUse.
//  Param[in] nMax     : 最大値。使わない場合は knNoUse.
//  Param[in] def      : デフォルト値。使わないときは ""
//  Param[in] reqFlg   : 入力必須項目か？ Y/N
//  Return    SettingsValue Object Pointer
kome::plugin::SettingsValue* createIntegerSTValue(
	kome::plugin::ParameterSettings& params,
	const int nMin, 
	const int nMax, 
	const std::string& def, 
	const bool reqFlg
){
	kome::plugin::SettingsValue* val = params.addValue();
	val->setType(kome::plugin::SettingsValue::TYPE_INT);
	if (nMin != knNoUse) {
		val->getNumberRestriction().setInclusiveMinValue(nMin);
	}
	if (nMax != knNoUse) {
		val->getNumberRestriction().setInclusiveMaxValue(nMax);
	}
	val->setDefaultValue(def.c_str());
	val->setRequired(reqFlg);
	return val;

}

// Double の Value 部分を作成する
//  Param[in] inParams : paramの元になるparam
//  Param[in] dMin     : 最小値。使わない場合は knNoUse.
//  Param[in] dMax     : 最大値。使わない場合は knNoUse.
//  Param[in] def      : デフォルト値。使わないときは ""
//  Param[in] reqFlg   : 入力必須項目か？ Y/N
//  Return    SettingsValue Object Pointer
kome::plugin::SettingsValue* createDoubleSTValue(
	kome::plugin::ParameterSettings& params,
	const double dMin, 
	const double dMax,
	const std::string& def, 
	const bool reqFlg
){
	kome::plugin::SettingsValue* val = params.addValue();
	val->setType(kome::plugin::SettingsValue::TYPE_DOUBLE);
	if (dMin != kdNoUse) {
		val->getNumberRestriction().setInclusiveMinValue(dMin);
	}
	if (dMax != kdNoUse) {
		val->getNumberRestriction().setInclusiveMaxValue(dMax);
	}
	val->setDefaultValue(def.c_str());
	val->setRequired(reqFlg);
	return val;
}

// String の Value 部分を作成する
//  Param[in] inParams : paramの元になるparam
//  Param[in] def      : デフォルト値。使わないときは ""
//  Param[in] reqFlg   : 入力必須項目か？ Y/N
//  Return    SettingsValue Object Pointer
kome::plugin::SettingsValue* createStringSTValue(
	kome::plugin::ParameterSettings& params,
	const std::string& def,
	const bool reqFlg
){
	kome::plugin::SettingsValue* val = params.addValue();
	val->setType(kome::plugin::SettingsValue::TYPE_STRING);
	val->setDefaultValue(def.c_str());
	val->setRequired(reqFlg);
	return val;
}

} // end of namespace

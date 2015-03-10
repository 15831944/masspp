/**
 * @file CreatePanelToolOfCreatePanel.h
 * @brief Panel Create Support
 *
 * @author M.Fukuda
 * @date 2013.02.22
 *
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#pragma once

#include <vector>

namespace pfc {
//定型化
	const int knNoUse(-INT_MAX);
	const double kdNoUse(-DBL_MAX);

// 共通する　ヘッダ名、ライン等の作成を行う
// param[in] parent    親Window
// param[in] addrownum ラインの後に追加される要素数。
// param[in] headLabel ヘッダラベルの内容
// 内部Sizerは最低でも２（ヘッダラベルとライン）を確保していて、
// 更に追加する形になる
wxFlexGridSizer* createHeaders(
	wxWindow* parent,
	const int addrownum,
	const wxString& headLabel
	);

//SettingsParamへの定義部を作成する
//Param[in] sName  : パラメータの名前。
//Param[in] holdfg : MassBank Section & sNameのアサインをする
kome::plugin::SettingsParam* onSetParamAdpt (
	kome::plugin::ParameterSettings& inParams,
	kome::plugin::SettingsValue*& val,
	const std::string& sName,
	bool holdfg = false);

void addEnumItem(	
	kome::plugin::SettingsValue*& val,
	const std::string& str1
	);

//SettingsFormへの定義部を作成する
//Param[in] sTitle  : フォーム名
//Param[in] sDesc   : フォームの説明文
kome::plugin::SettingsForm* onSetFormAdpt(
	kome::plugin::SettingsPage*& page,
	kome::plugin::SettingsParam*& param,
	const std::string& sTitle,
	const std::string& sDesc);

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
	bool holdfg = false);

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
	bool holdfg = false);

// SubTag & Value の　ListCtrlの Value部を作成する。
kome::plugin::SettingsValue* setSubTagListValue (
	kome::plugin::ParameterSettings& inParams,
	const std::vector<std::string>& subtagArr,
	const std::vector<std::string>* subtagNGArr = nullptr);

kome::plugin::SettingsValue* setListChildStringValue (
	kome::plugin::ParameterSettings& inParams,
	const std::string& childname,
	const std::vector<std::string>* childval,
	const bool required,
	const bool hasChecked = false
	);

// String の Value 部分を作成する
//  Param[in] inParams : paramの元になるparam
//  Param[in] def      : デフォルト値。使わないときは ""
//  Param[in] reqFlg   : 入力必須項目か？ Y/N
//  Return    SettingsValue Object Pointer
kome::plugin::SettingsValue* createStringSTValue(
	kome::plugin::ParameterSettings& params,
	const std::string& def,
	const bool reqFlg);

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
	const bool reqFlg);

// Double の Value 部分を作成する
//  Param[in] inParams : paramの元になるparam
//  Param[in] dMin     : 最小値。使わない場合は kdNoUse.
//  Param[in] dMax     : 最大値。使わない場合は kdNoUse.
//  Param[in] def      : デフォルト値。使わないときは ""
//  Param[in] reqFlg   : 入力必須項目か？ Y/N
//  Return    SettingsValue Object Pointer
kome::plugin::SettingsValue* createDoubleSTValue(
	kome::plugin::ParameterSettings& params,
	const double dmin,
	const double dMax,
	const std::string& def,
	const bool reqFlg);

}

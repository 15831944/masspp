/**
 * @file ExtendSettingPanel
 * @brief Panel Create Support
 *
 * @author M.Fukuda
 * @date 2013.02.22
 *
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#include "../rel_stdafx.h"
#include "ExtendSettingsPanel.h"

using namespace kome::massbank::wizard;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


ExtendSettingsPanel::ExtendSettingsPanel (
	wxWindow* parent,
	kome::plugin::SettingsPage* page,
	const bool dfltBtn,
	kome::objects::SettingParameterValues* settings,
	const bool saveVal
	) : kome::window::SettingsPanel(parent, page, dfltBtn, settings, saveVal)
{
}

// 内部のフォーム名と一致するフォームを探し、ＩＤを返す
int ExtendSettingsPanel::onSearchFormID_fromName(const std::string& keyStr) {
	// input error
	if (keyStr.empty()) return -1;
	for each(const auto& tmp in m_forms) {
		if(keyStr.compare(tmp.form->getTitle()) == 0) {
			return tmp.id;
		}
	}
	return -1;
}

// 対象のフォーム名の要素の値を取得する
std::string ExtendSettingsPanel::getValue(const std::string& formTitle) {
	if (formTitle.empty()) return "";
	int idx = onSearchFormID_fromName(formTitle);
	if(idx >= 0) {
		return this->getValueFromWindow(idx);
	}
	return "";
}


// 対象のフォーム名の要素に値を割り当てる
void ExtendSettingsPanel::setValue(
	const std::string& formTitle, 
	const char* str
) {
	int idx = onSearchFormID_fromName(formTitle);
	if(idx >= 0) {
		this->setValueFromString(idx, str);
	}
}

// 対象のフォーム名の要素に値を割り当てる
void ExtendSettingsPanel::setValue(
	const std::string& formTitle, 
	const std::string& str
) {
	int idx = onSearchFormID_fromName(formTitle);
	if(idx >= 0) {
		this->setValueFromString(idx, str.c_str());
	}
}

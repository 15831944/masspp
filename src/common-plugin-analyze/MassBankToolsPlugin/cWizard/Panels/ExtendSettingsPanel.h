/**
 * @file ExtendSettingPanel
 * @brief Panel Create Support
 *
 * @author M.Fukuda
 * @date 2013.02.22
 *
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 *
 * @ note -=====================================
 *  SettingsPanelを継承して、getValueFromWindowを内部的に
 *  使用できるようにしたクラス。フォーム名を渡すと、値が返される
 */

#pragma once

#include <vector>

// Public にして汎用化する。
namespace kome {
	namespace massbank {
		namespace wizard {

			class ExtendSettingsPanel : public kome::window::SettingsPanel {
			public:
				// constructor
				ExtendSettingsPanel (
					wxWindow* parent,
					kome::plugin::SettingsPage* page,
					const bool dfltBtn = true,
					kome::objects::SettingParameterValues* settings = NULL,
					const bool saveVal = true
					);

				virtual ~ExtendSettingsPanel(){}

			private:

				// フォーム名からフォームのＩＤを検索する
				int onSearchFormID_fromName(const std::string& keyStr);

			public:

				// 対象のフォーム名の要素を取得する
				std::string getValue(const std::string& formTitle);

				// 対象のフォーム名の要素に値を割り当てる
				void setValue(
					const std::string& formTitle,
					const char* str
					);

				// 対象のフォーム名の要素に値を割り当てる
				void setValue(
					const std::string& formTitle, 
					const std::string& str
					);
			};
		}
	}
}

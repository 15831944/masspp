/**
 * @file Call Vender&Instrument Select Combo Box
 * @brief Vender&Instrument Select Combo Box Informations
 *
 * @author M.Fukuda
 * @date 2013.01.18
 *
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#pragma once

#include "../rel_stdafx.h"

#pragma once

// Vender-InstrumentNameのコンボボックスのパネルを作成する
kome::window::SettingsPanel* createVenderInstrumentComboPanel(
	wxWindow* parent,
	kome::plugin::ParameterSettings& params,
	kome::objects::SettingParameterValues& settings,
	const std::string& samplesInstrument,
	const std::string& vendorHelp,
	const std::string& instruHelp
	);

// Instrument Type を　取得する
void getInstumentTypeFromMgr(std::vector<std::string>& _InstrumentTypeArr);

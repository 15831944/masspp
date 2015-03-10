/**
 * @file DataProcessing.h
 * @brief Data Processing List Box Informations
 *
 * @author M.Fukuda
 * @date 2013.02.26
 *
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#pragma once

#include "../rel_stdafx.h"

#pragma once

// DataProccessing‚Ìƒpƒlƒ‹‚ğì¬‚·‚é
kome::window::SettingsPanel* createDataProccessingListBox(
	wxWindow* parent,
	kome::plugin::ParameterSettings& params,
	kome::objects::SettingParameterValues& settings,
	const bool saveVal = false);

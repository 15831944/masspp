/**
 * @file PluginTools.h
 * @brief header file to use PluginTools library
 *
 * @author S.Tanaka
 * @date 2006.07.03
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_PLUGIN_TOOLS_H__
#define __KOME_PLUGIN_TOOLS_H__


//
// macros
// 

#ifdef _MSC_VER
	#define PLUGIN_TOOLS_CLASS __declspec(dllimport)
#else
	#define PLUGIN_TOOLS_CLASS
#endif	// _MSC_VER


//
// includes
//

#include "PluginTools/PluginInfo.h"
#include "PluginTools/PluginManager.h"
#include "PluginTools/PluginMenu.h"
#include "PluginTools/PluginCall.h"
#include "PluginTools/PluginFunctionItem.h"
#include "PluginTools/PluginCallTool.h"
#include "PluginTools/PluginIcon.h"
#include "PluginTools/PluginHelp.h"
#include "PluginTools/ParameterSettings.h"
#include "PluginTools/SettingsValue.h"
#include "PluginTools/SettingsParam.h"
#include "PluginTools/SettingsGroup.h"
#include "PluginTools/SettingsPage.h"
#include "PluginTools/SettingsForm.h"
#include "PluginTools/SettingsChapter.h"
#include "PluginTools/CommandManager.h"
#include "PluginTools/FileFactory.h"
#include "PluginTools/BatchInfo.h"
#include "PluginTools/BatchService.h"
#include "PluginTools/PluginComponent.h"	// @Date:2013/07/29	<Add>	A.Ozaki

#endif	// __KOME_PLUGIN_TOOLS_H__

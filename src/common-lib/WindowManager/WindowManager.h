/**
 * @file WindowManager.h
 * @brief header file to use WindowManager library
 *
 * @author S.Tanaka
 * @date 2006.07.20
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_WINDOW_MANAGER_H__
#define __KOME_WINDOW_MANAGER_H__

//
// macros
//

#ifdef _MSC_VER
	#define WINDOW_MANAGER_CLASS __declspec(dllimport)
	#define WXUSINGDLL
#else
	#define WINDOW_MANAGER_CLASS
#endif	// _MSC_VER


//
// includes
//

#include "WindowManager/WindowTool.h"
#include "WindowManager/Canvas.h"
#include "WindowManager/IconManager.h"
#include "WindowManager/DCGraphics.h"
#include "WindowManager/DialogProgress.h"
#include "WindowManager/DialogSelector.h"
#include "WindowManager/MainFrame.h"
#include "WindowManager/ChildFrame.h"
#include "WindowManager/ListTool.h"
#include "WindowManager/ListCtrl.h"
#include "WindowManager/NumberTextCtrl.h"
#include "WindowManager/GridListCtrl.h"
#include "WindowManager/GridListDialog.h"
#include "WindowManager/CheckGridListCtrl.h"
#include "WindowManager/CheckGridListPanel.h"
#include "WindowManager/GridSearchDialog.h"
#include "WindowManager/InputDialog.h"
#include "WindowManager/SettingsPanel.h"
#include "WindowManager/StandardDialog.h"
#include "WindowManager/StandardPanel.h"
#include "WindowManager/CollapsiblePane.h"
#include "WindowManager/SettingsPageDialog.h"
#include "WindowManager/CanvasBase.h"
#include "WindowManager/ProfileCanvas.h"
#include "WindowManager/SpectrumCanvas.h"
#include "WindowManager/ChromatogramCanvas.h"
#include "WindowManager/OverlappingSpectraCanvas.h"
#include "WindowManager/OverlappingChromatogramsCanvas.h"
#include "WindowManager/MarkComboBox.h"
#include "WindowManager/RawDataFileDialog.h"
#include "WindowManager/ListEditPanel.h"
#include "WindowManager/PluginSelectionPanel.h"
#include "WindowManager/StatusBar.h"
#include "WindowManager/ProgressDialog.h"
#include "WindowManager/EMailTextCtrl.h"
#include "WindowManager/Wizard.h"
#include "WindowManager/HelpGetterPanel.h"
#include "WindowManager/ParametersAndHelpPanel.h"
#include "WindowManager/SettingsHelpPanel.h"
#include "WindowManager/FileDropTarget.h"
#include "WindowManager/BitmapCanvas.h"
#include "WindowManager/PeakMatrixSettingsPageBase.h"
#include "WindowManager/RadioBoxBook.h"
#include "WindowManager/StandardWizardPage.h"
#include "WindowManager/ParameterTitlePanel.h"
#include "WindowManager/MessageDialog.h"
#include "WindowManager/MessageCheckDialog.h"
#include "WindowManager/FontCanvas.h"
#include "WindowManager/FontTool.h"
#include "WindowManager/ParametersAndHelpDialog.h"
#include "WindowManager/HelpPanel.h"


#endif	// __KOME_WINDOW_MANAGER_H__

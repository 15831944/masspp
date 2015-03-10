/**
 * @file rel_stdafx.h
 * @brief for stdafx.h's relative path
 *
 * @author M.Fukuda
 * @date 2013.01.18
 *
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

// フォルダ構造が異なるので、
// stdafx の位置関係が変わると全部書き直しになる。
// 相対位置をこちらで管理する。
#pragma once
#include "../stdafx.h"

//define で共通認識されるべき項目はこちら
#define BORDER_SIZE 3
#define GRID_GAP    3

/**
Help, 定型文は極力一か所に纏める。

Wizardの1ページにつき、WizardPage,基本Panelをかいた .h, .cppで構成する。

kome::window::StandardWizardPage　を継承したものを　Wizardの骨子とする。
これには、画像と、ParametersAndHelpPanelの子を張り付けるだけにする

ParametersAndHelpPanelを継承したPanelとして作る。
ここに極力すべてを書き込む。
複雑な処理が必要になるパネルは　その部分だけ　独立させたパネルとして定義する。
(Panelsのフォルダ下に入れる）

何らかの事情でsetDescを使えないが、Helpが必要な項目は、
kome::window::HelpGetterPanelを継承したパネルを作成して、
必要に応じて部分部分で入れる。
*/

wxBitmap getWizImg(const wxString& imgPath);
wxBitmap getWizImg(const std::string& imgPath);

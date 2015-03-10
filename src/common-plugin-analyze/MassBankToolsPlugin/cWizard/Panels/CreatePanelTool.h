/**
 * @file CreatePanelTool.h
 * @brief Panel Create Support
 *
 * @author M.Fukuda
 * @date 2013.02.22
 *
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#pragma once

namespace kome {
	namespace massbank {
		namespace wizard {
			class ExtendSettingsPanel;
		}
	}
}

namespace pfc {

	// inParamsのラッパー(表記を簡易化する為)
	class spvAdpt {
	public:
		spvAdpt(kome::objects::SettingParameterValues& spv)
			: spv_(spv){}
	private:
		kome::objects::SettingParameterValues& spv_;
	public:
		// settingのValueを返す
		std::string val(const std::string& tgt) const {
			return spv_.getParameterValue(tgt.c_str());
		}
		// settingのsubValueを返す
		kome::objects::SettingParameterValues* subvals(const std::string& key) {
			return spv_.getSubParameterValues(key.c_str());
		}
	};

	// Add Elmの表記を統一する為のクラス
	class ComboElm{
		struct CmbElm{
			std::string val;
			std::string show;
			std::string dsc;
			CmbElm(){}
			CmbElm(const std::string& v, const std::string& s, const std::string& d)
				: val(v), show(s), dsc(d){}
		};
	public:
		ComboElm(){}
		~ComboElm(){}
	private:
		std::vector<CmbElm> cmbElms_;
	public:
		// 値(構造体)を返す
		std::vector<CmbElm>* Elms() {return &cmbElms_;}
		void setVal(
			const std::string& val, 
			const bool nodsc = false);

		void setVal(
			const std::string& nameval, 
			const std::string& dsc);

		void setVal(
			const std::string& name, 
			const std::string& val,
			const std::string& dsc);

		void setVals(
			const std::vector<std::string>& vals,
			const std::vector<std::string>& shows,
			const std::vector<std::string>& dscs);
	};

	// 便利なんだけど　少し判りにくいパネル定義部分を
	// 簡単な名称で定義、生成できるようにするクラス。
	class CreatePanelTool {

	public:
		CreatePanelTool(kome::plugin::ParameterSettings&);
		~CreatePanelTool(){}
	private:
		// コンストラクタで取得する
		kome::plugin::ParameterSettings& inParams_;
		// Page(内部で生成される)
		kome::plugin::SettingsPage* page_;
		// 内部的な一時オブジェクト
		kome::plugin::SettingsValue* val_;
		// Groupが必要な場合は作成
		kome::plugin::SettingsGroup* gp_;
	public:
		void setGroup(const std::string& gpName);

		void makeStr(
			const std::string& sname, 
			const std::string& dsc, 
			const bool req = false, 
			const std::string& def = "",
			const bool savefg = true);

		// Set String(Label & TextBox)
		void setString(
			const bool req = false, 
			const std::string& def = "");

		void setInteger(
			const int* min,
			const int* max,
			const bool req = false,
			const std::string& def = "");

		void setDouble(
			const double* min,
			const double* max,
			const bool req = false,
			const std::string& def = "");

		// Set ComboBox
		void setComboBox(
			ComboElm& arr,
			const bool req = false,
			const std::string& def ="");

		void setEnumComboBox(
			const std::string& etype,
			const std::string& colname,
			const bool req = false,
			const std::string& def ="");

		// Set CheckListBox
		void setCheckListBox(
			const std::string& colname,
			ComboElm& arr,
			const bool req = false,
			const std::string& def ="");

		// Set Enum List(Enum ComboBox)
		void setEnumListBox(
			const std::string& colname,
			const std::string& EnumKey,
			const bool req = false,
			const std::string& def ="");

		// Set ListBox(Frame Only)
		// Need setListElem.. Functions
		void setListBox(
			const bool req = false,
			const std::string& def ="");

		// set String Column for List
		void setListElmString(
			const std::string& colname,
			const bool req = true);

		// set Double Column for List
		void setListElmDouble(
			const std::string& colname,
			const bool req = true);

		void setListElmDouble(
			const std::string& colname,
			const double* min,
			const double* max,
			const bool req = true);

		// set ComboBox Column for List
		void setListElmCombo(
			ComboElm& arr,
			const std::string& colname,
			const bool req = true);

		// Adapt to inner page, from.. so on.
		// form->add.. 等の部分を担う
		void makeThis(
			const std::string& sname, 
			const std::string& dsc, 
			const bool savefg = true);

		void makeThis(
			const std::string& sname, 
			const std::string& title, 
			const std::string& dsc, 
			const bool savefg = true);

		// パネルとして生成する。
		kome::window::SettingsPanel* generateSettingPanel(
			wxWindow* parent, 
			kome::objects::SettingParameterValues& spv);

		// パネルとして生成する。
		// 此方は、getValueで、Form名から内部の値を取得できる
		kome::massbank::wizard::ExtendSettingsPanel* generateExtendPanel(
			wxWindow* parent, 
			kome::objects::SettingParameterValues& spv);

	private:
		typedef const std::string CStr;
		typedef const bool CBool;
		typedef kome::plugin::SettingsValue SVal;

		SVal* inSetStr(CStr& name ="");
		SVal* inSetStr(CStr& name, CBool req, CStr& def);

		SVal* inSetInt(CStr& name = "");
		SVal* inSetInt(const int* min, const int* max, CStr& name, const bool req, CStr& def = "");

		SVal* inSetDbl(CStr& name ="");
		SVal* inSetDbl(const double* min, const double* max, CStr& name, CBool req, CStr& def);

		SVal* inSetBool(CStr& name ="");
		SVal* inSetBool(CStr& name, CBool req, CStr& def);

		SVal* inSetComboBox(ComboElm* arr, CStr& name ="");
		SVal* inSetComboBox(ComboElm* arr, CStr& name, CBool req, CStr& def);

		SVal* inSetEnumComboBox(CStr& key, CStr& name, CBool req, CStr& def);

		SVal* inSetCheckList(ComboElm* arr, CStr& name = "");
		SVal* inSetCheckList(ComboElm* arr, CStr& name, CBool req, CStr& def);

	private:
		SVal* onSetBase(
			CStr& name,
			kome::plugin::SettingsValue::ValueType tp);

		void inComboElmSet(
			ComboElm* arr, 
			SVal*& ref);

		void onAddReqDef(
			SVal*& srcVal, 
			CBool req, 
			CStr& def = "");

	};
}

/**
 * @file SearchResultPanel.h
 * @brief Search Result Dialog
 *
 * @author M.Fukuda
 * @date 2013.12.21
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#pragma once

namespace kome {
	namespace massbank {

		class ResultListCtrl;

		/**
		 * @class SearchResultListPanel
		 * @brief search result dialog class
		 */ 		
		class SearchResultListPanel : public kome::window::StandardPanel {

			// メニューボタンの情報を保持する構造体
		struct MenuButtonInfo {
			kome::plugin::PluginFunctionItem* func_item;
			wxIcon ok_icon;
			wxIcon ng_icon;
			wxBitmapButton* button;
			bool enable;
			unsigned int flagVal;

			// constructor
			MenuButtonInfo() 
				: func_item(nullptr)
				, ok_icon()
				, ng_icon()
				, button(nullptr)
				, enable(true)
				, flagVal(0)
			{}
		};

		public:
			/**
			 * @fn SearchResultListPanel( wxWindow* parent )
			 * @brief constructor
			 */
			SearchResultListPanel(
				wxWindow* parent);

			/**
			 * @fn virtual ~SearchResultListPanel()
			 * @brief destructor
			 */
			virtual ~SearchResultListPanel();
		
		protected:
			// Result List Ctrl Pointer
			ResultListCtrl* resultListCtrl_;
			std::vector<MenuButtonInfo> menu_button_infos_;

			unsigned int flags;
			bool modalsync_;

			wxButton* searchButton_;
			wxButton* exportButton_;

		public:
			/**
			 * @fn upDateMe()
			 * @brief Up Data My Self
			 */
			void upDateMe();
			void create();

		protected:
			/**
			 * @fn virtual wxSizer* createButtonSizer()
			 * @brief creates button sizer (override method)
			 * @return button sizer
			 */
			virtual wxSizer* createMainSizer();

			/**
			 * @fn void saveData( wxCommandEvent& evt )
			 * @brief saves data
			 * @param[in] evt event information
			 */
			void exportData(wxCommandEvent& evt);

			void findKeywordFromData(wxCommandEvent& evt);

			/**
			 * @fn void updateUI( wxUpdateUIEvent& evt )
			 * @brief updates UI
			 * @param[in] evt evnet information
			 */
			void updateUI(wxUpdateUIEvent& evt);

		protected:
			wxBoxSizer* onCreateMenuButtons();
			wxBoxSizer* onCreateBottomButtons();

			void onMakeMenuButtons(
				const char* typeFunc,
				std::vector<MenuButtonInfo>& refArr);

			void onSelectMenuButton(wxCommandEvent& evt);

			void onUpdateUIMenuButton(wxUpdateUIEvent& evt);

			void onToggleMenuButton(const unsigned int idx);

		private:
			DECLARE_EVENT_TABLE();
		};

	}
}

/**
 * @file SampleTreeCtrl.h
 * @brief interfaces of SampleTreeCtrl
 *
 * @author S.Tanaka
 * @date 2008.01.10
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_VIEW_RAW_DATA_TREE_CTRL_H__
#define __KOME_VIEW_RAW_DATA_TREE_CTRL_H__

#include <wx/wx.h>
#include <wx/treectrl.h>

namespace kome {
	namespace view {

		class SamplePanel;
		/**
		 * @class SampleTreeCtrl
		 * @brief sample tree GUI control class
		 */
		class SampleTreeCtrl : public wxTreeCtrl {
		public:
			/**
			 * @fn SampleTreeCtrl( wxWindow* parent )
			 * @brief constructor
			 * @param[in] parent parent window
			 */
			SampleTreeCtrl( wxWindow* parent );

			/**
			 * @fn virtual ~SampleTreeCtrl()
			 * @brief destructor
			 */
			virtual ~SampleTreeCtrl();

			/** file */
			FILE* m_fp;

			/** menu context */
			wxMenu* m_menuContext;

		public:
			/**
			 * @fn void onDblClicked( wxMouseEvent& )
			 * @brief This method is called when tree item is double clicked.
			 * @param evt mouse event information
			 */
			void onDblClicked( wxMouseEvent& );	// @date 2013/06/10 <Mod> OKADA

			/**
			 * @fn void onMouseMove( wxMouseEvent& )
			 * @brief 選択表示形式の画像にマウスカーソル変更
			 * @param evt mouse event information
			 */
			void onMouseMove( wxMouseEvent& );	// @date 2011.05.10 <Add> M.Izumi	// @date 2013/06/10 <Mod> OKADA

			/**
			 * @fn void onMouseRightUp( wxMouseEvent& evt )
			 * @brief 右クリックメニュー表示処理
			 * @param evt mouse event information
			 */
			void onMouseRightUp( wxMouseEvent& evt );

			/**
			 * @fn void onMouseRightDown( wxMouseEvent& evt )
			 * @brief 右クリックメニュー表示処理
			 * @param evt mouse event information
			 */
			void onMouseRightDown( wxMouseEvent& evt );

			/**
			 * @fn void onContextMenu( wxContextMenuEvent& evt )
			 * @brief This method is called on requesting context menu
			 * @param[in] evt context menu event information
			 */
			void onContextMenu( wxMouseEvent& evt );

			/**
			 * @fn wxTreeCtrl* getNowTree(void)
			 * @brief get now tree
			 * @return pointer of wxTreeCtrl
			 */
			wxTreeCtrl* getNowTree(void);

			/**
			 * @fn virtual wxMenu* createMenu()
			 * @brief creates menu
			 * @param[in] type DataType
			 * @return created menu
			 */
			wxMenu* createMenu( kome::objects::Variant::DataType type );

			/**
			 * @fn void onCollapse( wxCommandEvent& );
			 * @brief This method is called on context menu
			 * @param[in] evt context menu event information
			 */
			void onCollapse( wxCommandEvent& );	// @date 2013/06/10 <Mod> OKADA

			/**
			 * @fn void onExpand( wxCommandEvent& );
			 * @brief This method is called on context menu
			 * @param[in] evt context menu event information
			 */
			void onExpand( wxCommandEvent& );	// @date 2013/06/10 <Mod> OKADA

			/**
			 * @fn void onMenuPopup( wxCommandEvent& evt );
			 * @brief clicked popup menu
			 * @param[in] evt information of wxCommandEvent
			 */
			void onMenuPopup( wxCommandEvent& evt );

			/**
			 * @fn void onTooltip( wxTreeEvent& evt )
			 * @brief sets tooltip
			 * @param[in] evt tree event
			 */
			void onTooltip( wxTreeEvent& evt );

			// >>>>>>	@Date:2013/07/17	<Add>	A.Ozaki
			/**
			 * @fn void onTreeExpanding( wxTreeEvent& evt )
			 * @brief the item is being expanded
			 * @param evt tree event information
			 */
			void onTreeExpanding( wxTreeEvent& evt );
			// <<<<<<	@Date:2013/07/17	<Add>	A.Ozaki


		private:
			DECLARE_EVENT_TABLE()

		};
	}
}

#endif	//  __KOME_VIEW_RAW_DATA_TREE_CTRL_H__

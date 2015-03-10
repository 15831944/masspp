/**
 * @file SampleTreeCtrl.h
 * @brief interfaces of SampleTreeCtrl
 *
 * @author H.Parry
 * @date 2011.04.20
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation. All rights reserved.
 */


#ifndef __KOME_VIEW_RAW_DATA_TREE_CTRL_H__
#define __KOME_VIEW_RAW_DATA_TREE_CTRL_H__

#include <wx/wx.h>
#include <wx/treectrl.h>

namespace kome {
	namespace properties {

		class EditSampleUserPropertiesDialog;

		/**
		 * @class SampleTreeCtrl
		 * @brief sample tree GUI control class
		 */
		class SampleTreeCtrl : public wxTreeCtrl {
		public:
			/**
			 * @fn SampleTreeCtrl()
			 * @brief constructor
			 */
			SampleTreeCtrl(EditSampleUserPropertiesDialog* pParentDlg);

			/**
			 * @fn virtual ~SampleTreeCtrl()
			 * @brief destructor
			 */
			virtual ~SampleTreeCtrl();

		private:
			/** parent dialog */
			EditSampleUserPropertiesDialog* m_pParentDlg;
		public:
			/**
			 * @fn void onSelChanged( wxTreeEvent& evt )
			 * @brief This method is called when a tree item is single clicked and has changed.
			 * @param evt tree event information
			 */
			void onSelChanged( wxTreeEvent& evt );

			/**
			 * @fn void onSelChanging( wxTreeEvent& evt )
			 * @brief This method is called when tree item is single clicked and it about to change.
			 * @param evt tree event information
			 */
			void onSelChanging( wxTreeEvent& evt );

		private:
			DECLARE_EVENT_TABLE()

			
		};
	}
}

#endif	//  __KOME_VIEW_RAW_DATA_TREE_CTRL_H__

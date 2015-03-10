/**
 * @file SampleTreeManager.h
 * @brief interfaces of SampleTreeManager class
 *
 * @author H.Parry
 * @date 2011.04.20
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation. All rights reserved.
 */


#ifndef __KOME_VIEW_RAW_DATA_TREE_MANAGER_H__
#define __KOME_VIEW_RAW_DATA_TREE_MANAGER_H__


#include <map>
#include <wx/treectrl.h>


namespace kome {
	namespace properties {

		class EditSampleUserPropertiesDialog;
		class SampleTreeCtrl;

		/**
		 * @class SampleTreeManager
		 * @brief sample tree control management class
		 */
		class SampleTreeManager : public kome::objects::DefaultDataManager {
		public:
			/**
			 * @fn SampleTreeManager()
			 * @brief constructor
			 */
			SampleTreeManager();

			/**
			 * @fn virtual ~SampleTreeManager()
			 * @brief destructor
			 */
			virtual ~SampleTreeManager();

		protected:
			/** tree control */
			SampleTreeCtrl* m_tree;

			/** sample tree item map ( sample -> tree item id ) */
			std::map< kome::objects::Sample*, wxTreeItemId > m_sampleMap;

			/** group tree item map ( group -> tree item id ) */
			std::map< kome::objects::DataGroupNode*, wxTreeItemId > m_groupMap;

			/** root item id */
			wxTreeItemId m_rootId;

		public:
			/**
			 * @fn void createTreeCtrl()
			 * @brief creates tree ctrl
			 */
			void createTreeCtrl(EditSampleUserPropertiesDialog* pWnd);

			/**
			 * @fn DetailsListCtrl* getTreeCtrl()
			 * @brief gets tree ctrl
			 * @return the pointer to tree ctrl
			 */
			SampleTreeCtrl* getTreeCtrl();

			/**
			 * @fn void addSample( kome::objects::Sample* sample )
			 * @brief adds sample
			 * @param sample sample object to be adde
			 */
			void addSample( kome::objects::Sample* sample );

			/**
			 * @fn void updateNodes( kome::objects::DataGroupNode* group )
			 * @brief updates nodes
			 * @param group group
			 */
			void updateNodes( kome::objects::DataGroupNode* group );
						
		protected:
			/**
			 * @fn wxTreeItemId addTreeItem( kome::objects::DataGroupNode* group, wxTreeItemId& parentId )
			 * @brief adds tree item
			 * @param group parent spectrum group
			 * @param parentId parent tree item ID
			 */
			wxTreeItemId addTreeItem( kome::objects::DataGroupNode* group, wxTreeItemId& parentId );

			/**
			 * @fn void addChildren( kome::objects::DataGroupNode* group, wxTreeItemId& groupId )
			 * @brief adds child nodes to tree item
			 * @param group group
			 * @param groupId group tree item ID
			 */
			void addChildren( kome::objects::DataGroupNode* group, wxTreeItemId& groupId );
		};
	}
}

#endif	// __KOME_VIEW_RAW_DATA_TREE_MANAGER_H__

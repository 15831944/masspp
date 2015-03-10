/**
 * @file SampleTreeData.h
 * @brief interfaces of SampleTreeData class
 *
 * @author H.Parry
 * @date 2011.04.20
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation. All rights reserved.
 */


#ifndef __KOME_VIEW_RAW_DATA_TREE_DATA_H__
#define __KOME_VIEW_RAW_DATA_TREE_DATA_H__

#include <wx/treectrl.h>

namespace kome {
	namespace properties {
		/**
		 * @class SampleTreeData
		 * @brief information object that sample tree item has
		 */
		class SampleTreeData : public wxTreeItemData {
		public:
			/**
			 * @fn SampleTreeData()
			 * @brief constructor
			 */
			SampleTreeData();

			/**
			 * @fn virtual ~SampleTreeData()
			 * @brief destructor
			 */
			virtual ~SampleTreeData();

		protected:
			/** data */
			kome::objects::Variant m_data;

		public:
			/**
			 * @fn kome::objects::Variant& getData()
			 * @brief gets data
			 * @return data object
			 */
			kome::objects::Variant& getData();
		};
	}
}

#endif	//  __KOME_VIEW_RAW_DATA_TREE_DATA_H__

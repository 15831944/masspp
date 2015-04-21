/**
 * @file RangeListSelector.h
 * @brief interfaces of RangeListSelector class
 *
 * @author S.Tanaka
 * @date 2009.01.06
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_CORE_RANGE_LIST_SELECTOR_H__
#define __KOME_CORE_RANGE_LIST_SELECTOR_H__


#include "Selector.h"
#include "RangeList.h"


namespace kome {
	namespace core {

		/**
		 * @class RangeListSelector
		 * @brief selector class
		 */
		class CORE_TOOLKITS_CLASS RangeListSelector : public Selector {
		public:
			/**
			 * @typedef RangeType
			 * @brief range type
			 */
			typedef enum {
				RANGE_INDEX,
				RANGE_INT_VALUE,
				RANGE_DOUBLE_VALUE
			} RangeType;

		public:
			/**
			 * @fn RangeListSelector(
						const RangeList::DefaultType dflt = RangeList::DEFAULT_NONE,
						const RangeType type = RANGE_INDEX
					)
			 * @brief constructor
			 * @param[in] dflt default data selection type
			 * @param[in] type range type
			 */
			RangeListSelector(
				const RangeList::DefaultType dflt = RangeList::DEFAULT_NONE,
				const RangeType type = RANGE_INDEX
			);

			/**
			 * @fn virtual ~RangeListSelector()
			 * @brief destructor
			 */
			virtual ~RangeListSelector();

		protected:
			/** range type */
			RangeType m_type;

			/** range list */
			RangeList m_ranges;

		public:
			/**
			 * @fn RangeList& getRangeList()
			 * @brief gets ranges list object
			 * @return range list
			 */
			RangeList& getRangeList();

		protected:
			/**
			 * @fn virtual bool onSelect(
					const unsigned int num,
					const char** items,
					const char** values,
					bool* selected
				)
			 * @brief This method is called by select method. (override method)
			 * @param[in] num the number of items
			 * @param[in] items the array of item names
			 * @param[in] values the array of item values
			 * @param[out] selected the array to store selected flags
			 * @return If true, the selection was normally done.
			 */
			virtual bool onSelect(
				const unsigned int num,
				const char** items,
				const char** values,
				bool* selected
			);
		};
	}
}


#endif		// __KOME_CORE_RANGE_LIST_SELECTOR_H__

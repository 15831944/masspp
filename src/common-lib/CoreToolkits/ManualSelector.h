/**
 * @file ManualSelector.h
 * @brief interfaces of ManualSelector class
 *
 * @author S.Tanaka
 * @date 2008.01.22
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_CORE_MANUAL_SELECTOR_H__
#define __KOME_CORE_MANUAL_SELECTOR_H__


#include "Selector.h"

#include <set>


namespace kome {
	namespace core {

		/**
		 * @class ManualSelector
		 * @brief selector class
		 */
		class CORE_TOOLKITS_CLASS ManualSelector : public Selector {
		public:
			/**
			 * @fn ManualSelector()
			 * @brief constructor
			 */
			ManualSelector();

			/**
			 * @fn virtual ~ManualSelector()
			 * @brief destructor
			 */
			virtual ~ManualSelector();

		protected:
			/** selected items */
			std::set< int > m_selectedItemSet;

		public:
			/**
			 * @fn void setSelected( const int item, const bool selected )
			 * @brief sets selection status
			 * @param[in] item item index
			 * @param[in] selected selection status
			 */
			void setSelected( const int item, const bool selected );

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


#endif		// __KOME_CORE_MANUAL_SELECTOR_H__

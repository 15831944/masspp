/**
 * @file MzRangeFilterManager.h
 * @brief interfaces of MzRangeFilterManager class
 *
 * @author M.Izumi
 * @date 2013.01.08
 * 
 * Copyright(C) 2013 Shimadzu Corporation. All rights reserved.
 */

#ifndef __KOME_MZRANGE_FILTER_MANAGER_H__
#define __KOME_MZRANGE_FILTER_MANAGER_H__

namespace kome {
	namespace mzfilter{
		/**
		 * @class MzRangeFilterManager
		 * @brief m/z Range Filter Manager class
		 */
		class MzRangeFilterManager : public kome::objects::DefaultDataManager {
		protected:
			/**
			 * @fn MzRangeFilterManager()
			 * @brief constructor
			 */
			MzRangeFilterManager();

			/**
			 * @fn virtual ~MzRangeFilterManager()
			 * @brief destructor
			 */
			virtual ~MzRangeFilterManager();

		public:
			/**
			 * @fn bool openDialog(kome::objects::Variant& activeObj)
			 * @bfief open m/z range filter dialog
			 */
			bool openDialog(kome::objects::Variant& activeObj);
		public:
			/**
			 * @fn static MzRangeFilterManager& getInstance()
			 * @brief get mz range filter plug-in management object
			 * @return mz range filter plug-in management object (This is the only object)
			 */
			static MzRangeFilterManager& getInstance();
		};

	}
}

#endif // __KOME_MZRANGE_FILTER_MANAGER_H__

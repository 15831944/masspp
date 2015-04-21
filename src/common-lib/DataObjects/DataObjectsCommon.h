/**
 * @file DataObjectsCommon.h
 * @brief DataObjects library common type
 *
 * @author S.Tanaka
 * @date 2007.03.06
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#ifndef __KOME_OBJECTS_COMMON_H__
#define __KOME_OBJECTS_COMMON_H__


namespace kome {
	namespace objects {

		/**
		 * @typedef SearchType
		 * @brief search type
		 */
		typedef enum {
			SEARCH_PREV,
			SEARCH_NEXT,
			SEARCH_NEAR
		} SearchType;
	}
}

#endif		// __KOME_OBJECTS_COMMON_H__

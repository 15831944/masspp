/**
 * @file CommonType.h
 * @brief common type definition
 *
 * @author S.Tanaka
 * @date 2007.03.06
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_CLR_COMMON_TYPE_H__
#define __KOME_CLR_COMMON_TYPE_H__


namespace kome {
	namespace clr {
		/**
		 * @class SearchType
		 * @brief search type enumeration
		 */
		public ENUM_CLASS SearchType {
			PREV,
			NEXT,
			NEAR
		};
	}
}


#endif		// __KOME_CLR_COMMON_TYPE_H__

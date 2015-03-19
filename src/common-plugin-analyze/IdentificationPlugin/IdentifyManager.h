/**
 * @file IdentifyManager.h
 * @brief interfaces of IdentifyManager class
 *
 * @author OKADA.H
 * @date 2013/07/29
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */



#ifndef __KOME_IDENTIFY_MANAGER_H__
#define __KOME_IDENTIFY_MANAGER_H__

namespace kome {
	namespace ident {
		
		
		/**
		 * @class IdentifyManager
		 * @brief matrix view manager
		 */
		class IdentifyManager {
		
		
		public:
			/**
			 * @fn static IdentifyManager& getInstance()
			 * @brief gets IdentifyManager object (This is the only object.)
			 * @return identify manager object
			 */
			static IdentifyManager& getInstance();

		};
	}
}

#endif

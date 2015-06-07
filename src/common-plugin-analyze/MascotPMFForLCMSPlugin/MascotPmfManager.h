/**
 * @file MascotPmfManager.h
 * @brief interfaces of MascotPmfManager class
 *
 * @author S.Tanaka
 * @date 2012.10.26
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_MASCOT_MASCOT_PMF_MANAGER_H__
#define __KOME_MASCOT_MASCOT_PMF_MANAGER_H__


#include "MascotPmfForLCMSSearchEngine.h"


namespace kome {
	namespace mascot {

		/**
		 * @class MascotPmfManager
		 * @brief mascot search management class
		 */
		class MascotPmfManager {
		protected:
			/**
			 * @fn MascotPmfManager()
			 * @brief constructor
			 */
			MascotPmfManager();

			/**
			 * @fn virtual ~MascotPmfManager()
			 * @brief destructor
			 */
			virtual ~MascotPmfManager();

		protected:
			/** PMF engine */
			MascotPmfForLCMSSearchEngine m_pmfEngine;

		public:
			/**
			 * @fn MascotSearchEngineBase* getPmfEngine()
			 * @brief gets the PMF search engine
			 * @return PMF search engine
			 */
			MascotSearchEngineBase* getPmfEngine();

		public:
			/**
			 * @fn static MascotPmfManager& getInstance()
			 * @brief gets object
			 * @return MascotPmfManager object. (This is the only object.)
			 */
			static MascotPmfManager& getInstance();
		};
	}
}

#endif	// __KOME_MASCOT_MASCOT_PMF_MANAGER_H__

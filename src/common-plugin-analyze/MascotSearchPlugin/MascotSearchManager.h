/**
 * @file MascotSearchManager.h
 * @brief interfaces of MascotSearchManager class
 *
 * @author S.Tanaka
 * @date 2012.10.26
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_MASCOT_MASCOT_SEARCH_MANAGER_H__
#define __KOME_MASCOT_MASCOT_SEARCH_MANAGER_H__


#include "MascotPmfSearchEngine.h"
#include "MascotMisSearchEngine.h"


namespace kome {
	namespace mascot {

		/**
		 * @class MascotSearchManager
		 * @brief mascot search management class
		 */
		class MascotSearchManager {
		protected:
			/**
			 * @fn MascotSearchManager()
			 * @brief constructor
			 */
			MascotSearchManager();

			/**
			 * @fn virtual ~MascotSearchManager()
			 * @brief destructor
			 */
			virtual ~MascotSearchManager();

		protected:
			/** PMF engine */
			MascotPmfSearchEngine m_pmfEngine;

			/** MIS engine */
			MascotMisSearchEngine m_misEngine;

		public:
			/**
			 * @fn MascotSearchEngineBase* getPmfEngine()
			 * @brief gets the PMF search engine
			 * @return PMF search engine
			 */
			MascotSearchEngineBase* getPmfEngine();

			/**
			 * @fn MascotSearchEngineBase* getMisEngine()
			 * @brief gets the MIS search engine
			 * @return MIS search engine
			 */
			MascotSearchEngineBase* getMisEngine();

		public:
			/**
			 * @fn static MascotSearchManager& getInstance()
			 * @brief gets object
			 * @return MascotSearchManager object. (This is the only object.)
			 */
			static MascotSearchManager& getInstance();
		};
	}
}

#endif	// __KOME_MASCOT_MASCOT_SEARCH_MANAGER_H__

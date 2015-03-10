/**
 * @file XTandemManager.h
 * @brief interfaces of XTandemManager class
 *
 * @author N.Tashiro
 * @date 2012.01.25
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation. All rights reserved.
 */


#ifndef __KOME_XTANDEM_XTANDEM_MANAGER_H__
#define __KOME_XTANDEM_XTANDEM_MANAGER_H__


#include "TandemSearchEngine.h"


namespace kome {
	namespace xtandem {
		/**
		 * @class XTandemManager
		 * @brief xtandem search management class
		 */
		class XTandemManager {
		protected:
			/**
			 * @fn XTandemManager()
			 * @brief constructor
			 */
			XTandemManager();

			/**
			 * @fn virtual ~XTandemManager()
			 * @brief destructor
			 */
			virtual ~XTandemManager();

		protected:
			/** X! Tandem Search Engine */
			TandemSearchEngine m_engine;

			/** result directory */
			std::string m_resultDir;

		public:
			/**
			 * @fn TandemSearchEngine* getEngine()
			 * @brief gets the X! Tandem search engine
			 * @return X! Tandem search engine
			 */
			TandemSearchEngine* getEngine();

			/**
			 * @fn const char* getResultDir()
			 * @brief gets the result directory 
			 * @return result directory
			 */
			const char* getResultDir();

		public:
			/**
			 * @fn void getResiduesFiles( std::vector< kome::plugin::SettingsValue::EnumItem >* enumeration )
			 * @brief gets the residues files
			 * @param[out] the array to store residues files
			 */
			void getResiduesFiles( std::vector< kome::plugin::SettingsValue::EnumItem >* enumeration );

		public:
			/**
			 * @fn static XTandemManager& getInstance()
			 * @brief gets object
			 * @return XTandemManager object. (This is the only object.)
			 */
			static XTandemManager& getInstance();
		};
	}
}

#endif	// __KOME_XTANDEM_XTANDEM_MANAGER_H__

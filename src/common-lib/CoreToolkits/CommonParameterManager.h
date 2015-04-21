/**
 * @file CommonParameterManager.h
 * @brief interfaces of common parameter manager class
 *
 * @author M.Izumi
 * @date 2012.02.22
 * 
 * Copyright (C) 2014 Shimadzu Corporation All rights reserved.
 */


#ifndef __KOME_CORE_COMMON_PARAMETER_MANAGER_H__
#define __KOME_CORE_COMMON_PARAMETER_MANAGER_H__

#include <string>
#include <vector>

namespace kome {
	namespace core {
		
		/**
		 * @class CommonParameterManager
		 * @brief common parameter manager class
		 */
		class CORE_TOOLKITS_CLASS CommonParameterManager {
		public:
			/**
			 * @fn CommonParameterManager()
			 * @brief constructor
			 */
			CommonParameterManager();

			/**
			 * @fn virtual ~CommonParameterManager()
			 * @brief destructor
			 */
			virtual ~CommonParameterManager();
		
		protected:
			/**
			 * @struct ParamInfo
			 * @brief parameter info
			 */
			struct ParamInfo{
				std::string paramName;
				std::string paramValue;
				bool tempFlg;

			};

			/** parameters info  */
			std::vector< ParamInfo >m_paramInfo;

		public:
			/**
			 * @fn int getNumberOfParameters()
			 * @brief gets the number of parameters variables
			 * @return the number of parameters variables
			 */
			int getNumberOfParameters();

			/**
			 * @fn std::string getParameterName( unsigned int index )
			 * @brief gets parameters variable name
			 * @param[in] index parameters variable index
			 * @return parameter name (If the index is illegal, this method returns NULL.)
			 */
			std::string getParameterName( unsigned int index );
			/**
			 * @fn void setParameter( std::string paramName, std::string paramValue, bool tempFlg )
			 * @brief sets parameter
			 * @param[in] paramName parameters name
			 * @param[in] paramValue parameters value
			 * @param[in] tempFlg temporary parameter flag
			 */
			void setParameter( std::string paramName, std::string paramValue, bool tempFlg );
			
			/**
			 * @fn std::string getParameter( std::string paramName, std::string defaultName )
			 * @brief gets parameter
			 * @param[in] paramName parameters name
			 * @param[in] defaultName parameters default name
			 * @return parameter value (If the parameter info doesn't have value for specified name or value is empty, this method returns defaultName.)
			 */
			std::string getParameter( std::string paramName, std::string defaultName );
			
			/**
			 * @fn bool isTemporaryParameter( unsigned int index )
			 * @brief is temporary parameter
			 * @param[in] index parameters variable index
			 * @return true:is temporary parameter / false:in not temporary parameter
			 */
			bool isTemporaryParameter( unsigned int index );
		
		private:
			/**
			 * @fn void setIniString( std::string strKey, std::string strValue )
			 * @brief The value of a Ini file is set. 
		     * @param[in] strKey The key of an ini file 
			 * @param[in] strValue The value of an ini file
			 */
			void setIniString( std::string strKey, std::string strValue );
		public:
			/**
			 * @fn static CommonParameterManager& getInstance()
			 * @brief gets common parameter management object.
			 * @return common parameter object. (This is the only object.)
			 */
			static CommonParameterManager& getInstance();
		
			
		};

	}
}

#endif // __KOME_CORE_COMMON_PARAMETER_MANAGER_H__

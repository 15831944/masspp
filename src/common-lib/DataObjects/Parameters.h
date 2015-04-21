/**
 * @file Parameters.h
 * @brief interfaces of Parameters class
 *
 * @author S.Tanaka
 * @date 2006.08.25
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#ifndef __KOME_OBJECTS_PARAMETERS_H__
#define __KOME_OBJECTS_PARAMETERS_H__

#include "Variant.h"

#include <string>
#include <vector>
#include <map>
#include <boost/function.hpp>

namespace kome {
	namespace objects {

		/**
		 * @class Parameters
		 * @brief parameters of plug-in function management class
		 */
		class DATA_OBJECTS_CLASS Parameters {
		public:
			/**
			 * @fn Parameters()
			 * @brief constructor
			 */
			Parameters();

			/**
			 * @fn virtual ~Parameters()
			 * @brief destructor
			 */
			virtual ~Parameters();

			/**
			 * @fn unsigned int getNumberOfParameters()
			 * @brief gets the number of parameters
			 * @return the number of parameters
			 */
			unsigned int getNumberOfParameters();

			/**
			 * @fn const char* getName( unsigned int index )
			 * @brief gets parameter name
			 * @param[in] index parameter index
			 * @return parameter name (If index is illegal, this method returns NULL.)
			 */
			const char* getName( unsigned int index );

			/**
			 * @fn Variant* getValue( const char* name )
			 * @brief gets parameter value
			 * @param[in] name parameter name
			 * @return parameter value.
			 */
			Variant& getValue( const char* name );

			/**
			 * @fn bool hasParameter( const char* name )
			 * @brief gets wheter there is parameter with the specified name.
			 * @return If there is the parameter with the specified name, this method returns true.
			 */
			bool hasParameter( const char* name );

		protected:
			/** the array of parameter name */
			std::vector< std::string > m_parameters;

			/** the map of parameter value (parameter name -> parameter value) */
			std::map< std::string, Variant > m_parameterMap;
		};
	}
}

// type definition
typedef boost::function< kome::objects::Variant ( kome::objects::Parameters* ) > PLUGIN_FUNCTION;

#endif	// __KOME_OBJECTS_PARAMETERS_H__

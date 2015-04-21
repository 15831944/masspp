/**
 * @file ClrParameters.h
 * @brief interfaces of ClrParameters class
 *
 * @author S.Tanaka
 * @date 2006.08.28
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#ifndef __KOME_CLR_PARAMETERS_H__
#define __KOME_CLR_PARAMETERS_H__

#include "ClrVariant.h"

namespace kome {
	namespace clr {
		/**
		 * @class ClrParameters
		 * @brief parameters of plug-in function on CLR environment
		 */
		public ref class ClrParameters {
		public:
			/**
			 * @fn ClrParameters()
			 * @brief constructor
			 */
			ClrParameters();

			/**
			 * @fn ClrParameters( kome::objects::Parameters& params )
			 * @brief constructor
			 * @param params parameter information
			 */
			ClrParameters( kome::objects::Parameters& params );

			/**
			 * @fn virtual ~ClrParameters()
			 * @brief destructor
			 */
			virtual ~ClrParameters();

			/**
			 * @fn unsigned int getNumberOfParameters()
			 * @brief gets the number of parameters
			 * @return the number of parameters
			 */
			unsigned int getNumberOfParameters();

			/**
			 * @fn System::String^ getParameterKey( unsigned int index )
			 * @brief gets the parameter key name
			 * @param index parameter index
			 * @return parameter key name (If index is illegal number, this method returns nullptr.)
			 */
			System::String^ getParameterKey( unsigned int index );

			/**
			 * @fn ClrVariant^ getValue( System::String^ key )
			 * @brief gets the parameter value
			 * @param key parameter key
			 * @return parameter value (If there isn't parameter that has specified key, this method returns nullptr.)
			 */
			ClrVariant^ getValue( System::String^ key );

			/**
			 * @fn bool hasParameter( System::String^ key )
			 * @brief gets wheter there is parameter with the specified name.
			 * @return If there is the parameter with the specified name, this method returns true.
			 */
			bool hasParameter( System::String^ key );


		protected:
			/** parameter hash table ( parameter name -> parameter value ) */
			System::Collections::Hashtable^ m_parameters;

			/** the array of parameter keys */
			System::Collections::ArrayList^ m_keys;

		public:
			/** 
			 * @fn void setParameters( kome::objects::Parameters& params )
			 * @brief sets parameters
			 * @param params parameters information object
			 */
			void setParameters( kome::objects::Parameters& params );

			/**
			 * @fn void getParameters( kome::objects::Parameters& params )
			 * @brief gets parameters
			 * @param params parameters object to be stored
			 */
			void getParameters( kome::objects::Parameters& params );
		};
	}
}

#endif	// __KOME_CLR_PARAMETERS_H__

/**
 * @file GcmsSampleSet.h
 * @brief interface of AIASampleSet class
 *
 * @author Y.Fujita
 * @date 2012.07.20
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation All rights reserved.
 */


#ifndef __KOME_IO_AIA_MANAGER_H__
#define __KOME_IO_AIA_MANAGER_H__



#include <windows.h>


namespace kome {
	namespace io {
		namespace aia {

			class AIASampleSet;

			/** 
			 * @class AIAManager
			 * @brief AIA input manager class
			 */
			class AIAManager {
			protected:
				/**
				 * @fn AIAManager()
				 * @brief constructor
				 */
				AIAManager();

				/**
				 * @fn virtual ~AIAManager()
				 * @brief destructor
				 */
				virtual ~AIAManager();

			protected:
				/**
				 * @typedef ConvertFun
				 * @brief convert function type
				 */
				typedef int (WINAPI *ConvertFun)( char*, char* );

				/** dll module */
				HMODULE m_module;

				/** function */
				ConvertFun m_fun;

			public:
				/**
				 * @fn void convert( AIASampleSet* sampleSet )
				 * @brief convet AIA -> CDF
				 * @param sampleSet AIA sample set object
				 */
				void convert( AIASampleSet* sampleSet );

			public:
				/**
				 * @fn static AIAManager& getInstance()
				 * @brief gets AIA manager object
				 * @return AIA manager object (This is the only object.)
				 */
				static AIAManager& getInstance();
			};
		}
	}
}


#endif		//__KOME_IO_AIA_MANAGER_H__

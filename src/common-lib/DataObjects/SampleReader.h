/**
 * @file SampleReader.h
 * @brief Sample Reader
 * @author OKADA, H.
 * @date 2012/07/18
 * Copyright (C) 2014 Shimadzu Corporation All rights reserved.
 */

#ifndef _SAMPLE_READER_
#define _SAMPLE_READER_

#include "stdafx.h"
#include "Sample.h"

namespace kome {
	namespace objects {
		
		/** 
		 * @class SampleReader
		 * @ brief Sample Reader class
		 */
		class DATA_OBJECTS_CLASS SampleReader{
		public:
			/**
			 * @fn SampleReader()
			 * @brief constructor
			 */
			SampleReader();
			
			/**
			 * @fn ~SampleReader()
			 * @brief destructor
			 */
			virtual ~SampleReader();

		public:

			/**
			 * @fn std::string selectKeys( void )
			 * @brief select keys
			 * @return key
			 */
			std::string selectKeys( void );

			/**
			 * @fn kome::objects::Sample* openData( std::string strKey )
			 * @brief open data
			 * @param strKey
			 * @return sample object
			 */
			kome::objects::Sample* openData( std::string strKey );


		protected:

			/**
			 * @fn virtual std::string onSelectKeys( void ) = 0
			 * @brief on select keys
			 */
			virtual std::string onSelectKeys( void ) = 0;

			/**
			 * @fn virtual kome::objects::Sample* onOpenData( std::string strKey ) = 0
			 * @brief on open data
			 * @param strKey
			 * @return sample object
			 */
			virtual kome::objects::Sample* onOpenData( std::string strKey ) = 0;

		};
	}
}

#endif // _SAMPLE_READER_

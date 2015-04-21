/**
* @file SampleReaderWrapper.h
* @brief Sample Reader Wrapper
* @author OKADA, H.
* @date 2012/07/18
* Copyright (C) 2014 Shimadzu Corporation All rights reserved.
*/


#ifndef _SAMPLE_READER_WRAPPER_
#define _SAMPLE_READER_WRAPPER_

#include "stdafx.h"
#include "SampleWrapper.h"

namespace kome {
	namespace clr {

		class PluginInfo;

		/** 
		 * @class StatusBarInfo
		 * @ brief Status Bar class
		 */
		public ref class SampleReaderWrapper{
			
		public:
			/**
			 * @fn SampleReaderWrapper();
			 * @brief constructor
			 * @param[in] sample_reader setting parameter values object to be wrapped
			 */
			SampleReaderWrapper( kome::objects::SampleReader& sample_reader );
			
			/**
			 * @fn ~SampleReaderWrapper();
			 * @brief destructor
			 */
			virtual ~SampleReaderWrapper();
			
		protected:
			/** Sample reader to be wrapped */
			kome::objects::SampleReader& m_sampleReader;

		public:

			/**
			 * @fn kome::objects::SampleReader& getSampleReader()
			 * @brief get sample reader
			 * @return sample reader
			 */
			kome::objects::SampleReader& getSampleReader();

			/**
			 * @fn System::String^ selectKeys( void )
			 * @brief select keys
			 */
			System::String^ selectKeys( void );

			/**
			 * @fn kome::clr::SampleWrapper^ openData( System::String^ strKey )
			 * @brief open data
			 * @param strKey key to open data
			 */
			kome::clr::SampleWrapper^ openData( System::String^ strKey );

		};
	}
}


#endif

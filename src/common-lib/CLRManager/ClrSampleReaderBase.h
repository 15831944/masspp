/**
* @file ClrSampleReaderBase.h
* @brief Clr Sample Reader Base
* @author OKADA, H.
* @date 2012/07/18
* Copyright (C) 2014 Shimadzu Corporation All rights reserved.
*/


#ifndef _CLR_SAMPLE_READER_BASE_
#define _CLR_SAMPLE_READER_BASE_

#include "stdafx.h"
#include "SampleReaderWrapper.h"

namespace kome {
	namespace clr {

		ref class SampleWrapper;
		
		/** 
		 * @class ClrSampleReaderBase
		 * @ brief Clr Sample Reader Base class
		 */
		public ref class ClrSampleReaderBase abstract : public SampleReaderWrapper{

		public:			
			/**
			 * @fn ClrSampleReaderBase();
			 * @brief constructor
			 */
			ClrSampleReaderBase();
			
			/**
			 * @fn ~ClrSampleReaderBase();
			 * @brief destructor
			 */
			virtual ~ClrSampleReaderBase();
	
		public:

			/**
			 * @fn virtual System::String^ onSelectKeys( void ) = 0
			 * @brief Abstract method to select a key.
			 */
			virtual System::String^ onSelectKeys( void ) = 0;

			/**
			 * @fn virtual kome::clr::SampleWrapper^ onOpenData( System::String^ strKey ) = 0
			 * @brief Abstract method to open the sample as an argument key.
			 * @param strKey
			 */
			virtual kome::clr::SampleWrapper^ onOpenData( System::String^ strKey ) = 0;

		};
	}
}


#endif

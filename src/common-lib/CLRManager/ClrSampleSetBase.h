/**
 * @file ClrSampleSetBase.h
 * @brief interfaces of ClrSampleSetBase class
 *
 * @author S.Tanaka
 * @date 2008.01.07
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_CLR_SAMPLE_SET_BASE_H__
#define __KOME_CLR_SAMPLE_SET_BASE_H__


#include "SampleSetWrapper.h"
#include "ProgressWrapper.h" 

namespace kome {
	namespace clr {

		ref class SampleWrapper;
		ref class ProgressWrapper;

		/**
		 * @class ClrSampleSetBase
		 * @brief abstract class of spectrum information on CLR environment
		 */
#ifndef _MANAGED	// (for doxygen)
		public ref class ClrSampleSetBase : public SampleSetWrapper {
#else
		public ref class ClrSampleSetBase abstract : public SampleSetWrapper {
#endif	// _MANAGED
		public:
			/**
			 * @fn ClrSampleSetBase()
			 * @brief constructor
			 */
			ClrSampleSetBase();

			/**
			 * @fn virtual ~ClrSampleSetBase()
			 * @brief destructor
			 */
			virtual ~ClrSampleSetBase();

		public:
			/**
			 * @fn virtual bool onOpenFile( System::String^ path ) = 0
			 * @brief This method is called by openFile method. (abstract method)
			 * @param[in] path file path
			 * @param[in] progress
			 * @return If true, it succeeded to open the file
			 */
			virtual bool onOpenFile( System::String^ path ) = 0;

			/**
			 * @fn virtual bool onCloseFile() = 0
			 * @brief This method is called by closeFile method. (abstract method)
			 * @return If true, it succeeded to close the file
			 */
			virtual bool onCloseFile() = 0;

			/**
			 * @fn virtual bool onOpenSample( SampleWrapper^ sample ) = 0
			 * @brief This method is called by openSample method. (abstract method)
			 * @param[in,out] sample sample object to be opened (If NULL, opens all samples.)
			 * @param[in] progress
			 * @return If true, it succeeded to open the sample.
			 */
			virtual bool onOpenSample( SampleWrapper^ sample ) = 0;

			/**
			 * @fn virtual bool onCloseSample( SampleWrapper^ sample ) = 0
			 * @brief This method is called by closeSample method. (abstract method)
			 * @param[in,out] sample sample object to be closed (If NULL, closes all samples.)
			 * @return If true, it succeeded to close the sample.
			 */
			virtual bool onCloseSample( SampleWrapper^ sample ) = 0;
		};
	}
}

#endif		// __KOME_CLR_SAMPLE_SET_BASE_H__

/**
 * @file ClrDataManagerBase.h
 * @brief interfaces of ClrDataManagerBase class
 *
 * @author S.Tanaka
 * @date 2007.12.28
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_CLR_DATA_MANAGER_BASE_H__
#define __KOME_CLR_DATA_MANAGER_BASE_H__


#include "DataManagerWrapper.h"
#include "ClrDataManager.h"


namespace kome {
	namespace clr {

		ref class SampleWrapper;
		ref class SpectrumWrapper;
		ref class ChromatogramWrapper;
		ref class DataMapInfoWrapper;

		/**
		 * @class ClrDataManagerBase
		 * @brief abstract class of spectrum information on CLR environment
		 */
#ifndef _MANAGED	// (for doxygen)
		public ref class ClrDataManagerBase : public DataManagerWrapper {
#else
		public ref class ClrDataManagerBase abstract : public DataManagerWrapper {
#endif	// _MANAGED
		public:
			/**
			 * @fn ClrDataManagerBase()
			 * @brief constructor
			 */
			ClrDataManagerBase();

			/**
			 * @fn virtual ~ClrDataManagerBase()
			 * @brief destructor
			 */
			virtual ~ClrDataManagerBase();

		public:
			/**
			 * @fn virtual void onOpenSample( SampleWrapper^ sample ) = 0
			 * @brief This method is called when a sample is opened. (abstract method)
			 * @param[in,out] sample sample object to be opened
			 */
			virtual void onOpenSample( SampleWrapper^ sample ) = 0;

			/**
			 * @fn virtual void onCloseSample( SampleWrapper^ sample, bool deleting ) = 0
			 * @brief This method is called when a sample is closed. (abstract method)
			 * @param[in,out] sample sample object to be closed
			 * @param[in] deleting If true, the specified object is being deleted now.
			 */
			virtual void onCloseSample( SampleWrapper^ sample, bool deleting ) = 0;

			/**
			 * @fn virtual void onOpenSpectrum( SpectrumWrapper^ spec ) = 0
			 * @brief This method is called when a spectrum is opened. (abstract method)
			 * @param[in,out] spec spectrum object to be opened
			 */
			virtual void onOpenSpectrum( SpectrumWrapper^ spec ) = 0;

			/**
			 * @fn virtual void onCloseSpectrum( SpectrumWrapper^ spec, bool deleting ) = 0
			 * @brief This method is called when a spectrum is closed. (abstract method)
			 * @param[in,out] spec spectrum object to be opened
			 * @param[in] deleting If true, the specified object is being deleted now.
			 */
			virtual void onCloseSpectrum( SpectrumWrapper^ spec, bool deleting ) = 0;

			/**
			 * @fn virtual void onOpenChromatogram( ChromatogramWrapper^ chrom ) = 0
			 * @brief This method is called when a chromatogram is opened. (abstract method)
			 * @param[in,out] chrom chromatogram object to be opened
			 */
			virtual void onOpenChromatogram( ChromatogramWrapper^ chrom ) = 0;

			/**
			 * @fn virtual void onCloseChromatogram( ChromatogramWrapper^ chrom, bool deleting ) = 0
			 * @brief This method is called when a chromatogram is closed. (abstract method)
			 * @param[in,out] chrom chromatogram object to be closed
			 * @param[in] deleting If true, the specified object is being deleted now.
			 */
			virtual void onCloseChromatogram( ChromatogramWrapper^ chrom, bool deleting ) = 0;

			/**
			 * @fn virtual void onOpenDataMap( DataMapInfoWrapper^ dataMap ) = 0
			 * @brief This method is called when a data map is opened. (abstract method)
			 * @param[in,out] dataMap data map object to be opened
			 */
			virtual void onOpenDataMap( DataMapInfoWrapper^ dataMap ) = 0;

			/**
			 * @fn virtual void onCloseDataMap( DataMapInfoWrapper^ dataMap, bool deleting ) = 0
			 * @brief This method is called when a data map is closed. (abstract method)
			 * @param[in,out] dataMap data map object to be closed
			 * @param[in] deleting If true, the specified object is being deleted now.
			 */
			virtual void onCloseDataMap( DataMapInfoWrapper^ dataMap, bool deleting ) = 0;
		};
	}
}

#endif

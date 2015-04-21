/**
 * @file ClrSampleBase.h
 * @brief interfaces of ClrSampleBase class
 *
 * @author S.Tanaka
 * @date 2008.01.07
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_CLR_SAMPLE_BASE_H__
#define __KOME_CLR_SAMPLE_BASE_H__


#include "SampleWrapper.h"


namespace kome {
	namespace clr {

		ref class SampleSetWrapper;
		ref class DataGroupNodeWrapper;
		ref class SpectrumWrapper;
		ref class PeaksWrapper;
		ref class ProgressWrapper;
		
		/**
		 * @class ClrSampleBase
		 * @brief abstract class of spectrum information on CLR environment
		 */
#ifndef _MANAGED	// (for doxygen)
		public ref class ClrSampleBase : public SampleWrapper {
#else
		public ref class ClrSampleBase abstract : public SampleWrapper {
#endif	// _MANAGED
		public:
			/**
			 * @fn ClrSampleBase( SampleSetWrapper^ sampleSet )
			 * @brief constructor
			 * @param[in] sampleSet sample set object that has this sample
			 */
			ClrSampleBase( SampleSetWrapper^ sampleSet );

			/**
			 * @fn virtual ~ClrSampleBase()
			 * @brief destructor
			 */
			virtual ~ClrSampleBase();

		public:
			/**
			 * @fn virtual void onDetectPeaksByAPI( SpectrumWrapper^ spec, PeaksWrapper^ peaks )
			 * @brief detect peaks using Application Programming Interface
			 * @param[in] spec spectrum to detect peaks
			 * @param[out] peaks the object to store peaks information.
			 */
			virtual void onDetectPeaksByAPI( SpectrumWrapper^ spec, PeaksWrapper^ peaks );

		public:
			/**
			 * @fn virtual bool onOpenSample( DataGroupNodeWrapper^ rootGroup )
			 * @brief This method is called by openSample method. (abstract method)
			 * @param[out] rootGroup root spectrum group
			 * @return If true, it succeeded to open this sample.
			 */
			virtual bool onOpenSample( DataGroupNodeWrapper^ rootGroup ) = 0;

			/**
			 * @fn virtual bool onCloseSample()
			 * @brief This method is called by closeSample method. (abstract method)
			 * @return If true, it succeeded to close this sample.
			 */
			virtual bool onCloseSample() = 0;

		protected:
			/**
			 * @fn void setSupportedAPIPeaks( bool supported )
			 * @brief sets API peak dtection is supported on this sample.
			 * @param[in] supported If true, API peak detection is supported.
			 */
			void setSupportedAPIPeaks( bool supported );
		};
	}
}

#endif		// __KOME_CLR_SAMPLE_BASE_H__

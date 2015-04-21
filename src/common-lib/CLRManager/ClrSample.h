/**
 * @file ClrSample.h
 * @brief interfaces of ClrSample class
 *
 * @author S.Tanaka
 * @date 2008.01.07
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_CLR_SAMPLE_H__
#define __KOME_CLR_SAMPLE_H__


#include "ClrSampleBase.h"


namespace kome {
	namespace clr {

		/**
		 * @class ClrSample
		 * @brief Sample class that has managed class object
		 */
		class ClrSample : public kome::objects::Sample {
		public:
			/**
			 * @fn ClrSample( SampleSetWrapper^ sampleSet )
			 * @brief constructor
			 * @param[in] sampleSet sample set object that has this sample
			 */
			ClrSample( SampleSetWrapper^ sampleSet );

			/**
			 * @fn virtual ~ClrSample()
			 * @brief destructor
			 */
			virtual ~ClrSample();

		protected:
			/** base spectrum object */
			gcroot< ClrSampleBase^ > m_baseSample;

		public:
			/**
			 * @fn void setBaseSample( ClrSampleBase^ baseSample )
			 * @brief sets base spectrum object
			 * @param[in] baseSample base spectrum object
			 */
			void setBaseSample( ClrSampleBase^ baseSample );

			/**
			 * @fn ClrSampleBase^ getBaseSample()
			 * @brief gets base spectrum object
			 * @return base spectrum object
			 */
			ClrSampleBase^ getBaseSample();

		protected:
			/**
			 * @fn virtual void onDetectPeaksByAPI( kome::objects::Spectrum* spec, kome::objects::Peaks* peaks )
			 * @brief detect peaks using Application Programming Interface (override method)
			 * @param[in] spec spectrum to detect peaks
			 * @param[out] peaks the object to store peaks information
			 */
			virtual void onDetectPeaksByAPI( kome::objects::Spectrum* spec, kome::objects::Peaks* peaks );

		protected:
			/**
			 * @fn virtual bool onOpenSample( kome::objects::DataGroupNode* rootGroup )
			 * @brief This method is called by openSample method. (override method)
			 * @param[out] rootGroup root spectrum group
			 * @param[in,out] progress progress bar
			 * @return If true, it succeeded to open this sample.
			 */
			virtual bool onOpenSample( kome::objects::DataGroupNode* rootGroup, kome::core::Progress* progress = NULL );

			/**
			 * @fn virtual bool onCloseSample()
			 * @brief This method is called by closeSample method. (override method)
			 * @return If true, it succeeded to close this sample.
			 */
			virtual bool onCloseSample();
		};
	}
}

#endif	// __KOME_CLR_SAMPLE_H__

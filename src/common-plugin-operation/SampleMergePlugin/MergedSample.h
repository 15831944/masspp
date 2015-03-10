/**
 * @file MergedSample.h
 * @brief interfaces of MergedSample class
 *
 * @author S.Tanaka
 * @date 2009.03.03
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_SAMPLE_MERGE_SAMPLE_H__
#define __KOME_SAMPLE_MERGE_SAMPLE_H__

namespace kome {
	namespace merged {

		class MergedSampleSet;
		class MergedSpectrum;

		/**
		 * @class MergedSample
		 * @brief composition data object class
		 */
		class MergedSample : public kome::objects::Sample {
		public:
			/**
			 * @fn MergedSample( MergedSampleSet* sampleSet )
			 * @brief constructor
			 * @param sampleSet sample set object
			 */
			MergedSample( MergedSampleSet* sampleSet );

			/**
			 * @fn virtual ~MergedSample()
			 * @brief destructor
			 */
			virtual ~MergedSample();

		protected:
			/** sample set */
			MergedSampleSet* m_sampleSet;

		public:
			/**
			 * @fn MergedSampleSet* getMergedSampleSet()
			 * @brief gets SAMPLE_MERGE sample set object
			 * @return SAMPLE_MERGE sample set object
			 */
			MergedSampleSet* getMergedSampleSet();

		protected:
			/**
			 * @fn virtual bool onOpenSample( DataGroupNode* rootGroup )
			 * @brief This method is called by openTreatment method. (override method)
			 * @param rootGroup root spectrum group
			 * @return If true, it succeeded to open this treatment.
			 */
			virtual bool onOpenSample( kome::objects::DataGroupNode* rootGroup, kome::core::Progress* progress = NULL );

			/**
			 * @fn virtual bool onCloseSample()
			 * @brief This method is called by closeTreatment method. (override method)
			 * @return If true, it succeeded to close this treatment.
			 */
			virtual bool onCloseSample();

		protected:
			/**
			 * @fn static bool lessSpec( kome::objects::Spectrum* s0, kome::objects::Spectrum* s1 )
			 * @brief compares spectra to sort
			 * @param s0 spectrum object to be compared
			 * @param s1 spectrum object to compare
			 * @return If true, s0 sorts before s1.
			 */
			static bool lessSpec( kome::objects::Spectrum* s0, kome::objects::Spectrum* s1 );
		};
	}
}


#endif		// __KOME_SAMPLE_MERGE_SAMPLE_H__

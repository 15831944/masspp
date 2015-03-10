/**
 * @file MergedSpectrum.h
 * @brief interfaces of MergedSpectrum class
 *
 * @author S.Tanaka
 * @date 2009.03.03
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_SAMPLE_MERGE_SPECTRUM_H__
#define __KOME_SAMPLE_MERGE_SPECTRUM_H__


namespace kome {
	namespace merged {

		class MergedSample;

		/**
		 * @class MergedSpectrum
		 * @brief spectrum of composition sample class
		 */
		class MergedSpectrum : public kome::objects::AveragedSpectrum {
		public:
			/**
			 * @fn MergedSpectrum( MergedSample& sample )
			 * @brief constructor
			 * @param sample composition sample object
			 */
			MergedSpectrum( MergedSample& sample );

			/**
			 * @fn virtual ~MergedSpectrum()
			 * @brief destructor
			 */
			virtual ~MergedSpectrum();
		};
	}
}


#endif		// __KOME_SAMPLE_MERGE_SPECTRUM_H__

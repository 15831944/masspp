/**
 * @file MergedSpectrum.h
 * @brief interfaces of MergedSpectrum class
 *
 * @author S.Tanaka
 * @date 2009.01.05
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_OPERATION_MERGED_SPECTRUM_H__
#define __KOME_OPERATION_MERGED_SPECTRUM_H__


namespace kome {
	namespace operation {
		namespace merged {

			/**
			 * @class MergedSpectrum
			 * @brief merged spectrum class
			 */
			class MergedSpectrum : public kome::objects::AveragedSpectrum {
			public:
				/**
				 * @fn MergedSpectrumManager( kome::objects::DataGroupNode* group );
				 * @brief constructor
				 * @param group data group
				 */
				MergedSpectrum( kome::objects::DataGroupNode* group );

				/**
				 * @fn virtual ~MergedSpectrum()
				 * @brief destructor
				 */
				virtual ~MergedSpectrum();

			protected:
				/**
				 * @fn virtual void onGetXYData( kome::core::XYData* const xyData, const double minX, const double maxX )
				 * @brief This method is called by getXYData method. (override method)
				 * @param xyData the object to store data points
				 * @param minX minimum x value. (If minX is negative number, minimum x value is not unbounded.)
				 * @param maxX maximum x value. (If maxX is negative number, maximum x value is not unbounded.)
				 */
				virtual void onGetXYData( kome::core::XYData* const xyData, const double minX, const double maxX );
			};
		}
	}
}


#endif	// __KOME_OPERATION_MERGED_SPECTRUM_H__

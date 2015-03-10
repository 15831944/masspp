/**
 * @file LocalMaximumLabeling.h
 * @brief interfaces of LocalMaximumLabeling class
 *
 * @author S.Tanaka
 * @date 2006.10.11
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_LABELING_LOCAL_MAXIMUM_LABELING_H__
#define __KOME_LABELING_LOCAL_MAXIMUM_LABELING_H__


namespace kome {
	namespace labeling {
		namespace local {
			/**
			 * @class LocalMaximumLabeling
			 * @brief local peak detect class
			 */
			class LocalMaximumLabeling {
			public:
				/**
				 * @fn LocalMaximumLabeling()
				 * @brief constructor
				 */
				LocalMaximumLabeling();

				/**
				 * @fn virtual ~LocalMaximumLabeling()
				 * @brief destructor
				 */
				virtual ~LocalMaximumLabeling();

			public:
				/**
				 * @fn void pickPeaks( kome::core::XYData& xyData, kome::objects::Peaks& peaks )
				 * @brief pick peaks
				 * @param xyData data points
				 * @param peaks peak object to store peak data
				 */
				void pickPeaks( kome::core::XYData& xyData, kome::objects::Peaks& peaks );
			};
		}
	}
}


#endif	// __KOME_LABELING_LOCAL_MAXIMUM_LABELING_H__

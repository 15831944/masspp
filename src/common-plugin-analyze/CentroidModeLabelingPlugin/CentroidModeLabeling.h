/**
 * @file CentroidModeLabeling.h
 * @brief interfaces of CentroidModeLabeling class
 *
 * @author S.Tanaka
 * @date 2006.10.14
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#ifndef __KOME_LABELING_CENTROID_MODE_LABELING_H__
#define __KOME_LABELING_CENTROID_MODE_LABELING_H__


namespace kome {
	namespace labeling {
		namespace centroid {

			/**
			 * @class CentroidModeLabeling
			 * @brief all data labeling manager class
			 */
			class CentroidModeLabeling {
			public:
				/**
				 * @fn CentroidModeLabeling()
				 * @brief constructor
				 */
				CentroidModeLabeling();

				/**
				 * @fn virtual ~CentroidModeLabeling()
				 * @brief destructor
				 */
				virtual ~CentroidModeLabeling();

			public:
				/**
				 * @fn void getPeaks(
						kome::core::XYData& xyData,
						kome::objects::Peaks& peaks
					)
				 * @brief gets peaks
				 * @param xyData data points
				 * @param peaks peaks object to store peaks
				 */
				void getPeaks(
					kome::core::XYData& xyData,
					kome::objects::Peaks& peaks
				);
			};
		}
	}
}

#endif	// __KOME_LABELING_CENTROID_MODE_LABELING_H__

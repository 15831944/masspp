/**
 * @file GaussianWindowFilter.h
 * @brief interfaces of gaussianwindowfilter class extends WindowFilter
 *
 * @author M.Fukuda
 * @date 2010.06.08
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#ifndef __KOME_FILTER_GAUSSIAN_WINDOW_FILTER_H__
#define __KOME_FILTER_GAUSSIAN_WINDOW_FILTER_H__

#include "stdafx.h"
#include "WindowFilter.h"

namespace kome {
	namespace filter {
		namespace window {
			/**
			 * @class GaussianWindowFilter
			 * @brief gaussian window filter
			 */
			class GaussianWindowFilter : public WindowFilter {
			public:
				/**
				 * @fn GaussianWindowFilter()
				 * @brief constructor
				 * @param params parameters object
				 */
				GaussianWindowFilter(kome::objects::Parameters* params);

				/**
				 * @fn ~GaussianWindowFilter()
				 * @brief destructor
				 */
				~GaussianWindowFilter();

			protected:

				/**
				 * @fn getWeight(double x)
				 * @brief getWeight(overwrite WindowFilter method)
				 * @param x Length from Target Point
				 */
				double getWeight(double x);

			private:

				/**
				 * @fn setOtherStatus()
				 * @ brief setStatus : m_sigma
				 */
				void setOtherStatus();

				// sigma
				double m_sigma;
			};
		}
	}
}

#endif //__KOME_FILTER_GAUSSIAN_WINDOW_FILTER_H__

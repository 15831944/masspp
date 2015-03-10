/**
 * @file RectangularWindowFilter.h
 * @brief interfaces of rectangularwindowfilter class extends WindowFilter
 *
 * @author M.Fukuda
 * @date 2010.06.04
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_FILTER_RECTANGULAR_WINDOW_FILTER_H__
#define __KOME_FILTER_RECTANGULAR_WINDOW_FILTER_H__

#include "stdafx.h"
#include "WindowFilter.h"

namespace kome {
	namespace filter {
		namespace window {
			/**
			 * @class RectangulaWindowFilter
			 * @brief rectangula window filter
			 */
			class RectangulaWindowFilter : public WindowFilter {
			public:
				/**
				 * @fn RectangulaWindowFilter()
				 * @brief constructor
				 * @param params parameters object
				 */
				RectangulaWindowFilter(kome::objects::Parameters* params);

				/**
				 * @fn ~RectangulaWindowFilter()
				 * @brief destructor
				 */
				~RectangulaWindowFilter();

			protected:

				/**
				 * @fn getWeight(double x)
				 * @brief getWeight(overwrite WindowFilter method)
				 * @param x Length from Target Point
				 */
				double getWeight(double x);
			};

		}
	}
}

#endif //__KOME_FILTER_RECTANGULAR_WINDOW_FILTER_H__

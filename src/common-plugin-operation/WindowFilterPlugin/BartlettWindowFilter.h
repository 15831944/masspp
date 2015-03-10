/**
 * @file BartlettWindowFilter.h
 * @brief interfaces of bartlettwindowfilter class extends WindowFilter
 *
 * @author M.Fukuda
 * @date 2010.06.08
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#ifndef __KOME_FILTER_BARTLETT_WINDOW_FILTER_H__
#define __KOME_FILTER_BARTLETT_WINDOW_FILTER_H__

#include "stdafx.h"
#include "WindowFilter.h"

namespace kome {
	namespace filter {
		namespace window {
			/**
			 * @class BartlettWindowFilter
			 * @brief bartlett window filter
			 */
			class BartlettWindowFilter : public WindowFilter {
			public:
				/**
				 * @fn BartlettWindowFilter()
				 * @brief constructor
				 * @param params parameters object
				 */
				BartlettWindowFilter(kome::objects::Parameters* params);

				/**
				 * @fn ~BartlettWindowFilter()
				 * @brief destructor
				 */
				~BartlettWindowFilter();

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

#endif //__KOME_FILTER_BARTLETT_WINDOW_FILTER_H__

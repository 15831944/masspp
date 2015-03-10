/**
 * @file WindowFilter.h
 * @brief interfaces of WindowFilter class
 *
 * @author M.Fukuda
 * @date 2010.6.4
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#ifndef __KOME_FILTER_WINDOW_FILTER_H__
#define __KOME_FILTER_WINDOW_FILTER_H__


#include <vector>
#include <utility>
#include <cmath>

namespace kome {
	namespace filter {
		namespace window {

			/**
			 * @class WindowFilter
			 * @brief window filter
			 */
			class WindowFilter {

			public :
				/**
				 * @fn WindowFilter()
				 * @brief constructor
				 * @param params parameters object
				 */
				WindowFilter(kome::objects::Parameters* params);
				/**
				 * @fn WindowFilter()
				 * @brief destructor
				 */
				virtual ~WindowFilter();

			protected:
				// XYDATA source pointer
				kome::core::XYData* m_src;
				// XYData destination pointer
				kome::core::XYData* m_dst;
				// Setting Parameters pointer
				kome::objects::SettingParameterValues* m_settings;
				// Window Wize
				double m_width;

			public:				
				/**
				 * @fn execute()
				 * @brief executes filter
				 */
				void execute();

			protected:
				/**
				 * @fn bool checkStatus()
				 * @brief check status
				 * @return return bool
				 */
				bool checkStatus();

				/**
				 * @fn void setOtherStatus()
				 * @brief This method is called by checkStatus method. (overloaded method)
				 */
				virtual void setOtherStatus();

				/**
				 * @fn void AdaptationFilter()
				 * @brief adaptation target filter(calc by getWeight())
				 */
				void adaptationFilter();

				/**
				 * @fn void setStatusFromSetting()
				 * @brief This method is called by execute method. (abstruct method)
				 */
				virtual double getWeight(double x) = 0;


				/**
				 * @fn void CalcHalfWindowWeight(double &y, double &tw, unsigned const int tgt, unsigned const int lst, const int step)
				 * @brief Calc Weight Half Window Area
				 * @param x point.x(From XYdate.getX)
				 * @param y total point.y * wt
				 * @param tw total weight points
				 * @param tgt target index (Calc center)
				 * @param lst (Last Index)
				 * @param step (-1 : Clac first half / 1 : Calc latter half ) 
				 */
				void calcHalfWindowWeight(double x, double &y, double &tw, unsigned const int tgt, unsigned const int lst, const int step);
			};
		}
	}
}

#endif //__KOME_FILTER_WINDOW_FILTER_H__

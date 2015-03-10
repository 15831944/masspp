/**
 * @file LinearBaselineDetection.h
 * @brief interfaces of LinearBaselineDetection class
 *
 * @author S.Tanaka
 * @date 2007.07.18
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_BASELINE_LINEAR_DETECTION_H__
#define __KOME_BASELINE_LINEAR_DETECTION_H__


namespace kome {
	namespace baseline {
		namespace linear {

			/**
			 * @class LinearBaselineDetection
			 * @brief weighted average baseline class
			 */
			class LinearBaselineDetection {
			public:
				/**
				 * @fn LinearBaselineDetection()
				 * @brief constructor
				 */
				LinearBaselineDetection( kome::objects::SettingParameterValues* settings );

				/**
				 * @fn virtual ~LinearBaselineDetection()
				 * @brief destructor
				 */
				virtual ~LinearBaselineDetection();

			protected:
				/** settings */
				kome::objects::SettingParameterValues* m_settings;

				/** window width */
				double m_windowWidth;

			public:
				/**
				 * @fn void setWindowWidth( const double width )
				 * @brief sets window width
				 * @param width window width
				 */
				void setWindowWidth( const double width );

				/**
				 * @fn double getWindowWidth()
				 * @brief gets window width
				 * @return window width
				 */
				double getWindowWidth();

			public:
				/**
				 * @fn void getBaseline( kome::core::XYData& src, kome::core::XYData& baseline )
				 * @brief gets baseline
				 * @param src source data points
				 * @param baseline the object to store baseline
				 */
				void getBaseline( kome::core::XYData& src, kome::core::XYData& baseline );
			};
		}
	}
}

#endif	// __KOME_BASELINE_LINEAR_DETECTION_H__

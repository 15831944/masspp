/**
 * @file AbcBaselineDetectionManager.h
 * @brief interfaces of AbcBaselineDetectionManager class
 *
 * @author S.Tanaka
 * @date 2007.07.04
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_BASELINE_ABC_MANAGER_H__
#define __KOME_BASELINE_ABC_MANAGER_H__


namespace kome {
	namespace baseline {
		namespace abc {

			/**
			 * @class AbcBaselineDetectionManager
			 * @brief weighted average baseline class
			 */
			class AbcBaselineDetectionManager {
			protected:
				/**
				 * @fn AbcBaselineDetectionManager()
				 * @brief constructor
				 */
				AbcBaselineDetectionManager();

				/**
				 * @fn virtual ~AbcBaselineDetectionManager()
				 * @brief destructor
				 */
				virtual ~AbcBaselineDetectionManager();

			protected:
				/** window width */
				double m_windowWidth;

				/** noise factor */
				double m_noiseFactor;

				/** smoothing width */
				int m_smoothingWidth;

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

				/**
				 * @fn void setNoiseFactor( const double nf )
				 * @brief sets noise factor
				 * @param nf noise factor
				 */
				void setNoiseFactor( const double nf );

				/**
				 * @fn double getNoiseFactor()
				 * @brief gets noise factor
				 * @return noise factor
				 */
				double getNoiseFactor();

				/**
				 * @fn void setSmoothingWidth( const int width )
				 * @brief sets smoothing width
				 * @param width smoothing width
				 */
				void setSmoothingWidth( const int width );

				/**
				 * @fn int getSmoothingWidth()
				 * @brief gets smoothing width
				 * @return smoothing width
				 */
				int getSmoothingWidth();

			public:
				/**
				 * @fn void getBaseline( kome::core::XYData& src, kome::core::XYData& baseline )
				 * @brief gets baseline
				 * @param src source data points
				 * @param baseline the object to store baseline
				 */
				void getBaseline( kome::core::XYData& src, kome::core::XYData& baseline );


			protected:
				/**
				 * @fn double getNoiseThreshold( kome::core::XYData& src )
				 * @brief gets noise threshold
				 * @param src source data points
				 * @return noise threshold
				 */				 
				double getNoiseThreshold( kome::core::XYData& src );

			public:
				/**
				 * @fn static AbcBaselineDetectionManager& getInstance()
				 * @brief gets AbcBaselineDetectionManager object
				 * @return AbcBaselineDetectionManager object (This is the only object.)
				 */
				static AbcBaselineDetectionManager& getInstance();
			};
		}
	}
}

#endif	// __KOME_BASELINE_ABC_MANAGER_H__

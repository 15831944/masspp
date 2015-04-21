/**
 * @file SnCalculator.h
 * @brief interfaces of SnCalculator class
 *
 * @author S.Tanaka
 * @date 2014.01.09
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef _KOME_NUMERIC_SN_CALCULATOR_H__
#define _KOME_NUMERIC_SN_CALCULATOR_H__


namespace kome {
	namespace numeric {

		/**
		 * @class SnCalculator
		 * @brief SnCalculator class
		 */
		class NUMERICAL_TOOLS_CLASS SnCalculator {
		public:
			/**
			 * @fn SnCalculator()
			 * @brief constructor
			 */
			SnCalculator();

			/**
			 * @fn virtual ~SnCalculator()
			 * @brief destructor
			 */
			virtual ~SnCalculator();

		protected:
			/** xy data */
			kome::core::XYData* m_xyData;

		public:
			/**
			 * @fn void setXYData( kome::core::XYData& xyData )
			 * @brief sets data points
			 * @param[in] xyData data points
			 */
			void setXYData( kome::core::XYData& xyData );

			/**
			 * @fn double getSn( const double x )
			 * @brief gets S/N ratio
			 * @param x x coordinate
			 * @return S/N ratio
			 */
			double getSn( const double x );

		protected:
			/**
			 * @fn virtual void onSetXYData( kome::core::XYData& xyData ) = 0
			 * @brief This method is called by setXYData. (abstract method)
			 * @param[in] xyData data points
			 */
			virtual void onSetXYData( kome::core::XYData& xyData ) = 0;

			/**
			 * @fn virtual double onGetSn( const double x ) = 0
			 * @brief This method is called by getSn. (abstract method)
			 * @param[in] x x coordinate
			 * @return S/N ratio
			 */
			virtual double onGetSn( const double x ) = 0;

		};
	}
}

#endif		// _KOME_NUMERIC_SN_CALCULATOR_H__

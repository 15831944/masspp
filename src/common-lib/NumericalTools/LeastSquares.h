/**
 * @file LeastSquares.h
 * @brief interfaces of LeastSquares class
 *
 * @author S.Tanaka
 * @date 2007.05.09
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#ifndef __KOME_NUMERIC_LEAST_SQUARES_H__
#define __KOME_NUMERIC_LEAST_SQUARES_H__


namespace kome {
	namespace numeric {

		/**
		 * @class LeastSquares
		 * @brief least-squares algorithm class
		 */
		class NUMERICAL_TOOLS_CLASS LeastSquares {
		public:
			/**
			 * @fn static bool getPolynomia(
						kome::core::XYData& xyData,
						const unsigned int degree,
						kome::core::Vector& coefficients
				)
			 * @brief get polynomia by least-squares method
			 * @param[in] xyData data
			 * @param[in] degree the degree of polynomia
			 * @param[out] coefficients the vector to store coefficients (The dimention of the vector must be degree + 1.)
			 * @return If true, the least squares method is executed successfully.
			 */
			static bool getPolynomia(
				kome::core::XYData& xyData,
				const unsigned int degree,
				kome::core::Vector& coefficients
			);

			/**
			 * @fn static bool getPolynomia(
					kome::core::XYData& xyData,
					double* weights,
					const unsigned int degree,
					kome::core::Vector& coefficients
				)
			 * @brief gets polynomia by weighted least-squares method
			 * @param[in] xyData data
			 * @param[in] weights the array of weights (The size of the array must equals the length of the data.)
			 * @param[in] degree the degree of polynomia
			 * @param[out] coefficients the vector to store coefficients (The dimention of the vector must be degree + 1.)
			 * @return If true, the least squares method is executed successfully.
			 */
			static bool getPolynomia(
				kome::core::XYData& xyData,
				double* weights,
				const unsigned int degree,
				kome::core::Vector& coefficients
			);
		};
	}
}


#endif		// __KOME_NUMERIC_LEAST_SQUARES_H__

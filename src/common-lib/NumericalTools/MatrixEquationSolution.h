/**
 * @file MatrixEquationSolution.h
 * @brief interfaces of MatrixEquationSolution class
 *
 * @author S.Tanaka
 * @date 2007.05.08
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_NUMERIC_MATRIX_EQUATION_SOLUTION_H__
#define __KOME_NUMERIC_MATRIX_EQUATION_SOLUTION_H__


namespace kome {
	namespace numeric {

		/**
		 * @class MatrixEquationSolution
		 * @brief the class to solve matrix equation
		 */
		class NUMERICAL_TOOLS_CLASS MatrixEquationSolution {
		public:
			/**
			 * @fn static bool solveByGaussianElimination(
					kome::core::Matrix& mat,
					kome::core::Vector& vec,
					kome::core::Vector& ans,
				)
			 * @brief solves matrix elimination by Gaussian elimination
			 * @param[in] mat input matrix
			 * @param[in] vec input values
			 * @param[out] ans vector object to store answer (mat * ans = vec )
			 * @return If true, it succeeded to solve the matrix equation.
			 */
			static bool solveByGaussianElimination(
				kome::core::Matrix& mat,
				kome::core::Vector& vec,
				kome::core::Vector& ans
			);
		};
	}
}

#endif		// __KOME_NUMERIC_MATRIX_EQUATION_SOLUTION_H__

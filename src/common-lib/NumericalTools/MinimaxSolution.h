/**
 * @file MinimaxSolution.h
 * @brief interfaces of MinimaxSolution class
 *
 * @author S.Tanaka
 * @date 2007.08.30
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_NUMERIC_MINIMAX_SOLUTION_H__
#define __KOME_NUMERIC_MINIMAX_SOLUTION_H__


namespace kome {
	namespace numeric {

		/**
		 * @class MinimaxSolution
		 * @brief Minimax solution algorithm class
		 */
		class NUMERICAL_TOOLS_CLASS MinimaxSolution {
		public:

			/**
			 * @fn static void downhillSimplex(
					double* p,
					double* evals,
					const unsigned int dim,
					boost::function< double ( double* ) > fun,
					const double tol,
					const unsigned int iter
				)
			 * @brief executes Downhill Simplex method
			 * @param[in] p the pointer to vertices ( double[dim + 1][ dim ] )
			 * @param[out] evals the pointer to store evaluation values
			 * @param[in] dim the size of dimentions (the number of paramters)
			 * @param[in] fun evaluation function
			 * @param[in] tol tolerance
			 * @param[in] iter the max number of iterations
			 */
			static void downhillSimplex(
				double* p,
				double* evals,
				const unsigned int dim,
				boost::function< double ( double* ) > fun,
				const double tol,
				const unsigned int iter
			);
		};
	}
}


#endif		// __KOME_NUMERIC_MINIMAX_SOLUTION_H__

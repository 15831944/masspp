/**
 * @file MatrixTool.h
 * @brief interfaces of MatrixTool class
 *
 * @author S.Tanaka
 * @date 2007.02.21
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#ifndef __KOME_NUMERIC_MATRIX_TOOL_CLASS_H__
#define __KOME_NUMERIC_MATRIX_TOOL_CLASS_H__


namespace kome {
	namespace numeric {

		/**
		 * @class MatrixTool
		 * @brief matrix tool class
		 */
		class NUMERICAL_TOOLS_CLASS MatrixTool {
		public:
			/**
			 * @fn static kome::core::Matrix getIdentityMatrix( const unsigned int dim )
			 * @brief gets identity matrix
			 * @param[in] dim dimention size
			 * @return (dim)x(dim) identity matrix
			 */
			static kome::core::Matrix getIdentityMatrix( const unsigned int dim );

			/**
			 * @fn static kome::core::Matrix getZeroMatrix( const unsigned int dim )
			 * @brief gets zero matrix
			 * @param[in] dim dimention size
			 * @return (dim)x(dim) zero matrix
			 */
			static kome::core::Matrix getZeroMatrix( const unsigned int dim );
		};
	}
}

#endif		// __KOME_NUMERIC_MATRIX_TOOL_CLASS_H__

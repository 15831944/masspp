/**
 * @file Transformation3D.h
 * @brief interfaces of Transformation3D class
 *
 * @author S.Tanaka
 * @date 2007.02.21
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#ifndef __KOME_NUMERIC_TRANSFORMATION_3D_H__
#define __KOME_NUMERIC_TRANSFORMATION_3D_H__


namespace kome {
	namespace numeric {

		/**
		 * @class Transformation3D
		 * @brief 3 dimentional transformation class
		 */
		class NUMERICAL_TOOLS_CLASS Transformation3D {
		public:
			/**
			 * @fn static kome::core::Matrix getIdentityMatrix3()
			 * @brief gets 3x3 identity matrix
			 * @return 3x3 identity matrix
			 */
			static kome::core::Matrix getIdentityMatrix3();

			/**
			 * @fn static kome::core::Matrix getIdentityMatrix4()
			 * @brief getx 4x4 identity matrix
			 * @return 4x4 identity matrix
			 */
			static kome::core::Matrix getIdentityMatrix4();

			/**
			 * @fn static kome::core::Matrix getRotationMatrix3(
					const double theta,
					kome::core::Vector& axis
				)
			 * @brief gets 3x3 rotation matrix 
			 * @param[in] theta angle of rotation, in degrees
			 * @param[in] axis axis of rotation
			 * @return 3x3 rotation matrix
			 */
			static kome::core::Matrix getRotationMatrix3(
				const double theta,
				kome::core::Vector& axis
			);

			/**
			 * @fn static kome::core::Matrix getRotationMatrix4(
					const double theta,
					kome::core::Vector& axis
				)
			 * @brief gets 4x4 rotation matrix 
			 * @param[in] theta angle of rotation, in degrees
			 * @param[in] axis axis of rotation
			 * @return 4x4 rotation matrix
			 */
			static kome::core::Matrix getRotationMatrix4(
				const double theta,
				kome::core::Vector& axis
			);
		};

	}
}

#endif		 // __KOME_NUMERIC_TRANSFORMATION_3D_H__

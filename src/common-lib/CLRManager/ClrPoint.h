/**
 * @file ClrPoint.h
 * @brief interfaces of ClrPoint class
 *
 * @author S.Tanaka
 * @date 2010.01.12
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_CLR_POINT_H__
#define __KOME_CLR_POINT_H__


namespace kome {
	namespace clr {

		/**
		 * @class ClrPoint
		 * @brief point class on CLR environment
		 */
		public ref class ClrPoint {
		public:
			/**
			 * @fn ClrPoint()
			 * @brief constructor
			 */
			ClrPoint();

			/**
			 * @fn ClrPoint( double x, double y )
			 * @brief constructor
			 * @param[in] x the x-coordinate of the point
			 * @param[in] y the y-coordinate of the point
			 */
			ClrPoint( double x, double y );

			/**
			 * @fn virtual ~ClrPoint()
			 * @brief destructor
			 */
			virtual ~ClrPoint();

		protected:
			/** the x-coordinate of the point */
			double m_x;

			/** the y-coordinate of the point */
			double m_y;

		public:
			/**
			 * @fn void setPoint( double x, double y )
			 * @brief sets points
			 * @param[in] x the x-coordinate of the point
			 * @param[in] y the y-coordinate of the point
			 */
			void setPoint( double x, double y );

			/**
			 * @fn void x( double x )
			 * @brief sets the x-coordinate of the point
			 * @param[in] x the x-coordinate of the point
			 */
			void x( double x );

			/**
			 * @fn double x()
			 * @brief get the x-coordinate of the point
			 * @return the x-coordinate of the point
			 */
			double x();

			/**
			 * @fn void y( double y )
			 * @brief sets the y-coordinate of the point
			 * @param[in] y the y-coordinate of the point
			 */
			 void y( double y );

			 /**
			  * @fn double y()
			  * @brief gets the y-coordinate of the point
			  * @return the y-coordinate of the point
			  */
			 double y();
		};
	}
}

#endif	// __KOME_CLR_POINT_H__

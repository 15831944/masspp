/**
 * @file PointArray.h
 * @brief interfaces of PointArray class
 *
 * @author S.Tanaka
 * @date 2008.07.22
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_CORE_POINT_ARRAY_H__
#define __KOME_CORE_POINT_ARRAY_H__


#include "XYData.h"
#include "Point.h"

#include <vector>


namespace kome {
	namespace core {

		/**
		 * @class PointArray
		 * @brief the array of point class
		 */
		class CORE_TOOLKITS_CLASS PointArray : public XYData {
		public:
			/**
			 * @fn PointArray()
			 * @brief constructor
			 */
			PointArray();

			/** 
			 * @fn virtual ~PointArray()
			 * @brief destructor
			 */
			virtual ~PointArray();

		protected:
			/** the array of point */
			std::vector< Point< double > > m_points;

		public:
			/**
			 * @fn void sortByX( const bool desc )
			 * @brief sorts peaks by x
			 * @param[in] desc descending order flag
			 */
			void sortByX( const bool desc );

			/** 
			 * @fn void sortByY( const bool desc )
			 * @brief sorts peaks by y
			 * @param[in] desc descending order flag
			 */
			void sortByY( const bool desc );

		protected:
			/**
			 * @fn virtual void onClearPoints()
			 * @brief This method is called by clearPoints method. (override method)
			 */
			virtual void onClearPoints();

			/**
			 * @fn virtual void onAddPoint( const double x, const double y )
			 * @brief This method is called by addPoint method. (override method)
			 * @param[in] x x coordinate of point to be added
			 * @param[in] y y coordinate of point to be added
			 */
			virtual void onAddPoint( const double x, const double y );

			/**
			 * @fn virtual void onInsertPoint( const unsigned int index, const double x, const double y )
			 * @brief This method is called by insertPoint method. (override method)
			 * @param[in] index insert position
			 * @param[in] x x coordinate of point
			 * @param[in] y y coordinate of point
			 */
			virtual void onInsertPoint( const unsigned int index, const double x, const double y );

			/**
			 * @fn virtual void onDeletePoint( const unsigned int index )
			 * @brief This method is called by deletePoint method. (override method)
			 * @param[in] index point index
			 */
			virtual void onDeletePoint( const unsigned int index );

			/**
			 * @fn virtual unsigned int onGetLength()
			 * @brief this method is called by getLength method (override method)
			 * @return the number of points
			 */
			virtual unsigned int onGetLength();

			/**
			 * @fn virtual double onGetX( const unsigned int index )
			 * @brief This method is called by getX method. (override method)
			 * @param[in] index the index of point
			 * @return x coordinate
			 */
			virtual double onGetX( const unsigned int index );

			/**
			 * @fn virtual double onGetY( const unsigned int index )
			 * @brief This method is called by getY method. (override method)
			 * @param[in] index the index of point
			 * @return y coordiante
			 */
			virtual double onGetY( const unsigned int index );

			/**
			 * @fn virtual void onReserve( const unsigned int num )
			 * @brief This method is called by reserve method. (override method)
			 * @param[in] num of points to be reserved
			 */
			virtual void onReserve( const unsigned int num );
		};
	}
}


#endif		// __KOME_CORE_POINT_ARRAY_H__

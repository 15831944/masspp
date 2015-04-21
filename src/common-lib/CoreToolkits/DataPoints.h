/**
 * @file DataPoints.h
 * @brief interfaces of DataPoints class
 *
 * @author S.Tanaka
 * @date 2006.09.07
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_CORE_DATA_POINTS_H__
#define __KOME_CORE_DATA_POINTS_H__

#include "XYData.h"


namespace kome {
	namespace core {

		/**
		 * @class DataPoints
		 * @brief data points data of profile management class
		 */
		class CORE_TOOLKITS_CLASS DataPoints : public XYData {
		public:
			/**
			 * @typedef ArrayType
			 * @brief the type of array
			 */
			typedef enum {
				DOUBLE,
				FLOAT,
			} ArrayType;

		public:
			/**
			 * @fn DataPoints( ArrayType type = DOUBLE )
			 * @brief constructor
			 * @param[in] type the type of array
			 */
			DataPoints( ArrayType type = DOUBLE );

			/**
			 * @fn DataPoints( const DataPoints& pts )
			 * @brief copy constructor
			 * @param[in] pts copy source
			 */
			DataPoints( const DataPoints& pts );

			/**
			 * @fn virtual ~DataPoints()
			 * @brief destructor
			 */
			virtual ~DataPoints();

		public:
			/**
			 * @fn DataPoints& operator=( const DataPoints& other )
			 * @brief substitution operator
			 * @param[in] other source data points
			 * @return substituted data points
			 */
			DataPoints& operator=( const DataPoints& other );

		protected:
			/**
			 * @union DataArray
			 * @brief data array type
			 */
			union DataArray {
				double* dblarry;
				float*  fltarry;
			};

			/** type of array */
			ArrayType m_type;

			/** the array of x data */
			DataArray m_xArray;

			/** the array of y data */
			DataArray m_yArray;

			/** the size of array */
			unsigned int m_arraySize;

			/** the number of data points */
			unsigned int m_length;

		public:
			/**
			 * @fn void* getXData()
			 * @brief gets the pointer of x data array
			 * @return x data array
			 */
			void* getXData();

			/**
			 * @fn void* getYData()
			 * @brief gets the pointer of y data array
			 * @return y data array
			 */
			void* getYData();

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

		protected:
			/**
			 * @fn void changeSize( const unsigned int size ) 
			 * @brief change the array of data
			 * @param[in] size new size
			 */
			void changeSize( const unsigned int size ) ;
		};
	}
}

#endif	//  __KOME_CORE_DATA_POINTS_H__

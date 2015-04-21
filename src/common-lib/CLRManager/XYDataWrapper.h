/**
 * @file XYDataWrapper.h
 * @brief interfaces of XYDataWrapper class
 *
 * @author S.Tanaka
 * @date 2006.08.30
 * @date 2011.03.02 M.Aihara
 *       delete static Timer
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_CLR_XY_DATA_WRAPPER_H__
#define __KOME_CLR_XY_DATA_WRAPPER_H__


namespace kome {
	namespace clr {

		/** IO function */
		public delegate int IoFun( void*, int );

		/**
		 * @class XYDataWrapper
		 * @brief XYData object wrapper class to use CLR environment
		 */
		public ref class XYDataWrapper {
		public:
			/**
			 * @fn XYDataWrapper( kome::core::XYData& xyData )
			 * @brief ructor
			 * @param[in] xyData XYData object
			 */
			XYDataWrapper( kome::core::XYData& xyData );

			/**
			 * @fn virtual ~XYDataWrapper()
			 * @brief destructor
			 */
			virtual ~XYDataWrapper();

		protected:
			/**
			 * @fn virtual !XYDataWrapper()
			 * @brief finalizer
			 */
			!XYDataWrapper();	// @date 2012/06/22 <Add> OKADA


		protected:
			/** x, y data object */
			kome::core::XYData& m_xyData;

		public:
			/**
			 * @fn kome::core::XYData& getXYData()
			 * @brief gets the XYData object
			 * @return the pointer to XYData object
			 */
			kome::core::XYData& getXYData();

		public:
			/**
			 * @fn double getMinX()
			 * @brief gets minimum x
			 * @return minimum x
			 */
			double getMinX();

			/**
			 * @fn double getMaxX()
			 * @brief gets maximum x
			 * @return maximum x
			 */
			double getMaxX();

			/**
			 * @fn double getMinY()
			 * @brief gets minimum y
			 * @return minimum y
			 */
			double getMinY();

			/**
			 * @fn double getMaxY()
			 * @brief gets maximum y 
			 * @return maximum y
			 */
			double getMaxY();

		public:
			/**
			 * @fn void clearPoints()
			 * @brief clear all data points
			 */
			void clearPoints();

			/**
			 * @fn void addPoint( double x, double y)
			 * @brief adds point
			 * @param[in] x x coordinate of point to be added
			 * @param[in] y y coordinate of point to be added
			 */
			void addPoint( double x, double y);

			/**
			 * @fn void insertPoint( unsigned int index, double x, double y )
			 * @brief insertts point
			 * @param[in] index insert position
			 * @param[in] x x coordinate of point
			 * @param[in] y y coordinate of point
			 */
			void insertPoint( unsigned int index, double x, double y );

			/**
			 * @fn void updatePoint( unsigned int index, double x, double y )
			 * @brief updates points
			 * @param[in] index update position
			 * @param[in] x x coordinate of point
			 * @param[in] y y coordinate of point
			 */
			void updatePoint( unsigned int index, double x, double y );

			/**
			 * @fn void deletePoint( unsigned int index )
			 * @brief delete point
			 * @param[in] index point index
			 */
			void deletePoint( unsigned int index );

			/**
			 * @fn unsigned int getLength()
			 * @brief gets the number of points
			 * @@return the number of points
			 */
			unsigned int getLength();

			/**
			 * @fn double getX( unsigned int index )
			 * @brief gets x coordinate
			 * @param[in] index the index of point
			 * @return x coorsinate
			 */
			double getX( unsigned int index );

			/**
			 * @fn double getY( unsigned int index )
			 * @brief gets y coordinate
			 * @param[in] index the index of point
			 * @return y coordinate
			 */
			double getY( unsigned int index );

			/**
			 * @fn void reserve( unsigned int num )
			 * @brief reserves enough contiguous memory of array
			 * @param[in] num the number of points to be reserved
			 */
			void reserve( unsigned int num );

		public:
			/**
			 * @fn unsigned long getVersion()
			 * @brief gets the version
			 * @return version
			 */
			unsigned long getVersion();

		public:
			/**
			 * @fn int searchIndex( double x )
			 * @brief searches index of specified x value.
			 * @return index (If there isn't specified x, this method returns negative value.
					   (See SearchTool::binarySearch. SearchTool class exists in AlgorithmTools library.)
			 */
			int searchIndex( double x );

			/**
			 * @fn int getNearestIndex( double x )
			 * @brief gets nearest index
			 * @param[in] x x value
			 */
			int getNearestIndex( double x );

			/**
			 * @fn void filter( double absY, double relY )
			 * @brief executes filter
			 * @param[in] absY absolute y value
			 * @param[in] relY relative y value
			 */
			void filter( double absY, double relY );

		public:
			/**
			 * @fn bool importData( IoFun^ readFun )
			 * @brief imports data
			 * @param[in] readFun read function
			 */
			bool importData( IoFun^ readFun );

			/**
			 * @fn bool exportData( IoFun^ writeFun )
			 * @brief exports data
			 * @param[in] writeFun write function
			 */
			bool exportData( IoFun^ writeFun );
		};

		/**
		 * @class IoFunctionHelper
		 * @brief IO function helper class
		 */
		 class IoFunctionHelper {
		 public:
			 /**
			  * @fn IoFunctionHelper( gcroot< IoFun^ > fun )
			  * @brief constructor
			  * @param[in] fun IO function
			  */
			 IoFunctionHelper( gcroot< IoFun^ > fun );

			 /**
			  * @fn virtual ~IoFunctionHelper()
			  * @brief destructor
			  */
			 virtual ~IoFunctionHelper();

		 protected:
			 /** function */
			 gcroot< IoFun^ > m_fun;

		 public:
			 /**
			  * @fn int call( void* addr, int size )
			  * @brief calls function
			  * @param[in] addr the pointer to data
			  * @param[in] size data size
			  * @return actually reading or writing size
			  */
			 int call( void* addr, int size );
		 };
	}
}


#endif	// __KOME_CLR_XY_DATA_WRAPPER_H__

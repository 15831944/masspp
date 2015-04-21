/**
 * @file ClrXYDataBase.h
 * @brief interfaces of ClrXYDataBase class
 *
 * @author S.Tanaka
 * @date 2006.08.30
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#ifndef __KOME_CLR_XY_DATA_BASE_H__
#define __KOME_CLR_XY_DATA_BASE_H__


#include "XYDataWrapper.h"

#include "ClrXYData.h"


namespace kome {
	namespace clr {

		/**
		 * @class ClrXYDataBase
		 * @brief abstraction class of 2 dimention coordinate information on CLR environment
		 */
#ifndef _MANAGED	// (for doxygen)
		public ref class ClrXYDataBase : public XYDataWrapper {
#else
		public ref class ClrXYDataBase abstract : public XYDataWrapper {
#endif	// _MANAGED
		public:
			/**
			 * @fn ClrXYDataBase()
			 * @brief constructor
			 */
			ClrXYDataBase();

			/**
			 * @fn virtual ~ClrXYDataBase()
			 * @brief destructor
			 */
			virtual ~ClrXYDataBase();

		protected:
			/**
			 * @fn virtual !ClrXYDataBase()
			 * @brief finalizer
			 */
			!ClrXYDataBase(){}	// @date 2012/06/22 <Add> OKADA

		protected:
			/**
			 * @fn virtual bool onLoadData( IoFun^ readFun )
			 * @brief loads data from file
			 * @param[in] readFun read function
			 * @return If true, it succeeded to load the data.
			 */
			virtual bool onLoadData( IoFun^ readFun );

			/**
			 * @fn virtual bool onSaveData( IoFun^ writeFun )
			 * @brief saves data to file
			 * @param[in] writeFun write function
			 * @return If true, it succeeded to save the data
			 */
			virtual bool onSaveData( IoFun^ writeFun );

		public:
			/**
			 * @fn virtual void onClearPoints() = 0
			 * @brief This method is called by clearPoints method. (abstract method)
			 */
			virtual void onClearPoints() = 0;

			/**
			 * @fn virtual void onAddPoint( double x, double y ) = 0
			 * @brief This method is called by addPoint method. (abstract method)
			 * @param[in] x x coordinate of point to be added
			 * @param[in] y y coordinate of point to be added
			 */
			virtual void onAddPoint( double x, double y ) = 0;

			/**
			 * @fn virtual void onInsertPoint( unsigned int index, double x, double y ) = 0
			 * @brief This method is called by insertPoint method. (abstract method)
			 * @param[in] index insert position
			 * @param[in] x x coordinate of point
			 * @param[in] y y coordinate of point
			 */
			virtual void onInsertPoint( unsigned int index, double x, double y ) = 0;

			/**
			 * @fn virtual void onDeletePoint( unsigned int index ) = 0
			 * @brief This method is called by deletePointOnClr method. (abstract method)
			 * @param[in] index point index 
			 */
			virtual void onDeletePoint( unsigned int index ) = 0;

			/**
			 * @fn virtual unsigned int onGetLength() = 0
			 * @brief this method is called by getLength method (abstract method)
			 * @return the number of points
			 */
			virtual unsigned int onGetLength() = 0;

			/**
			 * @fn virtual double onGetX( unsigned int index ) = 0
			 * @brief This method is called by getX method. (abstract method)
			 * @param[in] index the index of point
			 * @return x coordinate
			 */
			virtual double onGetX( unsigned int index ) = 0;

			/**
			 * @fn virtual double onGetY( unsigned int index ) = 0
			 * @brief This method is called by getY method. (abstract method)
			 * @param[in] index the index of point
			 * @return y coordiante
			 */
			virtual double onGetY( unsigned int index ) = 0;

			/**
			 * @fn virtual void onReserve( unsigned int num ) = 0
			 * @brief This method is called by reserve method. (abstract method)
			 * @param[in] num of points to be reserved
			 */
			virtual void onReserve( unsigned int num ) = 0;
		};
	}
}


#endif	// __KOME_CLR_XY_DATA_BASE_H__

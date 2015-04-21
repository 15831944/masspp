/**
 * @file ClrDataPoints.h
 * @brief interfaces of ClrDataPoints class
 *
 * @author S.Tanaka
 * @date 2007.04.04
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_CLR_DATAPOINTS_H__
#define __KOME_CLR_DATAPOINTS_H__


#include "ClrXYDataBase.h"

//@date 2012/06/20 <Add> OKADA
#include <windows.h>

#using <System.dll>
#using <System.Windows.Forms.dll>
using namespace System;
using namespace System::ComponentModel;


namespace kome {
	namespace clr {

		/**
		 * @class ClrDataPoints
		 * @brief data points management class
		 */
		public ref class ClrDataPoints : ClrXYDataBase/*, public IDisposable */ {
		public:
			/**
			 * @fn ClrDataPoints()
			 * @brief constructor
			 */
			ClrDataPoints();

			/**
			 * @fn virtual ~ClrDataPoints()
			 * @brief destructor
			 */
			~ClrDataPoints();	// @date 2012/06/20 <Add> OKADA
			
		protected:
			/**
			 * @fn virtual !ClrDataPoints()
			 * @brief finalizer
			 */
			!ClrDataPoints();

		protected:
			/**
			 * @class XY
			 * @brief 2-dimentional coordinate
			 */
			ref class XY {
			public:
				/**
				 * @fn XY( double x, double y )
				 * @brief constructor
				 * @param[in] x x coordinate
				 * @param[in] y y coordinate
				 */
				XY( double x, double y );

				/**
				 * @fn virtual ~XY()
				 * @brief destructor
				 */
				virtual ~XY();

			public:
				/** x */
				double m_x;
				/** y */
				double m_y;
			};

			/** data points array list */
			System::Collections::ArrayList m_points;

		private:
			/** Resources whether or not abandoned. abandoned in the true */
			bool m_bDisposed;

		public:
			/**
			 * @fn virtual void onClearPoints()
			 * @brief This method is called by clearPoints method. (override method)
			 */
			virtual void onClearPoints() override;

			/**
			 * @fn virtual void onAddPoint( double x, double y )
			 * @brief This method is called by addPoint method. (override method)
			 * @param[in] x x coordinate of point to be added
			 * @param[in] y y coordinate of point to be added
			 */
			virtual void onAddPoint( double x, double y ) override;

			/**
			 * @fn virtual void onInsertPoint( unsigned int index, double x, double y )
			 * @brief This method is called by insertPoint method. (override method)
			 * @param[in] index insert position
			 * @param[in] x x coordinate of point
			 * @param[in] y y coordinate of point
			 */
			virtual void onInsertPoint( unsigned int index, double x, double y ) override;

			/**
			 * @fn virtual void onDeletePoint( unsigned int index )
			 * @brief This method is called by deletePointOnClr method. (override method)
			 * @param[in] index point index 
			 */
			virtual void onDeletePoint( unsigned int index ) override;

			/**
			 * @fn virtual unsigned int onGetLength()
			 * @brief this method is called by getLength method (override method)
			 * @return the number of points
			 */
			virtual unsigned int onGetLength() override;

			/**
			 * @fn virtual double onGetX( unsigned int index )
			 * @brief This method is called by getX method. (override method)
			 * @param[in] index the index of point
			 * @return x coordinate
			 */
			virtual double onGetX( unsigned int index ) override;

			/**
			 * @fn virtual double onGetY( unsigned int index )
			 * @brief This method is called by getY method. (override method)
			 * @param[in] index the index of point
			 * @return y coordiante
			 */
			virtual double onGetY( unsigned int index ) override;

			/**
			 * @fn virtual void onReserve( unsigned int num )
			 * @brief This method is called by reserve method. (override method)
			 * @param[in] num of points to be reserved
			 */
			virtual void onReserve( unsigned int num ) override;

		};
	}
}

#endif		// __KOME_CLR_DATAPOINTS_H__

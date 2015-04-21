/**
 * @file ClrXYData.h
 * @brief interfaces of ClrXYData class
 *
 * @author S.Tanaka
 * @date 2006.08.30
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#ifndef __KOME_CLR_XY_DATA_H__
#define __KOME_CLR_XY_DATA_H__

#include <set>

namespace kome {
	namespace clr {
		ref class ClrXYDataBase;

		/**
		 * @class ClrXYData
		 * @brief class of 2 dimentions coordinate data that has managed class object
		 */
		class ClrXYData : public kome::core::XYData {
		public:
			/**
			 * @fn ClrXYData()
			 * @brief constructor
			 */
			ClrXYData();

			/**
			 * @fn virtual ~ClrXYData()
			 * @brief destructor
			 */
			virtual ~ClrXYData();

		protected:
			/** base data object */
			gcroot< ClrXYDataBase^ > m_baseData;

		public:
			/**
			 * @fn void setBaseData( ClrXYDataBase^ baseData )
			 * @brief sets base data object
			 * @param[in] baseData base data object
			 */
			void setBaseData( ClrXYDataBase^ baseData );

			/**
			 * @fn ClrXYDataBase^ getBaseData()
			 * @brief gets base data object
			 * @return base data object
			 */
			ClrXYDataBase^ getBaseData();

		protected:
			/**
			 * @fn virtual void onClearPoints()
			 * @brief This method is called by clearPoints method. (abstract method)
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
			 * @param[in] num number of points to be reserved
			 */
			virtual void onReserve( const unsigned int num );
		};
	}
}

#endif	// __KOME_CLR_XY_DATA_H__

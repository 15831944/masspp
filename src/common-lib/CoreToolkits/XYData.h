/**
 * @file XYData.h
 * @brief interfaces of XYData class
 *
 * @author S.Tanaka
 * @date 2006.08.30
 * @date 2011.03.02 M.Aihara
 *       delete static Timer
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_CORE_XY_DATA_H__
#define __KOME_CORE_XY_DATA_H__


#include "Point.h"
#include "Timer.h"

#include <vector>
#include <list>
#include <boost/function.hpp>
#include <boost/optional.hpp>


namespace kome {
	namespace core {

		/**
		 * @class XYData
		 * @brief abstraction class of two dimention coordinate data
		 */
		class CORE_TOOLKITS_CLASS XYData {
		public:
			/**
			 * @fn XYData()
			 * @brief constructor
			 */
			XYData();

			/**
			 * @fn virtual ~XYData()
			 * @brief destructor
			 */
			virtual ~XYData();

		protected:
			/** updated flag */
			bool m_updated;

			/** min x */
			double m_minX;

			/** max x */
			double m_maxX;

			/** min y */
			double m_minY;

			/** max y */
			double m_maxY;

			/** version */
			unsigned long m_version;

		private:
			/** data file */
			std::string m_tmpDataFile;

			/** release flag */
			bool m_released;

		private:
			/** cache size */
			static const unsigned int m_cacheSize;

		protected:
			/** version */
			static unsigned long m_currentVersion;

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
			 * @fn void addPoint( const double x, const double y )
			 * @brief adds point
			 * @param[in] x x coordinate of point to be added
			 * @param[in] y y coordinate of point to be added
			 */
			void addPoint( const double x, const double y );

			/**
			 * @fn void insertPoint( const unsigned int index, const double x, const double y )
			 * @brief insertts point
			 * @param[in] index insert position
			 * @param[in] x x coordinate of point
			 * @param[in] y y coordinate of point
			 */
			void insertPoint( const unsigned int index, const double x, const double y );

			/**
			 * @fn void updatePoint( const unsigned int index, const double x, const double y )
			 * @brief updates points
			 * @param[in] index update position
			 * @param[in] x x coordinate of point
			 * @param[in] y y coordinate of point
			 */
			void updatePoint( const unsigned int index, const double x, const double y );

			/**
			 * @fn void deletePoint( const unsigned int index )
			 * @brief delete point
			 * @param[in] index point index
			 */
			void deletePoint( const unsigned int index );

			/**
			 * @fn unsigned int getLength()
			 * @brief gets the number of points
			 * @@return the number of points
			 */
			unsigned int getLength();

			/**
			 * @fn double getX( const unsigned int index )
			 * @brief gets x coordinate
			 * @param[in] index the index of point
			 * @return x coorsinate
			 */
			double getX( const unsigned int index );

			/**
			 * @fn double getY( const unsigned int index )
			 * @brief gets y coordinate
			 * @param[in] index the index of point
			 * @return y coordinate
			 */
			double getY( const unsigned int index );
			
			/**
			 * @fn void reserve( const unsigned int num )
			 * @brief reserves enough contiguous memory of array
			 * @param[in] num the number of points to be reserved
			 */
			void reserve( const unsigned int num );

		public:
			/**
			 * @fn unsigned long getVersion()
			 * @brief gets the version
			 * @return version
			 */
			unsigned long getVersion();

		public:
			/**
			 * @fn int searchIndex( const double x )
			 * @brief searches index of specified x value.
			 * @return index (If there isn't specified x, this method returns negative value.
					   (See SearchTool::binarySearch. SearchTool class exists in AlgorithmTools library.)
			 */
			int searchIndex( const double x );

			/**
			 * @fn int getNearestIndex( const double x )
			 * @brief gets nearest index
			 * @param[in] x x value
			 */
			int getNearestIndex( const double x );

			/**
			 * @fn void filter( const double absY = 0.0, const double relY = 0.0 )
			 * @brief executes filter
			 * @param[in] absY absolute y value
			 * @param[in] relY relative y value
			 */
			void filter( const double absY = 0.0, const double relY = 0.0 );

			/**
			 * @fn void getPoints( std::vector< Point< double > >& points, const bool ySort, const bool desc )
			 * @brief gets points array
			 * @param[in] points points array to store points data
			 * @param[in] ySort
					  - true: sort by x element
					  - false: sort by y element
			 * @param[in] desc descending order flag
			 */
			void getPoints( std::vector< Point< double > >& points, const bool ySort, const bool desc );

		public:
			/**
			 * @fn bool importData( boost::function< int ( void*, int ) > readFun )
			 * @brief imports data
			 * @param[in] readFun read function
			 */
			bool importData( boost::function< int ( void*, int ) > readFun );

			/**
			 * @fn bool exportData( boost::function< int ( void*, int ) > writeFun )
			 * @brief exports data
			 * @param[in] writeFun write function
			 */
			bool exportData( boost::function< int ( void*, int ) > writeFun );

		protected:
			/**
			 * @fn void updateRange()
			 * @brief updates range
			 */
			void updateRange();

		protected:
			/**
			 * @fn virtual bool onLoadData( boost::function< int ( void*, int ) > readFun )
			 * @brief loads data from file
			 * @param[in] readFun read function
			 * @return If true, it succeeded to load the data.
			 */
			virtual bool onLoadData( boost::function< int ( void*, int ) > readFun );

			/**
			 * @fn virtual bool onSaveData( boost::function< int ( void*, int ) > writeFun )
			 * @brief saves data to file
			 * @param[in] writeFun write function
			 * @return If true, it succeeded to save the data
			 */
			virtual bool onSaveData( boost::function< int ( void*, int ) > writeFun );

		protected:
			/**
			 * @fn virtual void onClearPoints() = 0
			 * @brief This method is called by clearPoints method. (abstract method)
			 */
			virtual void onClearPoints() = 0;

			/**
			 * @fn virtual void onAddPoint( const double x, const double y ) = 0
			 * @brief This method is called by addPoint method. (abstract method)
			 * @param[in] x x coordinate of point to be added
			 * @param[in] y y coordinate of point to be added
			 */
			virtual void onAddPoint( const double x, const double y ) = 0;

			/**
			 * @fn virtual void onInsertPoint( const unsigned int index, const double x, const double y ) = 0
			 * @brief This method is called by insertPoint method. (abstract method)
			 * @param[in] index insert position
			 * @param[in] x x coordinate of point
			 * @param[in] y y coordinate of point
			 */
			virtual void onInsertPoint( const unsigned int index, const double x, const double y ) = 0;

			/**
			 * @fn virtual void onDeletePoint( const unsigned int index ) = 0
			 * @brief This method is called by deletePoint method. (abstract method)
			 * @param[in] index point index
			 */
			virtual void onDeletePoint( const unsigned int index ) = 0;

			/**
			 * @fn virtual unsigned int onGetLength() = 0
			 * @brief this method is called by getLength method (abstract method)
			 * @return the number of points
			 */
			virtual unsigned int onGetLength() = 0;

			/**
			 * @fn virtual double onGetX( const unsigned int index ) = 0
			 * @brief This method is called by getX method. (abstract method)
			 * @param[in] index the index of point
			 * @return x coordinate
			 */
			virtual double onGetX( const unsigned int index ) = 0;

			/**
			 * @fn virtual double onGetY( const unsigned int index ) = 0
			 * @brief This method is called by getY method. (abstract method)
			 * @param[in] index the index of point
			 * @return y coordiante
			 */
			virtual double onGetY( const unsigned int index ) = 0;

			/**
			 * @fn virtual void onReserve( const unsigned int num ) = 0
			 * @brief This method is called by reserve method. (abstract method)
			 * @param[in] num of points to be reserved
			 */
			virtual void onReserve( const unsigned int num ) = 0;

		private:
			/**
			 * @fn void releaseData()
			 * @brief releases data
			 */
			void releaseData();

			/**
			 * @fn void recoverData()
			 * @brief recovers data
			 */
			void recoverData();

		private:
			/**
			 * @fn static std::list< XYData* >& getCacheList()
			 * @brief gets cache list
			 * @return cache list (This is the only object.)
			 */
			static std::list< XYData* >& getCacheList();
		};
	}
}

#endif // __KOME_CORE_XY_DATA_H__

/**
 * @file Peaks.h
 * @brief interfaces of Peaks class
 *
 * @author S.Tanaka
 * @date 2006.10.11
 * @date 2011.03.03 M.Aihara
 *       soft cache DataPoints
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_OBJECTS_PEAKS_H__
#define __KOME_OBJECTS_PEAKS_H__


#include <string>
#include <vector>

// soft cache DataPoints
#ifdef  ENABLE_MASSPP_PARALLEL_TBB_MODE
#define DATAPOINTS_SOFT_CACHE_TBB
#endif // ENABLE_MASSPP_PARALLEL_TBB_MODE

namespace kome {
	namespace objects {

		class PeakElement;

		/**
		 * @class Peaks
		 * @brief peaks information class
		 */
		class DATA_OBJECTS_CLASS Peaks : public kome::core::XYData {
		public:
			/**
			 * @fn Peaks()
			 * @brief constructor
			 */
			Peaks();

			/**
			 * @fn Peaks( const Peaks& peaks )
			 * @brief copy constructor
			 * @param[in] peaks copy source
			 */
			Peaks( const Peaks& peaks );

			/**
			 * @fn virtual ~Peaks()
			 * @brief destructor
			 */
			virtual ~Peaks();

		public:
			/**
			 * @fn Peaks& operator=( const Peaks& other )
			 * @brief substitution operator
			 * @param[in] other source peaks object
			 * @return copied peaks object
			 */
			Peaks& operator=( const Peaks& other );

		protected:
			/** the array of peak elements */
			std::vector< PeakElement* > m_peaks;

			/** peak title */
			std::string m_title;

			/** charge array */
			std::vector< short > m_charges;

			/** precursor */
			double m_precursor;

			/** data file */
			std::string m_dataFile;

// @date 2011.03.03 <Mod> M.Aihara ->
// soft cache DataPoints
#ifdef DATAPOINTS_SOFT_CACHE_TBB
            /** local DataPoints */
            kome::core::DataPoints* m_dps;
#endif // DATAPOINTS_SOFT_CACHE_TBB
// <Mod> M.Aihara
			
		public:
			/**
			 * @fn int issueId( PeakElement* peakElement )
			 * @brief to issue the peak id
			 * @param[in] peakElement peak element objects
			 * @return peak id
			 */
			int issueId( PeakElement* peakElement );

			/**
			 * @fn PeakElement* getPeakById( int id )
			 * @brief gets peak element by peak id
			 * @param[in] id peak id
			 * @return peak element objects
			 */
			PeakElement* getPeakById( int id );

			/**
			 * @fn void clearCharge()
			 * @brief clears charge
			 */
			void clearCharge();

			/**
			 * @fn void addCharge( const short charge )
			 * @brief adds charge
			 * @param[in] charge charge
			 */
			void addCharge( const short charge );

			/**
			 * @fn unsigned int getNumberOfCharges()
			 * @brief gets the number of charges
			 * @return the number of charges
			 */
			unsigned int getNumberOfCharges();

			/**
			 * @fn short getCharge( const unsigned int index )
			 * @brief gets charge
			 * @param[in] index charge index
			 * @return charge
			 */
			short getCharge( const unsigned int index );

			/**
			 * @fn void setPrecursor( const double precursor )
			 * @brief sets precursor
			 * @param[in] precursor precursor
			 */
			void setPrecursor( const double precursor );

			/**
			 * @fn double getPrecursor()
			 * @brief gets precursor
			 * @return precursor
			 */
			double getPrecursor();

		public:
			/**
			 * @fn void calcArea()
			 * @brief calculates area
			 */
			void calcArea();

			/**
			 * @fn void setDataPoints( kome::core::XYData& xyData )
			 * @brief sets data points
			 * @param[in] xyData data points
			 */
			void setDataPoints( kome::core::XYData& xyData );

		public:
			/**
			 * @fn PeakElement* createPeak( const double x, const double y )
			 * @brief creates peak
			 * @param[in] x x element of peak
			 * @param[in] y y element of peak
			 * @return new peak object
			 */
			PeakElement* createPeak( const double x, const double y );

			/**
			 * @fn PeakElement* getPeak( const unsigned int index )
			 * @brief gets peak
			 * @param[in] index peak index
			 * @return peak object (If NULL, the index is illegal.
			 */			
			PeakElement* getPeak( const unsigned int index );

		public:
			/**
			 * @fn void sort()
			 * @brief sorts peaks
			 */
			void sort();

		protected:
			/**
			 * @fn void arrangePeaks()
			 * @brief arrange peaks
			 */
			void arrangePeaks();

		protected:
			/**
			 * @fn virtual bool onLoadData( boost::function< int ( void*, int ) > readFun )
			 * @brief loads data from file (override method)
			 * @param[in] readFun read function
			 * @return If true, it succeeded to load the data.
			 */
			virtual bool onLoadData( boost::function< int ( void*, int ) > readFun );

			/**
			 * @fn virtual bool onSaveData( boost::function< int ( void*, int ) > writeFun )
			 * @brief saves data to file (override method)
			 * @param[in] writeFun write function
			 * @return If true, it succeeded to save the data
			 */
			virtual bool onSaveData( boost::function< int ( void*, int ) > writeFun );
			
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
			 * @fn static bool lessPeakX( PeakElement* p0, PeakElement* p1 )
			 * @brief compares to sort peaks by x coordinate
			 * @param[in] p0 peak to be compared
			 * @param[in] p1 peak to compare
			 * @return If true, p0 sorts before p1.
			 */
			static bool lessPeakX( PeakElement* p0, PeakElement* p1 );

			/**
			 * @fn static bool lessPeakY( PeakElement* p0, PeakElement* p1 )
			 * @brief compares to sort peaks by y coordinate
			 * @param[in] p0 peak to be compared
			 * @param[in] p1 peak to compare
			 * @return If true, p0 sorts before p1.
			 */
			static bool lessPeakY( PeakElement* p0, PeakElement* p1 );
		};
	}
}

#endif		// __KOME_OBJECTS_PEAKS_H__

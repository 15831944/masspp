/**
 * @file Peaks2DArray.h
 * @brief interfaces of Peaks2DArray class
 *
 * @author S.Tanaka
 * @date 2007.10.09
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_OBJECTS_PEAKS_2D_ARRAY_H__
#define __KOME_OBJECTS_PEAKS_2D_ARRAY_H__


#include <vector>


namespace kome {
	namespace objects {

		class Peak2DElement;

		/**
		 * @class Peaks2DArray
		 * @brief 2-dimentional peaks array
		 */
		class DATA_OBJECTS_CLASS Peaks2DArray {
		public:
			/**
			 * @fn Peaks2DArray()
			 * @brief constructor
			 */
			Peaks2DArray();

			/**
			 * @fn virtual ~Peaks2DArray()
			 * @brief destructor
			 */
			virtual ~Peaks2DArray();

		protected:
			/** peaks array */
			std::vector< Peak2DElement* > m_peaks;
						
		public:
			/**
			 * @fn int issueId( Peak2DElement* peak2dElement )
			 * @brief to issue the peak2d id
			 * @param[in] peak2dElement peak element objects
			 * @return peak2d id
			 */
			int issueId( Peak2DElement* peak2dElement );

			/**
			 * @fn Peak2DElement* getPeakById( int id )
			 * @brief gets peak element by peak2d id
			 * @param[in] id peak2d id
			 * @return peak2d element objects
			 */
			Peak2DElement* getPeakById( int id );

			/**
			 * @fn unsigned int getNumberOfPeaks()
			 * @brief gets the number of peaks
			 * @return the number of peaks
			 */
			unsigned int getNumberOfPeaks();

			/**
			 * @fn Peak2DElement* getPeak( const unsigned int idx )
			 * @brief gets peak element
			 * @param[in] idx peak index
			 * @return peak elment (If NULL, the index is illegal.)
			 */
			Peak2DElement* getPeak( const unsigned int idx );

			/**
			 * @fn int searchPeak( Peak2DElement* peak )
			 * @brief searches peak
			 * @param[in] peak peak object to be searched
			 * @return peak index (If specified peak is not found, this method returns negative number.)
			 */
			int searchPeak( Peak2DElement* peak );

		public:
			/**
			 * @fn void sortByRt( const bool desc )
			 * @brief sorts peaks by RT
			 * @param[in] desc descending order flag
			 */
			void sortByRt( const bool desc );

			/** 
			 * @fn void sortByMz( const bool desc )
			 * @brief sorts peaks by m/z
			 * @param[in] desc descending order flag
			 */
			void sortByMz( const bool desc );

			/**
			 * @fn void sortByIntensity( const bool desc )
			 * @brief sorts peaks by intensity
			 * param desc descending order flag
			 */
			void sortByIntensity( const bool desc );

		protected:
			/**
			 * @fn static bool lessRt( Peak2DElement* p0, Peak2DElement* p1, bool desc )
			 * @brief compares peaks to sort by RT
			 * @param[in] p0 peak object to be compared
			 * @param[in] p1 peak object to compare
			 * @param[in] desc descending order flag
			 * @return If true, p0 sorts before p1.
			 */
			static bool lessRt( Peak2DElement* p0, Peak2DElement* p1, bool desc );

			/**
			 * @fn static bool lessMz( Peak2DElement* p0, Peak2DElement* p1, bool desc )
			 * @brief compares peaks to sort by m/z
			 * @param[in] p0 peak object to be compared
			 * @param[in] p1 peak object to compare
			 * @param[in] desc descenfing order flag
			 * @return If true, p0 sorts before p1.
			 */
			static bool lessMz( Peak2DElement* p0, Peak2DElement* p1, bool desc );

			/**
			 * @fn static bool lessIntensity( Peak2DElement* p0, Peak2DElement* p1, bool desc )
			 * @brief compares peaks to sort by intensity
			 * @param[in] p0 peak object to be compared
			 * @param[in] p1 peak object to compare
			 * @param[in] desc descenfing order flag
			 * @return If true, p0 sorts before p1.
			 */
			static bool lessIntensity( Peak2DElement* p0, Peak2DElement* p1, bool desc );
		};
	}
}

#endif		// __KOME_OBJECTS_PEAKS_2D_ARRAY_H__

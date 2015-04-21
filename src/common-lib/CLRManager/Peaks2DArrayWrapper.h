/**
 * @file Peaks2DArrayWrapper.h
 * @brief interfaces of Peaks2DArrayWrapper class
 *
 * @author S.Tanaka
 * @date 2007.10.10
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_CLR_PEAKS_2D_ARRAY_WRAPPER_H__
#define __KOME_CLR_PEAKS_2D_ARRAY_WRAPPER_H__


namespace kome {
	namespace clr {

		ref class Peak2DElementWrapper;

		/**
		 * @class Peaks2DArrayWrapper
		 * @brief Peaks2DArray wrapper class to use on CLR environment
		 */
		public ref class Peaks2DArrayWrapper {
		public:
			/**
			 * @fn Peaks2DArrayWrapper( kome::objects::Peaks2DArray& arr )
			 * @brief constructor
			 * @param[in] arr peaks array 
			 */
			Peaks2DArrayWrapper( kome::objects::Peaks2DArray& arr );

			/**
			 * @fn virtual ~Peaks2DArrayWrapper()
			 * @brief destructor
			 */
			virtual ~Peaks2DArrayWrapper();

		protected:
			/** peaks array */
			kome::objects::Peaks2DArray& m_arr;

		public:
			/**
			 * @fn unsigned int getNumberOfPeaks()
			 * @brief gets the number of peaks
			 * @return the number of peaks
			 */
			unsigned int getNumberOfPeaks();

			/**
			 * @fn Peak2DElementWrapper^ getPeak( unsigned int idx )
			 * @brief gets peak element
			 * @param[in] idx peak index
			 * @return peak elment (If NULL, the index is illegal.)
			 */
			Peak2DElementWrapper^ getPeak( unsigned int idx );

			/**
			 * @fn int searchPeak( Peak2DElementWrapper^ peak )
			 * @brief searches peak
			 * @param[in] peak peak object to be searched
			 * @return peak index (If specified peak is not found, this method returns negative number.)
			 */
			int searchPeak( Peak2DElementWrapper^ peak );

		public:
			/**
			 * @fn void sortByRt( bool desc )
			 * @brief sorts peaks by RT
			 * @param[in] desc descending order flag
			 */
			void sortByRt( bool desc );

			/** 
			 * @fn void sortByMz( bool desc )
			 * @brief sorts peaks by m/z
			 * @param[in] desc descending order flag
			 */
			void sortByMz( bool desc );

			/**
			 * @fn void sortByIntensity( bool desc )
			 * @brief sorts peaks by intensity
			 * param desc descending order flag
			 */
			void sortByIntensity( bool desc );
		};
	}
}

#endif		// __KOME_CLR_PEAKS_2D_ARRAY_WRAPPER_H__

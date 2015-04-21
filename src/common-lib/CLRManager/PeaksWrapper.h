/**
 * @file PeaksWrapper.h
 * @brief interfaces of PeaksWrapper class
 *
 * @author S.Tanaka
 * @date 2006.11.09
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#ifndef __KOME_CLR_PEAKS_WRAPPER_H__
#define __KOME_CLR_PEAKS_WRAPPER_H__

#include "XYDataWrapper.h"

namespace kome {
	namespace clr {

		ref class PeakElementWrapper;

		/**
		 * @class PeaksWrapper
		 * @brief peaks wrapper class to use on CLR environment
		 */
		public ref class PeaksWrapper : public XYDataWrapper {
		public:
			/**
			 * @fn PeaksWrapper( kome::objects::Peaks& peaks )
			 * @brief constructor
			 * @param[in] peaks peaks object
			 */
			PeaksWrapper( kome::objects::Peaks& peaks );

			/**
			 * @fn PeaksWrapper()
			 * @brief constructor
			 */
			PeaksWrapper();

			/**
			 * @fn virtual ~PeaksWrapper()
			 * @brief destructor
			 */
			virtual ~PeaksWrapper();

		protected:
			/** peaks object */
			kome::objects::Peaks& m_peaks;

			/** delete flag */
			bool m_peakDelete;

		public:
			/**
			 * @fn kome::objects::Peaks& getPeaks()
			 * @brief gets peaks object
			 * @return peaks object
			 */
			kome::objects::Peaks& getPeaks();

		public:
			/**
			 * @fn void clearCharge()
			 * @brief clears charge
			 */
			void clearCharge();

			/**
			 * @fn void addCharge( short charge )
			 * @brief adds charge
			 * @param[in] charge charge
			 */
			void addCharge( short charge );

			/**
			 * @fn unsigned int getNumberOfCharges()
			 * @brief gets the number of charges
			 * @return the number of charges
			 */
			unsigned int getNumberOfCharges();

			/**
			 * @fn short getCharge( unsigned int index )
			 * @brief gets charge
			 * @param[in] index charge index
			 * @return charge
			 */
			short getCharge( unsigned int index );

			/**
			 * @fn void setPrecursor( double precursor )
			 * @brief sets precursor
			 * @param[in] precursor precursor
			 */
			void setPrecursor( double precursor );

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
			 * @fn void setDataPoints( XYDataWrapper^ xyData )
			 * @brief sets data points
			 * @param[in] xyData data points
			 */
			void setDataPoints( XYDataWrapper^ xyData );

		public:
			/**
			 * @fn PeakElementWrapper^ createPeak( double x, double y )
			 * @brief creates peak
			 * @param[in] x x element of peak
			 * @param[in] y y element of peak
			 * @return new peak object
			 */
			PeakElementWrapper^ createPeak( double x, double y );

			/**
			 * @fn PeakElementWrapper^ getPeak( unsigned int index )
			 * @brief gets peak
			 * @param[in] index peak index
			 * @return peak object (If NULL, the index is illegal.
			 */			
			PeakElementWrapper^ getPeak( unsigned int index );



		public:
			/**
			 * @fn void sort()
			 * @brief sorts peaks
			 */
			void sort();
		};
	}
}

#endif	// __KOME_CLR_PEAKS_WRAPPER_H__

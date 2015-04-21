/**
 * @file PeakElement.h
 * @brief interfaces of PeakElement class
 *
 * @author S.Tanaka
 * @date 2007.12.06
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_OBJECTS_PEAK_ELEMENT_H__
#define __KOME_OBJECTS_PEAK_ELEMENT_H__


#include <vector>


namespace kome {
	namespace objects {

		class Peaks;

		/**
		 * @class PeakElement
		 * @brief peak element
		 */
		class DATA_OBJECTS_CLASS PeakElement {
		public:
			/**
			 * @fn PeakElement()
			 * @brief constructor
			 */
			PeakElement();

			/**
			 * @fn PeakElement( Peaks* peaks )
			 * @brief constructor
			 */
			PeakElement( Peaks* peaks );

			/**
			 * @fn PeakElement( Peaks* peaks, const double x, const double y )
			 * @brief constructor
			 * @param[in] peaks peaks object
			 * @param[in] x x element value
			 * @param[in] y y element value
			 */
			PeakElement( Peaks* peaks, const double x, const double y );

			/**
			 * @fn virtual ~PeakElement()
			 * @brief destructor
			 */
			virtual ~PeakElement();

		protected:
			/** peaks */
			Peaks* m_peaks;

			/** x element value */
			double m_x;

			/** y element value */
			double m_y;

			/** peak left */
			kome::core::Point< double > m_left;

			/** peak right */
			kome::core::Point< double > m_right;

			/** apex */
			kome::core::Point< double > m_apex;

			/** charge */
			int m_charge;

		protected:
			/** peak area */
			double m_area;

			/** FWHM */
			double m_fwhm;

			/** peak ID */
			int m_peakId;

		public:
			/**
			 * @fn void setX( const double x )
			 * @brief sets x element value
			 * @param[in] x x element
			 */
			void setX( const double x );

			/**
			 * @fn double getX()
			 * @brief gets x element value
			 * @return x element
			 */
			double getX();

			/**
			 * @fn void setY( const double y )
			 * @brief sets y element value
			 * @param[in] y y element
			 */
			void setY( const double y );

			/**
			 * @fn double getY()
			 * @brief gets y element value
			 * @return y element
			 */
			double getY();

			/**
			 * @fn void setLeft( const double x, const double y )
			 * @brief sets peak left
			 * @param[in] x x element of peak left
			 * @param[in] y y element of peak left
			 */
			void setLeft( const double x, const double y );

			/**
			 * @fn double getLeftX()
			 * @brief gets x element of peak left
			 * @return x element of peak left
			 */
			double getLeftX();

			/**
			 * @fn double getLeftY()
			 * @brief gets y element of peak left
			 * @return y element of peak left
			 */
			double getLeftY();

			/**
			 * @fn void setRight( const double x, const double y )
			 * @brief sets peak right
			 * @param[in] x x element of peak right
			 * @param[in] y y element of peak right
			 */
			void setRight( const double x, const double y );

			/**
			 * @fn double getRightX()
			 * @brief gets x element of peak right
			 * @return x element of peak right
			 */
			double getRightX();

			/**
			 * @fn double getRightY()
			 * @brief gets y elemtn of peak right
			 * @return y elemtn of peak right
			 */
			double getRightY();

			/**
			 * @fn void setApex( const double x, const double y )
			 * @brief sets peak apex
			 * @param[in] x x element of peak apex
			 * @param[in] y y element of peak apex
			 */
			void setApex( const double x, const double y );

			/**
			 * @fn double getApexX()
			 * @brief gets x element of peak apex
			 * @return x element of peak apex
			 */
			double getApexX();

			/**
			 * @fn double getApexY()
			 * @brief gets y element of peak apex
			 * @return y element of peak apex
			 */
			double getApexY();

			/**
			 * @fn void searchApex( kome::core::XYData& xyData )
			 * @brief searches apex
			 * @param[in] xyData data points
			 */
			void searchApex( kome::core::XYData& xyData );

			/**
			 * @fn bool hasApex()
			 * @brief judges whether this peak has apex information or not.
			 * @return If true, this peak has apex information
			 */
			bool hasApex();

		public:
			/**
			 * @fn void setArea( const double area )
			 * @brief sets area
			 * @param[in] area area
			 */
			void setArea( const double area );

			/**
			 * @fn double getArea()
			 * @brief gets peak area
			 * @return peak area
			 */
			double getArea();

			/**
			 * @fn bool hasArea()
			 * @brief judges whether this peak has area value or not
			 * @return If true, this peak has area value
			 */
			bool hasArea();

			/**
			 * @fn double calcArea( kome::core::XYData& xyData )
			 * @brief calculates area
			 * @param[in] xyData data points
			 * @return area
			 */
			double calcArea( kome::core::XYData& xyData );


		public:
			/**
			 * @fn void setFwhm( const double fwhm )
			 * @brief sets the FWHM
			 * @param[in] fwhm FWHM
			 */
			void setFwhm( const double fwhm );

			/**
			 * @fn double getFwhm()
			 * @brief gets the FWHM
			 * @return FWHM
			 */
			double getFwhm();

			/**
			 * @fn bool hasFwhm()
			 * @brief judges whether this peak has FWHM value of not
			 * @return If true, this peak has FWHM value
			 */
			bool hasFwhm();

			/**
			 * @fn double calcFwhm( kome::core::XYData& xyData )
			 * @brief calculates FWHM
			 * @param[in] xyData data points
			 * @return FWHM
			 */
			double calcFwhm( kome::core::XYData& xyData );

		public:
			/**
			 * @fn void setId( int id )
			 * @brief sets peak ID
			 * @param[in] id peak ID
			 */
			void setId( int id );

			/**
			 * @fn int getId()
			 * @brief gets peak ID
			 */
			int getId();

		public:
			/**
			 * @fn void setCharge( const int charge )
			 * @brief sets the charge state
			 * @param[in] charge charge state
			 */
			void setCharge( const int charge );

			/**
			 * @fn int getCharge()
			 * @brief gets the charge state
			 * @return charge state
			 */
			int getCharge();
		};
	}
}


#endif		// __KOME_OBJECTS_PEAK_ELEMENT_H__

/**
 * @file PeakElementWrapper.h
 * @brief interfaces of PeakElementWrapper class
 *
 * @author S.Tanaka
 * @date 2007.12.06
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_CLR_PEAK_ELEMENT_WRAPPER_H__
#define __KOME_CLR_PEAK_ELEMENT_WRAPPER_H__


namespace kome {
	namespace clr {

		ref class XYDataWrapper;
		ref class ClrPoint;

		/**
		 * @class PeakElementWrapper
		 * @brief PeakElement wrapper class to use on CLR environment
		 */
		public ref class PeakElementWrapper {
		public:
			/**
			 * @fn PeakElementWrapper( kome::objects::PeakElement& peak )
			 * @brief constructor
			 * @param[in] peak peak object to be wrapped
			 */
			PeakElementWrapper( kome::objects::PeakElement& peak );

			/**
			 * @fn PeakElementWrapper()
			 * @brief constructor
			 */
			PeakElementWrapper();

			/**
			 * @fn virtual ~PeakElementWrapper()
			 * @brief destructor
			 */
			virtual ~PeakElementWrapper();

		protected:
			/** peak element */
			kome::objects::PeakElement& m_peak;

			/** delete flag */
			bool m_deleteFlg;

		public:
			/**
			 * @fn kome::objects::PeakElement& getPeakElement()
			 * @brief gets peak element object to be wrapped
			 * @return peak element object to be wrapped
			 */
			kome::objects::PeakElement& getPeakElement();

		public:
			/**
			 * @fn void setX( double x )
			 * @brief sets x element value
			 * @param[in] x x element
			 */
			void setX( double x );

			/**
			 * @fn double getX()
			 * @brief gets x element value
			 * @return x element
			 */
			double getX();

			/**
			 * @fn void setY( double y )
			 * @brief sets y element value
			 * @param[in] y y element
			 */
			void setY( double y );

			/**
			 * @fn double getY()
			 * @brief gets y element value
			 * @return y element
			 */
			double getY();

			/**
			 * @fn void setLeft( double x, double y )
			 * @brief sets peak left
			 * @param[in] x x element of peak left
			 * @param[in] y y element of peak left
			 */
			void setLeft( double x, double y );

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
			 * @fn void setRight( double x, double y )
			 * @brief sets peak right
			 * @param[in] x x element of peak right
			 * @param[in] y y element of peak right
			 */
			void setRight( double x, double y );

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
			 * @fn void setApex( double x, double y )
			 * @brief sets peak apex
			 * @param[in] x x element of peak apex
			 * @param[in] y y element of peak apex
			 */
			void setApex( double x, double y );

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
			 * @fn bool hasApex()
			 * @brief judges whether this peak has apex information or not.
			 * @return If true, this peak has apex information
			 */
			bool hasApex();

		public:
			/**
			 * @fn void setArea( double area )
			 * @brief sets peak area
			 * @param[in] area peak area
			 */
			void setArea( double area );

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
			 * @fn double calcArea( XYDataWrapper^ xyData )
			 * @brief calculates area
			 * @param[in] xyData data points
			 * @return area
			 */
			double calcArea( XYDataWrapper^ xyData );

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
			 * @fn double calcFwhm( XYDataWrapper^ xyData )
			 * @brief calculates FWHM
			 * @param[in] xyData data points
			 * @return FWHM
			 */
			double calcFwhm( XYDataWrapper^ xyData );

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
			 * @fn void setCharge( int charge )
			 * @brief sets the charge state
			 * @param[in] charge charge state
			 */
			void setCharge( int charge );

			/**
			 * @fn int getCharge()
			 * @brief gets the charge state
			 * @return charge state
			 */
			int getCharge();
		};
	}
}


#endif		// __KOME_CLR_PEAK_ELEMENT_WRAPPER_H__

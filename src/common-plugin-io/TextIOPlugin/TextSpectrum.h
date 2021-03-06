/**
 * @file TextSpectrum.h
 * @brief interfaces of TextSpectrum class
 *
 * @author S.Tanaka
 * @date 2010.01.08
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_IO_TEXT_SPECTRUM_H__
#define __KOME_IO_TEXT_SPECTRUM_H__



namespace kome {
	namespace io {
		namespace text {

			class TextSample;

			/**
			 * @class TextSpectrum
			 * @brief text sample spectrum class
			 */
			class TextSpectrum : public kome::objects::Spectrum {
			public:
				/**
				 * @fn TextSpectrum( TextSample* sample, const char* name )
				 * @brief constructor
				 * @param sample sample object
				 * @param name spectrum name
				 */
				TextSpectrum( TextSample* sample, const char* name );

				/**
				 * @fn virtual ~TextSpectrum()
				 * @brief destructor
				 */
				virtual ~TextSpectrum();

			protected:
				/** text sample */
				TextSample* m_textSample;

				/** data position */
				long long m_pos;

				/** min m/z */
				double m_minMz;

				/** max m/z */
				double m_maxMz;

			public:
				/**
				 * @fn TextSample* getTextSample()
				 * @brief gets text sample object
				 * @return text sample
				 */
				TextSample* getTextSample();

				/**
				 * @fn void setDataPos( const long long pos )
				 * @brief sets data position
				 * @param pos data position
				 */
				void setDataPos( const long long pos );

				/**
				 * @fn long long getDataPos()
				 * @brief gets data position
				 * @return data position
				 */
				long long getDataPos();

			public:
				/**
				 * @fn void addPoint( const double x, const double y )
				 * @brief adds point
				 * @param x the x-coordinate of the point
				 * @param y the y-coordinate of the point
				 */
				void addPoint( const double x, const double y );

			protected:
				/**
				 * @fn virtual void onGetXYData( kome::core::XYData* const xyData, const double minX, const double maxX )
				 * @brief This method is called by getXYData method. (override method)
				 * @param xyData the object to store data points
				 * @param minX minimum x value. (If minX is negative number, minimum x value is not unbounded.)
				 * @param maxX maximum x value. (If maxX is negative number, maximum x value is not unbounded.)
				 */
				virtual void onGetXYData( kome::core::XYData* const xyData, const double minX, const double maxX );

				/**
				 * @fn virtual void onGetXRange( double* minX, double* maxX )
				 * @brief This method is called by getMinX or getMaxX method. (override method)
				 * @param minX the pointer to store minimum x value
				 * @param maxX the pointer to store maximum x value
				 */
				virtual void onGetXRange( double* minX, double* maxX );

				/**
				 * @fn virtual double onGetTotalIntensity( const double minX, const double maxX )
				 * @brief This method is called by getTotalIntensity method. (override method)
				 * @param minX minimum x of range (If minX is negative number, minimum x value is unbounded.)
				 * @param maxX maximum x of range (If maxX is negative number, maximum x value is unbounded.)
				 * @return total intensity
				 */
				virtual double onGetTotalIntensity( const double minX, const double maxX );

				/**
				 * @fn virtual double onGetMaxIntensity( const double minX, const double maxX )
				 * @brief This method is called by getMaxIntensity method. (override method)
				 * @param minX minimum x of range (If minX is negative number, minimum x value is unbounded.)
				 * @param maxX maximum x of range (If maxX is negative number, maximum x value is unbounded.)
				 * @return max intensity
				 */
				virtual double onGetMaxIntensity( const double minX, const double maxX );

				// >>>>>> @Date:2013/09/05	<Add>	A.Ozaki
				//
				/**
				 * @fn virtual  bool onSetRequestLoadData( void )
				 * @brief This method is called by setRequestLoadData method. (abstract method)
				 */
				virtual void	onSetRequestLoadData( void );
				/**
				 * @fn virtual  bool onResetRequestLoadData( void )
				 * @brief This method is called by resetRequestLoadData method. (abstract method)
				 */
				virtual void	onResetRequestLoadData( void );
				
				/**
				 * @fn virtual  bool onIsRequestLoadData( void )
				 * @brief This method is called by isRequestLoadData method. (abstract method)
				 * @return If true, file read request is valid.
				 */
				virtual bool	onIsRequestLoadData( void );

				/**
				 * @fn virtual  bool onSetFirstAccess( void )
				 * @brief This method is called by setFirstAccess method. (abstract method)
				 */
				virtual void	onSetFirstAccess( void );

				/**
				 * @fn virtual  bool onResetFirstAccess( void )
				 * @brief This method is called by resetFirstAccess method. (abstract method)
				 */
				virtual void	onResetFirstAccess( void );

				/**
				 * @fn virtual  bool onIsFirstAccess( void )
				 * @brief This method is called by isFirstAccess method. (abstract method)
				 * @return If true, the first accessing.
				 */
				virtual bool	onIsFirstAccess( void );

				/**
				 * @fn virtual  bool onLoadData( void )
				 * @brief This method is called by loadData method. (abstract method)
				 * @return If true, file reading success.
				 */
				virtual bool	onLoadData( void );
				//
				// <<<<<< @Date:2013/09/05	<Add>	A.Ozaki
			};
		}
	}
}

#endif		// __KOME_IO_TEXT_SPECTRUM_H__

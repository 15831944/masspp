/**
 * @file MzXmlSpectrum.h
 * @brief interfaces of MzXmlSpectrum class
 *
 * @author S.Tanaka
 * @date 2007.03.27
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_IO_MZXML_SPECTRUM_H__
#define __KOME_IO_MZXML_SPECTRUM_H__


#include <vector>


namespace kome {
	namespace io {
		namespace mzxml {

			class MzXmlSample;

			/**
			 * @class MzXmlSpectrum
			 * @brief mzxml sample spectrum class
			 */
			class MzXmlSpectrum : public kome::objects::Spectrum {
			public:
				/**
				 * @fn MzXmlSpectrum( MzXmlSample* file, const char* name )
				 * @brief constructor
				 * @param file sample file
				 * @param name spectrum name
				 */
				MzXmlSpectrum( MzXmlSample* file, const char* name );

				/**
				 * @fn virtual ~MzXmlSpectrum()
				 * @brief destructor
				 */
				virtual ~MzXmlSpectrum();

			protected:
				/** data offset */
				long long m_offset;

				/** data align */
				long long m_align;

			public:
				/**
				 * @fn void setOffset( const long long offset )
				 * @brief sets data offset
				 * @param offset data offset
				 */
				void setOffset( const long long offset );

				/**
				 * @fn long long getOffset()
				 * @brief gets data offset
				 * @return data offset
				 */
				long long getOffset();

				/**
				 * @fn void setAlign( const long long align )
				 * @brief sets the data align
				 * @param[in] align
				 */
				void setAlign( const long long align );

				/**
				 * @fn long long getAlign()
				 * @brief gets the align
				 * @return align
				 */  
				long long getAlign();

			protected:
				/**
				 * @fn void parse( xercesc::SAX2XMLReader* parser )
				 * @brief parse XML data
				 * @param parser parser
				 */
				void parse( xercesc::SAX2XMLReader* parser );

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


#endif		// __KOME_IO_MZXML_SPECTRUM_H__

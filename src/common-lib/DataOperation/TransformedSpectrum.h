/**
 * @file TransformedSpectrum.h
 * @brief interfaces of TransformedSpectrum class
 *
 * @author S.Tanaka
 * @date 2008.05.08
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_OPERATION_TRANSFORMED_SPECTRUM_H__
#define __KOME_OPERATION_TRANSFORMED_SPECTRUM_H__


namespace kome {
	namespace operation {

		/**
		 * @class TransformedSpectrum
		 * @brief transformed spectrum class
		 */
		class DATA_OPERATION_CLASS TransformedSpectrum : public kome::objects::Spectrum {
		public:
			/**
			 * @fn TransformedSpectrum( kome::objects::Spectrum& org, const bool op = false )
			 * @brief constructor
			 * @param[in] org original spectrum
			 * @param[in] op If true, processed data is gotten when getting data points.
			 */
			TransformedSpectrum( kome::objects::Spectrum& org, const bool op = false );

			/**
			 * @fn virtual ~TransformedSpectrum()
			 * @brief destructor
			 */
			virtual ~TransformedSpectrum();

		protected:
			/** original spectrum */
			kome::objects::Spectrum& m_org;

		protected:
			/** x scale */
			double m_scX;

			/** x translation */
			double m_trX;

			/** y scale */
			double m_scY;

			/** y translation */
			double m_trY;
		public:
			/**
			 * @fn void setXScale( const double sc )
			 * @brief sets x scale
			 * @param[in] sc x scale
			 */
			void setXScale( const double sc );

			/**
			 * @fn double getXScale()
			 * @brief gets x scale
			 * @return x scale
			 */
			double getXScale();

			/**
			 * @fn void setXTranslation( const double tr )
			 * @brief sets x translation
			 * @param[in] tr x translation
			 */
			void setXTranslation( const double tr );

			/**
			 * @fn double getXTranslation()
			 * @brief gets x translation
			 * @return x translation
			 */
			double getXTranslation();

			/**
			 * @fn void setYScale( const double sc )
			 * @brief sets y scale
			 * @param[in] sc y scale
			 */
			void setYScale( const double sc );

			/**
			 * @fn double getYScale()
			 * @brief gets y scale
			 * @return y scale
			 */
			double getYScale();

			/**
			 * @fn void setYTranslation( const double tr )
			 * @brief sets y translation
			 * @param[in] tr y translation
			 */
			void setYTranslation( const double tr );

			/**
			 * @fn double getYTranslation()
			 * @brief gets y translation
			 * @return y translation
			 */
			double getYTranslation();

		protected:
			/**
			 * @fn virtual void onGetXYData( kome::core::XYData* const xyData, const double minX, const double maxX )
			 * @brief This method is called by getXYData method. (override method)
			 * @param[out] xyData the object to store data points
			 * @param[in] minX minimum x value. (If minX is negative number, minimum x value is not unbounded.)
			 * @param[in] maxX maximum x value. (If maxX is negative number, maximum x value is not unbounded.)
			 */
			virtual void onGetXYData( kome::core::XYData* const xyData, const double minX, const double maxX );

			/**
			 * @fn virtual void onGetXRange( double* minX, double* maxX )
			 * @brief This method is called by getMinX or getMaxX method. (override method)
			 * @param[out] minX the pointer to store minimum x value
			 * @param[out] maxX the pointer to store maximum x value
			 */
			virtual void onGetXRange( double* minX, double* maxX );

			/**
			 * @fn virtual double onGetTotalIntensity( const double minX, const double maxX )
			 * @brief This method is called by getTotalIntensity method. (override method)
			 * @param[in] minX minimum x of range (If minX is negative number, minimum x value is unbounded.)
			 * @param[in] maxX maximum x of range (If maxX is negative number, maximum x value is unbounded.)
			 * @return total intensity
			 */
			virtual double onGetTotalIntensity( const double minX, const double maxX );

			/**
			 * @fn virtual double onGetMaxIntensity( const double minX, const double maxX )
			 * @brief This method is called by getMaxIntensity method. (override method)
			 * @param[in] minX minimum x of range (If minX is negative number, minimum x value is unbounded.)
			 * @param[in] maxX maximum x of range (If maxX is negative number, maximum x value is unbounded.)
			 * @return max intensity
			 */
			virtual double onGetMaxIntensity( const double minX, const double maxX );

			// >>>>>> @Date:2013/09/05	<Add>	A.Ozaki
			//
			/**
			 * @fn virtual  void onSetRequestLoadData( void )
			 * @brief This method is called by setRequestLoadData method. (abstract method)
			 */
			virtual void	onSetRequestLoadData( void );
			/**
			 * @fn virtual  void onResetRequestLoadData( void )
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
			 * @fn virtual  void onSetFirstAccess( void )
			 * @brief This method is called by setFirstAccess method. (abstract method)
			 */
			virtual void	onSetFirstAccess( void );

			/**
			 * @fn virtual void onResetFirstAccess( void )
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

#endif		// __KOME_OPERATION_TRANSFORMED_SPECTRUM_H__

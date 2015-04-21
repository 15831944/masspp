/**
 * @file DataSpectrum.h
 * @brief interfaces of DataSpectrum class
 *
 * @author S.Tanaka
 * @date 2008.09.29
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_QUANT_DATA_SPECTRUM_H__
#define __KOME_QUANT_DATA_SPECTRUM_H__


namespace kome {
	namespace ident {

		/**
		 * @class DataSpectrum
		 * @brief dummy chromatogram class to draw data profile
		 */
		class IDENT_TOOLS_CLASS DataSpectrum : public kome::objects::Spectrum {
		public:
			/**
			 * @fn DataSpectrum( const int peakId, const int matrixSampleId )
			 * @brief constructor
			 * @param[in] peakId peak ID
			 * @param[in] matrixSampleId matrix sample ID
			 */
			DataSpectrum( const int peakId, const int matrixSampleId );

			/**
			 * @fn virtual ~DataSpectrum()
			 * @brief destructor
			 */
			virtual ~DataSpectrum();

		protected:
			/** peak ID */
			const int m_peakId;

			/** matrix sample ID */
			const int m_matrixSampleId;

			/** matrix peak position */
			double m_matrixPos;

			/** spectrum peak position */
			double m_specPos;

			/** scale */
			double m_scale;

			/** left position */
			kome::core::Point< double > m_left;

			/** right position */
			kome::core::Point< double > m_right;

		protected:
				/** normalized */
			bool m_normalized;

			/** aligned flag */
			bool m_aligned;

		protected:
			/** spectrum set */
			static std::set< DataSpectrum* > m_specSet;

		public:
			/**
			 * @fn int getMatrixPeakId()
			 * @brief gets the matrix peak ID
			 * @return matrix peak ID
			 */
			int getMatrixPeakId();

			/**
			 * @fn int getMatrixSampleId()
			 * @brief gets the matrix sample ID
			 * @return matrix sample ID
			 */
			int getMatrixSampleId();

		public:
			/**
			 * @fn double getGap()
			 * @brief gets the gap
			 * @return gap
			 */
			double getGap();

			/**
			 * @fn double getScale()
			 * @brief gets the scale
			 * @return scale
			 */
			double getScale();

		public:
			/**
			 * @fn void setPeakLeft( const double leftX, const double leftY )
			 * @brief sets peak left
			 * @param[in] leftX x coordinate of peak left
			 * @param[in] leftY y coordinate of peak left
			 */
			void setPeakLeft( const double leftX, const double leftY );

			/**
			 * @fn kome::core::Point< double > getPeakLeft()
			 * @brief gets peak left
			 * @return peak left position
			 */
			kome::core::Point< double > getPeakLeft();

			/**
			 * @fn void setPeakRight( const double rightX, const double rightY )
			 * @brief sets peak right
			 * @param[in] rightX x coordinate of peak right
			 * @param[in] rightY y coordinate of peak right
			 */
			void setPeakRight( const double rightX, const double rightY );

			/**
			 * @fn kome::core::Point< double > getPeakRight()
			 * @brief gets the peak right
			 * @return peak right position
			 */
			kome::core::Point< double > getPeakRight();

		public:
			/**
			 * @fn void setNormalized( const bool normalized )
			 * @brief sets the normalized flag value
			 * @param[in] normalized normalized flag value
			 */
			void setNormalized( const bool normalized );

			/**
			 * @fn bool isNormalized()
			 * @brief gets the normalized flag value
			 * @return normalized flag value
			 */
			bool isNormalized();

			/**
			 * @fn void setAligned( const bool aligned )
			 * @brief sets the aligned flag value
			 * @param[in] aligned aligned flag value
			 */
			void setAligned( const bool aligned );

			/**
			 * @fn bool isAligned()
			 * @brief gets the aligned flag value
			 * @return aligned flag value
			 */
			bool isAligned();

		public:
			/**
			 * @fn void updateSpec()
			 * @brief updates spectrum
			 */
			void updateSpec();

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
			 * @fn virtual void onSetRequestLoadData( void )
			 * @brief This method is called by setRequestLoadData method. (abstract method)
			 */
			virtual void	onSetRequestLoadData( void );
			/**
			 * @fn virtual void onResetRequestLoadData( void )
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

#endif		// __KOME_QUANT_DATA_SPECTRUM_H__

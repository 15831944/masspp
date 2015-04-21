/**
 * @file DataChromatogram.h
 * @brief interfaces of DataChromatogram class
 *
 * @author S.Tanaka
 * @date 2008.04.28
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_QUANT_DATA_CHROMATOGRAM_H__
#define __KOME_QUANT_DATA_CHROMATOGRAM_H__


namespace kome {
	namespace ident {

		/**
		 * @class DataChromatogram
		 * @brief dummy chromatogram class to draw data profile
		 */
		class IDENT_TOOLS_CLASS DataChromatogram : public kome::objects::Chromatogram {
		public:
			/**
			 * @fn DataChromatogram( const int idx )
			 * @brief constructor
			 * @param[in] peakId peak ID
			 * @param[in] matrixSampleId matrix sample ID
			 */
			DataChromatogram( const int peakId, const int matrixSampleId );

			/**
			 * @fn virtual ~DataChromatogram()
			 * @brief destructor
			 */
			virtual ~DataChromatogram();

		protected:
			/** peak ID */
			const int m_peakId;

			/** matrix sample ID */
			const int m_matrixSampleId;

			/** matrix peak position */
			double m_matrixPos;

			/** chromatogram peak position */
			double m_chromPos;

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
			/** chromatogram set */
			static std::set< DataChromatogram* > m_chromSet;

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
			 * @fn void updateChrom()
			 * @brief updates chromatogram
			 */
			void updateChrom();

		protected:
			/**
			 * @fn virtual void onGetXYData( kome::core::XYData* const xyData )
			 * @brief gets chromatogram data points (override method)
			 * @param[out] xyData object to store data points
			 */
			virtual void onGetXYData( kome::core::XYData* const xyData );

			/**
			 * @fn virtual void onGetSpectra(
						kome::objects::DataSet& dataSet,
						const double startRt,
						const double endRt,
						const kome::objects::SearchType startSearch,
						const kome::objects::SearchType endSearch
					)
			 * @brief This method is called by searchSpectrum or getSpectra method (override method)
			 * @param[out] dataSet dataSet object to store spectra
			 * @param[in] startRt start RT
			 * @param[in] endRt end RT
			 * @param[in] startSearch start search type
			 * @param[in] endSearch end search type
			 */
			virtual void onGetSpectra(
				kome::objects::DataSet& dataSet,
				const double startRt,
				const double endRt,
				const kome::objects::SearchType startSearch,
				const kome::objects::SearchType endSearch
			);

			/**
			 * @fn virtual double onGetMass( const unsigned int index )
			 * @brief This method is called by getMass method (override method)
			 * @param[in] index data index
			 * @return mass
			 */
			virtual double onGetMass( const unsigned int index );

			/**
			 * @fn virtual int onGetMsStage( const unsigned int index )
			 * @brief This method is called by getMsStage method (override method)
			 * @param[in] index data index
			 * @return MS stage.
			 */
			virtual int onGetMsStage( const unsigned int index );

			/**
			 * @fn virtual double onGetPrecursor( const unsigned int index )
			 * @brief This method is called by getPrecursor method (override method)
			 * @param[in] index data index
			 * @return precursor mass
			 */
			virtual double onGetPrecursor( const unsigned int index );
		};
	}
}

#endif		// __KOME_QUANT_DATA_CHROMATOGRAM_H__

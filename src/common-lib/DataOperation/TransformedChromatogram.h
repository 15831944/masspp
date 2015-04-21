/**
 * @file TransformedChromatogram.h
 * @brief interfaces of TransformedChromatogram class
 *
 * @author S.Tanaka
 * @date 2008.05.08
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_OPERATION_TRANSFORMED_CHROMATOGRAM_H__
#define __KOME_OPERATION_TRANSFORMED_CHROMATOGRAM_H__


namespace kome {
	namespace operation {

		/**
		 * @class TransformedChromatogram
		 * @brief transformed spectrum class
		 */
		class DATA_OPERATION_CLASS TransformedChromatogram : public kome::objects::Chromatogram {
		public:
			/**
			 * @fn TransformedChromatogram( kome::objects::Chromatogram& org, const bool op = false )
			 * @brief constructor
			 * @param[in] org original spectrum
			 * @param[in] op If true, processed data is gotten when getting data points.
			 */
			TransformedChromatogram( kome::objects::Chromatogram& org, const bool op = false );

			/**
			 * @fn virtual ~TransformedChromatogram()
			 * @brief destructor
			 */
			virtual ~TransformedChromatogram();

		protected:
			/** original chromatogram */
			kome::objects::Chromatogram& m_org;

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
			 * @fn virtual void onGetXYData( kome::core::XYData* const xyData )
			 * @brief This method is called by getXYData method (override method)
			 * @param[out] xyData object to store data points
			 */
			virtual void onGetXYData( kome::core::XYData* const xyData );

			/**
			 * @fn virtual void onGetSpectra(
					kome::objects::DataSet& spectra,
					const double startRt,
					const double endRt,
					const kome::objects::SearchType startSearch,
					const kome::objects::SearchType endSearch
				)
			 * @brief This method is called by searchSpectrum or getDataSet method (overriede method)
			 * @param[out] spectra spectra object to store spectra
			 * @param[in] startRt start RT
			 * @param[in] endRt end RT
			 * @param[in] startSearch start search type
			 * @param[in] endSearch end search type
			 */
			virtual void onGetSpectra(
				kome::objects::DataSet& spectra,
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

#endif		// __KOME_OPERATION_TRANSFORMED_CHROMATOGRAM_H__

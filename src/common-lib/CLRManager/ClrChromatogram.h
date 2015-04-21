/**
 * @file ClrChromatogram.h
 * @brief interfaces of ClrChromatogram class
 *
 * @author S.Tanaka
 * @date 2008.04.14
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_CLR_CHROMATOGRAM_H__
#define __KOME_CLR_CHROMATOGRAM_H__


#include "ClrChromatogramBase.h"


namespace kome {
	namespace clr {

		/**
		 * @class ClrChromatogram
		 * @brief Chromatogram class that has managed class object
		 */
		class ClrChromatogram : public kome::objects::Chromatogram {
		public:
			/**
			 * @fn ClrChromatogram( SampleWrapper^ sample )
			 * @brief constructor
			 * @param[in] sample sample object that has this spectrum
			 */
			ClrChromatogram( SampleWrapper^ sample );

			/**
			 * @fn virtual ~ClrChromatogram()
			 * @brief destructor
			 */
			virtual ~ClrChromatogram();

		protected:
			/** base spectrum object */
			gcroot< ClrChromatogramBase^ > m_baseChromatogram;

		public:
			/**
			 * @fn void setBaseChromatogram( ClrChromatogramBase^ baseChromatogram )
			 * @brief sets base spectrum object
			 * @param[in] baseChromatogram base spectrum object
			 */
			void setBaseChromatogram( ClrChromatogramBase^ baseChromatogram );

			/**
			 * @fn ClrChromatogramBase^ getBaseChromatogram()
			 * @brief gets base spectrum object
			 * @return base spectrum object
			 */
			ClrChromatogramBase^ getBaseChromatogram();

		protected:
			/**
			 * @fn virtual void onGetXYData( kome::core::XYData* const xyData )
			 * @brief This method is called by getXYData method. (override method)
			 * @param[out] xyData the object to store data points
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
			 * @brief This method is called by searchSpectrum or getDataSet method (override method)
			 * @param[out] dataSet data set object to store spectra
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

#endif	// __KOME_CLR_CHROMATOGRAM_H__

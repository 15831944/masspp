/**
 * @file BaselineChromatogram.h
 * @brief interfaces of BaselineChromatogram class
 *
 * @author S.Tanaka
 * @date 2007.07.05
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#ifndef __KOME_BASELINE_CHROMATOGRAM_H__
#define __KOME_BASELINE_CHROMATOGRAM_H__


namespace kome {
	namespace baseline {
		
		/**
		 * @class BaselineChromatogram
		 * @brief dummy chromatogram class to draw baseline
		 */
		class BaselineChromatogram : public kome::objects::Chromatogram {
		public:
			/**
			 * @fn BaselineChromatogram(
					kome::objects::Chromatogram& chrom,
					kome::plugin::PluginCall& fun,
					kome::objects::SettingParameterValues* settings
				)
			 * @brief constructor
			 * @param chrom chromatogram
			 * @param fun baseline function
			 * @param settings setting parameter values
			 */
			BaselineChromatogram(
				kome::objects::Chromatogram& chrom,
				kome::plugin::PluginCall& fun,
				kome::objects::SettingParameterValues* settings
			);

			/**
			 * @fn virtual ~BaselineChromatogram()
			 * @brief destructor
			 */
			virtual ~BaselineChromatogram();

		protected:
			/** spectrum */
			kome::objects::Chromatogram& m_chrom;

			/** item */
			kome::plugin::PluginFunctionItem m_item;

			/** settings */
			kome::objects::SettingParameterValues m_settings;

		public:
			/**
			 * @fn virtual void onGetXYData( kome::core::XYData* const xyData )
			 * @brief gets chromatogram data points (override method)
			 * @param xyData object to store data points
			 * @param op If true, getting operated data points
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
			 * @param spectra spectra object to store spectra
			 * @param startRt start RT
			 * @param endRt end RT
			 * @param startSearch start search type
			 * @param endSearch end search type
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
			 * @param index data index
			 * @return mass
			 */
			virtual double onGetMass( const unsigned int index );

			/**
			 * @fn virtual int onGetMsStage( const unsigned int index )
			 * @brief This method is called by getMsStage method (override method)
			 * @param index data index
			 * @return MS stage.
			 */
			virtual int onGetMsStage( const unsigned int index );

			/**
			 * @fn virtual double onGetPrecursor( const unsigned int index )
			 * @brief This method is called by getPrecursor method (override method)
			 * @param index data index
			 * @return precursor mass
			 */
			virtual double onGetPrecursor( const unsigned int index );
		};
	}
}


#endif		// __KOME_BASELINE_CHROMATOGRAM_H__

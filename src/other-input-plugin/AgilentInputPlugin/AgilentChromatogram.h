/**
 * @file AgilentChromatogram.h
 * @brief interfaces of AgilentChromatogram class
 *
 * @author M.Fukuda
 * @date 2013.12.13
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_IO_AGILENT_CHROMATOGRAM_H__
#define __KOME_IO_AGILENT_CHROMATOGRAM_H__

namespace kome {
	namespace io {
		namespace agilent {

			ref class AgilentSample;

			/**
			 * @class AgilentSpectrum
			 * @brief agilent spectrum data class
			 */
			public ref class AgilentChromatogram : public kome::clr::ClrChromatogramBase {
			
			public:
				// Constructor
				explicit AgilentChromatogram(
					AgilentSample^ srcsample,
					System::String^ sname,
					System::String^ iconname,
					Agilent::MassSpectrometry::DataAnalysis::IBDAChromData^ chrom
					);

				// destructor
				~AgilentChromatogram();

			protected:
				// sample
				AgilentSample^ mysample_;
				Agilent::MassSpectrometry::DataAnalysis::IBDAChromData^ chrom_;

				// scan type
				System::String^ m_scanType;

			public:

				//@brief This method is called by getXYData method. (abstract method)
				//@param[out] xyData the object to store data points
				virtual void onGetXYData( 
					kome::clr::XYDataWrapper^ xyData ) override;

				// @brief get spectra in specified RT range (abstract method)
				// @param[out] dataSet data set object to store spectra
				// @param[in] startRt start RT
				// @param[in] endRt end RT
				// @param[in] startSearch start search type
				// @param[in] endSearch end search type
				virtual void onGetSpectra(
					kome::clr::DataSetWrapper^ dataSet,
					double startRt,
					double endRt,
					kome::clr::SearchType startSearch,
					kome::clr::SearchType endSearch
					) override {};

				/**
				 * @brief This method is called by getMass method (abstract method)
				 * @param[in] index data index
				 * @return mass
				 */
				virtual double onGetMass( unsigned int index ) override { return 1.0;}

				/**
				 * @brief This method is called by getMsStage method (abstract method)
				 * @param[in] index data index
				 * @return MS stage.
				 */
				virtual int onGetMsStage( unsigned int index ) override {return 1;}

				/**
				 * @brief This method is called by getPrecursor method (abstract method)
				 * @param[in] index data index
				 * @return precursor mass
				 */
				virtual double onGetPrecursor( unsigned int index ) override {return 1;}
			};
		}
	}
}

#endif

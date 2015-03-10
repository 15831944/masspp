/**
 * @file AgilentSpectrum.h
 * @brief interfaces of AgilentSpectrum class
 *
 * @author S.Tanaka
 * @date 2009.05.27
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_IO_AGILENT_SPECTRUM_H__
#define __KOME_IO_AGILENT_SPECTRUM_H__


namespace kome {
	namespace io {
		namespace agilent {

			ref class AgilentSample;

			/**
			 * @class AgilentSpectrum
			 * @brief agilent spectrum data class
			 */
			public ref class AgilentSpectrum : public kome::clr::ClrSpectrumBase {
			public:
				/**
				 * @brief constructor
				 * @param sample agilent sample object that has this spectrum.
				 * @param name spectrum name
				 * @param spec spectrum data
				 */
				AgilentSpectrum(
					AgilentSample^ sample,
					System::String^ name,
					Agilent::MassSpectrometry::DataAnalysis::IBDASpecData^ spec
				);

				/**
				 * @fn virtual ~AgilentSpectrum()
				 * @brief destructor
				 */
				virtual ~AgilentSpectrum();

			protected:
				/** agilent sample file object */
				AgilentSample^ m_sample;

				/** spectrum */
				Agilent::MassSpectrometry::DataAnalysis::IBDASpecData^ m_spec;
			public:
				// Getter
				const int scanId(){return m_spec->ScanId;}
				const int parentScanId(){return m_spec->ParentScanId;}

			public:
				/**
				 * @brief This method is called by getXYDataOnCLR method. (override method)
				 * @param xyData the object to store data points
				 * @param minX minimum x value. (If minX is negative number, minimum x value is not specified.)
				 * @param maxX maximum x value. (If maxX is negative number, maximum x value is not specified.)
				 */
				virtual void onGetXYData(
					kome::clr::XYDataWrapper^ xyData,
					double minX,
					double maxX
				) override;


				/**
				 * @fn virtual void onGetXRange( double% minX, double% maxX )
				 * @brief This method is called by getXRangeOnCLR method. (override method)
				 * @param minX the pointer to store minimum x value
				 * @param maxX the pointer to store maximum x value
				 */
				virtual void onGetXRange( double% minX, double% maxX ) override;

				/**
				 * @fn virtual double onGetTotalIntensity( double minX, double maxX )
				 * @brief This method is called by getTotalIntensityOnCLR method. (override method)
				 * @param minX minimum x of range (If minX is negative number, minimum x value is unbounded.)
				 * @param maxX maximum x of range (If maxX is negative number, maximum x value is unbounded.)
				 * @return total intensity
				 */
				virtual double onGetTotalIntensity( double minX, double maxX ) override;

				/**
				 * @fn virtual double onGetMaxIntensity( double minX, double maxX )
				 * @brief This method is called by getMaxIntensityOnCLR method. (override method)
				 * @param minX minimum x of range (If minX is negative number, minimum x value is unbounded.)
				 * @param maxX maximum x of range (If maxX is negative number, maximum x value is unbounded.)
				 * @return max intensity
				 */
				virtual double onGetMaxIntensity( double minX, double maxX ) override;
			};
		}
	}
}

#endif	// __KOME_IO_AGILENT_SPECTRUM_H__

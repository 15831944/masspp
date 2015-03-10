/**
 * @file WatersSpectrum.h
 * @brief interfaces of WatersSpectrum class
 *
 * @author S.Tanaka
 * @date 2006.11.01
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_IO_WATERS_SPECTRUM_H__
#define __KOME_IO_WATERS_SPECTRUM_H__


namespace kome {
	namespace io {
		namespace waters {

			ref class WatersSample;

			/**
			 * @class WatersSpectrum
			 * @brief waters spectrum data class
			 */
			public ref class WatersSpectrum : public kome::clr::ClrSpectrumBase {
			public:
				/**
				 * @fn WatersSpectrum( WatersSample^ sample, System::String^ name )
				 * @brief constructor
				 * @param sample waters sample object that has this spectrum.
				 * @param name spectrum name
				 */
				WatersSpectrum( WatersSample^ sample, System::String^ name );

				/**
				 * @fn virtual ~WatersSpectrum()
				 * @brief destructor
				 */
				virtual ~WatersSpectrum();

			protected:
				/** waters sample file object */
				WatersSample^ m_sample;

			public:
				/**
				 * @fn virtual void onGetXYData(
						kome::clr::XYDataWrapper^ xyData,
						double minX,
						double maxX
					)
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

#endif	// __KOME_IO_WATERS_SPECTRUM_H__

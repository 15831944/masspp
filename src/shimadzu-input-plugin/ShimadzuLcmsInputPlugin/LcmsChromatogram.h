/**
 * @file LcmsChromatogram.h
 * @brief interfaces of LcmsChromatgram class
 *
 * @author ----
 * @date ----
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_IO_LCMS_CHROMATOGRAM_H__
#define __KOME_IO_LCMS_CHROMATOGRAM_H__

// #include "LcmsSpectrumBase.h"


namespace kome {
	namespace io {
		namespace lcms {

			ref class LcmsSample;

			/**
			 * @class LcmsChromatgram
			 * @brief chromatgram object class that Lcms raw data has
			 */
			public ref class LcmsChromatogram : public kome::clr::ClrChromatogramBase {
			public:
				/**
				 * @fn LcmsChromatgram( LcmsSample^ sample )
				 * @brief constructor
				 * @param sample sample object that has this chromatgram
				 * @param seg segment number
				 * @param evt event number
				 */
				LcmsChromatogram( LcmsSample^ sample, const int seg, const int evt );

				/**
				 * @fn ~LcmsChromatgram()
				 * @brief destructor
				 */
				~LcmsChromatogram();

			protected:
				/** sample */
				LcmsSample^ m_sample;

				/** segment number */
				int m_segNum;

				/** event number */
				int m_evtNum;

			public:
				/**
				 * @fn int getSegmentNumber()
				 * @brief gets the segment number
				 * @return segment number
				 */
				int getSegmentNumber();

				/**
				 * @fn int getEventNumber()
				 * @brief gets the event number
				 * @return event number
				 */
				int getEventNumber();

			public:
				/**
				 * @fn virtual void onGetXYData( XYDataWrapper^ xyData )
				 * @brief This method is called by getXYData method. (abstract method)
				 * @param[out] xyData the object to store data points
				 */
				virtual void onGetXYData( kome::clr::XYDataWrapper^ xyData ) override;

				/**
				 * @fn virtual void onGetSpectra(
							DataSetWrapper^ dataSet,
							double startRt,
							double endRt,
							SearchType startSearch,
							SearchType endSearch
					)
				 * @brief get spectra in specified RT range (abstract method)
				 * @param[out] dataSet data set object to store spectra
				 * @param[in] startRt start RT
				 * @param[in] endRt end RT
				 * @param[in] startSearch start search type
				 * @param[in] endSearch end search type
				 */
				virtual void onGetSpectra(
						kome::clr::DataSetWrapper^ dataSet,
						double startRt,
						double endRt,
						kome::clr::SearchType startSearch,
						kome::clr::SearchType endSearch
				) override;

				/**
				 * @fn virtual double onGetMass( unsigned int index ) = 0
				 * @brief This method is called by getMass method (abstract method)
				 * @param[in] index data index
				 * @return mass
				 */
				virtual double onGetMass( unsigned int index ) override;

				/**
				 * @fn virtual int onGetMsStage( unsigned int index ) = 0
				 * @brief This method is called by getMsStage method (abstract method)
				 * @param[in] index data index
				 * @return MS stage.
				 */
				virtual int onGetMsStage( unsigned int index ) override;

				/**
				 * @fn virtual double onGetPrecursor( unsigned int index ) = 0
				 * @brief This method is called by getPrecursor method (abstract method)
				 * @param[in] index data index
				 * @return precursor mass
				 */
				virtual double onGetPrecursor( unsigned int index ) override;
			};
		}
	}
}

#endif		// __KOME_IO_LCMS_CHROMATOGRAM_H__

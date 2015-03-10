/**
 * @file MsbChromatogram.h
 * @brief interfaces of MsbChromatogram class
 *
 * @author S.Tanaka
 * @date 2008.04.22
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_IO_MSB_CHROMATOGRAM_H__
#define __KOME_IO_MSB_CHROMATOGRAM_H__


#include "MsbManager.h"

#include <vector>


namespace kome {
	namespace io {
		namespace msb {

			class MsbSample;

			/**
			 * @class MsbChromatogram
			 * @brief msb sample spectrum class
			 */
			class MsbChromatogram : public kome::objects::Chromatogram {
			public:
				/**
				 * @fn MsbChromatogram( MsbSample* sample, FILE* fp )
				 * @brief constructor
				 * @param sample sample object
				 * @param fp file descriptor
				 */
				MsbChromatogram( MsbSample* sample, FILE* fp );

				/**
				 * @fn virtual ~MsbChromatogram()
				 * @brief destructor
				 */
				virtual ~MsbChromatogram();

			protected:
				/** file descriptor */
				FILE* m_fp;

				/** data length */
				unsigned int m_length;

				/** RT data align */
				unsigned long long m_rtAlign;
				/** intenisty data align */
				unsigned long long m_intAlign;
				/** mass data align */
				unsigned long long m_massAlign;
				/** MS stage data align */
				unsigned long long m_msStageAlign;
				/** precursor data align */
				unsigned long long m_precursorAlign;

			public:
				/**
				 * @fn void setDataLength( const unsigned int len )
				 * @brief sets data length
				 * @param len data length
				 */
				void setDataLength( const unsigned int len );

				/**
				 * @fn unsigned int getDataLength()
				 * @brief gets data length
				 * @return data length
				 */
				unsigned int getDataLength();

				/**
				 * @fn void setRtDataAlign( const unsigned long long align )
				 * @brief sets RT data align
				 * @param align RT data align
				 */
				void setRtDataAlign( const unsigned long long align );

				/**
				 * @fn unsigned long long getRtDataAlign()
				 * @brief gets RT data align
				 * @return RT data align
				 */
				unsigned long long getRtDataAlign();

				/**
				 * @fn void setIntensityDataAlign( const unsigned long long align )
				 * @brief sets intensity data align
				 * @param align intenisty data align
				 */
				void setIntensityDataAlign( const unsigned long long align );

				/**
				 * @fn unsigned long long getIntensityDataAlign()
				 * @brief gets intensity data align
				 * @return intensity data align
				 */
				unsigned long long getIntensityDataAlign();

				/**
				 * @fn void setMassDataAlign( const unsigned long long align )
				 * @brief sets mass data align
				 * @param align mass data align
				 */				 
				void setMassDataAlign( const unsigned long long align );

				/**
				 * @fn unsigned long long getMassDataAlign()
				 * @brief gets mass data align
				 * @return mass data align
				 */
				unsigned long long getMassDataAlign();

				/**
				 * @fn void setMsStageDataAlign( const unsigned long long align )
				 * @brief sets MS stage data align
				 * @param align MS stage data align
				 */
				void setMsStageDataAlign( const unsigned long long align );

				/**
				 * @fn unsigned long long getMsStageDataAlign()
				 * @brief gets MS stage data align
				 * @return MS stage data align
				 */
				unsigned long long getMsStageDataAlign();

				/**
				 * @fn void setPrecursorDataAlign( const unsigned long long align )
				 * @brief sets precursor data aling
				 * @param align precursor data align
				 */
				void setPrecursorDataAlign( const unsigned long long align );

				/**
				 * @fn unsigned long long getPrecursorDataAlign()
				 * @brief gets precursor data align
				 * @return precursor data align
				 */
				unsigned long long getPrecursorDataAlign();

			protected:
				/**
				 * @fn virtual void onGetXYData( kome::core::XYData* const xyData )
				 * @brief This method is called by getXYData method (override method)
				 * @param xyData object to store data points
				 */
				virtual void onGetXYData( kome::core::XYData* const xyData );

				/**
				 * @fn virtual void onGetSpectra(
						kome::objects::DataSet& dataSet,
						const double startRt,
						const double endRt,
						const kome::objects::SearchType startSearch,
						const kome::objects::SearchType endSearc
					)
				 * @brief This method is called by searchSpectrum or getSpectra method (overriede method)
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
}


#endif		// __KOME_IO_MSB_CHROMATOGRAM_H__

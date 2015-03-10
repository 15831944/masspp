/**
 * @file MzmlChromatogram.h
 * @brief interfaces of MzmlChromatogram class
 *
 * @author S.Tanaka
 * @date 2009.04.17
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_IO_MZML_CHROMATOGRAM_H__
#define __KOME_IO_MZML_CHROMATOGRAM_H__


#include <vector>


namespace kome {
	namespace io {
		namespace mzml {

			class MzmlSample;

			/**
			 * @class MzmlChromatogram
			 * @brief mzml sample spectrum class
			 */
			class MzmlChromatogram : public kome::objects::Chromatogram {
			public:
				/**
				 * @fn MzmlChromatogram( MzmlSample* file )
				 * @brief constructor
				 * @param file sample
				 */
				MzmlChromatogram( MzmlSample* file );

				/**
				 * @fn virtual ~MzmlChromatogram()
				 * @brief destructor
				 */
				virtual ~MzmlChromatogram();

			protected:
				/** sample */
				MzmlSample* m_mzmlSample;

			protected:
				/** data offset */
				long long m_offset;

			public:
				/**
				 * @fn void setOffset( const long long offset )
				 * @brief sets data offset
				 * @param offset data offset
				 */
				void setOffset( const long long offset );

				/**
				 * @fn long long getOffset()
				 * @brief gets data offset
				 * @return data offset
				 */
				long long getOffset();

			protected:
				/**
				 * @fn void parse( xercesc::SAX2XMLReader* parser )
				 * @brief parse XML data
				 * @param parser parser
				 */
				void parse( xercesc::SAX2XMLReader* parser );

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


#endif		// __KOME_IO_MZML_CHROMATOGRAM_H__

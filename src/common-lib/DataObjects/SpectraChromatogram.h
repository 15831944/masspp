/**
 * @file SpectraChromatogram.h
 * @brief interfaces of SpectraChromatogram class
 *
 * @author S.Tanaka
 * @date 2008.04.14
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_OBJECTS_GROUP_CHROMAOTGRAM_H__
#define __KOME_OBJECTS_GROUP_CHROMAOTGRAM_H__


#include "DataObjectsCommon.h"
#include "Chromatogram.h"

#include <vector>


namespace kome {
	namespace objects {

		/**
		 * @class SpectraChromatogram 
		 * @brief spectrum group chromatogram class
		 */
		class DATA_OBJECTS_CLASS SpectraChromatogram : public Chromatogram {
		public:
			/**
			 * @fn SpectraChromatogram( DataSet& dataSet, bool bProces=false  )
			 * @brief constructor
			 * @param[in] dataSet spectra data set
			 * @param[in] bProces Processing data is used if it is true. 
			 */
			SpectraChromatogram( DataSet& dataSet, bool bProces=false );

			/**
			 * @fn virtual ~SpectraChromatogram()
			 * @brief destructor
			 */
			virtual ~SpectraChromatogram();

		protected:
			/** spectra */
			DataSet* m_spectra;

			/** minimum value of m/z range */
			double m_minMz;

			/** maximum value of m/z range */
			double m_maxMz;

			/** minimum value of RT range */
			double m_minRt;

			/** maximum value of RT range */
			double m_maxRt;

			/** filter */
			unsigned char m_filter;

			/** TIC flag (true: TIC, false: BPC) */
			bool m_tic;
			
			/** processed flg */
			bool m_proc;
		protected:
			/** ms stage filter */
			static const unsigned char m_msFilters[];

		public:
			/**
			 * @fn DataSet& getMemberSpectra()
			 * @brief gets spectra
			 * @return spectra
			 */
			DataSet& getMemberSpectra();

			/**
			 * @fn void setMzRange( const double minMz, const double maxMz )
			 * @brief sets m/z range
			 * @param[in] minMz minimum value of m/z range (If minMz is negative number, m/z range doesn't have lower bound.)
			 * @param[in] maxMz maximum value of m/z range (If minMz is negative number, m/z range doesn't have upper bound.)
			 */
			void setMzRange( const double minMz, const double maxMz );

			/**
			 * @fn void setMinMz( const double mz )
			 * @brief sets minimum value of m/z range
			 * @param[in] mz minimum value of m/z range (If mz is negative number, m/z range doesn't have lower bound.)
			 */
			void setMinMz( const double mz );

			/**
			 * @fn void setMaxMz( const double mz )
			 * @brief sets maximum value of m/z range
			 * @param[in] mz maximum value of m/z range (If mz is negative number, m/z range doesn't have upper bound.)
			 */
			void setMaxMz( const double mz );

			/**
			 * @fn double getMinMz()
			 * @brief gets minimum value of m/z range
			 * @return minimum value of m/z range (If m/z range doesn't have lower bound, this method returns negative number.)
			 */
			double getMinMz();

			/**
			 * @fn double getMaxMz()
			 * @brief gets maximum value of m/z range
			 * @return maximum value of m/z range (If m/z range doesn't have upper bound, this method returns negative number.)
			 */
			double getMaxMz();

			/**
			 * @fn void setRtRange( const double minRt, const double maxRt )
			 * @brief sets RT range
			 * @param[in] minRt minimum value of RT range (If minRt is negative number, RT range doesn't have lower bound.)
			 * @param[in] maxRt maximum value of RT range (If maxRt is negative number, RT range doesn't have upper bound.)
			 */
			void setRtRange( const double minRt, const double maxRt );

			/**
			 * @fn void setMinRt( const double rt )
			 * @brief sets minimum value of RT range
			 * @param[in] rt minimum value of RT range (If minRt is negative number, RT range doesn't have lower bound.)
			 */
			void setMinRt( const double rt );

			/**
			 * @fn void setMaxRt( const double rt )
			 * @brief sets maximum value of RT range
			 * @param[in] rt maximum value of RT range (If maxRt is negative number, RT range doesn't have upper bound.)
			 */
			void setMaxRt( const double rt );

			/**
			 * @fn double getMinRt()
			 * @brief gets minimum value of RT range
			 * @return minimum value of RT range (If RT range doesn't have lower bound, this method returns negative number.)
			 */
			double getMinRt();

			/**
			 * @fn double getMaxRt()
			 * @brief gets maximum value of RT range
			 * @return maximum value of RT range (If RT range doesn't have upper bound, this method returns negative number.)
			 */
			double getMaxRt();

			/**
			 * @fn void setFilter( const unsigned char filter )
			 * @brief sets filter
			 * @param[in] filter 
					  - The least significant bit is the ms1 filter.
					  - The second least significant bit is the ms2 filter.
					  - ...
					  - The most least significant bit is the ms8 or more filter.
			 */
			void setFilter( const unsigned char filter );

			/**
			 * @fn unsigned char getFilter()
			 * @brief gets filter
			 * @return filter
					  - The least significant bit is the ms1 filter.
					  - The second least significant bit is the ms2 filter.
					  - ...
					  - The most least significant bit is the ms8 or more filter.
			 */
			unsigned char getFilter();

			/**
			 * @fn void setTIC()
			 * @brief sets Total Ion SpectraChromatogram mode
			 */
			void setTIC();

			/**
			 * @fn void setBPC()
			 * @brief sets Base Peak SpectraChromatogram mode
			 */
			void setBPC();

			/**
			 * @fn bool isTIC()
			 * @brief judges whether chromatogram mode is TIC (Total Ion SpectraChromatogram)
			 * @return whether chromatogram mode is TIC (Total Ion SpectraChromatogram)
			 */
			bool isTIC();

			/**
			 * @fn bool isBPC()
			 * @brief judges whether chromatogram mode is BPC (Base Peak SpectraChromatogram)
			 * @return whether chromatogram mode is BPC (Base Peak SpectraChromatogram)
			 */
			bool isBPC();

		protected:
			/**
			 * @fn void setTitle()
			 * @brief sets title
			 */
			void setTitle();

			/**
			 * @fn void getIntensities()
			 * @brief gets intensities
			 */
			void getIntensities();

			/**
			 * @fn void getMemberSpectra( std::vector< Spectrum* >& spectra )
			 * @brief gets spectra of chromatogram point
			 * @param[out] spectra spectrum array object to store spectra
			 */
			void getMemberSpectra( std::vector< Spectrum* >& spectra );

			/**
			 * @fn int searchSpectrumIndex(
					std::vector< Spectrum* >& spectra,
					const double rt,
					SearchType search
				)
			 * @brief searches spectrum
			 * @param[in] spectra spectrum array
			 * @param[in] rt retention time
			 * @param[in] search search type
			 * @return spectrum index
			 */
			int searchSpectrumIndex(
				std::vector< Spectrum* >& spectra,
				const double rt,
				SearchType search
			);

		protected:
			/**
			 * @fn virtual void onGetXYData( kome::core::XYData* const xyData )
			 * @brief This method is called by getXYData method (override method)
			 * @param[out] xyData object to store data points
			 */
			virtual void onGetXYData( kome::core::XYData* const xyData );

			/**
			 * @fn virtual void onGetSpectra(
					DataSet& spectra,
					const double startRt,
					const double endRt,
					const SearchType startSearch,
					const SearchType endSearch
				)
			 * @brief This method is called by searchSpectrum or getDataSet method (overriede method)
			 * @param[out] spectra spectra object to store spectra
			 * @param[in] startRt start RT
			 * @param[in] endRt end RT
			 * @param[in] startSearch start search type
			 * @param[in] endSearch end search type
			 */
			virtual void onGetSpectra(
				DataSet& spectra,
				const double startRt,
				const double endRt,
				const SearchType startSearch,
				const SearchType endSearch
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

#endif	// __KOME_OBJECTS_GROUP_CHROMAOTGRAM_H__

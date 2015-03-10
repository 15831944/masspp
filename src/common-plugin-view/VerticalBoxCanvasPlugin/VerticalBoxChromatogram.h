/**
 * @file VerticalBoxChromatogram.h
 * @brief VerticalBoxChromatogram.cpp—pƒwƒbƒ_
 * @author OKADA, H.
 * @date 2010.11
 * Copyright (C) 2014 Shimadzu Corporation All rights reserved.
 * 
 */


#ifndef _VERTICALBOXCHROMATOGRAM_H_
#define _VERTICALBOXCHROMATOGRAM_H_

namespace kome {
	namespace objects {
		/**
		 * @class VerticalBoxSpectrum
		 * @brief dummy spectrum class for VerticalBox draw
		 */
		class VerticalBoxChromatogram : public kome::objects::Chromatogram {
		public:
			/**
			 * @fn VerticalBoxSpectrum()
			 * @brief constructor
			 */
			VerticalBoxChromatogram();

			/**
			 * @fn virtual ~VerticalBoxSpectrum()
			 * @brief destructor
			 */
			virtual ~VerticalBoxChromatogram();

		protected:
			/** spectra */
			std::vector< Chromatogram* > m_chroms;

		public:
			/**
			 * @fn void clearChromatograms()
			 * @brief clears chromatograms
			 */
			void clearChromatograms();

			/**
			 * @fn void addChromatogram( Chromatogram* chrom )
			 * @brief adds chromatogram
			 * @param chrom chromatogram object to be added
			 */
			void addChromatogram( Chromatogram* chrom );

			/**
			 * @fn void removeChromatogram( Chromatogram* chrom )
			 * @brief removes chromatogram
			 * @param chrom chromatogram object to be removed
			 */
			void removeChromatogram( Chromatogram* chrom );

		protected:
			/**
			 * @fn int searchChromatogram( Chromatogram* chrom )
			 * @brief searches chromatogram
			 * @param chrom chromatogram object to be searched
			 * @return chromatogram index (If negative value, specified chromatogram is not found.)
			 */
			int searchChromatogram( Chromatogram* chrom );
			
		protected:
			/**
			 * @fn virtual void onGetXYData( kome::core::XYData* const xyData )
			 * @brief gets chromatogram data points (override method)
			 * @param xyData object to store data points
			 */
			virtual void onGetXYData( kome::core::XYData* const xyData );

			/**
			 * @fn virtual void onGetSpectra(
					DataSet& dataSet,
					const double startRt,
					const double endRt,
					const SearchType startSearch,
					const SearchType endSearch
				)
			 * @brief This method is called by searchSpectrum or getSpectra method (override method)
			 * @param dataSet data set object to store spectra
			 * @param startRt start RT
			 * @param endRt end RT
			 * @param startSearch start search type
			 * @param endSearch end search type
			 */
			virtual void onGetSpectra(
				DataSet& dataSet,
				const double startRt,
				const double endRt,
				const SearchType startSearch,
				const SearchType endSearch
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

#endif

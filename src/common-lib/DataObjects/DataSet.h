/**
 * @file DataSet.h
 * @brief interfaces of DataSet class
 *
 * @author S.Tanaka
 * @date 2006.10.11
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_OBJECTS_DATA_SET_H__
#define __KOME_OBJECTS_DATA_SET_H__


#include <vector>


namespace kome {
	namespace objects {

		class Sample;
		class Spectrum;
		class Chromatogram;
		class DataGroupNode;

		/**
		 * @class DataSet
		 * @brief one or more spectra management class
		 */
		class DATA_OBJECTS_CLASS DataSet {
		public:
			/**
			 * @fn DataSet()
			 * @brief constructor
			 */
			DataSet();

			/**
			 * @fn DataSet( DataGroupNode* group )
			 * @brief constructor
			 * @param[in] group spectrum group
			 */
			DataSet( DataGroupNode* group );

			/**
			 * @fn virtual ~DataSet()
			 * @brief destructor
			 */
			virtual ~DataSet();

		protected:
			/** sample */
			Sample* m_sample;

			/** group */
			DataGroupNode* m_group;

			/** spectrum array */
			std::vector< Spectrum* > m_spectra;

			/** chromatograms */
			std::vector< Chromatogram* > m_chroms;

		public:
			/**
			 * @fn Sample* getSample()
			 * @brief gets sample
			 * @return sample
			 */
			Sample* getSample();

			/**
			 * @fn DataGroupNode* getGroup()
			 * @brief gets spectrum group
			 * @return spectrum group
			 */
			DataGroupNode* getGroup();

		public:
			/**
			 * @fn unsigned int getNumberOfSpectra()
			 * @brief gets the number of spectra
			 * @return the number of spectra
			 */
			unsigned int getNumberOfSpectra();

			/**
			 * @fn Spectrum* getSpectrum( const unsigned int index )
			 * @brief gets the number of spectra
			 * @return the numbe of spectra
			 */
			Spectrum* getSpectrum( const unsigned int index );

			/**
			 * @fn unsigned int getNumberOfChromatograms()
			 * @brief gets the number of chromatograms
			 * @return the number of chromatograms
			 */
			unsigned int getNumberOfChromatograms();

			/** 
			 * @fn Chromatogram* getChromatogram( const unsigned int index )
			 * @brief gets chroamtogram
			 * @param[in] index chromatogram index
			 * @return chromatogram (If NULL, the index is illegal.)
			 */
			Chromatogram* getChromatogram( const unsigned int index );

		public:
			/**
			 * @fn virtual void sortSpectra()
			 * @brief sorts spectra in retention time order
			 */
			virtual void sortSpectra();

			/**
			 * @fn void clearSpectra()
			 * @brief clears spectra
			 */
			virtual void clearSpectra();

			/**
			 * @fn void addSpectrum( Spectrum* spec )
			 * @brief adds spectrum to group
			 * @param[in] spec spectrum object to be added
			 */
			virtual void addSpectrum( Spectrum* spec );

			/**
			 * @fn virtual void removeSpectrum( Spectrum* spec )
			 * @brief removes spectrum
			 * @param[in] spec spectrum to be removed
			 */
			virtual void removeSpectrum( Spectrum* spec );

			/**
			 * @fn virtual void insertSpectrum( Spectrum* spec, const int idx )
			 * @brief inserts spectrum
			 * @param[in] spec spectrum to be inserted.
			 * @param[in] idx insert position
			 */
			virtual void insertSpectrum( Spectrum* spec, const int idx );

			/**
			 * @fn virtual void clearChromatograms()
			 * @brief clears chromatograms
			 */
			virtual void clearChromatograms();

			/**
			 * @fn virtual void addChromatogram( Chromatogram* chrom )
			 * @brief adds chromatogram
			 * @param[in] chrom chromatogram
			 */
			virtual void addChromatogram( Chromatogram* chrom );

			/**
			 * @fn virtual void removeChromatogram( Chromatogram* chrom )
			 * @brief removes chromatogram
			 * @param[in] chrom chromatogram to be removed
			 */
			virtual void removeChromatogram( Chromatogram* chrom );

			/**
			 * @fn virtual void insertChromatogram( Chromatogram* chrom, const int idx )
			 * @brief inserts chromatogram 
			 * @param[in] chrom chromatogram to be inserted
			 * @param[in] idx insert position
			 */
			virtual void insertChromatogram( Chromatogram* chrom, const int idx );

		protected:
			/**
			 * @fn static bool lessSpectrum( Spectrum* spec0, Spectrum* spec1 )
			 * @brief compare to sort spectra
			 * @param[in] spec0 spectrum object to be compared
			 * @param[in] spec1 spectrum object to compare
			 * @return If spec0 sorts before spec1, this method returns true.
			 */
			static bool lessSpectrum( Spectrum* spec0, Spectrum* spec1 );
		};
	}
}

#endif	// __KOME_OBJECTS_DATA_SET_H__

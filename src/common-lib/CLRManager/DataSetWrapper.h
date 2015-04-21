/**
 * @file DataSetWrapper.h
 * @brief interfaces of DataSetWrapper class
 *
 * @author S.Tanaka
 * @date 2006.10.11
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_CLR_DATA_SET_WRAPPER_H__
#define __KOME_CLR_DATA_SET_WRAPPER_H__


namespace kome {
	namespace clr {

		ref class SampleWrapper;
		ref class DataGroupNodeWrapper;
		ref class SpectrumWrapper;
		ref class ChromatogramWrapper;
		ref class XYDataWrapper;

		/**
		 * @class DataSetWrapper
		 * @brief DataSet object wrapper class to use on CLR environment
		 */
		public ref class DataSetWrapper {
		public:
			/**
			 * @fn DataSetWrapper( DataGroupNodeWrapper^ group )
			 * @brief constructor
			 * @param[in] group spectrum group
			 */
			DataSetWrapper( DataGroupNodeWrapper^ group );

			/**
			 * @fn DataSetWrapper( kome::objects::DataSet& dataSet )
			 * @brief constructor
			 * @param[in] dataSet base object
			 */
			DataSetWrapper( kome::objects::DataSet& dataSet );

			/**
			 * @fn virtual ~DataSetWrapper()
			 * @brief destructor
			 */
			virtual ~DataSetWrapper();

		protected:
			/** base object */
			kome::objects::DataSet& m_dataSet;

			/** destroy flag */
			bool m_destroyFlg;

		public:
			/**
			 * @fn kome::objects::DataSet& getDataSet()
			 * @brief gets DataSet object
			 * @return DataSet object
			 */
			kome::objects::DataSet& getDataSet();

		public:
			/**
			 * @fn SampleWrapper^ getSample()
			 * @brief gets sample
			 * @return sample
			 */
			SampleWrapper^ getSample();

			/**
			 * @fn DataGroupNodeWrapper^ getGroup()
			 * @brief gets spectrum group
			 * @return spectrum group
			 */
			DataGroupNodeWrapper^ getGroup();

		public:
			/**
			 * @fn unsigned int getNumberOfSpectra()
			 * @brief gets the number of spectra
			 * @return the number of spectra
			 */
			unsigned int getNumberOfSpectra();

			/**
			 * @fn SpectrumWrapper^ getSpectrum( unsigned int index )
			 * @brief gets the number of spectra
			 * @return the numbe of spectra
			 */
			SpectrumWrapper^ getSpectrum( unsigned int index );

			/**
			 * @fn unsigned int getNumberOfChromatograms()
			 * @brief gets the number of chromatograms
			 * @return the number of chromatograms
			 */
			unsigned int getNumberOfChromatograms();

			/** 
			 * @fn ChromatogramWrapper^ getChromatogram( unsigned int index )
			 * @brief gets chroamtogram
			 * @param[in] index chromatogram index
			 * @return chromatogram (If NULL, the index is illegal.)
			 */
			ChromatogramWrapper^ getChromatogram( unsigned int index );

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
			 * @fn void addSpectrum( SpectrumWrapper^ spec )
			 * @brief adds spectrum to group
			 * @param[in] spec spectrum object to be added
			 */
			virtual void addSpectrum( SpectrumWrapper^ spec );


			/**
			 * @fn virtual void removeSpectrum( SpectrumWrapper^ spec )
			 * @brief removes spectrum
			 * @param[in] spec spectrum to be removed
			 */
			virtual void removeSpectrum( SpectrumWrapper^ spec );

			/**
			 * @fn virtual void insertSpectrum( SpectrumWrapper^ spec, int idx )
			 * @brief inserts spectrum
			 * @param[in] spec spectrum to be inserted.
			 * @param[in] idx insert position
			 */
			virtual void insertSpectrum( SpectrumWrapper^ spec, int idx );

			/**
			 * @fn virtual void clearChromatograms()
			 * @brief clears chromatograms
			 */
			virtual void clearChromatograms();

			/**
			 * @fn void addChromatogram( ChromatogramWrapper^ chrom )
			 * @brief adds chromatogram
			 * @param[in] chrom chromatogram
			 */
			void addChromatogram( ChromatogramWrapper^ chrom );

			/**
			 * @fn virtual void removeChromatogram( ChromatogramWrapper^ chrom )
			 * @brief removes chromatogram
			 * @param[in] chrom chromatogram to be removed
			 */
			virtual void removeChromatogram( ChromatogramWrapper^ chrom );

			/**
			 * @fn virtual void insertChromatogram( ChromatogramWrapper^ chrom, int idx )
			 * @brief inserts chromatogram 
			 * @param[in] chrom chromatogram to be inserted
			 * @param[in] idx insert position
			 */
			virtual void insertChromatogram( ChromatogramWrapper^ chrom, int idx );
		};
	}
}

#endif	// __KOME_CLR_DATA_SET_WRAPPER_H__

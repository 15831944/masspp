/**
 * @file ChromatogramWrapper.h
 * @brief interfaces of ChromatogramWrapper class
 *
 * @author S.Tanaka
 * @date 2006.10.03
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_CLR_CHROMATOGRAM_WRAPPER_H__
#define __KOME_CLR_CHROMATOGRAM_WRAPPER_H__


#include "CommonType.h"


namespace kome {
	namespace clr {

		ref class SampleWrapper;
		ref class XYDataWrapper;
		ref class DataGroupNodeWrapper;
		ref class PropertiesWrapper;
		ref class SpectrumWrapper;
		ref class DataSetWrapper;

		/**
		 * @class ChromatogramWrapper
		 * @brief Chromatogram object wrapper class to use on CLR environment
		 */
		public ref class ChromatogramWrapper {
		public:
			/**
			 * @fn ChromatogramWrapper( kome::objects::Chromatogram& chromatogram )
			 * @brief constructor
			 * @param[in] chromatogram chromatogram object
			 */
			ChromatogramWrapper( kome::objects::Chromatogram& chromatogram );

			/**
			 * @fn virtual ~ChromatogramWrapper()
			 * @brief destructor
			 */
			virtual ~ChromatogramWrapper();

		protected:
			/** Chromatogram object */
			kome::objects::Chromatogram& m_chromatogram;

		public:
			/**
			 * @fn kome::objects::Chromatogram& getChromatogram()
			 * @brief gets chromatogram object
			 * @return chromatogram object
			 */
			kome::objects::Chromatogram& getChromatogram();

		public:
			/**
			 * @fn SampleWrapper^ getSample()
			 * @brief gets sample
			 * @return sample
			 */
			SampleWrapper^ getSample();

			/**
			 * @fn void setName( System::String^ name )
			 * @brief sets chromatogram name
			 * @param[in] name chromatogram name
			 */
			void setName( System::String^ name );

			/**
			 * @fn System::String^ getName()
			 * @brief gets chromatogram name
			 * @return chromatogram name
			 */
			System::String^ getName();

			/**
			 * @fn void setMz( double mz )
			 * @brief sets m/z
			 * @param[in] mz m/z
			 */
			void setMz( double mz );

			/**
			 * @fn double getMz()
			 * @brief gets m/z
			 * @return m/z
			 */
			double getMz();

			/**
			 * @fn void setGroup( DataGroupNodeWrapper^ group )
			 * @brief sets spectrum group
			 * @param[in] group spectrum group
			 */
			void setGroup( DataGroupNodeWrapper^ group );

			/**
			 * @fn DataGroupNodeWrapper^ getGroup()
			 * @brief gets spectrum group
			 * @return spectrum group
			 */
			DataGroupNodeWrapper^ getGroup();

			/**
			 * @fn void setIcon( System::String^ icon )
			 * @brief sets icon name
			 * @param[in] icon
			 */
			void setIcon( System::String^ icon );

			/**
			 * @fn System::String^ getIcon()
			 * @brief gets icon name
			 * @return icon name
			 */
			System::String^ getIcon();

			/**
			 * @fn void setTitle( System::String^ title )
			 * @brief sets title
			 * @param[in] title title
			 */
			void setTitle( System::String^ title );

			/**
			 * @fn System::String^ getTitle()
			 * @brief gets title
			 * @return title
			 */
			System::String^ getTitle();

			/**
			 * @fn PropertiesWrapper^ getProperties()
			 * @brief gets properties
			 * @return properties
			 */
			PropertiesWrapper^ getProperties();

			/**
			 * @fn void setAutoCreated( bool autoCreated )
			 * @brief sets auto created flag
			 * @param[in] autoCreated auto created flag value
			 */
			void setAutoCreated( bool autoCreated );

			/**
			 * @fn bool isAutoCreated()
			 * @brief gets auto created flag value
			 * @return auto created flag value
			 */
			bool isAutoCreated();

			/**
			 * @fn void setVisible( bool visible )
			 * @brief sets the visible flag
			 * @param[in] visible visible flag value
			 */
			void setVisible( bool visible );

			/**
			 * @fn bool isVisible()
			 * @brief gets the visible flag value
			 * @return visible flag value
			 */
			 bool isVisible();

			/*
			 * @fn void setOrgChromatogram( ChromatogramWrapper^ chrom )
			 * @brief sets original chromatogram
			 * @param[in] chrom original chromatogram
			 */
			void setOrgChromatogram( ChromatogramWrapper^ chrom );

			/**
			 * @fn ChromatogramWrapper^ getOrgChromatogram()
			 * @brief gets original chromatogram
			 * @return original chromatogram
			 */
			ChromatogramWrapper^ getOrgChromatogram();

		public:
			/**
			 * @fn void setQ1( double q1 )
			 * @brief sets the Q1 Mass
			 * @param[in] q1 Q1 Mass
			 */
			void setQ1( double q1 );

			/**
			 * @fn double getQ1()
			 * @brief gets the Q1 Mass
			 * @return Q1 Mass
			 */
			double getQ1();

			/**
			 * @fn void setQ3( double q3 )
			 * @brief sets the Q3 Mass
			 * @param[in] q3 Q3 Mass
			 */
			void setQ3( double q3 );

			/**
			 * @fn double getQ3()
			 * @brief gets the Q3 Mass
			 * @return Q3 Mass
			 */
			double getQ3();

		public:
			/**
			 * @fn XYDataWrapper^ getXYData()
			 * @brief gets xy data from data manager
			 * @return xy data object
			 */
			XYDataWrapper^ getXYData();

			/**
			 * @fn void deleteXYData()
			 * @brief deletes xy data of data manager
			 */
			void deleteXYData();

			/**
			 * @fn virtual void getXYData( XYDataWrapper^ xyData, bool op )
			 * @brief gets chromatogram data points
			 * @param[out] xyData object to store data points
			 * @param[in] op If true, getting operated data points
			 */
			virtual void getXYData( XYDataWrapper^ xyData, bool op );  

		public:
			/**
			 * @fn SpectrumWrapper^ searchSpectrum( double rt, SearchType search )
			 * @brief searches spectrum
			 * @param[in] rt RT
			 * @param[in] search search type
			 * @return searched spectrum (If specified spectrum is not found, this method returns NULL.)
			 */
			SpectrumWrapper^ searchSpectrum( double rt, SearchType search );

			/**
			 * @fn void getSpectra(
						DataSetWrapper^ dataSet,
						double startRt,
						double endRt,
						SearchType startSearch,
						SearchType endSearch
				)
			 * @brief gets spectra in specified RT range
			 * @param[out] dataSet data set object to store spectra
			 * @param[in] startRt start RT
			 * @param[in] endRt end RT
			 * @param[in] startSearch start search type
			 * @param[in] endSearch end search type
			 */
			void getSpectra(
					DataSetWrapper^ dataSet,
					double startRt,
					double endRt,
					SearchType startSearch,
					SearchType endSearch
			);

			/**
			 * @fn double getMass( unsigned int index )
			 * @brief gets mass
			 * @param[in] index data index
			 * @return mass
			 */
			double getMass( unsigned int index );

			/**
			 * @fn int getMsStage( unsigned int index )
			 * @brief gets MS stage
			 * @param[in] index data index
			 * @return MS stage.
			 */
			int getMsStage( unsigned int index );

			/**
			 * @fn double getPrecursor( unsigned int index )
			 * @brief gets precursor mass
			 * @param[in] index data index
			 * @return precursor mass
			 */
			double getPrecursor( unsigned int index );

			/**
			 * @fn void getProperties( PropertiesWrapper^ properties )
			 * @brief gets spectrum and spectrum group properties
			 * @param[out] properties properties object to store properties
			 */
			void getProperties( PropertiesWrapper^ properties );

		public:
			/**
			 * @fn static bool isCommonProperty( System::String^ key )
			 * @brief check whther the specified property key is common property or not
			 * @param[in] key parameter key
			 * @return If true, specified key is common property key
			 */
			static bool isCommonProperty( System::String^ key );
		};
	}
}

#endif	// __KOME_CLR_CHROMATOGRAM_WRAPPER_H__

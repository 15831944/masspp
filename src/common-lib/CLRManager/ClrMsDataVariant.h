/**
 * @file ClrMsDataVariant.h
 * @brief interfaces of ClrMsDataVariant class
 *
 * @author S.Tanaka
 * @date 2008.12.25
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_CLR_MS_DATA_VARIANT_H__
#define __KOME_CLR_MS_DATA_VARIANT_H__


namespace kome {
	namespace clr {

		ref class ClrVariant;
		ref class SpectrumWrapper;
		ref class ChromatogramWrapper;
		ref class DataMapInfoWrapper;
		ref class DataGroupNodeWrapper;
		ref class SampleWrapper;
		ref class SampleSetWrapper;

		/**
		 * @class ClrMsDataVariant
		 * @brief this class treats value of various MS data types on CLR environment
		 */
		public ref class ClrMsDataVariant {
		public:
			/**
			 * @fn ClrMsDataVariant()
			 * @brief constructor
			 */
			ClrMsDataVariant();

			/**
			 * @fn ClrMsDataVariant( ClrVariant^ var )
			 * @brief constructor
			 * @param var variant object
			 */
			ClrMsDataVariant( ClrVariant^ var );

			/**
			 * @fn virtual ~ClrMsDataVariant()
			 * @brief destructor
			 */
			virtual ~ClrMsDataVariant();

		protected:
			/** variant object */
			ClrVariant^ m_var;

		public:
			/**
			 * @fn void setVariant( ClrVariant^ var )
			 * @brief sets variant object
			 * @param var variant object
			 */
			void setVariant( ClrVariant^ var );

			/**
			 * @fn ClrVariant^ getVariant()
			 * @brief gets variant object
			 * @return variant object
			 */
			ClrVariant^ getVariant();

		public:
			/**
			 * @fn void unsetObject()
			 * @brief unsets object
			 */
			void unsetObject();

			/**
			 * @fn void setSpectrum( SpectrumWrapper^ spec )
			 * @brief sets spectrum
			 * @param spec spectrum
			 */
			void setSpectrum( SpectrumWrapper^ spec );

			/**
			 * @fn void setChromatogram( ChromatogramWrapper^ chrom )
			 * @brief sets chromatogram
			 * @param chrom chromatogram
			 */
			void setChromatogram( ChromatogramWrapper^ chrom );

			/**
			 * @fn void setDataMap( DataMapInfoWrapper^ dataMap )
			 * @brief sets data map
			 * @param dataMap data map
			 */
			void setDataMap( DataMapInfoWrapper^ dataMap );

			/**
			 * @fn void setGroup( DataGroupNodeWrapper^ group )
			 * @brief sets group
			 * @param group group
			 */
			void setGroup( DataGroupNodeWrapper^ group );

			/**
			 * @fn void setSample( SampleWrapper^ sample )
			 * @brief  sets sample
			 * @param sample sample
			 */
			void setSample( SampleWrapper^ sample );

			/**
			 * @fn void setSampleSet( SampleSetWrapper^ sampleSet )
			 * @brief sets sample set
			 * @param sampleSet sample set
			 */
			void setSampleSet( SampleSetWrapper^ sampleSet );

			/**
			 * @fn SpectrumWrapper^ getSpectrum()
			 * @brief gets spectrum
			 * @return spectrum
			 */
			 SpectrumWrapper^ getSpectrum();

			 /**
			  * @fn ChromatogramWrapper^ getChromatogram()
			  * @brief gets chromatogram 
			  * @return chromatogram
			  */
			ChromatogramWrapper^ getChromatogram();

			/**
			 * @fn DataMapInfoWrapper^ getDataMap()
			 * @brief gets data map
			 * @return data map
			 */
			DataMapInfoWrapper^ getDataMap();

			/**
			 * @fn DataGroupNodeWrapper^ getGroup()
			 * @brief gets group
			 * @return group
			 */
			DataGroupNodeWrapper^ getGroup();

			/**
			 * @fn SampleWrapper^ getSample()
			 * @brief gets sample
			 * @return sample
			 */
			SampleWrapper^ getSample();

			/**
			 * @fn SampleSetWrapper^ getSampleSet()
			 * @brief gets sample set
			 * @return sample set
			 */
			SampleSetWrapper^ getSampleSet();
		};
	}
}

#endif	// __KOME_CLR_MS_DATA_VARIANT_H__

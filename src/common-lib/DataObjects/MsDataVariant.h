/**
 * @file MsDataVariant.h
 * @brief interfaces of MsDataVariant class
 *
 * @author S.Tanaka
 * @date 2008.12.24
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_OBJECTS_MS_DATA_VARIANT_H__
#define __KOME_OBJECTS_MS_DATA_VARIANT_H__


#include "Variant.h"


namespace kome {
	namespace objects {

		class Spectrum;
		class Chromatogram;
		class DataMapInfo;
		class DataGroupNode;
		class Sample;
		class SampleSet;

		/**
		 * @class MsDataVariant
		 * @brief class that treats MS data of various types
		 */
		class DATA_OBJECTS_CLASS MsDataVariant {
		public:
			/**
			 * @fn MsDataVariant()
			 * @brief constructor
			 */
			MsDataVariant();

			/**
			 * @fn MsDataVariant( Variant& var )
			 * @brief constructor
			 * @param[in] var variant object
			 */
			MsDataVariant( Variant& var );

			/**
			 * @fn virtual ~MsDataVariant()
			 * @brief destructor
			 */
			virtual ~MsDataVariant();

		protected:
			/** variant */
			Variant m_var;

		public:
			/**
			 * @fn void setVariant( Variant& var )
			 * @brief sets variant object
			 * @param[in] var variant object
			 */
			void setVariant( Variant& var );

			/**
			 * @fn Variant& getVariant()
			 * @brief gets variant object
			 * @return variant object
			 */
			Variant& getVariant();

		public:
			/**
			 * @fn void unsetObject()
			 * @brief unsets object
			 */
			void unsetObject();

			/**
			 * @fn void setSpectrum( Spectrum* spec )
			 * @brief sets spectrum
			 * @param[in] spec spectrum
			 */
			void setSpectrum( Spectrum* spec );

			/**
			 * @fn void setChromatogram( Chromatogram* chrom )
			 * @brief sets chromatogram
			 * @param[in] chrom chromatogram
			 */
			void setChromatogram( Chromatogram* chrom );

			/**
			 * @fn void setDataMap( DataMapInfo* dataMap )
			 * @brief sets data map
			 * @param[in] dataMap data map
			 */
			void setDataMap( DataMapInfo* dataMap );

			/**
			 * @fn void setGroup( DataGroupNode* group )
			 * @brief sets group
			 * @param[in] group group
			 */
			void setGroup( DataGroupNode* group );

			/**
			 * @fn void setSample( Sample* sample )
			 * @brief  sets sample
			 * @param[in] sample sample
			 */
			void setSample( Sample* sample );

			/**
			 * @fn void setSampleSet( SampleSet* sampleSet )
			 * @brief sets sample set
			 * @param[in] sampleSet sample set
			 */
			void setSampleSet( SampleSet* sampleSet );

			/**
			 * @fn Spectrum* getSpectrum()
			 * @brief gets spectrum
			 * @return spectrum
			 */
			 Spectrum* getSpectrum();

			 /**
			  * @fn Chromatogram* getChromatogram()
			  * @brief gets chromatogram 
			  * @return chromatogram
			  */
			Chromatogram* getChromatogram();

			/**
			 * @fn DataMapInfo* getDataMap()
			 * @brief gets data map
			 * @return data map
			 */
			DataMapInfo* getDataMap();

			/**
			 * @fn DataGroupNode* getGroup()
			 * @brief gets group
			 * @return group
			 */
			DataGroupNode* getGroup();

			/**
			 * @fn Sample* getSample()
			 * @brief gets sample
			 * @return sample
			 */
			Sample* getSample();

			/**
			 * @fn SampleSet* getSampleSet()
			 * @brief gets sample set
			 * @return sample set
			 */
			SampleSet* getSampleSet();
		};
	}
}


#endif // __KOME_OBJECTS_MS_DATA_VARIANT_H__

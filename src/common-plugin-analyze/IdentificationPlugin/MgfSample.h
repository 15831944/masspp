/**
 * @file MgfSample.h
 * @brief interfaces of MgfSample class
 *
 * @author M.Izumi
 * @date 2013.07.18
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#ifndef __KOME_IDENT_MGF_SAMPLE_H__
#define __KOME_IDENT_MGF_SAMPLE_H__

namespace kome {
	namespace ident {
	
		class MgfSampleSet;

		class MgfSample : public kome::objects::Sample {
		public:
			/**
			 * @fn MgfSample( MgfSampleSet* sampleSet )
			 * @brief constructor
			 * @param sampleSet sample set object
			 */
			MgfSample( MgfSampleSet* sampleSet );

			/**
			 * @fn virtual ~MgfSample()
			 * @brief destructor
			 */
			virtual ~MgfSample();

		protected:
			/** sample set */
			MgfSampleSet* m_sampleSet;
		public:
			MgfSampleSet* getSampleSet(){ return m_sampleSet; }

		protected:
			bool writePoint( const char* path, std::vector< std::pair< double, double > > points );

		protected:
			/**
			 * @fn virtual bool onOpenSample( DataGroupNode* rootGroup )
			 * @brief This method is called by openTreatment method. (override method)
			 * @param rootGroup root spectrum group
			 * @return If true, it succeeded to open this sample.
			 */
			virtual bool onOpenSample( kome::objects::DataGroupNode* rootGroup, kome::core::Progress* progress = NULL );

			/**
			 * @fn virtual bool onCloseSample()
			 * @brief This method is called by closeTreatment method. (override method)
			 * @return If true, it succeeded to close this sample.
			 */
			virtual bool onCloseSample();
		
		};
	}
}

#endif	// __KOME_IDENT_MGF_SAMPLE_H__

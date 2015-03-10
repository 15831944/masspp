/**
 * @file TextSample.h
 * @brief interfaces of TextSample class
 *
 * @author S.Tanaka
 * @date 2010.01.08
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_IO_TEXT_SAMPLE_H__
#define __KOME_IO_TEXT_SAMPLE_H__


namespace kome {
	namespace io {
		namespace text {

			class TextSampleSet;

			/**
			 * @class TextSample
			 * @brief text sample file management class
			 */
			class TextSample : public kome::objects::Sample {
			public:
				/**
				 * @fn TextSample( TextSampleSet* sampleSet )
				 * @brief constructor
				 * @param sampleSet sample set object
				 */
				TextSample( TextSampleSet* sampleSet );

				/**
				 * @fn virtual ~TextSample()
				 * @brief destructor
				 */
				virtual ~TextSample();			

			protected:
				/** msb sample set */
				TextSampleSet* m_sampleSet;

			public:
				/**
				 * @fn TextSampleSet* getTextSampleSet()
				 * @brief gets text sample set object
				 * @return text sample set object
				 */
				TextSampleSet* getTextSampleSet();

			protected:
				/**
				 * @fn virtual bool onOpenSample( DataGroupNode* rootGroup )
				 * @brief This method is called by openTreatment method. (override method)
				 * @param rootGroup root spectrum group
				 * @return If true, it succeeded to open this treatment.
				 */
				virtual bool onOpenSample( kome::objects::DataGroupNode* rootGroup, kome::core::Progress* progress = NULL );

				/**
				 * @fn virtual bool onCloseSample()
				 * @brief This method is called by closeTreatment method. (override method)
				 * @return If true, it succeeded to close this treatment.
				 */
				virtual bool onCloseSample();
			};
		}
	}
}

#endif		//  __KOME_IO_TEXT_SAMPLE_H__

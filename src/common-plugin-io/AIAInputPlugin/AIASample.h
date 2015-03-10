/**
 * @file AIASample.h
 * @brief interface of AIASample class
 *
 * @author Y.Fujita
 * @date 2012.07.20
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation All rights reserved.
 */


#ifndef __KOME_IO_AIA_SAMPLE_H__
#define __KOME_IO_AIA_SAMPLE_H__


namespace kome {
	namespace io {
		namespace aia {

			class AIASampleSet;

			/**
			 * @class AIASample
			 * @brief AIA file class
			 */
			class AIASample : public kome::objects::Sample {
			public:
				/**
				 * @fn AIASample( AIASampleSet* sampleSet )
				 * @brief constructor
				 * @param sampleSet GCMS sample set object
				 */
				AIASample( AIASampleSet* sampleSet );

				/**
				 * @fn virtual ~AIASample()
				 * @brief destructor
				 */
				virtual ~AIASample();

			protected:
				/** sample set */
				AIASampleSet* m_sampleSet;

				/** NetCDF ID */
				int m_ncid;

			public:
				/**
				 * @fn AIASampleSet* getAIASampleSet()
				 * @brief gets AIA sample set object
				 * @return AIA sample set object
				 */
				AIASampleSet* getAIASampleSet();

				/**
				 * @fn int getCdfId()
				 * @brief gets NetCDF ID
				 * @return NetCDF ID
				 */
				int getCdfId();

			protected:
				/**
				 * @fn void setProperties()
				 * @brief sets properties
				 */
				void setProperties();

				/**
				 * @fn void setInstrumentProps()
				 * @brief sets instrument properties
				 */
				void setInstrumentProps();

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
}


#endif		// __KOME_IO_AIA_SAMPLE_H__

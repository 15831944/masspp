/**
 * @file MsbSample.h
 * @brief interfaces of MsbSample class
 *
 * @author S.Tanaka
 * @date 2008.01.22
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_IO_MSB_SAMPLE_H__
#define __KOME_IO_MSB_SAMPLE_H__

#include <set>


namespace kome {
	namespace io {
		namespace msb {

			class MsbSampleSet;

			/**
			 * @class MsbSample
			 * @brief msb sample file management class
			 */
			class MsbSample : public kome::objects::Sample {
			public:
				/**
				 * @fn MsbSample( MsbSampleSet* sampleSet )
				 * @brief constructor
				 * @param sampleSet sample set object
				 */
				MsbSample( MsbSampleSet* sampleSet );

				/**
				 * @fn virtual ~MsbSample()
				 * @brief destructor
				 */
				virtual ~MsbSample();			

			protected:
				/** msb sample set */
				MsbSampleSet* m_sampleSet;

				/** MSB major version */
				unsigned short m_majorVer;

				/** MSB minor version */
				unsigned short m_minorVer;

			protected:
				/** common properties flag */
				static bool m_commonPropsFlg;

			public:
				/**
				 * @fn MsbSampleSet* getMsbSampleSet()
				 * @brief gets MSB sample set object
				 * @return MSB sample set object
				 */
				MsbSampleSet* getMsbSampleSet();

				/**
				 * @fn void setMsbMajorVersion( const unsigned short ver )
				 * @brief sets MSB major version
				 * @param ver MSB major version
				 */
				void setMsbMajorVersion( const unsigned short ver );

				/**
				 * @fn unsigned short getMsbMajorVersion()
				 * @brief gets MSB major version
				 * @return MSB major version
				 */
				unsigned short getMsbMajorVersion();

				/**
				 * @fn void setMsbMinorVersion( const unsigned short ver )
				 * @brief sets MSB minor version
				 * @param ver MSB minor version
				 */
				void setMsbMinorVersion( const unsigned short ver );

				/**
				 * @fn unsigned short getMsbMinorVersion()
				 * @brief gets MSB minor version
				 * @return MSB minor version
				 */
				unsigned short getMsbMinorVersion();

			protected:
				/**
				 * @fn virtual bool onOpenSample( DataGroupNode* rootGroup )
				 * @brief This method is called by openTreatment method. (override method)
				 * @param rootGroup root spectrum group
				 * @return If true, it succeeded to open this treatment.
				 */
				virtual bool onOpenSample( kome::objects::DataGroupNode* rootGroup, kome::core::Progress* progres );

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

#endif		//  __KOME_IO_MSB_SAMPLE_H__

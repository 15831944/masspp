/**
 * @file AbQstarSample.h
 * @brief interfaces of AbQstarSample class
 *
 * @author S.Tanaka
 * @date 2008.01.11
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_IO_AB_QSTAR_SAMPLE_H__
#define __KOME_IO_AB_QSTAR_SAMPLE_H__


namespace kome {
	namespace io {
		namespace qstar {

			ref class AbQstarSampleSet;

			/**
			 * @class AbQstarSample
			 * @brief sample of Applied Biosystems QSTAR data
			 */
			public ref class AbQstarSample : public kome::io::ab::AbSample {
			public:
				/**
				 * @fn AbQstarSample( kome::io::ab::AbSampleSet^ sampleSet )
				 * @brief constructor
				 * @param sampleSet sample set object
				 */
				AbQstarSample( kome::io::ab::AbSampleSet^ sampleSet );

				/**
				 * @fn virtual ~AbQstarSample()
				 * @brief destructor
				 */
				virtual ~AbQstarSample();

			public:
				/**
				 * @fn virtual bool onOpenSample( kome::clr::DataGroupNodeWrapper^ rootGroup )
				 * @brief This method is called by openSample method. (override method)
				 * @param rootGroup root spectrum group
				 * @return If true, it succeeded to open this sample.
				 */
				virtual bool onOpenSample( kome::clr::DataGroupNodeWrapper^ rootGroup ) override;
			};
		}
	}
}

#endif		// __KOME_IO_AB_QSTAR_SAMPLE_H__

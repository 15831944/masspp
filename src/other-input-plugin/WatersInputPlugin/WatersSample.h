/**
 * @file WatersSample.h
 * @brief interfaces of WatersSample class
 *
 * @author S.Tanaka
 * @date 2008.01.21
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_IO_WATERS_SAMPLE_H__
#define __KOME_IO_WATERS_SAMPLE_H__


namespace kome {
	namespace io {
		namespace waters {

			ref class WatersSampleSet;

			/**
			 * @class WatersSample
			 * @brief sample of Waters data
			 */
			public ref class WatersSample : public kome::clr::ClrSampleBase {
			public:
				/**
				 * @fn WatersSample( WatersSampleSet^ sampleSet, int function )
				 * @brief constructor
				 * @param sampleSet sample set object
				 * @param function function number	
				 */
				WatersSample( WatersSampleSet^ sampleSet, int function );

				/**
				 * @fn virtual ~WatersSample()
				 * @brief destructor
				 */
				virtual ~WatersSample();

			protected:
				/** sample set */
				WatersSampleSet^ m_sampleSet;

				/** function number */
				int m_func;

			public:
				/**
				 * @fn WatersSampleSet^ getWatersSampleSet()
				 * @brief gets sample set object
				 * @return sample set object
				 */
				WatersSampleSet^ getWatersSampleSet();

				/**
				 * @fn int getFunction()
				 * @brief gets function number
				 * @return function number
				 */
				int getFunction();

			protected:
				/**
				 * @fn void setFileProperties( kome::clr::PropertiesWrapper^ props )
				 * @brief sets file properties
				 * @param props properties object to store file properties
				 */
				void setFileProperties( kome::clr::PropertiesWrapper^ props );

				/**
				 * @fn void setFuncProperties( kome::clr::PropertiesWrapper^ props )
				 * @brief sets function properties
				 * @param props properties object to store function properties
				 */
				void setFuncProperties( kome::clr::PropertiesWrapper^ props );

			public:
				/**
				 * @fn virtual bool onOpenSample( kome::clr::DataGroupNodeWrapper^ rootGroup )
				 * @brief This method is called by openSample method. (override method)
				 * @param rootGroup root spectrum group
				 * @return If true, it succeeded to open this sample.
				 */
				virtual bool onOpenSample( kome::clr::DataGroupNodeWrapper^ rootGroup ) override;

				/**
				 * @fn virtual bool onCloseSample()
				 * @brief This method is called by closeSample method. (override method)
				 * @return If true, it succeeded to close this sample.
				 */
				virtual bool onCloseSample() override;
			};
		}
	}
}

#endif		// __KOME_IO_WATERS_SAMPLE_H__

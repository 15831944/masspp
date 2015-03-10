/**
 * @file WatersSampleSet.h
 * @brief interfaces of WatersSampleSet class
 *
 * @author S.Tanaka
 * @date 2008.01.21
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_IO_WATERS_SAMPLE_SET_H__
#define __KOME_IO_WATERS_SAMPLE_SET_H__


namespace kome {
	namespace io {
		namespace waters {

			/**
			 * @class WatersSampleSet
			 * @brief sample set of Waters data
			 */
			public ref class WatersSampleSet : public kome::clr::ClrSampleSetBase {
			public:
				/**
				 * @fn WatersSampleSet()
				 * @brief constructor
				 */
				WatersSampleSet();

				/**
				 * @fn virtual ~WatersSampleSet()
				 * @brief destructor
				 */
				virtual ~WatersSampleSet();

			protected:
				/** path */
				System::String^ m_path;

			public:
				/**
				 * @fn System::String^ getPath()
				 * @brief gets path
				 * @return path
				 */
				System::String^ getPath();

			  public:
				/**
				 * @fn virtual bool onOpenFile( System::String^ path ) override
				 * @brief This method is called by openFile method. (override method)
				 * @param path file path
				 * @return If true, it succeeded to open the file
				 */
				virtual bool onOpenFile( System::String^ path ) override;

				/**
				 * @fn virtual bool onCloseFile() override
				 * @brief This method is called by closeFile method. (override method)
				 * @param path file path
				 * @return If true, it succeeded to close the file
				 */
				virtual bool onCloseFile() override;

				/**
				 * @fn virtual bool onOpenSample( kome::clr::SampleWrapper^ sample ) override
				 * @brief This method is called by openSample method. (override method)
				 * @param sample sample object to be opened (If NULL, opens all samples.)
				 * @return If true, it succeeded to open the sample.
				 */
				virtual bool onOpenSample( kome::clr::SampleWrapper^ sample ) override;

				/**
				 * @fn virtual bool onCloseSample( kome::clr::SampleWrapper^ sample ) override
				 * @brief This method is called by closeSample method. (override method)
				 * @param sample sample object to be closed (If NULL, closes all samples.)
				 * @return If true, it succeeded to close the sample.
				 */
				virtual bool onCloseSample( kome::clr::SampleWrapper^ sample ) override;
			};
		}
	}
}

#endif		// __KOME_IO_WATERS_SAMPLE_SET_H__

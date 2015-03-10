/**
 * @file WatersInputManager.h
 * @brief interfaces of WatersInputManager class
 *
 * @author S.Tanaka
 * @date 2006.10.17
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_IO_WATERS_INPUT_MANAGER_H__
#define __KOME_IO_WATERS_INPUT_MANAGER_H__


namespace kome {
	namespace io {
		namespace waters {

			ref class WatersSampleSet;

			/**
			 * @class WatersInputManager
			 * @brief waters sample file input management class
			 */
			public ref class WatersInputManager {
			protected:
				/**
				 * @fn WatersInputManager()
				 * @brief constructor
				 */
				WatersInputManager();

				/**
				 * @fn virtual ~WatersInputManager()
				 * @brief destructor
				 */
				virtual ~WatersInputManager();

			protected:
				/** micromass file list */
				System::Collections::ArrayList^ m_list;

			public:
				/**
				 * @fn void addSampleSet( WatersSampleSet^ sampleSet )
				 * @brief adds sample set
				 * @param sampleSet sample set object to be added
				 */
				void addSampleSet( WatersSampleSet^ sample );

				/**
				 * @fn void removeSampleSet( WatersSampleSet^ sampleSet )
				 * @brief removes sample set
				 * @param sampleSet sample set object to be deleted
				 */
				void removeSampleSet( WatersSampleSet^ sampleSet );

			public:
				/**
				 * @fn static kome::clr::ClrVariant^ open( kome::clr::ClrParameters^ params )
				 * @brief open micromass file
				 * @param params information object. (This object has path information.)
				 * @return sample object.
				 */
				static kome::clr::ClrVariant^ open( kome::clr::ClrParameters^ params );

				/**
				 * @fn static kome::clr::ClrVariant^ isMicromassFolder( kome::clr::ClrParameters^ params )
				 * @brief check the folder (whether the foloder is micromass folder)
				 * @param params parameters information (This object ha path information.)
				 * @return If specified path is micromass folder, this method returns true.
				 */
				static kome::clr::ClrVariant^ isMicromassFolder( kome::clr::ClrParameters^ params );

			protected:
				/** waters input manager object (This is the only object.) */
				static WatersInputManager^ m_instance;

			public:
				/**
				 * @fn static WatersInputManager^ getInstance()
				 * @brief gets waters input manager object
				 * @return waters input manager object (This is the only object.)
				 */
				static WatersInputManager^ getInstance();
			};
		}
	}
}

#endif	// __KOME_IO_WATERS_INPUT_MANAGER_H__

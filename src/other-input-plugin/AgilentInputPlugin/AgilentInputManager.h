/**
 * @file AgilentInputManager.h
 * @brief interfaces of AgilentInputManager class
 *
 * @author S.Tanaka
 * @date 2009.05.27
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_IO_AGILENT_INPUT_MANAGER_H__
#define __KOME_IO_AGILENT_INPUT_MANAGER_H__


namespace kome {
	namespace io {
		namespace agilent {

			ref class AgilentSampleSet;

			/**
			 * @class AgilentInputManager
			 * @brief agilent sample file input management class
			 */
			public ref class AgilentInputManager {
			protected:
				/**
				 * @fn AgilentInputManager()
				 * @brief constructor
				 */
				AgilentInputManager();

				/**
				 * @fn virtual ~AgilentInputManager()
				 * @brief destructor
				 */
				virtual ~AgilentInputManager();

			protected:
				/** micromass file list */
				System::Collections::ArrayList^ m_list;

			public:
				/**
				 * @fn void addSampleSet( AgilentSampleSet^ sampleSet )
				 * @brief adds sample set
				 * @param sampleSet sample set object to be added
				 */
				void addSampleSet( AgilentSampleSet^ sample );

				/**
				 * @fn void removeSampleSet( AgilentSampleSet^ sampleSet )
				 * @brief removes sample set
				 * @param sampleSet sample set object to be deleted
				 */
				void removeSampleSet( AgilentSampleSet^ sampleSet );

			public:
				/**
				 * @fn static kome::clr::ClrVariant^ open( kome::clr::ClrParameters^ params )
				 * @brief open micromass file
				 * @param params information object. (This object has path information.)
				 * @return sample object.
				 */
				static kome::clr::ClrVariant^ open( kome::clr::ClrParameters^ params );

				/**
				 * @fn static kome::clr::ClrVariant^ isAgilentFolder( kome::clr::ClrParameters^ params )
				 * @brief check the folder (whether the foloder is micromass folder)
				 * @param params parameters information (This object ha path information.)
				 * @return If specified path is micromass folder, this method returns true.
				 */
				static kome::clr::ClrVariant^ isAgilentFolder( kome::clr::ClrParameters^ params );

			protected:
				/** agilent input manager object (This is the only object.) */
				static AgilentInputManager^ m_instance;

			public:
				/**
				 * @fn static AgilentInputManager^ getInstance()
				 * @brief gets agilent input manager object
				 * @return agilent input manager object (This is the only object.)
				 */
				static AgilentInputManager^ getInstance();
			};
		}
	}
}

#endif	// __KOME_IO_AGILENT_INPUT_MANAGER_H__

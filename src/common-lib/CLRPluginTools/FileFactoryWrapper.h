/**
 * @file FileFactoryWrapper.h
 * @brief interfaces of FileFactoryWrapper class
 *
 * @author S.Tanaka
 * @date 2007.01.22
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_CLR_FILE_FACTORY_WRAPPER_H__
#define __KOME_CLR_FILE_FACTORY_WRAPPER_H__


namespace kome { 
	namespace clr {

		/**
		 * @class FileFactoryWrapper
		 * @brief file factory wrapper class to use on CLR environment
		 */
		public ref class FileFactoryWrapper {
		public:
			/**
			 * @fn FileFactoryWrapper( kome::plugin::FileFactory& factory )
			 * @brief constructor
			 * @param[in] factory file factory object
			 */
			FileFactoryWrapper( kome::plugin::FileFactory& factory );

			/**
			 * @fn virtual ~FileFactoryWrapper()
			 * @brief destructor
			 */
			virtual ~FileFactoryWrapper();

		protected:
			/** file factory object */
			kome::plugin::FileFactory& m_factory;

		public:
			/**
			 * @fn kome::plugin::FileFactory& getFileFactory()
			 * @brief gets file factory object
			 * @return file factory object
			 */
			kome::plugin::FileFactory& getFileFactory();

		public:
			/**
			 * @fn SampleSetWrapper^ openFile( System::String^ path )
			 * @brief opens sample set file
			 * @param[in] path file path
			 * @return raw data file object (If NULL, it failed to open the file)
			 */
			SampleSetWrapper^ openFile( System::String^ path );

			/**
			 * @fn void closeFile( SampleSetWrapper^ sampleSet )
			 * @brief closes sample set
			 * @param[in] sampleSet sample set object to be closed
			 */
			void closeFile( SampleSetWrapper^ sampleSet );

			/**
			 * @fn SampleSetWrapper^ searchFile( System::String^ path )
			 * @brief searches sample set
			 * @param[in] path file path
			 * @return sample set file object. (If NULL, sample set of specified file is not found.)
			 */
			SampleSetWrapper^ searchFile( System::String^ path );

		public:
			/**
			 * @fn unsigned int getNumberOfFileFormats()
			 * @brief gets the number of file formats
			 * @return the number of file formats
			 */			 
			unsigned int getNumberOfFileFormats();

			/**
			 * @fn System::String^ getFormatName( unsigned int index )
			 * @brief gets file format name
			 * @param[in] index file format index
			 * @return file format name. (If the index is illegal, this method returns NULL.)
			 */
			System::String^ getFormatName( unsigned int index );

			/**
			 * @fn cli::array< System::String^ >^ getExtensions( unsigned int index )
			 * @brief gets file extensions
			 * @param[in] index file format index
			 * @return extenstions array
			 */
			cli::array< System::String^ >^ getExtensions( unsigned int index );

		public:
			/**
			 * @fn static FileFactoryWrapper^ getInstance()
			 * @brief get FileFactory object (This is the only object.)
			 * @return FileFactory object
			 */
			static FileFactoryWrapper^ getInstance();
		};
	}
}

#endif	// __KOME_CLR_FILE_FACTORY_WRAPPER_H__

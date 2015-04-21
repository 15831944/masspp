/**
 * @file FileFactory.h
 * @brief interfaces of FileFactory class
 *
 * @author S.Tanaka
 * @date 2006.08.17
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_IO_FILE_FACTORY_H__
#define __KOME_IO_FILE_FACTORY_H__


#include <list>
#include <vector>


namespace kome {
	namespace plugin {

		class PluginCall;

		/**
		 * @class FileFactory
		 * @brief raw data input management class
		 */
		class PLUGIN_TOOLS_CLASS FileFactory : public kome::objects::DefaultDataManager {
		protected:
			/**
			 * @fn FileFactory()
			 * @brief constructor
			 */
			FileFactory();

			/**
			 * @fn virtual ~FileFactory()
			 * @brief destructor
			 */
			virtual ~FileFactory();

		protected:
			/**
			 * @struct OpenInfo
			 * @brief file open function infomatio struct 
			 */
			struct OpenInfo {
				PluginCall* openCall; // file open function
				PluginCall* supportedCall; //  the function that judges supported
				std::list< std::string > exts; // set of extensions
				std::string name; // name
			};

			/** the array of open & close function informations */
			std::vector< OpenInfo > m_openFunctions;

			/** map of sample set objects */
			std::map< std::string, kome::objects::SampleSet* > m_sampleSetMap;

		public:
			/**
			 * @fn kome::objects::SampleSet* openFile( const char* path, kome::core::Progress* progress )
			 * @brief opens sample set file
			 * @param[in] path file path
			 * @param[in] progress
			 * @return raw data file object (If NULL, it failed to open the file)
			 */
			kome::objects::SampleSet* openFile( const char* path, kome::core::Progress* progress= NULL );

			/**
			 * @fn void closeFile( kome::objects::SampleSet* sampleSet )
			 * @brief closes sample set
			 * @param[in,out] sampleSet sample set object to be closed
			 */
			void closeFile( kome::objects::SampleSet* sampleSet );

			/**
			 * @fn kome::objects::SampleSet* searchFile( const char* path )
			 * @brief searches sample set
			 * @param[in] path file path
			 * @return sample set file object. (If NULL, sample set of specified file is not found.)
			 */
			kome::objects::SampleSet* searchFile( const char* path );

		protected:
			/**
			 * @fn kome::objects::SampleSet* onOpenFile( const char* path, kome::core::Progress* progress )
			 * @brief This method is called by openFile method
			 * @param[in] path file path
			 * @param[in] progress
			 * @return raw data file object (If NULL, it failed to open the file)
			 */
			kome::objects::SampleSet* onOpenFile( const char* path, kome::core::Progress* progress = NULL );

			/**
			 * @fn void onCloseFile( kome::objects::SampleSet* sampleSet )
			 * @brief This method is called by closeFile method
			 * @param[in,out] sampleSet sample set object to be closed
			 */
			void onCloseFile( kome::objects::SampleSet* sampleSet );

		protected:
			/**
			 * @fn void createFunctionArrays()
			 * @brief creates arrays of function informations
			 */
			void createFunctionArrays();

		public:
			/**
			 * @fn unsigned int getNumberOfFileFormats()
			 * @brief gets the number of file formats
			 * @return the number of file formats
			 */			 
			unsigned int getNumberOfFileFormats();

			/**
			 * @fn const char* getFormatName( const unsigned int index )
			 * @brief gets file format name
			 * @param[in] index file format index
			 * @return file format name. (If the index is illegal, this method returns NULL.)
			 */
			const char* getFormatName( const unsigned int index );

			/**
			 * @fn void getExtensions(
					const unsigned int index,
					std::vector<std::string> &extentions
				)
			 * @brief gets file extensions
			 * @param[in] index file format index
			 * @param[out] extentions the array of store extentions
			 */
			void getExtensions(
				const unsigned int index,
				std::vector<std::string> &extentions
			);

		protected:
			/**
			 * @fn virtual void onCloseSample( kome::objects::Sample* sample, const bool deleting )
			 * @brief This method is called when a sample is closed. (override method)
			 * @param[in] sample sample object to be closed
			 * @param[in] deleting If true, the specified object is being deleted now.
			 */
			virtual void onCloseSample( kome::objects::Sample* sample, const bool deleting );

		public:
			/**
			 * @fn static FileFactory& getInstance()
			 * @brief get FileFactory object (This is the only object.)
			 * @return FileFactory object
			 */
			static FileFactory& getInstance();

		protected:
			/**
			 * @fn static bool lessOpenInfo( const OpenInfo& oi0, const OpenInfo& oi1 )
			 * @brief compares structure of open function information to sort
			 * @param[in] oi0 structure of open function information to compare
			 * @param[in] oi1 structure of open function information to be compared
			 * @return If oi0 sorts before oi1, return true
			 */
			static bool lessOpenInfo( const OpenInfo& oi0, const OpenInfo& oi1 );
		};
	}
}

#endif		//  __KOME_IO_FILE_FACTORY_H__

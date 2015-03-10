/**
 * @file MzXmlSample.h
 * @brief interfaces of MzXmlSample class
 *
 * @author S.Tanaka
 * @date 2008.01.18
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_IO_MZXML_SAMPLE_H__
#define __KOME_IO_MZXML_SAMPLE_H__


namespace kome {
	namespace io {
		namespace mzxml {

			class MzXmlSampleSet;

			/**
			 * @class MzXmlSample
			 * @brief mzXML sample class
			 */
			class MzXmlSample : public kome::objects::Sample {
			public:
				/**
				 * @fn MzXmlSample( MzXmlSampleSet* sampleSet )
				 * @brief constructor
				 * @param sampleSet sample set object
				 */
				MzXmlSample( MzXmlSampleSet* sampleSet );

				/**
				 * @fn virtual ~MzXmlSample()
				 * @brief destructor
				 */
				virtual ~MzXmlSample();

			protected:
				/** sample set */
				MzXmlSampleSet* m_sampleSet;

				/** parent files */
				std::vector< std::string > m_parentFiles;

				/** data directory */
				std::string m_dataFile;

				/** file */
				FILE* m_fp;

			protected:
				/** parent file key count */
				static unsigned int m_parentFileCnt;

			public:
				/**
				 * @fn MzXmlSampleSet* getMzXmlSampleSet()
				 * @brief gets mzXML sample set object
				 * @return mzXML sample set object
				 */
				MzXmlSampleSet* getMzXmlSampleSet();

			public:
				/**
				 * @fn void addParentFile( const char* path )
				 * @brief adds parent files
				 * @param path parent file path
				 */
				void addParentFile( const char* path );

				/**
				 * @fn unsigned int getNumberOfParentFiles()
				 * @brief gets the number of parent files
				 * @return the number of parent files
				 */
				 unsigned int getNumberOfParentFiles();

				/**
				 * @fn const char* getParentFilePath( const unsigned int index )
				 * @brief gets parent file path
				 * @param index parent file index
				 * @return parent file path (If the index is illegal, this method returns NULL.)
				 */
				const char* getParentFilePath( const unsigned int index );

				/**
				 * @fn const char* getDataFile()
				 * @brief gets the data directory
				 * @return data directory
				 */
				const char* getDataFile();

				/**
				 * @fn void closeFile()
				 * @brief closes file
				 */
				void closeFile();

				/**
				 * @fn FILE* openReadFile()
				 * @brief open the read file
				 * @return file
				 */
				FILE* openReadFile();

				/**
				 * @fn FILE* openWriteFile()
				 * @brief open the write file
				 * @return file
				 */
				FILE* openWriteFile();

				/**
				 * @fn FILE* getFile()
				 * @return gets the file
				 * @return file
				 */
				FILE* getFile();

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


#endif		// __KOME_IO_MZXML_SAMPLE_H__

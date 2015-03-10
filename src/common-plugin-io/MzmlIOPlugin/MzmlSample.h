/**
 * @file MzmlSample.h
 * @brief interfaces of MzmlSample class
 *
 * @author S.Tanaka
 * @date 2009.03.09
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_IO_MZML_SAMPLE_H__
#define __KOME_IO_MZML_SAMPLE_H__


namespace kome {
	namespace io {
		namespace mzml {

			class MzmlSampleSet;

			/**
			 * @class MzmlSample
			 * @brief mzML sample class
			 */
			class MzmlSample : public kome::objects::Sample {
			public:
				/**
				 * @fn MzmlSample( MzmlSampleSet* sampleSet )
				 * @brief constructor
				 * @param sampleSet sample set object
				 */
				MzmlSample( MzmlSampleSet* sampleSet );

				/**
				 * @fn virtual ~MzmlSample()
				 * @brief destructor
				 */
				virtual ~MzmlSample();

			public:
				/** sample set */
				MzmlSampleSet* m_sampleSet;

				/** parent files */
				std::vector< std::string > m_parentFiles;

			public:
				/**
				 * @struct ArrayInfo
				 * @param array data information
				 */
				struct ArrayInfo {
					std::string name;
					int bits;
					bool compressed;
					double scale;
					bool isY;
				};

			protected:
				/** array information */
				std::vector< ArrayInfo > m_arrayInfo;

			public:
				/**
				 * @fn MzmlSampleSet* getMzmlSampleSet()
				 * @brief gets mzML sample set object
				 * @return mzML sample set object
				 */
				MzmlSampleSet* getMzmlSampleSet();

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

			public:
				/**
				 * @fn void addArrayInfo(
							const char* name,
							const int bits,
							const bool compressed,
							const double scale,
							const bool isY
						)
				 * @brief adds array information
				 * @param name array information name
				 * @param bits data size of element
				 * @param compressed if true, the array data is compressed
				 * @param scale unit size scale
				 * @param isY if true, the array data is y coordinate values
				 */
				void addArrayInfo(
					const char* name,
					const int bits,
					const bool compressed,
					const double scale,
					const bool isY
				);

				/**
				 * @fn ArrayInfo* getArrayInfo( const char* name )
				 * @brief gets array information
				 * @param name array information name
				 * @return array information (If NULL, specified name is not found.)
				 */
				ArrayInfo* getArrayInfo( const char* name );

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


#endif		// __KOME_IO_MZML_SAMPLE_H__

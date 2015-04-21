/**
* @file BatchInfo.h
* @brief Batch Info
* @author OKADA, H.
* @date 2012/02/03
* Copyright (C) 2012 Shimadzu Corporation All rights reserved.
*/

#ifndef _BATCH_INFO_H_
#define _BATCH_INFO_H_

#include "stdafx.h"
#include "PluginFunctionItem.h"

#include <map>
#include <vector>


#ifndef FILE_SAVE_FUNC_TYPE_NAME
#define FILE_SAVE_FUNC_TYPE_NAME   "SAVE_FILE"
#endif

#ifndef BATCH_TYPE_NAME
#define BATCH_TYPE_NAME   "batch"
#endif


namespace kome {
	namespace plugin {

		class PluginInfo;

		/** 
		 * @class BatchInfo
		 * @brief Batch Info class
		 */
		class PLUGIN_TOOLS_CLASS BatchInfo{

		public:
			/**
			 * @fn BatchInfo
			 * @brief constructor
			 */
			BatchInfo();
			
			/**
			 * @fn BatchInfo
			 * @brief destructor
			 */
			virtual ~BatchInfo();

		private:

			/** batch processing list name */
			std::string m_strBatchName;

			/** batch comment */
			std::string m_strComment;

			/** string of output file format */
			std::vector <std::string> m_vecstrFormatString;

			/** input file */
			std::vector <std::string> m_vecstrInputFile;

			/** output file */
			std::map<int, std::map<int, std::string> > m_mapOutputFile;

			/** data type */
			std::map<int, std::map<int, std::string> > m_mapDataType;	// @date 2012/08/29 <Add> OKADA

			/**
			 * @typedef call_batch_tag
			 * @brief for call
			 */
			typedef struct _call_batch{
				std::string strBatchProcessingFunction;	
				std::string strParameterOfBatchProcessingFunction;
			}call_batch_tag;
			
			/** call */
			std::vector <call_batch_tag> m_vectCall;

			/** array of SettingParameterValues */
			std::vector<kome::objects::SettingParameterValues *> m_vecpSettings;	
		public:
			/**
			 * @fn void setName( std::string )
			 * @brief A batch processing list name is set. 
			 * @param[in] strBatchName batch processing list name
			 */
			void setName( std::string strBatchName);

			/**
			 * @fn std::string getName( void );
			 * @brief A batch processing list name is acquired. 
			 * @return batch processing list name
			 */
			std::string getName( void );

			/**
			 * @fn void setComment( std::string )
			 * @brief A comment is set to a batch. 
			 * @param[in] strComment comment
			 */
			void setComment( std::string strComment );

			/**
			 * @fn std::string getComment( void )
			 * @brief A comment is acquired from a batch. 
			 * @return comment
			 */
			std::string getComment( void );

			/**
			 * @fn void clearFiles()
			 * @brief clears input/output file information.
			 */
			void clearFiles();

			/**
			 * @fn void initialize()
			 * @brief initialize
			 */
			void initialize();

			/**
			 * @fn void addSaveFunction( std::string )
			 * @brief An output file format is set. What is set is a character string which shows a format. 
			 * @param[in] strFormatString string of output file format
			 */
			void addSaveFunction( std::string strFormatString );

			/**
			 * @fn int getNumberOfSaveFunctions( void )
			 * @brief The number of output file formats is acquired. 
			 * @return number of output file formats
			 */
			int getNumberOfSaveFunctions( void );

			/**
			 * @fn kome::plugin::PluginFunctionItem* getSaveFunction( int nIndexOfBatch )
			 * @brief An output file format is acquired. 
			 * @param[in] nIndexOfBatch index of batch
			 * @return output file format
			 */
			kome::plugin::PluginFunctionItem* getSaveFunction( int nIndexOfBatch );

			/**
			 * @fn const char* getSaveFunction( int nIndexOfBatch )
			 * @brief An output file format is acquired. 
			 * @param[in] nIndexOfBatch index of batch
			 * @return output file format(string)
			 */
			const char* getSaveFunctionString( int nIndexOfBatch );

			/**
			 * @fn void addInputFile( std::string )
			 * @brief An input file is added. 
			 * @param[in] strInputFile input file
			 */
			void addInputFile( std::string strInputFile );

			/**
			 * @fn void addInputFile( std::string, std::string );
			 * @brief An input file is added. 
			 * @param[in] strDataType input file
			 * @param[in] strKey input file
			 */
			void addInputFile( std::string strDataType, std::string strKey );

			/**
			 * @fn std::string getInputFileType( int nIndex )
			 * @brief Get data-type from index number
			 * @param[in] nIndex nIndex index number
			 */
			 std::string getInputFileType( int nIndex );

			/**
			 * @fn int getNumberOfInputFiles( void )
			 * @brief The number of input files is acquired. 
			 * @return number of input files
			 */
			int getNumberOfInputFiles( void );

			/**
			 * @fn std::string getInputFile( int nIndexOfBatch )
			 * @brief An input file name is acquired. 
			 * @param[in] nIndexOfBatch index of batch
			 * @return input file name
			 */
			std::string getInputFile( int nIndexOfBatch );

			/**
			 * @fn void setOutputFile( int nIndexOfInputFile, int nIndexOfFormat, std::string strOutputFilePath)
			 * @brief An output file name is set. Arguments are an index of Input File, an index of a format, and an output file path. 
			 * @param[in] nIndexOfInputFile index of Input File
			 * @param[in] nIndexOfFormat index of a format
			 * @param[in] strOutputFilePath output file path
			 * @return output file name
			 */
			void setOutputFile( int nIndexOfInputFile, int nIndexOfFormat, std::string strOutputFilePath);

			/**
			 * @fn std::string getOutputFile( int nIndexOfInputFile, int nIndexOfFormat )
			 * @brief An output file name is acquired. Arguments are an index of InputFile, and an index of a format. 
			 * @param[in] nIndexOfInputFile index of InputFile
			 * @param[in] nIndexOfFormat index of a format
			 * @return output file name
			 */
			std::string getOutputFile( int nIndexOfInputFile, int nIndexOfFormat );

			/**
			 * @fn void setDataTypeIO( int nIndexOfInputFile, int nIndexOfFormat, std::string strDataType )
			 * @brief An data type name is set. Arguments are an index of Input File, an index of a format, and an data type. 
			 * @param[in] nIndexOfInputFile index of Input File
			 * @param[in] nIndexOfFormat index of a format
			 * @param[in] strDataType data type
			 */
			void setDataTypeIO( int nIndexOfInputFile, int nIndexOfFormat, std::string strDataType );

			/**
			 * @fn std::string getDataTypeIO( int nIndexOfInputFile, int nIndexOfFormat )
			 * @brief An data type name is acquired. Arguments are an index of InputFile, and an index of a format. 
			 * @param[in] nIndexOfInputFile index of InputFile
			 * @param[in] nIndexOfFormat index of a format
			 * @return data type name
			 */
			std::string getDataTypeIO( int nIndexOfInputFile, int nIndexOfFormat );

			/**
			 * @fn int getNumberOfBatchFunctions( void )
			 * @brief The number of batch processing is acquired. 
			 * @return NumberOfBatchFunctions
			 */
			int getNumberOfBatchFunctions( void );

			/**
			 * @fn void addCall( std::string strCommandlineOption, std::string strParameterOfOption)
			 * @brief Batch processing is added. 
			 * @param[in] strCommandlineOption option of batch(batch processing function)
			 * @param[in] strParameterOfOption parameter of option
			 */
			void addCall( std::string strCommandlineOption, std::string strParameterOfOption);

			/**
			 * @fn kome::plugin::PluginFunctionItem* getBatchFunction( int nIndexOfBatch )
			 * @brief A batch processing function is acquired. 
			 * @param[in] nIndexOfBatch index of batch
			 * @return batch processing function
			 */
			kome::plugin::PluginFunctionItem* getBatchFunction( int nIndexOfBatch );

			/**
			 * @fn kome::objects::SettingParameterValues* getBatchParameter( int nIndexOfBatch )
			 * @brief A batch processing parameter is acquired. 
			 * @param[in] nIndexOfBatch index of batch
			 * @return batch processing parameter
			 */
			kome::objects::SettingParameterValues* getBatchParameter( int nIndexOfBatch );

			/**
			 * @fn int getBatchProcessingFunction( int nIndexOfBatch, std::string& strFunction )
			 * @brief A batch processing function is acquired. 
			 * @param[in] nIndexOfBatch index of batch
			 * @param[in] strFunction processing function string
			 * @return -1:fail 0:success
			 */
			int getBatchProcessingFunction( int nIndexOfBatch, std::string& strFunction );

			/**
			 * @fn int getParameterOfBatchProcessingFunction( int nIndexOfBatch, std::string strParameter )
			 * @brief A batch processing function is acquired. 
			 * @param[in] nIndexOfBatch index of batch
			 * @param[in] strParameter processing parameter string 
			 * @return -1:fail 0:success
			 */
			int getParameterOfBatchProcessingFunction( int nIndexOfBatch, std::string& strParameter );

		};
	}
}

#endif

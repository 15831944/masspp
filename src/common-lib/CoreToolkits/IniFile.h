/**
 * @file IniFile.h
 * @brief interfaces of IniFile class
 *
 * @author S.Tanaka
 * @date 2006.08.18
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_CORE_INI_FILE_H__
#define __KOME_CORE_INI_FILE_H__


#include "Properties.h"

#include <string>
#include <vector>
#include <map>


namespace kome {
	namespace core {

		/**
		 * @class IniFile
		 * @brief ini file management class
		 */
		class CORE_TOOLKITS_CLASS IniFile {
		protected:
			/**
			 * @fn IniFile()
			 * @brief constructor
			 */
			IniFile();

			/**
			 * @fn virtual ~IniFile()
			 * @brief destructor
			 */
			virtual ~IniFile();

		public:
			/**
			 * @fn const char* getFilePath()
			 * @brief gets file path
			 * @return file path
			 */
			const char* getFilePath();

		protected:
			/** file path */
			std::string m_filePath;

			/** the array of section */
			std::vector< std::string > m_sections;

			/** section Properties map (section -> Properties) */
			std::map< std::string, Properties > m_sectionMap;

		public:
			/**
			 * @fn bool decryptPassword( const char* pcPassword, char* pcDecrypt, const size_t szDecrypt )
			 * @brief decrypt password
			 * @param[in] pcPassword encrypted password string
			 * @param[in] pcDecrypt decrypted password string
			 * @param[in] szDecrypt size of decrypt string buffer
			 * @return If it succeeded to decrypt passsword, this method returns true.
			 */
			bool decryptPassword( const char* pcPassword, char* pcDecrypt, const size_t szDecrypt );

			/**
			 * @fn bool encryptPassword( const char* pcPassword, char* pcEncrypt, const size_t szEncrypt )
			 * @brief decrypt password
			 * @param[in] pcPassword plain text password string
			 * @param[in] pcEncrypt encrypted password string
			 * @param[in] szEncrypt size of encrypt string buffer
			 * @return If it succeeded to encrypt passsword, this method returns true.
			 */
			bool encryptPassword( const char* pcPassword, char* pcEncrypt, const size_t szEncrypt );

		public:

			/**
			 * @fn void setString( const char* section, const char* key, const char* value )
			 * @brief sets string value
			 * @param[in] section parameter section
			 * @param[in] key parameter key
			 * @param[in] value parameter value
			 */
			void setString( const char* section, const char* key, const char* value );

			/**
			 * @fn const char* getString( const char* section, const char* key, const char* defaultValue )
			 * @brief gets string value
			 * @param[in] section parameter section
			 * @param[in] key parameter key
			 * @param[in] defaultValue default value
			 * @return parameter value.
					   (If there is not specified parameter, this method returns defaultValue.)
			 */
			const char* getString( const char* section, const char* key, const char* defaultValue );

			/**
			 * @fn void setBool( const char* section, const char* key, const bool value )
			 * @brief sets boolean value
			 * @param[in] section parameter section
			 * @param[in] key parameter key
			 * @param[in] value parameter value
			 */
			void setBool( const char* section, const char* key, const bool value );

			/**
			 * @fn bool getBool( const char* section, const char* key, const bool defaultValue )
			 * @brief gets boolean value
			 * @param[in] section parameter section
			 * @param[in] key parameter key
			 * @param[in] defaultValue default value
			 * @return parameter value.
					   (If there is not specified parameter or value is not integer number,
						this method returns defaultValue.)
			 */
			bool getBool( const char* section, const char* key, const bool defaultValue );

			/**
			 * @fn void setInt( const char* section, const char* key, const int value )
			 * @brief sets integer number value
			 * @param[in] section parameter section
			 * @param[in] key parameter key
			 * @param[in] value parameter value
			 */
			void setInt( const char* section, const char* key, const int value );

			/**
			 * @fn int getInt( const char* section, const char* key, const int defaultValue )
			 * @brief get integer number value
			 * @param[in] section parameter section
			 * @param[in] key parameter key
			 * @param[in] defaultValue default value
			 * @return parameter value.
					   (If there is not specified parameter or value is not integer number,
						this method returns defaultValue.)
			 */
			int getInt( const char* section, const char* key, const int defaultValue );

			/**
			 * @fn void setDouble( const char* section, const char* key, const double value )
			 * @brief sets real number value
			 * @param[in] section parameter section
			 * @param[in] key parameter key
			 * @param[in] value parameter value
			 */
			void setDouble( const char* section, const char* key, const double value );

			/**
			 * @fn double getDouble( const char* section, const char* key, const double defaultValue )
			 * @brief gets real number value
			 * @param[in] section parameter section
			 * @param[in] key parameter key 
			 * @param[in] defaultValue default value
			 * @return parameter value.
					   (If there is not specified parameter or value is not real number,
						this method returns defaultValue.)
			 */
			double getDouble( const char* section, const char* key, const double defaultValue );

			/**
			 * @fn void deleteValue( const char* section, const char* key )
			 * @brief deletes value
			 * @param[in] section parameter section
			 * @param[in] key parameter key
			 */
			void deleteValue( const char* section, const char* key );

		protected:
			/**
			 * @fn Properties* getProperties( const char* section )
			 * @brief gets properties object specified section
			 * @param[in] section section
			 * @return properties object specified section.
					   (If there isn't section specified name, this method returns NULL.)
			 */
			Properties* getProperties( const char* section );

		public:
			/**
			 * @fn unsigned int getNumberOfSections()
			 * @brief gets the number of sections
			 * @return the number of sections
			 */
			unsigned int getNumberOfSections();

			/**
			 * @fn const char* getSection( const unsigned int index )
			 * @brief gets section
			 * @param[in] index section index
			 * @return section specified index (If the index is illegal, this method returns NULL.)
			 */
			const char* getSection( const unsigned int index );

			/**
			 * @fn unsigned int getNumberOfParameters( const char* section )
			 * @brief gets the number of parameters specified section
			 * @param[in] section parameter section
			 * @return the number of parameters specified section
			 */
			unsigned int getNumberOfParameters( const char* section );

			/**
			 * @fn const char* getParameterName( const char* section, const unsigned int index )
			 * @brief gets parameter name
			 * @param[in] section parameter section
			 * @param[in] index parameter index
			 * @return parameter name specified section and index 
					   (If the section or the index is illegal, this method returns NULL.)
			 */
			const char* getParameterName( const char* section, const unsigned int index );

			/**
			 * @fn const char* getParameterValue( const char* section, const unsigned int index )
			 * @brief gets parameter value
			 * @param[in] section parameter section
			 * @param[in] index parameter index
			 * @return parameter value specified section and index
					   (If the section or the index is illegal, this method returns NULL.)
			 */
			const char* getParameterValue( const char* section, const unsigned int index );

		public:
			/**
			 * @fn bool load( const char* path )
			 * @brief load the ini file
			 * @param[in] path ini file path
			 * @return If it succeeded to load the file, this method returns true.
			 */
			bool load( const char* path );

			/**
			 * @fn bool save( const char* path = NULL )
			 * @brief save the ini file
			 * @param[in] path ini file path. (If path is NULL, it is saved to the same place as the loaded file.)
			 * @return If it succeeded to save the file, this method returns true.
			 */
			bool save( const char* path = NULL );

			/**
			 * @fn static IniFile* loadIniFile( const char* path )
			 * @brief loads ini file and gets IniFile object.
			 * @param[in] path ini file path
			 * @return ini file information object.
			 */
			static IniFile* loadIniFile( const char* path );

			/**
			 * @fn static IniFile* getIniFile( const char* path )
			 * @brief gets ini file object. If specified file is not opened, this method calls loadIniFile method.
			 * @param[in] path ini file path.
			 * @return ini file information object.
			 */
			static IniFile* getIniFile( const char* path );

			/**
			 * @fn static void saveAllIniFiles()
			 * @brief save all ini files
			 */
			static void saveAllIniFiles();

		protected:
			/**
			 * @class IniFileManager
			 * @brief IniFile object management class
			 */
			class IniFileManager {
			public:
				/**
				 * @fn IniFileManager()
				 * @brief constructor
				 */
				IniFileManager();
			
				/**
				 * @fn virtual ~IniFileManager()
				 * @brief destructor
				 */
				virtual ~IniFileManager();

			protected:
				/** file map (path -> ini file data) */
				std::map< std::string, IniFile* > m_fileMap;

			public:
				/**
				 * @fn void setFile( IniFile* file )
				 * @brief sets ini file information object to map
				 * @param[in] file ini file information object
				 */
				void setFile( IniFile* file );

				/**
				 * @fn IniFile* getFile( const char* path )
				 * @brief gets ini file information object
				 * @param[in] path ini file path
				 * @return ini file information object
				 */
				IniFile* getFile( const char* path );

				/**
				 * @fn void save()
				 * @brief save all ini files
				 */
				void save();

			public:
				/**
				 * @fn static IniFileManager& getInstance()
				 * @brief gets ini file manager object (This is the only object.)
				 * @return ini file manager object
				 */
				static IniFileManager& getInstance();
			};
		};
	}
}

#endif	// __KOME_CORE_INI_FILE_H__

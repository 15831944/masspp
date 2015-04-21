/**
 * @file ClrIniFile.h
 * @brief interfaces of ClrInifile class
 *
 * @author S.Tanaka
 * @date 2006.11.07
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_CLR_INI_FILE_H__
#define __KOME_CLR_INI_FILE_H__

namespace kome {
	namespace clr {

		/**
		 * @class ClrIniFile
		 * @brief ini file class on CLR environment
		 */
		public ref class ClrIniFile {
		protected:
			/**
			 * @fn ClrIniFile( kome::core::IniFile& ini )
			 * @brief constructor
			 * @param[in] ini ini file object
			 */
			ClrIniFile( kome::core::IniFile& ini );

			/**
			 * @fn virtual ~ClrIniFile()
			 * @brief destructor
			 */
			virtual ~ClrIniFile();

		public:
			/**
			 * @fn kome::core::IniFile& getIniFile()
			 * @brief gets ini file object
			 * @return ini file object
			 */
			kome::core::IniFile& getIniFile();

		protected:
			/** ini file object */
			kome::core::IniFile& m_ini;

		public:
			/**
			 * @fn System::String^ getFilePath()
			 * @brief gets file path
			 * @return file path
			 */
			System::String^ getFilePath();

			/**
			 * @fn void setString( System::String^ section, System::String^ key, System::String^ value )
			 * @brief sets string value
			 * @param[in] section parameter section
			 * @param[in] key parameter key
			 * @param[in] value parameter value
			 */
			void setString( System::String^ section, System::String^ key, System::String^ value );

			/**
			 * @fn System::String^ getString( System::String^ section, System::String^ key, System::String^ defaultValue )
			 * @brief gets string value
			 * @param[in] section parameter section
			 * @param[in] key parameter key
			 * @param[in] defaultValue default value
			 * @return parameter value.
					   (If there is not specified parameter, this method returns defaultValue.)
			 */
			System::String^ getString( System::String^ section, System::String^ key, System::String^ defaultValue );

			/**
			 * @fn void setInt( System::String^ section, System::String^ key, int value )
			 * @brief sets integer number value
			 * @param[in] section parameter section
			 * @param[in] key parameter key
			 * @param[in] value parameter value
			 */
			void setInt( System::String^ section, System::String^ key, int value );

			/**
			 * @fn int getInt( System::String^ section, System::String^ key, int defaultValue )
			 * @brief get integer number value
			 * @param[in] section parameter section
			 * @param[in] key parameter key
			 * @param[in] defaultValue default value
			 * @return parameter value.
					   (If there is not specified parameter or value is not integer number,
						this method returns defaultValue.)
			 */
			int getInt( System::String^ section, System::String^ key, int defaultValue );

			/**
			 * @fn void setDouble( System::String^ section, System::String^ key, double value )
			 * @brief sets real number value
			 * @param[in] section parameter section
			 * @param[in] key parameter key
			 * @param[in] value parameter value
			 */
			void setDouble( System::String^ section, System::String^ key, double value );

			/**
			 * @fn double getDouble( System::String^ section, System::String^ key, double defaultValue )
			 * @brief gets real number value
			 * @param[in] section parameter section
			 * @param[in] key parameter key 
			 * @param[in] defaultValue default value
			 * @return parameter value.
					   (If there is not specified parameter or value is not real number,
						this method returns defaultValue.)
			 */
			double getDouble( System::String^ section, System::String^ key, double defaultValue );

			/**
			 * @fn void deleteValue( System::String^ section, System::String^ key )
			 * @brief deletes value
			 * @param[in] section parameter section
			 * @param[in] key parameter key
			 */
			void deleteValue( System::String^ section, System::String^ key );

		public:
			/**
			 * @fn unsigned int getNumberOfSections()
			 * @brief gets the number of sections
			 * @return the number of sections
			 */
			unsigned int getNumberOfSections();

			/**
			 * @fn System::String^ getSection( unsigned int index )
			 * @brief gets section
			 * @param[in] index section index
			 * @return section specified index (If the index is illegal, this method returns NULL.)
			 */
			System::String^ getSection( unsigned int index );

			/**
			 * @fn unsigned int getNumberOfParameters( System::String^ section )
			 * @brief gets the number of parameters specified section
			 * @param[in] section parameter section
			 * @return the number of parameters specified section
			 */
			unsigned int getNumberOfParameters( System::String^ section );

			/**
			 * @fn System::String^ getParameterName( System::String^ section, unsigned int index )
			 * @brief gets parameter name
			 * @param[in] section parameter section
			 * @param[in] index parameter index
			 * @return parameter name specified section and index 
					   (If the section or the index is illegal, this method returns NULL.)
			 */
			System::String^ getParameterName( System::String^ section, unsigned int index );

			/**
			 * @fn System::String^ getParameterValue( System::String^ section, unsigned int index )
			 * @brief gets parameter value
			 * @param[in] section parameter section
			 * @param[in] index parameter index
			 * @return parameter value specified section and index
					   (If the section or the index is illegal, this method returns NULL.)
			 */
			System::String^ getParameterValue( System::String^ section, unsigned int index );

		public:
			/**
			 * @fn bool load( System::String^ path )
			 * @brief load the ini file
			 * @param[in] path ini file path
			 * @return If it succeeded to load the file, this method returns true.
			 */
			bool load( System::String^ path );

			/**
			 * @fn bool save( System::String^ path )
			 * @brief save the ini file
			 * @param[in] path ini file path. (If path is NULL, it is saved to the same place as the loaded file.)
			 * @return If it succeeded to save the file, this method returns true.
			 */
			bool save( System::String^ path );

		public:
			/**
			 * @fn static ClrIniFile^ loadIniFile( System::String^ path )
			 * @brief loads ini file and gets IniFile object.
			 * @param[in] path ini file path
			 * @return ini file information object.
			 */
			static ClrIniFile^ loadIniFile( System::String^ path );

			/**
			 * @fn static ClrIniFile^ getIniFile( System::String^ path )
			 * @brief gets ini file object. If specified file is not opened, this method calls loadIniFile method.
			 * @param[in] path ini file path.
			 * @return ini file information object. (If path is NULL, this method returns the object called recently.)
			 */
			static ClrIniFile^ getIniFile( System::String^ path );
		};

	}
}

#endif	// __KOME_CLR_INI_FILE_H__

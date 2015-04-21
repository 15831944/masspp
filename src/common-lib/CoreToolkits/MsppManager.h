/**
 * @file MsppManager.h
 * @brief interfaces of MsppManager class
 *
 * @author S.Tanaka
 * @date 2008.12.09
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_CORE_MSPP_MANAGER_H__
#define __KOME_CORE_MSPP_MANAGER_H__


#include <string>


namespace kome {
	namespace core {

		class IniFile;

		/**
		 * @class MsppManager
		 * @brief Mass++ manager class
		 */
		class CORE_TOOLKITS_CLASS MsppManager {
		protected:
			/**
			 * @fn MsppManager()
			 * @brief constructor
			 */
			MsppManager();

			/**
			 * @fn virtual ~MsppManager()
			 * @brief destructor
			 */
			virtual ~MsppManager();

		protected:
			/** application name */
			std::string m_app;

			/** registory name */
			std::string m_reg;

			/** version */
			std::string m_version;

			/** Mass++ directory */
			std::string m_msppDir;

			/** config directory */
			std::string m_confDir;

			/** temporary file directory */
			std::string m_tmpDir;

			/** process temporary file directory */
			std::string m_pidTmpDir;

			/** ini file */
			IniFile* m_ini;

			/** parameters file */
			IniFile* m_params;

			/** process  */
			unsigned long m_pid;

			/** mspp plugin path */
			std::vector< std::string > m_pluginPaths;

			/** platform name */
			std::string m_platform;

			// >>>>>>	@Date:2013/08/08	<Add>	A.Ozaki
			//
			/** initialized flag of MATLAB application */
			bool	m_bInitializedMATLAB;

			//
			// <<<<<<	@Date:2013/08/08	<Add>	A.Ozaki

		public:
			/**
			 * @fn const char* getAppName()
			 * @brief gets the application name
			 * @return application name
			 */
			const char* getAppName();

			/**
			 * @fn const char* getPlatformName()
			 * @brief gets the platform name
			 * @return platform name
			 */
			const char* getPlatformName();

			/**
			 * @fn const char* getRegName()
			 * @brief gets the registory name
			 * @return gegistory name
			 */
			const char* getRegName();

			/**
			 * @fn const char* getVersion()
			 * @brief gets Mass++ version
			 * @return Mass++ version
			 */
			const char* getVersion();

			/**
			 * @fn std::vector< std::string > getPluginDir()
			 * @brief get mspp plugin dir
			 * @return plugins path
			 */
			std::vector< std::string > getPluginDir();
		protected:
			/**
			 * @fn void setPath()
			 * @brief sets file path
			 */
			void setPath();

			/**
			 * @fn void deleteTmpFiles()
			 * @brief deletes temporary files
			 */
			void deleteTmpFiles();
			
			/**
			 * @fn void setPluginDir()
			 * @brief set mspp plugin dir
			 */
			void setPluginDir();
		public:
			/**
			 * @fn const char* getMsppDir()
			 * @brief gets Mass++ directory
			 * @return Mass++ directory
			 */
			const char* getMsppDir();

			/**
			 * @fn const char* getConfDir()
			 * @brief gets config directory
			 * @return config directory
			 */
			const char* getConfDir();

			/**
			 * @fn const char* getTmpDir()
			 * @brief gets temporary file directory name
			 * @return temporary file directory name
			 */
			const char* getTmpDir();

			/**
			 * @fn std::string getTmpFileName( const char* prefix, const char* suffix, const char* dir = NULL )
			 * @brief gets temporary file name
			 * @param[in] prefix prefix of temporary file name
			 * @param[in] suffix suffix of temporary file name
			 * @param[in] dir directory name
			 * @return temporary file name
			 */
			std::string getTmpFileName( const char* prefix, const char* suffix, const char* dir = NULL );

		public:
			/**
			 * @fn void setIniFile( IniFile* ini )
			 * @brief sets ini file
			 * @param[in] ini ini file
			 */
			void setIniFile( IniFile* ini );

			/**
			 * @fn IniFile* getIniFile()
			 * @brief gets ini file
			 * @return ini file object
			 */
			IniFile* getIniFile();

			/**
			 * @fn void setParameters( IniFile* params )
			 * @brief sets parameters file
			 * @param[in] params parameters file
			 */
			void setParameters( IniFile* params );

			/**
			 * @fn IniFile* getParameters()
			 * @brief gets parameters
			 * @return parameters file object
			 */
			IniFile* getParameters();

		public:
			/**
			 * @fn unsigned int getPid()
			 * @brief gets the process ID
			 * @return process ID
			 */
			unsigned int getPid();
// >>>>>>	@Date:2013/08/08	<Add>	A.Ozaki
//
		public:
			/**
			 * @fn bool isInitializedMATLAB( void )
			 * @brief gets MATLAB's initialized flag value
			  *@return  MATLAB's initialized flag value (true:already initialized,false:Uninitailize)
			  */
			bool	isInitializedMATLAB( void );

			/**
			 * @fn void setInitializedMATLAB( void )
			 * @brief sets initialized flag value
			  */
			void	setInitializedMATLAB( void );

			/**
			 * @fn void resetInitializedMATLAB( void )
			 * @brief resets initialized flag value
			  */
			void	resetInitializedMATLAB( void );

//
// <<<<<<	@Date:2013/08/08	<Add>	A.Ozaki

		public:
			/**
			 * @fn static MsppManager& getInstance()
			 * @brief gets MsppManager object (This is the only object.)
			 * @return MsppManager object
			 */
			static MsppManager& getInstance();
		};
	}
}


#endif	// __KOME_CORE_MSPP_MANAGER_H__

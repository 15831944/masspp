/**
 * @file ClrMsppManager.h
 * @brief ClrMsppManager class
 * @author T.Okuno
 * @date 2011.08.12
 * Copyright (C) 2014 Shimadzu Corporation All rights reserved.
 */

#ifndef __KOME_CLR_MSPP_MANAGER_H__
#define __KOME_CLR_MSPP_MANAGER_H__

namespace kome {
	namespace clr {

		ref class ClrIniFile;

		/**
		 * @class ClrMsppManager
		 * @brief this sigleton class allows to access the inMass++ aplication
		 */
		public ref class ClrMsppManager {
		protected:
			/**
			 * @fn ClrMsppManager()
			 * @brief constructor
			 */
			ClrMsppManager();

			/**
			 * @fn virtual ~ClrMsppManager()
			 * @brief destructor
			 */
			virtual ~ClrMsppManager();

		protected:
			// ClrMsppManager object (This is the only object.)
			static ClrMsppManager^		m_instance = nullptr;

			// Mass++ manager object
			kome::core::MsppManager&	m_msppManager;

		public:
			/**
			 * @fn System::String^ getVersion()
			 * @brief gets Mass++ version
			 * @return Mass++ version
			 */
			System::String^ getVersion();

			/**
			 * @fn System::String^ getMsppDir()
			 * @brief gets Mass++ directory
			 * @return Mass++ directory
			 */
			System::String^ getMsppDir();

			/**
			 * @fn System::String^ getConfDir()
			 * @brief gets config directory
			 * @return config directory
			 */
			System::String^ getConfDir();

			/**
			 * @fn System::String^ getTmpDir()
			 * @brief gets temporary file directory name
			 * @return temporary file directory name
			 */
			System::String^ getTmpDir();

			/**
			 * @fn System::String^ getTmpFileName( System::String^ prefix, System::String^ suffix, System::String^ dir )
			 * @brief gets temporary file name
			 * @param[in] prefix prefix of temporary file name
			 * @param[in] suffix suffix of temporary file name
			 * @param[in] dir directory name
			 * @return temporary file name
			 */
			System::String^ getTmpFileName( System::String^ prefix, System::String^ suffix, System::String^ dir );

		public:

			/**
			 * @fn ClrIniFile^ getIniFile()
			 * @brief gets ini file
			 * @return ini file object
			 */
			ClrIniFile^ getIniFile();

			/**
			 * @fn ClrIniFile^ getParameters()
			 * @brief gets parameters
			 * @return parameters file object
			 */
			ClrIniFile^ getParameters();

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
			 * @fn static ClrMsppManager^ getInstance()
			 * @brief gets MsppManager object (This is the only object.)
			 * @return MsppManager object
			 */
			static ClrMsppManager^ getInstance();
		};
	}
}

#endif		// __KOME_CLR_MSPP_MANAGER_H__

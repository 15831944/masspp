/**
 * @file BatchAnalysis.h
 * @brief interfaces of BatchAnalysis class
 *
 * @author S.Tanaka
 * @date 2014.10.27
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation All rights reserved.
 */


#ifndef __KOME_OPERATION_BATCH_ANALYSIS_H__
#define __KOME_OPERATION_BATCH_ANALYSIS_H__



namespace kome {
	namespace operation {

		/**
		 * @class BatchAnalysis
		 * @brief alignment class
		 */
		class DATA_OPERATION_CLASS BatchAnalysis {
		public:
			/**
			 * @fn BatchAnalysis( const char* analysisName )
			 * @brief constructor
			 * @param[in] analysisName analysis name
			 */
			BatchAnalysis( const char* analysisName );

			/**
			 * @fn virtual ~BatchAnalysis()
			 * @brief destructor
			 */
			virtual ~BatchAnalysis();

		protected:
			/** name */
			std::string m_name;

			/** file properties */
			std::vector< std::string > m_fileProperties;

			/** parameters */
			kome::plugin::ParameterSettings m_params;

			/** files page */
			kome::plugin::SettingsPage* m_filesPage;

			/** parameter page */
			kome::plugin::SettingsPage* m_parametersPage;

		protected:
			/** file name property name */
			static const char* FILE_NAME_PROP_NAME;

			/** directory parameter name */
			static const char* DIR_PARAM_NAME;

		public:
			/**
			 * @fn void clearFileProperties()
			 * @brief clears file propertyes
			 */
			void clearFileProperties();

			/**
			 * @fn int addFileProperty( const char* prop )
			 * @brief adds file property
			 * @param[in] prop file property
			 * @return file property index
			 */
			int addFileProperty( const char* prop );

			/**
			 * @fn unsigned int getNumberOfFileProperties()
			 * @brief gets the number of file properties
			 * @return the number of file properties
			 */
			unsigned int getNumberOfFileProperties();

			/**
			 * @fn const char* getFileProperty( const unsigned int idx )
			 * @brief gets the file property
			 * @param[in] property index
			 * @return file property
			 */
			const char* getFileProperty( const unsigned int idx );

			/**
			 * @fn kome::plugin::SettingsPage* getFilesPage()
			 * @brief gets the files page
			 * @return files page
			 */
			kome::plugin::SettingsPage* getFilesPage();

			/**
			 * @fn void getFiles( kome::objects::SettingParameterValues* settings, std::vector< std::pair< std::string, kome::core::Properties > >& files )
			 * @brief gets files
			 * @param[in] settings parameters
			 * @param[out] paths the array to store file paths.
			 */
			void getFiles( kome::objects::SettingParameterValues* settings, std::vector< std::pair< std::string, kome::core::Properties > >& files );

			/**
			 * @fn kome::plugin::SettingsPage* getParametersPage()
			 * @brief gets the parameters page
			 * @return parameters page
			 */
			kome::plugin::SettingsPage* getParametersPage();

			/**
			 * @fn void analyze( std::vector< std::string >& paths, kome::core::Progress& Progress )
			 * @brief analyzes
			 * @param[in] paths file paths
			 * @param[out] progress the object to show progress
			 */
			void analyze( std::vector< std::string >& paths, kome::core::Progress& progress );

			/**
			 * @fn void showResult()
			 * @brief shows analysis result
			 */
			void showResult();

		protected:
			/**
			 * @fn virtual void onCreateFilesPage( kome::plugin::ParameterSettings& params, kome::plugin::SettingsPage& page )
			 * @brief creates files page
			 * @param[out] params parameters information
			 * @param[out] page page
			 */
			virtual void onCreateFilesPage( kome::plugin::ParameterSettings& params, kome::plugin::SettingsPage& page );

			/**
			 * @fn virtual void onGetFiles( kome::objects::SettingParameterValues* settings, std::vector< std::pair< std::string, kome::core::Properties > >& files )
			 * @brief This method is called form the getFiles method.
			 * @param[in] settings parameters
			 * @param[out] files the array to store file informations
			 */
			virtual void onGetFiles( kome::objects::SettingParameterValues* settings, std::vector< std::pair< std::string, kome::core::Properties > >& files );

			/**
			 * @fn virtual void onCreateParametersPage( kome::plugin::ParameterSettings& params, kome::plugin::SettingsPage& page )
			 * @brief creates parameters
			 * @param[out] params parameters information
			 * @param[out] page page
			 */
			virtual void onCreateParametersPage( kome::plugin::ParameterSettings& params, kome::plugin::SettingsPage& page );

		protected:
			/**
			 * @fn virtual void onPrepareAnalysis( std::vector< std::string >& paths, kome::core::Progress& progress ) = 0
			 * @brief on prepares analysis
			 * @param[in] paths analysis files
			 * @param[out] progress the object to show the progress
			 */
			virtual void onPrepareAnalysis( std::vector< std::string >& paths, kome::core::Progress& progress ) = 0;

			/**
			 * @fn virtual void onAnalyze( const char* path, kome::core::Progress& progress ) = 0
			 * @brief on analyze
			 * @param[in] path file path
			 * @param[out] progress the object to show the progress
			 */
			virtual void onAnalyze( const char* path, kome::core::Progress& progress ) = 0;

			/**
			 * @fn virtual void onFinishAnalysis( kome::core::Progress& progress ) = 0
			 * @brief on finish analysis
			 * @param[out] progress the object to show the progress
			 */
			virtual void onFinishAnalysis( kome::core::Progress& progress ) = 0;

			/**
			 * @fn virtual void onShowResult() = 0
			 * @brief shows analysis result
			 */
			virtual void onShowResult() = 0;


		protected:
			/**
			 * @fn static void getFiles( const char* dir, std::vector< std::pair< std::string, kome::core::Properties > >& files )
			 * @brief gets files
			 * @param[in] dir directory
			 * @param[out] files the array to store file informations
			 */
			static void getFiles( const char* dir, std::vector< std::pair< std::string, kome::core::Properties > >& files );

		};
	}
}


#endif // __KOME_OPERATION_BATCH_ANALYSIS_H__

/**
 * @file CommandManager.h
 * @brief interfaces of CommandManager class
 *
 * @author S.Tanaka
 * @date 2007.07.12
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_PLUGIN_COMMAND_MANAGER_H__
#define __KOME_PLUGIN_COMMAND_MANAGER_H__


#include <map>
#include <string>
#include <utility>


namespace kome {
	namespace plugin {

		class PluginFunctionItem;
		class SettingsForm;
		class SettingsPage;

		/**
		 * @class CommandManager
		 * @brief batch management class
		 */
		class PLUGIN_TOOLS_CLASS CommandManager {
		protected:
			/**
			 * @fn CommandManager()
			 * @brief constructor
			 */
			CommandManager();

			/**
			 * @fn virtual ~CommandManager()
			 * @brief destructor
			 */
			virtual ~CommandManager();

		public:
			/**
			 * @typedef OptionStep
			 * @brief step
			 */
			typedef enum {
				STEP_INITIALIZE,
				STEP_INPUT,
				STEP_DATA_SET,
				STEP_OPERATION,
				STEP_FILTER,	// * priority changed
				STEP_MANIPULATE,	// add
				STEP_ANALYZE,
				STEP_IDENTIFY,
				STEP_RESULTS,	// add
				STEP_OUTPUT,
				STEP_FINALIZE,
				STEP_OTHER
			} OptionStep;

		protected:
			/**
			 * @struct OptionInfo
			 * @brief option information
			 */
			struct OptionInfo {
				PluginFunctionItem* item;
				OptionStep step;
			};

			/**
			 * @struct ParameterInfo
			 * @brief parameters information
			 */
			struct ParameterInfo {
				std::string type;
				std::string title;
				std::vector< PluginFunctionItem* > items;
			};

			/** the array of batch options */
			std::vector< OptionInfo > m_commandOptions;

			/** the array of execute functions */
			std::vector< std::pair< OptionInfo, kome::objects::SettingParameterValues > > m_execFunctions;

			/** the array of parameter information */
			std::vector< ParameterInfo > m_optionParams;

			/** usage */
			std::string m_usage;

			/** plug-in paths */
			std::vector< std::string > m_pluginPaths;

		public:
			/**
			 * @fn void init( const char* type )
			 * @brief initializes
			 * @param[in] type function type ("command" or "batch")
			 */
			void init( const char* type );

			/**
			 * @fn void createFunctionsArray()
			 * @brief creates batch functions array
			 */
			void createFunctionsArray();

		protected:
			/**
			 * @fn void checkDuplicated()
			 * @brief check options are not duplicated
			 */
			void checkDuplicated();

		public:
			/**
			 * @fn bool empty()
			 * @brief judges the function array is empty
			 * @return If true, the function array is empty
			 */
			bool empty();

			/**
			 * @fn const char* getUsage()
			 * @brief shows usage
			 * @return usage
			 */
			const char* getUsage();

		protected:
			/**
			 * @fn void showArgInfo( SettingsForm* form, unsigned int space )
			 * @brief shows option argument information
			 * @param[in] form option argument object
			 * @param[in] space space count
			 */
			void showArgInfo( SettingsForm* form, unsigned int space );

			/**
			 * @fn std::string getParameterUsage( const char* value )
			 * @brief gets parameter usage
			 * @param[in] value parameter value
			 * @return parameter usage
			 */
			std::string getParameterUsage( const char* value );

		public:
			/**
			 * @fn bool parseOption( int argc, char** argv )
			 * @brief parses option
			 * @param[in] argc the number of command arguments
			 * @param[in] argv the array of command arguments
			 * @return If it succeeded to parse options, this method returns true.
			 */
			bool parseOption( int argc, char** argv );

			/**
			 * @fn bool execute()
			 * @brief executes batch
			 * @return If it succeeded to execute, this method returns true.
			 */
			bool execute();

		protected:
			/**
			 * @fn void addPluginFunction( PluginFunctionItem* item )
			 * @brief adds plug-in function to the array
			 * @param[in] item plug-in function object to be added
			 */		
			void addPluginFunction( PluginFunctionItem* item );

			/**
			 * @fn OptionInfo* getOptionInfo( char* arg )
			 * @brief gets option information
			 * @param[in] arg argument (option)
			 * @return option information structure. (If argument or value is illegal, this method returns NULL.)
			 */
			OptionInfo* getOptionInfo( char* arg );

			/**
			 * @fn std::string getSubParameters(
						const char* arg,
						kome::objects::SettingParameterValues& settings,
						const char* itemName,
						const char* paramName
					)
			 * @brief gets sub parameters
			 * @param[in] arg argument
			 * @param[out] settings settings object to store parameters
			 * @param[in] itemName item name
			 * @param[in] paramName parameter name
			 * @return actual parameter value
			 */
			std::string getSubParameters(
				const char* arg,
				kome::objects::SettingParameterValues& settings,
				const char* itemName,
				const char* paramName
			);

		public:
			/**
			 * @fn void addPluginPath( const char* path )
			 * @brief adds plug-in path
			 * @param[in] path plug-in path
			 */
			void addPluginPath( const char* path );		

			/**
			 * @fn unsigned int getNumberOfPluginPaths()
			 * @brief gets the number of plug-in paths
			 * @return the number of plug-in paths
			 */
			unsigned int getNumberOfPluginPaths();

			/**
			 * @fn const char* getPluginPath( const unsigned int idx )
			 * @brief gets the plug-in path
			 * @param[in] idx plug-in path index
			 * @return plug-in path
			 */
			 const char* getPluginPath( const unsigned int idx );

		public:
			/**
			 * @fn static CommandManager& getInstance()
			 * @brief gets CommandManager object
			 * @return CommandManager object (This is the only object.)
			 */
			static CommandManager& getInstance();

		protected:
			/**
			 * @fn static bool lessOption( OptionInfo opt0, OptionInfo opt1 )
			 * @brief compare to sort option information array
			 * @param[in] opt0 option information to be compared
			 * @param[in] opt1 option information to compare
			 * @return If opt0 sorts before opt1, this method returns true.
			 */
			static bool lessOption( OptionInfo opt0, OptionInfo opt1 );
		};
	}
}

#endif		// __KOME_PLUGIN_COMMAND_MANAGER_H__

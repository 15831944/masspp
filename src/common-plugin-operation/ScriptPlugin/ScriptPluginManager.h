/**
 * @file ScriptPluginManager.h
 * @brief interfaces of ScriptPluginManager class
 *
 * @author M.Izumi
 * @date 2013.08.21 
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#ifndef __KOME_SCRIPT_MANAGER_H__
#define __KOME_SCRIPT_MANAGER_H__

namespace kome {
	namespace script {

		class ScriptPluginCall;
	
		class ScriptPluginManager :  public kome::objects::DefaultDataManager {

		protected:
			/**
			 * @fn ScriptPluginManager()
			 * @brief constructor
			 */
			ScriptPluginManager();

			/**
			 * @fn virtual ~ScriptPluginManager()
			 * @brief destructor
			 */
			virtual ~ScriptPluginManager();

		public:
// >>>>>>	@Date:2013/09/11	<Add>	A.Ozaki
// ‹N“®‚ÌƒXƒNƒŠƒvƒg“o˜^‚É³‚µ‚­“o˜^‚Å‚«‚È‚©‚Á‚½Œ‚ğC³
//
			typedef enum {
				TYPE_INITIALIZE = 0,
				TYPE_GUI,
			} ExecutingType;
//
// <<<<<<	@Date:2013/09/11	<Add>	A.Ozaki

		protected:			
			/** script */
			std::string m_strScript;
		public:
			
// >>>>>>	@Date:2013/09/11	<Add>	A.Ozaki
// ‹N“®‚ÌƒXƒNƒŠƒvƒg“o˜^‚É³‚µ‚­“o˜^‚Å‚«‚È‚©‚Á‚½Œ‚ğC³
//
			/**
			 * @fn void addPluginInfo( stScriptInfo stInfo, ExecutingType eType = TYPE_GUI )
			 * @brief add plugin info
			 * @param stInfo stScriptInfo
			 */
			void addPluginInfo( stScriptInfo stInfo, ExecutingType eType = TYPE_GUI );
//
// <<<<<<	@Date:2013/09/11	<Add>	A.Ozaki

			/**
			 * @fn void deletePluginInfo( stScriptInfo stInfo )
			 * @brief
			 */
			void deletePluginInfo( stScriptInfo stInfo );
						
			/**
			 * @fn kome::core::Script* getScript()
			 * @brief gets the script object
			 * @return script object
			 */
			kome::objects::Script* getScript(  kome::plugin::PluginFunctionItem* item );	// @Date:2013/08/06	<Modify>	A.Ozaki
			
			/**
			 * @fn std::string getScriptVal( const char* path )
			 * @brief get the script value
			 * @param path
			 */
			std::string getScriptVal( const char* path );

			/**
			 * @fn ScriptPluginCall* getScriptPluginCall( stScriptInfo stInfo )
			 * @brief get the ScriptPluginCall
			 * @prama stInfo
			 */
			ScriptPluginCall* getScriptPluginCall( stScriptInfo stInfo );

			/**
			 * @fn std::string scriptRun( 
						kome::objects::Script* sc,
						const char* val,
						kome::objects::Parameters* params 
					)
			 * @brief scritp run
			 * @param sc script object
			 * @param val script value
			 * @param params
			 */
			std::string scriptRun( 
				kome::objects::Script* sc,
				const char* val,
				kome::objects::Parameters* params 
			);
				
		public:
			/**
			 * @fn static SampleMergeManager& getInstance()
			 * @brief gets merged manager object
			 * @return merged manager object (This is the only object.)
			 */
			static ScriptPluginManager& getInstance();
		};
	}
}

#endif	// __KOME_SCRIPT_MANAGER_H__

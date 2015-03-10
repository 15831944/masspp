/**
 * @file ScriptPluginCall.h
 * @brief interfaces of ScriptPluginCall class
 *
 * @author M.Izumi
 * @date 2013.08.21 
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#ifndef __KOME_SCRIPT_PLUGINCALL_H__
#define __KOME_SCRIPT_PLUGINCALL_H__

namespace kome {
	namespace script {

		class ScriptPluginCall : public kome::plugin::PluginCall {
		public:
			/**
			 * @fn ScriptPluginCall( kome::plugin::PluginInfo* plugin )
			 * @brief constructor
			 * @param plugin PluginInfo
			 */
			ScriptPluginCall( kome::plugin::PluginInfo* plugin );

			/**
			 * @fn virtual ~ScriptPluginCall( )
			 * @brief destructor
			 */
			virtual ~ScriptPluginCall();
		
		public:
			/**
			 * @fn kome::objects::Variant invoke( kome::objects::Parameters* params )
			 * @brief execute function or method
			 * @param[in] params parameter information
			 * @return return value
			 */
			virtual	kome::objects::Variant invoke( kome::objects::Parameters* params );
		};
	}
}

#endif // __KOME_SCRIPT_PLUGINCALL_H__

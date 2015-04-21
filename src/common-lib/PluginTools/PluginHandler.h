/**
 * @file PluginHandler.h
 * @brief interfaces of PluginHandler class
 *
 * @author S.Tanaka
 * @date 2006.08.10
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_PLUGIN_HANDLER_H__
#define __KOME_PLUGIN_HANDLER_H__

#include <map>

#include "StatusBarInfo.h"	// @date 2011/01/12 <Add> OKADA

/**
 * @typedef popup_menu_info
 * @brief popup menu information
 */
typedef struct{
	std::string strMenuName;
//	PopupType					enPopupTypes;
//	int							nPopupOrders;
//	std::vector< PopupType >	v_enPopupTypes;
	std::vector< int >	v_enPopupTypes;
	std::vector< int >	v_nPopupOrders;
}popup_menu_info;


namespace kome {
	namespace plugin {

		class Machine;
		class PluginInfo;
		class PluginIcon;
		class PluginMenu;
		class PluginCall;
		class OptionArg;
		class PluginHelp;
		class PluginComponent;		// @Date:2013/08/21	<Add>	A.Ozaki

		/**
		 * @class PluginHandler
		 * @brief XML handler class to read plug-in file
		 */
		class PluginHandler : public kome::xml::XmlHandler {
		public:
			/**
			 * @fn PluginHandler( PluginInfo& plugin, PluginMenu& menu )
			 * @brief constructor
			 * @param[in] plugin kome::plugin::Plugin object to store plug-in information
			 * @param[in] menu kome::plugin::Plugin object to store menu information
			 */
			PluginHandler( PluginInfo& plugin, PluginMenu& menu );

			/**
			 * @fn virtual ~PluginHandler()
			 * @brief destructor
			 */
			virtual ~PluginHandler();

		protected:
			/** 
			 * @struct MenuInfo
			 * @brief menu information
			 */
			struct MenuInfo {
				int id;					   // menu id
				char accelerator;		   // menu accelerator
				int order;				   // menu order
				std::string target;		   // target
				std::string chkName;	   // status name
				std::string chkValue;	   // status value
			};

			/**
			 * @struct StatusInfo
			 * @brief status information
			 */
			struct StatusInfo {
				int id;			     // status id
				bool df;			 // default flag
				std::string name;	 // status name
				std::string value;   // status value
			};

			/**
			 * @struct EnumInfo
			 * @brief enumeration information
			 */
			struct EnumInfo {
				std::string value;  // value
				std::string desc;   // description
			};

// >>>>>>	ÅóDate:2013/08/21	<Add>	A.Ozaki
//
		protected:
			/** current processing plugin-component */
			PluginComponent	*m_pCurrentComponent;

			/** plug-in call map */
			std::map< std::string, PluginCall* >	m_mapCall;

			/** plug-in call */
			std::vector< PluginCall* >	m_vecCalls;

			/** current processing package name */
			std::string	m_strCurrentPackageName;			
//
// <<<<<<	@Date:2013/08/21	<Add>	A.Ozaki

		protected:
			/** plug-in information */
			PluginInfo& m_plugin;

			/** plug-in menu */
			PluginMenu& m_menu;

			/** icon map ( icon id -> icon object ) */
			std::map< unsigned int, PluginIcon* > m_iconMap;

			/** menu map ( menu id -> menu object ) */
			std::map< unsigned int, PluginMenu* > m_menuMap;

			std::map< unsigned int, PluginMenu* > m_menuPopupMap;	// @date 2012/11/15 <Add> OKADA
			
			std::map< unsigned int, popup_menu_info > m_menuPopupMap2;

			/** statusMap */
			std::map< unsigned int, std::pair< std::string, std::string > > m_statusMap;

			/** current reading machine object */
			Machine* m_currentMachine;

			/** current reading icon object */
			PluginIcon* m_currentIcon;

			/** current reading menu object */
			PluginMenu* m_currentMenu;

			/** current reading menu object */
			PluginMenu* m_ptr_currentPopupMenuTemp;		

			/** current reading option argument */
			OptionArg* m_currentArg;

			/** current reading help object */
			PluginHelp* m_currentHelp;

			/** menu information */
			MenuInfo m_menuInfo;

			/** status information */
			StatusInfo m_statusInfo;

			/** enumeration information */
			EnumInfo m_enumInfo;

			/** menu group order */
			int m_groupOrder;

			/** menu popup type */
			std::string m_popupType;

			/** menu popup order */
			int m_popupOrder;

			/** toolbar order */
			int m_toolbarOrder;

			/** now reading plug-in call object */
			PluginCall* m_currentCall;

			/** call method information */
			struct {
				bool isStatic;				// static method or not
				std::string methodName;		// method name
				std::string className;		// class name
			} m_methodInfo;

			/** parameter key */
			std::string m_paramKey;

			/** Status Bar */
			StatusBarInfo* m_currentStatusbarInfo;		// @date 2011/01/12 <Add> OKADA
			StatusBarInfo m_tempStatusBarInfo;			// @date 2011/01/18 <Add> OKADA

			/** in java */
			bool m_inJava;

			// Context Menu <menu-item>ÇÃèàóùÇÉ^ÉOÇ™ï¬Ç∂ÇÈéûÇ…çsÇ§ÇÊÇ§à⁄ìÆ

			/** menu information(Temp) */
			MenuInfo m_menuInfoTempMi;

			/** current reading menu object(Temp) */
			PluginMenu* m_currentMenuTempMi;

			/** menu group order(Temp)  */
			int m_groupOrderTempMi;

			/** toolbar order(Temp) */
			int m_toolbarOrderTempMi;

			/** menu map ( menu id -> menu object )(Temp) */
			std::map< unsigned int, PluginMenu* > m_menuMapTempMi;

			// <menu-item>
			/** id menu item */
			int m_nIdMenuItem;						// m_menuInfoTempMi.id

			/** accelerator menu item */
			std::string m_strAcceleratorMenuItem;	// std::string accelerator

			/** parent menu item */
			int m_nParentMenuItem;					// int parent

			/** order menu item */
			int m_nOrderMenuItem;					// m_menuInfoTempMi.order

			/** target menu item */
			std::string m_strTargetMenuItem;		// m_menuInfoTempMi.target

			/** checked id menu item */
			int m_nCheckedIdMenuItem;				// int checkedId 

			// </menu-name> 
			/** text menu name */
			std::string m_strTextMenuName;

			// <menu-group>
			/** order menu group */
			int m_nOrderMenuGroup;	               // m_groupOrder

			// </menu-group>
			/** text menu group */
			std::string m_strTextMenuGroup;

			/** order toolbar */
			int m_nOrderToolbar;

			/** text toolbar */
			std::string m_strTextToolbar;

			/** key shortcut key */
			std::vector< std::string >m_v_strKeyShortcutKey;

			/** type popup */
			std::vector< std::string >m_v_strTypePopup;

			/** order popup */
			std::vector< int >m_v_nOrderPopup;
			
			// </menu-description> 
			/** text menu description */
			std::string m_strTextMenuDescription;

		protected:
			/**
			 * @fn virtual void onStartDocument()
			 * @brief This method is called by startDocument method. (override method)
			 */
			virtual void onStartDocument();

			/**
			 * @fn virtual void onEndDocument()
			 * @brief This method is called by endDocument method. (override method)
			 */
			virtual void onEndDocument();

			/**
			 * @fn virtual void onStartElement( const char* name, kome::core::Properties& attrs )
			 * @brief This method is called by startElement method. (override method)
			 * @param[in] name tag name
			 * @param[in] attrs attributes
			 */
			virtual void onStartElement( const char* name, kome::core::Properties& attrs );

			/**
			 * @fn virtual void onEndElement( const char* name, const char* text )
			 * @brief This method is called by end element method. (override method)
			 * @param[in] name tag name
			 * @param[in] text body text
			 */
			virtual void onEndElement( const char* name, const char* text );			
		};
	}
}

#endif	// __KOME_PLUGIN_HANDLER_H__

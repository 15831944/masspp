/**
 * @file PluginMenuWrapper.h
 * @brief interfaces of PluginMenuWrapper class
 *
 * @author S.Tanaka
 * @date 2006.10.18
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_CLR_PLUGIN_MENU_WRAPPER_H__
#define __KOME_CLR_PLUGIN_MENU_WRAPPER_H__


namespace kome {
	namespace clr {

		ref class PluginCallWrapper;

		/**
		 * @class PluginMenuWrapper
		 * @brief plug-in menu wrapper class to use on CLR environment
		 */
		public ref class PluginMenuWrapper {
		public:
			/**
			 * @fn PluginMenuWrapper( kome::plugin::PluginMenu& menu )
			 * @brief constructor
			 * @param[in] menu plug-in menu object
			 */
			PluginMenuWrapper( kome::plugin::PluginMenu& menu );

			/**
			 * @fn virtual ~PluginMenuWrapper()
			 * @brief destructor
			 */
			virtual ~PluginMenuWrapper();

		public:
			/**
			 * @fn kome::plugin::PluginMenu& getPluginMenu()
			 * @brief gets plug-in menu object
			 * @return plug-in menu object
			 */
			kome::plugin::PluginMenu& getPluginMenu();

		public:
			/** menu target */
			ENUM_CLASS MenuTarget {
				NONE,
				SAMPLE,
				GROUP,
				SPECTRUM,
				CHROMATOGRAM,
				DATAMAP,
				SPEC_CHROM,
				NOT_NULL
			};

		public:
			/**
			 * @fn unsigned int getId()
			 * @brief gets menu ID
			 * @return menu ID
			 */
			unsigned int getId();

			/**
			 * @fn void setName( System::String^ name )
			 * @brief sets menu name
			 * @param[in] name menu name
			 */
			void setName( System::String^ name );

			/** 
			 * @fn System::String^ getName()
			 * @brief gets menu name
			 * @return menu name
			 */
			System::String^ getName();

			/**
			 * @fn System::String^ getAccName()
			 * @brief gets menu name icluding accelerator information
			 * @return menu name icluding accelerator information
			 */
			System::String^ getAccName();

			/**
			 * @fn void setOrder( int order )
			 * @brief sets order
			 * @param[in] order
			 */
			void setOrder( int order );

			/**
			 * @fn int getOrder()
			 * @brief gets order
			 * @return order
			 */
			int getOrder();

			/**
			 * @fn void setAccelerator( char accelerator )
			 * @brief sets acclerator
			 * @param[in] accelerator accelerator ( If accelerator is not alphabet, this menu doesn't have an accelerator )
			 */
			void setAccelerator( char accelerator );

			/**
			 * @fn char getAccelerator()
			 * @brief gets accelerator
			 * @return accelerator ( If this menu doesn't have an accelerator, this method returns 0. )
			 */
			char getAccelerator();

			/**
			* @fn void clearShortcutKey()
			* @brief clears shortcut key
			*/
			void clearShortcutKey();

			/**
			 * @fn void addShortcutKey( int code )
			 * @brief adds shortcut key
			 * @param[in] code shortcut key code
			 */
			void addShortcutKey( int code );

			/**
			 * @fn void addShortcutKey( System::String^ key )
			 * @brief adds shortcut key
			 * @param[in] key shortcut key
			 */
			void addShortcutKey( System::String^ key );

			/**
			 * @fn unsigned int getNumberOfShortcutKeys()
			 * @brief gets the number of shortcut keys
			 * @return the number of shortcut keys
			 */
			unsigned int getNumberOfShortcutKeys();

			/**
			 * @fn int getShortcutKey( unsigned int idx )
			 * @brief gets the shortcut key code
			 * @param[in] idx shortcut key index
			 * @return shortcut key code
			 */
			int getShortcutKey( unsigned int idx );

			/**
			 * @fn System::String^ getShortcutKey()
			 * @brief gets the shortcut key code
			 * @return shortcut key
			 */
			System::String^ getShortcutKey();

			/**
			 * @fn void setGroup( System::String^ name, int order )
			 * @brief sets group information
			 * @param[in] name menu group name
			 * @param[in] order group order
			 */
			void setGroup( System::String^ name, int order );

			/**
			 * @fn System::String^ getGroupName()
			 * @brief gets group name
			 * @return group name ( If this menu doesn't belong to any group, this method returns NULL. )
			 */
			System::String^ getGroupName();

			/**
			 * @fn int getGroupOrder()
			 * @brief returns gets group order
			 * @return group order (If this menu doesn't belongs to a group, this method returns negative value.)
			 */
			int getGroupOrder();

			/**
			 * @fn void setDescription( System::String^ desc )
			 * @brief sets menu description
			 * @param[in] desc menu descritpion
			 */
			void setDescription( System::String^ desc );

			/**
			 * @fn System::String^ getDescription()
			 * @brief gets menu description
			 * @return menu description
			 */
			System::String^ getDescription();

			/**
			 * @fn void setToolbar( System::String^ name, int order )
			 * @brief sets toolbar information
			 * @param[in] name toolbar name
			 * @param[in] order toolbar order
			 */
			void setToolbar( System::String^ name, int order );

			/**
			 * @fn System::String^ getToolbarName()
			 * @brief gets toolbar name
			 * @return toolbar name ( If NULL, any toolbar doesn't have this menu. )
			 */
			System::String^ getToolbarName();

			/**
			 * @fn int getToolbarOrder()
			 * @brief gets toolbar order
			 * @return toolbar order (If negative value, any toolbar doesn't have this menu. );
			 */
			int getToolbarOrder();

			/**
			 * @fn void setTarget( MenuTarget target )
			 * @brief sets menu target
			 * @param[in] target menu target
			 */
			void setTarget( MenuTarget target );

			/**
			 * @fn void setTarget( System::String^ target )
			 * @brief sets menu target
			 * @param[in] target character string meaning target
			 */
			void setTarget( System::String^ target );

			/**
			 * @fn MenuTarget getTarget()
			 * @brief gets menu target
			 * @return menu target
			 */
			MenuTarget getTarget();

			/**
			 * @fn void setCheckedStatus( System::String^ name, System::String^ value )
			 * @brief sets checked status
			 * @param[in] name checked status name
			 * @param[in] value checked status value
			 */
			void setCheckedStatus( System::String^ name, System::String^ value );

			/**
			 * @fn System::String^ getCheckedStatusName()
			 * @brief gets checked status name
			 * @return checked status name
			 */
			System::String^ getCheckedStatusName();

			/**
			 * @fn System::String^ getCheckedStatusValue();
			 * @brief gets checked status value
			 * @return checked status value
			 */
			System::String^ getCheckedStatusValue();

			/**
			 * @fn void setCallExec( PluginCallWrapper^ pluginCall )
			 * @brief sets function information called when execute
			 * @param[in] pluginCall function information called when execute
			 */
			void setCallExec( PluginCallWrapper^ pluginCall );

			/**
			 * @fn PluginCallWrapper^ getCallExec()
			 * @brief gets function information called when execute
			 * @return function information called when execute
			 */
			PluginCallWrapper^ getCallExec();

			/**
			 * @fn void setCallEnable( PluginCallWrapper^ pluginCall )
			 * @brief sets function information called judge its executable
			 * @param[in] pluginCall function information called judge its executable
			 */
			void setCallEnable( PluginCallWrapper^ pluginCall );

			/**
			 * @fn PluginCallWrapper^ getCallEnable()
			 * @brief gets function information called judge its executable
			 * @return function information called judge its executable
			 */
			PluginCallWrapper^ getCallEnable();

			/**
			 * @fn void setCallChecked( PluginCallWrapper^ pluginCall )
			 * @brief sets function information called judge check
			 * @param[in] pluginCall function information called judge check
			 */
			void setCallChecked( PluginCallWrapper^ pluginCall );

			/**
			 * @fn PluginCallWrapper^ getCallChecked()
			 * @brief function information called judge check
			 * @return function information called judge check
			 */
			PluginCallWrapper^ getCallChecked();

			/**
			 * @fn PluginMenuWrapper^ getParentMenu()
			 * @brief gets parent menu
			 * @return parent menu
			 */
			PluginMenuWrapper^ getParentMenu();

			/** 
			 * @fn PluginMenuWrapper^ createChildMenu( System::String^ name )
			 * @brief creates new child menu
			 * @param[in] name child menu name
			  * @return new child menu object.
					   ( If name is illegal or there is a child menu that already has same name, this method returns NULL.)
			 */
			PluginMenuWrapper^ createChildMenu( System::String^ name );

			/**
			 * @fn unsigned int getNumberOfChildren()
			 * @brief gets the number of child menus
			 * @return the number of child menus
			 */
			unsigned int getNumberOfChildren();

			/**
			 * @fn PluginMenuWrapper^ getChildMenu( unsigned int index )
			 * @brief gets child menu
			 * @param[in] index child menu index
			 * @return child menu object ( If there is no child menu that has index specified, this method returns NULL )
			 */
			PluginMenuWrapper^ getChildMenu( unsigned int index );

			/**
			 * @fn PluginMenuWrapper^ getChildMenu( System::String^ name )
			 * @brief gets child menu
			 * @param[in] name child menu name
			 * @return chile menu object ( If there is no child menu that has name specified, this method returns NULL )
			 */
			PluginMenuWrapper^ getChildMenu( System::String^ name );

			/**
			 * @fn bool execute( ClrVariant^ acObj )
			 * @brief execute menu command
			 * @param[in] acObj active object
			 * @return If the command execution ends successfully, this method returns true.
			 */
			bool execute( ClrVariant^ acObj );

			/**
			 * @fn bool isEnabled( ClrVariant^ acObj )
			 * @brief judges whether it is able to execute menu command
			 * @param[in] acObj active object
			 * @return If it is able to execute menu command, this method returns true.
			 */
			bool isEnabled( ClrVariant^ acObj );

			/**
			 * @fn bool isChecked( ClrVariant^ acObj )
			 * @brief judges whether the menu item is checked
			 * @param[in] acObj active object
			 * @return If the menu item is checked, this method returns true.
			 */
			bool isChecked( ClrVariant^ acObj );

			/**
			 * @fn bool isCheckable()
			 * @brief judges whether the menu item is checkable
			 * @return If the menu item is checkable, this method returns true.
			 */
			bool isCheckable();

			/** 
			 * @fn void sortMenu()
			 * @brief sorts menu
			 */
			void sortMenu();

		protected:
			/** plug-in menu object */
			kome::plugin::PluginMenu& m_menu;
		};
	}
}

#endif	// __KOME_CLR_PLUGIN_MENU_WRAPPER_H__

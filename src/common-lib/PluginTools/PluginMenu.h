/**
 * @file PluginMenu.h
 * @brief interfaces of PluginMenu class
 *
 * @author S.Tanaka
 * @date 2006.08.10
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_PLUGIN_MENU_H__
#define __KOME_PLUGIN_MENU_H__


#include <string>
#include <vector>
#include <map>


#define DEFAULT_ORDER	 5

/** Context menu */
enum PopupType{
	POPUP_NONE,
	POPUP_SPEC_ITEM,
	POPUP_CHROM_ITEM,
	POPUP_GROUP_ITEM,
	POPUP_SPEC_VIEW,
	POPUP_SPEC_SUB_VIEW,
	POPUP_CHROM_VIEW,
	POPUP_CHROM_SUB_VIEW,
	POPUP_DATAMAP_VIEW,
	POPUP_DATAMAP_SUB_VIEW,
};


/** PopupType */
typedef struct{
	PopupType					enPopupTypes;
	int							nPopupOrders;
	std::string					strName;
	void*						ptr_pluginCall;
	void*						ptr_parent;
	std::vector< void* >*		ptr_children;
}popup_info;



namespace kome {
	namespace plugin {

		class PluginCall;

		/**
		 * @class PluginMenu
		 * @brief menu information management class
		 */
		class PLUGIN_TOOLS_CLASS PluginMenu {
		public:
			/**
			 * @fn PluginMenu()
			 * @brief constructor
			 */
			PluginMenu();

			/**
			 * @fn virtual ~PluginMenu()
			 * @brief destructor
			 */
			virtual ~PluginMenu();

		public:
			/**
			 * @typedef enum MenuTarget
			 * @brief menu target type
			 */
			typedef enum{
				TARGET_NONE,
				TARGET_SAMPLE,
				TARGET_GROUP,
				TARGET_SPECTRUM,
				TARGET_CHROMATOGRAM,
				TARGET_DATAMAP,
				TARGET_SPEC_CHROM,
				TARGET_NOT_NULL,
				TARGET_TAB_SAMPLES,
				TARGET_SPECTRA,
				TARGET_TAB_SAMPLE
			} MenuTarget;

		protected:
			/**
			 * @struct GroupOrder
			 * @brief menu group order
			 */
			struct GroupOrder {
				PluginMenu* parent;
				std::string name;
				int order;
			};

			/** ID */
			unsigned int m_id;

			/** parent menu */
			PluginMenu* m_parent;

			/** menu name */
			std::string m_name;

			/** menu name including accelerator information */
			std::string m_accName;

			/** order */
			int m_order;

			/** accelerator */
			char m_accelerator;

			/** group name */
			std::string m_groupName;

			/** group order */
			int m_groupOrder;

			/** tool bar name */
			std::string m_toolbarName;

			/** description */
			std::string m_description;

			/** the array of child menus */
			std::vector< PluginMenu* >* m_children;

			/** menu target */
			MenuTarget m_target;

			/** checked status name */
			std::string m_checkedStatusName;

			/** checked status value */
			std::string m_checkedStatusValue;

			/** function information called when execute */
			PluginCall* m_execCall;

			/** function information called when judge its executable */
			PluginCall* m_enableCall;

			/** function information called when judge check */
			PluginCall* m_checkedCall;

			/** short cut key */
			std::vector< int > m_shortcutKeys;

		protected:
			/** menu group order array */
			static std::vector< GroupOrder > m_groupOrders;

			/** toolbar order array */
			static std::map< std::string, int > m_toolbarOrderMap;

		public:
			/**
			 * @fn void setId( const unsigned int id )
			 * @brief sets menu ID
			 * @param[in] id menu ID
			 */
			void setId( const unsigned int id );

			/**
			 * @fn unsigned int getId()
			 * @brief gets menu ID
			 * @return menu ID
			 */
			unsigned int getId();

			/**
			 * @fn void setName( const char* name )
			 * @brief sets menu name
			 * @param[in] name menu name
			 */
			void setName( const char* name );

			/** 
			 * @fn const char* getName()
			 * @brief gets menu name
			 * @return menu name
			 */
			const char* getName();

			/**
			 * @fn const char* getAccName()
			 * @brief gets menu name icluding accelerator information
			 * @return menu name icluding accelerator information
			 */
			const char* getAccName();

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
			 * @fn void setAccelerator( const char accelerator )
			 * @brief sets acclerator
			 * @param[in] accelerator accelerator ( If accelerator is not alphabet, this menu doesn't have an accelerator )
			 */
			void setAccelerator( const char accelerator );

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
			 * @fn void addShortcutKey( const int code )
			 * @brief adds shortcut key
			 * @param[in] code shortcut key code
			 */
			void addShortcutKey( const int code );

			/**
			 * @fn void addShortcutKey( const char* key )
			 * @brief adds short cut key
			 * @param[in] key short cut key
			 */
			void addShortcutKey( const char* key );

			/**
			 * @fn unsigned int getNumberOfShortcutKeys()
			 * @brief gets the number of shortcut keys
			 * @return the number of shortcut keys
			 */
			unsigned int getNumberOfShortcutKeys();

			/**
			 * @fn int getShortcutKey( const unsigned int idx )
			 * @brief gets the shortcut key code
			 * @param[in] idx shortcut key index
			 * @return shortcut key code
			 */
			int getShortcutKey( const unsigned int idx );

			/**
			 * @fn std::string getShortcutKey()
			 * @brief gets the short cut key
			 * @return short cut key
			 */
			std::string getShortcutKey();

			/**
			 * @fn void setGroup( const char* name, int order = -1 )
			 * @brief sets group information
			 * @param[in] name menu group name
			 * @param[in] order group order
			 */
			void setGroup( const char* name, int order = -1 );

			/**
			 * @fn const char* getGroupName()
			 * @brief gets group name
			 * @return group name ( If this menu doesn't belong to any group, this method returns NULL. )
			 */
			const char* getGroupName();

			/**
			 * @fn int getGroupOrder()
			 * @brief returns gets group order
			 * @return group order ( If this menu doesn't belong to any group, this method returns negative value. )
			 */
			int getGroupOrder();

			/**
			 * @fn void setToolbar( const char* name, int order = -1 )
			 * @brief sets toolbar information
			 * @param[in] name toolbar name
			 * @param[in] order toolbar order
			 */
			void setToolbar( const char* name, int order = -1 );

			/**
			 * @fn const char* getToolbarName()
			 * @brief gets toolbar name
			 * @return toolbar name ( If NULL, any toolbar doesn't have this menu. )
			 */
			const char* getToolbarName();

			/**
			 * @fn int getToolbarOrder()
			 * @brief gets toolbar order
			 * @return toolbar order (If negative value, any toolbar doesn't have this menu. );
			 */
			int getToolbarOrder();

			/**
			 * @fn void setDescription( const char* desc )
			 * @brief sets menu description
			 * @param[in] desc menu descritpion
			 */
			void setDescription( const char* desc );

			/**
			 * @fn const char* getDescription()
			 * @brief gets menu description
			 * @return menu description
			 */
			const char* getDescription();

			/**
			 * @fn void setTarget( const MenuTarget target )
			 * @brief sets menu target
			 * @param[in] target menu target
			 */
			void setTarget( const MenuTarget target );

			/**
			 * @fn void setTarget( const char* target )
			 * @brief sets menu target
			 * @param[in] target character string meaning target
			 */
			void setTarget( const char* target );

			/**
			 * @fn MenuTarget getTarget()
			 * @brief gets menu target
			 * @return menu target
			 */
			MenuTarget getTarget();

			/**
			 * @fn void setCheckedStatus( const char* name, const char* value )
			 * @brief sets checked status
			 * @param[in] name checked status name
			 * @param[in] value checked status value
			 */
			void setCheckedStatus( const char* name, const char* value );

			/**
			 * @fn const char* getCheckedStatusName()
			 * @brief gets checked status name
			 * @return checked status name
			 */
			const char* getCheckedStatusName();

			/**
			 * @fn const char* getCheckedStatusValue();
			 * @brief gets checked status value
			 * @return checked status value
			 */
			const char* getCheckedStatusValue();

			/**
			 * @fn void setCallExec( PluginCall* pluginCall )
			 * @brief sets function information called when execute
			 * @param[in] pluginCall function information called when execute
			 */
			void setCallExec( PluginCall* pluginCall );

			/**
			 * @fn PluginCall* getCallExec()
			 * @brief gets function information called when execute
			 * @return function information called when execute
			 */
			PluginCall* getCallExec();

			/**
			 * @fn void setCallEnable( PluginCall* pluginCall )
			 * @brief sets function information called judge its executable
			 * @param[in] pluginCall function information called judge its executable
			 */
			void setCallEnable( PluginCall* pluginCall );

			/**
			 * @fn PluginCall* getCallEnable()
			 * @brief gets function information called judge its executable
			 * @return function information called judge its executable
			 */
			PluginCall* getCallEnable();

			/**
			 * @fn void setCallChecked( PluginCall* pluginCall )
			 * @brief sets function information called judge check
			 * @param[in] pluginCall function information called judge check
			 */
			void setCallChecked( PluginCall* pluginCall );

			/**
			 * @fn PluginCall* getCallChecked()
			 * @brief function information called judge check
			 * @return function information called judge check
			 */
			PluginCall* getCallChecked();

			/**
			 * @fn PluginMenu* getParentMenu()
			 * @brief gets parent menu
			 * @return parent menu
			 */
			PluginMenu* getParentMenu();

			/** 
			 * @fn PluginMenu* createChildMenu( const char* name )
			 * @brief creates new child menu
			 * @param[in] name child menu name
			  * @return new child menu object.
					   ( If name is illegal or there is a child menu that already has same name, this method returns NULL.)
			 */
			PluginMenu* createChildMenu( const char* name );

			/**
			 * @fn unsigned int getNumberOfChildren()
			 * @brief gets the number of child menus
			 * @return the number of child menus
			 */
			unsigned int getNumberOfChildren();

			/**
			 * @fn PluginMenu* getChildMenu( unsigned int index )
			 * @brief gets child menu
			 * @param[in] index child menu index
			 * @return child menu object ( If there is no child menu that has index specified, this method returns NULL )
			 */
			PluginMenu* getChildMenu( unsigned int index );

			/**
			 * @fn PluginMenu* getChildMenu( const char* name )
			 * @brief gets child menu
			 * @param[in] name child menu name
			 * @return chile menu object ( If there is no child menu that has name specified, this method returns NULL )
			 */
			PluginMenu* getChildMenu( const char* name );

			/**
			 * @fn bool execute( kome::objects::Variant& acObj )
			 * @brief execute menu command
			 * @param[in] acObj active object
			 * @return If the command execution ends successfully, this method returns true.
			 */
			bool execute( kome::objects::Variant& acObj );

			/**
			 * @fn bool isEnabled( kome::objects::Variant& acObj )
			 * @brief judges whether it is able to execute menu command
			 * @param[in] acObj active object
			 * @return If it is able to execute menu command, this method returns true.
			 */
			bool isEnabled( kome::objects::Variant& acObj );

			/**
			 * @fn bool isChecked( kome::objects::Variant& acObj )
			 * @brief judges whether the menu item is checked
			 * @param[in] acObj active object
			 * @return If the menu item is checked, this method returns true.
			 */
			bool isChecked( kome::objects::Variant& acObj );

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
			/**
			 * @fn void setParentMenu( PluginMenu* parent )
			 * @brief sets parent menu
			 * @param[in] parent parent menu
			 */
			void setParentMenu( PluginMenu* parent );

			/**
			 * @fn void setAccName( const char* name )
			 * @brief sets menu name including accelerator information. This method called in sort method.
			 * @param[in] name menu name including accelerator information
			 */
			void setAccName( const char* name );

			/**
			 * @fn static bool lessMenu( PluginMenu* menu0, PluginMenu* menu1 )
			 * @brief compares to sort
			 * @param[in] menu0 menu object to compare
			 * @param[in] menu1 menu object to be compared
			 * @return If menu0 sorts before menu1, this method returns true
			 */
			static bool lessMenu( PluginMenu* menu0, PluginMenu* menu1 );

		// context menu
		private:
			/** Popup Type */
			std::vector<PopupType>	m_vectPopupTypes;

			/** Popup Orders */
			std::vector< int > m_vectPopupOrders;

			/** Popup Types */
			PopupType	m_ptPopupTypes;

			/** Popup Orders */
			int			m_nPopupOrders;
			
		public:
			/**
			 * @fn void addPopupTypeOrder( PopupType type, int order )
			 * @brief adds popup menu type
			 * @param[in] type type
			 * @param[in] order order
			 */
			void addPopupTypeOrder( PopupType type, int order );

			/**
			 * @fn PopupType getPopupType( int index )
			 * @brief gets popup type
			 * @param[in] index
			 * @return popup type
			 */
			PopupType getPopupType( int index );

			/**
			 * @fn int getPopupOrder( int index )
			 * @brief gets popup order.
			 * @param[in] index
			 * @return popup order
			 */
			int getPopupOrder( int index );

			/**
			 * @fn int getNumberOfPopup( void )
			 * @brief gets the number of popup menus
			 * @return the number of popup menus.
			 */
			int getNumberOfPopup( void );

		};
	}
}

#endif	// __KOME_PLUGIN_MENU_H__

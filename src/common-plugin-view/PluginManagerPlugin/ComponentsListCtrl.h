/**
 * @file ComponentsListCtrl.h
 * @brief interfaces of ComponentListCtrl class
 *
 * @author OKADA, H
 * @date 2013/08/22
 * 
 * Copyright(C) 2013 Shimadzu Corporation. All rights reserved.
 */

#ifndef __KOME_PLUGINMANAGER_VIEW_COMPONENTS_LIST_CTRL_H__
#define __KOME_PLUGINMANAGER_VIEW_COMPONENTS_LIST_CTRL_H__


typedef struct tag_components {	
	int nPluginIndex;			// Plugin Index(PluginManager::getPlugin(index)Å©ÇÃindexî‘çÜ)

	std::string strName;			// Function Name

	unsigned int uCompornentIndex;	// Compornent Index(PluginInfo->getCall(index)Å©ÇÃindexî‘çÜ)
	std::string strType;			// Function Type
	std::string strDescription;		// Compornent Description
	bool flgActive;					// true:selected false:not select
} COMPONENTS, *LPCOMPONENTS ;


namespace kome{
	namespace pluginmanager{
		namespace view{
			
			/**
			 * @class ComponentsListCtrl
			 * @brief peak position list control class
			 */
			class ComponentsListCtrl : public kome::window::CheckGridListCtrl {
			public:
				/**
				 * @fn ComponentsListCtrl( wxWindow* parent, const int width, const int height )
				 * @brief constructor
				 * @param parent parent window
				 * @param width width
				 * @param height height
				 */
				ComponentsListCtrl( wxWindow* parent, const int width, const int height );

				/**
				 * @fn virtual ~ComponentsListCtrl()
				 * @brief destructor
				 */
				virtual ~ComponentsListCtrl();

			protected:

				/** call column */
				int m_callName;		// @date 2013/08/22 <Add> OKADA

				/** path description */
				int m_descriptionCol;

				/** appended base column */
				int m_appendedCol;

			protected:

				/** functions */
				std::vector< COMPONENTS > m_vecComponents;

			public:
				/**
				 * @fn unsigned int getNumberOfFunctions()
				 * @brief gets the number of Functions
				 * @return the number of Functions
				 */
				unsigned int getNumberOfFunctions();

				/**
				 * @fn FUNCTIONS getComponent( const unsigned int idx )
				 * @brief gets the Function
				 * @param[in] idx Function index
				 * @return Function
				 */
				COMPONENTS getComponent( const unsigned int idx );

				/**
				 * @fn void clearComponents()
				 * @brief clears functions
				 */
				void clearComponents();

				/**
				 * @fn void addComponents( FUNCTIONS functions );
				 * @brief adds function
				 * @param[in] functions functions
				 */
				void addComponents( COMPONENTS functions );

			protected:
				/**
				 * @fn virtual void onCreate()
				 * @brief This method is called by create method
				 */
				virtual void onCreate();

				/**
				 * @fn virtual int getInt( const long long data, const int column ) const
				 * @brief gets integer value from data (override method)
				 * @param data data
				 * @param column column number
				 * @return integer value
				 */
				virtual int getInt( const long long data, const int column ) const;

				/**
				 * @fn virtual double getDouble( const long long data, const int column ) const
				 * @brief gets double value from data
				 * @param data data
				 * @param column column number
				 */
				virtual double getDouble( const long long data, const int column ) const;

				/**
				 * @fn virtual std::string getString( const long long data, const int column ) const
				 * @brief gets character string value (override method)
				 * @param data data
				 * @param column column number
				 */
				virtual std::string getString( const long long data, const int column ) const;

				/**
				 * @fn virtual void onSelectItem( const int item );
				 * @brief This method is called when a item is selected.
				 * @param item index
				 */
				virtual void onSelectItem( const int item );

				/**
				 * @fn virtual void onSelectAll()
				 * @brief This method is called when a all item is selected.
				 */
				virtual void onSelectAll();

				/**
				 * @fn virtual void onDeselectAll()
				 * @brief This method is called when a all item is deselected.
				 */
				virtual void onDeselectAll();

			public:
				/**
				 * @fn void onSizePluginMgr( wxSizeEvent& evt )
				 * @brief This method is called when list size is changed
				 * @param[in] evt size event information
				 */
				void onSizePluginMgr( wxSizeEvent& evt );

			private:
				DECLARE_EVENT_TABLE();

			};			
			
		}
	}
}

#endif

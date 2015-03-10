/**
 * @file FunctionListCtrl.h
 * @brief interfaces of FunctionListCtrl class
 *
 * @author OKADA, H
 * @date 2013/02/22
 * 
 * Copyright(C) 2013 Shimadzu Corporation. All rights reserved.
 */

#ifndef __KOME_PLUGINMANAGER_VIEW_FUNCTION_LIST_CTRL_H__
#define __KOME_PLUGINMANAGER_VIEW_FUNCTION_LIST_CTRL_H__


typedef struct tag_functions {	
	int nPluginIndex;			// Plugin Index(PluginManager::getPlugin(index)Å©ÇÃindexî‘çÜ)

	std::string strName;		// Function Name
	unsigned int uFunctionIndex;// Function Index(PluginInfo->getCall(index)Å©ÇÃindexî‘çÜ)
	std::string strType;		// Function Type
	std::string strDescription;	// Function Description
	bool flgActive;				// true:selected false:not select
} FUNCTIONS, *LPFUNCTIONS ;


namespace kome{
	namespace pluginmanager{
		namespace view{
			
			/**
			 * @class FunctionsListCtrl
			 * @brief peak position list control class
			 */
			class FunctionsListCtrl : public kome::window::CheckGridListCtrl {
			public:
				/**
				 * @fn FunctionsListCtrl( wxWindow* parent, const int width, const int height )
				 * @brief constructor
				 * @param parent parent window
				 * @param width width
				 * @param height height
				 */
				FunctionsListCtrl( wxWindow* parent, const int width, const int height );

				/**
				 * @fn virtual ~FunctionsListCtrl()
				 * @brief destructor
				 */
				virtual ~FunctionsListCtrl();

				/** displayed plugins */

			protected:

				/** call column */
				int m_callCol;

				/** type column */
				int m_typeCol;

				/** path description */
				int m_descriptionCol;

				/** appended base column */
				int m_appendedCol;

			protected:

				/** functions */
				std::vector< FUNCTIONS > m_vecFunctions;

			public:
				/**
				 * @fn unsigned int getNumberOfFunctions()
				 * @brief gets the number of Functions
				 * @return the number of Functions
				 */
				unsigned int getNumberOfFunctions();

				/**
				 * @fn FUNCTIONS getFunction( const unsigned int idx )
				 * @brief gets the Function
				 * @param[in] idx Function index
				 * @return Function
				 */
				FUNCTIONS getFunction( const unsigned int idx );

				/**
				 * @fn void clearFunctions()
				 * @brief clears functions
				 */
				void clearFunctions();

				/**
				 * @fn void addSpectrum( kome::objects::Spectrum* spec )
				 * @brief adds spectrum
				 * @param[in] spec spectrum
				 */
				void addSpectrum( kome::objects::Spectrum* spec );

				/**
				 * @fn void addFunctions( FUNCTIONS functions );
				 * @brief adds function
				 * @param[in] functions functions
				 */
				void addFunctions( FUNCTIONS functions );

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
				void onSizePluginMgr( wxSizeEvent& evt );	// @date 2013/05/13 <Add> OKADA

			private:
				DECLARE_EVENT_TABLE();

			};			
			
		}
	}
}

#endif

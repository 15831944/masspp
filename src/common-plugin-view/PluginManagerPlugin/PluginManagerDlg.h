/**
 * @file PluginManagerDlg.h
 * @brief interfaces of PluginManagerDlg class
 *
 * @author OKADA, H
 * @date 2013/02/22
 * 
 * Copyright(C) 2013 Shimadzu Corporation. All rights reserved.
 */

#ifndef __KOME_PLUGINMANAGER_VIEW_PLUGINMANAGER_DIALOG_H__
#define __KOME_PLUGINMANAGER_VIEW_PLUGINMANAGER_DIALOG_H__

#include "PluginManagerDlgBase.h"

namespace kome{
	namespace pluginmanager{
		namespace view{
			
			class PluginManagerDlgBase;
			class PluginsListCtrl;

			class ComponentsListCtrl;

			/**
			 * @class IdentificationDialog
			 * @brief auto labeling dialog class
			 */
			class PluginManagerDlg : public PluginManagerDlgBase {
			public:
				/**
				 * @fn IdentificationDialog(
				 * @brief constructor
				 * @param[in] parent parent window
				 * @param[in] acSample active sample
				 */
				PluginManagerDlg( wxWindow* parent, void* ptr );
			
				/**
				 * @fn virtual ~IdentificationDialog()
				 * @brief destructor
				 */
				virtual ~PluginManagerDlg();
			private:
				/** parent */
				wxWindow* m_parent;

				/** list control */
				PluginsListCtrl* m_listPlugins;

				/** list control */
				ComponentsListCtrl* m_listComponents;

				/** Id of PluginsList */
				wxWindowID m_wxIdPluginsList;

				/** window size height */
				int m_nHeight;

				/** window size width */
				int m_nWidth;
			public:
				/**
				 * @fn wxSizer* createPluginsSizer()
				 * @brief create plugins sizer
				 * @return wxSizer
				 */
				wxSizer* createPluginsSizer();

				/**
				 * @fn void onInitPackage( void* ptr )
				 * @brief on init package
				 * @param[in] ptr pointer
				 */
				void onInitPackage( void* ptr );

				/**
				 * @fn int selectPackage( int index )
				 * @brief select package
				 * @param[in] idx package index
				 * @return 0 success
				 */
				int selectPackage( int index );

				/**
				 * @fn int selectPackage( const char* pszPackage )
				 * @brief select package
				 * @param[in] pszPackage package name
				 * @return 0:success
				 */
				int selectPackage( const char* pszPackage );

				/**
				 * @fn void onSelectPackageEvt( wxCommandEvent& evt )
				 * @brief on select package event
				 * @param[in] evt wxCommandEvent
				 */
				void onSelectPackageEvt( wxCommandEvent& evt );

				/**
				 * @fn void onSelectPackage( void )
				 * @brief on select package
				 */
				void onSelectPackage( void );

				/**
				 * @fn wxSizer* createFunctionsSizer()
				 * @brief create functions sizer
				 * @return functions sizer
				 */
				wxSizer* createFunctionsSizer();

				/**
				 * @fn void onInitFunction( void* ptr )
				 * @brief initialize
				 * @param[in] pointer
				 */
				void onInitFunction( void* ptr );

				/**
				 * @fn void onSelectPlugin( void )
				 * @brief on select plugin
				 */
				void onSelectPlugin( void );

				/**
				 * @fn wxWindowID getWindowIdPluginsList()
				 * @brief get window id plugins list
				 * @return wxWindowID
				 */
				wxWindowID getWindowIdPluginsList();

				/**
				 * @fn std::string getSelectedPackage()
				 * @brief get selected package
				 * @return selected package name
				 */
				std::string getSelectedPackage();

				/**
				 * @fn void onApplyButton( wxCommandEvent& evt )
				 * @brief on apply button
				 * @param[in] evt
				 */
				void onApplyButton( wxCommandEvent& evt );

				/**
				 * @fn void onCloseButton( wxCommandEvent& evt )
				 * @brief on close button
				 * @param[in] evt
				 */
				void onCloseButton( wxCommandEvent& evt );

			protected:
				/**
				 * @fn void onSize( wxSizeEvent& evt )
				 * @brief This method is called when list size is changed
				 * @param[in] evt size event information
				 */
				void onSize( wxSizeEvent& evt );	// @date 2013/05/13 <Add> OKADA		

				private:
					DECLARE_EVENT_TABLE();
			};
		}
	}
}

#endif

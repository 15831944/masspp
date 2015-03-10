/**
 * @file PluginManagerDlgBase.h
 * @brief interfaces of PluginManagerDlgBase class
 *
 * @author OKADA, H
 * @date 2013/02/22
 * 
 * Copyright(C) 2013 Shimadzu Corporation. All rights reserved.
 */

#ifndef __KOME_PLUGINMANAGER_VIEW_PLUGINMANAGER_DIALOG_BASE_H__
#define __KOME_PLUGINMANAGER_VIEW_PLUGINMANAGER_DIALOG_BASE_H__


#define ID_APPLY_BUTTON		2010
#define ID_CLOSE_BUTTON		2011
#define ID_COMBOBOX_PACKAGE	2012

namespace kome{
	namespace pluginmanager{
		namespace view{
			
			/**
			 * @class IdentificationDialogBase
			 * @brief auto labeling dialog class
			 */
			class PluginManagerDlgBase : public kome::window::ParametersAndHelpDialog{	// @Date:2014/01/24	<extend help>	A.Ozaki


			public:
				/**
				 * @fn PluginManagerDlgBase( wxWindow* pParent, const char* title )
				 * @brief constructor
				 * @param[in] parent parent window
				 * @parma[in] title dialog title
				 */
				PluginManagerDlgBase( wxWindow* pParent, const char* title );

				/**
				 * @fn virtual ~PluginManagerDlgBase()
				 * @brief destructor
				 */
				virtual ~PluginManagerDlgBase();
						
			protected:
				/** parameters */
				kome::plugin::ParameterSettings m_params;

				/** combo box */
				wxComboBox* m_comboPackage;

				/** Selected Package Name */
				std::string m_strSelectedPackageName;

				/** parent */
				wxWindow* m_pParent;


			protected:
				/**
				 * @fn virtual wxSizer* createMainSizer()
				 * @brief creates main sizer (override method)
				 * @return main sizer
				 */
				virtual wxSizer* createMainSizer();

			protected:
				/**
				 * @fn virtual wxSizer* createPackageSizer()
				 * @brief creates package sizer
				 * @return package sizer
				 */
				virtual wxSizer* createPackageSizer();

				/**
				 * @fn virtual wxSizer* createPluginsSizer()
				 * @brief creates plugin sizer
				 * @return plugin sizer
				 */
				virtual wxSizer* createPluginsSizer();

				/**
				 * @fn virtual wxSizer* createFunctionsSizer()
				 * @brief creates function sizer
				 * @return function sizer
				 */
				virtual wxSizer* createFunctionsSizer();

				/**
				 * @fn virtual wxSizer* createButtonSizer()
				 * @brief creates function sizer
				 * @return function sizer
				 */
				virtual wxSizer* createButtonSizer();

				/**
				 * @fn virtual void onApplyButton( wxCommandEvent& evt )
				 * @brief on apply button
				 * @param[in] evt
				 */
				virtual void onApplyButton( wxCommandEvent& evt );

				/**
				 * @fn virtual void onCloseButton( wxCommandEvent& evt )
				 * @brief on close button
				 * @param[in] evt
				 */
				virtual void onCloseButton( wxCommandEvent& evt );

				/**
				 * @fn virtual void onSelectPackageEvt( wxCommandEvent& evt )
				 * @brief on select package
				 * @param[in] evt
				 */
				virtual void onSelectPackageEvt( wxCommandEvent& evt );

			public:
				/**
				 * @fn std::string getSelectedPackage()
				 * @brief get selected package
				 * @return selected package
				 */
				std::string getSelectedPackage();


			private:
				DECLARE_EVENT_TABLE();

			};
		}
	}
}

#endif


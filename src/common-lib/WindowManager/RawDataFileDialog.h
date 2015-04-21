/**
 * @file RawDataFileDialog.h
 * @brief interfaces of RawDataFileDialog class
 *
 * @author S.Tanaka
 * @date 2009.07.03
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_WINDOW_RAW_DATA_FILE_DIALOG_H__
#define __KOME_WINDOW_RAW_DATA_FILE_DIALOG_H__


#include "StandardDialog.h"

#include <wx/wx.h>
#include <wx/dirctrl.h>
#include <wx/generic/dirctrlg.h>
#include <wx/generic/filedlgg.h>
#include <wx/filepicker.h>


namespace kome {
	namespace window {

		/**
		 * @class RawDataFileDialog
		 * @brief file dialog to open MS raw data
		 */
		class WINDOW_MANAGER_CLASS RawDataFileDialog : public StandardDialog {
		public:
			/**
			 * @fn RawDataFileDialog( wxWindow* parent, const bool singleSelect = false )
			 * @brief constructor
			 * @param[in] parent parent window
			 * @param[in] singleSelect If true, only one raw data file is selected.
			 */
			RawDataFileDialog( wxWindow* parent, const bool singleSelect = false );

			/**
			 * @fn virtual ~RawDataFileDialog()
			 * @brief destructor
			 */
			virtual ~RawDataFileDialog();

		protected:
			/** directory select control */
			wxGenericDirCtrl* m_dirCtrl;

			/** file select control */
			wxListCtrl* m_fileCtrl;

			/** text */
			wxTextCtrl* m_text;

			/** filter combo */
			wxComboBox* m_filterCombo;

			/** error flag */
			bool m_errorFlg;

		protected:
			/**
			 * @struct FileType
			 * @brief file type
			 */
			struct FileType {
				std::string name;
				std::vector< std::string > extensions;
				std::set< std::string > extSet;
			};

			/** file types */
			std::vector< FileType > m_types;

		protected:
			/** selected directory */
			std::string m_dir;

			/** paths */
			std::vector< std::string > m_paths;

			/** single select flag */
			bool m_singleSelect;

		public:
			/**
			 * @fn unsigned int getNumberOfPaths()
			 * @brief gets the number of paths
			 * @return the number of paths
			 */ 
			unsigned int getNumberOfPaths();

			/**
			 * @fn const char* getPath( const unsigned int idx = 0 )
			 * @brief gets file path
			 * @param[in] idx path index
			 * @return file path (If NULL, the index is illegal.)
			 */
			const char* getPath( const unsigned int idx = 0 );

		protected:
			/**
			 * @fn void getFileTypes()
			 * @brief gets file types
			 */
			void getFileTypes();

		protected:
			/**
			 * @fn void onSelectDir( const char* dir )
			 * @brief on select directory
			 * @param[in] dir selected directory
			 */
			void onSelectDir( const char* dir );

			/**
			 * @fn void applyPaths( const bool dirChange )
			 * @brief applies paths
			 * @param[in] dirChange If true, changes currenct directory when a folder selected.
			 */
			void applyPaths( const bool dirChange );

		protected:
			/**
			 * @fn wxSizer* createFileSizer()
			 * @brief creates file sizer
			 * @return file sizer
			 */
			wxSizer* createFileSizer();

			/**
			 * @fn wxSizer* createInputSizer()
			 * @brief creates input sizer
			 * @return input sizer
			 */
			wxSizer* createInputSizer();

		protected:
			/**
			 * @fn virtual wxSizer* createMainSizer()
			 * @brief creates main sizer (override method)
			 * @return main sizer
			 */
			virtual wxSizer* createMainSizer();

			/**
			 * @fn virtual wxSizer* createButtonSizer()
			 * @brief creates button sizer (override method)
			 * @return button sizer
			 */
			virtual wxSizer* createButtonSizer();

		protected:
			/**
			 * @fn void onChangeDir( wxTreeEvent& evt )
			 * @brief This method is called when directory is changed
			 * @param[in] evt event information
			 */
			void onChangeDir( wxTreeEvent& evt );

			/**
			 * @fn void onSelectIcon( wxListEvent& evt )
			 * @brief This method is called when the selection of files is changed.
			 * @param[in] evt event information
			 */
			void onSelectIcon( wxListEvent& evt );

			/**
			 * @fn void onDoubleClickIcon( wxListEvent& evt )
			 * @brief This method is called when a file icon is double-clicked.
			 * @param[in] evt event information
			 */
			void onDoubleClickIcon( wxListEvent& evt );

			/**
			 * @fn void onSelectFilter( wxCommandEvent& evt )
			 * @brief This method is called when the filter is changed
			 * @param[in] evt event information
			 */
			void onSelectFilter( wxCommandEvent& evt );

			/**
			 * @fn void onOK( wxCommandEvent& evt )
			 * @brief This method is called when the "OK" button is clicked.
			 * @param[in] evt event information
			 */
			void onOK( wxCommandEvent& evt );

		public:
			/**
			 * @fn static std::string getDefaultDir()
			 * @brief gets the default directory
			 * @return default directory
			 */
			static std::string getDefaultDir();

			/**
			 * @fn static std::string getWildCard()
			 * @brief get the wild card string
			 * @return wild card string
			 */
			static std::string getWildCard();

		private:
			DECLARE_EVENT_TABLE()
		};
	}
}


#endif		// __KOME_WINDOW_RAW_DATA_FILE_DIALOG_H__

/**
 * @file BatchProcessingMethodConfigurationDialog.h
 * @brief interfaces of BatchProcessingMethodConfigurationDialog class
 *
 * @author OKADA, H
 * @date 2012-02-30
 * 
 * Copyright(C) 2012 Shimadzu Corporation. All rights reserved.
 */


#ifndef __KOME_BATCH_SERVICE_BATCH_PROCESSING_METHOD_CONFIGURATION_DIALOG_H__
#define __KOME_BATCH_SERVICE_BATCH_PROCESSING_METHOD_CONFIGURATION_DIALOG_H__


#include "BatchServicePluginManager.h"
#include "BatchGrid.h"
#include "BatchActionGrid.h"
#include "BatchJobListDlg.h"
#include "BatchProcessingSubmitJobDlg.h"

#include <wx/wx.h>

#ifdef __unix
	extern "C" {
#endif  // __unix

class wxNotebook;
class wxNotebookEvent;

namespace kome {
	namespace batchservice {

		class BatchBase;
		class BatchGrid;	// @date 2012/10/17 <Add> OKADA
		class BatchActionSelectPanel;	// @date 2012/10/17 <Add> OKADA

		/**
		 * @class SpectrumLabelingDialog
		 * @brief auto labeling dialog class
		 */
		class BatchProcessingMethodConfigurationDialog : public kome::window::ParametersAndHelpDialog {	// @Date:2014/01/24	<extend help>	A.Ozaki
		public:
			/**
			 * @fn BatchProcessingMethodConfigurationDialog(
						wxWindow* parent,
					)
			 * @brief constructor
			 * @param parent parent window
			 */
			BatchProcessingMethodConfigurationDialog(
				wxWindow* parent
			);

			/**
			 * @fn virtual ~BatchProcessingMethodConfigurationDialog()
			 * @brief destructor
			 */
			virtual ~BatchProcessingMethodConfigurationDialog();
		private:
			/** parameters */
			kome::plugin::ParameterSettings m_parameters;

			/** settings */
			kome::objects::SettingParameterValues m_settings;

			/** add button */
			wxBitmapButton*	m_pBatConfAddButton;

			/** edit button */
			wxBitmapButton*	m_pBatConfEditButton;

			/** save button */
			wxBitmapButton*	m_pBatConfSaveButton;

			/** save-as button */
			wxBitmapButton*	m_pBatConfSaveAsButton;

			/** delete button */
			wxBitmapButton*	m_pBatConfDeleteButton;

			/** save-as button */
			wxButton*			m_pApplyAllButton;

			/** close button */
			wxButton*			m_pCloseButton;

			/** batchi list */
			BatchGrid*			m_pBatchList;

			/** batchi list */
			BatchActionGrid*	m_pBatchActionList;

			/** batchi job list */
			wxButton*			m_pBatchJobListButton;

			/** Collapsible Pane */
			kome::window::CollapsiblePane*	m_pBatConfPane;

			/** 各tab内の情報(BatchActionSelectPanel)へのポインタを保存 */
			std::vector<void *> m_vect_pBatchActionSelectPanel;


		private:
			/** panels */
			std::vector< BatchBase* > m_panels;

			/** project index */
			int m_nProject;
	
		private:
					
			/** Batch Processing Submit Job */
			BatchProcessingSubmitJobDlg* m_pBatchProcessingSubmitJobDlg;

		private:
			/** メソッド名 */
			std::string m_strMethodName;

			/** コンボボックス（メソッド） */
			wxComboBox* m_pComboMethod;

			/** コンボボックスに指定するメソッドの一覧 */
			wxArrayString m_stringsMethod;

			/** タブ */
			wxNotebook* m_pNoteBook;

			/** データ変更フラグ */
			bool m_bChangeAction;	// trueの際、データが変更されているのでcancel時に注意メッセージを出す	 // SPEC 85856
		protected:
			/**
			 * @fn wxSizer* createMainSizer();
			 * @brief creates main sizer
			 * @return field sizer
			 */
			wxSizer* BatchProcessingMethodConfigurationDialog::createMainSizer();

			/**
			 * @fn wxSizer* createButtonSizer();
			 * @brief creates button sizer
			 * @return field sizer
			 */
			wxSizer* createButtonSizer();

			/**
			 * @fn wxSizer* createBatchMethodSizer();
			 * @brief creates BatchMethod sizer
			 * @return field sizer
			 */
			wxSizer* BatchProcessingMethodConfigurationDialog::createBatchMethodSizer();

			/**
			 * @fn wxSizer* createBatchButtonSizer();
			 * @brief creates BatchButton sizer
			 * @return field sizer
			 */
			wxSizer* BatchProcessingMethodConfigurationDialog::createBatchButtonSizer();

			/**
			 * @fn wxSizer* createBatchPaneSizer();
			 * @brief creates BatchPane sizer
			 * @return field sizer
			 */
			wxSizer* BatchProcessingMethodConfigurationDialog::createBatchPaneSizer();

			/**
			 * @fn void onToggleBatConfPane( wxCollapsiblePaneEvent& evt );
			 * @brief This method is called when the COLLAPSIBLE-PANE button is clicked.
			 * @param evt event information
			 */
			void BatchProcessingMethodConfigurationDialog::onToggleBatConfPane( wxCollapsiblePaneEvent& evt );

			/**
			 * @fn void onAdd( wxCommandEvent& evt )
			 * @brief This method is called when the add button is clicked.
			 * @param evt event information
			 */
			void onAdd( wxCommandEvent& evt );

			/**
			 * @fn void onEdit( wxCommandEvent& evt )
			 * @brief This method is called when the edit button is clicked.
			 * @param evt event information
			 */
			void onEdit( wxCommandEvent& evt );
			
			/**
			 * @fn void onSave( wxCommandEvent& evt )
			 * @brief This method is called when the save button is clicked.
			 * @param evt event information
			 */
			void onSave( wxCommandEvent& evt );
			
			/**
			 * @fn void onSaveAs( wxCommandEvent& evt )
			 * @brief This method is called when the save-as button is clicked.
			 * @param evt event information
			 */
			void onSaveAs( wxCommandEvent& evt );
			
			/**
			 * @fn void onDelete( wxCommandEvent& evt )
			 * @brief This method is called when the delete button is clicked.
			 * @param evt event information
			 */
			void onDelete( wxCommandEvent& evt );

			/**
			 * @fn void onBatJobList( wxCommandEvent& evt )
			 * @brief This method is called when the batch-job-list button is clicked.
			 * @param evt event information
			 */
			void BatchProcessingMethodConfigurationDialog::onBatJobList( wxCommandEvent& evt );

			/**
			 * @fn void onOkButton( wxCommandEvent& evt )
			 * @brief This method is called when the OK button is clicked.
			 * @param evt event information
			 */
			void BatchProcessingMethodConfigurationDialog::onOkButton( wxCommandEvent& evt );

			/**
			 * @fn void onCancelButton( wxCommandEvent& evt )
			 * @brief This method is called when the Cancel button is clicked.
			 * @param evt event information
			 */
			void BatchProcessingMethodConfigurationDialog::onCancelButton( wxCommandEvent& evt );

			/**
			 * @fn void getInitData( void )
			 * @brief get initial data
			 */
			void BatchProcessingMethodConfigurationDialog::getInitData( void );

			/**
			 * @fn void onChangePage( wxNotebookEvent& evt )
			 * @brief This method is called when the tab button is clicked.
			 * @param evt event information
			 */
			void onChangePage( wxNotebookEvent& evt );

			/**
			 * @fn void changeMethod( const char* szMethodName );
			 * @brief This method performs processing which reads method-data from an XML file. 
			 * @param szMethodName method name
			 */
			void changeMethod( const char* szMethodName );

			/**
			 * @fn void changeMethodCombobox( wxCommandEvent& evt );
			 * @brief This method is called when the Combobox is changed.
			 * @param evt event information
			 */
			void changeMethodCombobox( wxCommandEvent& evt );

			/**
			 * @fn void remakeMethodCombobox( wxCommandEvent& evt );
			 * @brief This method is not used any longer. 
			 * @param strSelectMethod ---
			 */
			void remakeMethodCombobox( std::string strSelectMethod );

		public:
			/**
			 * @fn int checkSameNameConbobox( const char * szNewMethodName )
			 * @brief 新しいメソッド名が既に存在するかどうかチェックします
			 * @param szNewMethodName
			 * @return
			 */
			int checkSameNameConbobox( const char * szNewMethodName );

		
		public:
			/**
			 * @fn BatchActionSelectPanel * getBatchActionSelectPanel( int index )
			 * @brief タブのパネルを返します
			 * @param index
			 * @return BatchActionSelectPanel
			 */
			BatchActionSelectPanel * getBatchActionSelectPanel( int index ){
				return ((BatchActionSelectPanel *)(m_vect_pBatchActionSelectPanel[index]));
			}

			/**
			 * @fn void editBatchActionSelectPanel( int index, int line_no )
			 * @brief タブのonEditを呼び出します
			 * @param index index
			 * @param line_no line no
			 */
			void editBatchActionSelectPanel( int index, int line_no );

			/**
			 * @fn clearDefaultOutputFormat( const char* szBatchMethod )
			 * @brief バッチメソッドが削除されるときに呼び出します
			 * @param szBatchMethod batch method name
			 */
			void deleteDefaultOutputFormat( const char* szBatchMethod );	// @date 2013/05/23 <Add> OKADA

			/**
			 * @fn void copyDefaultOutputFormat( const char* szBatchMethod, const char* szNewBatchMethod, bool flgRename=false )
			 * @brief バッチメソッドがコピーされるときに呼び出します
			 * @param szBatchMethod    batch method name of source
			 * @param szNewBatchMethod batch method name of destination
			 * @param flgRename        rename flag. if true, source method deleted.
			 */
			void copyDefaultOutputFormat( const char* szBatchMethod, const char* szNewBatchMethod, bool flgRename=false );

		public:
			/**
			 * @fn int updateBatchList( void );
			 * @brief creates BatchPane sizer
			 * @return always 0
			 */
			int updateBatchList( void );
			
			/**
			 * @fn int updateBatchList( void );
			 * @brief XMLファイルへ設定データを保存
			 * @param szMethodName method name
			 * @return always 0
			 */
			int exportBatchXML( const char* szMethodName );
			
			/**
			 * @fn wxArrayString getStringsMethod( void );
			 * @brief コンボボックスに指定するメソッドの一覧を取得
			 * @param szMethodName method name
			 * @return コンボボックスに指定するメソッドの一覧
			 */
			wxArrayString getStringsMethod( void );
			
			/**
			 * @fn std::string getCurrentMethod( void );
			 * @brief 現在選択されているメソッド名を取得
			 * @param szMethodName method name
			 * @return 現在のメソッド名
			 */
			std::string getCurrentMethod( void );

			/**
			 * @fn std::string getCurrentMethod( void );
			 * @brief 現在選択されているメソッド名をコンボボックスから取得
			 * @param szMethodName method name
			 * @return 現在のメソッド名
			 */
			std::string getComboMethodText( void );

			/**
			 * @fn onUpdateUI( wxUpdateUIEvent& evt )
			 * @brief updates button enable/disable
			 * @param evt event information
			 */
			void onUpdateUI( wxUpdateUIEvent& evt );				

			/**
			 * @fn param2vect( wxUpdateUIEvent& evt )
			 * @brief translate setting string 2 vector
			 * @param[in] szParam parameter string
			 * @param[out] vecParam vector parameter
			 */
			int param2vect( const char* szParam, std::vector<std::string>& vecParam );	// @date 2013/08/20 <Add> OKADA

		private:
			DECLARE_EVENT_TABLE()

		};
	}
}

#ifdef __unix
	}
#endif  // __unix


#endif// __KOME_BATCH_SERVICE_BATCH_PROCESSING_METHOD_CONFIGURATION_DIALOG_H__

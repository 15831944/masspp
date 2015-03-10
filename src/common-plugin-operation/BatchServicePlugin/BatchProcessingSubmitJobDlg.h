/**
 * @file BatchProcessingSubmitJobDlg.h
 * @brief interfaces of BatchProcessingSubmitJobDlg class
 *
 * @author OKADA, H
 * @date 2012-02-29
 * 
 * Copyright(C) 2012 Shimadzu Corporation. All rights reserved.
 */


#ifndef __KOME_BATCH_SERVICE_BATCH_PROCESSING_SUBMIT_JOB_DLG_H__
#define __KOME_BATCH_SERVICE_BATCH_PROCESSING_SUBMIT_JOB_DLG_H__



#include "BatchProcessingSubmitJobGrid.h"
#include "BatchJobListDlg.h"
#include "BatchServicePluginManager.h"	// for FormatCheckBox

// ファイル名を複数指定する際の区切り文字
#define STR_FILE_STRINGS_SEPARATOR	"|"		// masspp\src\common-plugin-io\FileIOPlugin\FileSampleReader.h 内でも定義しています

namespace kome {
	namespace batchservice {
		
		class BatchBase;
		
		class BatchProcessingSubmitJobDlg : public kome::window::ParametersAndHelpDialog {	// @Date:2014/01/24	<extend help>	A.Ozaki
		public:

			/**
			 * @fn BatchProcessingSubmitJobDlg( wxWindow* parent	);
			 * @brief constructor
			 * @param parent parent window
			 */
			BatchProcessingSubmitJobDlg(
				wxWindow* parent
			);
			
			/**
			 * @fn virtual ~BatchProcessingSubmitJobDlg();
			 * @brief destructor
			 */
			virtual ~BatchProcessingSubmitJobDlg();
		
		protected:
			/** Batch Processing Submit Job */
			BatchProcessingSubmitJobGrid * m_pBatchProcessingSubmitJobList;

			/** add button */
			wxBitmapButton*	m_pSubmitJobAddButton;

			/** delete button */
			wxBitmapButton*	m_pSubmitJobDeleteButton;

			/** up */
			wxBitmapButton*	m_pSubmitJobUpButton;

			/** down */
			wxBitmapButton*	m_pSubmitJobDownButton;

			/** output formats */
			wxBitmapButton*	m_pSubmitJobOutputFormatsButton;
			
			/** folder open */
			wxBitmapButton*	m_pSubmitJobFolderOpenButton;	// @date 2012/08/03 <Add> OKADA

			/** settings */                                        
			kome::objects::SettingParameterValues m_settings;	 // @date 2013/05/17 <Add> OKADA 
		private:
			/** name */
			wxTextCtrl* m_nameCtrl;

			/** comment */
			wxTextCtrl* m_textCtrl;

			/** combo box */
			wxComboBox* m_pCombo;

			/** data type */
			wxComboBox* m_pComboDataType;	// @date 2012/08/02 <Add> OKADA

			/* 当初表示するData Type */
			wxString m_stringDefaultSelectMethod;	// @date 2012/08/06 <Add> OKADA

			/* 現在選択されているData Type */
			wxString m_stringSelectedMethod;	// @date 2012/08/06 <Add> OKADA

		private:
			std::vector<Action> m_vactServerList;

			/** 入出力ファイル情報 */
			stIOInf m_stIoInf;
		
		protected:

			/** parameters */
			kome::plugin::ParameterSettings m_parameters;	

		public:
			/**
			 * @fn wxSizer* createMainSizer();
			 * @brief creates main sizer
			 * @return field sizer
			 */
			wxSizer* createMainSizer();

			/**
			 * @fn wxSizer* createButtonSizer();
			 * @brief creates button sizer
			 * @return field sizer
			 */
			wxSizer* createButtonSizer();

			/**
			 * @fn wxSizer* createButtonSizer();
			 * @brief creates Submit Batch button sizer
			 * @return field sizer
			 */
			wxSizer* createSubmitBatchButtonSizer();
		
		protected:
			/**
			 * @fn onSubmitButton( wxCommandEvent& evt )
			 * @brief This method is called when the SUBMIT button is clicked.
			 * @param evt event information
			 */
			void onSubmitButton( wxCommandEvent& evt );

			/**
			 * @fn onCancelButton( wxCommandEvent& evt )
			 * @brief This method is called when the cancel button is clicked.
			 * @param evt event information
			 */
			void onCancelButton( wxCommandEvent& evt );

			/**
			 * @fn onAddButton( wxCommandEvent& evt )
			 * @brief This method is called when the add button is clicked.
			 * @param evt event information
			 */
			void onAddButton( wxCommandEvent& evt );

			/**
			 * @fn onDeleteButton( wxCommandEvent& evt )
			 * @brief This method is called when the add button is clicked.
			 * @param evt event information
			 */
			void onDeleteButton( wxCommandEvent& evt );

			/**
			 * @fn onUpButton( wxCommandEvent& evt )
			 * @brief This method is called when the up button is clicked.
			 * @param evt event information
			 */
			void onUpButton( wxCommandEvent& evt );

			/**
			 * @fn onDownButton( wxCommandEvent& evt )
			 * @brief This method is called when the down button is clicked.
			 * @param evt event information
			 */
			void onDownButton( wxCommandEvent& evt );
				
			/**
			 * @fn onOpenFolder( wxCommandEvent& evt )
			 * @brief This method is called when the open-folder is clicked.
			 * @param evt event information
			 */
			void onOpenFolder( wxCommandEvent& evt );

			/**
			 * @fn onChangeComboDataType( wxCommandEvent& evt )
			 * @brief This method is called when the DataType-ComboBox is clicked.
			 * @param evt event information
			 */
			void onChangeComboDataType( wxCommandEvent& evt );

			/**
			 * @fn stIOInf getIOInf( void );
			 * @brief This method is called when the output-formats button is clicked.
			 * @param evt event information
			 */
			stIOInf BatchProcessingSubmitJobDlg::getIOInf( void );

			/**
			 * @fn void onUpdateUI( wxUpdateUIEvent& evt );
			 * @brief This method is called when idle time. ボタンのEnable/Disableをここで決めます
			 * @param evt event information
			 */
			void onUpdateUI( wxUpdateUIEvent& evt );
			
			/**
			 * @fn void splitText( std::vector& result, const std::string& src, std::string& separator );
			 * @brief This method is separate strings by separator. 
			 * @param result
			 * @param src
			 * @param separator
			 */
			void splitText( std::vector<std::string>& result, const std::string& src, const char * separator );

			/**
			 * @fn void BatchProcessingSubmitJobDlg::onSelectComboOutputFormat( kome::window::SettingsPanelEvent& evt )
			 * @brief
			 * @param evt
			 */
			void BatchProcessingSubmitJobDlg::onSelectComboOutputFormat( kome::window::SettingsPanelEvent& evt );

		private:
			DECLARE_EVENT_TABLE()
			
		};
	}
}


#endif

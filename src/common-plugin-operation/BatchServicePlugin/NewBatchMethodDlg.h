/**
 * @file NewBatchMethodDlg.h
 * @brief interfaces of NewBatchMethodDlg class
 *
 * @author OKADA, H
 * @date 2012-03-05
 * 
 * Copyright(C) 2012 Shimadzu Corporation. All rights reserved.
 */


#ifndef __KOME_BATCH_SERVICE_NEW_BATCH_METHOD_DLG_H__
#define __KOME_BATCH_SERVICE_NEW_BATCH_METHOD_DLG_H__

#include "BatchProcessingMethodConfigurationDialog.h"

typedef struct{
	std::string strNewMethodName;// エディットボックスに入力された新しいメソッド名
	bool bMethod;
	bool bTemplate;
	bool bBlank;
	wxString wxstrMethod;
	wxString wxstrTemplate;
}NewMethodSetting; 

namespace kome {
	namespace batchservice {
		class BatchBase;

		class NewBatchMethodDlg : public kome::window::ParametersAndHelpDialog {	// @Date:2014/01/24	<extend help>	A.Ozaki
		public:
			NewBatchMethodDlg(
				wxWindow* parent,
				void *pParent
			);
			
			virtual ~NewBatchMethodDlg();

		public:

			/** method ラジオボタンの有効/無効フラグ */
			bool m_bMethod;

			/** template ラジオボタンの有効/無効フラグ（現在未使用）　*/
			bool m_bTemplate;

			/** blank ラジオボタンの有効/無効フラグ */
			bool m_bBlank;

		private:
			/** new method setting */
			NewMethodSetting m_stNewMethodSetting;

			/** parent dlg */
			BatchProcessingMethodConfigurationDialog * m_pParentDlg;

			/** strings method */
			wxArrayString m_wxastrStringsMethod;

			/** edit boxに入力された新しいMethod名 */
			wxString m_wxstrNewMethodName;	

			/** 親ウインドウの現在のMethod */
			std::string m_strCurrentMethodParent;	 
			
			/** edit boxで選択されたMethod名 */
			wxString m_wxstrSelectedMethod;			

			/** edit boxで選択されたTemplate名 */
			wxString m_wxstrSelectedTemplate;		

			/** A text control for the name of method */
			wxTextCtrl* m_pNameCtrl;

			/** method radio button */
			wxRadioButton* m_rbMethod;

			/** template radio button */
			wxRadioButton* m_rbTemplate;

			/** blank radio button */
			wxRadioButton* m_rbBlank;
			
			/** method combo box */
			wxComboBox* m_pComboMethod;

			/** template combo box */
			wxComboBox* m_pComboTemplate;
		
		public:
			/**
			 * @fn wxSizer* createMainSizer()
			 * @brief create main sizer
			 * @return main sizer
			 */
			wxSizer* createMainSizer();

			/**
			 * @fn wxSizer* createUpsideSizer()
			 * @brief create upside sizer
			 * @return upside sizer
			 */
			wxSizer* createUpsideSizer();

			/**
			 * @fn wxSizer* createBottomSizer()
			 * @brief create bottom sizer
			 * @return bottom sizer
			 */
			wxSizer* createBottomSizer();	

			/**
			 * @fn wxSizer* createButtonSizer()
			 * @brief create button sizer
			 * @return button sizer
			 */
			wxSizer* createButtonSizer();

			/**
			 * @fn void onOkButton( wxCommandEvent& evt )
			 * @brief This method is called when the OK button is clicked.
			 * @param evt event information
			 */
			void onOkButton( wxCommandEvent& evt );

			/**
			 * @fn void changeMethodCombobox( wxCommandEvent& evt )
			 * @brief This method is called when the Method combobox button is clicked.
			 * @param evt event information
			 */
			void changeMethodCombobox( wxCommandEvent& evt );

			/**
			 * @fn void changeTemplateCombobox( wxCommandEvent& evt )
			 * @brief This method is called when the Template combobox button is clicked.
			 * @param evt event information
			 */
			void changeTemplateCombobox( wxCommandEvent& evt );

			/**
			 * @fn NewMethodSetting getNewMethodSetting( void )
			 * @brief get new method setting
			 * @return new method setting
			 */
			NewMethodSetting getNewMethodSetting( void );

			/**
			 * @fn void onUpdateUI( wxUpdateUIEvent& evt )
			 * @brief when updated UI, call this function
			 * @param evt event information
			 */
			void onUpdateUI( wxUpdateUIEvent& evt );

		private:
			DECLARE_EVENT_TABLE()

		};
	}
}



#endif

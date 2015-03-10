/**
 * @file OutputFormatsDlg.h
 * @brief interfaces of OutputFormatsDlg class
 *
 * @author OKADA, H
 * @date 2012-03-06
 * 
 * Copyright(C) 2012 Shimadzu Corporation. All rights reserved.
 */


#ifndef __KOME_BATCH_SERVICE_NEW_BATCH_METHOD_DLG_H__


#define __KOME_BATCH_SERVICE_NEW_BATCH_METHOD_DLG_H__

#include "BatchServicePluginManager.h"

namespace kome {
	namespace batchservice {
		class BatchBase;

		class OutputFormatsDlg : public kome::window::StandardDialog {
		public:
			/**
			 * @fn OutputFormatsDlg( wxWindow* parent )
			 * @brief コンストラクタ
			 * @param parent
			 */
			OutputFormatsDlg( wxWindow* parent );
			
			/**
			 * @fn virtual ~OutputFormatsDlg()
			 * @brief デストラクタ
			 */
			virtual ~OutputFormatsDlg();
		private:
			
			std::vector<wxCheckBox*>  m_vectCheckBox;
			std::vector<output_format_inf>  m_vectOutputFormatInf;

		public:
			/**
			 * @fn virtual wxSizer* createMainSizer();
			 * @brief creates main sizer (override method)
			 * @return field sizer
			 */
			wxSizer* createMainSizer();

			/**
			 * @fn virtual wxSizer* createButtonSizer();
			 * @brief creates main sizer (override method)
			 * @return field sizer
			 */
			wxSizer* createButtonSizer();
						
			/**
			 * @fn void getOutputFormat( std::vector<output_format_inf> & vect_stTempOutFrm)
			 * @brief get out put format
			 * @param vect_stTempOutFrm
			 */
			void getOutputFormat( std::vector<output_format_inf> & vect_stTempOutFrm);
						
			/**
			 * @fn void onOkButton( wxCommandEvent& evt )
			 * @brief on ok button
			 */
			void onOkButton( wxCommandEvent& evt );
						
			/**
			 * @fn wxCheckBox* getCheckBox( int index )
			 * @brief get check box
			 * @param index
			 */
			wxCheckBox* getCheckBox( int index );	// 2012/04/04 

		private:
			DECLARE_EVENT_TABLE()

		};
	}
}



#endif

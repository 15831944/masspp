/**
 * @file BatchJobListDlg.h
 * @brief interfaces of BatchGrid class
 *
 * @author OKADA, H
 * @date 2012-02-30
 * 
 * Copyright(C) 2012-2014 Shimadzu Corporation. All rights reserved.
 */


#ifndef __KOME_BATCH_SERVICE_BATCH_JOB_LIST_DLG_H__
#define __KOME_BATCH_SERVICE_BATCH_JOB_LIST_DLG_H__


#include "BatchServicePluginManager.h"
#include "BatchJobListGrid.h"
#include "BatchActionGrid.h"

#include <wx/wx.h>

namespace kome {
	namespace batchservice {

		class BatchBase;
		
		class BatchJobListDlg : public kome::window::StandardDialog {
		public:
			/**
			 * @fn BatchJobListDlg(	wxWindow* parent)
			 * @brief constructor
			 * @param parent parent window
			 */
			BatchJobListDlg(
				wxWindow* parent
			);
			
			/**
			 * @fn virtual ~BatchJobListDlg()
			 * @brief destructor
			 */
			virtual ~BatchJobListDlg();
		protected:
			/** parameters */
			kome::plugin::ParameterSettings m_parameters;
	
		protected:
			/** batch job list griod */
			BatchJobListGrid* m_pBatchJobList;
			/** delete button */
			wxBitmapButton*	m_pBatConfDeleteButton;
		protected:
			/**
			 * @fn virtual wxSizer* createMainSizer();
			 * @brief creates main sizer (override method)
			 * @return field sizer
			 */
			wxSizer* createMainSizer();

			/**
			 * @fn virtual wxSizer* createButtonSizer();
			 * @brief creates button sizer (override method)
			 * @return field sizer
			 */
			wxSizer* createButtonSizer();

			/**
			 * @fn wxSizer* createBatchButtonSizer()
			 * @brief creates button sizer (override method)
			 * @return field sizer
			 */
			wxSizer* createBatchButtonSizer();
			
			/**
			 * @fn void onBatJobListClose( wxCommandEvent& evt )
			 * @brief batch job list close
			 * @param evt
			 */
			void onBatJobListClose( wxCommandEvent& evt );

			/**
			 * @fn void onBatJobListDelete( wxCommandEvent& evt )
			 * @brief batch job liset delete
			 * @param evt
			 */
			void onBatJobListDelete( wxCommandEvent& evt );
						
		private:
			DECLARE_EVENT_TABLE()
			
		};
	}
}


#endif


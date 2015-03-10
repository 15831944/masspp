/**
 * @file BatchBase.h
 * @brief interfaces of BatchBase class
 *
 * @author OKADA, H
 * @date 2012-02-30
 * 
 * Copyright(C) 2012 Shimadzu Corporation. All rights reserved.
 */


#ifndef __KOME_BATCH_SERVICE_BATCHBASE_H__
#define __KOME_BATCH_SERVICE_BATCHBASE_H__

#include "BatchProcessingMethodConfigurationDialog.h"

namespace kome {
	namespace batchservice {
		class BatchBase : public kome::window::StandardPanel
		{
		public:
			/**
			 * @fn PeakMatrixSettingsPanelBase( wxWindow* pParent, const char* sPanelName, QuantitationDialog* pParentDlg )
			 * @brief constructor
			 * @param pParent parent window
			 * @param pParentDlg parent dialog
			 * @param sPanelName panel name
			 * @param bIncludeDefaultButton whether to include a "Default" button
			 */
			BatchBase( wxWindow* pParent
						, kome::batchservice::BatchProcessingMethodConfigurationDialog* pParentDlg
						, const char* sPanelName
						, bool bIncludeDefaultButton = true);

			/**
			 * @fn virtual ~PeakMatrixSettingsPanelBase()
			 * @brief destructor
			 */
			virtual ~BatchBase(void);

			// >>>>>>	@Date:2014/02/04	<Add>	A.Ozaki
			//
			/**
			 * @fn createWidgets( void )
			 * @brief create widgets on panel
			 */
			void	createWidgets( void );
			//
			// <<<<<<	@Date:2014/02/04	<Add>	A.Ozaki
		
		protected:
			/** 親ウインドウのポインタ */
			BatchProcessingMethodConfigurationDialog*		m_pParentDlg;
				
		private:
			/**
			 * @fn virtual wxSizer* createMainSizer()
			 * @brief creates main sizer
			 * @return main sizer
			 */
			virtual wxSizer* createMainSizer();

		protected:
			/**
			 * @fn virtual void createInnerSizer()
			 * @brief creates inner sizer
			 * @param InnerSizer inner sizer
			 */
			virtual void createInnerSizer(wxStaticBoxSizer* InnerSizer) = 0;


		private:
			DECLARE_EVENT_TABLE()
		};
	}
}

#endif

/**
 * @file BatchActionSelectPanel.h
 * @brief interfaces of BatchActionSelectPanel class
 *
 * @author OKADA, H
 * @date 2012-02-30
 * 
 * Copyright(C) 2012 Shimadzu Corporation. All rights reserved.
 */


#ifndef __KOME_BATCH_SERVICE_BATCH_SELECT_PANEL_H__
#define __KOME_BATCH_SERVICE_BATCH_SELECT_PANEL_H__

#include "BatchBase.h"
#include "BatchProcessingMethodConfigurationDialog.h"
#include "BatchActionGrid.h"

#define BORDER_SIZE		3

namespace kome {
	namespace batchservice {
		class BatchActionSelectPanel : public BatchBase
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
				BatchActionSelectPanel( wxWindow* pParent, BatchProcessingMethodConfigurationDialog* pParentDlg, int nPanelNo );

				/**
				 * @fn virtual ~PeakMatrixSettingsPanelBase()
				 * @brief destructor
				 */
				virtual ~BatchActionSelectPanel(void);

				protected:

				/** page */
				kome::plugin::ParameterSettings m_params;

				/** 左側の表 */
				BatchActionGrid*	m_pBatchListSelected;

				/** 右側の表 */
				BatchActionGrid*	m_pBatchListAvailable;

			protected:
				/** 親ウインドウのポインタ */
				kome::batchservice::BatchProcessingMethodConfigurationDialog* m_pParentDlg;
			
				/** [←]Button */
				wxBitmapButton*	m_pBatConfSelectActionButton;

				/** 鉛筆Button */
				wxBitmapButton*	m_pBatConfEditButton;

				/** [×] */
				wxBitmapButton*	m_pBatConfDeleteButton;

				/** [↑] */
				wxBitmapButton*	m_pBatConfUpButton;

				/** [↓] */
				wxBitmapButton*	m_pBatConfDownButton;

				/** Panel No */
				int m_nPanelNo;
			protected:
				/**
				 * @fn virtual void createInnerSizer(wxStaticBoxSizer* pInnerSizer)
				 * @brief creates inner sizer
				 * @param pInnerSizer inner sizer
				 */
				virtual void createInnerSizer(wxStaticBoxSizer* pInnerSizer);

			public:
				/**
				 * @fn onAdd( wxCommandEvent& evt )
				 * @brief This method is called when the Add[<-] button is clicked.
				 * @param evt event information
				 */
				void onAdd( wxCommandEvent& evt );

				/**
				 * @fn onEdit( wxCommandEvent& evt )
				 * @brief This method is called when the edit button is clicked.
				 * @param evt event information
				 */
				void onEdit( wxCommandEvent& evt );

				/**
				 * @fn editByIndex( int nIndex )
				 * @brief This method is called from onEdit()
				 * @param nIndex edit target info
				 * @return 0:success 1:fail(no operation)
				 */
				int BatchActionSelectPanel::editByIndex( int nIndex );
				
				/**
				 * @fn onDelete( wxCommandEvent& evt )
				 * @brief This method is called when the delete button is clicked.
				 * @param evt event information
				 */
				void onDelete( wxCommandEvent& evt );

				/**
				 * @fn onDeleteLine( int index, bool flgUpdate=false )
				 * @brief This method is called when the delete button is clicked.
				 * @param index delete line
				 * @param flgUpdate update flag
				 */
				int onDeleteLine( int index, bool flgUpdate=false );	// @date 2013/06/04 <Add> OKADA

			protected:			

				/**
				 * @fn onUp( wxCommandEvent& evt )
				 * @brief This method is called when the up button is clicked.
				 * @param evt event information
				 */
				void onUp( wxCommandEvent& evt );

				/**
				 * @fn onDown( wxCommandEvent& evt )
				 * @brief This method is called when the down button is clicked.
				 * @param evt event information
				 */
				void onDown( wxCommandEvent& evt );
				
				/**
				 * @fn onDown( wxCommandEvent& evt )
				 * @brief This method is get of Setting Xml File name array
				 * @return Xml File name array
				 */
				std::vector<std::string> getSettingXmlFiles( void );

			public:
				/**
				 * @fn getSelectedActions( void )
				 * @brief This method is get object of m_pBatchListSelected
				 * @return pointer for m_pBatchListSelected
				 */
				BatchActionGrid* getSelectedActions( void ){
					return m_pBatchListSelected;
				}

				/**
				 * @fn onUpdateUI( wxUpdateUIEvent& evt )
				 * @brief updates button enable/disable
				 * @param evt event information
				 */
				void onUpdateUI( wxUpdateUIEvent& evt );

				/**
				 * @fn kome::batchservice::BatchProcessingMethodConfigurationDialog* getParentDlg( void );
				 * @brief return parent window
				 * @return parent window
				 */
				kome::batchservice::BatchProcessingMethodConfigurationDialog* getParentDlg( void );

				// @date 2013/04/24 <Add> OKADA ------->
			private:
				/**
				 * @fn bool prepare()
				 * @brief prepare for the DB search
				 * @return If false, identification should not be continued.
				 */
				bool prepare();
				// @date 2013/04/24 <Add> OKADA <-------
		
			private:
				DECLARE_EVENT_TABLE()
		};
	}
}

#endif

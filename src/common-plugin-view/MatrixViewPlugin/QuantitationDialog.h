/**
 * @file QuantitationDialog.h
 * @brief interfaces of QuantitationDialog class
 *
 * @author H.Parry
 * @date 2011.04.27
 * 
 * Copyright(C) 2014 Shimadzu Corporation. All rights reserved.
 */

#ifndef __KOME_QUANTITATION_DIALOG_H__
#define __KOME_QUANTITATION_DIALOG_H__

#include <vector>
#include <wx/wx.h>

class wxToggleButton;
class wxCollapsiblePaneEvent;
class wxNotebookEvent;

namespace kome {
	namespace matrix {
		namespace view {

		class MatrixListCtrl;
		class PeakInfoDialog;

		/**
		 * @class QuantitationDialog
		 * @brief quantitation dialog class
		 */
		class QuantitationDialog : public kome::window::ParametersAndHelpDialog {
		public:
			/**
			 * @fn QuantitationDialog( wxWindow* parent )
			 * @brief constructor
			 * @param parent parent window
			 */
			QuantitationDialog(
				wxWindow* pParent
			);

			/**
			 * @fn virtual ~QuantitationDialog()
			 * @brief destructor
			 */
			virtual ~QuantitationDialog();
		private:
			/** matrix list ctrl */
			MatrixListCtrl*		m_pMatrixList;
			/** delete button */
			wxBitmapButton*		m_pMatrixDeleteButton;
			/** combo box */
			wxComboBox*			m_pMatrixCombo;
			/** export button */
			wxBitmapButton*		m_pExportButton;
			/** close button */
			wxButton*			m_pCloseButton;
			/** group flag */
			bool				m_bGroup;
			/** oberlap flag */
			bool				m_bOverlap;
			/** edit button */
			wxButton*			m_editButton;

			/** project index */
			int m_nProject;

			/** projects */
			std::vector< int > m_projects;

			/**
			 * @struct PeakAction
			 * @brief peak action 
			 */
			struct PeakAction {
				kome::plugin::PluginFunctionItem* fun;
				wxIcon icon0;
				wxIcon icon1;
				wxBitmapButton* button;
				PeakInfoDialog* dialog;
				int pos;
			};

			/** peak actions */
			std::vector< PeakAction > m_peakActions;

			/** peak ID */
			int m_peakId;

			/** y position */
			int m_yPos;
		public:
			/**
			 * @fn MatrixListCtrl* getMatrixListCtrl()
			 * @brief gets the matrix list control
			 * @return matrix list control
			 */
			MatrixListCtrl* getMatrixListCtrl();

		protected:
			/**
			 * @fn std::string getStatusName( const unsigned int idx )
			 * @brief gets the status name
			 * @param[in] idx peak action index
			 * @return status name
			 */
			std::string getStatusName( const unsigned int idx );

			/**
			 * @fn bool isVisibleDialog( const unsigned int idx )
			 * @brief judges whether the dilaog is visible or not
			 * @param[in] idx peak action index
			 * @return If true, the specified dialog is visible.
			 */
			bool isVisibleDialog( const unsigned int idx );

			/**
			 * @fn void openNewDialog( const unsigned int idx )
			 * @brief opens the new dialog
			 * @param[in] idx peak action index
			 */
			void openNewDialog( const unsigned int idx );

		public:
			/**
			 * @fn void setPeakId( const int peakId )
			 * @brief sets the peak ID
			 * @parma[in] peakId peak ID
			 */
			void setPeakId( const int peakId );

			/**
			 * @fn void openPeak( const int peakId )
			 * @brief opens peak
			 * @parma[in] peakId peak ID
			 */
			void openPeak( const int peakId );
			
			/**
			 * @fn void updateMatrixCombo()
			 * @brief updates matrix combobox
			 */
			void updateMatrixCombo();

			/** 
			 * @fn void setnProject()
			 * @brief set project
			 */
			void setnProject();

			/**
			 * @fn void closePeakInfoDlg()
			 * @brief close peak info dialog
			 */
			void closePeakInfoDlg();
		protected:
			/**
			 * @fn virtual wxSizer* createMainSizer();
			 * @brief creates main sizer (override method)
			 * @return field sizer
			 */
			virtual wxSizer* createMainSizer();

			/**
			 * @fn virtual wxSizer* createButtonSizer();
			 * @brief creates button sizer (override method)
			 * @return field sizer
			 */
			virtual wxSizer* createButtonSizer();

			/**
			 * @fn int getSelectedProject()
			 * @brief gets selected project
			 * @return selected project ID
			 */
			int getSelectedProject();

			/**
			 * @fn MatrixNameExists( const char* name )
			 * @brief checks if a matrix with the specified name exists
			 * @param name the matrix name to check
			 * @return true if a matrix with this name exists, otherwise false
			 */
			bool MatrixNameExists( const char* name );

			/**
			 * @fn virtual bool TransferDataFromWindow()
			 * @brief transfer window control data to value
			 * @return If a transfer failed, this method returns false.
			 */
			virtual bool TransferDataFromWindow();

			/**
			 * @fn virtual bool TransferDataToWindow()
			 * @brief transfer value data to window controls
			 * @return If a transfer failed, this method returns false.
			 */
			virtual bool TransferDataToWindow();

			/**
			 * @fn bool PreApplyValidateAllPanels()
			 * @brief perform the pre-apply validation checks on all panels
			 * @return If any panel failed its pre-apply validation checks, this method returns false.
			 */
			bool PreApplyValidateAllPanels();

			/**
			 * @fn virtual bool SaveAllPanelSettings(bool bOnlyEditedPanels = false)
			 * @brief save settings on all panels
			 * @param bOnlyEditedPanels apply only those panels which are edited?
			 * @return If any panel failed to be saved, this method returns false.
			 */
			bool SaveAllPanelSettings(bool bOnlyEditedPanels = false);

			/**
			 * @fn int ComboIndexFromProjectID(int iProjectID);
			 * @brief gets the combo selection for a project ID
			 * @param iProjectID the project ID
			 * @return combo index
			 */
			int ComboIndexFromProjectID(int iProjectID);

			/**
			 * @fn void SetAllPanelsToNotEdited()
			 * @brief Set edited state of all panels to false
			 */
			void SetAllPanelsToNotEdited();

		public:
			/**
			 * @fn int getProject()
			 * @brief gets project
			 * @return project ID to be selected
			 */
			int getProject();

			/**
			 * @fn void RecreateMatrixList()
			 * @brief recreates matrix list
			 */
			void RecreateMatrixList();

			/**
			* @fn int GetTotalNumberOfSamples()
			* @brief gets the number of samples that are currently registered on the Group Settings panel.
			* @return the number of samples that are registered in the current matrix.
			*/
			int GetTotalNumberOfSamples();

			/**
			 * @fn intbool MatrixListIsEmpty()
			 * @brief indicates whether the peak matrix list is empty
			 * @return true if the peak matrix list is empty, otherwise false.
			 */
			bool MatrixListIsEmpty();

		protected:
			/**
			 * @fn void onSize( wxSizeEvent& evt )
			 * @brief This method is called when the dialog size is changed
			 * @param evt size event information
			 */
			void onSize( wxSizeEvent& evt );

			
			/**
			 * @fn void onEditMatrix( wxCommandEvent& evt )
			 * @brief This method is called when the edit button is clicked.
			 * @param evt event information
			 */
			void onEditMatrix( wxCommandEvent& evt );

			/**
			 * @fn void onDeleteMatrix( wxCommandEvent& evt )
			 * @brief This method is called when the delete button is clicked.
			 * @param evt event information
			 */
			void onDeleteMatrix( wxCommandEvent& evt );

			/**
			 * @fn void onSelectNewMatrix( wxCommandEvent& evt )
			 * @brief This method is called when a matrix combo item is selected.
			 * @param evt event information
			 */
			void onSelectNewMatrix( wxCommandEvent& evt );

			/**
			 * @fn void onExport( wxCommandEvent& evt )
			 * @brief This method is called when the export button is clicked.
			 * @param evt event information
			 */
			void onExport( wxCommandEvent& evt );
	
			/**
			 * @fn void onClose( wxCommandEvent& evt )
			 * @brief This method is called when the close button is clicked.
			 * @param evt event information
			 */
			void onClose( wxCommandEvent& evt );

			/**
			 * @fn void onCloseButton( wxCloseEvent& evt )
			 * @brief This method is called when the close (system) button is clicked.
			 * @param evt event information
			 */
			void onCloseButton( wxCloseEvent& evt );

			/**
			 * @fn void onPeakButton( wxCommandEvent& evt )
			 * @brief This method is called when a matrix peak button is clicked.
			 *@ param[in] evt event information
			 */
			void onPeakButton( wxCommandEvent& evt );

			/**
			 * @fn void onPeakUpdateUIButton( wxUpdateUIEvent& evt )
			 * @brief updates the UI of the peak button
			 * @parma[in] evt event information
			 */
			void onPeakUpdateUIButton( wxUpdateUIEvent& evt );

		private:
			DECLARE_EVENT_TABLE();

			};
		}
	}
}

#endif	//  __KOME_QUANTITATION_DIALOG_H__

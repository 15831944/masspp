/**
 * @file EditSampleUserPropertiesDialog.h
 * @brief interfaces of EditSampleUserPropertiesDialog class
 *
 * @author H.Parry
 * @date 2011.04.04
 * 
 * Copyright(C) 2014 Shimadzu Corporation. All rights reserved.
 */

#ifndef __KOME_PRODUCT_ION_SELECTION_DIALOG_H__
#define __KOME_PRODUCT_ION_SELECTION_DIALOG_H__

#include <wx/wx.h>

namespace kome {
	namespace properties {
		class DetailsViewManager;
	
		class SampleTreeCtrl;
		class DetailsListCtrl;

		/**
		 * @class EditSampleUserPropertiesDialog
		 * @brief auto labeling dialog class
		 */
		class EditSampleUserPropertiesDialog : public kome::window::StandardDialog {
		public:
			/**
			 * @fn EditSampleUserPropertiesDialog( wxWindow* parent )
			 * @brief constructor
			 * @param parent parent window
			 */
			EditSampleUserPropertiesDialog(
				wxWindow* parent
			);

			/**
			 * @fn virtual ~EditSampleUserPropertiesDialog()
			 * @brief destructor
			 */
			virtual ~EditSampleUserPropertiesDialog();

		private:
			/** select num */
			unsigned int m_iSelected;

			/** active spectrum */
			kome::objects::Spectrum* m_pActiveSpectrum;

			/** active chromatogram */
			kome::objects::Chromatogram* m_pActiveChromatogram;

			/** active group */
			kome::objects::DataGroupNode* m_pActiveGroup;

			/** new button */
			wxButton *m_pNewButton;

			/** delete button */
			wxButton *m_pDeleteButton;

			/** key static text */
			wxStaticText* m_pKeyStaticText;

			/** value static text */
			wxStaticText* m_pValueStaticText;

			/** key text ctrl */
			wxTextCtrl* m_pKeyTextCtrl;

			/** value text ctrl */
			wxTextCtrl* m_pValueTextCtrl;

			/** selected item */
			long m_lSelectedItem;

			/** is edited flag */
			bool m_bIsEdited;
	
			/** pointer to the details view manager */
			DetailsViewManager* m_pDetailsViewMgr;

			/** pointer to the sample tree control */
			SampleTreeCtrl* m_treeCtrl;

			/** pointer to the details view list control */
			DetailsListCtrl* m_list;

		public:

			// Return the selected item index.
			inline unsigned int GetSelectedItem() {return m_iSelected;}

			/**
			 * @fn void SetActiveSpectrum(kome::objects::Spectrum* pActiveSpectrum)
			 * @brief This method is called when a spectrum item is clicked in the sample tree.
			 * @param pActiveSpectrum newly-active spectrum
			 */
			void SetActiveSpectrum(kome::objects::Spectrum* pActiveSpectrum);

			/**
			 * @fn void SetActiveChromatogram(kome::objects::Chromatogram* pActiveChromatogram)
			 * @brief This method is called when a chromatogram item is clicked in the sample tree.
			 * @param pActiveChromatogram newly-active chromatogram
			 */
			void SetActiveChromatogram(kome::objects::Chromatogram* pActiveChromatogram);

			/**
			 * @fn void SetActiveGroup(kome::objects::Group* pActiveGroup)
			 * @brief This method is called when a group item is clicked in the sample tree.
			 * @param pActiveGroup newly-active group
			 */
			void SetActiveGroup(kome::objects::DataGroupNode* pActiveGroup);

			/**
			 * @fn void ItemSelected(const long lSelectedItem, const std::string& sKey, const std::string& sValue)
			 * @brief This method is called when an item in the details grid is selected.
			 * @param lSelectedItem the selected row number
			 * @param sKey text of the Key column
			 * @param sValue text of the Value column
			 */
			void ItemSelected(const long lSelectedItem,
							const std::string& sKey, const std::string& sValue);

			/**
			 * @fn void NoListItemSelected()
			 * @brief This method is called when no item is selected in the grid view, to enable/disable controls appropriately.
			 */
			void NoListItemSelected();

			/**
			 * @fn bool CheckForUnsavedChangesAndValidate()
			 * @brief This method is called when a different item is selected in the sample tree or when OK is clicked, to check for unsaved changes and validate.
			 * @return true / false
			 */
			bool CheckForUnsavedChangesAndValidate();
				
			/**
			 * @fn  onButtonNew( wxCommandEvent& evt )
			 * @brief This function is called when the New button has been clicked
			 * @param evt the command event
			 */
			void onButtonNew( wxCommandEvent& evt );

			/**
			 * @fn  onButtonDelete( wxCommandEvent& evt )
			 * @brief This function is called when the Delete button has been clicked
			 * @param evt the command event
		 	 */
			void onButtonDelete( wxCommandEvent& evt );

			/**
			 * @fn void setEdit( bool bedit )
			 * @brief set edit
			 * @param bedit
			 */
			void setEdit( bool bedit ){ m_bIsEdited = bedit; }
			
			/**
			 * @fn bool getEdit()
			 * @brief get edit
			 * @param bedit
			 */
			bool getEdit(){ return m_bIsEdited; }
				
		public:
			/**
			 * @fn kome::objects::Spectrum* getActiveSpec()
			 * @brief get active spectrum
			 * @return active spectrum
			 */
			kome::objects::Spectrum* getActiveSpec(){ return m_pActiveSpectrum; }

			/**
			 * @fn kome::objects::Chromatogram* getActiveChrom()
			 * @brief get active chromatogram
			 * @return active chromatogram
			 */
			kome::objects::Chromatogram* getActiveChrom(){ return m_pActiveChromatogram; }

			/**
			 * @fn kome::objects::DataGroupNode* getActiveGroup()
			 * @brief get active group
			 * @return active group
			 */
			kome::objects::DataGroupNode* getActiveGroup(){ return m_pActiveGroup; }

			/**
			 * @fn void getProperties( kome::core::Properties& props )
			 * @brief get properties
			 * @param props
			 */
			void getProperties( kome::core::Properties& props );
			
		protected:
	

			/**
			 * @fn bool EmptySample();
			 * @brief determine if the sample has no chro / spectra
			 * @return true / false
			 */
			bool EmptySample(kome::objects::Sample* sample);

			/**
			 * @fn virtual wxSizer* createMainSizer();
			 * @brief creates main sizer (override method)
			 * @return field sizer
			 */
			virtual wxSizer* createMainSizer();

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
			 * @fn  onKeyEditChanged( wxCommandEvent& evt )
			 * @brief This function is called when the Key edit field has been changed
			 * @param evt the command event
			 */
			void onKeyEditChanged( wxCommandEvent& evt );

			/**
			 * @fn  onValueEditChanged( wxCommandEvent& evt )
			 * @brief This function is called when the Value edit field has been changed
			 * @param evt the command event
			 */
			void onValueEditChanged( wxCommandEvent& evt );
				
			/**
			 * @fn SampleTreeCtrl* getTreeCtrl()
			 * @brief gets tree ctrl
			 * @return the pointer to tree ctrl
			 */
			SampleTreeCtrl* getTreeCtrl();

			/**
			 * @fn void addSample( kome::objects::Sample* sample )
			 * @brief adds sample
			 * @param sample sample object to be adde
			 */
			void addSample( kome::objects::Sample* sample );

			/**
			 * @fn void updateNodes( kome::objects::DataGroupNode* group )
			 * @brief updates nodes
			 * @param group group
			 */
			void updateNodes( kome::objects::DataGroupNode* group );
		protected:
			/**
			 * @fn wxTreeItemId addTreeItem( kome::objects::DataGroupNode* group, wxTreeItemId& parentId )
			 * @brief adds tree item
			 * @param group parent spectrum group
			 * @param parentId parent tree item ID
			 */
			wxTreeItemId addTreeItem( kome::objects::DataGroupNode* group, wxTreeItemId& parentId );

			/**
			 * @fn void addChildren( kome::objects::DataGroupNode* group, wxTreeItemId& groupId )
			 * @brief adds child nodes to tree item
			 * @param group group
			 * @param groupId group tree item ID
			 */
			void addChildren( kome::objects::DataGroupNode* group, wxTreeItemId& groupId );

			/**
			 * @fn DetailsListCtrl* getListCtrl()
			 * @brief gets list ctrl
			 * @return the pointer to list ctrl
			 */
			DetailsListCtrl* getListCtrl();

			/**
			 * @fn void selectGroup( kome::objects::DataGroupNode* group )
			 * @brief selects spectrum group
			 * @param group selected spectrum group object
			 */
			void selectGroup( kome::objects::DataGroupNode* group );

			/**
			 * @fn void selectChrom( kome::objects::Chromatogram* chrom )
			 * @brief selects chromatogram
			 * @param chrom selected chromatogram object
			 */
			void selectChrom( kome::objects::Chromatogram* chrom );

			/**
			 * @fn void selectSpec( kome::objects::Spectrum* spectrum )
			 * @brief selects spectrum
			 * @param spectrum selected spectrum object
			 */
			void selectSpec( kome::objects::Spectrum* spectrum );

			/**
			 * @fn void deleteItems()
			 * @brief deletes items
			 */
			void deleteItems();

			/**
			 * @fn void newItem()
			 * @brief adds a new item
			 */
			void newItem();

			/**
			 * @fn void updateItem()
			 * @brief updates an item
			 */
			void updateItem(const long lRow, const int iColumn, const char *s);

			/**
			 * @fn void updateSpectrum( kome::objects::Spectrum* spectrum )
			 * @brief updates spectrum
			 */
			void updateSpectrum( kome::objects::Spectrum* spectrum );

		public:
			/**
			 * @fn void validateUserProperties()
			 * @brief validate user properties
			 */
			bool validateUserProperties();
		
		private:
			DECLARE_EVENT_TABLE()
		};
	}
}

#endif	//  __KOME_PRODUCT_ION_SELECTION_DIALOG_H__

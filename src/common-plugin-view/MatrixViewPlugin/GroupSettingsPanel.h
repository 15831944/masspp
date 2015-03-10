/**
 * @file GroupSettingsPanel.h
 * @brief interfaces of GroupSettingsPanel class
 *
 * @author H.Parry
 * @date 2011.04.28
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation. All rights reserved.
 */


#ifndef __KOME_MATRIX_GROUPSETTINGS_PANEL_H__
#define __KOME_MATRIX_GROUPSETTINGS_PANEL_H__

namespace kome {
	namespace matrix {
		namespace view {
			
			class GroupsGridCtrl;

			class SamplesGridCtrl;

			class CreatePeakMatrixDialog;

			/**
			 * @struct SampleInfo
			 * @brief sample information
			 */
			struct SampleInfo {
				std::string path;
				COLOR color;
				int category;
				std::string categoryName;
			};

			/**
			  * @struct CategoryInfo
			 * @brief sample information
			 */
			typedef struct {
				std::string name;
				int id;
				COLOR color;
				std::vector< SampleInfo > paths;
			} CategoryInfo;
			
			/**
			 * @class GroupSettingsPanel
			 * @brief category settings panel class
			 */
			class GroupSettingsPanel : public kome::window::PeakMatrixSettingsPageBase {
				public:
					/**
					 * @fn GroupSettingsPanel( wxWindow* parent, QuantitationDialog* pParentDlg )
					 * @brief constructor
					 * @param parent parent window
					 * @param pParentDlg parent dialog
					 */
					GroupSettingsPanel( wxWizard* parent );

					/**
					 * @fn virtual ~GroupSettingsPanel()
					 * @brief destructor
					 */
					virtual ~GroupSettingsPanel(void);

				protected:
					// Categories
					/** group grid list ctrl */
					GroupsGridCtrl* m_pCategoryList;
					/** new button */
					wxBitmapButton*	m_pCategoryNewButton;
					/** edit button */
					wxBitmapButton*	m_pCategoryEditButton;
					/** delete button */
					wxBitmapButton*	m_pCategoryDeleteButton;
					/** up button */
					wxBitmapButton*	m_pCategoryUpButton;
					/** down button */
					wxBitmapButton*	m_pCategoryDownButton;

					// Samples
					/** category */
					int m_categoryID;
					/** sample grid list ctrl */
					SamplesGridCtrl* m_pSampleList;
					/** add button */
					wxBitmapButton*	m_pSampleAddButton;
					/** delete button */
					wxBitmapButton*	m_pSampleDeleteButton;
					/** up button */
					wxBitmapButton*	m_pSampleUpButton;
					/** down button */
					wxBitmapButton*	m_pSampleDownButton;
					/** color button */
					wxBitmapButton*	m_pSampleColorButton;

					/** standard sample combobox */
					wxComboBox*						m_stdCombo; 

					/** path set */
					std::set< std::string > m_pathSet;
					/** file name set */
					std::set< std::string > m_filenameSet;

					// Category and sample information loaded into memory.
					std::vector< CategoryInfo > m_categoryArray;

					/** Flag to indicate whether groups or samples have been added or deleted. */
					bool m_bGroupSampleAdditionDeletion;
					
					/** color */
					COLOR m_color;
					/** parent dialog */
					CreatePeakMatrixDialog* m_pParentDlg;
				public:
					/**
				 	 * @fn virtual bool PreApplyValidationCheck(std::string& sError)
					 * @brief Perform Apply-time validation checks (i.e. checks that must not be done in IsValid function)
					 * @param sError error string from a single panel's pre-apply validation
					 * @return If validation succeeded, return true, otherwise false.
					*/
					virtual bool PreApplyValidationCheck(std::string& sError);

					/**
					 * @fn void SavePanelSettings(kome::core::Progress *pProgress)
					 * @brief This method is called when saving values.
					 * @param pProgress progress
					 * @return If saving values failed, this method returns false.
					 */
					virtual bool SavePanelSettings(kome::core::Progress *pProgress);

					/**
					 * @fn virtual bool IsValid()
					 * @brief validate panel settings
					 * @return If validation failed, this method returns false.
					 */
					virtual bool IsValid();

					/**
					* @fn void GetMatrixSamples(std::vector< std::string > *pSamples)
					* @brief gets the list of samples that are currently registered on the Group Settings panel.
					* @param pSamples array of samples
					*/
					void GetMatrixSamples(std::vector< std::string > *pSamples);

					/**
					 * @fn int GetTotalNumberOfSamples()
					 * @brief This method returns the total number of samples in all categories.
					 * @return total number of samples
					 */
					int GetTotalNumberOfSamples();

				public:
					/**
					 * @fn struct CategoryInfo* getSelectedCategoryInfo()
					 * @brief gets a pointer to selected category CategoryInfo struct
					 * @return pointer to selected category CategoryInfo struct
					 */
					// get selected category CategoryInfo*
					CategoryInfo* getSelectedCategoryInfo();

					/**
					 * @fn SelectCategoryItem( const char* name )
					 * @brief selects the category item with the specified name
					 * @param name the category name to select
					 */
					void SelectCategoryItem( const char* name );
					
					/**
					 * @fn SelectSampleItem( const char* name )
					 * @brief selects the sample item with the specified name
					 * @param name the sample name to select
					 */
					void SelectSampleItem( const char* name );

				protected:
					/**
					 * @fn void updateCategoryList()
					 * @brief updates list
					 */
					void updateCategoryList();							

					/**
					 * @fn int getSelectedCategoryArrayIndex()
					 * @brief gets selected category array index
					 * @return selected category array index
					 */
					int getSelectedCategoryArrayIndex();

					/**
					 * @fn int getSelectedCategoryID()
					 * @brief gets selected category ID
					 * @return selected category ID
					 */
					int getSelectedCategoryID();

					/**
					 * @fn int getTempCategoryID()
					 * @brief gets an unused temporary category ID
					 * @return temporary category ID
					 */
					int getTempCategoryID();

					/**
					 * @fn CategoryNameExists( const char* name )
					 * @brief checks if a category with the specified name exists
					 * @param name the category name to check
					 * @return true if a category with this name exists, otherwise false
					 */
					bool CategoryNameExists( const char* name );

					/**
					 * @fn bool openEditDialog( const bool editMode )
					 * @brief opens edit dialog
					 * @param editMode true: edit mode, false: append mode
					 * @return true if a change was made, otherwise false
					 */
					bool openEditDialog( const bool editMode );
					
					/**
					 * @fn void setColor()
					 * @brief set default color
					 */
					void setColor();
				public:

					/**
					 * @fn virtual void createInnerSizer(wxStaticBoxSizer* pInnerSizer)
					 * @brief creates inner sizer
					 * @param pInnerSizer inner sizer
					 */
					virtual void createInnerSizer(wxStaticBoxSizer* pInnerSizer);

					/**
					 * @fn virtual bool TransferDataFromWindow()
					 * @brief transfer window control data to value
					 * @return If a transfer failed, this method returns false.
					 */
					virtual bool TransferDataFromWindow();

					/**
					 * @fn void updateStandardSamples(bool bReadFromINIfile = true)
					 * @brief Update standard samples on this panel
					 * @param bReadFromINIfile whether to read from the INI file
					 */
					virtual void updateStandardSamples(bool bReadFromINIfile = true);
					
					
					/**
					 * @fn void RefreshPanelContentsForMatrix()
					 * @brief refresh panel contents for matrix
					 */
					virtual void RefreshPanelContentsForMatrix();

					/**
					 * @fn bool IsSelectedStandardSample(const char* sSample)
					 * @brief check whether a sample is selected as the standard sample on this panel.
					 * @param sSample sample filename
					 * @return If the sample is selected as a standard sample, return true.
					 */
					virtual bool IsSelectedStandardSample(const char* sSample);
					
				protected:
					/**
					 * @fn void updateSettings()
					 * @brief refresh settings for matrix
					 */
					void updateSettings();
					
					/**
					 * @fn void LoadCurrentMatrixCategorySamplesInfo()
					 * @brief load current matrix categories - samples information
					 */
					void LoadCurrentMatrixCategorySamplesInfo();

				public:
					/**
					 * @fn void onDeleteCategory( wxCommandEvent& evt )
					 * @brief This method is called when the delete button is clicked.
					 * @param evt event information
					 */
					void onDeleteCategory( wxCommandEvent& evt );

					/**
					 * @fn void onDeleteSample( wxCommandEvent& evt )
					 * @brief This method is called when "Delete" button is clicked.
					 * @param evt button event information
					 */
					void onDeleteSample( wxCommandEvent& evt );

					/**
					 * @fn void onColorSample( wxCommandEvent& evt )
					 * @brief This method is called when "Color" button is clicked.
					 * @param evt button event information
					 */
					void onColorSample( wxCommandEvent& evt );

					/**
					 * @fn CategoryInfo getGategoryInfo( int data )
					 * @brief get gategory info
					 * @param data
					 */ 
					CategoryInfo getGategoryInfo( int data );

					/**
					 * @fn CategoryInfo* getSelectGroup()
					 * @brief get select group
					 * @return select group info
					 */
					CategoryInfo* getSelectGroup();
					/**
					 * @fn void onSelectCategoryItem( wxListEvent& evt )
					 * @brief This method is called when a category item is clicked.
					 * @param evt list event information
					 */
					void onSelectCategoryItem( wxListEvent& evt );

					/**
					 * @fn void onDeselectCategoryItem( wxListEvent& evt )
					 * @brief This method is called when a category item is deselected.
					 * @param evt list event information
					 */
					void onDeselectCategoryItem( wxListEvent& evt );

					/**
					 * @fn void onCategoryDoubleClicked( wxListEvent& evt )
					 * @brief This method is called when category is double clicked.
					 * @param[in] evt mouse event
					 */
					void onCategoryDoubleClicked( wxListEvent& evt );

					/**
					 * @fn void onUpCategory( wxCommandEvent& evt )
					 * @brief This method is called when the up button is clicked.
					 * @param evt event information
					 */
					void onUpCategory( wxCommandEvent& evt );

					/**
					 * @fn void onDownCategory( wxCommandEvent& evt )
					 * @brief This method is calleed when the down button is clicked.
					 * @param evt event information
					 */
					 void onDownCategory( wxCommandEvent& evt );		

					/**
					 * @fn void onSelectSampleItem( wxListEvent& evt )
					 * @brief This method is called when a sample item is clicked.
					 * @param evt list event information
					 */
					void onSelectSampleItem( wxListEvent& evt );

					/**
					 * @fn void onDeselectSampleItem( wxListEvent& evt )
					 * @brief This method is called when a sample item is deselected.
					 * @param evt list event information
					 */
					void onDeselectSampleItem( wxListEvent& evt );

					/**
					 * @fn void onAddSample( wxCommandEvent& evt )
					 * @brief This method is called when "Add" button is clicked.
					 * @param evt button event information
					 */
					void onAddSample( wxCommandEvent& evt );	

					/**
					 * @fn void onUpSample( wxCommandEvent& evt )
					 * @brief This method is called when "Up" button is clicked.
					 * @param evt button event information
					 */
					void onUpSample( wxCommandEvent& evt );

					/**
					 * @fn void onDownSample( wxCommandEvent& evt )
					 * @brief This method is called when "Down" button is clicked.
					 * @param evt button event information
					 */
					void onDownSample( wxCommandEvent& evt );

					/**
					 * @fn void onNewCategory( wxCommandEvent& evt )
					 * @brief This method is called when the new button is clicked.
					 * @param evt event information
					 */
					void onNewCategory( wxCommandEvent& evt );

					/**
					 * @fn void onEditCategory( wxCommandEvent& evt )
					 * @brief This method is called when the edit button is clicked.
					 * @param evt event information
					 */
					void onEditCategory( wxCommandEvent& evt );			
				protected:
					/**
					 * @fn void onSize( wxSizeEvent& evt )
					 * @brief This method is called when the dialog size is changed
					 * @param evt size event information
					 */
					void onSize( wxSizeEvent& evt );					

					/**
					 * @fn void NoCategoryItemsSelected()
					 * @brief This method is called when no category item is selected.
					 */
					void NoCategoryItemsSelected();

					/**
					 * @fn void NoSampleItemsSelected()
					 * @brief This method is called when no sample item is selected.
					 */
					void NoSampleItemsSelected();
					
					/**
					 * @fn void onSelectStandard( wxCommandEvent& evt )
					 * @brief This method is called when an item in the Standard Sample combo list has been selected.
					 * @param evt event information
					 */
					void onSelectStandard( wxCommandEvent& evt );

					/**
					 * @fn void UpdateSampleList()
					 * @brief This method is called when the selected category is changed.
					 */
					void UpdateSampleList();		

				private:
					DECLARE_EVENT_TABLE()
			};
		}
	}
}

#endif		// __KOME_MATRIX_GROUPSETTINGS_PANEL_H__

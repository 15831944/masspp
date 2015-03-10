/**
 * @file GroupSettingsGridCtrl.h
 * @brief interfaces of GroupSettingsGridCtrl class
 * 
 * @author M.Izumi
 * @date 2012.12.07
 *
 * Copyright(C) 2006-2014 Shimadzu Corporation. All rights reserved.
 */

#ifndef __KOME_MATRIX_GROUPSETTINGS_GRIDCTRL_H__
#define __KOME_MATRIX_GROUPSETTINGS_GRIDCTRL_H__

namespace kome {
	namespace matrix {
		namespace view {

			class GroupSettingsPanel;
			CategoryInfo;
			/**
			 * @class GroupsGridCtrl
			 * @brief Groups Grid Ctrl class
			 */
			class GroupsGridCtrl : public kome::window::EditGridListCtrl {
			public:
				/**
				 * @fn GroupsGridCtrl( wxWindow* pParent )
				 * @brief constructor
				 * @param pParent parent window
				 */
				GroupsGridCtrl( GroupSettingsPanel* pParent );

				/**
				 * @fn virtual ~GroupsGridCtrl()
				 * @brief destructor
				 */
				virtual ~GroupsGridCtrl();
			

			protected:
				/** parent panel*/
				GroupSettingsPanel* m_pParent;
			protected:

				/**
				 * @fn virtual void onActivateData( const long long data )
				 * @brief This method is called when the data is activated (abstract method)
				 * @param[in] data activated data
				 */
				virtual void onActivateData( const long long data );
			
				/**
				 * @fn virtual bool onDeleteData( const long long data )
				 * @brief This method is called when a item is deleted.
				 * @param[in] data deleted item data
				 * @return If true, succeeded to delete the item.
				 */
				virtual bool onDeleteData( const long long data );

				/**
				 * @fn virtual std::string getString( const long long data, const int column ) const
				 * @brief gets character string value
				 * @param[in] data data
				 * @param[in] column column number
				 */
				virtual std::string getString( const long long data, const int column )const;
		
				/**
				* @fn void onCategoryDoubleClicked( wxListEvent& evt )
				* @brief This method is called when category is double clicked.
				* @param[in] evt mouse event
				*/
				void onCategoryDoubleClicked( wxListEvent& evt );

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
				* @fn void onAddSample( wxCommandEvent& evt )
				* @brief This method is called when "New" button is clicked.
				* @param evt button event information
				*/
				void onAddCategory( wxCommandEvent& evt );

				/**
				* @fn void onEditCategory( wxCommandEvent& evt )
				* @brief This method is called when "Edit" button is clicked.
				* @param evt button event information
				*/
				void onEditCategory( wxCommandEvent& evt );

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
			private:
				DECLARE_EVENT_TABLE()
			};


			// Sample Grid Ctrl class
			class SamplesGridCtrl : public kome::window::EditGridListCtrl {
			public:
				/**
				 * @fn SamplesGridCtrl( wxWindow* pParent )
				 * @brief constructor
				 * @param pParent parent window
				 */
				SamplesGridCtrl( GroupSettingsPanel* pParent );

				/**
				 * @fn virtual ~SamplesGridCtrl()
				 * @brief destructor
				 */
				virtual ~SamplesGridCtrl();

			protected:
				/** parent panel */
				GroupSettingsPanel* m_pParent;
			protected:

				/**
				 * @fn virtual void onActivateData( const long long data )
				 * @brief This method is called when the data is activated (abstract method)
				 * @param[in] data activated data
				 */
				virtual void onActivateData( const long long data );
			
				/**
				 * @fn virtual bool onDeleteData( const long long data )
				 * @brief This method is called when a item is deleted.
				 * @param[in] data deleted item data
				 * @return If true, succeeded to delete the item.
				 */
				virtual bool onDeleteData( const long long data );
								
				/**
				 * @fn virtual std::string getString( const long long data, const int column ) const
				 * @brief gets character string value
				 * @param[in] data data
				 * @param[in] column column number
				 */
				virtual std::string getString( const long long data, const int column )const;
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
				* @fn void onDeleteSample( wxCommandEvent& evt )
				* @brief This method is called when "Delete" button is clicked.
				* @param evt button event information
				*/
				void onDeleteSample( wxCommandEvent& evt );

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
		
			private:
				DECLARE_EVENT_TABLE()
			};

		}
	}
}


#endif // __KOME_MATRIX_GROUPSETTINGS_GRIDCTRL_H__

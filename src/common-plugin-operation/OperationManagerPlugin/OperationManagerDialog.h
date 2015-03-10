/**
 * @file OperationManagerDialog.h
 * @brief interfaces of OperationManagerDialog class
 *
 * @author S.Tanaka
 * @date 2007.03.09
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_OPERATION_MANAGER_DIALOG_H__
#define __KOME_OPERATION_MANAGER_DIALOG_H__

#include <wx/wx.h>

namespace kome {
	namespace operation {

		/**
		 * @class OperationManagerDialog
		 * @brief operation management dialog
		 */
		class OperationManagerDialog : public kome::window::StandardDialog {
		public:
			/**
			 * @fn OperationManagerDialog( wxWindow* parent )
			 * @brief constructor
			 * @param parent parent window
			 */
			OperationManagerDialog( wxWindow* parent );

			/**
			 * @fn virtual ~OperationManagerDialog()
			 * @brief destructor
			 */
			virtual ~OperationManagerDialog();

		protected:
			/**
			 * @typedef DataType
			 * @brief data type
			 */
			typedef enum {
				TYPE_SPEC,
				TYPE_CHROM
			} DataType;

			/**
			 * @union ObjectData
			 * @brief object
			 */
			union ObjectData {
				kome::objects::Spectrum* spec;
				kome::objects::Chromatogram* chrom;
			};

			/** list box */
			wxListBox* m_listBox;
			
			/** [Delete] Button */
			wxButton* m_delButton;

			/** operations */
			std::vector< kome::objects::XYDataOperation* > m_operations;

			/** remove operations */
			std::vector< kome::objects::XYDataOperation* > m_removeOperations;

			/** type */
			DataType m_type;

			/** data */
			ObjectData m_data;

		public:
			/**
			 * @fn void setSpectrum( kome::objects::Spectrum* spec )
			 * @brief sets spectrum data
			 * @param spec spectrum data
			 */
			void setSpectrum( kome::objects::Spectrum* spec );

			/**
			 * @fn void setChromatogram( kome::objects::Chromatogram* chrom )
			 * @brief sets chromatogram data
			 * @param chrom chromatogram
			 */
			void setChromatogram( kome::objects::Chromatogram* chrom );

			/**
			 * @fn unsigned int getNumberOfDeleteOperations()
			 * @brief gets the number of operations to be deleted
			 * @return the number of oeprations to be deleted
			 */
			unsigned int getNumberOfDeleteOperations();

			/**
			 * @fn kome::objects::XYDataOperation* getDeleteOperation( const unsigned int index )
			 * @brief gets operation to be deleted
			 * @param index operation to be deleted index
			 * @return operation to be deleted (If the index is illegal, this method returns NULL.)
			 */
			kome::objects::XYDataOperation* getDeleteOperation( const unsigned int index );

		protected:
			/**
			 * @fn void getOperations()
			 * @brief gets operations
			 */
			void getOperations();

		protected:
			/**
			 * @fn virtual wxSizer* createMainSizer()
			 * @brief creates main sizer
			 * @return main sizer
			 */
			virtual wxSizer* createMainSizer();
			/**
			 * @fn virtual wxSizer* createButtonSizer()
			 * @brief creates button sizer
			 * @return button sizer
			 */
			virtual wxSizer* createButtonSizer();
		protected:
			/**
			 * @fn void onDelete( wxCommandEvent& evt )
			 * @brief This method called on delete button event
			 * @param evt event information
			 */
			void onDelete( wxCommandEvent& evt );
			
			/**
			 * @fn void onDeleteAll( wxCommandEvent& evt )
			 * @brief This method called on Delete All button event
			 * @param evt event information
			 */
			void onDeleteAll( wxCommandEvent& evt );

			/**
			 * @fn void onDeleteUpdate( wxUpdateUIEvent& evt )
			 * @brief updates the delete button
			 * @param[in] evt update UI event
			 */
			void onDeleteUpdate( wxUpdateUIEvent& evt );

			/**
			 * @fn void onDeleteAllUpdate( wxUpdateUIEvent& evt )
			 * @brief updates the delte all button
			 * @parma[in] evt update UI event
			 */
			void onDeleteAllUpdate( wxUpdateUIEvent& evt );
			
			/**
			 * @fn void onOK( wxCommandEvent& evt )
			 * @brief This method is called when the OK button is clicked.
			 * @param evt button event
			 */
			void onOK( wxCommandEvent& evt );
			
			/**
			 * @fn void onCancel( wxCommandEvent& evt )
			 * @brief This button is called when the Cancel button is clicked.
			 * @param evt button event
			 */
			void onCancel( wxCommandEvent& evt );
						
			/**
			 * @fn void onSelect( wxCommandEvent& evt )
			 * @brief リストを選択時、DeleteボタンをEnableにする
			 * @param evt button event
			 */
			void onSelect( wxCommandEvent& evt );

			/**
			 * @fn void setDelFunction( 
				std::vector< int > selection,
				std::set< kome::objects::XYDataOperation* > rmOperationSet
				)
			 * @brief 選択された項目の削除
			 * @param evt button event
			 */
			void setRemoveFunction( 
				std::vector< int > selection,
				std::set< kome::objects::XYDataOperation* > rmOperationSet
				);
			
			/**
			 * @fn void onContextMenu( wxContextMenuEvent& evt )
			 * @brief This method is called on requesting context menu
			 * @param[in] evt context menu event information
			 */
			void onContextMenu( wxContextMenuEvent& evt );

			/**
			 * @fn virtual wxMenu* createMenu()
			 * @brief creates menu
			 * @return created menu
			 */
			virtual wxMenu* createMenu();

			/**
			 * @fn void onSelectAll( wxCommandEvent& evt )
			 * @brief This method is called when "Select All" is selected.
			 * @param[in] evt event information
			 */
			void onSelectAll( wxCommandEvent& evt );

			/**
			 * @fn void onDeselectAll( wxCommandEvent& evt )
			 * @brief This method is called when "Deselect All" is selected.
			 * @param[in] evt event information
			 */
			void onDeselectAll( wxCommandEvent& evt );

			/**
			 * @fn void selectAll()
			 * @brief selects all items
			 */
			void selectAll();

			/**
			 * @fn void deselectAll()
			 * @brief deselects all items
			 */
			void deselectAll();

			/**
			 * @fn void onUpdateUIPopupMenu( wxUpdateUIEvent& evt )
			 * @brief sets the status of popup-menu
			 * @param[in] evt update UI event information
			 */
			void onUpdateUIPopupMenu( wxUpdateUIEvent& evt );

		private:
			DECLARE_EVENT_TABLE();
		};
	}
}


#endif		// __KOME_OPERATION_MANAGER_DIALOG_H__

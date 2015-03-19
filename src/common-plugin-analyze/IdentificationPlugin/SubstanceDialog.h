/**
 * @file SubstanceDialog.h
 * @brief interfaces of GridListCtrl class
 *
 * @author S.Tanaka
 * @date 2008.09.09
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_IDENT_SUBSTANCE_DIALOG_H__
#define __KOME_IDENT_SUBSTANCE_DIALOG_H__


#include <wx/wx.h>


namespace kome {
	namespace ident {

		class GridListCtrl;
		class PeakInfoDialog;
		class SubstanceListCtrl;
		class TreeData;

		/**
		 * @class SubstanceDialog
		 * @brief grid list dialog class
		 */
		class SubstanceDialog : public kome::window::StandardDialog {
		public:
			/**
			 * @fn SubstanceDialog( wxWindow* parent, const char* title, const bool canDelete = false )
			 * @brief constructor
			 * @param[in] parent parent window
			 * @param[in] title dialog title
			 * @param[in] canDelete delete button flag
			 */
			SubstanceDialog( wxWindow* parent, const char* title, const bool canDelete = false );

			/**
			 * @fn virtual ~SubstanceDialog()
			 * @brief destructor
			 */
			virtual ~SubstanceDialog();

		protected:
			/** grid list */
			SubstanceListCtrl* m_list;

			/** tree */
			wxTreeCtrl* m_tree;

			/** spectrum map */
			std::map< int, kome::ident::SpecInfo > m_specMap;

			/** hit map */
			std::map< int, std::vector< kome::ident::HitInfo > > m_hitsMap;

			/** substance map */
			std::map< int, kome::ident::SubstanceInfo > m_substanceMap;

			/** substance ID map */
			std::map< int, wxTreeItemId > m_substanceIdMap;

			/** root node */
			wxTreeItemId m_root;

		private:
			/**
			 * @struct SubstanceButton
			 * @brief Substance Button
			 */
			struct SubstanceButton {
				kome::plugin::PluginFunctionItem* fun;
				wxIcon icon0;
				wxIcon icon1;
				wxBitmapButton* button;
				PeakInfoDialog* dialog;
				int pos;
			};

			/** Substance Button */
			std::vector< SubstanceButton > m_substanceButton;

			/**
			 * @struct SpectrumButton
			 * @brief Spectrum Button
			 */
			struct SpectrumButton {
				kome::plugin::PluginFunctionItem* fun;
				wxIcon icon0;
				wxIcon icon1;
				wxBitmapButton* button;
				PeakInfoDialog* dialog;
				int pos;
			};

			/** Spectrum Button */
			std::vector< SpectrumButton > m_spectrumButton;

			/** Search ID */
			long long m_llSearchID;

		public:
			/**
			 * @fn void updateDialog()
			 * @brief updates dialog
			 */
			void updateDialog();

		protected:
			/**
			 * @fn virtual wxSizer* createMainSizer()
			 * @brief creates main sizer (override method)
			 * @return main sizer
			 */
			virtual wxSizer* createMainSizer();

			/**
			 * @fn virtual wxSizer* createButtonSizer()
			 * @brief creates button sizer (override method)
			 * @return button sizer
			 */
			virtual wxSizer* createButtonSizer();

		protected:
			/**
			 * @fn void saveData( wxCommandEvent& evt )
			 * @brief saves data
			 * @param[in] evt event information
			 */
			void saveData( wxCommandEvent& evt );

			/**
			 * @fn void deleteData( wxCommandEvent& evt )
			 * @brief deletes data
			 * @param[in] evt event information
			 */
			void deleteData( wxCommandEvent& evt );

			/**
			 * @fn void updateUI( wxUpdateUIEvent& evt )
			 * @brief updates UI
			 * @param[in] evt evnet information
			 */
			void updateUI( wxUpdateUIEvent& evt );

		protected:
			/**
			 * @fn std::string getStatusNameSubstance( const unsigned int idx )
			 * @brief gets the status name
			 * @param[in] idx peak action index
			 * @return status name
			 */
			std::string getStatusNameSubstance( const unsigned int idx );	// @date 2013/01/30 <Add> OKADA

			/**
			 * @fn std::string getStatusNameSpectrum( const unsigned int idx )
			 * @brief gets the status name
			 * @param[in] idx peak action index
			 * @return status name
			 */
			std::string getStatusNameSpectrum( const unsigned int idx );	// @date 2013/01/30 <Add> OKADA

			/**
			 * @fn void onSubstanceButton( wxCommandEvent& evt )
			 * @brief This method is called when a Substance button is clicked.
			 *@ param[in] evt event information
			 */
			void onSubstanceButton( wxCommandEvent& evt );

			/**
			 * @fn void onSubstanceUpdateUIButton( wxUpdateUIEvent& evt )
			 * @brief updates the UI of the Substance button
			 * @parma[in] evt event information
			 */
			void onSubstanceUpdateUIButton( wxUpdateUIEvent& evt );

			/**
			 * @fn void onSpectrumButton( wxCommandEvent& evt )
			 * @brief This method is called when a Spectrum button is clicked.
			 *@ param[in] evt event information
			 */
			void onSpectrumButton( wxCommandEvent& evt );

			/**
			 * @fn void onSpectrumUpdateUIButton( wxUpdateUIEvent& evt )
			 * @brief updates the UI of the Spectrum button
			 * @parma[in] evt event information
			 */
			void onSpectrumUpdateUIButton( wxUpdateUIEvent& evt );

		public:
			/**
			 * @fn void clearStatus( const char szInitStr[] = "" )
			 * @brief clear status
			 * @param[in] szInitStr initial string
			 */
			void clearStatus( const char szInitStr[] = "" );

		public:
			/**
			 * @fn void SubstanceDialog::setSearchID( int nSearchID )
			 * @brief set Search ID 
			 * @param[in] nSearchID Search ID 
			 */
			void setSearchID( long long  nSearchID );

			/**
			 * @fn int searchSubstanceParent( int substanceID )
			 * @brief search substance parent 
			 * @param[in] substanceID substance ID
			 * @return substance parentId
			 */
			int searchSubstanceParent( int substanceID );

			/**
			 * @fn int void onSelectTree( wxTreeEvent& evt )
			 * @brief select tree ctrl
			 * @param[in] event id
			 */
			void onSelectTree( wxTreeEvent& evt );
                      			
		private:
			/**
			 * @fn void makeTree( const int substance )
			 * @brief makes tree
			 * @param[in] substance
			 */
			void makeTree( const int substance );

			/**
			 * @fn wxTreeItemId addSubstanceNode( const int substance, wxTreeItemId parent )
			 * @brief adds substance node
			 * @param[in] subsntace substance ID
			 * @param[in] parent parent node
			 * @return tree item ID
			 */
			wxTreeItemId addSubstanceNode( const int substance, wxTreeItemId parent ); 


		/**
		  * @fn char* getIconPath( void )
		  * @brief get icon path 
		  * @return icon path
		  */
		const char* getIconPath( void );

		/** selected tree id */
		TreeData* m_itemIdSelected;	// 選択されたtreeのidを保存
		
		public:

			/** Substanceダイアログを保存 */
			static SubstanceDialog* m_dlgSubstance;

		private:
			DECLARE_EVENT_TABLE();
		};

		/**
		 * @class TreeData
		 * @brief tree data
		 */
		class TreeData : public wxTreeItemData {
		public:
			/**
			 * @typedef NodeType
			 * @brief node type
			 */
			typedef enum {
				TYPE_SPECTRUM,
				TYPE_SUBSTANCE
			} NodeType;

		public:
			/**
			 * @fn TreeData( NodeType type )
			 * @brief constructor
			 * @param[in] type node type
			 */
			TreeData( NodeType type );

			/**
			 * @fn virtual ~TreeData()
			 * @brief destructor
			 */
			virtual ~TreeData();


		public:
			NodeType m_type;

			int m_id;

		public:
			kome::core::Properties m_props;
		};
	}
}


#endif	// __KOME_WINDOW_GRID_LIST_DIALOG_H__

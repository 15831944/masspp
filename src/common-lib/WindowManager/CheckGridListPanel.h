/**
 * @file CheckGridListPanel.h
 * @brief interfaces of CheckGridListPanel class
 *
 * @author S.Tanaka
 * @date 2011.04.28
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation All rights reserved.
 */


#ifndef __KOME_WINDOW_CHECK_GRID_LIST_PANEL_H__
#define __KOME_WINDOW_CHECK_GRID_LIST_PANEL_H__


#include "HelpGetterPanel.h"
#include "CheckGridListCtrl.h"


namespace kome {
	namespace window {

		/**
		 * @class CheckGridListPanel
		 * @brief standard panel
		 */
		class WINDOW_MANAGER_CLASS CheckGridListPanel : public HelpGetterPanel {
		public:
			/**
			 * @typedef enum AllButtonAlign
			 * @brief align of all button
			 */
			typedef enum {
				NONE_ALL = 0,
				TOP_ALL = 1,
				BOTTOM_ALL = 2
			} AllButtonAlign;

		public:
			/**
			 * @fn CheckGridListPanel(
						wxWindow* parent,
						const int id = -1,
						AllButtonAlign align = TOP_ALL,
						const char* allLabel = "All",
						const bool showNum = true
					)
			 * @brief constructor
			 * @param[in] parent parent window
			 * @param[in] id window ID
			 * @param[in] align of all button
			 * @param[in] allLabel label of checkbox for all
			 * @param[in] showNum If true, the number of selected items is displayed.
			 */
			CheckGridListPanel(
				wxWindow* parent,
				const int id = -1,
				AllButtonAlign align = TOP_ALL,
				const char* allLabel = "All",
				const bool showNum = true
			);

			/**
			 * @fn virtual ~CheckGridListPanel()
			 * @brief destructor
			 */
			virtual ~CheckGridListPanel();

		protected:
			/** all button align */
			AllButtonAlign m_align;

			/** check grid list */
			CheckGridListCtrl* m_checkList;

			/** all checkbox */
			wxCheckBox* m_allCheckBox;

			/** all label */
			std::string m_allLabel;

			/** num text */
			wxStaticText* m_numText;

			/** show num */
			bool m_showNum;

			/** description */
			std::string m_desc;

			/** image file path */
			std::string m_imageFilePath;

		public:
			/**
			 * @fn void setCheckGridListCtrl( CheckGridListCtrl* checkList )
			 * @brief sets check grid list control
			 * @param[in] checkList check grid list control
			 */
			void setCheckGridListCtrl( CheckGridListCtrl* checkList );

			/**
			 * @fn CheckGridListCtrl* getCheckGridListCtrl()
			 * @brief gets grid list control
			 * @return grid list control
			 */
			CheckGridListCtrl* getCheckGridListCtrl();

		public:
			/**			
			 * @fn void setNumberOfSelectedItems( const int num )
			 * @brief sets the number of selected items
			 * @param[in] num the number of selected items
			 */
			void setNumberOfSelectedItems( const int num );

		protected:
			/**
			 * @fn virtual wxSizer* createMainSizer()
			 * @brief creates main sizer
			 * @return main sizer
			 */
			virtual wxSizer* createMainSizer();

		protected:
			/**
			 * @fn void onAll( wxCommandEvent& evt )
			 * @brief This method is called when the "All" button is clicked.
			 * @param[in] evt event information
			 */
			void onAll( wxCommandEvent& evt );

			/**
			 * @fn void onUpdateAll( wxUpdateUIEvent& evt )
			 * @brief updates all check box
			 * @param[in] evt event information
			 */
			void onUpdateAll( wxUpdateUIEvent& evt );

		public:
			/**
			 * @fn void setDescription( const char* desc )
			 * @brief sets the description
			 * @param[in] desc description
			 */
			void setDescription( const char* desc );

			/**
			 * @fn void setImageFilePath( const char* path )
			 * @brief sets the image file path
			 * @param[in] path image file path
			 */
			void setImageFilePath( const char* path );

		protected:
			/**
			 * @fn virtual std::string getDescription( wxWindow* wnd )
			 * @brief method to get the description from a prameter control (override method)
			 * @param[in] wnd parameter control
			 * @return description
			 */
			virtual std::string getDescription( wxWindow* wnd );

			/**
			 * @fn virtual std::string getImageFilePath( wxWindow* wnd )
			 * @brief method to get image file path from a parameter control (override method)
			 * @param[in] wnd parameter control
			 * @return image file path
			 */
			virtual std::string getImageFilePath( wxWindow* wnd );

		private:
			DECLARE_EVENT_TABLE();
		};
	}
}


#endif   // __KOME_WINDOW_CHECK_GRID_LIST_PANEL_H__

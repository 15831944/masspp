/**
 * @file DialogSelector.h
 * @brief interfaces of DialogSelector class
 *
 * @author S.Tanaka
 * @date 2008.01.10
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_WINDOW_DIALOG_SELECTOR_H__
#define __KOME_WINDOW_DIALOG_SELECTOR_H__


#include "StandardDialog.h"

#include <string>
#include <wx/wx.h>
#include <wx/progdlg.h>


namespace kome {
	namespace window {

		/**
		 * @class DialogSelector
		 * @brief dialog selector class
		 */
		class WINDOW_MANAGER_CLASS DialogSelector : public kome::core::Selector {
		public:
			/**
			 * @fn DialogSelector( wxWindow* parent, const char* title, const bool multiple, const bool required )
			 * @brief constructor
			 * @param[in] parent parent window
			 * @param[in] title dialog title
			 * @param[in] multiple If true, allows multiple selection of items
			 * @param[in] required required flag
			 */
			DialogSelector( wxWindow* parent, const char* title, const bool multiple, const bool required );

			/**
			 * @fn virtual ~DialogSelector()
			 * @brief destructor
			 */
			virtual ~DialogSelector();

		protected:
			/** parent window */
			wxWindow* m_parent;

			/** title */
			std::string m_title;

			/** multiple selection flag */
			bool m_multiple;

			/** required flag */
			bool m_required;

		protected:
			/**
			 * @fn virtual bool onSelect(
					const unsigned int num,
					const char** items,
					const char** values,
					bool* selected
				)
			 * @brief This method is called by select method. (abstract method)
			 * @param[in] num the number of items
			 * @param[in] items the array of item names
			 * @param[in] values the array of item values
			 * @param[out] selected the array to store selected flags
			 * @return If true, the selection was normally done.
			 */
			virtual bool onSelect(
				const unsigned int num,
				const char** items,
				const char** values,
				bool* selected
			);

		protected:
			/**
			 * @class ComboDialog
			 * @brief dialog with combo box
			 */
			class ComboDialog : public StandardDialog {
			public:
				/**
				 * @fn ComboDialog(
						wxWindow* parent,
						const char* title,
						const unsigned int num,
						const char** items,
						const bool multiple,
						const bool required
					)
				 * @brief constructor
				 * @param[in] parent parent window
				 * @param[in] title dialog title
				 * @param[in] num the number of items
				 * @param[in] items the array of item names
				 * @param[in] multiple multiple flag
				 * @param[in] required required flag
				 */
				ComboDialog(
					wxWindow* parent,
					const char* title,
					const unsigned int num,
					const char** items,
					const bool multiple,
					const bool required
				);

				/**
				 * @fn virtual ~ComboDialog()
				 * @brief destructor
				 */
				virtual ~ComboDialog();

			protected:
				/** number of items */
				unsigned int m_num;

				/** the array of items */
				const char** m_items;

				/** multiple flag */
				bool m_multiple;

				/** required flag */
				bool m_required;

				/** selection control */
				wxWindow* m_ctrl;

				/** selected flags */
				bool* m_selected;

			public:
				/**
				 * @fn bool isSelected( const int item )
				 * @brief judges whether specified item is selected
				 * @param[in] item item number
				 * @return If true, specified item is selected
				 */
				bool isSelected( const int item );

			protected:
				/**
				 * @fn virtual wxSizer* createMainSizer()
				 * @brief creates main sizer (override method)
				 * @return main sizer
				 */
				virtual wxSizer* createMainSizer();

			protected:
				/**
				 * @fn virtual bool TransferDataFromWindow()
				 * @brief transfer window control data to value
				 * @return If a transfer failed, this method returns false.
				 */
				virtual bool TransferDataFromWindow();
			};
		};
	}
}

#endif		// __KOME_WINDOW_DIALOG_SELECTOR_H__

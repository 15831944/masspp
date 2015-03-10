/**
 * @file ProductIonSelectionDialog.h
 * @brief interfaces of ProductIonSelectionDialog class
 *
 * @author H.Parry
 * @date 2011.03.22
 * 
 * Copyright(C) 2014 Shimadzu Corporation. All rights reserved.
 */

#ifndef __KOME_PRODUCT_ION_SELECTION_DIALOG_H__
#define __KOME_PRODUCT_ION_SELECTION_DIALOG_H__

#include <wx/wx.h>

namespace kome {
	namespace spectrum {

		/**
		 * @class ProductIonSelectionDialog
		 * @brief auto labeling dialog class
		 */
		class ProductIonSelectionDialog : public kome::window::StandardDialog {
		public:
			/**
		 	 * @fn ProductIonSelectionDialog(
						wxWindow* parent,
						std::vector<kome::objects::Spectrum*>& productSpectra
					)
			 * @brief constructor
			 * @param parent parent window
			 * @param productSpectra
			 */
			ProductIonSelectionDialog(
				wxWindow* parent,
				std::vector<kome::objects::Spectrum*>& productSpectra
			);

			/**
			 * @fn virtual ~ProductIonSelectionDialog()
			 * @brief destructor
			 */
			virtual ~ProductIonSelectionDialog();

			/**
			 * @fn inline unsigned int GetSelectedItem()
			 * @brief Return the selected item index.
			 */
			inline unsigned int GetSelectedItem() {return m_iSelected;}

		protected:
			/** product spectra */
			std::vector<kome::objects::Spectrum*> m_productSpectra;
			/** values */
			std::vector<std::string> m_values;
			/** list */
			wxListBox* m_list;

		private:
			/** selected */
			unsigned int m_iSelected;
		protected:
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
			 * @brief transfer value to window control data
			 * @return If a transfer failed, this method returns false.
			 */
			virtual bool TransferDataToWindow();

		};
	}
}

#endif	//  __KOME_PRODUCT_ION_SELECTION_DIALOG_H__

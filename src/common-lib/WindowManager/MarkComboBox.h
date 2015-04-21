/**
 * @file MarkComboBox.h
 * @brief interfaces of MarkComboBox class
 *
 * @author S.Tanaka
 * @date 2009.06.29
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_WINDOW_MARK_COMBO_BOX_H__
#define __KOME_WINDOW_MARK_COMBO_BOX_H__


#include <vector>
#include <wx/wx.h>
#include <wx/bmpcbox.h>


namespace kome {
	namespace window {

		/**
		 * @class MarkComboBox
		 * @brief combobox class to select mark shape
		 */
		class WINDOW_MANAGER_CLASS MarkComboBox : public wxBitmapComboBox {
		public:
			/**
			 * @fn MarkComboBox(
						wxWindow* parent,
						const int id,
						const bool addNone
					)
			 * @brief constructor
			 * @param[in] parent parent window
			 * @param[in] id window ID
			 * @param[in] addNone If true, add "(None)" value to combo box.
			 */
			MarkComboBox(
				wxWindow* parent,
				const int id,
				const bool addNone
			);

			/**
			 * @fn virtual ~MarkComboBox()
			 * @brief destructor
			 */
			virtual ~MarkComboBox();

		protected:
			/** marks */
			std::vector< kome::img::Graphics::Mark > m_marks;

		public:
			/**
			 * @fn kome::img::Graphics::Mark getSelectedMark()
			 * @brief gets the selected mark
			 * @return selected mark
			 */
			kome::img::Graphics::Mark getSelectedMark();

			/**
			 * @fn void selectMark( const kome::img::Graphics::Mark mark )
			 * @brief selects mark
			 * @param[in] mark mark to be selected
			 */
			void selectMark( const kome::img::Graphics::Mark mark );

		protected:
			/**
			 * @fn void create()
			 * @brief creates mark combo box
			 */
			void create();

		private:
			DECLARE_EVENT_TABLE()
		};
	}
}


#endif		// __KOME_WINDOW_MARK_COMBO_BOX_H__

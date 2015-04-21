/**
 * @file GridSearchDialog.h
 * @brief interfaces of GridSearchCtrl class
 *
 * @author S.Tanaka
 * @date 2008.09.17
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_WINDOW_GRID_SEARCH_DIALOG_H__
#define __KOME_WINDOW_GRID_SEARCH_DIALOG_H__


#include <wx/wx.h>

#include "StandardDialog.h"


class wxNotebook;


namespace kome {
	namespace window {

		class GridListCtrl;

		/**
		 * @class GridSearchDialog
		 * @brief grid list dialog class
		 */
		class WINDOW_MANAGER_CLASS GridSearchDialog : public StandardDialog {
		public:
			/**
			 * @fn GridSearchDialog( wxWindow* parent )
			 * @brief constructor
			 * @param[in] parent parent window
			 */
			GridSearchDialog( wxWindow* parent );

			/**
			 * @fn virtual ~GridSearchDialog()
			 * @brief destructor
			 */
			virtual ~GridSearchDialog();

		public:
			/**
			 * @typedef enum SearchType
			 * @brief search mode
			 */
			typedef enum {
				SEARCH_NEW,
				SEARCH_NARROW,
				SEARCH_APPEND
			} SearchType;

			/**
			 * @typedef enum ValueType
			 * @brief value type
			 */
			typedef enum {
				VALUE_KEYWORD,
				VALUE_RANGE
			} ValueType;

		protected:
			/** grid list control */
			GridListCtrl* m_grid;

			/** search type */
			SearchType m_searchType;

			/** column */
			int m_col;

			/** value type */
			ValueType m_valueType;

			/** keyword */
			std::string m_keyword;

			/** min value */
			std::string m_minVal;

			/** max value */
			std::string m_maxVal;

		protected:
			/** search type radio box */
			wxRadioBox* m_searchBox;

			/** column combo box */
			wxComboBox* m_colBox;

			/** keyword text control */
			wxTextCtrl* m_keywordText;

			/** min value text control */
			wxTextCtrl* m_minValText;

			/** max value text control */
			wxTextCtrl* m_maxValText;

			/** note book */
			wxNotebook* m_note;

		public:
			/**
			 * @fn void setSearchType( const SearchType type )
			 * @brief sets search type
			 * @param[in] type search type
			 */
			void setSearchType( const SearchType type );

			/**
			 * @fn SearchType getSearchType()
			 * @brief gets search type
			 * @return search type
			 */
			SearchType getSearchType();

			/**
			 * @fn void setColumn( const int col )
			 * @brief sets column
			 * @param[in] col column number
			 */
			void setColumn( const int col );

			/**
			 * @fn int getColumn()
			 * @brief gets column
			 * @return column number
			 */
			int getColumn();

			/**
			 * @fn void setValueType( const ValueType type )
			 * @brief sets value type
			 * @param[in] type value type
			 */
			void setValueType( const ValueType type );

			/**
			 * @fn ValueType getValueType()
			 * @brief gets value type
			 * @return value type
			 */
			ValueType getValueType();

			/**
			 * @fn void setKeyword( const char* word )
			 * @brief sets keyword
			 * @param[in] word keyword
			 */
			void setKeyword( const char* word );

			/**
			 * @fn const char* getKeyword()
			 * @brief gets keyword
			 * @return keyword
			 */
			const char* getKeyword();

			/**
			 * @fn void setValueRange( const char* minVal, const char* maxVal )
			 * @brief sets value range
			 * @param[in] minVal minimum value
			 * @param[in] maxVal maximum  value
			 */
			void setValueRange( const char* minVal, const char* maxVal );

			/**
			 * @fn const char* getMinValue()
			 * @brief gets minimum value
			 * @return minimum value
			 */
			const char* getMinValue();

			/**
			 * @fn const char* getMaxValue()
			 * @brief gets maximum value
			 * @return maximum value
			 */
			 const char* getMaxValue();

		protected:
			/**
			 * @fn virtual wxSizer* createMainSizer()
			 * @brief creates main sizer (override method)
			 * @return main sizer
			 */
			virtual wxSizer* createMainSizer();

		protected:
			/**
			 * @fn virtual bool TransferDataToWindow()
			 * @brief transfer value to window control data
			 * @return If a transfer failed, this method returns false.
			 */
			virtual bool TransferDataToWindow();

			/**
			 * @fn virtual bool TransferDataFromWindow()
			 * @brief transfer window control data to value
			 * @return If a transfer failed, this method returns false.
			 */
			virtual bool TransferDataFromWindow();

		private:
			DECLARE_EVENT_TABLE();
		};
	}
}


#endif	// __KOME_WINDOW_GRID_SEARCH_DIALOG_H__

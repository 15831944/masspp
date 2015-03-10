/**
 * @file TppSearchGrid.h
 * @brief interfaces of TppSearchGrid class
 *
 * @author N.Tashiro
 * @date 2013.03.15
 * 
 * Copyright(C) 2013 Shimadzu. All rights reserved.
 */


#ifndef __KOME_TPP_SEARCH_GRID_H__
#define __KOME_TPP_SEARCH_GRID_H__

namespace kome {
	namespace Tpp{
		class TppSearchGrid : public kome::window::CheckGridListCtrl {
		protected:

			struct T_searchResult{
			public:
				std::string title;
				std::string date;
				std::string searchEngine;
				std::string comment;
			};


		public:
			/**
			 * @fn ScriptDialog( wxWindow* parent )
			 * @brief constructor
			 * @param parent parent window
			 */
			TppSearchGrid( wxWindow* parent, int id );

			/**
			 * @fn virtual ~ScriptDialog()
			 * @brief destructor
			 */
			virtual ~TppSearchGrid();

			/**
			 * @fn void AllEnable()
			 * @brief enable all row
			 */
			void AllEnable();

		protected:
			/**
			 * @fn virtual onCreate()
			 * @brief create columns
			 */
			virtual void onCreate();

			/**
			 * @fn void OnColClick( wxListEvent& evt )
			 * @brief This method is called when list column is clicked
			 * @param[in] evt list event object
			 */
			void OnColClick( wxListEvent& evt );

			/**
			* @fn virtual std::string getString( const long long data, const int column ) const
			* @brief gets character string value (override method)
			* @param data data
			* @param column column number
			*/
			virtual std::string getString( const long long data, const int col ) const;

		private:

			std::vector<T_searchResult> m_resultList;

			DECLARE_EVENT_TABLE();
		};
	}
}

#endif // __KOME_TPP_SEARCH_GRID_H__

/**
 * @file TppSearchGrid.h
 * @brief interfaces of TppSearchGrid class
 *
 * @author N.Tashiro
 * @date 2013.03.15
 * 
 * Copyright(C) 2013 Shimadzu. All rights reserved.
 */


#ifndef __KOME_TPP_RESULT_GRID_H__
#define __KOME_TPP_RESULT_GRID_H__

namespace kome {
	namespace Tpp{
		class TppResultGrid : public kome::window::GridListCtrl {
		protected:

			struct T_result{
			public:
				std::string Date;
				std::string Title;
				std::string Comment;
				std::string Path;
			};
		public:
			/**
			 * @fn ScriptDialog( wxWindow* parent )
			 * @brief constructor
			 * @param parent parent window
			 */
			TppResultGrid( wxWindow* parent, int id );

			/**
			 * @fn virtual ~ScriptDialog()
			 * @brief destructor
			 */
			virtual ~TppResultGrid();

		protected:
			/**
			 * @fn virtual onCreate()
			 * @brief create columns
			 */
			virtual void onCreate();

			void OnDClick( wxMouseEvent & evt );

			/**
			 * @fn void OnSelect( wxListEvent& evt )
			 * @brief This method is called when a list item is selected
			 * @param[in] evt list event object
			 */
			void OnSelect( wxListEvent& evt );

			/**
			 * @fn void OnDeselect( wxListEvent& evt )
			 * @brief This method is called when a list item isn't selected
			 * @param[in] evt list event object
			 */
			void OnDeselect( wxListEvent& evt );

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
			long long m_llSelectedIndex;

			std::vector<T_result> m_resultList;

			DECLARE_EVENT_TABLE();
		};
	}
}

#endif // __KOME_TPP_RESULT_GRID_H__

/**
 * @file SearchResultListCtrl.h
 * @brief interfaces of SearchResultListCtrl class
 *
 * @author S.Tanaka
 * @date 2012.03.05
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation All rights reserved.
 */


#ifndef __KOME_IDENT_SEARCH_RESULT_LIST_CTRL_H__
#define __KOME_IDENT_SEARCH_RESULT_LIST_CTRL_H__


#include <wx/timer.h>


namespace kome {
	namespace ident {

		class SearchResultDialog;	// @date 2013/01/23 <Add> OKADA

		/**
		 * @class SearchResultListCtrl
		 * @brief search result list control class
		 */
		class SearchResultListCtrl : public kome::window::GridListCtrl {
		public:
			/**
			 * @fn SearchResultListCtrl( wxWindow* parent )
			 * @brief constructor
			 */
			SearchResultListCtrl( wxWindow* parent );

			/**
			 * @fn virtual ~SearchResultListCtrl()
			 * @brief destructor
			 */
			virtual ~SearchResultListCtrl();

		protected:
			/** timer */
			wxTimer* m_timer;

		protected:
			/**
			 * @struct SearchInfo
			 * @brief search information
			 */
			struct SearchInfo {
				int id;
				std::string searchId;
				std::string title;
				std::string date;
				std::string db;
				int engineId;
				std::string engine;
				std::string comment;
				kome::ident::SearchEngine::SearchStatus status;
			};

			/** searches */
			std::vector< SearchInfo > m_searches;

		public:
			/**
			 * @fn int getSelectedSearch()
			 * @brief gets the selected search ID
			 */
			int getSelectedSearch();

		protected:
			/**
			 * @fn virtual void onCreate()
			 * @brief This method is called by create method
			 */
			virtual void onCreate();

			/**
			 * @fn virtual std::string getString( const long long data, const int column ) const
			 * @brief gets character string value
			 * @param[in] data data
			 * @param[in] column column number
			 */
			virtual std::string getString( const long long data, const int column ) const;

		protected:	
			/**
			 * @fn virtual void onActivateData( const long long data )
			 * @brief This method is called when the data is activated
			 * @param[in] data activated data
			 */
			virtual void onActivateData( const long long data );

		protected:
			/**
			 * @fn virtual bool onDeleteData( const long long data )
			 * @brief This method is called when a item is deleted.
			 * @param[in] data deleted item data
			 * @return If true, succeeded to delete the item.
			 */
			virtual bool onDeleteData( const long long data );

		public:
			/**
			 * @fn virtual void onActivateDataOther( const long long data )
			 * @brief This method is called when the data is activated by other method
			 * @param[in] data activated data
			 */
			virtual void onActivateDataOther( const long long data );

		public:
			/** dialog */
			static kome::ident::SearchResultDialog* m_dlg;

			/** parent dialog */
			wxWindow* m_parent;

			/**
			 * @fn void dispOnBrowser( void )
			 * @brief リストで選択された項目をブラウザで表示
			 */
			void dispOnBrowser( void );

		protected:
			/**
			 * @fn bool updateStatus()
			 * @brief updates status
			 * @return If true, some statuses are updated.
			 */
			bool updateStatus();

		protected:
			/**
			 * @fn void onTimer( wxTimerEvent& evt )
			 * @brief This method is called on timer event
			 * @param[in] evt event
			 */
			 void onTimer( wxTimerEvent& evt );

		private:
			DECLARE_EVENT_TABLE()
		};
	}
}


#endif // __KOME_IDENT_SEARCH_RESULT_LIST_CTRL_H__

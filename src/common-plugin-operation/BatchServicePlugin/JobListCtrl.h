/**
 * @file JobListCtrl.h
 * @brief interfaces of JobListCtrl class
 *
 * @author S.Tanaka
 * @date 2012.03.29
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation All rights reserved.
 */


#ifndef __KOME_BATCH_JOB_LIST_CTRL_H__
#define __KOME_BATCH_JOB_LIST_CTRL_H__


#include <wx/timer.h>


namespace kome {
	namespace batchservice {

		/**
		 * @class JobListCtrl
		 * @brief search result list control class
		 */
		class JobListCtrl : public kome::window::GridListCtrl {
		public:
			/**
			 * @fn JobListCtrl( wxWindow* parent )
			 * @brief constructor
			 */
			JobListCtrl( wxWindow* parent );

			/**
			 * @fn virtual ~JobListCtrl()
			 * @brief destructor
			 */
			virtual ~JobListCtrl();

		protected:
			/** timer */
			wxTimer* m_timer;

		public:
			/** dialog */
			static kome::window::GridListDialog* m_dlg;

			/** parent */
			wxWindow* m_parent;	// @date 2012/12/13 <Add> OKADA

		public:
			/**
			 * @fn void updateStatuses()
			 * @brief update statuses
			 */
			void updateStatuses();

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
			 * @fn virtual bool onDeleteData( const long long data )
			 * @brief This method is called when a item is deleted.
			 * @param[in] data deleted item data
			 * @return If true, succeeded to delete the item.
			 */
			virtual bool onDeleteData( const long long data );

		protected:
			/**
			 * @fn void timerEvent( wxTimerEvent& evt )
			 * @brief timer event
			 * @param[in] evt event information
			 */
			void timerEvent( wxTimerEvent& evt );

		private:
			DECLARE_EVENT_TABLE()
		};
	}
}


#endif // __KOME_BATCH_JOB_LIST_CTRL_H__

/**
 * @file DetailsListCtrl.h
 * @brief interfaces of DetailsListCtrl class
 *
 * @author S.Tanaka
 * @date 2007.02.13
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_VIEW_DETAILS_LIST_CTRL_H__
#define __KOME_VIEW_DETAILS_LIST_CTRL_H__


#include <vector>
#include <utility>


namespace kome {
	namespace view {

		/**
		 * @class DetailsListCtrl
		 * @brief details list control class
		 */
		class DetailsListCtrl : public kome::window::GridListCtrl {
		public:
			/**
			 * @fn DetailsListCtrl( wxWindow* parent )
			 * @brief constructor
			 */
			DetailsListCtrl( wxWindow* parent );

			/**
			 * @fn virtual ~DetailsListCtrl()
			 * @brief destructor
			 */
			virtual ~DetailsListCtrl();

		protected:
			/** properties */
			std::vector< std::pair< std::string, std::string > > m_properties;

		protected:
			/**
			 * @fn virtual void onCreate()
			 * @brief This method is called by create method (override method)
			 */
			virtual void onCreate();

		public:
			/**
			 * @fn void setProperties( kome::core::Properties& props, kome::core::Properties& userProps )
			 * @brief sets properties (including user properties)
			 * @param props properties object
			 */
			void setProperties( kome::core::Properties& props, kome::core::Properties& userProps );

		protected:
			/**
			 * @fn virtual std::string getString( const long long data, const int column ) const
			 * @brief gets character string value (override method)
			 * @param data data
			 * @param column column number
			 */
			virtual std::string getString( const long long data, const int column ) const;

			/**
			 * @fn void onClickCol( wxListEvent& evt )
			 * @brief called when a column header cell is clicked in the grid
			 * @param evt list event
			 */
			void onClickCol( wxListEvent& evt );

		private:
			DECLARE_EVENT_TABLE()
		};
	}
}


#endif	// __KOME_VIEW_DETAILS_LIST_CTRL_H__

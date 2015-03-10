/**
 * @file PeaksListCtrl.h
 * @brief interfaces of PeaksListCtrl class
 *
 * @author S.Tanaka
 * @date 2007.10.23
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_LABELING_PEAKS_LIST_CTRL_H__
#define __KOME_LABELING_PEAKS_LIST_CTRL_H__


namespace kome {
	namespace labeling {

		/**
		 * @class PeaksListCtrl
		 * @brief peaks list control class
		 */
		class PeaksListCtrl : public kome::window::GridListCtrl {
		public:
			/**
			 * @fn PeaksListCtrl( wxWindow* parent )
			 * @brief constructor
			 */
			PeaksListCtrl( wxWindow* parent );

			/**
			 * @fn virtual ~PeaksListCtrl()
			 * @brief destructor
			 */
			virtual ~PeaksListCtrl();

		protected:
			/** spectrum group */
			kome::objects::DataGroupNode* m_group;

		public:
			/**
			 * @fn void setGroup( kome::objects::DataGroupNode* group )
			 * @brief sets spectrum group
			 * @param group spectrum group
			 */
			void setGroup( kome::objects::DataGroupNode* group );

		protected:
			/**
			 * @fn void create()
			 * @brief creates list
			 */
			void create();

		protected:
			/**
			 * @fn virtual double getDouble( const long long data, const int column ) const
			 * @brief gets double value from data (override method)
			 * @param data data
			 * @param column column number
			 */
			virtual double getDouble( const long long data, const int column ) const;

		protected:
			/**
			 * @fn void onSelectItem( wxListEvent& evt )
			 * @brief This method is called when peaks are selected
			 * @param evt event information
			 */
			void onSelectItem( wxListEvent& evt );

		private:
			DECLARE_EVENT_TABLE()
		};
	}
}


#endif		// __KOME_LABELING_PEAKS_LIST_CTRL_H__

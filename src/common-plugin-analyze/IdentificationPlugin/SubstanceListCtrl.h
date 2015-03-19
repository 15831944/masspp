/**
 * @file SubstanceListCtrl.h
 * @brief interfaces of SubstanceListCtrl class
 *
 * @author S.Tanaka
 * @date 2012.03.05
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation All rights reserved.
 */


#ifndef __KOME_IDENT_SUBSTANCE_LIST_CTRL_H__
#define __KOME_IDENT_SUBSTANCE_LIST_CTRL_H__

#include "SubstanceDialog.h"


namespace kome {
	namespace ident {

		class SubstanceDialog;	// @date 2013/01/23 <Add> OKADA

		/**
		 * @class SubstanceListCtrl
		 * @brief search result list control class
		 */
		class SubstanceListCtrl : public kome::window::GridListCtrl {
		public:
			/**
			 * @fn SubstanceListCtrl( wxWindow* parent )
			 * @brief constructor
			 */
			SubstanceListCtrl( wxWindow* parent );

			/**
			 * @fn virtual ~SubstanceListCtrl()
			 * @brief destructor
			 */
			virtual ~SubstanceListCtrl();
        
        public:
			/** dialog */
			static kome::ident::SubstanceDialog* m_dlg;		// @date 2013/01/23 <Add> OKADA

			/** values */
			std::vector< std::pair< std::string, std::string > > m_values;

		public:
			/**
			 * @fn void clearValues()
			 * @brief clears values
			 */
			void clearValues();

			/**
			 * @fn void addValue( const char* key, const char* value )
			 * @brief adds value
			 * @param[in] key key
			 * @param[in] value value
			 */
			void addValue( const char* key, const char* value );

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


		public:// @date 2013/01/28 <Add> OKADA	// SubstanceDialogÇ≈É{É^Éìâüâ∫éûÇ…Ç±ÇÃèàóùÇåƒÇ‘
			/**
			 * @fn virtual void onActivateDataOther( const long long data )
			 * @brief This method is called when the data is activated by other method
			 * @param[in] data activated data
			 */
			virtual void onActivateDataOther( const long long data );	// @date 2013/01/28 <Add> OKADA
			
		
		private:
			DECLARE_EVENT_TABLE()
		};
	}
}


#endif // __KOME_IDENT_SEARCH_RESULT_LIST_CTRL_H__

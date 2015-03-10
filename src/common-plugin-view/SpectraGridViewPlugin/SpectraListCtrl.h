/**
 * @file SpectraListCtrl.h
 * @brief interfaces of SpectraListCtrl class
 *
 * @author S.Tanaka
 * @date 2007.02.15
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_VIEW_SPECTRA_LIST_CTRL_H__
#define __KOME_VIEW_SPECTRA_LIST_CTRL_H__


#include <vector>
#include <set>
#include <wx/wx.h>
#include <wx/listctrl.h>
#include <wx/timer.h>

using namespace kome::window;	// @date 2011/03/22 <Add> OKADA

namespace kome {
	namespace view {

		/**
		 * @class SpectraListCtrl
		 * @brief spectra list control class
		 */
		class SpectraListCtrl : public kome::window::EditGridListCtrl {
		public:
			/**
			 * @fn SpectraListCtrl( wxWindow* parent )
			 * @brief constructor
			 */
			SpectraListCtrl( wxWindow* parent );

			/**
			 * @fn virtual ~SpectraListCtrl()
			 * @brief destructor
			 */
			virtual ~SpectraListCtrl();

		protected:
			/** spectra */
			kome::objects::DataSet* m_dataSet;

			/** sample */
			kome::objects::Sample* m_sample;

			/** spectrum array */
			std::vector< kome::objects::Spectrum* > m_specArray;

			/** previous sort */
			int m_sort;
			
			/** pointer for parent window */
			wxWindow* pParent;
	
			/** key types */
			enum{	CD_TYPE_INT, CD_TYPE_DOUBLE, CD_TYPE_STRING	};

			/** key and types */
			std::map<std::string, int> m_keyType;
	
		public:
			/**
			 * @fn void setSample( kome::objects::Sample* sample )
			 * @brief sets sample
			 * @param group spectrum group
			 */
			void setSample( kome::objects::Sample* sample );

			/**
			 * @fn kome::objects::DataSet* getSpectra()
			 * @brief gets spectra object
			 * @return spectra object
			 */
			kome::objects::DataSet* getSpectra();

			/**
			 * @fn void updateTitle()
			 * @brief updates title
			 */
			void updateTitle();

			/**
			 * @fn void update()
			 * @brief updates spectra information
			 */
			void update();

		protected:
			/**
			 * @fn void create()
			 * @brief creates list ctrl
			 */
			void create();

			/**
			 * @fn void clear()
			 * @brief clears list control
			 */
			void clear();

			/**
			 * @fn void setSpectra( kome::objects::DataSet& dataSet )
			 * @brief sets spectra
			 * @param spectra spectra object
			 */
			void setSpectra( kome::objects::DataSet& dataSet );

			/**
			 * @fn void getSelectedSpectra( std::vector< kome::objects::Spectrum* >& selectedSpectra )
			 * @brief gets selected spectra
			 * @param selectedSpectra spectrum set to store selected spectra
			 */
			void getSelectedSpectra( std::vector< kome::objects::Spectrum* >& selectedSpectra );

		protected:
			/**
			 * @fn void onColClick( wxListEvent& evt )
			 * @brief This method is called when list column is clicked
			 * @param evt list event information
			 */
			void onColClick( wxListEvent& evt );

			/**
			 * @fn virtual void onActivateData( const long long data )
			 * @brief This method is called when the data is activated
			 * @param[in] data activated data
			 */
			virtual void onActivateData( const long long data );

		protected:
			/**
			 * @fn bool lessSpectrum( kome::objects::Spectrum* spec0, kome::objects::Spectrum* spec1, int col )
			 * @brief compare spectrum to sort
			 * @param spec0 spectrum to be compared
			 * @param spec1 spectrum to compare
			 * @param col sort column
			 * @return If spec0 sorts before spec1, this method returns true.
			 */
			bool lessSpectrum( kome::objects::Spectrum* spec0, kome::objects::Spectrum* spec1, int col );

		protected:
			/**
			 * @fn void onDelete( wxCommandEvent& evt )
			 * @brief This method is called when delete menu is clicked.
			 * @param evt command event information
			 */
			void onDelete( wxCommandEvent& evt );

		public:

			/**
			 * @fn virtual int OnGetItemImage( long item ) const
			 * @brief gets item image number (Override method)
			 * @param item item number
			 * @return item image number
			 */
			virtual int OnGetItemImage( long item ) const;

		protected:
			/**
			 * @fn void onUpdateUI( wxUpdateUIEvent& evt )
			 * @brief sets the status of menu
			 * @param[in] evt update UI event information
			 */
			void onUpdateUI( wxUpdateUIEvent& evt );

			/**
			 * @fn virtual wxMenu* createMenu()
			 * @brief creates menu
			 * @return created menu
			 */
			virtual wxMenu* createMenu();

		protected:
			/**
			 * @fn void onSettingPanel( wxCommandEvent& evt )
			 * @brief This method is called when SetPanel menu is clicked.
			 * @param evt command event information
			 */
			void onSettingPanel( wxCommandEvent& evt );

			/**
			 * @fn virtual void onCreate()
			 * @brief This method is called by create method (override method)
			 */
			virtual void onCreate();

			/**
			 * @fn virtual std::string getString( const long long data, const int column ) const
			 * @brief gets character string value (override method)
			 * @param data data
			 * @param column column number
			 */
			virtual std::string getString( const long long data, const int column ) const;

			/** spectrum array */
			std::vector< std::string > m_vecStrSpecGrid;

			/**
			 * @fn virtual int getInt( const long long data, const int column ) const
			 * @brief gets integer value from data
			 * @param[in] data data
			 * @param[in] column column number
			 * @return integer value
			 */
			virtual int getInt( const long long data, const int column ) const;

			/**
			 * @fn virtual double getDouble( const long long data, const int column ) const
			 * @brief gets double value from data
			 * @param[in] data data
			 * @param[in] column column number
			 */
			virtual double getDouble( const long long data, const int column ) const;

			/**
			 * @fn void SpectraListCtrl::checkType( void )
			 * @brief check Type(int, double, string) of Keys( RT, Name, TIC, etc... )
			 * @param void
			 */
			void checkType( void );

			/**
			 * @fn void checkTypeProp( void )
			 * @brief sub function of checkType();
			 * @param  kome::core::Properties & props  set property
			 */
			void checkTypeProp( kome::core::Properties & props );

			/**
			 * @fn virtual std::string getPopupMenuName( PopupMenuFlg nPopupMenuFlag )
			 * @brief get Popup Menu Name
			 * @param[in] nPopupMenuFlag popoup-menu
			 * @return Popup Menu text
			 */
			virtual std::string getPopupMenuName( PopupMenuFlg nPopupMenuFlag );

		private:
			DECLARE_EVENT_TABLE()
		};
	}
}


#endif // __KOME_VIEW_SPECTRA_LIST_CTRL_H__

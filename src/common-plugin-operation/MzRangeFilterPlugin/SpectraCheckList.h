/**
 * @file SpectraCheckList.h
 * @brief interfaces of Spectra CheckListCtrl class
 *
 * @author M.Izumi
 * @date 2013.01.08
 * 
 * Copyright(C) 2013 Shimadzu Corporation. All rights reserved.
 */

#ifndef __KOME_SPECTRA_CHECK_LIST_H__
#define __KOME_SPECTRA_CHECK_LIST_H__

namespace kome {
	namespace mzfilter {
		/**
		 * @class SpectraCheckList
		 * @brief Spectra check list ctrl class
		 */
		class SpectraCheckList : public kome::window::CheckGridListCtrl {
		public:
			/**
			 * @fn TestCheckList( wxWindow* parent )
			 * @brief constructor
			 */
			SpectraCheckList( wxWindow* parent, kome::objects::Spectrum* spec );

			/**
			 * @fn virtual ~TestCheckList()
			 * @brief destructor
			 */
			virtual ~SpectraCheckList();

		protected:
			/** sample object */
			kome::objects::Sample* m_sample;

			/** spectrum object */
			kome::objects::Spectrum* m_spec;

			/** index */
			int m_index;

			/** data info */
			typedef struct{
				std::string strNo;
				std::string strRt;
				std::string strStage;
				std::string strPrecursor;
				kome::objects::Spectrum* spec;
			}DataInfo;

			/** data info array */
			std::vector< DataInfo > m_listValInfo;

		public:
			
			/**
			 * @fn kome::objects::Spectrum* SpectraCheckList::getSelSpectrum( int data )
			 * @brief get select spectrum
			 * @param data
			 * @return active spectrum
			 */
			kome::objects::Spectrum* SpectraCheckList::getSelSpectrum( int data );

			/**
			 * @fn int getSelectIndex()
			 * @brief get the selected index
			 * @return index
			 */
			int getSelectIndex(){ return m_index; }
		protected:
			/**
			 * @fn virtual void onCreate()
			 * @brief create list
			 */
			virtual void onCreate();
			
			/**
			 * @fn virtual std::string getString( const long long data, const int column ) const
			 * @brief gets string
			 * @param data data
			 * @param column column ID
			 * @return string value
			 */
			virtual std::string getString( const long long data, const int column ) const;
		private:
			DECLARE_EVENT_TABLE()
		};
	}
}

#endif // __KOME_SPECTRA_CHECK_LIST_H__

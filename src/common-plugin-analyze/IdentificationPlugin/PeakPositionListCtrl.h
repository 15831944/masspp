/**
 * @file PeakPositionListCtrl.h
 * @brief interfaces of PeakPositionListCtrl class
 *
 * @author S.Tanaka
 * @date 2008.10.14
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_IDENT_PEAK_POSITION_LIST_CTRL_H__
#define __KOME_IDENT_PEAK_POSITION_LIST_CTRL_H__


namespace kome {
	namespace ident {

		/**
		 * @class PeakPositionListCtrl
		 * @brief peak position list control class
		 */
		class PeakPositionListCtrl : public kome::window::CheckGridListCtrl {
		public:
			/**
			 * @fn PeakPositionListCtrl( wxWindow* parent, const int width, const int height )
			 * @brief constructor
			 * @param parent parent window
			 * @param width width
			 * @param height height
			 */
			PeakPositionListCtrl( wxWindow* parent, const int width, const int height );

			/**
			 * @fn virtual ~PeakPositionListCtrl()
			 * @brief destructor
			 */
			virtual ~PeakPositionListCtrl();

		protected:
			/** appended columns */
			std::vector< int > m_appendedCols;

			/** groups */
			std::vector< int > m_groups;

			/** RT column */
			int m_rtCol;

			/** m/z column */
			int m_mzCol;

			/** charge column */
			int m_chargeCol;

			/** appended base column */
			int m_appendedCol;

		protected:
			/**
			 * @fn virtual void onCreate()
			 * @brief This method is called by create method
			 */
			virtual void onCreate();

			/**
			 * @fn virtual int getInt( const long long data, const int column ) const
			 * @brief gets integer value from data (override method)
			 * @param data data
			 * @param column column number
			 * @return integer value
			 */
			virtual int getInt( const long long data, const int column ) const;

			/**
			 * @fn virtual double getDouble( const long long data, const int column ) const
			 * @brief gets double value from data
			 * @param data data
			 * @param column column number
			 */
			virtual double getDouble( const long long data, const int column ) const;

			/**
			 * @fn virtual std::string getString( const long long data, const int column ) const
			 * @brief gets character string value (override method)
			 * @param data data
			 * @param column column number
			 */
			virtual std::string getString( const long long data, const int column ) const;

		private:
			DECLARE_EVENT_TABLE();
		};
	}
}

#endif	// __KOME_IDENT_PEAK_POSITION_LIST_CTRL_H__

/**
 * @file ResultListCtrl.h
 * @brief interfaces of ResultListCtrl class
 *
 * @author S.Tanaka
 * @date 2009.09.14
 *
 * @modify M.Fukuda
 * @date 2011.08.30
 *
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#ifndef __KOME_MASSBANK_RESULT_LIST_CTRL_H__
#define __KOME_MASSBANK_RESULT_LIST_CTRL_H__

namespace kome {
	namespace massbank {

		class RecordPeaksDialog;

		/**
		 * @class ResultListCtrl
		 * @brief search result list control class
		 */
		class ResultListCtrl : public kome::window::GridListCtrl {
		public:
			/**
			 * @fn ResultListCtrl
			 * @brief constructor
			 * @param parent parent window
			 * @param width widow width
			 * @param height window height
			 */
			ResultListCtrl(
				wxWindow* parent,
				const unsigned int width,
				const unsigned int height,
				const bool isModel
			);

			/**
			 * @fn virtual ~ResultListCtrl()
			 * @brief destructor
			 */
			virtual ~ResultListCtrl();

		protected:
			/** peaks dialog */
//			RecordPeaksDialog* m_pkDlg;

			// ColumIDs
			enum {
				COL_ID = 0,
				COL_TITLE = 1,
				COL_FORMULA = 2,
				COL_EXTMASS = 3,
				COL_SCORE = 4,
				COL_QUERYCNT = 5,
				COL_QUERY = 6,
				COL_NO_SC_QUERYCNT = 4,
				COL_NO_SC_QUERY = 5
			};

			// Use Flags
			bool m_useScore;
			bool m_useQuery;

			const bool isModal_;

			// if Use Query. use Query Col ID
			unsigned int m_Col_QueryCnt;
			unsigned int m_Col_Query;

		public:

			/**
			 * @fn void closePeaksDialog()
			 * @brief closes peaks dialog
			 */
			void closePeaksDialog();

			/**
			 * @fn void upDateMe()
			 * @brief up date my Data
			 */
			void upDateMe();

		protected:
			/**
			 * @fn virtual void onCreate()
			 * @brief This method is called by create method (override method)
			 */
			virtual void onCreate();

			/**
			 * @fn void onReCreateCtrlList()
			 * @brief re create ctrl List
			 */
			virtual void onReCreateCtrlList();

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
			 * @brief gets double value from data (override method)
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

		protected:
			/**
			 * @fn void onDblClick( wxMouseEvent& evt )
			 * @brief This method is called when the list item is double clicked.
			 * @param evt mouse event information
			 */
			void onDblClick( wxMouseEvent& evt );

		private:
			DECLARE_EVENT_TABLE();
		};
	}
}


#endif		// __KOME_MASSBANK_RESULT_LIST_CTRL_H__

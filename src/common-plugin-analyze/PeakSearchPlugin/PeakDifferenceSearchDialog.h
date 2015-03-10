/**
 * @file PeakDifferenceSearchDialog.h
 * @brief interfaces of PeakDifferenceSearchDialog class
 *
 * @author S.Tanaka
 * @date 2008.10.17
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_PEAK_DIFFERENCE_SEARCH_DIALOG_H__
#define __KOME_PEAK_DIFFERENCE_SEARCH_DIALOG_H__


#include "PeakSearchDialog.h"


namespace kome {
	namespace search {

		/**
		 * @class PeakDifferenceSearch Dialog
		 * @brief peak difference search dialog
		 */
		class PeakDifferenceSearchDialog : public PeakSearchDialog {
		public:
			/**
			 * @fn PeakDifferenceSearchDialog( wxWindow* parent )
			 * @brief constructor
			 * @param parent parent window
			 */
			PeakDifferenceSearchDialog( wxWindow* parent );

			/**
			 * @fn virtual ~PeakDifferenceSearchDialog()
			 * @brief destructor
			 */
			virtual ~PeakDifferenceSearchDialog();

		protected:
			/** m/z difference text */
			wxTextCtrl* m_mzDiffText;
			/** m/z tolerance text */
			wxTextCtrl* m_mzTolText;

		protected:
			/** m/z difference */
			double m_mzDiff;
			/** m/z tolerance */
			double m_mzTol;

		public:
			/**
			 * @fn double getMzDiff()
			 * @brief gets m/z difference
			 */
			double getMzDiff();

			/**
			 * @fn double getMzTol()
			 * @brief gets m/z tolerance
			 */
			double getMzTol();

		protected:
			/**
			 * @fn virtual wxSizer* createMainSizer()
			 * @brief creates main sizer
			 * @return main sizer
			 */
			virtual wxSizer* createMainSizer();

		protected:
			/**
			 * @fn wxSizer* createMzSizer()
			 * @brief creates m/z sizer
			 * @return m/z sizer
			 */
			wxSizer* createMzSizer();

		protected:
			/**
			 * @fn virtual bool TransferDataFromWindow()
			 * @brief transfer window control data to value
			 * @return If a transfer failed, this method returns false.
			 */
			virtual bool TransferDataFromWindow();

		private:
			DECLARE_EVENT_TABLE();
		};
	}
}

#endif		// __KOME_PEAK_DIFFERENCE_SEARCH_DIALOG_H__

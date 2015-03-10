/**
 * @file RecordPeaksDialog.h
 * @brief interfaces of RecordPeaksDialog
 *
 * @author S.Tanaka
 * @date 2009.09.15
 * 
 * @modify M.Fukuda
 * @date 2011.08.30
 *
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#ifndef __KOME_MASBANK_RECORD_PEAKS_DIALOG_H__
#define __KOME_MASBANK_RECORD_PEAKS_DIALOG_H__

class wxSplitterWindow;
class ImageCanvas;

namespace kome {
	namespace massbank {

		class PeaksCanvas;

		/**
		 * @class RecordPeaksDialog
		 * @brief MassBank record edit dialog class
		 */
		class RecordPeaksDialog : public kome::window::StandardPanel {

		public:
			/**
			 * @fn RecordPeaksDialog( wxWindow* parent )
			 * @brief constructor
			 * @param parent parent window
			 * @param title dialog title
			 */
			RecordPeaksDialog( wxWindow* parent );

			/**
			 * @fn virtual ~RecordPeaksDialog()
			 * @brief destructor
			 */
			virtual ~RecordPeaksDialog();
		public:

			/**
			 * @fn void upDateInfo()
			 * @brief up Date My Canvas & Text Info
			 */
			void upDateInfo();

		protected:

			// Chemical Formula PNG
			ImageCanvas* chemical_formula_;
//			kome::window::BitmapCanvas* chemical_formula_;
			
			// Peaks Canvas 
			PeaksCanvas* m_PeakCV;

			// text
			wxTextCtrl* m_ProfTC;

			wxSplitterWindow* detail_splitter_;
			wxSplitterWindow* canvas_splitter_;

			unsigned int oldflag_;

		protected:
			/**
			 * @fn virtual wxSizer* createMainSizer()
			 * @brief creates main sizer (override method)
			 * @return main sizer
			 */
			virtual wxSizer* createMainSizer();

		protected:
			/**
			 * @fn void onUpdateOverlap( wxUpdateUIEvent& evt )
			 * @brief on update UI of the overlap button
			 * @param evt event information
			 */
			void onUpdateOverlap( wxUpdateUIEvent& evt );

		private:
			DECLARE_EVENT_TABLE();
		};
	}
}


#endif	// __KOME_MASBANK_RECORD_PEAKS_DIALOG_H__

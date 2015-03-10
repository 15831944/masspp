/**
 * @file GlycanAnalysisStructureDialog.h
 * @brief Dialog class of GlycanAnalysisPlugin
 *
 * @author N.Tashiro
 * @date 2012.04.02
 * 
 * Copyright (C) 2014 Shimadzu Corporation All rights reserved.
 */

#ifndef __KOME_GLYCAN_ANALYSIS_STRUCTURE_DIALOG_H__
#define __KOME_GLYCAN_ANALYSIS_STRUCTURE_DIALOG_H__

#include <wx/wx.h>
#include "GlycanAnalysisStructures.h"
#include "GlycanAnalysisStructureCanvas.h"

namespace kome{
	namespace glycan{

		/**
		 * @class GlycanAnalysisStructureDialog
		 * @brief Dialog class of the Glycan Structure
		 */
		class GlycanAnalysisStructureDialog : public wxDialog {

		public:
			/**
			 * @fn GlycanAnalysisStructureDialog(wxWindow* parent , unsigned int unGNumber, GlycanParams* params, Annotation *annotation)
			 * @brief constructor
			 * @param[in] parent parent window
			 * @param[in] GNumber
			 * @param[in] parameters for analysis
			 * @param[in] annotation
			 */
			GlycanAnalysisStructureDialog(wxWindow* parent , unsigned int unGNumber, GlycanParams* params, Annotation *annotation);

			/**
			 * @fn virtual ~GlycanAnalysisDialog()
			 * @brief destructor
			 */
			virtual ~GlycanAnalysisStructureDialog();

			/**
			 * @fn Show( bool show )
			 * @brief show this dialog
			 * @return result of displaying dialog
			 */
			bool Show( bool show );

		private:
			/**
			 * @fn createMainSizer()
			 * @brief create main sizer
			 * @return pointer of wxSizer
			 */
			wxSizer* createMainSizer();
			
			/**
			 * @fn wxSizer* createCanvasSizer()
			 * @brief create canvas sizer
			 * @return pointer of wxSizer
			 */
			wxSizer* createCanvasSizer();

			/**
			 * @fn wxSizer* createButtonSizer()
			 * @brief create button sizer
			 * @return pointer of wxSizer
			 */
			wxSizer* createButtonSizer();

			/**
			 * @fn onCloseButton(wxCommandEvent& evt)
			 * @brief when press the Close button, called this function.
			 */
			void onCloseButton( wxCommandEvent& evt );

		private:
			/** Figure of Glycan Structure */
			GlycanAnalysisStructureCanvas* m_canvas;
			
		private:
			DECLARE_EVENT_TABLE();
		};
	}
}

#endif	//__KOME_GLYCAN_ANALYSIS_STRUCTURE_DIALOG_H__
/**
 * @file GlycanAnalysisStructureResults.h
 * @brief interfaces of Glycan Analysis Predicted Structures
 *
 * @author N.Tashiro
 * @date 2012.04.02
 * 
 * Copyright (C) 2014 Shimadzu Corporation All rights reserved.
 */

#ifndef __KOME_GLYCAN_ANALYSIS_STRUCTURE_RESULTS_H__
#define __KOME_GLYCAN_ANALYSIS_STRUCTURE_RESULTS_H__

#include "stdafx.h"
#include <wx/wx.h>
#include <string>
#include "GlycanAnalysisStructureCanvas.h"

/** KEGG URL */
#define KEGG_GLYCAN_URL "http://www.genome.jp/dbget-bin/www_bget?gl:"

namespace kome {
	namespace glycan{

		/**
		 * @class GlycanAnalysisStructureResults
		 * @brief Dialog class of GlycanAnalysisPlugin
		 */
		class GlycanAnalysisStructureResults : public wxScrolledWindow{
		public:	
			/**
 			 * @fn GlycanAnalysisStructureResults(wxDialog* parent)
			 * @brief constructor
			 */
			GlycanAnalysisStructureResults(wxDialog* parent);

			/**
			 * @fn ~GlycanAnalysisStructureResults()
			 * @brief destructor
			 */
			virtual ~GlycanAnalysisStructureResults();

			/**
			 * @fn virtual bool Show( bool show )
			 * @brief shows frame
			 * @param[in] show If true, displays the window. Otherwise, hides it.
			 */
			void OnShow( wxShowEvent& evt );

			/**
			 * @fn addResult(GLYCAN_STRUCTURE* pGlycanStructure, Annotation* pAnnotation, std::string strLabel, std::string strAnion)
			 * @brief add a glycan to results array
			 * @param[in] Glycan structure
			 * @param[in] Annotations
			 * @param[in] Labeling value
			 * @param[in] Anion value
			 * @return if Glycan structure or Annotations are NULL, return false
			 */
			bool addResult(GLYCAN_STRUCTURE* pGlycanStructure, Annotation* pAnnotation, const char* strLabel, const char* strAnion);

			/**
			 * @fn clearResult()
			 * @brief clear Predicted Structures
			 * @return true only
			 */
			bool clearResult();
			
			/**
			 * @fn onRightClick(wxMouseEvent& evt)
			 * @brief call openMenu() and skip event
			 */
			void onRightClick(wxMouseEvent& evt);

			/**
			 * @fn onRadioButton(wxCommandEvent& event)
			 * @brief when Radio button clicked, call this function
			 * @param command event
			 */
			void onRadioButton(wxCommandEvent& evt);

			/**
			 * @fn openMenu();
			 * @brief open the right click menu
			 */
			void openMenu();

			/**
			 * @fn onUpdateUI(wxUpdateUIEvent& event)
			 * @brief when updated UI, call this function
			 * @param update UI event
			 */
			void onUpdateUI(wxUpdateUIEvent& evt);

			/**
			 * @fn copyFigure(wxCommandEvent& event)
			 * @brief copy the selected figure
			 * @param command event
			 */			
			void copyFigure(wxCommandEvent& evt);

			/**
			 * @fn copyAllFigures(wxCommandEvent& event)
			 * @brief copy all figures in predicted structures
			 * @param command event
			 */
			void copyAllFigures(wxCommandEvent& evt);

			/**
			 * @fn saveAllFigures()
			 * @brief save all figures in predicted structures
			 */
			void saveAllFigures();

			/**
			 * @fn openGlycanBrowser(wxCommandEvent& event)
			 * @brief open the browser to access KEGG Glycan
			 * @param command event
			 */
			void accessKEGG(wxCommandEvent& evt);

			/**
			 * @fn getSelectedID(unsigned int number)
			 * @brief get ID of selected radiobutton
			 * @param[out] ID of selected button
			 * @return if no button is selected, return false
			 */
			bool getSelectedID(unsigned int* selectedID);

			/**
			 * @fn select(unsigned int unID)
			 * @brief select target radiobutton
			 * @param[in] ID
			 */
			void select(unsigned int unID);

			/**
			 * @fn select(unsigned int unID)
			 * @brief update predicted structures.
			 */
			void refreshResult();

		protected:
			/** Main Sizer */
			wxBoxSizer* m_sizer;

// 2014/02/24 Delete by nakaya Change of a selective state ->
			///** Sizer for Radiobutton*/
			//wxSizer* m_rbtnSizer;
// 2014/02/24 Delete by nakaya Change of a selective state <-

			/** Sizer for Figure of Glycan Structure*/
			wxSizer* m_canvasSizer;

// 2014/02/20 Add by nakaya The addition of a display item  ->
			/** RankSizer*/
			wxSizer* m_rankSizer;

			/** AssignedSizer*/
			wxSizer* m_assignedSizer;

			/** ScoreSizer*/
			wxSizer* m_scoreSizer;
// 2014/02/20 Add by nakaya The addition of a display item  <-

			/** List for Figures of Glycan Structure */
			std::vector< GlycanAnalysisStructureCanvas* > m_canvasList;

// 2014/02/24 Delete by nakaya Change of a selective state ->
			///** List for Radiobuttons */
			//std::vector< wxRadioButton* > m_radioBtnList;
// 2014/02/24 Delete by nakaya Change of a selective state <-

// 2014/02/20 Add by nakaya The addition of a display item  ->
			/** List for Rank */
			std::vector< TextCanvas* > m_rankCanvasList;

			/** List for Assigned */
			std::vector< TextCanvas* > m_assignedCanvasList;

			/** List for Score */
			std::vector< TextCanvas* > m_scoreCanvasList;
// 2014/02/20 Add by nakaya The addition of a display item  <-

			/** Right click menu */
			wxMenu* m_menu;
	
		protected:
			DECLARE_EVENT_TABLE()

		};
	}
}




#endif	// __KOME_GLYCAN_ANALYSIS_STRUCTURE_RESULTS_H__
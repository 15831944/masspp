/**
 * @file GlycanAnalysisDialog.h
 * @brief Dialog class of GlycanAnalysisPlugin
 *
 * @author N.Tashiro
 * @date 2012.04.02
 * 
 * Copyright (C) 2014 Shimadzu Corporation All rights reserved.
 */

#ifndef __KOME_GLYCAN_ANALYSIS_DIALOG_H__
#define __KOME_GLYCAN_ANALYSIS_DIALOG_H__

#include "stdafx.h"
#include <wx/wx.h>
#include <wx/html/htmlwin.h>
#include <wx/fs_mem.h>
#include <wx/statline.h>
#include "GlycanAnalysisStructures.h"
#include "GlycanAnalysisStructureResults.h"

/** Default Parameters */
#define GLYCAN_ANALYSIS_LABELING_DEFAULT "AQ"
#define GLYCAN_ANALYSIS_ANION_DEFAULT "H2PO4"

namespace kome {
	namespace glycan{

		/**
		 * @class GlycanAnalysisDialog
		 * @brief Dialog class of GlycanAnalysisPlugin
		 */
		class GlycanAnalysisDialog : public kome::window::StandardDialog {
		
		/** ENUM for UI ID */
		enum ENUM_CONTROL_ID{
			ID_STATIC_TITLE_PEAK_DETECTOR	= 1,
			ID_LIST_PEAK_DETECTOR,
			ID_BUTTON_PARAMETERS,
			ID_STATIC_TITLE_LABELING,
			ID_LIST_LABELING,
			ID_STATIC_TITLE_TOLERANCE,
			ID_STATIC_TOLERANCE,
			ID_STATIC_TITLE_ANION,
			ID_LIST_ANION,
			ID_BUTTON_DEFAULT,
			ID_BUTTON_APPLY,
			ID_BUTTON_CLOSE,
			ID_STRUCTURE_RESULTS,
// 2014/02/20 Add by nakaya The addition of a display item  ->
			ID_STATIC_STRUCTURE_TITLE,
// 2014/02/20 Add by nakaya The addition of a display item  <-
			ENUM_CONTROL_ID_NUMBER_PLUS1,
			ENUM_CONTROL_ID_NUMBER = ENUM_CONTROL_ID_NUMBER_PLUS1 - 1
		};

// 2014/02/24 Add by nakaya Change of a selective state ->
		// timer
		enum {
			ID_DRAW_TIMER = 0,
			INTERVAL_DRAW_TIMER = 100,
		};
// 2014/02/24 Add by nakaya Change of a selective state <-

		public:
			/**
			 * @fn GlycanAnalysisDialog()
			 * @brief constructor
			 * @param[in] parent parent window
			 */
			GlycanAnalysisDialog( wxWindow* parent );

			/**
			 * @fn virtual ~GlycanAnalysisDialog()
			 * @brief destructor
			 */
			virtual ~GlycanAnalysisDialog();

			/**
			 * @fn virtual bool Show( bool show )
			 * @brief shows frame
			 * @param[in] show If true, displays the window. Otherwise, hides it.
			 */
			virtual bool Show( bool show = true );
	
			/**
			 * @fn virtual bool onCancel()
			 * @brief close this dialog(from cancel button)
			 * @return true only
			 */
			virtual bool onCancel();
		
			/**
			 * @fn void onInitDialog( wxInitDialogEvent& evt )
			 * @brief This method is called on init dialog event
			 * @param[in] evt init dialog event
			 */
			virtual void onInitDialog( wxInitDialogEvent& evt );

			/**
			 * @fn bool setLabelList(std::vector< GLYCAN_LABEL > *labelList)
			 * @brief setting Labeling list
			 * @param[out] std::vector< GLYCAN_LABEL > *labelList
			 * @return If successful, return true
			 */
			bool setLabelList(std::vector< GLYCAN_LABEL > *labelList);

			/**
			 * @fn bool setAnionList(std::vector< GLYCAN_ANION > *anionList)
			 * @brief setting Anion list
			 * @param[out] std::vector< GLYCAN_ANION > *anionList
			 * @return If successful, return true
			 */
			bool setAnionList(std::vector< GLYCAN_ANION > *anionList);

			/**
			 * @fn void onDefaultButton( wxCommandEvent& evt )
			 * @brief set parameter to default
			 */
			void onDefaultButton( wxCommandEvent& evt );
			
			/**
			 * @fn void onApplyButton( wxCommandEvent& evt )
			 * @brief start calculate
			 */
			void onApplyButton( wxCommandEvent& evt );

			/**
			 * @fn void onCloseButton( wxCommandEvent& evt )
			 * @brief close this dialog
			 */
			void onCloseButton( wxCommandEvent& evt );
			
			/**
			 * @fn const char* getAnion()
			 * @brief get anion text
			 * @return anion text
			 */
			const char* getAnion();

			/**
			 * @fn void getSelectedResult(unsigned int* unID)
			 * @brief get selected figure ID
			 * @param[out] selected figure ID
			 * @return if number of results is 0, return false.
			 */
			bool getSelectedResult(unsigned int* unID);

// 2014/02/24 Add by nakaya Change of a selective state ->
			/**
			 * @fn void InitDisplay()
			 * @brief initial display
			 */
			void InitDisplay();
// 2014/02/24 Add by nakaya Change of a selective state <-

		private:
			
			/**
			 * @fn wxSizer* createMainSizer()
			 * @brief create main sizer
			 * @return pointer of wxSizer
			 */
			wxSizer* createMainSizer();

			/**
			 * @fn wxSizer* createParameterSizer()
			 * @brief create detail sizer
			 * @return pointer of wxSizer
			 */
			wxSizer* createDetailSizer();

			/**
			 * @fn wxSizer* createParameterSizer()
			 * @brief create param sizer
			 * @return pointer of wxSizer
			 */
			wxSizer* createParameterSizer();

			/**
			 * @fn wxSizer* createResultSizer()
			 * @brief create result sizer
			 * @return pointer of wxSizer
			 */
			wxSizer* createResultSizer();

			/**
			 * @fn wxSizer* createButtonSizer()
			 * @brief create button sizer
			 * @return pointer of wxSizer
			 */
			wxSizer* createButtonSizer();
			wxSizer* createNewButton();

		protected:

			/**
			 * @fn virtual bool TransferDataFromWindow()
			 * @brief transfer window control data to value
			 * @return If a transfer failed, this method returns false.
			 */
			virtual bool TransferDataFromWindow();

			/**
			 * @fn void OnCalcTimer( wxTimerEvent& event )
			 * @brief This method is called when the timer is started.
			 * @param[in] evt timer event
			 */
			void OnTimer( wxTimerEvent& event );

		protected:

			/** Peak detector list title*/
			wxStaticText*	m_pstcPeakDetectorTitle;

			/** Peak detector list parameter*/
			kome::plugin::ParameterSettings m_parameters;

			/** Peak detector list setting*/
			kome::objects::SettingParameterValues m_settings;

			/** Labeling list */
			wxStaticText*	m_pstcLabelingTitle;

			/** label for Labeling list */
			wxChoice* m_pLabelingList;
			
			/** Tolerance */
			wxStaticText*	m_pstcToleranceTitle;

			/** Label for Tolerance */
			wxTextCtrl*	m_pedtTolerance;

			/** Anion list */
			wxStaticText*	m_pstcAnionTitle;

			/** Label for Anion list */
			wxChoice*	m_pAnionList;

			/** Detailed result */
			wxStaticText*	m_pstcDetailTitle;

			/** Html for detailed result */
			wxHtmlWindow*	m_pDetailHtml;

			/** Default button */
			wxButton* m_pDefaultButton;

			/** Apply button */
			wxButton* m_pApplyButton;

			/** Close button */
			wxButton* m_pCloseButton;

			/** Predicted Structures */
			wxStaticText*	m_pstcResultTitle;

// 2014/02/20 Add by nakaya The addition of a display item  ->
			/** Structures title */
			wxStaticText*	m_pstcStructureTitle;
// 2014/02/20 Add by nakaya The addition of a display item  <-

			/** Label for Predicted Structures */
			GlycanAnalysisStructureResults*	m_pResultList;

// 2014/02/24 Add by nakaya Change of a selective state ->
			wxTimer* m_timer;
// 2014/02/24 Add by nakaya Change of a selective state <-

		private:
			DECLARE_EVENT_TABLE();

		};
	}
}

#endif	// __KOME_GLYCAN_ANALYSIS_DIALOG_H__
/**
 * @file PeakSearchDialog.h
 * @brief interfaces of PeakSearchDialog class
 *
 * @author S.Tanaka
 * @date 2008.09.08
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_PEAK_SEARCH_DIALOG_H__
#define __KOME_PEAK_SEARCH_DIALOG_H__


#include <wx/wx.h>

#include "PeakSearch.h"

// @date 2011/08/24 <Add> OKADA ------->
#define STAGE_PARAM_NAME	"Stage"
#define RT_PARAM_NAME		"RT"
#define MZ_PARAM_NAME		"m/z"
#define INT_PARAM_NAME		"Intensity"
#define DIFF_BOX_NAME		"Difference"
#define DIFF_PARAM_NAME		"Diff."
#define TOL_PARAM_NAME		"Tol."
// @date 2011/08/24 <Add> OKADA <-------


namespace kome {
	namespace search {

		/**
		 * @class PeakSearch Dialog
		 * @brief peaks search dialog
		 */
		class PeakSearchDialog : public kome::window::ParametersAndHelpDialog {
		public:
			/**
			 * @fn PeakSearchDialog( wxWindow* parent )
			 * @brief constructor
			 * @param parent parent window
			 */
			PeakSearchDialog( wxWindow* parent );

			/**
			 * @fn virtual ~PeakSearchDialog()
			 * @brief destructor
			 */
			virtual ~PeakSearchDialog();

		protected:
			/** search type radio box */
			wxRadioBox* m_searchTypeRadio;
			/** min stage text */
			wxTextCtrl* m_minStageText;
			/** max stage text */
			wxTextCtrl* m_maxStageText;
			/** min RT text */
			wxTextCtrl* m_minRtText;
			/** max RT text */
			wxTextCtrl* m_maxRtText;
			/** min m/z text */
			wxTextCtrl* m_minMzText;
			/** max m/z text */
			wxTextCtrl* m_maxMzText;
			/** min intensity text */
			wxTextCtrl* m_minIntText;
			/** max intensity text */
			wxTextCtrl* m_maxIntText;

		protected:
			/** min stage */
			int m_minStage;
			/** max stage */
			int m_maxStage;
			/** min RT */
			double m_minRt;
			/** max RT */
			double m_maxRt;
			/** min m/z */
			double m_minMz;
			/** max m/z */
			double m_maxMz;
			/** min intensity */
			double m_minInt;
			/** max intensity */
			double m_maxInt;

			/** params */
			kome::plugin::ParameterSettings m_params;			// @date 2011/08/24 <Add> OKADA
			/** settings */
			kome::objects::SettingParameterValues m_settings;	// @date 2011/08/24 <Add> OKADA

		public:
			/**
			 * @fn int getMinStage()
			 * @brief gets min stage
			 * @return min stage
			 */
			int getMinStage();

			/**
			 * @fn int getMaxStage()
			 * @brief gets max stage
			 * @return max stage
			 */
			int getMaxStage();

			/**
			 * @fn double getMinRt()
			 * @brief gets min RT
			 * @return min RT
			 */
			double getMinRt();

			/** 
			 * @fn double getMaxRt()
			 * @brief gets max RT
			 * @return max RT
			 */
			double getMaxRt();

			/**
			 * @fn double getMinMz()
			 * @brief gets min m/z
			 * @return min m/z
			 */
			double getMinMz();

			/**
			 * @fn double getMaxMz()
			 * @brief gets max m/z
			 * @return max m/z
			 */
			double getMaxMz();

			/**
			 * @fn double getMinIntensity()
			 * @brief gets min intensity
			 * @return min intensity
			 */
			double getMinIntensity();

			/**
			 * @fn double getMaxIntensity()
			 * @brief gets max intensity
			 * @return max intensity
			 */
			double getMaxIntensity();

		protected:
			/**
			 * @fn virtual wxSizer* createMainSizer()
			 * @brief creates main sizer
			 * @return main sizer
			 */
			virtual wxSizer* createMainSizer();

		protected:
			/**
			 * @fn wxSizer* createRangeSizer()
			 * @brief creates range sizer
			 * @return range sizer
			 */
			wxSizer* createRangeSizer();

			/**
			 * @fn void addRange( 
						wxSizer* sizer,
						const char* title,
						const int minId,
						const int maxId,
						wxTextCtrl** minText,
						wxTextCtrl** maxText
					)
			 * @brief adds range
			 * @param sizer sizer object 
			 * @param title range title
			 * @param minId minimum form ID
			 * @param maxId maximum form ID
			 * @param minText the pointer to store minimum form
			 * @param maxText the pointer to store maximum form
			 */
			void addRange( 
				wxSizer* sizer,
				const char* title,
				const int minId,
				const int maxId,
				wxTextCtrl** minText,
				wxTextCtrl** maxText
			);

			/**
			 * @fn bool setValue( wxTextCtrl* text, void* val, const char* name, const bool isInt )
			 * @brief sets value
			 * @param text text control
			 * @param val the pointer to store value
			 * @param name value name
			 * @param isInt If true, the value is integer value
			 * @return If true, it succeeded to get value
			 */
			bool setValue( wxTextCtrl* text, void* val, const char* name, const bool isInt );

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

#endif		// __KOME_PEAK_SEARCH_DIALOG_H__

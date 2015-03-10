/**
 * @file XYDataHugePanel.h
 * @brief xy data list and drop-down box class
 *
 * @author fujita
 * @date 2012.04.19
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation All rights reserved.
 */

#ifndef __KOME_VIEW_XY_DATA_HUGE_PANELL_H__
#define __KOME_VIEW_XY_DATA_HUGE_PANELL_H__


#include <vector>
#include <set>
#include <wx/wx.h>
#include <wx/spinctrl.h>

namespace kome {
	namespace view {

		class XYData2DListCtrl;
		class StandardPanel;

		/**
		 * @class XYDataHugePanel
		 * @brief xy data list and drop-down box class
		 */
		class XYDataHugePanel : public kome::window::StandardPanel {

		public:
			/**
			 * @fn XYDataHugePanel( wxWindow* parent )
			 * @brief constructor
			 */
			XYDataHugePanel( wxWindow* parent, const bool peaksFlg );

			/**
			 * @fn virtual ~XYDataHugePanel()
			 * @brief destructor
			 */
			virtual ~XYDataHugePanel();

		public:
			/**
			 * @fn void setData( kome::objects::Spectrum* spec, kome::core::XYData* xyData, const bool peaksFlg )
			 * @brief sets spectrum data information
			 * @param spec spectrum
			 * @param xyData xy data
			 * @param peaksFlg peaks flag
			 */
			void setData( kome::objects::Spectrum* spec, kome::core::XYData* xyData, const bool peaksFlg );

			/**
			 * @fn void setData( kome::objects::Chromatogram* chrom, kome::core::XYData* xyData, const bool peaksFlg )
			 * @brief sets chromatogram data information
			 * @param chrom chromatogram 
			 * @param xyData xy data
			 * @param peaksFlg peaks flag
			 */
			void setData( kome::objects::Chromatogram* chrom, kome::core::XYData* xyData, const bool peaksFlg );

		protected:
			/** data grid view */
			XYData2DListCtrl* m_dataXYData;
			/** peaks grid view */
			XYData2DListCtrl* m_peaksXYData;
			/** peaks flag */
			int m_peaksFlg;		
			/** evaluation function combo box */
			wxComboBox* m_EvalFuncCombo;
			/** evaluation function Flag */
			int m_EvalFuncFlag;

		public:
			/**
			 * @fn int getEvalFuncFlag()
			 * @brief get m_EvalFuncFlag
			  * @return m_EvalFuncFlag
			 */
			int getEvalFuncFlag();
	
		protected:
			/**
			 * @fn virtual wxSizer* createMainSizer()
			 * @brief creates main sizer
			 * @return main sizer
			 */
			virtual wxSizer* createMainSizer();

			/** list box */
			wxListBox* m_list;



		public:
			/**
			 * @fn void setGroup( kome::objects::DataGroupNode* group )
			 * @brief sets spectrum group
			 * @param group spectrum group
			 */
			void setGroup( kome::objects::DataGroupNode* group );

		protected:
			/**
			 * @fn void onDeleteProp( wxCommandEvent& evt )
			 * @brief This method is called when property filter "Delete" button is clicked.
			 * @param evt command event
			 */
			void onEvalFuncComboBox( wxCommandEvent& evt );

		private:
			DECLARE_EVENT_TABLE()

	};
	}
}

#endif  // KOME_VIEW_XY_DATA_HUGE_PANELL_H

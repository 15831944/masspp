/**
 * @file PlotPanel.h
 * @brief interfaces of PlotPanel class
 *
 * @author S.Tanaka
 * @date 2009.07.01
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_MATRIX_PLOT_PANEL_H__
#define __KOME_MATRIX_PLOT_PANEL_H__

#include <wx/wx.h>


namespace kome {
	namespace matrix {
		namespace groupplot {

			class PlotCanvas;

			/**
			 * @class PlotPanel
			 * @brief data profile viewing dialog
			 */
			class PlotPanel : public kome::window::StandardPanel {
			public:
				/**
				 * @fn PlotPanel( wxWindow* parent,  const int peakId )
				 * @brief constructor
				 * @param parent parent window
				 * @param peakId peak ID
				 */
				PlotPanel( wxWindow* parent,  const int peakId );

				/**
				 * @fn virtual ~PlotPanel()
				 * @brief destructor
				 */
				virtual ~PlotPanel();

			protected:
				/** peak ID */
				int m_peakId;

				/** canvas */
				PlotCanvas* m_canvas;

				/** group button */
				wxBitmapButton* m_groupButton;

				/** box button */
				wxBitmapButton* m_boxButton;

			public:
				/**
				 * @fn void setPeakId( const int peakId )
				 * @brief sets the peak ID
				 * @param[in] peakId peak ID
				 */
				void setPeakId( const int peakId );

			protected:
				/**
				 * @fn virtual wxSizer* createMainSizer()
				 * @brief creates main sizer
				 * @return main sizer
				 */
				virtual wxSizer* createMainSizer();
	
			protected:
				/**
				 * @fn void onOpenGroupPlot( wxCommandEvent& evt )
				 * @brief open group plot
				 * @param evt
				 */
				void onOpenGroupPlot( wxCommandEvent& evt );

				/**
				 * @fn void onOpenBoxPlot( wxCommandEvent& evt )
				 * @brief open box plot
				 * @param evt
				 */
				void onOpenBoxPlot( wxCommandEvent& evt );

				/**
				 * @fn bool getGroupPlotStatus()
				 * @brief get group plot status
				 * @return status
				 */
				bool getGroupPlotStatus();

				/**
				 * @fn bool getBoxPlotStatus()
				 * @brief get box plot status
				 * @return status
				 */
				bool getBoxPlotStatus();
			private:
				DECLARE_EVENT_TABLE();
			};
		}
	}
}


#endif		// __KOME_MATRIX_PLOT_PANEL_H__

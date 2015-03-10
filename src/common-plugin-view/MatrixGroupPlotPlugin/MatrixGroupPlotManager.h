/**
 * @file MatrixGroupPlotManager.h
 * @brief interfaces of MatrixGroupPlotManager class
 * 
 * @author M.Izumi
 * @date 2012.10.16 
 *
 * Copyright(C) 2012 Shimadzu Corporation. All rights reserved.
 */

#ifndef __KOME_MATRIX_GROUPPLOT_MANAGER_H__
#define __KOME_MATRIX_GROUPPLOT_MANAGER_H__

namespace kome {
	namespace matrix {
		namespace groupplot {
			class PlotPanel;

			/**
			 * @class MatrixGroupPlotManager
			 * @brief Group Plot Manager
			 */
			class MatrixGroupPlotManager {
			protected:
				/**
				 * @fn MatrixGroupPlotManager()
				 * @brief constructor
				 */
				MatrixGroupPlotManager();

				/**
				 * @fn virtual ~MatrixGroupPlotManager()
				 * @brief destructor
				 */
				virtual ~MatrixGroupPlotManager();

			protected:
				/** Plot Panel */
				PlotPanel* m_panel;

			public:
				/**
				 * @fn bool isPanel()
				 * @brief パネルに値が入っているか
				 * @return true:値が入っている false:NULL
				 */
				bool isPanel();

				/**
				 * @fn void setPanel( PlotPanel* panel )
				 * @brief set the PlotPanel
				 * @param panel PlotPanel dialog
				 */
				void setPanel( PlotPanel* panel );

				/**
				 * @fn PlotPanel* getPanel()
				 * @brief get the PlotPanel
				 * @return PlotPanel dialog
				 */
				PlotPanel* getPanel();

				/**
				 * @fn void createPanel( wxWindow* parent, int peakId )
				 * @brief create Plot Panel
				 * @param parent parent window
				 * @param peakId peak id
				 */
				void createPanel( wxWindow* parent, int peakId );

			public:
				/**
				 * @fn static MatrixGroupPlotManager& getInstance()
				 * @brief gets Matrix Group Plot plug-in management object
				 * @return Matrix Group Plot plug-in management object (This is the only object.)
				 */
				static MatrixGroupPlotManager& getInstance();

			};
		}

	}
}

#endif // __KOME_MATRIX_GROUPPLOT_MANAGER_H__

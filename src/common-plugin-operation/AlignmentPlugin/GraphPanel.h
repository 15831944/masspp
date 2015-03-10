/**
 * @file GraphPanel.h
 * @brief interfaces of GraphPanel class
 *
 * @author S.Tanaka
 * @date 2007.08.20
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_ALIGNMENT_GRAPH_PANEL_H__
#define __KOME_ALIGNMENT_GRAPH_PANEL_H__


#include "PanelBase.h"


namespace kome {
	namespace alignment {

		class ChromatogramCanvas;

		/**
		 * @class GraphPanel
		 * @brief graph panel class
		 */
		class GraphPanel : public PanelBase {
		public:
			/**
			 * @fn GraphPanel( wxWindow* parent )
			 * @brief constructor
			 * @param parent parent window
			 */
			GraphPanel( wxWindow* parent );

			/**
			 * @fn virtual ~GraphPanel()
			 * @brief destructor
			 */
			virtual ~GraphPanel();

		protected:
			/** control chromatogram canvas */
			ChromatogramCanvas* m_controlCanvas;

			/** sample chromatogram canvas */
			ChromatogramCanvas* m_sampleCanvas;

		protected:
			/**
			 * @fn virtual wxSizer* createMainSizer()
			 * @brief creates main sizer
			 * @return main sizer
			 */
			virtual wxSizer* createMainSizer();

		private:
			DECLARE_EVENT_TABLE()
		};
	}
}

#endif		// __KOME_ALIGNMENT_GRAPH_PANEL_H__

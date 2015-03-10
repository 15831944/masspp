/**
 * @file ResultPanel.h
 * @brief interfaces of ResultPanel class
 *
 * @author S.Tanaka
 * @date 2007.08.22
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_ALIGNMENT_RESULT_PANEL_H__
#define __KOME_ALIGNMENT_RESULT_PANEL_H__


#include "PanelBase.h"


namespace kome {
	namespace alignment {

		class ResultCanvas;

		/**
		 * @class ResultPanel
		 * @brief correction panel class
		 */
		class ResultPanel : public PanelBase {
		public:
			/**
			 * @fn ResultPanel( wxWindow* parent )
			 * @brief constructor
			 * @param parent parent window
			 */
			ResultPanel( wxWindow* parent );

			/**
			 * @fn virtual ~ResultPanel()
			 * @brief destructor
			 */
			virtual ~ResultPanel();

		protected:
			/** correction canvas */
			ResultCanvas* m_canvas;

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

#endif		// __KOME_ALIGNMENT_RESULT_PANEL_H__

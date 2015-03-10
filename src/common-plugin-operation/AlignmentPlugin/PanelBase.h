/**
 * @file PanelBase.h
 * @brief interfaces of PanelBase class
 *
 * @author S.Tanaka
 * @date 2007.08.20
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_ALIGNMENT_PANEL_BASE_H__
#define __KOME_ALIGNMENT_PANEL_BASE_H__


#include <wx/wx.h>


namespace kome {
	namespace alignment {

		/**
		 * @class PanelBase
		 * @brief base panel class
		 */
		class PanelBase : public kome::window::StandardPanel {
		public:
			/**
			 * @fn PanelBase( wxWindow* parent )
			 * @brief constructor
			 * @param parent parent window
			 */
			PanelBase( wxWindow* parent );

			/**
			 * @fn virtual ~PanelBase()
			 * @brief destructor
			 */
			virtual ~PanelBase();

			// >>>>>>	@Date:2014/02/04	<Add>	A.Ozaki
			//
			/**
			 * @fn createWidgets( void )
			 * @brief create widgets on panel
			 */
			void	createWidgets( void );
			//
			// <<<<<<	@Date:2014/02/04	<Add>	A.Ozaki

		private:
			DECLARE_EVENT_TABLE()
		};
	}
}

#endif		// __KOME_ALIGNMENT_PANEL_BASE_H__

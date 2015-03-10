/**
 * @file PeakInfoDialog.h
 * @brief interfaces of PeakInfoDialog class
 *
 * @author S.Tanaka
 * @date 2012.04.19
 * 
 * Copyright(C) 2012 Shimadzu Corporation. All rights reserved.
 */


#ifndef __KOME_PEAK_INFO_DIALOG_H__
#define __KOME_PEAK_INFO_DIALOG_H__


#include <vector>

#include <wx/wx.h>


namespace kome {
	namespace matrix {
		namespace view {

			/**
			 * @class PeakInfoDialog
			 * @brief auto labeling dialog class
			 */
			class PeakInfoDialog : public kome::window::StandardDialog {
			public:
				/**
				 * @fn PeakInfoDialog( wxWindow* parent, kome::plugin::PluginFunctionItem* item, const int peakId )
				 * @brief constructor
				 * @param[in] parent parent window
				 * @param[in] item function to get the peak information window
				 * @param[in] peakId peak ID
				 */
				PeakInfoDialog( wxWindow* pParent, kome::plugin::PluginFunctionItem* item, const int peakId );
				
				/**
				 * @fn virtual ~PeakInfoDialog()
				 * @brief destructor
				 */
				virtual ~PeakInfoDialog();

			protected:
				/** function */
				kome::plugin::PluginFunctionItem* m_item;

				/** peak ID */
				int m_peakId;

				/** window */
				wxWindow* m_wnd;

				/** sizer */
				wxSizer* m_sizer;

			public:
				/**
				 * @fn void setPeakId( const int peakId )
				 * @brief sets the peak ID
				 * @param[in] peakId peak ID
				 */
				void setPeakId( const int peakId );

			protected:
				/**
				 * @fn void setTitle()
				 * @brief sets the dialog title
				 */
				void setTitle();
				
			protected:
				/**
				 * @fn virtual wxSizer* createMainSizer();
				 * @brief creates main sizer (override method)
				 * @return field sizer
				 */
				virtual wxSizer* createMainSizer();

				/**
				 * @fn virtual wxSizer* createButtonSizer();
				 * @brief creates button sizer (override method)
				 * @return field sizer
				 */
				virtual wxSizer* createButtonSizer();

				/** 
				 * @fn void close( wxCloseEvent& evt )
				 * @brief close
				 */
				void close( wxCloseEvent& evt );

			private:
				DECLARE_EVENT_TABLE();
			};
		}
	}
}


#endif	//  __KOME_PEAK_INFO_DIALOG_H__

/**
 * @file ThresholdSettingDialog.h
 * @brief implements of ThresholdSettingDialog class
 *
 * @author M.Izumi
 * @date 2011.03.29
 * 
 * Copyright(C) 2013 Shimadzu Corporation. All rights reserved.
 */

#ifndef __KOME_VIEW_THRESHOLDSETTINGDIALOG_H__
#define __KOME_VIEW_THRESHOLDSETTINGDIALOG_H__

#include <wx/wx.h>

class ThresholdSettingDialog : public kome::window::StandardDialog {
	public:
		/**
		* @fn ThresholdSettingDialog( wxWindow* parent )
		* @brief constructor
		* @param parent parent window
		*/
		ThresholdSettingDialog( wxWindow* parent );       

		/**
		* @fn virtual ~ThresholdSettingDialog()
		* @brief destructor
		*/
		virtual ~ThresholdSettingDialog();               
	
	protected:
		/** 閾値を表示するテキスト */
		wxTextCtrl* m_ThresholdText;
		
		/** 閾値 */
		double m_Threshold;
	protected:
		/**
		 * @fn virtual wxSizer* createMainSizer()
		 * @brief creates main sizer (override method)
		 * @return main sizer
		 */
		virtual wxSizer* createMainSizer();       

		/**
		 * @fn virtual bool TransferDataFromWindow()
		 * @brief transfer window control data to value
		 * @return If a transfer failed, this method returns false.
		 */		
		virtual bool TransferDataFromWindow(); 

		/**
		 * @fn virtual bool TransferDataToWindow()
		 * @brief transfer value to window control data
		 * @return If a transfer failed, this method returns false.
		 */
		virtual bool TransferDataToWindow();     
	public:
		/**
		 * @fn void setThreshold( double d )
		 * @brief set threshold 
		 * @parm d threshold
		 */
		void setThreshold( double d );

		/**
		 * @fn double getThreshold()
		 * @brief get threshold 
		 * @return threshold
		 */
		double getThreshold();


	private:
		 DECLARE_EVENT_TABLE()
};

#endif		//  #ifndef __KOME_VIEW_THRESHOLDSETTINGDIALOG_H__

/**
 * @file SampleCloseCheckDialog.h
 * @brief interfaces of SampleCloseCheckDialog class
 *
 * @author M.Izumi
 * @date 2010.11.02
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation. All rights reserved.
 */

#ifndef __KOME_IO_SAMPLE_CLOSE_CHECK_DIALOG_H__
#define __KOME_IO_SAMPLE_CLOSE_CHECK_DIALOG_H__

#include <wx/wx.h>

 /**
  * @class SampleCloseCheckDialog
  * @brief sample close check class
  */
class SampleCloseCheckDialog : public kome::window::StandardDialog {
	public:
		/**
		* @fn SampleCloseCheckDialog( wxWindow* parent )
		* @brief constructor
		* @param parent parent window
		*/
		SampleCloseCheckDialog( wxWindow* parent );       

		/**
		* @fn virtual ~SampleCloseCheckDialog()
		* @brief destructor
		*/
		virtual ~SampleCloseCheckDialog();               

	protected:
		/** Check List Box */
		wxCheckListBox* m_chkList;				
		
		/**
		* @struct CheckListBox Information		 
		* @brief struct that has name and Check status
		*/
		struct ChekListBoxInfo {
			std::string  strName;				// name
			bool bCheck;						// checkBox Status
		};

		/** check list box Information */
		std::vector< ChekListBoxInfo > m_ChkListInfos;
		
	protected:
		/**
		* @fn virtual wxSizer* createMainSizer()
		* @brief creates main sizer (override method)
		* @return main sizer
		*/
		virtual wxSizer* createMainSizer();       

		/**
		* @fn virtual bool TransferDataToWindow()
		* @brief transfer value to window control data
		* @return If a transfer failed, this method returns false.
		*/
		virtual bool TransferDataToWindow();        
		
		/**
		* @fn void onCheck( wxCommandEvent& evt )
		* @frief This method is called when list items are checked
		* @param evt check list box event information
		*/
		void onCheck( wxCommandEvent& evt );

	public:
		/**
		* @fn void setChkListInfo(std::string strName, bool bChek);
		* @brief set structure
		* @param strName sample name
		* @param bChek checkBox Status
		*/
		void setChkListInfo(std::string strName, bool bChek);
		
		/**
		* @fn bool isChecked(const unsigned int index)
		* @brief sample whether the check box list is checked
		* @return If the check box is checked, this method returns true.
		*/
		bool isChecked(const unsigned int index);			
	private:
			 DECLARE_EVENT_TABLE()
};

#endif		//  #ifndef __KOME_IO_SAMPLE_CLOSE_CHECK_DIALOG_H__

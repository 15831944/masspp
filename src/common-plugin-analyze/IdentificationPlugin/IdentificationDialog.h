/**
 * @file IdentificationDialog.h
 * @brief interfaces of IdentificationDialog class
 *
 * @author H.Parry
 * @date 2012.01.30
 * 
 * Copyright(C) 2012 Shimadzu Corporation. All rights reserved.
 */


#ifndef __KOME_IDENT_IDENTIFICATION_DIALOG_H__
#define __KOME_IDENT_IDENTIFICATION_DIALOG_H__


#include <wx/wx.h>
#include "IdentificationDialogBase.h"              
#include "IdentificationSpectrumGetter.h"


namespace kome {
	namespace ident {

		class SpectrumListCtrl;

		/**
		 * @class IdentificationDialog
		 * @brief auto labeling dialog class
		 */
		class IdentificationDialog : public IdentificationDialogBase {
		public:
			/**
			 * @fn IdentificationDialog(
			 * @brief constructor
			 * @param[in] parent parent window
			 * @param[in] acSample active sample
			 */
			IdentificationDialog( wxWindow* parent, kome::objects::Sample* acSample );
			
			/**
			 * @fn virtual ~IdentificationDialog()
			 * @brief destructor
			 */
			virtual ~IdentificationDialog();

		protected:
			/** list control */
			SpectrumListCtrl* m_list;

			/** sample */
			kome::objects::Sample* m_sample;

			/** search engine */
			kome::ident::SearchEngine* m_currentEngine;
			
		public:
			/**
			 * @fn virtual SpectrumGetter* createSpectrumGetter()
			 * @brief creates the spectrum getter (override method)
			 * @return spectrum getter
			 */
			virtual SpectrumGetter* createSpectrumGetter();

		protected:
			/**
			 * @fn virtual wxSizer* createSpectraSizer()
			 * @brief creates spectra sizer
			 * @return spectra sizer
			 */
			virtual wxSizer* createSpectraSizer();

		protected:
			/**
			 * @fn virtual void onChangeEngine( kome::ident::SearchEngine* engine )
			 * @brief This method is called when selected search engine is changed.
			 * @param[in] engine selected search engine
			 */
			virtual void onChangeEngine( kome::ident::SearchEngine* engine );

		private:
			DECLARE_EVENT_TABLE();
		};
	}
}

#endif	//  __KOME_IDENT_IDENTIFICATION_DIALOG_H__

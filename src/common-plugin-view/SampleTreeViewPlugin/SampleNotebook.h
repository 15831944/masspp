/**
 * @file SampleNotebook.h
 * @brief interfaces of SampleNotebook
 *
 * @author S.Tanaka
 * @date 2011.07.22
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation
 */


#ifndef __KOME_VIEW_SAMPLE_NOTEBOOK_H__
#define __KOME_VIEW_SAMPLE_NOTEBOOK_H__


#include <wx/wx.h>
#include <wx/notebook.h>


namespace kome {
	namespace view {

		/**
		 * @class SampleNotebook
		 * @brief sample notebook
		 */
		class SampleNotebook : public wxNotebook {
		public:
			/**
			 * @fn SampleNotebook()
			 * @brief constructor
			 */
			SampleNotebook( wxWindow* parent );

			/**
			 * @fn virtual ~SampleNotebook()
			 * @brief destructor
			 */
			virtual ~SampleNotebook();

		protected:
			/**
			 * @fn void onChangePage( wxNotebookEvent& evt )
			 * @breif This method is called when a page is changed.
			 * @param[in] evt notebook event
			 */
			void onChangePage( wxNotebookEvent& evt );

		private:
			DECLARE_EVENT_TABLE()

		};
	}
}

#endif	//  __KOME_VIEW_SAMPLE_NOTEBOOK_H__

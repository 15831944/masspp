/**
 * @file RadioBoxBook.h
 * @brief interfaces of RadioBoxBook class
 *
 * @author S.Tanaka
 * @date 2013.01.22
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_WINDOW_RADIO_BOX_BOOK_H__
#define __KOME_WINDOW_RADIO_BOX_BOOK_H__


#include <wx/wx.h>
#include <wx/bookctrl.h>
#include <wx/radiobox.h>


namespace kome {
	namespace window {

		/**
		 * @class RadioBoxBook
		 * @brief standard panel
		 */
		class WINDOW_MANAGER_CLASS RadioBoxBook : public wxBookCtrlBase {
		public:
			/**
			 * @fn RadioBoxBook()
			 * @brief constructor
			 */
			RadioBoxBook();

			/**
			 * @fn RadioBoxBook(
					wxWindow* parent,
					int id = wxID_ANY,
					wxPoint pos = wxDefaultPosition,
					wxSize size = wxDefaultSize
				)
			 * @brief constructor
			 * @param[in] parent parent window
			 * @param[in] id window ID
			 * @param[in] pos window position
			 * @param[in] size window size
			 */
			RadioBoxBook(
				wxWindow* parent,
				int id = wxID_ANY,
				wxPoint pos = wxDefaultPosition,
				wxSize size = wxDefaultSize
			);

			/**
			 * @fn virtual ~RadioBoxBook()
			 * @brief destructor
			 */
			virtual ~RadioBoxBook();

		protected:
			/** selection */
			int m_selection;

			/** pages */
			std::vector< std::string > m_pages;

		protected:
			/**
			 * @fn void init()
			 * @brief initializes
			 */
			void init();

			/**
			 * @fn wxRadioBox* getRadioBox() const
			 * @brief gets the radio box
			 * @return radio box
			 */
			wxRadioBox* getRadioBox() const;

			/**
			 * @fn void createRadioBox()
			 * @brief creates radio box
			 * @return radio box
			 */
			void createRadioBox();

		public:
			/**
			 * @fn bool Create( wxWindow* parent, int id, wxPoint pos, wxSize size )
			 * @brief creates control
			 * @param[in] parent parent window
			 * @param[in] id window ID
			 * @param[in] pos window position
			 * @param[in] size window size
			 * @return If true, it succeeded to creates control
			 */
			bool Create( wxWindow* parent, int id, wxPoint pos, wxSize size );

		public:
			/**
			 * @fn virtual int GetSelection() const
			 * @brief returns the currently selected page, or wxNOT_FOUND if none was selected. (override method)
			 * @return selected page
			 */
			virtual int GetSelection() const;

			/**
			 * @fn virtual bool SetPageText( size_t page, const wxString& text )
			 * @brief Sets the text for the given page (override method)
			 * @param[in] page page index
			 * @param[in] text page text
			 * @return If true, it succeeded to set the page text
			 */
			virtual bool SetPageText( size_t page, const wxString& text );

			/**
			 * @fn virtual wxString GetPageText( size_t page ) const
			 * @brief returns the string for the given page. (override method)
			 * @param[in] page page index
			 * @return page text
			 */
			virtual wxString GetPageText( size_t page ) const;

			/**
			 * @fn virtual bool SetPageImage( size_t page, int image )
			 * @brief sets the image index for the given page. (override method)
			 * @param[in] page page index
			 * @param[in] image image index
			 * @return If true, it succeeded to set the image
			 */
			virtual bool SetPageImage( size_t page, int image );

			/**
			 * @fn virtual int GetPageImage( size_t page ) const
			 * @brief returns the image index for the given page. (override method)
			 * @param[in] page page index
			 * @return image index
			 */
			virtual int GetPageImage( size_t page ) const;

			/**
			 * @fn virtual wxSize CalcSizeFromPage(const wxSize& sizePage) const
			 * @brief calculates size from page
			 * @param[in] sizePage page size
			 * @return size window size
			 */
			virtual wxSize CalcSizeFromPage(const wxSize& sizePage) const;

			/**
			 * @fn virtual bool InsertPage( size_t index, wxWindow* page, const wxString& text, bool select, int imageId )
			 * @brief inserts a new page at the specified position. (override method)
			 * @param[in] index the position for the new page
			 * @param[in] page the new page
			 * @param[in] text the text for the new page
			 * @param[in] select whether the page should be selected
			 * @param[in] imageId the optional image index for the new page
			 * @return If true, it succeeded to insert the page.
			 */
			virtual bool InsertPage( size_t index, wxWindow* page, const wxString& text, bool select, int imageId );

			/**
			 * @fn virtual int SetSelection( size_t page )
			 * @brief sets the selection for the given page, returning the previous selection. (override method)
			 * @param[in] page the selection 
			 * @return previous selection.
			 */
			virtual int SetSelection( size_t page );

			/**
			 * @fn virtual int ChangeSelection( size_t page )
			 * @brief changes the selection for the given page, returning the previous selection. (override method)
			 * @param[in] page the selection 
			 * @return previous selection.
			 */
			virtual int ChangeSelection( size_t page );

			/**
			 * @fn virtual bool DeleteAllPages()
			 * @brief deletes all pages. (override method)
			 * @return If true, it succeeded to delete pages.
			 */
			virtual bool DeleteAllPages();

			/**
			 * @fn virtual void UpdateSelectedPage( size_t newsel )
			 * @brief updates selected page
			 * @param[in] newsel new selection page index
			 */
			virtual void UpdateSelectedPage( size_t newsel );

			/**
			 * @fn virtual wxBookCtrlBaseEvent* CreatePageChangingEvent() const
			 * @brief creates book control event
			 * @return book control event
			 */
			virtual wxBookCtrlBaseEvent* CreatePageChangingEvent() const;
			
			/**
			 * @fn virtual void MakeChangedEvent(wxBookCtrlBaseEvent& evt )
			 * @brief makes changed event
			 * @param[out] evt event
			 */
			virtual void MakeChangedEvent(wxBookCtrlBaseEvent& evt );

			/**
			 * @fn virtual wxWindow* DoRemovePage( size_t page )
			 * @brief removes page
			 * @param[in] page page index
			 * @return removed window
			 */
			virtual wxWindow* DoRemovePage( size_t page );

	protected:		
			/**
			 * @fn virtual bool TransferDataFromWindow()
			 * @brief transfer window control data to value
			 * @return If a transfer failed, this method returns false.
			 */
			virtual bool TransferDataFromWindow();

			/**
			 * @fn virtual bool Validate()
			 * @brief validates the values
			 * @return If any of the valudations failed, this method returns false.
			 */
			virtual bool Validate();

		protected:
			/**
			 * @fn void onRadioBoxSelected( wxCommandEvent& evt )
			 * @brief This method is called when the radio box is selected
			 * @param[in] evt event information
			 */
			void onRadioBoxSelected( wxCommandEvent& evt );

		private:
			DECLARE_EVENT_TABLE();
		};
	}
}


#endif		// __KOME_WIND

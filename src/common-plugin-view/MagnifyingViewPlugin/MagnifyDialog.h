/**
 * @file MagnifyDialog.h
 * @brief interfaces of MagnifyDialog class
 *
 * @author N.Tashiro
 * @date 2012.03.06
 * 
 * Copyright (C) 2014 Shimadzu Corporation All rights reserved.
 */

#ifndef __KOME_VIEW_SEL_RANGE_MAGNIFY_DIALOG_H__
#define __KOME_VIEW_SEL_RANGE_MAGNIFY_DIALOG_H__


#include <wx/wx.h>

namespace kome {
	namespace view {
		// ----------------------
		// Magnify Dialog
		// ----------------------
		class MagnifyDialog : public kome::window::ParametersAndHelpDialog {
		public:
			/**
			 * @fn MagnifyDialog()
			 * @brief constructor
			 * @param[in] parent parent window
			 * @param[in] spec spectrum object
			 */
			MagnifyDialog( wxWindow* parent, kome::objects::Spectrum* spec );

			/**
			 * @fn virtual ~MagnifyDialog()
			 * @brief destructor
			 */
			virtual ~MagnifyDialog();

		protected:
			/** ID */
			int m_id;

			/** start position  */
			double m_startPos;

			/** end position  */
			double m_endPos;

			/** scale */
			int m_scale;

			/** objects spectrum  */
			kome::objects::Spectrum* m_spec;

			/** parameters */
			kome::plugin::ParameterSettings m_params;

			/** settings */
			kome::objects::SettingParameterValues m_settings;

			/** flag */
			bool m_flg;

			/** Clear button */
			wxButton* m_btnClear;

		protected:
			/**
			 * @fn virtual wxSizer* createMainSizer()
			 * @brief creates main sizer (override method)
			 * @return main sizer
			 */
			virtual wxSizer* createMainSizer();

			/**
			 * @fn virtual wxSizer* createButtonSizer()
			 * @brief creates main sizer (override method)
			 * @return button sizer
			 */
			virtual wxSizer* createButtonSizer();

			/**
			 * @fn virtual bool TransferDataFromWindow()
			 * @brief transfer window control data to value
			 * @return If a transfer failed, this method returns false.
			 */
			virtual bool TransferDataFromWindow();

			/**
			 * @fn virtual bool TransferDataToWindow()
			 * @brief transfer value data to window controls
			 * @return If a transfer failed, this method returns false.
			 */
			virtual bool TransferDataToWindow();


		public:
			/**
			 * @fn double getStartPos()
			 * @brief get start position
			 * return start position
			 */
			double getStartPos();

			/**
			 * @fn void double getStartPos()
			 * @brief set start position
			 * @param sPos start position
			 */
			void setStartPos( double sPos );

			/**
			 * @fn double getEndPos()
			 * @brief get end position
			 * return end position
			 */
			double getEndPos();

			/**
			 * @fn void double getEndPos()
			 * @brief set end position
			 * @param ePos end position
			 */
			void setEndPos( double ePos );

			/**
			 * @fn int getScale()
			 * @brief get scale
			 * return scale
			 */
			int getScale();

			/**
			 * @fn void setScale( int scale )
			 * @brief set scale
			 * @param scale scale
			 */
			void setScale( int scale );

			/**
			 * @fn int getID()
			 * @brief get ID
			 * return ID
			 */
			int getID();

			/**
			 * @fn void setID( int id )
			 * @brief set ID
			 * @param id ID
			 */
			void setID( int id );

			/**
			 * @fn kome::objects::Spectrum* getSpec()
			 * @brief get spectrum
			 * return spectrum
			 */
			kome::objects::Spectrum* getSpec();

			/**
			 * @fn void setSpec( kome::objects::Spectrum* spec )
			 * @brief set spectrum
			 * @param spec spectrum
			 */
			void setSpec( kome::objects::Spectrum* spec );

		private:
			/**
			 * @fn void setWindowSettings()
			 * @brief set Window settings
			 */
			void setWindowSettings();

		protected:
			/**
			 * @fnvoid onOkButton( wxCommandEvent& evt )
			 * @brief This method is called when the OK button is clicked.
			 * @param[in] evt click event
			 */
			void onOkButton( wxCommandEvent& evt );

			/**
			 * @fnvoid onClear( wxCommandEvent& evt )
			 * @brief This method is called when the Clear button is clicked.
			 * @param[in] evt click event
			 */
			void onClear( wxCommandEvent& evt );
		private:
			DECLARE_EVENT_TABLE();
		};
	}
}

#endif // __KOME_VIEW_SEL_RANGE_MAGNIFY_DIALOG_H__

/**
 * @file SampleMergeDialog.h
 * @brief interfaces of SampleMergeDialog class
 *
 * @author S.Tanaka
 * @date 2009.03.03
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_SAMPLE_MERGE_DIALOG_H__
#define __KOME_SAMPLE_MERGE_DIALOG_H__


#include <wx/checklst.h>


namespace kome {
	namespace merged {

		/**
		 * @class SampleMergeDialog
		 * @brief sample merge dialog
		 */
		class SampleMergeDialog : public kome::window::StandardDialog {
		public:
			/**
			 * @fn SampleMergeDialog( wxWindow* parent )
			 * @brief constructor
			 * @param parent parent window
			 */
			SampleMergeDialog( wxWindow* parent );

			/**
			 * @fn virtual ~SampleMergeDialog()
			 * @brief destructor
			 */
			virtual ~SampleMergeDialog();

		protected:
			/** RT tolerance */
			double m_rtTol;

			/** m/z tolerance */
			double m_mzTol;

			/** select sample array */
			std::vector< kome::objects::Sample* > m_selectedSamples;

			/** samples */
			std::vector< kome::objects::Sample* > m_samples;

			/** parameters */
			kome::plugin::ParameterSettings m_params;

			/** settings */
			kome::objects::SettingParameterValues m_settings;

		protected:
			/** group list */
			wxCheckListBox* m_groupList;

		public:
			/**
			 * @fn double getRtTol()
			 * @brief gets RT tolerance
			 * @return RT tolerance
			 */
			double getRtTol();

			/**
			 * @fn double getMzTol()
			 * @brief gets m/z tolerance
			 * @return m/z tolerance
			 */
			double getMzTol();

			/**
			 * @fn unsigned int getNumberOfSamples()
			 * @brief gets the number of samples
			 * @return the number of samples
			 */
			unsigned int getNumberOfSamples();

			/**
			 * @fn kome::objects::Sample* getSample( const unsigned int idx )
			 * @brief gets sample
			 * @return sample
			 */
			kome::objects::Sample* getSample( const unsigned int idx );
		protected:
			/**
			 * @fn virtual wxSizer* createMainSizer()
			 * @brief creates main sizer
			 * @return main sizer
			 */
			virtual wxSizer* createMainSizer();

			
		protected:
			/**
			 * @fn virtual bool TransferDataFromWindow()
			 * @brief transfer window control data to value (override method)
			 * @return If a transfer failed, this method returns false.
			 */
			virtual bool TransferDataFromWindow();

		private:
			DECLARE_EVENT_TABLE();
		};
	}
}


#endif   // __KOME_SAMPLE_MERGE_DIALOG_H__

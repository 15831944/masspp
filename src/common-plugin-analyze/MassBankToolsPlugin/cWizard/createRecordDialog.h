/**
 * @file createRecordsWizardDialog.h
 * @brief MassBank's create records
 *
 * @author M.Fukuda
 * @date 2013.01.18
 *
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#ifndef __KOME_MASSBANK_CREATE_RECORD_WIZARD_H__
#define __KOME_MASSBANK_CREATE_RECORD_WIZARD_H__

#define ID_NEW_PROJ_WIZARD 5000

namespace kome {
namespace massbank {
namespace wizard {
		
		// -- Forward declaration --
		class TargetSpectraWizardPage;
		class ExportSettingsWizardPage;
		class PeakDetectionWizardPage;
		class RecordSettingsWizardPage;
		class ChemicalStructureWizardPage;
		class SampleInformationWizardPage;
		class InstrumentWizardPage;
		class SpectrumInformationWizardPage;
		class ChromatographyInformationWizardPage;
		class PrecurosrMolecularIonWizardPage;
		class SoftwareInformationWizardPage;
		class PeakAnnotationWizardPage;
		class FinalSequenceWizardPage;

		class RecordElements;
}}}

namespace kome {
	namespace massbank {

		/**
		 * @class createRecordsWizardDialog
		 * @brief Dialog for create massbank's record.
		 */
		class createRecordsWizardDialog
			: public kome::window::Wizard {
		public:
			/**
			 * @brief constructor
			 * @param[in] parent parent window
			 */
			createRecordsWizardDialog(wxWindow* parent);

			/**
			 * @brief destructor
			 */
			virtual ~createRecordsWizardDialog();
			
			wxWizardPageSimple* getFirstPage();

		protected:

			void OnWizardFinished(wxWizardEvent& event);

		protected:
			/**
			 * @fn virtual void onSkip( wxWizardPage* currentPage )
			 * @brief This method is called when the Skip button is clicked.
			 * @param[in] currentPage current page
			 */
			virtual void onSkip( wxWizardPage* currentPage ){}

			/**
			 * @fn virtual bool canSkip( wxWizardPage* currentPage )
			 * @brief judges whether the Skip button is enabled or not
			 * @return If true, the Skip button is enabled.
			 */
			virtual bool canSkip( wxWizardPage* currentPage ){return true;}

			/**
			 * @fn virtual void onNext( wxWizardPage* currentPage );
			 * @brief This method is called when the Next button is clicked.
			 * @param[in] currentPage current page
			 */
			virtual void onNext( wxWizardPage* currentPage ){}

			/**
			 * @fn virtual void onPrev( wxWizardPage* currentPage )
			 * @brief This method is called when the Back button is clicked.
			 * @param[in] currentPage current page
			 */
			virtual void onPrev( wxWizardPage* currentPage ){}

		private:
			// Wizard Pages
			wizard::TargetSpectraWizardPage*             m_page1;
			wizard::ExportSettingsWizardPage*            m_page2;
			wizard::PeakDetectionWizardPage*             m_page3;
			wizard::RecordSettingsWizardPage*            m_page4;
			wizard::ChemicalStructureWizardPage*         m_page5;
			wizard::SampleInformationWizardPage*         m_page6;
			wizard::InstrumentWizardPage*                m_page7;
			wizard::SpectrumInformationWizardPage*       m_page8;
			wizard::ChromatographyInformationWizardPage* m_page9;
			wizard::PrecurosrMolecularIonWizardPage*     m_page10;
			wizard::SoftwareInformationWizardPage*       m_page11;
			wizard::PeakAnnotationWizardPage*            m_page12;
			wizard::FinalSequenceWizardPage*             m_final;

			// MassBank Record Elements Keeper.
			std::shared_ptr<wizard::RecordElements> rcElmPtr_;

			//** parameter definitions */

			kome::plugin::ParameterSettings inParams_;
			//** parameter values */
			kome::objects::SettingParameterValues inSettings_;
				
			bool finishedFg;

			DECLARE_EVENT_TABLE();
		};
	} // end of namespace massbank
} // end of namespace kome

#endif __KOME_MASSBANK_CREATE_RECORD_WIZARD_H__

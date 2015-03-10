/**
 * @file MatrixPeaksMergePanel.h
 * @brief interfaces of IdentificationPanel class
 *
 * @author M.Izumi
 * @date 2013.01.15
 * 
 * Copyright(C) 2013 Shimadzu Corporation. All rights reserved.
 */

#ifndef __KOME_MATRIX_PEAKS_MERGE_PANEL_H__
#define __KOME_MATRIX_PEAKS_MERGE_PANEL_H__

namespace kome {
	namespace mergepeaks {
		
		/**
		 * @class PeakMagePanel
		 * @brief peak mage panel class
		 */
		class PeakMagePanel : public kome::window::PeakMatrixSettingsPageBase {
		public:
			/**
			 * @fn PeakMagePanel( wxWizard* pParent, const char* name )
			 * @brief constructor
			 */
			PeakMagePanel( wxWizard* pParent, const char* name );

			/**
			 * @fn virtual ~PeakMagePanel()
			 * @brief destructor
			 */
			virtual ~PeakMagePanel();
		protected:
			/** parameters */
			kome::plugin::ParameterSettings m_params;

			/** settings */
			kome::objects::SettingParameterValues m_settings;
			
			/** rt tol */
			double m_rtTol;
			
			/** m/z tol */
			double m_mzTol;

			/** peak matrix infomation */
			struct Info{
				int peakId;
				bool substance;
				double sum;
			};
		protected:
			/**
			* @fn virtual void createInnerSizer()
			* @brief creates inner sizer
			* @param InnerSizer inner sizer
			*/
			virtual void createInnerSizer(wxStaticBoxSizer* InnerSizer);
			
			/**
			 * @fn wxSizer* createSpectraSizer()
			 * @brief creates spectra sizer
			 * @return spectra sizer
			 */
			wxSizer* createSpectraSizer();
			
			/**
			 * @fn virtual bool TransferDataFromWindow()
			 * @brief transfer window control data to value
			 * @return If a transfer failed, this method returns false.
			 */
			virtual bool TransferDataFromWindow();

		protected:
		
			/**
			 * @fn void getSubstance(
						std::vector< int > searches,
						int searchId,
						std::vector< int > peaks, 
						std::vector< int >& subtrance
					)
			 * @brief get substance
			 * @param[in] searches
			 * @param[in] searchId
			 * @param[in] peaks
			 * @param[out] subtrance
			 */
			void getSubstance(
				std::vector< int > searches,
				int searchId,
				std::vector< int > peaks, 
				std::vector< int >& subtrance
				);

		public:
			/**
			 * @fn void getSubstanceCmd(
						std::vector< int > searches,
						int searchId,
						std::vector< int > peaks, 
						std::vector< int >& subtrance
					)
			 * @brief get substance from command line function
			 * @param[in] searches
			 * @param[in] searchId
			 * @param[in] peaks
			 * @param[out] subtrance
			 */
			void getSubstanceCmd(
				std::vector< int > searches,
				int searchId,
				std::vector< int > peaks, 
				std::vector< int >& subtrance
				);

		protected:
			/**
			 * @fn void DeleteMatrixPeak( int matrixId, std::vector< int > resPeaks, std::vector<int> substance  )
			 * @brief delete matrix peak
			 * @param matrixId
			 * @param resPeaks
			 * @param substance
			 */
			void DeleteMatrixPeak( int matrixId, std::vector< int > resPeaks, std::vector<int> substance );


		public:

			/**
			 * @fn void DeleteMatrixPeakCmd( int matrixId, std::vector< int > resPeaks, std::vector<int> substance  )
			 * @brief delete matrix peak from command line function
			 * @param matrixId
			 * @param resPeaks
			 * @param substance
			 */
			void DeleteMatrixPeakCmd( int matrixId, std::vector< int > resPeaks, std::vector<int> substance );


		protected:
			/**
			 * @fn void getTotalValueOfPeak( int matrixId, int peak, double& sum )
			 * @brief get total value of peak
			 * @param matrixId
			 * @param sum
			 */
			void getTotalValueOfPeak( int matrixId, int peak, double& sum );
		public:
			/**
			 * @fn virtual bool IsValid()
			 * @brief validate panel settings
			 * @return If validation failed, this method returns false.
			 */
			virtual bool IsValid();

			/**
			 * @fn virtual bool PreApplyValidationCheck(std::string& sError)
			 * @brief Perform Apply-time validation checks (i.e. checks that must not be done in IsValid function)
			 * @param sError error string from a single panel's pre-apply validation
			 * @return If validation succeeded, return true, otherwise false.
			 */
			virtual bool PreApplyValidationCheck(std::string& sError);

			/**
			 * @fn void SavePanelSettings(kome::core::Progress *pProgress)
			 * @brief This method is called when saving values.
			 * @param pProgress progress
			 * @return If saSavePanelSettingsving values failed, this method returns false.
			 */
			virtual bool SavePanelSettings(kome::core::Progress *pProgress);

			/**
			* @fn void updateStandardSamples(bool bReadFromINIfile = true)
			* @brief Update standard samples on this panel
			* @param bReadFromINIfile whether to read from the INI file
			*/
			virtual void updateStandardSamples(bool bReadFromINIfile = true);
				
			/**
			* @fn void RefreshPanelContentsForMatrix()
			* @brief refresh panel contents for matrix
			*/
			virtual void RefreshPanelContentsForMatrix();

		private:
			DECLARE_EVENT_TABLE();
		};
	}
}





#endif // __KOME_MATRIX_PEAKS_MERGE_PANEL_H__

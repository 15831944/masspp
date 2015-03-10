/**
 * @file ProfilePanel.h
 * @brief interfaces of ProfilePanel class
 *
 * @author S.Tanaka
 * @date 2008.04.28
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_MATRIX_PROFILE_PANEL_H__
#define __KOME_MATRIX_PROFILE_PANEL_H__


namespace kome {
	namespace matrix {
		namespace overlapping {

			class SpectrumDataCanvas;
			class ChromatogramDataCanvas;
			class PickedProfileDialog;

			/**
			 * @class ProfilePanel
			 * @brief data profile viewing dialog
			 */
			class ProfilePanel : public kome::window::StandardPanel {
			public:
				/**
				 * @fn ProfilePanel( wxWindow* parent,  const int peakId )
				 * @brief constructor
				 * @param parent parent window
				 * @param peakId peak ID
				 */
				ProfilePanel( wxWindow* parent,  const int peakId );

				/**
				 * @fn virtual ~ProfilePanel()
				 * @brief destructor
				 */
				virtual ~ProfilePanel();

			public:
				/**
				 * @struct DataInfo
				 * @brief data information
				 */
				struct DataInfo {
					int sampleId;
					int profileId;
					bool isChrom;
					double leftX;
					double leftY;
					double rightX;
					double rightY;
					std::string name;
					COLOR color;
				};

			protected:
				/**
				 * @struct GroupInfo
				 * @brief group information
				 */
				struct GroupInfo {
					int id;
					COLOR color;
					std::string name;
				};

			protected:
				/** peak ID */
				const int m_peakId;

				/** chromatogram flag */
				bool m_chromFlg;

				/** data canvas */
				kome::window::ProfileCanvas* m_canvas;

				/** picked profile dialog */
				PickedProfileDialog* m_dlg;

				/** edited flag */
				bool m_edited;

				/** normalized */
				bool m_normalized;

			public:
				/**
				 * @fn void setPeakId( const int peakId )
				 * @brief sets the peak ID
				 * @param[in] peakId peak ID
				 */
				void setPeakId( const int peakId );

				/**
				 * @fn int getPeakId()
				 * @brief gets peak ID
				 * @return peak ID
				 */
				int getPeakId();

			protected:
				/**
				 * @fn void getData( std::vector< DataInfo >& data )
				 * @brief gets profile data
				 * @param data the array to store data
				 */
				void getData( std::vector< DataInfo >& data );

			public:
				/**
				 * @fn void commitPeaks()
				 * @brief commits peaks
				 */
				void commitPeaks();

				/**
				 * @fn void rollbackPeaks()
				 * @brief rollbacks peaks
				 */
				void rollbackPeaks();

			protected:
				/**
				 * @fn void setNormalized()
				 * @breif sets normalized flag value
				 */
				void setNormalized();

			protected:
				/**
				 * @fn virtual wxSizer* createMainSizer()
				 * @brief creates main sizer
				 * @return main sizer
				 */
				virtual wxSizer* createMainSizer();

			protected:
				/**
				 * @fn void onPickButton( wxCommandEvent& evt )
				 * @brief This method is called when the pick button is clicked
				 * @param evt button event information
				 */
				void onPickButton( wxCommandEvent& evt );

				/**
				 * @fn void onAlignButton( wxCommandEvent& evt )
				 * @brief This method is called when the align button is clicked
				 * @param evt button event information
				 */
				void onAlignButton( wxCommandEvent& evt );

				/**
				 * @fn void onPeakButton( wxCommandEvent& evt )
				 * @brief This method is called when the peaks button is clicked
				 * @param evt button event information
				 */
				void onPeakButton( wxCommandEvent& evt );

			private:
				DECLARE_EVENT_TABLE();
			};
		}
	}
}


#endif		// __KOME_MATRIX_PROFILE_PANEL_H__

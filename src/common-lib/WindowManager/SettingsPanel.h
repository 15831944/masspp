/**
 * @file SettingsPanel.h
 * @brief interfaces of SettingsPanel class
 *
 * @author S.Tanaka
 * @date 2007.02.02
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_WINDOW_SETTINGS_PANEL_H__
#define __KOME_WINDOW_SETTINGS_PANEL_H__


#include "HelpGetterPanel.h"


#include <string>
#include <vector>
#include <list>
#include <map>

#include <wx/wx.h>


namespace kome {
	namespace window {

		class SettingsPanelEvent;

		/**
		 * @class SettingsPanel
		 * @brief each settings panel class
		 */
		class WINDOW_MANAGER_CLASS SettingsPanel : public HelpGetterPanel {
		public:
			/**
			 * @fn SettingsPanel(
					wxWindow* parent,
					kome::plugin::SettingsPage* page,
					const bool dfltBtn = true,
					kome::objects::SettingParameterValues* settings = NULL,
					const bool saveVal = true
				)
			 * @brief constructor
			 * @param[in] parent parent window
			 * @param[in] page settings page information
			 * @param[in] dfltBtn default button flag
			 * @param[in] settings the object to store settings values
			 * @param[in] saveVal save value flag
			 */
			SettingsPanel(
				wxWindow* parent,
				kome::plugin::SettingsPage* page,
				const bool dfltBtn = true,
				kome::objects::SettingParameterValues* settings = NULL,
				const bool saveVal = true
			);

			/**
			 * @fn virtual ~SettingsPanel()
			 * @brief destructor
			 */
			virtual ~SettingsPanel();

		
		protected:
			/** page information object */
			kome::plugin::SettingsPage* m_page;

			/** default button flag */
			const bool m_dfltBtn;

			/** settings values */
			kome::objects::SettingParameterValues* m_settings;

			/** save value flag */
			const bool m_saveVal;

			/** new settings */
			bool m_newSettings;

			/**
			 * @struct GroupInfo
			 * @brief group information
			 */
			struct GroupInfo {
				std::string name;
				std::vector< kome::plugin::SettingsForm* > params;
				unsigned int size;
			};
	
			/**
			 * @struct ClusterInfo
			 * @brief cluster information
			 */
			struct ClusterInfo {
				std::vector< GroupInfo* > groups;
			};

			/**
			 * @struct FormInfo
			 * @brief form information
			 */
			struct FormInfo {
				kome::plugin::SettingsForm* form;
				int id;
			};

		protected:
			/** new control ID */
			unsigned int m_newId;

			/** forms */
			std::vector< FormInfo > m_forms;

			/** edited */
			bool m_edited;

			/** combobox map */
			std::map< int, std::vector< kome::plugin::SettingsValue::EnumItem > > m_comboMap;

		protected:
			/** event flag */
			volatile bool m_evtFlg;

		public:
			/**
			 * @fn void setDefaultValues( const bool thisWindow = true, const bool subWindow = true )
			 * @brief sets default values
			 * @param[in] thisWindow If true, parameters are set to default values in this window.
			 * @param[in] subWindow If true, parameters are set to default values in sub windows.
			 */
			void setDefaultValues( const bool thisWindow = true, const bool subWindow = true );

			/**
			 * @fn virtual bool Validate()
			 * @brief validates the values
			 * @return If any of the valudations failed, this method returns false.
			 */
			virtual bool Validate();
		
		public:

			/**
			 * @fn void setEdited( const bool edited )
			 * @brief sets edited flag
			 * @param[in] edited edited flag state
			 */
			void setEdited( const bool edited );

			/**
			 * @fn bool isEdited()
			 * @brief gets edited flag
			 * @return edited flag state
			 */
			bool isEdited();

		protected:
			/**
			 * @fn void createGroups( std::vector< GroupInfo >& groups )
			 * @brief creates groups array
			 * @param[out] groups the array of group object to be sotred
			 */
			void createGroups( std::vector< GroupInfo >& groups );

			/**
			 * @fn FormInfo* searchForm( const int id )
			 * @brief searches form
			 * @param[in] id form ID
			 * @return form information
			 */
			FormInfo* searchForm( const int id );

		public:
			/**
			 * @fn wxWindow* searchWindow( kome::plugin::SettingsForm* form )
			 * @brief searches window
			 * @param[in] form form information
			 * @return window
			 */
			wxWindow* searchWindow( kome::plugin::SettingsForm* form );

		protected:
			/**
			 * @fn virtual wxSizer* createMainSizer()
			 * @brief creates main sizer
			 * @return main sizer
			 */
			virtual wxSizer* createMainSizer();

		protected:
			/**
			 * @fn wxSizer* createClusterSizer( ClusterInfo& cluster )
			 * @brief creates cluster sizer
			 * @param[in] cluster cluster information
			 * @return cluster sizer
			 */
			wxSizer* createClusterSizer( ClusterInfo& cluster );

			/**
			 * @fn wxSizer* createGroupSizer( GroupInfo& group )
			 * @brief creates group sizer
			 * @param[in] group group information
			 * @return group sizer
			 */
			wxSizer* createGroupSizer( GroupInfo& group );

			/**
			 * @fn wxSizer* createParameterSizer( kome::plugin::SettingsForm* form )
			 * @brief creates parameter sizer
			 * @param[in] form form information
			 * @return parameter sizer
			 */
			wxSizer* createParameterSizer( kome::plugin::SettingsForm* form );

			/**
			 * @fn wxSizer* createComboBoxSizer( kome::plugin::SettingsForm* form, const int id )
			 * @brief creates combo box sizer
			 * @param[in] form form information
			 * @param[in] id form ID
			 * @return combo box sizer
			 */
			wxSizer* createComboBoxSizer( kome::plugin::SettingsForm* form, const int id );

			/**
			 * @fn wxSizer* createCheckBoxSizer( kome::plugin::SettingsForm* form, const int id )
			 * @brief creates check box sizer
			 * @param[in] form form information
			 * @param[in] id form ID
			 * @return check box sizer
			 */
			wxSizer* createCheckBoxSizer( kome::plugin::SettingsForm* form, const int id );

			/**
			 * @fn wxSizer* createIntValueSizer( kome::plugin::SettingsForm* form, const int id )
			 * @brief creates integer value sizer
			 * @param[in] form form information
			 * @param[in] id form ID
			 * @return integer value sizer
			 */
			wxSizer* createIntValueSizer( kome::plugin::SettingsForm* form, const int id );

			/**
			 * @fn wxSizer* createRealNumberValueSizer( kome::plugin::SettingsForm* form, const int id )
			 * @brief creates real number value sizer
			 * @param[in] form form information
			 * @param[in] id form ID
			 * @return real number value sizer
			 */
			wxSizer* createRealNumberValueSizer( kome::plugin::SettingsForm* form, const int id );

			/**
			 * @fn wxSizer* createStringValueSizer( kome::plugin::SettingsForm* form, const int id )
			 * @brief creates string value sizer
			 * @param[in] form form information
			 * @param[in] id form ID
			 * @return string value sizer
			 */
			wxSizer* createStringValueSizer( kome::plugin::SettingsForm* form, const int id );

			/**
			 * @fn wxSizer* createPasswdValueSizer( kome::plugin::SettingsForm* form, const int id )
			 * @brief creates password sizer
			 * @param[in] form form information
			 * @param[in] id form ID
			 * @return password value sizer
			 */
			wxSizer* createPasswdValueSizer( kome::plugin::SettingsForm* form, const int id );

			/**
			 * @fn wxSizer* createFileSizer( kome::plugin::SettingsForm* form, const int id )
			 * @brief creates file sizer
			 * @param[in] form form information
			 * @param[in] id form ID
			 * @return file sizer
			 */
			wxSizer* createFileSizer( kome::plugin::SettingsForm* form, const int id );

			/**
			 * @fn wxSizer* createColorSizer( kome::plugin::SettingsForm* form, const int id )
			 * @brief creates color sizer
			 * @param[in] form form information
			 * @param[in] id form ID
			 * @return color sizer
			 */
			wxSizer* createColorSizer( kome::plugin::SettingsForm* form, const int id );

			/**
			 * @fn wxSizer* createIntRangeSizer( kome::plugin::SettingsForm* form, const int id )
			 * @brief creates integer value range sizer
			 * @param[in] form form information
			 * @param[in] id form ID
			 * @return integer value range sizer
			 */
			wxSizer* createIntRangeSizer( kome::plugin::SettingsForm* form, const int id );

			/**
			 * @fn wxSizer* createDoubleRangeSizer( kome::plugin::SettingsForm* form, const int id )
			 * @brief creates real number value range sizer
			 * @param[in] form form information
			 * @param[in] id form ID
			 * @return real number value range sizer
			 */
			wxSizer* createDoubleRangeSizer( kome::plugin::SettingsForm* form, const int id );

			/**
			 * @fn wxSizer* createFontSizer( kome::plugin::SettingsForm* form, const int id )
			 * @brief creates font sizer
			 * @param[in] form form information
			 * @param[in] id form ID
			 */
			wxSizer* createFontSizer( kome::plugin::SettingsForm* form, const int id );

			/**
			 * @fn wxSizer* createListSizer(
						kome::plugin::SettingsForm* form,
						const int id,
						bool hideAllCheckbox
					)
			 * @brief creates list sizer
			 * @param[in] form form information
			 * @param[in] id form ID
			 * @param[in] hideAllCheckbox "Hide All Checkbox" flag
			 * @return list sizer
			 */
			wxSizer* createListSizer(
				kome::plugin::SettingsForm* form,
				const int id,
				bool hideAllCheckbox
			);

			/**
			 * @fn wxSizer* createDefaultButtonSizer()
			 * @brief creates default button sizer
			 * @return group sizer
			 */
			wxSizer* createDefaultButtonSizer();

			/**
			 * @fn wxSizer* creatEmailValueSizer( kome::plugin::SettingsForm* form, const int id )
			 * @brief creates e-mail value sizer
			 * @return group sizer
			 */
			wxSizer* creatEmailValueSizer( kome::plugin::SettingsForm* form, const int id );

		public:
			/**
			 * @fn void processEvent( kome::plugin::SettingsForm* form )
			 * @brief processes event
			 * @param[in] form form information
			 */
			void processEvent( kome::plugin::SettingsForm* form );

		protected:
			/**
			 * @fn bool checkValue(
						kome::plugin::SettingsForm* form,
						const char* val,
						std::string& msg
					)
			 * @brief checks value
			 * @param[in] form form information
			 * @param[in] val value
			 * @param[out] msg string object to store error message
			 * @return If true, specified value is valid
			 */
			bool checkValue(
				kome::plugin::SettingsForm* form,
				const char* val,
				std::string& msg
			);

		protected:
			/**
			 * @fn std::string getValueFromWindow( const int id )
			 * @brief gets string value from window
			 * @param[in] id form ID
			 */
			std::string getValueFromWindow( const int id );

			/**
			 * @fn bool setValueFromString(
						const int id,
						const char* str
					)
			 * @brief sets value from string
			 * @param[in] id form ID
			 * @param[in] str string value
			 * @return If true, value is successfully set.
			 */
			bool setValueFromString(
				const int id,
				const char* str
			);

		protected:
			/**
			 * @fn void onSection( const unsigned int id )
			 * @brief This method is called when a section is updated
			 * @param id form ID
			 */
			void onSection( const unsigned int id );

			/**
			 * @fn void updateEnumWindow( unsigned int id, const char* val )
			 * @brief updates enumeration window
			 * @param[in] id form ID
			 * @param[in] val parent value
			 */
			void updateEnumWindow( unsigned int id, const char* val );

		protected:
			/**
			 * @fn void onSpin( wxEvent& evt )
			 * @brief This method is called when the spin button is clicked.
			 * @param[in] evt event information
			 */
			void onSpin( wxEvent& evt );

			/**
			 * @fn void onText( wxEvent& evt )
			 * @brief This method is called when the text control value is updated.
			 * @param[in] evt event information
			 */
			void onText( wxEvent& evt );

			/**
			 * @fn void onCombo( wxEvent& evt )
			 * @brief This method is called when a combobox is changed.
			 * @param[in] evt event information
			 */
			void onCombo( wxEvent& evt );

			/**
			 * @fn void onParamButton( wxEvent& evt )
			 * @brief This method is called when the parameters button is clicked.
			 * @param[in] evt evnet information
			 */
			void onParamButton( wxEvent& evt );

			/**
			 * @fn void onDefaultButton( wxCommandEvent& evt )
			 * @brief This button is called when the default button is clicked.
			 * @param[in] evt event information
			 */
			void onDefaultButton( wxCommandEvent& evt );

		public:
			/**
			 * @fn virtual bool TransferDataToWindow()
			 * @brief transfer value to window control data
			 * @return If a transfer failed, this method returns false.
			 */
			virtual bool TransferDataToWindow();
		
			/**
			 * @fn virtual bool TransferDataFromWindow()
			 * @brief transfer window control data to value
			 * @return If a transfer failed, this method returns false.
			 */
			virtual bool TransferDataFromWindow();

		protected:
			/**
			 * @fn virtual std::string getDescription( wxWindow* wnd )
			 * @brief method to get the description from a prameter control
			 * @param[in] wnd parameter control
			 * @return description
			 */
			virtual std::string getDescription( wxWindow* wnd );

			/**
			 * @fn virtual std::string getImageFilePath( wxWindow* wnd )
			 * @brief method to get image file path from a parameter control
			 * @param[in] wnd parameter control
			 * @return image file path
			 */
			virtual std::string getImageFilePath( wxWindow* wnd );

		protected:
			/**
			 * @fn static void createClusters( std::vector< ClusterInfo >& clusters, std::vector< GroupInfo >& groups )
			 * @brief creates clusters array
			 * @param[out] clusters the array of cluster object to be stored
			 * @param[in] groups groups information to create clusters
			 */
			static void createClusters(
				std::vector< ClusterInfo >& clusters,
				std::vector< GroupInfo >& groups
			);

			/**
			 * @fn static void createClusters(
					std::vector< ClusterInfo >& clusters,
					std::vector< GroupInfo >& groups,
					double* var,
					std::list< int >& indexList,
					const unsigned int maxSize,
					int num
				)
			 * @brief create clusters array
			 * @param[out] clusters the array of cluster object to be stored
			 * @param[in] groups groups information to create clusters
			 * @param[in] var the pointer to variance
			 * @param[in] indexList index list
			 * @param[in] maxSize max size
			 * @param[in] num the number of rest clusters
			 */
			static void createClusters(
				std::vector< ClusterInfo >& clusters,
				std::vector< GroupInfo >& groups,
				double* var,
				std::list< int >& indexList,
				const unsigned int maxSize,
				int num
			);

			/**
			 * @fn static unsigned int getNumberOfRows( const unsigned int num )
			 * @brief gets the number of cluster rows
			 * @param[in] num the number of clusters
			 * @return the number of cluster rows
			 */
			static unsigned int getNumberOfRows( const unsigned int num );

		private:
			DECLARE_EVENT_TABLE()
		};



		/**
		 * @class SettingsPanelEvent
		 * @brief settings panel event
		 */
		class WINDOW_MANAGER_CLASS SettingsPanelEvent : public wxCommandEvent {
		public:
			/**
			 * @fn SettingsPanelEvent( const int id, SettingsPanel* panel, kome::plugin::SettingsForm* form )
			 * @brief constructor
			 * @param[in] id control ID
			 * @param[in] panel panel object
			 * @param[in] form form information
			 */
			SettingsPanelEvent( const int id, SettingsPanel* panel, kome::plugin::SettingsForm* form );

			/**
			 * @fn virtual ~SettingsPanelEvent()
			 * @brief destructor
			 */
			virtual ~SettingsPanelEvent();

		protected:
			/** panel */
			SettingsPanel* m_panel;

			/** form */
			kome::plugin::SettingsForm* m_form;

		public:
			/**
			 * @fn SettingsPanel* getPanel()
			 * @brief gets the panel
			 * @return panel
			 */
			SettingsPanel* getPanel();

			/**
			 * @fn kome::plugin::SettingsForm* getForm()
			 * @brief gets the form
			 * @return form information
			 */
			kome::plugin::SettingsForm* getForm();
		};

	}
}

// event
BEGIN_DECLARE_EVENT_TYPES()
	DECLARE_EXPORTED_EVENT_TYPE( WINDOW_MANAGER_CLASS, EVT_COMMAND_SETTINGS_PANEL, 2011 )
END_DECLARE_EVENT_TYPES()

typedef void (wxEvtHandler::*SettingsPanelEventFunction)( kome::window::SettingsPanelEvent& );

#define SettingsPanelEventHandler( func ) \
    (wxObjectEventFunction)(wxEventFunction)wxStaticCastEvent( SettingsPanelEventFunction, &func )

#define EVT_KOME_SETTINGS_PANEL( id, fn ) \
    wx__DECLARE_EVT1( EVT_COMMAND_SETTINGS_PANEL, id, SettingsPanelEventHandler( fn ) )



#endif		// __KOME_WINDOW_SETTINGS_PANEL_H__

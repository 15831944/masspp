/**
 * @file HelpManager.h
 * @brief interfaces of HelpManager class
 *
 * @author S.Tanaka
 * @date 2007.03.16
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 **/


#ifndef __KOME_VIEW_HELP_MANAGER_H__
#define __KOME_VIEW_HELP_MANAGER_H__


#include "HelpTypedef.h"

#include <wx/wx.h>

class wxHtmlHelpController;


namespace kome {
	namespace view {

		/**
		 * @class HelpManager
		 * @brief help plug-in management class
		 */
		class HelpManager {
		protected:
			/**
			 * @fn HelpManager()
			 * @brief constructor
			 */
			HelpManager();

			/**
			 * @fn virtual ~HelpManager()
			 * @brief destructor
			 */
			virtual ~HelpManager();

		protected:
			/** help controller */
			wxHtmlHelpController* m_controller;

			/** Path to the various plug-ins */
			std::vector< std::string > m_vctHtmlFullPath;
			/** HLP files in the HTML path */
			std::vector< std::string > m_vctHtmlPath;
			/** The HHP File Path */
			std::string m_strHhpFilePath;
			/** The HHK File Path */
			std::string m_strHhkFilePath;
			/** The HHC File Path */
			std::string m_strHhcFilePath;
			/** The Default File Path */
			std::string m_strDefaultFilePath;

			/** HHC file information for each plug-in objects */
			std::vector< ST_HELP_OBJECT > m_HelpChapterInfo;
			/** HHK file information for each plug-in objects */
			std::vector< ST_HELP_KEY_OBJECT > m_HelpKeywordInfo;

		public:
			/**
			 * @fn bool showHelpWindow()
			 * @brief shows help window
			 * @return If false, it failed to create help information.
			 */
			bool showHelpWindow();

		protected:
			/**
			 * @fn void createHelp()
			 * @brief creates help view
			 */
			void createHelp();

			/**
			 * @fn bool getFromHppFileToHTMLPath( const char* szHppFileName, int nIdx, bool bOrderInfo )
			 * @brief HPP file to get the HTML from the specified path.
			 * @param szHppFileName read HHP file name
			 * @param nIdx plugin index
			 * @param bOrderInfo Having order information or not
			 * @return Returns FALSE on failure.Processing returns TRUE if successful.
			 */
			bool getFromHppFileToHTMLPath( const char* szHppFileName, int nIdx, bool bOrderInfo );

			/**
			 * @fn bool createTemporaryHppFile( const char* hppfilename )
			 * @brief HHP file to create a temporary.
			 * @return Returns FALSE on failure.Processing returns TRUE if successful.
			 */
			bool createTemporaryHppFile( void );

			/**
			 * @fn bool createTemporaryHhkFile( void )
			 * @brief HHK file to create a temporary.
			 * @return Returns FALSE on failure.Processing returns TRUE if successful.
			 */
			bool createTemporaryHhkFile( void );

			/**
			 * @fn bool createTemporaryHhcFile( void )
			 * @brief HHC file to create a temporary. 
			 * @return Returns FALSE on failure.Processing returns TRUE if successful.
			 */
			bool createTemporaryHhcFile( void );

			/**
			 * @fn void createULTag( int nId, std::ofstream& ofHppFile)
			 * @brief Recursively writes the contents of the UL tag.
			 * @param nId ID of HHC file object structures
			 * @param ofHppFile std::ofstream
			 * @return none
			 */
			void createULTag( int nId, std::ofstream& ofHppFile);

			/**
			 * @fn std::string getDotCutName( const char* szMenu )
			 * @brief It is deleted '...' from the end of the character string.
			 * @param szMenu original menu name
			 * @return update menu name
			 */
			std::string getDotCutName( const char* szMenu );

			/**
			 * @fn void sortHelpChapterByMenu( int nChildId, kome::plugin::PluginMenu* pplgMenu )
			 * @brief Sorts less than the second hierarchy.
			 * @param nChildId ID of HHC file object structures
			 * @param pplgMenu Pointer of PluginMenu class
			 */
			void sortHelpChapterByMenu( int nChildId, kome::plugin::PluginMenu* pplgMenu );

		protected:
			/**
			 * @fn static bool lessHelp( kome::plugin::PluginHelp* help0, kome::plugin::PluginHelp* help1 )
			 * @brief compares to sort the array of helps
			 * @param help0 help object to be compared
			 * @param help1 help object to compare
			 * @return If true, help0 sorts before help1.
			 */
			static bool lessHelp( kome::plugin::PluginHelp* help0, kome::plugin::PluginHelp* help1 );

		public:
			/**
			 * @fn static HelpManager& getInstance()
			 * @brief gets log view plug-in management object
			 * @return log view plug-in management object (This is the only object.)
			 */
			static HelpManager& getInstance();
		};
	}
}

#endif		// __KOME_VIEW_HELP_MANAGER_H__

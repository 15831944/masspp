/**
 * @file HelpReadHhc.h
 * @brief read HHC file class
 * @author Y.Murata
 * @date 2011.12.21
 * Copyright (C) 2014 Shimadzu Corporation All rights reserved.
 */

#ifndef __HELP_READ_HHC_H__
#define __HELP_READ_HHC_H__

#include "HelpTypedef.h"

namespace kome {
	namespace view {

		/**
		 * @class HelpReadHhc
		 * @brief read HHC file class
		 */
		class HelpReadHhc {
		public:
			/**
			 * @fn HelpReadHhc( std::vector< ST_HELP_OBJECT >* pvctHelp = NULL )
			 * @param pvctHelp HHC file information for each plug-in objects
			 * @brief constructor
			 */
			HelpReadHhc( std::vector< ST_HELP_OBJECT >* pvctHelp = NULL );

			/**
			 * @fn virtual ~ScoreCalculation()
			 * @brief destructor
			 */
			virtual ~HelpReadHhc();

			/**
			 * @fn bool createChapterInfo( const char* szHhkFileName, const char* szHtmlFullPath )
			 * @brief Create an HHC file from the specified chapters.
			 * @param szHhcFileName read HHC file name
			 * @param szHtmlFullPath full path to add
			 * @return Returns FALSE on failure.Processing returns TRUE if successful.
			 */
			bool createChapterInfo( const char* szHhcFileName, const char* szHtmlFullPath );

		// member
		private:
			/** ID for each plug-in */
			int					m_nId;
			/** Parent ID to the various plug-ins */
			std::vector< int >	m_vctParentId;
			/** HHK file information for each plug-in objects */
			std::vector< ST_HELP_OBJECT >*	m_pvctHelp;
		};
	}
}

#endif // __HELP_READ_HHC_H__

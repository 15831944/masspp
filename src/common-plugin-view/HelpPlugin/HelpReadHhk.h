/**
 * @file HelpReadHhk.h
 * @brief read HHK file class
 * @author Y.Murata
 * @date 2011.12.21
 * Copyright (C) 2014 Shimadzu Corporation All rights reserved.
 */

#ifndef __HELP_READ_HHK_H__
#define __HELP_READ_HHK_H__

#include "HelpTypedef.h"

namespace kome {
	namespace view {

		/**
		 * @class HelpReadHhk
		 * @brief read HHK file class
		 */
		class HelpReadHhk {
		public:
			/**
			 * @fn HelpReadHhk( std::vector< ST_HELP_KEY_OBJECT >* pvctHelpKey = NULL )
			 * @param pvctHelpKey HHK file information for each plug-in objects
			 * @brief constructor
			 */
			HelpReadHhk( std::vector< ST_HELP_KEY_OBJECT >* pvctHelpKey = NULL );

			/**
			 * @fn virtual ~ScoreCalculation()
			 * @brief destructor
			 */
			virtual ~HelpReadHhk();

			/**
			 * @fn bool createKeywordInfo( const char* szHhkFileName, const char* szHtmlFullPath )
			 * @brief Create an HHK file from the specified keyword.
			 * @param szHhkFileName read HHK file name
			 * @param szHtmlFullPath full path to add
			 * @return Returns FALSE on failure.
			 */
			bool createKeywordInfo( const char* szHhkFileName, const char* szHtmlFullPath );

		// member
		private:
			/** HHK file information for each plug-in objects */
			std::vector< ST_HELP_KEY_OBJECT >*	m_pvctHelpKey;
		};
	}
}

#endif // __HELP_READ_HHK_H__

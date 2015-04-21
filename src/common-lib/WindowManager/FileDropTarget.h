/**
 * @file FileDropTarget.h
 * @brief interfaces of FileDropTarget class
 *
 * @author S.Tanaka
 * @date 2013.01.10
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation All rights reserved.
 */


#ifndef __KOME_WINDOW_FILE_DROP_TARGET_H__
#define __KOME_WINDOW_FILE_DROP_TARGET_H__


#include <wx/dnd.h>


namespace kome {
	namespace window {

		/**
		 * @class FileDropTarget
		 * @brief This class has some functions for list
		 */
		class WINDOW_MANAGER_CLASS FileDropTarget : public wxFileDropTarget {
		public:
			/**
			 * @fn FileDropTarget()
			 * @brief constructor
			 */
			FileDropTarget();

			/**
			 * @fn virtual ~FileDropTarget()
			 * @brief destructor
			 */
			virtual ~FileDropTarget();

		public:
			/**
			 * @fn virtual bool OnDropFiles( wxCoord x, wxCoord y, const wxArrayString& filenames )
			 * @brief This method is called when receiving dropped files.
			 * @param[in] x The x coordinate of the mouse
			 * @param[in] y The y coordinate of the mouse
			 * @param[in] filenames An array of filenames.
			 * @return true to accept the data of false to veto the operation.
			 */
			virtual bool OnDropFiles( wxCoord x, wxCoord y, const wxArrayString& filenames );
		};
	}
}


#endif		// __KOME_WINDOW_FILE_DROP_TARGET_H__

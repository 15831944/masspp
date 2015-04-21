/**
 * @file DisplayProgressWrapper.h
 * @brief interfaces of DisplayProgressWrapper class
 *
 * @author S.Tanaka
 * @date 2013.10.24
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation
 */


#ifndef __KOME_CLR_DISPLAY_PROGRESS_WRAPPER_H__
#define __KOME_CLR_DISPLAY_PROGRESS_WRAPPER_H__


#include "ProgressWrapper.h"


namespace kome {
	namespace clr {

		/**
		 * @class DisplayProgressWrapper
		 * @brief progress wrapper class to use on CLR environment
		 */
		public ref class DisplayProgressWrapper : ProgressWrapper{
		public:
			/**
			 * @fn DisplayProgressWrapper( System::String^ title )
			 * @brief constructor
			 * @param[in] title title
			 */
			DisplayProgressWrapper( System::String^ title );

			/**
			 * @fn virtual ~DisplayProgressWrapper()
			 * @brief destructor
			 */
			virtual ~DisplayProgressWrapper();
		};
	}
}

#endif	// __KOME_CLR_DISPLAY_PROGRESS_WRAPPER_H__

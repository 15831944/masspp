/**
 * @file LcmsInputManager.h
 * @brief interfaces of LcmsInputManager class
 *
 * @author S.Tanaka
 * @date 2008.02.08
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_IO_LCMS_INPUT_MANAGER_H__
#define __KOME_IO_LCMS_INPUT_MANAGER_H__


namespace kome {
	namespace io {
		namespace lcms {

			/**
			 * @class LcmsInputManager
			 * @brief Lcms data input management class
			 */
			public ref class LcmsInputManager {
			public:
				/**
				 * @fn static kome::clr::ClrVariant open( kome::clr::ClrParameters^ params )
				 * @brief open Lcms data file
				 * @param params plug-in call parameter information
				 * @return return value of plug-in function
				 */
				static kome::clr::ClrVariant^ open( kome::clr::ClrParameters^ params );
			};
		}
	}
}

#endif	// __KOME_IO_LCMS_INPUT_MANAGER_H__

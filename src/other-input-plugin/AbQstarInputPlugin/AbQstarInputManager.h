/**
 * @file AbQstarInputManager.h
 * @brief interfaces of AbQstarInputManager class
 *
 * @author S.Tanaka
 * @date 2007.04.16
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#ifndef __KOME_IO_AB_QSTAR_INPUT_MANAGER_H__
#define __KOME_IO_AB_QSTAR_INPUT_MANAGER_H__

namespace kome {
	namespace io {
		namespace qstar {

			/** 
			 * @class AbQstarInputManager
			 * @brief Applied Biosystems raw data input manager class
			 */
			public ref class AbQstarInputManager {
			public:
				/**
				 * @fn static kome::clr::ClrVariant open( kome::clr::ClrParameters^ params )
				 * @brief opens Applied Biosystems raw data file
				 * @param params plug-in call parameter information
				 * @return return value of plug-in function
				 */
				static kome::clr::ClrVariant^ open( kome::clr::ClrParameters^ params );
			};
		}
	}
}



#endif	// __KOME_IO_AB_QSTAR_INPUT_MANAGER_H__

/**
 * @file TppSearchGrid.h
 * @brief interfaces of TppSearchGrid class
 *
 * @author N.Tashiro
 * @date 2013.03.15
 * 
 * Copyright(C) 2013 Shimadzu. All rights reserved.
 */


#ifndef __KOME_TPP_IDENTIFICATION_MANAGER_H__
#define __KOME_TPP_IDENTIFICATION_MANAGER_H__

namespace kome {
	namespace Tpp{
		class TppIdentificationManager :
			public kome::ident::IdentificationManager {

		public:
			TppIdentificationManager();

			virtual IdentificationManager getInstance();

			virtual ~TppIdentificationManager();
		};
	}
}

#endif // __KOME_TPP_IDENTIFICATION_MANAGER_H__

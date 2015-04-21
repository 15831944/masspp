/**
 * @file CommonTypes.h
 * @brief common types definition
 *
 * @author S.Tanaka
 * @date 2011.06.16
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation All rights reserved.
 */


#ifndef __KOME_CORE__TYPES_H__
#define __KOME_CORE__TYPES_H__


namespace kome {
    namespace core {
        namespace key {
			/** key code */
			enum {
				KEY_BACKSPACE = 0x0100,
				KEY_TAB	   = 0x0101,
				KEY_RETURN	= 0x0102,
				KEY_SPACE	 = 0x0103,
				KEY_DELETE	= 0x0104,
				KEY_SHIFT	 = 0x0105,
				KEY_ALT	   = 0x0106,
				KEY_CONTROL   = 0x0107,
				KEY_PAUSE	 = 0x0108,
				KEY_END	   = 0x0109,
				KEY_HOME	  = 0x010A,
				KEY_LEFT	  = 0x010B,
				KEY_UP		= 0x010C,
				KEY_RIGHT	 = 0x010D,
				KEY_DOWN	  = 0x010E,
				KEY_INSERT	= 0x010F,
				KEY_F1		= 0x0110,
				KEY_F2		= 0x0111,
				KEY_F3		= 0x0112,
				KEY_F4		= 0x0113,
				KEY_F5		= 0x0114,
				KEY_F6		= 0x0115,
				KEY_F7		= 0x0116,
				KEY_F8		= 0x0117,
				KEY_F9		= 0x0118,
				KEY_F10	   = 0x0119,
				KEY_F11	   = 0x011A,
				KEY_F12	   = 0x011B,
				KEY_F13	   = 0x011C,
				KEY_F14	   = 0x011D,
				KEY_F15	   = 0x011E,
				KEY_F16	   = 0x011F,
				KEY_NUMLOCK   = 0x0120,
				KEY_SCROLL	= 0x0121,
				KEY_PAGEUP	= 0x0122,
				KEY_PAGEDOWN  = 0x0123,
                KEY_ESCAPE    = 0x0124
			};
		}
	}
}


#endif    // __KOME_CORE__TYPES_H__

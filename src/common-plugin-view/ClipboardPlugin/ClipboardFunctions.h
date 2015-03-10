/**
 * @file ClipboardFunctions.h
 * @brief interfaces of Clipboard plug-in functions
 *
 * @author S.Tanaka
 * @date 2007.05.23
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_CLIPBOARD_FUNCTIONS_H__
#define __KOME_CLIPBOARD_FUNCTIONS_H__


#ifdef __unix
	extern "C" {
#endif  // __unix


/**
 * @fn kome::objects::Variant copyTextData( kome::objects::Parameters* params )
 * @brief copies text data
 * @param params parameters object
 * @return If true, it succeeded to copy text data to clipboard. (boolean)
 */
kome::objects::Variant copyTextData( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant copyBitmapData( kome::objects::Parameters* )
 * @brief copies bitmap data
 * @param params parameters object
 * @return If true, it succeeded to copy bitmap data to clipboard. (boolean)
 */
kome::objects::Variant copyBitmapData( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant copyMetafileData( kome::objects::Parameters* params )
 * @brief copies metafile data
 * @param params parameters object
 * @return If true, it succeeded to copy metafile data to clipboard. (boolean)
 */
kome::objects::Variant copyMetafileData( kome::objects::Parameters* params );

// @date 2011.08.23 <Add> M.Izumi ->
/**
 * @fn kome::objects::Variant isVisibleBitmap( kome::objects::Parameters* )
 * @brief enable/disable check for Bitmap menu 
 * @param params parameters object
 * @return return value (true:enable / false:disable)
 */
kome::objects::Variant isVisibleBitmap( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant isVisibleMetafile( kome::objects::Parameters* );
 * @brief enable/disable check for Metafile menu 
 * @param params parameters object
 * @return return value (true:enable / false:disable)
 */
kome::objects::Variant isVisibleMetafile( kome::objects::Parameters* params );
// @date 2011.08.23 <Add> M.Izumi <-

#ifdef __unix
	}
#endif  // __unix



#endif		// __KOME_CLIPBOARD_FUNCTIONS_H__

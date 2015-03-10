/**
 * @file MzXmlIOPluginFunctions.h
 * @brief interfaces of MzXmlIO plug-in function
 *
 * @author S.Tanaka
 * @date 2007.03.27
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_IO_MZXML_IO_PLUGIN_FUNCTIONS_H__
#define __KOME_IO_MZXML_IO_PLUGIN_FUNCTIONS_H__


#ifdef __unix
	extern "C" {
#endif  // __unix


/**
 * @fn kome::objects::Variant openMzXml( kome::objects::Parameters* params )
 * @brief open mzxml file
 * @param params parameters object. (This object has path information.)
 * @return return value. (Sample Object)
 */
kome::objects::Variant openMzXml( kome::objects::Parameters* params );

/**
 * @fn kome::objects::Variant saveMzXml( kome::objects::Parameters* params )
 * @brief save mzxml file
 * @param params parameters object. (This object has spectrum group and path information.)
 * @return return value. (If it succeeded to save the file, this function returns ture. [bool])
 */
kome::objects::Variant saveMzXml( kome::objects::Parameters* params );

#ifdef __unix
	}
#endif  // __unix



#endif// __KOME_IO_MZXML_IO_PLUGIN_FUNCTIONS_H__

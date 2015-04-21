/**
 * @file Xml.h
 * @brief interfaces of Xml class
 *
 * @author S.Tanaka
 * @date 2007.08.21
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_XML_H__
#define __KOME_XML_H__


#include <string>


namespace kome {
	namespace xml {

		class XML_TOOLS_CLASS Xml {
		public:
			/**
			 * @fn static std::string text2xml( const char* s )
			 * @brief converts character string to XML entity
			 * @param[in] s character string
			 * @return XML entity string
			 */
			static std::string text2xml( const char* s );
		};
	}
}


#endif		// __KOME_XML_H__

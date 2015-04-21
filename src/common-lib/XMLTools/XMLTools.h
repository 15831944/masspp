/**
 * @file XMLTools.h
 * @brief header file to use XMLTools library
 *
 * @author S.Tanaka
 * @date 2006.08.11
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_XML_TOOLS_H__
#define __KOME_XML_TOOLS_H__


//
// macros
// 

#ifdef _MSC_VER
	#define XML_TOOLS_CLASS __declspec(dllimport)
#else
	#define XML_TOOLS_CLASS
#endif	// _MSC_VER


//
// includes
//

#include "XMLTools/Xml.h"
#include "XMLTools/XercesTool.h"
#include "XMLTools/XmlHandler.h"


#endif //  __KOME_XML_TOOLS_H__

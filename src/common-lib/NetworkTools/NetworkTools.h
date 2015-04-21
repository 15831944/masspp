/**
 * @file DbTools.h
 * @brief header file to use DbTools library
 *
 * @author S.Tanaka
 * @date 2009.07.14
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_NETWORK_TOOLS_H__
#define __KOME_NETWORK_TOOLS_H__


//
// macros
// 

#ifdef _MSC_VER
	#define NETWORK_TOOLS_CLASS __declspec(dllimport)
#else
	#define NETWORK_TOOLS_CLASS
#endif	// _MSC_VER


//
// includes
//

#include "NetworkTools/Connection.h"
#include "NetworkTools/BasicConnection.h"
#include "NetworkTools/HttpConnection.h"
#include "NetworkTools/ProxyHttpConnection.h"
#include "NetworkTools/NetworkManager.h"
#include "NetworkTools/Wsdl.h"
#include "NetworkTools/WsdlPortType.h"
#include "NetworkTools/WsdlOperation.h"
#include "NetworkTools/WsdlBinding.h"
#include "NetworkTools/WsdlHandler.h"
#include "NetworkTools/WinHttpConnection.h"


#endif //  __KOME_NETWORK_TOOLS_H__

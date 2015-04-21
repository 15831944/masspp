/**
 * @file EventManager.h
 * @brief header file to use EventManager library
 *
 * @author S.Tanaka
 * @date 2007.08.31
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_EVENT_MANAGER_H__
#define __KOME_EVENT_MANAGER_H__


//
// macros
// 

#ifdef _MSC_VER
	#define EVENT_MANAGER_CLASS __declspec(dllimport)
#else
	#define EVENT_MANAGER_CLASS
#endif	// _MSC_VER

//
// includes
//

#include "EventManager/MouseEvent.h"
#include "EventManager/KeyEvent.h"
#include "EventManager/SizeEvent.h"
#include "EventManager/MoveEvent.h"


#endif	// __KOME_EVENT_MANAGER_H__

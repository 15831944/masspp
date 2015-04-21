/**
 * @file CoreToolkits.h
 * @brief header file to use CoreToolkit library
 *
 * @author S.Tanaka
 * @date 2006.07.24
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_CORE_TOOLKITS_H__
#define __KOME_CORE_TOOLKITS_H__

//
// macros
// 

#ifdef _MSC_VER
	#define CORE_TOOLKITS_CLASS __declspec(dllimport)
#else
	#define CORE_TOOLKITS_CLASS
#endif	// _MSC_VER


//
// includes
//

#include "CoreToolkits/TreeNode.h"
#include "CoreToolkits/Heap.h"
#include "CoreToolkits/Properties.h"
#include "CoreToolkits/IniFile.h"
#include "CoreToolkits/ConversionTool.h"
#include "CoreToolkits/Rect.h"
#include "CoreToolkits/Point.h"
#include "CoreToolkits/PointArray.h"
#include "CoreToolkits/RangeList.h"
#include "CoreToolkits/NumberRestriction.h"
#include "CoreToolkits/Vector.h"
#include "CoreToolkits/Matrix.h"
#include "CoreToolkits/Progress.h"
#include "CoreToolkits/DisplayProgress.h"
#include "CoreToolkits/XYData.h"
#include "CoreToolkits/DataPoints.h"
#include "CoreToolkits/SearchTool.h"
#include "CoreToolkits/Base64.h"
#include "CoreToolkits/Sha1.h"
#include "CoreToolkits/Selector.h"
#include "CoreToolkits/ManualSelector.h"
#include "CoreToolkits/RangeListSelector.h"
#include "CoreToolkits/Timer.h"
#include "CoreToolkits/DataAccessor.h"
#include "CoreToolkits/MemoryAccessor.h"
#include "CoreToolkits/FileAccessor.h"
#include "CoreToolkits/Buffer.h"
#include "CoreToolkits/ResourceBundle.h"
#include "CoreToolkits/MsppManager.h"
#include "CoreToolkits/CommonTypes.h"
#include "CoreToolkits/CommonParameterManager.h"

#endif	//  __KOME_CORE_TOOLKITS_H__

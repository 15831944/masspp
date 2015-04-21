/**
 * @file DataOperation.h
 * @brief header file to use DataOperation library
 *
 * @author S.Tanaka
 * @date 2007.08.23
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_DATA_OPERATION_H__
#define __KOME_DATA_OPERATION_H__


//
// macros
// 

#ifdef _MSC_VER
	#define DATA_OPERATION_CLASS __declspec(dllimport)
#else
	#define DATA_OPERATION_CLASS
#endif	// _MSC_VER


//
// includes
//

#include "DataOperation/Alignment.h"
#include "DataOperation/BaselineSubtract.h"
#include "DataOperation/Filter.h"
#include "DataOperation/AlignedChromatogram.h"
#include "DataOperation/TransformedSpectrum.h"
#include "DataOperation/TransformedChromatogram.h"
#include "DataOperation/Operation.h"
#include "DataOperation/OperationManager.h"
#include "DataOperation/Normalization.h"
#include "DataOperation/ManipulatedSample.h"
#include "DataOperation/ManipulatedSpectrum.h"
#include "DataOperation/BatchAnalysis.h"


#endif	// __KOME_DATA_OPERATION_H__

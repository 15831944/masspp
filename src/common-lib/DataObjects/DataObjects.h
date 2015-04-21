/**
 * @file DataObjects.h
 * @brief header file to use DataObjects library
 *
 * @author S.Tanaka
 * @date 2006.07.20
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_OBJECTS_DATA_OBJECTS_H__
#define __KOME_OBJECTS_DATA_OBJECTS_H__


//
// macros
//

#ifdef _MSC_VER
	#define DATA_OBJECTS_CLASS __declspec(dllimport)
#else
	#define DATA_OBJECTS_CLASS
#endif	// _MSC_VER


//
// includes
//

#include "DataObjects/DataObjectsCommon.h"
#include "DataObjects/Variant.h"
#include "DataObjects/MsDataVariant.h"
#include "DataObjects/Parameters.h"
#include "DataObjects/SettingParameterValues.h"
#include "DataObjects/SampleSet.h"
#include "DataObjects/Sample.h"
#include "DataObjects/Spectrum.h"
#include "DataObjects/DataSet.h"
#include "DataObjects/DataGroupNode.h"
#include "DataObjects/Peaks.h"
#include "DataObjects/PeakElement.h"
#include "DataObjects/Chromatogram.h"
#include "DataObjects/DataManager.h"
#include "DataObjects/DefaultDataManager.h"
#include "DataObjects/ActiveObjectsManager.h"
#include "DataObjects/PeaksManager.h"
#include "DataObjects/PointsManager.h"
#include "DataObjects/StatusManager.h"
#include "DataObjects/AveragedSpectrum.h"
#include "DataObjects/SpectraChromatogram.h"
#include "DataObjects/OverlappingSpectrum.h"
#include "DataObjects/OverlappingChromatogram.h"
#include "DataObjects/XYDataOperation.h"
#include "DataObjects/DataMapInfo.h"
#include "DataObjects/Peak2DElement.h"
#include "DataObjects/Peaks2DArray.h"
#include "DataObjects/Peaks2D.h"
#include "DataObjects/PeaksCluster2D.h"
#include "DataObjects/DataMapManager.h"
#include "DataObjects/SampleReader.h"	// 2012/07/19 <Add> OKADA
#include "DataObjects/Script.h"			// @Date:2013/07/24	<Add>	A.Ozaki

#endif	//	__KOME_OBJECTS_DATA_OBJECTS_H__

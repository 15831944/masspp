/**
 * @file IdentificationTools.h
 * @brief header file to use IdentificationTools library
 *
 * @author H.Parry
 * @date 2012.01.25
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation. All rights reserved.
 */


#ifndef __KOME_IDENTIFICATION_TOOLS_H__
#define __KOME_IDENTIFICATION_TOOLS_H__


//
// macros
// 

#ifdef _MSC_VER
	#define IDENT_TOOLS_CLASS __declspec(dllimport)
#else
	#define IDENT_TOOLS_CLASS
#endif	// _MSC_VER


//
// includes
//

#include "IdentificationTools/Substance.h"
#include "IdentificationTools/Hit.h"
#include "IdentificationTools/SearchResult.h"
#include "IdentificationTools/SearchEngine.h"
#include "IdentificationTools/Enzyme.h"
#include "IdentificationTools/Modification.h"
#include "IdentificationTools/DataSpectrum.h"
#include "IdentificationTools/DataChromatogram.h"
#include "IdentificationTools/IdentificationManager.h"
#include "IdentificationTools/MgfManager.h"
#include "IdentificationTools/ProteinSubstance.h"
#include "IdentificationTools/PeptideSubstance.h"
#include "IdentificationTools/SampleGetter.h"
#include "IdentificationTools/ExternalDB.h"
#include "IdentificationTools/PeakPositions.h"
#include "IdentificationTools/PeakPositionsDetector.h"
#include "IdentificationTools/PeakValuesCalculator.h"


#endif //  __KOME_IDENTIFICATION_TOOLS_H__

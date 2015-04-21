/**
 * @file NumericalTools.h
 * @brief header file to use NumericalTools library
 *
 * @author S.Tanaka
 * @date 2006.07.14
 * 
 * Copyright(C) 2006-2013 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_NUMERICAL_TOOLS_H__
#define __KOME_NUMERICAL_TOOLS_H__


//
// macros
// 

#ifdef _MSC_VER
	#define NUMERICAL_TOOLS_CLASS __declspec(dllimport)
#else
	#define NUMERICAL_TOOLS_CLASS
#endif	// _MSC_VER


//
// includes
//

#include "NumericalTools/Math.h"
#include "NumericalTools/MatrixTool.h"
#include "NumericalTools/MatrixEquationSolution.h"
#include "NumericalTools/MovingAverage.h"
#include "NumericalTools/Transformation3D.h"
#include "NumericalTools/Statistics.h"
#include "NumericalTools/Interpolation.h"
#include "NumericalTools/Differentiation.h"
#include "NumericalTools/Integration.h"
#include "NumericalTools/LeastSquares.h"
#include "NumericalTools/DynamicPrograming.h"
#include "NumericalTools/MinimaxSolution.h"
#include "NumericalTools/TDistribution.h"
#include "NumericalTools/FDistribution.h"
#include "NumericalTools/NormalDistribution.h"
#include "NumericalTools/SnCalculator.h"


#endif	// __KOME_NUMERICAL_TOOLS_H__

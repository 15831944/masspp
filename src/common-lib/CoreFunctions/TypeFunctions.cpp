/**
 * @file TypeFunctions.cpp
 * @brief implements of type of variables function
 *
 * @author S.Tanaka
 * @date 2007.02.21
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "TypeFunctions.h"


#include <math.h>


// get the closest integer
int roundnum( const double v ) {
	return (int)floor( v + 0.5 );
}

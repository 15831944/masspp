/**
 * @file Math.cpp
 * @brief implements of Math class
 *
 * @author S.Tanaka
 * @date 2007.01.18
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#include "stdafx.h"
#include "Math.h"

#include <math.h>


using namespace kome::numeric;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// pi
const double Math::PI = 3.1415926535897932384626433832795028841971693993751058209749445923078164;

// napierian logarithm
const double Math::E  = 2.7182818284590452353602874713526624977572470936999595749669676277240766;

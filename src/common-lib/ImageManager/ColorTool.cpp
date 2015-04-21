/**
 * @file ColorTool.cpp
 * @brief implements of ColorTool class
 *
 * @author S.Tanaka
 * @date 2006.07.13
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"

#include "ColorTool.h"

#include <string>

using namespace kome::img;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// class members
const COLOR ColorTool::BLACK   = getColor( 0x00, 0x00, 0x00 );
const COLOR ColorTool::SILVER  = getColor( 0xc0, 0xc0, 0xc0 );
const COLOR ColorTool::GRAY	= getColor( 0x80, 0x80, 0x80 );
const COLOR ColorTool::WHITE   = getColor( 0xff, 0xff, 0xff );
const COLOR ColorTool::MAROON  = getColor( 0x80, 0x00, 0x00 );
const COLOR ColorTool::RED	 = getColor( 0xff, 0x00, 0x00 );
const COLOR ColorTool::PURPLE  = getColor( 0x80, 0x00, 0x80 );
const COLOR ColorTool::FUCHSIA = getColor( 0xff, 0x00, 0xff );
const COLOR ColorTool::GREEN   = getColor( 0x00, 0x80, 0x00 );
const COLOR ColorTool::LIME	= getColor( 0x00, 0xff, 0x00 );
const COLOR ColorTool::OLIVE   = getColor( 0x80, 0x80, 0x00 );
const COLOR ColorTool::YELLOW  = getColor( 0xff, 0xff, 0x00 );
const COLOR ColorTool::NAVY	= getColor( 0x00, 0x00, 0x80 );
const COLOR ColorTool::BLUE	= getColor( 0x00, 0x00, 0xff );
const COLOR ColorTool::TEAL	= getColor( 0x00, 0x80, 0x80 );
const COLOR ColorTool::AQUA	= getColor( 0x00, 0xff, 0xff );


// get R value of color
unsigned char ColorTool::getRed( COLOR color ) {
	return (unsigned char)( color & 0xff );
}

// get G value of color
unsigned char ColorTool::getGreen( COLOR color ) {
	return (unsigned char)( ( color / 0x100 ) & 0xff );
}

// get B value of color
unsigned char ColorTool::getBlue( COLOR color ) {
	return (unsigned char)( ( color / 0x10000 ) & 0xff );
}

// get COLOR data
COLOR ColorTool::getColor( unsigned char r, unsigned char g, unsigned char b ) {
	return ( r + g * 0x100 + b * 0x10000 );
}

// get COLOR data
COLOR ColorTool::getColor( const char* s ) {
	// to lower case
	std::string str = tolowercase( s );

	// check string
	static kome::core::ConversionElement< std::string, COLOR > colorStrings[] = {
		{ "black",   BLACK },
		{ "silver",  SILVER },
		{ "gray",	GRAY },
		{ "white",   WHITE },
		{ "maroon",  MAROON },
		{ "red",	 RED },
		{ "purple",  PURPLE },
		{ "fuchsia", FUCHSIA },
		{ "green",   GREEN },
		{ "lime",	LIME },
		{ "olive",   OLIVE },
		{ "yellow",  YELLOW },
		{ "navy",	NAVY },
		{ "blue",	BLUE },
		{ "teal",	TEAL },
		{ "aqua",	AQUA }
	};

	// get color
	COLOR color = CONVERT_FROM_ARRAY( str, (COLOR)0x1000000, colorStrings );
	if( color < 0x1000000 ) {
		return color;
	}

	// get long
	if( !str.empty() && str.at( 0 ) == '#' ) {
		str = str.substr( 1 );
	}
	int c = long();
	if( !isint( str.c_str(), 16, &c ) ) {
		c = 0;
		LOG_WARN( FMT( "Cannot convert string[%s] to color.", NVL( s, "(null)" ) ) );
	}
	c = c % 0x1000000;
	unsigned char blue  = c & 0xff;
	unsigned char green = ( c >> 8 ) & 0xff;
	unsigned char red   = ( c >> 16 ) & 0xff;

	color = getColor( red, green, blue );

	return color;
}

// get string
std::string ColorTool::getString( COLOR color ) {
	std::string s = "#";

	s.append( FMT( "%02x", getRed( color ) ) );
	s.append( FMT( "%02x", getGreen( color ) ) );
	s.append( FMT( "%02x", getBlue( color ) ) );

	return s;
}

// gets additive color
COLOR ColorTool::getAdditiveColor( const COLOR c0, const COLOR c1, const double w0, const double w1 ) {
	// RGB
	unsigned char r = roundnum( ( w0 * getRed( c0 ) + w1 * getRed( c1 ) ) / ( w0 + w1 ) );
	unsigned char g = roundnum( ( w0 * getGreen( c0 ) + w1 * getGreen( c1 ) ) / ( w0 + w1 ) );
	unsigned char b = roundnum( ( w0 * getBlue( c0 ) + w1 * getBlue( c1 ) ) / ( w0 + w1 ) );

	// get color
	return getColor( r, g, b );
}

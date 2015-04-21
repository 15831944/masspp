/**
 * @file Font.cpp
 * @brief implements of Font class
 *
 * @author S.Tanaka
 * @date 2006.09.27
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "Font.h"

using namespace kome::img;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


// constructor
Font::Font(
		const int size,
		const Family family,
		const Style style,
		const Weight weight,
		const char* face,
		const bool underline,
		const bool strikethrough
) {
	setSize( size );
	setFamily( family );
	setStyle( style );
	setWeight( weight );
	setFace( face );
	setUnderline( underline );
	setStrikeThrough( strikethrough );
}

// destructor
Font::~Font() {
}

// set font size
void Font::setSize( const int size ) {
	m_size = size;
}

// get font size
int Font::getSize() {
	return m_size;
}

// set font family
void Font::setFamily( const Family family ) {
	m_family = family;
}

// get font family
Font::Family Font::getFamily() {
	return m_family;
}

// set font style
void Font::setStyle( const Style style ) {
	m_style = style;
}

// get font style
Font::Style Font::getStyle() {
	return m_style;
}

// set font weight
void Font::setWeight( const Weight weight ) {
	m_weight = weight;
}

// get font weight
Font::Weight Font::getWeight() {
	return m_weight;
}

// set font face name
void Font::setFace( const char* face ) {
	m_face = std::string( NVL( face, "" ) );
}

// get font face name
const char* Font::getFace() {
	return m_face.c_str();
}

// set underline
void Font::setUnderline( const bool underline ) {
	m_underline = underline;
}

// has underline
bool Font::hasUnderline() {
	return m_underline;
}

// set strike-through
void Font::setStrikeThrough( const bool strikethrough ) {
	m_strikeThrough = strikethrough;
}

// has strike-through
bool Font::hasStrikeThrough() {
	return m_strikeThrough;
}

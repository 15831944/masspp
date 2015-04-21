/**
 * @file FontWrapper.cpp
 * @brief implements of FontWrapper class
 *
 * @author S.Tanaka
 * @date 2006.09.27
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#include "stdafx.h"
#include "FontWrapper.h"

#include <utility>

#include "ClrObjectTool.h"
#include "ConversionArrays.h"


using namespace kome::clr;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
FontWrapper::FontWrapper( kome::img::Font& font ) : m_font( font ) {
}

// destructor
FontWrapper::~FontWrapper() {
}

// get font object
kome::img::Font& FontWrapper::getFont() {
	return m_font;
}

// set font size
void FontWrapper::setSize( int size ) {
	m_font.setSize( size );
}

// get font size
int FontWrapper::getSize() {
	return m_font.getSize();
}

// set font family
void FontWrapper::setFamily( Family family ) {
	kome::img::Font::Family f = INVERT_FROM_ARRAY( family, kome::img::Font::FAMILY_SERIF, g_fontFamilies );
	m_font.setFamily( f );
}

// get font family
FontWrapper::Family FontWrapper::getFamily() {
	Family f = CONVERT_FROM_ARRAY( m_font.getFamily(), Family::SERIF, g_fontFamilies );
	return f;
}

// set font style
void FontWrapper::setStyle( Style style ) {
	kome::img::Font::Style s = INVERT_FROM_ARRAY( style, kome::img::Font::STYLE_NORMAL, g_fontStyles );
	m_font.setStyle( s );
}

// get font style
FontWrapper::Style FontWrapper::getStyle() {
	Style s = CONVERT_FROM_ARRAY( m_font.getStyle(), Style::NORMAL, g_fontStyles );
	return s;
}

// get font weight
void FontWrapper::setWeight( Weight weight ) {
	kome::img::Font::Weight w = INVERT_FROM_ARRAY( weight, kome::img::Font::WEIGHT_LIGHT, g_fontWeights );
	m_font.setWeight( w );
}

// get font weight
FontWrapper::Weight FontWrapper::getWeight() {
	Weight w = CONVERT_FROM_ARRAY( m_font.getWeight(), Weight::LIGHT, g_fontWeights );
	return w;
}

// set font face name
void FontWrapper::setFace( System::String^ face ) {
	m_font.setFace( ClrObjectTool::convertString( face ).c_str() );
}

// get font face name
System::String^ FontWrapper::getFace() {
	return gcnew System::String( m_font.getFace() );
}

// set underline
void FontWrapper::setUnderline( const bool underline ) {
	m_font.setUnderline( underline );
}

// has underline
bool FontWrapper::hasUnderline() {
	return m_font.hasUnderline();
}

// set strike-through
void FontWrapper::setStrikeThrough( const bool strikethrough ) {
	m_font.setStrikeThrough( strikethrough );
}

// has strike-through
bool FontWrapper::hasStrikeThrough() {
	return m_font.hasStrikeThrough();
}

/**
 * @file FontTool.cpp
 * @brief implements of FontTool class
 *
 * @author S.Tanaka
 * @date 2013.04.09
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "FontTool.h"

#include <wx/wx.h>


using namespace kome::window;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// font family conversion array
kome::core::ConversionElement< kome::img::Font::Family, int > g_fontFamilies[] = {
	{ kome::img::Font::FAMILY_SERIF,	  wxFONTFAMILY_ROMAN },
	{ kome::img::Font::FAMILY_SANS_SERIF, wxFONTFAMILY_SWISS },
	{ kome::img::Font::FAMILY_CURSIVE,	wxFONTFAMILY_SCRIPT },
	{ kome::img::Font::FAMILY_FANTASY,	wxFONTFAMILY_DECORATIVE },
	{ kome::img::Font::FAMILY_MONOSPACE,  wxFONTFAMILY_MODERN }
};

// font style conversion array
kome::core::ConversionElement< kome::img::Font::Style, int > g_fontStyles[] = {
	{ kome::img::Font::STYLE_NORMAL, wxNORMAL },
	{ kome::img::Font::STYLE_SLANT,  wxSLANT },
	{ kome::img::Font::STYLE_ITALIC, wxITALIC }
};

// font weight conversion array
kome::core::ConversionElement< kome::img::Font::Weight, int > g_fontWeights[] = {
	{ kome::img::Font::WEIGHT_LIGHT, wxLIGHT },
	{ kome::img::Font::WEIGHT_BOLD,  wxBOLD }
};


// convert
wxFont FontTool::convertFont( kome::img::Font& font ) {
	// font properties
	int size		 = font.getSize();
	int family	   = CONVERT_FROM_ARRAY( font.getFamily(), (int)wxDEFAULT, g_fontFamilies );
	int style		= CONVERT_FROM_ARRAY( font.getStyle(), (int)wxNORMAL, g_fontStyles );
	int weight	   = CONVERT_FROM_ARRAY( font.getWeight(), (int)wxLIGHT, g_fontWeights );
	std::string face = std::string( NVL( font.getFace(), "" ) );

	// font object
	wxFont f(
		size,
		family,
		style,
		weight,
		font.hasUnderline(),
		wxT( face.c_str() )
	);

	f.SetUnderlined( font.hasUnderline() );

	return f;
}

// convert
kome::img::Font FontTool::convertFont( wxFont& font ) {
	// font properties
	int size = font.GetPointSize();
	kome::img::Font::Family family = INVERT_FROM_ARRAY( font.GetFamily(), kome::img::Font::FAMILY_SERIF, g_fontFamilies );
	kome::img::Font::Style style = INVERT_FROM_ARRAY( font.GetStyle(), kome::img::Font::STYLE_NORMAL, g_fontStyles );
	kome::img::Font::Weight weight = INVERT_FROM_ARRAY( font.GetWeight(), kome::img::Font::WEIGHT_LIGHT, g_fontWeights );
	std::string face = font.GetFaceName().c_str();

	// font object
	kome::img::Font f(
		size, family, style, weight, face.c_str(), font.GetUnderlined()
	);

	return f;
}

// convert font from string
kome::img::Font FontTool::convertFontFromString(  const char* fontInfo ){
	// default
	std::vector< std::string > defaultElms;
	defaultElms.push_back( "#000000" );
	defaultElms.push_back( "8" );
	defaultElms.push_back( "0" );
	defaultElms.push_back( "0" );
	defaultElms.push_back( "1" );
	defaultElms.push_back( "Calibri" );

	// get information
	std::vector< std::string > elms;
	stringtoken( fontInfo, ",", elms );

	while( elms.size() < defaultElms.size() ) {
		elms.push_back( defaultElms[ elms.size() ] );
	}

	if( elms.size() >= 6 ) {
		COLOR color = kome::img::ColorTool::getColor( elms[ 0 ].c_str() );
		kome::img::Font font(
			toint( elms[ 1 ].c_str(), 10, 8 ),
			(kome::img::Font::Family)toint( elms[ 4 ].c_str(), 10, 0 ),
			(kome::img::Font::Style)toint( elms[ 2 ].c_str(), 10, 0 ),
			(kome::img::Font::Weight)toint( elms[ 3 ].c_str(), 10, 0 ),
			elms[ 5 ].c_str()
		);

		return font;
	}

	return NULL;
}

// get font
kome::img::Font FontTool::getFontFromIni( const char* section, const char* key ){
	// get value 
	kome::core::MsppManager& msppMgr = kome::core::MsppManager::getInstance();
	kome::core::IniFile* ini = msppMgr.getIniFile();
	
	std::string s;
	if( ini != NULL ){
		s = ini->getString( section, key, "" );
	}	

	return kome::window::FontTool::convertFontFromString( s.c_str() );
}

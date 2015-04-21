/**
 * @file ConversionArrays.h
 * @brief definition arrays for conversion
 *
 * @author S.Tanaka
 * @date 2006.09.28
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#ifndef __KOME_CLR_CONVERSION_ARRAYS_H__
#define __KOME_CLR_CONVERSION_ARRAYS_H__

#include "CommonType.h"
#include "GraphicsWrapper.h"
#include "FontWrapper.h"


// global variables
namespace kome {
	namespace clr {
		/** the array for conversion of search type */
		static kome::core::ConversionElement< kome::objects::SearchType, SearchType > g_searchTypes[] = {
			{ kome::objects::SEARCH_PREV, SearchType::PREV },
			{ kome::objects::SEARCH_NEXT, SearchType::NEXT },
			{ kome::objects::SEARCH_NEAR, SearchType::NEAR }
		};

		/** the array for conversion of draw style */
		static kome::core::ConversionElement< kome::img::Graphics::DrawStyle, GraphicsWrapper::DrawStyle > g_graphicsDrawStyles[] = {
			{ kome::img::Graphics::STYLE_SOLID,	  GraphicsWrapper::DrawStyle::SOLID },
			{ kome::img::Graphics::STYLE_DOT,			 GraphicsWrapper::DrawStyle::DOT },
			{ kome::img::Graphics::STYLE_LONG_DASH,  GraphicsWrapper::DrawStyle::LONG_DASH },
			{ kome::img::Graphics::STYLE_SHORT_DASH, GraphicsWrapper::DrawStyle::SHORT_DASH },
			{ kome::img::Graphics::STYLE_DOT_DASH,   GraphicsWrapper::DrawStyle::DOT_DASH },
		};

		/** the array for conversion of fill pattern */
		static kome::core::ConversionElement< kome::img::Graphics::FillPattern, GraphicsWrapper::FillPattern > g_graphicsFillPatterns[] = {
			{ kome::img::Graphics::PATTERN_SOLID,	  GraphicsWrapper::FillPattern::SOLID },
			{ kome::img::Graphics::PATTERN_BDIAGONAL,  GraphicsWrapper::FillPattern::BDIAGONAL },
			{ kome::img::Graphics::PATTERN_FDIAGONAL,  GraphicsWrapper::FillPattern::FDIAGONAL },
			{ kome::img::Graphics::PATTERN_CROSS,	  GraphicsWrapper::FillPattern::CROSS },
			{ kome::img::Graphics::PATTERN_CROSS_DIAG, GraphicsWrapper::FillPattern::CROSS_DIAG },
			{ kome::img::Graphics::PATTERN_HORIZONTAL, GraphicsWrapper::FillPattern::HORIZONTAL },
			{ kome::img::Graphics::PATTERN_VERTICAL,   GraphicsWrapper::FillPattern::VERTICAL }
		};

		/** the array for conversion of fill style */
		static kome::core::ConversionElement< kome::img::Graphics::FillStyle, GraphicsWrapper::FillStyle > g_graphicsFillStyles[] = {
			{ kome::img::Graphics::RULE_ODDEVEN, GraphicsWrapper::FillStyle::ODDEVEN },
			{ kome::img::Graphics::RULE_WINDING, GraphicsWrapper::FillStyle::WINDING }
		};

		/** the array for conversion of align */
		static kome::core::ConversionElement< kome::img::Graphics::TextHAlign, GraphicsWrapper::TextHAlign > g_graphicsHAligns[] = {
			{ kome::img::Graphics::HALIGN_LEFT,   GraphicsWrapper::TextHAlign::LEFT },
			{ kome::img::Graphics::HALIGN_CENTER, GraphicsWrapper::TextHAlign::CENTER },
			{ kome::img::Graphics::HALIGN_RIGHT,  GraphicsWrapper::TextHAlign::RIGHT }
		};

		/** the array for conversion of valign */
		static kome::core::ConversionElement< kome::img::Graphics::TextVAlign, GraphicsWrapper::TextVAlign > g_graphicsVAligns[] = {
			{ kome::img::Graphics::VALIGN_TOP,	GraphicsWrapper::TextVAlign::TOP },
			{ kome::img::Graphics::VALIGN_MIDDLE, GraphicsWrapper::TextVAlign::MIDDLE },
			{ kome::img::Graphics::VALIGN_BOTTOM, GraphicsWrapper::TextVAlign::BOTTOM }
		};

		/** the array for conversion of marks */
		static kome::core::ConversionElement< kome::img::Graphics::Mark, GraphicsWrapper::Mark > g_graphicsMarks[] = {
			{ kome::img::Graphics::MARK_NONE,		   GraphicsWrapper::Mark::NONE },
			{ kome::img::Graphics::MARK_CIRCLE,		 GraphicsWrapper::Mark::CIRCLE },
			{ kome::img::Graphics::MARK_FILLED_CIRCLE,  GraphicsWrapper::Mark::FILLED_CIRCLE },
			{ kome::img::Graphics::MARK_DELTA,		  GraphicsWrapper::Mark::DELTA },
			{ kome::img::Graphics::MARK_FILLED_DELTA,   GraphicsWrapper::Mark::FILLED_DELTA },
			{ kome::img::Graphics::MARK_NABLA,		  GraphicsWrapper::Mark::NABLA },
			{ kome::img::Graphics::MARK_FILLED_NABLA,   GraphicsWrapper::Mark::FILLED_NABLA },
			{ kome::img::Graphics::MARK_SQUARE,		 GraphicsWrapper::Mark::SQUARE },
			{ kome::img::Graphics::MARK_FILLED_SQUARE,  GraphicsWrapper::Mark::FILLED_SQUARE },
			{ kome::img::Graphics::MARK_DIAMOND,		GraphicsWrapper::Mark::DIAMOND },
			{ kome::img::Graphics::MARK_FILLED_DIAMOND, GraphicsWrapper::Mark::FILLED_DIAMOND },
			{ kome::img::Graphics::MARK_STAR,		   GraphicsWrapper::Mark::STAR },
			{ kome::img::Graphics::MARK_FILLED_STAR,	GraphicsWrapper::Mark::FILLED_STAR },
			{ kome::img::Graphics::MARK_PLUS,		   GraphicsWrapper::Mark::PLUS },
			{ kome::img::Graphics::MARK_TIMES,		  GraphicsWrapper::Mark::TIMES }
		};

		/** the array for conversion of font family */
		static kome::core::ConversionElement< kome::img::Font::Family, FontWrapper::Family > g_fontFamilies[] = {
			{ kome::img::Font::FAMILY_SERIF,	  FontWrapper::Family::SERIF },
			{ kome::img::Font::FAMILY_SANS_SERIF, FontWrapper::Family::SANS_SERIF },
			{ kome::img::Font::FAMILY_CURSIVE,	FontWrapper::Family::CURSIVE },
			{ kome::img::Font::FAMILY_FANTASY,	FontWrapper::Family::FANTASY },
			{ kome::img::Font::FAMILY_MONOSPACE,  FontWrapper::Family::MONOSPACE }
		};

		/** the array for conversion of font style */
		static kome::core::ConversionElement< kome::img::Font::Style, FontWrapper::Style > g_fontStyles[] = {
			{ kome::img::Font::STYLE_NORMAL, FontWrapper::Style::NORMAL },
			{ kome::img::Font::STYLE_SLANT,  FontWrapper::Style::SLANT },
			{ kome::img::Font::STYLE_ITALIC, FontWrapper::Style::ITALIC }
		};

		/** the array for conversion of font weight */
		static kome::core::ConversionElement< kome::img::Font::Weight, FontWrapper::Weight > g_fontWeights[] = {
			{ kome::img::Font::WEIGHT_LIGHT, FontWrapper::Weight::LIGHT },
			{ kome::img::Font::WEIGHT_BOLD,  FontWrapper::Weight::BOLD }
		};
	}
}

#endif	// __KOME_CLR_CONVERSION_ARRAYS_H__

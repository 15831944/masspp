/**
 * @file GraphTool.cpp
 * @brief implements of GraphTool class
 *
 * @author S.Tanaka
 * @date 2007.08.22
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "GraphTool.h"

#include "Graphics.h"
#include "ColorTool.h"

#include <math.h>


using namespace kome::img;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// draw graph rect
void GraphTool::drawGraphRect( Graphics& graphics ) {
	// rect
	int rectPoints[] = {
		graphics.getGraphPositionLeft(),  graphics.getGraphPositionTop(),
		graphics.getGraphPositionLeft(),  graphics.getGraphPositionBottom(),
		graphics.getGraphPositionRight(), graphics.getGraphPositionBottom(),
		graphics.getGraphPositionRight(), graphics.getGraphPositionTop(),
		graphics.getGraphPositionLeft(),  graphics.getGraphPositionTop()
	};

	// draw
	graphics.drawLines( rectPoints, sizeof( rectPoints ) / ( sizeof( int ) * 2 ) );
}

// draw x scale
void GraphTool::drawXScale( Graphics& graphics, const unsigned int density, const int length, const int space, const bool grid, const double minX, const double maxX ) {
	// get align
	Graphics::TextHAlign hAlign = graphics.getTextHAlign();
	Graphics::TextVAlign vAlign = graphics.getTextVAlign();

	// start & end
	const double startX = ( minX < 0.0 ? graphics.getMinX() : minX );
	const double endX = ( maxX < 0.0 ? graphics.getMaxX() : maxX );

	// get unit
	double range = endX - startX;
	double range2 = graphics.getMaxX() - graphics.getMinX();
	double alpha = range2 / range;
	double minSpace = range * (double)MAX( 1, density ) / (double)graphics.getGraphWidth();
	int logUnit = (int)ceil( log10( minSpace ) );
	double unit = pow( 10.0, (double)logUnit );
	bool insertHalf = ( unit > 2.0 *  minSpace );

	// index
	int startIndex = (int)floor( startX / unit );
	int endIndex = (int)ceil( endX / unit );

	// set alignment
	graphics.setTextHAlign( Graphics::HALIGN_CENTER );
	graphics.setTextVAlign( Graphics::VALIGN_TOP );

	// label format
	std::string fmt = FMT( "%%.%df", ( logUnit < 0 ) ? -logUnit : 0 );

	// get label skip
	int tmpSkip = 1;
	int skip = 1;
	int prev = 1;
	if( space >= 0 ) {
		for( int i = startIndex; i <= endIndex; i++ ) {
			// position
			double x = unit * (double)i;
			double x2 = graphics.getMinX() + ( x - startX ) * alpha;

			double px = double();
			double py = double();
			graphics.transformDataToPosition( x2, 0.0, &px, &py );

			int ppx = roundnum( px );

			// label
			std::string lbl = FMT( fmt.c_str(), x );
			// size
			int width = int();
			int height = int();			
			graphics.getTextSize( lbl.c_str(), &width, &height );

			int left = ppx - width / 2;
			int right = ppx + width / 2;

			// draw
			if( i == 0 || left > prev ) {
				prev = right + space;
				tmpSkip = 1;
			}
			else {
				tmpSkip++;
				skip = MAX( tmpSkip, skip );
			}
		}
	}

	// grid color
	COLOR color = graphics.getColor();
	COLOR gridColor = grid ? kome::img::ColorTool::getAdditiveColor( color, ColorTool::WHITE ) : color;

	// draw label and scale
	for( int i = startIndex; i <= endIndex; i++ ) {
		// get length
		int l = ( insertHalf || i % 5 == 0 ) ? length : length / 2;

		// position
		double x = unit * (double)i;
		double x2 = graphics.getMinX() + ( x - startX ) * alpha;

		double px = double();
		double py = double();
		graphics.transformDataToPosition( x2, 0.0, &px, &py );

		int ppx = roundnum( px );

		// grid
		if( grid && ppx >= graphics.getGraphPositionLeft() && ppx <= graphics.getGraphPositionRight() ) {
			// color						
			graphics.setColor( gridColor );

			// draw grid
			int gridLine[] = {
				ppx, graphics.getGraphPositionTop(),
				ppx, graphics.getGraphPositionBottom()
			};
			graphics.drawLines( gridLine, sizeof( gridLine ) / ( 2 * sizeof( int ) ) );

			// color
			graphics.setColor( color );
		}

		// draw scale
		if( l > 0 && ppx >= graphics.getGraphPositionLeft() && ppx <= graphics.getGraphPositionRight() ) {
			int scaleLine[] = {
				ppx, graphics.getGraphPositionBottom(),
				ppx, graphics.getGraphPositionBottom() + l
			};
			graphics.drawLines( scaleLine, sizeof( scaleLine ) / ( 2 * sizeof( int ) ) );
		}

		// draw label
		if( space >= 0 && i % skip == 0 ) {
			// label
			std::string lbl = FMT( fmt.c_str(), x );

			// size
			int width = int();
			int height = int();			
			graphics.getTextSize( lbl.c_str(), &width, &height );

			int left = ppx - width / 2;
			int right = ppx + width / 2;

			// draw
			if( left > 0 && right < (int)graphics.getWidth() 
					&& ppx >= graphics.getGraphPositionLeft() && ppx <= graphics.getGraphPositionRight() ) {
				graphics.drawText( lbl.c_str(), ppx, graphics.getGraphPositionBottom() + length + 1 );
			}
		}

		// half scale
		if( insertHalf ) {
			// position
			x += unit / 2.0;
			graphics.transformDataToPosition( x, 0.0, &px, &py );

			ppx = roundnum( px );

			l = length / 2;

			// draw scale			
			if( l > 0 && ppx >= graphics.getGraphPositionLeft() && ppx <= graphics.getGraphPositionRight() ) {
				int scaleLine[] = {
					ppx, graphics.getGraphPositionBottom(),
					ppx, graphics.getGraphPositionBottom() + l
				};
				graphics.drawLines( scaleLine, sizeof( scaleLine ) / ( 2 * sizeof( int ) ) );
			}
		}
	}

	// restore align
	graphics.setTextHAlign( hAlign );
	graphics.setTextVAlign( vAlign );
}

// draw y scale
void GraphTool::drawYScale( Graphics& graphics, const unsigned int density, const int length, const int space, const bool grid, const double minY, const double maxY ) {
	// get align
	Graphics::TextHAlign hAlign = graphics.getTextHAlign();
	Graphics::TextVAlign vAlign = graphics.getTextVAlign();

	// start & end
	const double startY = ( minY < 0.0 ? graphics.getMinY() : minY );
	const double endY = ( maxY < 0.0 ? graphics.getMaxY() : maxY );

	// get unit
	double range = endY - startY;
	double range2 = graphics.getMaxY() - graphics.getMinY();
	double alpha = range2 / range;
	double minSpace = range * (double)MAX( 1, density ) / (double)graphics.getGraphHeight();
	int logUnit = (int)ceil( log10( minSpace ) );
	double unit = pow( 10.0, (double)logUnit );
	bool insertHalf = ( unit > 2.0 *  minSpace );

	// index
	int startIndex = (int)floor( startY / unit );
	int endIndex = (int)ceil( endY / unit );

	// set alignment
	graphics.setTextHAlign( Graphics::HALIGN_RIGHT );
	graphics.setTextVAlign( Graphics::VALIGN_MIDDLE);

	// label format
	std::string fmt = FMT( "%%.%df", ( logUnit < 0 ) ? -logUnit : 0 );

	// get label skip
	int tmpSkip = 1;
	int skip = 1;
	int prev = (int)graphics.getHeight() - 1;
	if( space >= 0 ) {
		for( int i = startIndex; i <= endIndex; i++ ) {
			// position
			double y = unit * (double)i;
			double y2 = graphics.getMinY() + ( y - startY ) * alpha;

			double px = double();
			double py = double();
			graphics.transformDataToPosition( 0.0, y2, &px, &py );

			int ppy = roundnum( py );

			// label
			std::string lbl = FMT( fmt.c_str(), y );
			// size
			int width = int();
			int height = int();			
			graphics.getTextSize( lbl.c_str(), &width, &height );

			int top = ppy - height / 2;
			int bottom = ppy + height / 2;

			// draw
			if( i == 0 || bottom < prev ) {
				prev = top - space;
				tmpSkip = 1;
			}
			else {
				tmpSkip++;
				skip = MAX( tmpSkip, skip );
			}
		}
	}

	// grid color
	COLOR color = graphics.getColor();
	COLOR gridColor = grid ? kome::img::ColorTool::getAdditiveColor( color, ColorTool::WHITE ) : color;

	// draw label and scale	
	for( int i = startIndex; i <= endIndex; i++ ) {
		// get length
		int l = ( insertHalf || i % 5 == 0 ) ? length : length / 2;

		// position
		double y = unit * (double)i;
		double y2 = graphics.getMinY() + ( y - startY ) * alpha;

		double px = double();
		double py = double();
		graphics.transformDataToPosition( 0.0, y2, &px, &py );

		int ppy = roundnum( py );

		// grid
		if( grid && ppy >= graphics.getGraphPositionTop() && ppy <= graphics.getGraphPositionBottom() ) {
			// color						
			graphics.setColor( gridColor );

			// draw grid
			int gridLine[] = {
				graphics.getGraphPositionLeft(),  ppy,
				graphics.getGraphPositionRight(), ppy
			};
			graphics.drawLines( gridLine, sizeof( gridLine ) / ( 2 * sizeof( int ) ) );

			// color
			graphics.setColor( color );
		}

		// draw scale
		if( l > 0 && ppy >= graphics.getGraphPositionTop() && ppy <= graphics.getGraphPositionBottom() ) {
			int scaleLine[] = {
				graphics.getGraphPositionLeft(),	 ppy,
				graphics.getGraphPositionLeft() - l, ppy
			};
			graphics.drawLines( scaleLine, sizeof( scaleLine ) / ( 2 * sizeof( int ) ) );
		}

		// draw label
		if( space >= 0 && i % skip == 0 ) {
			// label
			std::string lbl = FMT( fmt.c_str(), y );

			// size
			int width = int();
			int height = int();			
			graphics.getTextSize( lbl.c_str(), &width, &height );

			int top = ppy - height / 2;
			int bottom = ppy + height / 2;

			// draw
			if( bottom <= (int)graphics.getHeight() && top > 0 
					&& ppy >= graphics.getGraphPositionTop() && ppy <= graphics.getGraphPositionBottom() ) {
				graphics.drawText( lbl.c_str(), graphics.getGraphPositionLeft() - length - 1, ppy );
			}
		}

		// half scale
		if( insertHalf ) {
			// position
			y += unit / 2.0;
			graphics.transformDataToPosition( 0.0, y, &px, &py );

			ppy = roundnum( py );

			l = length / 2;

			// draw scale
			if( l > 0 && ppy >= graphics.getGraphPositionTop() && ppy <= graphics.getGraphPositionBottom() ) {
				int scaleLine[] = {
					graphics.getGraphPositionLeft(),	 ppy,
					graphics.getGraphPositionLeft() - l, ppy
				};
				graphics.drawLines( scaleLine, sizeof( scaleLine ) / ( 2 * sizeof( int ) ) );
			}
		}
	}

	// restore align
	graphics.setTextHAlign( hAlign );
	graphics.setTextVAlign( vAlign );
}

// get max width of y labels
int GraphTool::getMaxWidthOfYLabels( Graphics& graphics, const unsigned int density, const double minY, const double maxY ) {
	// check graph size
	if( graphics.getGraphPositionTop() >= graphics.getGraphPositionBottom() ) {
		return -1;
	}

	// start & end
	const double startY = ( minY < 0.0 ? graphics.getMinY() : minY );
	const double endY = ( maxY < 0.0 ? graphics.getMaxY() : maxY );

	// get unit
	double range = endY - maxY;
	double minSpace = range * (double)MAX( 1, density ) / (double)graphics.getGraphHeight();
	int logUnit = (int)ceil( log10( minSpace ) );
	double unit = pow( 10.0, (double)logUnit );
	bool insertHalf = ( unit > 2.0 *  minSpace );

	// index
	int startIndex = (int)floor( startY / unit );
	int endIndex = (int)ceil( endY / unit );

	// label format
	std::string fmt = FMT( "%%.%df", ( logUnit < 0 ) ? -logUnit : 0 );

	// get width
	int maxWidth = 0;
	for( int i = startIndex; i <= endIndex; i++ ) {
		// y
		double y = unit * (double)i;
	
		// label
		std::string lbl = FMT( fmt.c_str(), y );

		// size
		int width = int();
		int height = int();			
		graphics.getTextSize( lbl.c_str(), &width, &height );

		maxWidth = MAX( width, maxWidth );
	}

	return maxWidth;
}

// get color
COLOR GraphTool::getColorFromIni( const char* section, const char* key, const char* defaultColor ){
		
	// default 
	COLOR color = kome::img::ColorTool::getColor( defaultColor );
	
	// get value 
	kome::core::MsppManager& msppMgr = kome::core::MsppManager::getInstance();
	kome::core::IniFile* ini = msppMgr.getIniFile();

	if( ini != NULL ){
		color = kome::img::ColorTool::getColor( ini->getString( section, key, defaultColor ) );
	}

	return color;
}

// get width
int GraphTool::getWidthFromIni( const char* section, const char* key ){
	// default 
	int width = 1;

	// get value 
	kome::core::MsppManager& msppMgr = kome::core::MsppManager::getInstance();
	kome::core::IniFile* ini = msppMgr.getIniFile();

	if( ini != NULL ){
		width = ini->getInt( section, key, 1 );
	}

	return width;
}

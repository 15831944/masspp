/**
 * @file MagnifyingViewManager.cpp
 * @brief interfaces of MagnifyingViewManager class
 *
 * @author N.Tashiro
 * @date 2012.03.06
 * 
 * Copyright (C) 2014 Shimadzu Corporation All rights reserved.
 */


#include "stdafx.h"
#include "MagnifyingViewManager.h"
#include "MagnifyingViewOperation.h"


using namespace kome::view;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


#define DEFAULT_SCALE			10
#define SCALE_SPACE_HEIGHT		20
//2012/03/22 upd by t.okuno <- update drawing label.
#define LABEL_SPACE				2
#define SECTION								"Label Drawing"
#define SPEC_ACCURACY_PARAM_NAME			"SPEC_ACCURACY"
//2012/03/22 upd by t.okuno -> update drawing label.

// constructor
MagnifyingViewManager::MagnifyingViewManager() {
	m_bDraw = false;
}

// destructor
MagnifyingViewManager::~MagnifyingViewManager() {
}

// add scale information
MagnifyingViewManager::ScaleInfo* MagnifyingViewManager::addScaleInfo( kome::objects::Spectrum* spec, const double start, const double end ) {
	// array
	std::vector< ScaleInfo >* arr = &( m_scaleMap[ spec ] );

	// add
	arr->resize( arr->size() + 1 );
	ScaleInfo* scaleInfo = &( arr->back() );
	scaleInfo->spec = spec;
	scaleInfo->minX = start;
	scaleInfo->maxX = end;
	scaleInfo->scale = DEFAULT_SCALE;
	
	return scaleInfo;
}

// get the number of scale info
unsigned int MagnifyingViewManager::getNumberOfScaleInfo( kome::objects::Spectrum* spec ) {
	if( m_scaleMap.find( spec ) == m_scaleMap.end() ) {
		return 0;
	}

	return m_scaleMap[ spec ].size();
}

// get scale info
MagnifyingViewManager::ScaleInfo* MagnifyingViewManager::getScaleInfo( kome::objects::Spectrum* spec, const unsigned int idx ) {
	// check the map
	if( m_scaleMap.find( spec ) == m_scaleMap.end() ) {
		return NULL;
	}

	// array
	std::vector< ScaleInfo >* arr = &( m_scaleMap[ spec ] );

	if( idx >= arr->size() ) {
		return NULL;
	}

	return &( arr->at( idx ) );
}

// remove scale info
void MagnifyingViewManager::removeScaleInfo( kome::objects::Spectrum* spec, const unsigned int idx ) {
	// check the map
	if( m_scaleMap.find( spec ) == m_scaleMap.end() ) {
		return;
	}

	// array
	std::vector< ScaleInfo >* arr = &( m_scaleMap[ spec ] );
	if( idx >= arr->size() ) {
		return;
	}

	arr->erase( arr->begin() + idx );
}

// clear scale info
void MagnifyingViewManager::clearScaleInfo( kome::objects::Spectrum* spec ) {
	m_scaleMap.erase( spec );
}

// down button (spectrum)
bool MagnifyingViewManager::downButton( kome::objects::Spectrum& spec,
										kome::evt::MouseEvent& evt,
										kome::img::Graphics& graphics,
										wxWindow* canvas ) {
	return false;
}

// double click (spectrum)
bool MagnifyingViewManager::dblClick( kome::objects::Spectrum& spec,
									  kome::evt::MouseEvent& evt,
									  kome::img::Graphics& graphics,
									  wxWindow* canvas ) {
	int		nX = evt.x();
	int		nY = evt.y();
	int		nTop = graphics.getGraphPositionTop();

	if( (nTop > nY) || ((nTop + SCALE_SPACE_HEIGHT) < nY) ){
		return false;
	}

	int		nRight = graphics.getGraphPositionRight();
	int		nLeft = graphics.getGraphPositionLeft();
	unsigned int	i;
	unsigned int	nPos;
	unsigned int	nSize = getNumberOfScaleInfo( &spec );

	for( i = 0; i < nSize; ++i ){
		nPos = nSize - i - 1;
		ScaleInfo* scaleInfo = getScaleInfo( &spec, i );

		if( (scaleInfo->spec == &spec) ) {
			// get draw range
			double	dWork = 0.0;
			double	dStartX = 0.0;
			double	dEndX = 0.0;

			graphics.transformDataToPosition( scaleInfo->minX, 0.0, &dStartX, &dWork );
			graphics.transformDataToPosition( scaleInfo->maxX, 0.0, &dEndX, &dWork );

			int		nStartX = (int)(dStartX + 0.5);
			int		nEndX = (int)(dEndX + 0.5);

			if( nStartX < nLeft ){
				nStartX = nLeft;
			}
			if( nEndX > nRight ){
				nEndX = nRight;
			}

			if( (nStartX > nX) || (nEndX < nX) ){
				continue;
			}

			MagnifyDialog dlg( kome::window::WindowTool::getMainWindow(), scaleInfo->spec );
// >>>>>>	@Date:2013/06/03	<Modify>	A.Ozaki
// nPosの算出方法がおかしい
// ScaleInfoが登録されている場所のindexを使用しないと、削除するy軸拡大領域が
// 異なってきます。
// 今の計算だと必ず先頭を指すことになります。

			dlg.setID( i );

// <<<<<<	@Date:2013/06/03	<Modify>	A.Ozaki
			dlg.setStartPos( scaleInfo->minX );
			dlg.setEndPos( scaleInfo->maxX );
			dlg.setScale( scaleInfo->scale );
			dlg.setSpec( scaleInfo->spec );
			//2012/03/07 upd by t.okuno -> change dialog type. (Show -> ShowModal)

			if( dlg.ShowModal() == wxID_OK ) {
				MagnifyingViewOperation* op = new MagnifyingViewOperation();
				op->setShortName("magnify_view");
				op->setOperationName("Magnifying View");
				op->setOperationType( kome::operation::Operation::TYPE_GUI );
				op->setTargetSample( scaleInfo->spec->getSample() );
				op->setSpectrum( scaleInfo->spec );
				op->setScaleIndex( nPos );
				op->setRange( dlg.getStartPos(), dlg.getEndPos() );
				op->setScale( (double)dlg.getScale() );

				if( op->execute() ) {
					kome::window::WindowTool::refresh();
				}
				else {
					delete op;
				}
			}

			//2012/03/07 upd by t.okuno <- change dialog type. (Show -> ShowModal)
			return true;
		}
	}

	return false;
}

// get object
MagnifyingViewManager& MagnifyingViewManager::getInstance() {
	
	static MagnifyingViewManager mgr;

	return mgr;
}

// draw spectrum window
bool MagnifyingViewManager::DrawSpecWindowFg( kome::objects::Parameters* params )
{
	if( m_bDraw ){
		return false;
	}

	m_bDraw = true;

	kome::objects::Spectrum* spectrum = kome::plugin::PluginCallTool::getSpectrum( params );
	if( spectrum == NULL ){
		return false;
	}
	kome::img::Graphics* graphics = kome::plugin::PluginCallTool::getGraphics( params );
	if( graphics == NULL ){
		return false;
	}

	wxWindow* canvas = kome::plugin::PluginCallTool::getCanvas( params );
	if( canvas == NULL ){
		return false;
	}

	for( std::map< kome::objects::Spectrum*, std::vector< ScaleInfo > >::iterator it = m_scaleMap.begin();
				it != m_scaleMap.end(); it++ ) {
		kome::objects::Spectrum* spec = (*it).first;
		unsigned int num = getNumberOfScaleInfo( spec );
		// Spec No.86119 Magnifying is applied to all spectra. @date 2012.06.22 <Add> M.Izumi
		if( spec == spectrum ){
			for( unsigned int i = 0; i < num; i++ ) {
				DrawMagnify( i, spec, graphics, canvas );
			}
		}
	}

	m_bDraw = false;

	return true;
}

bool MagnifyingViewManager::DrawMagnify( unsigned int nID,
										 kome::objects::Spectrum* spectrum,
										 kome::img::Graphics* graphics,
										 wxWindow* canvas ) {
	// check param
	if( (spectrum == NULL) || m_scaleMap.find( spectrum ) == m_scaleMap.end() || nID >= m_scaleMap[ spectrum ].size() ) {
		LOG_ERROR( FMT( "Failed to get the scale information." ) );
		return false;
	}

	if( (graphics == NULL) || (canvas == NULL) ) {
		LOG_ERROR( FMT( "Failed to get the graphics information." ) );
		return false;
	}

	// get draw range
	double	dWork = 0.0;
	double	dStartX = 0.0;
	double	dEndX = 0.0;
	ScaleInfo* scaleInfo = getScaleInfo( spectrum, nID );
	
	graphics->transformDataToPosition( scaleInfo->minX, 0.0, &dStartX, &dWork );
	graphics->transformDataToPosition( scaleInfo->maxX, 0.0, &dEndX, &dWork );

	int		nLeft = graphics->getGraphPositionLeft();
	int		nTop = graphics->getGraphPositionTop();
	int		nRight = graphics->getGraphPositionRight();
	int		nStartX = (int)(dStartX + 0.5);
	int		nEndX = (int)(dEndX + 0.5);
	int		nStartY = nTop + 1;
	int		nEndY = graphics->getGraphPositionBottom();
	double dMinX = graphics->getMinX();
	double dMaxX = graphics->getMaxX();
	double dMinY = graphics->getMinY();
	double dMaxY = graphics->getMaxY();

	if( nStartX < nLeft ){
		nStartX = nLeft;
	}
	if( nEndX > nRight ){
		nEndX = nRight;
	}
	if( nStartX > nEndX ){
		return false;
	}
	graphics->transformPositionToData( nStartX, 0.0, &dStartX, &dWork );
	graphics->transformPositionToData( nEndX, 0.0, &dEndX, &dWork );

	// memory device context
	wxDC* pdcCurr = ((kome::window::DCGraphics*)graphics)->getDC();
	wxMemoryDC memDc;

	// get propaties
	kome::img::Graphics::DrawStyle	drawStyleOld = graphics->getDrawStyle();
	COLOR	colorOld = graphics->getColor();
	double	dTranslateXOld = graphics->getTranslateX();
	double	dTranslateYOld = graphics->getTranslateY();
	((kome::window::DCGraphics*)graphics)->setDC( &memDc );
	graphics->setTranslate( -nLeft, -nTop );

	memDc.SetBackground( *wxWHITE_BRUSH );

	// draw graph background
	wxBitmap bgBmp( graphics->getGraphWidth(), graphics->getGraphHeight() );
	memDc.SelectObject( bgBmp );

	memDc.Clear();

	// draw back
	graphics->setColor( kome::img::ColorTool::WHITE );
	graphics->fillRect( nStartX, nStartY, nEndX - nStartX, nEndY - nStartY );

	// draw graph
	graphics->setColor( kome::img::ColorTool::RED );
	if( scaleInfo->scale < 1 ){
		scaleInfo->scale = 1;
	}
	graphics->setMaxY( dMinY + (dMaxY - dMinY) / (double)scaleInfo->scale );
	graphics->setGraphPositionLeft( nStartX );
	graphics->setGraphPositionRight( nEndX );
	graphics->setMinX( dStartX );
	graphics->setMaxX( dEndX );

	kome::plugin::PluginCallTool::drawSpectrumBg( *spectrum, *graphics, canvas );
	kome::plugin::PluginCallTool::drawSpectrum( *spectrum, *graphics, canvas );
	kome::plugin::PluginCallTool::drawSpectrumFg( *spectrum, *graphics, canvas );
	
	//2012/03/22 upd by t.okuno -> update drawing label.
	kome::objects::PeaksManager& pkMgr = kome::objects::PeaksManager::getInstance();
	kome::objects::Peaks* peaks = pkMgr.getPeaks( spectrum );
	if( peaks != NULL ) {
		// accuracy 
		int accuracy = 2;

		// get value
		kome::core::MsppManager& msppMgr = kome::core::MsppManager::getInstance();
		kome::core::IniFile* ini = msppMgr.getIniFile();
		if( ini != NULL ) {
			accuracy = ini->getInt( SECTION, SPEC_ACCURACY_PARAM_NAME, 2 );
		}

		// get xy data
		kome::core::XYData* xyData = spectrum->getXYData();

		// get drawing data
		kome::img::DrawingXYData drawing = kome::img::DrawingXYData( *peaks );

		// draw label
		drawLabel( &drawing, accuracy, peaks, xyData, graphics );
	}
	//2012/03/22 upd by t.okuno <- update drawing label.

	graphics->setMinX( dMinX );
	graphics->setMaxX( dMaxX );
	graphics->setGraphPositionLeft( nLeft );
	graphics->setGraphPositionRight( nRight );
	graphics->setMaxY( dMaxY );

	int			nTextWidth = 0;
	int			nTextHeight = 0;
	wxString	wxStr = FMT( "x%d", scaleInfo->scale );

	// get propaties
	kome::img::Graphics::TextHAlign	textHAlignOld = graphics->getTextHAlign();
	kome::img::Graphics::TextVAlign	textVAlignOld = graphics->getTextVAlign();

	// get text size
	graphics->getTextSize( wxStr.c_str(), &nTextWidth, &nTextHeight );

	// draw text
	graphics->setColor( kome::img::ColorTool::YELLOW );
	graphics->fillRect( nStartX, nStartY, nEndX - nStartX, SCALE_SPACE_HEIGHT );
	graphics->setColor( kome::img::ColorTool::BLACK );
	graphics->setTextHAlign( kome::img::Graphics::HALIGN_LEFT );
	graphics->setTextVAlign( kome::img::Graphics::VALIGN_TOP );
	graphics->drawText( wxStr.c_str(),
						nStartX + (nEndX - nStartX - nTextWidth) / 2,
						nStartY + (SCALE_SPACE_HEIGHT - nTextHeight) / 2 );

	// recover propaties
	graphics->setTextHAlign( textHAlignOld );
	graphics->setTextVAlign( textVAlignOld );

	// draw line
	graphics->setDrawStyle( kome::img::Graphics::STYLE_SOLID );
	graphics->setColor( kome::img::ColorTool::BLACK );
	graphics->drawLine( nStartX, nStartY, nStartX, nEndY );
	graphics->drawLine( nEndX, nStartY, nEndX, nEndY );

	pdcCurr->Blit( nStartX, nStartY, nEndX - nStartX + 1, nEndY - nStartY,
				   &memDc, nStartX - nLeft, nStartY - nTop );

	memDc.SelectObject( wxNullBitmap );

	// recover propaties
	graphics->setDrawStyle( drawStyleOld );
	graphics->setColor( colorOld );
	graphics->setTranslate( dTranslateXOld, dTranslateYOld );

	((kome::window::DCGraphics*)graphics)->setDC( pdcCurr );

	return true;
}

//2012/03/22 upd by t.okuno -> update drawing label.
// draw label
void MagnifyingViewManager::drawLabel(
	kome::img::DrawingXYData* drawing,
	const int accuracy,
	kome::objects::Peaks* peaks,
	kome::core::XYData* xyData,
	kome::img::Graphics* g
){
	// check the parameter
	if( drawing == NULL || xyData == NULL || g == NULL ) {
		return;
	}

	// get data
	std::vector< std::pair< double, double > > xyArray;

	unsigned short usLevel;
	usLevel = drawing->getLevel( g->getGraphWidth(), g->getMinX(), g->getMaxX() );

	usLevel += 1;

	unsigned int uiLength = drawing->getLength( usLevel );

	for( unsigned int i = 0; i < uiLength; i++ ) {

		double dTopX = drawing->getTopX( usLevel, i );
		double dTopY = drawing->getTopY( usLevel, i );

		if( dTopX > g->getMinX() && dTopX < g->getMaxX() ) {
			xyArray.push_back( std::make_pair( dTopX, dTopY ) );
		}
	}

	// set color
	COLOR oldColor = g->getColor();
	g->setColor( kome::img::ColorTool::BLACK );

	// draw label
	std::list< kome::core::Rect< int > > rectList;
	for( unsigned i = 0; i < xyArray.size(); i++ ) {
		// x, y
		double x = xyArray[ i ].first;
		double y = xyArray[ i ].second;

		// label
		std::string label = FMT( "%d", roundnum( x ) );
		if( accuracy > 0 ) {
			std::string lblFmt = FMT( "%%.%df", accuracy );
			label = FMT( lblFmt.c_str(), x );
		}
	
		// get position
		double px = double();
		double py = double();
		g->transformDataToPosition( x, y, &px, &py );

		px = floor( px + 0.5 );
		py = floor( py + 0.5 );

		// get size
		int w = int();
		int h = int();
		g->getTextSize( label.c_str(), &w, &h );

		int l = (int)px - w / 2;
		if( l < g->getGraphPositionLeft() + LABEL_SPACE ) {
			l = g->getGraphPositionLeft() + LABEL_SPACE;
		}
		int r = l + w;
		if( r > g->getGraphPositionRight() - LABEL_SPACE ) {
			r = g->getGraphPositionRight() - LABEL_SPACE;
			l = r - w;
		}

		int b = (int)py;
		int t = (int)py - h;

		// check labeling position
		bool canDraw = ( l >= g->getGraphPositionLeft() && r <= g->getGraphPositionRight()
							&& t >= g->getGraphPositionTop() && b <= g->getGraphPositionBottom() );
		for( std::list< kome::core::Rect< int > >::iterator it = rectList.begin(); it != rectList.end() && canDraw; it++ ) {
			kome::core::Rect< int >& rect = (*it);
			if( l <= rect.right && r >= rect.left && b >= rect.top && t <= rect.bottom ) {
				canDraw = false;
			}
		}

		if( canDraw ){
			double endPos = 0.0;

			if( endPos > y && endPos > 0.0){
				canDraw = false;
			}
		}

		// draw label
		if( canDraw ) {
			g->setTextHAlign( kome::img::Graphics::HALIGN_LEFT );
			g->setTextVAlign( kome::img::Graphics::VALIGN_TOP );

			g->drawText(
				label.c_str(),
				l,
				t
			);

			kome::core::Rect< int > rect( l, t, r, b );
			rectList.push_back( rect );
		}
	}

	// recover color
	g->setColor( oldColor );
}
//2012/03/22 upd by t.okuno <- update drawing label.

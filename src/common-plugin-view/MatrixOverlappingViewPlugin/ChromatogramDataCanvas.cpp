/**
 * @file ChromatogramDataCanvas.cpp
 * @brief implements of ChromatogramDataCanvas class
 *
 * @author S.Tanaka
 * @date 2008.04.28
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "ChromatogramDataCanvas.h"


using namespace kome::matrix::overlapping;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



#define PEAK_POSITION_LENGTH	 10


BEGIN_EVENT_TABLE( ChromatogramDataCanvas, kome::window::OverlappingChromatogramsCanvas )
END_EVENT_TABLE()


// constructor
ChromatogramDataCanvas::ChromatogramDataCanvas(
		wxWindow* parent,
		const int width,
		const int height,
		const int peakId,
		std::vector< ProfilePanel::DataInfo >& data
) : OverlappingChromatogramsCanvas( parent, width, height ) {
	// initialize
	m_drawPeakFlg = false;
	m_peakId = -1;

	setPeakId( peakId, data );
}

// destructor
ChromatogramDataCanvas::~ChromatogramDataCanvas() {
	deleteChroms();
}


// set peak ID
void ChromatogramDataCanvas::setPeakId( const int peakId, std::vector< ProfilePanel::DataInfo >& data ) {
	// check the member
	if( m_peakId == peakId ) {
		return;
	}
	m_peakId = peakId;

	// manager
	kome::ident::IdentificationManager& identMgr = kome::ident::IdentificationManager::getInstance();

	// peak information
	m_rt = identMgr.getMatrixPeakRt( peakId );
	if( m_rt < 0.0 ) {
		return;
	}

	// add chromatograms
	for( unsigned int i = 0; i < data.size(); i++ ) {
		ProfilePanel::DataInfo& d = data[ i ];

		if( d.profileId >= 0 && d.isChrom ) {
			// create chromatogram
			kome::ident::DataChromatogram* chrom = new kome::ident::DataChromatogram( peakId, d.sampleId );
			m_dataChroms.push_back( chrom );
			chrom->setName( d.name.c_str() );
			chrom->setTitle( d.name.c_str() );

			// add
			addChromatogram( chrom );
			setColor( chrom, d.color );
		}
	}

	// range
	kome::plugin::PluginCallTool::changeChromatogramRange(
		*( getBaseChromatogram() ),
		std::max( 0.0, m_rt - 3.0 ),
		m_rt + 3.0,
		(long long)this
	);
}

// set normalized
void ChromatogramDataCanvas::setNormalized( const bool normalized ) {
	// update chromatograms
	for( unsigned int i = 0; i < m_overlappingChroms.size(); i++ ) {
		kome::objects::Chromatogram* chrom = m_overlappingChroms[ i ].chrom;
		chrom->deleteXYData();
		kome::plugin::PluginCallTool::onUpdateChromatogram( *chrom );
	}
	for( unsigned int i = 0; i < m_dataChroms.size(); i++ ) {
		kome::ident::DataChromatogram* chrom = m_dataChroms[ i ];
		chrom->setNormalized( normalized );
		chrom->deleteXYData();
		kome::plugin::PluginCallTool::onUpdateChromatogram( *chrom );
	}

	// refresh
	update();
}

// set aligned
void ChromatogramDataCanvas::setAligned( const bool aligned ) {
	// update chromatograms
	for( unsigned int i = 0; i < m_overlappingChroms.size(); i++ ) {
		kome::objects::Chromatogram* chrom = m_overlappingChroms[ i ].chrom;
		chrom->deleteXYData();
		kome::plugin::PluginCallTool::onUpdateChromatogram( *chrom );
	}
	for( unsigned int i = 0; i < m_dataChroms.size(); i++ ) {
		kome::ident::DataChromatogram* chrom = m_dataChroms[ i ];
		chrom->setAligned( aligned );
		chrom->deleteXYData();
		kome::plugin::PluginCallTool::onUpdateChromatogram( *chrom );
	}

	// refresh
	update();
}

// set draw peak flag
void ChromatogramDataCanvas::setDrawingPeak( const bool drawing ) {
	m_drawPeakFlg = drawing;
}

// update chromatograms
void ChromatogramDataCanvas::updateChroms() {
	// update chromatograms
	for( unsigned int i = 0; i < m_overlappingChroms.size(); i++ ) {
		kome::objects::Chromatogram* chrom = m_overlappingChroms[ i ].chrom;
		chrom->deleteXYData();
		kome::plugin::PluginCallTool::onUpdateChromatogram( *chrom );
	}
	for( unsigned int i = 0; i < m_dataChroms.size(); i++ ) {
		kome::ident::DataChromatogram* chrom = m_dataChroms[ i ];
		chrom->updateChrom();
		chrom->deleteXYData();
		kome::plugin::PluginCallTool::onUpdateChromatogram( *chrom );
	}

	// refresh
	update();
}

// delete chromatograms
void ChromatogramDataCanvas::deleteChroms() {
	for( unsigned int i = 0; i < m_dataChroms.size(); i++ ) {
		delete m_dataChroms[ i ];
	}
	m_dataChroms.clear();
}

// on draw graph foreground
void ChromatogramDataCanvas::onDrawGraphFg( kome::window::DCGraphics& g ) {
	// default
	kome::window::OverlappingChromatogramsCanvas::onDrawGraphFg( g );

	// set color
	g.setColor( kome::img::ColorTool::BLACK );

	// draw position
	g.setDrawStyle( kome::img::Graphics::STYLE_SOLID );

	double px = double();
	double py = double();

	g.transformDataToPosition( m_rt, 0.0, &px, &py );
	int ppx = roundnum( px );
	if( ppx >= g.getGraphPositionLeft() && ppx <= g.getGraphPositionRight() ) {
		g.drawLine(
			ppx,
			g.getGraphPositionTop(),
			ppx,
			g.getGraphPositionBottom()
		);
	}
}

// on draw profile
void ChromatogramDataCanvas::onDrawChromatogramProfile(
		kome::window::DCGraphics& g,
		kome::objects::Chromatogram& chrom
) {
	// default
	kome::window::OverlappingChromatogramsCanvas::onDrawChromatogramProfile( g, chrom );

	// draw peak
	if( m_drawPeakFlg ) {
		// manager
		kome::objects::PointsManager& ptMgr = kome::objects::PointsManager::getInstance();

		// chromatogram
		kome::ident::DataChromatogram* dChrom = dynamic_cast< kome::ident::DataChromatogram* >( &chrom );
		if( dChrom == NULL ) {
			kome::operation::TransformedChromatogram* tChrom
				= dynamic_cast< kome::operation::TransformedChromatogram* >( &chrom );
			if( tChrom != NULL ) {
				dChrom = dynamic_cast< kome::ident::DataChromatogram* >( tChrom->getOrgChromatogram() );
			}
		}
		kome::core::XYData* xyData = ptMgr.getXYData( &chrom );

		// draw position
		if( dChrom != NULL && xyData != NULL ) {
			// left
			kome::core::Point< double > left = dChrom->getPeakLeft();
			if( left.px >= 0.0 ) {
				// y
				kome::core::DataPoints dps;
				kome::numeric::Interpolation::linear( *xyData, dps, &( left.px ), 1 );

				double y = left.py;
				if( dps.getLength() > 0 ) {
					y = dps.getY( 0 );
				}

				// transform
				double px0 = double();
				double py0 = double();
				double px1 = double();
				double py1 = double();

				g.transformDataToPosition( left.px, y, &px0, &py0 );
				g.transformDataToPosition( left.px, left.py, &px1, &py1 );

				g.drawLine(
					roundnum( px1 ),
					roundnum( py1 ) - PEAK_POSITION_LENGTH,
					roundnum( px1 ),
					roundnum( py1 )
				);
			}

			// right
			kome::core::Point< double > right = dChrom->getPeakRight();
			if( right.px >= 0.0 ) {
				// y
				kome::core::DataPoints dps;
				kome::numeric::Interpolation::linear( *xyData, dps, &( right.px ), 1 );

				double y = right.py;
				if( dps.getLength() > 0 ) {
					y = dps.getY( 0 );
				}

				// transform
				double px0 = double();
				double py0 = double();
				double px1 = double();
				double py1 = double();

				g.transformDataToPosition( right.px, y, &px0, &py0 );
				g.transformDataToPosition( right.px, right.py, &px1, &py1 );

				g.drawLine(
					roundnum( px1 ),
					roundnum( py1 ) - PEAK_POSITION_LENGTH,
					roundnum( px1 ),
					roundnum( py1 )
				);
			}
		}
	}
}

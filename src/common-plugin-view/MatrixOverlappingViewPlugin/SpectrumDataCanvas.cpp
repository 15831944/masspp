/**
 * @file SpectrumDataCanvas.cpp
 * @brief implements of SpectrumDataCanvas class
 *
 * @author S.Tanaka
 * @date 2008.09.29
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "SpectrumDataCanvas.h"


using namespace kome::matrix::overlapping;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



#define PEAK_POSITION_LENGTH	 10


BEGIN_EVENT_TABLE( SpectrumDataCanvas, kome::window::OverlappingSpectraCanvas )
END_EVENT_TABLE()


// constructor
SpectrumDataCanvas::SpectrumDataCanvas(
		wxWindow* parent,
		const int width,
		const int height,
		const int peakId,
		std::vector< ProfilePanel::DataInfo >& data
) : OverlappingSpectraCanvas( parent, width, height ), m_peakId( peakId ) {
	// initialize
	m_drawPeakFlg = false;
	m_peakId = -1;

	// add spectra
	setPeakId( peakId, data );
}

// destructor
SpectrumDataCanvas::~SpectrumDataCanvas() {
	deleteSpectra();
}

// set peak ID
void SpectrumDataCanvas::setPeakId( const int peakId, std::vector< ProfilePanel::DataInfo >& data ) {
	// check the member
	if( m_peakId == peakId ) {
		return;
	}
	m_peakId = peakId;

	// manager
	kome::ident::IdentificationManager& identMgr = kome::ident::IdentificationManager::getInstance();
	kome::objects::PeaksManager& pkMgr = kome::objects::PeaksManager::getInstance();

	// peak information
	double mz = identMgr.getMatrixPeakMz( peakId );

	m_mz = mz;
	if( m_mz < 0.0 ) {
		return;
	}

	// delete spectra
	deleteSpectra();

	// spectra
	for( unsigned int i = 0; i < data.size(); i++ ) {
		ProfilePanel::DataInfo& d = data[ i ];

		if( d.sampleId >= 0 && !d.isChrom ) {
			// create spectrum
			kome::ident::DataSpectrum* spec = new kome::ident::DataSpectrum( m_peakId, d.sampleId );
			m_dataSpectra.push_back( spec );
			spec->setName( d.name.c_str() );
			spec->setTitle( d.name.c_str() );

			// peak
			kome::objects::Peaks* peaks = pkMgr.createPeaks( spec );
			kome::objects::PeakElement* peak = peaks->createPeak( mz, 0.0 );
			identMgr.getPeakInfo( m_peakId, d.sampleId, peak );

			// add
			addSpectrum( spec );
			setColor( spec, d.color );
		}
	}

	// range
	kome::plugin::PluginCallTool::changeSpectrumRange(
		*( getBaseSpectrum() ),
		std::max( 0.0, m_mz - 3.0 ),
		m_mz + 3.0,
		(long long)this
	);
}

// set normalized
void SpectrumDataCanvas::setNormalized( const bool normalized ) {
	// update spectra
	for( unsigned int i = 0; i < m_overlappingSpectra.size(); i++ ) {
		 kome::objects::Spectrum* spec = m_overlappingSpectra[ i ].spec;
		spec->deleteXYData();
		kome::plugin::PluginCallTool::onUpdateSpectrum( *spec );
	}
	for( unsigned int i = 0; i < m_dataSpectra.size(); i++ ) {
		kome::ident::DataSpectrum* spec = m_dataSpectra[ i ];
		spec->setNormalized( normalized );
		spec->deleteXYData();
		kome::plugin::PluginCallTool::onUpdateSpectrum( *spec );
	}

	// refresh
	update();
}

// set aligned 
void SpectrumDataCanvas::setAligned( const bool aligned ) {
	// update spectra
	for( unsigned int i = 0; i < m_overlappingSpectra.size(); i++ ) {
		 kome::objects::Spectrum* spec = m_overlappingSpectra[ i ].spec;
		spec->deleteXYData();
		kome::plugin::PluginCallTool::onUpdateSpectrum( *spec );
	}
	for( unsigned int i = 0; i < m_dataSpectra.size(); i++ ) {
		kome::ident::DataSpectrum* spec = m_dataSpectra[ i ];
		spec->setAligned( aligned );
		spec->deleteXYData();
		kome::plugin::PluginCallTool::onUpdateSpectrum( *spec );
	}

	// refresh
	update();
}

// set drawing flag
void SpectrumDataCanvas::setDrawingPeak( const bool drawing ) {
	m_drawPeakFlg = drawing;
}

// update spectra
void SpectrumDataCanvas::updateSpectra() {
	// update spectra
	for( unsigned int i = 0; i < m_overlappingSpectra.size(); i++ ) {
		 kome::objects::Spectrum* spec = m_overlappingSpectra[ i ].spec;
		spec->deleteXYData();
		kome::plugin::PluginCallTool::onUpdateSpectrum( *spec );
	}
	for( unsigned int i = 0; i < m_dataSpectra.size(); i++ ) {
		kome::ident::DataSpectrum* spec = m_dataSpectra[ i ];
		spec->updateSpec();
		spec->deleteXYData();
		kome::plugin::PluginCallTool::onUpdateSpectrum( *spec );
	}

	// refresh
	update();
}

// delete spectra
void SpectrumDataCanvas::deleteSpectra() {
	for( unsigned int i = 0; i < m_dataSpectra.size(); i++ ) {
		delete m_dataSpectra[ i ];
	}
	m_dataSpectra.clear();
}

// on draw graph foreground
void SpectrumDataCanvas::onDrawGraphFg( kome::window::DCGraphics& g ) {
	// default
	kome::window::OverlappingSpectraCanvas::onDrawGraphFg( g );

	// set color
	g.setColor( kome::img::ColorTool::BLACK );

	// draw position
	g.setDrawStyle( kome::img::Graphics::STYLE_SOLID );

	double px = double();
	double py = double();

	g.transformDataToPosition( m_mz, 0.0, &px, &py );
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
void SpectrumDataCanvas::onDrawSpectrumProfile(
		kome::window::DCGraphics& g,
		kome::objects::Spectrum& spec
) {
	
	// default
	kome::window::OverlappingSpectraCanvas::onDrawSpectrumProfile( g, spec );

	// draw peak
	if( m_drawPeakFlg ) {
		// manager
		kome::objects::PointsManager& ptMgr = kome::objects::PointsManager::getInstance();

		// spectrum
		kome::ident::DataSpectrum* dSpec = dynamic_cast< kome::ident::DataSpectrum* >( &spec );
		if( dSpec == NULL ) {
			kome::operation::TransformedSpectrum* tSpec
				= dynamic_cast< kome::operation::TransformedSpectrum* >( &spec );
			if( tSpec != NULL ) {
				dSpec = dynamic_cast< kome::ident::DataSpectrum* >( tSpec->getOrgSpectrum() );
			}
		}
		kome::core::XYData* xyData = ptMgr.getXYData( &spec );

		// draw position
		if( dSpec != NULL && xyData != NULL ) {
			// left
			kome::core::Point< double > left = dSpec->getPeakLeft();
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
			kome::core::Point< double > right = dSpec->getPeakRight();
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

/**
 * @file PickedProfileDialog.h
 * @brief interfaces of PickedProfileDialog class
 *
 * @author S.Tanaka
 * @date 2009.03.17
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "PickedProfileDialog.h"


using namespace kome::matrix::overlapping;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



#define GRID_GAP					3
#define BORDER_SIZE					3


BEGIN_EVENT_TABLE( PickedProfileDialog, kome::window::StandardDialog )
END_EVENT_TABLE()


// constructor
PickedProfileDialog::PickedProfileDialog(
		wxWindow* parent,
		kome::ident::DataSpectrum* spec,
		const double minX,
		const double maxX
) : kome::window::StandardDialog( parent, "Profile" ),
		m_minX( minX ), m_maxX( maxX ) {
	// initialize
	m_spec = spec;
	m_chrom = NULL;
	m_peaks = NULL;
	m_canvas = NULL;

	init();
}

// constructor
PickedProfileDialog::PickedProfileDialog(
		wxWindow* parent,
		kome::ident::DataChromatogram* chrom,
		const double minX,
		const double maxX
) : kome::window::StandardDialog( parent, "Profile" ),
		m_minX( minX ), m_maxX( maxX ) {
	// initialize
	m_spec = NULL;
	m_chrom = chrom;
	m_peaks = NULL;
	m_canvas = NULL;

	init();
}

// destructor
PickedProfileDialog::~PickedProfileDialog() {
	if( IsShown() ) {
		updatePeak();
	}
}

// initialize
void PickedProfileDialog::init() {
	// managers
	kome::ident::IdentificationManager& identMgr = kome::ident::IdentificationManager::getInstance();
	kome::objects::PeaksManager& pkMgr = kome::objects::PeaksManager::getInstance();

	// initialize
	m_canvas = NULL;

	// profile information
	int profileId = -1;
	int peakId = -1;
	int sampleId = -1;
	if( m_spec != NULL ) {
		peakId = m_spec->getMatrixPeakId();
		sampleId = m_spec->getMatrixSampleId();
	}
	else if( m_chrom != NULL ) {
		peakId = m_chrom->getMatrixPeakId();
		sampleId = m_chrom->getMatrixSampleId();
	}

	if( peakId >= 0 && sampleId >= 0 ) {
		profileId = identMgr.getProfile( peakId, sampleId );
	}


	kome::objects::PeakElement peakElm( NULL );
	bool chromFlg = identMgr.isChromatogramProfile( profileId );
	kome::core::XYData* xyData = NULL;
	identMgr.getPeakInfo( peakId, sampleId, &peakElm );

	// create data
	double gap = 0.0;
	double scale = 1.0;
	if( m_chrom != NULL ) {	// chromatogram
		xyData = m_chrom->getXYData();
		m_peaks = pkMgr.createPeaks( m_chrom );
		gap = m_chrom->getGap();
		scale = m_chrom->getScale();
	}
	else if( m_spec != NULL ) {	// spectrum
		xyData = m_spec->getXYData();
		m_peaks = pkMgr.createPeaks( m_spec );
		gap = m_spec->getGap();
		scale = m_spec->getScale();
	}
	if( m_peaks != NULL ) {
		m_peaks->setDataPoints( *xyData );
		m_peaks->clearPoints();
	}

	// add peak
	if( peakElm.getX() >= 0.0 ) {
		kome::objects::PeakElement* peak = m_peaks->createPeak( peakElm.getX() - gap, peakElm.getY() * scale );
		
		if( xyData->getLength() > 0 ) {
			// apex
			int lIdx = xyData->getNearestIndex( peakElm.getLeftX() - gap );
			int rIdx = xyData->getNearestIndex( peakElm.getRightX() - gap );

			double apexX = 0.0;
			double apexY = 0.0;
			for( int i = lIdx; i <= rIdx; i++ ) {
				double x = xyData->getX( i );
				double y = xyData->getY( i );
				if( i == lIdx || y > apexY ) {
					apexX = x;
					apexY = y;
				}
			}

			peak->setApex( apexX - gap, apexY * scale );

			// left, right
			peak->setX( peakElm.getX() - gap );
			peak->setY( peakElm.getY() * scale );
			peak->setLeft( peakElm.getLeftX() - gap, peakElm.getLeftY() * scale );
			peak->setRight( peakElm.getRightX() - gap, peakElm.getRightY() * scale );
		}
	}
}

// update peak
void PickedProfileDialog::updatePeak() {
	// managers
	kome::ident::IdentificationManager& identMgr = kome::ident::IdentificationManager::getInstance();

	// check the member
	if( m_peaks == NULL || ( m_spec == NULL && m_chrom == NULL ) ) {
		return;
	}

	// peak, group
	int peakId = -1;
	int sampleId = -1;
	int profileId = -1;

	if( m_spec != NULL ) {
		peakId = m_spec->getMatrixPeakId();
		sampleId = m_spec->getMatrixSampleId();
	}
	else if( m_chrom != NULL ) {
		peakId = m_chrom->getMatrixPeakId();
		sampleId = m_chrom->getMatrixSampleId();
	}

	if( sampleId >= 0 && peakId >= 0 ) {
		profileId = identMgr.getProfile( peakId, sampleId );
	}

	if( profileId < 0 ) {
		return;
	}

	// RT
	double rt = identMgr.getMatrixPeakRt( peakId );
	bool isChrom = identMgr.isChromatogramProfile( profileId );

	// search peak
	double d = 0.0;
	kome::objects::PeakElement* peak = NULL;
	for( unsigned int i = 0; i < m_peaks->getLength(); i++ ) {
		kome::objects::PeakElement* tmp = m_peaks->getPeak( i );
		double tmpD = fabs( tmp->getX() );

		if( peak == NULL || tmpD < d ) {
			peak = tmp;
			d = tmpD;
		}
	}

	// gap, scale
	double gap = 0.0;
	double scale = 1.0;
	if( m_spec != NULL ) {
		gap = m_spec->getGap();
		scale = m_spec->getScale();
	}
	if( m_chrom != NULL ) {
		gap = m_chrom->getGap();
		scale = m_chrom->getScale();
	}

	const double intensity = ( peak == NULL ? -1.0 : peak->getArea() ) / scale;
	const double x = ( peak == NULL ? -1.0 : peak->getX() ) + gap;
	const double leftX = ( peak == NULL ? -1.0 : peak->getLeftX() ) + gap;
	const double leftY = ( peak == NULL ? -1.0 : peak->getLeftY() ) / scale;
	const double rightX = ( peak == NULL ? -1.0 : peak->getRightX() ) + gap;
	const double rightY = ( peak == NULL ? -1.0 : peak->getRightY() ) / scale;

	if( isChrom && m_chrom != NULL ) {
		rt = x;
	}
	identMgr.setPeakValue( peakId, sampleId, intensity, peak, profileId );

	// update peak
	if( m_spec != NULL ) {
		m_spec->setPeakLeft( leftX, leftY );
		m_spec->setPeakRight( rightX, rightY );
		m_spec->updateSpec();
		m_spec->deleteXYData();
		kome::plugin::PluginCallTool::onUpdateSpectrum( *m_spec );
	}
	if( m_chrom != NULL ) {
		m_chrom->setPeakLeft( leftX, leftY );
		m_chrom->setPeakRight( rightX, rightY );
		m_chrom->updateChrom();
		m_chrom->deleteXYData();
		kome::plugin::PluginCallTool::onUpdateChromatogram( *m_chrom );
	}

	// refresh
	kome::window::WindowTool::refresh();
	wxWindow* parent = GetParent();
	while( parent != NULL ) {
		wxTopLevelWindow* topWnd = dynamic_cast< wxTopLevelWindow* >( parent );
		if( topWnd != NULL ) {
			topWnd->Refresh();
		}
		parent = parent->GetParent();
	}
}

// create main sizer
wxSizer* PickedProfileDialog::createMainSizer() {
	// sizer
	wxGridSizer* sizer = new wxGridSizer( 1, 1, GRID_GAP, GRID_GAP );

	// canvas
	if( m_spec != NULL ) {
		kome::window::SpectrumCanvas* canvas = new kome::window::SpectrumCanvas( this, 600, 200, m_spec );
		m_canvas = canvas;
	}
	else {
		kome::window::ChromatogramCanvas* canvas = new kome::window::ChromatogramCanvas( this, 600, 200, m_chrom );
		m_canvas = canvas;
	}

	sizer->Add( m_canvas, 1, wxALL | wxGROW, BORDER_SIZE );

	// range
	if( m_minX >= 0.0 && m_maxX >= m_minX ) {
		if( m_spec != NULL ) {
			kome::plugin::PluginCallTool::changeSpectrumRange( *m_spec, m_minX, m_maxX, (long long)m_canvas );
		}
		if( m_chrom != NULL ) {
			kome::plugin::PluginCallTool::changeChromatogramRange( *m_chrom, m_minX, m_maxX, (long long)m_canvas );
		}
	}

	return sizer;
}

// on OK
bool PickedProfileDialog::onOK() {
	updatePeak();

	if( m_spec != NULL ) {
		kome::plugin::PluginCallTool::onCloseSpectrum( *m_spec, m_canvas );
	}
	if( m_chrom != NULL ) {
		kome::plugin::PluginCallTool::onCloseChromatogram( *m_chrom, m_canvas );
	}

	return true;
}

// on calcel
bool  PickedProfileDialog::onCancel() {
	if( m_spec != NULL ) {
		kome::plugin::PluginCallTool::rollbackSpecPeaks( *m_spec );
	}
	if( m_chrom != NULL ) {
		kome::plugin::PluginCallTool::rollbackChromPeaks( *m_chrom );
	}

	if( m_spec != NULL ) {
		kome::plugin::PluginCallTool::onCloseSpectrum( *m_spec, m_canvas );
	}
	if( m_chrom != NULL ) {
		kome::plugin::PluginCallTool::onCloseChromatogram( *m_chrom, m_canvas );
	}
	
	return true;
}

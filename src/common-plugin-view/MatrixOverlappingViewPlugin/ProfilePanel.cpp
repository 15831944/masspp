/**
 * @file ProfilePanel.cpp
 * @brief implements of ProfilePanel class
 *
 * @author S.Tanaka
 * @date 2008.04.28
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "ProfilePanel.h"
#include "ChromatogramDataCanvas.h"
#include "SpectrumDataCanvas.h"
#include "PickedProfileDialog.h"
#include "MatrixOverlappingViewManager.h"

#include <wx/tglbtn.h>


using namespace kome::matrix::overlapping;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



#define GRID_GAP							3
#define BORDER_SIZE						 3

#define ID_PICK_BUTTON					331
#define ID_SHOW_PEAK_BUTTON			   332
#define ID_ALIGN_BUTTON				   333


BEGIN_EVENT_TABLE( ProfilePanel, kome::window::StandardPanel )
	EVT_TOGGLEBUTTON( ID_ALIGN_BUTTON, ProfilePanel::onAlignButton )
	EVT_TOGGLEBUTTON( ID_SHOW_PEAK_BUTTON, ProfilePanel::onPeakButton )
	EVT_BUTTON( ID_PICK_BUTTON, ProfilePanel::onPickButton )
END_EVENT_TABLE()


// constructor
ProfilePanel::ProfilePanel( wxWindow* parent, const int peakId )
		: kome::window::StandardPanel( parent, wxID_ANY ), m_peakId( peakId ) {
	// manager
	kome::ident::IdentificationManager& identMgr = kome::ident::IdentificationManager::getInstance();

	// initialize
	m_canvas = NULL;
	m_dlg = NULL;
	m_edited = false;
	m_normalized = false;
	m_chromFlg = false;

	createControls();
}

// destrucor
ProfilePanel::~ProfilePanel() {
	MatrixOverlappingViewManager& mgr = MatrixOverlappingViewManager::getInstance();
	mgr.setPanel( NULL );

	// delete dialog
	if( m_dlg != NULL ) {
		delete m_dlg;
	}
	m_dlg = NULL;
	
	// commit
	if( IsShown() && m_edited ) {
		if( kome::window::WindowTool::showYesNo( "Save the peak matrix?", "Save", "Don't save" ) ) {
			commitPeaks();
		}
		else {
			rollbackPeaks();
		}
	}
}


// set peak ID
void ProfilePanel::setPeakId( const int peakId ) {
	// check the member
	if( m_peakId == peakId ) {
		return;
	}

	// data
	std::vector< DataInfo > data;
	getData( data );

	SpectrumDataCanvas* specCanvas = dynamic_cast< SpectrumDataCanvas* >( m_canvas );
	if( specCanvas != NULL ) {
		specCanvas->setPeakId( peakId, data );
	}
	ChromatogramDataCanvas* chromCanvas = dynamic_cast< ChromatogramDataCanvas* >( m_canvas );
	if( chromCanvas != NULL ) {
		chromCanvas->setPeakId( peakId, data );
	}

	// refresh
	m_canvas->Refresh();
}

// get peak ID
int ProfilePanel::getPeakId() {
	return m_peakId;
}

// get data
void ProfilePanel::getData( std::vector< DataInfo >& data ) {
	// check the member
	if( m_peakId < 0 ) {
		return;
	}

	// manager
	kome::ident::IdentificationManager& identMgr = kome::ident::IdentificationManager::getInstance();

	// selected project
	int project = identMgr.getSelectedMatrix();

	// groups
	std::vector< int > matrixGroups;
	if( project >= 0 ) {
		identMgr.getGroups( matrixGroups, project );
	}

	// matrix samples
	std::vector< int > samples;
	for( unsigned int i = 0; i < matrixGroups.size(); i++ ) {
		identMgr.getMatrixSamplesFromGroup( samples, matrixGroups[ i ] );
	}

	// get profile
	for( unsigned int i = 0; i < samples.size(); i++ ) {
		bool chromFlg = bool();
		kome::objects::PeakElement peakElm( NULL );

		int profileId = identMgr.getProfile( m_peakId, samples[ i ] );

		if( profileId >= 0 ) {
			identMgr.getPeakInfo( m_peakId, samples[ i ], &peakElm );

			data.resize( data.size() + 1 );
			data.back().sampleId = samples[ i ];
			data.back().profileId = profileId;
			data.back().isChrom = chromFlg;
			data.back().leftX = peakElm.getLeftX();
			data.back().leftY = peakElm.getLeftY();
			data.back().rightX = peakElm.getRightX();
			data.back().rightY = peakElm.getRightY();
			data.back().name = identMgr.getMatrixSampleName( samples[ i ] );
			data.back().color = identMgr.getMatrixSampleColor( samples[ i ] );
			data.back().isChrom = identMgr.isChromatogramProfile( profileId );
		}
	}
}

// commit peaks
void ProfilePanel::commitPeaks() {
	// commit peaks
	for( unsigned int i = 0; i < m_canvas->getNumberOfSpectra(); i++ ) {
		kome::objects::Spectrum* spec = m_canvas->getSpectrum( i )->getOrgSpectrum();
		kome::plugin::PluginCallTool::commitSpecPeaks( *spec );
	}
	for( unsigned int i = 0; i < m_canvas->getNumberOfChromatograms(); i++ ) {
		kome::objects::Chromatogram* chrom = m_canvas->getChromatogram( i )->getOrgChromatogram();
		kome::plugin::PluginCallTool::commitChromPeaks( *chrom );
	}
}

// rollback peaks
void ProfilePanel::rollbackPeaks() {
	// rollback peaks
	for( unsigned int i = 0; i < m_canvas->getNumberOfSpectra(); i++ ) {
		kome::objects::Spectrum* spec = m_canvas->getSpectrum( i )->getOrgSpectrum();
		kome::plugin::PluginCallTool::rollbackSpecPeaks( *spec );
	}
	for( unsigned int i = 0; i < m_canvas->getNumberOfChromatograms(); i++ ) {
		kome::objects::Chromatogram* chrom = m_canvas->getChromatogram( i )->getOrgChromatogram();
		kome::plugin::PluginCallTool::rollbackChromPeaks( *chrom );
	}
}

// set normalized
void ProfilePanel::setNormalized() {
	// initialize
	m_normalized = false;

	// manager
	kome::ident::IdentificationManager& identMgr = kome::ident::IdentificationManager::getInstance();

	// peaks, groups
	int project = identMgr.getSelectedMatrix();
	if( project < 0 ) {
		return;
	}

	std::vector< int > peaks;
	identMgr.getMatrixPeaks( peaks, project );

	std::vector< int > matrixGroups;
	std::vector< int > samples;
	identMgr.getGroups( matrixGroups, project );
	for( unsigned int i = 0; i < matrixGroups.size(); i++ ) {
		identMgr.getMatrixSamplesFromGroup( samples, matrixGroups[ i ] );
	}

	// judge
	for( unsigned int i = 0; i < peaks.size() && !m_normalized; i++ ) {
		int peak = peaks[ i ];

		for( unsigned int j = 0; j < samples.size() && !m_normalized; j++ ) {
			int sample = samples[ j ];

			if( identMgr.getPeakValue( peak, sample ) != identMgr.getNormalizedPeakValue( peak, sample )  ) {
				m_normalized = true;
			}
		}
	}
}

// create main size
wxSizer* ProfilePanel::createMainSizer() {
	// sizer
	wxFlexGridSizer* sizer = new wxFlexGridSizer(  2, 1, GRID_GAP, GRID_GAP );
	sizer->AddGrowableCol( 0 );
	sizer->AddGrowableRow( 0 );

	// get data ID
	std::vector< DataInfo > data;
	getData( data );

	// chromatogram flag
	int specCnt = 0;
	int chromCnt = 0;
	for( unsigned int i = 0; i < data.size(); i++ ) {
		if( data[ i ].isChrom ) {
			chromCnt++;
		}
		else {
			specCnt++;
		}
	}
	m_chromFlg = ( chromCnt > specCnt );

	// canvas
	if( m_chromFlg ) {
		m_canvas = new ChromatogramDataCanvas( this, 800, 200, m_peakId, data );
	}
	else {
		m_canvas = new SpectrumDataCanvas( this, 800, 200, m_peakId, data );
	}

	sizer->Add( m_canvas, 1, wxALL | wxGROW, BORDER_SIZE );

	// buttons
	wxSizer* buttonSizer = new wxBoxSizer( wxHORIZONTAL );

	wxButton* pickButton = new wxButton( this, ID_PICK_BUTTON, wxT( "Pick" ) );
	buttonSizer->Add( pickButton, 0, wxALIGN_CENTER_VERTICAL, BORDER_SIZE );

	wxToggleButton* peaksButton = new wxToggleButton( this, ID_SHOW_PEAK_BUTTON, wxT( "Show Peak" ) );
	buttonSizer->Add( peaksButton, 0, wxALIGN_CENTER_VERTICAL, BORDER_SIZE );

	wxToggleButton* alignButton = new wxToggleButton( this, ID_ALIGN_BUTTON, wxT( "Align" ) );
	buttonSizer->Add( alignButton, 0, wxALIGN_CENTER_VERTICAL, BORDER_SIZE );

	sizer->Add( buttonSizer, 1, wxALL | wxGROW, BORDER_SIZE );

	return sizer;
}

// on pick button
void ProfilePanel::onPickButton( wxCommandEvent& evt ) {
	// manager
	kome::ident::IdentificationManager& identMgr = kome::ident::IdentificationManager::getInstance();

	// delete dialog
	if( m_dlg != NULL ) {
		delete m_dlg;
	}
	m_dlg = NULL;

	// active object
	kome::objects::Spectrum* spec = NULL;
	kome::objects::Chromatogram* chrom = NULL;

	if( m_canvas != NULL ) {
		kome::objects::MsDataVariant obj( m_canvas->getActiveObject() );
		spec = obj.getSpectrum();
		chrom = obj.getChromatogram();
	}

	if( spec == NULL && chrom == NULL ) {
		kome::window::WindowTool::showInfo( "To pick a trace, it must be selected first. Select a chromatogram / spectrum trace." );
		return;
	}

	// check the normalization
	if( m_normalized ) {
		if( kome::window::WindowTool::showYesNo(
				"Normalization has to be cleared before editing peaks.\nClear normalization?"
			)
		) {
			// reset
			wxBeginBusyCursor();

			int matrix = identMgr.getSelectedMatrix();

			std::vector< int > groups;
			std::vector< int > samples;
			std::vector< int > peaks;

			identMgr.getGroups( groups, matrix );
			for( unsigned int i = 0; i < groups.size(); i++ ) {
				identMgr.getMatrixSamplesFromGroup( samples, groups[ i ] );
			}

			identMgr.getMatrixPeaks( peaks, matrix );

			for( unsigned int i = 0; i < peaks.size(); i++ ) {
				for( unsigned int j = 0; j < samples.size(); j++ ) {
					const double intensity = identMgr.getPeakValue( peaks[ i ], samples[ j ] );
					identMgr.setNormalizedIntensity( peaks[ i ], samples[ j ], intensity );
				}
			}

			wxEndBusyCursor();

			if( m_chromFlg ) {
				( ( ChromatogramDataCanvas* )m_canvas )->updateChroms();
			}
			else {
				( ( SpectrumDataCanvas* )m_canvas )->updateSpectra();
			}

			m_normalized = false;
		}
		else {
			return;
		}
	}

	// graphics
	kome::img::Graphics& graphics = m_canvas->getGraphics();

	// data ID
	int id = -1;
	double pos = -1.0;
	double scale = 1.0;

	if( spec != NULL ) {
		kome::ident::DataSpectrum* dSpec = (kome::ident::DataSpectrum*)spec->getOrgSpectrum();
		if( dSpec->getMatrixPeakId() >= 0 && dSpec->getMatrixSampleId() >= 0 ) {
			m_dlg = new PickedProfileDialog( this, dSpec, graphics.getMinX(), graphics.getMaxX() );
		}
	}
	else {
		kome::ident::DataChromatogram* dChrom = (kome::ident::DataChromatogram*)chrom->getOrgChromatogram();
		if( dChrom->getMatrixPeakId() >= 0 && dChrom->getMatrixSampleId() >= 0 ) {
			m_dlg = new PickedProfileDialog( this, dChrom, graphics.getMinX(), graphics.getMaxX() );
		}
	}

	if( m_dlg != NULL ) {
		m_dlg->Show();
		m_edited = true;
	}
}

// on align button
void ProfilePanel::onAlignButton( wxCommandEvent& evt ) {
	// set the flag
	if( m_chromFlg ) {	// chromatogram
		ChromatogramDataCanvas* canvas = (ChromatogramDataCanvas*)m_canvas;
		canvas->setAligned( evt.IsChecked() );
	}
	else {	// spectrum
		SpectrumDataCanvas* canvas = (SpectrumDataCanvas*)m_canvas;
		canvas->setAligned( evt.IsChecked() );
	}

	// refresh
	Refresh();
}

// on peak button
void ProfilePanel::onPeakButton( wxCommandEvent& evt ) {
	// set the flag
	if( m_chromFlg ) {	// chromatogram
		ChromatogramDataCanvas* canvas = (ChromatogramDataCanvas*)m_canvas;
		canvas->setDrawingPeak( evt.IsChecked() );
	}
	else {	// spectrum
		SpectrumDataCanvas* canvas = (SpectrumDataCanvas*)m_canvas;
		canvas->setDrawingPeak( evt.IsChecked() );
	}

	// refresh
	Refresh();
}

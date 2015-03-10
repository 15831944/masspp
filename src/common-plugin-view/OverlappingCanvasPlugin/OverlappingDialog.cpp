/**
 * @file OverlappingDialog.cpp
 * @brief implements of OverlappingDialog class
 *
 * @author S.Tanaka
 * @date 2008.04.19
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "OverlappingDialog.h"
#include "OverlappingSettingsViewManager.h" // @date 2011.10.26 <Add> M.Izumi
// @date 2012.02.21 sugo add -->
#include "OverlappingSpectraCanvasEx.h"
#include "OverlappingChromatogramsCanvasEx.h"
// @date 2012.02.21 <-- sugo

#include <wx/colordlg.h>


using namespace kome::view;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



#define BORDER_SIZE						 3
#define GRID_GAP						 3

#define ID_LIST							 1
#define ID_UPDATE_BUTTON				 2
#define ID_REMOVE_BUTTON				 3
#define ID_COLOR_BUTTON					 4
#define ID_SCALE_X_TEXT_EDIT			 5
#define ID_SCALE_X_SPIN					 6
#define ID_TRANSLATE_X_TEXT_EDIT		 7
#define ID_TRANSLATE_X_SPIN				 8
#define ID_SCALE_Y_TEXT_EDIT			 9
#define ID_SCALE_Y_SPIN					10
#define ID_TRANSLATE_Y_TEXT_EDIT		11
#define ID_TRANSLATE_Y_SPIN				12
// @date 2012.02.21 sugo add BMP button ID's
#define ID_BMP_DELETE_BUTTON			13
#define ID_BMP_UP_BUTTON				14
#define ID_BMP_DOWN_BUTTON				15
#define ID_BMP_COLOR_BUTTON				16
#define ID_BMP_UPDATE_BUTTON			17
#define ID_BMP_AUTOYSCAL_CHCK			18


#define EXT_EDIT_WIDTH					75   
#define MIN_SCALE_VALUE					0.000001
#define MAX_SCALE_VALUE					1000000.0		
#define MIN_TRANSLATE_VALUE				-1000000.0
#define MAX_TRANSLATE_VALUE				1.0e+12		// @date 2011.11.25 <Mod> M.Izumi
#define SCALE_SPIN_DELTA				0.01
#define TRANSLATE_SPIN_DELTA			0.1
#define LISTBOX_HEIGHT					200
#define TEXT_EDIT_WIDTH					75

#define SPACER_SIZE						10
#define UNSELECT_MENU_ID				10000		// @date 2011.10.26 <Add> M.Izumi

// @date 2012.02.21 sugo add
#define OFFSET_SPIN_DELTA				1
#define MIN_OFFSET						0
#define MAX_OFFSET						100
#define DEFAULT_OFFSET					0
#define ANGLE_SPIN_DELTA				1
#define MIN_ANGLE						0
#define MAX_ANGLE						180
#define DEFAULT_ANGLE					90

// for context menu
#define ID_POPUP_MENU_DELETE	60003
#define ID_POPUP_MENU_COLOR		60004
#define ID_POPUP_MENU_UNSELECT	60005

#define STR_POPUP_MENU_DELETE	"Delete"
#define STR_POPUP_MENU_COLOR	"Color"
#define STR_POPUP_MENU_UNSELECT "Unselect"


BEGIN_EVENT_TABLE( OverlappingDialog, kome::window::StandardPanel )
	EVT_LISTBOX( ID_LIST, OverlappingDialog::onSelect )
	// @date 2012.02.21 sugo change Event ID -->
	EVT_BUTTON( ID_BMP_UPDATE_BUTTON, OverlappingDialog::onUpdateButton )
	EVT_BUTTON( ID_BMP_DELETE_BUTTON, OverlappingDialog::onRemoveButton )
	EVT_BUTTON( ID_BMP_COLOR_BUTTON, OverlappingDialog::onColorButton )
	// @date 2012.02.21 <-- sugo 
	EVT_SPIN_UP( ID_SCALE_X_SPIN, OverlappingDialog::onScaleXSpinUp )
	EVT_SPIN_DOWN( ID_SCALE_X_SPIN, OverlappingDialog::onScaleXSpinDown )
	EVT_SPIN_UP( ID_TRANSLATE_X_SPIN, OverlappingDialog::onTranslateXSpinUp )
	EVT_SPIN_DOWN( ID_TRANSLATE_X_SPIN, OverlappingDialog::onTranslateXSpinDown )
	// @date 2012.02.21 sugo add Event -->
	EVT_CHECKBOX( ID_BMP_AUTOYSCAL_CHCK, OverlappingDialog::onAutoYScaleCheck )
	EVT_BUTTON( ID_BMP_UP_BUTTON, OverlappingDialog::onUpButton )
	EVT_BUTTON( ID_BMP_DOWN_BUTTON, OverlappingDialog::onDownButton )
	// @date 2012.02.21 <-- sugo 

	EVT_MENU( ID_POPUP_MENU_UNSELECT, OverlappingDialog::onUnSelectAll )	

	EVT_MENU( ID_POPUP_MENU_COLOR, OverlappingDialog::onColor )		// Context Menu
	EVT_MENU( ID_POPUP_MENU_DELETE, OverlappingDialog::onDelete )	// Context Menu
	EVT_UPDATE_UI_RANGE( ID_POPUP_MENU_DELETE, ID_POPUP_MENU_COLOR, OverlappingDialog::onUpdateUIPopupMenu )// Context Menu

	EVT_CONTEXT_MENU( OverlappingDialog::onContextMenu )
END_EVENT_TABLE()


// constructor
OverlappingDialog::OverlappingDialog( wxWindow* parent )
		: StandardPanel( parent ) {
	m_canvas = NULL;
	m_list = NULL;
	m_removeButton = NULL;

	m_scXText = NULL;
	m_scXSpin = NULL;
	m_trXText = NULL;
	m_trXSpin = NULL;

	m_pOverlappingDeleteButton = NULL;
	m_pOverlappingUpButton = NULL;
	m_pOverlappingDownButton = NULL;
	m_pOverlappingColorButton = NULL;
	m_pOverlappingUpdateButton = NULL;
	m_pOverlappingAutoYScalCheckbox = NULL;

	m_bSpinDown = false;
	m_bListSelect = true;
	m_offset = DEFAULT_OFFSET;
	m_angle = DEFAULT_ANGLE;
	createControls();
}

// destructor
OverlappingDialog::~OverlappingDialog() {
}

// set canvas
void OverlappingDialog::setCanvas( kome::window::CanvasBase* canvas )
{
	m_canvas = canvas;
	m_chromFlg = ( dynamic_cast< kome::window::OverlappingChromatogramsCanvasEx* >( canvas ) != NULL );
	update();
}

// update dialog
void OverlappingDialog::update() {
	// check the member
	if( m_list == NULL ) {
		return;
	}

	// @date 2012.02.22 sugo add -->
	m_list->Freeze();
	// @date 2012.02.22 <-- sugo

	// clear list
	while( m_list->GetCount() > 0 ) {
		m_list->Delete( 0 );
	}
	m_list->DeselectAll();

	// @date 2012.02.21 sugo add -->
	int		nVal;
	bool	bSetValue = true;

	if( m_scXText->GetValue().IsEmpty() ){
		m_offset = DEFAULT_OFFSET;
	}
	else if( isint( m_scXText->GetValue().c_str(), 10, &nVal ) ){
		if( m_offset == nVal ){
			bSetValue = false;
		}
		else{
			m_offset = nVal;
		}
	}
	if( bSetValue ){
		m_offset = (m_offset < MIN_OFFSET ? MIN_OFFSET :
				   (m_offset > MAX_OFFSET ? MAX_OFFSET : m_offset));
		m_scXText->SetValue( wxT( FMT( "%d", m_offset ).c_str() ) );
	}

	bSetValue = true;
	if( m_trXText->GetValue().IsEmpty() ){
		m_angle = DEFAULT_ANGLE;
	}
	else if( isint( m_trXText->GetValue().c_str(), 10, &nVal ) ){
		if( m_angle == nVal ){
			bSetValue = false;
		}
		else{
			m_angle = nVal;
		}
	}
	if( bSetValue ){
		m_angle = (m_angle < MIN_ANGLE ? MIN_ANGLE :
				  (m_angle > MAX_ANGLE ? MAX_ANGLE : m_angle));
		m_trXText->SetValue( wxT( FMT( "%d", m_angle ).c_str() ) );
	}
	// @date 2012.02.21 <-- sugo
	
	// update
	if( m_chromFlg ) {		// chromatograms
		// canvas
		kome::window::OverlappingChromatogramsCanvasEx* chromCanvas = NULL;
		// @date 2013.09.10 <Mod> M.Izumi ->
		try {
			chromCanvas	= dynamic_cast< kome::window::OverlappingChromatogramsCanvasEx* >( m_canvas );
		}
		catch( ... ) {
		}
		// @date 2013.09.10 <Mod> M.Izumi <-


		if( chromCanvas != NULL ) {
			// @date 2012.02.21 sugo add -->
			chromCanvas->SetOffset(m_offset);
			chromCanvas->SetAngle(m_angle);
			// @date 2012.02.21 <-- sugo

			for( unsigned int i = 0; i < chromCanvas->getNumberOfChromatograms(); i++ ) {
				// chromatogram
				kome::objects::Chromatogram* chrom = chromCanvas->getChromatogram( i );

				std::string name = chrom->getTitle();
				kome::objects::DataGroupNode* group = chrom->getGroup();
				while( group != NULL ) {
					name.append( FMT( " - %s", group->getName() ) );
					group = group->getParentGroup();
				}
				
				m_list->Append( name.c_str(), chrom );
			}

			// @date 2011.10.26 <Add> M.Izumi ->
			for( unsigned int j=0; j < chromCanvas->getNumberOfChromatograms(); j++ ){

				kome::objects::Chromatogram* selChrom = chromCanvas->getSelectedChrom(j);

				for( unsigned int i=0; i < (int)m_list->GetCount(); i++ ){
					kome::operation::TransformedChromatogram* listChrom = (kome::operation::TransformedChromatogram*)m_list->GetClientData( i );
				
					if( selChrom != NULL ){
						if( listChrom->getOrgChromatogram() == selChrom->getOrgChromatogram() ){
							m_list->SetSelection( i );
						}
					}
				}
			}
			// @date 2011.10.26 <Add> M.Izumi <-
				
		}
	}
	else {  // spectra
		// canvas
		kome::window::OverlappingSpectraCanvasEx* specCanvas = NULL;
		// @date 2013.09.10 <Mod> M.Izumi ->
		try {
			specCanvas = dynamic_cast< kome::window::OverlappingSpectraCanvasEx* >( m_canvas );
		}
		catch( ... ){
		}
		// @date 2013.09.10 <Mod> M.Izumi <-

		if( specCanvas != NULL ) {
			// @date 2012.02.21 sugo add -->
			specCanvas->SetOffset(m_offset);
			specCanvas->SetAngle(m_angle);
			// @date 2012.02.21 <-- sugo

			for( unsigned int i = 0; i < specCanvas->getNumberOfSpectra(); i++ ) {
				// spectrum
				int orinum = specCanvas->GetOverlappingSpectrumNum(i);
				kome::objects::Spectrum* spec = specCanvas->getSpectrum( orinum );

				std::string name = spec->getTitle();
				kome::objects::DataGroupNode* group = spec->getGroup();
				while( group != NULL ) {
					name.append( FMT( " - %s", group->getName() ) );
					group = group->getParentGroup();
				}

				m_list->Append( name.c_str(), spec );
			}

			// @date 2011.10.26 <Add> M.Izumi ->
			for( unsigned int j=0; j < specCanvas->getNumberOfSpectra(); j++ ){
				kome::objects::Spectrum* selSpec = specCanvas->getSelectedSpec(j);

				for( unsigned int i=0; i < (int)m_list->GetCount(); i++ ){
					kome::operation::TransformedSpectrum* listSpec = (kome::operation::TransformedSpectrum*)m_list->GetClientData( i );
				
					if( selSpec != NULL ){
						if( listSpec->getOrgSpectrum() == selSpec->getOrgSpectrum() ){
							m_list->SetSelection( i );
						}
					}
				}
			}
			// @date 2011.10.26 <Add> M.Izumi <-	
		
		}
	}
	// @date 2012.02.22 sugo add -->
	m_list->Thaw();
	// @date 2012.02.22 <-- sugo

	// select
	select();
}

// select spectra or chromatograms
void OverlappingDialog::select() {
	// check values
	if( m_list == NULL || m_list->GetCount() == 0 ) {
		return;
	}

	// check values
	double scX = double();
	int scXStep = 0;
	double trX = double();
	int trXStep = 0;
	double scY = double();
	int scYStep = 0;
	double trY = double();
	int trYStep = 0;
	COLOR color		= kome::img::ColorTool::getColor(
		m_defaultColor.Red(),
		m_defaultColor.Green(),
		m_defaultColor.Blue()
	);
	int clStep = 0;

	// selected items
	std::vector< int > selectedItems;
	for( unsigned int i = 0; i < m_list->GetCount(); i++ ) {
		if( m_list->IsSelected( i ) ) {
			selectedItems.push_back( (int)i );
		}
	}

	if( m_chromFlg ) {		// chromatogram
		// canvas
		kome::window::OverlappingChromatogramsCanvasEx* chromsCanvas
			= dynamic_cast< kome::window::OverlappingChromatogramsCanvasEx* >( m_canvas );

		// unselect all
		if( selectedItems.size() > 0 && chromsCanvas != NULL ) {
			chromsCanvas->clearSelectedChroms();
		}

		// chromatograms
		for( unsigned int i = 0; i < selectedItems.size(); i++ ) {
			int item = selectedItems[ i ];
			kome::operation::TransformedChromatogram* chrom
				= dynamic_cast< kome::operation::TransformedChromatogram* >( chromsCanvas->getChromatogram( item ) );

			// add selected
			if( chromsCanvas != NULL ) {
				chromsCanvas->addSelectedChrom( chrom );
				chromsCanvas->setIndex( item );
			}
			
			if( chrom == NULL ) {
				scXStep = 2;
				trXStep = 2;
				scYStep = 2;
				trYStep = 2;
				clStep  = 2;
			}
			else {
				// x scale
				if( scXStep == 0 ) {
					scX = chrom->getXScale();
					scXStep = 1;
				}
				else if( scXStep == 1 ) {
					if( scX != chrom->getXScale() ) {
						scXStep = 2;
					}
				}

				// x translation
				if( trXStep == 0 ) {
					trX = chrom->getXTranslation();
					trXStep = 1;
				}
				else if( trXStep == 1 ) {
					if( trX != chrom->getXTranslation() ) {
						trXStep = 2;
					}
				}

				// y scale
				if( scYStep == 0 ) {
					scY = chrom->getYScale();
					scYStep = 1;
				}
				else if( scYStep == 1 ) {
					if( scY != chrom->getYScale() ) {
						scYStep = 2;
					}
				}

				// y translation
				if( trYStep == 0 ) {
					trY = chrom->getYTranslation();
					trYStep = 1;
				}
				else if( trYStep == 1 ) {
					if( trY != chrom->getYTranslation() ) {
						trYStep = 2;
					}
				}

				// color
				if( clStep == 0 ) {
					color = chromsCanvas->getColor( item );
					clStep = 1;
				}
				else if( clStep == 1 ) {
					if( color != chromsCanvas->getColor( item ) ) {
						clStep = 2;
					}
				}
			}
		}
	}
	else {  // spectra
		// canvas
		kome::window::OverlappingSpectraCanvasEx* spectraCanvas
			= dynamic_cast< kome::window::OverlappingSpectraCanvasEx* >( m_canvas );

		// unselect all
		if( selectedItems.size() > 0 && spectraCanvas != NULL ) {
			spectraCanvas->clearSelectedSpectra();
		}

		// spectra
		for( unsigned int i = 0; i < selectedItems.size(); i++ ) {
			int item = selectedItems[ i ];
			int orinum = spectraCanvas->GetOverlappingSpectrumNum(item);
			kome::operation::TransformedSpectrum* spec
				= dynamic_cast< kome::operation::TransformedSpectrum* >( spectraCanvas->getSpectrum( orinum ) );

			// add selected
			if( spectraCanvas != NULL ) {
				spectraCanvas->addSelectedSpec( spec );
				spectraCanvas->setIndex( item );
			}
						
			if( spec == NULL ) {
				scXStep = 2;
				trXStep = 2;
				scYStep = 2;
				trYStep = 2;
				clStep  = 2;
			}
			else {
				// x scale
				if( scXStep == 0 ) {
					scX = spec->getXScale();
					scXStep = 1;
				}
				else if( scXStep == 1 ) {
					if( scX != spec->getXScale() ) {
						scXStep = 2;
					}
				}

				// x translation
				if( trXStep == 0 ) {
					trX = spec->getXTranslation();
					trXStep = 1;
				}
				else if( trXStep == 1 ) {
					if( trX != spec->getXTranslation() ) {
							trXStep = 2;
					}
				}

				// y scale
				if( scYStep == 0 ) {
					scY = spec->getYScale();
					scYStep = 1;
				 }
				else if( scYStep == 1 ) {
					if( scY != spec->getYScale() ) {
							scYStep = 2;
					}
				}

				// y translation
				if( trYStep == 0 ) {
					trY = spec->getYTranslation();
					trYStep = 1;
				 }
				else if( trYStep == 1 ) {
					if( trY != spec->getYTranslation() ) {
						trYStep = 2;
					}
				}

				// color
				if( clStep == 0 ) {
				color = spectraCanvas->getColor( item );
					clStep = 1;
				}
				else if( clStep == 1 ) {
					if( color != spectraCanvas->getColor( item ) ) {
						clStep = 2;
					}
				}
			}
		}
	}

	if( clStep == 1 ) {
		wxColour c(
			kome::img::ColorTool::getRed( color ),
			kome::img::ColorTool::getGreen( color ),
			kome::img::ColorTool::getBlue( color )
		);
	
	}
	
	setButtonStatus(); // @date 2011.10.26 <Add> M.Izumi

	// refresh
	kome::window::WindowTool::refresh();
}

// transform
bool OverlappingDialog::transform() {
	// scale x
	int scX = 1; // @date 2012.02.21 sugo Change
	bool scXFlg = false;
	if( !checkScaling( m_scXText, &scX, &scXFlg ) ) {
		return false;
	}
	if( scXFlg ) {
		m_scXSpin->SetValue( scX ); // @date 2012.02.21 sugo Change
	}

	// translate x
	int trX = 0; // @date 2012.02.21 sugo Change
	bool trXFlg = false;
	if( !checkTranslation( m_trXText, &trX, &trXFlg ) ) {
		return false;
	}
	if( trXFlg ) {
		m_trXSpin->SetValue( trX ); // @date 2012.02.21 sugo Change
	}

	// selected items
	std::vector< int > selectedItems;
	for( unsigned int i = 0; i < m_list->GetCount(); i++ ) {
		if( m_list->IsSelected( i ) ) {
			selectedItems.push_back( (int)i );
		}
	}
	
	// transform
	for( unsigned int i = 0; i < selectedItems.size(); i++ ) {
		int item = selectedItems[ i ];
		if( m_chromFlg ) {  // chromatogram
			kome::operation::TransformedChromatogram* chrom
				= (kome::operation::TransformedChromatogram*)m_list->GetClientData( item );

			// update
			kome::plugin::PluginCallTool::onUpdateChromatogram( *chrom );
		}
		else {		// spectrum
			kome::operation::TransformedSpectrum* spec
				= (kome::operation::TransformedSpectrum*)m_list->GetClientData( item );
	
			// update
			kome::plugin::PluginCallTool::onUpdateSpectrum( *spec );
		}
	}

	// update canvas
	if( m_chromFlg ) {
		kome::window::OverlappingChromatogramsCanvasEx* chromsCanvas = (kome::window::OverlappingChromatogramsCanvasEx*)m_canvas;
		if( chromsCanvas != NULL ){
			// @date 2012.02.24 sugo add -->
			chromsCanvas->SetOffset( scX );
			chromsCanvas->SetAngle( trX );
			// @date 2012.02.24 <-- sugo 
			chromsCanvas->update();
		}
	}
	else {
		kome::window::OverlappingSpectraCanvasEx* spectraCanvas = (kome::window::OverlappingSpectraCanvasEx*)m_canvas;
		if( spectraCanvas != NULL ){
			// @date 2012.02.21 sugo add -->
			spectraCanvas->SetOffset( scX );
			spectraCanvas->SetAngle( trX );
			// @date 2012.02.21 <-- sugo 
			spectraCanvas->update();
		}
	}

	// select
	for( unsigned int i = 0; i < selectedItems.size(); i++ ) {
		m_list->SetSelection( selectedItems[ i ] );
	}
	
	return true;
}

// check scaling
bool OverlappingDialog::checkScaling( wxTextCtrl* text, int* val, bool* flg ) {
	// check the member
	if( text == NULL ) {
		return false;
	}

	// get value
	int sc = 1;
	if( isint( text->GetValue().c_str(),10, &sc ) ) {
		if( sc < MIN_OFFSET || sc > MAX_OFFSET ) {	// out of range
			std::string msg = FMT(
				"Offset must be a number between %d and %d.",
				MIN_OFFSET,
				MAX_OFFSET
			);

			kome::window::WindowTool::showInfo( msg.c_str() );
			return false;
		}
		else {		// normal value
			*flg = true;
			*val = sc;
		}
	}
	else {

		std::string msg = "Offset must be a numeric value.";

		kome::window::WindowTool::showInfo( msg.c_str() );
		return false;
	}
	// @date 2012.02.21 <-- sugo 

	return true;
}

// check translation
bool OverlappingDialog::checkTranslation( wxTextCtrl* text, int* val, bool* flg ) {
	// check the member
	if( text == NULL ) {
		return false;
	}

	// get value
	int tr = 1;
	if( isint( text->GetValue().c_str(),10, &tr ) ) {
		if( tr < MIN_ANGLE || tr > MAX_ANGLE ) {	// out of range
			std::string msg = FMT(
				"Angle must be a number between %d and %d.",
				MIN_ANGLE,
				MAX_ANGLE
			);

			kome::window::WindowTool::showInfo( msg.c_str() );
			return false;
		}
		else {		// normal value
			*flg = true;
			*val = tr;
		}
	}
	else {

		std::string msg = "Angle must be a numeric value.";

		kome::window::WindowTool::showInfo( msg.c_str() );
		return false;
	}

	return true;
}

// edit sizer
wxSizer* OverlappingDialog::createEditSizer() {

	// @date 2012.02.21 sugo change normal button to BMP button
	wxBoxSizer* SamplesSizer = new wxBoxSizer( wxVERTICAL );

	// Add buttons
	wxBoxSizer* pButtonSizer = new wxBoxSizer( wxHORIZONTAL);

	kome::window::IconManager& iconMgr = kome::window::IconManager::getInstance();

	// delete button
	m_pOverlappingDeleteButton = new wxBitmapButton( this, ID_BMP_DELETE_BUTTON, *iconMgr.getIcon("overlapping_del") );
	m_pOverlappingDeleteButton->SetToolTip("Remove trace");
	m_pOverlappingDeleteButton->SetBitmapDisabled(*iconMgr.getIcon("overlapping_dis_del"));
	m_pOverlappingDeleteButton->Enable( false );
	pButtonSizer->Add( m_pOverlappingDeleteButton, 0, wxALIGN_CENTRE_VERTICAL | wxALL, BORDER_SIZE );

	// up button
	m_pOverlappingUpButton = new wxBitmapButton( this, ID_BMP_UP_BUTTON, *iconMgr.getIcon("overlapping_up") );
	m_pOverlappingUpButton->SetToolTip("Move trace up");
	m_pOverlappingUpButton->SetBitmapDisabled(*iconMgr.getIcon("overlapping_dis_up"));
	m_pOverlappingUpButton->Enable( false );
	pButtonSizer->Add( m_pOverlappingUpButton, 0, wxALIGN_CENTRE_VERTICAL | wxALL, BORDER_SIZE );

	// down button
	m_pOverlappingDownButton = new wxBitmapButton( this, ID_BMP_DOWN_BUTTON, *iconMgr.getIcon("overlapping_dwn") );
	m_pOverlappingDownButton->SetToolTip("Move trace down");
	m_pOverlappingDownButton->SetBitmapDisabled(*iconMgr.getIcon("overlapping_dis_dwn"));
	m_pOverlappingDownButton->Enable( false ); 
	pButtonSizer->Add( m_pOverlappingDownButton, 0, wxALIGN_CENTRE_VERTICAL | wxALL, BORDER_SIZE );

	// color button
	m_pOverlappingColorButton = new wxBitmapButton( this, ID_BMP_COLOR_BUTTON, *iconMgr.getIcon("overlapping_color") );
	m_pOverlappingColorButton->SetToolTip("Change line color");
	m_pOverlappingColorButton->SetBitmapDisabled(*iconMgr.getIcon("overlapping_dis_color"));
	m_pOverlappingColorButton->Enable( false ); 
	pButtonSizer->Add( m_pOverlappingColorButton, 0, wxALIGN_CENTRE_VERTICAL | wxALL, BORDER_SIZE );

	SamplesSizer->Add( pButtonSizer, 0, (wxALL & ~wxLEFT) | wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL, BORDER_SIZE );

	return SamplesSizer;
}

// transform sizer
wxSizer* OverlappingDialog::createTransformSizer() {
    // create sizer
	// @date 2012.02.20 Sugo change parameter rows number form 2 to 1
	wxFlexGridSizer* sizer = new wxFlexGridSizer( 4, 1, GRID_GAP, GRID_GAP );	// SPEC No.85568 @date 2012.04.18<Mod> M.Izumi

	sizer->AddGrowableCol( 1 );
	sizer->AddGrowableCol( 3 );

	// Scale X
	sizer->Add(
		// @date 2012.02.21 sugo change
		new wxStaticText( this, wxID_ANY, wxT( "Offset(%)" ) ),
		0,
		wxALIGN_CENTER_VERTICAL
	);

	wxSizer* scXSizer = new wxBoxSizer( wxHORIZONTAL );
	sizer->Add( scXSizer, 0, wxGROW | wxALL );
	m_scXText = new kome::window::NumberTextCtrl(
		this,
		ID_SCALE_X_TEXT_EDIT,
		// @date 2012.02.21 sugo change
		true,
		false
	);
	
	m_scXText->SetValue( FMT("%d",DEFAULT_OFFSET) );
	scXSizer->Add( m_scXText, 0, wxALIGN_CENTER_VERTICAL, BORDER_SIZE );

	m_scXSpin = new wxSpinButton(
		this,
		ID_SCALE_X_SPIN,
		wxDefaultPosition,
		wxSize( -1, m_scXText->GetBestSize().GetHeight() ),
		wxSP_VERTICAL | wxSP_ARROW_KEYS
	);

	m_scXSpin->SetRange(	
		MIN_OFFSET, MAX_OFFSET
	);
	
	scXSizer->Add( m_scXSpin, 0, wxALIGN_CENTER_VERTICAL, BORDER_SIZE );
	scXSizer->AddSpacer( SPACER_SIZE );

	// Translate X
	sizer->Add(
		// @date 2012.02.21 sugo change
		new wxStaticText( this, wxID_ANY, wxT( "Angle" ) ),
		0,
		wxALIGN_CENTER_VERTICAL
	);
	wxSizer* trXSizer = new wxBoxSizer( wxHORIZONTAL );
	sizer->Add( trXSizer, 0, wxGROW | wxALL );

	m_trXText = new kome::window::NumberTextCtrl(
		this,
		ID_TRANSLATE_X_TEXT_EDIT,
		// @date 2012.02.21 sugo change
		true,
		false
	);
	
	m_trXText->SetValue( FMT("%d", DEFAULT_ANGLE ) );
	trXSizer->Add( m_trXText, 0, wxALIGN_CENTER_VERTICAL, BORDER_SIZE );

	m_trXSpin = new wxSpinButton(
		this,
		ID_TRANSLATE_X_SPIN,
		wxDefaultPosition,
		wxSize( -1, m_trXText->GetBestSize().GetHeight() ),
		wxSP_VERTICAL | wxSP_ARROW_KEYS
	);
	
	m_trXSpin->SetRange(
		// @date 2012.02.21 sugo change
		MIN_ANGLE, MAX_ANGLE
	);
	
	
	trXSizer->Add( m_trXSpin, 0, wxALIGN_CENTER_VERTICAL, BORDER_SIZE );

	return sizer;
}

// @date 2012.02.21 sugo add createUpdateSizer
// Auto Y Scaling and Update
wxSizer* OverlappingDialog::createUpdateSizer() {

	// Auto Y Scaling
	wxBoxSizer* pButtonSizer = new wxBoxSizer( wxHORIZONTAL);
	// for justifing x position
	pButtonSizer->Add(
		new wxStaticText( this, wxID_ANY, wxT( " " ) ),
		0,
		wxALIGN_CENTER_VERTICAL
	);

	// label
	pButtonSizer->Add(
		new wxStaticText( this, wxID_ANY, wxT( "Auto Y Scaling" ) ),
		0,
		wxALIGN_CENTER_VERTICAL
	);

	// for justifing x position
	pButtonSizer->Add(
		new wxStaticText( this, wxID_ANY, wxT( " " ) ),
		0,
		wxALIGN_CENTER_VERTICAL
	);

	// check box
	m_pOverlappingAutoYScalCheckbox = new wxCheckBox(
		this,
		ID_BMP_AUTOYSCAL_CHCK,
		wxT( "" )
	);

	pButtonSizer->Add( m_pOverlappingAutoYScalCheckbox, 0, wxALIGN_CENTER_VERTICAL , BORDER_SIZE );

	// for justifing x position
	// must set 1 to second parameter of function Add() 
	pButtonSizer->Add(
		new wxStaticText( this, wxID_ANY, wxT( "" ) ),
		1,
		wxALIGN_CENTER_VERTICAL
	);

	// update button
	kome::window::IconManager& iconMgr = kome::window::IconManager::getInstance();
	// UpボタンをUpdateボタンに変更 BitmapがないのでwxButtonに変更 @date 2012.04.16 M.Izumi
	m_pOverlappingUpdateButton = new wxButton( this, ID_BMP_UPDATE_BUTTON, wxT( "Update" ) );
	m_pOverlappingUpdateButton->SetToolTip("Update");
	pButtonSizer->Add( m_pOverlappingUpdateButton, 0, (wxALL & ~wxLEFT) | wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL, BORDER_SIZE );

	return pButtonSizer;
}

// creates main sizer
wxSizer* OverlappingDialog::createMainSizer() {
	// main sizer
	wxFlexGridSizer* sizer = new wxFlexGridSizer( 4, 1, GRID_GAP, GRID_GAP );

	// @date 2012.02.21 sugo add createUpdateSizer
	wxSizer* UpdateSizer = createUpdateSizer();
	sizer->Add( UpdateSizer, 0, (wxALL & ~wxLEFT) | wxGROW | wxALIGN_CENTER_VERTICAL, BORDER_SIZE );

	// @date 2012.02.20 Sugo Replace the Position of "transform sizer" and "list box"
	// transform sizer
	wxSizer* transformSizer = createTransformSizer();
	sizer->Add( transformSizer, 1, wxALL | wxGROW, BORDER_SIZE );
	// @date 2012.02.21 sugo change
	sizer->AddGrowableRow( 2 );
	sizer->AddGrowableCol( 0 );

	// list box
	m_list = new wxListBox( this, ID_LIST, wxDefaultPosition, wxDefaultSize );	// SPEC No.85568 @date 2012.04.18 <Mod> M.Izumi
	sizer->Add( m_list, 1, wxALL | wxGROW, BORDER_SIZE );

	// edit sizer
	wxSizer* editSizer = createEditSizer();
	// @date 2012.02.21 sugo change
	sizer->Add( editSizer, 2, (wxALL & ~wxLEFT) | wxGROW | wxALIGN_CENTER_VERTICAL, BORDER_SIZE );


	// update
	update();

	return sizer;
}

// on select
void OverlappingDialog::onSelect( wxCommandEvent& evt ) {
	select();
}

// on update
void OverlappingDialog::onUpdateButton( wxCommandEvent& evt ) {
	// @date 2011.12.01 <Mod> M.Izumi
	if( transform() ){
		update();
	}
	// @date 2012.02.21 sugo add -->
	else{
		m_offset = (m_offset < MIN_OFFSET ? MIN_OFFSET :
				   (m_offset > MAX_OFFSET ? MAX_OFFSET : m_offset));
		m_scXText->SetValue( wxT( FMT( "%d", m_offset ).c_str() ) );
		m_angle = (m_angle < MIN_ANGLE ? MIN_ANGLE :
				  (m_angle > MAX_ANGLE ? MAX_ANGLE : m_angle));
		m_trXText->SetValue( wxT( FMT( "%d", m_angle ).c_str() ) );
	}
	// @date 2012.02.21 <-- sugo
}

// on remove
void OverlappingDialog::onRemoveButton( wxCommandEvent& evt ) {

	onRemoveButton();
}

// on remove
void OverlappingDialog::onRemoveButton( void ) {
	// selected items
	std::vector< int > selectedItems;
	for( unsigned int i = 0; i < m_list->GetCount(); i++ ) {
		if( m_list->IsSelected( i ) ) {
			selectedItems.push_back( (int)i );
		}
	}

	// remove
	for( int i = (int)selectedItems.size() - 1; i >= 0; i-- ) {
		int item = selectedItems[ i ];

		if( m_chromFlg ) {		// chromatogram
			kome::objects::Chromatogram* chrom = (kome::objects::Chromatogram*)m_list->GetClientData( item );
			kome::window::OverlappingChromatogramsCanvasEx* chromsCanvas = (kome::window::OverlappingChromatogramsCanvasEx*)m_canvas;
			if( chromsCanvas != NULL ){
				chromsCanvas->removeChromatogram( chrom );
			}
		}
		else {  // spectrum
			kome::objects::Spectrum* spec = (kome::objects::Spectrum*)m_list->GetClientData( item );
			kome::window::OverlappingSpectraCanvasEx* spectraCanvas = (kome::window::OverlappingSpectraCanvasEx*)m_canvas;
			if( spectraCanvas != NULL ){
				spectraCanvas->removeSpectrum( spec );
			}
		}
	}

	// deselect
	m_list->DeselectAll();
	select();	
}

// on color
void OverlappingDialog::onColorButton( wxCommandEvent& evt ) {
	// color dialog
	wxColourDialog dialog( this );
	if( dialog.ShowModal() != wxID_OK ) {
		return;
	}

	// get color
	wxColour& color = dialog.GetColourData().GetColour();

	COLOR c = kome::img::ColorTool::getColor( color.Red(), color.Green(), color.Blue() );

	// selected items
	std::vector< int > selectedItems;
	for( unsigned int i = 0; i < m_list->GetCount(); i++ ) {
		if( m_list->IsSelected( i ) ) {
			selectedItems.push_back( (int)i );
		}
	}

	// change color
	for( unsigned int i = 0; i < selectedItems.size(); i++ ) {
		int item = selectedItems[ i ];

		if( m_chromFlg ) {		// chromatogram
			kome::objects::Chromatogram* chrom = (kome::objects::Chromatogram*)m_list->GetClientData( item );
			kome::window::OverlappingChromatogramsCanvasEx* chromsCanvas = (kome::window::OverlappingChromatogramsCanvasEx*)m_canvas;
			if( chromsCanvas != NULL ){
				chromsCanvas->setColor( chrom, c );
			}
		}
		else {  // spectrum
			kome::objects::Spectrum* spec = (kome::objects::Spectrum*)m_list->GetClientData( item );
			kome::window::OverlappingSpectraCanvasEx* spectraCanvas = (kome::window::OverlappingSpectraCanvasEx*)m_canvas;
			if( spectraCanvas != NULL ){
				spectraCanvas->setColor( spec, c );
			}
		}
	}

	// update canvas
	if( m_chromFlg ) {
		kome::window::OverlappingChromatogramsCanvasEx* chromsCanvas = (kome::window::OverlappingChromatogramsCanvasEx*)m_canvas;
		if( chromsCanvas != NULL ){
			chromsCanvas->update();
		}
	}
	else {
		kome::window::OverlappingSpectraCanvasEx* spectraCanvas = (kome::window::OverlappingSpectraCanvasEx*)m_canvas;
		if( spectraCanvas != NULL ){
			spectraCanvas->update();
		}
	}
}

// @date 2011.11.25 <Mod> M.Izumi ->
// on scale x spin up
void OverlappingDialog::onScaleXSpinUp( wxSpinEvent& evt ) {
	
	if( m_bSpinDown ){
		return;
	}
	m_bSpinDown = true;

	int		nVal;
	if( isint( m_scXText->GetValue().c_str(), 10, &nVal ) ){
		//2012/03/07 upd by t.okuno -> check spin range
		m_offset = (nVal < MIN_OFFSET ? MIN_OFFSET :
				   (nVal > MAX_OFFSET ? MAX_OFFSET : nVal));
		//2012/03/07 upd by t.okuno <- check spin range
	}
	m_offset = m_offset + OFFSET_SPIN_DELTA;
	//2012/03/07 add by t.okuno -> check spin range
	m_offset = (m_offset < MIN_OFFSET ? MIN_OFFSET :
			   (m_offset > MAX_OFFSET ? MAX_OFFSET : m_offset));
	//2012/03/07 add by t.okuno <- check spin range

	m_scXText->SetValue( wxT( FMT( "%d", m_offset ).c_str() ) );

	// transform
	if( !transform() ){
		m_offset = (m_offset < MIN_OFFSET ? MIN_OFFSET :
				   (m_offset > MAX_OFFSET ? MAX_OFFSET : m_offset));
		m_scXText->SetValue( wxT( FMT( "%d", m_offset ).c_str() ) );
	}

	m_bSpinDown = false;
	// @date 2012.02.21 <-- sugo 
}

// on scale x spin down
void OverlappingDialog::onScaleXSpinDown( wxSpinEvent& evt ) {

	if( m_bSpinDown ){
		return;
	}
	m_bSpinDown = true;

	int		nVal;
	if( isint( m_scXText->GetValue().c_str(), 10, &nVal ) ){
		m_offset = nVal;
		//2012/03/07 upd by t.okuno -> check spin range
		m_offset = (nVal < MIN_OFFSET ? MIN_OFFSET :
				   (nVal > MAX_OFFSET ? MAX_OFFSET : nVal));
		//2012/03/07 upd by t.okuno <- check spin range
	}
	m_offset = m_offset - OFFSET_SPIN_DELTA;
	//2012/03/07 add by t.okuno -> check spin range
	m_offset = (m_offset < MIN_OFFSET ? MIN_OFFSET :
			   (m_offset > MAX_OFFSET ? MAX_OFFSET : m_offset));
	//2012/03/07 add by t.okuno <- check spin range

	m_scXText->SetValue( wxT( FMT( "%d", m_offset ).c_str() ) );

	// transform
	if( !transform() ){
		m_offset = (m_offset < MIN_OFFSET ? MIN_OFFSET :
				   (m_offset > MAX_OFFSET ? MAX_OFFSET : m_offset));
		m_scXText->SetValue( wxT( FMT( "%d", m_offset ).c_str() ) );
	}

	m_bSpinDown = false;
	// @date 2012.02.21 <-- sugo 
}

// on translate x spin up
void OverlappingDialog::onTranslateXSpinUp( wxSpinEvent& evt ){
	// get value

	if( m_bSpinDown ){
		return;
	}
	m_bSpinDown = true;

	int		nVal;
	if( isint( m_trXText->GetValue().c_str(), 10, &nVal ) ){
		//2012/03/07 upd by t.okuno -> check spin range
		m_angle = (nVal < MIN_ANGLE ? MIN_ANGLE :
				  (nVal > MAX_ANGLE ? MAX_ANGLE : nVal));
		//2012/03/07 upd by t.okuno <- check spin range
	}
	m_angle = m_angle + ANGLE_SPIN_DELTA;
	//2012/03/07 add by t.okuno -> check spin range
	m_angle = (m_angle < MIN_ANGLE ? MIN_ANGLE :
			  (m_angle > MAX_ANGLE ? MAX_ANGLE : m_angle));
	//2012/03/07 add by t.okuno <- check spin range

	m_trXText->SetValue( wxT( FMT( "%d", m_angle ).c_str() ) );

	// transform
	if( !transform() ){
		m_angle = (m_angle < MIN_ANGLE ? MIN_ANGLE :
				  (m_angle > MAX_ANGLE ? MAX_ANGLE : m_angle));
		m_trXText->SetValue( wxT( FMT( "%d", m_angle ).c_str() ) );
	}

	m_bSpinDown = false;
	// @date 2012.02.21 <-- sugo 
}

// on translate x spin down
void OverlappingDialog::onTranslateXSpinDown( wxSpinEvent& evt ) {
	// @date 2012.02.21 sugo Change -->

	if( m_bSpinDown ){
		return;
	}
	m_bSpinDown = true;

	int		nVal;
	if( isint( m_trXText->GetValue().c_str(), 10, &nVal ) ){
		//2012/03/07 upd by t.okuno -> check spin range
		m_angle = (nVal < MIN_ANGLE ? MIN_ANGLE :
				  (nVal > MAX_ANGLE ? MAX_ANGLE : nVal));
		//2012/03/07 upd by t.okuno <- check spin range
	}
	m_angle = m_angle - ANGLE_SPIN_DELTA;
	//2012/03/07 add by t.okuno -> check spin range
	m_angle = (m_angle < MIN_ANGLE ? MIN_ANGLE :
			  (m_angle > MAX_ANGLE ? MAX_ANGLE : m_angle));
	//2012/03/07 add by t.okuno <- check spin range

	m_trXText->SetValue( wxT( FMT( "%d", m_angle ).c_str() ) );

	// transform
	if( !transform() ){
		m_angle = (m_angle < MIN_ANGLE ? MIN_ANGLE :
				  (m_angle > MAX_ANGLE ? MAX_ANGLE : m_angle));
		m_trXText->SetValue( wxT( FMT( "%d", m_angle ).c_str() ) );
	}

	m_bSpinDown = false;
	// @date 2012.02.21 <-- sugo 
}

// @date 2012.02.21 sugo add -->
// 'Auto Y Scale' check box is clicked
void OverlappingDialog::onAutoYScaleCheck( wxCommandEvent& evt ) {
	if( m_pOverlappingAutoYScalCheckbox == NULL ){
		return;
	}

	// reconstruction of specCanvas
	if( m_chromFlg ) {		// chromatogram
		// in function update() m_list is deleted and reconstructed base on chromCanvas
		kome::window::OverlappingChromatogramsCanvasEx* chromCanvas
			= dynamic_cast< kome::window::OverlappingChromatogramsCanvasEx* >( m_canvas );
		if( chromCanvas != NULL ){
			chromCanvas->SetAutoYScale( (m_pOverlappingAutoYScalCheckbox->IsChecked() ? true : false) );
			chromCanvas->update();
		}
	}
	else {  // spectrum
		// in function update() m_list is deleted and reconstructed base on specCanvas
		kome::window::OverlappingSpectraCanvasEx* specCanvas
			= dynamic_cast< kome::window::OverlappingSpectraCanvasEx* >( m_canvas );
		if( specCanvas != NULL ){
			specCanvas->SetAutoYScale( (m_pOverlappingAutoYScalCheckbox->IsChecked() ? true : false) );
			specCanvas->update();
		}
	}
}

// the up(BMP) button is clicked
void OverlappingDialog::onUpButton( wxCommandEvent& evt ) {
	if( m_list == NULL ){
		return;
	}

	int itemnum = m_list->GetSelection();
	if(itemnum != 0) {
		void *clientdata;
		wxString temp = m_list->GetString(itemnum);
		clientdata = m_list->GetClientData(itemnum);
		m_list->Freeze();
		m_list->InsertItems(1,&temp,itemnum-1);
		m_list->Delete(itemnum+1);
		m_list->SetClientData(itemnum-1,clientdata);
		m_list->SetSelection( itemnum -1 );
		m_list->Thaw();

		// button state change
		setButtonStatus();

		// reconstruction of specCanvas
		if( m_chromFlg ) {		// chromatogram
			// in function update() m_list is deleted and reconstructed base on chromCanvas
			kome::window::OverlappingChromatogramsCanvasEx* chromCanvas
				= dynamic_cast< kome::window::OverlappingChromatogramsCanvasEx* >( m_canvas );
			if( chromCanvas != NULL ){
				chromCanvas->exchange(itemnum,itemnum-1);
				chromCanvas->update();
			}
		}
		else {  // spectrum
			// in function update() m_list is deleted and reconstructed base on specCanvas
			kome::window::OverlappingSpectraCanvasEx* specCanvas
				= dynamic_cast< kome::window::OverlappingSpectraCanvasEx* >( m_canvas );
			if( specCanvas != NULL ){
				specCanvas->exchange(itemnum,itemnum-1);
				specCanvas->update();
			}
		}
	}
}

// the down(BMP) button is clicked
void OverlappingDialog::onDownButton( wxCommandEvent& evt ) {
	if( m_list == NULL ){
		return;
	}

	int itemnum = m_list->GetSelection();
	if(itemnum != (int)(m_list->GetCount() - 1)) {
		void *clientdata;
		wxString temp = m_list->GetString(itemnum);
		clientdata = m_list->GetClientData(itemnum);
		m_list->Freeze();
		m_list->InsertItems(1,&temp,itemnum+2);
		m_list->Delete(itemnum);
		m_list->SetClientData(itemnum+1,clientdata);
		m_list->SetSelection( itemnum + 1 );
		m_list->Thaw();

		// button state change
		setButtonStatus();

		// reconstruction of specCanvas
		if( m_chromFlg ) {		// chromatogram
			// in function update() m_list is deleted and reconstructed base on chromCanvas
			kome::window::OverlappingChromatogramsCanvasEx* chromCanvas
				= dynamic_cast< kome::window::OverlappingChromatogramsCanvasEx* >( m_canvas );
			if( chromCanvas != NULL ){
				chromCanvas->exchange(itemnum,itemnum+1);
				chromCanvas->update();
			}
		}
		else {  // spectrum
			// in function update() m_list is deleted and reconstructed base on specCanvas
			kome::window::OverlappingSpectraCanvasEx* specCanvas
				= dynamic_cast< kome::window::OverlappingSpectraCanvasEx* >( m_canvas );
			if( specCanvas != NULL ){
				specCanvas->exchange(itemnum,itemnum+1);
				specCanvas->update();
			}
		}
	}
}
// @date 2012.02.21 <-- sugo


// @date 2011.10.26 <Add> M.Izumi ->
//	キャンバスで選択されたスペクトルをリストに反映				
void OverlappingDialog::SelectSpecOfCanvas( kome::objects::Spectrum* spec ){
	OverlappingSettingsViewManager& mgr = OverlappingSettingsViewManager::getInstance();
	// overlapping spectra canvas
	kome::window::OverlappingSpectraCanvasEx* specCanvas	=
		dynamic_cast< kome::window::OverlappingSpectraCanvasEx* >( mgr.getCurrentCanvas() );
    
	if( m_list != NULL ) {
		m_list->Freeze(); // @date 2012.02.22 sugo add

		// unselect all
		m_list->DeselectAll();
	
		if( spec != NULL ){
			for( unsigned int i=0; i< m_list->GetCount(); i++ ){
				kome::operation::TransformedSpectrum* listSpec = (kome::operation::TransformedSpectrum*)m_list->GetClientData( i );

				if( spec->getOrgSpectrum() == listSpec->getOrgSpectrum() ){
					m_list->SetSelection( i );
				}
			}
		}
		m_list->Thaw(); // @date 2012.02.22 sugo add
	}
	select();
}

//	キャンバスで選択されたクロマトグラムをリストに反映				
void OverlappingDialog::SelectChromOfCanvas( kome::objects::Chromatogram* chrom ){
	OverlappingSettingsViewManager& mgr = OverlappingSettingsViewManager::getInstance();
	// overlapping chrom canvas
	kome::window::OverlappingChromatogramsCanvasEx* chromCanvas = 
		dynamic_cast< kome::window::OverlappingChromatogramsCanvasEx* >( mgr.getCurrentCanvas() );

	if( m_list != NULL ) {
		
		m_list->Freeze(); // @date 2012.02.22 sugo add 
		
		// unselect all
		m_list->DeselectAll();
	
		if( chrom != NULL ){
			for( unsigned int i=0; i< m_list->GetCount(); i++ ){
				kome::operation::TransformedChromatogram* listChrom = (kome::operation::TransformedChromatogram*)m_list->GetClientData( i );

				if( chrom->getOrgChromatogram() == listChrom->getOrgChromatogram() ){
					m_list->SetSelection( i );
				}
			}
		}
		m_list->Thaw(); // @date 2012.02.22 sugo add
	}
	select();
}

// ボタンの状態切替		
void OverlappingDialog::setButtonStatus(){
	int i;
	int nSelect = -1;
	int nListCount = (int)m_list->GetCount();
	bool bflg = false;

	for( i=0; i < nListCount; i++ ){
		if( m_list->IsSelected(i) ){
			bflg = true;
			nSelect = i;
			break;
		}
	}
	
	if( bflg ){
		if( bflg != m_bListSelect ){
			m_pOverlappingDeleteButton->Enable( true );
			m_pOverlappingColorButton->Enable( true );
		}

		if( nSelect == 0 ){
			if( m_pOverlappingUpButton->IsEnabled() ){
				m_pOverlappingUpButton->Enable( false );
			}
		}
		else if( m_pOverlappingUpButton->IsEnabled() == false ){
			m_pOverlappingUpButton->Enable( true );
		}
		if( nSelect == (nListCount - 1) ){
			if( m_pOverlappingDownButton->IsEnabled() ){
				m_pOverlappingDownButton->Enable( false );
			}
		}
		else if( m_pOverlappingDownButton->IsEnabled() == false ){
			m_pOverlappingDownButton->Enable( true );
		}
	}
	else if( bflg != m_bListSelect ){
		m_pOverlappingDeleteButton->Enable( false );
		m_pOverlappingColorButton->Enable( false );
		m_pOverlappingUpButton->Enable( false );
		m_pOverlappingDownButton->Enable( false );
	}

	m_bListSelect = bflg;
}

// unselect all ( list )
void OverlappingDialog::onUnSelectAll( wxCommandEvent& evt ){
	// check the member
	if( m_list != NULL ) {
		OverlappingSettingsViewManager& mgr = OverlappingSettingsViewManager::getInstance();

		if( m_chromFlg ){	// chromatogram
			// overlapping chrom canvas
			kome::window::OverlappingChromatogramsCanvasEx* chromCanvas = 
				dynamic_cast< kome::window::OverlappingChromatogramsCanvasEx* >( mgr.getCurrentCanvas() );
			if( chromCanvas != NULL ){
				chromCanvas->clearSelectedChroms();
			}
		
		}else{				// spectrum
			
			// overlapping spectra canvas
			kome::window::OverlappingSpectraCanvasEx* specCanvas	=
				dynamic_cast< kome::window::OverlappingSpectraCanvasEx* >( mgr.getCurrentCanvas() );
			if( specCanvas != NULL ){
				specCanvas->clearSelectedSpectra();
			}
		}
		// unselect all
		m_list->DeselectAll();
		select();
	}
	
}

// Delete ( list )
void OverlappingDialog::onDelete( wxCommandEvent& evt ){

	// 項目が選択されているか?
	bool flgSelected = false;
	// selected items
	std::vector< int > selectedItems;
	for( unsigned int i = 0; i < m_list->GetCount(); i++ ) {
		if( m_list->IsSelected( i ) ) {
			flgSelected = true;	// 選択されている
		}
	}
	// onRemoveButtonと同様の処理
	if( flgSelected ){
		onRemoveButton( evt );
	}

}

// Color ( list )
void OverlappingDialog::onColor( wxCommandEvent& evt ){

	// 項目が選択されているか?
	bool flgSelected = false;
	// selected items
	std::vector< int > selectedItems;
	for( unsigned int i = 0; i < m_list->GetCount(); i++ ) {
		if( m_list->IsSelected( i ) ) {
			flgSelected = true;	// 選択されている
		}
	}

	// onColorButtonと同様の処理
	if( flgSelected ){
		onColorButton( evt );
	}
}

// UI enable/disable check
void OverlappingDialog::onUpdateUIPopupMenu( wxUpdateUIEvent& evt ){
	int nEventId = evt.GetId();

	// ID check
	if(	( nEventId == ID_POPUP_MENU_DELETE ) || ( nEventId == ID_POPUP_MENU_COLOR )
			|| ( nEventId == ID_POPUP_MENU_UNSELECT ) ){
		// OK
	}else{
		// error
		return;
	}

	// 項目が選択されているか?
	bool flgSelected = false;
	// selected items
	std::vector< int > selectedItems;
	for( unsigned int i = 0; i < m_list->GetCount(); i++ ) {
		if( m_list->IsSelected( i ) ) {
			flgSelected = true;	// 選択されている
		}
	}

	// 選択されていればenable、されていなければdisable
	evt.Enable( flgSelected );

}

// create context Menu
void OverlappingDialog::onContextMenu( wxContextMenuEvent& evt ){
	wxPoint ptScreen = evt.GetPosition();
	wxPoint pt;
	pt = ScreenToClient( ptScreen );

	wxMenu menu;

	menu.Append( ID_POPUP_MENU_COLOR, wxT( STR_POPUP_MENU_COLOR ) );
	menu.Append( ID_POPUP_MENU_DELETE, wxT( STR_POPUP_MENU_DELETE ) );
	menu.AppendSeparator();
	menu.Append( ID_POPUP_MENU_UNSELECT, wxT( STR_POPUP_MENU_UNSELECT ) );

	PopupMenu( &menu, pt.x, pt.y );
	
}

// @date 2011.10.26 <Add> M.Izumi <-


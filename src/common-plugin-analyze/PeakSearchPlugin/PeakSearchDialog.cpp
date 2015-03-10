/**
 * @file PeakSearchDialog.cpp
 * @brief implements of PeakSearchDialog class
 *
 * @author S.Tanaka
 * @date 2008.09.08
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "PeakSearchDialog.h"


using namespace kome::search;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



#define BORDER_SIZE					3
#define GRID_GAP					3
#define NUMBER_EDIT_WIDTH		   75

#define ID_MIN_STAGE_TEXT			1
#define ID_MAX_STAGE_TEXT			2
#define ID_MIN_RT_TEXT				3
#define ID_MAX_RT_TEXT				4
#define ID_MIN_MZ_TEXT				5
#define ID_MAX_MZ_TEXT				6
#define ID_MIN_INT_TEXT				7
#define ID_MAX_INT_TEXT				8


BEGIN_EVENT_TABLE( PeakSearchDialog, kome::window::ParametersAndHelpDialog )
END_EVENT_TABLE()


// constructor
PeakSearchDialog::PeakSearchDialog( wxWindow* parent )
		: kome::window::ParametersAndHelpDialog( parent, "Filter Peaks", "Filter" ), m_params( NULL, true ) {
	// initialize
	m_minStageText = NULL;
	m_maxStageText = NULL;
	m_minRtText = NULL;
	m_maxRtText = NULL;
	m_minMzText = NULL;
	m_maxMzText = NULL;
	m_minIntText = NULL;
	m_maxIntText = NULL;

	m_minStage = -1;
	m_maxStage = -1;
	m_minRt = -1.0;
	m_maxRt = -1.0;
	m_minMz = -1.0;
	m_maxMz = -1.0;
	m_minInt = -1.0;
	m_maxInt = -1.0;
}

// destructor
PeakSearchDialog::~PeakSearchDialog() {
}

// get min stage
int PeakSearchDialog::getMinStage() {
	return m_minStage;
}

// get max stage
int PeakSearchDialog::getMaxStage() {
	return m_maxStage;
}

// get min RT
double PeakSearchDialog::getMinRt() {
	return m_minRt;
}

// get max RT
double PeakSearchDialog::getMaxRt() {
	return m_maxRt;
}

// get min m/z
double PeakSearchDialog::getMinMz() {
	return m_minMz;
}

// get max m/z
double PeakSearchDialog::getMaxMz() {
	return m_maxMz;
}

// get min intenisty
double PeakSearchDialog::getMinIntensity() {
	return m_minInt;
}

// get max intensity
double PeakSearchDialog::getMaxIntensity() {
	return m_maxInt;
}

// create main sizer
wxSizer* PeakSearchDialog::createMainSizer() {
	return createRangeSizer();
}

// create createRangeSizer sizer
wxSizer* PeakSearchDialog::createRangeSizer() {	// @date 2011/08/24 <Add> OKADA
	// create spectrum sizer
	wxFlexGridSizer* specSearchSizer = new wxFlexGridSizer( 4, 4, GRID_GAP, GRID_GAP );
	specSearchSizer->AddGrowableRow( 0 );
	specSearchSizer->AddGrowableRow( 1 );
	specSearchSizer->AddGrowableRow( 2 );
	specSearchSizer->AddGrowableRow( 3 );
	specSearchSizer->AddGrowableCol( 0 );
	specSearchSizer->AddGrowableCol( 1 );
	specSearchSizer->AddGrowableCol( 2 );
	specSearchSizer->AddGrowableCol( 3 );

	// page information
	kome::plugin::SettingsPage* page = new kome::plugin::SettingsPage( m_params );
	page->setCommon( false );
	page->setName( "filter_peaks" );

	// @date 2011.03.17 <Mod> M.Izumi ->
	
	// scan
	kome::plugin::SettingsValue* scan_intVal = m_params.addValue();
	scan_intVal->setType( kome::plugin::SettingsValue::TYPE_INT_RANGE );
	scan_intVal->getNumberRestriction().setInclusiveMinValue( 0.0 );
	// <-
	scan_intVal->setRequired( false );

	kome::plugin::SettingsValue* dblValRange = m_params.addValue();
	dblValRange->setType( kome::plugin::SettingsValue::TYPE_DOUBLE_RANGE );
	dblValRange->getNumberRestriction().setInclusiveMinValue( 0.0 );
	dblValRange->setRequired( false );

	kome::plugin::SettingsParam* param = m_params.addParam();
	kome::plugin::SettingsForm* form = page->addForm();

	param->setName( STAGE_PARAM_NAME );
	param->setValue( scan_intVal );
	

	form->setTitle( STAGE_PARAM_NAME );
	form->setParam( param );
	form->setDesc( "Set the MS stage range of spectra." );

	param = m_params.addParam();
	form = page->addForm();
	param->setName( RT_PARAM_NAME );
	param->setValue( dblValRange );
	
	form->setTitle( RT_PARAM_NAME );
	form->setParam( param );
	form->setDesc( "Set the RT range of spectra." );

	param = m_params.addParam();
	form = page->addForm();
	param->setName( MZ_PARAM_NAME );
	param->setValue( dblValRange );
	
	form->setTitle( MZ_PARAM_NAME );
	form->setParam( param );
	form->setDesc( "Set the m/z range of peaks." );

	param = m_params.addParam();
	form = page->addForm();
	param->setName( INT_PARAM_NAME );
	param->setValue( dblValRange );
	
	form->setTitle( INT_PARAM_NAME );
	form->setParam( param );
	form->setDesc( "Set the intensity range of peaks." );

	// panel
	kome::window::SettingsPanel* specPanel = new kome::window::SettingsPanel(
		this,
		page,
		false,
		&m_settings,
		false
	);

	specSearchSizer->Add( specPanel, 1, wxGROW | wxALL, BORDER_SIZE );

	return specSearchSizer;
}

// adds range
void PeakSearchDialog::addRange( 
		wxSizer* sizer,
		const char* title,
		const int minId,
		const int maxId,
		wxTextCtrl** minText,
		wxTextCtrl** maxText
) {
	// title
	sizer->Add(
		new wxStaticText( this, wxID_ANY, wxT( title ) ),
		0,
		wxALIGN_CENTER_VERTICAL,
		BORDER_SIZE
	);

	// create text ctrl of min value
	wxTextCtrl* minCtrl = new wxTextCtrl(
		this,
		minId,
		wxEmptyString,
		wxDefaultPosition,
		wxSize( NUMBER_EDIT_WIDTH, -1 )
	);
	sizer->Add( minCtrl, 0, wxALIGN_CENTER_VERTICAL, BORDER_SIZE );
	if( minText != NULL ) {
		*minText = minCtrl;
	}

	// dash
	sizer->Add(
		new wxStaticText( this, wxID_ANY, wxT( "-" ) ),
		0,
		wxALIGN_CENTER_VERTICAL,
		BORDER_SIZE
	);

	// create text ctrl of max value
	wxTextCtrl* maxCtrl = new wxTextCtrl(
		this,
		maxId,
		wxEmptyString,
		wxDefaultPosition,
		wxSize( NUMBER_EDIT_WIDTH, -1 )
	);
	if( maxText != NULL ) {
		*maxText = maxCtrl;
	}
	sizer->Add( maxCtrl, 0, wxALIGN_CENTER_VERTICAL, BORDER_SIZE );
}

// set value
bool PeakSearchDialog::setValue( wxTextCtrl* text, void* val, const char* name, const bool isInt ) {
	// get string
	std::string s = trimstring( text->GetValue().c_str() );

	// get value
	if( isInt ) {	// integer value
		int v = -1;

		if( !s.empty() ) {
			if( !isint( s.c_str(), 10, &v ) ) {	// not number
				std::string msg = FMT( "%s must be an integer.", name );
				kome::window::WindowTool::showInfo( msg.c_str() );
				return false;
			}
			if( v < 0 ) {	// negative value
				std::string msg = FMT( "%s cannot be a negative value.", name );
				kome::window::WindowTool::showInfo( msg.c_str() );
				return false;
			}
		}

		*( (int*)val ) = v;
	}
	else {	// dobule value
		double v = -1.0;

		if( !s.empty() ) {
			if( !isdouble( s.c_str(), &v ) ) {	// not number
				std::string msg = FMT( "%s must be a numeric value.", name );
				kome::window::WindowTool::showInfo( msg.c_str() );
				return false;
			}
			if( v < 0.0 ) {	// negative value
				std::string msg = FMT( "%s cannot be a negative value.", name );
				kome::window::WindowTool::showInfo( msg.c_str() );
				return false;
			}
		}

		*( (double*)val ) = v;
	}

	return true;
}

// transfer data from window
bool PeakSearchDialog::TransferDataFromWindow() {
	// ranges

	// m/z range
	const char* range;
	std::vector<std::string> tokens;
	range = m_settings.getParameterValue( STAGE_PARAM_NAME );
	tokens.clear();
	stringseparate( range, ":", tokens );

	m_minStage = -1;
	if( tokens.size() > 0 ) {
		std::string token = tokens[ 0 ];
		int minStage = -1;
		if( isint( token.c_str(), 10, &minStage ) ) {
			m_minStage = minStage;
		}
	}

	m_maxStage = -1;
	if( tokens.size() > 1 ) {
		std::string token = tokens[ 1 ];
		int maxStage = -1;
		if( isint( token.c_str(), 10, &maxStage ) ) {
			m_maxStage = maxStage;
		}
	}

	// RT range
	range = m_settings.getParameterValue( RT_PARAM_NAME );
	tokens.clear();
	stringseparate( range, ":", tokens );

	m_minRt = -1.0;
	if( tokens.size() > 0 ) {
		std::string token = tokens[ 0 ];
		double minRt = -1.0;
		if( isdouble( token.c_str(), &minRt ) ) {
			m_minRt = minRt;
		}
	}

	m_maxRt = -1.0;
	if( tokens.size() > 1 ) {
		std::string token = tokens[ 1 ];
		double maxRt = -1.0;
		if( isdouble( token.c_str(), &maxRt ) ) {
			m_maxRt = maxRt;
		}
	}

	// m/z range
	range = m_settings.getParameterValue( MZ_PARAM_NAME );
	tokens.clear();
	stringseparate( range, ":", tokens );

	m_minMz = -1.0;
	if( tokens.size() > 0 ) {
		std::string token = tokens[ 0 ];
		double minMz = -1.0;
		if( isdouble( token.c_str(), &minMz ) ) {
			m_minMz = minMz;
		}
	}

	m_maxMz = -1.0;
	if( tokens.size() > 1 ) {
		std::string token = tokens[ 1 ];
		double maxMz = -1.0;
		if( isdouble( token.c_str(), &maxMz ) ) {
			m_maxMz = maxMz;
		}
	}

	// intensity range
	range = m_settings.getParameterValue( INT_PARAM_NAME );
	tokens.clear();
	stringseparate( range, ":", tokens );

	m_minInt = -1.0;
	if( tokens.size() > 0 ) {
		std::string token = tokens[ 0 ];
		double minInt = -1.0;
		if( isdouble( token.c_str(), &minInt ) ) {
			m_minInt = minInt;
		}
	}

	m_maxInt = -1.0;
	if( tokens.size() > 1 ) {
		std::string token = tokens[ 1 ];
		double maxInt = -1.0;
		if( isdouble( token.c_str(), &maxInt ) ) {
			m_maxInt = maxInt;
		}
	}

	return true;
}

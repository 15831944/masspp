/**
 * @file MatrixPeaksMergePanel.cpp
 * @brief interfaces of IdentificationPanel class
 *
 * @author M.Izumi
 * @date 2013.01.15
 * 
 * Copyright(C) 2013 Shimadzu Corporation. All rights reserved.
 */

#include "stdafx.h"
#include "MatrixPeaksMergePanel.h"

#include <iterator>
#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>
#include <sstream>

#include "../../common-lib/WindowManager/matrix_flow07.xpm"


using namespace kome::mergepeaks;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



#define GRID_GAP                     3
#define BORDER_SIZE                  3

#define SECTION                      "MatrixPeaksMerge"
#define RT_TOL_PARAM_NAME			 "rt tol"
#define RT_TOL_PARAM_KEY			 "RT_TOLERANCE"
#define MZ_TOL_PARAM_NAME			 "mz tol"
#define MZ_TOL_PARAM_KEY			 "MZ_TOLERANCE"

BEGIN_EVENT_TABLE( PeakMagePanel, kome::window::PeakMatrixSettingsPageBase )
END_EVENT_TABLE()

// constructor
PeakMagePanel::PeakMagePanel( wxWizard* parent, const char* name )
	: kome::window::PeakMatrixSettingsPageBase( parent, name ), m_params( NULL, false ){
	SetSizer( createMainSizer() );
}

// destructor
PeakMagePanel::~PeakMagePanel(){
}

void PeakMagePanel::createInnerSizer(wxStaticBoxSizer* InnerSizer){
	// set bitmap
	wxBitmap addBmp( matrix_flow07_xpm );
	
	wxBoxSizer* pCategoriesSizer = new wxBoxSizer( wxVERTICAL );
	pCategoriesSizer->Add(  new wxStaticBitmap( this, wxID_ANY, addBmp ), 1, (wxALL & ~wxRIGHT) | wxGROW, BORDER_SIZE );

	wxSizer* gHoriznSizer = new wxBoxSizer( wxHORIZONTAL);
	gHoriznSizer->Add( pCategoriesSizer, 1, (wxALL & ~wxRIGHT) | wxGROW | wxALIGN_CENTER_VERTICAL, BORDER_SIZE );
	InnerSizer->Add( gHoriznSizer );
	
	// sizer
	wxFlexGridSizer* sizer = new wxFlexGridSizer( 3, 1, GRID_GAP, GRID_GAP );
	sizer->AddGrowableRow( 1 );
	sizer->AddGrowableCol( 0 );

	wxBoxSizer* textSizer = new wxBoxSizer( wxVERTICAL );
	textSizer->Add( new wxStaticText( this, wxID_ANY, wxT("Input tolerance to delete peaks which looks like duplicating with other peak.")) );
	textSizer->AddSpacer( 20 );
	sizer->Add( textSizer, 1, wxALL | wxGROW, 20 );	
	
	// spectra sizer
	wxSizer* spectraSizer = createSpectraSizer();
	sizer->Add( spectraSizer, 1, wxALL | wxGROW, BORDER_SIZE );

	InnerSizer->Add( sizer, wxALL | wxGROW, 20 );	
}

// create spectra sizer
wxSizer* PeakMagePanel::createSpectraSizer(){
	
	// create sizer
	wxFlexGridSizer* sizer = new wxFlexGridSizer( 3, 1, GRID_GAP, GRID_GAP );
	sizer->AddGrowableCol( 0 );
	sizer->AddGrowableRow( 0 );
	sizer->AddGrowableRow( 1 );

	// tolerance
	kome::plugin::SettingsPage* page = new kome::plugin::SettingsPage( m_params );

	kome::plugin::SettingsValue* val = m_params.addValue();
	val->setType( kome::plugin::SettingsValue::TYPE_DOUBLE );
	val->getNumberRestriction().setInclusiveMinValue( 0.0 );

	kome::plugin::SettingsParam* param = m_params.addParam();
	param->setValue( val );
	param->setSection( SECTION );
	param->setName( RT_TOL_PARAM_NAME );
	param->setKey( RT_TOL_PARAM_KEY );

	kome::plugin::SettingsForm* form = page->addForm();
	form->setParam( param );
	form->setTitle( "RT Tolerance" );
	form->setDesc( "Set the RT tolerance to merge peaks." );

	param = m_params.addParam();
	param->setValue( val );
	param->setSection( SECTION );
	param->setName( MZ_TOL_PARAM_NAME );
	param->setKey( MZ_TOL_PARAM_KEY );

	form = page->addForm();
	form->setParam( param );
	form->setTitle( "m/z Tolerance" );
	form->setDesc( "Set the m/z tolerance to merge peaks." );

	kome::window::SettingsPanel* panel = new kome::window::SettingsPanel( this, page, false, &m_settings, true );
	sizer->Add( panel, 1, wxALL | wxGROW, BORDER_SIZE );

	return sizer;
}

// Validate panel settings 
bool PeakMagePanel::IsValid(){
	return true;
}

// save panel settings
bool PeakMagePanel::SavePanelSettings(kome::core::Progress *pProgress){
	if( !TransferDataFromWindow() ) {
		return false;
	}
	
	pProgress->setStatus(GetPanelName().c_str());
	if( pProgress == NULL ) {
		pProgress = &kome::core::Progress::getIgnoringProgress();
	}

	std::ostringstream infoString;
	infoString << "RT Tolerance: " << m_rtTol << "\n";
	infoString << "m/z Tolerance: " << m_mzTol << "\n";

	// check the parameter
	if( m_rtTol < 0.0 || m_mzTol ) {
		pProgress->fill();
		return true;
	}

	// ï€ë∂èàóù
	kome::ident::IdentificationManager& identMgr = kome::ident::IdentificationManager::getInstance();
	int matrixId = identMgr.getSelectedMatrix();
		
	// peaks
	std::vector< int > peaks;
	identMgr.getMatrixPeaks( peaks, matrixId );
	if( peaks.size() == 0 ) {
		return true;
	}
	
	// search 
	std::vector< int > searches;
	identMgr.getSearchesFromMatrix( searches, matrixId );
	int searchId = -1;
	for( unsigned int i = 0; i < searches.size(); i++ ) {
		searchId = std::max( searchId, searches[ i ] );
	}

	// get substance
	std::vector< int >substance;
	getSubstance( searches, searchId, peaks, substance );
	
	std::vector< int > resPeakId;
	
	double rt = identMgr.getMatrixPeakRt( peaks[0] );
	double mz = identMgr.getMatrixPeakMz( peaks[0] );

	for( unsigned int i=1; i < peaks.size() && !pProgress->isStopped(); i++ ){

		// progress
		pProgress->setStatus( 
			FMT( "Peak Searching [%d/%d] ", ( i + 1 ), peaks.size() ).c_str()
		);

		double srt = rt-m_rtTol;
		double ert = rt+m_rtTol;
		double smz = mz-m_mzTol;
		double emz = mz+m_mzTol;
	
		resPeakId.clear();
		for( unsigned int j=i; j < peaks.size(); j++ ){
			
			rt = identMgr.getMatrixPeakRt( peaks[j] );
			mz = identMgr.getMatrixPeakMz( peaks[j] );
	
			if(( srt < rt && rt <= ert) &&(smz < mz && mz <= emz) ){
				resPeakId.push_back( peaks[j] );
			}
					
		}

		DeleteMatrixPeak( matrixId, resPeakId, substance );

		rt = identMgr.getMatrixPeakRt( peaks[i] );
		mz = identMgr.getMatrixPeakMz( peaks[i] );

		// progress
		//ÅöpProgress->setPosition( i + 1 );

	}

	if( !pProgress->isStopped() ){
		pProgress->fill();
	}

	return true;
}

// delete matrix peak
void PeakMagePanel::DeleteMatrixPeak( int matrixId, std::vector< int > resPeaks, std::vector<int> substance ){
	kome::ident::IdentificationManager& identMgr = kome::ident::IdentificationManager::getInstance();
		
	double max=0.0;
	int peakId = 0;
	
	std::vector< Info > subInfo;
	
	for( unsigned int i=0; i < resPeaks.size(); i++ ){
		int peak = resPeaks[i];
		
		double sum = 0.0;
		if( substance.empty() ){
			getTotalValueOfPeak( matrixId, peak, sum );
			if( max < sum ){
				max = sum;
				peakId = peak;
			}
		}else{
			for( unsigned int j=0; j < substance.size(); j++ ){
				if( peak == substance[i] ){
					getTotalValueOfPeak( matrixId, peak, sum );
					if( max < sum ){
						max = sum;
						peakId = peak;
					}
				}
			}
		}
	}
	
	for( unsigned int i=0; i < resPeaks.size(); i++ ){
		if( peakId != resPeaks[i] ){
			identMgr.deleteMatrixPeak( resPeaks[i] );
		}
	}
}


void PeakMagePanel::getTotalValueOfPeak( int matrixId, int peak , double& sum ){

	kome::ident::IdentificationManager& identMgr = kome::ident::IdentificationManager::getInstance();

	// groups
	std::vector< int > groups;
	identMgr.getGroups( groups, matrixId );

	// samples
	std::vector< int > samples;
	for( unsigned int i=0; i < groups.size(); i++ ){
		identMgr.getMatrixSamplesFromGroup( samples, groups[i] );
	}

	for( unsigned int j=0; j < samples.size(); j++ ){
		int sample = samples[j];

		sum += identMgr.getPeakValue( peak, sample );
	}
	
}

bool PeakMagePanel::PreApplyValidationCheck(std::string& sError){
	return true;
}

// Update standard samples on this panel
void PeakMagePanel::updateStandardSamples(bool bReadFromINIfile){
}

// Refresh panel contents for matrix
void PeakMagePanel::RefreshPanelContentsForMatrix(){
}

// transfer data from window
bool PeakMagePanel::TransferDataFromWindow(){
	// default
	if( !PeakMatrixSettingsPageBase::TransferDataFromWindow() ) {
		return false;
	}
		
	m_rtTol =  m_settings.getDoubleValue( RT_TOL_PARAM_NAME, -1.0 );
	m_mzTol = m_settings.getDoubleValue( MZ_TOL_PARAM_NAME, -1.0 );

	std::string s = FMT( "- %s(RT Tol=%f : m/z Tol=%f)\n", m_sPanelName.c_str(), m_rtTol, m_mzTol );
	setSettingParam( s.c_str() );

	return true;
}

// get substance
void PeakMagePanel::getSubstance( 
	std::vector< int > searches, 
	int searchId,
	std::vector< int > peaks, 
	std::vector< int >& subtrance 
){
	// manager
	kome::ident::IdentificationManager& identMgr = kome::ident::IdentificationManager::getInstance();
	bool bflg = false;

	for( unsigned int i = 0; i < peaks.size(); i++ ) {
		// add peak
		const int peakId = peaks[ i ];
		
		// hit spectra
		searches.clear();
		double score = -FLT_MAX;
		identMgr.getMatrixPeakSpectraFromSearchAndPeak( searches, searchId, peakId );
		for( unsigned int j = 0; j < searches.size(); j++ ) {
			const int specId = identMgr.getMatrixPeakSpectrumSpectrum( searches[ j ] );
			std::vector< int > hits;
			int substance = -1;
			identMgr.getHitsFromSpectrum( hits, specId );
			for( unsigned int k = 0; k < hits.size(); k++ ) {
				const int hitId = hits[ k ];
				kome::ident::HitInfo hit;
				identMgr.getHit( hitId, hit );
				bool loop = true;
				for( unsigned int l = 0; l < hit.props.size() && loop; l++ ) {
					if( hit.props.at( l ).type == kome::ident::TYPE_SCORE ) {
						double val = double();
						if( isdouble( hit.props.at( l ).value.c_str(), & val ) ) {
							if( val > score ) {
								substance = hit.substance;
								loop = false;
							}
						}
					}
				}
				if( substance < 0 ) {
					substance = hit.substance;
				}
			}

			kome::ident::SubstanceInfo subInfo;
			subInfo.parentId = substance;
			while( subInfo.parentId >= 0 ) {
				identMgr.getSubstance( subInfo.parentId, subInfo );
			}
			if( !subInfo.accession.empty() || !subInfo.name.empty() ) {
				
				bflg = true;
			}
		}
		if( bflg ){
			subtrance.push_back( peakId );
		}
	}
}


// get substance
void PeakMagePanel::getSubstanceCmd( 
	std::vector< int > searches, 
	int searchId,
	std::vector< int > peaks, 
	std::vector< int >& subtrance 
	)
{
	getSubstance( searches, searchId, peaks, subtrance );
}

// Delete Matrix Peak Cmd
void PeakMagePanel::DeleteMatrixPeakCmd( int matrixId, std::vector< int > resPeaks, std::vector<int> substance ){

	DeleteMatrixPeak( matrixId, resPeaks, substance );
}


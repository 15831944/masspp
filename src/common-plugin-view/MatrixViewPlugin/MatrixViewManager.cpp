/**
 * @file MatrixViewManager.cpp
 * @brief implements of MatrixViewManager class
 *
 * @author S.Tanaka
 * @date 2009.11.24
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "MatrixViewManager.h"
#include "QuantitationDialog.h"
#include "CreatePeakMatrixDialog.h"
#include "MatrixListCtrl.h"

using namespace kome::matrix::view;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
MatrixViewManager::MatrixViewManager() {
	// initialize
	m_dlg = NULL;
	m_peakMatrixDlg = NULL;

	m_vColumn.clear();	// @date 2013/08/14 <Add> OKADA
	m_data.clear();		// @date 2013/08/14 <Add> OKADA
}

// destructor
MatrixViewManager::~MatrixViewManager() {
	releasePeakMatrixDialog();
}

// release matrix dialog
void MatrixViewManager::releaseDialog() {
	// @date 2012.09.05 <Mod> M.Izumi ->
	if( m_dlg != NULL ){
		delete m_dlg;
		m_dlg = NULL;
	}
	// @date 2012.09.05 <Mod> M.Izumi <-
}

// show matrix dialog
void MatrixViewManager::toggleDialog() {
	// show flag
	bool showing = !isShownDialog();

	if ((showing) || m_dlg->Close())
	{
		// delete old dialog
		if( m_dlg != NULL ) {
			delete m_dlg;
			m_dlg = NULL;
		}
	}

	// show
	m_dlg = new QuantitationDialog( kome::window::WindowTool::getMainWindow());
	m_dlg->SetPosition( wxPoint( 200, 200 ) ); // 表示位置 @date 2011.11.07 <Add> M.Izumi
	m_dlg->Show( true );
}

// judge whether matrix dialog is opened
bool MatrixViewManager::isShownDialog() {
	if( m_dlg == NULL ) {
		return false;
	}
	return m_dlg->IsShown();
}

// update dialog
void MatrixViewManager::updateDialog() {
	// delete old dialog
	if( m_dlg != NULL ) {
		delete m_dlg;
		m_dlg = NULL;
	}

	// show dialog
	toggleDialog();
}

// update matrix combo
void MatrixViewManager::updateMatrixCombo(){
	if( m_dlg != NULL ) {
		m_dlg->updateMatrixCombo();
	}
}

// set project
void MatrixViewManager::setProject(){
	if( m_dlg != NULL ){
		m_dlg->setnProject();
	}
}

// matrix list is empty
bool MatrixViewManager::MatrixListIsEmpty(){
	if( m_dlg != NULL ){
		return m_dlg->MatrixListIsEmpty();
	}
	return false;
}

// update matrix list
void MatrixViewManager::updateMatrixList(){
	if( m_dlg != NULL ) {
		m_dlg->RecreateMatrixList();
	}
}
// refresh dialog
void MatrixViewManager::refreshDialog() {
	if( isShownDialog() ) {
		m_dlg->Refresh();
	}
}

// activate the peak
void MatrixViewManager::activatePeak( const int peakId ) {
	// manager
	kome::ident::IdentificationManager& identMgr = kome::ident::IdentificationManager::getInstance();

	// activate
	if( isShownDialog() ) {
		MatrixListCtrl* list = m_dlg->getMatrixListCtrl();

		// search item
		int item = -1;
		for( unsigned int i = 0; i < list->getNumberOfData() && item < 0; i++ ) {
			if( list->getData( i ) == peakId ) {
				item = (int)i;
			}
		}

		// select
		if( item >=  0 ) {
			identMgr.addFocusedPeak( peakId );
			kome::window::ListTool::unselectAll( list );
			kome::window::ListTool::selectItem( list, item );

			wxRect rect;
			if( list->GetSubItemRect( item, 0, rect ) ) {
				list->ScrollList( 0, rect.y - rect.height );
			}
		}

		refreshDialog();
	}
}

// check whether can close dialog
bool MatrixViewManager::canCloseDialog() {
	return true;
}

// opne peak matrix dialog
void MatrixViewManager::openPeakMatrixDialog( const bool bedit ){
	releasePeakMatrixDialog();
	
	// dialog
	m_peakMatrixDlg = new CreatePeakMatrixDialog( kome::window::WindowTool::getMainWindow(), true, bedit );
	
	m_peakMatrixDlg->RunWizard( m_peakMatrixDlg->GetFirstPage() );
}

// release peak matrix dialog
void MatrixViewManager::releasePeakMatrixDialog(){
	if( m_peakMatrixDlg != NULL ){
		delete m_peakMatrixDlg;		
	}
	m_peakMatrixDlg = NULL;
	
}

// get peak matrix dialog
CreatePeakMatrixDialog* MatrixViewManager::getPeakMatrixDlg(){
	return m_peakMatrixDlg;	
}

// get MatrixViewManager object
MatrixViewManager& MatrixViewManager::getInstance() {
	// create object
	static MatrixViewManager mgr;

	return mgr;
}

// close peak info dlg
void MatrixViewManager::onClosePeakInfoDlg(){
	if( m_dlg != NULL ){
		m_dlg->closePeakInfoDlg();
	}
}


kome::objects::Variant MatrixViewManager::saveTextPeakMatrixDataFileDlg( kome::objects::Parameters* params )
{
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = false;
	

	// matrix name
	kome::objects::SettingParameterValues* settings = kome::plugin::PluginCallTool::getSettingValues( params );
	if( settings == NULL ) {
		LOG_ERROR( FMT( "Failed to get arguments information." ) );
		return ret;
	}

	const char* n = settings->getParameterValue( "path" );
	std::string strPath = NVL( n, "" );
	if( strPath.empty() ) {
		LOG_ERROR( FMT( "Failed to get the path." ) );
		return ret;
	}

	ret = saveTextPeakMatrixDataFile( strPath.c_str() );

	return ret;
}

// command line
kome::objects::Variant MatrixViewManager::saveTextPeakMatrixDataFile( const char *szFilePath, kome::objects::Parameters* params ){

	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = false;
	
	// 参考元：MatrixListCtrl::onCreate()
	// manager
	kome::ident::IdentificationManager& identMgr = kome::ident::IdentificationManager::getInstance();

	// selected project
	int project = identMgr.getSelectedMatrix();

	// header
	bool rtFlg = false;
	bool mzFlg = false;
	bool chargeFlg = false;

	m_rtColFMLC = -1;
	m_mzColFMLC = -1;
	m_chargeColFMLC = -1;
	m_appendedColFMLC = 0;

	identMgr.getMatrixFlags( project, &rtFlg, &mzFlg, &chargeFlg );

	if( rtFlg ) {
		addColumn( "RT", kome::window::GridListCtrl::TYPE_UDOUBLE );
		m_rtColFMLC = m_appendedColFMLC;
		m_appendedColFMLC += 1;
	}
	if( mzFlg ) {
		addColumn( "m/z", kome::window::GridListCtrl::TYPE_UDOUBLE );
		m_mzColFMLC = m_appendedColFMLC;
		m_appendedColFMLC += 1;
	}
	if( chargeFlg ) {
		addColumn( "Charge", kome::window::GridListCtrl::TYPE_UINT );
		m_chargeColFMLC = m_appendedColFMLC;
		m_appendedColFMLC += 1;
	}

	std::vector< int > searches;
	identMgr.getSearchesFromMatrix( searches, project );
	int searchId = -1;
	for( unsigned int i = 0; i < searches.size(); i++ ) {
		searchId = std::max( searchId, searches[ i ] );
	}
	if( searchId >= 0 ) {
		addColumn( "Substance", kome::window::GridListCtrl::TYPE_STRING, NULL, 160 );
		m_substanceColFMLC = m_appendedColFMLC;
		m_appendedColFMLC += 1;
	}

	// appended columns
	m_appendedColsFMLC.clear();
	if( project >= 0 ) {
		identMgr.getMatrixColumns( m_appendedColsFMLC, project );
	}

	for( unsigned int i = 0; i < m_appendedColsFMLC.size(); i++ ) {
		std::string name = identMgr.getMatrixColumnName( m_appendedColsFMLC[ i ] );
		kome::ident::ColumnDataType type = identMgr.getMatrixColumnType( m_appendedColsFMLC[ i ] );

		kome::window::GridListCtrl::ColumnDataType t = kome::window::GridListCtrl::TYPE_STRING;
		if( type == kome::ident::TYPE_INT ) {
				t = kome::window::GridListCtrl::TYPE_INT;
		}
		else if( type == kome::ident::TYPE_UINT ) {
			t = kome::window::GridListCtrl::TYPE_UINT;
		}
		else if( type == kome::ident::TYPE_DOUBLE ) {
			t = kome::window::GridListCtrl::TYPE_DOUBLE;
		}
		else if( type == kome::ident::TYPE_UDOUBLE ) {
			t = kome::window::GridListCtrl::TYPE_UDOUBLE;
		}

		addColumn( name.c_str(), t );
	}

	// groups
	m_samplesFMLC.clear();

	std::vector< int > groups;
	identMgr.getGroups( groups, project );
	for( unsigned int i = 0; i < groups.size(); i++ ) {
		identMgr.getMatrixSamplesFromGroup( m_samplesFMLC, groups[ i ] );
	}

	for( unsigned int i = 0; i < m_samplesFMLC.size(); i++ ) {
		std::string name = identMgr.getMatrixSampleName( m_samplesFMLC[ i ] );
		addColumn( name.c_str(), kome::window::GridListCtrl::TYPE_UDOUBLE );
	}

	//ここでヘッダ部分1行出力可
	// header

	// save 
	FILE* fp = fileopen( szFilePath, "w" );
	if( fp == NULL ) {
		std::string msg = FMT( "Cannot open the file for writing. Check the file / folder permissions. [%s]", szFilePath );
		LOG_ERROR( FMT( msg.c_str() ) );
		return ret;
	}
		
	// header
	for( unsigned int j = 0; j < m_vColumn.size(); j++ ) {
		// tab
		if( j > 0 ) {
			fprintf( fp, "\t" );
		}

		// column title
		const char* title = m_vColumn[j].strTitle.c_str();
		fprintf( fp, "%s", NVL( title, "" ) );
	}

	// @date 2013/09/11 <Add> OKADA ------->
	// get progress
	kome::core::Progress* progress = NULL;
	if( params != NULL ){
		progress = kome::plugin::PluginCallTool::getProgress( params );
	}
	if( progress == NULL ) {
		progress = &kome::core::Progress::getIgnoringProgress();
	}
	// @date 2013/09/11 <Add> OKADA <-------
	
	// add items
	std::vector< int > peaks;
	identMgr.getMatrixPeaks( peaks, project );

	progress->setRange( 0, peaks.size() );	// @date 2013/09/11 <Add> OKADA

	for( unsigned int i = 0; i < peaks.size(); i++ ) {

		progress->setPosition( i );	// @date 2013/09/11 <Add> OKADA

		// add peak
		const int peakId = peaks[ i ];
		addData( peakId, false );

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
				std::string subName = subInfo.accession;
				if( !subInfo.name.empty() ) {
					if( !subName.empty() ) {
						subName.append( " : " );
					}
					subName.append( subInfo.name );
				}

				m_substanceMapFMLC[ peakId ] =subName;
			}
		}

		// ここで1行出力可
		writeData1Line( i, fp );
	}

	progress->fill();	// @date 2013/09/11 <Add> OKADA

	// close
	fflush( fp );
	fclose( fp );

	ret.prim.boolVal = false;

	return ret;
}

//void addColumn( const char* title, const ColumnDataType type, const int width = 80 );
void MatrixViewManager::addColumn( const char* title, const kome::window::GridListCtrl::ColumnDataType type, void* ptr, const int width ){
	if( ptr == NULL ){
		// command line等、画面を開かないバージョン
		int nSize = m_vColumn.size();
		column_t column;
		column.nColumnNo = nSize;
		column.strTitle = title;
		column.type = type;
		m_vColumn.push_back( column );
	}else{

		kome::matrix::view::MatrixListCtrl* pMatrixList = (kome::matrix::view::MatrixListCtrl*)ptr;
		if( pMatrixList == NULL ){
			return;
		}

		pMatrixList->addColumn( title, type, width );
	}
}

//void addColumn( const char* title, const ColumnDataType type, const int width = 80 );
void MatrixViewManager::addData( long long data, const bool update, void* ptr ){
	if( ptr == NULL ){
		// command line等、画面を開かないバージョン
		m_data.push_back( data );

	}else{
		// window
		kome::matrix::view::MatrixListCtrl* pMatrixList = (kome::matrix::view::MatrixListCtrl*)ptr;
		if( pMatrixList == NULL ){
			return;
		}

		pMatrixList->addData( data, update );
	}
}


void MatrixViewManager::writeData1Line( int nPeekCnt, FILE* fp )
{
	if( fp == NULL ){
		return;
	}

	// manager
	kome::ident::IdentificationManager& identMgr = kome::ident::IdentificationManager::getInstance();

	int i = nPeekCnt;

	// sample begin column
	const int sampleBeginCol = m_appendedColFMLC + (int)m_appendedColsFMLC.size();

	// new line
	fprintf( fp, "\n" );

	// value
	for( unsigned int col = 0; col < m_vColumn.size(); col++ ) {
		if( col > 0 ) {
			fprintf( fp, "\t" );
		}

		// string object
		std::string str;
		const long long data = m_data[i];

		if( col == m_rtColFMLC ){	// RT
			double rt = identMgr.getMatrixPeakRt( data );
			if( rt > - FLT_MAX ) {
				if( m_vColumn[col].type == kome::window::GridListCtrl::TYPE_DOUBLE || rt >= 0.0 ) {
					str = FMT( "%f", rt );
				}
			}
		}else if( col == m_mzColFMLC ) {	// m/z
			double mz = identMgr.getMatrixPeakMz( data );
			if( mz > - FLT_MAX ) {
				if( m_vColumn[col].type == kome::window::GridListCtrl::TYPE_DOUBLE || mz >= 0.0 ) {
					str = FMT( "%f", mz );
				}
			}
		}else if( col == m_chargeColFMLC ){	// charge
			int charge = identMgr.getMatrixPeakCharge( data );
			if( charge > INT_MIN ) {
				if( m_vColumn[col].type == kome::window::GridListCtrl::TYPE_INT || charge >= 0 ) {
					str = FMT( "%d", charge );
				}
			}
		}else if( col >= (unsigned int)m_appendedColFMLC && col < (unsigned int)sampleBeginCol ){	// appended value
			if( ( m_vColumn[col].type == kome::window::GridListCtrl::TYPE_INT )||
				( m_vColumn[col].type == kome::window::GridListCtrl::TYPE_UINT ) ){
				int idx = col - m_appendedColFMLC;
				std::string s = identMgr.getAppendedValue( data, m_appendedColsFMLC[ idx ] );
				int val = int();
				if( !isint( s.c_str(), 10, &val ) ) {
					val = INT_MIN;
				}
				if( val > INT_MIN ) {
					if( m_vColumn[col].type == kome::window::GridListCtrl::TYPE_INT || val >= 0 ) {
						str = FMT( "%d", val );
					}
				}
			}else if( ( m_vColumn[col].type == kome::window::GridListCtrl::TYPE_DOUBLE )||
						( m_vColumn[col].type == kome::window::GridListCtrl::TYPE_UDOUBLE ) ){
				double val = double();
				int idx = col - m_appendedColFMLC;
				std::string s = identMgr.getAppendedValue( data, m_appendedColsFMLC[ idx ] );
				if( !isdouble( s.c_str(), &val ) ) {
					val = - FLT_MAX;
				}
				if( val > - FLT_MAX ) {
					if( m_vColumn[col].type == kome::window::GridListCtrl::TYPE_DOUBLE || val >= 0.0 ) {
						str = FMT( "%f", val );
					}
				}
			}else if( m_vColumn[col].type == kome::window::GridListCtrl::TYPE_STRING ){
				int idx = col - m_appendedColFMLC;
				str = identMgr.getAppendedValue( data, m_appendedColsFMLC[ idx ] );
			}
		}else if( col == m_substanceColFMLC ){	// substance
			// index
			int idx = col - m_appendedColFMLC;

			if( col == m_substanceColFMLC ) {
				if( m_substanceMapFMLC.find( data ) != m_substanceMapFMLC.end() ) {
					str = m_substanceMapFMLC.at( data );
				}
			}
		}else if( col >= (unsigned int)sampleBeginCol ){	// intensity
			// group ID
			int id = -1;
			int idx = col - sampleBeginCol;
			if( idx < (int)m_samplesFMLC.size() ) {
				id = m_samplesFMLC[ idx ];
			}

			double val = double();
			if( id >= 0 ) {
				val = identMgr.getNormalizedPeakValue( data, id );
			}
			if( val > - FLT_MAX ) {
				if( m_vColumn[col].type == kome::window::GridListCtrl::TYPE_DOUBLE || val >= 0.0 ) {
					str = FMT( "%f", val );
				}
			}
		}
		fprintf( fp, "%s", str.c_str() );

	}
	
};

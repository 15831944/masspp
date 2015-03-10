/**
 * @file AlignmentManager.cpp
 * @brief implements of AlignmentManager class
 *
 * @author S.Tanaka
 * @date 2007.08.20
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "AlignmentManager.h"
#include "CorrDummyChromatogram.h"
#include "ResultChromatogram.h"

#include <boost/bind.hpp>

using namespace kome::alignment;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


#define ALIGNMENT_LONG_NAME	  "AB3D Alignment (TIC Pattern)"  // RT アライメントとしてこれが選択されている場合のみスペクトルのスケーリングを行う。
         

       
// constructor
AlignmentManager::AlignmentManager() {
	m_ctrlChrom = NULL;
	m_ctrlChromResult = NULL;

	m_trmtChrom = NULL;
	m_trmtChromResult = NULL;

	m_corrDummyChrom = NULL;

	m_alignment = NULL;

	m_cursorPos = 0;

	m_startPos = 0;
	m_endPos = 0;

	m_RawCtrlSample = 0;			// @date 2013.05.14 <Add> Fujita 
	m_RawTrmtSample = 0;			// @date 2013.05.14 <Add> Fujita 
	m_ScaledCtrlSample = 0;			// @date 2013.05.14 <Add> Fujita 
	m_ScaledTrmtSample = 0;			// @date 2013.05.14 <Add> Fujita 

	m_flg = 0;						// @date 2013.05.27 <Add> Fujita   //  m_RawCtrlTics と　m_RawTrmtTicsに値を入れたかどうかのフラグ 

}

// destructor
AlignmentManager::~AlignmentManager() {
	clearChromatograms();
}

// clear chromatograms
void AlignmentManager::clearChromatograms() {
	clearControl();
	clearTreatment();
	clearCorrection();	
}

// clear control
void AlignmentManager::clearControl() {
	// control chromatogram
	if( m_ctrlChrom != NULL ) {
		kome::plugin::PluginCallTool::onCloseChromatogram( *m_ctrlChrom, NULL );
		delete m_ctrlChrom;
		m_ctrlChrom = NULL;
	}

	// result control chromatogram
	if( m_ctrlChromResult != NULL ) {
		kome::plugin::PluginCallTool::onCloseChromatogram( *m_ctrlChromResult, NULL );
		delete m_ctrlChromResult;
		m_ctrlChromResult = NULL;
	}
}

// clear treatment
void AlignmentManager::clearTreatment() {
	// treatment chromatogram
	if( m_trmtChrom != NULL ) {
		kome::plugin::PluginCallTool::onCloseChromatogram( *m_trmtChrom, NULL );
		delete m_trmtChrom;
		m_trmtChrom = NULL;
	}

	// result treatment chromatogram
	if( m_trmtChromResult != NULL ) {
		kome::plugin::PluginCallTool::onCloseChromatogram( *m_trmtChromResult, NULL );
		delete m_trmtChromResult;
		m_trmtChromResult = NULL;
	}
}

// clear correction points
void AlignmentManager::clearCorrection() {
	if( m_corrDummyChrom != NULL ) {
		kome::plugin::PluginCallTool::onCloseChromatogram( *m_corrDummyChrom, NULL );
		delete m_corrDummyChrom;
		m_corrDummyChrom = NULL;
	}
	m_corrPts.clearPoints();
}

// get control chromatogram
kome::objects::Chromatogram* AlignmentManager::getControlChrom() {
	return m_ctrlChrom;
}

// get result control chromatogram
kome::objects::Chromatogram* AlignmentManager::getResultControlChrom() {
	return m_ctrlChromResult;
}

// get treatment chromatogram
kome::objects::Chromatogram* AlignmentManager::getTreatmentChrom() {
	return m_trmtChrom;
}

// get result treatment chromatogram
kome::objects::Chromatogram* AlignmentManager::getResultTreatmentChrom() {
	return m_trmtChromResult;
}

// get dummy chromatogram to draw correction graph
kome::objects::Chromatogram* AlignmentManager::getCorrectionDummyChrom() {
	return m_corrDummyChrom;
}

// unset alignment
void AlignmentManager::unsetAlignment() {
	if( m_alignment != NULL ) {
		delete m_alignment;
	}
	m_alignment = NULL;
}

// select control chromatogram
void AlignmentManager::selectCtrlChrom( kome::objects::Chromatogram* chrom ) {
	// check the chromatogram
	kome::objects::Chromatogram* oldChrom = NULL;
	if( m_ctrlChrom != NULL ) {
		oldChrom = m_ctrlChrom->getOrgChromatogram();
	}
	if( chrom == oldChrom ) {
		return;
	}

	// groups
	kome::objects::DataGroupNode* group = NULL;
	if( chrom != NULL ) {
		group = chrom->getGroup();
	}

	kome::objects::DataGroupNode* oldGroup = NULL;
	if( oldChrom != NULL ) {
		oldGroup = oldChrom->getGroup();
	}

	if( group != oldGroup ) {
		clearCorrection();
		unsetAlignment();
	}

	// clear control
	clearControl();

	// create new chromatogram
	if( chrom != NULL ) {
		// chromatogram
		m_ctrlChrom = new kome::operation::TransformedChromatogram( *chrom );
		m_ctrlChromResult = new ResultChromatogram( true );
	}

	// refres
	kome::window::WindowTool::refresh();
}

// select treatment group
void AlignmentManager::selectTrmtChrom( kome::objects::Chromatogram* chrom ) {
	// check the chromatogram
	kome::objects::Chromatogram* oldChrom = NULL;
	if( m_trmtChrom != NULL ) {
		oldChrom = m_trmtChrom->getOrgChromatogram();
	}
	if( chrom == oldChrom ) {
		return;
	}

	// groups
	kome::objects::DataGroupNode* group = NULL;
	if( chrom != NULL ) {
		group = chrom->getGroup();
	}

	kome::objects::DataGroupNode* oldGroup = NULL;
	if( oldChrom != NULL ) {
		oldGroup = oldChrom->getGroup();
	}

	// clear treatment
	clearTreatment();
	clearCorrection();

	// create new chromatogram
	if( chrom != NULL ) {
		// chromatogram
		m_trmtChrom = new kome::operation::TransformedChromatogram( *chrom );
		m_trmtChromResult = new ResultChromatogram( false );
	}

	// update
	if( group != oldGroup || m_trmtChrom == NULL ) {
		unsetAlignment();
	}

	kome::window::DialogProgress progress( kome::window::WindowTool::getMainWindow(), "RT Align Sample" );
	updateCorrectionPoints( progress );
}

// get max Intensiti
double AlignmentManager::getSrcMaxIntensity() {
	// chromatograms
	kome::objects::Chromatogram* chroms[] = {
		m_ctrlChrom,
		m_trmtChrom
	};
	unsigned int num = sizeof( chroms ) / sizeof( kome::objects::Chromatogram* );

	// get intensity
	double intensity = 0.0;
	for( unsigned int i = 0; i < num; i++ ) {
		// chromatogram
		kome::objects::Chromatogram* chrom = chroms[ i ];

		// intensity
		if( chrom != NULL ) {
			kome::core::XYData* xyData = chrom->getXYData();
			if( xyData != NULL ) {
				intensity = std::max( xyData->getMaxY(), intensity );
			}
		}
	}

	return intensity;
}

// get min RT
double AlignmentManager::getSrcMinRt() {
	// chromatograms
	kome::objects::Chromatogram* chroms[] = {
		m_ctrlChrom,
		m_trmtChrom
	};
	unsigned int num = sizeof( chroms ) / sizeof( kome::objects::Chromatogram* );

	// get RT
	double rt = -1.0;
	for( unsigned int i = 0; i < num; i++ ) {
		// chromatogram
		kome::objects::Chromatogram* chrom = chroms[ i ];

		// intensity
		if( chrom != NULL ) {
			kome::core::XYData* xyData = chrom->getXYData();
			if( xyData != NULL ) {
				double tmp = xyData->getMinX();
				if( rt < 0.0 || tmp < rt ) {
					rt = tmp;
				}
			}
		}
	}

	rt = MAX( rt, 0.0 );
	return rt;
}

// get max RT
double AlignmentManager::getSrcMaxRt() {
	// chromatograms
	kome::objects::Chromatogram* chroms[] = {
		m_ctrlChrom,
		m_trmtChrom
	};
	unsigned int num = sizeof( chroms ) / sizeof( kome::objects::Chromatogram* );

	// get RT
	double rt = 0.0;
	for( unsigned int i = 0; i < num; i++ ) {
		// chromatogram
		kome::objects::Chromatogram* chrom = chroms[ i ];

		// intensity
		if( chrom != NULL ) {
			kome::core::XYData* xyData = chrom->getXYData();
			rt = std::max( xyData->getMaxX(), rt );
		}
	}

	return rt;
}

// get max Intensiti
double AlignmentManager::getResultMaxIntensity() {
	// chromatograms
	kome::objects::Chromatogram* chroms[] = {
		m_ctrlChromResult,
		m_trmtChromResult
	};
	unsigned int num = sizeof( chroms ) / sizeof( kome::objects::Chromatogram* );

	// get intensity
	double intensity = 0.0;
	for( unsigned int i = 0; i < num; i++ ) {
		// chromatogram
		kome::objects::Chromatogram* chrom = chroms[ i ];

		// intensity
		if( chrom != NULL ) {
			kome::core::XYData* xyData = chrom->getXYData();
			if( xyData != NULL ) {
				intensity = std::max( xyData->getMaxY(), intensity );
			}
		}
	}

	return intensity;
}

// get min RT
double AlignmentManager::getResultMinRt() {
	// chromatograms
	kome::objects::Chromatogram* chroms[] = {
		m_ctrlChromResult,
		m_trmtChromResult
	};
	unsigned int num = sizeof( chroms ) / sizeof( kome::objects::Chromatogram* );

	// get RT
	double rt = -1.0;
	for( unsigned int i = 0; i < num; i++ ) {
		// chromatogram
		kome::objects::Chromatogram* chrom = chroms[ i ];

		// intensity
		if( chrom != NULL ) {
			kome::core::XYData* xyData = chrom->getXYData();
			if( xyData != NULL ) {
				double tmp = xyData->getMinX();
				if( rt < 0.0 || tmp < rt ) {
					rt = tmp;
				}
			}
		}
	}

	rt = MAX( rt, 0.0 );
	return rt;
}

// get max RT
double AlignmentManager::getResultMaxRt() {
	// chromatograms
	kome::objects::Chromatogram* chroms[] = {
		m_ctrlChromResult,
		m_trmtChromResult
	};
	unsigned int num = sizeof( chroms ) / sizeof( kome::objects::Chromatogram* );

	// get RT
	double rt = 0.0;
	for( unsigned int i = 0; i < num; i++ ) {
		// chromatogram
		kome::objects::Chromatogram* chrom = chroms[ i ];

		// intensity
		if( chrom != NULL ) {
			kome::core::XYData* xyData = chrom->getXYData();
			rt = std::max( xyData->getMaxX(), rt );
		}
	}

	return rt;
}

// set cursor position
void AlignmentManager::setCursorPos( const int pos ) {
	m_cursorPos = pos;
}

// get cursor position
int AlignmentManager::getCursorPos() {
	return m_cursorPos;
}

// set drag start position
void AlignmentManager::setDragStartPos( const int pos ) {
	m_startPos = pos;
}

// get drag start position
int AlignmentManager::getDragStartPos() {
	return m_startPos;
}

// set drag end position
void AlignmentManager::setDragEndPos( const int pos ) {
	m_endPos = pos;
}

// get drag end position
int AlignmentManager::getDragEndPos() {
	return m_endPos;
}

// push range
void AlignmentManager::pushRange( const double start, const double end ) {
	m_rangeStack.push( std::make_pair( start, end ) );
}

// pop range
void AlignmentManager::popRange() {
	if( m_rangeStack.size() > 0 ) {
		m_rangeStack.pop();
	}
}

// clear range
void AlignmentManager::clearRange() {
	while( m_rangeStack.size() > 0 ) {
		m_rangeStack.pop();
	}
}

// get start
double AlignmentManager::getStart() {
	if( m_rangeStack.size() == 0 ) {
		return -1.0;
	}
	return m_rangeStack.top().first;
}

// get end
double AlignmentManager::getEnd() {
	if( m_rangeStack.size() == 0 ) {
		return -1.0;
	}
	return m_rangeStack.top().second;
}

// get correction points
kome::core::XYData& AlignmentManager::getCorrectionPoints() {
	return m_corrPts;
}

// update correction points
void AlignmentManager::updateCorrectionPoints( kome::core::Progress& progress ) {
	// clear
	clearCorrection();

	// check alignment
	if( m_alignment == NULL ) {
		return;
	}

	// check chromatogram
	if( m_trmtChrom == NULL ) {
		return;
	}

	// group
	kome::objects::DataGroupNode* group = m_trmtChrom->getGroup();
	const double mz = m_trmtChrom->getOrgChromatogram()->getMz();

	// alignment
	std::vector< double > rts;
	std::vector< double > alignedRts;

	unsigned int num = group->getNumberOfSpectra();
	if( num > 0 ) {
		rts.reserve( num );
		alignedRts.reserve( num );
	}

	for( unsigned int i = 0; i < num; i++ ) {
		double rt = group->getSpectrum( i )->getRt();
		double alignedRt = m_alignment->convertRt( rt, mz );

		rts.push_back( rt );
		alignedRts.push_back( alignedRt );
	}

	if( progress.isStopped() ) {
		m_corrPts.clearPoints();
	}
	else {
		if( num > 0 ) {
			m_corrPts.reserve( num );
		}
		for( unsigned int i = 0; i < num; i++ ) {
			m_corrPts.addPoint( rts[ i ], alignedRts[ i ] );
		}

		progress.fill();
	}

	// update chromatogram
	kome::objects::Chromatogram* chroms[] = {
		m_ctrlChrom,
		m_ctrlChromResult,
		m_trmtChrom,
		m_trmtChromResult,
		m_corrDummyChrom
	};
	num = sizeof( chroms ) / sizeof( kome::objects::Chromatogram* );

	for( unsigned int i = 0; i < num; i++ ) {
		kome::objects::Chromatogram* chrom = chroms[ i ];
		if( chrom != NULL ) {
			chrom->deleteXYData();
			kome::plugin::PluginCallTool::onUpdateChromatogram( *chrom );
		}
	}

	// chromatogram
	m_corrDummyChrom = new CorrDummyChromatogram();

	kome::window::WindowTool::refresh();
}

// prepare alignment
bool AlignmentManager::prepareAlignment(						// @date 2013.04.04 <Add> Fujita  scaling用のitem, settingsを追加
		kome::plugin::PluginFunctionItem& alignment_item,
		kome::objects::SettingParameterValues* alignment_settings,
		kome::plugin::PluginFunctionItem& scaling_item,
		kome::objects::SettingParameterValues* scaling_settings,
		kome::core::Progress& progress
) {
	// check groups
	kome::objects::DataGroupNode* ctrlGroup = NULL;
	if( m_ctrlChrom != NULL ) {
		ctrlGroup = m_ctrlChrom->getGroup();
	}
	if( ctrlGroup == NULL ) {
		LOG_WARN( FMT( "Failed to get the control chromatogram." ) );
		return false;
	}

	kome::objects::DataGroupNode* trmtGroup = NULL;
	if( m_trmtChrom != NULL ) {
		trmtGroup = m_trmtChrom->getGroup();
	}
	if( trmtGroup == NULL ) {
		LOG_WARN( FMT( "Failed to get the treatment chromatogram." ) );
		return false;
	}
	
	// check old alignment object
	if( m_alignment != NULL ) {
		unsetAlignment();
	}

	

	// create parameters
	kome::objects::Parameters params;

	kome::plugin::PluginCallTool::setControlGroup( params, *ctrlGroup );
	kome::plugin::PluginCallTool::setGroup( params, *trmtGroup );
	if( alignment_settings != NULL ) {
		kome::plugin::PluginCallTool::setSettingValues( params, *alignment_settings );
	}

	// get alignment object
	kome::objects::Variant var = alignment_item.getCall()->invoke( &params );
	m_alignment = (kome::operation::Alignment*)var.prim.pt;

	// progress
	progress.createSubProgresses( 2 );

	// prepare
	if( m_alignment != NULL ) {

		// スペクトラムをスケーリングし、TICを再計算する
		kome::objects::Parameters params;

		// scaling of control TIC 
		m_RawCtrlSample = ctrlGroup->getSample();                                      // store raw control sample object in m_RawCtrlSample.        
		if ( m_flg == 0 ) {
			m_RawCtrlTics.clear();
			kome::objects::DataGroupNode* temp_DataGroupNode = m_RawCtrlSample->getGroup(0);  
			int spec_num = temp_DataGroupNode->getNumberOfSpectra();
			for ( int i = 0 ; i < spec_num ; i++ ) {										// store raw sample's TICs in m_RawCtrlTics vector.
				kome::objects::Spectrum* spec = temp_DataGroupNode->getSpectrum( i );
				m_RawCtrlTics.push_back(spec->getTotalIntensity());
			}
		}
			
		kome::plugin::PluginCallTool::setSample( params, *m_RawCtrlSample );           // sampleの情報をparamsに入れる  
		kome::plugin::PluginCallTool::setSettingValues( params, *scaling_settings );   // パラメータの情報をparamsに入れる。
			
		const char* alignment_name = alignment_item.getLongName();

		if ( &scaling_item != NULL && 0 == strcmp( alignment_name, ALIGNMENT_LONG_NAME) ){          // alignment_nameがALIGNMENT_LONG_NAMEと等しければスペクトラムのスケーリングを行う。
			int k = 0;
			var = scaling_item.getCall()->invoke( &params );				   // perform scaling of control sample            
			m_ScaledCtrlSample = (kome::objects::Sample*)var.prim.pt;      // spectra which have scaled TIC is stred in re_ScaledCtrlSample.
		}
		else {
			m_ScaledCtrlSample = m_RawCtrlSample;
		}

		// scaling of treatment TIC
		m_RawTrmtSample = trmtGroup->getSample();					   // store raw treatment sample object in m_RawTrmtSample. 
		
		if ( m_flg == 0 ) {
			m_RawTrmtTics.clear();
			kome::objects::DataGroupNode* temp_DataGroupNode = m_RawTrmtSample->getGroup(0);
			int spec_num = temp_DataGroupNode->getNumberOfSpectra();
			for ( int i = 0 ; i < spec_num ; i++ ) {										// store raw sample's TICs in m_RawCtrlTics vector.
				kome::objects::Spectrum* spec = temp_DataGroupNode->getSpectrum( i );
				m_RawTrmtTics.push_back(spec->getTotalIntensity());
			}
		}

		kome::plugin::PluginCallTool::setSample( params, *m_RawTrmtSample );

		alignment_name = alignment_item.getLongName();

		if ( &scaling_item != NULL && 0 == strcmp( alignment_name, ALIGNMENT_LONG_NAME ) ){
			var = scaling_item.getCall()->invoke( &params );					 // perform scaling of treatment sample  
			m_ScaledTrmtSample = (kome::objects::Sample*)var.prim.pt;		 // spectra which have scaled TIC is stred in re_ScaledTrmtSample. 
		}
		else {
			m_ScaledTrmtSample = m_RawTrmtSample;
		}

		// perform alignment
		m_alignment->prepare( m_ScaledCtrlSample, m_ScaledTrmtSample, progress.getSubProgress( 0 ) );

		m_flg = 1;
		
	}


	// update correction
	if( progress.isStopped() ) {
		delete m_alignment;
		m_alignment = NULL;			
		
		return true;
	}

	updateCorrectionPoints( *( progress.getSubProgress( 1 ) ));   // クロマトグラムの表示更新？
	if( !progress.isStopped() ) {
		progress.fill();
	}
	
	return true;
}
  
// reinstate TIC value
void AlignmentManager::ReinstateTicValue() {
	// control 
	if ( m_flg == 1 ){
		kome::objects::DataGroupNode* temp_DataGroupNode = m_RawCtrlSample->getGroup(0);
		int spec_num = temp_DataGroupNode->getNumberOfSpectra();
		for ( int i = 0 ; i < spec_num ; i++ ) {
			kome::objects::Spectrum* spec = temp_DataGroupNode->getSpectrum( i );
			spec->setTotalIntensity(m_RawCtrlTics[i]);
		}

		// treamtment
		temp_DataGroupNode = m_RawTrmtSample->getGroup(0);
		spec_num = temp_DataGroupNode->getNumberOfSpectra();
		for ( int i = 0 ; i < spec_num ; i++ ) {
			kome::objects::Spectrum* spec = temp_DataGroupNode->getSpectrum( i );
			spec->setTotalIntensity(m_RawTrmtTics[i]);
		}
	}
}	


// get instance
AlignmentManager& AlignmentManager::getInstance() {
	// create object (This is the only object.)
	static AlignmentManager mgr;

	return mgr;
}



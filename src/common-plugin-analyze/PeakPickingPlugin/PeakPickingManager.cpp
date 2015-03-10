/**
 * @file PeakPickingManager.cpp
 * @brief implements of PeakPickingManager
 *
 * @author S.Tanaka
 * @date 2006.10.11
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "PeakPickingManager.h"
#include "PeakDetectOperation.h"

#include <boost/bind.hpp>
#include <vector>		// @date 2011/07/04 <Add> OKADA

using namespace kome::labeling;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


#define SECTION								"Auto Labeling"
#define DEFAULT_CHARGE_KEY_PREFIX			"DEFAULT_CHARGE"
#define CHROM_ABS_INT_KEY					"CHROM_ABSOLUTE_THRESHOLD"
#define CHROM_REL_INT_KEY					"CHROM_RELATIVE_THRESHOLD"
#define CHROM_COUNT_KEY						"CHROM_MIN_COUNT"
#define SPEC_ABS_INT_KEY					"SPEC_ABSOLUTE_THRESHOLD"
#define SPEC_REL_INT_KEY					"SPEC_RELATIVE_THRESHOLD"
#define SPEC_COUNT_KEY						"SPEC_MIN_COUNT"
#define SPEC_PEAK_FILTER_FUNK				"spec_peak_filter"
#define SPECTRUM_PEAK_PICKING_FUNC_TYPE		"SPEC_PEAK_PICKING"
#define CHARGE_DETECT_FUNC_TYPE				"CHARGE_DETECT"

#define MAX_CHARGE							 8

// constructor
PeakPickingManager::PeakPickingManager() {
	m_targetName = "";
}

// destructor
PeakPickingManager::~PeakPickingManager() {
}

// @date 2011.01.25 <Add> M.Izumi <-
// sets parent peak picking function
void PeakPickingManager::setPeakDetectFunction( kome::plugin::PluginFunctionItem fun ) {
	m_peakFun = fun;
}

// get charge peak picking function
kome::plugin::PluginFunctionItem PeakPickingManager::getPeakDetectFunction() {
	return m_peakFun;
}

// set parent peak picking settings
void PeakPickingManager::setPeakDetectSettings( kome::objects::SettingParameterValues settings ) {
	m_peakSettings = settings;
}

// get parent peak picking settings
kome::objects::SettingParameterValues PeakPickingManager::getPeakDetectSettings() {
	return m_peakSettings;
}
// @date 2011.01.25 <Add> M.Izumi <-

// peak picking
void PeakPickingManager::detectPeaks(
		kome::objects::Spectrum* spec,
		kome::core::XYData* xyData,
		kome::plugin::PluginFunctionItem* peakPickingFun,
		kome::objects::SettingParameterValues* labelSettings,
		kome::plugin::PluginFunctionItem* chargeDetectFun,
		kome::objects::SettingParameterValues* chargeSettings
) {
	// check the parameter
	if( spec == NULL ) {
		return;
	}

	// get xy data
	kome::core::DataPoints pts;
	if( xyData == NULL ) {
		spec->getXYData( &pts, true );
		xyData = &pts;
	}

	// create peaks
	kome::objects::PeaksManager& pkMgr = kome::objects::PeaksManager::getInstance();
	kome::objects::Peaks* peaks = pkMgr.createPeaks( spec );

	// initialize peaks
	peaks->clearPoints();
	peaks->clearCharge();
	if( spec->getMsStage() > 1 ) {
		peaks->setPrecursor( spec->getPrecursor() );
	}

	// create parameters
	kome::objects::Parameters peakParams;

	kome::plugin::PluginCallTool::setSpectrum( peakParams, *spec );
	kome::plugin::PluginCallTool::setXYData( peakParams, *xyData );
	kome::plugin::PluginCallTool::setPeaks( peakParams, *peaks );

	peaks->setDataPoints( *xyData );

	if( labelSettings != NULL ) {
		kome::plugin::PluginCallTool::setSettingValues( peakParams, *labelSettings );
	}

	// peak detection
	if( peakPickingFun != NULL) {
		peakPickingFun->getCall()->invoke( &peakParams );
	}

	// properties
	kome::core::Properties& props = spec->getProperties();
	props.setIntValue( "Peaks Count", (int)peaks->getLength() );
	if( peaks->getLength() == 0 ) {
		props.setValue( "Max Peak Value", "" );
	}
	else {
		double maxInt = 0.0;
		for( unsigned int i = 0; i < peaks->getLength(); i++ ) {
			maxInt = std::max( maxInt, peaks->getY( i ) );
		}		
		props.setDoubleValue( "Max Peak Value", maxInt );
	}

	// charge detection
	if( spec->getMsStage() > 1 ) {
		// auto charge detect
		if( chargeDetectFun != NULL ) {
			kome::objects::Parameters chargeParams;

			kome::plugin::PluginCallTool::setSpectrum( chargeParams, *spec );
			kome::plugin::PluginCallTool::setXYData( chargeParams, *xyData );
			kome::plugin::PluginCallTool::setPeaks( chargeParams, *peaks );

			if( chargeSettings != NULL ) {
				kome::plugin::PluginCallTool::setSettingValues( chargeParams, *chargeSettings );
			}

			// charge detection
			chargeDetectFun->getCall()->invoke( &chargeParams );
		}
	}	

	// broadcast
	kome::plugin::PluginCallTool::onUpdateSpecPeaks( *spec );
	kome::plugin::PluginCallTool::onUpdateSpectrum( *spec );
}

// peak picking
void PeakPickingManager::detectPeaks(
		kome::objects::Chromatogram* chrom,
		kome::core::XYData* xyData,
		kome::plugin::PluginFunctionItem* fun,
		kome::objects::SettingParameterValues* labelSettings
) { 
	// check the parameter
	if( chrom == NULL || fun == NULL ) {
		return;
	}

	// get xy data
	kome::core::DataPoints pts;
	if( xyData == NULL ) {
		chrom->getXYData( &pts, true );
		xyData = &pts;
	}

	// create peaks
	kome::objects::PeaksManager& pkMgr = kome::objects::PeaksManager::getInstance();
	kome::objects::Peaks* peaks = pkMgr.createPeaks( chrom );

	// page
	kome::plugin::SettingsPage* page = fun->getSettingsPage();

	// create parameters
	kome::objects::Parameters params;

	kome::plugin::PluginCallTool::setChromatogram( params, *chrom );
	kome::plugin::PluginCallTool::setXYData( params, *xyData );
	kome::plugin::PluginCallTool::setPeaks( params, *peaks );
	peaks->setDataPoints( *xyData );

	if( labelSettings != NULL ) {
		kome::plugin::PluginCallTool::setSettingValues( params, *labelSettings );
	}
	
	// @date 2011.08.24 <Mod> M.Izumi ->
	// SPEC 82175 : Switching the Chromatogram Filter (Application down) 
	// peak detection
	kome::plugin::PluginCall* call = fun->getCall();
	if( call != NULL ){
		call->invoke( &params );
	}
	// <-

	// broadcast
	kome::plugin::PluginCallTool::onUpdateChromPeaks( *chrom );
}

// get target spectra
void PeakPickingManager::getTargetSpectra(
		kome::objects::Variant& activeObj,
		std::vector< kome::objects::Spectrum* >& spectra,
		PeakPickingTarget target
) {
	// manager
	kome::objects::ActiveObjectsManager& aoMgr = kome::objects::ActiveObjectsManager::getInstance();
	
	// active object
	kome::objects::MsDataVariant obj( activeObj );
	kome::objects::Spectrum* spec = obj.getSpectrum();
	kome::objects::DataGroupNode* group = obj.getGroup();
	kome::objects::Sample* sample = obj.getSample();

	// get spectra
	if( target == TARGET_CURRENT_SPECTRUM ) {
		if( spec != NULL ) {
			spectra.push_back( spec );
		}
	}
	else if( target == TARGET_FILTERED_SPECTRA ) {
		// active spectra
		if( sample != NULL ) {
			kome::objects::DataSet* s = aoMgr.getFilteredDataSet( sample );
			if( s != NULL ) {
				for( unsigned int i = 0; i < s->getNumberOfSpectra(); i++ ) {
					spectra.push_back( s->getSpectrum( i ) );
				}
			}
		}
	}
	else {
		// get all spectra
		if( sample != NULL ) {
			kome::objects::DataSet s( sample->getRootDataGroupNode() );
			sample->getRootDataGroupNode()->getDataSet( &s );

			// each spectra
			for( unsigned int i = 0; i < s.getNumberOfSpectra(); i++ ) {
				kome::objects::Spectrum* spec = s.getSpectrum( i );
				if( ( target != TARGET_MS_SPECTRA || spec->getMsStage() == 1 )
						&& ( target != TARGET_MSMS_SPECTRA || spec->getMsStage() > 1 ) ) {
					spectra.push_back( spec );
				}
			}
		}
	}
}

// save peaks
bool PeakPickingManager::savePeaks(
		kome::objects::Peaks2D& peaks,
		const char* filePath,
		kome::core::Progress& progress
) {
	// open file
	FILE* fp = fileopen( filePath, "w" );
	if( fp == NULL ) {
		return false;
	}

	// header
	fprintf( fp, "RT\tm/z\tIntensity\tArea\n" );

	// progress
	if( peaks.getNumberOfPeaks() == 0 ) {
		progress.fill();
		return true;
	}
	progress.setRange( 0, peaks.getNumberOfPeaks() );

	// write peaks
	for( unsigned int i = 0; i < peaks.getNumberOfPeaks(); i++ ) {
		// peak
		kome::objects::Peak2DElement* peak = peaks.getPeak( i );

		const double rt = peak->getRt();
		const double mz = peak->getMz();
		const double intensity = peak->getIntensity();

		// write
		if( rt >= 0.0 && mz >= 0.0 && intensity >= 0.0 ) {
			fprintf( fp, "%f\t%f\t%f\n", rt, mz, intensity );
		}

		progress.setPosition( i + 1 );
	}

	// close
	fflush( fp );
	fclose( fp );
	progress.fill();

	return true;
}


// get instance
PeakPickingManager& PeakPickingManager::getInstance() {
	// create object
	static PeakPickingManager mgr;

	return mgr;
}


// ターゲットの値を名前に変換して返す
std::string PeakPickingManager::getTargetName( PeakPickingTarget target ){
	
	if( target == TARGET_CURRENT_SPECTRUM ){
		m_targetName = "CURRENT SPECTRUM";
	}else if( target == TARGET_MS_SPECTRA ){
		m_targetName = "MS SPECTRA";
	}else if( target == TARGET_MSMS_SPECTRA ){
		m_targetName = "MSMS SPECTRA";
	}else if( target == TARGET_FILTERED_SPECTRA ){
		m_targetName = "FILTERED SPECTRA";
	}else if( target == TARGET_ALL_SPECTRA ){
		m_targetName = "ALL SPECTRA";
	}else if( target == TARGET_UNKNOWN ){
		m_targetName = "UNKNOWN";
	}

	return m_targetName;
}

// execute peak filtering
bool PeakPickingManager::executePeakFiltering(
	std::vector<kome::objects::Spectrum*>spectra,
	const char* peak_filter
){
	bool bflg = true;
	// manager
	kome::objects::PeaksManager& pkMgr = kome::objects::PeaksManager::getInstance();

	// array size
	const unsigned int num = spectra.size();

	// peaks
	std::vector< kome::objects::Peaks* > peaks;
	peaks.resize( num );
	for( unsigned int i = 0; i < spectra.size(); i++ ) {
		kome::objects::Spectrum* s = spectra[ i ];
		kome::objects::Peaks* p = pkMgr.getPeaks( s );

		peaks[ i ] = p;
	}
	    
	// filter
	kome::plugin::PluginCallTool::filterSpecPeaks(
		( num == 0 ? NULL : &( spectra[ 0 ] ) ),
		( num == 0 ? NULL : &( peaks[ 0 ] ) ),
		num,
		peak_filter
	);
	     
	return bflg;
}

// calculate noise for S/N Filtering
double PeakPickingManager::NoiseCauculation(double halfWidth, double threshold, kome::core::XYData* xyData){

    // 移動平均計算 
	std::vector<double> rawIntArray; // 生データのintensity
	std::vector<double> SmoIntArray; // smoothing後のintensityを入れる

	int length = xyData->getLength();   // データ点数
	double max_intensity = xyData->getMaxY();

	for (int n = 0; n < length; n++)
		{
			rawIntArray.push_back(xyData->getY(n));   // 生データの記録
			SmoIntArray.push_back(-1.0);              // 一先ず-1.0を入れて初期化
		}

	double peakThreshold = 1.0;       // 考慮するピークintensityの閾値係数
	
	peakThreshold = max_intensity*(double(threshold)*0.01);   // この値のintensityを持つピークのみノイズ計算の対象にする。

	for (int n = halfWidth-1; n < length - halfWidth-1; n++) {    // vectorのindexは0から始まるので-1する
			
		double windSum = 0.0;	// n ± halfWidth の範囲にintensityが閾値以上の点がない時だけintensityの合計を求める
		for (int diff = -halfWidth; diff <= halfWidth; diff++) {
			int k = n + diff;
			length;
			int number = rawIntArray.size();
			double yVal = rawIntArray[n + diff+1];
			if (yVal > peakThreshold)      // 生データのintenistyの値が閾値以上なら、windSumに-1を入れて、「移動合計」計算を止める。
			{
				windSum = -1.0;
				break;
			}
			windSum += yVal;               // 生データのintenistyの値が閾値以下の場合、「移動合計」を計算する。
		}

		if (windSum !=  -1) {	// 上のfor文でintensityが閾値以上の点があると-1
			SmoIntArray[n] = windSum/(2*halfWidth + 1);       // スムージング後の値
		}
	}

	// ここまでで、smoothingしたintensityが求まったのでノイズを計算する。
	double sumDeviation = 0;
	int count = 0;
	for (int n = 0; n < length; n++) {
		if (SmoIntArray[n] != -1) {     // -1が入っている要素は、smoothing itnensityが計算されていない箇所   
			double deviation = rawIntArray[n] - SmoIntArray[n];    // raw data とsmoothing後のintensity差
			sumDeviation += deviation*deviation;                   // 差の2乗を計算
			count++;
		}
	}

	double stdDeviation = -1.0;
	if (count > 0) {   // count > 0 なら、つまり分散が計算されていれば
		stdDeviation = sqrt(sumDeviation/(double)count);
	}

	return stdDeviation;
};


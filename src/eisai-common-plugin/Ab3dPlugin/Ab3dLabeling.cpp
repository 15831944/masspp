/**
 * @file Ab3dLabeling.cpp
 * @brief implements of Ab3dLabeling class
 *
 * @author S.Tanaka
 * @date 2007.10.11
 * @data 2011.03.02 <Del> M.Aihara
 *       delete static Timer
 * @data 2011.03.02 <Add> M.Aihara
 *       add TBB parallel code
 * @data 2011.03.02 <Mod> M.Aihara
 *       modify TBB parallel code
 * @data 2011.03.02 <Add> M.Aihara
 *       add TBB mutex code
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "Ab3dLabeling.h"

// @data 2011.03.02 <Add> M.Aihara ->
// add TBB parallel code
#ifdef  ENABLE_MASSPP_PARALLEL_TBB_MODE
#include "tbb/parallel_for.h"
#include "tbb/parallel_reduce.h"
#include "tbb/blocked_range.h"
#include "tbb/spin_mutex.h"
using namespace tbb;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG

// @data 2011.03.02 <Add> M.Aihara ->
// add TBB mutex code
namespace
{
	typedef spin_mutex AB3D_ParallelMutexType;
	AB3D_ParallelMutexType  AB3D_ParallelMutex_01;
	AB3D_ParallelMutexType  AB3D_ParallelMutex_02;
	AB3D_ParallelMutexType  AB3D_ParallelMutex_03;
}
// <Add> M.Aihara <-

#endif // ENABLE_MASSPP_PARALLEL_TBB_MODE
// <Add> M.Aihara <-

using namespace kome::ab3d;


#define SPEC_SMOOTHING_PARAM_NAME			   "spec_smoothing"
#define SPEC_PEAK_DETECTOR_PARAM_NAME		   "spec_peak_detector"
#define MZ_MERGE_DISTANCE_PARAM_NAME			"mz_d"
#define RT_MERGE_DISTANCE_PARAM_NAME			"rt_d"
#define PEAK_END_HEIGHT_PARAM_NAME				"end_height"
#define DELETE_SHOULDER_FLAG_PARAM_NAME		 "del_shoulder"
#define SHOULDER_MASS_RANGE_PARAM_NAME		  "shld_mz_range"
#define SHOULDER_RT_RANGE_PARAM_NAME			"shld_rt_range"
#define INT_THRESHOLD_TYPE_PARAM_NAME		   "int_type"
#define INT_THRESHOLD_PARAM_NAME				"int_threshold"

#define SMOOTHING_FUNC_NAME					 "XYDATA_FILTER"
#define SPECTRUM_PEAK_DETECTOR_FUNC_NAME		"SPEC_PEAK_PICKING"
#define CHROMATOGRAM_PEAK_DETECTOR_FUNC_NAME	"CHROM_PEAK_PICKING"


// conversion array
kome::core::ConversionElement< std::string, Ab3dLabeling::ThresholdType > g_ab3dLabelingIntThresholdTypeConversionArray[] = {
	{ "abs",  Ab3dLabeling::THRESHOLD_ABS },
	{ "rel",  Ab3dLabeling::THRESHOLD_REL },
	{ "auto", Ab3dLabeling::THRESHOLD_AUTO }
};

// constructor
Ab3dLabeling::Ab3dLabeling( kome::objects::SettingParameterValues* settings ) {
	// settings
	m_settings = settings;
	std::string s;

	// plug-in manager
	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();

	// spectrum smoothing
	const char* specSmoothing = NULL;
	if( m_settings != NULL ) {
		specSmoothing = m_settings->getParameterValue( SPEC_SMOOTHING_PARAM_NAME );
	}
	m_specSmoothing = plgMgr.getFunctionItem( SMOOTHING_FUNC_NAME, NVL( specSmoothing, "" ) );

	// spectrum peak detector
	const char* specPeakDetector = NULL;
	if( m_settings != NULL ) {
		specPeakDetector = m_settings->getParameterValue( SPEC_PEAK_DETECTOR_PARAM_NAME );
		if( specPeakDetector != NULL ){

			// The "auto" spectrum peak detector is no longer supported in Mass++, so if it is specified in the INI
			// file, set specPeakDetector to NULL, which will force "local" to be used as the peak detector.
			if (!strcmp(specPeakDetector, "auto"))
			{
				specPeakDetector = NULL;
			}
		}
	}
	m_specPeakDetector = plgMgr.getFunctionItem( SPECTRUM_PEAK_DETECTOR_FUNC_NAME, NVL( specPeakDetector, "local" ) );

	// merge distance
	m_mzDistance = 0.25;
	if( m_settings != NULL ) {
		m_mzDistance = m_settings->getDoubleValue( MZ_MERGE_DISTANCE_PARAM_NAME, 0.25 );
	}

	m_rtDistance = 0.5;
	if( m_settings != NULL ) {
		m_rtDistance = m_settings->getDoubleValue( RT_MERGE_DISTANCE_PARAM_NAME, 0.5 );
	}

	// height percentage
	m_endHeight = 10.0;
	if( m_settings != NULL ) {
		m_endHeight = m_settings->getDoubleValue( PEAK_END_HEIGHT_PARAM_NAME, 10.0 );
	}

	// shoulder
	m_deleteShoulderFlag = false;
	if( m_settings != NULL ) {
		m_deleteShoulderFlag = m_settings->getBoolValue( DELETE_SHOULDER_FLAG_PARAM_NAME, false );
	}

	m_shoulderMzRange = 0.5;
	if( m_settings != NULL ) {
		m_shoulderMzRange = m_settings->getDoubleValue( SHOULDER_MASS_RANGE_PARAM_NAME, 0.5 );
	}

	m_shoulderRtRange = 0.5;
	if( m_settings != NULL ) {
		m_shoulderRtRange = m_settings->getDoubleValue( SHOULDER_RT_RANGE_PARAM_NAME, 0.5 );
	}

	// intensity threshold type
	const char* intThresholdType = NULL;
	if( m_settings != NULL ) {
		intThresholdType = m_settings->getParameterValue( INT_THRESHOLD_TYPE_PARAM_NAME );
	}
	s = tolowercase( intThresholdType );
	m_intThresholdType = CONVERT_FROM_ARRAY( s, THRESHOLD_ABS, g_ab3dLabelingIntThresholdTypeConversionArray );

	// intensity threshold
	m_intThreshold = 1.0;
	if( m_settings != NULL ) {
		m_intThreshold = m_settings->getDoubleValue( INT_THRESHOLD_PARAM_NAME, 1.0 );
	}

	// range
	m_minRt = -1.0;
	m_maxRt = -1.0;
	m_minMz = -1.0;
	m_maxMz = -1.0;

	// timer
	// @data 2011.03.02 <Del> M.Aihara ->
	// m_specTimer = NULL;
	// m_peaksTimer = NULL;
	// m_mergeTimer = NULL;
	// <Del> M.Aihara <-
}

// destructor
Ab3dLabeling::~Ab3dLabeling() {
}

// set range
void Ab3dLabeling::setRange(
		const double minRt,
		const double maxRt,
		const double minMz,
		const double maxMz
) {
	m_minRt = minRt;
	m_maxRt = maxRt;
	m_minMz = minMz;
	m_maxMz = maxMz;
}

// execute labeling
void Ab3dLabeling::executeLabeling(
		kome::objects::DataSet& dataSet,
		kome::objects::Peaks2D& peaks,
		kome::core::Progress& progress
) {
	// timer
	// @data 2011.03.02 <Del> M.Aihara ->
	// m_specTimer = progress.createTimer( "AB3D Labeling : Getting Spectrum" );
	// m_peaksTimer = progress.createTimer( "AB3D Labeling : Peak Detection" );
	// m_mergeTimer = progress.createTimer( "AB3D Labeling : Peak Merge" );
	// <Del> M.Aihara <-

	// log
	std::string msg = FMT(
		"AB3D Labeling [spec_filter=%s, spec_peaks=%s, mz_tol=%.2f, rt_tol=%.2f, end_height=%.2f%%",
		m_specSmoothing == NULL ? "none" : m_specSmoothing->getShortName(),
		m_specPeakDetector->getShortName(),
		m_mzDistance,
		m_rtDistance,
		m_endHeight
	);

	if( m_deleteShoulderFlag ) {
		msg.append( FMT( ", shoulder_mz_range=%.2f", m_shoulderMzRange ) );
		msg.append( FMT( ", shoulder_rt_range=%.2f", m_shoulderRtRange ) );
	}

	if( m_intThresholdType == THRESHOLD_AUTO ) {
		msg.append( ", int_threshold=auto" );
	}
	else {
		msg.append( FMT( ", int_threshold=%.2f", m_intThreshold ) );
		if( m_intThresholdType == THRESHOLD_REL ) {
			msg.append( "(rel)" );
		}
		else {
			msg.append( "(abs)" );
		}
	}
	msg.append( "]" );
	LOG_INFO( msg );

	// data set
	kome::objects::DataSet ds( dataSet.getGroup() );
	for( unsigned int i = 0; i < dataSet.getNumberOfSpectra(); i++ ) {
		kome::objects::Spectrum* spec = dataSet.getSpectrum( i );
		if( spec->hasChromatogram() && spec->getMsStage() == 1 ) {
			ds.addSpectrum( spec );
		}
	}
	// extend for SRM
	if (ds.getNumberOfSpectra() == 0) {
		for( unsigned int i = 0; i < dataSet.getNumberOfSpectra() && !progress.isStopped(); i++ ) { // @date 2013.11.22 <Mod> M.Izumi
			kome::objects::Spectrum* spec = dataSet.getSpectrum( i );
			if( spec->hasChromatogram() && spec->getMsStage() == 2 ) {
				ds.addSpectrum( spec );
			}
		}
	}

	// labeling
	if( dataSet.getNumberOfSpectra() > 0 ) {
		detectPeaks( ds, peaks, progress );
	}
	if( progress.isStopped() ){ // @date 2013.11.22 <Mod> M.Izumi
		progress.fill();
	}
}

// detect peaks
void Ab3dLabeling::detectPeaks(
		kome::objects::DataSet& dataSet,
		kome::objects::Peaks2D& peaks,
		kome::core::Progress& progress
) {
	// progress
	int step = 2;
	progress.createSubProgresses( step );

	// spectra
	const double tol = m_rtDistance * 5.0;
	double minRt = m_minRt;
	if( minRt >= 0.0 ) {
		minRt = std::max( 0.0, minRt - tol );
	}
	double maxRt = m_maxRt;
	if( maxRt >= 0.0 ) {
		maxRt = maxRt + tol;
	}

	kome::objects::DataSet spectra( dataSet.getGroup() );

	for( unsigned int i = 0; i < dataSet.getNumberOfSpectra(); i++ ) {
		kome::objects::Spectrum* spec = dataSet.getSpectrum( i );
		const double rt = spec->getRt();

		if( ( minRt < 0.0 || rt >= minRt ) && ( maxRt < 0.0 || rt <= maxRt )
				&& spec->getMsStage() <= 2 && spec->hasChromatogram() ) {
			spectra.addSpectrum( spec );
		}
	}

	// spectrum peak picking
	unsigned int specNum = spectra.getNumberOfSpectra();
	if( specNum == 0 ) {
		return;
	}

	kome::core::XYData** pointsArray = new kome::core::XYData*[ specNum ];
	for( unsigned int i = 0; i < specNum; i++ ) {
		pointsArray[ i ] = new kome::core::DataPoints();
	}
	kome::objects::Peaks* peaksArray = new kome::objects::Peaks[ specNum ];

	detectSpectrumPeaks( spectra, pointsArray, peaksArray, *( progress.getSubProgress( 0 ) ) );

	// merge
	if( !progress.isStopped() ) {
		mergePeaks( spectra, peaks, pointsArray, peaksArray, *( progress.getSubProgress( 1 ) ) );
	}

	// filter
	for( int i = (int)peaks.getNumberOfPeaks() - 1; i >= 0; i-- ) {
		kome::objects::Peak2DElement* peak = peaks.getPeak( i );

		if( ( m_minRt >= 0.0 && peak->getRt() < m_minRt )
				|| ( m_maxRt >= 0.0 && peak->getRt() > m_maxRt )
				|| ( m_minMz >= 0.0 && peak->getMz() < m_minMz )
				|| ( m_maxMz >= 0.0 && peak->getMz() > m_maxMz ) ) {
			peaks.removePeak( i );
		}
	}

	// fill
	if( !progress.isStopped() ) {
		progress.fill();
	}

	// delete array
	for( unsigned int i = 0; i < specNum; i++ ) {
		delete pointsArray[ i ];
	}
	delete[] pointsArray;
	delete[] peaksArray;
}

// @data 2011.03.02 <Add> M.Aihara ->
// add TBB parallel code
#ifdef  PARALLEL_DETECT_TBB
void Ab3dLabeling::parallel_detectSpectrumPeaks(
		Ab3dLabeling* ab3ptr,
		kome::objects::DataSet& dataSet,
		kome::core::XYData** pointsArray,
		kome::objects::Peaks* peaksArray,
		kome::core::Progress& progress,
		double minInt,
		int i,
		int &pos
){

	kome::objects::Spectrum* spec = dataSet.getSpectrum( i );
	kome::objects::Peaks* peaks = peaksArray + i;

	if( !progress.isStopped() ) {
		kome::core::XYData* points = pointsArray[ i ];
		ab3ptr->getSpectrumData( *spec, *points );
		peaks->setDataPoints( *points );
		ab3ptr->detectSpectrumPeaks( *spec, *points, *peaks, minInt );

		{
			// @data 2011.03.02 <Add> M.Aihara ->
			// add TBB mutex code
			AB3D_ParallelMutexType::scoped_lock lock( AB3D_ParallelMutex_01 );
			// <Add> M.Aihara
			pos += 1;
			progress.setPosition( pos );

		}
	}
}
#endif  // PARALLEL_DETECT_TBB
// <Add> M.Aihara <-

// detect spectrum peaks
void Ab3dLabeling::detectSpectrumPeaks(
		kome::objects::DataSet& dataSet,
		kome::core::XYData** pointsArray,
		kome::objects::Peaks* peaksArray,
		kome::core::Progress& progress
) {
	// check members
	if( m_specPeakDetector == NULL ) {
		progress.fill();
		return;
	}

	// minimum intensity
	double minInt = getMinIntensity( dataSet );

	// sample
	kome::objects::Sample* smpl = dataSet.getSample();

	// peak detection
	progress.setRange( 0, dataSet.getNumberOfSpectra() );
	progress.setPosition( 0 );
	int pos = 0;
	int num = (int)dataSet.getNumberOfSpectra();

/// #pragma omp parallel for schedule( dynamic, 1 ) shared( pos ) if( smpl != NULL && smpl->isParallelReadable() )

// @data 2011.03.02 <Mod> M.Aihara ->
// modify TBB parallel code
#ifdef  PARALLEL_DETECT_TBB
	ApplyParallel_detectSpectrumPeaks apdsp(this, dataSet,pointsArray,peaksArray,progress,minInt,pos);
	parallel_for( blocked_range<size_t>(0, num, 1000 ), apdsp );
#else
	for( int i = 0; i < num; i++ ) {
		// spectrum, peaks
		kome::objects::Spectrum* spec = dataSet.getSpectrum( i );
		kome::objects::Peaks* peaks = peaksArray + i;

		if( !progress.isStopped() ) {
			kome::core::XYData* points = pointsArray[ i ];
			getSpectrumData( *spec, *points );
			peaks->setDataPoints( *points );
			detectSpectrumPeaks( *spec, *points, *peaks, minInt );

			pos += 1;
			progress.setPosition( pos );
		}
	}
#endif // PARALLEL_DETECT_TBB
// <Mod> M.Aihara <-

	progress.fill();
}

// get spectrum data
void Ab3dLabeling::getSpectrumData( kome::objects::Spectrum& spec, kome::core::XYData& xyData ) {
	// smoothing settings
	kome::objects::SettingParameterValues* smoothingSettings = NULL;
	if( m_settings != NULL && m_specSmoothing != NULL ) {
		smoothingSettings = m_settings->getSubParameterValues( SPEC_SMOOTHING_PARAM_NAME, m_specSmoothing->getShortName() );
	}

	// data points
	if( m_specSmoothing == NULL ) {
		// @data 2011.03.02 <Del> M.Aihara
		// m_specTimer->start();
		spec.getXYData( &xyData, false );
		// @data 2011.03.02 <Del> M.Aihara
		// m_specTimer->stop();
	}
	else {
		// @data 2011.03.02 <Del> M.Aihara
		// m_specTimer->start();
		kome::core::DataPoints tmpDps;		
		spec.getXYData( &tmpDps, false );

		// smoothing
		kome::objects::Parameters smoothingParams;

		kome::plugin::PluginCallTool::setXYData( smoothingParams, tmpDps );
		kome::plugin::PluginCallTool::setUpdatedXYData( smoothingParams, xyData );
		if( smoothingSettings != NULL ) {
			kome::plugin::PluginCallTool::setSettingValues( smoothingParams, *smoothingSettings );			
		}
		m_specSmoothing->getCall()->invoke( &smoothingParams );

		// @data 2011.03.02 <Del> M.Aihara
		// m_specTimer->stop();
	}
}

// detect spectrum peaks
void Ab3dLabeling::detectSpectrumPeaks(
		kome::objects::Spectrum& spec,
		kome::core::XYData& xyData,
		kome::objects::Peaks& peaks,
		const double minInt
) {
	// peak picking settings
	kome::objects::SettingParameterValues* peakSettings = NULL;
	if( m_settings != NULL ) {
		peakSettings = m_settings->getSubParameterValues( SPEC_PEAK_DETECTOR_PARAM_NAME, m_specPeakDetector->getShortName() );
	}

	// peak detection
	kome::objects::Parameters peakParams;	
	kome::plugin::PluginCallTool::setSpectrum( peakParams, spec );
	kome::plugin::PluginCallTool::setXYData( peakParams, xyData );
	kome::plugin::PluginCallTool::setPeaks( peakParams, peaks );
	if( peakSettings != NULL ) {
		kome::plugin::PluginCallTool::setSettingValues( peakParams, *peakSettings );
	}

	// @data 2011.03.02 <Del> M.Aihara
	// m_peaksTimer->start();
	m_specPeakDetector->getCall()->invoke( &peakParams );
	// @data 2011.03.02 <Del> M.Aihara
	// m_peaksTimer->stop();

	// filter
	peaks.filter( minInt, 0.0 );
}

// @data 2011.03.02 <Add> M.Aihara ->
// add TBB parallel code
#ifdef  PARALLEL_MERGE_TBB
void Ab3dLabeling::parallel_mergePeaks(
		Ab3dLabeling* ab3ptr,
		kome::objects::DataSet& dataSet,
		kome::objects::Peaks2D& peaks,
		kome::core::XYData** pointsArray,
		kome::objects::Peaks* peaksArray,
		kome::core::Progress& progress,
		kome::core::Progress* pgrs0,
		int i,
		int &pos,
		std::vector<kome::objects::Peak2DElement*>& m_savePeaks)
{
	{
		if( !progress.isStopped() ) {
			// spectrum
			kome::objects::Spectrum* spec = dataSet.getSpectrum( i );
			double specRt = spec->getRt();

			kome::objects::Peaks* specPeaks = peaksArray + i;
			kome::core::XYData* specData = pointsArray[ i ];

			// each peaks
			for( int j = 0; j < (int)specPeaks->getLength(); j++ ) {
				kome::objects::PeakElement* specPeak = specPeaks->getPeak( j );

				double mz = specPeak->getX();
				double intensity = specPeak->getY();

				kome::objects::Peak2DElement peak( NULL );
				peak.setRt( specRt, specRt, specRt );
				peak.setMz( mz, mz, mz );
				peak.setIntensity( intensity );

				// merge
				bool ret = ab3ptr->mergeSpectrumPeaks( specPeaks, &peak, specRt, intensity );

				for( int k = (int)i - 1;
						ret && k >= 0
							&& dataSet.getSpectrum( k )->getRt() >= peak.getStartRt() - ab3ptr->m_rtDistance;
						k--
				) {
					ret = ab3ptr->mergeSpectrumPeaks( peaksArray + k, &peak, dataSet.getSpectrum( k )->getRt(), intensity );
				}

				for( int k = (int)i + 1;
						ret && k < (int)dataSet.getNumberOfSpectra()
							&& dataSet.getSpectrum( k )->getRt() <= peak.getEndRt() + ab3ptr->m_rtDistance;
						k++
				) {
					ret = ab3ptr->mergeSpectrumPeaks( peaksArray + k, &peak, dataSet.getSpectrum( k )->getRt(), intensity );
				}

				// add peak
				if( ret ) {
//#pragma omp critical ( createPeakByAB3D )
					{
						kome::objects::Peak2DElement* p = peaks.createPeak_TBB(
							peak.getRt(),
							peak.getStartRt(),
							peak.getEndRt(),
							peak.getMz(),
							peak.getStartMz(),
							peak.getEndMz(),
							peak.getIntensity()
						);
						m_savePeaks.push_back( p );
					}

				}
			}
			{
				// @data 2011.03.02 <Add> M.Aihara ->
				// add TBB mutex code
				AB3D_ParallelMutexType::scoped_lock lock( AB3D_ParallelMutex_03 );
				// <Add> M.Aihara <-
				pos += 1;
				pgrs0->setPosition( pos );
			}
		}
	}
}
#endif //  PARALLEL_MERGE_TBB
// <Add> M.Aihara <-

// merge peaks
void Ab3dLabeling::mergePeaks(
		kome::objects::DataSet& dataSet,
		kome::objects::Peaks2D& peaks,
		kome::core::XYData** pointsArray,
		kome::objects::Peaks* peaksArray,
		kome::core::Progress& progress
) {
	// start timer
	// @data 2011.03.02 <Del> M.Aihara
	// m_mergeTimer->start();

	// progress
	kome::core::Progress* pgrs0 = &progress;
	if( m_deleteShoulderFlag ) {
		progress.createSubProgresses( 2 );
		pgrs0 = progress.getSubProgress( 0 );
	}
	
	// the number of spectra
	int num = (int)dataSet.getNumberOfSpectra();

	// merge
	int pos = 0;
	pgrs0->setPosition( pos );

// #pragma omp parallel for schedule( dynamic, 1 ) shared( pos )

// @data 2011.03.02 <Mod> M.Aihara ->
// modify TBB parallel code
#ifdef  PARALLEL_MERGE_TBB
	ApplyParallel_mergePeaks apmgp(this,dataSet,peaks,pointsArray,peaksArray,progress,pgrs0,pos);
	parallel_reduce(  blocked_range<size_t>(0, num, 800 ), apmgp );
	peaks.push_back_TBB( apmgp.m_savePeak );
	//for( int i = 0; i < num; i++ ) {
	//	parallel_mergePeaks(this,dataSet,peaks,pointsArray,peaksArray,progress,i,pos);
	//}
#else

	for( int i = 0; i < num; i++ ) {
		if( !progress.isStopped() ) {
			// spectrum
			kome::objects::Spectrum* spec = dataSet.getSpectrum( i );
			double specRt = spec->getRt();

			kome::objects::Peaks* specPeaks = peaksArray + i;
			kome::core::XYData* specData = pointsArray[ i ];

			// each peaks
			for( int j = 0; j < (int)specPeaks->getLength(); j++ ) {
				kome::objects::PeakElement* specPeak = specPeaks->getPeak( j );

				double mz = specPeak->getX();
				double intensity = specPeak->getY();

				kome::objects::Peak2DElement peak( NULL );
				peak.setRt( specRt, specRt, specRt );
				peak.setMz( mz, mz, mz );
				peak.setIntensity( intensity );

				// merge
				bool ret = mergeSpectrumPeaks( specPeaks, &peak, specRt, intensity );

				for( int k = (int)i - 1;
						ret && k >= 0
							&& dataSet.getSpectrum( k )->getRt() >= peak.getStartRt() - m_rtDistance;
						k--
				) {
					ret = mergeSpectrumPeaks( peaksArray + k, &peak, dataSet.getSpectrum( k )->getRt(), intensity );
				}

				for( int k = (int)i + 1;
						ret && k < (int)dataSet.getNumberOfSpectra()
							&& dataSet.getSpectrum( k )->getRt() <= peak.getEndRt() + m_rtDistance;
						k++
				) {
					ret = mergeSpectrumPeaks( peaksArray + k, &peak, dataSet.getSpectrum( k )->getRt(), intensity );
				}

				// add peak
				if( ret ) {
#pragma omp critical ( createPeakByAB3D )
					{
						kome::objects::Peak2DElement* p = peaks.createPeak(
							peak.getRt(),
							peak.getStartRt(),
							peak.getEndRt(),
							peak.getMz(),
							peak.getStartMz(),
							peak.getEndMz(),
							peak.getIntensity()
						);
					}
				}
			}

			pos += 1;
			pgrs0->setPosition( pos );
		}
	}

#endif //  PARALLEL_MERGE_TBB
// <Mod> M.Aihara <-

	pgrs0->fill();

	// sort
	peaks.sortByIntensity( true );

	// delete shoulder flag
	if( m_deleteShoulderFlag ) {
		// get shoulder peaks
		kome::core::Progress* pgrs1 = progress.getSubProgress( 1 );

		pos = 0;
		num = (int)peaks.getNumberOfPeaks();
		std::vector< int > removeElements;

// @data 2011.03.02 <Mod> M.Aihara ->
// modify TBB parallel code
#ifdef PARALLEL_MERGE_REMOVE_TBB
        ApplyParallel_mergePeaksCollectRemoveElems A(peaks,
                                                     num,
                                                     m_shoulderMzRange,
                                                     m_shoulderRtRange,
													 pgrs1,
													 pos);
        parallel_reduce(blocked_range<int>(0, num, 800), A);
        removeElements = A.removeElements;
#else /* PARALLEL_MERGE_REMOVE_TBB */

#pragma omp parallel for schedule( dynamic, 1 ) shared( pos, removeElements )
		for( int i = 0; i < num; i++ ) {
			kome::objects::Peak2DElement* peak = peaks.getPeak( i );

			for( int j = i + 1; j < num; j++ ) {
				kome::objects::Peak2DElement* tmp = peaks.getPeak( j );

				if( tmp->getRt() >= peak->getStartRt() - m_shoulderRtRange
						&& tmp->getRt() <= peak->getEndRt() + m_shoulderRtRange
						&& tmp->getMz() >= peak->getStartMz() - m_shoulderMzRange
						&& tmp->getMz() <= peak->getEndMz() + m_shoulderMzRange
				) {
#pragma omp critical ( addShoulderPeakByAB3D )
					{
						removeElements.push_back( j );
					}
				}
			}

			pos += 1;
			pgrs1->setPosition( pos );
		}

#endif /* PARALLEL_MERGE_REMOVE_TBB */
// <Mod> M.Aihara <-

		// delete
		std::sort( removeElements.begin(), removeElements.end() );
		std::unique( removeElements.begin(), removeElements.end() );

		for( int i = (int)removeElements.size() - 1; i >= 0; i-- ) {
			peaks.removePeak( removeElements[ i ] );
		}

		pgrs1->fill();
	}

	progress.fill();

	// @data 2011.03.02 <Del> M.Aihara
	// m_mergeTimer->stop();
}

// merge spectrum peaks
bool Ab3dLabeling::mergeSpectrumPeaks(
		kome::objects::Peaks* peaks,
		kome::objects::Peak2DElement* peak,
		const double specRt,
		const double maxInt
) {
	// peak information
	double rt = peak->getRt();
	double startRt = peak->getStartRt();
	double endRt = peak->getEndRt();

	double mz = peak->getMz();
	double startMz = peak->getStartMz();
	double endMz = peak->getEndMz();

	double intensity = peak->getIntensity();

	// index
	double minMz = mz - m_mzDistance;
	double maxMz = mz + m_mzDistance;

	int startIdx = peaks->searchIndex( minMz );
	if( startIdx < 0 ) {
		startIdx = - startIdx - 1;
	}

	int endIdx = peaks->searchIndex( maxMz );
	if( endIdx < 0 ) {
		endIdx = - endIdx - 2;
	}

	// check intensity
	for( int i = startIdx; i <= endIdx; i++ ) {
		if( peaks->getY( i ) > maxInt ) {
			return false;
		}
	}

	// merge
	bool flg = false;
	double minInt = maxInt * m_endHeight / 100.0;
	for( int i = startIdx; i <= endIdx; i++ ) {
		kome::objects::PeakElement* p = peaks->getPeak( i );
		double x = p->getX();
		double y = p->getY();

		if( y >= minInt ) {
			startMz = std::min( x, startMz );
			endMz =std::max( x, endMz );

			flg = true;
		}
	}

	// set peak
	if( flg ) {
		startRt = std::min( specRt, startRt );
		endRt = std::max( specRt, endRt );

		peak->setRt( rt, startRt, endRt );
		peak->setMz( mz, startMz, endMz );
		peak->setIntensity( intensity );
	}

	return true;
}

// get minimum intensity
double Ab3dLabeling::getMinIntensity( kome::objects::DataSet& dataSet ) {
	// absolute intensity
	if( m_intThresholdType == THRESHOLD_ABS ) {
		return m_intThreshold;
	}

	// BPC
	kome::objects::SpectraChromatogram bpc( dataSet );
	bpc.setBPC();

	kome::core::DataPoints dps;
	bpc.getXYData( &dps, false );

	// minimum intensity
	double intensity = m_intThreshold;
	if( m_intThresholdType == THRESHOLD_REL ) {
		intensity *= ( dps.getMaxY() / 100.0 );
	}
	else if( m_intThresholdType == THRESHOLD_AUTO ) {
		intensity = dps.getMinY();
	}

	return intensity;
}

// compare RT value of peaks
bool Ab3dLabeling::lessRt( kome::objects::Peak2DElement* p0, kome::objects::Peak2DElement* p1 ) {
	return ( p0->getRt() < p1->getRt() );
}

// compare m/z value of peaks
bool Ab3dLabeling::lessMz( kome::objects::Peak2DElement* p0, kome::objects::Peak2DElement* p1 ) {
	return ( p0->getMz() < p1->getMz() );
}

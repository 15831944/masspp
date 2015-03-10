/**
 * @file PeakDetectOperation.cpp
 * @brief Peak Detection Operation class
 *
 * @author M.Izumi
 * @date 2012.01.31
 * 
 * Copyright (C) 2014 Shimadzu Corporation All rights reserved.
 */

#include "stdafx.h"
#include "PeakDetectOperation.h"
#include "PeakPickingManager.h"
#include "Peaks2dManager.h"

using namespace kome::labeling;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


#define SPEC_PARAM_SHORT_NAME	"spec_peak_detect"
#define CHROM_PARAM_SHORT_NAME	"chrom_peak_detect"
#define GROUP_PARAM_SHORT_NAME	"2d_peak_detect"

// constructor
PeakDetectOperation::PeakDetectOperation(){
	// 初期化
	m_spectra.clear();
	m_chrom = NULL;
	m_group = NULL;
	m_peakDetector		= NULL;
	m_chargeDetector	= NULL;
	m_target			= -1;
}

// destructor
PeakDetectOperation::~PeakDetectOperation(){	
}

// on execute
bool PeakDetectOperation::onExecute(){
	bool ret = false;
	if( !m_spectra.empty() ){
		// spec
		ret = specPeakDetectFun();
	}

	if( m_chrom != NULL ){
		// chrom
		ret = chromPeakDetectFun();
	}

	if( m_group != NULL ){
		// 2d
		ret = peak2DDetectFun();
	}
	
	return ret;
}

//　on save condition
void PeakDetectOperation::onSaveCondition( boost::function< int ( void*, int ) > writeFun ){
	if( !m_spectra.empty() ){
		// spec
		saveSpecPeak( writeFun );
	}

	if( m_chrom != NULL ){
		// chrom
		saveChromPeak( writeFun );		
	}

	if( m_group != NULL ){
		// 2d
		savePeakAndCluster( writeFun );	
	}
}

//  on load condition
void PeakDetectOperation::onLoadCondition( boost::function< int ( void*, int ) > readFun ){
	if( !m_spectra.empty() ){
		// spec
		loadSpecPeak( readFun );	
	}

	if( m_chrom != NULL ){
		// chrom
		loadChromPeak( readFun );
	}

	if( m_group != NULL ){
		// 2d
		loadPeakAndCluster( readFun );
	}
	
	kome::window::WindowTool::refresh();
}

// get the description
std::string PeakDetectOperation::onGetDescription(){
	std::string s;
	
	if( !m_spectra.empty() ){
		// spec
		s = getSpecDescription();
	}

	if( m_chrom != NULL ){
		// chrom
		s = getChromDescription();
	}

	if( m_group != NULL ){
		// 2d
		s = get2dDescription();
	}

	return s;
}

// get the parameter string 
std::string PeakDetectOperation::onGetParametersString(){
	std::string s;

	if( !m_spectra.empty() ){
		// spec
		s = getSpecParametersString();
	}

	if( m_chrom != NULL ){
		// chrom
		s = getChromParametersString();
	}

	if( m_group != NULL ){
		// 2d
		s = get2dParametersString();
	}

	return s;
}

// set the parameter string 
void PeakDetectOperation::onSetParametersString( const char* strParam ){
	std::vector<std::string> tokens;
	stringtoken( strParam, ",", tokens );

	std::string strShortName = getShortName();
	if( strShortName.compare(SPEC_PARAM_SHORT_NAME) == 0 ){
		setSpecParametersString( strParam );
	}else if( strShortName.compare(CHROM_PARAM_SHORT_NAME) == 0 ){
		setChromParametersString( strParam );
	}else if( strShortName.compare(GROUP_PARAM_SHORT_NAME) == 0 ){
		set2dParametersString( strParam );
 	}
}

// on save spec peak
void PeakDetectOperation::saveSpecPeak( boost::function< int ( void*, int ) > writeFun ){
	
	// size
	unsigned int inum = m_spectra.size();
	writeFun( &inum, sizeof(inum) );

	for( unsigned int i=0; i < m_spectra.size(); i++ ){

		kome::objects::Spectrum* spec = m_spectra[i];
		if( spec != NULL ){	
			
			int id = spec->getId();
			writeFun( &id, sizeof(id) );
	
			// peak
			kome::objects::PeaksManager& mgr = kome::objects::PeaksManager::getInstance();
			kome::objects::Peaks* peak = mgr.getPeaks( spec );
			
			// create peak
			if( peak == NULL ){							
				mgr.createPeaks(spec);
				peak = mgr.getPeaks( spec );
			}
			// peak export data
			peak->exportData( writeFun );		
		}
	}
}

// on load spec peak
void PeakDetectOperation::loadSpecPeak( boost::function< int ( void*, int ) > readFun ){	
	unsigned int isize = 0;
	readFun( &isize, sizeof( isize ) );

	for( unsigned int i=0; i < isize; i ++ ){
		
		int id = 0;
		readFun ( &id, sizeof( id ) );
		
		// get sample
		kome::objects::Sample* sample = getTargetSample();
		// spectrum
		kome::objects::Spectrum* spec = NULL;
		if( sample != NULL ){
			// get spec by id
			spec = sample->getSpectrumById( id );
		}

		if( spec == NULL ){	
			return;
		}

		kome::objects::PeaksManager& mgr = kome::objects::PeaksManager::getInstance();
		kome::objects::Peaks* peak = mgr.getPeaks( spec );
		if( peak == NULL ){
			peak = mgr.createPeaks( spec );
		}
		
		peak->clearPoints();
		// peak import data
		peak->importData( readFun );				
		

		kome::plugin::PluginCallTool::onUpdateSpecPeaks( *spec );
		kome::plugin::PluginCallTool::onUpdateSpectrum( *spec );		
	}	
}

// on save chrom peak
void PeakDetectOperation::saveChromPeak( boost::function< int ( void*, int ) > writeFun ){
	
	// chromatogram
	if( m_chrom != NULL ){
		int id = m_chrom->getId();
		writeFun( &id, sizeof(id) );
	
		// peak
		kome::objects::PeaksManager& mgr = kome::objects::PeaksManager::getInstance();
		kome::objects::Peaks* peak = mgr.getPeaks( m_chrom );
		// create peak
		if( peak == NULL ){
			mgr.createPeaks( m_chrom );
			peak = mgr.getPeaks( m_chrom );
		}
		peak->exportData( writeFun );
	}
}

// on load chrom peak
void PeakDetectOperation::loadChromPeak( boost::function< int ( void*, int ) > readFun ){
	
	int id = 0;
	readFun( &id, sizeof(id) );

	// get sample
	kome::objects::Sample* sample = getTargetSample();
	// chromatogram
	kome::objects::Chromatogram* chrom = NULL;
	if( sample != NULL ){
		chrom = sample->getChromatogramById( id );
	}

	if( chrom == NULL ){
		return;
	}

	kome::objects::PeaksManager& mgr = kome::objects::PeaksManager::getInstance();
	kome::objects::Peaks* peak = mgr.getPeaks( chrom );
	if( peak == NULL ){
		peak = mgr.createPeaks( chrom );
	}
	peak->clearPoints();
	// peak import data
	peak->importData( readFun );
	
	kome::plugin::PluginCallTool::onUpdateChromPeaks( *chrom );
}


// on save peak and cluset (2d)
void PeakDetectOperation::savePeakAndCluster( boost::function< int ( void*, int ) > writeFun ){
	// group
	if( m_group == NULL ){
		return;
	}
	int id = m_group->getId();
	writeFun( &id, sizeof( id ) );
	
	// peak
	kome::objects::PeaksManager& mgr = kome::objects::PeaksManager::getInstance();
	kome::objects::Peaks2D* peaks = mgr.getPeaks( m_group );
	unsigned long ipeaks;
	if( peaks == NULL ){
		mgr.createPeaks(m_group);
		peaks = mgr.getPeaks( m_group );		
	}
	ipeaks = peaks->getNumberOfPeaks();
	// length
	writeFun( &ipeaks, sizeof( ipeaks ) );
	
	// write data
	if( ipeaks > 0 ) {
		double* arr = new double[ ipeaks * 14 ];

		for( unsigned int i = 0; i < ipeaks; i++ ) {
					
			// array
			int idx = i * 14;
			// peak
			kome::objects::Peak2DElement* peak = peaks->getPeak(i);
			if( peak != NULL ){
				arr[ idx + 0 ] = peak->getRt();
				arr[ idx + 1 ] = peak->getStartRt();
				arr[ idx + 2 ] = peak->getEndRt();
				arr[ idx + 3 ] = peak->getMz();
				arr[ idx + 4 ] = peak->getStartMz();
				arr[ idx + 5 ] = peak->getEndMz();
				arr[ idx + 6 ] = peak->getIntensity();	
			}

			// cluster
			kome::objects::PeaksCluster2D* cluster = peak->getCluster();
			if( cluster != NULL ){
				arr[ idx + 8 ] = cluster->getRt();
				arr[ idx + 9 ] = cluster->getStartRt();
				arr[ idx +10 ] = cluster->getEndRt();
				arr[ idx +11 ] = cluster->getMz();
				arr[ idx +12 ] = cluster->getStartMz();
				arr[ idx +13 ] = cluster->getEndMz();
			}
		}

		writeFun( arr, sizeof( double ) * ipeaks * 14 );
		delete[] arr;
	}	
}

// on load peak and cluster (2d)
void PeakDetectOperation::loadPeakAndCluster( boost::function< int ( void*, int ) > readFun ){
	
	int id = 0;
	readFun( &id, sizeof(id) );
	
	kome::objects::Sample* sample = getTargetSample();
	kome::objects::DataGroupNode* group = NULL;
	if( sample != NULL ){		
		group = sample->getGroupById( id );
	}
	
	if( group == NULL ){
		return;
	}

	kome::objects::PeaksManager& mgr = kome::objects::PeaksManager::getInstance();
	kome::objects::Peaks2D* peak = mgr.getPeaks( group );
			
	if( peak == NULL ){
		peak = mgr.createPeaks( group );
	}
	peak->clearClusters();	
	peak->clearPeaks();		

	unsigned long ipeaks = 0;
	readFun( &ipeaks, sizeof( ipeaks ) );	

	// read data
	if( ipeaks > 0 ) {
		double* arr = new double[ ipeaks * 14 ];
		readFun( arr, sizeof( double ) * ipeaks * 14 );			
			
		for( unsigned int i = 0; i < ipeaks; i++ ) {
			int idx = i * 14;
			double rt     = arr[ idx + 0 ];
			double srt    = arr[ idx + 1 ];
			double ert    = arr[ idx + 2 ];
			double mz     = arr[ idx + 3 ];
			double smz    = arr[ idx + 4 ];
			double emz    = arr[ idx + 5 ];
			double intent = arr[ idx + 6 ];
			
			// set peak 2d element
			kome::objects::Peak2DElement* peak2d = peak->createPeak( rt, srt, ert, mz, smz, emz, intent );		
			
			// create cluster
			kome::objects::PeaksCluster2D* cluster = peak->createCluster();
			rt  = arr[ idx + 8 ];
			srt = arr[ idx + 9 ];
			ert = arr[ idx +10 ];
			mz  = arr[ idx +11 ];
			smz = arr[ idx +12 ];
			emz = arr[ idx +13 ];
				
			cluster->setRt( rt );
			cluster->setMz( mz );
			cluster->setRtRange( srt, ert );
			cluster->setMzRange( smz, emz );
			cluster->setIntensity( intent );
				
			// set cluster 
			if( peak2d != NULL ){
				peak2d->setCluster(cluster);
			}
		}

		delete[] arr;
	}
	
	kome::plugin::PluginCallTool::onUpdate2DPeaks( *group );
}

// spectrum peak detect function
bool PeakDetectOperation::specPeakDetectFun(){
	bool ret = false;
	
	// peak manager
	PeakPickingManager& mgr = PeakPickingManager::getInstance();
	
	// progress
	kome::core::Progress* pprogress = NULL;		
	kome::window::DialogProgress* progress = NULL;
	if( kome::window::WindowTool::getMainWindow() != NULL && m_spectra.size() > 1 ){
			// progress
			progress = new kome::window::DialogProgress( 
			kome::window::WindowTool::getMainWindow(),
			"Detect Peaks", "preparing..."
		);
		pprogress = progress;
	}

	if( progress == NULL ){
		pprogress = &kome::core::Progress::getIgnoringProgress();
	}

	pprogress->setRange( 0, (int)m_spectra.size() );
	pprogress->setPosition( 0 );

	// -----------------------------------------------------------
	// peak detect
	// -----------------------------------------------------------
	for( unsigned int i = 0; i < m_spectra.size() && !pprogress->isStopped(); i++ ) {
		// spectrum
		kome::objects::Spectrum* spec = m_spectra[ i ];

		kome::core::XYData* xyData = NULL;
 		if( this->getOperationType() == kome::operation::Operation::TYPE_AUTO ){
 			xyData = m_spectra[0]->getXYData();
		}

		// status
		std::string msg = FMT( "Peak Detection (%s) ...... [%d/%d]", spec->getName(), ( i + 1 ), m_spectra.size() );
		pprogress->setStatus( msg.c_str() );
	
		// ------------------			
		// peak
		// ------------------
		mgr.detectPeaks(
			spec,
			xyData,
			m_peakDetector,
			&m_labelSettings,
			m_chargeDetector,
			&m_chargeSettings
		);
						
		// position
		pprogress->setPosition( i + 1 );
	}

	if( !pprogress->isStopped() ){
		ret = true;
	}
	pprogress->fill();
	
	delete progress;

	return ret;
}

// chromatogram peak detect function
bool PeakDetectOperation::chromPeakDetectFun(){
	bool ret = false;

	// peak manager
	PeakPickingManager& mgr = PeakPickingManager::getInstance();
	
	if( m_chrom == NULL ){
		return ret;
	}

	if( m_peakDetector == NULL ){
		return ret;
	}

	if( &m_labelSettings == NULL ){
		return ret;
	}

	// ---------------------------------------------------------------
	// peak detection
	// ---------------------------------------------------------------
	mgr.detectPeaks( m_chrom, m_chrom->getXYData(), m_peakDetector, &m_labelSettings );
	
	ret = true;
	return ret;
}

// 2d peak detect function
bool PeakDetectOperation::peak2DDetectFun(){
	bool ret = false;
	
	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();
	kome::objects::PeaksManager& pkMgr = kome::objects::PeaksManager::getInstance();
	Peaks2dManager& mgr = Peaks2dManager::getInstance();
		
	// progress
	kome::core::Progress* pprogress = NULL;		
	kome::window::DialogProgress* progress = NULL;
	if( kome::window::WindowTool::getMainWindow() != NULL ){
		progress = new kome::window::DialogProgress( 
			kome::window::WindowTool::getMainWindow(), 
			"Detect 2D Peaks"
		);
		pprogress = progress;
	}

	if( pprogress == NULL ){
		pprogress = &kome::core::Progress::getIgnoringProgress();
	}

	// data set
	kome::objects::DataSet* dataSet = m_group;
	kome::objects::Peaks2D* peaks = pkMgr.createPeaks( m_group );
	
	// ------------------------------------------
	// peak detect
	// ------------------------------------------
	mgr.detectPeaks(
		*m_group,
		*dataSet,
		*peaks,
		m_peakDetector,
		&m_labelSettings,
		*pprogress
	);
	
	if( !pprogress->isStopped() ){
		ret = true;
	}else{
		pprogress->fill();
	}

	delete progress;

	return ret;
}

//　get spectrum description
std::string PeakDetectOperation::getSpecDescription(){
	std::string s;
	
	PeakPickingManager& mgr = PeakPickingManager::getInstance();
		
		std::string targetName ="";
	if( getOperationType() == kome::operation::Operation::TYPE_AUTO ){
		if( m_spectra[0] != NULL ){
			targetName.append( FMT("%s", m_spectra[0]->getName() ) );
		}
	}else{
		targetName = mgr.getTargetName( PeakPickingManager::PeakPickingTarget(m_target));// get Target Name
	}
	s.append( FMT( "%s [%s](", m_peakDetector->getLongName(), targetName.c_str() ) );
	
	unsigned int inum = m_labelSettings.getNumberOfParameters();
	
	// parameter 抜き出し
	for( unsigned int i=0; i < inum; i++ ){				
		s.append( FMT("[%s=%s]", m_labelSettings.getParameterName(i), m_labelSettings.getParameterValue(i)) );
		// sub
		kome::objects::SettingParameterValues* sub = m_labelSettings.getSubParameterValues( m_labelSettings.getParameterName(i), m_labelSettings.getParameterValue(i) );
		if( sub != NULL ){
			unsigned int subnum = sub->getNumberOfParameters();
			for( unsigned int j=0; j < subnum; j++ ){
				s.append( FMT("[%s=%s]", sub->getParameterName(j), sub->getParameterValue(j)) );
			}	
		}			
	}	
	
	s.append( ")" );
	
	return s;
}

// set spectrum parameter string 
void PeakDetectOperation::setSpecParametersString( const char* strParam ){
	// sample Id, spec Id,(target),PeakDetect Type, PeakDetect Name, [param value]....
	std::vector<std::string> tokens;
	stringtoken( strParam, "," , tokens );
 
	if( tokens.size() < 5 ){
		return;
	}

	// sample
	kome::objects::Sample* sample = kome::objects::Sample::getSampleById( toint( tokens[0].c_str(), 10, -1 ) );
	if( sample == NULL ){
		return;
	}
	setTargetSample( sample );

	// target
	int index = tokens[2].find(")");
	m_target = toint( tokens[2].substr(1, index).c_str(), 10, -1 );
	
	// spectra
	kome::objects::DataGroupNode* group = sample->getGroupById( toint( tokens[1].c_str(), 10, -1 ) );
	if( group == NULL ){
		return;
	}
	getTargetSpectra( group );

	// function item
	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();
	m_peakDetector = plgMgr.getFunctionItem( tokens[3].c_str(), tokens[4].c_str() );

	//　Chargeがある時
// >>>>>>	@Date:2013/09/10	<Modified>	A.Ozaki
//  Token分割したあと、charge detectorの情報まで、指定されていないので、
//  領域の数を確認してから参照しないと、落ちてしまいます
//
//	if( tokens[5].find( "[" ) == -1 ){
//		m_chargeDetector = plgMgr.getFunctionItem( tokens[5].c_str(), tokens[6].c_str() );
//	}
	if  ( 6 <= tokens.size( ) )
	{
		if  ( tokens[5].find( "[" ) == -1 )
		{
			if  ( 7 <= tokens.size( ) )
			{
				m_chargeDetector = plgMgr.getFunctionItem( tokens[5].c_str(), tokens[6].c_str() );
			}
		}
	}
//
// <<<<<<	@Data:2013/09/10	<Modified>	A.Ozaki

	// Peak Detect, Charge Detectのパラメータ取得
	std::vector<std::string> strSettings;

	index = FMT( "%s", strParam ).find("[");
	std::string	tmpParam;

	tmpParam.clear( );
	if  ( 0 < index )
	{
		tmpParam = FMT( "%s", strParam ).substr(index).c_str();
	}

	
	// Parameter切り分け
	stringtoken( tmpParam.c_str(), ":", strSettings );
	
	// peak detect settings
	kome::plugin::SettingsPage* page = ( m_peakDetector == NULL ? NULL :m_peakDetector->getSettingsPage() );
	if( page != NULL ){
		page->setParameterString( m_labelSettings, strSettings[0].c_str() );
	}

	// charge settings
	kome::plugin::SettingsPage* chargePage = ( m_chargeDetector == NULL ? NULL : m_chargeDetector->getSettingsPage() );
	if( chargePage != NULL ){
		chargePage->setParameterString( m_chargeSettings, strSettings[1].c_str() );
	}	
}

// get spectrum parameteras string 
std::string PeakDetectOperation::getSpecParametersString(){
	std::string s;

	// get function
	kome::plugin::PluginCall* fun = ( m_peakDetector == NULL ? NULL : m_peakDetector->getCall() );
	if( fun == NULL ) {
		return s;
	}
	kome::plugin::PluginCall* chargefun = ( m_chargeDetector == NULL ? NULL : m_chargeDetector->getCall() );
	
	// sample Id, spec Id,(target),PeakDetect Type, PeakDetect Name, [param value]....
	if( getOperationType() == kome::operation::Operation::TYPE_AUTO ){
		kome::objects::Spectrum* spec = m_spectra[0];
		if( spec != NULL ){
			s.append( FMT("%d,%d",
				spec->getSample()->getSampleId(),
				spec->getId() )
			);			
		}
			
	}else{
		kome::objects::DataSet ds;
		for( unsigned int i=0; i < m_spectra.size(); i++ ){
			kome::objects::Spectrum* spec = m_spectra[i];
			if( spec != NULL ){
				ds.addSpectrum( spec );
			}
		}
		kome::objects::DataGroupNode* group = ds.getGroup();
		if( group != NULL ){
			s.append( FMT("%d,%d",
				group->getSample()->getSampleId(),
				group->getId())
			);
		}
		
	}
	s.append( FMT(",(%d)", m_target));
	s.append( FMT(",%s,%s,%s,%s",
		fun->getType(),
		fun->getProperties().getStringValue("short name",""),
		(chargefun == NULL ) ? "" : chargefun->getType(),
		(chargefun == NULL ) ? "" : chargefun->getProperties().getStringValue("short name","" ))
	);

	
	// peak detector parameter strings
	if( m_peakDetector != NULL ){
		kome::plugin::SettingsPage* page = m_peakDetector->getSettingsPage();
		if( page != NULL ){
			s.append( FMT(",%s", page->getParameterString(m_labelSettings).c_str()));
		}
	}

	// charge detector parameter strings
	if( m_chargeDetector != NULL ){
		kome::plugin::SettingsPage* page = m_chargeDetector->getSettingsPage();
		if( page != NULL ){
			s.append( FMT(":%s", page->getParameterString(m_chargeSettings).c_str()));
		}
	}

	return s;
}

// get chromatogram description
std::string PeakDetectOperation::getChromDescription(){
	std::string s;
	PeakPickingManager& mgr = PeakPickingManager::getInstance();

	s.append( FMT( "%s [%s](", m_peakDetector->getLongName(), m_chrom->getName() ) );
	unsigned int inum = m_labelSettings.getNumberOfParameters();
	// parameter 抜き出し
	for( unsigned int i=0; i < inum; i++ ){
		s.append(FMT( "[%s=%s]",m_labelSettings.getParameterName(i), m_labelSettings.getParameterValue(i) ));
			
		// sub
		kome::objects::SettingParameterValues* sub = m_labelSettings.getSubParameterValues( m_labelSettings.getParameterName(i), m_labelSettings.getParameterValue(i) );
		unsigned int subnum = sub->getNumberOfParameters();
		for( unsigned int j=0; j < subnum; j++ ){
			s.append( FMT("[%s=%s]", sub->getParameterName(j), sub->getParameterValue(j)) );
		}
	}

	s.append( ")" );
	return s;
}

// set chrom parameters string
void PeakDetectOperation::setChromParametersString( const char* strParam ){
	std::vector<std::string> tokens;
	stringtoken( strParam, ",", tokens );

	if( tokens.size() < 4 ){
		return;
	}

	// sample 
	kome::objects::Sample* sample = kome::objects::Sample::getSampleById( toint( tokens[0].c_str(), 10, -1 ) );
	if( sample == NULL ){
		return;
	}
	setTargetSample( sample );

	// chromatogram
	m_chrom = sample->getChromatogramById( toint( tokens[1].c_str(), 10, -1 ) );
	if( m_chrom == NULL ){
		return;
	}
	// function item
	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();
	m_peakDetector = plgMgr.getFunctionItem( tokens[3].c_str(), tokens[4].c_str() );
	
	// setting parameter
	std::vector<std::string> strSettings;
	
	int index = FMT( "%s", strParam ).find("[");
	if( index != -1 ){
		std::string tmpParam = FMT( "%s", strParam ).substr(index).c_str();

		stringtoken( tmpParam.c_str(), ":", strSettings );

		// peak detect settings
		kome::plugin::SettingsPage* page = ( m_peakDetector == NULL ? NULL :m_peakDetector->getSettingsPage() );
		if( page != NULL ){
			page->setParameterString( m_labelSettings, strSettings[0].c_str() );
		}
	}
}

// get chromatogram parameters string
std::string PeakDetectOperation::getChromParametersString(){
	std::string s;

	// get function
	kome::plugin::PluginCall* fun = ( m_peakDetector == NULL ? NULL : m_peakDetector->getCall() );
	if( fun == NULL ) {
		return s;
	}

	// sample Id, chrom Id,(target),PeakDetect Type, PeakDetect Name, [param value]....
	if( m_chrom == NULL ){
		return s;
	}
	s.append( FMT("%d,%d,(%d)",
		m_chrom->getSample()->getSampleId(),
		m_chrom->getId(),
		m_target));

	s.append( FMT(",%s,%s",
		fun->getType(),
		fun->getProperties().getStringValue("short name",""))
	);

	// parameter settings
	if( m_peakDetector != NULL ){
		kome::plugin::SettingsPage* page = m_peakDetector->getSettingsPage();
		if( page != NULL ){
			s.append( FMT(",%s", page->getParameterString(m_labelSettings).c_str()));
		}
	}
	return s;
}

// get 2d description
std::string PeakDetectOperation::get2dDescription(){
	std::string s;
	
	s.append( FMT( "%s [%s](", m_peakDetector->getLongName(), m_group->getName() ) );
	unsigned int inum = m_labelSettings.getNumberOfParameters();
	// parameter 抜き出し
	for( unsigned int i=0; i < inum; i++ ){
		s.append(FMT( "[%s=%s]",m_labelSettings.getParameterName(i), m_labelSettings.getParameterValue(i) ));
			
		// sub
		kome::objects::SettingParameterValues* sub = m_labelSettings.getSubParameterValues( m_labelSettings.getParameterName(i), m_labelSettings.getParameterValue(i) );
		unsigned int subnum = sub->getNumberOfParameters();
		for( unsigned int j=0; j < subnum; j++ ){
			s.append( FMT("[%s=%s]", sub->getParameterName(j), sub->getParameterValue(j)) );
		}
	}

	s.append( ")" );

	return s;
}

// set 2d parameters string
void PeakDetectOperation::set2dParametersString( const char* strParam ){
	// sample Id, group Id,(target),PeakDetect Type, PeakDetect Name, [param value]....
	std::vector<std::string> tokens;
	stringtoken( strParam, "," , tokens );
 
	if( tokens.size() < 5 ){
		return;
	}
	// sample
	kome::objects::Sample* sample = kome::objects::Sample::getSampleById( toint( tokens[0].c_str(), 10, -1 ) );
	if( sample == NULL ){
		return;
	}
	setTargetSample( sample );

	// group 
	m_group = sample->getGroupById( toint( tokens[1].c_str(), 10, -1 ) );
	if( m_group == NULL ){
		return;
	}

	// function item
	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();
	m_peakDetector = plgMgr.getFunctionItem( tokens[3].c_str(), tokens[4].c_str() );

	// settings
	std::vector<std::string> strSettings;
	
	int index = FMT( "%s", strParam ).find("[");
	std::string tmpParam = FMT( "%s", strParam ).substr(index).c_str();

	stringtoken( tmpParam.c_str(), ":", strSettings );

	// peak detect settings
	kome::plugin::SettingsPage* page = ( m_peakDetector == NULL ? NULL :m_peakDetector->getSettingsPage() );
	if( page != NULL ){
		page->setParameterString( m_labelSettings, strSettings[0].c_str() );
	}
}


// get 2d parameters string
std::string PeakDetectOperation::get2dParametersString(){
	std::string s;
	// get function
	kome::plugin::PluginCall* fun = ( m_peakDetector == NULL ? NULL : m_peakDetector->getCall() );
	if( fun == NULL ) {
		return s;
	}
	
	// sample Id, group Id,(target),PeakDetect Type, PeakDetect Name, [param value]....
	if( m_group != NULL ){
		s.append( FMT("%d,%d",
			m_group->getSample()->getSampleId(),
			m_group->getId())
		);
	}
		
	s.append( FMT(",(%d)", m_target));
	s.append( FMT(",%s,%s",
		fun->getType(),
		fun->getProperties().getStringValue("short name","") )
	);

	// peak detector parameter
	if( m_peakDetector != NULL ){
		kome::plugin::SettingsPage* page = m_peakDetector->getSettingsPage();
		if( page != NULL ){
			s.append( FMT(",%s", page->getParameterString(m_labelSettings).c_str()));
		}
	}

	return s;
}

// get spectra
std::vector<kome::objects::Spectrum*> PeakDetectOperation::getSpectra(){
	return m_spectra;
}

// set spectra
void PeakDetectOperation::setSpectra( std::vector<kome::objects::Spectrum*> spectra ){
	m_spectra.clear();
	m_spectra = spectra;
}

// get chromatogram
kome::objects::Chromatogram* PeakDetectOperation::getChrom(){
	return m_chrom;
}

// set chromatogram
void PeakDetectOperation::setChrom( kome::objects::Chromatogram* chrom ){
	m_chrom = chrom;
}

// get group
kome::objects::DataGroupNode* PeakDetectOperation::getGroup(){
	return m_group;
}

// set group
void PeakDetectOperation::setGroup( kome::objects::DataGroupNode* group ){
	m_group = group;
}

// get peak detector
kome::plugin::PluginFunctionItem* PeakDetectOperation::getPeakDetect(){
	return m_peakDetector;
}

// set peak detector
void PeakDetectOperation::setPeakDetect( kome::plugin::PluginFunctionItem* item ){
	m_peakDetector = item;
}

// get charge detector
kome::plugin::PluginFunctionItem* PeakDetectOperation::getChargeDetect(){
	return m_chargeDetector;
}

// set charge detector
void PeakDetectOperation::setChargeDetct( kome::plugin::PluginFunctionItem* item){
	m_chargeDetector = item;
}

// get label settings
kome::objects::SettingParameterValues PeakDetectOperation::getLabelSettings(){
	return m_labelSettings;
}

// set label settings
void PeakDetectOperation::setLabelSettings( kome::objects::SettingParameterValues labelSettings ){
	m_labelSettings = labelSettings;
}

// get charge settings
kome::objects::SettingParameterValues PeakDetectOperation::getChargeSettings(){
	return m_chargeSettings;
}

// set charge settings
void PeakDetectOperation::setChargeSettings( kome::objects::SettingParameterValues chargeSettings ){
	m_chargeSettings = chargeSettings;
}

// get target
int PeakDetectOperation::getTarget(){
	return m_target;
}

// set target
void PeakDetectOperation::setTarget( int target ){
	m_target = target;
}

// get target spectra
void PeakDetectOperation::getTargetSpectra( kome::objects::DataGroupNode* group ){
	kome::objects::Spectrum* spec = NULL;

	if( m_target == PeakPickingManager::TARGET_CURRENT_SPECTRUM ){
		m_spectra.push_back( group->getSpectrum( 0 ));
	}else if( m_target == PeakPickingManager::TARGET_FILTERED_SPECTRA ) {
		kome::objects::ActiveObjectsManager& aoMgr = kome::objects::ActiveObjectsManager::getInstance();
		// active spectra
		if( group != NULL ) {
			kome::objects::DataSet* s = aoMgr.getFilteredDataSet( group->getSample() );
			if( s != NULL ) {
				for( unsigned int i = 0; i < s->getNumberOfSpectra(); i++ ) {
					m_spectra.push_back( s->getSpectrum( i ) );
				}
			}
		}
	}else{
		kome::objects::Sample* sample = getTargetSample();
		// get all spectra
		if( sample != NULL ) {
			kome::objects::DataSet s( sample->getRootDataGroupNode() );
			sample->getRootDataGroupNode()->getDataSet( &s );

			// each spectra
			for( unsigned int i = 0; i < s.getNumberOfSpectra(); i++ ) {
				kome::objects::Spectrum* spec = s.getSpectrum( i );
				if( ( m_target != PeakPickingManager::TARGET_MS_SPECTRA || spec->getMsStage() == 1 )
					&& ( m_target != PeakPickingManager::TARGET_MSMS_SPECTRA || spec->getMsStage() > 1 ) ) {
					m_spectra.push_back( spec );
				}
			}
		}
	}
}

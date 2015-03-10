/**
 * @file ChromatogramOperationManager.cpp
 * @brief implements of ChromatogramOperationManager class
 *
 * @author S.Tanaka
 * @date 2007.04.10
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "ChromatogramOperationManager.h"
#include "GenelateChromOperation.h"
#include "GenelateChromatogramDialog.h"

using namespace kome::operation::chrom;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG

using namespace kome::operation::sel_range;

#define CHROMATOGRAM_FILTER_STATUS_NAME	"CHROMATOGRAM_FILTER"


// constructor
ChromatogramOperationManager::ChromatogramOperationManager() {
	// set status
	kome::objects::StatusManager& statusMgr = kome::objects::StatusManager::getInstance();
	const char* filterStatus = statusMgr.getValue( CHROMATOGRAM_FILTER_STATUS_NAME );
	std::string fs = NVL( filterStatus, "" );

	if( fs.compare( "ms" ) == 0 ) {
		m_filter = 0x01;
	}
	else if( fs.compare( "msms" )  == 0 ) {
		m_filter = 0xfe;
	}
	else {
		m_filter = 0xff;
	}
}

// destructor
ChromatogramOperationManager::~ChromatogramOperationManager() {
	// delete chromatograms
	for( std::set< kome::objects::Chromatogram* >::iterator it = m_chromSet.begin();
			it != m_chromSet.end(); it++ ) {
		delete (*it);
	}
	m_chromSet.clear();
}

// set chromatogram filter
void ChromatogramOperationManager::setFilter( const unsigned char filter ) {
	m_filter = filter;
	onUpdateFilter();
}

// get chromatogram filter
unsigned char ChromatogramOperationManager::getFilter() {
	return m_filter;
}

// update filter
void ChromatogramOperationManager::onUpdateFilter() {
	// manager
	kome::objects::ActiveObjectsManager& aoMgr = kome::objects::ActiveObjectsManager::getInstance();

	kome::objects::Parameters param; // @date 2011.01.25 <Add> M.Izumi
	// update
	const unsigned int num = kome::window::ChildFrame::getNumberOfFrames();
	for( unsigned int i = 0; i < num; i++ ) {
		// window
		kome::window::ChildFrame* frame = kome::window::ChildFrame::getFrame( i );

		// chromatogram
		for( unsigned int j = 0; j < frame->getNumberOfChromatograms(); j++ ) {
			kome::objects::Chromatogram* chrom = frame->getChromatogram( j );
			kome::objects::SpectraChromatogram* gchrom
				= dynamic_cast< kome::objects::SpectraChromatogram* >( chrom->getOrgChromatogram() );	

			// filter
			if( gchrom != NULL ) {
				gchrom->setFilter( m_filter );
			}

			// update
			kome::plugin::PluginCallTool::onUpdateChromatogram( *chrom );

			// set chromatogram
			kome::plugin::PluginCallTool::setChromatogram( param, *chrom ); // @date 2011.01.25 <Add> M.Izumi
		}
	}
	// @date 2011.01.25 <Add> M.Izumi ->
	// managers
	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();
	plgMgr.callAllFunctions( &param, "ON_CHROMATOGRAM_PROCESSING" );
	// @date 2011.01.25 <Add> M.Izumi <-

	// refresh
	kome::window::WindowTool::refresh();
}

// update chromatogram set
void ChromatogramOperationManager::updateChromSet() {
	// store chromatogram set
	std::set< kome::objects::Chromatogram* > chromSet;
	for( std::set< kome::objects::Chromatogram* >::iterator it = m_chromSet.begin();
			it != m_chromSet.end(); it++ ) {
		chromSet.insert( *it );
	}
	m_chromSet.clear();

	// opened chromatogram
	std::set< kome::objects::Chromatogram* > opened;
	for( unsigned int i = 0; i < kome::window::ChildFrame::getNumberOfFrames(); i++ ) {
		kome::window::ChildFrame* frame = kome::window::ChildFrame::getFrame( i );
		for( unsigned int j = 0; j < frame->getNumberOfChromatograms(); j++ ) {
			kome::objects::Chromatogram* chrom = frame->getChromatogram( j );

			opened.insert( chrom );
			opened.insert( chrom->getOrgChromatogram() );
		}
	}

	// update chromatogram set
	for( std::set< kome::objects::Chromatogram* >::iterator it = chromSet.begin(); 
			it != chromSet.end(); it++ ) {
		kome::objects::Chromatogram* chrom = *it;
		if( opened.find( chrom ) == opened.end() ) {
			delete chrom;
		}
		else {
			m_chromSet.insert( chrom );
		}
	}
}

// create MC
kome::objects::Chromatogram* ChromatogramOperationManager::createMC(
		kome::objects::DataGroupNode* group,
		const double startMz,
		const double endMz
) {
	// create chromatogram object
	kome::objects::SpectraChromatogram* chrom = createChromatogram( group, startMz, endMz );

	// set flag
	if( chrom != NULL ) {
		chrom->setTIC();
		// name
		chrom->setName( "MC" ); // @data 2010.11.19 <Mod> M.Izumi
	}

	return chrom;
}

// create MC Group
kome::objects::Chromatogram* ChromatogramOperationManager::createMCGroup(
		kome::objects::DataGroupNode* group,
		const double startMz,
		const double endMz
) {
	// create chromatogram object
	kome::objects::SpectraChromatogram* chrom = createChromatogramGroup( group, startMz, endMz, false );

	// set flag
	if( chrom != NULL ) {
		chrom->setTIC();
		// name
		chrom->setName( "MC" );	 // @data 2010.11.19 <Mod> M.Izumi
	}

	return chrom;
}

// create BPC
kome::objects::Chromatogram* ChromatogramOperationManager::createBPC(
		kome::objects::DataGroupNode* group,
		const double startMz,
		const double endMz
) {
	// create chromatogram object
	kome::objects::SpectraChromatogram* chrom = createChromatogram( group, startMz, endMz );

	// set flag
	if( chrom != NULL ) {
		chrom->setBPC();
		// name
		chrom->setName( "BPC" );	// @data 2010.11.19 <Mod> M.Izumi
	}
	
	return chrom;
}

// create BPC Group
kome::objects::Chromatogram* ChromatogramOperationManager::createBPCGroup(
		kome::objects::DataGroupNode* group,
		const double startMz,
		const double endMz
) {
	// create chromatogram object
	kome::objects::SpectraChromatogram* chrom = createChromatogramGroup( group, startMz, endMz );

	// set flag
	if( chrom != NULL ) {
		chrom->setBPC();
		// name
		chrom->setName( "BPC" );	// @data 2010.11.19 <Mod> M.Izumi
	}
	
	return chrom;
}

// create Processed MC @date 2012.10.03 <Add> M.Izumi
kome::objects::Chromatogram* ChromatogramOperationManager::createProcessedMC(
	kome::objects::DataGroupNode* group,
	const double startMz,
	const double endMz
){
	// create chromatogram object
	kome::objects::SpectraChromatogram* chrom = createChromatogram( group, startMz, endMz, true );

	// set flag
	if( chrom != NULL ) {
		chrom->setName( "Processed MC" );	
	}
	
	return chrom;
}

// 2010/08/30 APPEND OKADA ------->
// open appending chromatogram MC node dialog
bool ChromatogramOperationManager::openAppendingChromMCDialog( kome::objects::SettingParameterValues& settings ) {
	// page
	kome::plugin::ParameterSettings params( NULL, false );
	kome::plugin::SettingsPage* page = new kome::plugin::SettingsPage( params );
	page->setName( "xic" );
	page->setTitle( "MC" );

	// tolerance
	kome::plugin::SettingsValue* value = params.addValue();
	value->setType( kome::plugin::SettingsValue::TYPE_DOUBLE );
	value->getNumberRestriction().setInclusiveMinValue( 0.0 );
	value->setDefaultValue( "0.5" );
	value->setRequired( true );

	kome::plugin::SettingsParam* param = params.addParam();
	param->setName( CHROM_TOLERANCE_PARAM_NAME );
	param->setSection( "MC Chromatogram" );
	param->setKey( "MZ_TOLERANCE" );
	param->setValue( value );
	
	kome::plugin::SettingsForm* form = page->addForm();
	form->setTitle( "start m/z" );
	form->setParam( param );

	// interval
	value = params.addValue();
	value->setType( kome::plugin::SettingsValue::TYPE_DOUBLE );
	value->getNumberRestriction().setExclusiveMinValue( 0.0 );
	value->setDefaultValue( "1.0" );
	value->setRequired( true );

	param = params.addParam();
	param->setName( CHROM_INTERVAL_PARAM_NAME );
	param->setSection( "MC Chromatogram" );
	param->setKey( "MZ_INTERVAL" );
	param->setValue( value );
	
	form = page->addForm();
	form->setTitle( "end m/z" );
	form->setParam( param );

	// dialog
	kome::window::SettingsPageDialog dlg(
		kome::window::WindowTool::getMainWindow(),
		page,
		&settings,
		true
	);

	return ( dlg.ShowModal() == wxID_OK );
}


bool ChromatogramOperationManager::openAppendingChromBPCDialog( kome::objects::SettingParameterValues& settings ) {
	// page
	kome::plugin::ParameterSettings params( NULL, false );
	kome::plugin::SettingsPage* page = new kome::plugin::SettingsPage( params );
	page->setName( "xic" );
	page->setTitle( "BPC" );

	// tolerance
	kome::plugin::SettingsValue* value = params.addValue();
	value->setType( kome::plugin::SettingsValue::TYPE_DOUBLE );
	value->getNumberRestriction().setInclusiveMinValue( 0.0 );
	value->setDefaultValue( "0.5" );
	value->setRequired( true );

	kome::plugin::SettingsParam* param = params.addParam();
	param->setName( CHROM_TOLERANCE_PARAM_NAME );
	param->setSection( "BPC Chromatogram" );
	param->setKey( "MZ_TOLERANCE" );
	param->setValue( value );
	
	kome::plugin::SettingsForm* form = page->addForm();
	form->setTitle( "start m/z" );
	form->setParam( param );

	// interval
	value = params.addValue();
	value->setType( kome::plugin::SettingsValue::TYPE_DOUBLE );
	value->getNumberRestriction().setExclusiveMinValue( 0.0 );
	value->setDefaultValue( "1.0" );
	value->setRequired( true );

	param = params.addParam();
	param->setName( CHROM_INTERVAL_PARAM_NAME );
	param->setSection( "BPC Chromatogram" );
	param->setKey( "MZ_INTERVAL" );
	param->setValue( value );
	
	form = page->addForm();
	form->setTitle( "end m/z" );
	form->setParam( param );

	// dialog
	kome::window::SettingsPageDialog dlg(
		kome::window::WindowTool::getMainWindow(),
		page,
		&settings,
		true
	);

	return ( dlg.ShowModal() == wxID_OK );
}
// 2010/08/30 APPEND OKADA <-------

// nTitleKind 0:MC, 1:BPC
bool ChromatogramOperationManager::openAppendingChromMcBpcDialog( kome::objects::SettingParameterValues& settings, int nTitleKind ) {
	// page
	kome::plugin::ParameterSettings params( NULL, false );
	kome::plugin::SettingsPage* page = new kome::plugin::SettingsPage( params );
	page->setName( "xic" );
	if( 0 == nTitleKind ){
		page->setTitle( "MC" );
	}else if( 1 == nTitleKind ){
		page->setTitle( "BPC" );
	}else{
		// parameter error
		return false;
	}

	// MC BPC RANGE
	kome::plugin::SettingsValue* value = params.addValue();
	value->setType( kome::plugin::SettingsValue::TYPE_DOUBLE_RANGE );
	value->getNumberRestriction().setInclusiveMinValue( 0.0 );
	value->setRequired( true );

	kome::plugin::SettingsParam* param = params.addParam();
	param->setName( CHROM_RANGE_PARAM_NAME );
	param->setSection( "MC BPC Chromatogram" );	// iniファイルのセクション名
	param->setKey( "MZ_RANGE" );			// iniファイルのキー名
	param->setValue( value );
	
	kome::plugin::SettingsForm* form = page->addForm();
	form->setTitle( "m/z" );
	form->setParam( param );

	// dialog
	kome::window::SettingsPageDialog dlg(
		kome::window::WindowTool::getMainWindow(),
		page,
		&settings,
		true
		, NULL
		, false	// [DEFAULT]ボタンを表示しない
	);

	return ( dlg.ShowModal() == wxID_OK );
}

// add chromatogram nodes
void ChromatogramOperationManager::addChromNodes(
		kome::objects::DataGroupNode& group,
		const double tol,
		const double interval,
		const bool bpc
) {
	// existing chromatograms
	std::vector< kome::objects::Chromatogram* > chroms;
	for( unsigned int i = 0; i < group.getNumberOfChromatograms(); i++ ) {
		kome::objects::Chromatogram* chrom = group.getChromatogram( i );
		if( chrom->isAutoCreated() ) {
			chroms.push_back( chrom );
		}
		else {
			m_chromSet.insert( chrom );
		}
	}

	group.clearChromatograms();
	for( unsigned int i = 0; i < chroms.size(); i++ ) {
		group.addChromatogram( chroms[ i ] );
	}
	updateChromSet();

	// spectra
	std::vector< kome::objects::Spectrum* > spectra;
	double minMz = double();
	double maxMz = double();
	for( unsigned int i = 0; i < group.getNumberOfSpectra(); i++ ) {
		kome::objects::Spectrum* spec = group.getSpectrum( i );

		double tmpMin = spec->getMinX();
		double tmpMax = spec->getMaxX();
		if( minMz < 0.0 || spectra.size() == 0 || tmpMin < minMz ) {
			minMz = tmpMin;
		}
		if( maxMz < 0.0 || spectra.size() == 0 || tmpMax > maxMz ) {
			maxMz = tmpMax;
		}
		spectra.push_back( spec );
	}

	if( spectra.size() == 0 ) {
		return;
	}

	// index
	int start = std::max( 0, (int)floor( minMz / interval ) );
	int end = std::max( 0, (int)ceil( maxMz / interval ) );

	// chromatogram
	for( int i = start; i <= end; i++ ) {
		double mz = (double)i * interval;
		if( mz + tol >= minMz && mz - tol <= maxMz ) {
			kome::objects::SpectraChromatogram* chrom = new kome::objects::SpectraChromatogram( group );
			chrom->setMz( mz );
			chrom->setMzRange( std::max( 0.0, mz - tol ), mz + tol );
			chrom->setFilter( m_filter );
			if( bpc ) {
				chrom->setBPC();
			}
			else {
				chrom->setTIC();
			}

			chrom->setName(
				FMT( ( bpc ? "BPC [mz=%.2f]" : "XIC [mz=%.2f]" ), mz ).c_str()
			);

			group.addChromatogram( chrom );
		}
	}
}

// create chromatogram
kome::objects::SpectraChromatogram* ChromatogramOperationManager::createChromatogram(
		kome::objects::DataGroupNode* group,
		const double startMz,
		const double endMz,
		bool bProces	
) {
	// check group
	if( group == NULL || group->getNumberOfSpectra() < 2 ) {
		LOG_WARN( FMT( "It is impossible to create a chromatogram." ) );
		return NULL;
	}

	// 2010/09/02 APPEND OKADA
	return createChromatogramGroup( group, startMz, endMz, bProces );

}


// create chromatogram group
kome::objects::SpectraChromatogram* ChromatogramOperationManager::createChromatogramGroup(
		kome::objects::DataGroupNode* group,
		const double startMz,
		const double endMz,
		bool bProcess	
) {
	// check group
	if( group == NULL || group->getNumberOfSpectra() < 2 ) {
		LOG_WARN( FMT( "It is impossible to create a chromatogram." ) );
		return NULL;
	}
	
	// create chromatogram object
	kome::objects::SpectraChromatogram* chrom = new kome::objects::SpectraChromatogram( *group, bProcess );
	m_chromSet.insert( chrom );

	// set range
	chrom->setMzRange( startMz, endMz );

	// set filter
	chrom->setFilter( m_filter );

	return chrom;
}


// on close chromatogram
void ChromatogramOperationManager::onCloseChromatogram( kome::objects::Chromatogram* chrom, const bool deleting ) {
}


//2012/02/24 add by t.okuno -> This plugin distributes 'MC' and 'BPC'.
// creates Generate Chromatogram dialog
void ChromatogramOperationManager::createGenerateChromatogram( kome::objects::DataGroupNode* group,
															   const double startMz,
															   const double endMz )
{
	// @date 2103.07.11 <Add> M.Izumi
	bool menuFlg = false;
	if( startMz < 0.0 && endMz < 0.0 ){
		menuFlg = true;
	}
	// @date 2012.04.26 <Mod> M.Izumi ->
	// SPEC No.85858 Average Spectrum / Generate Chromatogram operations create a new window, but it is hidden
	GenelateChromatogramDialog dlg( kome::window::WindowTool::getMainWindow(), group, menuFlg );
	dlg.setStartPos( startMz );
	dlg.setEndPos( endMz );

	if( dlg.ShowModal() == wxID_OK ){
		// ------------------------
		// Operation Log追加
		// ------------------------
		GenelateChormOperaton* opt = new GenelateChormOperaton();
		opt->setOperationName( "Genelate Chromatogram" );
		opt->setOperationType( kome::operation::Operation::TYPE_STANDARD );
		opt->setShortName( "chrom_opt" );
		opt->setTargetSample( group->getSample() );
		opt->setGroup( group );
		opt->setStartPos( dlg.getStartPos( ) );			// @Date:2013/09/07	<Modified>	A.Ozaki
		opt->setEndPos( dlg.getEndPos( ) );				// @Date:2013/09/07	<Modified>	A.Ozaki
		opt->setParamName( dlg.getStrParamName() );
		opt->setTitle( dlg.getTitle() );

		if( !opt->execute() ){
			delete opt;
		}		
	}
	//<-
}


//2012/02/24 add by t.okuno <- This plugin distributes 'MC' and 'BPC'.

// get object
ChromatogramOperationManager& ChromatogramOperationManager::getInstance() {
	// create object (This is the only object)
	static ChromatogramOperationManager mgr;

	return mgr;
}

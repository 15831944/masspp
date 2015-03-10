/**
 * @file FileIOManager.cpp
 * @brief implements of FileIOManager class
 *
 * @author S.Tanaka
 * @date 2006.10.14
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "FileIOManager.h"
#include "SampleCloseCheckDialog.h"	
#include "FileIOPluginOperation.h"

#include <string>
#include <list>
#include <algorithm>
#include <map>
#include <boost/bind.hpp>
#include <wx/wx.h>
#include <wx/utils.h>

using namespace kome::io;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG

//using namespace kome::io::axima;

#define SECTION						"File IO"
#define DEFAULT_SAVE_FILTER_KEY		"DEFAULT_SAVE_FILTER"
#define DEFAULT_DIR_KEY				"DEFAULT_DIR"

#define ALL_FILES_NAME				"All Files"
#define ALL_SPECTRUM_FILES_NAME		"All Mass Data Files"
#define DEFAULT_FILE_FORMAT_NAME	"Mass Data File"
#define FILE_OPEN_CAPTION			"Select File"
#define FILE_SAVE_CAPTION			"Save As"

#define FILE_SAVE_FUNC_TYPE_NAME	"SAVE_FILE"

#define LONG_NAME_PARAM_NAME		"long name"
#define SHORT_NAME_PARAM_NAME		"short name"
#define EXT_PARAM_NAME				"ext"

// constructor
FileIOManager::FileIOManager() {

	m_saveItem = NULL;
	m_saveSettings = NULL;

	m_sampleSelector = new kome::core::RangeListSelector(
		kome::core::RangeList::DEFAULT_ALL
	);
	
	// open sample
	m_openSample = NULL;
}

// destructor
FileIOManager::~FileIOManager() {
	if( m_sampleSelector != NULL ) {
		delete m_sampleSelector;
	}
	m_sampleSelector = NULL;
}

// open raw data
kome::objects::SampleSet* FileIOManager::openFile( const char* path, kome::core::Progress* progress ) {
	// managers
	kome::plugin::FileFactory& factory = kome::plugin::FileFactory::getInstance();
	kome::objects::ActiveObjectsManager& aoMgr = kome::objects::ActiveObjectsManager::getInstance();
	
	// main frame
	kome::window::MainFrame* frame = (kome::window::MainFrame*)kome::window::WindowTool::getMainWindow();

	// -------
	// open
	// -------
	kome::objects::SampleSet* sampleSet = factory.searchFile( path );
	if( sampleSet == NULL ) {
		sampleSet = factory.openFile( path, progress ); // add param

		if( progress != NULL && progress->isStopped() ){
			return sampleSet;
		}
	}
		
	if( sampleSet == NULL ) {
		aoMgr.removeHistoryFile( path );
				
		if( frame != NULL ) {
			const char* msg = getlasterrormessage();
			kome::window::WindowTool::showError( msg );
			frame->updateMRU();
		}
	}
	else {
		kome::plugin::PluginCallTool::onOpenSampleSet( *sampleSet );
		aoMgr.addHistoryFile( path );
		
		if( frame != NULL ) {
			frame->updateMRU();
		}
	}
		
	return sampleSet;
}

// set save item
void FileIOManager::setSaveItem( kome::plugin::PluginFunctionItem* item ) {
	m_saveItem = item;
}

// get save item
kome::plugin::PluginFunctionItem* FileIOManager::getSaveItem() {
	return m_saveItem;
}

// set status settings
void FileIOManager::setSaveSettings( kome::objects::SettingParameterValues* saveSettings){
	 m_saveSettings = saveSettings;
}

// get status settings
kome::objects::SettingParameterValues FileIOManager::getSaveSettings(){
	return *m_saveSettings;
}

// set GUI selector
void FileIOManager::setGuiSelector() {
	// delete old selector
	if( m_sampleSelector != NULL ) {
		delete m_sampleSelector;
	}

	// dialog selector
	m_sampleSelector = new kome::window::DialogSelector(
			kome::window::WindowTool::getMainWindow(),
			"Select Sample",
			true,
			true
	);
}

// get sample selector
kome::core::Selector* FileIOManager::getSelector() {
	return m_sampleSelector;
}

// clear sample name
void FileIOManager::clearSampleNames() {
	m_sampleNameSet.clear();
}

// add sample name
void FileIOManager::addSampleName( const char* name ) {
	m_sampleNameSet.insert( NVL( name, "" ) );
}

// add sample type
void FileIOManager::addSampleType( const char* type) {
	m_sampleTypeSet.insert( NVL( type, "" ) );
}

// open selected sample
void FileIOManager::openSelectedSample( kome::objects::SampleSet* sampleSet, kome::core::Progress* progress ) {
	// check the sample set
	if( sampleSet == NULL || sampleSet->getNumberOfSamples() == 0 ) {
		return;
	}

	// select samples
	std::vector< int > samples;
	if( sampleSet->getNumberOfSamples() == 1 ) {
		samples.push_back( 0 );
	}
	else {
		// initialize selector
		m_sampleSelector->clearItems();

		for( unsigned int i = 0; i < sampleSet->getNumberOfSamples(); i++ ) {
			m_sampleSelector->addItem( sampleSet->getSample( i )->getName() );
		}

		// select
		if( m_sampleSelector->select() ) {
			for( unsigned int i = 0; i < sampleSet->getNumberOfSamples(); i++ ) {
				if( m_sampleSelector->isSelected( i ) ) {
					samples.push_back( i );
				}
			}

			if( samples.size() == 0 ) {
				samples.push_back( 0 );
			}
		}
	}

	// open samples
	for( unsigned int i = 0; i < samples.size(); i++ ) {
		m_openSample = NULL;
		// sample
		kome::objects::Sample* sample = sampleSet->getSample( samples[ i ] );
		if( sample != NULL
				&& ( m_sampleNameSet.empty() || ( m_sampleNameSet.find( sample->getName() ) != m_sampleNameSet.end() ) ) 
				&& ( m_sampleTypeSet.empty() || ( m_sampleTypeSet.find( sample->getType() ) != m_sampleTypeSet.end() ) ) ) {
			// check
			if( sample->isOpened() ) {  // already opened
				LOG_ERROR_CODE(
					FMT(
						"The sample is already opened. [%s - %s]",
						sample->getName(),
						sampleSet->getFileName()
					),
					ERR_OTHER
				);
			}
			else if( sample->openSample( progress ) ) {   // succeeded
				m_openSample = sample;
				kome::plugin::PluginCallTool::onOpenSample( *sample );
			}
			else {  // failed
				LOG_ERROR_CODE(
					FMT(
						"Failed to open the sample. [%s - %s]",
						sample->getName(),
						sampleSet->getFileName()
					),
					ERR_OTHER
				);
			}
		}
	}
}

//---------------------------------------------------->	2010.11.02 Add M.Izumi
// search Saectrum sample file name
std::vector<std::string> searchSpecSampleFileName(kome::window::Canvas* canvas){
	kome::objects::Spectrum* spec;
	kome::objects::Sample* sample;
	std::vector<std::string> strAry;
	std::string strTmp="";
	strAry.clear();

	int iSize = canvas->getNumberOfSpectra();
	
	for( int i =0; i < iSize; i++ ){
		spec = canvas->getSpectrum( i );
		sample = spec->getSample();
		std::string str = sample->getSampleSet()->getFileName();
		if( strTmp.compare(str) != 0 ){
			strAry.push_back(str);
			strTmp = str;			
		}
	}
	return strAry;
}

// search Chromatogram sample file name
std::vector<std::string> searchChromSampleFileName(kome::window::Canvas* canvas){
	kome::objects::Chromatogram* chrom;
	kome::objects::Sample* sample;
	std::vector<std::string> strAry;	
	std::string strTmp="";
	strAry.clear();

	int iSize = canvas->getNumberOfChromatograms();
	
	for( int i =0; i < iSize; i++ ){
		chrom = canvas->getChromatogram( i );
		sample = chrom->getSample();
		std::string str = sample->getSampleSet()->getFileName();
		if( strTmp.compare(str) != 0 ){
			strAry.push_back(str);
			strTmp = str;			
		}
	}
	return strAry;
}

// Set the state of the checkboxInfo
void FileIOManager::setChkListInfos(std::vector<std::string> strInfo, bool bchk ){
	std::vector<CheckListBoxInfo>::iterator it; 
	
	for(unsigned int i=0; i < strInfo.size(); i++ ){
		for( it = m_ChkListInfos.begin(); it != m_ChkListInfos.end(); ++it ){
			if( it->strName.compare( strInfo[i].c_str() ) == 0 ){
				it->bCheck = bchk;
			}
		}
	}
}

// Set the state of the checkboxInfo（Spectrum）
void FileIOManager::setChkListInfosSpec(kome::window::Canvas* canvas){
	kome::objects::Spectrum* spec = canvas->getSpectrum();
	if( spec == NULL ){ return; }// @date 2010.12.21 <Add> M.Izumi NULLチェック
	kome::objects::Sample* sample = spec->getSample();
	if( sample != NULL ){		 // @date 2010.12.21 <Add> M.Izumi NULLチェック
		kome::objects::SampleSet* sampleSet = sample->getSampleSet();						
		std::vector<CheckListBoxInfo>::iterator it; 					
		for( it = m_ChkListInfos.begin(); it != m_ChkListInfos.end(); ++it ){
					
			if( it->strName.compare( sample->getRootDataGroupNode()->getName() ) == 0 ){
				it->bCheck = true;			
			}
		}
	}
}

// Set the state of the checkboxInfo（Chromatogram）
void FileIOManager::setChkListInfosChrom(kome::window::Canvas* canvas){
	kome::objects::Chromatogram* chrom = canvas->getChromatogram();
	if( chrom == NULL ){ return; }// @date 2010.12.21 <Add> M.Izumi NULLチェック
	
	kome::objects::Sample* sample = chrom->getSample();
	if( sample != NULL ){		  // @date 2010.12.21 <Add> M.Izumi NULLチェック
		
		std::vector<CheckListBoxInfo>::iterator it; 	
		for( it = m_ChkListInfos.begin(); it != m_ChkListInfos.end(); ++it ){
		
			if( it->strName.compare( sample->getRootDataGroupNode()->getName() ) == 0 ){
				it->bCheck = true;
			}
		}
	}
}

// set CheckListBoxInfo and sample
void FileIOManager::setChkListInfosAndSamples( kome::objects::Sample* sample ){
	// managers
	kome::objects::ActiveObjectsManager& aoMgr = kome::objects::ActiveObjectsManager::getInstance();
	
	// @date 2011.08.24 <Add> M.Izumi
	// SPEC 82165:Sample name mismatch (Close Dialog)
	std::vector< kome::objects::Sample* > strTmp = aoMgr.getTabSamples();
	for( unsigned int j=0; j < strTmp.size(); j++ ){
		CheckListBoxInfo tmpInfo = CheckListBoxInfo();
		tmpInfo.bCheck = ( strTmp[ j ] == sample );
		tmpInfo.strName = strTmp[j]->getRootDataGroupNode()->getName();
		
		m_ChkListInfos.push_back(tmpInfo);
		m_samples.push_back( strTmp[j] );
	}
}

// Search the status of the canvas showing
void FileIOManager::ShowCanvasTypeSearch(){
	std::vector<std::string> strInfo;
	strInfo.clear();
	
	// Displays the number of Canvas 
	unsigned int iCount = kome::window::ChildFrame::getNumberOfFrames();
	
	if(iCount != 0){
		for( unsigned int i=0; i < iCount; i++ ){
			kome::window::ChildFrame* frame = kome::window::ChildFrame::getFrame( i );
			kome::window::Canvas* canvas = dynamic_cast< kome::window::Canvas* >( frame->getCanvas() );

			if( canvas != NULL ) {
				// overlapping
				int style = canvas->getCanvasStyle();	// @date 2010.12.10 <Add> M.Izumi
				if((style & kome::window::Canvas::MULTIPLE_OBJECTS)){	// @date 2010.12.10 <Mod> M.Izumi
					switch( canvas->getActiveObject().type ) {
						// spectrum
					case kome::objects::Variant::SPECTRUM:
						strInfo = searchSpecSampleFileName(canvas);
						setChkListInfos(strInfo, false);
						break;
						// chromatogram
					case kome::objects::Variant::CHROMATOGRAM:
						strInfo = searchChromSampleFileName(canvas);
						setChkListInfos(strInfo,false);
						break;
					default:
						break;
					}
				
				// single
				}else{ 
					switch( canvas->getActiveObject().type ) {
					// spectrum
					case kome::objects::Variant::SPECTRUM:
						setChkListInfosSpec(canvas);
						break;
					// chromatogram
					case kome::objects::Variant::CHROMATOGRAM:
						setChkListInfosChrom(canvas);
						break;
					default:
						break;
					}
				}
			}
		}
	}
}

// open SampleCloseCheckDialog
bool FileIOManager::openSampleCloseCheckDaialog( kome::objects::Sample* sample ){
	m_ChkListInfos.clear();
	m_samples.clear();
	
	setChkListInfosAndSamples( sample );
	ShowCanvasTypeSearch();
	
	SampleCloseCheckDialog dlg( kome::window::WindowTool::getMainWindow() );
	
	for( unsigned int j=0; j < m_ChkListInfos.size(); j++ ){
		dlg.setChkListInfo(m_ChkListInfos[j].strName, m_ChkListInfos[j].bCheck);
	}

	// show dialog
	if( dlg.ShowModal() == wxID_OK){
		// @date 2011.01.12 <Mod> M.Izumi ->
		// Spec No.79873 マージしたサンプルをClose処理すると例外が発生する不具合修正
		int iCount = m_samples.size();
		while( 0 <= iCount ){
			if( dlg.isChecked( iCount ) ){
				closeSample(m_samples[iCount]);				
			}
			iCount--;
		}

		for( unsigned int i=0; i < m_samples.size(); i++ ){
			m_samples.erase( m_samples.begin() + i );
		}
		// @date 2011.01.12 <Mod> M.Izumi <-

		// update
		kome::window::WindowTool::refresh();
	}

	return true;
}
// <----------------------------------------------------------- 2010.11.02 Add M.Izumi

// open sample
bool FileIOManager::openSampleSet() {
	// File Open Dialog
	kome::window::RawDataFileDialog dlg( kome::window::WindowTool::getMainWindow() );

	// dialog open
	bool result = true;
	if( dlg.ShowModal() == wxID_OK ) {
		// path
		std::vector< std::string > paths;
		for( unsigned int i = 0; i < dlg.getNumberOfPaths(); i++ ) {
			paths.push_back( dlg.getPath( i ) );
		}
		std::sort( paths.begin(), paths.end() );

		// SPEC 81818: Cannot cancel progress dialog when opening a large file // @date 2011.09.22 <Mod> M.Izumi
		std::string path;
		std::string fileName;
#if 0
		if( paths.size() == 1 ){
			// path
			path = paths[ 0 ];
			fileName = getfilename( path.c_str() );
		
			wxBeginBusyCursor();

			// Operation 作成
			FileIoOperation* opt = new FileIoOperation();
			opt->setOperationType( kome::operation::Operation::TYPE_INPUT );
			opt->setShortName( "file_open" );
			opt->setOperationName( "File Open" );

			opt->setFilePath( path.c_str() );

			if( opt->execute() ){
			}else{
				delete opt;
			}
			result = true;

			wxEndBusyCursor();

		}else{
			// progress
			kome::window::DialogProgress progress( kome::window::WindowTool::getMainWindow(), "Open" );
			progress.setRange( 0, paths.size() );
			
			// open
			result = false;
			// Operation 作成
			FileIoOperation* opt = NULL;

			for( int i = 0; i < (int)paths.size() && !progress.isStopped(); i++ ) {
				// path
				path = paths[ i ];
				fileName = getfilename( path.c_str() );

				// progress
				progress.setStatus( FMT( "[%d/%d] %s", ( i + 1 ), paths.size(), fileName.c_str() ).c_str() );

				opt = new FileIoOperation();
				opt->setOperationType( kome::operation::Operation::TYPE_INPUT );				
				opt->setShortName( "file_open" );
				opt->setOperationName( "File Open" );
								
				opt->setFilePath( path.c_str() );

				if( opt->execute() ){
				}else{
					delete opt;
				}
			
				progress.setPosition( i + 1 );
			}

			// click cansel button 
			if( progress.isStopped() ){
				delete opt;
			}
			
			result = true;			
		}
#else 
			// progress
			kome::window::DialogProgress progress( kome::window::WindowTool::getMainWindow(), "Open" );
			//progress.setRange( 0, paths.size() );
						
			// open
			result = false;
			// Operation 作成
			FileIoOperation* opt = NULL;
						
			// progress
			progress.setStatus( FMT( "File Open..." ).c_str(), true );
			progress.setRange( 0, paths.size() );

			for( int i = 0; i < (int)paths.size() && !progress.isStopped(); i++ ) {
				// progress
				progress.setPosition( i );
				progress.setStatus( FMT( "[%d/%d] %s", ( i + 1 ), paths.size(), fileName.c_str() ).c_str() );

				// path
				path = paths[ i ];
				fileName = getfilename( path.c_str() );
								
				opt = new FileIoOperation();
				opt->setOperationType( kome::operation::Operation::TYPE_INPUT );				
				opt->setShortName( "file_open" );
				opt->setOperationName( "File Open" );
								
				opt->setFilePath( path.c_str() );
				opt->setProgress( &progress );	
				
				if( opt->execute() ){
				}else{
					delete opt;
				}	

				progress.setPosition( i, true );
			}

			// click cansel button 
			if( progress.isStopped() ){
				progress.fill();
				return true;//エラーダイアログ出さないため
			}
			
			progress.fill();

			result = true;			

		}	
#endif

	return result;
}

// export spectra with dialog
bool FileIOManager::exportSpectraWithDialog( kome::objects::DataSet& dataSet, kome::plugin::PluginFunctionItem* item, const bool checkData, bool* cancelFlg ) {
	// check data
	if ( (checkData) && ( dataSet.getNumberOfSpectra() == 0 && dataSet.getNumberOfChromatograms() == 0 ) ) {
		LOG_ERROR_CODE( FMT( "This function cannot be used because there is no data to be saved. Open a data file first." ), ERR_OTHER );
		return false;
	}

	// ini file
	kome::core::MsppManager& msppMgr = kome::core::MsppManager::getInstance();
	kome::core::IniFile* ini = msppMgr.getIniFile();

	// get save functions
	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();
	
	unsigned int num = plgMgr.getNumberOfFunctionItems( FILE_SAVE_FUNC_TYPE_NAME );
	if( num == 0 ) {
		LOG_ERROR_CODE( FMT( "No file save algorithms are installed.\nExit Mass++ and install at least one file save plug-in." ), ERR_OTHER );
		return false;
	}
	if( item == NULL ) {
		LOG_ERROR( FMT( "Failed to get the save function." ) );
		return false;
	}

	// file name
	std::string fileName;
	if (checkData)
	{
		if( dataSet.getNumberOfSpectra() > 0 ) {
			fileName = dataSet.getSpectrum( 0 )->getSample()->getSampleSet()->getFileName();
		}
		else {
			fileName = dataSet.getChromatogram( 0 )->getSample()->getSampleSet()->getFileName();
		}
		unsigned int pos = fileName.rfind( "." );
		if( pos != (unsigned int)fileName.npos ) {
			fileName = fileName.substr( 0, pos );
		}
	}

	// default dir
	std::string defaultDir = ini->getString( SECTION, DEFAULT_DIR_KEY, "" );

	// get filters
	std::string ext = item->getCall()->getProperties().getStringValue( EXT_PARAM_NAME, "" );
	if( ext.empty() ) {
		ext = "*";
	}

	// @date 2011/12/19 <Add> OKADA for SPEC 83470 ------->
	else if (fileName.size() > 0) {
		// fileName の 末尾に ext が含まれていなければ、追加する
		std::string::size_type pos = fileName.rfind( ext );
		std::string::size_type lenOfFilename = fileName.length();
		std::string::size_type lenOfExt = ext.length();
		if(( lenOfFilename-lenOfExt != pos ) ||( pos==std::string::npos )){
			fileName.append( "." );
			fileName.append( ext );
		}
	}
	// @date 2011/12/19 <Add> OKADA <-------

	std::string filters = FMT( "%s (*.%s)|*.%s", item->getLongName(), ext.c_str(), ext.c_str() );
	
	// file save dialog
	wxFileDialog dialog(
		kome::window::WindowTool::getMainWindow(),
		wxT( FILE_SAVE_CAPTION ),
		wxT( defaultDir.c_str() ),
		wxEmptyString,
		wxT( filters.c_str() ),
		wxFD_SAVE | wxFD_OVERWRITE_PROMPT
	);
	dialog.SetFilterIndex( 0 );
	if (fileName.size() > 0)
	{
		dialog.SetFilename( wxT( fileName.c_str() ) );
	}

	// open dialog
	bool ret = true;
	if( dialog.ShowModal() == wxID_OK ) {
				
		// get target
		int target = ini->getInt(  EXPORT_SECTION, TARGET_KEY, 0 );
		
		// ----------------
		// Operation 作成
		// ----------------
		FileIoOperation* opt = new FileIoOperation();
		opt->setTargetSample( dataSet.getSample() );
		opt->setOperationType( kome::operation::Operation::TYPE_OUTPUT );
		opt->setOperationName( "Export Sample" );
		opt->setShortName( "export_sample" );
		
		opt->setDataSet( dataSet );
		opt->setFuncItem( item );
		opt->setFilePath( dialog.GetPath().c_str() );
		opt->setTarget( target );
		opt->setSaveSettings( m_saveSettings );

		bool bflg = false;
		if( opt->execute() ){
			ret = true;
		}else{
			delete opt;
			bflg = true;
		}
		
		// ini
		if( ini != NULL ) {
			ini->setString( SECTION, DEFAULT_DIR_KEY, getdir( dialog.GetPath().c_str() ).c_str() );
			ini->setInt( SECTION, DEFAULT_SAVE_FILTER_KEY, dialog.GetFilterIndex() );
		}

        if( cancelFlg != NULL ) {
			*cancelFlg = bflg;
		}	

	}
	else {
		if( cancelFlg != NULL ) {
			*cancelFlg = true;
		}
	}
	
	return ret;
}

// save
bool FileIOManager::exportSpectra(
		kome::objects::DataSet& dataSet,
		kome::plugin::PluginFunctionItem* saveItem,
		const char* path,
		kome::core::Progress& progress,
		kome::objects::SettingParameterValues* saveSettings		              // @date 2012/8/20 <Add> FUJITA 
) {
	// create parameters
	kome::objects::Parameters params;
	kome::plugin::PluginCallTool::setDataSet( params, dataSet );
	kome::plugin::PluginCallTool::setPath( params, path );
	kome::plugin::PluginCallTool::setProgress( params, progress );
	kome::plugin::PluginCallTool::setSettingValues( params, *saveSettings );   // @date 2012/8/20 <Add> FUJITA 

	// save
	kome::plugin::PluginCall* call = saveItem->getCall();
	return call->invoke( &params ).prim.boolVal;
}

// close sample
void FileIOManager::closeSample( kome::objects::Sample* sample ) {
	// check the parameter
	if( sample == NULL ) {
		return;
	}

	// close
	if( sample->isOpened() ) {

		if (sample->isEdited() && kome::window::WindowTool::getMainWindow() != NULL )
		{
			// Reset the edited flag before closing the sample.
			sample->setEdited(false);
		}
		
		kome::objects::SampleSet* sampleSet = sample->getSampleSet();
		
		sample->closeSample();
	}
}

// save sample
bool FileIOManager::saveSample( kome::objects::Sample* sample )
{
	bool bSuccess = true;

	// data set
	kome::objects::DataGroupNode* root = sample->getRootDataGroupNode();
	kome::objects::DataSet dataSet( root );
	root->getDataSet( &dataSet );
	
	// check data
	if( dataSet.getNumberOfSpectra() == 0 && dataSet.getNumberOfChromatograms() == 0 ) {
		LOG_ERROR_CODE( FMT( "This function cannot be used because there is no data to be saved. Open a data file first." ), ERR_OTHER );
		bSuccess = false;
	}

	if (bSuccess)
	{
		// get save functions
		kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();
		unsigned int num = plgMgr.getNumberOfFunctionItems( FILE_SAVE_FUNC_TYPE_NAME );
		if( num == 0 ) {
			LOG_ERROR_CODE( FMT( "No file save algorithms are installed.\nExit Mass++ and install at least one file save plug-in." ), ERR_OTHER );
			bSuccess = false;
		}
	}

	std::string filePath;
	std::string sCurrentSampleFileExtension;
	if (bSuccess)
	{
		// file path
		if( dataSet.getNumberOfSpectra() > 0 ) {
			filePath = dataSet.getSpectrum( 0 )->getSample()->getSampleSet()->getFilePath();
		}
		else {
			filePath = dataSet.getChromatogram( 0 )->getSample()->getSampleSet()->getFilePath();
		}

		unsigned int pos = filePath.rfind( "." );
		if ( (pos != (unsigned int)filePath.npos) && (pos < filePath.length() - 1) )
		{
			sCurrentSampleFileExtension = filePath.substr( pos + 1, filePath.length() - 1 - pos );
		}
		else
		{
			LOG_ERROR_CODE( FMT( "Data file could not be saved because the current sample's filename has no extension." ), ERR_OTHER );
			bSuccess = false;
		}
	}

	if (bSuccess)
	{
		// save
		kome::plugin::PluginFunctionItem* saveItem = FindPluginManagerFunctionItem(sCurrentSampleFileExtension);

		if (saveItem != NULL)
		{
			// progress
			kome::window::DialogProgress progress(
				kome::window::WindowTool::getMainWindow(),
				"Writing data"
			);

			// create parameters
			kome::objects::Parameters params;
			kome::plugin::PluginCallTool::setDataSet( params, dataSet );
			kome::plugin::PluginCallTool::setPath( params, filePath.c_str() );
			kome::plugin::PluginCallTool::setProgress( params, progress );

			// save
			kome::plugin::PluginCall* call = saveItem->getCall();
			bSuccess = call->invoke( &params ).prim.boolVal;

			if (!bSuccess)
			{
				char sMessage[300];
				_snprintf(sMessage, 300, "Cannot save the sample. Check the file / folder permissions. [%s]", NVL( filePath.c_str(), "" ));
				kome::window::WindowTool::showError(sMessage);
			}
		}
		else
		{
			LOG_ERROR_CODE( FMT( "File cannot be saved because there is no plugin for saving samples with the current file extension..\nSpecify a supported file extension." ), ERR_OTHER );
			bSuccess = false;
		}
	}

	return bSuccess;
}

kome::plugin::PluginFunctionItem* FileIOManager::FindPluginManagerFunctionItem(const std::string& fileExtension)
{
	kome::plugin::PluginFunctionItem* foundItem = NULL;

	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();
	for( unsigned int i = 0;
		(i < plgMgr.getNumberOfFunctionItems( FILE_SAVE_FUNC_TYPE_NAME) && (!foundItem) ); i++ )
	{
		// get save item
		kome::plugin::PluginFunctionItem* item = plgMgr.getFunctionItem( FILE_SAVE_FUNC_TYPE_NAME, i );

		// extension
		std::string ext = item->getCall()->getProperties().getStringValue( EXT_PARAM_NAME, "" );

		if (fileExtension == ext)
		{
			foundItem = item;
		}
	}

	return foundItem;
}

// set output file name suffix
void FileIOManager::setSuffix( const char* suffix ) {
	m_suffix = NVL( suffix, "" );
}

// get output file name suffix
const char* FileIOManager::getSuffix() {
	return m_suffix.c_str();
}

// get save filter
std::string FileIOManager::getSaveFilters( ) {
	// filter
	std::string filters;

	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();
	for( unsigned int i = 0; i < plgMgr.getNumberOfFunctionItems( FILE_SAVE_FUNC_TYPE_NAME ); i++ ) {
		// get save item
		kome::plugin::PluginFunctionItem* item = plgMgr.getFunctionItem( FILE_SAVE_FUNC_TYPE_NAME, i );

		// extention
		std::string ext = item->getCall()->getProperties().getStringValue( EXT_PARAM_NAME, "" );

		// add to filter
		std::string f = FMT( "%s (*.%s)|*.%s", item->getLongName(), ext.c_str(), ext.c_str() );
		if( !filters.empty() ) {
			filters.append( "|" );
		}
		filters.append( f );
	}

	return filters;
}

// save active spectra
bool FileIOManager::saveSharedData( const char* path ) {
	// return value
	bool ret = true;

	// managers
	kome::objects::ActiveObjectsManager& aoMgr = kome::objects::ActiveObjectsManager::getInstance();

	// check the member
	if( m_saveItem == NULL ) {
		std::string msg = "Specify output file format.";
		LOG_ERROR( msg );
		return false;
	}

	// get file name and directory
	std::string dir = getdir( path );
	std::string fileName = getfilename( path );

	unsigned int pos = fileName.find_last_of( "." );
	std::string ext;
	if( pos != (unsigned int)fileName.npos ) {
		ext = fileName.substr( pos + 1 );
		fileName = fileName.substr( 0, pos );
	}

	// create data set
	kome::objects::Sample* sample = aoMgr.getActiveSample();
	kome::objects::DataSet* dataSet = aoMgr.getFilteredDataSet( sample );

	// save
	ret = saveFile( dataSet, dir.c_str(), fileName.c_str(), ext.c_str() );
	
	return ret;
}

// save spectra
bool FileIOManager::saveFile(
		kome::objects::DataSet* dataSet,
		const char* dir,
		const char* fileName,
		const char* ext
) {
	// divide flag
	bool divFlg = false;
	if( !m_suffix.empty() ) {
		divFlg = true;
	}

	std::string target = m_saveItem->getCall()->getProperties().getStringValue( "target", "" );
	target = tolowercase( target.c_str() );
	target = trimstring( target.c_str() );

	if( target.compare( "spectrum" ) == 0 ) {
		divFlg = true;
	}

	// file names
	std::vector< std::pair< kome::objects::DataSet*, std::string > > spectra;
	std::set< std::string > fileNameSet;
	bool cntFlg = false;

	if( divFlg ) {
		for( unsigned int i = 0; i < dataSet->getNumberOfSpectra(); i++ ) {
			kome::objects::Spectrum* spec = dataSet->getSpectrum( i );
			kome::objects::DataSet* ds = new kome::objects::DataSet();
			ds->addSpectrum( spec );

			// file name
			kome::core::Properties props;
			spec->getProperties( props );

			std::string name = fileName;

			if( !m_suffix.empty() ) {
				std::string suffix = props.replaceString( m_suffix.c_str(), "#", "#", "null" );
				name = FMT( "%s%s", name.c_str(), suffix.c_str() );
			}

			if( fileNameSet.find( name ) != fileNameSet.end() ) {
				cntFlg = true;
			}

			fileNameSet.insert( name );
			spectra.push_back( std::make_pair( ds, name ) );
		}
	}
	else {
		spectra.push_back( std::make_pair( dataSet, fileName ) );
	}

	// change file name
	for( unsigned int i = 0; i < spectra.size(); i++ ) {
		std::string name = spectra[ i ].second;
		if( cntFlg ) {
			name = FMT( "%s-%08d.%s", name.c_str(), i, ext );
		}
		else {
			name = FMT( "%s.%s", name.c_str(), ext );
		}

		spectra[ i ].second = getpath( dir, name.c_str() );
	}

	// progress
	kome::core::DisplayProgress progress( "Writing" );
	if( spectra.size() > 0 ) {
		progress.createSubProgresses( spectra.size() );
	}
	
	// export
	bool ret = true;
	for( unsigned int i = 0; i < spectra.size() && !progress.isStopped(); i++ ) {
		if( !exportSpectra(
				*( spectra[ i ].first ),
				m_saveItem,
				spectra[ i ].second.c_str(),
				*( progress.getSubProgress( i ) ),
				m_saveSettings 
		) ) {
			ret = false;
		}
	}

	// delete
	if( divFlg ) {
		for( unsigned int i = 0; i < spectra.size(); i++ ) {
			delete spectra[ i ].first;
		}
	}

	if( !progress.isStopped() ){ // @date 2013.11.22 <Add> M.Izumi
		progress.fill();
	}
	return ret;
}

// get spectrum file path
void FileIOManager::getSpectrumFilePath(
		kome::objects::Sample* sample,
		kome::objects::DataSet* dataSet,
		const char* dir,
		const char* fileName,
		const char* ext,
		std::map< kome::objects::Spectrum*, std::string >& pathMap
) {
	// count map
	std::map< std::string, int > countMap0;
	std::map< std::string, int > countMap1;

	// get file path
	for( unsigned int i = 0; i < dataSet->getNumberOfSpectra(); i++ ) {
		// spectrum
		kome::objects::Spectrum* spec = dataSet->getSpectrum( i );

		// properties
		kome::core::Properties properties;
		spec->getProperties( properties );		

		// get file name
		std::string suffix = properties.replaceString( m_suffix.c_str(), "#", "#", "null" );

		// path
		std::string path = getpath( dir, fileName );
		path += suffix;

		// count
		if( countMap0.find( path ) == countMap0.end() ) {
			countMap0[ path ] = 1;
		}
		else {
			countMap0[ path ] = countMap0[ path ] + 1;
		}

		// set to map
		pathMap[ spec ] = path;
	}

	// check duplicated
	for( unsigned int i = 0; i < dataSet->getNumberOfSpectra(); i++ ) {
		// spectrum
		kome::objects::Spectrum* spec = dataSet->getSpectrum( i );

		// path
		std::string path = pathMap[ spec ];
		int count = 0;
		if( countMap0.find( path ) != countMap0.end() ) {
			count = countMap0[ path ];
		}

		if( count > 1 ) {
			int num = 1;
			if( countMap1.find( path ) != countMap1.end() ) {
				num = countMap1[ path ] + 1;
			}
			countMap1[ path ] = num;

			path += FMT( "-%d.%s", num, ext );
		}
		else {
			path += FMT( ".%s", ext );
		}
	
		// insert to map
		pathMap[ spec ] = path;
	}
}

// set open sample
void FileIOManager::setOpenSample( kome::objects::Sample* sample ){
	m_openSample = sample;
	
}

// get open sample
kome::objects::Sample* FileIOManager::getOpenSample(){
	return m_openSample;
}

// get instance
FileIOManager& FileIOManager::getInstance() {
	// create object (This is the only object.)
	static FileIOManager mgr;
	
	return mgr;
}

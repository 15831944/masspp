/**
 * @file FileIOPluginOperation.cpp
 * @brief File Io Operation class
 *
 * @author M.Izumi
 * @date 2012.01.25
 * 
 * Copyright (C) 2014 Shimadzu Corporation All rights reserved.
 */

#include "stdafx.h"
#include "FileIOPluginOperation.h"
#include "FileIOManager.h"
#include "ExportSampleDialog.h"

using namespace kome::io;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


// constructor
FileIoOperation::FileIoOperation(){

	// èâä˙âª
	m_filePath = "";
	m_index = -1;
	m_dataSet = NULL;
	m_item = NULL;
	m_target = -1;

	m_progress = NULL;	// @date 2014.07.08 <Add> M.Izumi
}

// destructor
FileIoOperation::~FileIoOperation(){	
}

// on execute
bool FileIoOperation::onExecute(){	
	bool ret = false;
	FileIOManager& mgr = FileIOManager::getInstance();
	std::string strShortName = getShortName();
	if( strShortName.compare( "file_open" ) == 0 ){
		
		// sample set
		kome::objects::SampleSet* sampleSet = mgr.openFile( m_filePath.c_str(), m_progress );
		if( sampleSet == NULL ) {					
			return ret;
		}

		if( m_progress != NULL && m_progress->isStopped() ){	// @date 2014.07.08 <Add> M.Izumi
			// managers
			kome::plugin::FileFactory& factory = kome::plugin::FileFactory::getInstance();
			factory.closeFile( sampleSet );
			
			return true;
		}

		// SPEC No.86090 @date 2012.06.11 <Add> M.Izumi
		kome::objects::Sample* sample = mgr.getOpenSample();
		if( sample == NULL ){
			return ret;
		}

		setTargetSample( sample );
		m_index = sample->getSampleIndex();
	}else if( strShortName.compare( "export_sample" ) == 0 ){
		
		// path
		kome::core::MsppManager& msppMgr = kome::core::MsppManager::getInstance();
		std::string dir = "sample_export";
		std::string tmpDir = getpath( msppMgr.getTmpDir(), dir.c_str() );
		if( !fileexists( tmpDir.c_str() ) ) {
			makedirectory( tmpDir.c_str() );
		}

		std::string fileName = msppMgr.getTmpFileName( "outputdata", ".tmp", dir.c_str() );
		std::string filePath = getpath( tmpDir.c_str(), fileName.c_str() );		
		
		kome::core::Progress* pprogress = NULL;		
		kome::window::DialogProgress* progress = NULL;
		if( kome::window::WindowTool::getMainWindow() != NULL ){
			 // progress
			 progress = new kome::window::DialogProgress( 
				kome::window::WindowTool::getMainWindow(),
				"Writing data"
			);
			pprogress = progress;
		}

		if( pprogress == NULL ){
			pprogress = &kome::core::Progress::getIgnoringProgress();
		}
		 
        if( m_dataSet.getNumberOfSpectra() == 0 && m_dataSet.getNumberOfChromatograms() == 0 ) {
			LOG_ERROR_CODE( FMT("There is no data to be saved. Open a data file first." ), ERR_OTHER );
			return false;
		}
		if( m_item == NULL ) {
			LOG_ERROR( FMT( "Failed to get the save function." ) );
			return false;
		}

		// save
		ret = mgr.exportSpectra(
			m_dataSet,
			m_item,
			filePath.c_str(),
			*pprogress,
			&m_saveSettings                           // @date 2012/8/20 <Add> FUJITA 
		);

		// move
		if( !pprogress->isStopped() ) {
			ret = copyfile( filePath.c_str(), m_filePath.c_str() );
		}
		removefile( filePath.c_str() );
		
		delete progress;
	}

	ret = true;
	return ret;
}

// on load condition
void FileIoOperation::onLoadCondition( boost::function< int ( void*, int ) > readFun ){
}

// on save condition
void FileIoOperation::onSaveCondition( boost::function< int ( void*, int ) > writeFun ){
}


// get the description
std::string FileIoOperation::onGetDescription(){
	std::string s;
	std::string strShortName = getShortName();
	if( strShortName.compare( "file_open" ) == 0 ){
		kome::objects::Sample* sample = getTargetSample();
		if( sample != NULL ){
			s.append( FMT("File Open %s:%s", sample->getName(), sample->getSampleSet()->getFileName() ) );
		}
	}else if( strShortName.compare( "export_sample" ) == 0 ){
		s.append( FMT("Export Sample: %s", m_filePath.c_str()).c_str() );
	}
	return s;
}

// gets the parameter string
std::string FileIoOperation::onGetParametersString(){
	std::string s;
	std::string strShortName = getShortName();
	if( strShortName.compare( "file_open" ) == 0 ){
		// sample index, path
		s.append( FMT("%d,%s", m_index, m_filePath.c_str() ) );
	}else if( strShortName.compare( "export_sample" ) == 0 ){
		// sample Id, spec Id, target, funcitem type, funcitem name, path
		// sample Id, , target, funcitem type, funcitem name, path
		// sample Id, group Id, target, funcitem type, funcitem name, path
		// sample Id, chrom Id, target, funcitem type, funcitem name, path
		int inum = 0;
		if( m_target == ExportSampleDialog::TARGET_SPECTRUM ){				// current spectrum
			inum = m_dataSet.getNumberOfSpectra()-1;
			kome::objects::Spectrum* spec = m_dataSet.getSpectrum( inum );
			if( spec != NULL ){
				s.append( FMT("%d,%d,",spec->getSample()->getSampleId(), spec->getId() ));
			}
		}else if( m_target == ExportSampleDialog::TARGET_FILTERED ){		// filtered spectra
			kome::objects::DataGroupNode* group = m_dataSet.getGroup();
			if( group != NULL ){
				s.append( FMT("%d,%d,", group->getSample()->getSampleId(), group->getId() ) );
			}
		}else if( m_target ==  ExportSampleDialog::TARGET_SAMPLE ){			// sample
			if( m_dataSet.getSample() != NULL ){
				s.append( FMT("%d,-1,", m_dataSet.getSample()->getSampleId() ) );
			}

		}else if( m_target == ExportSampleDialog::TARGET_CHROMATOGRAM ){	// current chromatogram
			inum = m_dataSet.getNumberOfChromatograms()-1;
			kome::objects::Chromatogram* chrom = m_dataSet.getChromatogram(inum);
			if( chrom != NULL ){
				s.append( FMT("%d,%d,", chrom->getSample()->getSampleId(), chrom->getId()));
			}
		}
		
		// target
		s.append( FMT("%d,", m_target ) );
				
		// get function
		kome::plugin::PluginCall* fun = ( m_item == NULL ) ? NULL : m_item->getCall();
		if( fun != NULL ) {			
			s.append( FMT("%s,%s",
				fun->getType(),
				fun->getProperties().getStringValue("short name",""))
			);
		}

		// path
		s.append( FMT( ",%s", m_filePath.c_str() ));

		// @date 2014.02.10 <Add> M.Izumi ->
		if( &m_saveSettings != NULL ){
			kome::plugin::SettingsPage* page = m_item->getSettingsPage();
			if( page != NULL ){
				s.append( FMT(",%s", page->getParameterString( m_saveSettings ).c_str()) );
			}
		}
		// @date 2014.02.10 <Add> M.Izumi <-
	}

	return s;
}

// sets the parameter string
void FileIoOperation::onSetParametersString( const char* strParam ){
	// sample index, path
	std::vector< std::string > tokens;
	stringtoken( strParam, ",", tokens );

	std::string strShortName = getShortName();
	if( strShortName.compare( "file_open" ) == 0 ){
		if( tokens.size() < 1 ){
			return;
		}

		m_index = toint( tokens[0].c_str(), 10, -1 );
		m_filePath = tokens[1].c_str();
	}else if( strShortName.compare( "export_sample" ) == 0 ){
		if( tokens.size() < 6 ){
			return;
		}

		// sample Id, spec Id, target, funcitem type, funcitem name, path
		// sample Id, , target, funcitem type, funcitem name, path
		// sample Id, group Id, target, funcitem type, funcitem name, path
		// sample Id, chrom Id, target, funcitem type, funcitem name, path
		int sampleId  = toint( tokens[0].c_str(), 10, -1 );
		int objectId = toint( tokens[1].c_str(), 10, -1 );
		m_target = toint(tokens[2].c_str(), 10, -1 );
		// function item
		kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();
		m_item = plgMgr.getFunctionItem( tokens[3].c_str(), tokens[4].c_str() );
		
		m_filePath = tokens[5].c_str();
		kome::objects::Sample* sample = kome::objects::Sample::getSampleById( sampleId );
		if( m_target == ExportSampleDialog::TARGET_SPECTRUM ){				// current spectrum
			if( sample != NULL ){
				kome::objects::Spectrum* spec = sample->getSpectrumById( objectId );
				if( spec != NULL ){
					kome::objects::DataSet ds( spec->getGroup() );
					ds.addSpectrum( spec );
					m_dataSet = ds;	
				}
			}

		}else if( m_target == ExportSampleDialog::TARGET_FILTERED ){		// filtered spectra
			if( sample != NULL ){
				kome::objects::ActiveObjectsManager& aoMgr = kome::objects::ActiveObjectsManager::getInstance();
				kome::objects::DataSet* ds = aoMgr.getFilteredDataSet( sample );
				if( ds != NULL ) {
					m_dataSet = *ds;
				}
			}
		}else if( m_target ==  ExportSampleDialog::TARGET_SAMPLE ){			// sample
			if( sample != NULL ){
				// data set	
				kome::objects::DataGroupNode* root = sample->getRootDataGroupNode();
				kome::objects::DataSet ds( root );
				root->getDataSet( &ds );

				m_dataSet = ds;
			}
		}else if( m_target == ExportSampleDialog::TARGET_CHROMATOGRAM ){	// current chromatogram
			if( sample != NULL ){
				kome::objects::Chromatogram* chrom = sample->getChromatogramById( objectId );
				if( chrom != NULL ){
					kome::objects::DataSet ds( chrom->getGroup() );
					ds.addChromatogram( chrom );
					m_dataSet = ds;	
				}
			}
		}

		// @date 2014.02.10 <Add> M.Izumi ->
		int index = tokens[6].find_first_of("[");
		std::string strSettings = tokens[6].substr(index);

		// setting parameter values
		kome::plugin::SettingsPage* page = m_item->getSettingsPage();
		if( page != NULL ){
			page->setParameterString( m_saveSettings, strSettings.c_str() );
		}
		// @date 2014.02.10 <Add> M.Izumi <-
	}
}

// set sample index
void FileIoOperation::setIndex( int index ){
	m_index = index;
}

// get sample index
int FileIoOperation::getIndex(){
	return m_index;
}

// set file path
void FileIoOperation::setFilePath( const char* path ){
	m_filePath = path;
}

// get file path
std::string FileIoOperation::getFilePath(){
	return m_filePath;
}

// set dataSet
void FileIoOperation::setDataSet( kome::objects::DataSet dataSet ){
	m_dataSet = dataSet;
}

// get dataSet
kome::objects::DataSet FileIoOperation::getDataSet(){
	return m_dataSet;
}

// set export sample function item
void FileIoOperation::setFuncItem( kome::plugin::PluginFunctionItem* item ){
	m_item = item;
}

// get export sample function item
kome::plugin::PluginFunctionItem* FileIoOperation::getFuncItem(){
	return m_item;
}

// set target
void FileIoOperation::setTarget( int target ){
	m_target = target;
}

// get target
int FileIoOperation::getTarget(){
	return m_target;
}

// @date 2014.07.08 <Add> M.Izumi -.
// set progress
void FileIoOperation::setProgress( kome::core::Progress* progress ){
	m_progress = progress;
}

// get progress
kome::core::Progress* FileIoOperation::getProgress(){
	return m_progress;
}
// @date 2014.07.08 <Add> M.Izumi <-

/**
 * @file FileIOPluginFunctions.cpp
 * @brief implements of FileIO plug-in function
 *
 * @author S.Tanaka
 * @date 2006.08.14
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "FileIOPluginFunctions.h"
#include "FileIOManager.h"
#include "ExportSampleDialog.h"		
#include "FileIOPluginOperation.h"	
#include "FileSampleReader.h"	// @date 2012/07/26 <Add> OKADA

using namespace kome::io;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


#define PATH_PARAM_NAME			   "path"	// setOutputFilePath()関数内で使用
#define TYPE_PARAM_NAME			   "type"
#define KEY_PARAM_NAME			   "key"
#define FORMAT_PARAM_NAME		   "format"
#define SUFFIX_PARAM_NAME		   "str"
#define SAMPLES_PARAM_NAME		   "samples"
#define FILE_SAVE_FUNC_TYPE_NAME   "SAVE_FILE"

#define CURRENT_SPECTRUM			0
#define FILTERED_SPECCTRA			1
#define SAMPLE						2


// open file
kome::objects::Variant openFileOnBatch( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = false;
	
	// managers
	FileIOManager& mgr = FileIOManager::getInstance();
	kome::objects::ActiveObjectsManager& aoMgr = kome::objects::ActiveObjectsManager::getInstance();

	// argument
	kome::objects::SettingParameterValues* settings = kome::plugin::PluginCallTool::getSettingValues( params );
	if( settings == NULL ) {
		LOG_ERROR( FMT( "Failed to get arguments information." ) );
		return ret;
	}

	// Command　line DB 対応
	// @date 2012/07/31 <Add>
	const char* szType = settings->getParameterValue( TYPE_PARAM_NAME );
	if( szType == NULL || strcmp(szType, "") == 0 ) { 
		LOG_ERROR_CODE( FMT( "Failed to get the type." ), ERR_NULL_POINTER );
		return ret;
	}
	kome::plugin::PluginFunctionItem* pItem;
	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();
	//　szType には "file","ditfp"のどちらかが入る
	pItem = plgMgr.getFunctionItem( "GET_FILE_READER", szType );

	if( pItem != NULL ){
		const char* szKey = settings->getParameterValue( KEY_PARAM_NAME );
		if( szKey == NULL || strcmp(szKey, "") == 0 ) { 
			LOG_ERROR_CODE( FMT( "Failed to get the key." ), ERR_NULL_POINTER );
			return ret;
		}

		kome::objects::SampleReader* pSampleReader = (kome::objects::SampleReader*)pItem->getCall()->invoke( NULL ).prim.pt;
		
		kome::objects::Sample* sample = NULL;	// @date 2012/12/25 <Add> OKADA

		if( pSampleReader != NULL ){
			sample = pSampleReader->openData( szKey ); // @date 2012/12/25 <Add> OKADA
		}
		
		if( sample != NULL ){					// @date 2012/12/25 <Add> OKADA
			aoMgr.setActiveSample( sample );	// @date 2012/12/25 <Add> OKADA
		}										// @date 2012/12/25 <Add> OKADA

	}

	// retun value
	ret.prim.boolVal = true;
	return ret;
}

// select file and open it
kome::objects::Variant openFileWithDialog( kome::objects::Parameters* ) {
	// create return value structure
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;

	// File IO Manager
	FileIOManager& mgr = FileIOManager::getInstance();
	
	// open
	ret.prim.boolVal = mgr.openSampleSet();
	return ret;
}

// open Sample Close Check Daialog	Add M.Izumi
kome::objects::Variant openDialog( kome::objects::Parameters* params ) {
	// create return value structure
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;

	// active object
	kome::objects::Variant var = kome::plugin::PluginCallTool::getActiveObject( params );
	kome::objects::MsDataVariant obj( var );
	kome::objects::Sample* sample = obj.getSample();

	// File IO Manager
	FileIOManager& mgr = FileIOManager::getInstance();

	// open 
	ret.prim.boolVal = mgr.openSampleCloseCheckDaialog( sample );
	return ret;
}

// open ExportSampleDialog		@date 2011.07.25 <Add> M.Izumi
kome::objects::Variant openExportSampleDialog( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = false;
		
	kome::objects::Variant var = kome::plugin::PluginCallTool::getActiveObject( params );
	kome::objects::MsDataVariant obj( var );
	// active objects manager
	kome::objects::ActiveObjectsManager& aoMgr = kome::objects::ActiveObjectsManager::getInstance();
	
	// dataSet
	kome::objects::DataSet dataSet;
	// sample
	kome::objects::Sample* sample = NULL;
	
	// File IO Manager
	FileIOManager& ioMgr = FileIOManager::getInstance();

	ExportSampleDialog dlg( kome::window::WindowTool::getMainWindow(), var );

	if( dlg.ShowModal() == wxID_OK ){ 
		ExportSampleDialog::ExportTarget target = dlg.getTarget();
		kome::plugin::PluginFunctionItem* item = dlg.getSelectedItem();

		if( target == ExportSampleDialog::TARGET_SPECTRUM ){		// current spectrum
			kome::objects::Spectrum* spec = obj.getSpectrum();
			if( spec == NULL ) {
				LOG_ERROR( FMT( "A spectrum is not active." ) );
			
				return ret;
			}

			kome::objects::DataSet ds( spec->getGroup() );
			ds.addSpectrum( spec );
			dataSet = ds;	
	
		}else if( target == ExportSampleDialog::TARGET_FILTERED ){	// filtered spectra
			kome::objects::Sample* sample = obj.getSample();
			kome::objects::DataSet* ds = aoMgr.getFilteredDataSet( sample );
			if( sample == NULL || ds == NULL ) {
				LOG_ERROR( FMT( "A group is not active." ) );
			
				return ret;
			}
			dataSet = *ds;
	
		}else if( target == ExportSampleDialog::TARGET_SAMPLE ){			// sample
			// sample
			sample = obj.getSample();
		
			if( sample == NULL ) {
				LOG_ERROR( FMT( "A sample is not active." ) );
				return ret;
			}

			// each group
			for( unsigned int i = 0; i < sample->getNumberOfGroups(); i++ ) {
				kome::objects::DataGroupNode* group = sample->getGroup( i );

				// each spectra
				for( unsigned int j = 0; j < group->getNumberOfSpectra(); j++ ) {
					kome::objects::Spectrum* spec = group->getSpectrum( j );
					dataSet.addSpectrum( spec );
				}

				// each chromatograms
				for( unsigned int j = 0; j < group->getNumberOfChromatograms(); j++ ) {
					kome::objects::Chromatogram* chrom = group->getChromatogram( j );
					if( !chrom->isAutoCreated() ) {
						dataSet.addChromatogram( chrom );
					}
				}
			}
		}

		// @date 2011/11/30 <Add> OKADA ------->
		else if( target == ExportSampleDialog::TARGET_CHROMATOGRAM ){
			kome::objects::Chromatogram* chrom = obj.getChromatogram();
			if( chrom == NULL ) {
				LOG_ERROR( FMT( "A chromatogram is not active." ) );
			
				return ret;
			}
			kome::objects::DataSet ds( chrom->getGroup() );
			ds.addChromatogram( chrom );
			dataSet = ds;	
		}
		// @date 2011/11/30 <Add> OKADA <-------
	
		// save
		bool cancelFlg = false;
		ret.prim.boolVal = ioMgr.exportSpectraWithDialog( dataSet, item, true, &cancelFlg );

		if( target == ExportSampleDialog::TARGET_SAMPLE && ret.prim.boolVal == true && !cancelFlg ){
			sample->setEdited(false);
			// Update the Sample Tree view to show that the sample has been saved
			kome::plugin::PluginCallTool::onUpdateSample( *sample );
		}
	}
	else
	{
		// Selecting Cancel on the dialog should not cause an error to appear.
		ret.prim.boolVal = true;
	}
	
	return ret;
}

// close sample
kome::objects::Variant closeOpenedSample( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = false;

	// sample
	kome::objects::Variant var = kome::plugin::PluginCallTool::getActiveObject( params );
	kome::objects::MsDataVariant obj( var );
	kome::objects::Sample* sample = obj.getSample();
	if( sample == NULL ) {
		LOG_ERROR( FMT( "A sample is not active." ) );
		return ret;
	}

	// File IO Manager
	FileIOManager& ioMgr = FileIOManager::getInstance();

	// close
	ioMgr.closeSample( sample );
	ret.prim.boolVal = true;

	return ret;
}

// close all sample files
kome::objects::Variant closeAllFiles( kome::objects::Parameters* ) {
	// return value
	kome::objects::Variant ret;

	kome::objects::ActiveObjectsManager& aoMgr = kome::objects::ActiveObjectsManager::getInstance();
	int iSize = aoMgr.getNumberOfOpenedSamples();
	for( int i = 0; i < iSize; i++ ) {
		kome::objects::Sample* sample = aoMgr.getOpenedSample( i );
		kome::objects::SampleSet* sampleSet = sample->getSampleSet();
		
		if (sample->isEdited())
		{
			// Reset the edited flag before closing the sample.
			sample->setEdited(false);
		}
	}

	// close
	kome::plugin::FileFactory& factory = kome::plugin::FileFactory::getInstance();
	factory.closeFile( NULL );

	return ret;
}

// initialize GUI
kome::objects::Variant initGUI( kome::objects::Parameters* ) {
	// return value
	kome::objects::Variant ret;

	// prepare sample selector
	FileIOManager& mgr = FileIOManager::getInstance();
	mgr.setGuiSelector();

	return ret;
}

// select sample
kome::objects::Variant selectSamples( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;

	// get sample set
	kome::objects::SampleSet* sampleSet = kome::plugin::PluginCallTool::getSampleSet( params );
	if( sampleSet == NULL ) {
		return ret;
	}	

	kome::core::Progress* progress = kome::plugin::PluginCallTool::getProgress( params );

	// select sample
	FileIOManager& mgr = FileIOManager::getInstance();
	mgr.openSelectedSample( sampleSet, progress );

	return ret;
}

// set output file name suffix
kome::objects::Variant setFileNameSuffix( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = true;

	// manager
	FileIOManager& mgr = FileIOManager::getInstance();

	// argument
	kome::objects::SettingParameterValues* settings = kome::plugin::PluginCallTool::getSettingValues( params );
	if( settings == NULL ) {
		LOG_ERROR( FMT( "Failed to get arguments information." ) );
		return ret;
	}

	const char* suffix = settings->getParameterValue( SUFFIX_PARAM_NAME );
	if( suffix != NULL ) {
		mgr.setSuffix( suffix );
	}

	return ret;
}

// save data
kome::objects::Variant setOutputFilePath( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.pt = false;

	// manager
	FileIOManager& mgr = FileIOManager::getInstance();

	// argument
	kome::objects::SettingParameterValues* settings = kome::plugin::PluginCallTool::getSettingValues( params );
	if( settings == NULL ) {
		LOG_ERROR( FMT( "Failed to get arguments information." ) );
		return ret;
	}

	// output function
	const char* name = settings->getParameterValue( FORMAT_PARAM_NAME );
	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();
	kome::plugin::PluginFunctionItem* item = plgMgr.getFunctionItem( FILE_SAVE_FUNC_TYPE_NAME, name );
	if( item == NULL ) {		// illegal format name
		LOG_ERROR( FMT( "Failed to get the save function for the format." ) );
		return ret;
	}

	mgr.setSaveItem( item );

	kome::objects::SettingParameterValues* saveSettings = settings->getSubParameterValues( FORMAT_PARAM_NAME, name );
	if( saveSettings != NULL ) {
		mgr.setSaveSettings( saveSettings );
	}

	// set path
	const char* path = settings->getParameterValue( PATH_PARAM_NAME );
	if( path == NULL ) {
		LOG_ERROR_CODE( FMT( "Failed to get the file path." ), ERR_NULL_POINTER );
		return ret;
	}

	// save  
	ret.prim.boolVal = mgr.saveSharedData( path );

	return ret;
}

// set sample numbers
kome::objects::Variant setSampleNumbers( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = false;

	// manager
	FileIOManager& mgr = FileIOManager::getInstance();

	// argument
	kome::objects::SettingParameterValues* settings = kome::plugin::PluginCallTool::getSettingValues( params );
	if( settings == NULL ) {
		LOG_ERROR( FMT( "Failed to get arguments information." ) );
		return ret;
	}

	const char* samples = settings->getParameterValue( SAMPLES_PARAM_NAME );

	// selector
	kome::core::RangeListSelector* selector
		= dynamic_cast< kome::core::RangeListSelector* >( mgr.getSelector() );
	if( selector == NULL ) {
		LOG_ERROR( FMT( "Failed to get the sample selector object." ) );
		return ret;
	}

	// set range
	selector->getRangeList().importIntRanges( samples );

	ret.prim.boolVal = true;
	return ret;
}

// set sample names
kome::objects::Variant setSampleNames( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = false;

	// manager
	FileIOManager& mgr = FileIOManager::getInstance();

	// argument
	kome::objects::SettingParameterValues* settings = kome::plugin::PluginCallTool::getSettingValues( params );
	if( settings == NULL ) {
		LOG_ERROR( FMT( "Failed to get arguments information." ) );
		return ret;
	}

	const char* samples = settings->getParameterValue( SAMPLES_PARAM_NAME );

	// samples
	std::vector< std::string > names;
	stringtoken( samples, ",", names );

	for( unsigned int i = 0; i < names.size(); i++ ) {
		mgr.addSampleName( names[ i ].c_str() );	
	}

	ret.prim.boolVal = true;
	return ret;
}

// set sample types
kome::objects::Variant setSampleTypes( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = false;

	// manager
	FileIOManager& mgr = FileIOManager::getInstance();

	// argument
	kome::objects::SettingParameterValues* settings = kome::plugin::PluginCallTool::getSettingValues( params );
	if( settings == NULL ) {
		LOG_ERROR_CODE( FMT( "Failed to get arguments information." ), ERR_NULL_POINTER );
		return ret;
	}

	const char* samples = settings->getParameterValue( SAMPLES_PARAM_NAME );

	// samples
	std::vector< std::string > types;
	stringtoken( samples, ",", types );

	for( unsigned int i = 0; i < types.size(); i++ ) {
		mgr.addSampleType( types[ i ].c_str() );	
	}

	ret.prim.boolVal = true;
	return ret;
}

// print number of samples
kome::objects::Variant printNumberOfSamples( kome::objects::Parameters* ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = true;

	// manager
	kome::objects::ActiveObjectsManager& aoMgr = kome::objects::ActiveObjectsManager::getInstance();

	// sample count
	unsigned int cnt = aoMgr.getNumberOfOpenedSamples();

	printf( "Sample Count: %d\n", cnt );
	LOG_INFO( FMT( "Sample Count: %d\n", cnt ) );

	return ret;
}

// open the raw data
kome::objects::Variant openRawdataFile( kome::objects::Parameters* params ) {
	// return value
	kome::objects::Variant ret;

	// path
	char* path = kome::plugin::PluginCallTool::getPath( params );

	// add progress @date 2014.0.08 <Add> M.Izumi ->
	kome::window::DialogProgress progress( kome::window::WindowTool::getMainWindow(), "Open" );
	progress.setStatus( FMT( "File Open..." ).c_str(), true );
	progress.setRange( 0, 1 );
	// @date 2014.07.08 <Add> M.Izumi <-

	// -----------------
	// Operation 作成
	// -----------------
	FileIoOperation* opt = new FileIoOperation();
	opt->setOperationType( kome::operation::Operation::TYPE_INPUT );
	opt->setShortName( "file_open" );
	opt->setOperationName( "File Open" );

	opt->setFilePath( path );

	opt->setProgress( &progress );// @date 2014.07.08 <Add> M.Izumi

	if( opt->execute() ){
		ret.prim.boolVal = true;
	}else{
		delete opt;
	}

	return ret;
}

// get file open operation
kome::objects::Variant getFileOpenOperation( kome::objects::Parameters* ){
	// return value
	kome::objects::Variant ret;

	// create an object
	ret.prim.pt = new FileIoOperation();

	return ret;
}

// get export sample operation
kome::objects::Variant getExportSampleOperation( kome::objects::Parameters* ){
	kome::objects::Variant ret;

	// create an object
	ret.prim.pt = new FileIoOperation();

	return ret;
}

// readSampleFile
kome::objects::Variant readSampleFile( kome::objects::Parameters* ) {
	// create return value structure
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::FILE_READER;

	ret.prim.pt = new FileSampleReader();

	return ret;
}

kome::objects::Variant closeSampleFromContextMenu( kome::objects::Parameters* params ) {
	// create return value structure
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;

	// active object
	kome::objects::Variant var = kome::plugin::PluginCallTool::getActiveObject( params );
	kome::objects::MsDataVariant obj( var );
	
	kome::objects::Sample* sample = obj.getSample();
	if( sample == NULL ){ // @date 2103.06.21 <Add> M.Izumi
		return ret;
	}

	// File IO Manager
	FileIOManager& mgr = FileIOManager::getInstance();

	mgr.closeSample( sample );

	ret.prim.boolVal = true;

	// open
	return ret;
}

//　SampleTreeViewで該当する spectrum(Chromatogramではなく)をダブルクリックしたときに開くのと同じ処理
kome::objects::Variant openSpectrumFromContextMenu( kome::objects::Parameters* params ) {
	// create return value structure
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;

	// active object
	kome::objects::Variant var = kome::plugin::PluginCallTool::getActiveObject( params );
	kome::objects::MsDataVariant obj( var );

	// get spectrum
	kome::objects::Spectrum* spec = obj.getSpectrum();
	
	if( spec != NULL ) {
		spec->setOperationFlag( true ); // @date 2013.03.15<Add> M.Izumi
		if( kome::plugin::PluginCallTool::onOpenSpectrum( *spec ) ) {
			kome::plugin::PluginCallTool::openSpectrum( *spec );
		}
	}

	ret.prim.boolVal = true;

	return ret;
}


//　SampleTreeViewで該当する Chromatogramをダブルクリックしたときに開くのと同じ処理
kome::objects::Variant openChromatogramFromContextMenu( kome::objects::Parameters* params ) {
	// create return value structure
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;

	// active object
	kome::objects::Variant var = kome::plugin::PluginCallTool::getActiveObject( params );
	kome::objects::MsDataVariant obj( var );

	// get Chromatogram
	kome::objects::Chromatogram* chrom = obj.getChromatogram();

	if( chrom != NULL ) {
		// set operation flag
		chrom->setOperationFlag( true );
		if( kome::plugin::PluginCallTool::onOpenChromatogram( *chrom ) ) {
			kome::plugin::PluginCallTool::openChromatogram( *chrom );
		}
	}

	ret.prim.boolVal = true;

	return ret;
}


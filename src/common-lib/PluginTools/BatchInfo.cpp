/**
* @file BatchInfo.cpp
* @brief Batch Info
* @author OKADA, H.
* @date 2012/02/03
* Copyright (C) 2014 Shimadzu Corporation All rights reserved.
* <ï‚ë´ê‡ñæ>
*/

#include "BatchInfo.h"
#include "PluginManager.h"
#include "SettingsPage.h"
#include "SettingsForm.h"
#include "SettingsParam.h"
#include "SettingsValue.h"

using kome::plugin::BatchInfo;

// constructor
BatchInfo::BatchInfo() {
	m_strBatchName	= "";
	m_strComment	= "";
	m_vecstrFormatString.clear();
	m_vecstrInputFile.clear();
	m_mapOutputFile.clear();
	m_vectCall.clear();
	m_vecpSettings.clear();
}

// destructor
BatchInfo::~BatchInfo() {

	kome::objects::SettingParameterValues * pSettingsTemp = NULL;
	while( 0 < m_vecpSettings.size() ){
		pSettingsTemp = m_vecpSettings[m_vecpSettings.size()-1];
		m_vecpSettings.pop_back();
		if( pSettingsTemp != NULL ){
			delete pSettingsTemp;
			pSettingsTemp = NULL;
		}
	}
}

// set name
void BatchInfo::setName( std::string strBatchName )
{
	m_strBatchName = strBatchName;

	return;
};

// get name
std::string BatchInfo::getName( void )
{
	return m_strBatchName;
};

// set comment
void BatchInfo::setComment( std::string strComment )
{
	m_strComment = strComment;
};

// get comment
std::string BatchInfo::getComment( void )
{
	return m_strComment;
};

// clear input/output files
void BatchInfo::clearFiles() {
	m_vecstrFormatString.clear();
	m_vecstrInputFile.clear();
	m_mapOutputFile.clear();

	m_vectCall.clear();
	for( unsigned int i = 0; i < m_vecpSettings.size(); i++ ) {
		delete m_vecpSettings[ i ];
	}
	m_vecpSettings.clear();

}

// add output file format
void BatchInfo::addSaveFunction( std::string strFormatString )
{
	m_vecstrFormatString.push_back( strFormatString );
};

// get number of save functions
int BatchInfo::getNumberOfSaveFunctions( void )
{
	return m_vecstrFormatString.size();
};

// get save function
kome::plugin::PluginFunctionItem* BatchInfo::getSaveFunction( int nIndexOfBatch )
{
	if( getNumberOfSaveFunctions() <= nIndexOfBatch ){
		return NULL;
	}else{
		// items
		kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();

		kome::plugin::PluginFunctionItem* item = plgMgr.getFunctionItem( FILE_SAVE_FUNC_TYPE_NAME, m_vecstrFormatString[nIndexOfBatch].c_str() );

		return item;
	}
};

// get save function
const char* BatchInfo::getSaveFunctionString( int nIndexOfBatch )
{
	if( getNumberOfSaveFunctions() <= nIndexOfBatch ){
		return NULL;
	}else{
		// items
		return m_vecstrFormatString[nIndexOfBatch].c_str();
	}
};

// add input file
void BatchInfo::addInputFile( std::string strInputFile )
{
	m_vecstrInputFile.push_back( strInputFile );
};

// add input file
void BatchInfo::addInputFile( std::string strDataType, std::string strKey )
{
};

// get data-type from index number
std::string getInputFileType( int nIndex ){
	nIndex = 0;	// @date 2013/06/10 <Add> OKADA	// C4100
	return "";
}

// get number of input files
int BatchInfo::getNumberOfInputFiles( void )
{
	return m_vecstrInputFile.size();
};

// get input file
std::string BatchInfo::getInputFile( int nIndexOfBatch )
{
	if( getNumberOfInputFiles() <= nIndexOfBatch ){
		return "";
	}else{
		return m_vecstrInputFile[nIndexOfBatch];
	}
};

// set output file
void BatchInfo::setOutputFile( int nIndexOfInputFile, int nIndexOfFormat, std::string strOutputFilePath)
{
	if( nIndexOfInputFile < 0 ){
		return;	// err;
	}
	if( nIndexOfFormat < 0 ){
		return;	// err;
	}

	m_mapOutputFile[nIndexOfInputFile][nIndexOfFormat] = strOutputFilePath;
};

// get output file
std::string BatchInfo::getOutputFile( int nIndexOfInputFile, int nIndexOfFormat )
{
	return m_mapOutputFile[nIndexOfInputFile][nIndexOfFormat];
};

// set output file
void BatchInfo::setDataTypeIO( int nIndexOfInputFile, int nIndexOfFormat, std::string strDataType)
{
	if( nIndexOfInputFile < 0 ){
		return;	// err;
	}
	if( nIndexOfFormat < 0 ){
		return;	// err;
	}

	m_mapDataType[nIndexOfInputFile][nIndexOfFormat] = strDataType;
};

// get output file
std::string BatchInfo::getDataTypeIO( int nIndexOfInputFile, int nIndexOfFormat )
{
	return m_mapDataType[nIndexOfInputFile][nIndexOfFormat];
};

// get number of batch functions
int BatchInfo::getNumberOfBatchFunctions( void )
{
	return m_vectCall.size();
};

// add call
void BatchInfo::addCall( std::string strBatchProcessingFunction, std::string strParameterOfBatchProcessingFunction )
{
	// strBatchProcessingFunction : short name
	// strParameterOfBatchProcessingFunction : parameter value( separated by space )

	call_batch_tag	temp;

	temp.strBatchProcessingFunction = strBatchProcessingFunction;
	temp.strParameterOfBatchProcessingFunction = strParameterOfBatchProcessingFunction;

	m_vectCall.push_back( temp );

	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();
	kome::plugin::PluginFunctionItem* pItem = plgMgr.getFunctionItem( BATCH_TYPE_NAME, strBatchProcessingFunction.c_str() );

	kome::plugin::SettingsPage * spage = NULL;
	if( pItem != NULL ){
		spage = pItem->getSettingsPage();

		if( spage == NULL ){				// @date 2012/04/27 <Add> OKADA
			;	// ÉpÉâÉÅÅ[É^Ç™Ç»Ç¢Ç≈Ç∑	// @date 2012/04/27 <Add> OKADA
		}else{								// @date 2012/04/27 <Add> OKADA
			kome::objects::SettingParameterValues * pSettings = new kome::objects::SettingParameterValues();
			spage->setParameterString( *pSettings, strParameterOfBatchProcessingFunction.c_str() );
			m_vecpSettings.push_back( pSettings );

		}	// @date 2012/04/27 <Add> OKADA
	}

};

// get batch function
kome::plugin::PluginFunctionItem* BatchInfo::getBatchFunction( int nIndexOfBatch )
{
	if( getNumberOfBatchFunctions() <= nIndexOfBatch ){
		// error
		return NULL;
	}else{
		kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();

		kome::plugin::PluginFunctionItem* item = plgMgr.getFunctionItem( BATCH_TYPE_NAME, m_vectCall[nIndexOfBatch].strBatchProcessingFunction.c_str() );
		
		return item;
	}
};

// get batch parameter
kome::objects::SettingParameterValues* BatchInfo::getBatchParameter( int nIndexOfBatch )
{
	if( m_vecpSettings.size() <= (unsigned int)nIndexOfBatch ){
		// error
		return NULL;
	}
	return m_vecpSettings[nIndexOfBatch];
};

// get batch function
int BatchInfo::getBatchProcessingFunction( int nIndexOfBatch, std::string& strFunction )
{
	if( getNumberOfBatchFunctions() <= nIndexOfBatch ){
		// error
		return -1;
	}else{
		strFunction =  m_vectCall[nIndexOfBatch].strBatchProcessingFunction;
	}

	return 0;
};

int BatchInfo::getParameterOfBatchProcessingFunction( int nIndexOfBatch, std::string& strParameter )
{
	if( getNumberOfBatchFunctions() <= nIndexOfBatch ){
		// error
		return -1;
	}else{
		strParameter =  m_vectCall[nIndexOfBatch].strParameterOfBatchProcessingFunction;
	}

	return 0;
};

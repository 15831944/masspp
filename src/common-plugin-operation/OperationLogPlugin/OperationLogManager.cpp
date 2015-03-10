/**
 * @file OperationLOGManager.cpp
 * @brief interfaces of OperationManager class
 *
 * @author M.Izumi
 * @date 2011.12.08
 * 
 * Copyright (C) 2014 Shimadzu Corporation All rights reserved.
 */

#include "stdafx.h"
#include "OperationLogManager.h"
#include "OperationLogDialog.h"
#include "OperationLogHandler.h"
#include "OperationXmlHandler.h"	// @date 2013/08/15 <Add> OKADA

using namespace kome::operation;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


#define OPERATIONLOG_VIEW_STATUS_NAME "OPERATION_LOG_VIEW"


// constructor
OperationLOGManager::OperationLOGManager(){
	// initialize
	m_operationLogView = NULL;
	m_batchInfo = NULL;
	m_path = "";
}

// destructor
OperationLOGManager::~OperationLOGManager(){
	m_operationLogView = NULL;

	delete m_batchInfo;		

}

// toggle OperationLog view visiblity
void OperationLOGManager::toggleOperationLogViewVisibility(){
	
	// check the member
	if( m_operationLogView == NULL ) {
		createOperationLogView();
		return;
	}
			
	// frame
	wxWindow* wnd = kome::window::WindowTool::getMainWindow();
	kome::window::MainFrame* mainFrame = dynamic_cast< kome::window::MainFrame* >( wnd );
	if( mainFrame == NULL ) {
		return;
	}

	// toggle visibility
	mainFrame->toggleVisibleBar( m_operationLogView );
}

// create OperationLog view
void OperationLOGManager::createOperationLogView(){
	// check the member
	if( m_operationLogView != NULL ) {
		return;
	}

	// frame 
	wxWindow* wnd = kome::window::WindowTool::getMainWindow();
	kome::window::MainFrame* mainFrame = dynamic_cast< kome::window::MainFrame* >( wnd );

	// create
	m_operationLogView = new OperationLogDialog( kome::window::WindowTool::getMainWindow() );
	if( mainFrame != NULL ) {
		mainFrame->appendPane( m_operationLogView, "Operation Log", kome::window::WindowTool::DOCK_ALIGN_RIGHT );
	}
}

// is visible OperationLog view
bool OperationLOGManager::isVisibleOperationLogView(){
	// frame
	wxWindow* wnd = kome::window::WindowTool::getMainWindow();
	kome::window::MainFrame* mainFrame = dynamic_cast< kome::window::MainFrame* >( wnd );
	if( mainFrame == NULL ) {
		return false;
	}

	// check
	return mainFrame->isVisibleBar( m_operationLogView );
}

// Undo処理
void OperationLOGManager::UndoFunc(){
	// operation manager object
	kome::operation::OperationManager& optMgr = kome::operation::OperationManager::getInstance();
	int iFinish = optMgr.getNumberOfFinishedOperations()-1;
	
	Operation* opt = NULL;
	bool bflg = false;
	
	while( iFinish > -1 && !bflg ){
		
		// 最後に実行された操作を取得
		opt = optMgr.getFinishedOperation( iFinish );
		if( opt != NULL ){
			int type = opt->getOperationType();
			optMgr.movetoCanseled( opt );	
				
			if( type == 2 || type == 4 ){
				iFinish--;
				if( iFinish != -1 ){
					continue;
				}
			}
			bflg = true;
		
			std::string strPath;
			strPath = optMgr.getPreviousDataFilePath( opt );
			FILE* fp = fileopen( strPath.c_str(), "rb" );
			if( fp == NULL ) {
				return;
			}
			
			// read
			kome::core::FileAccessor acc( fp );
			opt->loadCondition( boost::bind( &kome::core::FileAccessor::read, &acc, _1, _2 ) );
			
			fclose( fp );
		}
	}
}

// Redo処理
void OperationLOGManager::RedoFunc(){
	// operation manager object
	kome::operation::OperationManager& optMgr = kome::operation::OperationManager::getInstance();
	int iCansel = optMgr.getNumberOfCanceledOperations()-1;

	Operation* opt = NULL;
	bool bflg = false;

	while( iCansel > -1 && !bflg ){
		// 最後にキャンセルされた操作を取得
		Operation* opt = optMgr.getCanceledOperation( iCansel );
		if( opt != NULL ){
			int type = opt->getOperationType();
			optMgr.moveToFinished( opt );
			if( type == 2 || type == 4 ){
				iCansel--;
				if( iCansel != -1 ){
					continue;
				}
			}
			bflg = true;

			std::string strPath;
			strPath = optMgr.getUpdateDataFilePath( opt );

			FILE* fp = fileopen( strPath.c_str(), "rb" );
			if( fp == NULL ) {
				return;
			}
			
			// read
			kome::core::FileAccessor acc( fp );
			opt->loadCondition( boost::bind( &kome::core::FileAccessor::read, &acc, _1, _2 ) );
			
			fclose( fp );	
		}
	}
}

// OperatonLog List 更新
void OperationLOGManager::UpdateOperatonLogList(){
	if( isVisibleOperationLogView() ){
		m_operationLogView->updateLogList();
	}
}

// release view
void OperationLOGManager::releaseView() {
	// check the member
	if( m_operationLogView == NULL ) {
		return;
	}

	// frame
	wxWindow* wnd = kome::window::WindowTool::getMainWindow();
	kome::window::MainFrame* mainFrame = dynamic_cast< kome::window::MainFrame* >( wnd );

	// set status
	kome::objects::StatusManager& statusMgr = kome::objects::StatusManager::getInstance();
	statusMgr.setValue( OPERATIONLOG_VIEW_STATUS_NAME, mainFrame->isVisibleBar( m_operationLogView ) ? "on" : "off" );

	// release
	m_operationLogView = NULL;
}

// get batch info
kome::plugin::BatchInfo* OperationLOGManager::getBatchInfo(){
	if( m_batchInfo != NULL ){
		delete m_batchInfo;
	}
	return m_batchInfo = new kome::plugin::BatchInfo();
}

// on close sample
void OperationLOGManager::onCloseSample( kome::objects::Sample* sample, const bool deleting ){
	// check the map
	if( sample == NULL ) {
		return;
	}
	kome::operation::OperationManager& mgr = kome::operation::OperationManager::getInstance();
	unsigned int inum = mgr.getNumberOfOperations()-1;

	while( inum < -1 ){
		kome::operation::Operation* opt = mgr.getOperation( inum );
		if( opt != NULL ){
			// close sample Operation Log 削除
			if( opt->getTargetSample() == sample ){
				delete opt;
				opt = NULL;
			}
		}

		inum--;
	}
	
	// list update
	UpdateOperatonLogList();
}

// open FileSave dialog
void OperationLOGManager::openFileSaveDialog(){
	
	// file save dialog
	wxFileDialog dlg(
		kome::window::WindowTool::getMainWindow(),
		wxT( "Save As" ),
		wxEmptyString,
		wxEmptyString,
		wxT( "mspXml File (*.mspXML)|*.mspXML" ),	// @date 2013.09.04 <Mod> M.Izumi
		wxFD_SAVE | wxFD_OVERWRITE_PROMPT
	);

	if( dlg.ShowModal() == wxID_OK ) {
		// get path
		m_path = dlg.GetPath();
				
		saveMspXmlFile( dlg.GetPath() );
	
	}else{
		kome::operation::OperationManager& optMgr = kome::operation::OperationManager::getInstance();
		optMgr.setSaveCancelFlg( true );
	}
}

// save msp xml file
void OperationLOGManager::saveMspXmlFile( const char* path ){
	// open the file
	FILE* fp = fileopen( path, "wb" );
	if( fp == NULL ) {
		LOG_ERROR_CODE( FMT( "Failed to open the file for writing. Check file / folder permissions. [%s]", path ), ERR_FILE_OPEN_FAILED );
		return;
	}
		
	kome::operation::OperationManager& mgr = kome::operation::OperationManager::getInstance();
	
	mgr.setSaveCount( mgr.getNumberOfOperations() );

	// wait cursor
	wxBeginBusyCursor();	

	// write xml file
	writeXmlFile( fp );

	// restore cursor
	wxEndBusyCursor();
	
	// close
	fflush( fp );
	fclose( fp );
}

// write xml file
void OperationLOGManager::writeXmlFile( FILE* fp ){
	
	fprintf( fp, "<?xml version=\"1.0\"?>\n" );
	fprintf( fp, "<operations>\n" );

	writeOperations( fp );
		
	fprintf( fp, "</operations>\n" );	
}

// write operations
void OperationLOGManager::writeOperations( FILE* fp ){
	kome::operation::OperationManager& mgr = kome::operation::OperationManager::getInstance();
	// finish operation
	unsigned int inum = mgr.getNumberOfFinishedOperations();
	
	for( unsigned int i=0; i < inum; i++ ){

		kome::operation::Operation* opt = mgr.getOperation( i );
		std::string strType = tolowercase( getOperationType( opt->getOperationType() ).c_str() );		

		fprintf( fp, "	<operation type=\"%s\" state=\"%d\">\n", strType.c_str(), mgr.getState(i) );
		fprintf( fp, "	  <name>%s</name>\n", opt->getOperationName() );
		fprintf( fp, "	  <short-name>%s</short-name>\n", opt->getShortName() );
		fprintf( fp, "	  <desc>%s</desc>\n", opt->getDescription() );
		fprintf( fp, "	  <param>%s</param>\n", opt->getParametersString() );
		
		fprintf( fp, "	  <data>\n" );
		
		std::string strPrevPath = opt->getPrevFilePath();	
		std::string strUpdatePath = opt->getUpdateFilePath();	
		
		if( !writeDataTag( fp, strPrevPath.c_str(), true ) ){
			fprintf( fp, "		 <before></before>\n" );	
		}
		if( !writeDataTag( fp, strUpdatePath.c_str(), false ) ){
			fprintf( fp, "		 <after></after>\n" );	
		}

		fprintf( fp, "	  </data>\n" );
				
		fprintf( fp, "	</operation>\n" );
	}
}

// write data tag
#define	IO_BUFFER_SIZE			( 32 * 1024 )

bool OperationLOGManager::writeDataTag( FILE* fp, const char* path, bool bprev )
{
// >>>>>>	@Date:2013/09/09	<Changed>	A.Ozaki
// ファイルサイズがあまりにも大きいと出力できずに落ちてしまうので、処理を変更しました
//
	FILE	*fpData = fileopen( path, "rb" );

	if  ( (FILE *)NULL == fpData )
	{		
		return	false;
	}

	unsigned char	ucData[IO_BUFFER_SIZE];
	char	cBase64[IO_BUFFER_SIZE * 2 + 10];
	size_t	szReadSize = 0;

	if  ( true == bprev )
	{
		// before
		fprintf( fp, "		 <before>" );
	}
	else
	{
		// after
		fprintf( fp, "		 <after>" );			
	}

	while ( 1 )
	{
		szReadSize = fread( (void *)ucData, (size_t)1, (size_t)IO_BUFFER_SIZE, fpData );
		memset( (void *)cBase64, 0x00, sizeof( cBase64 ) );
		int	nBase64Size = kome::core::Base64::encode( ucData, szReadSize, cBase64, sizeof( cBase64 ) );
		fprintf( fp, "%s", cBase64 );
		if  ( szReadSize < IO_BUFFER_SIZE )
		{
			break;
		}
	}
	fclose( fpData );
	
	if  ( true == bprev )
	{
		// before
		fprintf( fp, "</before>\n" );
	}
	else
	{
		// after
		fprintf( fp, "</after>\n" );			
	}

	return true;
}

// open msp xml file
void OperationLOGManager::openXmlFile( const char* path ){	
	setPath( path );

	// parse
	OperationLogHandler handler;
	handler.parse( path );

	if( handler.isError() ){
		kome::window::WindowTool::showError( "Failed to open msp xml file." );
	}		
}

// int → std::string
std::string OperationLOGManager::getOperationType( int index ){
	std::string s;

	switch ( index ){
//	case 0:
	case OPERATION_TYPE_STANDARD:
		s = "STANDARD";
		break;
//	case 1:
	case OPERATION_TYPE_GUI:
		s = "GUI";
		break;
//	case 2:
	case OPERATION_TYPE_INPUT:
		s = "INPUT";
		break;
//	case 4:
	case OPERATION_TYPE_OUTPUT:
		s = "OUTPUT";
		break;
//	case 8:
	case OPERATION_TYPE_AUTO:
		s = "AUTO";
		break;
	default:
		break;
	}

	return s;
}

// char → int
int OperationLOGManager::getOperationType( const char* type ){
	int index = -1;

	if( strcmp( type , "standard" ) == 0 ){
//		index = 0;
		index = OPERATION_TYPE_STANDARD;
	}else if( strcmp( type, "gui" ) == 0 ){
//		index = 1;
		index = OPERATION_TYPE_GUI;
	}else if( strcmp( type, "input" ) == 0 ){
//		index = 2;
		index = OPERATION_TYPE_INPUT;
	}else if( strcmp( type, "output" ) == 0 ){
//		index = 4;
		index = OPERATION_TYPE_OUTPUT;
	}else if( strcmp( type, "auto" ) == 0 ){
//		index = 8;
		index = OPERATION_TYPE_AUTO;
	}

	return index;
}

// read operation
void OperationLOGManager::readOperation( 
	kome::operation::Operation* opt, 
	int state,
	std::string prevPath,
	std::string upDatePath 
){
	// manager
	kome::operation::OperationManager& optMgr = kome::operation::OperationManager::getInstance();
	
	// input, output
	if( opt->getOperationType() == kome::operation::Operation::TYPE_INPUT ||
		opt->getOperationType() == kome::operation::Operation::TYPE_OUTPUT ){

			if( opt->execute() ){
						
			}else{
				delete opt;
			}
	// other
	}else{
	
		FILE* fp;
		if( state == 0 ){ // finish
			
			fp = fileopen( upDatePath.c_str(), "rb" );

		}else if( state == 1 ){ // cancel
			fp = fileopen( prevPath.c_str(), "rb" );
		}

		if( fp != NULL ){
			kome::core::FileAccessor acc( fp );
			opt->loadCondition( boost::bind( &kome::core::FileAccessor::read, &acc, _1, _2 ) );
			fclose( fp );
		}

		UpdateOperatonLogList();
	}
}

// open msp xml file
bool OperationLOGManager::execXmlFileFromCommandLine( const char* path ){	
//	setPath( path );

	// parse
	OperationXmlHandler handler;
	handler.parse( path );

	if( handler.isError() ){
		printf( "Failed to open msp xml file." );
	}	

	std::vector< OperationXmlHandler::XmlCommand_t > xmlcommands;
	handler.getXmlCommands( xmlcommands );

	// バッチ操作
	size_t uCnt = xmlcommands.size();

	for( size_t i=0; i<uCnt; i++ ){
		// 対象となるバッチ操作は、GUI, Input, Output以外のものとする
		switch( xmlcommands[i].optType ){
		
		case OPERATION_TYPE_GUI:	// fall-throw
			break;
	
		default:
			// -operation 実施
			kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();
			kome::plugin::PluginFunctionItem* item;	

			item = plgMgr.getFunctionItem( "batch", "operation" );
			if( item != NULL ){
				kome::objects::SettingParameterValues settings;
				settings.setValue( "operation", xmlcommands[i].shortName.c_str() );
				settings.setValue( "parameter", xmlcommands[i].param.c_str() );

				// execute
				try{
					kome::objects::Parameters params;
					kome::plugin::PluginCallTool::setSettingValues( params, settings );

					kome::objects::Variant ret = item->getCall()->invoke( &params );
					if( !ret.prim.boolVal ) {
						return false;
					}
				}
				catch( ... ) {
					return false;
				}

			}

			break;
		}
	}

	return true;	// success
}

// get instance
OperationLOGManager& OperationLOGManager::getInstance(){
	// create object (This is the only object.)
	static OperationLOGManager mgr;

	return mgr;
}

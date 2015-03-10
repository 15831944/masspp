/**
 * @file OperationLogFunctions.cpp
 * @brief implements of OperationManager plug-in functions
 *
 * @author M.Izumi
 * @date 2011.7.19 
 * 
 * Copyright (C) 2014 Shimadzu Corporation All rights reserved.
 */

#include "stdafx.h"
#include "OperationLogFunctions.h"
#include "OperationLOGManager.h"


using namespace kome::operation;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// open operations dialog
kome::objects::Variant toggleOperationDialog( kome::objects::Parameters* ){
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = true;

	// toggle visibility
	OperationLOGManager& mgr = OperationLOGManager::getInstance();
	mgr.toggleOperationLogViewVisibility();
	
	return ret;
}

// chec operation log dialog
kome::objects::Variant checkOperationLogDialog( kome::objects::Parameters* ){
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = false;

	// check the visibility
	OperationLOGManager& mgr = OperationLOGManager::getInstance();
	ret.prim.boolVal = mgr.isVisibleOperationLogView();

	return ret;
}

// Undo処理
kome::objects::Variant ProcUndo( kome::objects::Parameters* ){
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = true;
	// Undo処理
	OperationLOGManager& mgr = OperationLOGManager::getInstance();
	mgr.UndoFunc();
	
	return ret;
}

// Redo処理
kome::objects::Variant ProcRedo( kome::objects::Parameters* ){
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = true;
	// Redo処理
	OperationLOGManager& mgr = OperationLOGManager::getInstance();
	mgr.RedoFunc();
	return ret;
}

// Log List 更新
kome::objects::Variant SendOptInfo( kome::objects::Parameters* ){
	// return value
	kome::objects::Variant ret;
	
	OperationLOGManager& mgr = OperationLOGManager::getInstance();
	mgr.UpdateOperatonLogList();

	return ret;
}

// IniファイルにOperation Log Viewの表示状態を保存
kome::objects::Variant exitGui( kome::objects::Parameters* ){
	// return value
	kome::objects::Variant ret;

	// release view
	OperationLOGManager& mgr = OperationLOGManager::getInstance();
	mgr.releaseView();

	return ret;
}

// init operatino log view 
kome::objects::Variant initOperationLogView( kome::objects::Parameters* ){
	// return value
	kome::objects::Variant ret;

	// create view
	OperationLOGManager& mgr = OperationLOGManager::getInstance();
	mgr.createOperationLogView();

	return ret;
}

// enable check Menu Undo
kome::objects::Variant enableMenuUndo( kome::objects::Parameters* params ){
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = false;

	kome::operation::OperationManager& optMgr = kome::operation::OperationManager::getInstance();
	int ifinish = optMgr.getNumberOfFinishedOperations();
	if( ifinish != 0 ){
		ret.prim.boolVal = true;
	}

	return ret;
}

// enable check Menu Redo
kome::objects::Variant enableMenuRedo( kome::objects::Parameters* params ){
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = false;

	kome::operation::OperationManager& optMgr = kome::operation::OperationManager::getInstance();
	int icansel = optMgr.getNumberOfCanceledOperations();
	if( icansel != 0 ){
		ret.prim.boolVal = true;
	}

	return ret;
}

// batch_set BatchInfo*へのポインタを返す
#if 1
kome::objects::Variant batchSetFunction( kome::objects::Parameters* params ){
	// return value
	kome::objects::Variant ret;
		
	// オペレーション取得
	kome::operation::OperationManager& mgr = kome::operation::OperationManager::getInstance();
	int ifinished = mgr.getNumberOfFinishedOperations();
	
	// active sample 取得
	kome::objects::Sample* acSample = kome::plugin::PluginCallTool::getSample( params );	// @date 2013/08/19 <Add> OKADA

	if( acSample == NULL ){
		return ret;
	}

	// BatchInfo クラス取得
	kome::operation::OperationLOGManager& logMgr = kome::operation::OperationLOGManager::getInstance();
	kome::plugin::BatchInfo* batchInfo = logMgr.getBatchInfo();
	if( batchInfo == NULL ){
		return ret;
	}

	kome::operation::Operation* opt = NULL;
	for( int i=0; i < ifinished; i++ ){
		opt = mgr.getFinishedOperation(i);
		// アクティブサンプルと一致, TYPE_GUI,TYPE_INPUT,TYPE_OUTPUT以外
		if( opt->getTargetSample() == acSample &&
			opt->getOperationType() != kome::operation::Operation::TYPE_GUI && 
			opt->getOperationType() != kome::operation::Operation::TYPE_INPUT &&
			opt->getOperationType() != kome::operation::Operation::TYPE_OUTPUT){
			// add call
			batchInfo->addCall( opt->getShortName(), opt->getParametersString() );
		}
	}
	ret.prim.pt = batchInfo;
	return ret;
}
#else
// @date 2013/08/19 <Add> OKADA ------->
kome::plugin::BatchInfo* batchSetFunction( kome::objects::Sample* acSample ){
	// return value
//	kome::objects::Variant ret;
		
	// オペレーション取得
	kome::operation::OperationManager& mgr = kome::operation::OperationManager::getInstance();
	int ifinished = mgr.getNumberOfFinishedOperations();
	
	// active sample 取得
//	kome::objects::ActiveObjectsManager& actMgr = kome::objects::ActiveObjectsManager::getInstance();
//	kome::objects::Sample* acSample = actMgr.getActiveSample();
	if( acSample == NULL ){
		return NULL;
	}

	// BatchInfo クラス取得
	kome::operation::OperationLOGManager& logMgr = kome::operation::OperationLOGManager::getInstance();
	kome::plugin::BatchInfo* batchInfo = logMgr.getBatchInfo();
	if( batchInfo == NULL ){
		return NULL;
	}

	kome::operation::Operation* opt = NULL;
	for( int i=0; i < ifinished; i++ ){
		opt = mgr.getFinishedOperation(i);
		// アクティブサンプルと一致, TYPE_GUI,TYPE_INPUT,TYPE_OUTPUT以外
		if( opt->getTargetSample() == acSample &&
			opt->getOperationType() != kome::operation::Operation::TYPE_GUI && 
			opt->getOperationType() != kome::operation::Operation::TYPE_INPUT &&
			opt->getOperationType() != kome::operation::Operation::TYPE_OUTPUT){
			// add call
			batchInfo->addCall( opt->getShortName(), opt->getParametersString() );
		}
	}
//	ret.prim.pt = batchInfo;
	return batchInfo;
}
// @date 2013/08/19 <Add> OKADA <-------
#endif




// get operation list
kome::objects::Variant getOperationList( kome::objects::Parameters* params ){
	// return value
	kome::objects::Variant ret;
	// vector
	kome::objects::Variant var = params->getValue( "enumeration" );
	std::vector< kome::plugin::SettingsValue::EnumItem >* enumeration
		= (std::vector< kome::plugin::SettingsValue::EnumItem >*)var.prim.pt;
	if( enumeration == NULL ) {
		LOG_ERROR( FMT( "Failed to get the enumeration item for the operation list" ) );
		return ret;
	}

	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();
	unsigned int iCount = plgMgr.getNumberOfFunctionItems("operation");
	
	if (iCount > 0)
	{
		//.xmlファイルの　batchexecutableが"true"のものだけ取得	
		std::vector< kome::plugin::PluginFunctionItem* > tmpItem;

		for ( unsigned int i = 0; i < iCount; i++)
		{
			kome::plugin::PluginFunctionItem* item = plgMgr.getFunctionItem( "operation", i );
			if( item != NULL ){		
				kome::core::Properties& properties = item->getCall()->getProperties();
				std::string str = properties.getStringValue("batchexecutable","false");
			
				if( str.compare( "true" ) == 0 ){
					tmpItem.push_back( item );					
				}
			}
		}

		// enumerations
		enumeration->resize( tmpItem.size() );
		for( unsigned int i = 0; i < tmpItem.size(); i++ ){
			( *enumeration )[ i ].name = tmpItem[i]->getLongName();
			( *enumeration )[ i ].value = tmpItem[i]->getShortName();
		}
	}
	else
	{
		LOG_ERROR( FMT( "The operation list is empty." ) );
	}
		
	return ret;
}

// run batch operations [ %msppcmd -operation operation parameter ]
kome::objects::Variant runBatchOperations( kome::objects::Parameters* params ){
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = false;

	kome::objects::SettingParameterValues* settings = kome::plugin::PluginCallTool::getSettingValues( params );
	if( settings == NULL ){
		return ret;
	}

	// 第一引数
	const char* shortName = settings->getParameterValue( "operation" );
	std::string  strParam = NVL( shortName, "" );
	if( strParam.empty() ){
		return ret;
	}

	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();
	kome::plugin::PluginFunctionItem* item;	
	kome::operation::Operation* opt= NULL;	
	
	item = plgMgr.getFunctionItem( "operation", shortName );
		
	if( item != NULL ){			
		wxString str = item->getShortName();
		kome::core::Properties& properties = item->getCall()->getProperties();
		std::string strbatchexecute = properties.getStringValue("batchexecutable","false");

		if( strParam.compare( str ) == 0 && strbatchexecute.compare("true") == 0 ){
			opt = (kome::operation::Operation*)item->getCall()->invoke( NULL ).prim.pt;
			if( opt != NULL ){
				opt->setOperationName( item->getLongName() );
				opt->setShortName( item->getShortName() );
				opt->setDescription( item->getCall()->getDescription() );
			}
		}
	}

	
	if( opt != NULL ){
		// 第二引数
		std::string  str = settings->getParameterValue( "parameter" );
		if( str.empty() ){
			return ret;
		}
		
		opt->setParametersString(str.c_str());
		ret.prim.boolVal = opt->execute();	// execute
				
	}

	return ret;
}

// save msp xml file
kome::objects::Variant SaveMspXmlFile( kome::objects::Parameters* params ){
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = false;

	// manager
	OperationLOGManager& mgr = OperationLOGManager::getInstance();

	if( strcmp( mgr.getPath(),"" ) != 0 ){
		mgr.saveMspXmlFile( mgr.getPath() );
	}else{		
		mgr.openFileSaveDialog();
	}

	ret.prim.boolVal = true;
	return ret;
}

// save as msp xml file
kome::objects::Variant SaveAsMspXmlFile( kome::objects::Parameters* params ){
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = false;
		
	// manager
	OperationLOGManager& mgr = OperationLOGManager::getInstance();

	mgr.openFileSaveDialog();
	
	ret.prim.boolVal = true;
	return ret;
}

// open masp xml file
kome::objects::Variant openMspXmlFile( kome::objects::Parameters* params ){
	// create return value structure
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = false;

	// manager
	OperationLOGManager& mgr = OperationLOGManager::getInstance();
	kome::objects::ActiveObjectsManager& aoMgr = kome::objects::ActiveObjectsManager::getInstance();
	kome::operation::OperationManager& optMgr = kome::operation::OperationManager::getInstance();
	
	bool bflg = true;
	// 開いているサンプルがあるか
	int inum = aoMgr.getNumberOfOpenedSamples();
	if( inum > 0 ){
		// 確認ダイアログ表示
		if( !kome::window::WindowTool::showOkCancel( "Opened samples will be closed." ) ){
			bflg = false;
		}else{
			// 保存したオペレーション数
			int saveCount = optMgr.getSaveCount();
			if( saveCount < 0 || saveCount != optMgr.getNumberOfOperations() ){
				int iYesNoCancel = kome::window::WindowTool::showYesNoCancel( "There are unsaved changes. Do you want to save the changes?" );

				if( iYesNoCancel == 1 ){
					// yes
					if( strcmp( "", mgr.getPath() ) == 0 ){
						mgr.openFileSaveDialog();
					}else{
						mgr.saveMspXmlFile( mgr.getPath() );
					}
				
				}else if( iYesNoCancel == 0 ){
					//bflg = false;
				}else if( iYesNoCancel == -1 ){
					bflg = false;
				}
			}

			if( bflg ){ 
				inum--;
				while( inum > -1 ){
					kome::objects::Sample* sample = aoMgr.getOpenedSample( inum );
					if( sample != NULL ){
						// close
						if( sample->isOpened() ) {

							if (sample->isEdited() && kome::window::WindowTool::getMainWindow() != NULL )
							{
								// Reset the edited flag before closing the sample.
								sample->setEdited(false);

							}

							sample->closeSample();
						}
					}
					inum--;
				}
			

				// sample id reset
				kome::objects::Sample::resetId();
			}
		}
	}
	
	if( bflg ){
		// file dialog
		wxFileDialog dlg(
			kome::window::WindowTool::getMainWindow(),
			wxFileSelectorPromptStr,
			wxEmptyString,
			wxEmptyString,
			wxT( "mspXml Files (*.mspXML)|*.mspXML" ), // @date 2013.09.04 <Mod> M.Izumi
			wxOPEN | wxFILE_MUST_EXIST
		);
		
		if( dlg.ShowModal() == wxID_OK ) {
			// wait cursor
			wxBeginBusyCursor();

			std::string path = dlg.GetPath();
			mgr.openXmlFile( path.c_str() );		
			
			// restore cursor
			wxEndBusyCursor();
		}
	}

	ret.prim.boolVal = true;
	return ret;
}

// exit
kome::objects::Variant exitFuncSaveMspXml( kome::objects::Parameters*  ){
	// return value
	kome::objects::Variant ret;
	
	// manager
	OperationLOGManager& mgr = OperationLOGManager::getInstance();
	OperationManager& optMgr = OperationManager::getInstance();
			
	bool bflg = optMgr.getSaveCountFlg();
	if( bflg ){
			
		// wait cursor
		wxBeginBusyCursor();
			
		if( strcmp( "", mgr.getPath() ) == 0 ){
			mgr.openFileSaveDialog();
		}else{
			mgr.saveMspXmlFile( mgr.getPath() );
		}

		// restore cursor
		wxEndBusyCursor();
	}
		
	return ret;
}

// run batch operations [ %msppcmd -operation_xml xmlOperation-file ]
kome::objects::Variant runBatchOperationXml( kome::objects::Parameters* params ){
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = false;

	kome::objects::SettingParameterValues* settings = kome::plugin::PluginCallTool::getSettingValues( params );
	if( settings == NULL ){
		return ret;
	}

	// 第一引数
	const char* shortName = settings->getParameterValue( "operation_xml" );
	std::string  strParam = NVL( shortName, "" );
	if( strParam.empty() ){
		return ret;
	}
	
	////////////////////////////////////////
	//

	// manager
	OperationLOGManager& mgr = OperationLOGManager::getInstance();

	ret.prim.boolVal = mgr.execXmlFileFromCommandLine( strParam.c_str() );

	//
	////////////////////////////////////////

	return ret;
}


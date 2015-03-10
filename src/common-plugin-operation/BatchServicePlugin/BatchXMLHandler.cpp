
/**
 * @file BatchXMLHandler.cpp
 * @brief interfaces of BatchXmlHandler class
 *
 * @author okada_h
 * @date 2012.02.14
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation All rights reserved.
 */

#include "stdafx.h"
#include "BatchXMLHandler.h"

using namespace kome::batchservice;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



#define STR_DEFAULT_VALUE	"DEFAULT_VALUE"


BatchXmlHandler::BatchXmlHandler()
{
	return;
}

BatchXmlHandler::~BatchXmlHandler()
{
	return;
}

void BatchXmlHandler::Initialize()
{
	m_vectActions.clear();

	// 初期化
	strMethodName = "";
	m_tempAction.nStage = SN_SENTINEL;
	m_tempAction.nPrecedence = -1;
	m_tempAction.strName = "";
	m_tempAction.strNameShort = "";
	m_tempAction.nBatchIndex = -1;
	m_tempAction.flgSettings = 0;
	m_tempAction.strSettings = "";

	return;
}

// start document
void BatchXmlHandler::onStartDocument()
{
	Initialize();
}

// end document
void BatchXmlHandler::onEndDocument()
{
	return;
}


// 開始タグ
void BatchXmlHandler::onStartElement(
    const char* name,
    kome::core::Properties& attrs 
) {
    if( strcmp( name,  "batch"  ) == 0 ) {    // <batch>
		m_country.name =  "" ;    // 初期化
		m_country.date =  "" ;
		m_country.code = attrs.getIntValue( "code", -1  );    // code 取得
    }
	else if( strcmp( name,  "batch-fun"  ) == 0 ) {    // <batch-fun>
		std::string strTemp;
		strTemp = attrs.getStringValue( "call", STR_DEFAULT_VALUE );
		if( strTemp == STR_DEFAULT_VALUE ){
			// 読み込み失敗
		}else{
			m_tempAction.strNameShort = strTemp;

			kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();
			kome::plugin::PluginFunctionItem* pItem = plgMgr.getFunctionItem( "batch",	m_tempAction.strNameShort.c_str() );
			kome::plugin::SettingsPage * spage = NULL;
			if( pItem != NULL ){
				m_tempAction.nPrecedence = pItem->getCall()->getOrder();
				m_tempAction.strName = pItem->getLongName();
			}

			// nBatchIndexを設定
			{
				unsigned int uiNum = plgMgr.getNumberOfFunctionItems( "batch" );
		
				for( unsigned int i=0; i<uiNum; i++ ){
					kome::plugin::PluginFunctionItem* pItemTemp = plgMgr.getFunctionItem( "batch", i );
					if( pItemTemp == NULL ){
						// error
					}else{
						std::string strShortName = pItemTemp->getShortName();
						if( m_tempAction.strNameShort == strShortName ){
							m_tempAction.nBatchIndex = uiNum;
							break;
						}
					}
				}
			}
			// nStageを取得
			{
				BatchServicePluginManager &mgr = BatchServicePluginManager::getInstance();
				int nCntAction = mgr.getCountOfAvailableActions();
				for( int i=0; i<nCntAction; i++ ){
					Action acTmpAction;
					mgr.getAvailableAction( i, &acTmpAction );

					if( acTmpAction.strNameShort == m_tempAction.strNameShort ){
						m_tempAction.nStage = acTmpAction.nStage;
						break;
					}
				}
			}
		}
	}
}


// 終了タグ
void BatchXmlHandler::onEndElement( const char* name, const char* text ) {
    if( strcmp( name,  "functions"  ) == 0 ) {	// </functions>
        m_country.functions = text;				// functions 取得
    }

    if( strcmp( name,  "name"  ) == 0 ) {	// </name>
		strMethodName  = text;				// name(メソッド名) 取得
        m_country.name = text;				// name 取得
    }
    else if( strcmp( name,  "date"  ) == 0 ) {	// </date>
        m_country.date = text;					// date 取得
    }
    else if( strcmp( name,  "param-name"  ) == 0 ) {	// </param-name>
		//	param-name
		m_country.date = text;					// param-name 取得
    }

	// param-value
    else if( strcmp( name,  "param-value"  ) == 0 ) {	// </param-value>

		std::string strKakkoS = "[";
		std::string strKakkoE = "]";
		std::string strText   = text;

		m_tempAction.strSettings = strKakkoS + strText + strKakkoE;

		// @date 2013/03/28 <Add> OKADA ------->
		m_tempAction.vec_strSettings.push_back( strText );	
		std::string strSettingsTemp;
		strSettingsTemp.append( "[" );
		for( unsigned int i=0; i<m_tempAction.vec_strSettings.size(); i++ ){	// @date 2013/06/10 <Mod> OKADA
			if( 1 <= i ){
				strSettingsTemp.append( "," );
			}
			strSettingsTemp.append( m_tempAction.vec_strSettings[i] );
		}
		strSettingsTemp.append( "]" );
		m_tempAction.strSettings = strSettingsTemp;
		// @date 2013/03/28 <Add> OKADA <-------

		// 2012/03/27 11:57
		kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();
		kome::plugin::PluginFunctionItem* pItem = plgMgr.getFunctionItem( "batch", m_tempAction.strNameShort.c_str() );

		kome::plugin::SettingsPage * spage = NULL;
		if( pItem != NULL ){
			spage = pItem->getSettingsPage();

			kome::objects::SettingParameterValues settings;
			if( spage != NULL ){

				spage->setParameterString( settings, m_tempAction.strSettings.c_str() );

				m_tempAction.flgSettings = 1;
				m_tempAction.strSettings = spage->getParameterString( settings );
			}
		}

	}
	else if( strcmp( name,  "batch-fun"  ) == 0 ) {	// </batch-fun>

		// @date 2013/04/03 <Add> OKADA ------->
		for( unsigned int i=0; i<m_tempAction.vec_strSettings.size(); i++ ){
			std::string str = trimstring( m_tempAction.vec_strSettings[i].c_str() );
			std::string strReplaced = replacestring( str.c_str(), "\\,", "," );
			m_tempAction.vec_strSettings[i] = strReplaced;
		}
		// @date 2013/04/03 <Add> OKADA <-------

		m_vectActions.push_back( m_tempAction );	// batch-fun 取得

		// m_tempAction初期化
		strMethodName = "";
		m_tempAction.nStage = SN_SENTINEL;
		m_tempAction.nPrecedence = -1;
		m_tempAction.strName = "";
		m_tempAction.strNameShort = "";
		m_tempAction.nBatchIndex = -1;
		m_tempAction.flgSettings = 0;
		m_tempAction.strSettings = "";
		m_tempAction.vec_strSettings.clear();	// @date 2013/03/28 <Add> OKADA
	}

	else if( strcmp( name,  "batch"  ) == 0 ) {	// </batch>
		m_countries.push_back( m_country );		// 追加
	}
}

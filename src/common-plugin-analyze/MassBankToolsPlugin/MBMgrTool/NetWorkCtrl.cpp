/**
 * @file NetWorkCtrl
 * @brief NetWorkCtrl
 *
 * @author M.Fukuda
 * @date 2013.04.24
 *
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#include "../stdafx.h"
#include "NetWorkCtrl.h"

#define SECTION                    "MassBank"

#define INSTRUMENT_TYPES_KEY       "INSTRUMENT_LIST"
#define MASSBANK_URL_KEY           "MASSBANK_URL"
#define MASSBANK_URL "http://www.massbank.jp/"
#define NO_INI_DATA "NODATA"
#define MASSBANK_API  "api/services/MassBankAPI?wsdl"

using namespace kome::massbank;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


NetWorkCtrl::NetWorkCtrl()
	: m_wsdl()      // wdsl
	, url_()       // massbank URL
	, proxy_()     // proxy
	, useNetFg_(false)         // networkを使うか否か
	, changedFg_(false)        // 変更点があったか否か
	, showNG_(false)           // 接続不可の警告を出すか否か
	, hasExFg_(true)           // Ex系の命令に接続先が対応しているか否か
{}

const bool NetWorkCtrl::checkMyConnect(bool showNGFg) 
{
	showNG_ = showNGFg;
	if (onCheckNewInfoChanged()) {
		return onWsdlConnectControler();
	} else {
		return useNetFg_;
	}
}

// 以前の設定と今の内部設定を比較する。
// 変更がある(起動時はBrank->Iniの読み込みが入るので強制的に実行される
// Iniにデータが無い場合は自己保存する。
// 変更があればTrueを返す。
const bool NetWorkCtrl::onCheckNewInfoChanged()
{
	kome::core::MsppManager& msppMgr = kome::core::MsppManager::getInstance();
	kome::core::IniFile* ini = msppMgr.getIniFile();
	
	bool ret = false;

	if( ini != NULL ) {
		// if getting Ini & Target Section Not Having. Question.
		std::string url = ini->getString(SECTION, MASSBANK_URL_KEY, NO_INI_DATA);
		std::string proxy = ini->getString("Proxy","SERVER",NO_INI_DATA);

		// iniにデフォルトでURLの項目がない場合
		if (strcmp(url.c_str(), NO_INI_DATA) == 0) {
			url = MASSBANK_URL;
			LOG_INFO_CODE(FMT("Set [%s] ini Files. default Address.", url.c_str()), ERR_OK);
			ini->setString( SECTION, MASSBANK_URL_KEY, url.c_str() );
		}
		if (url_ != url) {
			url_ = url;
			ret =  true;
		}
		if (proxy_ != proxy) {
			proxy_ = proxy;
			ret = true;
		}
	}
	changedFg_ = ret;
	return ret;
}

// create wsdl control, if no create as offline. retrun faile
// wsdlで接続を試みる。Online / Offlineの監視も同時に行う。
const bool NetWorkCtrl::onWsdlConnectControler()
{
	useNetFg_ = false; // Net Info reset.

	if (onLoadWsdl()) {
		if(m_wsdl.getNumberOfBindings() != 0) {
			useNetFg_ = true;
			hasExFg_ = onCheckSOAP("searchSpectrumEx");
			LOG_INFO( FMT("Successful MassBank connection to : %s ", url_.c_str()));
		} else {
			LOG_ERROR(FMT("Failed to get WSDL."));
		}
	}

	// OffLine 確定
	if (!useNetFg_) {
		LOG_INFO( FMT ("MassBank will be used offline."));
	}
	return useNetFg_;
}

// load WSDL
bool NetWorkCtrl::onLoadWsdl() 
{
	// managers
	kome::net::NetworkManager& netMgr = kome::net::NetworkManager::getInstance();

	// if No URL, kick connection proccess
	if (url_.empty()) {
		return false;
	}

	std::string url = url_;

	// get connection
	kome::net::HttpConnection* con = netMgr.getHttpConnection();
	if(con == NULL) {
		LOG_ERROR(FMT("Failed to get the connection."));
		return false;
	}

	// get WSDL
	if(url[url.length() - 1] != '/'){
		url.append("/");
	}
	url.append( MASSBANK_API );

	wxBeginBusyCursor();

	// Test Open NetWorks
	bool ret = con->openUrl(url.c_str());
	if ( ret ) {
		con->setMethod(kome::net::HttpConnection::METHOD_GET);
		con->sendRequest();

		m_wsdl.readWsdl(*con);
		if (m_wsdl.getNumberOfPortTypes() <= 0) {
			ret = false;
			m_wsdl.clear();
		}
	}

	wxEndBusyCursor();

	if (! ret && showNG_) {
		std::string emsg = FMT("Cannot connect : %s. \n", url_.c_str());
		emsg += "Check the MassBank URL and Proxy setting in the Tools -> Options dialog.";
		kome::window::WindowTool::showError(emsg.c_str(), "Connection failed.");
	}

	con->release();
	return ret;
}

// 与えられた　APIを実行する。
// メソッド名が無い場合は、リジェクトされる。
const bool NetWorkCtrl::execAPI(
	const std::string& method,
	std::string& xmlStr,
	std::vector<char>& res,
	int& errCode
){
	kome::net::WsdlBinding* binding = m_wsdl.searchMethod(method.c_str());
	int idx = -1;
	if(binding != NULL) {
		idx = binding->searchOperation(method.c_str());
	}
	// メソッド名が無い。（バージョンが違う可能性がある）
	if(binding == NULL || idx < 0) {
		LOG_ERROR(FMT("Failed to get the SOAP method. [%s]", method.c_str()));
		return false;
	}

	// connection
	kome::net::NetworkManager& netMgr = kome::net::NetworkManager::getInstance();
	kome::net::HttpConnection* con = netMgr.getHttpConnection();
	if( con == NULL ) {
		LOG_ERROR(FMT("Failed to get the connection."));
		return false;
	}
	con->setMethod(kome::net::HttpConnection::METHOD_POST);
	con->openUrl(binding->getEndPoint());
	con->setHeader("SOAPAction", binding->getAction(idx));
	con->writeToRequest(&(xmlStr[0]), xmlStr.length());
	con->sendRequest();

	kome::net::Wsdl::getSoapResponse(*con, res);
	con->release();

	if (res.empty()) return false;

	return true;
}

const bool NetWorkCtrl::onCheckSOAP(
	const std::string& method
){
	kome::net::WsdlBinding* binding = m_wsdl.searchMethod(method.c_str());
	if(binding == NULL) {
		return false;
	} else {
		if (binding->searchOperation(method.c_str()) < 0) {
			return false;
		}
	}
	return true;
}

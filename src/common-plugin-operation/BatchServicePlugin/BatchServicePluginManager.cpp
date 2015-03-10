/**
 * @file BatchServicePluginManager.cpp
 * @brief implements of plug-in functions
 *
 * @author okada_h
 * @date 2012/02/15
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation All rights reserved.
 */


#include "stdafx.h"
#include "BatchServicePluginManager.h"
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include "BatchButtonDef.h"
#include "BatchXMLHandler.h"
#include "JobListCtrl.h"	// for JobListCtrl in dispBatchJobWindow()

#define TABLE_FORMAT       "MSPP_SAVE_FORMAT"
#define COL_ID_FORMAT      "ID"
#define COL_NAME_FORMAT    "NAME"
#define COL_DESC_FORMAT    "DESCRIPTION"
#define COL_EXT_FORMAT     "EXTENSION"
#define IDX_ID_FORMAT      "INDEX_ID_BATCH_FORMAT"
#define IDX_NAME_FORMAT    "INDEX_NAME_BATCH_FORMAT"

#define TABLE_IO           "MSPP_INPUT_OUTPUT_FILE"
#define COL_ID_IO          "ID"
#define COL_JOB_IO         "JOB"
#define COL_FMT_IO         "OUTPUT_FORMAT"
#define COL_INPUT_IO       "INPUT_ID"
#define COL_PATH_IO        "PATH"
#define IDX_ID_IO          "INDEX_ID_BATCH_IO"
#define IDX_JOB_IO         "INDEX_JOB_BATCH_IO"
#define IDX_INPUT_IO       "INDEX_INPUT_BATCH_IO"
#define IDX_INPUT_FMT_IO   "INDEX_INPUT_FORMAT_IO"

#define TABLE_JOB          "MSPP_BATCH_JOB"
#define COL_ID_JOB         "ID"
#define COL_TITLE_JOB      "TITLE"
#define COL_COMMENT_JOB    "COMMENT"
#define COL_DATE_JOB       "DATE"
#define COL_SERVER_JOB     "SERVER"
#define COL_STRING_JOB     "JOB_ID"
#define COL_STATUS_JOB     "STATUS"
#define IDX_ID_JOB         "INDEX_ID_BATCH_JOB"
#define IDX_SERVER_JOB     "INDEX_SERVER_BATCH_JOB"

#define TABLE_SERVER       "MSPP_BATCH_SERVER"
#define COL_ID_SERVER      "ID"
#define COL_NAME_SERVER    "NAME"
#define COL_DESC_SERVER    "DESCRIPTION"
#define IDX_ID_SERVER      "INDEX_ID_BATCH_SERVER"
#define IDX_NAME_SERVER    "INDEX_NAME_BATCH_SERVER"

#define TABLE_FUN          "MSPP_BATCH_FUNCTION"
#define COL_ID_FUN         "ID"
#define COL_NAME_FUN       "NAME"
#define COL_DESC_FUN       "DESCRIPTION"
#define IDX_ID_FUN         "INDEX_ID_BATCH_FUN"
#define IDX_NAME_FUN       "INDEX_NAME_BATCH_FUN"

#define TABLE_CALL         "MSPP_BATCH_CALL"
#define COL_ID_CALL        "ID"
#define COL_JOB_CALL       "JOB"
#define COL_FUN_CALL       "FUNCTION"
#define COL_PARAM_CALL     "PARAMETER"
#define IDX_ID_CALL        "INDEX_ID_BATCH_CALL"
#define IDX_JOB_CALL       "INDEX_JOB_BATCH_CALL"


// OutputFolder
#define INI_OUTPUT_FOLDER_SECTION	"BatchProcessingSubmitJob"
#define INI_OUTPUT_FOLDER_KEY		"DEFAULT_DIR"
#define EXT_PARAM_NAME				"ext"		// @date 2013/05/17 <Add> OKADA

using namespace kome::batchservice;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


char* g_aszStageString[] = { 
    "1. Initialize",
    "2. Filter",
    "3. Manipulate",
    "4. Analyze",
    "5. Identify",
    "6. Filter Results",
};

typedef struct{
	stage_no	nStage;			// ステージ番号。①Initialize～⑥FilterResults
	std::string	strName;		// long name
}ActionTable;

// @date 2013/03/28 <Add> OKADA
ActionTable ga_actStepToStageNo[] = 
{
	{ SN_INITIALIZE,	wxT("INITIALIZE")	},	// ①Initialize
	{ SN_FILTER,		wxT("FILTER")		},	// ②
	{ SN_MANIPULATE,	wxT("MANIPULATE"),	},	// ③
	{ SN_ANALYZE,		wxT("ANALYZE"),		},	// ④
	{ SN_IDENTIFY,		wxT("IDENTIFY"),	},	// ⑤
	{ SN_FILTER_RESULTS,wxT("RESULTS"),		},	// ⑥Filter Results
	{ SN_SENTINEL,		"",					},
};


// constructor
BatchServicePluginManager::BatchServicePluginManager() {

	m_vAvailableActions.clear();

	m_vectXmlActions.clear();
	m_db = NULL;
	m_strOutputDirBatchProcessingSubmitJob = "";
}

// destructor
BatchServicePluginManager::~BatchServicePluginManager() {
}


void BatchServicePluginManager::initBatchServicePluginManager( void ){

	m_vAvailableActions.clear();
}


// xmlファイル内のstep文字列からstage番号を得る
stage_no BatchServicePluginManager::getStageNoFromStepString( const char * pszStep ){

	std::string strStep = pszStep;
	transform(strStep.begin(), strStep.end(), strStep.begin(), toupper);  

	int cntOfTable = 0;
#ifdef _countof
	cntOfTable = _countof(ga_actStepToStageNo);
#else
	cntOfTable = sizeof(ga_actStepToStageNo)/sizeof(ga_actStepToStageNo[0]);
#endif
	for( int i=0; i<cntOfTable; i++ ){
		std::string strOrgName = ga_actStepToStageNo[i].strName;
		transform(strOrgName.begin(), strOrgName.end(), strOrgName.begin(), toupper);

		if( strOrgName == strStep ){
			return ga_actStepToStageNo[i].nStage;
		}
	}

	return SN_SENTINEL;
}


bool operator<(const Action& left, const Action& right)
{
	return left.nStage < right.nStage ;
}


bool compare_Precedence(const Action& left, const Action& right)
{
	return left.nPrecedence < right.nPrecedence ;
}


bool compare_long_name(const Action& left, const Action& right)
{
	return left.strName < right.strName ;
}


bool compare_short_name(const Action& left, const Action& right)
{
	return left.strNameShort < right.strNameShort ;
}


int BatchServicePluginManager::setAvailableActions( Action actDat ){
	m_vAvailableActions.push_back( actDat );

	std::stable_sort( m_vAvailableActions.begin(), m_vAvailableActions.end(), compare_Precedence); //比較関数を指定してソート
	std::stable_sort( m_vAvailableActions.begin(), m_vAvailableActions.end() ); //比較関数を指定してソート

	return 0;
}


std::vector<Action>	BatchServicePluginManager::sortActionsByPrecedence( std::vector<Action>	vActions ){
	std::stable_sort( vActions.begin(), vActions.end(), compare_Precedence); //比較関数を指定して安定ソート

	return vActions;
}


void BatchServicePluginManager::sortActionsByPrecedenceRef( std::vector<Action>&	vActions ){
	std::stable_sort( vActions.begin(), vActions.end(), compare_Precedence); //比較関数を指定して安定ソート

	return;
}


int BatchServicePluginManager::getCountOfAvailableActions( void )
{ 
	return m_vAvailableActions.size();
}


int BatchServicePluginManager::getAvailableAction( int nCnt, Action * action ){
	
	int nCount = getCountOfAvailableActions();

	if( nCount <= nCnt ){
		return -1;
	}

	if( nCnt < 0 ){
		return -1;
	}

	*action = m_vAvailableActions[nCnt];

	return 0;
}


// get batch path
std::string BatchServicePluginManager::getBatchJobPath( void ){
	// バッチ用XMLファイルのdirectory

	// conf dir
	std::string strHomeDir = gethomedir();
	std::string strConfDir = getabsolutepath( strHomeDir.c_str(), ".mspp" );
	if( !fileexists( strConfDir.c_str() ) ) {
		makedirectory( strConfDir.c_str() );
	}
	std::string strBatchJobDir = getabsolutepath( strConfDir.c_str(), "batch" );
	if( !fileexists( strBatchJobDir.c_str() ) ) {
		makedirectory( strBatchJobDir.c_str() );
	}

	return strBatchJobDir;
}


// 各バッチメソッドが記録されたxmlファイル名の一覧を得る
std::vector<std::string> BatchServicePluginManager::getSettingXmlFiles( void ){

	std::string strBatchJobDir = getBatchJobPath();

	boost::filesystem::path p( strBatchJobDir, boost::filesystem::native );
	p = boost::filesystem::absolute( p );
	std::string s = p.string();

	// get file list
	std::vector< std::string > pathArr;
	if( boost::filesystem::is_directory( p ) ) {
		// p はディレクトリです
		boost::filesystem::directory_iterator end;
		for( boost::filesystem::directory_iterator it( p );	it != end; it++ ) {
			std::string strFileName = it->path().leaf().string();

			{	// ".xml"ファイルかどうかチェックする
				std::string strExt = getext( strFileName.c_str() );
				if( compareignorecase( "XML", strExt.c_str() ) == 0 ){
					// 一致
				}else{
					continue;	// 不一致
				}
			}

			pathArr.push_back( strFileName );
		}
	}
	else {
		pathArr.push_back( strBatchJobDir );

	}

	return pathArr;
}


// get setting parameter values
kome::objects::SettingParameterValues* BatchServicePluginManager::getSettingParameterValues( int nIndexOfBatch ){
	return m_batchinfo.getBatchParameter( nIndexOfBatch );
}


kome::plugin::BatchInfo& BatchServicePluginManager::getBatchiInfo( void ){
	return m_batchinfo;
}


// XMLファイルへ出力
int BatchServicePluginManager::exportBatchXML( const char* szMethodName,  std::vector<Action> vectActions )
{
	


	m_batchinfo.clearFiles();   // <Add> 20140717 fujita　バッチ名、コメント以外のバッチの情報を初期化する。
	// add batch info
	for( unsigned int i = 0; i < vectActions.size(); i++ ) {
		std::string strSettings = trimstring( vectActions[ i ].strSettings.c_str() );
		if( strSettings.size() > 0 ) {
			if( strSettings[ 0 ] == '[' && strSettings[ strSettings.size() - 1 ] == ']' ) {
				strSettings = strSettings.substr( 1, strSettings.size() - 2 );
			}
		}

		// @date 2013/04/02 <Add> OKADA ------->
		// addCallに渡す用の文字列作成
		std::string strSetting;
		strSetting = "[";

		std::string strTemp;
		for( unsigned int j=0; j<vectActions[ i ].vec_strSettings.size(); j++ ){
			if( 1 <= j ){
				strSetting += ",";
			}
			strTemp = vectActions[i].vec_strSettings[j];
			std::string::size_type index = strTemp.find( "[" );

			std::string strTrans;
			if( index == std::string::npos ){
				// [が無い(サブパラメータが無い)

				// @date 2013/04/03 <Add> OKADA ------->
				std::string str = trimstring( strTemp.c_str() );
				strTrans = replacestring( str.c_str(), ",", "\\," );
				// @date 2013/04/03 <Add> OKADA <-------
				strSetting += strTrans;
			}else{
				strSetting += strTemp;
			}
		}
		strSetting += "]";
		// addCall()内で setParameterString() を呼び出している。strSettingはsetParameterString
		// の第二引数に入る。setParameterString()関数では、複数のパラメータ文字列をカンマで区切る
		// 仕様になっている。
		// 例：Detect Spectrum Peaksで、Peak Detector がGION(Window Widthが5、Timesが10)、
		//   Charge DetectorがIntensity Rate Charge Detectの場合
		//   "[gion[5,10],intrate]" → 「-peaks "gion[5,10]" "intrate" 」
		// 
		// 一方、「Select Samples By Name」の場合、サンプル名をカンマで区切って指定する。
		// 例：-smplname "Test1,Test2,Test3"
		// このとき、パラメータは「"Test1,Test2,Test3"」で1つなので、Test1やTest2の後のカンマが
		// 区切り文字ではないことを示す為、「"Test1\,Test2\,Test3"」という形式に変換してから
		// setParameterString()関数に与える必要がある。上記ブロックの処理は、そのための処理である。
		// @date 2013/04/02 <Add> OKADA <-------

		m_batchinfo.addCall(
			vectActions[ i ].strNameShort.c_str(),
			strSetting.c_str()	// @date 2013/04/02 <Add> OKADA
		);
	}


	// XML file path
	std::string strXMLFileDir = getBatchJobPath();
	std::string strAbsPathWithoutExt = getabsolutepath( strXMLFileDir.c_str(), szMethodName );
	std::string strAbsPath = strAbsPathWithoutExt + ".xml";

	// open file
	FILE* fp = fileopen( strAbsPath.c_str(), "w" );
	if( fp == NULL ) {
		return -1;
	}

	// XMLヘッダ
	fprintf( fp, XML_HEADER	);
	
	// <batch>
	fprintf( fp, XML_BAT_START	);
	
	// <name>
	fprintf( fp, XML_NAME_START );
	fprintf( fp, "%s", szMethodName );
	fprintf( fp, XML_NAME_END );

	// <date>
	SYSTEMTIME st;
	GetLocalTime( &st );
	fprintf( fp, XML_DATE_START );
	fprintf( fp, "%04d-%02d-%02d+%02d:%02d", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute );
	fprintf( fp, XML_DATE_END );
	
	// <functions>
	fprintf( fp, XML_FUNCTIONS_START	);

	kome::objects::SettingParameterValues settings;
	for( unsigned int nIndex=0; nIndex<vectActions.size(); nIndex++ ){
		settings.clear();

		//　<batch-fun call="peaks"> 
		fprintf( fp, XML_BATCHFUN_CALL_START, vectActions[nIndex].strNameShort.c_str() );
	
		// <params>
		fprintf( fp, XML_PARAMS_START );

		if( vectActions[nIndex].flgSettings == 1 ){
			// @date 2012/03/26 <Add> OKADA ------->
			kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();
			kome::plugin::PluginFunctionItem* pItem = plgMgr.getFunctionItem( "batch", vectActions[nIndex].strNameShort.c_str() );
			kome::plugin::SettingsPage * spage = NULL;
			if( pItem != NULL ){
				spage = pItem->getSettingsPage();

				std::string strSettings = vectActions[nIndex].strSettings;
				if( spage != NULL ){
					spage->setParameterString( settings, strSettings.c_str() );
				}
			}
			// @date 2012/03/26 <Add> OKADA <-------
			
		}

		unsigned int uiNum = settings.getNumberOfParameters();

		for( unsigned int i=0; i<uiNum; i++ ){
			// <param>
			fprintf( fp, XML_PARAM_START );

			const char* n = settings.getParameterName( i );
			const char* v = settings.getParameterValue( i );

			// <param-name>*</param-name>
			fprintf( fp, XML_PARAM_NAME, n );

			bool bVal = settings.hasSubParameterValues( n, v );

			if( bVal == false ){
				// @date 2013/04/03 <Add> OKADA ------->
				std::string str = trimstring( v );
				std::string strV = replacestring( str.c_str(), ",", "\\," );
				// @date 2013/04/03 <Add> OKADA <-------

				fprintf( fp, XML_PARAM_VALUE, strV.c_str() );										// @date 2013/04/01 <Add> OKADA
			}else{

				// strSettings.c_str() は'[multi[local,centroid]]'形式
				std::string strTemp = vectActions[nIndex].strSettings.c_str();

				std::string strTemp2;
				int nLen = strTemp.length();
				std::string::size_type stStart = strTemp.find_first_of( '[' );
				std::string::size_type stEnd = strTemp.find_last_of( ']' );
				if( nLen <= 2 ){
					;	// 通常あり得ないエラー
				}else if(( stStart == std::string::npos )||( stEnd == std::string::npos )){
					;	// 通常あり得ないエラー
				}else if( stStart != 0 ){
					;	// 通常あり得ないエラー
				}else if( stEnd != (nLen-1) ){
					;	// 通常あり得ないエラー
				}else{
					strTemp2 = strTemp.substr( stStart+1, nLen-2 );	// 文字列から、先頭と末尾の[]を除去
				};
				std::string strParamValue = strTemp2;

				// @date 2013/03/28 <Add> OKADA ------->
				if( i < vectActions[nIndex].vec_strSettings.size()  ){
					strParamValue = vectActions[nIndex].vec_strSettings[i];
				}else{
					// 通常あり得ない
					strParamValue = "";
				}
				// @date 2013/03/28 <Add> OKADA <-------

				// <param-value>*</param-value>
				fprintf( fp, XML_PARAM_VALUE, strParamValue.c_str() );	

				int k = 0;
			}

			// </param>
			fprintf( fp, XML_PARAM_END );

		}

		// </params>
		fprintf( fp, XML_PARAMS_END );

		// </batch-fun>
		fprintf( fp, XML_BATCHFUN_CALL_END );
	}

	// </functions>
	fprintf( fp, XML_FUNCTIONS_END	);

	// </batch>
	fprintf( fp, XML_BAT_END	);

	if( fp != NULL ){
		fclose( fp );
	}

	return 0;
}


// save last record file
// 最後に選択したxmlファイルの名前をiniファイルに保存する
int BatchServicePluginManager::saveLastRecordXmlFilename( const char * szXMLFilename ){
	// szXMLFilename = Filename of XML without extension(拡張子を除いたファイル名)

	kome::core::MsppManager& msppMgr = kome::core::MsppManager::getInstance();
	kome::core::IniFile* ini = msppMgr.getParameters();

	ini->setString( SECTION_XML, DEFAULT_XML_KEY, szXMLFilename );

	return 0;
}

// get last record xml filename
const char * BatchServicePluginManager::getLastRecordXmlFilename( void )
{
	kome::core::MsppManager& msppMgr = kome::core::MsppManager::getInstance();
	kome::core::IniFile* ini = msppMgr.getParameters();

	return ini->getString( SECTION_XML, DEFAULT_XML_KEY, "" );
}


int BatchServicePluginManager::readXML( const char * szXmlFilePath ){
	BatchXmlHandler handler;

	handler.parse( szXmlFilePath );

	if( handler.isError() ) {
		// error / ファイルがない、等
		m_vectXmlActions.clear();
	}else{
		// no error
		m_vectXmlActions = handler.getVectActions();
	}

	return 0;
}


std::vector<Action> BatchServicePluginManager::getVectXmlActions( void )
{
	return m_vectXmlActions;
}


// get DB
kome::db::Database* BatchServicePluginManager::getDb() {
	// db
	kome::db::DbManager& dbMgr = kome::db::DbManager::getInstance();
	kome::db::Database* db = dbMgr.getDb();
	if( m_db == db ) {
		return db;
	}
	m_db = db;
	if( db == NULL ) {
		LOG_ERROR( FMT( "Failed to get the DB connection." ) );
		return db;
	}

	// tables
	std::vector< kome::db::Table > tables;
	tables.resize( 6 );

	// format table
	kome::db::Table* table = &( tables[ 0 ] );

	table->setName( TABLE_FORMAT );

	kome::db::TableColumn* col = table->addColumn( COL_ID_FORMAT );
	col->setDataType( kome::db::TableColumn::INTEGER );
	col->setNotNull( true );
	col->setAutoIncrement( true );
	kome::db::TableIndex* idx = table->addIndex( IDX_ID_FORMAT );
	idx->addColumn( col );

	col = table->addColumn( COL_NAME_FORMAT );
	col->setDataType( kome::db::TableColumn::VARCHAR );
	col->setNotNull( true );
	idx = table->addIndex( IDX_NAME_FORMAT );
	idx->addColumn( col );

	col = table->addColumn( COL_DESC_FORMAT );
	col->setDataType( kome::db::TableColumn::VARCHAR );

	col = table->addColumn( COL_EXT_FORMAT );
	col->setDataType( kome::db::TableColumn::VARCHAR );
	col->setNotNull( true );

	// IO table
	table = &( tables[ 1 ] );

	table->setName( TABLE_IO );

	col = table->addColumn( COL_ID_IO );
	col->setDataType( kome::db::TableColumn::INTEGER );
	col->setNotNull( true );
	col->setAutoIncrement( true );
	idx = table->addIndex( IDX_ID_IO );
	idx->addColumn( col );

	col = table->addColumn( COL_JOB_IO );
	col->setDataType( kome::db::TableColumn::INTEGER );
	col->setNotNull( true );
	idx = table->addIndex( IDX_JOB_IO );
	idx->addColumn( col );

	col = table->addColumn( COL_FMT_IO );
	col->setDataType( kome::db::TableColumn::INTEGER );
	col->setNotNull( true );
	idx = table->addIndex( IDX_INPUT_FMT_IO );
	idx->addColumn( col );

	col = table->addColumn( COL_INPUT_IO );
	col->setDataType( kome::db::TableColumn::INTEGER );
	idx->addColumn( col );
	idx = table->addIndex( IDX_INPUT_IO );
	idx->addColumn( col );

	col = table->addColumn( COL_PATH_IO );
	col->setDataType( kome::db::TableColumn::VARCHAR );
	col->setNotNull( true );

	// job table
	table = &( tables[ 2 ] );

	table->setName( TABLE_JOB );

	col = table->addColumn( COL_ID_JOB );
	col->setDataType( kome::db::TableColumn::INTEGER );
	col->setNotNull( true );
	col->setAutoIncrement( true );
	idx = table->addIndex( IDX_ID_JOB );
	idx->addColumn( col );

	col = table->addColumn( COL_TITLE_JOB );
	col->setDataType( kome::db::TableColumn::VARCHAR );
	col->setNotNull( true );

	col = table->addColumn( COL_COMMENT_JOB );
	col->setDataType( kome::db::TableColumn::VARCHAR );

	col = table->addColumn( COL_DATE_JOB );
	col->setDataType( kome::db::TableColumn::VARCHAR );
	col->setNotNull( true );

	col = table->addColumn( COL_SERVER_JOB );
	col->setDataType( kome::db::TableColumn::INTEGER );
	col->setNotNull( true );
	idx = table->addIndex( IDX_SERVER_JOB );
	idx->addColumn( col );

	col = table->addColumn( COL_STRING_JOB );
	col->setDataType( kome::db::TableColumn::VARCHAR );

	col = table->addColumn( COL_STATUS_JOB );
	col->setDataType( kome::db::TableColumn::INTEGER );

	// server table
	table = &( tables[ 3 ] );

	table->setName( TABLE_SERVER );

	col = table->addColumn( COL_ID_SERVER );
	col->setDataType( kome::db::TableColumn::INTEGER );
	col->setNotNull( true );
	col->setAutoIncrement( true );
	idx = table->addIndex( IDX_ID_SERVER );
	idx->addColumn( col );

	col = table->addColumn( COL_NAME_SERVER );
	col->setDataType( kome::db::TableColumn::VARCHAR );
	col->setNotNull( true );
	idx = table->addIndex( IDX_NAME_SERVER );
	idx->addColumn( col );

	col = table->addColumn( COL_DESC_SERVER );
	col->setDataType( kome::db::TableColumn::VARCHAR );

	// function table
	table = &( tables[ 4 ] );

	table->setName( TABLE_FUN );

	col = table->addColumn( COL_ID_FUN );
	col->setDataType( kome::db::TableColumn::INTEGER );
	col->setNotNull( true );
	col->setAutoIncrement( true );
	idx = table->addIndex( IDX_ID_FUN );
	idx->addColumn( col );

	col = table->addColumn( COL_NAME_FUN );
	col->setDataType( kome::db::TableColumn::VARCHAR );
	col->setNotNull( true );
	idx = table->addIndex( IDX_NAME_FUN );
	idx->addColumn( col );

	col = table->addColumn( COL_DESC_FUN );
	col->setDataType( kome::db::TableColumn::VARCHAR );

	// call table
	table = &( tables[ 5 ] );

	table->setName( TABLE_CALL );

	col = table->addColumn( COL_ID_CALL );
	col->setDataType( kome::db::TableColumn::INTEGER );
	col->setNotNull( true );
	col->setAutoIncrement( true );
	idx = table->addIndex( IDX_ID_CALL );
	idx->addColumn( col );

	col = table->addColumn( COL_JOB_CALL );
	col->setDataType( kome::db::TableColumn::INTEGER );
	col->setNotNull( true );
	idx = table->addIndex( IDX_JOB_CALL );
	idx->addColumn( col );

	col = table->addColumn( COL_FUN_CALL );
	col->setDataType( kome::db::TableColumn::VARCHAR );
	col->setNotNull( true );

	col = table->addColumn( COL_PARAM_CALL );
	col->setDataType( kome::db::TableColumn::VARCHAR );

	// create table
	for( unsigned int i = 0; i < tables.size(); i++ ) {
		db->createTable( tables[ i ]  );
	}

	return db;
}


// register batch information
void BatchServicePluginManager::registerBatchInfo(
		kome::plugin::BatchInfo& batchInfo,
		kome::plugin::PluginFunctionItem* server,
		const char* id
) {
	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return;
	}

	// server
	int serverId = registerServer( server );
	if( serverId < 0 ) {
		return;
	}

	// server table
	int jobId = registerJob(
		batchInfo.getName().c_str(),
		batchInfo.getComment().c_str(),
		serverId,
		id
	);
	if( jobId < 0 ) {
		return;
	}

	// output formats
	std::vector< int > formats;
	for( int i = 0; i < batchInfo.getNumberOfSaveFunctions(); i++ ) {
		kome::plugin::PluginFunctionItem* outFun = batchInfo.getSaveFunction( i );
		formats.push_back( registerFormat( outFun ) );
	}

	for( int i = 0; i < batchInfo.getNumberOfInputFiles(); i++ ) {
		int inputId = registerFilePath(
			batchInfo.getInputFile( i ).c_str(),
			-1,
			jobId,
			-1
		);

		for( unsigned int j = 0; j < formats.size(); j++ ) {
			registerFilePath(
				batchInfo.getOutputFile( i, j ).c_str(),
				formats[ j ],
				jobId,
				inputId
			);
		}
	}

	// register calls
	for( int i = 0; i < batchInfo.getNumberOfBatchFunctions(); i++ ) {
		// function
		kome::plugin::PluginFunctionItem* fun = batchInfo.getBatchFunction( i );
		kome::plugin::SettingsPage* page = fun->getSettingsPage();
		kome::objects::SettingParameterValues* settings = batchInfo.getBatchParameter( i );

		std::string param;
		if( page != NULL && settings != NULL ) {
			param = page->getParameterString( *settings );
			if( param.length() > 1 ) {
				if( param.front() == '[' && param.back() == ']' ) {
					param = param.substr( 1, param.length() - 2 );
				}
			}
		}

		int funId = registerFunction( fun );
		registerCall( jobId, funId, param.c_str() );
	}

	db->commit();
}


// get jobs
void BatchServicePluginManager::getJobs( std::vector< int >& jobs ) {
	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return;
	}

	// job IDs
	std::string sql = FMT( "select %s from %s;", COL_ID_JOB, TABLE_JOB );
	kome::db::ResultSet* rs = db->executeQuery( sql.c_str() );
	if( rs != NULL ) {
		while( rs->next() ) {
			const int id = rs->getInt( 0, -1 );
			if( id >= 0 ) {
				jobs.push_back( id );
			}
		}
		rs->close();
	}
}


// get job title
std::string BatchServicePluginManager::getJobTitle( const int jobId ) {
	// title
	std::string title;

	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return title;
	}

	// title
	std::string sql = FMT(
		"select %s from %s where %s = ?;",
		COL_TITLE_JOB,
		TABLE_JOB,
		COL_ID_JOB
	);

	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindInteger( 1, jobId );
		kome::db::ResultSet* rs = stmt->executeQuery();
		if( rs != NULL ) {
			if( rs->next() ) {
				title = rs->getString( 0, "" );
			}
			rs->close();
		}
		stmt->close();
	}

	return title;
}


// get job comment
std::string BatchServicePluginManager::getJobComment( const int jobId ) {
	// comment
	std::string comment;

	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return comment;
	}

	// comment
	std::string sql = FMT(
		"select %s from %s where %s = ?;",
		COL_COMMENT_JOB,
		TABLE_JOB,
		COL_ID_JOB
	);

	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindInteger( 1, jobId );
		kome::db::ResultSet* rs = stmt->executeQuery();
		if( rs != NULL ) {
			if( rs->next() ) {
				comment = rs->getString( 0, "" );
			}
			rs->close();
		}
		stmt->close();
	}

	return comment;
}


// get job date
std::string BatchServicePluginManager::getJobDate( const int jobId ) {
	// date
	std::string date;

	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return date;
	}

	// title
	std::string sql = FMT(
		"select %s from %s where %s = ?;",
		COL_DATE_JOB,
		TABLE_JOB,
		COL_ID_JOB
	);

	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindInteger( 1, jobId );
		kome::db::ResultSet* rs = stmt->executeQuery();
		if( rs != NULL ) {
			if( rs->next() ) {
				date = rs->getString( 0, "" );
			}
			rs->close();
		}
		stmt->close();
	}

	return date;
}


// get job string
std::string BatchServicePluginManager::getJobString( const int jobId ) {
	// string
	std::string str;

	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return str;
	}

	// string
	std::string sql = FMT(
		"select %s from %s where %s = ?;",
		COL_STRING_JOB,
		TABLE_JOB,
		COL_ID_JOB
	);

	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindInteger( 1, jobId );
		kome::db::ResultSet* rs = stmt->executeQuery();
		if( rs != NULL ) {
			if( rs->next() ) {
				str = rs->getString( 0, "" );
			}
			rs->close();
		}
		stmt->close();
	}

	return str;
}


// get job status
kome::plugin::BatchService::BatchStatus BatchServicePluginManager::getJobStatus( const int jobId ) {
	// status;
	kome::plugin::BatchService::BatchStatus status = kome::plugin::BatchService::STATUS_WAITING;

	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return status;
	}

	// title
	std::string sql = FMT(
		"select %s from %s where %s = ?;",
		COL_STATUS_JOB,
		TABLE_JOB,
		COL_ID_JOB
	);

	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindInteger( 1, jobId );
		kome::db::ResultSet* rs = stmt->executeQuery();
		if( rs != NULL ) {
			if( rs->next() ) {
				status = (kome::plugin::BatchService::BatchStatus)rs->getInt( 0, (int)kome::plugin::BatchService::STATUS_WAITING );
			}
			rs->close();
		}
		stmt->close();
	}

	return status;
}


// get job server
int BatchServicePluginManager::getJobServer( const int jobId ) {
	// server ID
	int serverId = -1;

	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return serverId;
	}

	// server ID
	std::string sql = FMT(
		"select %s from %s where %s = ?;",
		COL_SERVER_JOB,
		TABLE_JOB,
		COL_ID_JOB
	);

	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindInteger( 1, jobId );
		kome::db::ResultSet* rs = stmt->executeQuery();
		if( rs != NULL ) {
			if( rs->next() ) {
				serverId = rs->getInt( 0, -1 );
			}
			rs->close();
		}
		stmt->close();
	}

	return serverId;
}


//  get server name
std::string BatchServicePluginManager::getServerName( const int serverId ) {
	// name
	std::string name;

	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return name;
	}

	// title
	std::string sql = FMT(
		"select %s from %s where %s = ?;",
		COL_NAME_SERVER,
		TABLE_SERVER,
		COL_ID_SERVER
	);

	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindInteger( 1, serverId );
		kome::db::ResultSet* rs = stmt->executeQuery();
		if( rs != NULL ) {
			if( rs->next() ) {
				name = rs->getString( 0, "" );
			}
			rs->close();
		}
		stmt->close();
	}

	return name;
}


// get server description
std::string BatchServicePluginManager::getServerDescription( const int serverId ) {
	// desc
	std::string desc;

	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return desc;
	}

	// desc
	std::string sql = FMT(
		"select %s from %s where %s = ?;",
		COL_DESC_SERVER,
		TABLE_SERVER,
		COL_ID_SERVER
	);

	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindInteger( 1, serverId );
		kome::db::ResultSet* rs = stmt->executeQuery();
		if( rs != NULL ) {
			if( rs->next() ) {
				desc = rs->getString( 0, "" );
			}
			rs->close();
		}
		stmt->close();
	}

	return desc;
}


// update job status
void BatchServicePluginManager::updateJobStatus( const int jobId, kome::plugin::BatchService::BatchStatus status ) {
	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return;
	}

	// update status
	std::string sql = FMT(
		"update %s set %s = ? where %s = ?;",
		TABLE_JOB,
		COL_STATUS_JOB,
		COL_ID_JOB
	);

	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindInteger( 1, (int)status );
		stmt->bindInteger( 2, jobId );

		if( stmt->execute() ) {
			db->commit();
		}
		else {
			db->rollback();
		}
		stmt->close();
	}

	db->commit();
}


// delete job
void BatchServicePluginManager::deleteJob( const int jobId ) {
	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return;
	}

	// delete file paths
	std::string sql = FMT( "delete from %s where %s = ?;", TABLE_IO, COL_JOB_IO );
	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindInteger( 1, jobId );
		if( stmt->execute() ) {
			db->commit();
		}
		else {
			db->rollback();
		}
		stmt->close();
	}

	// delete calls
	sql = FMT( "delete from %s where %s = ?;", TABLE_CALL, COL_JOB_CALL );
	stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindInteger( 1, jobId );
		if( stmt->execute() ) {
			db->commit();
		}
		else {
			db->rollback();
		}
		stmt->close();
	}

	// delete job
	sql = FMT( "delete from %s where %s = ?;", TABLE_JOB, COL_ID_JOB );
	stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindInteger( 1, jobId );
		if( stmt->execute() ) {
			db->commit();
		}
		else {
			db->rollback();
		}
		stmt->close();
	}

	db->commit();
}


// update statuses
void BatchServicePluginManager::updateStatuses() {
	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return;
	}

	// manager
	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();

	// jobs
	std::vector< int > jobs;
	getJobs( jobs );

	for( unsigned int i = 0; i < jobs.size(); i++ ) {
		const int jobId = jobs[ i ];
		kome::plugin::BatchService::BatchStatus status = getJobStatus( jobId );

		if( status == kome::plugin::BatchService::STATUS_RUNNING
				|| status == kome::plugin::BatchService::STATUS_WAITING ) {
			int server = getJobServer( jobId );
			if( server >= 0 ) {
				std::string name = getServerName( server );
				kome::plugin::PluginFunctionItem* item = plgMgr.getFunctionItem( "get_batch_service", name.c_str() );
				if( item != NULL ) {
					std::string str = getJobString( jobId );
					kome::plugin::BatchService* service = (kome::plugin::BatchService*)item->getCall()->invoke( NULL ).prim.pt;
					if( service != NULL ) {
						kome::plugin::BatchService::BatchStatus newStatus = service->getStatus( str.c_str() );
						if( newStatus != status ) {
						    updateJobStatus( jobId, newStatus );
						}
					}
				}
			}
		}
	}

	db->commit();
}


// register server
int BatchServicePluginManager::registerServer( kome::plugin::PluginFunctionItem* server ) {
	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return -1;
	}

	// search
	int id = -1;
	std::string sql = FMT(
		"select %s from %s where %s = ?;",
		COL_ID_SERVER,
		TABLE_SERVER,
		COL_NAME_SERVER
	);

	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindString( 1, server->getShortName() );
		kome::db::ResultSet* rs = stmt->executeQuery();
		if( rs != NULL ) {
			if( rs->next() ) {
				id = rs->getInt( 0, -1 );
			}
			rs->close();
		}
		stmt->close();
	}

	if( id >= 0 ) {
		return id;
	}

	// register
	sql = FMT(
		"insert into %s (%s, %s) values(?, ?);",
		TABLE_SERVER,
		COL_NAME_SERVER,
		COL_DESC_SERVER
	);

	bool flg = false;
	stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindString( 1, server->getShortName() );
		stmt->bindString( 2, server->getLongName() );

		flg = stmt->execute();
		if( flg ) {
			db->commit();
		}
		else {
			db->rollback();
		}
		stmt->close();
	}

	// ID
	if( flg ) {
		id = std::max( -1, db->getMaxValue( TABLE_SERVER, COL_ID_SERVER ) );
	}

	db->commit();

	return id;
}


// register job
int BatchServicePluginManager::registerJob(
		const char* title,
		const char* comment,
		const int serverId,
		const char* jobId
) {
	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return -1;
	}

	// date
	time_t t = time( NULL );
	struct tm* date = localtime( &t );
	std::string dateStr = datetostring( date );

	// add
	std::string sql = FMT(
		"insert into %s (%s, %s, %s, %s, %s, %s) values(?, ?, ?, ?, ?, ?);",
		TABLE_JOB,
		COL_TITLE_JOB,
		COL_COMMENT_JOB,
		COL_DATE_JOB,
		COL_SERVER_JOB,
		COL_STRING_JOB,
		COL_STATUS_JOB
	);
	
	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	bool flg = false;
	if( stmt != NULL ) {
		stmt->bindString( 1, NVL( title, "" ) );
		stmt->bindString( 2, NVL( comment, "" ) );
		stmt->bindString( 3, dateStr.c_str() );
		stmt->bindInteger( 4, serverId );
		stmt->bindString( 5, NVL( jobId, "" ) );
		stmt->bindInteger( 6, (int)kome::plugin::BatchService::STATUS_WAITING );

		flg = stmt->execute();
		if( flg ) {
			db->commit();
		}
		else {
			db->rollback();
		}
		stmt->close();
	}

	// ID
	int id = -1;
	if( flg ) {
		id = std::max( -1, db->getMaxValue( TABLE_JOB, COL_ID_JOB ) );
	}

	db->commit();

	return id;
}


// register output format
int BatchServicePluginManager::registerFormat( kome::plugin::PluginFunctionItem* outFun ) {
	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return -1;
	}

	// search
	int id = -1;
	std::string sql = FMT(
		"select %s from %s where %s = ?;",
		COL_ID_FORMAT,
		TABLE_FORMAT,
		COL_NAME_FORMAT
	);

	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindString( 1, outFun->getShortName() );
		kome::db::ResultSet* rs = stmt->executeQuery();
		if( rs != NULL ) {
			if( rs->next() ) {
				id = rs->getInt( 0, -1 );
			}
			rs->close();
		}
		stmt->close();
	}

	if( id >= 0 ) {
		return id;
	}

	// register
	sql = FMT(
		"insert into %s (%s, %s, %s) values(?, ?, ?);",
		TABLE_FORMAT,
		COL_NAME_FORMAT,
		COL_DESC_FORMAT,
		COL_EXT_FORMAT
	);

	std::string ext = outFun->getCall()->getProperties().getStringValue( "ext", "" );

	bool flg = false;
	stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindString( 1, outFun->getShortName() );
		stmt->bindString( 2, outFun->getLongName() );
		stmt->bindString( 3, ext.c_str() );

		flg = stmt->execute();
		if( flg ) {
			db->commit();
		}
		else {
			db->rollback();
		}
		stmt->close();
	}

	// ID
	if( flg ) {
		id = std::max( -1, db->getMaxValue( TABLE_FORMAT, COL_ID_FORMAT ) );
	}

	db->commit();

	return id;
}


// register file path
int BatchServicePluginManager::registerFilePath( const char* path, const int fmtId, const int jobId, const int inputId ) {
	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return -1;
	}

	// search
	int id = -1;
	std::string sql = FMT( 
		"select %s from %s where %s = ? and %s = ? and %s = ? and %s = ?;",
		COL_ID_IO,
		TABLE_IO,
		COL_JOB_IO,
		COL_FMT_IO,
		COL_INPUT_IO,
		COL_PATH_IO
	);

	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindInteger( 1, jobId );
		stmt->bindInteger( 2, fmtId );
		stmt->bindInteger( 3, inputId );
		stmt->bindString( 4, NVL( path, "" ) );

		kome::db::ResultSet* rs = stmt->executeQuery();
		if( rs != NULL ) {
			if( rs->next() ) {
				id = rs->getInt( 0, -1 );
			}
			rs->close();
		}
		stmt->close();
	}

	if( id >= 0 ) {
		return id;
	}

	// insert
	sql = FMT(
		"insert into %s (%s, %s, %s, %s) values (?, ?, ?, ?);",
		TABLE_IO,
		COL_JOB_IO,
		COL_FMT_IO,
		COL_INPUT_IO,
		COL_PATH_IO
	);

	bool flg = false;
	stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindInteger( 1, jobId );
		stmt->bindInteger( 2, fmtId );
		stmt->bindInteger( 3, inputId );
		stmt->bindString( 4, NVL( path, "" ) );

		flg = stmt->execute();
		if( flg ) {
			db->commit();
		}
		else {
			db->rollback();
		}
		stmt->close();
	}

	if( flg ) {
		id = std::max( -1, db->getMaxValue( TABLE_IO, COL_ID_IO ) );
	}

	db->commit();

	return id;
}


// register function
int BatchServicePluginManager::registerFunction( kome::plugin::PluginFunctionItem* fun ) {
	// db
	kome::db::Database* db = getDb();
	if( db == NULL ) {
		return -1;
	}

	// search
	int id = -1;
	std::string sql = FMT(
		"select %s from %s where %s = ?;",
		COL_ID_FUN,
		TABLE_FUN,
		COL_NAME_FUN
	);

	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindString( 1, fun->getShortName() );
		kome::db::ResultSet* rs = stmt->executeQuery();
		if( rs != NULL ) {
			if( rs->next() ) {
				id = rs->getInt( 0, -1 );
			}
			rs->close();
		}
		stmt->close();
	}

	if( id >= 0 ) {
		return id;
	}

	// register
	sql = FMT(
		"insert into %s (%s, %s) values(?, ?);",
		TABLE_FUN,
		COL_NAME_FUN,
		COL_DESC_FUN
	);

	bool flg = false;
	stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindString( 1, fun->getShortName() );
		stmt->bindString( 2, fun->getLongName() );

		flg = stmt->execute();
		if( flg ) {
			db->commit();
		}
		else {
			db->rollback();
		}
		stmt->close();
	}

	// ID
	if( flg ) {
		id = std::max( -1, db->getMaxValue( TABLE_FUN, COL_ID_FUN ) );
	}
	db->commit();

	return id;
}


// register call
int BatchServicePluginManager::registerCall( int jobId, int funId, const char* param ) {
	// db
	kome::db::Database* db = getDb();

	// add
	std::string sql = FMT(
		"insert into %s (%s, %s, %s) values(?, ?, ?);",
		TABLE_CALL,
		COL_JOB_CALL,
		COL_FUN_CALL,
		COL_PARAM_CALL
	);

	bool flg = false;
	kome::db::Statement* stmt = db->prepare( sql.c_str() );
	if( stmt != NULL ) {
		stmt->bindInteger( 1, jobId );
		stmt->bindInteger( 2, funId );
		stmt->bindString( 3, NVL( param, "" ) );

		flg = stmt->execute();
		if( flg ) {
			db->commit();
		}
		else {
			db->rollback();
		}
	}

	int id = -1;
	if( flg ) {
		id = std::max( -1, db->getMaxValue( TABLE_CALL, COL_ID_CALL ) );
	}

	db->commit();

	return id;
}

//起動時にINIファイルから読み込み。
//	OutputFolder


void BatchServicePluginManager::setDirBatchProcessingSubmitJob( std::string strDir )
{
	// メンバ変数に格納 //////////
	m_strOutputDirBatchProcessingSubmitJob = strDir;

	// INIファイルへ保存 //////////

	// managers
	kome::core::MsppManager& msppMgr = kome::core::MsppManager::getInstance();

	// ini file
	kome::core::IniFile* ini = msppMgr.getIniFile();
	if( ini != NULL ) {
		ini->setString( INI_OUTPUT_FOLDER_SECTION, INI_OUTPUT_FOLDER_KEY, strDir.c_str() );	// @date 2012/08/28 <Add> OKADA
	}

};


std::string BatchServicePluginManager::getDirBatchProcessingSubmitJob( void )
{
	// INIファイルから読み込み //////////

	// managers
	kome::core::MsppManager& msppMgr = kome::core::MsppManager::getInstance();

	// ini file
	kome::core::IniFile* ini = msppMgr.getIniFile();

	// default directory
	std::string path;
	if( ini != NULL ) {
		const char* p = ini->getString( INI_OUTPUT_FOLDER_SECTION, INI_OUTPUT_FOLDER_KEY, "" );
		path = NVL( p, "" );
	}

	m_strOutputDirBatchProcessingSubmitJob = path;

	return m_strOutputDirBatchProcessingSubmitJob;
};


// inputファイルのフルパスと拡張子とindexから、outputファイル名を取得
std::string BatchServicePluginManager::getOutputFineName( std::string strInputFileName, std::string strTime, std::string strOutExt, int nIndexOfInputFile, int nIndexOfFormat ){
	std::string strOutputFormat;
	std::string strOutputFormatDir;

	std::string strFilename = getfilename( strInputFileName.c_str() );
	std::string strExt      = getext( strFilename.c_str() );

	std::string strFilenameWithoutExt;
	if( strExt.length() <= 0 ){
		strFilenameWithoutExt = strFilename;
	}else{
		size_t pos = strFilename.find_last_of( "." );
		strFilenameWithoutExt = strFilename.substr( 0, pos );
	}
	
	strOutputFormat = strfmt( "%s_%s_%d_%d.%s", strFilenameWithoutExt.c_str(), strTime.c_str(), nIndexOfInputFile, nIndexOfFormat, strOutExt.c_str() );   // 出力ファイル名を作成
	// INIファイルからDefaultのディレクトリを読み込み
	strOutputFormatDir = getDirBatchProcessingSubmitJob();	// 出力先ディレクトリを取得

	std::string strFullPath = getpath( strOutputFormatDir.c_str(), strOutputFormat.c_str() );
	return strFullPath;
}


void BatchServicePluginManager::dispBatchJobWindow( bool flgDisp ){

	// toggle the visibility
	bool visible = ( JobListCtrl::m_dlg != NULL && JobListCtrl::m_dlg->IsShown() );

	// delete
	if( JobListCtrl::m_dlg != NULL ) {
		delete JobListCtrl::m_dlg;
		JobListCtrl::m_dlg = NULL;
	}

	// create
	if( !visible || flgDisp ) {
		kome::window::GridListDialog* dlg = new kome::window::GridListDialog(
			kome::window::WindowTool::getMainWindow(),
			"Batch Job List",
			true
		);
		
		JobListCtrl* listCtrl = new JobListCtrl( dlg );
		dlg->Show();
		JobListCtrl::m_dlg = dlg;
	}
}

// get instance
BatchServicePluginManager& BatchServicePluginManager::getInstance() {
	// create object
	static BatchServicePluginManager mgr;

	return mgr;
}



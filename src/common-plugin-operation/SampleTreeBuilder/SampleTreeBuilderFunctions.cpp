/**
 * @file SampleTreeBuilderFunctions.cpp
 * @brief implementation of plug-in functions
 *
 * @author okada_h
 * @date 2012.09.28
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation All rights reserved.
 */


#include "stdafx.h"
#include "SampleTreeBuilderFunctions.h"
#include "SampleTreeBuilderManager.h"

#include <memory>	// for auto_ptr

using kome::plugin::FileFactory;	// for FileFactory
using kome::objects::SampleSet;		// for SampleSet
using kome::objects::Sample;		// for Sample
using kome::objects::DataGroupNode;	// for DataGroupNode
using kome::objects::Spectrum;		// for Spectrum

using kome::SampleTreeBuilder::SampleTreeBuilderSampleSet;
using kome::SampleTreeBuilder::SampleTreeBuilderSample;

#define MEMORY_LEAKER			/*NO_ACTION*/
#define MEMORY_LEAKER_TXT(X)	/*NO_ACTION*/


SampleTreeBuilderSampleSet::SampleTreeBuilderSampleSet(){

	m_p_csv_parser = new CsvParser;

	m_ptr_sample_new = NULL;

	SampleTreeBuilderManager &mgr = SampleTreeBuilderManager::getInstance();
	mgr.registPtr( this );
};


SampleTreeBuilderSampleSet::~SampleTreeBuilderSampleSet()
{

	delete m_p_csv_parser;	// @date 2012/10/03 <Add> OKADA
	m_p_csv_parser=NULL;

	SampleTreeBuilderManager &mgr = SampleTreeBuilderManager::getInstance();
	mgr.unregistPtr( this );

};

// on open file
bool SampleTreeBuilderSampleSet::onOpenFile( const char* path, kome::core::Progress* progress )  // add param @date 2014.07.08 <Mod> M.Izumi
{
	path = NULL;	// @date 2013/06/10 <Add> OKADA	// warning C4100: 'path' : 引数は関数の本体部で 1 度も参照されません。
//	※ここでSampleの派生クラス、SampleTreeBuilderSample を作成
	SampleTreeBuilderSample  *ptr_sample_new = new SampleTreeBuilderSample( this );
	m_ptr_sample_new = ptr_sample_new;

	this->addSample( ptr_sample_new );	// ここでaddSampleする

	return ptr_sample_new->openSample();	// SampleTreeBuilderSampleのonOpenSample( rootGroup )が呼ばれる。rootGroupには初期化済みインスタンスが入る
}

// on close file
bool SampleTreeBuilderSampleSet::onCloseFile() 
{
	return true;
}

// on open sample
bool SampleTreeBuilderSampleSet::onOpenSample( kome::objects::Sample* sample, kome::core::Progress* progress )  // add param @date 2014.07.08 <Mod> M.Izumi
{
	sample = NULL;	// @date 2013/06/10 <Add> OKADA	// warning C4100: 'sample' : 引数は関数の本体部で 1 度も参照されません。
	return true;
}

// on close sample
bool SampleTreeBuilderSampleSet::onCloseSample( kome::objects::Sample* sample ) 
{
	sample = NULL;	// @date 2013/06/10 <Add> OKADA	// warning C4100: 'sample' : 引数は関数の本体部で 1 度も参照されません。
	return true;
}

/* 文字列を大文字に直して比較 */
int capitalized_strcmp(const char *s1, const char *s2)
{
    while (toupper(*s1) == toupper(*s2)) {  /* 文字が等しい間ループする */
        if (*s1 == '\0'){                   /* 末尾まで等しければ */
            return (0);                     /* ０を返す */
		}
        s1++;                               /* 文字列の途中なので */
        s2++;                               /* インクリメントする */
    }    /* 等しくないので文字の値の差を返す */
    return (toupper(*s1) - toupper(*s2));
};


bool SampleTreeBuilderSample::onOpenSample( DataGroupNode* rootGroup, kome::core::Progress* progress ) 
{
	bool bRet = false;

	if( rootGroup == NULL ){
		return false;	// エラー
	}
	
	CsvParser* ptrCsvParser;
	ptrCsvParser = m_sampleSet->getPtrCsvParser();
	size_t sizeOfCsvDatas = ptrCsvParser->getNumberOfCsvData();	// csvファイルの行数
	for( unsigned int i=0; i<sizeOfCsvDatas; i++ ){

		SampleSet* pSsSource = NULL;

		std::string strDataType = ptrCsvParser->getCsvData( i ).getDataType();
		if( capitalized_strcmp( strDataType.c_str(), "file" ) == 0 ){
			// file読み込み処理
			FileFactory& factory = FileFactory::getInstance();
			std::string strFilename = ptrCsvParser->getCsvData( i ).getKey();
			pSsSource = factory.openFile( strFilename.c_str(), NULL );	// add param
		}else{
			// DIT-FPからの読み込み処理
		}
		if (pSsSource == NULL) {
			std::string strFilename = ptrCsvParser->getCsvData( i ).getKey();
			printf( "Error: Can't open file. '%s'\n", strFilename.c_str() );
			return false;	// エラー
		}
		ptrCsvParser->getCsvData( i ).setSs( pSsSource );	// エ

		// Sample Setに含まれているサンプルの個数を得る
		Sample* sampleSource = NULL;

		unsigned int nSample = 0;	// 最初の１つのみ読み取る
		sampleSource = pSsSource->getSample(nSample);
		if( sampleSource == NULL ){ 
			continue;	// nSample番目のサンプルが取得できなかった
		}
		
		bool bOpenedSource = sampleSource->isOpened();
		if( bOpenedSource == false ){
			sampleSource->openSample();	// 読み出し元ファイルのサンプルがオープンされていなければオープンする
		}
		
		// 取得したいスペクトルの名前を得る（openFile()関数を呼ぶ前に設定した物）
		std::string strSpecName = ptrCsvParser->getCsvData( i ).getSpectrum();
		if( strSpecName == "" ){ 
			printf( "Error: Spectrum not found. \n" );
			continue; 
		}

		// 読み出し元ファイルのサンプルのroot nodeを得る
		DataGroupNode* nodeSource = sampleSource->getRootDataGroupNode();
		if (nodeSource == NULL){
			continue;	// 読み出し元ファイルのroot node（DataGroupNode）が取得できなかった
			// 通常あり得ない
		}

		// スペクトル名で検索してスペクトルを得る
		Spectrum* spec_Source = NULL;
		if( strSpecName != "*" ){
			spec_Source = getSpecByName( nodeSource, strSpecName.c_str() );
		}else{
			int n = 0;
			spec_Source =  nodeSource->getSpectrum(n);   // SpecName が * の場合には、先頭のスペクトルを処理対象とする。
		}
		if( spec_Source == NULL ){ 
			// その名前のスペクトルは、読み込み元ファイルにありませんでした
			printf( "Error: Spectrum not found. '%s'\n",  strSpecName.c_str() );
			return false;	// エラー
		}

		ptrCsvParser->getCsvData( i ).setSpec( spec_Source );	// カ

		// キ
		// 親が指定されているかチェック
		std::string strParent = ptrCsvParser->getCsvData( i ).getParent();

		if( strParent == "*" ){
			// 親は指定されていない
			ptrCsvParser->getCsvData( i ).setPspec( NULL );	// ク

			int nStageNum = atoi( ptrCsvParser->getCsvData( i ).getStageStr().c_str() );
			ptrCsvParser->getCsvData( i ).setStage( nStageNum );

		}else{
			// 親は指定されている
			int nParent = atoi( strParent.c_str() );

			if( (int)sizeOfCsvDatas < nParent ){
				// 全体の行数よりも大きい値が指定されているのでNG
				printf( "Error: Parent spectrum is not found. row no=%d\n", nParent );
				return false;	// エラー
			}else{
				;
			}
			if( nParent <= 0 ){
				// 親番号は、1～
				return false;	// エラー
			}
			// 親の指定は、現在の行番号より若くなければならない
			if( nParent < ptrCsvParser->getCsvData( i ).getRow() ){
				// 正常
			}else{
				// 親番号がおかしい
				printf( "Error: Parent spectrum is not right. Number=%d\n", nParent );
				return false;	// エラー
			}
			// 親は指定されている
			bool flgIcchi = false;
			int nParentIndex = -1;
			for( unsigned int j=0; j<i; j++ ){
				if( nParent == ptrCsvParser->getCsvData( j ).getRow() ){
					// 一致した
					Spectrum* pSpec = ptrCsvParser->getCsvData( j ).getSpec();
					ptrCsvParser->getCsvData( i ).setPspec( pSpec );	// ク
					flgIcchi = true;
					nParentIndex = j;
				}
			}
			if( flgIcchi == false ){
				// 指定された親の行番号が見つからない
				return false;	// エラー	
			}

			int nStageNum = atoi( ptrCsvParser->getCsvData( i ).getStageStr().c_str() );
			ptrCsvParser->getCsvData( i ).setStage( nStageNum );
		}

		// スペクトルをラッパークラスに変換
		kome::operation::TransformedSpectrum* trans_spec = new kome::operation::TransformedSpectrum( *spec_Source );
		if( trans_spec == NULL ){ continue;	}

		// ここで trans_spec の内容を加工する 
		// 親スペクトルを設定
		{
			int nParent = atoi( strParent.c_str() );	// strParentが「*」の場合、nParentは0になるから、どれとも一致しない
			bool flgIcchi = false;
			for( unsigned int j=0; j<i; j++ ){
				if( nParent == ptrCsvParser->getCsvData( j ).getRow() ){
					// 一致した
					flgIcchi = true;
					trans_spec->setParentSpectrum( ptrCsvParser->getCsvData( j ).getTransSpec() );
				}
			}
			if( flgIcchi == false ){	// 親が無ければ、念のためNULLを入れる
				trans_spec->setParentSpectrum( NULL );
			}
		}

		// MS Stage番号を設定

		// 親スペクトルが指定されており、（条件1）
		// かつ、親スペクトルのMS Stage番号がある場合（条件2）
		bool flgIn = false;
		if( std::string::npos == strParent.find( "*", 0 ) )			// 条件1
		{	// 条件1
			int nParentMSStageNo = -1;

			// 親のMS Stage番号を取得
			int nParent = atoi( strParent.c_str() );
			for( unsigned int j=0; j<i; j++ ){
				if( nParent == ptrCsvParser->getCsvData( j ).getRow() ){
					// 一致した
					nParentMSStageNo = ptrCsvParser->getCsvData( j ).getStage();
				}
			}

			if( 0 < nParentMSStageNo ){	// 条件2
				flgIn = true;
				std::string strMSStageNum = ptrCsvParser->getCsvData( i ).getStageStr();
				if( strMSStageNum != "*" ){
					// 親+1が指定されていればOK
					if(ptrCsvParser->getCsvData( i ).getStage() == nParentMSStageNo+1){
						// 親+1が指定されていたのでOK
					}else{
						// 警告メッセージを表示
						printf( "Warning: MS Stage is %d in csv file. Right value is %d. \n", ptrCsvParser->getCsvData( i ).getStage(), nParentMSStageNo+1 );
					}
					ptrCsvParser->getCsvData( i ).setStage( nParentMSStageNo+1 );	// 親+1の値になる
				}else{
					ptrCsvParser->getCsvData( i ).setStage( nParentMSStageNo+1 );	// 親+1の値になる
				}
				
			}
		}
		if( flgIn == false ){
			// 上記、条件1/2のどちらにも当てはまらない場合
			std::string strMSStageNum = ptrCsvParser->getCsvData( i ).getStageStr();
			if( std::string::npos == strMSStageNum.find( "*", 0 ) ){
				// MSStage 値チェック
				int nMSStage = atoi(strMSStageNum.c_str());
				if(( nMSStage < 1 )||( 10 < nMSStage )){
					printf( "Error: MS Stage is %d\n", nMSStage );
					return false;	// エラー
				}

				// そのまま
				ptrCsvParser->getCsvData( i ).setStage( atoi(strMSStageNum.c_str()) );
			}else{
				// MS Stageは"*" が設定されている

				// 元のMSStageを取得
				int nOriginalMSStageNo = ptrCsvParser->getCsvData( i ).getSpec()->getMsStage();
				ptrCsvParser->getCsvData( i ).setStage( nOriginalMSStageNo );
				if( 0 < nOriginalMSStageNo ){	// MSStageが設定されている
					ptrCsvParser->getCsvData( i ).setStage( nOriginalMSStageNo );
				}else{
					ptrCsvParser->getCsvData( i ).setStage( nOriginalMSStageNo );
					// 設定されていない。警告。
					printf( "Warning: Original MS Stage is not seted.\n" );
				}
			}
		}
		// アイコンはMS1, MS2, MS3, MS4, MS5, MS6, MS7, MSnがあります。8以上は MSn のアイコンにして下さい。
		int nStage = ptrCsvParser->getCsvData( i ).getStage();
		trans_spec->setMsStage( nStage );
		if(( 1 <= nStage )&&( nStage <= 7 )){	// MS○アイコンを指定
			char szIcon[16];
			sprintf( szIcon, "MS%d", nStage );
			trans_spec->setIcon( szIcon );
		}else if( 8 <= nStage ){
			trans_spec->setIcon( "MSn" );
		};

		// プリカーサイオン質量の設定 //////////

		// 数で指定。変更しない場合は「*」。MS Stage番号が1の場合は「無効」（負の数）とする
		std::string strPcMass = ptrCsvParser->getCsvData( i ).getPcMassStr();
		if( std::string::npos == strPcMass.find( "*", 0 ) ){
			// "*"が無い
			// MS Stage番号が1の場合は「無効」
			if( ptrCsvParser->getCsvData( i ).getStage() == 1 ){
				trans_spec->setPrecursor( -1.0 );
				ptrCsvParser->getCsvData( i ).setPcMassNum( -1.0 );
			}else{
				double precursor = atof( strPcMass.c_str() );
				trans_spec->setPrecursor( precursor );
				ptrCsvParser->getCsvData( i ).setPcMassNum( precursor );
			}
		}else{
			// 変更しない
			double precursor = trans_spec->getPrecursor( ptrCsvParser->getCsvData( i ).getStage() );
			ptrCsvParser->getCsvData( i ).setPcMassNum( precursor );
		}

		// リテンションタイムの設定 //////////

		// 数で指定。変更しない場合は「*」。「*」指定時に元データにリテンションタイムが設定されていない場合は、警告
		std::string strRT =  ptrCsvParser->getCsvData( i ).getRetentionTimeStr();

		if( std::string::npos == strRT.find( "*", 0 ) ){
			// "*"が無い
			double dRT = atof( strRT.c_str() );
			ptrCsvParser->getCsvData( i ).setRetentionTime( dRT );
			trans_spec->setRt( dRT );
			trans_spec->setHasChromatogram( true );	// @date 2013/03/27 <Add> OKADA
		}else{
			// "*"がある
			// 変更しない
			// 「*」指定時に元データにリテンションタイムが設定されていない場合は、警告
			double dRT = trans_spec->getRt();
			if( dRT < 0 ){
				// リテンションタイムが設定されていない
				// 警告
				printf( "Warning: Retention time is nothing." );
			}
			ptrCsvParser->getCsvData( i ).setRetentionTime( dRT );
		}

		// SPEC 88275
		// scan numberを、テーブル（csv)の行番号-1に設定する
		trans_spec->setScanNumber( i );	// loop counter（変数 i）は0から始まるので、iをセットする // @date 2012/10/22 <Add> OKADA

		ptrCsvParser->getCsvData( i ).setTransSpec( trans_spec );	// trans_specを保存

		// スペクトルを追加
		rootGroup->addSpectrum( trans_spec );	// root node の下に追加（階層は設けない）
		bRet = true;

	}

	return bRet;
}


bool SampleTreeBuilderSample::onCloseSample() {
	return true;
};

// DataGroupNode内から名前でスペクトルを検索する
Spectrum* getSpecByName( DataGroupNode* node, const char *szRequestedName ){
	const unsigned int numSpec = node->getNumberOfSpectra();
	for (unsigned int n = 0; n < numSpec; n++){
		Spectrum* spec = node->getSpectrum(n);
		const char *szName = spec->getName();


		// 大文字・小文字等を区別しない比較（"Scan 0"と"scan 0"を同じように扱う）
		size_t sizeName          = strlen(szName);
		size_t sizeRequestedName = strlen(szRequestedName);

		if( sizeName != sizeRequestedName ){
			continue;	// 文字列長が異なるので不一致。一致しなかったので次の文字列を探す
		}

		bool flgIcchi = true;
		for( size_t sizeCount=0; sizeCount<sizeName; sizeCount++ ){
			if( toupper( szName[sizeCount] ) == toupper( szRequestedName[sizeCount] ) ){
				// 一致
			}else{
				// 不一致
				flgIcchi = false;
				break;
			}
		}
		if( flgIcchi == false ){
			continue;	// 一致しなかったので次の文字列を探す
		}else{
			// 一致
			return spec;	
		}
	}

	const unsigned int numChild = node->getNumberOfChildren();
	for (unsigned int n = 0; n < numChild; n++){
		DataGroupNode* child = node->getChild(n);
		if (child != NULL){
			return getSpecByName(child, szRequestedName);
		}
		else{
			assert(false);
		}
	}

	return NULL;
}

// --------------------
// CsvParser
// --------------------
// constructor
CsvParser::CsvParser(){
};

// destructor
CsvParser::~CsvParser(){
};


// コマンドラインから実行したときのエントリ関数
kome::objects::Variant execSampleTreeBuilderCommandline( kome::objects::Parameters* params) 
{
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = false;

	ret = execSampleTreeBuilderBatch( params );

	return ret;
}

// メニューから実行したときのエントリ関数
kome::objects::Variant execSampleTreeBuilderBatch( kome::objects::Parameters* params ) 
{
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::BOOL;
	ret.prim.boolVal = false;

	SampleTreeBuilderSampleSet *pSampleSetNew = new SampleTreeBuilderSampleSet;

	bool flgNonError = true;
	bool flgFileOpenOk = false;
	if( pSampleSetNew ){

		// ここでcsvファイルを読み込む

		// CSVファイル名を取得
		// get settings
		kome::objects::SettingParameterValues* settings
			= kome::plugin::PluginCallTool::getSettingValues( params );
		if( settings == NULL ) {
			LOG_ERROR( FMT( "Failed to get the parameters file path." ) );
			return ret;
		}

		// csv file name
		const char* pszCsvFileName = NULL;
		pszCsvFileName = settings->getParameterValue( "csv_filename" );

		FILE * fp;
		char szRow[1024] = { '\0' };	// NULL文字で埋める

		fp = fopen( pszCsvFileName , "r" );
		if( fp == NULL ) {
			printf( "Error: Can not open this file '%s'\n", pszCsvFileName );
			return ret;	// ファイルオープンエラー
		}

		while( fgets( szRow , sizeof( szRow ) , fp ) != NULL ) {
			szRow[sizeof( szRow )-1] = '\0';	// 不要だが念のため

			size_t length = strlen(szRow);
			if( sizeof(szRow)-length <= 1 ){
				// buffer一杯まで読み込んでいる
				printf( "row=%s", szRow );/* szRowの末尾は\n\0 */
				printf( "Error: Buffer over flow. In csv file, maximu length is %d bytes by row. \n", sizeof( szRow )-1 );
				fclose(fp);
				return ret;
			}
			if( szRow[0] == CSV_COMMENT_LETTER ){
				// コメント行です！
				printf( "Comment=%s" , szRow );
			}else{

				printf( "row=%s", szRow );/* szRowの末尾は\n\0 */

				int nRet = pSampleSetNew->getPtrCsvParser()->addRow( szRow );	// ここでCSVパーサに与える
				if( nRet != 0 ){
					flgNonError = false;	// エラー
					break;
				}
				// 行番号チェック
				{
					// 行番号が異常か否かをチェック
					int nLines = pSampleSetNew->getPtrCsvParser()->getNumberOfCsvData();
					if( 1 <= nLines ){
						int nRow = pSampleSetNew->getPtrCsvParser()->getCsvData( nLines-1 ).getRow();
						if( nRow == nLines ){
							// 行数と最新の行番号が一致。OK
						}else{
							;	// 行番号が異常
							printf( "Error: Row number error.\n" );
							return ret;
						}
					}
				}
			}
		}

		if( flgNonError == true ){
			flgFileOpenOk = pSampleSetNew->openFile( "", NULL );	// SampleTreeBuilderSampleSetクラス内のonOpenFile()が呼ばれる
		}else{
			// エラーが発生したので、変換処理しない
		}

		fclose(fp);
	}

	// この処理を追加すると、解放が不要になる(by 田中)
	if(( flgNonError == true )&&( flgFileOpenOk )){	// @date 2013/03/26 <Add> OKADA	// ファイルオープン失敗時、onOpenSample()内でクラッシュする為 条件追加
		SampleTreeBuilderSample  *pSampleNew = (*pSampleSetNew).getSample();
		kome::plugin::PluginCallTool::onOpenSample( *pSampleNew );
	
		if( flgFileOpenOk ){
			// ファイルに出力する設定
			kome::objects::ActiveObjectsManager& aoMgr = kome::objects::ActiveObjectsManager::getInstance();	// manager
			aoMgr.setActiveSample( pSampleNew );
		}
	}

	if(( flgNonError == true )&&( flgFileOpenOk == true )){
		ret.prim.boolVal = true;
	}else{
		ret.prim.boolVal = false;
	}

	return ret;
}

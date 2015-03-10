/**
 * @file PeakMatrixManager.cpp
 * @brief implements of PeakMatrixManager
 *
 * @author S.Tanaka
 * @date 2007.12.14
 * 
 * Copyright(C) 2006-2013 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "PeakMatrixManager.h"

#include <fstream>
#include <boost/bind.hpp>


#include <string.h>
 
using namespace kome::matrix;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


#define SECTION							 "Peak Matrix"
#define RT_TOLERANCE_PARAM_KEY			 "RT_TOLERANCE"
#define MZ_TOLERANCE_PARAM_KEY			 "MZ_TOLERANCE"

#define using_multivariate_analysis    // これが定義されていると、多変量解析プラグインが使える

#ifdef _WIN64
	#undef using_multivariate_analysis
#endif

// constructor
PeakMatrixManager::PeakMatrixManager() {
	// initialize
	m_standardId = -1;
}

// destructor
PeakMatrixManager::~PeakMatrixManager() {
}
 
// initialize matrix row
void PeakMatrixManager::initRow( const int prj ) {
	// manager
	kome::ident::IdentificationManager& identMgr = kome::ident::IdentificationManager::getInstance();

	// delete peaks
	std::vector< int > peaks;
	identMgr.getMatrixPeaks( peaks, prj );

	for( unsigned int i = 0; i < peaks.size(); i++ ) {
		identMgr.deleteMatrixPeak( peaks[ i ] );
	}
}

// initialize matrix column
void PeakMatrixManager::initCol( const int prj ) {
	// manager
	kome::ident::IdentificationManager& identMgr = kome::ident::IdentificationManager::getInstance();

	// delete samples
	std::vector< int > groups;
	identMgr.getGroups( groups, prj );
	for( unsigned int i = 0; i < groups.size(); i++ ) {
		identMgr.deleteGroup( groups[ i ] );
	}

	// delete columns
	std::vector< int > cols;
	identMgr.getMatrixColumns( cols, prj );

	for( unsigned int i = 0; i < cols.size(); i++ ) {
		identMgr.deleteMatrixColumn( cols[ i ] );
	}
}

// initialize matrix
void PeakMatrixManager::initMatrix( const int prj ) {
	// initialize row & col
	initRow( prj );
	initCol( prj );
}

// adds sample set
void PeakMatrixManager::addSampleSet(
		const char* path,
		const int prj,
		const COLOR color,
		const char* group
) {
	// check parameters
	if( path == NULL || prj < 0 ) {
		return;
	}

	// manager
	kome::ident::IdentificationManager& identMgr = kome::ident::IdentificationManager::getInstance();

	// group ID
	int groupId = -1;
	std::string g = NVL( group, "" );
	if( !g.empty() ) {
		// search
		std::vector< int > groups;
		identMgr.getGroups( groups, prj );

		for( unsigned int i = 0; i < groups.size() && groupId < 0; i++ ) {
			std::string tmp = identMgr.getGroupName( groups[ i ] );
			if( tmp.compare( group ) == 0 ) {
				groupId = groups[ i ];
			}
		}

		// add
		if( groupId < 0 ) {
			groupId = identMgr.addGroup( g.c_str(), prj, color );
		}
	}

	// add sample set
	kome::plugin::FileFactory& factory = kome::plugin::FileFactory::getInstance();

	bool opened0 = false;
	kome::objects::SampleSet* ss = factory.searchFile( path );
	if( ss == NULL ) {
		ss = factory.openFile( path );
	}
	else {
		opened0 = true;
	}

	for( unsigned int i = 0; i < ss->getNumberOfSamples(); i++ ) {
		kome::objects::Sample* s = ss->getSample( i );
		int sampleId = identMgr.addSample( s );
		std::string sampleName = s->getName();
		if( !sampleName.empty() ) {
			sampleName.append( " : " );
		}
		sampleName.append( getfilename( path ) );

		if( sampleId >= 0 ) {
			identMgr.addMatrixSample( groupId, sampleId, sampleName.c_str(), color );
		}
	}

	if( !opened0 ) {
		factory.closeFile( ss );
	}
}

// read sample list file
bool PeakMatrixManager::readSampleList( const char* path, const int prj, kome::core::Progress* progress ) {
	// progress
	if( progress == NULL ) {
		progress = &( kome::core::Progress::getIgnoringProgress() );
	}

	// array
	std::vector< SampleFile > sampleFiles;

	// file
	FILE* fp = fileopen( path, "r" );
	if( fp == NULL ) {
		LOG_ERROR_CODE( FMT( "Failed to open the file. [%s]", path ), ERR_FILE_OPEN_FAILED );
		return false;
	}

	// read
	char line[ 2048 ];
	while( fgets( line, 2048, fp ) != NULL ) {
		std::string smplFile;
		std::string group;
		COLOR color = kome::img::ColorTool::BLUE;

		line[ 2047 ] = '\0';

		std::vector< std::string > tokens;

		stringtoken(line, ",\t",tokens);

		// trim
		for( unsigned int i = 0; i < tokens.size(); i++ ) {
			tokens[ i ] = trimstring( tokens[ i ].c_str() );
		}

		// properties
		if( tokens.size() >= 1 ) {
			smplFile = tokens[ 0 ];
		}
		if( tokens.size() >= 2 ) {
			if( !tokens[ 1 ].empty() ) {
				color = kome::img::ColorTool::getColor( tokens[ 1 ].c_str() );
			}
		}
		if( tokens.size() >= 3 ) {
			group = tokens[ 2 ];
		}

		// add to the array
		if( !smplFile.empty() ) {
			sampleFiles.resize( sampleFiles.size() + 1 );
			sampleFiles.back().path = smplFile;
			sampleFiles.back().color = color;
			sampleFiles.back().group = group;

			// standard
			if( tokens.size() >= 4 ) {
				if( compareignorecase( tokens[ 3 ].c_str(), "standard" ) == 0 ) {
					setStandard( smplFile.c_str() );
				}
			}
		}
	}

	// close
	fclose( fp );

	// progress
	if( sampleFiles.size() == 0 ) {
		LOG_ERROR_CODE( FMT( "No sample raw data files could be added because the sample list file does not correctly specify any. Check the sample list file contents." ), ERR_OTHER );
		return false;
	}

	progress->setRange( 0, sampleFiles.size() );
	progress->setPosition( 0 );

	for( unsigned int i = 0; i < sampleFiles.size() && !progress->isStopped(); i++ ) {
		progress->setStatus(
			FMT(
				"Reading Raw Data... [%d/%d] (%s)",
				( i + 1 ),
				sampleFiles.size(),
				sampleFiles[ i ].path.c_str()
			).c_str()
		);

		addSampleSet(
			sampleFiles[ i ].path.c_str(),
			prj,
			sampleFiles[ i ].color,
			sampleFiles[ i ].group.c_str()
		);

		progress->setPosition( i + 1 );
	}

	if( !progress->isStopped() ) {
		progress->fill();
	}

	return true;
}

// set standard file path
void PeakMatrixManager::setStandard( const char* path ) {
	m_standard = absolutepath( path );
	m_standardId = -1;
}

// get standard file path
const char* PeakMatrixManager::getStandard() {
	return m_standard.c_str();
}

// get standard group ID
int PeakMatrixManager::getStandardGroupId() {
	// check members
	if( m_standardId >= 0 || m_standard.empty() ) {
		return m_standardId;
	}

	// manager
	kome::ident::IdentificationManager& identMgr = kome::ident::IdentificationManager::getInstance();

	// selected matrix
	int prj = identMgr.getSelectedMatrix();
	if( prj < 0 ) {
		return m_standardId;
	}

	// sample numbers
	std::vector< int > matrixGroups;
	std::vector< int > samples;
	identMgr.getGroups( matrixGroups, prj );
	for( unsigned int i = 0; i < matrixGroups.size(); i++ ) {
		identMgr.getMatrixSamplesFromGroup( samples, matrixGroups[ i ] );
	}

	// search
	for( unsigned int i = 0; i < samples.size() && m_standardId < 0; i++ ) {
		int matrixSampleId = samples[ i ];
		int sampleId = identMgr.getMatrixSampleSample( matrixSampleId );
		std::string path = identMgr.getSampleFilePath( sampleId );

		if( m_standard.compare( path ) == 0 ) {
			m_standardId = matrixSampleId;
		}
	}

	return m_standardId;
}

// fill matrix
bool PeakMatrixManager::fillMatrix(
		kome::plugin::PluginFunctionItem* peakSearch,
		kome::objects::SettingParameterValues* settings,
		const double rtTol,
		const double mzTol,
		const bool initFlg,
		std::vector< int >& groups,   // groupsとなっているが、samplesの間違い？
		const int stdGroup,
		kome::core::Progress* progress,
		const char* supplement_method,
		kome::ident::PeakPositions* peakPos,
		bool bPeakValStat,
		std::map< int, int >& peakIdMap
) {
	// manager
	kome::ident::IdentificationManager& identMgr = kome::ident::IdentificationManager::getInstance();
	
	// selected matrix
	int matrix = identMgr.getSelectedMatrix();

	// peaks
	std::vector< int > peaks;
	identMgr.getMatrixPeaks( peaks, matrix );
	
	// sample getter
	kome::ident::SampleGetter getter;
	
	// standard
	kome::objects::Sample* stdSample = NULL;
	kome::ident::SampleReadInfo stdReadInfo;
	
	// peak values calculator 
	kome::ident::PeakValuesCalculator* peakValCalc = NULL;

	// peak valueが選択可
	if( !bPeakValStat  ){
		// check parameters
		if( peakSearch == NULL ) {
			return false;
		}
		if( progress == NULL ) {
			progress = &kome::core::Progress::getIgnoringProgress();
		}

		
		// groups
		if( groups.size() == 0 ) {
			LOG_ERROR_CODE( FMT( "Fill Matrix processing could not be performed because no samples are specified in the matrix. Specify at least one sample." ), ERR_OTHER);
			return false;
		}

		// progress
		progress->createSubProgresses( groups.size() + 1 );
		kome::core::Progress* resetProgress = progress->getSubProgress( 0 );

		// matrix
		if( matrix < 0 ) {
			LOG_ERROR_CODE( FMT( "Fill Matrix processing cannot be performed because no matrix exists." ), ERR_OTHER );
			return false;
		}
		
		// init
		if( initFlg ) {
			for( unsigned int i = 0; i < peaks.size(); i++ ) {
				identMgr.deletePeakValuesFromMatrixPeak( peaks[ i ] );
			}
		}
		else {
			for( unsigned int i = 0; i < groups.size(); i++ ) {       // グループの数だけloop
				for( unsigned int j = 0; j < peaks.size(); j++ ) {    // ピークの数だけloop　
					double val = identMgr.getPeakValue( peaks[ j ], groups[ i ] );  // グループiに属するピークjの値を読んでくる
			
					identMgr.setNormalizedIntensity( peaks[ j ], groups[ i ], val );				
	
				}
			}
		}
		resetProgress->fill();

		if( stdGroup >= 0 ) { 
			int stdSampleId = identMgr.getMatrixSampleSample( stdGroup );
			stdSample = identMgr.openSample( stdSampleId, stdReadInfo );
		}
	
		kome::operation::Alignment* alignment = identMgr.getAlignment( matrix );    // アライメント実行
		
		getter.setMatrix( matrix );
		getter.setAlignment( alignment );
		getter.setStdSample( stdSample );

		// parameters
		kome::objects::Parameters params;
		if( settings != NULL ) {
			kome::plugin::PluginCallTool::setSettingValues( params, *settings );
		}
		
		peakValCalc = (kome::ident::PeakValuesCalculator*)(peakSearch->getCall()->invoke( &params ).prim.pt);

		if( peakValCalc == NULL ){
			return false;
		}

		// peak values calculation
		peakValCalc->calculateValues( getter, *peakPos, *progress );	 

		int peakNum = peakPos->getNumberOfPeaks();
		for( int i=0; i < peakNum; i++ ){
			double rt = peakPos->getPeakPositionRt( i );
			double mz = peakPos->getPeakPositionMz( i );
			double charge = peakPos->getPeakPositonCharge( i );
			
			// add peak
			int matrixPeakId = -1;
			if( peakIdMap.find( i ) != peakIdMap.end() ) {
				matrixPeakId = peakIdMap[ i ];
			}
			
			if( matrixPeakId >= 0 ) {
				for( int j=0; j < peakPos->getNumberOfMatrixSamples(); j++ ){
					int sampleId = peakPos->getMatrixSampleId( j );
					kome::objects::PeakElement* pk = peakPos->getPeak( i, sampleId );
					
					double val = peakPos->getPeakValue( i, sampleId );
					
					int profile = peakPos->getProfile( i, sampleId );
					
					if( matrixPeakId >= 0 ) {
						identMgr.setPeakValue( matrixPeakId, sampleId, val, pk, profile );  // ピークIDとサンプルIDで指定されるセルに、ピークの値をセット
					}
				}
			}		
		}
	}

	// fill matrixはここまでで終了	
	
	// @date 2013/08/28 <Add> FUJITA--------------->
	// 以下、セル中の値が0の箇所に、ユーザが選択した手法で適切な値を入れる処理

	const char* direction_type = peakSearch->getShortName();   // "spectrum"方向か"chromatgram"方向か？
	const char* value_type = settings->getParameterValue("val_type");//  getParameterName(3);  // 求めているのは"intensity"か"area"か？

	if (strcmp(value_type, "area") == 0){ // value_typeがareaだったらif文の中に入る
		// ピーク面積を計算している場合のみ下記を実行。intensityでピークマトリクスを作成する場合は実行しない
		
		// 以下、補完のコード
		if(strcmp(supplement_method, "(None)") == 0) {  // セル補完方法が(None)なら、デフォルトである「空欄」のまま何もしない
			; // 何もしない
		} else { // ピークを補間する場合は、下記を実行
			// ピークのm/z、RT値を入れた配列を作成。
			std::vector<double> Rt_array;  // 
			std::vector<double> Mz_array;  // 
			
			for( unsigned int i=0; i < peakIdMap.size(); i++ ){		

				double rt = peakPos->getPeakPositionRt( i );
				double mz = peakPos->getPeakPositionMz( i );
				Rt_array.push_back( rt );   // 対象のピークのm/z    
				Mz_array.push_back( mz );   // 対象のピークのRT     
				
//				Rt_array.push_back( identMgr.getMatrixPeakRt(i) );   // 対象のピークのm/z    
//				Mz_array.push_back( identMgr.getMatrixPeakMz(i) );   // 対象のピークのRT     

			}
			// 補完する
			std::pair< kome::objects::Sample*, kome::ident::SampleReadInfo > tmp;
			for ( unsigned int k = 0; k < groups.size(); k++ ){    //// groupsとなっているが、samplesの間違い？ ここはgroupsは実はsamplesであると考えて実装してみている
																   // サンプルごとに考える
				int sampleId = groups[ k ];	
				int original_sampleId = identMgr.getMatrixSampleSample(sampleId);
				kome::ident::SampleReadInfo sampleReadInfo;
				kome::objects::Sample* Sample = identMgr.openSample( original_sampleId, sampleReadInfo );

				kome::objects::Spectrum* spec = Sample->getSpectrumById(0);   // 一先ず0を入れる
				kome::core::XYData* xyData = spec->getXYData();
				int xydata_length = xyData->getLength(); 

				tmp.first = Sample;
				tmp.second = sampleReadInfo;
		
//				int before_index = 1;   // 面積計算時のfor-loopの開始index
				int before_index = 0;   // 面積計算時のfor-loopの開始index ////
				for( unsigned int i = 0; i < peakIdMap.size(); i++ ) {  // サンプルの、あるピーク位置において、ピークareaの値が0であるセルを探す。
    				int peakId = peakIdMap[ i ];
                    // ピークのm/z、RT値を得る
					double peakRt = Rt_array[i];  //    identMgr.getMatrixPeakMz(peakId);   // 対象のピークのm/z
			        double peakMz = Mz_array[i];   // 対象のピークのRT
			
				//	double cell_val = identMgr.getNormalizedPeakValue( peakId, sampleId );  // 注目しているセル中の値
					double cell_val = identMgr.getPeakValue( peakId, sampleId );

					if ( cell_val > 0 ){
						;	// ピーク値が0以上なら何もしない
					} else {    // 0以下、ないしその他の値(空欄など)だったら、ユーザが選択した方法で適当な値を用いて埋める
						
						// ピーク情報取得
						kome::objects::PeakElement* pk = peakPos->getPeak( peakId, sampleId );

						// profile取得(ピーク波形描画に必要)
						int profile = -1;                    // 適当な値を入れる
						profile = peakPos->getProfile( i, sampleId );
		
						if (strcmp(supplement_method, "0") == 0 ){   // 0を入れる
							if( peakId >= 0 ){	
								identMgr.setPeakValue( peakId, sampleId, 0, pk, profile );   // ピークIDとサンプルIDで指定されるセルに、ピークの値をセット
							}
						} else if ( strcmp(supplement_method, "Area") == 0 ){   // ピーク面積を計算して、入れる
							
							double left_mz = peakMz - mzTol/2;
							double right_mz = peakMz + mzTol/2;
							double left_rt = peakRt - rtTol/2;
							double right_rt = peakRt + rtTol/2;

							if(strcmp(direction_type, "spectrum") == 0) { // スペクトラム方向の場合 //////////
								double area = 0.0;   // calculate 面積

								int data_point_count = 0;
								for (int l = before_index; l < xydata_length-1; l++) {   
								 	if (left_mz < xyData->getX(l) && xyData->getX(l) < right_mz) {
								 		// x, y
								 		double x0 = xyData->getX( l - 1 );
										double y0 = xyData->getY( l - 1 );
										double x1 = xyData->getX( l );
										double y1 = xyData->getY( l );
										// add area
										area += ( y0 + y1 ) * ( x1 - x0 ) / 2.0;   // 範囲内の台形の面積を足していく
										data_point_count++;
									} else if (right_mz < xyData->getX(l) ) {
										break; 			//  	xyData->getX(l)には昇順でm/z値が入っている。right_mz < xyData->getX(l)になったらfor-loop打ち切り													
									}
									before_index = l - data_point_count; // for-loopのindexを、一つ前のピークのleft_mzのindexから始める。
								}
		
								if( peakId >= 0 ){	
									identMgr.setPeakValue( peakId, sampleId, area, pk, profile );   // ピークIDとサンプルIDで指定されるセルに、ピークの値をセット
								}
							} else {                                    // クロマトグラム方向の場合 //////////
								kome::objects::DataGroupNode* node = Sample->getGroup(0);
								unsigned int spec_number = node->getNumberOfSpectra();
								double RT_area = 0;
								double pre_rt = 0;
								double current_rt = 0;
								double pre_max = 0;
								double current_max = 0;
								int first_flg = 0;
								for ( unsigned int l = 0; l < spec_number; l++){
									kome::objects::Spectrum* temp_spec = node->getSpectrum(l);
									kome::core::XYData* temp_xyData = temp_spec->getXYData();
									int temp_xydata_length = temp_xyData->getLength(); 
									current_rt = temp_spec->getRt();
									if (left_rt < current_rt && current_rt < right_rt) {
										first_flg = 1;
										for (int ll = 0; ll <  temp_xydata_length; ll++){
											if (left_mz < temp_xyData->getX(ll) && temp_xyData->getX(ll) < right_mz && current_max < temp_xyData->getY(ll)) {
												current_max = temp_xyData->getY(ll);     // m/z tolerance内で最も大きいintensityを用いて面積を計算する
											}
										}
										if (first_flg > 0 ){
											RT_area += ( pre_max + current_max ) * ( current_rt - pre_rt ) / 2.0;   // 範囲内の台形の面積を足していく
										}
										pre_rt = current_rt;
										pre_max = current_max;
									}
								}
								if( peakId >= 0 ){	
									identMgr.setPeakValue( peakId, sampleId, RT_area, pk, profile ); 
								}
							}								
						}
				 	}
				} // for( unsigned int i = 0; i < peaks.size(); i++ ) { とペア

				identMgr.releaseSample( tmp.first, tmp.second );

			} // for ( unsigned int k = 0; k < groups.size(); k++ ){ とペア 
		} // if(strcmp(supplement_method, "(None)") == 0) { とペア
	}
	// @date 2013/08/28 <Add> FUJITA---------------<

	identMgr.releaseSample( stdSample, stdReadInfo );

	if( !progress->isStopped() ) {
		progress->fill();
	}

	delete peakValCalc;

	return true;
}


// scaling peaks 
bool PeakMatrixManager::scalingPeaks(
	kome::plugin::PluginFunctionItem** scalingArr,
	kome::objects::SettingParameterValues** settingsArr,
	const unsigned int num,
	const int stdGroup,
	kome::core::Progress* progress
){
	bool ret = true;

	// check parameters
	if( scalingArr == NULL || settingsArr == NULL || num == 0 ) {
		LOG_ERROR_CODE( FMT( "Cannot perform Peak Scaling because no methods for peak analysis are specified." ), ERR_NULL_POINTER );
		return false;
	}

	if( progress == NULL ) {
		progress = &kome::core::Progress::getIgnoringProgress();
	}

	// manager
	kome::ident::IdentificationManager& identMgr = kome::ident::IdentificationManager::getInstance();

	// selected matrix
	int matrix = identMgr.getSelectedMatrix();
	if( matrix < 0 ) {
		LOG_ERROR_CODE( FMT( "Peak Scaling processing cannot be performed because no matrix exists." ), ERR_OTHER );
		return false;
	}

	// progress
	progress->createSubProgresses( num );

	// groupus
	std::vector< int > samples;
	std::vector< int > groups;
	identMgr.getGroups( groups, matrix );
	for( unsigned int i = 0; i < groups.size(); i++ ) {
		identMgr.getMatrixSamplesFromGroup( samples, groups[ i ] );
	}
	
	// scaling
	int matrixSampleId = groups[ 0 ];
	int sampleId = identMgr.getMatrixSampleSample( matrixSampleId );


	for( unsigned int j = 0; (j < num ) && !progress->isStopped() && (ret); j++ ) {

		kome::plugin::PluginFunctionItem* scaling = scalingArr[ j ];
		kome::objects::SettingParameterValues* settings = settingsArr[ j ];

		// progress
		kome::core::Progress* subProgress = progress->getSubProgress( j );
		progress->setStatus( 
			FMT( "Peak scaling [%d/%d] ... %s", ( j + 1 ), num, scaling->getLongName() ).c_str()
		);


		// search
		// parameters
		kome::objects::Parameters params;
		if( settings != NULL ) {
			kome::plugin::PluginCallTool::setSettingValues( params, *settings );
		}
		kome::plugin::PluginCallTool::setProgress( params, *subProgress );
		kome::plugin::PluginCallTool::setInt( params, "matrix", matrix );
			
		// scaling
		ret = scaling->getCall()->invoke( &params ).prim.boolVal;
	}
	
	if( !progress->isStopped() ) {
		progress->fill();
	}

	return ret;
}

// analyze peaks
bool PeakMatrixManager::analyzePeaks(
		kome::plugin::PluginFunctionItem** analysisArr,
		kome::objects::SettingParameterValues** settingsArr,
		const unsigned int num,
		const int stdGroup,
		kome::core::Progress* progress
) {
	bool ret = true;

	// check parameters
	if( analysisArr == NULL || settingsArr == NULL || num == 0 ) {
		LOG_ERROR_CODE( FMT( "Cannot perform Peak Analysis because no methods for peak analysis are specified." ), ERR_NULL_POINTER );
		return false;
	}

	if( progress == NULL ) {
		progress = &kome::core::Progress::getIgnoringProgress();
	}

	// manager
	kome::ident::IdentificationManager& identMgr = kome::ident::IdentificationManager::getInstance();

	// selected matrix
	int matrix = identMgr.getSelectedMatrix();
	if( matrix < 0 ) {
		LOG_ERROR_CODE( FMT( "Peak Analysis processing cannot be performed because no matrix exists." ), ERR_OTHER );
		return false;
	}
// --> <Del> by.Fujita
	// standard
//	kome::objects::Sample* stdSample = NULL;
//	kome::ident::SampleReadInfo stdReadInfo;
//	if( stdGroup >= 0 ) { 
//		int stdSampleId = identMgr.getMatrixSampleSample( stdGroup );
//		stdSample = identMgr.openSample( stdSampleId, stdReadInfo );
//	}
// --< 

	// progress
	progress->createSubProgresses( num );

	// analysis
	for( unsigned int i = 0; (i < num && !progress->isStopped()) && (ret); i++ ) {
		kome::plugin::PluginFunctionItem* analysis = analysisArr[ i ];
		kome::objects::SettingParameterValues* settings = settingsArr[ i ];

		// progress
		kome::core::Progress* subProgress = progress->getSubProgress( i );
		progress->setStatus( 
			FMT( "Peak Analysis [%d/%d] ... %s", ( i + 1 ), num, analysis->getLongName() ).c_str()
		);

		// parameters
		kome::objects::Parameters params;
// --> <Del> by.Fujita
//		if( stdSample != NULL ) {
//			kome::plugin::PluginCallTool::setControlGroup( params, *( stdSample->getRootDataGroupNode() ) );
//		}
// --< 
		kome::plugin::PluginCallTool::setProgress( params, *subProgress );
		if( settings != NULL ) {
			kome::plugin::PluginCallTool::setSettingValues( params, *settings );
		}

		kome::plugin::PluginCallTool::setInt( params, "matrix", matrix );

		// analysis
		ret = analysis->getCall()->invoke( &params ).prim.boolVal;
		subProgress->fill();
	}

//	identMgr.releaseSample( stdSample, stdReadInfo );  <Del> 20140728 by.Fujita

	if( !progress->isStopped() ) {
		progress->fill();
	}

	return ret;
}

#ifdef using_multivariate_analysis  
// multivariate analysis peaks
bool PeakMatrixManager::multivariate_analysisPeaks(
		kome::plugin::PluginFunctionItem** multivariate_analysisArr,
		kome::objects::SettingParameterValues** settingsArr,
		const unsigned int num,
		const int stdGroup,
		kome::core::Progress* progress
) {
	bool ret = true;

	// check parameters
	if( multivariate_analysisArr == NULL || settingsArr == NULL || num == 0 ) {
		LOG_ERROR_CODE( FMT( "Cannot perform Multivariate Analysis because no methods for multivariate analysis are specified." ), ERR_NULL_POINTER );
		return false;
	}

	if( progress == NULL ) {
		progress = &kome::core::Progress::getIgnoringProgress();
	}

	// manager
	kome::ident::IdentificationManager& identMgr = kome::ident::IdentificationManager::getInstance();

	// selected matrix
	int matrix = identMgr.getSelectedMatrix();
	if( matrix < 0 ) {
		LOG_ERROR_CODE( FMT( "Multivariate Analysis processing cannot be performed because no matrix exists." ), ERR_OTHER );
		return false;
	}

	// progress
	progress->createSubProgresses( num );

	// multivariate analysis
	for( unsigned int i = 0; (i < num && !progress->isStopped()) && (ret); i++ ) {
		kome::plugin::PluginFunctionItem* multivariate_analysis = multivariate_analysisArr[ i ];
		kome::objects::SettingParameterValues* settings = settingsArr[ i ];

		// progress
		kome::core::Progress* subProgress = progress->getSubProgress( i );
		progress->setStatus( 
			FMT( "Peak Analysis [%d/%d] ... %s", ( i + 1 ), num, multivariate_analysis->getLongName() ).c_str()
		);

		// parameters
		kome::objects::Parameters params;
		kome::plugin::PluginCallTool::setProgress( params, *subProgress );
		if( settings != NULL ) {
			kome::plugin::PluginCallTool::setSettingValues( params, *settings );
		}

		kome::plugin::PluginCallTool::setInt( params, "matrix", matrix );

		// multivariate analysis
		ret = multivariate_analysis->getCall()->invoke( &params ).prim.boolVal;          // 多変量解析実行
		subProgress->fill();
	}

	if( !progress->isStopped() ) {
		progress->fill();
	}


	return ret;
}
#endif

// normalizations
bool PeakMatrixManager::normalizations(
	kome::plugin::PluginFunctionItem* peakDetect,
	kome::objects::SettingParameterValues* peakDetectSettings,
	kome::plugin::PluginFunctionItem* ThrFunc,
	kome::objects::SettingParameterValues* ThrSettings,
	kome::plugin::PluginFunctionItem* RtRangeFunc,
	kome::objects::SettingParameterValues* RtRangeSettings,
	kome::plugin::PluginFunctionItem* MzRangeFunc,
	kome::objects::SettingParameterValues* MzRangeSettings,
	kome::plugin::PluginFunctionItem** normalizArr,
	kome::objects::SettingParameterValues* settingsArr,
	std::vector< int >& groups,
	const unsigned int num, // PluginFunctionItem数
	const int stdGroup,
	kome::core::Progress* progress
){
	bool ret = true;

	// check peak detect params
	if( peakDetect == NULL || peakDetectSettings == NULL ){
		LOG_ERROR_CODE( FMT( "Cannot perform Normalization Peak Detection because no methods for peak detect are specified." ), ERR_NULL_POINTER );
		return false;
	}

	// check parameters
	if( normalizArr == NULL || settingsArr == NULL || num == 0 ){
		LOG_ERROR_CODE( FMT( "Cannot perform Normalization because no methods for normalization are specified." ), ERR_NULL_POINTER );
		return false;
	}

	if( progress == NULL ){
		progress = &kome::core::Progress::getIgnoringProgress();
	}
		
	// manager
	kome::ident::IdentificationManager& identMgr = kome::ident::IdentificationManager::getInstance();

	// groups
	if( groups.size() == 0 ) {
		LOG_ERROR_CODE( FMT( "Normalization processing could not be performed because no samples are specified in the matrix. Specify at least one sample." ), ERR_OTHER);
		return false;
	}

	// selected matrix
	int matrix = identMgr.getSelectedMatrix();
	if( matrix < 0 ) {
		LOG_ERROR_CODE( FMT( "Normalization processing cannot be performed because no matrix exists." ), ERR_OTHER );
		return false;
	}

	// progress
	progress->createSubProgresses( groups.size() + 1 );
	kome::core::Progress* resetProgress = progress->getSubProgress( 0 );	

	for( unsigned int i = 0; i < groups.size(); i++ ) {
		identMgr.deletePeakValuesFromMatrixSample( groups[ i ] );
	}

	// standard
	kome::objects::Sample* stdSample = NULL;
	kome::ident::SampleReadInfo stdReadInfo;

	if( stdGroup >= 0 ){
		int stdSampleId = identMgr.getMatrixSampleSample( stdGroup );
		stdSample = identMgr.openSample( stdSampleId, stdReadInfo );
	}
	

	// manager
	PeakMatrixManager& mgr = PeakMatrixManager::getInstance();
	bool btype = false;
	double intentMin = -1.0;
	double intentMax = -1.0;
	double rtMin = -1.0;
	double rtMax = -1.0;
	double mzMin = -1.0;
	double mzMax = -1.0;

	// Peak Filtering
	// Threshold
	if( ThrFunc != NULL && ThrSettings != NULL ){
		mgr.setNormalizFilter( 
			matrix, 
			ThrFunc, 
			ThrSettings, 
			"threshold",
			intentMin,
			intentMax,
			btype,
			rtMin,
			rtMax,
			mzMin,
			mzMax );
	}

	// RT Range
	if( RtRangeFunc != NULL && RtRangeSettings != NULL ){
		mgr.setNormalizFilter( matrix, RtRangeFunc, RtRangeSettings, "rt_range",
			intentMin,
			intentMax,
			btype,
			rtMin,
			rtMax,
			mzMin,
			mzMax  );
	}

	// m/z Range
	if( MzRangeFunc != NULL && MzRangeSettings != NULL ){
		mgr.setNormalizFilter( matrix, MzRangeFunc, MzRangeSettings, "mz_range" ,
			intentMin,
			intentMax,
			btype,
			rtMin,
			rtMax,
			mzMin,
			mzMax );
	}
		
	std::string strParam;
	kome::plugin::SettingsPage* page = peakDetect->getSettingsPage();
	if( page != NULL ){
		strParam = page->getParameterString( *peakDetectSettings );
	}
	identMgr.setNormalizationFilter( 
		matrix, 
		intentMin,
		intentMax, 
		btype, 
		rtMin, 
		rtMax, 
		mzMin, 
		mzMax, 
		peakDetect->getShortName(),
		strParam.c_str() );
	

	// add method
	for( unsigned int i=0; i < num ; i++ ){
		kome::plugin::PluginFunctionItem* item = normalizArr[i];
		kome::objects::SettingParameterValues settings = settingsArr[i];
	
		kome::plugin::SettingsPage* page = item->getSettingsPage();
		std::string sparam  = ( page != NULL ? page->getParameterString( settings ) : "" );
								
		// add Normalization Method 
		identMgr.addNormalizationMethod( matrix, item->getShortName(), sparam.c_str() );	
	}
	
	// search
	for( unsigned int i = 0; i < groups.size() && !progress->isStopped(); i++ ) {
		// sample
		int matrixSampleId = groups[ i ];
		int sampleId = identMgr.getMatrixSampleSample( matrixSampleId );

		kome::ident::SampleReadInfo readInfo;
		kome::objects::Sample* sample = identMgr.openSample( sampleId, readInfo );
					
		// progress
		kome::core::Progress* subProgress = progress->getSubProgress( i + 1 );
		progress->setStatus( 
			FMT( "Normalization [%d/%d] ... %s", ( i + 1 ), groups.size(), sample->getRootDataGroupNode()->getName() ).c_str()
		);

		// search
		if( sample != NULL ){

			subProgress->fill();

			// release
			identMgr.releaseSample( sample, readInfo );
			
		}

		// progress
		progress->setPosition( i + 1 );
	}
		
	identMgr.releaseSample( stdSample, stdReadInfo );
	if( !progress->isStopped() ) {
		progress->fill();
	}

	return ret;
}

// set normaliz filter
void PeakMatrixManager::setNormalizFilter( 
	int matrix,
	kome::plugin::PluginFunctionItem* item,
	kome::objects::SettingParameterValues* settings,
	const char* paramval,
	double& intentMin,
	double& intentMax,
	bool& btype,
	double& rtMin,
	double& rtMax,
	double& mzMin,
	double& mzMax
){
	// manager
	kome::ident::IdentificationManager& identMgr = kome::ident::IdentificationManager::getInstance();

	kome::plugin::SettingsPage* page = item->getSettingsPage();
	std::string strParam;
	if( page != NULL ){
		strParam = page->getParameterString( *settings );
	}

	const char* strSetting = settings->getParameterValue( paramval );
	std::vector<std::string> vals;
	kome::plugin::SettingsValue::separateListValue( strSetting, vals );
	std::vector< double > range;
	for( unsigned int i=0; i < vals.size(); i++ ){
		range.push_back( todouble( vals[i].c_str(), -1.0 ) );
	}
		
	if( !range.empty() ){
		if( strcmp( item->getShortName(), "call_threshold" ) == 0 ){
			const char* type = settings->getParameterValue( "type" );
		
			if( strcmp( type, "absolute" ) != 0 ){
				btype = true;
			}
			intentMin = ( range.size() >= 1 ? range[0] : -1.0 );
			intentMax = ( range.size() >= 2 ? range[1] : -1.0 );
		}else if( strcmp( item->getShortName(), "call_rt_range" ) == 0 ){
			rtMin = ( range.size() >= 1 ? range[0] : -1.0 );
			rtMax = ( range.size() >= 2 ? range[1] : -1.0 );
		}else if( strcmp( item->getShortName(), "call_mz_range" ) == 0 ){
			mzMin = ( range.size() >= 1 ? range[0] : -1.0 );
			mzMax = ( range.size() >= 2 ? range[1] : -1.0 );
		}
	}		
}


// output matrix data
bool PeakMatrixManager::outputMatrix( const int prj, const char* path ) {
	// manager
	kome::ident::IdentificationManager& identMgr = kome::ident::IdentificationManager::getInstance();

	// open
	FILE* fp = fileopen( path, "w" );
	if( fp == NULL ) {
		LOG_ERROR_CODE( FMT( "Failed to open the file for writing. Check file / folder permissions. [%s]", path ), ERR_FILE_OPEN_FAILED );
		return false;
	}

	// appended columns
	std::vector< int > cols;
	identMgr.getMatrixColumns( cols, prj );

	// groups
	std::vector< int > matrixGroups;
	std::vector< int > samples;
	identMgr.getGroups( matrixGroups, prj );
	for( unsigned int i = 0; i < matrixGroups.size(); i++ ) {
		identMgr.getMatrixSamplesFromGroup( samples, matrixGroups[ i ] );
	}

	// peaks
	std::vector< int > peaks;
	identMgr.getMatrixPeaks( peaks, prj );

	// write header
	fprintf( fp, "RT\tm/z" );

	std::vector< kome::ident::ColumnDataType > types;
	if( cols.size() > 0 ) {
		types.resize( cols.size() );
	}

	for( unsigned int i = 0; i < cols.size(); i++ ) {
		kome::ident::ColumnDataType type = identMgr.getMatrixColumnType( cols[ i ] );
		std::string colName = identMgr.getMatrixColumnName( cols[ i ] );

		fprintf( fp, "\t%s", colName.c_str() );
		types[ i ] = type;
	}

	for( unsigned int i = 0; i < samples.size(); i++ ) {
		std::string name = identMgr.getMatrixSampleName( samples[ i ] );
		fprintf( fp, "\t%s", name.c_str() );
	}
	fprintf( fp, "\n" );

	// write matrix
	for( unsigned int i = 0; i < peaks.size(); i++ ) {
		// position
		const int peak = peaks[ i ];
		double rt = identMgr.getMatrixPeakRt( peak );
		double mz = identMgr.getMatrixPeakMz( peak );

		std::string rtStr = ( rt >= 0.0 ? FMT( "%f", rt ) : "" );
		std::string mzStr = ( mz >= 0.0 ? FMT( "%f", mz ) : "" );

		fprintf( fp, "%s\t%s", rtStr.c_str(), mzStr.c_str() );

		// appended column
		for( unsigned int j = 0; j < cols.size(); j++ ) {
			const int col = cols[ j ];
			std::string val = identMgr.getAppendedValue( peak, col );
			if( types[ j ] == kome::ident::TYPE_UDOUBLE
					|| types[ j ] == kome::ident::TYPE_UINT ) {
				double v = todouble( val.c_str(), -1.0 );
				if( v < 0.0 ) {
					val = "";
				}
			}

			fprintf( fp, "\t%s", val.c_str() );
		}

		// peak value
		for( unsigned int j = 0; j < samples.size(); j++ ) {
			const double intensity = identMgr.getNormalizedPeakValue( peak, samples[ j ] );
			if( intensity < 0.0 ) {
				fprintf( fp, "\t" );
			}
			else {
				fprintf( fp, "\t%f", intensity );
			}
		}

		fprintf( fp, "\n" );
	}

	// close
	fflush( fp );
	fclose( fp );

	return true;
}

// get instance
PeakMatrixManager& PeakMatrixManager::getInstance() {
	// create object
	static PeakMatrixManager mgr ;

	return mgr;
}



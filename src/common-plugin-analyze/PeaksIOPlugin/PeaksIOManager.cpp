/**
 * @file PeaksIOManager.cpp
 * @brief implements of PeaksIOManager class
 *
 * @author S.Tanaka
 * @date 2011.02.14
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "PeaksIOManager.h"
#include "PeaksIOPluginOperation.h"


using namespace kome::io::peaks;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



#define OVERLAPPING_COLOR      kome::img::ColorTool::FUCHSIA


// constructor
PeaksIOManager::PeaksIOManager() {
	m_blink = -1;
	m_blinkCnt = 0;

	m_point.clear();
	m_filter = -1;
}

// destructor
PeaksIOManager::~PeaksIOManager() {
}

bool PeaksIOManager::getDataSet( kome::objects::Variant var, int target, kome::objects::DataSet& dataSet ){
	bool ret = false;
	kome::objects::MsDataVariant obj( var );
			
	// spec
	kome::objects::Spectrum* spec = obj.getSpectrum();
	// chrom
	kome::objects::Chromatogram* chrom = obj.getChromatogram();
	// sample
	kome::objects::Sample* sample = obj.getSample();

	if( var.type == kome::objects::Variant::SPECTRUM ) {
		// spectrum
		if( spec == NULL ){
			LOG_ERROR( FMT( "Failed to get an active spectrum." ) );
			//return ret;
		}
		if( target == 0  ){
			kome::objects::DataSet ds( spec->getGroup() );
			ds.addSpectrum( spec );
			dataSet = ds;	
				
		}
	}else if( var.type == kome::objects::Variant::CHROMATOGRAM ) {
		// chromatogram
		if( chrom == NULL ){
			LOG_ERROR( FMT( "Failed to get an active chromatogram." ) );
			return ret;
		}

		if( target == 0 ){
			kome::objects::DataSet ds( chrom->getGroup() );
			ds.addChromatogram( chrom );
			dataSet = ds;	
		}
	}

	if( target == 1 ){
		// active objects manager
		kome::objects::ActiveObjectsManager& aoMgr = kome::objects::ActiveObjectsManager::getInstance();
		kome::objects::DataSet* ds = aoMgr.getFilteredDataSet( sample );
		if( sample == NULL || ds == NULL ) {
			LOG_ERROR( FMT( "A group is not active." ) );
			
			return ret;
		}
		dataSet = *ds;
	}else if( target == 2 ){
		// sample
		sample = obj.getSample();
		
		if( sample == NULL ) {
			LOG_ERROR( FMT( "A sample is not active." ) );
			return ret;
		}

		// data set	
		kome::objects::DataGroupNode* root = sample->getRootDataGroupNode();
		kome::objects::DataSet ds( root );
		root->getDataSet( &ds );

		dataSet = ds;		
	}

	return true;
}

// export spectrum peaks
bool PeaksIOManager::exportSpecPeaks( kome::objects::DataSet ds, const char* path ){
	// open the file
	FILE* fp = fileopen( path, "w" );
	if( fp == NULL ) {
		LOG_ERROR_CODE( FMT( "Failed to open the file. [%s]", path ), ERR_FILE_OPEN_FAILED );
		return false;
	}

	// get peak
	kome::objects::PeaksManager& pkMgr = kome::objects::PeaksManager::getInstance();
	for( unsigned int i=0; i < ds.getNumberOfSpectra(); i++ ){
		kome::objects::Spectrum* spec = ds.getSpectrum( i );
		kome::objects::Peaks* peaks = pkMgr.getPeaks( spec );

		if( peaks == NULL ){
			LOG_ERROR_CODE( FMT( "Peaks cannot be exported because there is no peak information. Detect peaks first." ), ERR_NULL_POINTER );
// >>>>>>	SPEC95338	@Date:2013/10/28	<Add>	A.Ozaki
			fflush( fp );
			fclose( fp );
// <<<<<<	SPEC95338	@Date:2013/10/28	<Add>	A.Ozaki
			return false;
		}
				
		fprintf( fp, "# SpectrumId %d\n", spec->getId() ); // Spec No.97279 @date 2014.02.25 <Add> M.Izumi
		for( unsigned int j=0; j < peaks->getLength(); j++ ){
			kome::objects::PeakElement* peak = peaks->getPeak( j );
			if( peak != NULL ){
				fprintf(
					fp,
					"%f\t%f\t%f\t%f\t%f\t%f\n", 
					peak->getX(), 
					peak->getY(), 
					peak->getLeftX(), 
					peak->getRightX(), 
					peak->getLeftY(),
					peak->getRightY() );
			}
		}
		fprintf( fp, "\n" );	// @date 2014.02.25 <Add> M.Izumi
	}
	
	// close
	fflush( fp );
	fclose( fp );

	return true;
}

// export chromatogram peaks
bool PeaksIOManager::exportChromPeaks( kome::objects::DataSet ds, const char* path ){
	// open the file
	FILE* fp = fileopen( path, "w" );
	if( fp == NULL ) {
		LOG_ERROR_CODE( FMT( "Failed to open the file. [%s]", path ), ERR_FILE_OPEN_FAILED );
		return false;
	}

	
	// get peak
	kome::objects::PeaksManager& pkMgr = kome::objects::PeaksManager::getInstance();
	for( unsigned int i=0; i < ds.getNumberOfChromatograms(); i++ ){
		kome::objects::Chromatogram* chrom = ds.getChromatogram( i );
		kome::objects::Peaks* peaks = pkMgr.getPeaks( chrom );
		
		if( peaks == NULL ){
			LOG_ERROR_CODE( FMT( "Peaks cannot be exported because there is no peak information. Detect peaks first." ), ERR_NULL_POINTER );
// >>>>>>	SPEC95338	@Date:2013/10/28	<Add>	A.Ozaki
			fflush( fp );
			fclose( fp );
// <<<<<<	SPEC95338	@Date:2013/10/28	<Add>	A.Ozaki
			return false;
		}
				
		for( unsigned int j=0; j < peaks->getLength(); j++ ){
			kome::objects::PeakElement* peak = peaks->getPeak( j );
			if( peak != NULL ){
				fprintf(
					fp,
					"%f\t%f\t%f\t%f\t%f\t%f\n", 
					peak->getX(), 
					peak->getY(), 
					peak->getLeftX(), 
					peak->getRightX(), 
					peak->getLeftY(),
					peak->getRightY() );
			}

		}
	}

	// close
	fflush( fp );
	fclose( fp );

	return true;	
}

// export 2D peaks
bool PeaksIOManager::export2dPeaks(
		kome::objects::DataGroupNode* group, 
		const char* path
) {
	// get peak
	kome::objects::PeaksManager& pkMgr = kome::objects::PeaksManager::getInstance();
	kome::objects::Peaks2D* peaks = pkMgr.getPeaks( group );
	if( peaks == NULL ) {
		LOG_ERROR_CODE( FMT( "Peaks cannot be exported because there is no peak information. Detect peaks first." ), ERR_NULL_POINTER );	// @date 2012/04/26 <Mod> OKADA	// SPEC 85580(1)
		return false;
	}

	// open the file
	FILE* fp = fileopen( path, "w" );
	if( fp == NULL ) {
		LOG_ERROR_CODE( FMT( "Failed to open the file. [%s]", path ), ERR_FILE_OPEN_FAILED );
		return false;
	}

	// clusters
	if( peaks->getNumberOfClusters() > 0) {
		fprintf( fp, "> Clusters\n" );
	}
	std::map< kome::objects::PeaksCluster2D*, int > clusterMap;

	for( unsigned int i = 0; i < peaks->getNumberOfClusters(); i++ ) {
		kome::objects::PeaksCluster2D* cluster = peaks->getCluster( i );

		int id = (int)i + 1;

		fprintf(
			fp,
			"%d\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%d\n",
			id,
			cluster->getRt(),
			cluster->getMz(),
			cluster->getIntensity(),
			cluster->getStartRt(),
			cluster->getEndRt(),
			cluster->getStartMz(),
			cluster->getEndMz(),
			cluster->getCharge()
		);

		clusterMap[ cluster ] = id;
	}

	// blank
	if( peaks->getNumberOfClusters() > 0 ) {
		fprintf( fp, "\n" );
	}

	// peaks
	if( peaks->getNumberOfPeaks() > 0 ) {
		fprintf( fp, "> Peaks\n" );
	}

	for( unsigned int i = 0; i < peaks->getNumberOfPeaks(); i++ ) {
		kome::objects::Peak2DElement* peak = peaks->getPeak( i );
		kome::objects::PeaksCluster2D* cluster = peak->getCluster();

		int id = ( cluster == NULL ? 0 : clusterMap[ cluster ] );

		fprintf(
			fp,
			"%f\t%f\t%f\t%f\t%f\t%f\t%f\t%d\n",
			peak->getRt(),
			peak->getMz(),
			peak->getIntensity(),
			peak->getStartRt(),
			peak->getEndRt(),
			peak->getStartMz(),
			peak->getEndMz(),
			id
		);
	}

	// close
	fflush( fp );
	fclose( fp );

	return true;
}

// mgf ファイルの情報をセット
bool PeaksIOManager::setMgfFileInfo( const char* path ){
	m_point.clear();

	// file open
	FILE* fp = fileopen( path, "r" );
	if( fp == NULL ) {
		LOG_ERROR_CODE( FMT( "Failed to open the file. [%s]", path ), ERR_FILE_OPEN_FAILED );
		return false;
	}

	bool bflg = false;
	int specId = -1;

	// read file
	char buff[ 2048 ];
	while( fgets( buff, 2048, fp ) != NULL ) {
		// line
		std::string line = tolowercase( trimstring( buff ).c_str() );

		if( line.empty() ) {    // empty line
			bflg = false;
		}else{
			if( line.compare( "begin ions" ) == 0 ){
				bflg = true;
			}

			double x = -1.0;
			double y = -1.0;

			if( bflg ){
				if( line.compare( "title" ) != -1 ){
					std::vector< std::string > tokens;
					stringtoken( line.c_str(), ",", tokens );
										
					for( std::vector< std::string >::iterator it = tokens.begin(); it != tokens.end() && specId == -1; it++ ){
						std::string str = *it;
						// @date 2013.10.28 <Mod> M.Izumi ->
						if( str.find( "spec_id" ) != -1 ){
							std::vector< std::string >s;
							stringtoken( str.c_str(), ":", s );

							if( !s.empty() ){
								std::string strId = replacestring( s[1].c_str(), " ", "" );
								specId = toint( strId.c_str(), 10, -1 );
							}
						}	
						// @date 2013.10.28 <Mod> M.Izumi <-
					}
				}else if( line.compare("end ions") == 0 ){
					bflg = false;
					specId = -1;
				}

				if( specId >= 0 ){
					sscanf(
						buff,
						"%lf %lf",
						&x,
						&y
					);
					
					PeaksIOManager::peaks p;
					if( x >= 0.0 && y >= 0.0 ){
						p.x = x;
						p.y = y;
					
						m_point[ specId ].push_back( p ); // @date 2014.02.25 <Mod> M.Izumi
					
					}
				}
			}
		}
	}
	
	// close
	fclose( fp );

	return true;
}

// import chromatogram or spectrum peaks
bool PeaksIOManager::importPeaks( kome::objects::Peaks* peaks , const char* path ){
	// mgfファイルの時
	if( m_filter == 1 ){		
		setMgfFileInfo( path );
	}else{
		m_point.clear();
		
		// file open
		FILE* fp = fileopen( path, "r" );
		if( fp == NULL ) {
			LOG_ERROR_CODE( FMT( "Failed to open the file. [%s]", path ), ERR_FILE_OPEN_FAILED );
			return false;
		}

		bool bflg = false;	// Peak List .txt 用のフラグ
		int scanNum = -1;
		// read file
		char buff[ 2048 ];
		while( fgets( buff, 2048, fp ) != NULL ) {
			// line
			std::string line = tolowercase( trimstring( buff ).c_str() );

			if( line.empty() ) {    // empty line
				bflg = false;
				scanNum = -1;
			}else{
			
				if( line[ 0 ] == '#' ){
					// scan number取得
					std::vector< std::string > tmp;
					stringtoken( buff, " ", tmp );
				
					if( tmp.size() > 2 ){	// @date 2013.09.09 <Add> M.Izumi
						scanNum = toint( tmp[2].c_str(), 10, -1 );
					}
					bflg = true;
				}
				std::vector< std::string > tokens;
				stringtoken( buff, " \t,", tokens );
			
				double x = -1.0;
				double y = -1.0;
				double leftX = -1.0;
				double rightX = -1.0;
				double leftY = -1.0;
				double rightY = -1.0;
			
				if( bflg ){
										
					isdouble( tokens[0].c_str(), &x );
					isdouble( tokens[1].c_str(), &y );
					if( tokens.size() >= 7 ){	
						/*isdouble( tokens[2].c_str(), &leftX );
						isdouble( tokens[3].c_str(), &leftY ); 
						isdouble( tokens[6].c_str(), &rightX );
						isdouble( tokens[7].c_str(), &rightY );*/
					}else{
					
						sscanf(
							buff,
							"%lf %lf %lf %lf %lf %lf",
							&x,
							&y,
							&leftX,
							&rightX,
							&leftY,
							&rightY
						);
					}

					PeaksIOManager::peaks p;
					p.leftX = leftX;
					p.rightX = rightX;
					p.leftY = leftY;
					p.rightY = rightY;


					if( x >= 0.0 && y >= 0.0 ){
						p.x = x;
						p.y = y;
							
						m_point[ scanNum ].push_back( p );
					}					
					

					continue;
				}else{

					if( tokens.size() >= 7 ){
						isdouble( tokens[0].c_str(), &x );
						isdouble( tokens[1].c_str(), &y );
						isdouble( tokens[2].c_str(), &leftX );
						isdouble( tokens[3].c_str(), &leftY ); 
						isdouble( tokens[6].c_str(), &rightX );
						isdouble( tokens[7].c_str(), &rightY );

					}else{
						/*sscanf(
							buff,
							"%lf %lf %lf %lf %lf %lf",
							&x,
							&y,
							&leftX,
							&rightX,
							&leftY,
							&rightY
						);*/
					}
					
				}

				// add peak
				if( x >= 0.0 && y >= 0.0 && leftX >= 0.0 && rightX >= 0.0 && leftY >= 0.0 && rightY >= 0.0 ){
					kome::objects::PeakElement* peak = peaks->createPeak( x, y );
					peak->setLeft( leftX, leftY );
					peak->setRight( rightX, rightY );
				}

			}
		}
		
		// close
		fclose( fp );
	}

	return true;
}

// import 2d peaks
bool PeaksIOManager::import2dPeaks(
		kome::objects::Peaks2D* peaks,
		const char* path
) {
	// file open
	FILE* fp = fileopen( path, "r" );
	if( fp == NULL ) {
		LOG_ERROR_CODE( FMT( "Failed to open the file. [%s]", path ), ERR_FILE_OPEN_FAILED );
		return false;
	}

	// cluster map
	std::map< int, kome::objects::PeaksCluster2D* > clusterMap;

	// read file
	char buff[ 2048 ];
	bool clusterReading = false;
	while( fgets( buff, 2048, fp ) != NULL ) {
		// line
		std::string line = tolowercase( trimstring( buff ).c_str() );

		if( line.empty() ) {    // empty line
			clusterReading = false;
		}
		else {
			if( line[ 0 ] == '>' || line[ 0 ] == '#' ) {    // comment
				if( strstr( line.c_str(), "peak" ) != NULL ) {
					clusterReading = false;
				}
				else if( strstr( line.c_str(), "cluster" ) != NULL ) {
					clusterReading = true;
				}
			}
			else {    // peak or cluster
				if( clusterReading ) {    // clusters
					int id = 0;
					double rt = -1.0;
					double mz = -1.0;
					double intensity = -1.0;
					double startRt = -1.0;
					double endRt = -1.0;
					double startMz = -1.0;
					double endMz = -1.0;
					int charge = 0;

					sscanf(
						buff,
						"%d %lf %lf %lf %lf %lf %lf %lf %d",
						&id,
						&rt,
						&mz,
						&intensity,
						&startRt,
						&endRt,
						&startMz,
						&endMz,
						&charge
					);

					// add cluster
					if( rt >= 0.0 && mz >= 0.0 && intensity >= 0.0 ) {
						kome::objects::PeaksCluster2D* cluster = peaks->createCluster();
						cluster->setRt( rt );
						cluster->setMz( mz );
						cluster->setIntensity( intensity );
						cluster->setRtRange( startRt, endRt );
						cluster->setMzRange( startMz, endMz );
						cluster->setCharge( charge );

						clusterMap[ id ] = cluster;
					}
				}
				else {    // peaks
					std::vector< std::string > tokens;
					stringtoken( buff, " \t,", tokens );

					double rt = -1.0;
					double mz = -1.0;
					double intensity = -1.0;
					double startRt = -1.0;
					double endRt = -1.0;
					double startMz = -1.0;
					double endMz = -1.0;
					int clusterId = 0;

					if( tokens.size() >= 14 ) {
						double cRt = -1.0;
						double cMz = -1.0;
						double cInt = -1.0;
						double cStartRt = -1.0;
						double cEndRt = -1.0;
						double cStartMz = -1.0;
						double cEndMz = -1.0;

						sscanf(
							buff,
							"%lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf",
							&rt,
							&mz,
							&intensity,
							&startRt,
							&endRt,
							&startMz,
							&endMz,
							&cRt,
							&cMz,
							&cInt,
							&cStartRt,
							&cEndRt,
							&cStartMz,
							&cEndMz
						);

						// search cluster
						if( cRt >= 0.0 && cMz >= 0.0 && cInt >= 0.0 ) {
							kome::objects::PeaksCluster2D* cluster = NULL;

							for( unsigned int i = 0; i < peaks->getNumberOfClusters() && cluster == NULL; i++ ) {
								kome::objects::PeaksCluster2D* tmp = peaks->getCluster( i );

								if( fabs( tmp->getRt() - cRt ) < 0.01
										&& fabs( tmp->getMz() - cMz ) < 0.01
										&& fabs( tmp->getStartRt() - cStartRt ) < 0.01
										&& fabs( tmp->getEndRt() - cEndRt ) < 0.01
										&& fabs( tmp->getStartMz() - cStartMz ) < 0.01
										&& fabs( tmp->getEndMz() - cEndMz ) < 0.01
								) {
									cluster = tmp;
								}
							}

							if( cluster == NULL ) {
								cluster = peaks->createCluster();
								int id = (int)peaks->getNumberOfClusters();

								cluster->setRt( cRt );
								cluster->setMz( cMz );
								cluster->setIntensity( cInt );
								cluster->setRtRange( cStartRt, cEndRt );
								cluster->setMzRange( cStartMz, cEndMz );
							}
							else {
								for( std::map< int, kome::objects::PeaksCluster2D* >::iterator it = clusterMap.begin();
										it != clusterMap.end(); it++ ) {
									if( (*it).second == cluster ) {
										clusterId = (*it).first;
									}
								}
							}
						}
					}
					else if( tokens.size() >= 8 ) {
						sscanf(
							buff,
							"%lf %lf %lf %lf %lf %lf %lf %d",
							&rt,
							&mz,
							&intensity,
							&startRt,
							&endRt,
							&startMz,
							&endMz,
							&clusterId
						);
					}
					else if( tokens.size() >= 7 ) {
						sscanf(
							buff,
							"%lf %lf %lf %lf %lf %lf %lf",
							&rt,
							&mz,
							&intensity,
							&startRt,
							&endRt,
							&startMz,
							&endMz
						);
					}

					// add peak
					if( rt >= 0.0 && mz >= 0.0 && intensity >= 0.0 ) {
						kome::objects::Peak2DElement* peak
							= peaks->createPeak( rt, startRt, endRt, mz, startMz, endMz, intensity );

						kome::objects::PeaksCluster2D* cluster
							= ( clusterMap.find( clusterId ) == clusterMap.end() ? NULL : clusterMap[ clusterId ] );
						if( cluster != NULL ) {
							cluster->addPeak( peak );
						}
					}
				}
			}
		}
	}

	// close
	fclose( fp );

	return true;
}

// adds overlapping peaks
bool PeaksIOManager::addOverlapping2dPeaks( kome::objects::DataMapInfo* dataMap, const char* path ) {
	// peaks
	kome::objects::Peaks2D& peaks = m_overlapping2dMap[ dataMap ];
	peaks.clearClusters();
	peaks.clearPeaks();

	// import
	const bool ret = import2dPeaks( &peaks, path );

	// @date 2012/04/24 <Add> OKADA ------->	// SPEC 85657
	unsigned int unPeaksNum = peaks.getNumberOfPeaks();
	if( unPeaksNum <= 0 ){
		kome::window::WindowTool::showError( "Cannot import 2D peaks. The specified file does not contain peak information.\nSelect a file containing peak information." );
	}
	// @date 2012/04/24 <Add> OKADA <-------

	// timer
	if(( ret ) && ( 1 <= unPeaksNum ))	// @date 2012/04/24 <Add> OKADA
	{
		if( m_blink < 0 ) {
			m_blink = kome::window::WindowTool::setTimer( blinkOverlappingPeaks, 500, false );
		}
	}
	else {
		m_overlapping2dMap.erase( dataMap );
	}

	return ret;
}

// deletes overlapping 2D peaks
void PeaksIOManager::deleteOverlapping2dPeaks( kome::objects::DataMapInfo* dataMap ) { 
	// delete
	m_overlapping2dMap.erase( dataMap );

	// stop timer
	if( m_overlapping2dMap.size() == 0 && m_blink >= 0 ) {
		kome::window::WindowTool::killTimer( m_blink );
		m_blink = -1;
	}
}

// draws overlapping 2D peaks
void PeaksIOManager::drawOverlapping2dPeaks( kome::objects::DataMapInfo* dataMap, kome::img::Graphics& g ) {
	// check the peaks
	if( m_overlapping2dMap.find( dataMap ) == m_overlapping2dMap.end() ) {
		return;
	}

	// check the count
	if( m_blinkCnt % 3 == 2 ) {
		return;
	}

	// peaks
	kome::objects::Peaks2D& peaks = m_overlapping2dMap[ dataMap ];

	// set color
	COLOR oldColor = g.getColor();
	g.setColor( OVERLAPPING_COLOR );

	// draw peaks
	for( unsigned int i = 0; i < peaks.getNumberOfPeaks(); i++ ) {
		kome::objects::Peak2DElement* peak = peaks.getPeak( i );

		if( peak->getEndRt() >= g.getMinY()
				&& peak->getStartRt() <= g.getMaxY()
				&& peak->getEndMz() >= g.getMinX()
				&& peak->getStartMz() <= g.getMaxX()
		) {
			double srt = -1.0;
			double ert = -1.0;
			double smz = -1.0;
			double emz = -1.0;
			
			g.transformDataToPosition( peak->getStartMz(), peak->getStartRt(), &smz, &srt );
			g.transformDataToPosition( peak->getEndMz(), peak->getEndRt(), &emz, &ert );

			int psrt = roundnum( srt ) + 1;
			int pert = roundnum( ert ) - 1;
			int psmz = roundnum( smz ) - 1;
			int pemz = roundnum( emz ) + 1;

			int lines[] = {
				psmz, psrt,
				pemz, psrt,
				pemz, pert,
				psmz, pert,
				psmz, psrt
			};

			g.drawLines( lines, sizeof( lines ) / sizeof( int ) / 2 );
		}
	}

	// restore
	g.setColor( oldColor );
}


// show file open dialog
std::string PeaksIOManager::showFileOpenDialog( bool bflg ) {
	// file path
	std::string path;
	std::string strFormat;
	if( bflg ){
		strFormat = FMT( "Text Files (*.txt)|*.txt|Mascot Generic Format (*.mgf)|*.mgf" );
	}else{
		strFormat = FMT( "Text Files (*.txt)|*.txt" );
	}

	// file
	wxFileDialog dlg(
		kome::window::WindowTool::getMainWindow(),
		wxFileSelectorPromptStr,
		wxEmptyString,
		wxEmptyString,
		wxT( strFormat.c_str() ),
		wxOPEN | wxFILE_MUST_EXIST
	);

	if( dlg.ShowModal() == wxID_OK ) {
		if( bflg ){ 
			m_filter = -1;
			m_filter = dlg.GetFilterIndex();
		}
		path = dlg.GetPath().c_str();
	}

	return path;
}

// show file save dialog
std::string PeaksIOManager::showFileSaveDialog() {
	// file path
	std::string path;

	// file
	wxFileDialog dlg(
		kome::window::WindowTool::getMainWindow(),
		wxFileSelectorPromptStr,
		wxEmptyString,
		wxEmptyString,
		wxT( "Text Files (*.txt)|*.txt" ),
		wxSAVE | wxOVERWRITE_PROMPT
	);

	if( dlg.ShowModal() == wxID_OK ) {
		path = dlg.GetPath().c_str();
	}

	return path;
}

// blink overlapping peaks
void PeaksIOManager::blinkOverlappingPeaks() {
	// manager
	PeaksIOManager& mgr = getInstance();

	// each canvases
	for( unsigned int i = 0; i < kome::window::ChildFrame::getNumberOfFrames(); i++ ) {
		kome::window::ChildFrame* frame = kome::window::ChildFrame::getFrame( i );
		kome::objects::Variant var = frame->getActiveObject();
		kome::objects::MsDataVariant obj( var );
	
		if( mgr.m_overlapping2dMap.find( obj.getDataMap() ) != mgr.m_overlapping2dMap.end() ) {
			frame->Refresh();
		}
	}

	// blink count
	mgr.m_blinkCnt = ( mgr.m_blinkCnt + 1 ) % 3;
}

// get instance
PeaksIOManager& PeaksIOManager::getInstance() { 
	// create object (This is the only object.)
	static PeaksIOManager mgr;

	return mgr;
}

// get number of points 
int PeaksIOManager::getNumberOfPoints(){
	return m_point.size();
}

 // import spec peaks 
bool PeaksIOManager::importSpecPeaks(kome::objects::Spectrum* spec){
	bool ret = false;

	kome::objects::PeaksManager& pkMgr = kome::objects::PeaksManager::getInstance();
	// file path
	std::string path = showFileOpenDialog( true );
	
	if( spec == NULL ){
		LOG_ERROR( FMT( "Failed to get an active spectrum." ) );
		return ret;
	}
	
	// spectrum
	wxBeginBusyCursor();
		
	kome::objects::Peaks* peaks = pkMgr.getPeaks( spec );
	if( peaks == NULL ){
		peaks = pkMgr.createPeaks( spec );
	}

	if( path.empty() ){
		ret = true;
	}else{
	
		ret = importPeaks( peaks, path.c_str() );
	
		int inum = getNumberOfPoints();
		if( inum >= 1 ){
			kome::objects::ActiveObjectsManager& mgr = kome::objects::ActiveObjectsManager::getInstance();
			kome::objects::Sample* sample = mgr.getActiveSample();
			if( sample == NULL ){
				return ret;
			}

			// data set	
			kome::objects::DataGroupNode* root = sample->getRootDataGroupNode();
			kome::objects::DataSet ds( root );
			root->getDataSet( &ds );

			for( unsigned int i=0; i < ds.getNumberOfSpectra(); i++ ){
				kome::objects::Spectrum* spec = ds.getSpectrum( i );
				if( spec != NULL ){
					for( std::map< int, std::vector< PeaksIOManager::peaks > >::iterator it = m_point.begin(); 
						it != m_point.end(); it++ ){
						int specId = it->first;
						int id = ( spec->getOrgSpectrum() != NULL ? spec->getOrgSpectrum()->getId() : spec->getId() );
						
						// @date 2013.10.18 <Mod> M.Izumi
						if( specId == id ){
							peaks = pkMgr.getPeaks( spec );
							if( peaks == NULL ){
								peaks = pkMgr.createPeaks( spec );
							}
														
							std::vector< PeaksIOManager::peaks > p = it->second;

							for( std::vector< PeaksIOManager::peaks >::iterator xypoint = p.begin();
								xypoint != p.end(); xypoint++ ){

									PeaksIOManager::peaks tmp = *xypoint;

									double x = tmp.x;
									double y = tmp.y;

									kome::objects::PeakElement* peak = peaks->createPeak( x, y );
									if( tmp.leftX >= 0.0 && tmp.rightX >= 0.0 && tmp.leftY >= 0.0 && tmp.rightY >= 0.0 ){
										peak->setLeft( tmp.leftX, tmp.leftY );
										peak->setRight( tmp.rightX, tmp.rightY );
									}
							}

							kome::core::DataPoints dps;
							spec->getXYData( &dps, false );
							peaks->setDataPoints( dps );
							
							kome::plugin::PluginCallTool::onUpdateSpecPeaks( *spec );
							kome::plugin::PluginCallTool::onUpdateSpectrum( *spec );
						}
					}
				}
			}
		
		}else{

			kome::core::DataPoints dps;
			spec->getXYData( &dps, false );
			peaks->setDataPoints( dps );
			kome::plugin::PluginCallTool::onUpdateSpecPeaks( *spec );
			kome::plugin::PluginCallTool::onUpdateSpectrum( *spec );
		
		}
		
		ret = true;
		kome::window::WindowTool::refresh();
			
	}
	wxEndBusyCursor();

	return ret;
}

bool PeaksIOManager::importChormPeaks( kome::objects::Chromatogram* chrom ){
	bool ret = false;

	kome::objects::PeaksManager& pkMgr = kome::objects::PeaksManager::getInstance();

	// file path
	std::string path = showFileOpenDialog();

	if( chrom == NULL ){
		LOG_ERROR( FMT( "Failed to get an active chromatogram." ) );
		return ret;
	}

	// chromatogram
	wxBeginBusyCursor();
		
	kome::objects::Peaks* peaks = pkMgr.getPeaks( chrom );
	if( peaks == NULL ){
		peaks = pkMgr.createPeaks( chrom );
	}

	if( path.empty() ){
		ret = true;
	}else{
		ret = importPeaks( peaks, path.c_str() );
		
		kome::core::DataPoints dps;
		chrom->getXYData( &dps, false );
		peaks->setDataPoints( dps );

		kome::plugin::PluginCallTool::onUpdateChromPeaks( *chrom );

		kome::window::WindowTool::refresh();
		
	}
	wxEndBusyCursor();

	return ret;
}

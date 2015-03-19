/**
 * @file MgfSample.cpp
 * @brief interfaces of MgfSample class
 *
 * @author M.Izumi
 * @date 2013.07.18
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#include "stdafx.h"
#include "MgfSampleSet.h"
#include "MgfSample.h"
#include "MgfSpectrum.h"
   
using namespace kome::ident;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


// constructor
MgfSample::MgfSample( MgfSampleSet* sampleSet ) : kome::objects::Sample( sampleSet ){
	m_sampleSet = sampleSet;
}

// destructor
MgfSample::~MgfSample(){
}

// on open sample
bool MgfSample::onOpenSample( kome::objects::DataGroupNode* rootGroup, kome::core::Progress* progress ){
	// get file
	FILE* fp = ( m_sampleSet == NULL ? NULL : m_sampleSet->getFile() );
	if( fp == NULL ) {
		const char* path = m_sampleSet->getFilePath();
		if( m_sampleSet->openFile(path) ){
			fp = m_sampleSet->getFile();
			if( fp == NULL ){
				LOG_ERROR_CODE( FMT( "Failed to open the sample. [%s]", path ), ERR_FILE_OPEN_FAILED );
				return false;
			}
		}
	}

	// manager
	kome::ident::IdentificationManager& identMgr = kome::ident::IdentificationManager::getInstance();
	kome::core::MsppManager& msppMgr = kome::core::MsppManager::getInstance();

	bool bflg = false;		
	
	std::string strPath;	
	std::vector< std::pair< double, double > >  points;

	MgfSpectrum* spec = NULL;

// >>>>>>	@Date:2013/11/11	<Add>	A.Ozaki
//
	// Mgf File Folder
	std::string folderName = "MgfFile";
	std::string folder = getpath( msppMgr.getTmpDir( ), folderName.c_str( ) );
	
	if  ( !fileexists( folder.c_str( ) ) )
	{
		makedirectory( folder.c_str( ) );
	}

	std::string strCommonName;

	// time
	time_t	now = time( NULL );
	struct tm*	date = localtime( &now );
	std::string	dateStr = datetostring( date );

	strCommonName = FMT( "_%s", dateStr.c_str( ) );

//
// @Date:2013/11/11	<Add>	A.Ozaki

	// read file
	char buff[ 2048 ];
	int scanNum = 0;
	
	int charge;
	int tmpScanNum;
	kome::ident::SpecInfo info;

	while( fgets( buff, 2048, fp ) != NULL ) {
		
		// line
		std::string line = tolowercase( trimstring( buff ).c_str() );

		if( line.empty() ) {    // empty line
			bflg = false;
		}else{
			if( line.compare( "begin ions" ) == 0 ){
				bflg = true;
				spec = new MgfSpectrum( dynamic_cast< MgfSample* >( rootGroup->getSample() ), "" );
				rootGroup->addSpectrum( spec );	
				
				// default		
				charge = -1;
				tmpScanNum = -1;
		
				info.charges.clear();
				info.id = 0;
				info.name = "";
				info.path = "";
				info.peakIds.clear();
				info.polarity;
				info.retentionTime = 0.0;
				info.sampleIndex = 0;
				info.sampleName = "";
				info.specId = 0;
				info.stage = 2;

				spec->setCentroidMode( true );
				spec->setAutoZeroPoints( true );
				spec->setName( info.name.c_str() );
				spec->setRt( info.retentionTime );
				spec->setTitle( info.name.c_str() );
				spec->setMsStage( info.stage );
				spec->setPolarity( info.polarity );
				spec->setGroup( rootGroup );	
				spec->setHasChromatogram( info.retentionTime >= 0.0 );
				rootGroup->getSample()->setSampleIndex( info.sampleIndex );
			}
						
			double x = -1.0;
			double y = -1.0;

			if( bflg ){				

				if( line.compare( "title" ) != -1 ){
					line = line.substr( line.find( "=" ) + 1 ).c_str();	
										
					if( !identMgr.ParseSpectrumTitle( line.c_str(), info, &charge ) ){
						spec->setTitle( line.c_str() );											
					} else {
						spec->setId( info.specId );
						spec->setName( info.name.c_str() );
						spec->setRt( info.retentionTime );
						spec->setTitle( info.name.c_str() );
						spec->setMsStage( info.stage );
						spec->setPolarity( info.polarity );
						spec->setGroup( rootGroup );	
						spec->setHasChromatogram( info.retentionTime >= 0.0 );
						rootGroup->getSample()->setSampleIndex( info.sampleIndex );
					}
				
				}else if( line.compare( "end ions" ) == 0 ){
					bflg = false;					
// >>>>>>	@Date:2013/11/11	<Modified>	A.Ozaki
//
					if( tmpScanNum == -1 ){
						spec->setScanNumber( scanNum );
					}

					std::string strName;

					strName.clear( );
					if( info.name.empty() ){
						strName.append( FMT("Scan <%d> (Precursor=<%f>, Charge=<%d>", spec->getScanNumber(), spec->getPrecursor(), charge ) );
					}else{
						int iFind = info.name.find( " " );
						if  ( iFind != -1 )
						{
							info.name = info.name.substr( iFind+1 ).c_str( );
						}
						strName.append( info.name.c_str( ) );
					}

					spec->setName( strName.c_str() );

					strName.append( strCommonName.c_str( ) );

					std::string tmpFile = msppMgr.getTmpFileName( strName.c_str( ), ".tmp", folder.c_str( ) );
					
					strPath = getpath( folder.c_str( ), tmpFile.c_str( ) );
					spec->setPointPath( strPath.c_str( ) );

					if( charge > 0 ) {
						spec->setPrecursorCharge( charge );
					}
//
// <<<<<<	@Date:2013/11/11	<Modified>	A.Ozaki
					writePoint( strPath.c_str(), points );
					points.clear();
					scanNum++;
					
				}else if( line.find( "pepmass" ) != -1 ){
					int iFind = line.find( "=" );
					if( iFind != -1 ){
						line = line.substr( iFind+1 );
					}
					double precusor = todouble( line.c_str(), 0.0 );
					spec->setPrecursor( precusor );

				}else if ( line.find( "charge" ) != -1 ) {
					int iFind = line.find( "=" );
					if( iFind != -1 ){
						line = line.substr( iFind+1 );
					}
					charge = atol( line.c_str() );
				}else{ // peak 

					sscanf(	buff, "%lf %lf", &x, &y	);
					
					if( x >= 0.0 && y >= 0.0 ){
					
						std::pair< double, double > p;
						p.first = x;
						p.second = y;
						points.push_back( p );

						spec->addPoint( x, y );
					}
				}	
			}	
		}
	}

	fflush( fp );
	fclose( fp );
	
	return true;
}

// on close sample
bool MgfSample::onCloseSample(){
	kome::objects::DataGroupNode* group = getRootDataGroupNode();
	if( group != NULL ){
		int inum = group->getNumberOfSpectra()-1;
		
		while( inum > -1 ){
			kome::objects::Spectrum* spec = group->getSpectrum( inum );
			if( spec != NULL ){
				delete spec;
				group->removeSpectrum( spec );
			}

			inum--;
		}
	}
	return true;
}

// write point
bool MgfSample::writePoint( const char* path, std::vector< std::pair< double, double > > points ){
	// open the file
	FILE* fp = fileopen( path, "w" );
	if( fp == NULL ) {
		LOG_ERROR_CODE( FMT( "Failed to open the file. [%s]", path ), ERR_FILE_OPEN_FAILED );

		return false;
	}

	for( std::vector< std::pair< double, double > >::iterator it = points.begin(); 
		it != points.end(); it++ ){
		double x = it->first;
		double y = it->second;

		fprintf( fp, "%f,%f\n", x, y );
	}

	// close
	fflush( fp );
	fclose( fp );

	return true;
}

/**
 * @file MzXmlManager.cpp
 * @brief implements of MzXmlManager class
 *
 * @author S.Tanaka
 * @date 2007.03.27
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "MzXmlManager.h"

#include "ChromPointSpectrum.h"

#include <vector>
#include <set>
#include <algorithm>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>

#define DATA_STATUS		     "data_status"

using namespace kome::io::mzxml;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
MzXmlManager::MzXmlManager() {
	init();
}

// destructor
MzXmlManager::~MzXmlManager() {
}

// initialize
void MzXmlManager::init() {
	m_scanNumberMap.clear();
}

// export data
bool MzXmlManager::exportDataSet( 
		const char* path,
		kome::objects::DataSet& dataSet,
		kome::core::Progress& progress,
		kome::objects::SettingParameterValues* saveSettings  // @date 2012/08/21 <Add> FUJITA
) {	
	// check spectra
	if( dataSet.getNumberOfSpectra() == 0 &&  dataSet.getNumberOfChromatograms() == 0 ) {
		LOG_ERROR_CODE( FMT( "There is no data to be exported. Open a data file first." ), ERR_OTHER );
		return false;
	}

	// open the file
	FILE* fp = fileopen( path, "wb" );
	if( fp == NULL ) {
		LOG_ERROR_CODE( FMT( "Failed to open the file for writing. Check file / folder permissions. [%s]", path ), ERR_FILE_OPEN_FAILED );
		return false;
	}

	// initialize
	init();

	// export
	exportDataSet( fp, dataSet, progress, saveSettings );

	// close
	fflush( fp );
	fclose( fp );

	// part of the way
	return true;
}

// export data
void MzXmlManager::exportDataSet( 
		FILE* fp,
		kome::objects::DataSet& dataSet,
		kome::core::Progress& progress,
		kome::objects::SettingParameterValues* saveSettings		// @date 2012/08/21 <Add> FUJITA
) {
	// get the array
	std::vector< kome::objects::Spectrum* > specArr;
	specArr.reserve( dataSet.getNumberOfSpectra() );
	for( unsigned int i = 0; i < dataSet.getNumberOfSpectra(); i++ ) {
		// spectrum
		specArr.push_back( dataSet.getSpectrum( i ) );
	}
	std::sort( specArr.begin(), specArr.end(), lessSpec );

	// set range
	progress.setRange( 0, specArr.size() + 4 );
	progress.setPosition( 0 );
	if( progress.isStopped() ) {
		return;
	}

	// write header
	progress.setStatus( "Writing header..." );
	writeHeader( fp );
	progress.setPosition( 1 );
	if( progress.isStopped() ) {
		return;
	}

	// write file data
	progress.setStatus( "Writing sample file information..." );
	writeFileData( fp, specArr );
	progress.setPosition( 2 );
	if( progress.isStopped() ) {
		return;
	}

	// write software information
	progress.setStatus( "Writing software information..." );
	writeSoftwareInfo( fp );
	progress.setPosition( 3 );
	if( progress.isStopped() ) {
		return;
	}

	// write spectrum data
	std::vector< unsigned long long > offsets;
	offsets.reserve( specArr.size() );
	for( unsigned int i = 0; i < specArr.size(); i++ ) {
		std::string status = FMT(
			"Writing spectrum data. (%s) [%d/%d]",
			specArr[ i ]->getName(),
			( i + 1 ),
			specArr.size()
		);
		progress.setStatus( status.c_str() );

		offsets.push_back( filetell( fp ) );

		unsigned int scanNumber = i + 1;
		m_scanNumberMap[ specArr[ i ] ] = scanNumber;
		writeSpectrum( fp, *( specArr[ i ] ), saveSettings );

		progress.setPosition( 4 + i );
		if( progress.isStopped() ) {
			return;
		}
	}

	// end msRun tag
	fprintf( fp, "  </msRun>\n" );

	// write index
	progress.setStatus( "Writing offsets..." );
	writeIndex( fp, offsets );

	// end tag
	fprintf( fp, "</mzXML>\n" );
	progress.setPosition( 4 + specArr.size() );
	if( progress.isStopped() ) {
		return;
	}

}

// write header
void MzXmlManager::writeHeader( FILE* fp ) {
	fprintf( fp, "%s\n", "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>" );
	fprintf( fp, "%s\n", "<mzXML" );
	fprintf( fp, "%s\n", "xmlns=\"http://sashimi.sourceforge.net/schema_revision/mzXML_2.0\"" );
	fprintf( fp, "%s\n", "xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\"" );
	fprintf( fp, "%s\n", "xsi:schemaLocation=\"http://sashimi.sourceforge.net/schema_revision/mzXML_2.0 http://sashimi.sourceforge.net/schema_revision/mzXML_2.0/mzXML_idx_2.0.xsd\">" );
}

// write file data
void MzXmlManager::writeFileData( FILE* fp, std::vector< kome::objects::Spectrum* >& spectra ) {
	// manager
	kome::objects::PointsManager& ptMgr = kome::objects::PointsManager::getInstance();

	std::vector< kome::objects::Sample* > sampleArr;
	std::map< kome::objects::Sample*, bool > processedMap;
	std::set< kome::objects::Sample* > sampleSet;
	for( unsigned int i = 0; i < spectra.size(); i++ ) {
		// spectrum
		kome::objects::Spectrum* spec = spectra[ i ];

		// sample
		kome::objects::Sample* sample = spectra[ i ]->getSample();
		if( sampleSet.find( sample ) == sampleSet.end() ) {
			sampleArr.push_back( sample );
			sampleSet.insert( sample );

			processedMap[ sample ] = false;
		}

		// check processed
		processedMap[ sample ] = ( ptMgr.getNumberOfOperations( spec ) > 0 );
	}

	// msRun
	fprintf(
		fp,
		"  <msRun scanCount=\"%d\" startTime=\"PT%fS\" endTime=\"PT%fS\">\n",
		spectra.size(),
		( spectra.front()->getStartRt() * 60.0 ),
		( spectra.back()->getEndRt() * 60.0 )
	);

	// host name
	char hostName[ 1024 ];
	gethostname( hostName, 1024 );
	hostName[ 1023 ] = '\0';

	// parent files
	for( unsigned int i = 0; i < sampleArr.size(); i++ ) {
		// sample
		kome::objects::Sample* sample = sampleArr[ i ];
		kome::objects::SampleSet* sampleSet = sample->getSampleSet();

		// get URI
		std::string uri = FMT( "file://%s", hostName );

		boost::filesystem::path p( sampleSet->getFilePath(), boost::filesystem::native );
		p = boost::filesystem::absolute( p );
		std::string s = p.string();

		if( s.empty() || s.at( 0 ) != '/' ) {
			uri.append( "/" );
		}
		uri.append( s );


		// file type
		std::string type = "RAWData";
		if( processedMap[ sample ] ) {
			type = "processedData";
		}

		// get file list
		std::vector< std::string > pathArr;
		if( boost::filesystem::is_directory( p ) ) {
			boost::filesystem::directory_iterator end;
			for( boost::filesystem::directory_iterator it( p );
					it != end; it++ ) {
				std::string fileName = (*it).path().leaf().string();
				std::string child = getpath( sampleSet->getFilePath(), fileName.c_str() );

				if( !isdirectory( child.c_str() ) ) {
					pathArr.push_back( child );
				}
			}
		}
		else {
			pathArr.push_back( sampleSet->getFilePath() );
		}

		// write parent file tag
		for( unsigned int j = 0; j < pathArr.size(); j++ ) {
			// sha1
			std::string sha1 = kome::core::Sha1::transformFile( pathArr[ j ].c_str() );

			// write
			fprintf( fp, "	<parentFile fileName=\"%s\"\n", kome::xml::Xml::text2xml( uri.c_str() ).c_str() );
			fprintf( fp, "				fileType=\"%s\"\n", kome::xml::Xml::text2xml( type.c_str() ).c_str() );
			fprintf( fp, "				fileSha1=\"%s\" />\n", kome::xml::Xml::text2xml( sha1.c_str() ).c_str() );
		}
	}
}

// write software information
void MzXmlManager::writeSoftwareInfo( FILE* fp ) {
	// plug-in
	const char* pluginName = "MzXML IO";

	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();
	kome::plugin::PluginInfo* plugin = plgMgr.getPlugin( pluginName );
	std::string version = "0.0";
	if( plugin != NULL ) {
		version = plugin->getVersion();
	}

	// get
	fprintf( fp, "	<dataProcessing>\n" );
	fprintf( fp, "	  <software type=\"conversion\"\n" );
	fprintf( fp, "				name=\"Mass++ %s plug-in\"\n", kome::xml::Xml::text2xml( pluginName ).c_str() );
	fprintf( fp, "				version=\"%s\" />\n", kome::xml::Xml::text2xml( version.c_str() ).c_str() );
	fprintf( fp, "	</dataProcessing>\n" );
}

// write spectrum data
void MzXmlManager::writeSpectrum(
			FILE* fp,
			kome::objects::Spectrum& spec,
			kome::objects::SettingParameterValues* saveSettings
) {

	// get data status
	bool op = saveSettings->getBoolValue( DATA_STATUS,false );

	// get datapoints
	kome::core::DataPoints pts( kome::core::DataPoints::FLOAT );
	kome::core::DataPoints tmpPts;
	spec.getXYData( &tmpPts, op );

	for( unsigned int i = 0; i < tmpPts.getLength() ;i++ ) {
		const double x = tmpPts.getX( i );
		const double y = tmpPts.getY( i );
		if( y > 0.0 ) {
			pts.addPoint( x, y );
		}		
	}

	// get ID
	unsigned int id = m_scanNumberMap[ &spec ];

	// write scan tag
	fprintf( fp, "	<scan num=\"%d\"\n", id );
	fprintf( fp, "		  msLevel=\"%d\"\n", spec.getMsStage() );
	fprintf( fp, "		  peaksCount=\"%d\"\n", pts.getLength() );
	if( spec.getPolarity() != kome::objects::Spectrum::POLARITY_UNKNOWN ) {
		fprintf(
			fp,
			"		  polarity=\"%s\"\n",
			( spec.getPolarity() == kome::objects::Spectrum::POLARITY_NEGATIVE ) ? "-" : "+"
		);
	}
	if( spec.getRt() >= 0.0 ) {
		fprintf( fp, "		  retentionTime=\"PT%fS\"\n", ( spec.getRt() * 60.0 ) );
	}
	fprintf( fp, "		  centroided=\"%s\"\n", spec.isCentroidMode() ? "true" : "false" );
	fprintf( fp, "		  lowMz=\"%f\"\n", spec.getMinX() );
	fprintf( fp, "		  highMz=\"%f\"\n", spec.getMaxX() );
	fprintf( fp, "		  basePeakMz=\"%f\"\n", spec.getBasePeakMass() );
	fprintf( fp, "		  basePeakIntensity=\"%f\"\n", spec.getMaxIntensity() );
	fprintf( fp, "		  totIonCurrent=\"%f\">\n", spec.getTotalIntensity() );

	// write precursor
	if( spec.getPrecursor() >= 0.0 ) {
		// get parent spectrum
		unsigned int parentId = 0;
		kome::objects::Spectrum* parent = spec.getParentSpectrum();
		if( parent != NULL && m_scanNumberMap.find( parent ) != m_scanNumberMap.end() ) {
			parentId = m_scanNumberMap[ parent ];
		}

		// write precursor
		if( parentId > 0 ) {
			fprintf(
				fp,
				"	  <precursorMz precursorScanNum=\"%d\" precursorIntensity=\"%f\">%f</precursorMz>\n",
				parentId,
				spec.getPrecursorIntensity(),
				spec.getPrecursor()
			);
		}
		else {
			fprintf(
				fp,
				"	  <precursorMz precursorIntensity=\"%f\">%f</precursorMz>\n",
				spec.getPrecursorIntensity(),
				spec.getPrecursor()
			);
		}
	}

	// >>>>>>	@Date:2013/08/19	<Modify>	A.Ozaki
	//
	std::string	strSpotId;
	strSpotId.clear( );
	if  ( (char *)NULL != spec.getSpotId( ) )
	{
		strSpotId = FMT( "%s", spec.getSpotId( ) );
	}

	if  ( 0 < strSpotId.length( ) )
	{
		fprintf(
			fp,
			"	  <maldi plateID=\"1\" spotID=\"%s\">\n",
			strSpotId.c_str( )
		);
	}
	//
	// <<<<<<	@Date:2013/08/19	<Modify>	A.Ozaki

	// write peaks tag
	fprintf( fp, "	  <peaks precision=\"32\"\n" );
	fprintf( fp, "			 byteOrder=\"network\"\n" );
	fprintf( fp, "			 pairOrder=\"m/z-int\">" );

	// write data
	if( pts.getLength() > 0 ) {
		// create data array
		unsigned long length = pts.getLength() * 2;
		float* mzIntArr = new float[ length ];

		for( unsigned int i = 0; i < pts.getLength(); i++ ) {
			mzIntArr[ i * 2 ] = (float)pts.getX( i );
			mzIntArr[ i * 2 + 1 ] = (float)pts.getY( i );
		}

		if( islittleendian() ) {
			for( unsigned int i = 0; i < length; i++ ) {
				memreverse( mzIntArr + i, sizeof( float ) );
			}
		}

		// convert data
		unsigned long buffLength = length * sizeof( float ) * 2;
		char* base64Buff = new char[ buffLength ];

		long size = kome::core::Base64::encode( mzIntArr, length * sizeof( float ), base64Buff, buffLength );
		if( size < 0 ) {
			LOG_WARN( FMT( "Failed to encode data by base64." ) );
		}
		else {
			char* pos = base64Buff;
			while( size > 0 ) {
				int writeSize = MIN( size, 1024 );
				writeSize = fwrite( pos, 1, writeSize, fp );
				size -= writeSize;
				pos += writeSize;
			}
		}

		// delete array
		delete[] base64Buff;
		delete[] mzIntArr;
	}

	// end tag
	fprintf( fp, "</peaks>\n" );

	// >>>>>>	@Date:2013/08/20	<Add>	A.Ozaki
	if  ( 0 < strSpotId.length( ) )
	{
		fprintf( fp, "	</maldi>\n" );
	}
	// <<<<<<	@Date:2013/08/20	<Add>	A.Ozaki
	fprintf( fp, "	</scan>\n" );
}

// write index
void MzXmlManager::writeIndex( FILE* fp, std::vector< unsigned long long >& offsets ) {
	// get index offset
	unsigned long long indexOffset = filetell( fp );

	// write index tag
	fprintf( fp, "  <index name=\"scan\">\n" );

	// write offsets
	for( unsigned int i = 0; i < offsets.size(); i++ ) {
		// ID
		unsigned long id = i + 1;

		// write
		fprintf( fp, "	<offset id=\"%d\">%lld</offset>\n", id, offsets[ i ] );
	}

	// end index tag
	fprintf( fp, "  </index>\n" );

	// index offset tag
	fprintf( fp, "  <indexOffset>%lld</indexOffset>\n", indexOffset );
}

// compare to sort
bool MzXmlManager::lessSpec( kome::objects::Spectrum* spec0, kome::objects::Spectrum* spec1 ) {
	return ( spec0->getRt() < spec1->getRt() );
}

// get instance
MzXmlManager& MzXmlManager::getInstance() { 
	// create object (This is the only object.)
	static MzXmlManager mgr;

	return mgr;
}

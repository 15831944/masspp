/**
 * @file MzmlManager.cpp
 * @brief implements of MzmlManager class
 *
 * @author S.Tanaka
 * @date 2009.03.09
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "MzmlManager.h"

#include "Accession.h"
#include "AccessionManager.h"

#include <float.h>

#include <vector>
#include <set>
#include <algorithm>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>


#ifdef _MSC_VER
	#include <Winsock2.h>
#else
	#include <unistd.h>
#endif


using namespace kome::io::mzml;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



#define SOFTWARE_ACC		 "MS:1000531"
#define MACHINE_ACC		     "MS:1000031"

#define DATA_STATUS		     "data_status"

// #define MZML_USE_REFERENCEABLE_PARAM_GROUP


// constructor
MzmlManager::MzmlManager() : m_reverseFlg( isbigendian() ) {
	// initialize
	init();
}

// destructor
MzmlManager::~MzmlManager() {
}

// initialize variables
void MzmlManager::init() {
	m_specIdxMap.clear();
	m_chromIdxMap.clear();
}

// export data
bool MzmlManager::exportDataSet(
		const char* path,
		kome::objects::DataSet& dataSet,
		kome::core::Progress& progress,
		kome::objects::SettingParameterValues* saveSettings		// @date 2012/08/21 <Add> FUJITA
) {	
	// initialize
	init();

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

	// export
	exportDataSet( fp, dataSet, progress, saveSettings );

	// close
	fflush( fp );
	fclose( fp );

	return true;
}

// export data
void MzmlManager::exportDataSet(
		FILE* fp,
		kome::objects::DataSet& dataSet,
		kome::core::Progress& progress,
		kome::objects::SettingParameterValues* saveSettings
) {
	// data array
	std::vector< kome::objects::Spectrum* > spectra;
	std::vector< kome::objects::Chromatogram* > chroms;
	std::vector< kome::objects::Sample* > samples;
	std::set< kome::objects::Sample* > sampleSet;

	for( unsigned int i = 0; i < dataSet.getNumberOfSpectra(); i++ ) {
		kome::objects::Spectrum* spec = dataSet.getSpectrum( i );
		spectra.push_back( spec );

		kome::objects::Sample* smpl = spec->getSample();
		if( smpl != NULL && sampleSet.find( smpl ) == sampleSet.end() ) {
			sampleSet.insert( smpl );
			samples.push_back( smpl );
		}
	}

	for( unsigned int i = 0; i < dataSet.getNumberOfChromatograms(); i++ ) {
		kome::objects::Chromatogram* chrom = dataSet.getChromatogram( i );
		if( !chrom->isAutoCreated() ) {
			chroms.push_back( chrom );
		}

		kome::objects::Sample* smpl = chrom->getSample();
		if( smpl != NULL && sampleSet.find( smpl ) == sampleSet.end() ) {
			sampleSet.insert( smpl );
			samples.push_back( smpl );
		}
	}

	// add sort  @date 2011.10.31 <Add> M.Izumi
	std::sort( spectra.begin(), spectra.end(), lessSpec );
	
	// progress
	const bool specFlg = ( dataSet.getNumberOfSpectra() > 0 );
	const bool chromFlg = ( dataSet.getNumberOfChromatograms() > 0 );

	unsigned int subPrgsNum = 1;
	if( specFlg ) {
		subPrgsNum++;
	}
	if( chromFlg ) {
		subPrgsNum++;
	}

	progress.createSubProgresses( subPrgsNum );

	kome::core::Progress* mainPrgs = progress.getSubProgress( 0 );
	kome::core::Progress* specPrgs = ( specFlg ? progress.getSubProgress( 1 ) : NULL );
	kome::core::Progress* chromPrgs = ( chromFlg ? progress.getSubProgress( subPrgsNum - 1 ) : NULL );

	mainPrgs->setRange( 0, 7 );

	// header
	mainPrgs->setPosition( 0 );
	progress.setStatus( "Writing Header..." );
	writeHeader( fp );
	mainPrgs->setPosition( 1 );

	// file description
	writeFileDescription( fp, samples );
	mainPrgs->setPosition( 2 );

	// referenceable param group list
	writeReferenceableParamGroup( fp, ( spectra.size() > 0 ), ( chroms.size() > 0 ) );
	mainPrgs->setPosition( 3 );

	// software listsssss
	writeSoftwareList( fp, samples );
	mainPrgs->setPosition( 4 );

	// instrument list
	writeInstrumentList( fp, samples );
	mainPrgs->setPosition( 5 );

	// processing list
	writeProcessingList( fp );
	mainPrgs->setPosition( 6 );

	// run tag
	writeRun( fp, samples, spectra, chroms, specPrgs, chromPrgs, saveSettings ); 
	if( progress.isStopped() ) {
		return;
	}

	if( specPrgs != NULL ) {
		specPrgs->fill();
	}
	if( chromPrgs != NULL ) {
		chromPrgs->fill();
	}

	// index list
	writeIndexList( fp, spectra, chroms );

	mainPrgs->setPosition( 7 );
	mainPrgs->fill();
	progress.fill();
}

// write header
void MzmlManager::writeHeader( FILE* fp ) {
	fprintf( fp, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n" );
	fprintf( fp, "<indexedmzML xmlns=\"http://psi.hupo.org/ms/mzml\"\n" );
	fprintf( fp, "			 xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\"\n" );
	fprintf( fp, "			 xsi:schemaLocation=\"http://psi.hupo.org/ms/mzml http://psidev.info/files/ms/mzML/xsd/mzML1.1.0_idx.xsd\">\n" );
	fprintf( fp, "  <mzML xmlns=\"http://psi.hupo.org/ms/mzml\"\n" );
	fprintf( fp, "		xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\"\n" );
	fprintf( fp, "		xsi:schemaLocation=\"http://psi.hupo.org/ms/mzml http://psidev.info/files/ms/mzML/xsd/mzML1.1.0.xsd\"\n" );
	fprintf( fp, "		version=\"1.1.0\">\n" );
	fprintf( fp, "	<cvList count=\"2\">\n" );
	fprintf( fp, "	  <cv id=\"MS\"\n" );
	fprintf( fp, "		  fullName=\"Proteomics Standards Initiative Mass Spectrometry Ontology\"\n" );
	fprintf( fp, "		  version=\"1.3.1\"\n" );
	fprintf( fp, "		  URI=\"http://psidev.info/ms/mzML/psi-ms.obo\" />\n" );
	fprintf( fp, "	  <cv id=\"UO\"\n" );
	fprintf( fp, "		  fullName=\"Unit Ontology\"\n" );
	fprintf( fp, "		  version=\"1.15\"\n" );
	fprintf( fp, "		  URI=\"http://obo.cvs.sourceforge.net/obo/obo/ontology/phenotype/unit.obo\" />\n" );
	fprintf( fp, "	</cvList>\n" );
}

// write file description
void MzmlManager::writeFileDescription( FILE* fp, std::vector< kome::objects::Sample* >& samples ) {
	// start tag
	fprintf( fp, "	<fileDescription>\n" );

	writeFileContent( fp, samples );
	writeSourceFileList( fp, samples );

	// end tag
	fprintf( fp, "	</fileDescription>\n" );

}

// write file content
void MzmlManager::writeFileContent( FILE* fp, std::vector< kome::objects::Sample* >& samples ) {
	// start tag
	fprintf( fp, "	  <fileContent>\n" );

	// file content
	fprintf( fp, "		<cvParam cvRef=\"MS\" accession=\"MS:1000580\" name=\"MSn spectrum\" value=\"\"/>\n" );

	// end tag
	fprintf( fp, "	  </fileContent>\n" );
}

// write source file list
void MzmlManager::writeSourceFileList( FILE* fp, std::vector< kome::objects::Sample* >& samples ) {
	// manager
	AccessionManager& accMgr = AccessionManager::getInstance();

	// start tag
	fprintf( fp, "	  <sourceFileList>\n" );

	// each samples
	std::set< kome::objects::SampleSet* > sampleSets;

	for( unsigned int i = 0; i < samples.size(); i++ ) {
		kome::objects::Sample* s = samples[ i ];
		kome::objects::SampleSet* ss = s->getSampleSet();

		if( sampleSets.find( ss ) == sampleSets.end() ) {
			sampleSets.insert( ss );

			// file name & directory
			std::string fileName = ss->getFileName();
			std::string fileDir = getdir( ss->getFilePath() );
			fileDir = replacestring( fileDir.c_str(), "\\", "/" );

			fprintf(
				fp,
				"		<sourceFile id=\"SF%d\" name=\"%s\" location=\"file://%s\" />\n",
				( i + 1 ),
				fileName.c_str(),
				fileDir.c_str()
			);
		}
	}

	// end tag
	fprintf( fp, "	  </sourceFileList>\n" );
}

// write referenceable parameters group
void MzmlManager::writeReferenceableParamGroup( FILE* fp, const bool specFlg, const bool chromFlg ) {

#ifdef MZML_USE_REFERENCEABLE_PARAM_GROUP

	// count
	int cnt = 0;
	if( specFlg && chromFlg ) {
		cnt = 3;
	}
	else if( specFlg || chromFlg ) {
		cnt = 2;
	}
	if( cnt == 0 ) {
		return;
	}

	// start tag
	fprintf( fp, "	<referenceableParamGroupList count=\"%d\">\n", cnt );

	// m/z
	if( specFlg ) {
		fprintf( fp, "	  <referenceableParamGroup id=\"mz_params\">\n" );
		fprintf( fp, "		<cvParam cvRef=\"MS\" accession=\"MS:1000514\"  name=\"m/z array\" value=\"\"\n" );
		fprintf( fp, "				 unitCvRef=\"MS\" unitAccession=\"MS:1000040\" unitName=\"m/z\"/>\n" );
		fprintf( fp, "		<cvParam cvRef=\"MS\" accession=\"MS:1000521\" name=\"32-bit float\" value=\"\"/>\n" );
		fprintf( fp, "		<cvParam cvRef=\"MS\" accession=\"MS:1000576\" name=\"no compression\" value=\"\"/>\n" );
		fprintf( fp, "	  </referenceableParamGroup>\n" );
	}

	// RT
	if( chromFlg ) {
		fprintf( fp, "	  <referenceableParamGroup id=\"rt_params\">\n" );
		fprintf( fp, "		<cvParam cvRef=\"MS\" accession=\"MS:1000595\"  name=\"time array\" value=\"\"\n" );
		fprintf( fp, "				 unitCvRef=\"UO\" unitAccession=\"UO:0000031\" unitName=\"minute\"/>\n" );
		fprintf( fp, "		<cvParam cvRef=\"MS\" accession=\"MS:1000521\" name=\"32-bit float\" value=\"\"/>\n" );
		fprintf( fp, "		<cvParam cvRef=\"MS\" accession=\"MS:1000576\" name=\"no compression\" value=\"\"/>\n" );
		fprintf( fp, "	  </referenceableParamGroup>\n" );
	}

	// intensity
	fprintf( fp, "	  <referenceableParamGroup id=\"int_params\">\n" );
	fprintf( fp, "		<cvParam cvRef=\"MS\" accession=\"MS:1000515\" name=\"intensity array\" value=\"\"/>\n" );
	fprintf( fp, "		<cvParam cvRef=\"MS\" accession=\"MS:1000521\" name=\"32-bit float\" value=\"\"/>\n" );
	fprintf( fp, "		<cvParam cvRef=\"MS\" accession=\"MS:1000576\" name=\"no compression\ value=\"\"/>\n" );
	fprintf( fp, "	  </referenceableParamGroup>\n" );

	// end tag
	fprintf( fp, "	</referenceableParamGroupList>\n" );

#endif	// MZML_USE_REFERENCEABLE_PARAM_GROUP
}

// write software list
void MzmlManager::writeSoftwareList( FILE* fp, std::vector< kome::objects::Sample* >& samples ) {
	// managers
	kome::core::MsppManager& msppMgr = kome::core::MsppManager::getInstance();
	AccessionManager& accMgr = AccessionManager::getInstance();

	// softwares
	std::vector< std::pair< std::string, std::string > > softwares;
	std::set< std::string > softwareSet;

	std::string software = "Mass_plus_plus";
	std::string version = msppMgr.getVersion();

	softwareSet.insert( software );
	softwares.push_back( std::make_pair( software, version ) );

	for( unsigned int i = 0; i < samples.size(); i++ ) {
		kome::objects::Sample* smpl = samples[ i ];

		software = smpl->getSoftwareName();
		version = smpl->getSoftwareVersion();

		if( !software.empty() && softwareSet.find( software ) == softwareSet.end() ) {
			softwareSet.insert( software );
			softwares.push_back( std::make_pair( software, version ) );
		}
	}

	// start tag
	fprintf( fp, "	<softwareList count=\"%d\">\n", softwares.size() );

	// software accession
	Accession* parentAcc = accMgr.getAccession( SOFTWARE_ACC );
	const double minScore = 3.0;

	// each softwares
	for( unsigned int i = 0; i < softwares.size(); i++ ) {
		// name, version
		software = softwares[ i ].first;
		version = softwares[ i ].second;

		// accession
		Accession* softAcc = NULL;
		double score = minScore - 0.01;

		for( unsigned int j = 0; j < accMgr.getNumberOfAccessions(); j++ ) {
			Accession* tmp = accMgr.getAccession( j );
			if( tmp->getOrigin() == parentAcc ) {
				double tmpScore = getSimilarityScore( tmp->getName(), software.c_str() );
				if( tmpScore > score ) {
					softAcc = tmp;
					score = tmpScore;
				}
			}
		}

		// tag
		fprintf( fp, "	  <software id=\"%s\" version=\"%s\"", software.c_str(), version.c_str() );
		if( softAcc == NULL ) {
			fprintf( fp, "/>\n" );
		}
		else {
			fprintf(
				fp,
				">\n		<cvParam cvRef=\"MS\" accession=\"%s\" name=\"%s\" value=\"\"/>\n",
				softAcc->getId(),
				softAcc->getName()
			);
			fprintf( fp, "	  </software>\n" );
		}
	}

	// end tag
	fprintf( fp, "	</softwareList>\n" );
}

// write instrument list
void MzmlManager::writeInstrumentList( FILE* fp, std::vector< kome::objects::Sample* >& samples ) {
	// manager
	AccessionManager& accMgr = AccessionManager::getInstance();

	// instruments
	std::vector< std::string > instruments;
	std::set< std::string > instrumentSet;

	for( unsigned int i = 0; i < samples.size(); i++ ) {
		std::string instrument = samples[ i ]->getInstrument();
		if( instrument.empty() ) {
			instrument = "undefined";
		}

		if( instrumentSet.find( instrument ) == instrumentSet.end() ) {
			instrumentSet.insert( instrument );
			instruments.push_back( instrument );
		}
	}

	// start tag
	fprintf( fp, "	<instrumentConfigurationList count=\"%d\">\n", instruments.size() );

	// each instruments
	Accession* parentAcc = accMgr.getAccession( MACHINE_ACC );
	const double minScore = 3.0;

	for( unsigned int i = 0; i < instruments.size(); i++ ) {
		// accession
		std::string instrument = instruments[ i ];
		Accession* machineAcc = NULL;
		double score = minScore - 0.01;

		for( unsigned int j = 0; j < accMgr.getNumberOfAccessions(); j++ ) {
			Accession* tmp = accMgr.getAccession( j );
			if( tmp->getOrigin() == parentAcc ) {
				double tmpScore = getSimilarityScore( tmp->getName(), instrument.c_str() );
				if( tmpScore > score ) {
					machineAcc = tmp;
					score = tmpScore;
				}
			}
		}

		// tag
		fprintf( fp, "	  <instrumentConfiguration id=\"%s\"", instrument.c_str() );
		if( machineAcc == NULL ) {
			fprintf( fp, "/>\n" );
		}
		else {
			fprintf(
				fp,
				">\n		<cvParam cvRef=\"MS\" accession=\"%s\" name=\"%s\" value=\"\"/>\n",
				machineAcc->getId(),
				machineAcc->getName()
			);
			fprintf( fp, "	  </instrumentConfiguration>\n" );
		}
	}

	// end tag
	fprintf( fp, "	</instrumentConfigurationList>\n" );
}

// write processing list
void MzmlManager::writeProcessingList( FILE* fp ) {
	// start tag
	fprintf( fp, "	<dataProcessingList count=\"1\">\n" );

	fprintf( fp, "	  <dataProcessing id=\"Mspp_proc\">\n" );
	fprintf( fp, "		<processingMethod softwareRef=\"Mass_plus_plus\" order=\"1\">\n" );
	fprintf( fp, "		  <cvParam cvRef=\"MS\" accession=\"MS:1000544\" name=\"Conversion to mzML\" value=\"\"/>\n" );
	fprintf( fp, "		</processingMethod>\n" );
	fprintf( fp, "	  </dataProcessing>\n" );

	// end tag
	fprintf( fp, "	</dataProcessingList>\n" );
}

// write run tag
void MzmlManager::writeRun(
		FILE* fp,
		std::vector< kome::objects::Sample* >& samples,
		std::vector< kome::objects::Spectrum* >& spectra,
		std::vector< kome::objects::Chromatogram* >& chroms,
		kome::core::Progress* specPrgs,
		kome::core::Progress* chromPrgs,
		kome::objects::SettingParameterValues* saveSettings		// @date 2012/08/21 <Add> FUJITA
) {
	// instrument
	std::string instrument;
	if( samples.size() > 0 ) {
		instrument = samples.front()->getInstrument();
	}
	if( instrument.empty() ) {
		instrument = "undefined";
	}
		
	// start tag
	fprintf( fp, "	<run id=\"R1\" defaultInstrumentConfigurationRef=\"%s\">\n", instrument.c_str() );

	// write spectra
	fprintf(
		fp,
		"	  <spectrumList defaultDataProcessingRef=\"Mspp_proc\" count=\"%d\"",
		spectra.size()
	);

	if( spectra.size() == 0 ) {
		fprintf( fp, "/>\n" );
	}
	else {
		fprintf( fp, ">\n" );

		specPrgs->setRange( 0, spectra.size() );
		specPrgs->setPosition( 0 );

		// each spectra
		for( unsigned int i = 0; i < spectra.size() && !specPrgs->isStopped(); i++ ) {
			// spectrum
			kome::objects::Spectrum* spec = spectra[ i ];

			std::string msg = FMT(
				"Writing Spectrum [%d/%d] ..... %s",
				( i + 1 ),
				spectra.size(),
				spec->getName()
			);
			specPrgs->setStatus( msg.c_str() );

			// write spectrum
			writeSpectrum( fp, *spec, saveSettings );
			specPrgs->setPosition( i + 1 );
		}

		if( specPrgs->isStopped() ) {
			return;
		}
		specPrgs->fill();

		fprintf( fp, "	  </spectrumList>\n" );
	}

	// write chromatograms
	if( chroms.size() > 0 ) {
		fprintf(
			fp,
			"	  <chromatogramList defaultDataProcessingRef=\"Mspp_proc\" count=\"%d\">\n",
			chroms.size()
		);

		chromPrgs->setRange( 0, chroms.size() );
		chromPrgs->setPosition( 0 );

		// each chromatograms
		for( unsigned int i = 0; i < chroms.size() && !chromPrgs->isStopped(); i++ ) {
			// chromatogram
			kome::objects::Chromatogram* chrom = chroms[ i ];

			std::string msg = FMT(
				"Writing Chromatogram [%d/%d] ..... %s",
				( i + 1 ),
				chroms.size(),
				chrom->getName()
			);
			chromPrgs->setStatus( msg.c_str() );

			// write chromatogram
			writeChromatogram( fp, *chrom, saveSettings );
			chromPrgs->setPosition( i + 1 );
		}

		if( chromPrgs->isStopped() ) {
			return;
		}
		chromPrgs->fill();

		fprintf( fp, "	  </chromatogramList>\n" );
	}

	// end tag
	fprintf( fp, "	</run>\n" );
}

// write spectrum
void MzmlManager::writeSpectrum( 
		FILE* fp,
		kome::objects::Spectrum& spec,
		kome::objects::SettingParameterValues* saveSettings
) {
	// get data status
	bool op = saveSettings->getBoolValue( DATA_STATUS,false );
	
	// data points
	kome::core::DataPoints tmpDps;
	kome::core::DataPoints dps( kome::core::DataPoints::FLOAT );

	spec.getXYData( &tmpDps, op );
	for( unsigned int i = 0; i < tmpDps.getLength(); i++ ) {
		const double x = tmpDps.getX( i );
		const double y = tmpDps.getY( i );

		if( y > 0.0 ) {
			dps.addPoint( x, y );
		}
	}

	// reverse
	const unsigned int len = dps.getLength();
	if( m_reverseFlg ) {
		float* xArr = (float*)dps.getXData();
		float* yArr = (float*)dps.getYData();

		for( unsigned int i = 0; i < len; i++ ) {
			memreverse( xArr + i, sizeof( float ) );
			memreverse( yArr + i, sizeof( float ) );
		}
	}

	// index
	const int idx = (int)m_specIdxMap.size();

	// start tag
	fprintf( fp, "		" );

	m_specIdxMap[ &spec ] = filetell( fp );

	// >>>>>>	@Date:2013/08/19	<Modify>	A.Ozaki
	//
	std::string	strSpotId;
	strSpotId.clear( );
	if  ( (char *)NULL != spec.getSpotId( ) )
	{
		strSpotId = FMT( "%s", spec.getSpotId( ) );
	}

	if  ( 0 == strSpotId.length( ) )
	{
		fprintf(
			fp,
			"<spectrum id=\"%s\" index=\"%d\" defaultArrayLength=\"%d\">\n",
			spec.getName(),
			idx,
			len
		);
	}
	else
	{
		fprintf(
			fp,
			"<spectrum id=\"%s\" index=\"%d\" defaultArrayLength=\"%d\" spotID=\"%s\">\n",
			spec.getName(),
			idx,
			len,
			strSpotId.c_str( )
		);
	}
	//
	// <<<<<<	@Date:2013/08/19	<Modify>	A.Ozaki

	// properties
	kome::core::Properties& props = spec.getProperties();

	if( spec.getMsStage() >= 1 ) {
		fprintf(
			fp,
			"		  <cvParam cvRef=\"MS\" accession=\"MS:1000511\" name=\"ms level\" value=\"%d\"/>\n",
			spec.getMsStage()
		);
	}

	if( spec.isCentroidMode() ) {
		fprintf( fp, "		  <cvParam cvRef=\"MS\" accession=\"MS:1000127\" name=\"centroid spectrum\" value=\"\"/>\n" );
	}
	else {
		fprintf( fp, "		  <cvParam cvRef=\"MS\" accession=\"MS:1000128\" name=\"profile spectrum\" value=\"\"/>\n" );
	}

	if( spec.getPolarity() == kome::objects::Spectrum::POLARITY_NEGATIVE ) {
		fprintf( fp, "		  <cvParam cvRef=\"MS\" accession=\"MS:1000129\" name=\"negative scan\" value=\"\"/>\n" );
	}
	else if( spec.getPolarity() == kome::objects::Spectrum::POLARITY_POSITIVE ) {
		fprintf( fp, "		  <cvParam cvRef=\"MS\" accession=\"MS:1000130\" name=\"positive scan\" value=\"\"/>\n" );
	}

	if( spec.getTotalIntensity() >= 0.0 ) {
		fprintf(
			fp,
			"		  <cvParam cvRef=\"MS\" accession=\"MS:1000285\" name=\"total ion current\" value=\"%f\"/>\n",
			spec.getTotalIntensity()
		);
	}

	if( spec.getBasePeakMass() >= 0.0 ) {
		fprintf(
			fp,
			"		  <cvParam cvRef=\"MS\" accession=\"MS:1000504\" name=\"base peak m/z\" value=\"%f\"/>\n",
			spec.getBasePeakMass()
		);
	}

	if( spec.getMaxIntensity() >= 0.0 ) {
		fprintf( 
			fp,
			"		  <cvParam cvRef=\"MS\" accession=\"MS:1000505\" name=\"base peak intensity\" value=\"%f\"/>\n",
			spec.getMaxIntensity()
		);
	}

	// scan list
	fprintf(
		fp,
		"		  <scanList count=\"1\">\n"
	);

	std::string instrument = spec.getSample()->getInstrument();
	if( instrument.empty() ) {
		instrument = "undefined";
	}

	fprintf(
		fp,
		"			<scan>\n"
	);

	std::string filter = props.getStringValue( "Filter", "" );
	if( !filter.empty() ) {
		fprintf( 
			fp,
			"			  <cvParam cvRef=\"MS\" accession=\"MS:1000512\" name=\"filter string\" value=\"%s\"/>\n",
			filter.c_str()
		);
	}

	if( spec.getRt() >= 0.0 ) {
		fprintf( 
			fp,
			"			  <cvParam cvRef=\"MS\" accession=\"MS:1000016\" name=\"scan start time\" value=\"%f\"\n",
			spec.getRt()
		);

		fprintf( 
			fp,
			"					   unitCvRef=\"UO\" unitAccession=\"UO:0000031\" unitName=\"minute\"/>\n"
		);
	}

	fprintf(
		fp,
		"			  <scanWindowList count=\"1\">\n"
	);

	fprintf(
		fp,
		"				<scanWindow>\n"
	);

	if( spec.getMinX() >= 0.0 ) {
		fprintf(
			fp,
			"				  <cvParam cvRef=\"MS\" accession=\"MS:1000501\" name=\"scan window lower limit\" value=\"%f\"/>\n",
			spec.getMinX()
		);
	}
	if( spec.getMaxX() >= 0.0 ) {
		fprintf(
			fp,
			"				  <cvParam cvRef=\"MS\" accession=\"MS:1000500\" name=\"scan window upper limit\" value=\"%f\"/>\n",
			spec.getMaxX()
		);
	}

	fprintf(
		fp,
		"				</scanWindow>\n"
	);

	fprintf(
		fp,
		"			  </scanWindowList>\n"
	);

	fprintf(
		fp,
		"			</scan>\n"
	);

	fprintf(
		fp,
		"		  </scanList>\n"
	);

	// p recursor
	const double precursor = spec.getPrecursor();
	if( precursor >= 0.0 ) {
		kome::objects::Spectrum* parent = spec.getParentSpectrum();
		std::string parentId = ( parent == NULL ? "" : parent->getName() );

		fprintf(
			fp,
			"		  <precursorList count=\"1\">\n"
		);

		fprintf(
			fp,
			"			<precursor spectrumRef=\"%s\">\n",
			parentId.c_str()
		);

		fprintf(
			fp,
			"			  <selectedIonList count=\"1\">\n"
		);

		fprintf(
			fp,
			"				<selectedIon>\n"
		);

		fprintf( 
			fp,
			"				  <cvParam cvRef=\"MS\" accession=\"MS:1000744\" name=\"selected ion m/z\"\n"
		);

		fprintf(
			fp,
			"						 value=\"%f\" unitCvRef=\"MS\" unitAccession=\"MS:1000040\" unitName=\"m/z\"/>\n",
			precursor
		);

		fprintf(
			fp,
			"				</selectedIon>\n"
		);

		fprintf(
			fp,
			"			  </selectedIonList>\n"
		);

		fprintf(
			fp,
			"			</precursor>\n"
		);

		fprintf(
			fp,
			"		  </precursorList>\n"
		);
	}

	// data points
	if( len > 0 ) {
		// prepare
		unsigned long buffLength = len * sizeof( float ) * 2;
		char* base64Buff = new char[ buffLength ];

		fprintf( fp, "		  <binaryDataArrayList count=\"2\">\n" );

		// m/z
		unsigned long size = kome::core::Base64::encode( dps.getXData(), sizeof( float ) * len, base64Buff, buffLength );

		fprintf( fp, "			<binaryDataArray encodedLength=\"%d\">\n", size );

#ifdef MZML_USE_REFERENCEABLE_PARAM_GROUP
		fprintf( fp, "			  <referenceableParamGroupRef ref=\"mz_params\"/>\n" );
#else
		fprintf( fp, "			  <cvParam cvRef=\"MS\" accession=\"MS:1000514\"  name=\"m/z array\" value=\"\"\n" );
		fprintf( fp, "					   unitCvRef=\"MS\" unitAccession=\"MS:1000040\" unitName=\"m/z\"/>\n" );
		fprintf( fp, "			  <cvParam cvRef=\"MS\" accession=\"MS:1000521\" name=\"32-bit float\" value=\"\"/>\n" );
		fprintf( fp, "			  <cvParam cvRef=\"MS\" accession=\"MS:1000576\" name=\"no compression\" value=\"\"/>\n" );
#endif	// MZML_USE_REFERENCEABLE_PARAM_GROUP

		fprintf( fp, "			  <binary>" );


		char* pos = base64Buff;
		while( size > 0 ) {
			int writeSize = MIN( size, 1024 );
			writeSize = fwrite( pos, 1, writeSize, fp );
			size -= writeSize;
			pos += writeSize;
		}

		fprintf( fp, "</binary>\n" );
		fprintf( fp, "			</binaryDataArray>\n" );

		// intensity
		size = kome::core::Base64::encode( dps.getYData(), sizeof( float ) * len, base64Buff, buffLength );

		fprintf( fp, "			<binaryDataArray encodedLength=\"%d\">\n", size );

#ifdef MZML_USE_REFERENCEABLE_PARAM_GROUP
		fprintf( fp, "			  <referenceableParamGroupRef ref=\"int_params\"/>\n" );
#else
		fprintf( fp, "			  <cvParam cvRef=\"MS\" accession=\"MS:1000515\" name=\"intensity array\" value=\"\"/>\n" );
		fprintf( fp, "			  <cvParam cvRef=\"MS\" accession=\"MS:1000521\" name=\"32-bit float\" value=\"\"/>\n" );
		fprintf( fp, "			  <cvParam cvRef=\"MS\" accession=\"MS:1000576\" name=\"no compression\" value=\"\"/>\n" );
#endif	// MZML_USE_REFERENCEABLE_PARAM_GROUP

		fprintf( fp, "			  <binary>" );

		pos = base64Buff;
		while( size > 0 ) {
			int writeSize = MIN( size, 1024 );
			writeSize = fwrite( pos, 1, writeSize, fp );
			size -= writeSize;
			pos += writeSize;
		}

		fprintf( fp, "</binary>\n" );
		fprintf( fp, "			</binaryDataArray>\n" );

		fprintf( fp, "		  </binaryDataArrayList>\n" );

		delete[] base64Buff;
	}

	// end tag
	fprintf( fp, "		</spectrum>\n" );
}

// writing chromatogrm
void MzmlManager::writeChromatogram(
		FILE* fp,
		kome::objects::Chromatogram& chrom,
		kome::objects::SettingParameterValues* saveSettings
) {
	// get data status
	bool op = saveSettings->getBoolValue( DATA_STATUS,false );		// @date 2012/08/21 <Add> FUJITA

	// data points
	kome::core::DataPoints dps( kome::core::DataPoints::FLOAT );
//	chrom.getXYData( &dps, false );                                 // @date 2012/08/21 <Del> FUJITA
	chrom.getXYData( &dps, op );                                    // @date 2012/08/21 <Add> FUJITA

	// reverse
	const unsigned int len = dps.getLength();
	if( m_reverseFlg ) {
		float* xArr = (float*)dps.getXData();
		float* yArr = (float*)dps.getYData();

		for( unsigned int i = 0; i < len; i++ ) {
			memreverse( xArr + i, sizeof( float ) );
			memreverse( yArr + i, sizeof( float ) );
		}
	}

	// index
	const int idx = m_chromIdxMap.size();

	// start tag
	fprintf( fp, "		" );

	m_chromIdxMap[ &chrom ] = filetell( fp );

	fprintf(
		fp,
		"<chromatogram id=\"%s\" index=\"%d\" defaultArrayLength=\"%d\">\n",
		chrom.getName(),
		idx,
		len
	);

	// data points
	if( len > 0 ) {
		// prepare
		unsigned long buffLength = len * sizeof( float ) * 2;
		char* base64Buff = new char[ buffLength ];

		fprintf( fp, "		  <binaryDataArrayList count=\"2\">\n" );

		// RT
		unsigned long size = kome::core::Base64::encode( dps.getXData(), sizeof( float ) * len, base64Buff, buffLength );

		fprintf( fp, "			<binaryDataArray encodedLength=\"%d\">\n", size );

#ifdef MZML_USE_REFERENCEABLE_PARAM_GROUP
		fprintf( fp, "			  <referenceableParamGroupRef ref=\"rt_params\"/>\n" );
#else
		fprintf( fp, "			  <cvParam cvRef=\"MS\" accession=\"MS:1000595\"  name=\"time array\" value=\"\"\n" );
		fprintf( fp, "					   unitCvRef=\"UO\" unitAccession=\"UO:0000031\" unitName=\"minute\"/>\n" );
		fprintf( fp, "			  <cvParam cvRef=\"MS\" accession=\"MS:1000521\" name=\"32-bit float\" value=\"\"/>\n" );
		fprintf( fp, "			  <cvParam cvRef=\"MS\" accession=\"MS:1000576\" name=\"no compression\" value=\"\"/>\n" );
#endif	// MZML_USE_REFERENCEABLE_PARAM_GROUP

		fprintf( fp, "			  <binary>" );

		char* pos = base64Buff;
		while( size > 0 ) {
			int writeSize = MIN( size, 1024 );
			writeSize = fwrite( pos, 1, writeSize, fp );
			size -= writeSize;
			pos += writeSize;
		}

		fprintf( fp, "</binary>\n" );
		fprintf( fp, "			</binaryDataArray>\n" );

		// intensity
		size = kome::core::Base64::encode( dps.getYData(), sizeof( float ) * len, base64Buff, buffLength );

		fprintf( fp, "			<binaryDataArray encodedLength=\"%d\">\n", size );

#ifdef MZML_USE_REFERENCEABLE_PARAM_GROUP
		fprintf( fp, "			  <referenceableParamGroupRef ref=\"int_params\"/>\n" );
#else
		fprintf( fp, "			  <cvParam cvRef=\"MS\" accession=\"MS:1000515\" name=\"intensity array\" value=\"\"/>\n" );
		fprintf( fp, "			  <cvParam cvRef=\"MS\" accession=\"MS:1000521\" name=\"32-bit float\" value=\"\"/>\n" );
		fprintf( fp, "			  <cvParam cvRef=\"MS\" accession=\"MS:1000576\" name=\"no compression\" value=\"\"/>\n" );
#endif	// MZML_USE_REFERENCEABLE_PARAM_GROUP

		fprintf( fp, "			  <binary>" );

		pos = base64Buff;
		while( size > 0 ) {
			int writeSize = MIN( size, 1024 );
			writeSize = fwrite( pos, 1, writeSize, fp );
			size -= writeSize;
			pos += writeSize;
		}

		fprintf( fp, "</binary>\n" );
		fprintf( fp, "			</binaryDataArray>\n" );

		fprintf( fp, "		  </binaryDataArrayList>\n" );

		delete[] base64Buff;
	}

	// end tag
	fprintf( fp, "		</chromatogram>\n" );
}

// write run tag
void MzmlManager::writeIndexList(
		FILE* fp,
		std::vector< kome::objects::Spectrum* >& spectra,
		std::vector< kome::objects::Chromatogram* >& chroms
) {
	// end mzML tag
	fprintf( fp, "  </mzML>\n" );

	// list count
	int listCnt = 0;
	if( spectra.size() > 0 ) {
		listCnt++;
	}
	if( chroms.size() > 0 ) {
		listCnt++;
	}

	// write index list
	const long long idxOffset = filetell( fp ) + 2;
	if( listCnt > 0 ) {
		// start tag
		fprintf( fp, "  <indexList count=\"%d\">\n", listCnt );

		// spectra
		if( spectra.size() > 0 ) {
			fprintf( fp, "	<index name=\"spectrum\">\n" );

			for( unsigned int i = 0; i < spectra.size(); i++ ) {
				kome::objects::Spectrum* spec = spectra[ i ];
				if( m_specIdxMap.find( spec ) != m_specIdxMap.end() ) {
					const long long offset = m_specIdxMap[ spec ];

					fprintf( fp, "	  <offset idRef=\"%s\">%lld</offset>\n", spec->getName(), offset );
				}
			}

			fprintf( fp, "	</index>\n" );
		}

		// chromatograms
		if( chroms.size() > 0 ) {
			fprintf( fp, "	<index name=\"chromatogram\">\n" );

			for( unsigned int i = 0; i < chroms.size(); i++ ) {
				kome::objects::Chromatogram* chrom = chroms[ i ];
				if( m_chromIdxMap.find( chrom ) != m_chromIdxMap.end() ) {
					const long long offset = m_chromIdxMap[ chrom ];

					fprintf( fp, "	  <offset idRef=\"%s\">%lld</offset>\n", chrom->getName(), offset );
				}
			}

			fprintf( fp, "	</index>\n" );
		}

		// end tag
		fprintf( fp, "  </indexList>\n" );

		// index offset
		fprintf( fp, "  " );
		fprintf( fp, "<indexListOffset>%lld</indexListOffset>\n", idxOffset );
	}

	// end root tag
	fprintf( fp, "</indexedmzML>\n" );
}

// gets similarity score
double MzmlManager::getSimilarityScore( const char* s0, const char* s1 ) {
	// check parameters
	if( s0 == NULL || s1 == NULL ) {
		return ( - FLT_MAX );
	}

	// size
	unsigned int l0 = strlen( s0 );
	unsigned int l1 = strlen( s1 );

	if( l0 == 0 && l1 == 0 ) {
		return 0;
	}
	else if( l0 == 0 || l1 == 0 ) {
		return ( - FLT_MAX );
	}

	// dynamic programing
	kome::numeric::DynamicPrograming dp( l0, l1 );
	dp.calculate( boost::bind( getMatchScore, s0, _1, s1, _2 ), 0.1 );

	return dp.getAccumulatedScore( (int)l0 - 1, (int)l1 - 1 );
}

// get match score
double MzmlManager::getMatchScore( const char* s0, int idx0, const char* s1, int idx1 ) {
	// character
	char c0 = s0[ idx0 ];
	char c1 = s1[ idx1 ];

	bool a0 = ( isalnum( c0 ) != 0 );
	bool a1 = ( isalnum( c1 ) != 0 );

	// score
	if( a0 && a1 ) {
		if( tolower( c0 ) == tolower( c1 ) ) {
			return 1.0;
		}
		else {
			return -1.0;
		}
	}

	if( !a0 && !a1 ) {
		return 1.0;
	}

	return - 0.1;
}

// get instance
MzmlManager& MzmlManager::getInstance() { 
	// create object (This is the only object.)
	static MzmlManager mgr;

	return mgr;
}


// compare to sort	@date 2011.10.31 <Add> M.Izumi
bool MzmlManager::lessSpec( kome::objects::Spectrum* spec0, kome::objects::Spectrum* spec1 ) {
	return ( spec0->getRt() < spec1->getRt() );
}

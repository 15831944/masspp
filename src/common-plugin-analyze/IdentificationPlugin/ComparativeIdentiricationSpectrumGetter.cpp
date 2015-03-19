/**
 * @file ComparativeIdentificationSpectrumGetter.h
 * @brief interfaces of ComparativeIdentificationSpectrumGetter class
 *
 * @author S.Tanaka
 * @date 2012.08.02
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation All rights reserved.
 */


#include "stdafx.h"
#include "ComparativeIdentificationSpectrumGetter.h"


using namespace kome::ident;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
ComparativeIdentificationSpectrumGetter::ComparativeIdentificationSpectrumGetter( kome::ident::SearchEngine* engine ) {
	// initialize
	m_engine = engine;
	m_rtTol = -1.0;
	m_mzTol = -1.0;
	m_fileIdx = -1;
	m_specIdx = -1;
	m_sFlg = false;
	m_ssFlg = false;
}

// destructor
ComparativeIdentificationSpectrumGetter::~ComparativeIdentificationSpectrumGetter() {
}

// add file
void ComparativeIdentificationSpectrumGetter::addFile( const char* path ) {
	m_paths.push_back( NVL( path, "" ) );

	m_fileIdx = -1;
	m_specIdx = -1;
	m_spectra.clear();
	m_peakIdMap.clear();
	m_sFlg = false;
	m_ssFlg = false;
}

// clear files
void ComparativeIdentificationSpectrumGetter::clearFiles() {
	m_paths.clear();

	m_fileIdx = -1;
	m_specIdx = -1;
	m_spectra.clear();
	m_peakIdMap.clear();
	m_sFlg = false;
	m_ssFlg = false;
}

// add peak
void ComparativeIdentificationSpectrumGetter::addPeak( const int peakId ) {
	// manager
	kome::ident::IdentificationManager& identMgr = kome::ident::IdentificationManager::getInstance();

	// peak
	double rt = identMgr.getMatrixPeakRt( peakId );
	double mz = identMgr.getMatrixPeakMz( peakId );

	if( rt >= 0.0 || mz >= 0.0 ) {
		m_peaks.resize( m_peaks.size() + 1 );
		m_peaks.back().id = peakId;
		m_peaks.back().rt = rt;
		m_peaks.back().mz = mz;
	}

	// initialize
	m_fileIdx = -1;
	m_specIdx = -1;
	m_spectra.clear();
	m_peakIdMap.clear();
}

// clear peaks
void ComparativeIdentificationSpectrumGetter::clearPeaks() {
	// clear
	m_peaks.clear();

	m_fileIdx = -1;
	m_specIdx = -1;
	m_spectra.clear();
	m_peakIdMap.clear();
}

// set RT tolerance
void ComparativeIdentificationSpectrumGetter::setRtTol( const double tol ) {
	m_rtTol = tol;
}

// set m/z tolerance
void ComparativeIdentificationSpectrumGetter::setMzTol( const double tol ) {
	m_mzTol = tol;
}

// prepare next spectrum
bool ComparativeIdentificationSpectrumGetter::next( kome::core::Progress& progress ) {
	// manager
	kome::plugin::FileFactory& ff = kome::plugin::FileFactory::getInstance();

	// file index
	if( m_fileIdx < 0 ) {
		if( m_paths.size() ) {
			progress.createSubProgresses( m_paths.size() );
		}
	}
	if( m_fileIdx >= (int)m_paths.size() ) {
		return false;
	}

	// progress
	kome::core::Progress* prgs = ( m_fileIdx < 0 ? NULL : progress.getSubProgress( m_fileIdx ) );

	// spectrum index
	m_specIdx++;
	if( prgs != NULL ) {
		prgs->setPosition( m_specIdx );
	}

	if( m_specIdx >= (int)m_spectra.size() ) {    // next file
		// close sample
		if( m_sFlg ) {
			m_currSample->closeSample();
		}
		if( m_ssFlg ) {
			ff.closeFile( m_currSample->getSampleSet() );
		}

		// initialize
		m_specIdx = 0;
		m_spectra.clear();
		m_fileIdx++;

		while( m_spectra.size() == 0 && m_fileIdx < (int)m_paths.size() && !progress.isStopped() ) {
			// progress
			kome::core::Progress* prgs = progress.getSubProgress( m_fileIdx );

			// sample set
			kome::objects::SampleSet* ss = ff.searchFile( m_paths[ m_fileIdx ].c_str() );
			m_ssFlg = ( ss == NULL || !ss->isOpened() );
			if( ss == NULL ) {
				ss = ff.openFile( m_paths[ m_fileIdx ].c_str());
			}
			if( ss != NULL && !ss->isOpened() ) {
				ss->openFile( m_paths[ m_fileIdx ].c_str(), prgs);	// add param
			}

			// sample
			kome::objects::Sample* s = ( ss == NULL ? NULL : ss->getSample( 0 ) );
			m_sFlg = ( s != NULL && !s->isOpened() );
			if( m_sFlg ) {
				s->openSample();
			}

			m_currSample = s;

			// spectra
			std::vector< kome::objects::Spectrum* > spectra;
			m_spectra.clear();
			m_peakIdMap.clear();
			if( s != NULL ) {
				for( unsigned int i = 0; i < s->getNumberOfGroups() && !prgs->isStopped(); i++ ) {
					kome::objects::DataGroupNode* group = s->getGroup( i );
					for( unsigned int j = 0; j < group->getNumberOfSpectra(); j++ ) {
						kome::objects::Spectrum* spec = group->getSpectrum( j );
						if( m_engine != NULL && m_engine->isValidSpectrum( spec ) ) {
							spectra.push_back( spec );
						}
					}
				}
			}
			
			for( unsigned int i = 0; i < spectra.size() && !prgs->isStopped(); i++ ) {
				kome::objects::Spectrum* spec = spectra[ i ];
				
				for( unsigned int j = 0; j < m_peaks.size(); j++ ) {
					const int id = m_peaks[ j ].id;
					const double rt = m_peaks[ j ].rt;
					const double mz = m_peaks[ j ].mz;
					
					if( ( mz < 0.0 || ( spec->getPrecursor() >= 0.0 && fabs( mz - spec->getPrecursor() ) <= m_mzTol ) )
						&& ( rt < 0.0 || ( spec->getRt() >= 0.0 && fabs( rt - spec->getRt() ) <= m_rtTol ) ) ) {
							m_peakIdMap[ spec ].push_back( id );
					}
				}

				if( m_peakIdMap[ spec ].size() > 0 ) {
					m_spectra.push_back( spec );
				}
			}
			
			if( prgs->isStopped() ){ 
				return false;
			}
			if( m_spectra.size() == 0 ) {
				if( m_sFlg ) {
					m_currSample->closeSample();
				}
				if( m_ssFlg ) {
					ff.closeFile( m_currSample->getSampleSet() );
				}
				prgs->fill();
				m_fileIdx++;
			}
			else {
				prgs->setRange( 0, m_spectra.size() );
			}
		}
	}

	return ( m_specIdx < (int)m_spectra.size() );
}


// get spectrum
kome::objects::Spectrum* ComparativeIdentificationSpectrumGetter::getSpectrum() {
	// get spectrum
	if( m_specIdx < (int)m_spectra.size() ) {
		return m_spectra[ m_specIdx ];
	}

	return NULL;
}

// get the number of peak IDs
unsigned int ComparativeIdentificationSpectrumGetter::getNumberOfPeakIds() {
	// spectrum
	kome::objects::Spectrum* spec = getSpectrum();
	if( spec == NULL ) {	
		return 0;
	}

	return m_peakIdMap[ spec ].size();
}

// get peak ID
int ComparativeIdentificationSpectrumGetter::getPeakId( const unsigned int idx ) {
	// spectrum
	kome::objects::Spectrum* spec = getSpectrum();
	if( spec == NULL ) {	
		return -1;
	}

	return m_peakIdMap[ spec ].at( idx );
}

// set parameters
void ComparativeIdentificationSpectrumGetter::setParameters( const char* s ) {
	// string object
	std::string str = NVL( s, "" );

	// clear
	clearFiles();
	clearPeaks();

	// separate
	std::string::size_type pos = str.find( "||" );
	std::string paths;
	std::string peaks; 
	if( pos == str.npos ) {
		paths = str.substr( 0, pos );
		peaks = str.substr( pos + 2 );
	}

	// files
	std::vector< std::string > files;
	stringtoken( paths.c_str(), "|", files );
	for( unsigned int i = 0; i < files.size(); i++ ) {
		std::string path = files[ i ];
		if( !path.empty() ) {
			addFile( path.c_str() );
		}
	}

	// peaks
	std::vector< std::string > peakIds;
	stringtoken( peaks.c_str(), ":", peakIds );
	for( unsigned int i = 0; i < peakIds.size(); i++ ) {
		int peakId = toint( peakIds[ i ].c_str(), 10, -1 );
		if( peakId >= 0 ) {
			addPeak( peakId );
		}
	}
}

// get parameters
std::string ComparativeIdentificationSpectrumGetter::getParameters() { 
	// return value
	std::string ret;

	// files
	for( unsigned int i = 0; i < m_paths.size(); i++ ) {
		if( i > 0 ) {
			ret.append( "|" );
		}
		ret.append( m_paths[ i ] );

	}
	ret.append( "||" );

	// peak IDs
	for( unsigned int i = 0; i < m_peaks.size(); i++ ) {
		if( i > 0 ) {
			ret.append( ":" );
		}
		ret.append( FMT( "%d", m_peaks[ i ].id ) );
	}

	return ret;
}

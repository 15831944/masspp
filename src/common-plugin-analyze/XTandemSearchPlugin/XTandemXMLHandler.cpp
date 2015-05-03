/**
 * @file XTandemXMLHandler.cpp
 * @brief interfaces of XTandemXMLHandler class
 *
 * @author S.Nakaya
 * @date 2011.1.26
 * 
 * Copyright(C) 2006-2014 Shimadzu Co., Ltd. All rights reserved.
 */

#include "stdafx.h"
#include "XTandemXMLHandler.h"

using namespace kome::xtandem;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// tags
#define GROUP_TAG                      "group"
#define PROTEIN_TAG                    "protein"
#define PEPTIDE_TAG                    "peptide"
#define NOTE_TAG                       "note"
#define DOMAIN_TAG                     "domain"
#define AA_TAG                         "aa"

#define GROUP_MH_ATTR                  "mh"
#define GROUP_EXPECT_ATTR              "expect"
#define GROUP_SUMI_ATTR                "sumI"
#define GROUP_MAXI_ATTR                "maxI"
#define GROUP_FI_ATTR                  "fI"

#define PROTEIN_SUMI_ATTR              "sumI"
#define PROTEIN_EXPECT_ATTR            "expect"

#define DOMAIN_START_ATTR              "start"
#define DOMAIN_END_ATTR                "end"
#define DOMAIN_EXPECT_ATTR             "expect"
#define DOMAIN_MH_ATTR                 "mh"
#define DOMAIN_DELTA_ATTR              "delta"
#define DOMAIN_HYPER_SCORE_ATTR        "hyperscore"
#define DOMAIN_NEXT_SCORE_ATTR         "nextscore"
#define DOMAIN_A_SCORE_ATTR            "a_score"
#define DOMAIN_B_SCORE_ATTR            "b_score"
#define DOMAIN_C_SCORE_ATTR            "c_score"
#define DOMAIN_X_SCORE_ATTR            "x_score"
#define DOMAIN_Y_SCORE_ATTR            "y_score"
#define DOMAIN_Z_SCORE_ATTR            "z_score"
#define DOMAIN_A_IONS_ATTR             "a_ions"
#define DOMAIN_B_IONS_ATTR             "b_ions"
#define DOMAIN_C_IONS_ATTR             "c_ions"
#define DOMAIN_X_IONS_ATTR             "x_ions"
#define DOMAIN_Y_IONS_ATTR             "y_ions"
#define DOMAIN_Z_IONS_ATTR             "z_ions"
#define DOMAIN_PRE_ATTR                "pre"
#define DOMAIN_POST_ATTR               "post"
#define DOMAIN_SEQ_ATTR                "seq"
#define DOMAIN_MISSED_CLEAVAGES_ATTR   "missed_cleavages"


// constructor
XTandemXMLHandler::XTandemXMLHandler(
		kome::ident::SearchResult& result,
		kome::ident::SearchInfo& searchInfo,
		kome::ident::SpecInfo** spectra,
		const unsigned int specNum
) : m_result( result ), m_searchInfo( searchInfo ) {
	onStartDocument();

	if( spectra != NULL ) {
		for( unsigned int i = 0; i < specNum; i++ ) {
			kome::ident::SpecInfo* spec = spectra[ i ];
			std::string key = FMT( "%d:%d:%s", spec->sampleIndex, spec->specId, spec->path.c_str() );
			m_specMap[ key ] = spec;
		}
	}

	// modifications
	m_mods.clear();
	for( unsigned int i = 0; i < searchInfo.fixedMods.size(); i++ ) {
		m_mods.push_back( searchInfo.fixedMods.at( i ) );
	}
	for( unsigned int i = 0; i < searchInfo.varMods.size(); i++ ) {
		m_mods.push_back( searchInfo.varMods.at( i ) );
	}
}

// destructor
XTandemXMLHandler::~XTandemXMLHandler() {
	if( m_currPeptide != NULL ){
		delete m_currPeptide;
	}
	m_currPeptide = NULL;
}

// start document
void XTandemXMLHandler::onStartDocument() {
	m_inProtein = false;
	m_inPeptide = false;
	m_groupDepth = 0;

	m_currProteins.clear();
	m_currPeptide = NULL;
	m_currHits.clear();

	m_proteinMap.clear();
	m_peptideMap.clear();
}

// end document
void XTandemXMLHandler::onEndDocument() {
	onStartDocument();
}

// start element
void XTandemXMLHandler::onStartElement( const char* name, kome::core::Properties& attrs ) {
	// each tags
	if( strcmp( name, GROUP_TAG ) == 0 ) {    // <group>
		if( m_groupDepth == 0 ) {    // protein group
			m_proteinGroup.mh = attrs.getDoubleValue( GROUP_MH_ATTR, -1.0 );
			m_proteinGroup.expect = attrs.getDoubleValue( GROUP_EXPECT_ATTR, -1.0 );
			m_proteinGroup.sumI = attrs.getDoubleValue( GROUP_SUMI_ATTR, -1.0 );
			m_proteinGroup.maxI = attrs.getDoubleValue( GROUP_MAXI_ATTR, -1.0 );
			m_proteinGroup.fI = attrs.getDoubleValue( GROUP_FI_ATTR, -1.0 );

			m_currProteins.clear();
			m_currPeptide = NULL;
			m_currHits.clear();
		}

		// depth
		m_groupDepth += 1;
	}
	else if( strcmp( name, PROTEIN_TAG ) == 0 ) {    // <protein>
		// flag
		m_inProtein = true;

		// protein
		m_proteinInfo.expect = attrs.getDoubleValue( PROTEIN_EXPECT_ATTR, 0.0 );
		m_proteinInfo.sumI = attrs.getDoubleValue( PROTEIN_SUMI_ATTR, -1.0 );
	}
	else if( strcmp( name, PEPTIDE_TAG ) == 0 ) {    // <peptide>
		m_inPeptide = true;
	}
	else if( strcmp( name, DOMAIN_TAG ) == 0 ) {    // <domain>
		if( m_inPeptide ) {    // peptide
			const int start = attrs.getIntValue( DOMAIN_START_ATTR, -1 );
			const int end = attrs.getIntValue( DOMAIN_END_ATTR, -1 );
			std::string preStr = attrs.getStringValue( DOMAIN_PRE_ATTR, "" );
			std::string postStr = attrs.getStringValue( DOMAIN_POST_ATTR, "" );

			if( m_currProteins.size() == 1 ) {    // hit
				const double expect = attrs.getDoubleValue( DOMAIN_EXPECT_ATTR, -1.0 );
				const double mh = attrs.getDoubleValue( DOMAIN_MH_ATTR, -1.0 );
				const double delta = attrs.getDoubleValue( DOMAIN_DELTA_ATTR, -1.0 );
				const double hyperscore = attrs.getDoubleValue( DOMAIN_HYPER_SCORE_ATTR, -1.0 );
				const double nextscore = attrs.getDoubleValue( DOMAIN_NEXT_SCORE_ATTR, -1.0 );
				const double ascore = attrs.getDoubleValue( DOMAIN_A_SCORE_ATTR, -1.0 );
				const double bscore = attrs.getDoubleValue( DOMAIN_B_SCORE_ATTR, -1.0 );
				const double cscore = attrs.getDoubleValue( DOMAIN_C_SCORE_ATTR, -1.0 );
				const double xscore = attrs.getDoubleValue( DOMAIN_X_SCORE_ATTR, -1.0 );
				const double yscore = attrs.getDoubleValue( DOMAIN_Y_SCORE_ATTR, -1.0 );
				const double zscore = attrs.getDoubleValue( DOMAIN_Z_SCORE_ATTR, -1.0 );
				const int aions = std::max( 0, attrs.getIntValue( DOMAIN_A_IONS_ATTR, 0 ) );
				const int bions = std::max( 0, attrs.getIntValue( DOMAIN_B_IONS_ATTR, 0 ) );
				const int cions = std::max( 0, attrs.getIntValue( DOMAIN_C_IONS_ATTR, 0 ) );
				const int xions = std::max( 0, attrs.getIntValue( DOMAIN_X_IONS_ATTR, 0 ) );
				const int yions = std::max( 0, attrs.getIntValue( DOMAIN_Y_IONS_ATTR, 0 ) );
				const int zions = std::max( 0, attrs.getIntValue( DOMAIN_Z_IONS_ATTR, 0 ) );
				std::string seq = attrs.getStringValue( DOMAIN_SEQ_ATTR, "" );
				const int missedcleavages = attrs.getIntValue( DOMAIN_MISSED_CLEAVAGES_ATTR, -1 );

				if( !seq.empty() && start >= 0 && start < end ) {
					// peptide name
					std::string pepName = seq;
					if( !preStr.empty() ) {
						pepName = FMT( "%s.%s", preStr.substr( preStr.length() - 1 ).c_str(), seq.c_str() );
					}
					if( !postStr.empty() ) {
						pepName = FMT( "%s.%s", pepName.c_str(), postStr.substr( 0, 1 ).c_str() );
					}
					
					// map name
					std::string mapName = FMT(
						"%s:%s:%d:%d",
						m_currProteins.back()->getAccession().c_str(),
						pepName.c_str(),
						start,
						end
					);
					
					// peptide information
					if( m_peptideMap.find( mapName ) == m_peptideMap.end() ) {
						m_currPeptide = new kome::ident::PeptideSubstance( &m_result, "", pepName.c_str(), m_currProteins.back() );
						m_result.addSubstance( m_currPeptide );
						m_peptideMap[ mapName ] = m_currPeptide;
						
						m_currPeptide->setStart( start );
						m_currPeptide->setEnd( end );
						m_currPeptide->setResidueBefore( preStr.c_str() );
						m_currPeptide->setResidueAfter( postStr.c_str() );
						m_currPeptide->setSequence( seq.c_str() );
					}
					else {
						m_currPeptide = m_peptideMap[ mapName ];
					}
					
					// hit
					kome::ident::Hit* currHit = m_result.addHit( m_currPeptide );
					m_currHits.push_back( currHit );
					currHit->setProteinCount( 1 );
					currHit->setRank( 1 );
					currHit->setDelta( delta );
					currHit->setMissedCleavagesCount( missedcleavages );
					currHit->setCalculatedMR( mh );
					currHit->setMatchedIonCount( aions + bions + cions + xions + yions + zions );
					currHit->addValue( "Protein sumI", FMT( "%f", m_proteinInfo.sumI ).c_str(), kome::ident::ValueType::TYPE_PROPERTY );
					currHit->addValue( "Protein maxI", FMT( "%f", m_proteinGroup.maxI ).c_str(), kome::ident::ValueType::TYPE_PROPERTY );
					currHit->addValue( "Protein fI", FMT( "%f", m_proteinGroup.fI ).c_str(), kome::ident::ValueType::TYPE_PROPERTY );
					if( hyperscore >= 0.0 ) {
						currHit->addValue( "hyperscore", FMT( "%f", hyperscore ).c_str(), kome::ident::ValueType::TYPE_SCORE );
						currHit->setPeptideScore( hyperscore );
					}
					if( nextscore >= 0.0 ) {
						currHit->addValue( "nextscore", FMT( "%f", nextscore ).c_str(), kome::ident::ValueType::TYPE_SCORE );
					}
					if( bscore >= 0.0 ) {
						currHit->addValue( "bscore", FMT( "%f", bscore ).c_str(), kome::ident::ValueType::TYPE_SCORE );
					}
					if( yscore >= 0.0 ) {
						currHit->addValue( "yscore", FMT( "%f", yscore ).c_str(), kome::ident::ValueType::TYPE_SCORE );
					}
					if( cscore >= 0.0 ) {
						currHit->addValue( "cscore", FMT( "%f", cscore ).c_str(), kome::ident::ValueType::TYPE_SCORE );
					}
					if( zscore >= 0.0 ) {
						currHit->addValue( "zscore", FMT( "%f", zscore ).c_str(), kome::ident::ValueType::TYPE_SCORE );
					}
					if( ascore >= 0.0 ) {
						currHit->addValue( "ascore", FMT( "%f", ascore ).c_str(), kome::ident::ValueType::TYPE_SCORE );
					}
					if( xscore >= 0.0 ) {
						currHit->addValue( "xscore", FMT( "%f", xscore ).c_str(), kome::ident::ValueType::TYPE_SCORE );
					}
					currHit->setProteinExpect( m_proteinInfo.expect );
					currHit->setPeptideExpect( expect );
				}
			}

			else if( m_currProteins.size() > 1 ) {    // alt substance
				m_currHits.back()->addAltSubstance( m_currProteins.back(), preStr.c_str(), postStr.c_str(), start, end );
			}
		}
	}
	else if( strcmp( name, AA_TAG ) == 0 ) {    // <aa>
		if( m_inPeptide && !m_currProteins.empty() && m_currPeptide != NULL ) {    // peptide
			// modification
			std::string type = attrs.getStringValue( "type", "" );
			const int pos = attrs.getIntValue( "at", -1 );
			const double mass = attrs.getDoubleValue( "modified", 0.0 );

			if( !type.empty() && pos >= 0 ) {
				kome::ident::Modification* m = NULL;
				double d = 0.1;

				for( unsigned int i = 0; i < m_mods.size(); i++ ) {
					kome::ident::Modification* tmp = m_mods[ i ];
					std::string residue = tmp->getAminoAcidResidue();

					if( residue.find( type ) != residue.npos
							|| ( residue.find( "N-term" ) != residue.npos && pos == m_currPeptide->getStart() )
							|| ( residue.find( "C-term" ) != residue.npos && pos == pos == m_currPeptide->getEnd() ) ) {
						const double tmpD = fabs( mass - tmp->getMonoMass() );
						if( tmpD < d ) {
							d = tmpD;
							m = tmp;
						}
					}
				}

				if( m!= NULL ) {
					const int acPos = pos - m_currPeptide->getStart();
					m_currPeptide->addModification( m->getName().c_str(), acPos );
					if( !m_currHits.empty() ) {
						m_currHits.back()->addModification( m, pos - m_currPeptide->getStart() + 1 );
					}
				}
			}
		}
	}
}

// end element
void XTandemXMLHandler::onEndElement( const char* name, const char* text ) {
	// manager
	kome::ident::IdentificationManager& identMgr = kome::ident::IdentificationManager::getInstance();

	// each tags
	if( strcmp( name, GROUP_TAG ) == 0 ) {    // </group>
		// depth
		m_groupDepth -= 1;

		if( m_groupDepth == 0 ) {    // protein group
			m_currProteins.clear();
			m_currPeptide = NULL;
			m_currHits.clear();
		}
	}
	else if( strcmp( name, PROTEIN_TAG ) == 0 ) {    // </protein>
		m_inProtein = false;
	}
	else if( strcmp( name, PEPTIDE_TAG ) == 0 ) {    // </peptide>
		m_inPeptide = false;
	}
	else if( strcmp( name, NOTE_TAG ) == 0 ) {    // <note>
		if( m_inProtein ) {    // protein
			// regex
			if( m_regex.empty() ) {
				for( unsigned int i = 0; i < (int)identMgr.getNumberOfLocalDbs() && m_regex.empty(); i++ ) {
					std::string tmpDb = identMgr.getLocalDb( i );
					if( tmpDb.compare( m_searchInfo.db.c_str() ) == 0 ) {
						m_regex = identMgr.getAccessionRegx( i );
					}
				}
			}

			// accession
			std::string description = NVL( text, "" );
			std::string acc = regexsearch( description.c_str(), m_regex.c_str() );

			// protein
			if( !acc.empty() ) {
				if( m_proteinMap.find( acc ) == m_proteinMap.end() ) {
					kome::ident::ProteinSubstance* protein
						= new kome::ident::ProteinSubstance( &m_result, acc.c_str(), description.c_str(), NULL );
					m_result.addSubstance( protein );
					m_proteinMap[ acc ] = protein;
					m_currProteins.push_back( protein );
				}
				else {
					kome::ident::ProteinSubstance* protein = m_proteinMap[ acc ];
					m_currProteins.push_back( protein );
				}
			}
		}
		else if( !m_currHits.empty() ) {    // spectrum information
			// spectrum information
			int charge = -1;
			kome::ident::SpecInfo specInfo;
			kome::ident::SpecInfo* spec = NULL;
			identMgr.ParseSpectrumTitle( text, specInfo, &charge );

			std::string key = FMT( "%d:%d:%s", specInfo.sampleIndex, specInfo.specId, specInfo.path.c_str() );
			if( m_specMap.find( key ) != m_specMap.end() ) {
				spec = m_specMap[ key ];
			}

			for( unsigned int i = 0; i < m_currHits.size(); i++ ) {
				if( spec != NULL ) {
					if( charge > 0 ) {
						m_currHits[ i ]->setCharge( charge );
					}
					m_currHits[ i ]->addSpectrum( spec );
				}
			}
		}
	}
}

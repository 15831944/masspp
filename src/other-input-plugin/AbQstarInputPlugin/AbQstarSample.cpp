/**
 * @file AbQstarSample.cpp
 * @brief implements of AbQstarSample class
 *
 * @author S.Tanaka
 * @date 2008.01.11
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "AbQstarSample.h"


using namespace kome::io::qstar;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



#define INSTRUMENT_NAME	   "QSTAR"
#define SOFTWARE_NAME		 "Analyst QS"


// constructor
AbQstarSample::AbQstarSample( kome::io::ab::AbSampleSet^ sampleSet ) : kome::io::ab::AbSample( sampleSet ) {
	setInstrument( INSTRUMENT_NAME );
	setSoftwareName( SOFTWARE_NAME );
}

// destructor
AbQstarSample::~AbQstarSample() {
}

// on open sample
bool AbQstarSample::onOpenSample( kome::clr::DataGroupNodeWrapper^ rootGroup ) {
	// default
	kome::io::ab::AbSample::onOpenSample( rootGroup );

	// logger
	kome::clr::ClrLogger^ logger = kome::clr::ClrLogger::getInstance();

	for( unsigned int i = 0; i < rootGroup->getNumberOfChildren(); i++ ) {  // each periods
		// period group
		kome::clr::DataGroupNodeWrapper^ periodGroup = rootGroup->getChild( i );

		for( unsigned int j = 0; j < periodGroup->getNumberOfChildren(); j++ ) {	// each experiments
			// experiment group
			kome::clr::DataGroupNodeWrapper^ expGroup = periodGroup->getChild( j );

			// IDA
			EXPLOREVBUTILSLib::PrecursorInfoColl^ coll = nullptr;
			
			try {
				EXPLOREVBUTILSLib::IDAInfoClass ida;
				ida.WiffFileName = m_sampleSet->getFilePath();
				ida.SampleID = m_sampleNum;
				ida.PeriodID = i;
				ida.ExperimentID = j - 1;

				bool bOvMS1(false);
				bool bNoPrc(true);

				for ( unsigned int s = 0; s < expGroup->getNumberOfSpectra(); s++ ) {
					kome::clr::SpectrumWrapper^ spec = expGroup->getSpectrum( s );
					if ( spec->getMsStage() > 1 ) {
						bOvMS1 = true;
					}
					if (spec->getPrecursor() > 0.0 ) {
						bNoPrc = false;
						if (bOvMS1) {
							break;
						}
					}
				}

				if ( bOvMS1 && bNoPrc ) {
					ida.GetPrecursorInfo();
					coll = ida.PrecursorInfoColl;
					if ( coll == nullptr ) {
						break;
					}

					unsigned int rsr = coll->Count;
					if ( rsr < expGroup->getNumberOfSpectra()) {
						rsr = expGroup->getNumberOfSpectra();
					}

					for ( unsigned int ikk = 0; ikk < rsr; ikk++) {
						double ddmy = coll->Item( ikk + 1 )->PrecursorMass;
						if ( ddmy > 0.0 ) {
							kome::clr::SpectrumWrapper^ spec = expGroup->getSpectrum( ikk );
							spec->setPrecursor( ddmy );
						}
					}
				}
			}
			catch( System::Exception^ ) {
			}			
		}
	}

	return true;
}

/**
 * @file LcmsSample.cpp
 * @brief implements of LcmsSample class
 *
 * @author S.Tanaka
 * @date 2008.02.08
 * 
 * Copyright(C) 2006-2013 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "LcmsSample.h"

#include "LcmsSampleSet.h"
#include "LcmsSpectrum.h"
#include "LcmsAveragedSpectrum.h"
#include "LcmsChromatogram.h"


using namespace kome::io::lcms;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



#define E_ERR1			 0x80040301
#define E_ERR2			 0x80040302
#define E_ERR3			 0x80040303

#define GC_INSTRUMENT_NAME	  "GCMS"
#define LC_INSTRUMENT_NAME    "LCMS-IT-TOF"
#define SOFTWARE_NAME	      "LCMS solution"

#define MASS_UNIT		  0.0001


// constructor
LcmsSample::LcmsSample( LcmsSampleSet^ sampleSet )
		: kome::clr::ClrSampleBase( sampleSet ) {
	// initialize
	m_sampleSet = sampleSet;

	m_creationDate = nullptr;
	m_creationOperator = nullptr;
	m_modificationDate = nullptr;
	m_modificationOperator = nullptr;
	m_sampleId = nullptr;
	m_trayName = nullptr;
	m_vial = nullptr;

	m_rts = nullptr;
	m_tics = nullptr;
	m_bpcs = nullptr;
	m_types = nullptr;
	m_precs = nullptr;
	m_others = nullptr;

	m_rootGroup = nullptr;

	m_allScanCnt = 0;

	m_exp = nullptr;
}

// destructor
LcmsSample::~LcmsSample() {
}

// get Lcms sample set
LcmsSampleSet^ LcmsSample::getLcmsSampleSet() {
	return m_sampleSet;
}

// set creation date
void LcmsSample::setCreationDate( System::DateTime^ date ) {
	m_creationDate = date;
}

// get creation date
System::DateTime^ LcmsSample::getCreationDate() {
	return m_creationDate;
}

// set creation operator
void LcmsSample::setCreationOperator( System::String^ op ) {
	m_creationOperator = op;
}

// get creation operator
System::String^ LcmsSample::getCreationOperator() {
	return m_creationOperator;
}

// set modification date
void LcmsSample::setModificationDate( System::DateTime^ date ) {
	m_modificationDate = date;
}

// get modification date
System::DateTime^ LcmsSample::getModificationDate() {
	return m_modificationDate;
}

// set modification operator
void LcmsSample::setModificationOperator( System::String^ op ) {
	m_modificationOperator = op;
}

// get modification operator
System::String^ LcmsSample::getModificationOperator() {
	return m_modificationOperator;
}

// set sample ID
void LcmsSample::setSampleId( System::String^ id ) {
	m_sampleId = id;
}

// get sample ID
System::String^ LcmsSample::getSampleId() {
	return m_sampleId;
}

// set tray name
void LcmsSample::setTrayName( System::String^ name ) {
	m_trayName = name;
}

// get tray name
System::String^ LcmsSample::getTrayName() {
	return m_trayName;
}

// set vial
void LcmsSample::setVial( System::String^ vial ) {
	m_vial = vial;
}

// get vial
System::String^ LcmsSample::getVial() {
	return m_vial;
}

// set description
void LcmsSample::setDesc( System::String^ desc ) {
	m_desc = desc;
}

// get description
System::String^ LcmsSample::getDesc() {
	return m_desc;
}

// set properties
void LcmsSample::setProperties( kome::clr::PropertiesWrapper^ props ) {
	// check the parameter
	if( props == nullptr ) {
		return;
	}

	// set properties
	props->setValue(
		"Creation Date",
		m_creationDate == nullptr ? "" : m_creationDate->ToString()
	);
	props->setValue( "Creation Operator", m_creationOperator );
	props->setValue(
		"Modification Date",
		m_modificationDate == nullptr ? "" : m_modificationDate->ToString()
	);
	props->setValue( "Modification Operator", m_modificationOperator );
	props->setValue( "Sample ID", m_sampleId );
	props->setValue( "Tray Name", m_trayName );
	props->setValue( "Vial", m_vial );
	props->setValue( "Sample Description", m_desc );

}

// create spectra
void LcmsSample::createSpectra() {
	// check members
	if( m_rts == nullptr ) {
		return;
	}

	// logger
	kome::clr::ClrLogger^ logger = kome::clr::ClrLogger::getInstance();

	// each scans
	for( int i = 0; i < m_allScanCnt; i++ ) {
		// attributes
		long scanNum = i + 1;
		int nRt = m_rts[ i ];
		double rt = (double)nRt / 60000.0;
		double tic = (double)m_tics[ i ];
		double precursor = (double)m_precs[ i ] * MASS_UNIT;
		long type = m_types[ i ];
		long other = m_others[ i ];
		long segNum = ( type >> 8 ) & 0xff;
		long evtNum = type & 0xff;
		long stage = other & 0xff;
		long scanType = ( type >> 16 ) & 0xff;
		long charge = ( type >> 24 ) & 0xff;

		// key
		int key = segNum * 1000 + evtNum;

		// event
		EventInfo^ evt = (EventInfo^)m_evtTable[ key ];

		// BPC
		long bpc = 0;
		System::String^ bpcKey = key.ToString() + ":" + nRt.ToString();
		if( m_bpcTable->Contains( bpcKey ) ) {
			bpc = (long)m_bpcTable[ bpcKey ];
		}
		m_bpcs[ i ] = bpc;

		// spectrum object
		if( evt->node != nullptr ) {
			System::String^ specName = "Scan " + scanNum.ToString();
			LcmsSpectrum^ spec = gcnew LcmsSpectrum( this, specName );
			evt->node->addSpectrum( spec );

			spec->setScanNumber( scanNum );
			spec->setRt( rt );
			spec->setTotalIntensity( tic );
			spec->setMaxIntensity( (double)bpc );
			spec->setMsStage( stage );
			if( stage > 1 ) {
				spec->setPrecursor( precursor );
			}
			spec->setScanType( scanType );
			spec->setSegmentNumber( segNum );
			spec->setEventNumber( evtNum );
			spec->setHasChromatogram( true );
		}
	}
}

// create precursor spectra
void LcmsSample::createPrecursorSpectra(
		kome::clr::DataGroupNodeWrapper^ rootGroup,
		cli::array< long >^ mzs,
		cli::array< long >^ ints,
		cli::array< long >^ types,
		cli::array< long >^ types2
) {
	// logger
	kome::clr::ClrLogger^ logger = kome::clr::ClrLogger::getInstance();

	// check parameters
	if( rootGroup == nullptr || mzs == nullptr || ints == nullptr
			|| types == nullptr || types2 == nullptr ) {
		return;
	}

	// precursor count
	int precCnt = mzs->Length;
	if( precCnt == 0 ) {
		return;
	}

	// group
	kome::clr::DataGroupNodeWrapper^ msGroup = rootGroup->createChildGroup( "Averaged Parents" );
	kome::clr::DataGroupNodeWrapper^ msmsGroup = rootGroup->createChildGroup( "Averaged MS/MS" );

	// each precursors
	for( int i = 0; i < precCnt; i++ ) {
		// precursor
		double precursor = (double)mzs[ i ] * MASS_UNIT;

		// spectrum
		LcmsAveragedSpectrum^ parentSpec = gcnew LcmsAveragedSpectrum( this, "Averaged Spectrum " + i.ToString() + " (Precursor=" + precursor.ToString() + ")" );
		LcmsAveragedSpectrum^ msmsSpec = gcnew LcmsAveragedSpectrum( this, "Averaged MS/MS Scan " + i.ToString() );

		msGroup->addSpectrum( parentSpec );
		msmsGroup->addSpectrum( msmsSpec );
		msmsSpec->setParentSpectrum( parentSpec );
			
		// type
		long type = types[ i ];
		long pol = ( type & 0xff );
		long eve = ( ( type >> 8 ) & 0xff );
		long stage = ( ( type >> 16 ) & 0xff );
		long charge = ( ( type >> 24 ) & 0xff );

		// type2
		long type2 = types2[ i ];
		long seg = ( type2 & 0xff );
		long eveType = ( ( type2 >> 8 ) & 0xff );
		long parent = ( ( type >> 16 ) & 0xffff );

		// precursor scans
		System::Object^ scanNumObjs = nullptr;
		System::Object^ eventObjs = nullptr;
		System::Object^ rtObjs = nullptr;
		System::Object^ loopObjs = nullptr;
		System::Object^ precScanObj = nullptr;

		m_exp->GetPrecursorScans( 0, i, scanNumObjs, eventObjs, rtObjs, loopObjs, precScanObj, 0 );
			
		cli::array< long >^ scanNums = (cli::array< long >^)scanNumObjs;
		cli::array< long >^ rts = (cli::array< long >^)rtObjs;
		cli::array< long >^ precScans = (cli::array< long >^ )precScanObj;

		int scanCnt = scanNums->Length;
		long msTic = 0;
		long msBpc = 0;
		double msStartRt = -1.0;
		double msEndRt = -1.0;

		long msmsTic = 0;
		long msmsBpc = 0;
		double msmsStartRt = -1.0;
		double msmsEndRt = -1.0;

		for( int j = 0; j < scanCnt; j++ ) {
			int parentScan = precScans[ j ];
			int scan = scanNums[ j ];
			parentSpec->addScan( parentScan );
			msmsSpec->addScan( scan );

			long tmp = m_tics[ parentScan - 1 ];
			msTic += tmp;
			tmp = m_bpcs[ parentScan - 1 ];
			if( tmp > msBpc ) {
				msBpc = tmp;
			}

			tmp = m_tics[ scan - 1 ];
			msmsTic += tmp;
			tmp = m_bpcs[ scan - 1 ];
			if( tmp > msmsBpc ) {
				msmsBpc = tmp;
			}

			double rt = (double)m_rts[ parentScan - 1 ] / 60000.0;
			if( msStartRt < 0.0 || rt < msStartRt ) {
				msStartRt = rt;
			}
			if( msEndRt < 0.0 || rt > msEndRt ) {
				msEndRt = rt;
			}

			rt = (double)m_rts[ scan - 1 ] / 60000.0;
			if( msmsStartRt < 0.0 || rt < msmsStartRt ) {
				msmsStartRt = rt;
			}
			if( msmsEndRt < 0.0 || rt > msmsEndRt ) {
				msmsEndRt = rt;
			}
		}

		// polarity
		kome::clr::SpectrumWrapper::Polarity polarity = kome::clr::SpectrumWrapper::Polarity::UNKNOWN;
		if( pol == 0 ) {
			polarity = kome::clr::SpectrumWrapper::Polarity::POSITIVE;
		}
		else if( pol == 1 ) {
			polarity = kome::clr::SpectrumWrapper::Polarity::NEGATIVE;
		}

		// attributes
		parentSpec->setMsStage( stage );
		parentSpec->setPolarity( polarity );
		parentSpec->setSegmentNumber( seg );
		parentSpec->setTotalIntensity( (double)msTic );
		parentSpec->setMaxIntensity( (double)msBpc );
		parentSpec->setHasChromatogram( false );
		parentSpec->setRt( msStartRt, msEndRt );

		msmsSpec->setMsStage( stage + 1 );
		msmsSpec->setPolarity( polarity );
		msmsSpec->setSegmentNumber( seg );
		msmsSpec->setEventNumber( eve );
		msmsSpec->setPrecursor( precursor );
		msmsSpec->setPrecursorCharge( charge );
		msmsSpec->setTotalIntensity( (double)msmsTic );
		msmsSpec->setMaxIntensity( (double)msmsBpc );
		msmsSpec->setHasChromatogram( false );
		msmsSpec->setRt( msmsStartRt, msmsEndRt );
	}
}

// on open sample
bool LcmsSample::onOpenSample( kome::clr::DataGroupNodeWrapper^ rootGroup ) {
	// logger
	kome::clr::ClrLogger^ logger = kome::clr::ClrLogger::getInstance();

	// file name
	System::String^ fileName = getSampleSet()->getFileName()->ToLower();

	// set instrument
	if( fileName->EndsWith( ".lcd" ) ) {
		setInstrument( LC_INSTRUMENT_NAME );
	}
	else {
		setInstrument( GC_INSTRUMENT_NAME );
	}
	setSoftwareName( SOFTWARE_NAME );
  
	// export object
	m_exp = m_sampleSet->getExport();

	if( m_exp == nullptr ) {
		return false;
	}

	// sample properties
	setProperties( rootGroup->getProperties() );

	// TIC
	System::Object^ rtObjs	= nullptr;
	System::Object^ ticObjs   = nullptr;
	System::Object^ typeObjs  = nullptr;
    System::Object^ precObjs  = nullptr;
	System::Object^ otherObjs = nullptr;
	m_exp->GetTIC( rtObjs, ticObjs, typeObjs, precObjs, otherObjs, m_allScanCnt );
	if( m_allScanCnt <= 0 ) {
		return true;
	}

	m_rts = (cli::array< long >^)rtObjs;
	m_tics = (cli::array< long >^)ticObjs;
	m_types = (cli::array< long >^)typeObjs;
	m_precs = (cli::array< long >^)precObjs;
	m_others = (cli::array< long >^)otherObjs;
	m_bpcs = gcnew cli::array< long >( m_allScanCnt );

	// precursor
	System::Object^ precMzObjs = nullptr;
	System::Object^ precTypeObjs = nullptr;
	System::Object^ precType2Objs = nullptr;
	System::Object^ precIntObjs = nullptr;

	try {
		m_exp->GetPrecursors( 0, precMzObjs, precTypeObjs, precType2Objs, precIntObjs, true,  -1, -1, -1, -1, -1);
	}
	catch( ... ) {
	}

	cli::array< long >^ precMzs = (cli::array< long >^)precMzObjs;
	cli::array< long >^ precInts = (cli::array< long >^)precIntObjs;
	cli::array< long >^ precTypes = (cli::array< long >^)precTypeObjs;
	cli::array< long >^ precTypes2 = (cli::array< long >^)precType2Objs;

	if( precMzs != nullptr ) {
		logger->info( precMzs->Length.ToString() + " Precursors" );
	}

	System::Collections::Generic::HashSet<int>^ keySet = gcnew System::Collections::Generic::HashSet<int>();
	if( precMzs != nullptr ) {
		for( int i = 0; i < precMzs->Length; i++ ) {
			// type
			long type = precTypes[ i ];
			int eve = ( ( type >> 8 ) & 0xff );

			// type2
			long type2 = precTypes2[ i ];
			int seg = ( type2 & 0xff );

			int key = seg * 1000 + eve;

			keySet->Add( key );
		}
	}

	// BPC
	int segCnt = int();
	m_exp->GetSegmentCount( segCnt );

    m_evtTable = gcnew System::Collections::Hashtable();
	m_bpcTable = gcnew System::Collections::Hashtable();

	// segments
	for( int i = 0; i < segCnt; i++ ) {
		// segment
		long segNum = i + 1;

		int evtCnt = int();
		int startTime = int();
		int endTime = int();
		System::String^ otherSegInfo = nullptr;
		m_exp->GetSegment( segNum, startTime, endTime, evtCnt, otherSegInfo );

		double startTime0 = (double)startTime / 60000.0;
		double endTime0 = (double)endTime / 60000.0;

		// events
		for( int j = 0; j < evtCnt; j++ ) {
			long evtNum = j + 1;
			int key = segNum * 1000 + evtNum;

			System::Object^ rtObjs	= nullptr;
			System::Object^ intObjs   = nullptr;

			// BPC
			try{
				int cnt = 0;
				m_exp->GetEventBPC( segNum, evtNum, rtObjs, intObjs, cnt, 1 );
				cli::array< long >^ rts = (cli::array< long >^)rtObjs;
				cli::array< long >^ ints = (cli::array< long >^)intObjs;


				for( int k = 0; k < cnt; k++ ) {
					System::String^ bpcKey = key.ToString() + ":" + rts[ k ].ToString();
					m_bpcTable[ bpcKey ] = ints[ k ];
				}
			}
			catch( ... ) {
			}

			// event information
			int type = 0;
			int stage = 0;
			int pol = 0;
			int startMz = 0;
			int endMz = 0;
			System::String^ other = nullptr;
			m_exp->GetEvent( segNum, evtNum, type, stage, pol, startMz, endMz, other );

			EventInfo^ evt = gcnew EventInfo();
			evt->pol = kome::clr::SpectrumWrapper::Polarity::UNKNOWN;
			if( pol == 0 ) {
				evt->pol = kome::clr::SpectrumWrapper::Polarity::POSITIVE;
			}
			else if( pol == 1 ) {
				evt->pol = kome::clr::SpectrumWrapper::Polarity::NEGATIVE;
			}
			evt->startMz = (double)startMz * MASS_UNIT;
			evt->endMz = (double)endMz * MASS_UNIT;
			evt->seg = segNum;
			evt->eve = evtNum;
			evt->stage = stage;
			evt->node = nullptr;

			if( keySet->Contains( key ) || precMzs == nullptr || precMzs->Length == 0 ) {
				System::String^ name = "MS";
				if( stage == 2 ) {
					name = "MS/MS";
				}
				else {
					name = "MS" + stage.ToString();
				}
				
				name = name + " : Segment=" + segNum.ToString();
				name = name + " [" + startTime0.ToString() + "-" + endTime0.ToString() + "]";
				name = name + ", Event=" + evtNum.ToString();
				name = name + " [" + evt->startMz.ToString() + "-" + evt->endMz.ToString() + "]";

				evt->node = rootGroup->createChildGroup( name );
			}

			m_evtTable[ key ] = evt;				
		}
	}

	// create spectra
	createSpectra();

	// create precursor spectra
	createPrecursorSpectra( rootGroup, precMzs, precInts, precTypes, precTypes2 );

	// chromatograms
	createChromatograms();

    return true;
}

// create chromatograms
void LcmsSample::createChromatograms() {
	// logger
	kome::clr::ClrLogger^ logger = kome::clr::ClrLogger::getInstance();

	// each events
	System::Collections::ICollection^ keys = m_evtTable->Keys;
	System::Collections::IEnumerator^ enu = keys->GetEnumerator();

	while( enu->MoveNext() ) {
		int key = (int)enu->Current;
		EventInfo^ evt = (EventInfo^)m_evtTable[ key ];

		if( evt != nullptr && evt->node != nullptr ) {
			bool flg = ( evt->startMz == evt->endMz );
			double prec = -1.0;
			
			for( unsigned int i = 0; flg && i < evt->node->getNumberOfSpectra(); i++ ) {
				kome::clr::SpectrumWrapper^ spec = evt->node->getSpectrum( i );

				if( prec < 0.0 ) {
					prec = spec->getPrecursor();
				}
				else if( prec != spec->getPrecursor() ) {
					flg = false;
				}
			}
		
			if( flg && evt->startMz == evt->endMz ) {
				LcmsChromatogram^ chrom = gcnew LcmsChromatogram( this, evt->seg, evt->eve );
				chrom->setIcon( prec >= 0.0 ? "MRM_chrom" : "SRM_chrom" );
				System::String^ name = ( prec >= 0.0 ? "MRM:" : "SRM:" );
				bool commaFlg = false;

				kome::clr::SpectrumWrapper^ spec = evt->node->getSpectrum( 0 );
				int scanNum = spec->getScanNumber();

				logger->info( "Scan = " + scanNum.ToString() );

				// get data
				System::Object^ mzObjs = nullptr;
				System::Object^ intObjs = nullptr;
				int count = int();

				m_exp->GetScanData( scanNum, mzObjs, intObjs, count, 1 );

				cli::array< long >^ mzs = (cli::array< long >^)mzObjs;
				cli::array< long >^ ints = (cli::array< long >^)intObjs;

				double q1 = -1.0;
				if( mzs->Length >= 2 ) {
					q1 = (double)mzs[ 1 ] * MASS_UNIT;
				}
				double q3 = -1.0;
				if( mzs->Length >= 1 ) {
					q3 = (double)mzs[ 0 ] * MASS_UNIT;
				}
				
				if( q1 > 0.0 ) {
					chrom->getProperties()->setDoubleValue( "Q1 Mass", q1 );
					name = name + "Q1=" + q1.ToString();
					commaFlg = true;
				}

				if( q3 > 0.0 ) {
					chrom->getProperties()->setDoubleValue( "Q3 Mass", q3 );
					if( commaFlg ) {
						name = name + ", ";
					}
					name = name + "Q3=" + q3.ToString();
				}

				chrom->setName( name );
				evt->node->addChromatogram( chrom );
			}
		}
	}
}

// on close sample
bool LcmsSample::onCloseSample() {
	return true;
}

// detect peaks by API
void LcmsSample::onDetectPeaksByAPI( kome::clr::SpectrumWrapper^ spec, kome::clr::PeaksWrapper^ peaks ) {
	// spectrum
	LcmsSpectrumBase^ lcmsSpec = dynamic_cast< LcmsSpectrumBase^ >( spec );
	if( lcmsSpec == nullptr ) {
		return;
	}

	// detect peaks
	lcmsSpec->getApiPeaks( peaks );
	return;
}

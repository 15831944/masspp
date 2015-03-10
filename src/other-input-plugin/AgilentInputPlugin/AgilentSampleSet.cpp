/**
 * @file AgilentSampleSet.cpp
 * @brief implements of AgilentSampleSet class
 *
 * @author S.Tanaka
 * @date 2009.05.27
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#include "stdafx.h"
#include "AgilentSampleSet.h"

#include "AgilentSample.h"
#include "AgilentInputManager.h"

using namespace kome::io::agilent;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
AgilentSampleSet::AgilentSampleSet() {
}

// destructor
AgilentSampleSet::~AgilentSampleSet() {
}

// on open file
bool AgilentSampleSet::onOpenFile( System::String^ path ) {
	// reader object
	int filemode = AGMODE::ERRORTYPE;
	try {
		Agilent::MassSpectrometry::DataAnalysis::IMsdrDataReader^ reader
			= gcnew Agilent::MassSpectrometry::DataAnalysis::MassSpecDataReader();
		if( !reader->OpenDataFile(path) ) {
			return false;
		}
		cli::array<double>^ mois;
		filemode = checkSampleMode(reader, mois);
		reader->CloseDataFile();
	} catch(...) {
		return false;
	}

	// sample
	if(filemode == AGMODE::ERRORTYPE) {
//		LOG_ERROR(FMT("not support type."));
		return false;
	}

	addAgilentSample(path, "", filemode, -1);

	return true;
}

// Sample�̃��[�h���`�F�b�N����
int AgilentSampleSet::checkSampleMode(
	Agilent::MassSpectrometry::DataAnalysis::IMsdrDataReader^ reader,
	cli::array<double >^% moints
){
	namespace agilentdll = Agilent::MassSpectrometry::DataAnalysis;
	using namespace agilentdll;

	IBDAMSScanFileInformation^ pMSscanfileinf = reader->MSScanFileInformation;
	int scnum = pMSscanfileinf->TotalScansPresent;
	IBDAActuals^ act = reader->ActualsInformation;

	MSScanType sctype = pMSscanfileinf->ScanTypes;
	MSStorageMode storage = pMSscanfileinf->SpectraFormat;
	DeviceType devtype = pMSscanfileinf->DeviceType;

	//ScanType�ƁAFixedCycle..�܂�A�������Ă邩�ۂ��𔻒�B
	bool fcldpFg = pMSscanfileinf->IsFixedCycleLengthDataPresent();

	if (sctype == MSScanType::MultipleReaction) {
		// MRM�Ȃ̂ŁAMRM���[�h�Ƃ���
		return AGMODE::MRM; // AS MRMCODE
	} else if(fcldpFg && sctype == MSScanType::PrecursorIon) {
		// Precursor Ion & Segment������Ԃł���B
		// Precursor �̋敪���Q�ȏ゠��ꍇ�́ASEGMENT�Ƃ��ĕԂ��B
		cli::array<IBDAMSScanTypeInformation^ >^ arr 
			= pMSscanfileinf->GetMSScanTypeInformation();
		moints = arr[0]->MzOfInterest;
		if (moints->Length > 1) {
			return AGMODE::SEGMENT;
		} else {
			return AGMODE::DEF;
		}
	} else if (sctype == MSScanType::Scan) {
		return AGMODE::DEF;
	} else if(
		sctype == (MSScanType::ProductIon|MSScanType::Scan)
		&& storage == MSStorageMode::ProfileSpectrum
		&& devtype == DeviceType::QuadrupoleTimeOfFlight) {
		//�����ł��Ȃ��p�^�[��
		return AGMODE::ERRORTYPE;
	}

	// �Y���Ȃ��B�s���ƕԂ��Ēʏ폈��
	return AGMODE::UNKNOWN;
}

// Agilent�̃t�@�C���Ƃ��Ēǉ�����
void AgilentSampleSet::addAgilentSample(
	System::String^ fpath,
	System::String^ spName, 
	const int mode, 
	const double moi
){
	AgilentSample^ sample = gcnew AgilentSample(this, fpath ,mode,moi);
	if(spName->Length >0) {
		sample->setName(spName);
	}
	addSample(sample);
}


// on close file
bool AgilentSampleSet::onCloseFile() {
	// remove sample set
	AgilentInputManager^ mgr = AgilentInputManager::getInstance();
	mgr->removeSampleSet( this );

//	if(reader->OpenDataFile( path )

	return true;
}

// on open sample
bool AgilentSampleSet::onOpenSample(kome::clr::SampleWrapper^ sample) {
	return true;
}

// on close sample
bool AgilentSampleSet::onCloseSample( kome::clr::SampleWrapper^ sample ) {
	return true;
}

/**
 * @file ThermoSampleSetSampleSet.cpp
 * @brief interfaces of ThermoSampleSet class
 *
 * @author M.Fukuda
 * @date 2011.10.14
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#include "stdafx.h"
#include "ThermoSampleSet.h"
#include "ThermoSample.h"
#include "RawCtrl.h"

#include <comutil.h>
#include <stdio.h>
#include <io.h>
#include <fcntl.h>

using namespace kome::io::thermo;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


// constructor
//ThermoSampleSet::ThermoSampleSet() : m_XRCtrl(NULL), b_opend(false){
//}
ThermoSampleSet::ThermoSampleSet() : b_opend(false){
CoInitialize( NULL );
	m_RawCtrl = new RawCtrl();
}

// destructor
ThermoSampleSet::~ThermoSampleSet() {
	delete m_RawCtrl;
//	CoUninitialize();
}

RawCtrl* ThermoSampleSet::getRawCtrl() {
	onOpenRawFile( this->getFilePath());
	if ( b_opend ) {
		return m_RawCtrl;
	} else {
		return NULL;
	}
}


// on open sample
bool ThermoSampleSet::onOpenSample( kome::objects::Sample* sample ) {
	return true;
}

// on close sample
bool ThermoSampleSet::onCloseSample( kome::objects::Sample* sample ) {

	int count(0);
	for ( unsigned int i = 0; i < getNumberOfSamples(); i++ ) {
		kome::objects::Sample* s = getSample(i);
		if (s->isOpened()) {
			count++;
		}
	}

	if (count == 0){
		onCloseRawFile();
	}

	return true;
}

bool ThermoSampleSet::onOpenFile( const char* path ) {

	if ( ! onOpenRawFile( path ) ) {
		return false;
	}
	long ctrlNum(0);
	// 現状、タイプセレクタを　MS(Controller Type MS = 0) のみ呼んでいる
	// 旧ライブラリではセレクタがここに入っている
	// しかしMS以外は正常処理できないので不要だと思うけど・・
	m_RawCtrl->getMSCtrlNum( &ctrlNum );
	for ( long setCtrlNum = 1; setCtrlNum <= ctrlNum; setCtrlNum++) {
		ThermoSample* sample = new ThermoSample( this, 0, setCtrlNum );
		std::string sampleName = "MS";
		if ( ctrlNum > 1 ) {
			sampleName.append(FMT("(%d)", setCtrlNum));
		}
		sample->setName(sampleName.c_str());
		this->addSample( sample );
	}

	onCloseRawFile();

	return true;
 }

 bool ThermoSampleSet::onCloseFile() {
	return onCloseRawFile();
}

// on open Raw file
bool ThermoSampleSet::onOpenRawFile( const char* path ) {
	// Already Opend Files?
	if (b_opend) {
		return true;
	}

	// Path Check
	FILE *fp = NULL;
	fopen_s(&fp, path, "r");
	if (! fp) {
		LOG_ERROR(FMT("Can not Find Path : %s", path));
		return false;
	}
	fclose(fp);

	// open Files
	if ( ! m_RawCtrl->openMyFile(path) ) {
		return false;
	} else {
		b_opend = true;
	}
	return true;
}

// on close file
bool ThermoSampleSet::onCloseRawFile() {
	m_RawCtrl->closeMyFile();
	b_opend = false;
	return true;
}


/**
 * @file ThermoSpectrum.cpp
 * @brief interfaces of Thermo Spectrum class header
 *
 * @author M.Fukuda
 * @date 2012.04.16
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#include "stdafx.h"
#include "ThermoSpectrum.h"
#include "ThermoSample.h"
#include "RawCtrl.h"
#include "B_DataLabels.h"

using namespace kome::io::thermo;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


ThermoSpectrum::ThermoSpectrum( ThermoSample* thermoSample, const char* pcName ) : 
	kome::objects::Spectrum( thermoSample, pcName ), m_sample(thermoSample) {}

ThermoSpectrum::~ThermoSpectrum(){}

// SetSpectra Status
// Sampleの中でSpectrumに情報を与えていた部分をこちらに分離。
void ThermoSpectrum::setSpecStatus(const long nSpecNum) {
	setScanNumber( nSpecNum );
	RawCtrl* rawCtrl = m_sample->getRawCtrl();

	// get Filter
	std::string sFilter;
	rawCtrl->getFilterFromScanNum(nSpecNum, sFilter);
	setHasChromatogram(true);
	setFilter(sFilter);
	
	// get Centroid
	bool isCent;
	rawCtrl->IsCentroidScanForScanNum( nSpecNum, &isCent );
	setCentroidMode( isCent );

	long nMSOrder;
	rawCtrl->getMSOrder( nSpecNum, &nMSOrder );
	setMsStage( nMSOrder );
	if ( nMSOrder > 1 ) {
		double dPrec;
		rawCtrl->getPrecursor( nSpecNum, nMSOrder, &dPrec );
		setPrecursor( dPrec );
	}

	// begging get Scan Headers
	long nPackets = 0;
	double dRT(0.0), dMinMZ(-1.0), dMaxMZ(-1.0);
	double dTIC = 0.0;
	double dBasePeakMass = 0.0;
	double dBasePeakIntensity = 0.0;
	long nChannels = 0;
	long bUniformTime = FALSE;
	double dFrequency = 0.0;

	rawCtrl->getScanHeader(nSpecNum, &nPackets, &dRT, &dMinMZ, &dMaxMZ,
		&dTIC, &dBasePeakMass, &dBasePeakIntensity, &nChannels, bUniformTime, &dFrequency);
	
	std::string specName = FMT("Scan %ld : %s", nSpecNum, sFilter.c_str());
	setTitle( specName.c_str());
	setRt( dRT );
	setXRange( dMinMZ, dMaxMZ );
	setTotalIntensity( dTIC );
	setBasePeakMass( dBasePeakMass );
	setMaxIntensity( dBasePeakIntensity );
}

// onGet XYData
void ThermoSpectrum::onGetXYData( kome::core::XYData* const xyData, const double minX, const double maxX ){
	if (xyData == NULL) { return; } // Error!
	RawCtrl* rawCtrl = m_sample->getRawCtrl();
	// 旧関数の最後のポイントで判断していた部分を
	// 予め範囲を与えてThermoのライブラリに該当箇所を返させる
	if (rawCtrl == NULL) {return;} // Error!!

	if ( minX < 0.0 && maxX < 0.0 ) {
		// Full Range
		rawCtrl->getSpecData(getScanNumber(),xyData);
	} else {
		double dMinX = ( minX < 0.0 ) ? getMinX() : minX;
		double dMaxX = ( maxX < 0.0 ) ? getMaxX() : maxX;
		rawCtrl->getSpecDataWithRange(getScanNumber(), dMinX, dMaxX, xyData);
	}
}

// SetFilter
void ThermoSpectrum::setFilter( const std::string& sFilter ) {
	m_sFilter = sFilter;
	// check Have Chrom
//	this->setHasChromatogram(sFilter.find("Full") != std::string::npos );
	this->setHasChromatogram(!sFilter.empty());
	
	bool IsPOSITIVE = (sFilter.find("+") != std::string::npos );
	if ( IsPOSITIVE ) {
		setPolarity ( kome::objects::Spectrum::POLARITY_POSITIVE );
	} else {
		setPolarity ( kome::objects::Spectrum::POLARITY_NEGATIVE );
	}
	if (sFilter.find( "SIM" ) != std::string::npos ) {
		setIcon( "SIM_spec" );
	} else if( sFilter.find( "cnl" ) != std::string::npos ) {
		setIcon( "cnl_spec" );
	}
	this->getProperties().setValue("Filter",sFilter.c_str());
}


void ThermoSpectrum::onGetXRange( double* minX, double* maxX ){
	*minX = getMinX();
	*maxX = getMaxX();
}

// TotalIntensity (XIC)を返す
double ThermoSpectrum::onGetTotalIntensity( const double minX, const double maxX ){
	return getXIC_BPC(minX, maxX, m_stXic, true);
}

// BPCを返す
double ThermoSpectrum::onGetMaxIntensity( const double minX, const double maxX ){
	return getXIC_BPC(minX, maxX, m_stBpc, false);
}

// XIC,BPCの格納パターンをみて、未取得なら再取得、再配置を行う。
// 取得済みなら各々の値を返す(TotalとMaxの処理部分を一括化)
const double ThermoSpectrum::getXIC_BPC( const double _dMinX, const double _dmaxX, const stDataCtrl& orgCtrl, bool bXIC ){
	if ( hasChromatogram() ) {
		double dMinX = ( _dMinX < 0.0 ) ? getMinX() : _dMinX;
		double dMaxX = ( _dmaxX < 0.0 ) ? getMaxX() : _dmaxX;
		if ( fabs( dMinX - orgCtrl.dMzL ) < 1.0E-6 && fabs ( dMaxX - orgCtrl.dMzR ) < 1.0E-6 ) {
			return orgCtrl.dIntensity;
		} else {
			if (ReCalcXICorBPC_DataWithRange(dMinX, dMaxX, getGroup(), bXIC)) {
				return orgCtrl.dIntensity;
			} else {
				return -1.0;
			}
		}
	} else {
		kome::core::DataPoints dps;
		onGetXYData( &dps, _dMinX, _dmaxX );
		double intensity(0.0);

		for ( unsigned int i = 0; i < dps.getLength(); i++ ){
			if (bXIC) {
				intensity += dps.getY(i);
			} else {
				if ( intensity < dps.getY(i) ){
					intensity = dps.getY(i);
				}
			}
		}
		return intensity;
	}
}

// XIC,BPCに値をセットする
void ThermoSpectrum::setXICorBPC( const double dMzL, const double dMzR, const double dIntensity, bool bXic) {
	if (bXic) {
		m_stXic.dIntensity = dIntensity;
		m_stXic.dMzL = dMzL;
		m_stXic.dMzR = dMzR;
	} else {
		m_stBpc.dIntensity = dIntensity;
		m_stBpc.dMzL = dMzL;
		m_stBpc.dMzR = dMzR;
	}
}

//指定されたLR幅でXICかBPCをライブラリから取得して各スペクトラムに渡す
const bool ThermoSpectrum::ReCalcXICorBPC_DataWithRange( const double dMzL, const double dMzR, DGN* pDGN, bool bXic){
	RawCtrl* pRawCtrl = m_sample->getRawCtrl();

	kome::core::DataPoints dps;
	unsigned int nDps(0);
	std::string sFilter;

	//	pRawCtrl->getFilterFromScanNum( getScanNumber(), sFilter );
//	std::string sMzRange = FMT("%0.6lf-%0.6lf", dMzL, dMzR);
	std::string sMzRange = FMT("%0.2lf-%0.2lf", dMzL, dMzR);
//	bool ret = pRawCtrl->getChromAllXIC( &dps, sFilter.c_str(), sMzRange.c_str());
	bool ret = pRawCtrl->getChromAllXIC( NULL, sMzRange.c_str(), &dps);

	if ( ! ret ) { return false;}
	long xx = pDGN->getNumberOfSpectra();
	for ( unsigned int idx = 0; idx < pDGN->getNumberOfSpectra(); idx++ ){
		ThermoSpectrum* spec = dynamic_cast < ThermoSpectrum* >(pDGN->getSpectrum(idx));

		if ( spec != NULL && spec->hasChromatogram()) {
			spec->setXICorBPC( dMzL, dMzR, dps.getY(nDps), bXic );
			nDps++;
		}
	}
	return true;
}

const bool ThermoSpectrum::getVenderSpecturmPeakValues(kome::objects::Peaks* peaks) {
	const long myScanNo = this->getScanNumber();
	if (this->isCentroidMode()) {
		LOG_WARN(FMT("The Spectrum : %s is Centroid Mode. Can not get Vender Peaks.",this->getName()));
		return false;
	}

	EMS::BO::DataLabels tmpDLs;
	RawCtrl* pRawCtrl = m_sample->getRawCtrl();
	double* rstArr = nullptr;
	int rstSize = 0;

	pRawCtrl->set_ResizeLabelFunc(boost::bind(&EMS::BO::DataLabels::ReserveSize, &tmpDLs, _1));
	pRawCtrl->set_AddDataLabelFunc(boost::bind(&EMS::BO::DataLabels::AddPoint, &tmpDLs, _1, _2));
	bool ret = pRawCtrl->getTgtScanLabelDatas(myScanNo);
	if (ret) {
		tmpDLs.outputMyDataLabels(rstArr, &rstSize);
		for (int i = 0; i < rstSize; i++) {
			peaks->addPoint(rstArr[i*4], rstArr[i*4+1]);
		}
		delete[] rstArr;
	}
	return true;
}

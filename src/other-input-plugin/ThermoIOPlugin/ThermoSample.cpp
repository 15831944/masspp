/**
 * @file ThermoSample.cpp
 * @brief Thermo Type Sample Class
 *
 * @author M.Fukuda
 * @date 2012.04.16
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#include "stdafx.h"
#include "ThermoSample.h"

#include <boost/regex.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/assign.hpp>

#include "ThermoSampleSet.h"
#include "RawCtrl.h"
#include "ThermoSpectrum.h"
#include "ThermoChromatogram.h"

using namespace kome::io::thermo;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


// constructor
ThermoSample::ThermoSample(
	ThermoSampleSet* sampleSet,
	const int _type, 
	const long _ctrl
) : kome::objects::Sample(sampleSet)
	, m_sampleSet(sampleSet)
	, m_type(_type)
	, m_controller(_ctrl)
{
	sCtrlMap = boost::assign::map_list_of(-1,"no-device")(0,"MS")(1,"Analog")(2,"A/D card")(3,"PDA")(4,"UV");
	m_name = FMT( "Type : %s Name : %s ", sCtrlMap[m_type].c_str(), sampleSet->getFileName());
	setSupportedAPIPeaks(true);

}

// destructor
ThermoSample::~ThermoSample() 
{
}

RawCtrl* ThermoSample::getRawCtrl() 
{
	return m_sampleSet->getRawCtrl();
}

const bool ThermoSample::getRawCtrlwithMyTypes(RawCtrl* pRawCtrl_)
{
	pRawCtrl_ = m_sampleSet->getRawCtrl();
	return pRawCtrl_->SetCurrentController( m_type, m_controller );
}


bool ThermoSample::onOpenSample( kome::objects::DataGroupNode* rootGroup ) 
{
	RawCtrl* rawCtrl = m_sampleSet->getRawCtrl();

	rawCtrl->SetCurrentController( m_type, m_controller );
	kome::core::Properties& props = rootGroup->getProperties();

	setSoftwareName( "XCalibur" );
	props.setValue( "Controller Type", sCtrlMap[m_type].c_str() );
	props.setIntValue( "Controller Number", m_controller );
	setSoftwareVersion(rawCtrl->getInstSoftwareVersion().c_str());

	setInstrument(rawCtrl->getInstrumentName().c_str());
	rawCtrl->setSampleInfo( props );

	// get First to End
	long nFstScan = 0, nLastScan = 0, nTotalScan = 0;
	rawCtrl->getSpecScanRange( &nFstScan, &nLastScan, &nTotalScan );

	// check Filter having SRM
	bool bIsSRM(false);

	rawCtrl->IsHaveSRMData(nFstScan, &bIsSRM);
	if ( bIsSRM ) {
		createChrom(rawCtrl, rootGroup);
	} else {
		createSpec(rawCtrl, nFstScan, nLastScan, rootGroup);
	}
	return true;
}

bool ThermoSample::onCloseSample() {
	return false;
}

// Create Spectrum
const bool ThermoSample::createSpec(
	RawCtrl* rawCtrl, 
	const long nFstScan, 
	const long nLastScan,
	DGN* rootGroup
) {
	double dReso;
	ThermoSpectrum* prevSpecArr[10];
	for ( size_t x = 0; x < 10; x ++ ) {
		prevSpecArr[x] = NULL;
	}

	rawCtrl->getResolution( &dReso );
	for ( long nScan = nFstScan; nScan < nLastScan; nScan++ ) {
		std::string sName = FMT("Scan %ld", nScan);
		ThermoSpectrum* spec = new ThermoSpectrum( this, sName.c_str() );
		// ヘッダー由来のデータを投入
		spec->setSpecStatus( nScan );
		spec->setResolution( dReso );
		int nStage = spec->getMsStage();

		// as Orignal
		if ( nStage >= 10 ){
			nStage = 9;
		}

		if ( nStage > 1 ) {
			ThermoSpectrum* parSpec = prevSpecArr[ nStage - 1 ];
			spec->setParentSpectrum( parSpec );
		}
		prevSpecArr[ nStage ] = spec;
		rootGroup->addSpectrum( spec );
	}
	return true;
}

// Get MRM Data (Chromatogram)
const bool ThermoSample::createChrom(RawCtrl* rawCtrl, DGN* rootGroup) {
	std::vector<std::string> sFilArr;
	if (! rawCtrl->getFiltes(sFilArr) ) {
		return false;
	}
	
	// get SRM Parces
	boost::regex regSRM("SRM ms2 ([0-9]+[.|0-9]+) (\\[)([^\\]]+)(\\])");
	boost::regex regQ3(",");
	boost::smatch rstReg;
	
	//名前の生成ルール: "Range" + rangeN + "[Q1=" + Q1 + ",Q3=" + Q3 + "]";
	int nRangeX(1);

	size_t nfilSize = sFilArr.size();
	for ( size_t x = 0; x < nfilSize; x++ ) {
		if ( boost::regex_search(sFilArr[x], rstReg, regSRM) ) {
			// parse q1, q3 block
			std::string q1str  = rstReg.str(1);
			std::string q3Str  = rstReg.str(3);
			std::vector<std::string> q3Arr;
			boost::regex_split(std::back_inserter(q3Arr), q3Str, regQ3);

			double q1 = todouble(q1str.c_str(),0.0);

			// split q3 block
			for ( size_t q3cnt = 0; q3cnt < q3Arr.size(); q3cnt++) {
				ThermoChromatogram* chrom = new ThermoChromatogram(this);
				chrom->setFilter(sFilArr[x]);
				chrom->setMRMStatus(nRangeX, q1, q3Arr.at(q3cnt));
				rootGroup->addChromatogram( chrom );
				nRangeX++;
			}
		}
	}
	return true;
}

// Virtual (get Peaks From Dll's API)
void ThermoSample::onDetectPeaksByAPI(
	kome::objects::Spectrum* spec,
	kome::objects::Peaks* peaks
){
	ThermoSpectrum* tspec =  dynamic_cast<ThermoSpectrum*>(spec);
	if (tspec != nullptr) {
		tspec->getVenderSpecturmPeakValues(peaks);
	}
}

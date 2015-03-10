/**
 * @file ThermoChromatogram.cpp
 * @brief Thermo MSFileReader Chromatogram Objects File
 *
 * @author M.Fukuda
 * @date 2012.04.16
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#include "stdafx.h"
#include "ThermoChromatogram.h"

#include <boost/format.hpp>
#include <boost/algorithm/string.hpp>

#include "ThermoSample.h"
#include "RawCtrl.h"

using namespace kome::io::thermo;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


ThermoChromatogram::ThermoChromatogram(ThermoSample* thermoSample) 
	: kome::objects::Chromatogram(thermoSample), m_sample(thermoSample) {
}

ThermoChromatogram::~ThermoChromatogram(){}

// setup MRM status
void ThermoChromatogram::setMRMStatus( const int rangeX, const double& q1, const std::string& q3Str ) {

	mzRange = q3Str;
	std::vector<std::string> rstArr;
	boost::split(rstArr, q3Str, boost::is_any_of("-")); // -‚Å•ªŠ„
	
	double q3f = todouble(trimstring(rstArr[0].c_str()).c_str(),0.0);
	double q3l = todouble(trimstring(rstArr[1].c_str()).c_str(),0.0);

	double q3 = (q3f + q3l) / 2.0;
	boost::format chromN("Range%d [Q1=%.4lf,Q3=%.4lf]");
	std::cout << chromN % rangeX % q1 % q3;
	
	setName(str(chromN).c_str());
	setTitle(str(chromN).c_str());
	setIcon( "MRM_chrom" );
	m_precursor = q1;
	m_mass = q3;

	kome::core::Properties* props = &this->getProperties();
	props->setIntValue( "Range Number", rangeX);
	props->setDoubleValue( "Q1 Mass", q1 );
	props->setDoubleValue( "Q3 Mass", q3 );

}

void ThermoChromatogram::setFilter( const std::string& filter ) {
	m_filter = filter;
}

void ThermoChromatogram::onGetXYData( kome::core::XYData* const xyData ){
	RawCtrl* rawCtrl = m_sample->getRawCtrl();
	rawCtrl->getChromAllXIC(m_filter.c_str(), mzRange.c_str(), xyData);
}


/**
 * @file BackgroundSubtraction.cpp
 * @brief implements of BackgroundSubtraction class
 *
 * @author S.Tanaka
 * @date 2009.12.08
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "BackgroundSubtraction.h"


using namespace kome::background;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
BackgroundSubtraction::BackgroundSubtraction() {
	setName( "Background Subtract" );

	m_minRt = -1.0;
	m_maxRt = -1.0;
}

// destructor
BackgroundSubtraction::~BackgroundSubtraction() {
}

// clear background points
void BackgroundSubtraction::clearBackground() {
	m_bg.clearPoints();
}

// set background points
void BackgroundSubtraction::setBackground( kome::core::XYData& xyData ) {
	// clear
	clearBackground();

	// add points
	const unsigned int num = xyData.getLength();
	for( unsigned int i = 0; i < num; i++ ) {
		m_bg.addPoint( xyData.getX( i ), xyData.getY( i ) );
	}
}

// set RT range
void BackgroundSubtraction::setRt( const double minRt, const double maxRt ) {
	m_minRt = minRt;
	m_maxRt = maxRt;
	m_bg.clearPoints();
}

// get the number of background points
unsigned int BackgroundSubtraction::getNumberOfPoints() {
	return m_bg.getLength();
}

// get the x-coordinate of the background point
double BackgroundSubtraction::getX( const unsigned int idx ) {
	if( idx >= m_bg.getLength() ) {
		return -1.0;
	}
	return m_bg.getX( idx );
}

// get the y-coordinate of the background point
double BackgroundSubtraction::getY( const unsigned int idx ) {
	if( idx >= m_bg.getLength() ) {
		return -1.0;
	}
	return m_bg.getY( idx );
}

// subtract background
void BackgroundSubtraction::subtractBackground(
		kome::core::XYData& src,
		kome::core::XYData& dst
) {
	// x array
	std::vector< double > xArr;
	for( unsigned int i = 0; i < src.getLength(); i++ ) {
		xArr.push_back( src.getX( i ) );
	}
	if( xArr.size() == 0 ) {
		return;
	}

	// background points
	kome::core::DataPoints bg;
	kome::numeric::Interpolation::linear( m_bg, bg, &( xArr[ 0 ] ), xArr.size() );

	// SPEC 83485: The result of background subtraction looks odd.
	// バックグランド(m_bg)のm/zの範囲がサブトラクト対象(xArr)のm/z範囲より小さい場合、
	// Interpolation処理でbgを補間する際にm_bgに含まれない範囲を補間しようとして、
	// 範囲外のYDataに値を与えてしまう。範囲外のYData(intensity)は0.0にするよう修正。
	{// @date 2011/12/15 <Add> OKADA ------->

		for( unsigned int i = 0; i < bg.getLength(); i++ ) {
			// p_dXorg[0]より小さい p_dX[x]に関しては、p_dY[]の値を0にする
			if(  bg.getX(i) < m_bg.getMinX() ){
				bg.updatePoint( i,  bg.getX(i), 0.0 );
			}

			// p_dXorg[last]より大きい p_dX[x]に関しては、p_dY[]の値を0にする
			if( m_bg.getMaxX() <  bg.getX(i) ){
				bg.updatePoint( i,  bg.getX(i), 0.0 );
			}
		}
	}	// @date 2011/12/15 <Add> OKADA <-------

	// subtracted points
	for( unsigned int i = 0; i < src.getLength(); i++ ) {
		dst.addPoint( src.getX( i ), std::max( 0.0, src.getY( i ) - bg.getY( i ) ) );
	}
}

// update chromatogram
void BackgroundSubtraction::onUpdate(
		kome::core::XYData& src,
		kome::core::XYData& dst,
		kome::objects::Chromatogram& chrom
) {
	subtractBackground( src, dst );
}

// update spectrum
void BackgroundSubtraction::onUpdate(
		kome::core::XYData& src,
		kome::core::XYData& dst,
		kome::objects::Spectrum& spec
) {
	// set background
	kome::objects::Sample* sample = spec.getSample();
	if( sample != m_sample && m_minRt >= 0.0 && m_maxRt >= 0.0 ) {
		// get data set
		kome::objects::DataSet ds;
		sample->getRootDataGroupNode()->getDataSet( &ds );

		// averaged spectrum
		kome::objects::AveragedSpectrum aveSpec( ds.getGroup() );
	
		for( unsigned int i=0; i < ds.getNumberOfSpectra(); i++ ){
			kome::objects::Spectrum* s = ds.getSpectrum( i );
			double rt = s->getRt();
			// パラメータの範囲内 スペクトル取得
			if( m_minRt < rt && rt < m_maxRt ){
				if( s->getMsStage() == 1 ){
					aveSpec.addSpectrum( s );
				}
			}
		}

		kome::core::DataPoints background;
		aveSpec.getXYData( &background, -1.0, -1.0, true );

		setBackground( background );
	}

	// subtract
	subtractBackground( src, dst );
}

// update sample
void BackgroundSubtraction::onUpdate(
		kome::core::XYData& src,
		kome::core::XYData& dst,
		kome::objects::Sample& sample
) {
	subtractBackground( src, dst );
}

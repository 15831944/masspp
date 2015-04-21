/**
 * @file AveragedSpectrum.cpp
 * @brief implements of AveragedSpectrum class
 *
 * @author S.Tanaka
 * @date 2006.11.06
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "AveragedSpectrum.h"

#include "Sample.h"
#include "DataGroupNode.h"

#include <math.h>
#include <list>
#include <utility>


using namespace kome::objects;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
AveragedSpectrum::AveragedSpectrum( DataGroupNode* group )
		: Spectrum( ( group == NULL ? NULL : group->getSample() ), "" ) {
	m_d = 0.05;
	setGroup( group );
}

// destructor
AveragedSpectrum::~AveragedSpectrum() {
}

// add spectrum
void AveragedSpectrum::addSpectrum( Spectrum* spectrum ) {
	// update stage
	if( m_spectra.size() == 0 ) {
		setMsStage( spectrum->getMsStage() );
	}
	else if( spectrum->getMsStage() != getMsStage() ) {
		setMsStage( -1 );
	}

	// update RT
	double startRt = getStartRt();
	if( spectrum->getStartRt() >= 0.0
			&& ( spectrum->getStartRt() < startRt || startRt < 0.0 ) ) {
		setStartRt( spectrum->getStartRt() );
	}
	double endRt = getEndRt();
	if( spectrum->getEndRt() >= 0.0
			&& ( spectrum->getEndRt() > endRt || endRt < 0.0 ) ) {
		setEndRt( spectrum->getEndRt() );
	}

	// TIC, BPC, BPI
	if( m_spectra.size() == 0 ) {
		setTotalIntensity( spectrum->getTotalIntensity() );
		setMaxIntensity( spectrum->getMaxIntensity() );
		setBasePeakMass( spectrum->getBasePeakMass() );
	}
	else {
		// TIC
		double tic = getTotalIntensity() * (double)m_spectra.size();
		tic += spectrum->getTotalIntensity();
		tic /= (double)( m_spectra.size() + 1 );
		setTotalIntensity( tic );

		// BPC, BPI
		if( spectrum->getMaxIntensity() > getMaxIntensity() ) {
			setMaxIntensity( spectrum->getMaxIntensity() );
			setBasePeakMass( spectrum->getBasePeakMass() );
		}
	}

	// precursor
	double precursor = 0.0;
	int precCnt = 0;
	for( unsigned int i = 0; i < m_spectra.size(); i++ ) {
		double tmpPrec = m_spectra[ i ]->getPrecursor();
		if( tmpPrec > 0.0 ) {
			precursor += tmpPrec;
			precCnt++;
		}
	}
	if( precCnt > 0 ) {
		precursor /= (double)precCnt;
		setPrecursor( precursor );
	}

	// add spectrum to array
	m_spectra.push_back( spectrum );

	// update title @date 2011.02.07 <Add> M.Izumi
	updateTitle();
}

// get the number of spectra
unsigned int AveragedSpectrum::getNumberOfSpectra() {
	return m_spectra.size();
}

// get spectrum
Spectrum* AveragedSpectrum::getSpectrum( const unsigned int idx ) {
	if( idx >= m_spectra.size() ) {
		return NULL;
	}
	return m_spectra[ idx ];
}

// set merge distance
void AveragedSpectrum::setMergeDistance( const double d ) {
	m_d = std::max( d, 0.0 );
}

// get merge distance
double AveragedSpectrum::getMergeDistance() {
	return m_d;
}

// update title
void AveragedSpectrum::updateTitle() {
	// name
	std::string name = "Average Spectrum";
	if( getStartRt() >= 0.0 && getEndRt() >= 0.0 ) {
		name += FMT( " [%.4f - %.4f]", getStartRt(), getEndRt() );
	}
	setName( name.c_str() );

	// title
	name += FMT( " %d spectra", m_spectra.size() );
	setTitle( name.c_str() );
}

// get xy data
void AveragedSpectrum::onGetXYData(
		kome::core::XYData* const xyData,
		const double minX,
		const double maxX
) {
	// check spectra
	if( m_spectra.size() == 0 ) {
		return;
	}

	bool isProfile = false;;
	for( unsigned int i = 0; i < m_spectra.size() && !isProfile; i++ ) {
		if( !m_spectra[ i ]->isCentroidMode() ) {
			isProfile = true;
		}
	}

	setCentroidMode( !isProfile );


	// data points
	kome::core::DataPoints dps;
	if( m_spectra.size() > 0 ) {
		getAveragedDataPoints( &( m_spectra[ 0 ] ), m_spectra.size(), dps );

		for( unsigned int i = 0; i < dps.getLength(); i++ ) {
			const double x = dps.getX( i );
			const double y = dps.getY( i ) / (double)m_spectra.size();

			if( ( minX < 0.0 || x >= minX ) && ( maxX < 0.0 || x <= maxX ) ) {
				xyData->addPoint( x, y );
			}
		}
	}
}

// get x range
void AveragedSpectrum::onGetXRange( double* minX, double* maxX ) {
	double minXX = double();
	double maxXX = double();

	// get x range
	if( m_spectra.size() > 0 ) {
		minXX = m_spectra[ 0 ]->getMinX();		
		maxXX = m_spectra[ 0 ]->getMaxX();

		for( unsigned int i = 1; i < m_spectra.size(); i++ ) {
			double tmpMinX = m_spectra[ i ]->getMinX();
			double tmpMaxX = m_spectra[ i ]->getMaxX();

			minXX = MIN( minXX, tmpMinX );
			maxXX = MAX( maxXX, tmpMaxX );
		}
	}

	// store
	*minX = minXX;
	*maxX = maxXX;
}

// get total intensity
double AveragedSpectrum::onGetTotalIntensity( const double minX, const double maxX ) {
	// get total intensity
	double intensity = 0.0;
	for( unsigned int i = 0; i < m_spectra.size(); i++ ) {
		intensity += m_spectra[ i ]->getTotalIntensity( minX, maxX );
	}
	intensity /= (double)m_spectra.size();

	return intensity;
}

// get max intensity
double AveragedSpectrum::onGetMaxIntensity( const double minX, const double maxX ) {
	// get max intensity
	double intensity = 0.0;
	for( unsigned int i = 0; i < m_spectra.size(); i++ ) {
		double y = m_spectra[ i ]->getMaxIntensity( minX, maxX );
		if( y > intensity ) {
			intensity = y;
		}
	}

	return intensity;
}

// get the averaged data points
void AveragedSpectrum::getAveragedDataPoints( kome::objects::Spectrum** spectra, const unsigned int num, kome::core::XYData& xyData ) {
	// check parameters
	if( spectra == NULL || num == 0 ) {
		return;
	}

	// get data points
	if( num == 1 ) {
		spectra[ 0 ]->getXYData( &xyData, -1.0, -1.0, false );
	}
	else {
		unsigned int mid = num / 2;

		kome::core::DataPoints dps0;
		getAveragedDataPoints( spectra, mid, dps0 );

		kome::core::DataPoints dps1;
		getAveragedDataPoints( spectra + mid, num - mid, dps1 );

		mergeDataPoints( dps0, dps1, xyData );
	}
}

// merge data points
void AveragedSpectrum::mergeDataPoints( kome::core::XYData& src0, kome::core::XYData& src1, kome::core::XYData& dst ) {
	// points
	class Point {
	public:
		Point() {};
		virtual ~Point() {};
	public:
		double x;
		double y;
		int spec;
	public:
		static bool lessPoint( Point& p0, Point& p1 ) {
			return ( p0.x < p1.x );
		};
	};
	std::vector< Point > points;

	double d = 0.05;
	double prevX = -1.0;

	// first data points
	const unsigned int len0 = src0.getLength();
	for( unsigned int i = 0; i < len0; i++ ) {
		const double x = src0.getX( i );
		const double y = src0.getY( i );
		if( y > 0.0 ) {
			points.resize( points.size() + 1 );
			points.back().x = x;
			points.back().y = y;
			points.back().spec = 0;

			if( prevX > 0.0 ) {
				double tmpD = fabs( x - prevX );
				if( tmpD < d ) {
					d = tmpD;
				}
			}
			prevX = x;
		}
	}

	// second data points
	const unsigned int len1 = src1.getLength();
	for( unsigned int i = 0; i < len1; i++ ) {
		const double x = src1.getX( i );
		const double y = src1.getY( i );
		if( y > 0.0 ) {
			points.resize( points.size() + 1 );
			points.back().x = x;
			points.back().y = y;
			points.back().spec = 1;

			if( prevX > 0.0 ) {
				double tmpD = fabs( x - prevX );
				if( tmpD < d ) {
					d = tmpD;
				}
			}
			prevX = x;
		}
	}

	// sort
	std::sort( points.begin(), points.end(), Point::lessPoint );

	// add point
	std::set< int > specSet;
	double sumX = 0.0;
	double sumY = 0.0;
	for( unsigned int i = 0; i < points.size(); i++ ) {
		const double x = points[ i ].x;
		const double y = points[ i ].y;
		const int spec = points[ i ].spec;

		if( !specSet.empty() ) {
			double tmpX = sumX / sumY;
			if( specSet.find( spec ) != specSet.end() || tmpX + d < x ) {
				double tmpY = sumY;
				dst.addPoint( tmpX, tmpY );
				specSet.clear();
			}
		}

		if( specSet.empty() ) {
			sumX = x * y;
			sumY = y;
		}
		else {
			sumX = sumX + x * y;
			sumY = sumY + y;
		}

		specSet.insert( spec );
	}

	if( !specSet.empty() ) {
		double tmpX = sumX / sumY;
		double tmpY = sumY;
		dst.addPoint( tmpX, tmpY );
	}
}

/**
 * @file Ab3dAlignment.cpp
 * @brief implements of Ab3dAlignmentclass
 *
 * @author S.Tanaka
 * @date 2008.08.11
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "Ab3dAlignment.h"


using namespace kome::ab3d;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



#define FITTING_METHOD_PARAM_NAME		  "fit_method"


// fitting method array
kome::core::ConversionElement< std::string, Ab3dAlignment::FittingMethod > g_ab3dFittingMethodConversionArray[] = {
	{ "linear_interpolation", Ab3dAlignment::FITTING_LINEAR_INTERPOLATION },
	{ "spline_interpolation", Ab3dAlignment::FITTING_SPLINE_INTERPOLATION },
	{ "least_square_linear",  Ab3dAlignment::FITTING_LEAST_SQUARE_LINEAR },
	{ "least_square_cubic",   Ab3dAlignment::FITTING_LEAST_SQUARE_CUBIC },
	{ "logistic_fitting",	 Ab3dAlignment::FITTING_LOGISTIC_REGRESSION }
};


// constructor
Ab3dAlignment::Ab3dAlignment( kome::objects::SettingParameterValues* settings ) {
	// settings
	m_settings = settings;

	// fitting method
	const char* fittingMethod = NULL;
	if( settings != NULL ) {
		fittingMethod = m_settings->getParameterValue( FITTING_METHOD_PARAM_NAME );
	}
	std::string s = tolowercase( fittingMethod );
	m_fittingMethod = CONVERT_FROM_ARRAY( s, FITTING_LINEAR_INTERPOLATION, g_ab3dFittingMethodConversionArray );
}

// destructor
Ab3dAlignment::~Ab3dAlignment() {
}

// get correction points
bool Ab3dAlignment::getCorrectionPoints( kome::core::XYData& corrPts, kome::core::XYData& tbPts ) {
	// RT
	kome::objects::Sample* sample = getTreatmentSample();
	if( sample == NULL ) {
		return true;
	}

	kome::objects::DataSet dataSet;
	sample->getRootDataGroupNode()->getDataSet( &dataSet );

	const unsigned int num = dataSet.getNumberOfSpectra();
	std::vector< double > rts;
	for( unsigned int i = 0; i < num; i++ ) {
		kome::objects::Spectrum* spec = dataSet.getSpectrum( i );
		const double rt = spec->getRt();
		if( rt >= 0.0 ) {
			rts.push_back( rt * 60.0 );
		}
	}

	std::sort( rts.begin(), rts.end() );

	if( rts.empty() ) {
		return true;
	}

	// fitting
	bool ret = false;
	if( m_fittingMethod == FITTING_SPLINE_INTERPOLATION ) { // spline interpolation
		ret = executeSplineInterpolation( corrPts, tbPts, &( rts[ 0 ] ), num );
	}
	else if( m_fittingMethod == FITTING_LEAST_SQUARE_LINEAR ) {		// least square method (linear polynomial)
		ret = executeLeastSquareLinear( corrPts, tbPts, &( rts[ 0 ] ), num );
	}
	else if( m_fittingMethod == FITTING_LEAST_SQUARE_CUBIC ) {		// least square method (cubic polynomial)
		ret = executeLeastSquareCubic( corrPts, tbPts, &( rts[ 0 ] ), num );
	}
	else if( m_fittingMethod == FITTING_LOGISTIC_REGRESSION ) {		// logistic regression
		ret = executeLogisticRegression( corrPts, tbPts, &( rts[ 0 ] ), num );
	}
	else {  // linear interpolation
		ret = executeLinearInterpolation( corrPts, tbPts, &( rts[ 0 ] ), num );
	}

	return ret;
}

// linear interpolation
bool Ab3dAlignment::executeLinearInterpolation( kome::core::XYData& corrPts, kome::core::XYData& tbPts, double* rts, const unsigned int num ) {
	// check the number of data points
	if( tbPts.getLength() < 2 ) {
		LOG_WARN_CODE( FMT( "The number of traceback points is too little." ), ERR_OTHER );
		return false;
	}

	// linear interpolation
	kome::core::DataPoints dps;
	kome::numeric::Interpolation::linear( tbPts, dps, rts, num );

	// add points
	corrPts.reserve( num );
	for( unsigned int i = 0; i < num; i++ ) {
		// x, y
		double x = dps.getX( i ) / 60.0;
		double y = dps.getY( i ) / 60.0;

		// add
		corrPts.addPoint( x, y );
	}

	return true;
}

// spline interpolation
bool Ab3dAlignment::executeSplineInterpolation( kome::core::XYData& corrPts, kome::core::XYData& tbPts, double* rts, const unsigned int num ) {
	// check the number of data points
	if( tbPts.getLength() < 4 ) {
		LOG_WARN_CODE( FMT( "The number of traceback points is too little." ), ERR_OTHER );
		return false;
	}

	// linear interpolation
	kome::core::DataPoints dps;
	kome::numeric::Interpolation::spline( tbPts, dps, rts, num );

	// add points
	corrPts.reserve( num );
	for( unsigned int i = 0; i < num; i++ ) {
		// x, y
		double x = dps.getX( i ) / 60.0;
		double y = dps.getY( i ) / 60.0;

		// add
		corrPts.addPoint( x, y );
	}

	return true;
}

// least square method (linear polynomial)
bool Ab3dAlignment::executeLeastSquareLinear( kome::core::XYData& corrPts, kome::core::XYData& tbPts, double* rts, const unsigned int num ) {
	// detect linear polynomial
	kome::core::Vector vec( 2 );

	if( !kome::numeric::LeastSquares::getPolynomia( tbPts, 1, vec ) ) {
		LOG_WARN_CODE( FMT( "Failed to get the linear polynomial" ), ERR_OTHER );
		return false;
	}
	LOG_INFO( FMT( "AB3D Alignment - Fitting Function (y = %f + %fx)", vec( 0 ), vec( 1 ) ) );

	// add points
	corrPts.reserve( num );
	for( unsigned int i = 0; i < num; i++ ) {
		// x, y
		double x = rts[ i ];
		double y = vec( 0 ) + vec( 1 ) * x;

		// add points
		corrPts.addPoint( x / 60.0, y / 60.0 );
	}
	return true;
}

// least square method (cubic polynomial)
bool Ab3dAlignment::executeLeastSquareCubic( kome::core::XYData& corrPts, kome::core::XYData& tbPts, double* rts, const unsigned int num ) {
	// detect linear polynomial
	kome::core::Vector vec( 4 );

	if( !kome::numeric::LeastSquares::getPolynomia( tbPts, 3, vec ) ) {
		LOG_WARN_CODE( FMT( "Failed to get the linear polynomial" ), ERR_OTHER );
		return false;
	}
	LOG_INFO( FMT( "AB3D Alignment - Fitting Function (y = %f + %fx + %fx^2 + %fx^3)", vec( 0 ), vec( 1 ), vec( 2 ), vec( 3 ) ) );

	// add points
	corrPts.reserve( num );
	for( unsigned int i = 0; i < num; i++ ) {
		// x, y
		double x = rts[ i ];
		double y = vec( 0 ) + vec( 1 ) * x + vec( 2 ) * x * x + vec( 3 ) * x * x * x;

		// add points
		corrPts.addPoint( x / 60.0, y / 60.0 );
	}
	return true;
}

// logistic regression fitting
bool Ab3dAlignment::executeLogisticRegression( kome::core::XYData& corrPts, kome::core::XYData& tbPts, double* rts, const unsigned int num ) {
	// check the number of data points
	if( tbPts.getLength() < 4 ) {
		LOG_WARN_CODE( FMT( "The number of traceback points is too little." ), ERR_OTHER );
		return false;
	}

	// parameters
	double lower = tbPts.getMinY();
	double upper = tbPts.getMaxY();
	double x50   = ( tbPts.getMinX() + tbPts.getMaxX() ) / 3.0;
	double hill  = 3.0;

	double vertices[] = {
		lower,	   upper,	   x50,	   hill,
		lower - 1.0, upper,	   x50,	   hill,
		lower,	   upper + 1.0, x50,	   hill,
		lower,	   upper,	   x50 * 2.0, hill,
		lower,	   upper,	   x50,	   hill + 1.0
	};
	double evals[ 5 ];

	// fitting
	kome::numeric::MinimaxSolution::downhillSimplex(
		vertices,
		evals,
		4,
		boost::bind( getSumOfDifferentialSquareOfLogistic, _1, &tbPts ),
		1.0e-10,
		5000
	);
	lower = vertices[ 0 ];
	upper = vertices[ 1 ];
	x50   = vertices[ 2 ];
	hill  = vertices[ 3 ];

	LOG_INFO(
		FMT(
			"AB3D Alignment - Fitting Function (y = %f + ( %f - %f ) / ( 1.0 + (%f/x)^%f )",
			lower,
			upper,
			lower,
			x50,
			hill
		)
	);

	// add points
	corrPts.reserve( num );
	for( unsigned int i = 0; i < num; i++ ) {
		// x, y
		double x = rts[ i ];
		double y = lower + ( upper - lower ) / ( 1.0 + pow( x50 / x, hill ) );

		// add points
		corrPts.addPoint( x / 60.0, y / 60.0 );
	}

	return true;
}

// get spectra
void Ab3dAlignment::getSpectra( kome::objects::Sample* sample, kome::objects::DataSet& spectra ) {
	// parameter
	if( sample == NULL ) {
		return;
	}

	// spectra
	for( unsigned int i = 0; i < sample->getNumberOfGroups(); i++ ) {
		kome::objects::DataGroupNode* group = sample->getGroup( i );
		for( unsigned int j = 0; j < group->getNumberOfSpectra(); j++ ) {
			kome::objects::Spectrum* spec = group->getSpectrum( j );
			if( spec->getMsStage() == 1 && spec->getRt() >= 0.0 ) {
				spectra.addSpectrum( spec );
			}
		}
	}

	// sort
	spectra.sortSpectra();
}

// get sum of differential square of logistic regression function
double Ab3dAlignment::getSumOfDifferentialSquareOfLogistic( double* vertex, kome::core::XYData* dps ) {
	// get parameters
	double lower = vertex[ 0 ];
	double upper = vertex[ 1 ];
	double x50   = vertex[ 2 ];
	double hill  = vertex[ 3 ];

	// calculate
	double s = 0.0;
	for( unsigned int i = 0; i < dps->getLength(); i++ ) {
		// x, y
		double x = dps->getX( i );
		double y = dps->getY( i );

		// diff
		if( x > 0.0 ) {
			double f = lower + ( upper - lower ) / ( 1.0 + pow( x50 / x, hill ) );
			double diff = fabs( y - f );

			s += diff * diff;
		}		
	}

	return s;
}

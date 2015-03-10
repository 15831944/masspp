/**
 * @file XYDataListCtrl.cpp
 * @brief implements of XYDataListCtrl class
 *
 * @author S.Tanaka
 * @date 2007.11.12
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "XYDataListCtrl.h"

#include <boost/bind.hpp>


using namespace kome::view;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG




BEGIN_EVENT_TABLE( XYDataListCtrl, kome::window::GridListCtrl )
END_EVENT_TABLE()


// constructor
XYDataListCtrl::XYDataListCtrl( wxWindow* parent ) 
		: kome::window::GridListCtrl( parent, wxID_ANY, 200, 300, false ) {
	// initialize
	m_spectrum = NULL;
	m_chromatogram = NULL;
	m_xyData = NULL;
	m_peaksFlg = false;
}

// destructor
XYDataListCtrl::~XYDataListCtrl() {
}

// set spectrum
void XYDataListCtrl::setData( kome::objects::Spectrum* spec, kome::core::XYData* xyData, const bool peaksFlg ) {
	// set members
	m_spectrum = spec;
	m_xyData = xyData;
	m_peaksFlg = peaksFlg;
	if( spec != NULL ) {
		m_chromatogram = NULL;
	}
	
	// clear data
	deselectAll();
	clearData( false );

	// add data
	if( xyData != NULL ) {
		for( unsigned int i = 0; i < xyData->getLength(); i++ ) {
			addData( i, false );
		}
	}

	// create
	create();

	// update list
	updateList();
}

// set chromatogram
void XYDataListCtrl::setData( kome::objects::Chromatogram* chrom, kome::core::XYData* xyData, const bool peaksFlg ) {
	// set member
	m_chromatogram = chrom;
	m_xyData = xyData;
	m_peaksFlg = peaksFlg;
	if( chrom != NULL ) {
		m_spectrum = NULL;
	}

	// clear data
	deselectAll();
	clearData( false );

	// add data
	if( xyData != NULL ) {
		for( unsigned int i = 0; i < xyData->getLength(); i++ ) {
			addData( i, false );
		}
	}

	// create
	create();

	// update list
	updateList();
}

// create
void XYDataListCtrl::create() {
	// initialize column
	while( GetColumnCount() > 0 ) {
		DeleteColumn( 0 );
	}

	// get title
	std::string xTitle = "X";
	std::string yTitle = "Y";
	if( m_spectrum != NULL ) {
		kome::objects::DataGroupNode* group = m_spectrum->getGroup();
		xTitle = group->getSpecXTitle();
		yTitle = group->getSpecYTitle();
	}
	if( m_chromatogram != NULL ) {
		xTitle = m_chromatogram->getGroup()->getChromXTitle();
		yTitle = m_chromatogram->getGroup()->getChromYTitle();
	}

	// insert  column
	addColumn( xTitle.c_str(), TYPE_UDOUBLE );
	addColumn( yTitle.c_str(), TYPE_DOUBLE );
	if( m_peaksFlg ) {
		addColumn( "Left", TYPE_UDOUBLE );
		addColumn( "Apex", TYPE_UDOUBLE );
		addColumn( "Right", TYPE_UDOUBLE );
		addColumn( "Area", TYPE_UDOUBLE );
		addColumn( "FWHM", TYPE_UDOUBLE );
	}

	// get data
	m_xArray.clear();
	m_yArray.clear();
	m_leftArray.clear();
	m_rightArray.clear();
	m_apexArray.clear();
	m_areaArray.clear();
	m_fwhmArray.clear();

	if( m_xyData != NULL && m_xyData->getLength() > 0 ) {
		m_xArray.resize( m_xyData->getLength() );
		m_yArray.resize( m_xyData->getLength() );

		if( m_peaksFlg ) {
			m_leftArray.resize( m_xyData->getLength() );
			m_rightArray.resize( m_xyData->getLength() );
			m_apexArray.resize( m_xyData->getLength() );			
			m_areaArray.resize( m_xyData->getLength() );
			m_fwhmArray.resize( m_xyData->getLength() );
		}

		for( unsigned int i = 0; i < m_xyData->getLength(); i++ ) {
			m_xArray[ i ] = m_xyData->getX( i );
			m_yArray[ i ] = m_xyData->getY( i );

			if( m_peaksFlg ) {
				kome::objects::Peaks* peaks = (kome::objects::Peaks*)m_xyData;
				kome::objects::PeakElement* peak = peaks->getPeak( i );

				m_leftArray[ i ] = peak->getLeftX();
				m_rightArray[ i ] = peak->getRightX();
				m_apexArray[ i ] = peak->getApexX();
				m_areaArray[ i ] = peak->getArea();
				m_fwhmArray[ i ] = peak->getFwhm();
			}
		}
	}
}

// get double value
double XYDataListCtrl::getDouble( const long long data, const int column ) const {
	// get value
	double v = -1.0;
	if( column == 0 ) {		// x
		if( data >= 0 && data < (int)m_xArray.size() ) {
			v = m_xArray[ data ];
		}
	}
	else if( column == 1 ) {		// y
		if( data >= 0 && data < (int)m_yArray.size() ) {
			v = m_yArray[ data ];
		}
	}
	else if( column == 2 ) {		// left
		if( data >= 0 && data < (int)m_leftArray.size() ) {
			v = m_leftArray[ data ];
		}
	}
	else if( column == 3 ) {		// apex
		if( data >= 0 && data < (int)m_apexArray.size() ) {
			v = m_apexArray[ data ];
		}
	}
	else if( column == 4 ) {		// right
		if( data >= 0 && data < (int)m_rightArray.size() ) {
			v = m_rightArray[ data ];
		}
	}
	else if( column == 5 ) {		// area
		if( data >= 0 && data < (int)m_areaArray.size() ) {
			v = m_areaArray[ data ];
		}
	}
	else if( column == 6 ) {	// FWHM
		if( data >= 0 && data < (int)m_fwhmArray.size() ) {
			v = m_fwhmArray[ data ];
		}
	}

	return v;
}

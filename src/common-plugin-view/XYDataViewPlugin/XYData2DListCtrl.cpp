/**
 * @file XYData2DListCtrl.cpp
 * @brief implements of XYData2DListCtrl class
 *
 * @author okada_h
 * @date 2011/05/25
 * 
 * Copyright(C) 2014 Shimadzu Corporation. All rights reserved.
 */


#include "stdafx.h"
#include "XYData2DListCtrl.h"
#include "XYDataViewManager.h"

#include <boost/bind.hpp>


using namespace kome::view;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



enum peak_kind {	// m_peaksFlg用
	FLG_DATA,
	FLG_PEAK,
	FLG_2DPEAK
};


BEGIN_EVENT_TABLE( XYData2DListCtrl, kome::window::GridListCtrl )
	EVT_LIST_ITEM_SELECTED(   wxID_ANY, XYData2DListCtrl::onSelectItem )
	EVT_LIST_ITEM_DESELECTED( wxID_ANY, XYData2DListCtrl::onSelectItem )
	EVT_LIST_ITEM_FOCUSED(    wxID_ANY, XYData2DListCtrl::onSelectItem )
END_EVENT_TABLE()

// constructor
XYData2DListCtrl::XYData2DListCtrl( wxWindow* parent ) : kome::window::GridListCtrl( parent, wxID_ANY, 200, 300, false ) {
	// initialize
	m_spectrum = NULL;
	m_chromatogram = NULL;
	m_xyData = NULL;
	m_peaksFlg = FLG_DATA;
	
	m_group = NULL;
	
	// create list
	create2D();

	// Context Menu
	setPopupMenuFlag( POPUP_SELECT | POPUP_DESELECT | POPUP_COPY | POPUP_FILTER | POPUP_SAVE );

}

// destructor
XYData2DListCtrl::~XYData2DListCtrl() {
}


// set spectrum
void XYData2DListCtrl::setData( kome::objects::Spectrum* spec, kome::core::XYData* xyData, const bool peaksFlg ) {
	// set members
	m_spectrum = spec;
	m_xyData = xyData;
	m_peaksFlg = peaksFlg ? FLG_PEAK : FLG_DATA;
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
	createXYData();	// @date 2011/05/25 <Add> OKADA

	// update list
	updateList();
}

// set chromatogram
void XYData2DListCtrl::setData( kome::objects::Chromatogram* chrom, kome::core::XYData* xyData, const bool peaksFlg ) {
	// set member
	m_chromatogram = chrom;
	m_xyData = xyData;
	m_peaksFlg = peaksFlg ? FLG_PEAK : FLG_DATA;
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
	createXYData();	// @date 2011/05/25 <Add> OKADA

	// update list
	updateList();
}

void XYData2DListCtrl::setGroup( kome::objects::DataGroupNode* group ) {
	m_peaksFlg = FLG_2DPEAK;
	
	// clear
	SetItemCount( 0 );
	kome::view::XYDataViewManager& mgr = kome::view::XYDataViewManager::getInstance();	//@date 2011/05/12 <Add> OKADA
	mgr.clearSelectedPeaks();

	// set
	m_group = group;
	if( group == NULL ) {
		Refresh();
		return;
	}

	// initialize
	deselectAll();
	clearData( false );

	// create list
	create2D();

	// set peaks
	kome::objects::PeaksManager& pkMgr = kome::objects::PeaksManager::getInstance();
	kome::objects::Peaks2D* peaks = pkMgr.getPeaks( group );
	if( peaks != NULL && peaks->getNumberOfPeaks() > 0 ) {
		for( unsigned int i = 0; i < peaks->getNumberOfPeaks(); i++ ) {
			addData( (long long)peaks->getPeak( i ), false );
		}
	}

	// update
	updateList();
}

// create
void XYData2DListCtrl::createXYData()	// @date 2011/05/25 <Add> OKADA
{
	// initialize column
	while( GetColumnCount() > 0 ) {
		DeleteColumn( 0 );
	}

	// get title
	std::string xTitle = "X";
	std::string yTitle = "Y";
	if( m_spectrum != NULL ) {
		kome::objects::DataGroupNode* group = m_spectrum->getGroup();
		// >>>>>>	@Date:2013/05/31	<Modify>	A.Ozaki
		// こちらの下記同様チェックは行うようにします
		if  ( (kome::objects::DataGroupNode *)NULL != group )
		{
			xTitle = group->getSpecXTitle();
			yTitle = group->getSpecYTitle();
		}
		// <<<<<<	@Date:2013/05/31	<Modify>	A.Ozaki
	}
	if( m_chromatogram != NULL ) {
		// >>>>>>	@Date:2013/05/31	<Modify>	A.Ozaki
		// Detect Peaksで得られたデータの場合、group情報が存在しないので、
		// NULLになっている場合があります。
		if  ( (kome::objects::DataGroupNode *)NULL != m_chromatogram->getGroup( ) )
		{
			xTitle = m_chromatogram->getGroup()->getChromXTitle();
			yTitle = m_chromatogram->getGroup()->getChromYTitle();
		}
		// <<<<<<	@Date:2013/05/31	<Modify>	A.Ozaki
	}

	// insert  column
	addColumn( xTitle.c_str(), TYPE_UDOUBLE );
	addColumn( yTitle.c_str(), TYPE_DOUBLE );

	if( m_peaksFlg == FLG_PEAK ) 
	{
		addColumn( "Left (X)", TYPE_UDOUBLE );
		addColumn( "Left (Y)", TYPE_UDOUBLE );
		addColumn( "Apex (X)", TYPE_UDOUBLE );
		addColumn( "Apex (Y)", TYPE_UDOUBLE );
		addColumn( "Right (X)", TYPE_UDOUBLE );
		addColumn( "Right (Y)", TYPE_UDOUBLE );
		addColumn( "Area", TYPE_UDOUBLE );
		addColumn( "FWHM", TYPE_UDOUBLE );
	}

	// get data
	m_xArray.clear();
	m_yArray.clear();
	m_lxArray.clear();
	m_lyArray.clear();
	m_rxArray.clear();
	m_ryArray.clear();
	m_axArray.clear();
	m_ayArray.clear();
	m_areaArray.clear();
	m_fwhmArray.clear();

	if( m_xyData != NULL && m_xyData->getLength() > 0 ) {
		m_xArray.resize( m_xyData->getLength() );
		m_yArray.resize( m_xyData->getLength() );

		if( m_peaksFlg == FLG_PEAK ) 
		{
			m_lxArray.resize( m_xyData->getLength() );
			m_lyArray.resize( m_xyData->getLength() );
			m_rxArray.resize( m_xyData->getLength() );
			m_ryArray.resize( m_xyData->getLength() );
			m_axArray.resize( m_xyData->getLength() );			
			m_ayArray.resize( m_xyData->getLength() );
			m_areaArray.resize( m_xyData->getLength() );
			m_fwhmArray.resize( m_xyData->getLength() );
		}

		for( unsigned int i = 0; i < m_xyData->getLength(); i++ ) {
			m_xArray[ i ] = m_xyData->getX( i );
			m_yArray[ i ] = m_xyData->getY( i );

			if( m_peaksFlg == FLG_PEAK  ) 
			{
				kome::objects::Peaks* peaks = (kome::objects::Peaks*)m_xyData;
				kome::objects::PeakElement* peak = peaks->getPeak( i );

				m_lxArray[ i ] = peak->getLeftX();
				m_lyArray[ i ] = peak->getLeftY();
				m_rxArray[ i ] = peak->getRightX();
				m_ryArray[ i ] = peak->getRightY();
				m_axArray[ i ] = peak->getApexX();
				m_ayArray[ i ] = peak->getApexY();
				m_areaArray[ i ] = peak->getArea();
				m_fwhmArray[ i ] = peak->getFwhm();
			}
		}
	}
}


// create list
void XYData2DListCtrl::create2D() {

	// initialize column
	while( GetColumnCount() > 0 ) {
		DeleteColumn( 0 );
	}

	// insert columns
	addColumn( "RT", TYPE_UDOUBLE );
	addColumn( "m/z", TYPE_UDOUBLE );
	addColumn( "Intensity", TYPE_UDOUBLE );
	addColumn( "RT (Start)", TYPE_UDOUBLE );
	addColumn( "RT (End)", TYPE_UDOUBLE );
	addColumn( "m/z (Start)", TYPE_UDOUBLE );
	addColumn( "m/z (End)", TYPE_UDOUBLE );
	addColumn( "Cluster RT", TYPE_UDOUBLE );
	addColumn( "Cluster m/z", TYPE_UDOUBLE );
	addColumn( "Cluster Intensity", TYPE_UDOUBLE );
	addColumn( "Cluster RT (Start)", TYPE_UDOUBLE );
	addColumn( "Cluster RT (End)", TYPE_UDOUBLE );
	addColumn( "Cluster m/z (Start)", TYPE_UDOUBLE );
	addColumn( "Cluster m/z (End)", TYPE_UDOUBLE );
	addColumn( "Charge", TYPE_UINT );
	addColumn( "Precursor Mass", TYPE_UDOUBLE );
	addColumn( "Area", TYPE_UDOUBLE );
}

// get int
int XYData2DListCtrl::getInt( const long long data, const int column ) const {
	// peak
	kome::objects::Peak2DElement* peak = (kome::objects::Peak2DElement*)data; 

	// check
	if( peak == NULL ) {
		return -1.0;
	}

	// value
	int val = -1;
	if( column == 14 ) {
		val = peak->getCharge();
	}

	return val;
}

// wrapper
double XYData2DListCtrl::getDouble( const long long data, const int column ) const {

	switch( m_peaksFlg ){
	case FLG_DATA:
	case FLG_PEAK:
		return getDoubleXYData( data, column );
		break;
	case FLG_2DPEAK:
		return getDouble2D( data, column );
		break;
	default:
		// 通常あり得ない
		break;
	}
	
	return 0.0;
}

// get double value
double XYData2DListCtrl::getDoubleXYData( const long long data, const int column ) const {
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
	else if( column == 2 ) {		// left (x)
		if( data >= 0 && data < (int)m_lxArray.size() ) {
			v = m_lxArray[ data ];
		}
	}
	else if( column == 3 ) {		// left (y)
		if( data >= 0 && data < (int)m_lyArray.size() ) {
			v = m_lyArray[ data ];
		}
	}
	else if( column == 4 ) {		// apex (x)
		if( data >= 0 && data < (int)m_axArray.size() ) {
			v = m_axArray[ data ];
		}
	}
	else if( column == 5 ) {		// apex (y)
		if( data >= 0 && data < (int)m_ayArray.size() ) {
			v = m_ayArray[ data ];
		}
	}
	else if( column == 6 ) {		// right (x)
		if( data >= 0 && data < (int)m_rxArray.size() ) {
			v = m_rxArray[ data ];
		}
	}
	else if( column == 7 ) {		// right (y)
		if( data >= 0 && data < (int)m_ryArray.size() ) {
			v = m_ryArray[ data ];
		}
	}
	else if( column == 8 ) {		// area
		if( data >= 0 && data < (int)m_areaArray.size() ) {
			v = m_areaArray[ data ];
		}
	}
	else if( column == 9 ) {        // fwhm
		if( data >= 0 && data < (int)m_fwhmArray.size() ) {
			v = m_fwhmArray[ data ];
		}
	}

	return v;
}

// get peak value
double XYData2DListCtrl::getDouble2D( long long data, int col ) const {
	// peak
	kome::objects::Peak2DElement* peak = (kome::objects::Peak2DElement*)data; 

	// check
	if( peak == NULL ) {
		return -1.0;
	}

	// value
	double val = -1.0;
	if( col < 7 || col >= 15 ) {		// peak value
		if( col == 0 ) { // RT
			val = peak->getRt();
		}
		else if( col == 1 ) {	// m/z
			val = peak->getMz();
		}
		else if( col == 2 ) {	// intensity
			val = peak->getIntensity();
		}
		else if( col == 3 ) {	// start RT
			val = peak->getStartRt();
		}
		else if( col == 4 ) {	// end RT
			val = peak->getEndRt();
		}
		else if( col == 5 ) {	// start m/z
			val = peak->getStartMz();
		}
		else if( col == 6 ) {	// end m/z
			val = peak->getEndMz();
		}
		else if( col == 15 ) {    // precursor mass
			val = peak->getPrecursorMass();
		}
		else if( col == 16 ) {    // area
			val = peak->getArea();
		}
	}
	else {  // cluster value
		kome::objects::PeaksCluster2D* cluster = peak->getCluster();
		if( cluster != NULL ) {
			if( col == 7 ) { // RT
				val = cluster->getRt();
			}
			else if( col == 8 ) {	// m/z
				val = cluster->getMz();
			}
			else if( col == 9 ) {	// intensity
				val = cluster->getIntensity();
			}
			else if( col == 10 ) {	// start RT
				val = cluster->getStartRt();
			}
			else if( col == 11 ) {	// end RT
				val = cluster->getEndRt();
			}
			else if( col == 12 ) {	// start m/z
				val = cluster->getStartMz();
			}
			else if( col == 13 ) {	// end m/z
				val = cluster->getEndMz();
			}
		}
	}

	return val;
}

// on select item
void XYData2DListCtrl::onSelectItem( wxListEvent& evt ) {
	// clear
	kome::view::XYDataViewManager& mgr = kome::view::XYDataViewManager::getInstance();
	mgr.clearSelectedPeaks();

	if (m_peaksFlg == FLG_2DPEAK)
	{
		// add selected peaks
		for( int i = 0; i < GetItemCount(); i++ ) {
			if( GetItemState( i, wxLIST_STATE_SELECTED ) > 0 ) {
				mgr.addSelectedPeaks( (kome::objects::Peak2DElement*)getListData( i ) );
			}
		}
	}

	// refresh
	kome::window::WindowTool::refresh();
}

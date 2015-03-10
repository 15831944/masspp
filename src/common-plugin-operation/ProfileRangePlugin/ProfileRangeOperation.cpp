/**
 * @file ProfileRangeOperation.cpp
 * @brief Profile Range Operation class
 *
 * @author M.Izumi
 * @date 2011.12.16
 * 
 * Copyright (C) 2014 Shimadzu Corporation All rights reserved.
 */

#include "stdafx.h"
#include "ProfileRangeOperation.h"
#include "ProfileRangeManager.h"

using namespace kome::operation::range;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


#define SPEC_RANGE	"spec_range"
#define CHROM_RANGE "chrom_range"

// constructor
ProfileRangeOperation::ProfileRangeOperation(){
	m_chrom = NULL;
	m_spec = NULL;
	m_canvas = NULL;

	m_bReset = false;
	m_range = NULL;
	m_index = -1;

	m_minX = -1.0;
	m_maxX = -1.0;
	m_minY = -1.0;
	m_maxY = -1.0;		 
}

// destructor
ProfileRangeOperation::~ProfileRangeOperation(){	
}

// on execute
bool ProfileRangeOperation::onExecute(){	
	// manager
	ProfileRangeManager& mgr = ProfileRangeManager::getInstance();
	
	if( m_canvas == NULL ){
		return false;
	}

	// check parameters
	if( (((m_spec == NULL) || (m_spec->getGroup() == NULL)) &&
		 ((m_chrom == NULL) || (m_chrom->getGroup() == NULL))) ){ 

		kome::window::SpectrumCanvas* specCanvas = dynamic_cast< kome::window::SpectrumCanvas* >( m_canvas );
		kome::window::ChromatogramCanvas* chromCanvas = dynamic_cast< kome::window::ChromatogramCanvas* >( m_canvas );
		if( specCanvas == NULL && chromCanvas == NULL ){
			LOG_ERROR( FMT( "Failed to get the objects." ) );
			return false;
		}
	}
	
	bool flgRet = false;

	// Clear
	if( m_bReset ){
		mgr.resetZoom( *m_range, m_index );
		flgRet = true;
	}else{

		std::string strShortName = getShortName();
		// set range
		if( strShortName.compare( SPEC_RANGE ) == 0 ){
			switch( m_index ){
			case 0:
				if(	( 0.0 <= m_minX ) && ( m_minX <= m_maxX ) ){
					mgr.pushSpectrumXRange( m_range, m_canvas, m_minX, m_maxX );
					flgRet = true;
				}
				break;
			case 1:
				if(	( 0.0 <= m_minY ) && ( m_minY <= m_maxY ) ){
					mgr.pushSpectrumYRange( m_range, m_canvas, m_minY, m_maxY );
					flgRet = true;
				}
				break;
			case 2:
				if(	( ( 0.0 <= m_minX ) && ( m_minX <= m_maxX ) ) && 
					( ( 0.0 <= m_minY ) && ( m_minY <= m_maxY ) )	  ){
					mgr.pushSpectrumXRange( m_range, m_canvas, m_minX, m_maxX );
					mgr.pushSpectrumYRange( m_range, m_canvas, m_minY, m_maxY );
					flgRet = true;
				}
				break;
			default:
				break;
			}
		}
		else if( strShortName.compare( CHROM_RANGE ) == 0 ){
			switch( m_index ){
			case 0:
				if(	( 0.0 <= m_minX ) && ( m_minX <= m_maxX ) )
				{
					mgr.pushChromatogramXRange( m_range, m_canvas, m_minX, m_maxX );
					flgRet = true;
				}
				break;
			case 1:
				if(	( 0.0 <= m_minY ) && ( m_minY <= m_maxY ) )
				{
					mgr.pushChromatogramYRange( m_range, m_canvas, m_minY, m_maxY );
					flgRet = true;
				}
				break;
			case 2:
				if(	( ( 0.0 <= m_minX ) && ( m_minX <= m_maxX ) ) && 
					( ( 0.0 <= m_minY ) && ( m_minY <= m_maxY ) )	  ){
					mgr.pushChromatogramXRange( m_range, m_canvas, m_minX, m_maxX );
					mgr.pushChromatogramYRange( m_range, m_canvas, m_minY, m_maxY );
					flgRet = true;
				}
				break;
			default:
				break;
			}
		}
	}

	return flgRet;
}

// on load condition
void ProfileRangeOperation::onLoadCondition( boost::function< int ( void*, int ) > readFun ){	
	
	int len = int();
	readFun( &len, sizeof( len ) );

	double* arr = new double[ len * 2 ];
	readFun( arr, sizeof( double ) * len * 2 );

	while( !m_range->xRange.empty() ){
		m_range->xRange.pop();
	}
	while( !m_range->yRange.empty() ){
		m_range->yRange.pop();
	}

	for( int i = len-1; -1 < i; i-- ){
		int idx = i * 2;
		double min = arr[ idx + 0 ];
		double max = arr[ idx + 1 ];

		m_range->xRange.push( std::make_pair( min, max ) );
	}

	delete[] arr;

	len = 0;
	readFun( &len, sizeof( len ) );

	arr = new double[ len * 2 ];
	readFun( arr, sizeof( double ) * len * 2 );

	for( int i = len-1; -1 < i; i-- ){
		int idx = i * 2;
		double min = arr[ idx + 0 ];
		double max = arr[ idx + 1 ];

		m_range->yRange.push( std::make_pair( min, max ) );
	}
	delete[] arr;	

	kome::window::WindowTool::refresh();
}

// on save condition
void ProfileRangeOperation::onSaveCondition( boost::function< int ( void*, int ) > writeFun ){	
		
	ProfileRangeManager::RangeInfo range = *m_range;
	// length
	int len = ( m_range == NULL ? 0 : range.xRange.size() );
	writeFun( &len, sizeof( len ) );
	
	int i=0;
	double* arr = new double[ len * 2 ];

	while( !range.xRange.empty() ){
		// array
		int idx = i * 2;
		arr[ idx + 0 ] = range.xRange.top().first;
		arr[ idx + 1 ] = range.xRange.top().second;

		range.xRange.pop();
		i++;	
	}

	writeFun( arr, sizeof( double ) * len * 2 );

	delete[] arr;

	len = ( m_range == NULL ? 0 : range.yRange.size() );
	writeFun( &len, sizeof( len ) );
	i=0;
	arr = new double[ len * 2 ];
	while( !range.yRange.empty() ){
		// array
		int idx = i * 2;
		arr[ idx + 0 ] = range.yRange.top().first;
		arr[ idx + 1 ] = range.yRange.top().second;

		range.yRange.pop();
		i++;
	}
	writeFun( arr, sizeof( double ) * len * 2 );

	delete[] arr;
}

// get the description
std::string ProfileRangeOperation::onGetDescription(){
	std::string s = FMT( "Change Displaying Range" );
	if( m_spec != NULL ){
		s.append( FMT( " %s", m_spec->getName() ) );
	}
	if( m_chrom != NULL ){
		s.append( FMT( " %s", m_chrom->getName() ) );
	}
	if( m_bReset ){
		s.append( FMT( "[Reset Zoom]" ) );
	}else{
		s.append( FMT("[m/z:%.2f-%.2f, intensity:%.2f-%.2f]", std::max( 0.0, m_minX ), std::max( 0.0, m_maxX ), std::max( 0.0, m_minY ), std::max( 0.0, m_maxY ) ) );
	}

	return s;
}

// gets the parameter string
std::string ProfileRangeOperation::onGetParametersString(){
	std::string s;
	
	// sample id, spec or chrom id, index, rengeX, rangeY, reset flag, index
	kome::objects::Sample* sample = getTargetSample();
	int sampleid = ( sample == NULL ? -1 :sample->getSampleId() );
	s.append( FMT( "%d," , sampleid ) );
	
	int index = int();
	int style = (m_canvas == NULL ? 0 : ((kome::window::Canvas*)m_canvas)->getCanvasStyle());
	if( m_spec != NULL ){		
		
		s.append( FMT( "%d,",  dynamic_cast<kome::window::SpectrumCanvas*>( m_canvas )->getIndex() ) );
	}

	if( m_chrom != NULL ){

		s.append( FMT( "%d,",  dynamic_cast<kome::window::ChromatogramCanvas*>( m_canvas )->getIndex() ) );
	}

	s.append( FMT( "%f:%f,%f:%f,", m_minX, m_maxX, m_minY, m_maxY ) );
	s.append( FMT( "%s,", ( m_bReset == true ? "true" : "false" ) ) );
	s.append( FMT( "%d", m_index ) );
	
	return s;
}

// sets the parameter string
void ProfileRangeOperation::onSetParametersString( const char* strParam ){
	// sample id, spec or chrom id, index, rengeX, rangeY, reset flag, index
	std::vector<std::string> tokens;
	stringtoken( strParam, ",", tokens );
	if( tokens.size() < 5 ){
		return;
	}
	
	kome::objects::Sample* sample = kome::objects::Sample::getSampleById( toint( tokens[0].c_str(), 10, -1 ) );
	
	setTargetSample( sample );

	std::string shortName = getShortName();
	int index = toint( tokens[1].c_str(), 10, -1 );
		
	m_canvas = kome::window::Canvas::getCanvas( index );
	

	std::vector< std::string > range;
	stringtoken( tokens[2].c_str(), ":", range );

	m_minX = todouble( range[0].c_str(), -1.0 );
	m_maxX = todouble( range[1].c_str(), -1.0 );

	range.clear();
	stringtoken( tokens[3].c_str(), ":", range );
	m_minY = todouble( range[0].c_str(), -1.0 );
	m_maxY = todouble( range[1].c_str(), -1.0 );

	m_bReset = tobool( tokens[4].c_str(), false );
	m_index = toint( tokens[5].c_str(), 10, -1 );

	ProfileRangeManager& mgr = ProfileRangeManager::getInstance();
	m_range = mgr.getRange( m_canvas );
}

// set chromatogram
void ProfileRangeOperation::setChrom( kome::objects::Chromatogram* chrom ){
	m_chrom = chrom;
}

// get chromatogram
kome::objects::Chromatogram* ProfileRangeOperation::getChrom(){
	return m_chrom;
}

// set spectrum
void ProfileRangeOperation::setSpec( kome::objects::Spectrum* spec ){
	m_spec = spec;
}

// get spectrum
kome::objects::Spectrum* ProfileRangeOperation::getSpec(){
	return m_spec;
}

// set canvas
void ProfileRangeOperation::setCanvas( wxWindow* canvas ){
	m_canvas = canvas;
}

// get canvas
wxWindow* ProfileRangeOperation::getCanvas(){
	return m_canvas;
}

// set reset flg
void ProfileRangeOperation::setbReset( bool reset ){
	m_bReset = reset;
}

// get reset flg
bool ProfileRangeOperation::getbRest(){
	return m_bReset;
}

// set range info
void ProfileRangeOperation::setRangeInfo( ProfileRangeManager::RangeInfo* range ){
	m_range = range;
}

// get range info
ProfileRangeManager::RangeInfo* ProfileRangeOperation::getRangeInfo(){
	return m_range;
}

// set index
void ProfileRangeOperation::setIndex( int index ){
	m_index = index;
}

// get index
int ProfileRangeOperation::getIndex(){
	return m_index;
}

// set min x
void ProfileRangeOperation::setMinX( double minX ){
	m_minX = minX;
}

// get min x
double ProfileRangeOperation::getMinX(){
	return m_minX;
}

// set max x
void ProfileRangeOperation::setMaxX( double maxX ){
	m_maxX = maxX;
}

// get max x
double ProfileRangeOperation::getMaxX(){
	return m_maxX;
}

// set min y
void ProfileRangeOperation::setMinY( double minY ){
	m_minY = minY;
}

// get min y
double ProfileRangeOperation::getMinY(){
	return m_minY;
}

// set max y
void ProfileRangeOperation::setMaxY( double maxY ){
	m_maxY = maxY;
}

// get max y
double ProfileRangeOperation::getMaxY(){
	return m_maxY;
}

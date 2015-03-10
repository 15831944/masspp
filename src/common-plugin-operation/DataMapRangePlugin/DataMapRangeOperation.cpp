/**
 * @file DataMapRangeOperation.cpp
 * @brief DataMap Range Operation class
 *
 * @author M.Izumi
 * @date 2013.05.14
 * 
 * Copyright (C) 2014 Shimadzu Corporation All rights reserved.
 */

#include "stdafx.h"
#include "DataMapRangeOperation.h"


using namespace kome::operation::range;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


// constructor
DataMapRangeOperation::DataMapRangeOperation(){
	m_startMz = -1.0;
	m_endMz = -1.0;
	m_startRt = -1.0;
	m_endRt = -1.0; 

	m_dataMapInfo = NULL;
	m_canvas = NULL;
	
	m_bReset = false;

}

// destructor
DataMapRangeOperation::~DataMapRangeOperation(){
}

// get the description
std::string DataMapRangeOperation::onGetDescription() {
	std::string s = FMT( "DataMap Range" );

	if( m_dataMapInfo != NULL ){
		kome::objects::DataGroupNode* group = &m_dataMapInfo->getGroup();
		if( group != NULL ){
			s.append( FMT( " (%s)", group->getName() ) );
		}
	}
	
	if( m_bReset ){
		s.append( " [Reset Zoom]" );
	}else{
		s.append( FMT( " [m/z:%.2f-%.2f, RT:%.2f-%.2f]", 
			std::max( 0.0, m_startMz ), std::max( 0.0, m_endMz ), std::max( 0.0, m_startRt ), std::max( 0.0, m_endRt ) ) );
	}
	return s;
}

// gets the parameter string
std::string DataMapRangeOperation::onGetParametersString() {
	std::string s;

	if( m_dataMapInfo != NULL ){
		
		kome::objects::DataGroupNode* group = &m_dataMapInfo->getGroup();
		if( group != NULL ){
			int index = dynamic_cast< kome::window::Canvas* >(m_canvas)->getIndex();
			s = FMT( "%d,%d,%d,%f,%f,%f,%f", 
				getTargetSample()->getSampleId(), group->getId(), index, m_startMz, m_endMz, m_startRt, m_endRt );
		}
	}

	return s;
}

// sets the parameter string
void DataMapRangeOperation::onSetParametersString( const char* strParam ) {
	std::vector< std::string > tokens;
	stringtoken( strParam, ",", tokens );
	if( tokens.size() < 6 ) {
		return;
	}

	// sample
	kome::objects::Sample* sample = kome::objects::Sample::getSampleById( toint( tokens[0].c_str(), 10, -1 ) );
	if( sample == NULL ){
		return;
	}
	setTargetSample( sample );

	// group
	int id = toint( tokens[1].c_str(), 10, -1 );
	kome::objects::DataGroupNode* group = m_targetSample->getGroupById( id );

	if( group == NULL ){
		return;
	}

	int index = toint( tokens[ 2 ].c_str(), 10, -1 );
	m_canvas = kome::window::Canvas::getCanvas( index );
	if( m_canvas == NULL ){
		return;
	}
	kome::objects::MsDataVariant obj( dynamic_cast<kome::window::Canvas*>( m_canvas )->getActiveObject());
	m_dataMapInfo = obj.getDataMap();

	m_startMz = todouble( tokens[ 3 ].c_str(), -1.0 );
	m_endMz = todouble( tokens[ 4 ].c_str(), -1.0 );
	m_startRt = todouble( tokens[ 5 ].c_str(), -1.0 );
	m_endRt = todouble( tokens[ 6 ].c_str(), -1.0 );
}

// save the condition
void DataMapRangeOperation::onSaveCondition( boost::function< int ( void*, int ) > writeFun ) {
	DataMapRangeManager& mgr = DataMapRangeManager::getInstance();

	if( m_dataMapInfo != NULL ){
		
		kome::objects::DataGroupNode* group = &m_dataMapInfo->getGroup();
		if( group != NULL ){
			int groupId = group->getId();
			writeFun( &groupId, sizeof( groupId ) );
		}
	}
		
	double minMz = m_dataMapInfo->getMinMz();
	double maxMz = m_dataMapInfo->getMaxMz();
	double minRt = m_dataMapInfo->getMinRt();
	double maxRt = m_dataMapInfo->getMaxRt();

	writeFun( &minMz, sizeof( minMz ) );
	writeFun( &maxMz, sizeof( maxMz ) );
	writeFun( &minRt, sizeof( minRt ) );
	writeFun( &maxRt, sizeof( maxRt ) );	
	
	// length
// >>>>>>	@Date:2012/05/30	<Add>	A.Ozaki
// ResetZoom時には、m_rangeがNULLで設定されます(OperationLog対応)
// そのため、元のロジックだとwhile条件でm_rangeを参照するため
// NULLポインタを参照して、落ちてしまいます。
// m_rangeがNULLの時には、強制的に１データのみ出力するようにします
	
	DataMapRangeManager::RangeInfo* range = mgr.getRange( m_dataMapInfo );
	int	len = ( range == NULL ? 1 : range->xRange.size( ) );
	writeFun( &len, sizeof( len ) );
		
	int i = 0;
	double*	arr = new double[len * 2];
	
	if  ( NULL == range )
	{
		arr[0] = 0.0;
		arr[1] = 0.0;
	}
	else
	{
		while ( !range->xRange.empty( ) )
		{
			// array 
			arr[i * 2 + 0] = range->xRange.top( ).first;
			arr[i * 2 + 1] = range->xRange.top( ).second;
		
			range->xRange.pop( );
			i++;
		}
	}
	writeFun( arr, sizeof( double ) * len * 2 );

	delete[] arr;

	len = ( range == NULL ? 1 : range->yRange.size () );
	writeFun( &len, sizeof( len ) );
	i = 0;
	arr = new double[ len * 2 ];
	if  ( NULL == range )
	{
		arr[0] = 0.0;
		arr[1] = 0.0;
	}
	else
	{
		while ( !range->yRange.empty( ) )
		{
			// array
			arr[i * 2 + 0] = range->yRange.top( ).first;
			arr[i * 2 + 1] = range->yRange.top( ).second;

			range->yRange.pop( );
			i++;
		}
	}
	writeFun( arr, sizeof( double ) * len * 2 );

	delete[] arr;

// <<<<<<	@Date:2013/05/30	<Modify>	A.Ozaki
}

// load the condition
void DataMapRangeOperation::onLoadCondition( boost::function< int ( void*, int ) > readFun ) {
	DataMapRangeManager& mgr = DataMapRangeManager::getInstance();
	
	int id = int();
	readFun( &id, sizeof( id ) );

	kome::objects::Sample* sample = getTargetSample();
	kome::objects::DataGroupNode* group = NULL;
	if( sample != NULL ){
		group = sample->getGroupById( id );
	}

	if( group == NULL ){
		return;
	}
	
	double startMz = double();
	double endMz = double();
	double startRt = double();
	double endRt = double();

	readFun( &startMz, sizeof( startMz ) );
	readFun( &endMz, sizeof( endMz ) );
	readFun( &startRt, sizeof( startRt ) ) ;
	readFun( &endRt, sizeof( endRt ) );

	int len = 0;
	readFun( &len, sizeof( len ) );

	double* arr = new double[ len * 2 ];
	readFun( arr, sizeof( double ) * len * 2 );

	DataMapRangeManager::RangeInfo* range = mgr.getRange( m_dataMapInfo );

	// clear
	if( range != NULL ){
		while( !range->xRange.empty() ){
			range->xRange.pop();
		}
		while( !range->yRange.empty() ){
			range->yRange.pop();
		}
	}

	for( int i = len-1; -1 < i; i-- ){
		int idx = i * 2;
		double min = arr[ idx + 0 ];
		double max = arr[ idx + 1 ];

		range->xRange.push( std::make_pair( min, max ) );
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

		range->yRange.push( std::make_pair( min, max ) );
	}
	delete[] arr;
	
	mgr.setRange( m_dataMapInfo, m_canvas, startMz, endMz, startRt, endRt );
		
	kome::window::WindowTool::refresh();
}

// execute
bool DataMapRangeOperation::onExecute() {
	bool bflg = false;
	DataMapRangeManager& mgr = DataMapRangeManager::getInstance();	
		
	// Reset Zoom
	if( m_bReset ){
		bflg = mgr.resetZoomDatamapFromContextMenuManager( *m_dataMapInfo, m_canvas );
	// 拡大
	}else{		
		mgr.setChangingDataMapInfo( m_dataMapInfo );

		bflg = mgr.setRange( m_dataMapInfo, m_canvas, m_startMz, m_endMz, m_startRt, m_endRt );

		mgr.setChangingDataMapInfo( NULL );
	}
		
	return bflg;
}

// sets start m/z
void DataMapRangeOperation::setStartMz( double startMz ){
	m_startMz = startMz;
}

// gets start m/z
double DataMapRangeOperation::getStartMz(){
	return m_startMz;
}

// sets end m/z 
void DataMapRangeOperation::setEndMz( double endMz ){
	m_endMz = endMz;
}

// gets end m/z
double DataMapRangeOperation::getEndMz(){
	return m_endMz;
}

// sets start rt
void DataMapRangeOperation::setStartRt( double startRt){
	m_startRt = startRt;
}

// gets start rt
double DataMapRangeOperation::getStartRt(){
	return m_startRt;
}

// sets end rt
void DataMapRangeOperation::setEndRt( double endRt ){
	m_endRt = endRt;
}

// gets end rt
double DataMapRangeOperation::getEndRt(){
	return m_endRt;
}

// sets canvas
void DataMapRangeOperation::setCanvas( wxWindow* canvas ){
	m_canvas = canvas;
}

// gets canvas
wxWindow* DataMapRangeOperation::getCanvas(){
	return m_canvas;
}

// sets datamap info
void DataMapRangeOperation::setDataMapInfo( kome::objects::DataMapInfo* dataMapInfo ){
	m_dataMapInfo = dataMapInfo;
}

// gets datamap info
kome::objects::DataMapInfo* DataMapRangeOperation::getDataMapInfo(){
	return m_dataMapInfo;
}

// sets reset flag
void DataMapRangeOperation::setbReset( bool bRest ){
	m_bReset = bRest;
}

// gets reset flag
bool DataMapRangeOperation::getbReset(){
	return m_bReset;
}


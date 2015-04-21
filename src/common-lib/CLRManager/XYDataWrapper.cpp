/**
 * @file XYDataWrapper.cpp
 * @brief implements of XYDataWrapper class
 *
 * @author S.Tanaka
 * @date 2006.08.30
 * @date 2011.03.02 <Del> M.Aihara
 *       delete static Timer
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "XYDataWrapper.h"

#include "ClrObjectTool.h"

#include <boost/bind.hpp>


using namespace kome::clr;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
XYDataWrapper::XYDataWrapper( kome::core::XYData& xyData ) : m_xyData( xyData ) {
}

// destructor
XYDataWrapper::~XYDataWrapper() {
}

// finalizer
XYDataWrapper::!XYDataWrapper(){
};

// get XYData object
kome::core::XYData& XYDataWrapper::getXYData() {
	return m_xyData;
}

// get minimum x
double XYDataWrapper::getMinX() {
	return m_xyData.getMinX();
}

// get maximum x
double XYDataWrapper::getMaxX() {
	return m_xyData.getMaxX();
}

// get minimum y
double XYDataWrapper::getMinY() {
	return m_xyData.getMinY();
}

// get maximum y
double XYDataWrapper::getMaxY() {
	return m_xyData.getMaxY();
}

// clear points
void XYDataWrapper::clearPoints() {
	m_xyData.clearPoints();
}

// add point
void XYDataWrapper::addPoint( double x, double y ) {
	m_xyData.addPoint( x, y );
}

// insert point
void XYDataWrapper::insertPoint( unsigned int index, double x, double y ) {
	m_xyData.insertPoint( index, x, y );
}

// update point
void XYDataWrapper::updatePoint( unsigned int index, double x, double y ) {
	m_xyData.updatePoint( index, x, y );
}

// delete point
void XYDataWrapper::deletePoint( unsigned int index ) {
	m_xyData.deletePoint( index );
}

// get the number of points
unsigned int XYDataWrapper::getLength() {
	return m_xyData.getLength();
}

// get x
double XYDataWrapper::getX( unsigned int index ) {
	return m_xyData.getX( index );
}

// get y
double XYDataWrapper::getY( unsigned int index ) {
	return m_xyData.getY( index );
}

// reserve
void XYDataWrapper::reserve( unsigned int num ) {
	m_xyData.reserve( num );
}

// get version
unsigned long XYDataWrapper::getVersion() {
	return m_xyData.getVersion();
}

// search index
int XYDataWrapper::searchIndex( double x ) {
	return m_xyData.searchIndex( x );
}

// get nearest index
int XYDataWrapper::getNearestIndex( double x ) {
	return m_xyData.getNearestIndex( x );
}

// execute filter
void XYDataWrapper::filter( double absY, double relY ) {
	m_xyData.filter( absY, relY );
}

// import data
bool XYDataWrapper::importData( IoFun^ readFun ) {
	// create helper object
	IoFunctionHelper helper( readFun );

	// import
	bool ret = m_xyData.importData(
		boost::bind(
			&IoFunctionHelper::call,
			&helper,
			_1,
			_2
		)
	);

	return ret;
}

// export data
bool XYDataWrapper::exportData( IoFun^ writeFun ) {
	// create helper object
	IoFunctionHelper helper( writeFun );

	// export
	bool ret = m_xyData.exportData(
		boost::bind(
			&IoFunctionHelper::call,
			&helper,
			_1,
			_2
		)
	);

	return ret;
}

// get the total time of loading temporary file
// @date 2011.03.02 <Del> M.Aihara ->
// double XYDataWrapper::getInputTotalTime() {
//	return kome::core::XYData::getInputTotalTime();
// }
// <Del> M.Aihara <-

// get the total time of saving temporary file
// @date 2011.03.02 <Del> M.Aihara ->
// double XYDataWrapper::getOutputTotalTime() {
//	return kome::core::XYData::getOutputTotalTime();
// }
// <Del> M.Aihara <-

//
// IoFunctionHelper class
//

// constructor
IoFunctionHelper::IoFunctionHelper( gcroot< IoFun^ > fun ) {
	m_fun = fun;
}

// destructor
IoFunctionHelper::~IoFunctionHelper() {
}

// call function
int IoFunctionHelper::call( void* addr, int size ) {
	// get fucntion
	IoFun^ fun = m_fun;
	if( fun == nullptr ) {
		return 0;
	}

	// call
	int ret = fun( addr, size );

	return ret;
}

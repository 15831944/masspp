/**
 * @file ClrXYData.cpp
 * @brief implements of ClrXYData class
 *
 * @author S.Tanaka
 * @date 2006.08.30
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#include "stdafx.h"
#include "ClrXYData.h"

#include "ClrXYDataBase.h"
#include "ClrObjectPool.h"


using namespace kome::clr;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
ClrXYData::ClrXYData() {
}

// destructor
ClrXYData::~ClrXYData() {
	ClrObjectPool::getInstance().removeXYData( *this );
	m_baseData = nullptr;
}

// set base data object
void ClrXYData::setBaseData( ClrXYDataBase^ baseData ) {
	m_baseData = baseData;
}

// get base data object
ClrXYDataBase^ ClrXYData::getBaseData() {
	ClrXYDataBase^ baseData = m_baseData;
	return baseData;
}

// on clear points
void ClrXYData::onClearPoints() {
	ClrXYDataBase^ baseData = m_baseData;
	if( baseData != nullptr ) {
		baseData->onClearPoints();
	}
}

// on add point
void ClrXYData::onAddPoint( const double x, const double y ) {
	ClrXYDataBase^ baseData = m_baseData;
	if( baseData != nullptr ) {
		baseData->onAddPoint( x, y );
	}
}

// on insert point
void ClrXYData::onInsertPoint( const unsigned int index, const double x, const double y ) {
	ClrXYDataBase^ baseData = m_baseData;
	if( baseData != nullptr ) {
		baseData->onInsertPoint( index, x, y );
	}
}

// on delete point
void ClrXYData::onDeletePoint( const unsigned int index ) {
	ClrXYDataBase^ baseData = m_baseData;
	if( baseData != nullptr ) {
		baseData->onDeletePoint( index );
	}
}

// on get length
unsigned int ClrXYData::onGetLength() {
	ClrXYDataBase^ baseData = m_baseData;
	if( baseData == nullptr ) {
		return 0;
	}
	return baseData->onGetLength();
}

// on get x
double ClrXYData::onGetX( const unsigned int index ) {
	ClrXYDataBase^ baseData = m_baseData;
	if( baseData == nullptr ) {
		throw gcnew System::Exception( "There is no base data object." );
	}
	return baseData->onGetX( index );
}

// on get y
double ClrXYData::onGetY( const unsigned int index ) {
	ClrXYDataBase^ baseData = m_baseData;
	if( baseData == nullptr ) {
		throw gcnew System::Exception( "There is no base data object." );
	}
	return baseData->onGetY( index );
}

// on reserve
void ClrXYData::onReserve( const unsigned int num ) {
	ClrXYDataBase^ baseData = m_baseData;
	if( baseData == nullptr ) {
		return;
	}
	return baseData->onReserve( num );
}

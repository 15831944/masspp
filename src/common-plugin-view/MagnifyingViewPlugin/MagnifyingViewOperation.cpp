/**
 * @file MagnifyingViewOperation.cpp
 * @brief interfaces of MagnifyingViewOperation class
 *
 * @author S.Tanaka
 * @date 2012.04.02
 * 
 * Copyright (C) 2014 Shimadzu Corporation All rights reserved.
 */


#include "stdafx.h"
#include "MagnifyingViewOperation.h"
#include "MagnifyingViewManager.h"


using namespace kome::view;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
MagnifyingViewOperation::MagnifyingViewOperation() {
	
	m_spec = NULL;
	m_idx = -1;
	m_start = -1.0;
	m_end = -1.0;
	m_scale = -1.0;
}

// destructor
MagnifyingViewOperation::~MagnifyingViewOperation() {
}

// set the spectrum
void MagnifyingViewOperation::setSpectrum( kome::objects::Spectrum* spec ) {
	m_spec = spec;
}

// get the spectrum
kome::objects::Spectrum* MagnifyingViewOperation::getSpectrum() {
	return m_spec;
}

// set the scale index
void MagnifyingViewOperation::setScaleIndex( const int idx ) {
	m_idx = idx;
}

// get the scale index
int MagnifyingViewOperation::getScaleIndex() {
	return m_idx;
}

// set the range
void MagnifyingViewOperation::setRange( const double start, const double end ) {
	m_start = start;
	m_end = end;
}

// get the range start
double MagnifyingViewOperation::getRangeStart(){
	return m_start;
}

// get the range end
double MagnifyingViewOperation::getRangeEnd() {
	return m_end;
}

// set the scale
void MagnifyingViewOperation::setScale( const double scale ) {
	m_scale = scale;
}

// get the scale 
double MagnifyingViewOperation::getScale() {
	return m_scale;
}

// get the description
std::string MagnifyingViewOperation::onGetDescription() {
	// [Operation Log]
	// Operation Log で表示する文字列を作成します。

	std::string s = FMT( "Magnify" );
	if( m_spec != NULL ) {
		s.append( FMT( " %s", m_spec->getName() ) );
	}
	if( m_scale < 0.0 ) {   // クリア時
		s.append( " [Clear]");
	}
	else {
		s.append( FMT( " [Range:%.2f-%.2f, Scale:%.2f]", std::max( 0.0, m_start ), std::max( 0.0, m_end ), m_scale ) );
	}

	// [Operation Log]
    // この部分は変更する必要があると思います。
	// ここで作った string オブジェクトはこのメソッドを抜けると消えてしまうので
    // Operation クラスにメンバ変数を持たせてセットするか
    // 戻り値を const char* ではなく string にした方が良いかもしれません。
	return s;
}

// gets the parameter string
std::string MagnifyingViewOperation::onGetParametersString() {
	// [Operation Log]
	// 次回 onExecute を実行して同じ処理ができる様な string を作成します。

	std::string s;
	if( m_spec != NULL ) {
         s = FMT( "%d,%d,%d,%f,%f,%f", m_spec->getSample()->getSampleId(), m_spec->getOrgSpectrum()->getId(), m_idx, m_start, m_end, m_scale );
	}

	// [Operation Log]
	// string の retrun については onGetDescription と同様
	// ここで作った string オブジェクトはこのメソッドを抜けると消えてしまうので
    // Operation クラスにメンバ変数を持たせてセットするか
    // 戻り値を const char* ではなく string にした方が良いかもしれません。
	return s;
}

// sets the parameter string
void MagnifyingViewOperation::onSetParametersString( const char* strParam ) {
	// [Operation Log]
	// 文字列よりメンバ変数を復旧します。

	std::vector< std::string > tokens;
	stringtoken( strParam, ",", tokens );
	if( tokens.size() < 5 ) {
		return;
	}
	// sample
	kome::objects::Sample* sample = kome::objects::Sample::getSampleById( toint( tokens[0].c_str(), 10, -1 ) );
	if( sample == NULL ){
		return;
	}
	setTargetSample( sample );

	// spectrum
	int specId = toint( tokens[ 1 ].c_str(), 10, -1 );
	m_spec = m_targetSample->getSpectrumById( specId );
	if( m_spec == NULL ){
		return;
	}

	// index
	m_idx = toint( tokens[ 2 ].c_str(), 10, -1 );
		
	m_start = todouble( tokens[ 3 ].c_str(), -1.0 );
	m_end = todouble( tokens[ 4 ].c_str(), -1.0 );
	m_scale = todouble( tokens[ 5 ].c_str(), -1.0 );
	
}

// save the condition
void MagnifyingViewOperation::onSaveCondition( boost::function< int ( void*, int ) > writeFun ) {

	// [Operation Log]
	// Undo, Redo の為に状態を保存します。

	MagnifyingViewManager& mgr = MagnifyingViewManager::getInstance();

	unsigned int num = mgr.getNumberOfScaleInfo( m_spec );
	writeFun( &num, sizeof( num ) );
	for( unsigned int i = 0; i < num; i++ ) {
		MagnifyingViewManager::ScaleInfo* scaleInfo = mgr.getScaleInfo( m_spec, i );
		
		int id = scaleInfo->spec->getId();
		
		writeFun( &id, sizeof( id ) );

		writeFun( &( scaleInfo->minX ), sizeof( scaleInfo->minX ) );
		writeFun( &( scaleInfo->maxX ), sizeof( scaleInfo->maxX ) );
		writeFun( &( scaleInfo->scale ), sizeof( scaleInfo->scale ) );
	}
}

// load the condition
void MagnifyingViewOperation::onLoadCondition( boost::function< int ( void*, int ) > readFun ) {
	// [Operation Log]
	// Undo, Redo 時など save condition で保存した内容を復元します。

	MagnifyingViewManager& mgr = MagnifyingViewManager::getInstance();

	mgr.clearScaleInfo( m_spec );

	unsigned int num = 0;
	readFun( &num, sizeof( num ) );

	for( unsigned int i = 0; i < num; i++ ) {
		int specId = int();
		double start = double();
		double end = double();
		int scale = int();
		int idx = int();
		
		readFun( &specId, sizeof( specId ) );
		
		readFun( &start, sizeof( start ) );
		readFun( &end, sizeof( end ) );
		readFun( &scale, sizeof( scale ) );
		
		// spectrum
		kome::objects::Spectrum* spec = m_targetSample->getSpectrumById( specId );
		// add
		MagnifyingViewManager::ScaleInfo* scaleInfo = mgr.addScaleInfo( spec, start, end );
		scaleInfo->scale = scale;
	}
}

// execute
bool MagnifyingViewOperation::onExecute() {
	// [Operation Log]
	// 実際の処理を行ないます。
	// saveCondition はここではなくて Operation::execute メソッドの中で
    // 行なっておきます。

	MagnifyingViewManager& mgr = MagnifyingViewManager::getInstance();

	// check parameters
	if( m_spec == NULL ) {
		LOG_ERROR( FMT( "Failed to get the spectrum." ) );
		return false;
	}

	if( m_idx < 0 ) {    // add
		MagnifyingViewManager::ScaleInfo* scaleInfo = mgr.addScaleInfo( m_spec, m_start, m_end );
		scaleInfo->scale = m_scale;
	}
	else {    // update
		if( m_scale < 0 ) {    // delete
			mgr.removeScaleInfo( m_spec, m_idx );
		}
		else {    // edit
			MagnifyingViewManager::ScaleInfo* scaleInfo = mgr.getScaleInfo( m_spec, m_idx );
			if( scaleInfo == NULL ) {
				LOG_ERROR( FMT( "Failed to get the scale information." ) );
				return false;
			}
			scaleInfo->minX = m_start;
			scaleInfo->maxX = m_end;
			scaleInfo->scale = m_scale;
		}
	}

	return true;
}

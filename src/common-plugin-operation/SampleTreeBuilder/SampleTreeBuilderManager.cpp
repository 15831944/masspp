/**
 * @file SampleTreeBuilderManager.cpp
 * @brief interfaces of sample tree builder manager functions
 *
 * @author okada_h
 * @date 2012.09.28
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation All rights reserved.
 */

#include "stdafx.h"
#include "SampleTreeBuilderFunctions.h"
#include "SampleTreeBuilderManager.h"


// constructor
SampleTreeBuilderManager::SampleTreeBuilderManager(){
}

// destructor
// Mass++が終了する際、この処理が呼び出される。SampleTreeBuilderSampleSet*が削除されずに残っていれば、オブジェクトを削除する
SampleTreeBuilderManager::~SampleTreeBuilderManager()
{
	// 要素を逆方向に辿る
	std::set< kome::SampleTreeBuilder::SampleTreeBuilderSampleSet* >::reverse_iterator it = m_setSsPtrs.rbegin();
	while( it != m_setSsPtrs.rend() )
	{
		kome::SampleTreeBuilder::SampleTreeBuilderSampleSet* ptr = *it;
		delete ptr;	// ※objectがdeleteされる際、destructor経由でunregistPtr()が呼ばれます

		if( m_setSsPtrs.empty() ){
			break;
		}

		if( m_setSsPtrs.find( ptr ) == m_setSsPtrs.end() ){
			// 登録されていない
		}else{
			m_setSsPtrs.erase( *it );	// ※objectがdeleteされる際、destructor経由でunregistPtr()が呼ばれるため、通常、ここに処理が来ることは無い
		}

		++it;
	}
	
}


// get instance
SampleTreeBuilderManager& SampleTreeBuilderManager::getInstance() {
	// create object
	static SampleTreeBuilderManager mgr;

	return mgr;
}

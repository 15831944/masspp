/**
 * @file CompoundDbManager.cpp
 * @brief implements of CompoundDbManager class
 *
 * @author S.Tanaka
 * @date 2013.08.06
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "CompoundDbManager.h"

#include "CasDb.h"
#include "ChebiDb.h"
#include "KeggDb.h"
#include "KnapsackDb.h"
#include "LipidBankDb.h"
#include "LipidMapsDb.h"
#include "PubChemDb.h"


using namespace kome::external_db;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
CompoundDbManager::CompoundDbManager() {
	CasDb* cas = new CasDb();
	m_databases.push_back( cas );

	ChebiDb* chebi = new ChebiDb();
	m_databases.push_back( chebi );

	KeggDb* kegg = new KeggDb();
	m_databases.push_back( kegg );

	KnapsackDb* knapsack = new KnapsackDb();
	m_databases.push_back( knapsack );

	LipidBankDb* lipidbank = new LipidBankDb();
	m_databases.push_back( lipidbank );

	LipidMapsDb* lipidmaps = new LipidMapsDb();
	m_databases.push_back( lipidmaps );

	PubChemDb* pubchem = new PubChemDb();
	m_databases.push_back( pubchem );
}

// destructor
CompoundDbManager::~CompoundDbManager() {
}

// get database
kome::ident::ExternalDB* CompoundDbManager::getDb( const char* db ) {
	// return value
	kome::ident::ExternalDB* obj = NULL;

	// string
	std::string s = trimstring( tolowercase( db ).c_str() );

	// search
	for( unsigned int i = 0; i < m_databases.size() && obj == NULL; i++ ) {
		kome::ident::ExternalDB* tmp = m_databases[ i ];

		std::vector< std::string > tokens;
		stringtoken( tmp->getName(), " \t\r\n", tokens );
		std::string tmpName;
		for( unsigned int j = 0; j < tokens.size(); j++ ) {
			tmpName.append( trimstring( tolowercase( tokens[ j ].c_str() ).c_str() ) );
		}

		if( tmpName.compare( s ) == 0 ) {
			obj = tmp;
		}
	}

	return obj;
}

// get instance
CompoundDbManager& CompoundDbManager::getInstance() {
	// create object (This is the only object.)
	static CompoundDbManager mgr;

	return mgr;
}

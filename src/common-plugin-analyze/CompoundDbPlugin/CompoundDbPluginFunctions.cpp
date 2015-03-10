/**
 * @file CompoundDbPluginFunctions.cpp
 * @brief implementation of plug-in functions
 *
 * @author tanaka-s
 * @date 2013.08.06
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation All rights reserved.
 */


#include "stdafx.h"
#include "CompoundDbPluginFunctions.h"

#include "CompoundDbManager.h"


using namespace kome::external_db;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// CAS database
kome::objects::Variant getCasDb( kome::objects::Parameters* ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::OTHER;

	// manager
	CompoundDbManager& mgr = CompoundDbManager::getInstance();

	// DB
	ret.prim.pt = mgr.getDb( "cas" );
	return ret;
}

// ChEBI database
kome::objects::Variant getChebiDb( kome::objects::Parameters* ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::OTHER;

	// manager
	CompoundDbManager& mgr = CompoundDbManager::getInstance();

	// DB
	ret.prim.pt = mgr.getDb( "chebi" );
	return ret;
}

// KEGG database
kome::objects::Variant getKeggDb( kome::objects::Parameters* ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::OTHER;

	// manager
	CompoundDbManager& mgr = CompoundDbManager::getInstance();

	// DB
	ret.prim.pt = mgr.getDb( "kegg" );
	return ret;
}

// KNApSAcK database
kome::objects::Variant getKnapsackDb( kome::objects::Parameters* ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::OTHER;

	// manager
	CompoundDbManager& mgr = CompoundDbManager::getInstance();

	// DB
	ret.prim.pt = mgr.getDb( "knapsack" );
	return ret;
}

// LipidBank databas
kome::objects::Variant getLipidbankDb( kome::objects::Parameters* ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::OTHER;

	// manager
	CompoundDbManager& mgr = CompoundDbManager::getInstance();

	// DB
	ret.prim.pt = mgr.getDb( "lipidbank" );
	return ret;
}

// LIPID MAPS database
kome::objects::Variant getLipidmapsDb( kome::objects::Parameters* ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::OTHER;

	// manager
	CompoundDbManager& mgr = CompoundDbManager::getInstance();

	// DB
	ret.prim.pt = mgr.getDb( "lipidmaps" );
	return ret;
}

// PubChem database
kome::objects::Variant getPubchemDb( kome::objects::Parameters* ) {
	// return value
	kome::objects::Variant ret;
	ret.type = kome::objects::Variant::OTHER;

	// manager
	CompoundDbManager& mgr = CompoundDbManager::getInstance();

	// DB
	ret.prim.pt = mgr.getDb( "pubchem" );
	return ret;
}


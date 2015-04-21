/**
 * @file ClrParameters.cpp
 * @brief implements of ClrParametes class
 *
 * @author S.Tanaka
 * @date 2006.08.28
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#include "stdafx.h"
#include "ClrParameters.h"

#include "ClrVariant.h"
#include "ClrObjectTool.h"

#include <string>


using namespace kome::clr;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// construcotr
ClrParameters::ClrParameters() {
	m_parameters = gcnew System::Collections::Hashtable();
	m_keys = gcnew System::Collections::ArrayList();
}

ClrParameters::ClrParameters( kome::objects::Parameters& params ) {
	m_parameters = gcnew System::Collections::Hashtable();
	m_keys = gcnew System::Collections::ArrayList();

	setParameters( params );
}

// destructor
ClrParameters::~ClrParameters() {
	m_parameters = nullptr;
	m_keys = nullptr;
}

// get the number of parameters
unsigned int ClrParameters::getNumberOfParameters() {
	return m_keys->Count;
}

// get parameter key
System::String^ ClrParameters::getParameterKey( unsigned int index ) {
	// check the index
	if( index >= (unsigned int)m_keys->Count ) {
		return nullptr;
	}

	return (System::String^)m_keys[index];
}

// get parameter value
kome::clr::ClrVariant^ ClrParameters::getValue( System::String^ key ) {
	// check the map
	if( !m_parameters->Contains( key ) ) { 
		m_parameters[ key ] = gcnew kome::clr::ClrVariant();
		m_keys->Add( key );
	}

	return (kome::clr::ClrVariant^)m_parameters[ key ];
}

// judge whether there is the parameter with the specified name.
bool ClrParameters::hasParameter( System::String^ key ) {
	// check the map
	return m_parameters->Contains( key );
}

// set parameters
void ClrParameters::setParameters( kome::objects::Parameters& params ) {
	for( unsigned int i = 0; i < params.getNumberOfParameters(); i++ ) {
		// get key
		std::string keyStr( NVL( params.getName( i ), "" ) );
		System::String^ key = gcnew System::String( keyStr.c_str() );

		// set key
		m_keys->Add( key );

		// set value
		m_parameters[ key ] = gcnew ClrVariant( params.getValue( keyStr.c_str() ) );
	}
}

// get parameters
void ClrParameters::getParameters( kome::objects::Parameters& params ) {
	for( int i = 0; i < m_keys->Count; i++ ) {
		// get key
		System::String^ key = (System::String^)m_keys[ i ];

		// get value
		ClrVariant^ val = (ClrVariant^)m_parameters[ key ];

		// add parameter
		if( key != nullptr && val != nullptr ) {
			kome::objects::Variant& v = params.getValue( ClrObjectTool::convertString( key ).c_str() );

			val->transmit( v );
		}
	}
}

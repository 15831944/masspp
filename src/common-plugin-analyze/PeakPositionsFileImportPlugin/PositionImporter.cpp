/**
 * @file PositionImporter.cpp
 * @brief implements of PositionImporter class
 *
 * @author S.Tanaka
 * @date 2008.12.10
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "PositionImporter.h"

using namespace kome::position::file;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG


#define FILE_PARAM_NAME					"file"
#define SEPARATOR_PARAM_NAME			"separator"
#define RT_PARAM_NAME                   "rt_column"
#define MZ_PARAM_NAME                   "mz_column"
#define CHARGE_PARAM_NAME               "charge_column"
#define APPENDED_PARAM_NAME             "appended"

// constructor
PositionImporter::PositionImporter( kome::objects::SettingParameterValues* settings ) {
	m_settings = settings;
	setParameters();
}

// destructor
PositionImporter::~PositionImporter() {
}

// execute
void PositionImporter::execute( kome::objects::Peaks2D* peaks ) {
	// check parameter
	_ASSERT( peaks != NULL );

	// buffer
	char line[ 4096 ];

	// check members
	if( m_rtCol <= 0 && m_mzCol <= 0 ) {
		LOG_ERROR_CODE( FMT( "Both of RT Column Number and m/z Column Number is none. At least one of them have to be specified. Input the column number of RT or m/z." ), ERR_OTHER );
		return;
	}

	// open the file
	FILE* fp = fileopen( m_filePath.c_str(), "r" );
	if( fp == NULL ) {
		LOG_ERROR_CODE(
			FMT(
				"Failed to open the file. [%s]",
				m_filePath.c_str()
			),
			ERR_FILE_OPEN_FAILED
		);
		return;
	}

	// add positions
	while( fgets( line, 4096, fp ) != NULL ) {
		// token line
		std::vector< std::string > tokens;
		tokenLine( line, tokens );
		int size = (int)tokens.size();

		// RT, mz
		double rt = -1.0;
		double mz = -1.0;
		int charge = -1;
		bool flg = true;

		if( m_rtCol > 0 && m_rtCol <= size ) {
			rt = todouble( tokens[ m_rtCol - 1 ].c_str(), -1.0 );
		}

		if( m_mzCol > 0 && m_mzCol <= size ) {
			mz = todouble( tokens[ m_mzCol - 1 ].c_str(), -1.0 );
		}

		if( m_chargeCol > 0 && m_chargeCol <= size ) {
			if( !isint( tokens[ m_chargeCol - 1 ].c_str(), 10, &charge ) ){
				charge = -1;
			}
		}

		// add
		if( ( m_rtCol <= 0 || rt >= 0.0 ) && ( m_mzCol <= 0 || mz >= 0.0 ) ) {
			kome::objects::Peak2DElement* peak = peaks->createPeak( rt, mz, -1.0);
			peak->setCharge( charge );
			
			// appended columns
			for( unsigned int i = 0; i < m_columns.size(); i++ ) {
				int colNum = m_columns[ i ].col_number;

				if( colNum <= size ) {
					std::string val = tokens[ colNum - 1 ];

					peak->setParameter( m_columns[ i ].name.c_str(), val.c_str(), m_columns[ i ].type );
					
				}
			}

		}
	}

	// close
	fclose( fp );
}

// token line
void PositionImporter::tokenLine( const char* line, std::vector< std::string >& tokens ) {
	// check the parameter
	if( line == NULL ) {
		return;
	}
	if( m_separator.empty() ) {
		tokens.push_back( line );
	}

	// token
	const char* s = line;
	while( s != NULL && *s != '\0' ) {
		const char* s2 = strstr( s, m_separator.c_str() );

		if( s2 == NULL ) {	// last token
			tokens.push_back( s );
			s = NULL;
		}
		else {
			std::string tok( s, s2 );
			tokens.push_back( tok );

			s = s2 + m_separator.length();
		}
	}
}

// set parameters
void PositionImporter::setParameters() {
	// initialize
	m_filePath = "";
	m_rtCol = -1;
	m_mzCol = -1;
	m_chargeCol = -1;
	m_separator = "\t";

	// get from settings object
	if( m_settings == NULL ) {
		return;
	}

	// input data
	const char* file = m_settings->getParameterValue( FILE_PARAM_NAME );
	m_filePath = NVL( file, "" );

	// RT, m/z, charge
	m_rtCol = m_settings->getIntValue( RT_PARAM_NAME, -1 );
	m_mzCol = m_settings->getIntValue( MZ_PARAM_NAME, -1 );
	m_chargeCol = m_settings->getIntValue( CHARGE_PARAM_NAME, -1 );

	// separator
	const char* separator = m_settings->getParameterValue( SEPARATOR_PARAM_NAME );
	if( separator != NULL ) {
		if( strcmp( separator, "comma" ) == 0 ) {
			m_separator = ",";
		}
		if( strcmp( separator, "pipe" ) == 0 ) {
			m_separator = "|";
		}
	}

	// appended columns
	const char* appended = m_settings->getParameterValue( APPENDED_PARAM_NAME );
	std::string column = NVL( appended, "" );
	if( column.empty() ) {
		return;
	}

	std::vector< std::string > columns;
	kome::plugin::SettingsValue::separateListValue( column.c_str(), columns );

	for( unsigned int i = 0; i < columns.size(); i++ ) {
		std::vector< std::string > vals;
		kome::plugin::SettingsValue::separateListValue( columns[ i ].c_str(), vals );

		int num = -1;
		std::string name;
		std::string type;

		if( vals.size() >= 3 ) {
			num = toint( vals[ 0 ].c_str(), 10, -1 );
			name = vals[ 1 ];
			type = vals[ 2 ];
		}

		if( num > 0 && !name.empty() ) {
			m_columns.resize( m_columns.size() + 1 );

			m_columns.back().col_number = num;
			m_columns.back().name = name;

			m_columns.back().type = kome::ident::TYPE_STRING;
			if( type.compare( "int" ) == 0 ) {
				m_columns.back().type = kome::ident::TYPE_UINT;
			}
			else if( type.compare( "real" ) == 0 ) {
				m_columns.back().type = kome::ident::TYPE_UDOUBLE;
			}

			m_columns.back().id = -1;
		}
	}
}

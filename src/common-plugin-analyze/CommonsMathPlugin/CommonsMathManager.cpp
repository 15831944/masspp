/**
 * @file CommonsMathManager.cpp
 * @brief implements of CommonsMath class.
 *
 * @author satstnka
 * @date 2012.02.02
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation All rights reserved.
 */


#include "stdafx.h"
#include "CommonsMathManager.h"


using namespace kome::math;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
CommonsMathManager::CommonsMathManager() {
}

// destructor
CommonsMathManager::~CommonsMathManager() {
}

// t-test
double CommonsMathManager::tTest( double* s0, const int n0, double* s1, const int n1 ) {
	// parameters
	if( s0 == NULL || s1 <= 0 || s1 == NULL || s1 <= 0 ) {
		LOG_ERROR_CODE( FMT( "T-Test cannot be performed because no samples are specified." ), ERR_OTHER );
		return -1.0;
	}

	// JNI Environment
	kome::java::JavaManager& javaMgr = kome::java::JavaManager::getInstance();
	JNIEnv* env = javaMgr.getEnvironment();
	if( env == NULL ) {
		LOG_ERROR( FMT( "Failed to get JNI Environment." ) );
		return -1.0;
	}

	// TestUtils class
	jclass test = env->FindClass( "Lorg/apache/commons/math/stat/inference/TestUtils;" );
	if( test == NULL ) {
		LOG_ERROR( FMT( "Failed to get the TestUtils class." ) );
		return -1.0;
	}

	// method
	jmethodID mid = env->GetStaticMethodID( test, "tTest", "([D[D)D" );
	if( mid == NULL ) {
		LOG_ERROR( FMT( "Failed to get the TestUtils::tTest method." ) );
		return -1.0;
	}

	// t-test
	jdoubleArray arr0 = env->NewDoubleArray( n0 );
	jdoubleArray arr1 = env->NewDoubleArray( n1 );

	env->SetDoubleArrayRegion( arr0, 0, n0, s0 );
	env->SetDoubleArrayRegion( arr1, 0, n1, s1 );

	double p = env->CallStaticDoubleMethod( test, mid, arr0, arr1 );
	std::string msg = javaMgr.getException();
	if( !msg.empty() ) {
		p = -1.0;
		LOG_ERROR( FMT( "Commons Math(t-test): %s", msg.c_str() ) );
	}

	return p;
}

// ANOVA
double CommonsMathManager::anova( double** vals, int* sizes, int groupCnt ) {
	// parameters
	if( vals == NULL || sizes == NULL || groupCnt <= 0 ) {
		LOG_ERROR( FMT( "There is an empty sample." ) );
		return -1.0;
	}

	// JNI Environment
	kome::java::JavaManager& javaMgr = kome::java::JavaManager::getInstance();
	JNIEnv* env = javaMgr.getEnvironment();
	if( env == NULL ) {
		LOG_ERROR( FMT( "Failed to get JNI Environment." ) );
		return -1.0;
	}

	// TestUtils class
	jclass test = env->FindClass( "Lorg/apache/commons/math/stat/inference/TestUtils;" );
	if( test == NULL ) {
		LOG_ERROR( FMT( "Failed to get the TestUtils class." ) );
		return -1.0;
	}

	// method
	jmethodID mid = env->GetStaticMethodID( test, "oneWayAnovaPValue", "(Ljava/util/Collection;)D" ); 
	if( mid == NULL ) {
		LOG_ERROR( FMT( "Failed to get the TestUtils::oneWayAnovaPValue method." ) );
		return -1.0;
	}

	// collection
	jclass colClass = env->FindClass( "Ljava/util/ArrayList;" );
	if( colClass == NULL ) {
		LOG_ERROR( FMT( "Failed to get the ArrayList class." ) );
		return -1.0;
	}

	jmethodID tmpId = env->GetMethodID( colClass, "<init>", "()V" );
	jobject collection = env->NewObject( colClass, tmpId );
	if( collection == NULL ) {
		LOG_ERROR( FMT( "Failed to create a ArrayList object." ) );
		return -1.0;
	}

	tmpId = env->GetMethodID( colClass, "add", "(Ljava/lang/Object;)Z" );

	for( int i = 0; i < groupCnt; i++ ) {
		jdoubleArray arr = env->NewDoubleArray( sizes[ i ] );
		env->SetDoubleArrayRegion( arr, 0, sizes[ i ], vals[ i ] );
		env->CallIntMethod( collection, tmpId, arr );
	}

	// anova
	double p = env->CallStaticDoubleMethod( test, mid, collection );
	std::string msg = javaMgr.getException();
	if( !msg.empty() ) {
		p = -1.0;
		LOG_ERROR( FMT( "Commons Math(t-test): %s", msg.c_str() ) );
	}

	return p;
}

// get instance
CommonsMathManager& CommonsMathManager::getInstance() {
	// create object
	static CommonsMathManager mgr;

	return mgr;
}

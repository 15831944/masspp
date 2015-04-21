/**
 * @file JavaManager.cpp
 * @brief implements of JavaManager class
 *
 * @author S.Tanaka
 * @date 2012.02.01
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#include "stdafx.h"
#include "JavaManager.h"

#include <wx/wx.h>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>


#ifdef _MSC_VER    
    #include  <wx/msw/registry.h>
#endif    // _MSC_VER


#define JAVA_DIR_NAME     "jre7"


using namespace kome::java;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
JavaManager::JavaManager() {
    // initialize
	m_vm = NULL;
	m_env = NULL;
	m_lib = NULL;
}

// destructor
JavaManager::~JavaManager() {
	destroyJVM();
}

// JVM directory
const char* JavaManager::getJvmDir() {
	// find the directory
	if( m_jvmDir.empty() ) {
		kome::core::MsppManager& msppMgr = kome::core::MsppManager::getInstance();

		// Java Directory
		std::string optDir = getpath( msppMgr.getMsppDir(), "opt" );
		std::string javaDir = getpath( optDir.c_str(), JAVA_DIR_NAME );
		std::string binDir = getpath( javaDir.c_str(), "bin" );
		std::string jvmDir = getpath( binDir.c_str(), "client" );
		if( !fileexists( jvmDir.c_str() ) ) {
			jvmDir = getpath( binDir.c_str(), "server" );
		}

		m_jvmDir = jvmDir;
	}

	// return
	return m_jvmDir.c_str();
}

// create JVM
void JavaManager::createJVM() {
	// destroy
	if( m_vm != NULL || m_env != NULL || m_lib != NULL ) {
		destroyJVM();
	}

	// managers
	kome::core::MsppManager& msppMgr = kome::core::MsppManager::getInstance();
	kome::plugin::PluginManager& plgMgr = kome::plugin::PluginManager::getInstance();

	// class paths
	std::string jarsDir = getpath( msppMgr.getMsppDir(), "jars" );
	std::set< std::string > classpaths;
	std::set< std::string > libpaths;

	for( unsigned int i = 0; i < plgMgr.getNumberOfPlugins(); i++ ) {
		kome::plugin::PluginInfo* plugin = plgMgr.getPlugin( i );
		for( unsigned int j = 0; j < plugin->getNumberOfJavaClassPathes(); j++ ) {
			std::string path = getpath( plugin->getDir(), plugin->getJavaClassPath( j ) );
			std::string dir = getdir( path.c_str() );
			classpaths.insert( path );
			libpaths.insert( dir );
		}
	}

	char classpath[ 0x1000 ];
	sprintf( classpath, "-Djava.class.path=" );

	boost::filesystem::path jarPath( jarsDir.c_str(), boost::filesystem::native );
	boost::filesystem::directory_iterator end;
	int cnt = 0;

	for( boost::filesystem::directory_iterator it( jarPath ); it != end; it++ ) {
		std::string fileName = (*it).path().leaf().string();
		std::string filePath = getpath( jarsDir.c_str(), fileName.c_str() );
		if( cnt > 0 ) {
			strcat( classpath, PATH_SEPARATOR );
		}
		strcat( classpath, filePath.c_str() );
		cnt++;
	}
	
	for( std::set< std::string >::iterator it = classpaths.begin(); it != classpaths.end(); it++ ) {
		if( cnt > 0 ) {
			strcat( classpath, PATH_SEPARATOR );
		}
		strcat( classpath, (*it).c_str() );
		cnt++;
	}
	LOG_DEBUG( FMT( "Java Class Path: %s", classpath ) );


	// options
	std::vector< JavaVMOption > options;
	options.resize( 1 );
	options[ 0 ].optionString = classpath;

	/// library paths
	char libpath[ 0x1000 ];
	sprintf( libpath, "-Djava.library.path=%s", jarsDir.c_str() );
	LOG_DEBUG( FMT( "Java Library Path: %s", libpath ) );
	options.resize( options.size() + 1 );
	options.back().optionString = libpath;
	for( std::set< std::string >::iterator it = libpaths.begin(); it != libpaths.end(); it++ ) {
		sprintf( libpath, "-Djava.library.path=\"%s\"", (*it).c_str() );
		options.resize( options.size() + 1 );
		options.back().optionString = libpath;
	}

	
	// create JVM
#ifdef _MSC_VER
	createWindowsJVM( options );
#else
	createUnixJVM();
#endif    // _MSC_VER

}

#ifdef _MSC_VER
// create JVM under the Windows
void JavaManager::createWindowsJVM( std::vector< JavaVMOption >& options ) {
	// library path
	std::string libpath = getpath( getJvmDir(), "jvm.dll" );

	// args
	JavaVMInitArgs args;
	if( options.size() > 0 ) {
		args.options = &( options[ 0 ] );
		args.nOptions = (int)options.size();
	}

	// version
	int javaVersion = JNI_VERSION_1_6;

	args.version = javaVersion;

	// load library
	HINSTANCE hinst = LoadLibrary( libpath.c_str() );
	m_lib = hinst;
	if( hinst == NULL ) {
		LOG_ERROR( FMT( "Failed to load the library. [%s]", libpath.c_str() ) );
		return;
	}

	typedef jint (JNICALL CreateVMFun)( JavaVM**, JNIEnv**, void* );

	// create virtual machine
	CreateVMFun* fun = (CreateVMFun*)GetProcAddress( hinst, "JNI_CreateJavaVM" );
	if( fun == NULL ) {
		LOG_ERROR( FMT( "Failed to get the function. [JNI_CreateJavaVM]" ) );
	}

	int ret = fun( &m_vm, &m_env, &args );
	if( ret < 0 ) {
		LOG_ERROR( FMT( "Failed to create the Java Virtual Machine" ) );

		m_vm = NULL;
		m_env = NULL;
	}
}

#else

// create JVM under the Unix
void JavaManager::createUnixJVM( std::vector< JavaVMOption >& options ) {
	LOG_WARN( FMT( "This method is under construction" ) );
}

#endif    // _MSC_VER

// destroy JVM
void JavaManager::destroyJVM() {

	// close
#if _MSC_VER
	FreeLibrary( (HINSTANCE)m_lib );
#else

#endif    // _MSC_VER


	// initialize
	m_vm = NULL;
	m_env = NULL;
	m_lib = NULL;

}

// get JVM
JavaVM* JavaManager::getJVM() {
	if( m_vm == NULL ) {
		createJVM();
	}

	return m_vm;
}

// get JNI Environment
JNIEnv* JavaManager::getEnvironment() {
	if( m_env == NULL ) {
		createJVM();
	}

	return m_env;
}

// convert string
std::string JavaManager::getString( jstring s ) {
	// string
	std::string s0;	
	if( m_env == NULL ) {
		LOG_ERROR( FMT( "Java Virtual Machine is not yet created." ) );
		return s0;
	}

	const char* s1 = m_env->GetStringUTFChars( s, NULL );
	s0 = s1;

	// release
	m_env->ReleaseStringUTFChars( s, s1 );

	return s0;
}

// convert string
jstring JavaManager::getString( const char* s ) {
	if( m_env == NULL ) {
		LOG_ERROR( FMT( "Java Virtual Machine is not yet created." ) );
		return NULL;
	}

	jstring s0 = m_env->NewStringUTF( s );
	return s0;
}

// get exception
std::string JavaManager::getException() {
	// string
	std::string s;

	// exception
	if( m_env == NULL ) {
		return s;
	}

	// throwable
	jthrowable throwable = m_env->ExceptionOccurred();
	if( throwable == NULL ) {
		return s;
	}

	// string writer
	jclass stringWriterClass = m_env->FindClass( "Ljava/io/StringWriter;" );
	if( stringWriterClass == NULL ) {
		LOG_ERROR( FMT( "Failed to get the StringWriter class." ) );
		return s;
	}

	jmethodID stringWriterMethod = m_env->GetMethodID( stringWriterClass, "<init>", "()V" );
	if( stringWriterMethod == NULL ) {
		LOG_ERROR( FMT( "Failed to get the StringWriter constructor." ) );
		return s;
	}

	jobject stringWriterObj = m_env->NewObject( stringWriterClass, stringWriterMethod );
	if( stringWriterObj == NULL ) {
		LOG_ERROR( FMT( "Failed to create a StringWriter object." ) );
		return s;
	}

	// print writer
	jclass printWriterClass = m_env->FindClass( "Ljava/io/PrintWriter;" );
	if( printWriterClass == NULL ) {
		LOG_ERROR( FMT( "Failed to get the PrintWriter class." ) );
		return s;
	}

	jmethodID printWriterMethod = m_env->GetMethodID( printWriterClass, "<init>", "(Ljava/io/Writer;)V" );
	if( printWriterMethod == NULL ) {
		LOG_ERROR( FMT( "Failed to get the PrintWriter constructor." ) );
		return s;
	}

	jobject printWriterObj = m_env->NewObject( printWriterClass, printWriterMethod, stringWriterObj );
	if( printWriterObj == NULL ) {
		LOG_ERROR( FMT( "Failed to create a PrintWriter object." ) );
		return s;
	}

	// print stack trace
	jclass throwableClass = m_env->FindClass( "Ljava/lang/Throwable;" );
	if( throwableClass == NULL ) {
		LOG_ERROR( FMT( "Failed to get the Throwable class." ) );
		return s;
	}

	jmethodID throwableMethod = m_env->GetMethodID( throwableClass, "printStackTrace", "(Ljava/io/PrintWriter;)V" );
	if( throwableMethod == NULL ) {
		LOG_ERROR( FMT( "Failed to get the printStackTrace method." ) );
		return s;
	}

	m_env->CallVoidMethod( throwable, throwableMethod, printWriterObj );

	// write
	jclass writerClass = m_env->FindClass( "Ljava/io/Writer;" );
	if( writerClass == NULL ) {
		LOG_ERROR( FMT( "Failed to get the Writer class." ) );
		return s;
	}

	jmethodID writerMethod = m_env->GetMethodID( writerClass, "close", "()V" );
	if( writerMethod == NULL ) {
		LOG_ERROR( FMT( "Failed to get the close method. " ) );
		return s;
	}

	m_env->CallVoidMethod( printWriterObj, writerMethod );

	stringWriterMethod = m_env->GetMethodID( stringWriterClass, "toString", "()Ljava/lang/String;" );
	if( stringWriterMethod == NULL ) {
		LOG_ERROR( FMT( "Failed to get the toString method." ) );
		return s;
	}

	jstring js = (jstring)m_env->CallObjectMethod( stringWriterObj, stringWriterMethod );
	if( js == NULL ) {
		return s;
	}

	s = getString( js );

	return s;
}

// get instance
JavaManager& JavaManager::getInstance() {
	static JavaManager mgr;

	return mgr;
}

/**
 * @file IronPythonScript.h
 * @brief interfaces of IronPythonScript class
 *
 * @author S.Tanaka
 * @date 2012.07.11
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation All rights reserved.
 */


#include "stdafx.h"
#include "IronPythonScript.h"


using namespace kome::script::python;


#include <crtdbg.h>
#ifdef _DEBUG
    #define new new( _NORMAL_BLOCK, __FILE__, __LINE__ )
    #define malloc( s ) _malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif    // _DEBUG



// constructor
IronPythonScript::IronPythonScript() {
	addVariable( "MASSPP_HOME", "Mass++ installation directory." );
	addVariable( "SCRIPT_LOAD_PATH", "Script file path." );
	addVariable( "activeObj", "Active Object. (ClrVariant)" );
	addVariable( "params", "Parameters. (ClrParameters)" );

}

// destructor
IronPythonScript::~IronPythonScript() {
}

// on run
// >>>>>>	@Date:2013/08/02	<Modify>	A.Ozaki
//
System::String^ IronPythonScript::onRun( System::String^ script, kome::clr::ClrVariant var, kome::clr::ClrParameters^ pParams ) {
//
// <<<<<<	@Date:2013/08/02	<Modify>	A.Ozaki
	// return value
	System::String^ ret = "";
	kome::clr::ClrLogger^ logger = kome::clr::ClrLogger::getInstance();

	// manager
	kome::clr::ClrMsppManager^ msppMgr = kome::clr::ClrMsppManager::getInstance();

	// path
	System::String^ msppPath = msppMgr->getMsppDir();
	if( msppPath->EndsWith( "\\" ) ) {
		msppPath = msppPath->Substring( 0, msppPath->Length - 1 );
	}
	msppPath->Replace( "\\", "\\\\" );

	try{
		// engine
		Microsoft::Scripting::Hosting::ScriptEngine^ engine = IronPython::Hosting::Python::CreateEngine();
		
		// runtime
		Microsoft::Scripting::Hosting::ScriptRuntime^ runTime = engine->Runtime;
		
		// set output
		System::IO::MemoryStream^ stream = gcnew System::IO::MemoryStream();
		System::IO::StreamWriter^ writer = gcnew System::IO::StreamWriter( stream );
		runTime->IO->SetOutput( stream,  writer );
		runTime->IO->SetErrorOutput( stream, writer );

		// source
		Microsoft::Scripting::Hosting::ScriptSource^ source = engine->CreateScriptSourceFromString( script );

		// scope
		Microsoft::Scripting::Hosting::ScriptScope^ scope = engine->CreateScope();
		scope->SetVariable( "activeObj", %var );
		scope->SetVariable( "params", pParams );
		scope->SetVariable( "MASSPP_HOME", msppPath );
		scope->SetVariable( "SCRIPT_LOAD_PATH", kome::clr::ScriptWrapper::getScriptFilePath() );

		// execute
		source->Execute( scope );

		// string
		int length = (int)stream->Length;
		cli::array< byte >^ bytes = gcnew cli::array< byte >( length );
		stream->Seek( 0, System::IO::SeekOrigin::Begin );
		stream->Read( bytes, 0, (int)stream->Length );

		ret = System::Text::Encoding::GetEncoding( "utf-8" )->GetString( bytes, 0, (int)stream->Length );
	}
	catch( System::Exception^ e ) {
		Microsoft::Scripting::Hosting::ScriptEngine^ engine = IronPython::Hosting::Python::CreateEngine();
		Microsoft::Scripting::Hosting::ExceptionOperations^ eo = engine->GetService<Microsoft::Scripting::Hosting::ExceptionOperations^>();
		ret = eo->FormatException(e);
	}

	return ret;
}

// on get template
System::String^ IronPythonScript::onGetTemplate() {
	// template
	System::String^ ret = "import clr\n\n"
		+ "clr.AddReferenceToFileAndPath(MASSPP_HOME + \"\\\\bin\\\\CLRManager.dll\")\n"
		+ "clr.AddReferenceToFileAndPath(MASSPP_HOME + \"\\\\bin\\\\CLRPluginTools.dll\")\n\n"
		+ "from kome.clr import *\n\n"
		+ "msObj = ClrMsDataVariant(activeObj)\n";

	return ret;
}

/**
 * @file ScriptWrapper.h
 * @brief interfaces of ScriptWrapper class
 *
 * @author S.Tanaka
 * @date 2012.07.10
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_CLR_SCRIPT_WRAPPER_H__
#define __KOME_CLR_SCRIPT_WRAPPER_H__

// >>>>>>	@Date:2013/07/24	<Modify>	A.Ozaki
//
// Moved the ScriptClass to DataObjects library from CoreToolkits library.
//  ex.
//   kome::core::Script -> kome::objects::Script
//
// <<<<<<	@Date:2013/07/24	<Modify>	A.Ozaki

namespace kome {
	namespace clr {

		ref class TimerWrapper;
// >>>>>>	@Date:2013/07/29	<Add>	A.Ozaki
		ref class	ClrVariant;
		ref class	ClrParameters;
// <<<<<<	@Date:2013/07/29	<Add>	A.Ozaki

		/**
		 * @class ScriptWrapper
		 * @brief script wrapper class to use on CLR environment
		 */
		public ref class ScriptWrapper {
		public:
			/**
			 * @fn ScriptWrapper( kome::objects::Script& script )
			 * @brief constructor
			 * @param[in] script script object
			 */
			ScriptWrapper( kome::objects::Script& script );

			/**
			 * @fn virtual ~ScriptWrapper()
			 * @brief destructor
			 */
			virtual ~ScriptWrapper();

		protected:
			/** script object */
			kome::objects::Script& m_script;

		public:
			/**
			 * @fn kome::objects::Script& getScript()
			 * @brief gets script object
			 * @return script object
			 */
			kome::objects::Script& getScript();

		public:
			/**			
			 * @fn System::String^ getTemplate()
			 * @brief gets the template
			 * @return template
			 */
			System::String^ getTemplate();

		public:
// >>>>>>	@Date:2013/07/29	<Modify>	A.Ozaki
			/**
			 * @fn bool run( System::String^ script, ClrVariant var, ClrParameters ^pParams )
			 * @brief run the script
			 * @param[in] script character string of the script
			 * @param[in] var ActiveObject what is provided by the Variant type
			 * @param[in] pParams pointer of parameter
			 * @return result string
			 */
			System::String^ run( System::String^ script, ClrVariant var, ClrParameters ^pParams );
// <<<<<<	@Date:2013/07/29	<Modify>	A.Ozaki

			/**
			 * @fn bool run()
			 * @brief run the script
			 * @return result string
			 */
			System::String^ run();

		public:
			/**
			 * @fn void clearVariables()
			 * @brief clears variables
			 */
			void clearVariables();

			/**
			 * @fn void addVariable( System::String^ name, System::String^ desc )
			 * @brief adds variable
			 */
			void addVariable( System::String^ name, System::String^ desc );

			/**
			 * @fn unsigned int getNumberOfVariables()
			 * @brief gets the number of variables
			 * @return the number of variables
			 */
			unsigned int getNumberOfVariables();

			/**
			 * @fn System::String^ getVariableName( unsigned int idx )
			 * @brief gets the variable name
			 * @param[in] idx variable index
			 * @return variable name
			 */
			System::String^ getVariableName( unsigned int idx );

			/**
			 * @fn System::String^ getVariableDescription( unsigned int idx )
			 * @brief gets the variable description
			 * @param[in] idx variable index
			 * @return variable description
			 */
			System::String^ getVariableDescription( unsigned int idx );

		public:
			/**
			 * @fn static void setScriptFilePath( System::String^ path )
			 * @brief sets the script file path
			 * @param[in] path script file path
			 */
			static void setScriptFilePath( System::String^ path );

			/**
			 * @fn static System::String^ getScriptFilePath()
			 * @brief gets the script file path
			 * @return script file path
			 */
			static System::String^ getScriptFilePath();

		};
	}
}


#endif	// __KOME_CLR_SCRIPT_WRAPPER_H__

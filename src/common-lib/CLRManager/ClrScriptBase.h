/**
 * @file ClrScriptBase.h
 * @brief interfaces of ClrScriptBase class
 *
 * @author S.Tanaka
 * @date 2012.07.10
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_CLR_SCRIPT_BASE_H__
#define __KOME_CLR_SCRIPT_BASE_H__


#include "ScriptWrapper.h"


namespace kome {
	namespace clr {
		/**
		 * @class ClrScriptBase
		 * @brief abstraction class of script on CLR environment
		 */
#ifndef _MANAGED	// (for doxygen)
		public ref class ClrScriptBase : public ScriptWrapper {
#else
		public ref class ClrScriptBase abstract : public ScriptWrapper {
#endif	// _MANAGED
		public:
			/**
			 * @fn ClrScriptBase()
			 * @brief constructor
			 */
			ClrScriptBase();

			/**
			 * @fn virtual ~ClrScriptBase()
			 * @brief destructor
			 */
			virtual ~ClrScriptBase();

		public:
// >>>>>>	@Date:2013/07/29	<Modify>	A.Ozaki
//
			/**
			 * @fn virtual System::String^ onRun( System::String^ script ) = 0
			 * @brief This method is called from the run method. (abstract method)
			 * @param[in] script character string of the script
			 * @param[in] var ActiveObject what is provided by the Variant type
			 * @param[in] pParams pointer of parameter
			 * @return result string
			 */
			virtual System::String^ onRun( System::String^ script, ClrVariant var, ClrParameters ^pParams ) = 0;
//
// >>>>>>	@Date:2013/07/29	<Modify>	A.Ozaki

			/**
			 * @fn virtual System::String^ onGetTemplate() = 0
			 * @brief This method is called from the getTempalte method. (abstract method)
			 * @return template
			 */
			virtual System::String^ onGetTemplate() = 0;
		};
	}
}

#endif	// __KOME_CLR_SCRIPT_BASE_H__

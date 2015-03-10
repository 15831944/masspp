/**
 * @file IronPythonScript.h
 * @brief interfaces of IronPythonScript class
 *
 * @author S.Tanaka
 * @date 2012.07.11
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation All rights reserved.
 */


#ifndef __KOME_IRONPYTHON_SCRIPT_H__
#define __KOME_IRONPYTHON_SCRIPT_H__


namespace kome {
	namespace script {
		namespace python {
			/**
			 * @class IronPythonScript
			 * @brief abstraction class of script on CLR environment
			 */
			public ref class IronPythonScript : public kome::clr::ClrScriptBase {
			public:
				/**
				 * @fn IronPythonScript()
				 * @brief constructor
				 */
				IronPythonScript();
				
				/**
				 * @fn virtual ~IronPythonScript()
				 * @brief destructor
				 */
				virtual ~IronPythonScript();

			public:
// >>>>>>	@Date:2013/08/02	<Modify>	A.Ozaki
//
				/**
				 * @fn virtual System::String^ onRun( System::String^ script, kome::clr::ClrVariant var, kome::clr::ClrParameters^ pParams ) override
				 * @brief This method is called from the run method. (abstract method)
				 * @param[in] script character string of the script
				 * @param[in] var ActiveObject what is provided by the Variant type
				 * @param[in] pParams pointer of parameter
				 * @return result string
				 */
				virtual System::String^ onRun( System::String^ script, kome::clr::ClrVariant var, kome::clr::ClrParameters^ pParams ) override;
//
// <<<<<<	@Date:2013/08/02	<Modify>	A.Ozaki

				/**
				 * @fn virtual System::String^ onGetTemplate() override
				 * @brief This method is called from the getTempalte method. (abstract method)
				 * @return template
				 */
				virtual System::String^ onGetTemplate() override;
			};
		}
	}
}

#endif	// __KOME_IRONPYTHON_SCRIPT_H__

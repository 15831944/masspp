/**
 * @file IronPythonManager.h
 * @brief interfaces of IronPythonManager class
 *
 * @author S.Tanaka
 * @date 2012.07.11
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation All rights reserved.
 */


#ifndef __KOME_SCRIPT_IRON_PYTHON_MANAGER_H__
#define __KOME_SCRIPT_IRON_PYTHON_MANAGER_H__


namespace kome {
	namespace script {
		namespace python {

			ref class IronPythonScript;

			/**
			 * @class IronPythonManager
			 * @brief IronPython management class
			 */
			public ref class IronPythonManager {
			protected:
				/**
				 * @fn IronPythonManager()
				 * @brief constructor
				 */
				IronPythonManager();

				/**
				 * @fn virtual ~IronPythonManager()
				 * @brief destructor
				 */
				virtual ~IronPythonManager();

			protected:
				/** script */
				IronPythonScript^ m_script;

				/** manager */
				static IronPythonManager^ m_mgr;

			public:
				/**
				 * @fn IronPythonScript^ getScript()
				 * @brief gets the script object
				 * @return script object
				 */
				IronPythonScript^ getScript();

			public:
				/**
				 * @fn static IronPythonManager^ getInstance()
				 * @brief gets the IronPythonManager object. (This is the only object.)
				 * @return IronPythonManager object
				 */
				static IronPythonManager^ getInstance();

			public:
				/**
				 * @fn static kome::clr::ClrVariant getScript( kome::clr::ClrParameters^ params )
				 * @brief gets the ironpython script
				 * @param params plug-in call parameter information
				 * @return return value of plug-in function
				 */
				static kome::clr::ClrVariant^ getScript( kome::clr::ClrParameters^ params );
			};
		}
	}
}

#endif	// __KOME_SCRIPT_IRON_PYTHON_MANAGER_H__

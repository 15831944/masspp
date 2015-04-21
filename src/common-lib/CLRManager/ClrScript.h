/**
 * @file ClrScript.h
 * @brief interfaces of ClrScript class
 *
 * @author S.Tanaka
 * @date 2012.07.10
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_CLR_SCRIPT_H__
#define __KOME_CLR_SCRIPT_H__

// >>>>>>	@Date:2013/07/24	<Modify>	A.Ozaki
//
// Moved the ScriptClass to DataObjects library from CoreToolkits library.
//  ex.
//   kome::core::Script -> kome::objects::Script
//
// <<<<<<	@Date:2013/07/24	<Modify>	A.Ozaki

namespace kome {
	namespace clr {

		ref class ClrScriptBase;
		
		/**
		 * @class ClrScript
		 * @brief script object that has managed class object
		 */
		class ClrScript : public kome::objects::Script {
		public:
			/**
			 * @fn ClrScript()
			 * @brief constructor
			 */
			ClrScript();

			/**
			 * @fn virtual ~ClrScript()
			 * @brief destructor
			 */
			virtual ~ClrScript();

		protected:
			/** base script object */
			gcroot< ClrScriptBase^ > m_baseScript;

		public:
			/**
			 * @fn void setBaseScript( ClrScriptBase^ baseScript )
			 * @brief sets base script object
			 * @param[in] baseScript base script object
			 */
			void setBaseScript( ClrScriptBase^ baseScript );

			/**
			 * @fn ClrScriptBase^ getBaseScript()
			 * @brief gets base script object
			 * @return base script object
			 */
			ClrScriptBase^ getBaseScript();

		protected:
// >>>>>>	@Date:2013/07/29	<Modify>	A.Ozaki
//
			/**
			 * @fn virtual std::string onRun( const char* script )
			 * @brief This method is called from the run method. (override method)
			 * @param[in] script character string of the script
			 * @param[in] var ActiveObject what is provided by the Variant type
			 * @param[in] pParams pointer of parameter
			 * @return result string
			 */
			virtual std::string onRun( const char* script, kome::objects::Variant &var, kome::objects::Parameters *pParams );
//
// >>>>>>	@Date:2013/07/29	<Modify>	A.Ozaki

			/**
			 * @fn virtual std::string onGetTemplate()
			 * @brief This method is called from the getTempalte method. (override method)
			 * @return template
			 */
			virtual std::string onGetTemplate();
		};
	}
}

#endif	// __KOME_CLR_SCRIPT_H__

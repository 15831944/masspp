/**
 * @file Script.h
 * @brief interfarces of Script class
 *
 * @author S.Tanaka
 * @date 2012.07.10
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

// >>>>>>	@Date:2013/07/24	<Modify>	A.Ozaki
//
// Moved the ScriptClass to DataObjects library from CoreToolkits library.
//  ex.
//   CORE_TOOLKITS_CLASS -> DATA_OBJECTS_CLASS
//   core -> objects
//
// <<<<<<	@Date:2013/07/24	<Modify>	A.Ozaki

#ifndef __KOME_OBJECTS_SCRIPT_H__
#define __KOME_OBJECTS_SCRIPT_H__


#include <vector>
#include <set>

// >>>>>>	@Date:2013/07/29	<Add>	A.Ozaki
#include	"Variant.h"
#include	"Parameters.h"
// <<<<<<	@Date:2013/07/29	<Add>	A.Ozaki

namespace kome {
	namespace objects {

		/**
		 * @class Script
		 * @brief script abstract class
		 */
		class DATA_OBJECTS_CLASS Script {
		public:
			/**
			 * @fn Script()
			 * @brief constructor
			 */
			Script();

			/**
			 * @fn virtual ~Script()
			 * @brief destructor
			 */
			virtual ~Script();

		protected:
			/** script */
			std::string m_script;

			/**
			 * @struct VariableInfo
			 * @brief variable information
			 */
			struct VariableInfo {
				std::string name;
				std::string desc;
			};

			/** variables */
			std::vector< VariableInfo > m_variables;

			/** script file path */
			static std::string m_filePath;

// >>>>>>	@Date:2013/07/29	<Add>	A.Ozaki
			/** Active Object */
			kome::objects::Variant		m_refVariant;

			/** parameter */
			kome::objects::Parameters	*m_pParams;
// <<<<<<	@Date:2013/07/29	<Add>	A.Ozaki

		public:
			/**
			 * @fn void setScript( const char* script )
			 * @brief sets the script
			 * @param[in] script character string of the script
			 */
			void setScript( const char* script );

			/**
			 * @fn const char* getScript()
			 * @brief gets the script
			 * @return character string of the script
			 */
			const char* getScript();

// >>>>>>	@Date:2013/07/29	<Add>	A.Ozaki
			/**
			 * @fn void setVariant( kome::objects::Variant &var )
			 * @brief sets the variant(Active Object)
			 * @param[in] var ActiveObject what is provided by the Variant type
			 */
			void	setVariant( kome::objects::Variant &var );

			/**
			 * @fn const kome::objects::Variant &getVariant( void )
			 * @brief gets the variant
			 * @return refer of ActiveObject what is provided by the Variant type
			 */
			const kome::objects::Variant &getVariant( void );

			/**
			 * @fn void setParameters( kome::objects::Parameters *pParams )
			 * @brief sets the variant(Active Object)
			 * @param[in] pParams pointer of parameter
			 */
			void	setParameters( kome::objects::Parameters *pParams );

			/**
			 * @fn const kome::objects::Parameters	*getParameters( void )
			 * @brief gets the parameter
			 * @return pointer of parameter
			 */
			const kome::objects::Parameters	*getParameters( void );
// <<<<<<	@Date:2013/07/29	<Add>	A.Ozaki

		public:
			/**			
			 * @fn std::string getTemplate()
			 * @brief gets the template
			 * @return template
			 */
			std::string getTemplate();

		public:
// >>>>>>	@Date:2013/07/29	<Add>	A.Ozaki
			/**
			 * @fn std::string run( const char* script, kome::objects::Variant &var, kome::objects::Parameters *pParams )
			 * @brief run the script
			 * @param[in] script character string of the script
			 * @param[in] var ActiveObject what is provided by the Variant type
			 * @param[in] pParams pointer of parameter
			 * @return result string
			 */
			std::string run( const char* script, kome::objects::Variant &var, kome::objects::Parameters *pParams );
// <<<<<<	@Date:2013/07/29	<Add>	A.Ozaki

			/**
			 * @fn std::string run()
			 * @brief run the script
			 * @return result string
			 */
			std::string run();

		public:
			/**
			 * @fn void clearVariables()
			 * @brief clears variables
			 */
			void clearVariables();

			/**
			 * @fn void addVariable( const char* name, const char* desc )
			 * @brief adds variable
			 */
			void addVariable( const char* name, const char* desc );

			/**
			 * @fn unsigned int getNumberOfVariables()
			 * @brief gets the number of variables
			 * @return the number of variables
			 */
			unsigned int getNumberOfVariables();

			/**
			 * @fn const char* getVariableName( const unsigned int idx )
			 * @brief gets the variable name
			 * @param[in] idx variable index
			 * @return variable name
			 */
			const char* getVariableName( const unsigned int idx );

			/**
			 * @fn const char* getVariableDescription( const unsigned int idx )
			 * @brief gets the variable description
			 * @param[in] idx variable index
			 * @return variable description
			 */
			const char* getVariableDescription( const unsigned int idx );

		public:
			/**
			 * @fn static void setScriptFilePath( const char* path )
			 * @brief sets the script file path
			 * @param[in] path script file path
			 */
			static void setScriptFilePath( const char* path );

			/**
			 * @fn static const char* getScriptFilePath()
			 * @brief gets the script file path
			 * @return script file path
			 */
			static const char* getScriptFilePath();

		protected:
// >>>>>>	@Date:2013/07/29	<Add>	A.Ozaki
			/**
			 * @fn virtual std::string onRun( const char* script ) = 0
			 * @brief This method is called from the run method. (abstract method)
			 * @param[in] script character string of the script
			 * @param[in] var ActiveObject what is provided by the Variant type
			 * @param[in] pParams pointer of parameter
			 * @return result string
			 */
			virtual std::string onRun( const char* script, kome::objects::Variant &var, kome::objects::Parameters *pParams ) = 0;
// <<<<<<	@Date:2013/07/29	<Add>	A.Ozaki

			/**
			 * @fn virtual std::string onGetTemplate() = 0
			 * @brief This method is called from the getTempalte method. (abstract method)
			 * @return template
			 */
			virtual std::string onGetTemplate() = 0;
		};

		/**
		 * @class ScriptManager
		 * @brief script manager class
		 */
		class ScriptManager {
		protected:
			/**
			 * @fn ScriptManager()
			 * @brief constructor
			 */
			ScriptManager();

			/**
			 * @fn virtual ~ScriptManager()
			 * @brief destructor
			 */
			virtual ~ScriptManager();

		protected:
			/** script set */
			std::set< Script* > m_scriptSet;

		public:
			/**
			 * @fn void addScript( Script* script )
			 * @brief adds script
			 * @param[in] script script object
			 */
			void addScript( Script* script );

			/**
			 * @fn void removeScript( Script* script )
			 * @brief removes script
			 * @param[in] script script object
			 */	
			void removeScript( Script* script );

		public:
			/**
			 * @fn static ScriptManager& getInstance()
			 * @brief gets the instance. (This is the only object.)
			 * @return ScriptManager object
			 */
			static ScriptManager& getInstance();

		};

	}
}

#endif	// __KOME_OBJECTS_SCRIPT_H__

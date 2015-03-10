/**
 * @file VariablesListCtrl.h
 * @brief interfaces of VariablesListCtrl class
 *
 * @author S.Tanaka
 * @date 2012.10.18
 * 
 * Copyright(C) 2012 Shimadzu Corporation. All rights reserved.
 */


#ifndef __KOME_SCRIPT_VARIABLES_LIST_CTRL__
#define __KOME_SCRIPT_VARIABLES_LIST_CTRL__


namespace kome {
	namespace script {

		class ScriptDialog;

		/**
		 * @class VariablesListCtrl
		 * @brief variables list control
		 */
		class VariablesListCtrl : public kome::window::GridListCtrl {
		public:
			/**
			 * @fn VariablesListCtrl( wxWindow* parent, ScriptDialog* dlg )
			 * @brief constructor
			 * @param[in] parent parent
			 * @param[in] dlg script dialog
			 */
			VariablesListCtrl( wxWindow* parent, ScriptDialog* dlg );

			/**
			 * @fn virtual ~VariablesListCtrl()
			 * @brief destructor
			 */
			virtual ~VariablesListCtrl();

		protected:
			/** script */
			kome::objects::Script* m_script;	// @Date:2013/08/06	<Modify>	A.Ozaki

			/** script dialog */
			ScriptDialog* m_dlg;

		public:
			/**
			 * @fn void setScript( kome::objects::Script* script )	// @Date:2013/08/06	<Modify>	A.Ozaki
			 * @breif sets the script
			 * @param[in] script script
			 */
			void setScript( kome::objects::Script* script );	// @Date:2013/08/06	<Modify>	A.Ozaki

			/**
			 * @fn kome::objects::Script* getScript()	// @Date:2013/08/06	<Modify>	A.Ozaki
			 * @brief gets the script
			 * @return script
			 */			 
			kome::objects::Script* getScript();	// @Date:2013/08/06	<Modify>	A.Ozaki

		protected:
			/**
			 * @fn virtual void onCreate()
			 * @brief This method is called by create method
			 */
			virtual void onCreate();

			/**
			 * @fn virtual std::string getString( const long long data, const int column ) const
			 * @brief gets character string value
			 * @param[in] data data
			 * @param[in] column column number
			 */
			virtual std::string getString( const long long data, const int column ) const;

			/**
			 * @fn virtual void onActivateData( const long long data )
			 * @brief This method is called when the data is activated
			 * @param[in] data activated data
			 */
			virtual void onActivateData( const long long data );

		private:
			DECLARE_EVENT_TABLE()
		};
	}
}


#endif    // __KOME_SCRIPT_VARIABLES_LIST_CTRL__

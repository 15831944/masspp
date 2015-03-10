/**
 * @file ScriptListCtrl.h
 * @brief interfaces of ScriptListCtrl class
 *
 * @author A.Ozaki
 * @date 2013.08.05
 * 
 * Copyright(C) 2013-2013 Shimadzu Corporation. All rights reserved.
 */

#ifndef __KOME_SCRIPT_LIST_CTRL_H__
#define __KOME_SCRIPT_LIST_CTRL_H__

#include	"ScriptListDialog.h"


namespace kome{
	namespace script{
		
		/**
		 * @class ScriptListCtrl
		 * @brief peak position list control class
		 */
		class ScriptListCtrl : public kome::window::GridListCtrl {

		public:
			/**
			 * @fn ScriptListCtrl( wxWindow* pParent, const int width, const int height )
			 * @brief constructor
			 * @param pParent parent window
			 * @param width width
			 * @param height height
			 */
			ScriptListCtrl( wxWindow* pParent, const int width, const int height );

			/**
			 * @fn virtual ~ScriptListCtrl( )
			 * @brief destructor
			 */
			virtual ~ScriptListCtrl( );
	
		protected:
			/** script list */
			std::vector< stScriptInfo >	m_vecScriptInfo;

			/** selected no */
			int	m_nSelectedIndex;	// ëIëÇ≥ÇÍÇΩçsî‘çÜ 

			/** parent */
			wxWindow	*m_pParent;

		public:
			/**
			 * @fn unsigned int getNumberOfScripts( void )
			 * @brief gets the number of script
			 * @return the number of script
			 */
			unsigned int getNumberOfScripts( void );

			/**
			 * @fn stScriptInfo getScript( const unsigned int idx )
			 * @brief gets the script
			 * @param[in] idx script index
			 * @return script
			 */
			stScriptInfo getScript( const unsigned int idx );

			/**
			 * @fn void setScript( stScriptInfo insScriptInfo, const unsigned int idx )
			 * @brief sets the script
			 * @param[in] insScriptInfo script info
			 * @param[in] idx script index
			 */
			void setScript( stScriptInfo insScriptInfo, const unsigned int idx );

			/**
			 * @fn void clearScripts( void )
			 * @brief clear Scripts
			 */
			void clearScripts( void );

			/**
			 * @fn void addScriptInfo( stScriptInfo insScriptInfo, const bool bUpdate = false )
			 * @brief add script
			 */
			void addScriptInfo( stScriptInfo insScriptInfo, const bool bUpdate = false );

			/**
			 * @fn void deleteScriptInfo( const long long idx, const bool bUpdate = false )
			 * @brief delete script
			 */
			void deleteScriptInfo( const long long idx, const bool bUpdate = false );
		protected:
			/**
			 * @fn virtual void onCreate( )
			 * @brief This method is called by create method
			 */
			virtual void onCreate( );

			/**
			 * @fn virtual int getInt( const long long data, const int column ) const
			 * @brief gets integer value from data (override method)
			 * @param data data
			 * @param column column number
			 * @return integer value
			*/
			virtual int	getInt( const long long data, const int column ) const;

			/**
			 * @fn virtual double getDouble( const long long data, const int column ) const
			 * @brief gets double value from data
			 * @param data data
			 * @param column column number
			 */
			virtual double	getDouble( const long long data, const int column ) const;

			/**
			 * @fn virtual std::string getString( const long long data, const int column ) const
			 * @brief gets character string value (override method)
			 * @param data data
			 * @param column column number
			 */
			virtual std::string	getString( const long long data, const int column ) const;

		public:
			/**
			 * @fn void onSizePluginMgr( wxSizeEvent& evt )
			 * @brief This method is called when list size is changed
			 * @param[in] evt size event information
			 */
			void onSizeScriptCtrl( wxSizeEvent& evt );	// @date 2013/05/13 <Add> OKADA

		public:
			/**
			 * @fn void OnSelect( wxListEvent& evt )
			 * @brief This method is called when a list item is selected
			 * @param[in] evt list event object
			 */
			void OnSelect( wxListEvent& evt );

			/**
			 * @fn int getSelectedIndex( void )
			 * @brief gets selected index
			 * @return selected raw no
			 */
			int	getSelectedIndex( void );

			/**
			 * @fn void OnSelect( void )
			 * @brief This method is called when a list item is selected
			 */
			void OnSelect( void );

		private:
			DECLARE_EVENT_TABLE( );
		};
	}
}

#endif				// __KOME_SCRIPT_LIST_CTRL_H__

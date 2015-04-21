/**
* @file StatusBarInfo.h
* @brief Status Bar Info
* @author OKADA, H.
* @date 2011/01/12
* Copyright (C) 2014 Shimadzu Corporation All rights reserved.
*/

#ifndef _STATUSBAR_INFO_H_
#define _STATUSBAR_INFO_H_

#define DEFAULT_STATUS_BAR_NAME "common"

namespace kome {
	namespace plugin {

		class PluginInfo;

		/** 
		 * @class StatusBarInfo
		 * @brief Status Bar class
		 */
		class PLUGIN_TOOLS_CLASS StatusBarInfo{
			public:
			
				/**
				 * @fn StatusBarInfo()
				 * @brief constructor
				 */
				StatusBarInfo();
			
				/**
				 * @fn ~StatusBarInfo()
				 * @brief constructor
				 */
				virtual ~StatusBarInfo();

				
				/**
				 * @typedef enum StatusBarTarget
				 * @brief status bar target type No
				 */
				typedef enum{
					STATUS_BAR_TARGET_NONE, 		// none(almighty)
					STATUS_BAR_TARGET_SPECTRUM, 	// spectrum
					STATUS_BAR_TARGET_CHROMATOGRAM, // chromatogram
					STATUS_BAR_TARGET_DATAMAP 		// datamap
				}StatusBarTargetNo;

				/**
				 * @typedef enum StatusBarTarget
				 * @brief status bar target type Value. ( ex. 1=none 2=spectrum, 7=none & spectrum & chromatogram )
				 */
				typedef enum{
					VAL_SB_TGT_OTHER,												// zero, illegal value
					VAL_SB_TGT_NONE			= (1 << STATUS_BAR_TARGET_NONE),		// none(almighty)
					VAL_SB_TGT_SPECTRUM		= (1 << STATUS_BAR_TARGET_SPECTRUM),	// spectrum
					VAL_SB_TGT_CHROMATOGRAM	= (1 << STATUS_BAR_TARGET_CHROMATOGRAM),// chromatogram
					VAL_SB_TGT_DATAMAP		= (1 << STATUS_BAR_TARGET_DATAMAP)		// datamap
				}StatusBarTargetValue;
		protected:
			
				/** order */
				int m_nOrder;

				/** status bar name */
				std::string m_status_bar_name;

				/** status bar target */
				StatusBarTargetValue m_targetVal;		// StatusBarTargetValue

				/** display message */
				std::string m_status_bar_message;

				/** m_flgDisp - 0:このステータスバーを非表示 1:表示 */
				int m_flgDisp;

				/** plug-in */
				std::vector< PluginInfo* > m_vpPlugin;

				int m_width;  // @date 2011.10.19 <Add> M.Izumi
			

		public:
			/**
			 * @fn void clearStatusBarInfo( void )
			 * @brief clear status bar info
			 */
			void clearStatusBarInfo( void );

			/**
			 * @fn void setTargetNo( const StatusBarTargetNo target_no )
			 * @brief sets menu target
			 * @param[in] target_no menu target no
			 */
			void setTargetNo( const StatusBarTargetNo target_no );

			/**
			 * @fn void setTargetVal( const StatusBarTargetValue target_val )
			 * @brief sets menu target
			 * @param[in] target_val menu target value
			 */
			void setTargetVal( const StatusBarTargetValue target_val );

			/**
			 * @fn void setTarget( const char* target )
			 * @brief sets status bar target
			 * @param[in] target character string meaning target
			 */
			void setTarget( const char* target );

			/**
			 * @fn StatusBarTargetValue getTargetVal( void )
			 * @brief gets menu target
			 * @return menu target
			 */
			StatusBarTargetValue getTargetVal( void );

			/**
			 * @fn void setName( const char* name )
			 * @brief sets status bar name
			 * @param[in] name character string meaning name
			 */
			void setName( const char* name );

			/**
			 * @fn const char* getName( void )
			 * @brief sets status bar name
			 * @return name
			 */
			const char* getName( void );

			/**
			 * @fn std::string getNameString( void )
			 * @brief sets status bar name
			 */
			std::string getNameString( void );

			/**
			 * @fn void setOrder( int order )
			 * @brief set status bar order
			 * @param[in] order the order to sort the status bar
			 */
			void setOrder( int order );

			/**
			 * @fn int getOrder( void )
			 * @brief get status bar order
			 * @return  the order to sort the status bar.
			 */
			int getOrder( void );

			/**
			 * @fn void setFlgDisp( int flgDisp )
			 * @brief set status bar display flag
			 * @param[in] flgDisp 0: not displayed, 1: displayed
			 */
			void setFlgDisp( int flgDisp );

			/**
			 * @fn int getFlgDisp( void )
			 * @brief get status bar display flag
			 * @return  0: not displayed, 1: displayed
			 */
			int getFlgDisp( void );

			/**
			 * @fn void setPlugin( PluginInfo* plugin )
			 * @brief sets plug-in
			 * @param[in] plugin plug-in
			 */
			void setPlugin( PluginInfo* plugin );

			/**
			 * @fn PluginInfo* getPlugin( void )
			 * @brief gets the 1st plug-in 1st
			 * @return plug-in
			 */
			PluginInfo* getPlugin( void );

			/**
			 * @fn PluginInfo* getPlugin( int iCnt )
			 * @brief gets plug-in
			 * @param[in] iCnt plug-in no
			 * @return plug-in
			 */
			PluginInfo* getPlugin( int iCnt );

			/**
			 * @fn void setMessage( const char* message )
			 * @brief set message
			 * @param[in] message
			 */
			void setMessage( const char* message );

			/**
			 * @fn const char* getMessage( void )
			 * @brief get message
			 * @return message
			 */
			const char* getMessage( void );
	
			/**
			 * @fn bool operator <(const StatusBarInfo& a)const
			 * @brief operator <
			 * @return true / false
			 */
			bool operator <(const StatusBarInfo& a)const;

			/**
			 * @fn bool operator <=(const StatusBarInfo& a)const
			 * @brief operator <=
			 * @return true / false
			 */
			bool operator <=(const StatusBarInfo& a)const;
						
			/**
			 * @fn int getWidth()
			 * @brief get statusbar width
			 * @return statusbar width
			 */
			int getWidth();

			/**
			 * @fn void setWidth( int width )
			 * @brief set statusbar width
			 * @param[in] width
			 */
			void setWidth( int width );
		};

	}
}


#endif


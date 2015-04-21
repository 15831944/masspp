/**
 * @file StatusBar.h
 * @brief Status Bar
 * @author OKADA, H.
 * @date 2011/01/07
 * Copyright (C) 2014 Shimadzu Corporation All rights reserved.
 */

#ifndef _STATUSBAR_H_
#define _STATUSBAR_H_

#include <wx/statusbr.h> 
#include "MainFrame.h"
#include "wx/frame.h"

namespace kome {
	namespace window {
		
		/** 
		 * @class StatusBar
		 * @ brief Status Bar class
		 */
		class WINDOW_MANAGER_CLASS StatusBar : public wxFrameBase {
			
		protected:
			/**
			 * @fn StatusBar()
			 * @brief constructor
			 */
			StatusBar();
			
			/**
			 * @fn ~StatusBar()
			 * @brief constructor
			 */
			virtual ~StatusBar();
			
		public:
			/**
			 * @ typedef enum StatusBarTarget
			 * @brief status bar target type 
			 */
			typedef enum{
				STATUS_BAR_TARGET_NONE, 		// none
				STATUS_BAR_TARGET_SPECTRUM, 	// spectrum
				STATUS_BAR_TARGET_CHROMATOGRAM, // chromatogram
				STATUS_BAR_TARGET_DATAMAP 		// datamap
			}StatusBarTarget;

		public:
			/**
			 * @fn static StatusBar& getInstance( void )
			 * @brief get StatusBar object
			 * @return refer to the only StatusBar object
			 */
			static StatusBar& getInstance( void );

		protected:
			/**
			 * @fn void getStatusBarInfo( void )
			 * @brief get getStatusBarInfo / xmlファイルから読み込まれたステータスバー情報を全て取得する
			 */
			void getStatusBarInfo( void );
			
		public:
			/**
			 * @fn int setRangeStatusMsg( const char* szPos, const char* szMessage )
			 * @brief get setRangeStatusMsg / ステータスバーに表示させたい文字列を設定
			 * @param[in] szPos ステータスバーの名前
			 * @param[in] szMessage ステータスバーに表示させたい文字列を指定
			 * @return plug-in szPosに名前が一致するステータスバーが見つかりませんでした
			 */
			int setRangeStatusMsg( const char* szPos, const char* szMessage );

		protected:
			/**
			 * @fn void getMainWindow( void );
			 * @brief メインウインドウへのポインタを取得
			 */
			void getMainWindow( void );

		protected:
			/**
			 * @fn int isDispNow( kome::plugin::StatusBarInfo& statusBarInfo )
			 * @brief 引数で指定したステータスバーを表示させるか否か判別する
			 * @param[in] statusBarInfo 判断するステータスバーを指定
			 * @return 0=表示すべきでない 1=表示すべき
			 */
			int isDispNow( kome::plugin::StatusBarInfo& statusBarInfo );

		protected:
			/**
			 * @fn kome::plugin::StatusBarInfo::StatusBarTargetValue getActiveWindowKind( void )
			 * @brief 現在アクティブなウインドウの種類を取得する
			 * @return kome::plugin::StatusBarInfo::StatusBarTargetValue 
							VAL_SB_TGT_OTHER		アクティブなウインドウが取得できない
							VAL_SB_TGT_NONE			アクティブなウインドウは、spectrum, chromatogram, datamapのどれでもない
							VAL_SB_TGT_SPECTRUM		アクティブなウインドウは、spectrum
							VAL_SB_TGT_CHROMATOGRAM	アクティブなウインドウは、chromatogram
							VAL_SB_TGT_DATAMAP		アクティブなウインドウは、datamap

			 */
			kome::plugin::StatusBarInfo::StatusBarTargetValue getActiveWindowKind( void );

		public:
			/**
			 * @fn void dispAuto( void )
			 * @brief 全てのステータスバーをチェックして、表示すべきものを表示
			 */
			void dispAuto( void );

		protected:
			/**
			 * @fn int getPositionByName( const char* szName )
			 * @brief 引数で指定された名前のステータスバーの位置（左から数えて何番目か）を返す。
			 * @param[in] szName ステータスバー名
			 * @return int ステータスバーの位置。左から順に、0,1,2…と数える。
			 */
			int getPositionByName( const char* szName );

		public:
			/**
			 * @fn int setFlgDisp(  const char* szPos, int flgDisp )
			 * @brief 指定したステータスバーの表示/非表示を決める
			 * @param[in] szPos ステータスバー名
			 * @param[in] flgDisp 0:このステータスバーを非表示 0以外:表示
			 * @return 0:成功 -1:szPosに一致するステータスバーはない
			 */
			int setFlgDisp(  const char* szPos, int flgDisp );

			/**
			 * @fn int setStatusMsg( const char* szPos, const char* szMessage )
			 * @brief get setStatusMsg / ステータスバーに表示させたい文字列を設定
			 * @param[in] szPos ステータスバーの名前
			 * @param[in] szMessage ステータスバーに表示させたい文字列を指定
			 * @return plug-in szPosに名前が一致するステータスバーが見つかりませんでした
			 */
			int setStatusMsg( const char* szPos, const char* szMessage );
 		private:
			
			/** status bar */
			wxStatusBar *m_pStatbar;

			/** status bar info */
			std::vector< kome::plugin::StatusBarInfo > m_totalStatusBarInfo;

			/** MainFrame */
			kome::window::MainFrame* m_pFrame;

		};
	}
}


#endif

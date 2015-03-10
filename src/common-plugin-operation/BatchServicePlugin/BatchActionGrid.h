/**
 * @file BatchActionGrid.h
 * @brief interfaces of BatchActionGrid class
 *
 * @author OKADA, H
 * @date 2012-02-30
 * 
 * Copyright(C) 2012 Shimadzu Corporation. All rights reserved.
 */

#ifndef __KOME_BATCH_SERVICE_BATCH_ACTION_GRID_H__
#define __KOME_BATCH_SERVICE_BATCH_ACTION_GRID_H__

#include <vector>
#include "BatchServicePluginManager.h"


namespace kome {
	namespace batchservice {
		
		class BatchActionGrid : public kome::window::EditGridListCtrl {
		public:

			/**
			 * @fn BatchActionGrid(
					wxWindow* parent,
					const unsigned int nPanelNo,
					const unsigned int nActionKind
				)
			 * @brief constructor
			 * @param parent parent window
			 * @param nPanelNo タブ(1)-(6)の番号
			 * @param nActionKind 1:Available Actionsを初期化
			 */
			BatchActionGrid( wxWindow* parent, int nPanelNo, int nActionKind );    // コンストラクタ

			/**
			 * @fn virtual ~BatchActionGrid()
			 * @brief destructor
			 */
			virtual ~BatchActionGrid();    // デストラクタ

		protected:
			/** セルの行数 */
			int m_nRows;

			/** セルの列数 */
			int m_nCols;

			/** parent window */
			wxWindow* m_pParent;

			/** パネル（タブ）番号 (0-5) */
			int m_nPanelNo;

			/** 0=Selected Actions, 1=Available Actions */
			int m_nActionKind;

		public:	// @date 2013/07/02 <Add> OKADA	// SPEC 93492
			/**
			 * @fn void BatchActionGrid::init( void )
			 * @brief initialize
			 */
			void BatchActionGrid::init( void );	// 初期化

		protected:
			/**
			 * @fn virtual void onCreate()
			 * @brief This method is called by create method (override method)
			 */
			virtual void onCreate();		// テーブル作成

		public:
			// Actionを追加
			/**
			 * @fn int addAction( Action& act )
			 * @brief add action
			 * @param act new action
			 * @return result of add. -1=error(not added) 0=success(added)
			 */
			int addAction( Action& act );

			/**
			 * @fn int updateAction( int index, Action act )
			 * @brief update line
			 * @param index This line is overwite by act.
			 * @param act   new action
			 * @return result of update. -1=error(not updated) 0=success(updated)
			 */
			int updateAction( int index, Action act );

			/**
			 * @fn int deleteAction( int index )
			 * @brief delete line
			 * @param index This line is deleted.
			 * @return result of delete. -1=error(not deleted) 0=success(deleted)
			 */
			int deleteAction( int index );

			/**
			 * @fn int RefreshBatchActionGrid( void )
			 * @brief redraw sheet
			 * @return always 0
			 */
			int RefreshBatchActionGrid( void );
			
			// 引数で指定された行を入れ替える
			/**
			 * @fn int swapAction( int nIndex1, int nIndex2 )
			 * @brief swap action value by index
			 * @param index1 1st line
			 * @param index2 2nd line
			 * @return result of swap. -1=error(not swapped) 1=success(swapped)
			 */
			int swapAction( int nIndex1, int nIndex2 );


			// BatchActionGridが持っているActionの個数を返す
			/**
			 * @fn int getActionSize( void )
			 * @brief gets action size
			 * @return Action size
			 */
			int getActionSize( void ){
				return m_vectActions.size();
			}

			// BatchActionGridが持っているActionを返す
			/**
			 * @fn Action getAction( int index )
			 * @brief gets action value
			 * @param index index of Actions
			 * @return Action value
			 */
			Action getAction( int index );

		protected:
			/**
			 * @fn void onColClick( wxListEvent& evt )
			 * @brief This method is called when list column is clicked
			 * @param[in] evt list event information
			 */
			void onColClick( wxListEvent& evt );

			/**
			 * @fn void onDoubleClick( wxListEvent& evt )
			 * @brief This method is called when list column is double-clicked
			 * @param[in] evt list event information
			 */
			void onDoubleClick( wxListEvent& evt );

			/**
			 * @fn virtual bool onDeleteData( const long long data )
			 * @brief This method is called when a item is deleted.
			 * @param[in] data deleted item data
			 * @return If true, succeeded to delete the item.
			 */
			virtual bool onDeleteData( const long long data );
		protected:
			/**
			 * @fn virtual int getInt( const long long data, const int column ) const
			 * @brief gets interger value
			 * @param data data
			 * @param col column ID
			 * @return integer value
			 */
			virtual int getInt( const long long data, const int col ) const;

			/**
			 * @fn virtual std::string getString( const long long data, const int col ) const;
			 * @brief gets string
			 * @param data data
			 * @param col column ID
			 * @return string value
			 */
			virtual std::string getString( const long long data, const int col ) const;

			/**
			 * @fn int BatchActionGrid::setRows( int nRow );
			 * @brief Set Grid lines. This method is not used any longer. 
			 * @param[in] nRow Grid Size(row)
			 */
			int BatchActionGrid::setRows( int nRow );

			/** アクションリスト：①Initialize～⑥FilterResults */
			std::vector<Action>  m_vectActions;

		public:
			/**
			 * @fn void getVectActions( void )
			 * @brief This method is return m_vectActions
			 */
			std::vector<Action> getVectActions( void ){ return m_vectActions; };

			/**
			 * @fn void getVectActions( void )
			 * @brief This method is return m_vectActions
			 * @param[in] index index of m_vectActions
			 */
			Action getVectActions( int index ){ 
				return m_vectActions[index]; 
			};

			/**
			 * @fn void getCountOfVectActions( void )
			 * @brief This method is return m_vectActions's size
			 */
			int getCountOfVectActions( void ){ 
				return m_vectActions.size(); 
			};

	
		private:
			DECLARE_EVENT_TABLE()
		};

	}
}

#endif

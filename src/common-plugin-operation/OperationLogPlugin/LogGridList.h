/**
 * @file LogGridList.h
 * @brief interfaces of OperationManager class
 *
 * @author M.Izumi
 * @date 2011.12.09
 * 
 * Copyright (C) 2014 Shimadzu Corporation All rights reserved.
 */


#ifndef __KOME_OPERATION_LOG_GRID_LIST_H__
#define __KOME_OPERATION_LOG_GRID_LIST_H__

#include <wx/wx.h>
#include <vector>

namespace kome {
	namespace operation {
		
		/**
		 * @class LogGridList
		 * @brief Log GridList ctrl class
		 */
		class LogGridList : public kome::window::GridListCtrl {
		
		public:
			/**
			 * @fn LogGridList()
			 * @brief constructor
			 */
			LogGridList( wxWindow* parent );

			/**
			 * @fn virtual ~LogGridList()
			 * @brief destructor
			 */
			virtual ~LogGridList();
					
		protected:
			/** operation info */
			typedef struct{
				std::string strName;
				int			iType;
				std::string strDescription;
				kome::operation::Operation* opt;
			}OperationInfo;

			/** operation infos */
			std::vector< OperationInfo > m_optInfo;

		protected:
			/**
			 * @fn virtual void onCreate()
			 * @brief This method is called by create method
			 */
			virtual void onCreate();

			/**
			 * @fn virtual std::string getString( const long long data, const int column ) const
			 * @brief gets character string value (override method)
			 * @param data data
			 * @param column column number
			 */
			virtual std::string getString( const long long data, const int column ) const;

			/**
			 * @fn void addDataInfo()
			 * @brief add info
			 */
			void addDataInfo();
					
			/**
			 * @fn std::string getStrType( int i ) const
			 * @brief get str type
			 * @param i
			 */
			std::string getStrType( int i ) const;
			
		public:
			/**
			 * @fn void UpdateLogList()
			 * @brief update log list
			 */
			void UpdateLogList();

			/**
			 * @fn void getSelectedOperation( std::vector< kome::operation::Operation* >& operations )
			 * @brief get selected operation
			 * @param[out] operations
			 */
			void getSelectedOperation( std::vector< kome::operation::Operation* >& operations );
		private:
			DECLARE_EVENT_TABLE()
		};

	}
}

#endif	// __KOME_OPERATION_LOG_GRID_LIST_H__

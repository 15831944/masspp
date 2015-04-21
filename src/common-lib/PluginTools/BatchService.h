/**
* @file BatchService.h
* @brief Batch Service
* @author OKADA, H.
* @date 2012/02/07
* Copyright (C) 2012 Shimadzu Corporation All rights reserved.
*/

#include "stdafx.h"
#include "BatchInfo.h"

#ifndef _BATCH_SERVICE_H_
#define _BATCH_SERVICE_H_

namespace kome {
	namespace plugin {

		class PluginInfo;

		/** 
		 * @class BatchService
		 * @brief BatchService class
		 */
		class PLUGIN_TOOLS_CLASS BatchService{

		public:
			/**
			 * @fn BatchService
			 * @brief constructor
			 */
			BatchService();
			
			/**
			 * @fn BatchService
			 * @brief destructor
			 */
			virtual ~BatchService();

		public:
			/**
			 * @typedef BatchStatus
			 * @brief batch status
			 */
			typedef enum {
				STATUS_WAITING = 0,
				STATUS_RUNNING = 1,
				STATUS_COMPLETED = 2,
				STATUS_ERROR = 3
			} BatchStatus;

		public:			
			/**
			 * @fn std::string execute(BatchInfo* ptrBatchInfo)
			 * @brief A batch program is run. The character string for specifying a batch job as a return value is returned. 
			 * @param[in] ptrBatchInfo pointer of BatchInfo
			 * @return character string for specifying a batch job
			 */
			std::string execute(BatchInfo* ptrBatchInfo);
			
			/**
			 * @fn BatchStatus getStatus( const char* str )
			 * @brief The run state of a batch is acquired. The character string for specifying the batch job acquired at the time of execute execution is used as an argument. 
			 * @param[in] str character string for specifying a batch job.
			 * @return BatchStatus
			 */
			BatchStatus getStatus( const char* str );
			
		protected:
			/**
			 * @fn std::string execute(BatchInfo* ptrBatchInfo)
			 * @brief The abstract method called from execute(). 
			 * @param[in] ptrBatchInfo pointer of BatchInfo
			 * @return character string for specifying a batch job
			 */
			virtual std::string onExecute(BatchInfo* ptrBatchInfo) = 0;

			/**
			 * @fn BatchStatus onGetStatus( const char* str )
			 * @brief The abstract method called from getStatus(). 
			 * @param[in] str character string for specifying a batch job.
			 * @return BatchStatus
			 */
			virtual BatchStatus onGetStatus( const char* str ) = 0;
			
		};
	}
}


#endif


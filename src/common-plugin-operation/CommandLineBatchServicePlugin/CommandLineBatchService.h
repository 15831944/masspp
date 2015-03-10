/**
 * @file CommandLineBatchService.h
 * @brief interfaces of plug-in functions
 *
 * @author satstnka
 * @date 2012.03.28
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation All rights reserved.
 */


#ifndef __KOME_COMMAND_LINE_BATCH_SERVICE__H__
#define __KOME_COMMAND_LINE_BATCH_SERVICE__H__


namespace kome {
    namespace batchservice {
        namespace command {

			/**
			 * @class CommandLineBatchService
			 * @brief command line batch service
			 */
			class CommandLineBatchService : public kome::plugin::BatchService {
			public:
				/**
				 * @fn CommandLineBatchService()
				 * @brief constructor
				 */
				 CommandLineBatchService();

				 /**
				  * @fn virtual ~CommandLineBatchService()
				  * @brief destructor
				  */
				 virtual ~CommandLineBatchService();

			protected:
				/**
				 * @fn void deleteFiles()
				 * @brief deletes files
				 */
				void deleteFiles();

			protected:
				/**
				 * @fn std::string execute( kome::plugin::BatchInfo* ptrBatchInfo )
				 * @brief The override method called from execute().
				 * @param[in] ptrBatchInfo pointer of BatchInfo
				 * @return character string for specifying a batch job
				 */
				virtual std::string onExecute( kome::plugin::BatchInfo* ptrBatchInfo );

				/**
				 * @fn BatchStatus onGetStatus( const char* str );
				 * @brief The override method called from getStatus().
				 * @param[in] str character string for specifying a batch job.
				 * @return BatchStatus
				 */
				virtual BatchStatus onGetStatus( const char* str );

			protected:
				/**
				 * @fn static std::string getCommandDir()
				 * @brief gets the command directory
				 * @return command directory
				 */
				static std::string getCommandDir();
			};

			/**
			 * @class CommandLineBatchServiceManager
			 * @brief the manager of command line batch service
			 */
			class CommandLineBatchServiceManager {
			protected:
				/**
				 * @fn CommandLineBatchServiceManager()
				 * @brief constructor
				 */
				CommandLineBatchServiceManager();

				/**
				 * @fn virtual ~CommandLineBatchServiceManager()
				 * @brief destructor
				 */
				virtual ~CommandLineBatchServiceManager();

			protected:
				/** batch service */
				CommandLineBatchService m_service;

			public:
				/**
				 * @fn CommandLineBatchService* getService()
				 * @brief gets the service object
				 * @return service object
				 */
				CommandLineBatchService* getService();

			public:
				/**
				 * @fn static CommandLineBatchServiceManager& getInstance()
				 * @brief gets the CommandLineBatchServiceManager object. (This is the only object.)
				 * @return the CommandLineBatchServiceManager object
				 */
				static CommandLineBatchServiceManager& getInstance();
			};
		}
	}
}


#endif    // __KOME_COMMAND_LINE_BATCH_SERVICE__H__

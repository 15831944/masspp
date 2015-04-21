/**
 * @file Operation.h
 * @brief implements of GridEmailTextCtrl class
 *
 * @author M.Izumi
 * @date 2011.12.06
 * 
 * Copyright (C) 2014 Shimadzu Corporation All rights reserved.
 */


#ifndef __KOM_OPERATION_OPERATION_H__
#define __KOM_OPERATION_OPERATION_H__


namespace kome {
	namespace operation {
		/**
		 * @class Operation
		 * @brief Operation information class
		 */
		class DATA_OPERATION_CLASS Operation {
		public:
			/**
			 * @fn Operation()
			 * @brief constructor
			 */
			Operation();

			/**
			 * @fn virtual ~Operation()
			 * @brief destructor
			 */
			virtual ~Operation();
		
		protected:
			/** operation name */
			std::string m_operatName;
			
			/** short name */
			std::string m_shortName;
			
			/** operation type */
			int m_type;
			
			/** descript */
			std::string m_descript;
			
			/** target sample */
			kome::objects::Sample* m_targetSample;
			
			/** parameter string */
			std::string m_paramString;
			
			/** previouse data file path */
			std::string m_prevPath;
			
			/** update data file path */
			std::string m_updatePath;
						
		public:	
			/**
			 * @typedef OperationType
			 * @brief operation type
			 */
			typedef enum {
				TYPE_STANDARD = 0,   
				TYPE_GUI      = 1,
				TYPE_INPUT    = 2,
				TYPE_OUTPUT   = 4,
				TYPE_AUTO     = 8
			} OperationType;

		public:
			/**
			 * @fn void setOperationName( const char* operationName )
			 * @brief sets operation name
			 * @param[in] operationName  operation name
			 */
			void setOperationName( const char* operationName );
			
			/**
			 * @fn const char* getOperationName()
			 * @brief gets operation name
			 * @return operation name
			 */
			const char* getOperationName();
			
			/**
			 * @fn void setShortName( const char* shortName )
			 * @brief sets short name
			 * @param[in] shortName short name
			 */
			void setShortName( const char* shortName );
			
			/**
			 * @fn const char* getShortName()
			 * @brief gets short name
			 * @return short name
			 */
			const char* getShortName();
			
			/**
			 * @fn void setOperationType( int type )
			 * @brief sets operation type
			 * @param[in] type operaton type
			 */
			void setOperationType( int type );
			
			/**
			 * @fn int getOperationType()
			 * @brief gets operation type
			 * @return operaton type
			 */
			int getOperationType();
			
			/**
			 * @fn void setDescription( const char* description )
			 * @brief sets description
			 * @param[in] description operaton description
			 */
			void setDescription( const char* description );
			
			/**
			 * @fn const char* getDescription()
			 * @brief gets description
			 * @return operaton description
			 */
			const char* getDescription();
			
			/**
			 * @fn void setTargetSample( kome::objects::Sample* sample )
			 * @brief sets target sample objects
			 * @param[in] sample target sample objects
			 */
			void setTargetSample( kome::objects::Sample* sample );
			
			/**
			 * @fn kome::objects::Sample* getTargetSample()
			 * @brief gets target sample objects
			 * @return target sample objects
			 */
			kome::objects::Sample* getTargetSample();

			/**
			 * @fn void setParametersString( const char* parameter )
			 * @brief sets parameters string
			 * @param[in] parameter parameters string
			 */
			void setParametersString( const char* parameter );
			
			/**
			 * @fn const char* getParametersString()
			 * @brief gets operaton parameters string
			 * @return operaton parameters string
			 */
			const char* getParametersString();

			/**
			 * @fn void loadCondition( boost::function< int ( void*, int ) > readFun )
			 * @brief load the saved state with operations
			 * @param[in] readFun function to read saved state
			 */
			void loadCondition( boost::function< int ( void*, int ) > readFun );
			
			/**
			 * @fn void saveCondition( boost::function< int ( void*, int ) > writeFun  )
			 * @brief save the current state with engine operations
			 * @param[in] writeFun function to save state
			 */
			void saveCondition( boost::function< int ( void*, int ) > writeFun  );
			
			/**
			 * @fn bool execute()
			 * @brief execute operation
			 * @return If true, it succeeded to perform search.
			 */
			bool execute();
			
			/**
			 * @fn std::string getPrevFilePath()
			 * @brief gets previouse data file path
			 * @return previouse data file path
			 */
			std::string getPrevFilePath();
			
			/**
			 * @fn void setPrevFilePath( const char* strPath )
			 * @brief sets previouse data file path
			 * @param[in] strPath previouse data file path
			 */
			void setPrevFilePath( const char* strPath );

			/**
			 * @fn std::string getUpdateFilePath()
			 * @brief gets udate data file path
			 * @return udate data file path
			 */
			std::string getUpdateFilePath();

			/**
			 * @fn void setUpdateFilePath( const char* strPath )
			 * @brief sets udate data file path
			 * @param[in] strPath update data file path
			 */
			void setUpdateFilePath( const char* strPath );

			/**
			 * @fn void removeOperation( Operation* opt )
			 * @brief remove operation
			 * @param[in] opt delete operation
			 */
			void removeOperation( Operation* opt );
		protected:
			/**
			 * @fn virtual std::string onGetDescription()
			 * @brief get discription
			 * @return discription
			 */
			virtual std::string onGetDescription() = 0;
			
			/**
			 * @fn virtual std::string onGetParametersString()
			 * @brief get parameters string 
			 * @return parameters string 
			 */
			virtual std::string onGetParametersString() = 0;
			
			/**
			 * @fn virtual void onSetParametersString( const char* strParam )
			 * @brief set parameters string 
			 * @param[in] strParam parameters string
			 */
			virtual void onSetParametersString( const char* strParam ) = 0;
			
			/**
			 * @fn virtual void onLoadCondition( boost::function< int ( void*, int ) > readFun )
			 * @brief load the saved state with respect to to search engine operations
			 * @param[in] readFun function to read saved state
			 */
			virtual void onLoadCondition( boost::function< int ( void*, int ) > readFun ) = 0;
			
			/**
			 * @fn virtual void onSaveCondition( boost::function< int ( void*, int ) > writeFun )
			 * @brief save the current state with respect to search engine operations
			 * @param[in] writeFun function to save state
			 */
			virtual void onSaveCondition( boost::function< int ( void*, int ) > writeFun ) = 0;
			
			/**
			 * @fn virtual bool onExecute()
			 * @brief execute operation
			 * @return If true, it succeeded to perform search.
			 */
			virtual bool onExecute() = 0;
				
			
		};
	}
}

#endif	// __KOM_OPERATION_OPERATION_H__

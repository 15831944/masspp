/**
 * @file AlignmentOperation.h
 * @brief Alignment Operation class
 *
 * @author M.Izumi
 * @date 2012.03.12
 * 
 * Copyright (C) 2014 Shimadzu Corporation All rights reserved.
 */

#ifndef __KOME_ALIGNMENT_OPERATION_H__
#define __KOME_ALIGNMENT_OPERATION_H__

namespace kome{
	namespace alignment{
		class AlignmentOperation : public kome::operation::Operation{
		public:
			/**
			 * @fn AlignmentOperation()
			 * @brief constructor
			 */
			AlignmentOperation();

			/**
			 * @fn virtual ~AlignmentOperation()
			 * @brief destructor
			 */
			virtual~AlignmentOperation();
		protected:
			/** old previous data file path */
			std::string m_oldPrevPath;
			
			/** treatment gropu object */
			kome::objects::DataGroupNode* m_trmtGroup;
		public:
			/**
			 * @fn void setGroup( kome::objects::DataGroupNode* group )
			 * @brief set group
			 * @pram[in] group group object
			 */
			void setGroup( kome::objects::DataGroupNode* trmtGroup );

			/**
			 * @fn kome::objects::DataGroupNode* getGroup()
			 * @brief gets group
			 * @return gropu object
			 */
			kome::objects::DataGroupNode* getGroup();
		protected:
			/**
			 * @fn virtual bool onExecute()
			 * @brief execute operation
			 * @return If true, it succeeded to perform search.
			 */
			virtual bool onExecute();

			/**
			 * @fn virtual void onLoadCondition( boost::function< int ( void*, int ) > readFun )
			 * @brief load the saved state with respect to to search engine operations
			 * @param[in] readFun function to read saved state
			 */
			virtual void onLoadCondition( boost::function< int ( void*, int ) > readFun );

			/**
			 * @fn virtual void onSaveCondition( boost::function< int ( void*, int ) > writeFun )
			 * @brief save the current state with respect to search engine operations
			 * @param[in] writeFun function to save state
			 */
			virtual void onSaveCondition( boost::function< int ( void*, int ) > writeFun  );

			/**
			 * @fn virtual std::string onGetDescription()
			 * @brief get discription (override method)
			 * @return discription
			 */
			virtual std::string onGetDescription();
			
			/**
			 * @fn virtual std::string onGetParametersString()
			 * @brief get parameters string (override method)
			 * @return parameters string 
			 */
			virtual std::string onGetParametersString();
			
			/**
			 * @fn virtual void onSetParametersString( const char* strParam )
			 * @brief set parameters string  (override method)
			 * @param[in] strParam parameters string
			 */
			virtual void onSetParametersString( const char* strParam );
			
				
		};
	}
}

#endif // __KOME_ALIGNMENT_OPERATION_H__

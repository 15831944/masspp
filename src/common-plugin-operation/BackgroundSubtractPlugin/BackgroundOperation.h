/**
 * @file BackgroundOperation.h
 * @brief Background Subtraction Operation class
 *
 * @author M.Izumi
 * @date 2012.01.26
 * 
 * Copyright (C) 2014 Shimadzu Corporation All rights reserved.
 */


#ifndef __KOME_BACKGROUND_OPERATION_H__
#define __KOME_BACKGROUND_OPERATION_H__

namespace kome {
	namespace background {

		class BackgroundSubtraction;

		/**
		 * @class BackgroundOperation
		 * @brief Background operation class
		 */
		class BackgroundOperation : public kome::operation::Operation {

		public:
			/**
			 * @fn BackgroundOperation()
			 * @brief constructor
			 */
			BackgroundOperation();

			/**
			 * @fn virtual ~BackgroundOperation()
			 * @brief destructor
			 */
			virtual ~BackgroundOperation();
		
		protected:
			/** spectrum opject */ 
			kome::objects::Spectrum* m_spec;

			/** current background subtraction */
			BackgroundSubtraction* m_bs;

			/** back ground spectrum */
			kome::objects::Spectrum* m_bgSpec;
		public:
			/**
			 * @fn void setSpec( kome::objects::Spectrum* spec )
			 * @brief set spectrum
			 * @param[in] spec spectrum object
			 */
			void setSpec( kome::objects::Spectrum* spec );

			/**
			 * @fn kome::objects::Spectrum* getSpec()
			 * @brief gets spectrum
			 * @return spectrum object
			 */
			kome::objects::Spectrum* getSpec();

			/**
			 * @fn void setBackgroundSubtraction( BackgroundSubtraction* bs )
			 * @brief set current background subtraction
			 * @param[in] bs current background subtraction
			 */
			void setBackgroundSubtraction( BackgroundSubtraction* bs );

			/**
			 * @fn BackgroundSubtraction* getBackgroundSubtraction()
			 * @brief get current background subtraction
			 * @return bs current background subtraction
			 */
			BackgroundSubtraction* getBackgroundSubtraction();

			/**
			 * @fn void setBackgroundSpec( kome::objects::Spectrum* spec )
			 * @brief set background spectrum
			 *@param[in] spec background spectrum object
			 */
			void setBackgroundSpec( kome::objects::Spectrum* spec );

			/**
			 * @fn kome::objects::Spectrum* getBackgroundSpec()
			 * @brief get background spectrum
			 * @return spectrum object
			 */
			kome::objects::Spectrum* getBackgroundSpec();
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

#endif // __KOME_BACKGROUND_OPERATION_H__

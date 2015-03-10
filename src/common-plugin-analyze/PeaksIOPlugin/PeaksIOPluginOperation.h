/**
 * @file PeaksIOPluginOperation.h
 * @brief Peaks IO Plugin Operation class
 *
 * @author M.Izumi
 * @date 2012.03.02
 * 
 * Copyright (C) 2014 Shimadzu Corporation All rights reserved.
 */

#ifndef __KOME_IO_PEAKS_PEAKS_IO_OPERATION_H__
#define __KOME_IO_PEAKS_PEAKS_IO_OPERATION_H__

namespace kome{
	namespace io {
		namespace peaks{
			/**
			 * @class AverageSampleOperation
			 * @brief average sample operation class
			 */
			class PeaksIoOperation : public kome::operation::Operation {
		
			public:
				/**
				 * @fn PeaksIoOperation()
				 * @brief constructor
				 */
				PeaksIoOperation();

				/**
				 * @fn virtual ~PeaksIoOperation()
				 * @brief destructor
				 */
				virtual ~PeaksIoOperation();

			protected:
			
		
			public:
			
			
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
}

#endif // __KOME_IO_PEAKS_PEAKS_IO_OPERATION_H__

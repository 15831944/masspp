/**
 * @file AverageSampleOperation.h
 * @brief Average Sample Operation class
 *
 * @author M.Izumi
 * @date 2012.01.27
 * 
 * Copyright (C) 2014 Shimadzu Corporation All rights reserved.
 */

#ifndef __KOME_AVERAGE_SAMPLE_OPERATION_H__
#define __KOME_AVERAGE_SAMPLE_OPERATION_H__

namespace kome{
	namespace merged {
		/**
		 * @class AverageSampleOperation
		 * @brief average sample operation class
		 */
		class AverageSampleOperation : public kome::operation::Operation {
		
		public:
			/**
			 * @fn AverageSampleOperation()
			 * @brief constructor
			 */
			AverageSampleOperation();

			/**
			 * @fn virtual ~AverageSampleOperation()
			 * @brief destructor
			 */
			virtual ~AverageSampleOperation();

		protected:
			/**  RT toleerance */
			double m_rtTol;

			/** m/z tolerance */
			double m_mzTol;
			
			/** sample object */
			std::vector< kome::objects::Sample* > m_samples;

		public:
			/**
			 * @fn void setRtTol( double rtTol )
			 * @brief set RT toleerance
			 * @param[in] rtTol RT toleerance
			 */
			void setRtTol( double rtTol );

			/**
			 * @fn double getRtTol()
			 * @brief get RT toleerance
			 * @return rtTol RT toleerance
			 */
			double getRtTol();

			/**
			 * @fn void setMzTol( double mzTol )
			 * @brief sets m/z tolerance
			 * @param[in] mzTol m/z tolerance
			 */
			void setMzTol( double mzTol );

			/**
			 * @fn double getMzTol();
			 * @brief gets m/z tolerance
			 * @return m/z tolerance
			 */
			double getMzTol();

			/**
			 * @fn void setSamples( std::vector< kome::objects::Sample* > samples )
			 * @brief set sample objects
			 * @param[in] sample objects
			 */
			void setSamples( std::vector< kome::objects::Sample* > samples );

			/**
			 * @fn std::vector< kome::objects::Sample* > getSamples()
			 * @brief get sample objects
			 * @return sample objects
			 */
			std::vector< kome::objects::Sample* > getSamples();
			
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

#endif // __KOME_AVERAGE_SAMPLE_OPERATION_H__

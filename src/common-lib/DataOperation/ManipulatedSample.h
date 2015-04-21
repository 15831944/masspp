/**
 * @file ManipulatedSample.h
 * @brief Manipulated Sample class
 *
 * @author M.Izumi
 * @date 2012.11.12
 * 
 * Copyright(C) 2012 Shimadzu Corporation. All rights reserved.
 */

#ifndef __KOME_OPERATION_MANIPULATED_SAMPLE_H__
#define __KOME_OPERATION_MANIPULATED_SAMPLE_H__

namespace kome{
	namespace operation{

		class DataGroupNode;
		class ManipulatedSpectrum;
		class Normalization;
	
		/**
		* @class ManipulatedSample
		* @brief ManipulatedSample class
		*/
		class DATA_OPERATION_CLASS ManipulatedSample : public kome::objects::Sample {
		public:
			/**
			 * @fn ManipulatedSample( kome::objects::Sample& org )
			 * @brief constructor
			 */
			ManipulatedSample( kome::objects::Sample& org );

			/**
			 * @fn ~ManipulatedSample( kome::objects::Sample& org )
			 * @brief destructor
			 */
			virtual ~ManipulatedSample();

		protected:
			/** orignal sample */
			kome::objects::Sample& m_org;
			
			/** operations */
			std::vector<kome::objects::XYDataOperation*> m_opts;

		public:		
			/**
			 * @fn void addOperation( kome::objects::XYDataOperation* opt )
			 * @brief add operation
			 * @param opt xyData Operaiton object
			 */
			void addOperation( kome::objects::XYDataOperation* opt );
			
			/**
			 * @fn void removeOperation( kome::objects::XYDataOperation* opt )
			 * @brief remove operation
			 * @param opt xyData Operation object
			 */
			void removeOperation( kome::objects::XYDataOperation* opt );

			/**
			 * @fn void clearOperation()
			 * @brief clear operation
			 */
			void clearOperation();

			/**
			 * @fn kome::objects::Sample* getOrgSample()
			 * @brief gets original sample
			 * @return org sample object
			 */
			kome::objects::Sample* getOrgSample();

			/**
			 * @fn void setOperationInfo( kome::plugin::PluginCall* call, kome::objects::SettingParameterValues* settings, kome::objects::DataGroupNode* stdGroup )
			 * @brief set operation info
			 * @param call plugin call
			 * @param settings setting parameter values
			 * @param stdGroup standard group
			 */
			void setOperationInfo( kome::plugin::PluginCall* call, kome::objects::SettingParameterValues* settings, kome::objects::DataGroupNode* stdGroup );
			
			/**
			 * @fn unsigned int getNumberOfOperation()
			 * @brief get number of operation
			 * @return operation size
			 */
			unsigned int getNumberOfOperation();

			/**
			 * @fn kome::objects::XYDataOperation* getOperation( int index )
			 * @brief get operation
			 * @return xyData Operation
			 */
			kome::objects::XYDataOperation* getOperation( int index );
		protected:
			/**
			 * @fn void addSpectrum( kome::objects::DataGroupNode* orgGroup, kome::objects::DataGroupNode* dstGroup )
			 * @brief add spectrum
			 * @param orgGroup group of original sample data
			 * @param dstGroup group of manipulated sample data
			 */
			void addSpectrum( kome::objects::DataGroupNode* orgGroup, kome::objects::DataGroupNode* dstGroup );

		protected:
			/**
			 * @fn virtual bool onOpenSample( DataGroupNode* rootGroup, kome::core::Progress* progress = NULL)
			 * @brief This method is called by openSample method. (abstract method)
			 * @param[out] rootGroup root spectrum group
			 * @param[out] progress progress bar instance
			 * @return If true, it succeeded to open this sample.
			 */
			virtual bool onOpenSample( kome::objects::DataGroupNode* rootGroup, kome::core::Progress* progress = NULL );

			/**
			 * @fn virtual bool onCloseSample()
			 * @brief This method is called by closeSample method. (abstract method)
			 * @return If true, it succeeded to close this sample.
			 */
			virtual bool onCloseSample();
		};
	}
}

#endif // __KOME_OPERATION_MANIPULATED_SAMPLE_H__

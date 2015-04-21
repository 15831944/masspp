/**
 * @file XYDataOperationWrapper.h
 * @brief interfaces of XYDataOperationWrapper class
 *
 * @author S.Tanaka
 * @date 2007.03.08
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_CLR_XY_DATA_OPERATION_WRAPPER_H__
#define __KOME_CLR_XY_DATA_OPERATION_WRAPPER_H__


namespace kome {
	namespace clr {

		ref class XYDataWrapper;
		ref class SpectrumWrapper;
		ref class ChromatogramWrapper;
		ref class SampleWrapper;

		/**
		 * @class XYDataOperationWrapper
		 * @brief xy data operation wrapper class to use on CLR environment
		 */
		public ref class XYDataOperationWrapper {
		public:
			/**
			 * @fn XYDataOperationWrapper( kome::objects::XYDataOperation& operation )
			 * @brief constructor
			 * @param[in] operation xy data operation object to be wrapped
			 */
			XYDataOperationWrapper( kome::objects::XYDataOperation& operation );

			/**
			 * @fn virtual ~XYDataOperationWrapper()
			 * @brief destructor
			 */
			virtual ~XYDataOperationWrapper();

		protected:
			/** xy data operation object to be wrapped */
			kome::objects::XYDataOperation& m_operation;

		public:
			/**
			 * @fn kome::objects::XYDataOperation& getXYDataOperation()
			 * @brief gets xy data operation object
			 * @return xy data operation object
			 */
			kome::objects::XYDataOperation& getXYDataOperation();

		public:
			/**
			 * @fn void setName( System::String^ name )
			 * @brief sets name
			 * @param[in] name name 
			 */
			void setName( System::String^ name );

			/**
			 * @fn System::String^ getName()
			 * @brief gets name
			 * @return name
			 */
			System::String^ getName();

		public:
			/**
			 * @fn void update( XYDataWrapper^ src, XYDataWrapper^ dst, ChromatogramWrapper^ chrom )
			 * @brief updates xy data
			 * @param[in] src source xy data
			 * @param[out] dst the object to store updated xy data
			 * @param[in] chrom chromatogram
			 */
			void update( XYDataWrapper^ src, XYDataWrapper^ dst, ChromatogramWrapper^ chrom );

			/**
			 * @fn void update( XYDataWrapper^ src, XYDataWrapper^ dst, SpectrumWrapper^ spec )
			 * @brief updates xy data
			 * @param[in] src source xy data
			 * @param[out] dst the object to store updated xy data
			 * @param[in] spec spectrum
			 */
			void update( XYDataWrapper^ src, XYDataWrapper^ dst, SpectrumWrapper^ spec );
			
			/**
			 * @fn void update( XYDataWrapper^ src, XYDataWrapper^ dst, SampleWrapper^ sample )
			 * @brief updates xy data
			 * @param[in] src source xy data
			 * @param[out] dst the object to store updated xy data
			 * @param[in] sample sample
			 */
			void update( XYDataWrapper^ src, XYDataWrapper^ dst, SampleWrapper^ sample );
		};
	}
}

#endif		// __KOME_CLR_XY_DATA_OPERATION_WRAPPER_H__

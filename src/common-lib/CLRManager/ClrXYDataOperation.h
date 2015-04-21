/**
 * @file ClrXYDataOperation.h
 * @brief interfaces of ClrXYDataOperation class
 *
 * @author S.Tanaka
 * @date 2007.03.08
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#ifndef __KOME_CLR_XY_DATA_OPERATION_H__
#define __KOME_CLR_XY_DATA_OPERATION_H__


namespace kome {
	namespace clr {

		ref class ClrXYDataOperationBase;

		/**
		 * @class ClrXYDataOperation
		 * @brief xy data operation management class to use on CLR environment
		 */
		class ClrXYDataOperation : public kome::objects::XYDataOperation {
		public:
			/**
			 * @fn ClrXYDataOperation()
			 * @brief constructor
			 */
			ClrXYDataOperation();

			/**
			 * @fn virtual ~ClrXYDataOperation()
			 * @brief destructor
			 */
			virtual ~ClrXYDataOperation();

		protected:
			/** opertion base */
			gcroot< ClrXYDataOperationBase^ > m_baseOperation;

		public:
			/**
			 * @fn void setBaseOperation( ClrXYDataOperationBase^ base )
			 * @brief sets base operation object
			 * @param[in] base base operation object
			 */
			void setBaseOperation( ClrXYDataOperationBase^ base );

			/**
			 * @fn ClrXYDataOperationBase^ getBaseOperation()
			 * @brief gets baseoperation object
			 * @return baseoperation object
			 */
			ClrXYDataOperationBase^ getBaseOperation();

		protected:
			/**
			 * @fn virtual void onUpdate(
					kome::core::XYData& src,
					kome::core::XYData& dst,
					kome::objects::Chromatogram& chrom
				)
			 * @brief This method is called by update method. (override method)
			 * @param[in] src source xy data
			 * @param[out] dst the object to store updated xy data
			 * @param[in] chrom chromatogram
			 */
			virtual void onUpdate(
				kome::core::XYData& src,
				kome::core::XYData& dst,
				kome::objects::Chromatogram& chrom
			);

			/**
			 * @fn virtual void onUpdate(
					kome::core::XYData& src,
					kome::core::XYData& dst,
					kome::objects::Spectrum& spec
				)
			 * @brief This method is called by update method. (override method)
			 * @param[in] src source xy data
			 * @param[out] dst the object to store updated xy data
			 * @param[in] spec spectrum
			 */
			virtual void onUpdate(
				kome::core::XYData& src,
				kome::core::XYData& dst,
				kome::objects::Spectrum& spec
			);

			/**
			 * @fn virtual void onUpdate(
					kome::core::XYData& src,
					kome::core::XYData& dst,
					kome::objects::Sample& sample
				)
			 * @brief This method is called by update method. (override method)
			 * @param[in] src source xy data
			 * @param[out] dst the object to store updated xy data
			 * @param[in] sample sample
			 */
			virtual void onUpdate(
				kome::core::XYData& src,
				kome::core::XYData& dst,
				kome::objects::Sample& sample
			);
		};
	}
}

#endif		// __KOME_CLR_XY_DATA_OPERATION_H__

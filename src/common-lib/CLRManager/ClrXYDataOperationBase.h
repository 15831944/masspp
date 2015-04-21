/**
 * @file ClrXYDataOperationBase.h
 * @brief interfaces of ClrXYDataOperationBase class
 *
 * @author S.Tanaka
 * @date 2007.03.08
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#ifndef __KOME_CLR_XY_DATA_OPERATION_BASE_H__
#define __KOME_CLR_XY_DATA_OPERATION_BASE_H__


#include "XYDataOperationWrapper.h"


namespace kome {
	namespace clr {

		/**
		 * @class ClrXYDataOperationBase
		 * @brief abstract class of xy data operation on CLR environment
		 */
#ifndef _MANAGED	// (for doxygen)
		public ref class ClrXYDataOperationBase : public XYDataOpertionWrapper {
#else
		public ref class ClrXYDataOperationBase abstract : public XYDataOperationWrapper {
#endif		// _MAANGED

		public:
			/**
			 * @fn ClrXYDataOperationBase()
			 * @brief constructor
			 */
			ClrXYDataOperationBase();

			/**
			 * @fn virtual ~ClrXYDataOperationBase()
			 * @brief destructor
			 */
			virtual ~ClrXYDataOperationBase();

		public:
			/**
			 * @fn virtual void onUpdate(
					XYDataWrapper^ src,
					XYDataWrapper^ dst,
					ChromatogramWrapper^ chrom
				) = 0
			 * @brief This method is called by updateOnClr method. (abstract method)
			 * @param[in] src source xy data
			 * @param[out] dst the object to store updated xy data
			 * @param[in] chrom chromatogram
			 */
			virtual void onUpdate(
				XYDataWrapper^ src,
				XYDataWrapper^ dst,
				ChromatogramWrapper^ chrom
			) = 0;

			/**
			 * @fn virtual void onUpdate(
					XYDataWrapper^ src,
					XYDataWrapper^ dst,
					SpectrumWrapper^ spec
				) = 0
			 * @brief This method is called by updateOnClr method. (abstract method)
			 * @param[in] src source xy data
			 * @param[out] dst the object to store updated xy data
			 * @param[in] spec spectrum
			 */
			virtual void onUpdate(
				XYDataWrapper^ src,
				XYDataWrapper^ dst,
				SpectrumWrapper^ spec
			) = 0;

			/**
			 * @fn virtual void onUpdate(
					XYDataWrapper^ src,
					XYDataWrapper^ dst,
					SampleWrapper^ sample
				) = 0
			 * @brief This method is called by updateOnClr method. (abstract method)
			 * @param[in] src source xy data
			 * @param[out] dst the object to store updated xy data
			 * @param[in] sample sample
			 */
			virtual void onUpdate(
				XYDataWrapper^ src,
				XYDataWrapper^ dst,
				SampleWrapper^ sample
			) = 0;
		};
	}
}

#endif		// __KOME_CLR_XY_DATA_OPERATION_BASE_H__

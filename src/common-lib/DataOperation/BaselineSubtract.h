/**
 * @file BaselineSubtract.h
 * @brief interfaces of BaselineSubtract class
 *
 * @author S.Tanaka
 * @date 2007.08.23
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_OPERATION_BASELINE_SUBTRACT_H__
#define __KOME_OPERATION_BASELINE_SUBTRACT_H__


#include <vector>
#include <utility>


namespace kome {

	namespace plugin {
		class PluginCall;
	}

	namespace operation {

		/**
		 * @class BaselineSubtract
		 * @brief baseline subtract class
		 */
		class DATA_OPERATION_CLASS BaselineSubtract : public kome::objects::XYDataOperation {
		public:
			/**
			 * @fn BaselineSubtract()
			 * @brief constructor
			 */
			BaselineSubtract();

			/**
			 * @fn virtual ~BaselineSubtract()
			 * @brief destructor
			 */
			virtual ~BaselineSubtract();

		protected:
			/** baseline detect function */
			kome::plugin::PluginCall* m_fun;

			/** parameter values */
			kome::objects::SettingParameterValues m_settings;

		protected:

			/**
			 * @fn void init()
			 * @brief initializes
			 */
			void init();

		public:
			/**
			 * @fn void setBaselineInfo( kome::plugin::PluginCall* func, kome::objects::SettingParameterValues* settings )
			 * @brief sets baseline information
			 * @param[in] func baseline function
			 * @param[in] settings baseline setting parameter values
			 */
			void setBaselineInfo( kome::plugin::PluginCall* func, kome::objects::SettingParameterValues* settings );

		protected:
			/**
			 * @fn void subtractBaseline(
					kome::core::XYData& src,
					kome::core::XYData& dst,
					kome::core::XYData& baseline
				)
			 * @brief subtracts baseline
			 * @param[in] src source xy data
			 * @param[out] dst the object to store subtracted xy data
			 * @param[in] baseline baseline data
			 */
			void subtractBaseline(
				kome::core::XYData& src,
				kome::core::XYData& dst,
				kome::core::XYData& baseline
			);

		protected:
			/**
			 * @fn virtual void getUpdatedData( kome::core::XYData& src, kome::core::XYData& dst )
			 * @brief gets updated data points
			 * @param[in] src source xy data
			 * @param[out] dst the object to store updated xy data
			 */
			virtual void getUpdatedData( kome::core::XYData& src, kome::core::XYData& dst );

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

#endif		// __KOME_OPERATION_BASELINE_SUBTRACT_H__

/**
 * @file Normalization.h
 * @brief Normalization class
 *
 * @author M.Izumi
 * @date 2012.07.20
 * 
 * Copyright (C) 2014 Shimadzu Corporation All rights reserved.
 */

#ifndef __KOME_OPERATION_NORMALIZATION_H__
#define __KOME_OPERATION_NORMALIZATION_H__

#include <vector>
#include <utility>

namespace kome{
	
	namespace plugin {
		class PluginCall;
	}
	
	namespace operation {
		/**
		 * @class Normalization
		 * @brief normalization class
		 */
		class DATA_OPERATION_CLASS Normalization : public kome::objects::XYDataOperation {
		public:
			/**
			 * @fn Normalization()
			 * @brief constructor
			 */
			Normalization();

			/**
			 * @fn virtual ~Normalization()
			 * @brief destructor
			 */
			virtual ~Normalization();

		protected:
			/** filter function */
			kome::plugin::PluginCall* m_fun;

			/** parameters */
			kome::objects::SettingParameterValues m_settings;

			/** standard group */
			kome::objects::DataGroupNode*  m_stdGroup;

			/** treatment group */
			kome::objects::DataGroupNode*  m_trmtGroup;
			
		protected:

			/**
			 * @fn void init()
			 * @brief initializes
			 */
			void init();

		public:
			/**
			* @fn void setNormalizInfo( kome::plugin::PluginCall* func, kome::objects::SettingParameterValues* settings, kome::objects::DataGroupNode* stdGroup )
			 * @brief sets filter information
			 * @param[in] func filter function
			 * @param[in] settings setting parameter values
			 * @param[in] stdGroup standard group
			 */
			void setNormalizInfo( kome::plugin::PluginCall* func, 
				kome::objects::SettingParameterValues* settings,
				kome::objects::DataGroupNode* stdGroup
			);
	
		protected:
			/**
			 * @fn virtual void getUpdatedData( kome::core::XYData& src, kome::core::XYData& dst, kome::objects::Spectrum* spec  )
			 * @brief gets updated data points
			 * @param[in] src source xy data
			 * @param[out] dst the object to store updated xy data
			 * @param[in] spec spectrum 
			 */
			virtual void getUpdatedData( kome::core::XYData& src, kome::core::XYData& dst, kome::objects::Spectrum* spec );

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
			 * @param[in] sample sample object
			 */
			virtual void onUpdate(
				kome::core::XYData& src,
				kome::core::XYData& dst,
				kome::objects::Sample& sample
			);
		};
		
	}

}

#endif // __KOME_OPERATION_NORMALIZATION_H__

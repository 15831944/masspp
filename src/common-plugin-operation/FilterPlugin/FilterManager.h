/**
 * @file FilterManager.h
 * @brief interfaces of FilterManager class
 *
 * @author S.Tanaka
 * @date 2007.08.16
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#ifndef __KOME_FILTER_MANAGER_H__
#define __KOME_FILTER_MANAGER_H__


namespace kome {
	namespace filter {
		class Filter;
		/**
		 * @class FilterManager
		 * @brief Internal Standard Correction management class
		 */
		class FilterManager {
		protected:
			/**
			 * @fn FilterManager()
			 * @brief constructor
			 */
			FilterManager();

			/**
			 * @fn virtual ~FilterManager()
			 * @brief destructor
			 */
			virtual ~FilterManager();

		protected:
			/**
			 * @fn void setValMatchOperationSpec( kome::operation::Filter* filter, kome::plugin::PluginFunctionItem* item )
			 * @brief Set the value of the matching operation
			 * @param filter filter operation
			 * @param item plugin function item
			 */
			void setValMatchOperationSpec( kome::operation::Filter* filter, kome::plugin::PluginFunctionItem* item );

			/**
			 * @fn void setValMatchOperationChrom( kome::operation::Filter* filter, kome::plugin::PluginFunctionItem* item )
			 * @brief Set the value of the matching operation
			 * @param filter filter operation
			 * @param item plugin function item
			 */
			void setValMatchOperationChrom( kome::operation::Filter* filter, kome::plugin::PluginFunctionItem* item );
		public:
			/**
			 * @fn void executeFilter(
					kome::objects::Chromatogram* chrom,
					kome::plugin::PluginFunctionItem* item,
					kome::objects::SettingParameterValues* settings
				)
			 * @brief executes chromatogram filter
			 * @param chrom chromatogram
			 * @param item filter function item
			 * @param settings setting parameter values
			 * @parma index XYDataOperation index
			 */
			void executeFilter(
				kome::objects::Chromatogram* chrom,
				kome::plugin::PluginFunctionItem* item,
				kome::objects::SettingParameterValues* settings,
				int index = -1
			);

			/**
			 * @fn void executeFilter(
					kome::objects::Spectrum* spec,
					kome::plugin::PluginFunctionItem* item,
					kome::objects::SettingParameterValues* settings
				)
			 * @brief executes spectrum filter
			 * @param spec spectrum
			 * @param item filter function item
			 * @param settings setting parmaeter values
			 * @parma index XYDataOperation index
			 */
			void executeFilter(
				kome::objects::Spectrum* spec,
				kome::plugin::PluginFunctionItem* item,
				kome::objects::SettingParameterValues* settings,
				int index = -1 
			);
				
		public:
			/**
			 * @fn static FilterManager& getInstance()
			 * @brief gets Internal Standard Correction manager
			 * @return Internal Standard Correction manager class (This is the only object.)
			 */
			static FilterManager& getInstance();
		};
	}
}

#endif		// __KOME_FILTER_MANAGER_H__

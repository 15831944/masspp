/**
 * @file Filter.h
 * @brief interfaces of Filter class
 *
 * @author S.Tanaka
 * @date 2007.08.23
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_OPERATION_FILTER_H__
#define __KOME_OPERATION_FILTER_H__


#include <vector>
#include <utility>


namespace kome {

	namespace plugin {
		class PluginCall;
	}

	namespace operation {

		/**
		 * @class Filter
		 * @brief filter class
		 */
		class DATA_OPERATION_CLASS Filter : public kome::objects::XYDataOperation {
		public:
			/**
			 * @fn Filter()
			 * @brief constructor
			 */
			Filter();

			/**
			 * @fn virtual ~Filter()
			 * @brief destructor
			 */
			virtual ~Filter();

		protected:
			/** filter function */
			kome::plugin::PluginCall* m_fun;

			/** parameters */
			kome::objects::SettingParameterValues m_settings;

		protected:

			/**
			 * @fn void init()
			 * @brief initializes
			 */
			void init();

		public:
			/**
			 * @fn void setFilterInfo( kome::plugin::PluginCall* func, kome::objects::SettingParameterValues* settings )
			 * @brief sets filter information
			 * @param[in] func filter function
			 * @param[in] settings setting parameter values
			 */
			void setFilterInfo( kome::plugin::PluginCall* func, kome::objects::SettingParameterValues* settings );

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

		/**
		 * @class MzRangeFilter
		 * @brief m/z range filter class
		 */
		class DATA_OPERATION_CLASS MzRangeFilter : public kome::objects::XYDataOperation {

		public:
			/**
			 * @fn MzRangeFilter( double startMz, double endMz )
			 * @brief constructor
			 * @param[in] startMz start m/z
			 * @param[in] endMz end m/z			 
			 */
			MzRangeFilter( double startMz, double endMz );

			/**
			 * @fn virtual ~MzRangeFilter()
			 * @brief destructor
			 */
			virtual ~MzRangeFilter();
			
		protected:
			/** start m/z */
			double m_startMz;
			
			/** end m/z */
			double m_endMz;
	
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

#endif		// __KOME_OPERATION_FILTER_H__


/**
 * @file BaselineSubtractManager.h
 * @brief interfaces of BaselineSubtractManager class
 *
 * @author S.Tanaka
 * @date 2007.03.12
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#ifndef __KOME_BASELINE_SUBTRACT_MANAGER_H__
#define __KOME_BASELINE_SUBTRACT_MANAGER_H__


namespace kome {
	namespace baseline {
		class BaselineSubtract;
		class BaselineChromatogram;
		class BaselineSpectrum;
		class BaselineOperation;

		/**
		 * @class BaselineSubtractManager
		 * @brief Internal Standard Correction management class
		 */
		class BaselineSubtractManager {
		protected:
			/**
			 * @fn BaselineSubtractManager()
			 * @brief constructor
			 */
			BaselineSubtractManager();

			/**
			 * @fn virtual ~BaselineSubtractManager()
			 * @brief destructor
			 */
			virtual ~BaselineSubtractManager();

		protected:
			/** baseline chromatogram map */
			std::map< kome::objects::Chromatogram*, BaselineChromatogram* > m_chromBaselineMap;

			/** baseline spectrum map */
			std::map< kome::objects::Spectrum*, BaselineSpectrum* > m_specBaselineMap;

		protected:
			/**
			 * @fn void setValMatchOperationSpec( kome::operation::BaselineSubtract* baseline, kome::plugin::PluginFunctionItem* item )
			 * @brief Set the value of the matching operation
			 * @param baseline baseline operation
			 * @param item plugin function item
			 */
			void setValMatchOperationSpec( kome::operation::BaselineSubtract* baseline, kome::plugin::PluginFunctionItem* item );

			/**
			 * @fn void setValMatchOperationChrom( kome::operation::BaselineSubtract* baseline, kome::plugin::PluginFunctionItem* item )
			 * @brief Set the value of the matching operation
			 * @param baseline baseline operation
			 */
			void setValMatchOperationChrom( kome::operation::BaselineSubtract* baseline, kome::plugin::PluginFunctionItem* item );
		
		public:
			/**
			 * @fn void setDrawBaseline(
					kome::objects::Chromatogram* chrom,
					kome::plugin::PluginFunctionItem* item,
					kome::objects::SettingParameterValues* settings
				)
			 * @brief sets draw chromatogram baseline
			 * @param chrom chromatogram
			 * @param item baseline function item
			 * @param settings setting parameter values
			 */
			void setDrawBaseline(
				kome::objects::Chromatogram* chrom,
				kome::plugin::PluginFunctionItem* item,
				kome::objects::SettingParameterValues* settings
			);

			/**
			 * @fn void setDrawBaseline(
					kome::objects::Spectrum* spec,
					kome::plugin::PluginFunctionItem* item,
					kome::objects::SettingParameterValues* settings
				)
			 * @brief sets draw spectrum baseline
			 * @param spec spectrum
			 * @param item baseline function item
			 * @param settings setting parameter values
			 */
			void setDrawBaseline(
				kome::objects::Spectrum* spec,
				kome::plugin::PluginFunctionItem* item,
				kome::objects::SettingParameterValues* settings
			);

			/**
			 * @fn void eraseBaseline( kome::objects::Chromatogram* chrom )
			 * @brief erases chromatogram baseline
			 */
			void eraseBaseline( kome::objects::Chromatogram* chrom );

			/**
			 * @fn void eraseBaseline( kome::objects::Spectrum* spec )
			 * @brief erases spectrum baseline
			 */
			void eraseBaseline( kome::objects::Spectrum* spec );

			/**
			 * @fn void subtractBaseline(
					kome::objects::Chromatogram* chrom,
					kome::plugin::PluginFunctionItem* item,
					kome::objects::SettingParameterValues* settings
				)
			 * @brief subtracts chromatogram baseline
			 * @param chrom chromatogram
			 * @param item baseline function item
			 * @param settings setting parameter values
			 * @parma index XYDataOperation index
			 */
			void subtractBaseline(
				kome::objects::Chromatogram* chrom,
				kome::plugin::PluginFunctionItem* item,
				kome::objects::SettingParameterValues* settings,
				int index = -1
			);

			/**
			 * @fn void subtractBaseline(
					kome::objects::Spectrum* spec,
					kome::plugin::PluginFunctionItem* item,
					kome::objects::SettingParameterValues* settings
				)
			 * @brief subtracts spectrum baseline
			 * @param spec spectrum
			 * @param item baseline function item
			 * @param settings setting parameter values
			 * @parma index XYDataOperation index
			 */
			void subtractBaseline(
				kome::objects::Spectrum* spec,
				kome::plugin::PluginFunctionItem* item,
				kome::objects::SettingParameterValues* settings,
				int index = -1
			);

			/**
			 * @fn void drawBaseline(
					kome::objects::Chromatogram* chrom,
					kome::img::Graphics* g
				)
			 * @brief draws chromatogram baseline
			 * @param chrom chromatogram
			 * @param g graphics
			 * @param canvas canvas
			 */
			void drawBaseline(
					kome::objects::Chromatogram* chrom,
					kome::img::Graphics* g,
					wxWindow* canvas
			);

			/**
			 * @fn void drawBaseline(
					kome::objects::Spectrum* spec,
					kome::img::Graphics* g
				)
			 * @brief draws spectrum baseline
			 * @param spec spectrum
			 * @param g graphics
			 * @param canvas canvas
			 */
			void drawBaseline(
				kome::objects::Spectrum* spec,
				kome::img::Graphics* g,
				wxWindow* canvas
			);
						
		public:
			/**
			 * @fn static BaselineSubtractManager& getInstance()
			 * @brief gets Internal Standard Correction manager
			 * @return Internal Standard Correction manager class (This is the only object.)
			 */
			static BaselineSubtractManager& getInstance();
		};
	}
}

#endif		// __KOME_BASELINE_SUBTRACT_MANAGER_H__

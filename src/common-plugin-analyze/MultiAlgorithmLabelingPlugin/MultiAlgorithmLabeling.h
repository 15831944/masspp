/**
 * @file MultiAlgorithmLabeling.h
 * @brief interfaces of MultiAlgorithmLabeling class
 *
 * @author S.Tanaka
 * @date 2007.07.20
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_LABELING_MULTI_ALGORITHM_H__
#define __KOME_LABELING_MULTI_ALGORITHM_H__


namespace kome {
	namespace labeling {
		namespace multi {

			/**
			 * @class MultiAlgorithmLabeling
			 * @brief weighted average labeling class
			 */
			class MultiAlgorithmLabeling {
			public:
				/**
				 * @fn MultiAlgorithmLabeling()
				 * @brief constructor
				 */
				MultiAlgorithmLabeling();

				/**
				 * @fn virtual ~MultiAlgorithmLabeling()
				 * @brief destructor
				 */
				virtual ~MultiAlgorithmLabeling();

			public:
				/**
				 * @fn static void executeLabeling(
						kome::plugin::PluginFunctionItem& item,
						kome::objects::Parameters& params,
						kome::objects::SettingParameterValues* settings
					)
				 * @brief executes labeling
				 * @param item peak detector
				 * @param params parameters
				 * @param settings setting parameter values
				 */
				static void executeLabeling(
					kome::plugin::PluginFunctionItem& item,
					kome::objects::Parameters& params,
					kome::objects::SettingParameterValues* settings
				);
			};
		}
	}
}

#endif	// __KOME_LABELING_MULTI_ALGORITHM_H__

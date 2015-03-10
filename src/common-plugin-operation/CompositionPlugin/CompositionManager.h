/**
 * @file CompositionManager.h
 * @brief interfaces of CompositionManager class
 *
 * @author S.Tanaka
 * @date 2009.02.18
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_COMPOSITION_MANAGER_H__
#define __KOME_COMPOSITION_MANAGER_H__


#include <utility>
#include <map>


namespace kome {
	namespace composition {
		class SampleFusion;
		class SampleFusionOperation;
		/**
		 * @class CompositionManager
		 * @brief composition management class
		 */
		class CompositionManager : public kome::objects::DefaultDataManager {
		protected:
			/**
			 * @fn CompositionManager()
			 * @brief constructor
			 */
			CompositionManager();

			/**
			 * @fn virtual ~CompositionManager()
			 * @brief destructor
			 */
			virtual ~CompositionManager();

		protected:
			/** composition data map */
			std::map< kome::objects::Sample*, std::pair< kome::objects::Sample*, kome::objects::Sample* > > m_compositionMap;

		public:
			/**
			 * @fn void addData(
						kome::objects::Sample* data,
						kome::objects::DataGroupNode* ctrlGroup,
						kome::objects::DataGroupNode* trmtGroup
					)
			 * @brief adds composition data
			 * @param data composition data
			 * @param ctrlGroup control group
			 * @param trmtGroup treatment group
			 */
			void addData(
				kome::objects::Sample* data,
				kome::objects::DataGroupNode* ctrlGroup,
				kome::objects::DataGroupNode* trmtGroup
			);

			
		protected:
			/**
			 * @fn virtual void onCloseSample( kome::objects::Sample* sample, const bool deleting )
			 * @brief This method is called when a sample is closed. (override method)
			 * @param[in] sample sample object to be closed
			 * @param[in] deleting If true, the specified object is being deleted now.
			 */
			virtual void onCloseSample( kome::objects::Sample* sample, const bool deleting );

		public:
			/**
			 * @fn static CompositionManager& getInstance()
			 * @brief gets composition manager
			 * @return linear alignment manager class (This is the only object.)
			 */
			static CompositionManager& getInstance();
		};
	}
}

#endif		// __KOME_COMPOSITION_MANAGER_H__

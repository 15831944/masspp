/**
 * @file BasicScalingPluginManager.h
 * @brief implements of BasicScaling class.
 *
 * @author M.Izumi
 * @date 2012.11.05
 * 
 * Copyright(C) 2006-2014 Shimadzu Corporation All rights reserved.
 */

#ifndef __KOME_BASE_SCALING_MANAGER_H__
#define __KOME_BASE_SCALING_MANAGER_H__

namespace kome {
	namespace scaling {

		/**
		 * @class BasicScalingManager
		 * @brief Internal basic scaling class
		 */
		class BasicScalingManager {
		protected:
			/**
			 * @fn BaseScalingManager()
			 * @brief constructor
			 */
			BasicScalingManager();

			/**
			 * @fn virtual ~BaseScalingManager()
			 * @brief destructor
			 */
			virtual ~BasicScalingManager();

		public:
			/** 
			 * @fn double UV( double p, double v)
			 * @brief UV scaling
			 * @param[in] p : sigma of targeted m/z peaks
			 * @param[in] v : peak-value of scaling-targeted peak  
			 * @return UV scaled value
			 */
			double UV( double p, double v );

			/** 
			 * @fn double pareto( double p, double v  )
			 * @brief pareto scaling
			 * @param[in] p : sigma of targeted m/z peaks
			 * @param[in] v : peak-value of scaling-targeted peak  
			 * @return pareto scaled value
			 */
			double pareto( double p, double v );    
			 
			/**
			 * @fn double log( double p )
			 * @brief log
			 * @param p
			 * @return log value
			 */
			double log( double p );

			/**
			 * @fn double ln( double p )
			 * @brief ln
			 * @param p
			 * @return ln
			 */
			double ln( double p );
		public:
			/**
			 * @fn static CommonsMathManager& getInstance()
			 * @brief gets the ComonsMathManager object. (This is the only object.)
			 * @return CommonsMathManager object
			 */
			static BasicScalingManager& getInstance();

		};

	}
}

#endif	// __KOME_BASE_SCALING_MANAGER_H__

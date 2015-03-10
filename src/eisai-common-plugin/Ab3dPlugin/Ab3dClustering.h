/**
 * @file Ab3dClustering.h
 * @brief interfaces of Ab3dClustering class
 *
 * @author S.Tanaka
 * @date 2007.10.30
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_AB3D_CLUSTERING_H__
#define __KOME_AB3D_CLUSTERING_H__


namespace kome {
	namespace ab3d {

		/**
		 * @class Ab3dClustering
		 * @brief AB3D labeling class
		 */
		class Ab3dClustering {
		public:
			/**
			 * @fn Ab3dClustering( kome::objects::SettingParameterValues* settings );
			 * @brief constructor
			 * @param settings setting parameter values
			 */
			Ab3dClustering( kome::objects::SettingParameterValues* settings );

			/** 
			 * @fn virtual ~Ab3dClustering()
			 * @brief destructor
			 */
			virtual ~Ab3dClustering();

		protected:
			/** setting parameters */
			kome::objects::SettingParameterValues* m_settings;

			/** charge flags */
			bool m_chargeFlags[ 4 ];

			/** RT tolerance */
			double m_rtTol;

			/** m/z tolerance */
			double m_mzTol;


		public:
			/**
			 * @fn void detectClusters(
					kome::objects::Peaks2D& peaks,
					kome::core::Progress& progress
				)
			 * @brief detect clusters
			 * @param peaks peak object to store cluster information
			 * @param progress progress object to show progress
			 */
			void detectClusters(
				kome::objects::Peaks2D& peaks,
				kome::core::Progress& progress
			);
		};
	}
}


#endif		// __KOME_AB3D_CLUSTERING_H__

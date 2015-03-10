/**
 * @file Ab3dCompositionCluster.h
 * @brief interfaces of Ab3dCompositionCluster class
 *
 * @author S.Tanaka
 * @date 2007.10.16
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_AB3D_COMPOSITION_CLUSTER_H__
#define __KOME_AB3D_COMPOSITION_CLUSTER_H__


namespace kome {
	namespace ab3d {

		/**
		 * @class Ab3dCompositionCluster
		 * @brief AB3D composition class
		 */
		class Ab3dCompositionCluster {
		public:
			/**
			 * @fn Ab3dCompositionCluster()
			 * @brief constructor
			 */
			Ab3dCompositionCluster();

			/**
			 * @fn virtual ~Ab3dCompositionCluster()
			 * @brief destructor
			 */
			virtual ~Ab3dCompositionCluster();

		protected:
			/** cluster */
			kome::objects::PeaksCluster2D* m_cluster;

			/** intensity */
			double m_intensity;

			/** ctrl intensity */
			double m_trmtInt;

			/** treatment intensity */
			double m_ctrlInt;

			/** control clusters */
			std::vector< kome::objects::PeaksCluster2D* > m_ctrlClusters;

			/** treatment clusters */
			std::vector< kome::objects::PeaksCluster2D* > m_trmtClusters;

		public:
			/**
			 * @fn void setCluster( kome::objects::PeaksCluster2D* cluster );
			 * @brief sets cluster
			 * @param cluster cluster
			 */
			void setCluster( kome::objects::PeaksCluster2D* cluster );

			/**
			 * @fn kome::objects::PeaksCluster2D* getCluster()
			 * @brief gets cluster
			 * @return cluster
			 */
			kome::objects::PeaksCluster2D* getCluster();

			/**
			 * @fn void setIntensity( const double intensity )
			 * @brief sets intensity
			 * @param intensity intensity
			 */
			void setIntensity( const double intensity );

			/**
			 * @fn double getIntensity()
			 * @brief gets intensity
			 * @return intensity
			 */
			double getIntensity();

			/**
			 * @fn void setCtrlIntensity( const double intensity )
			 * @brief sets control intensity
			 * @param intensity control intensity
			 */
			void setCtrlIntensity( const double intensity );

			/**
			 * @fn double getCtrlIntensity()
			 * @brief gets control intensity
			 * @return control intensity
			 */
			double getCtrlIntensity();

			/** 
			 * @fn void setSmplIntensity( const double intensity )
			 * @brief sets treatment intensity
			 * @param intensity treatment intensity
			 */
			void setSmplIntensity( const double intensity );

			/**
			 * @fn double getSmplIntensity()
			 * @brief gets treatment intensity
			 * @return treatment intensity
			 */
			double getSmplIntensity();

			/**
			 * @fn void addCtrlCluster( kome::objects::PeaksCluster2D* cluster )
			 * @brief adds control cluster
			 * @param cluster control cluster
			 */
			void addCtrlCluster( kome::objects::PeaksCluster2D* cluster );

			/**
			 * @fn unsigned int getNumberOfCtrlClusters()
			 * @brief gets the number of control clusters
			 * @return the number of control clusters
			 */
			unsigned int getNumberOfCtrlClusters();

			/**
			 * @fn kome::objects::PeaksCluster2D* getCtrlCluster( const unsigned int idx )
			 * @brief gets control cluster
			 * @param idx control cluster index
			 * @return control cluster (If NULL, the index is illegal.)
			 */
			kome::objects::PeaksCluster2D* getCtrlCluster( const unsigned int idx );

			/**
			 * @fn void addSmplCluster( kome::objects::PeaksCluster2D* cluster )
			 * @brief adds treatment cluster
			 * @param cluster treatment cluster
			 */
			void addSmplCluster( kome::objects::PeaksCluster2D* cluster );

			/**
			 * @fn unsigned int getNumberOfSmplClusters()
			 * @brief gets the number of treatment clusters
			 * @return the number of treatment clusters
			 */
			unsigned int getNumberOfSmplClusters();

			/**
			 * @fn kome::objects::PeaksCluster2D* getSmplCluster( const unsigned int idx )
			 * @brief gets treatment cluster
			 * @param idx treatment cluster
			 * @return treatment cluster (If NULL, the index is illegal.)
			 */
			kome::objects::PeaksCluster2D* getSmplCluster( const unsigned int idx );

		};
	}
}

#endif		// __KOME_AB3D_COMPOSITION_CLUSTER_H__

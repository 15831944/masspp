/**
 * @file Ab3dManager.h
 * @brief interfaces of Ab3dManager class
 *
 * @author S.Tanaka
 * @date 2007.09.12
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_AB3D_MANAGER_H__
#define __KOME_AB3D_MANAGER_H__


namespace kome {
	namespace ab3d {

		class Ab3dSampleSet;
		class Ab3dSample;
		class Ab3dCompositionCluster;
		class PeaksDialog;

		/**
		 * @class Ab3dManager
		 * @brief ab3d management class
		 */
		class Ab3dManager : public kome::objects::DefaultDataManager {
		protected:
			/**
			 * @fn Ab3dManager()
			 * @brief constructor
			 */
			Ab3dManager();

			/**
			 * @fn virtual ~Ab3dManager()
			 * @brief destructor
			 */
			virtual ~Ab3dManager();

		protected:
			/** data array */
			std::vector< Ab3dSampleSet* > m_sampleSets;

			/** peaks dialog */
			PeaksDialog* m_peaksDlg;

			/** selected clusters array */
			std::vector< std::pair< Ab3dSample*, Ab3dCompositionCluster* > > m_selectedClusters;

		public:
			/**
			 * @fn void addSampleSet( Ab3dSampleSet* sampleSet )
			 * @brief adds ab3d composition sample set
			 * @param sampleSet ab3d composition sample set
			 */
			void addSampleSet( Ab3dSampleSet* sampleSet );

			/**
			 * @fn void deleteSampleSet( Ab3dSampleSet* sampleSet )
			 * @brief deletes ab3d composition sample set
			 * @param sampleSet ab3d composition sample set
			 */
			void deleteSampleSet( Ab3dSampleSet* sampleSet );

			/**
			 * @fn unsigned int getCount()
			 * @brief gets data count
			 * @return data count
			 */
			unsigned int getCount();

			/**
			 * @fn Ab3dSample* getSample( const unsigned int idx )
			 * @brief gets composition sample
			 * @param idx data index
			 * @return composition sample (If NULL, specified index is illegal.)
			 */
			Ab3dSample* getSample( const unsigned int idx );

		protected:
			/**
			 * @fn virtual void onCloseSample( kome::objects::Sample* sample, const bool deleting )
			 * @brief This method is called when a sample is closed. (override method)
			 * @param sample sample object to be closed
			 * @param[in] deleting If true, the specified object is being deleted now.
			 */
			virtual void onCloseSample( kome::objects::Sample* sample, const bool deleting );

		protected:
			/**
			 * @fn static bool lessCluster(
					std::pair< kome::objects::DataGroupNode*, kome::objects::PeaksCluster2D* >& c0,
					std::pair< kome::objects::DataGroupNode*, kome::objects::PeaksCluster2D* >& c1
				)
			 * @brief compares clusters to sort
			 * @param c0 cluster information to be compared
			 * @param c1 cluster information to compare
			 * @return If true, c0 sorts before c1.
			 */
			static bool lessCluster(
				std::pair< kome::objects::DataGroupNode*, kome::objects::PeaksCluster2D* >& c0,
				std::pair< kome::objects::DataGroupNode*, kome::objects::PeaksCluster2D* >& c1
			);

		public:
			/**
			 * @fn static Ab3dManager& getInstance()
			 * @brief gets ab3d manager object
			 * @return ab3d manager object (This is the only object.)
			 */
			static Ab3dManager& getInstance();
		};
	}
}

#endif		// __KOME_AB3D_MANAGER_H__

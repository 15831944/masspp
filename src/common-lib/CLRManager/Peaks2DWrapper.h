/**
 * @file Peaks2DWrapper.h
 * @brief interfaces of Peaks2DWrapper class
 *
 * @author S.Tanaka
 * @date 2007.10.10
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_CLR_PEAKS_2D_WRAPPER_H__
#define __KOME_CLR_PEAKS_2D_WRAPPER_H__


#include "Peaks2DArrayWrapper.h"


namespace kome {
	namespace clr {

		ref class Peak2DElementWrapper;
		ref class PeaksCluster2DWrapper;


		/**
		 * @class Peaks2DWrapper
		 * @brief Peaks2D wrapper object to use on CLR environment
		 */
		public ref class Peaks2DWrapper : public Peaks2DArrayWrapper {
		public:
			/**
			 * @fn Peaks2DWrapper( kome::objects::Peaks2D& peaks )
			 * @brief constructor
			 * @param[in] peaks 2D peaks object to be wrapped
			 */
			Peaks2DWrapper( kome::objects::Peaks2D& peaks );

			/**
			 * @fn virtual ~Peaks2DWrapper()
			 * @brief destructor
			 */
			virtual ~Peaks2DWrapper();

		protected:
			/** peaks */
			kome::objects::Peaks2D& m_peaks;

		public:
			/**
			 * @fn kome::objects::Peaks2D& getPeaks()
			 * @brief gets peaks object to be wrapped
			 * @return peaks object to be wrapped
			 */
			kome::objects::Peaks2D& getPeaks();

		public:
			/**
			 * @fn Peak2DElementWrapper^ createPeak(
					double rt,
					double startRt,
					double endRt,
					double mz,
					double startMz,
					double endMz,
					double intensity
				)
			 * @brief creates new peak element
			 * @param[in] rt RT
			 * @param[in] startRt start RT
			 * @param[in] endRt end RT
			 * @param[in] mz m/z
			 * @param[in] startMz start m/z
			 * @param[in] endMz end m/z
			 * @param[in] intensity intensity
			 * @return new peak element object
			 */
			Peak2DElementWrapper^ createPeak(
				double rt,
				double startRt,
				double endRt,
				double mz,
				double startMz,
				double endMz,
				double intensity
			);

			/**
			 * @fn Peak2DElementWrapper^ createPeak(
					double rt,
					double mz,
					double intensity
				)
			 * @brief creates new peak element
			 * @param[in] rt RT
			 * @param[in] mz m/z
			 * @param[in] intensity intensity
			 * @return new peak element object
			 */
			Peak2DElementWrapper^ createPeak(
				double rt,
				double mz,
				double intensity
			);

			/**
			 * @fn void clearPeaks()
			 * @brief clears peaks
			 */
			void clearPeaks();

			/**
			 * @fn void removePeak( Peak2DElementWrapper^ peak )
			 * @brief removes peak
			 * @param[in] peak peak to be removed
			 */
			void removePeak( Peak2DElementWrapper^ peak );

			/**
			 * @fn void removePeak( unsigned int idx )
			 * @brief removes peak
			 * @param[in] idx peak index
			 */
			void removePeak( unsigned int idx );

		public:
			/**
			 * @fn PeaksCluster2DWrapper^ createCluster()
			 * @brief creates cluster
			 * @return new cluster object
			 */
			PeaksCluster2DWrapper^ createCluster();

			/**
			 * @fn unsigned int getNumberOfClusters()
			 * @brief gets the number of clusters
			 * @return the number of clusters
			 */
			unsigned int getNumberOfClusters();

			/**
			 * @fn PeaksCluster2DWrapper^ getCluster( unsigned int idx )
			 * @brief gets cluster
			 * @param[in] idx cluster index
			 * @return cluster (If NULL, the index is illegal.)
			 */
			PeaksCluster2DWrapper^ getCluster( unsigned int idx );

			/**
			 * @fn void clearClusters()
			 * @brief clears clusters
			 */
			void clearClusters();

			/**
			 * @fn void removeCluster( PeaksCluster2DWrapper^ cluster )
			 * @brief removes cluster
			 * @param[in] cluster cluster to be removed
			 */
			void removeCluster( PeaksCluster2DWrapper^ cluster );

			/**
			 * @fn void removeCluster( unsigned int idx )
			 * @brief removes cluster
			 * @param[in] idx cluster index
			 */
			void removeCluster( unsigned int idx );

		public:
			/**
			 * @fn void setDataId( int id )
			 * @brief sets the data ID
			 * @param[in] id data ID
			 */
			void setDataId( int id );

			/**
			 * @fn int getDataId()
			 * @brief gets the data ID
			 * @return data ID
			 */
			int getDataId();
		};
	}
}

#endif		// __KOME_CLR_PEAKS_2D_WRAPPER_H__

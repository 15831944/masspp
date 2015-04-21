/**
 * @file Peaks2D.h
 * @brief interfaces of Peaks2D class
 *
 * @author S.Tanaka
 * @date 2007.10.09
 * @date 2011.03.04 M.Aihara
 *       add createPeak TBB parallel code
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_OBJECTS_PEAKS_2D_H__
#define __KOME_OBJECTS_PEAKS_2D_H__


#include "Peaks2DArray.h"

#include <vector>


namespace kome {
	namespace objects {

		class PeaksCluster2D;

		/**
		 * @class Peaks2D
		 * @brief 2-dimentional peaks
		 */
		class DATA_OBJECTS_CLASS Peaks2D : public Peaks2DArray {
		public:
			/**
			 * @fn Peaks2D()
			 * @brief constructor
			 */
			Peaks2D();

			/**
			 * @fn Peaks2D( const Peaks2D& peaks )
			 * @brief copy constructor
			 * @param[in] peaks copy source
			 */
			Peaks2D( const Peaks2D& peaks );

			/**
			 * @fn virtual ~Peaks2D()
			 * @brief destructor
			 */
			virtual ~Peaks2D();

		public:
			/**
			 * @fn Peaks2D& operator=( const Peaks2D& other )
			 * @brief substitution operator
			 * @param[in] other source peaks
			 * @return copied peaks object
			 */
			Peaks2D& operator=( const Peaks2D& other );

		protected:
			/** clusters */
			std::vector< PeaksCluster2D* > m_clusters;

		protected:
			/** data ID */
			int m_dataId;

		public:
			/**
			 * @fn Peak2DElement* createPeak(
					const double rt,
					const double startRt,
					const double endRt,
					const double mz,
					const double startMz,
					const double endMz,
					const double intensity
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
			Peak2DElement* createPeak(
				const double rt,
				const double startRt,
				const double endRt,
				const double mz,
				const double startMz,
				const double endMz,
				const double intensity
			);

// @date 2011.03.04 <Add> M.Aihara ->
// add createPeak TBB parallel code
#ifdef  ENABLE_MASSPP_PARALLEL_TBB_MODE
			/**
			 * @fn Peak2DElement* createPeak_TBB(
					const double rt,
					const double startRt,
					const double endRt,
					const double mz,
					const double startMz,
					const double endMz,
					const double intensity
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
			Peak2DElement* createPeak_TBB(
				const double rt,
				const double startRt,
				const double endRt,
				const double mz,
				const double startMz,
				const double endMz,
				const double intensity
			);
			/**
			 * @fn Peak2DElement* push_back_TBB(
				std::vector<kome::objects::Peak2DElement*>& peak
				)
			 * @brief push_back new peak element
			 * @param[in] peak element object
			 */
			void push_back_TBB(
				std::vector<kome::objects::Peak2DElement*>& peak
			);
#endif //  ENABLE_MASSPP_PARALLEL_TBB_MODE
// <Add> M.Aihara <-

			/**
			 * @fn Peak2DElement* createPeak(
					const double rt,
					const double mz,
					const double intensity
				)
			 * @brief creates new peak element
			 * @param[in] rt RT
			 * @param[in] mz m/z
			 * @param[in] intensity intensity
			 * @return new peak element object
			 */
			Peak2DElement* createPeak(
				const double rt,
				const double mz,
				const double intensity
			);

			/**
			 * @fn void clearPeaks()
			 * @brief clears peaks
			 */
			void clearPeaks();

			/**
			 * @fn void removePeak( Peak2DElement* peak )
			 * @brief removes peak
			 * @param[in] peak peak to be removed
			 */
			void removePeak( Peak2DElement* peak );

			/**
			 * @fn void removePeak( const unsigned int idx )
			 * @brief removes peak
			 * @param[in] idx peak index
			 */
			void removePeak( const unsigned int idx );

		public:
			/**
			 * @fn PeaksCluster2D* createCluster()
			 * @brief creates cluster
			 * @return new cluster object
			 */
			PeaksCluster2D* createCluster();

			/**
			 * @fn unsigned int getNumberOfClusters()
			 * @brief gets the number of clusters
			 * @return the number of clusters
			 */
			unsigned int getNumberOfClusters();

			/**
			 * @fn PeaksCluster2D* getCluster( const unsigned int idx )
			 * @brief gets cluster
			 * @param[in] idx cluster index
			 * @return cluster (If NULL, the index is illegal.)
			 */
			PeaksCluster2D* getCluster( const unsigned int idx );

			/**
			 * @fn void clearClusters()
			 * @brief clears clusters
			 */
			void clearClusters();

			/**
			 * @fn void removeCluster( PeaksCluster2D* cluster )
			 * @brief removes cluster
			 * @param[in] cluster cluster to be removed
			 */
			void removeCluster( PeaksCluster2D* cluster );

			/**
			 * @fn void removeCluster( const unsigned int idx )
			 * @brief removes cluster
			 * @param[in] idx cluster index
			 */
			void removeCluster( const unsigned int idx );

		public:
			/**
			 * @fn void setDataId( const int id )
			 * @brief sets the data ID
			 * @param[in] id data ID
			 */
			void setDataId( const int id );

			/**
			 * @fn int getDataId()
			 * @brief gets the data ID
			 * @return data ID
			 */
			int getDataId();

		};
	}
}

#endif		// __KOME_OBJECTS_PEAKS_2D_H__

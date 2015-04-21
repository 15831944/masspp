/**
 * @file PeaksCluster2DWrapper.h
 * @brief interfaces of PeaksCluster2DWrapper class
 *
 * @author S.Tanaka
 * @date 2007.10.10
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_CLR_PEAKS_2D_CLUSTER_WRAPPER_H__
#define __KOME_CLR_PEAKS_2D_CLUSTER_WRAPPER_H__


#include "Peaks2DArrayWrapper.h"


namespace kome {
	namespace clr {

		ref class Peak2DElementWrapper;

		/**
		 * @class PeaksCluster2DWrapper
		 * @brief PeaksCluster2D wrapper class to use on CLR environment
		 */
		public ref class PeaksCluster2DWrapper : public Peaks2DArrayWrapper {
		public:
			/**
			 * @fn PeaksCluster2DWrapper( kome::objects::PeaksCluster2D& cluster )
			 * @brief constructor
			 * @param[in] cluster cluster object to be wrapped
			 */
			PeaksCluster2DWrapper( kome::objects::PeaksCluster2D& cluster );

			/**
			 * @fn virtual ~PeaksCluster2DWrapper()
			 * @brief destructor
			 */
			virtual ~PeaksCluster2DWrapper();

		protected:
			/** cluster */
			kome::objects::PeaksCluster2D& m_cluster;

		public:
			/**
			 * @fn kome::objects::PeaksCluster2D& getCluster()
			 * @brief gets cluster object to be wrapped
			 * @return cluster object to be wrapped
			 */
			kome::objects::PeaksCluster2D& getCluster();

		public:
			/**
			 * @fn void setRt( double rt )
			 * @brief sets RT
			 * @param[in] rt RT
			 */
			void setRt( double rt );

			/**
			 * @fn double getRt()
			 * @brief gets RT
			 * @return RT
			 */
			double getRt();

			/**
			 * @fn void setRtRange( double startRt, double endRt )
			 * @brief sets RT range
			 * @param[in] startRt start RT
			 * @param[in] endRt end RT
			 */
			void setRtRange( double startRt, double endRt );

			/**
			 * @fn double getStartRt()
			 * @brief gets start RT
			 * @return start RT
			 */
			double getStartRt();

			/**
			 * @fn double getEndRt()
			 * @brief gets end RT
			 * @return end RT
			 */
			double getEndRt();

			/**
			 * @fn void setMz( double mz )
			 * @brief sets m/z
			 * @param[in] mz m/z
			 */
			void setMz( double mz );

			/**
			 * @fn double getMz()
			 * @brief gets m/z
			 * @return m/z
			 */
			double getMz();

			/**
			 * @fn void setMzRange( double startMz, double endMz )
			 * @brief sets m/z range
			 * @param[in] startMz start m/z
			 * @param[in] endMz endMz
			 */
			void setMzRange( double startMz, double endMz );

			/**
			 * @fn double getStartMz()
			 * @brief gets start m/z
			 * @return start m/z
			 */
			double getStartMz();

			/**
			 * @fn double getEndMz()
			 * @brief gets end m/z
			 * @return end m/z
			 */
			double getEndMz();

			/**
			 * @fn void setIntensity( double intensity )
			 * @brief sets intensity
			 * @param[in] intensity intensity
			 */
			void setIntensity( double intensity );

			/**
			 * @fn double getIntensity()
			 * @brief gets intensity 
			 * @return intensity
			 */
			double getIntensity();

		public:
			/**
			 * @fn void setCharge( int charge )
			 * @brief sets charge
			 * @param[in] charge charge
			 */
			void setCharge( int charge );

			/**
			 * @fn int getCharge()
			 * @brief gets charge
			 * @return charge
			 */
			int getCharge();
		
		public:
			/**
			 * @fn void addPeak( Peak2DElementWrapper^ peak )
			 * @brief adds peak
			 * @param[in] peak peak to be added
			 */
			void addPeak( Peak2DElementWrapper^ peak );

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

			/**
			 * @fn void clearPeaks()
			 * @brief clears peaks
			 */
			void clearPeaks();
		};
	}
}

#endif		// __KOME_CLR_PEAKS_2D_CLUSTER_WRAPPER_H__

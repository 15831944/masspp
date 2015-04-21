/**
 * @file PeaksCluster2D.h
 * @brief interfaces of PeaksCluster2D class
 *
 * @author S.Tanaka
 * @date 2007.10.09
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_OBJECTS_PEAKS_2D_CLUSTER_H__
#define __KOME_OBJECTS_PEAKS_2D_CLUSTER_H__


#include "Peaks2DArray.h"


namespace kome {
	namespace objects {

		class Peak2DElement;

		/**
		 * @class PeaksCluster2D
		 * @brief 2-dimentional peaks cluster
		 */
		class DATA_OBJECTS_CLASS PeaksCluster2D : public Peaks2DArray {
		public:
			/**
			 * @fn PeaksCluster2D()
			 * @brief constructor
			 */
			PeaksCluster2D();

			/**
			 * @fn virtual ~PeaksCluster2D()
			 * @brief destructor
			 */
			virtual ~PeaksCluster2D();

		protected:
			/** RT */
			double m_rt;
			/** RT settled flag */
			bool m_rtFlg;

			/** start RT */
			double m_startRt;
			/** end RT */
			double m_endRt;
			/** RT range settled flag */
			bool m_rtRangeFlg;

			/** m/z */
			double m_mz;
			/** m/z settled flag */
			bool m_mzFlg;

			/** start m/z */
			double m_startMz;
			/** end m/z */
			double m_endMz;
			/** m/z range settled flag */
			bool m_mzRangeFlg;

			/** intensity */
			double m_intensity;
			/** intensity settled flag */
			bool m_intFlg;

			/** charge */
			int m_charge;

		public:
			/**
			 * @fn void setRt( const double rt )
			 * @brief sets RT
			 * @param[in] rt RT
			 */
			void setRt( const double rt );

			/**
			 * @fn double getRt()
			 * @brief gets RT
			 * @return RT
			 */
			double getRt();

			/**
			 * @fn void setRtRange( const double startRt, const double endRt )
			 * @brief sets RT range
			 * @param[in] startRt start RT
			 * @param[in] endRt end RT
			 */
			void setRtRange( const double startRt, const double endRt );

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
			 * @fn void setMz( const double mz )
			 * @brief sets m/z
			 * @param[in] mz m/z
			 */
			void setMz( const double mz );

			/**
			 * @fn double getMz()
			 * @brief gets m/z
			 * @return m/z
			 */
			double getMz();

			/**
			 * @fn void setMzRange( const double startMz, const double endMz )
			 * @brief sets m/z range
			 * @param[in] startMz start m/z
			 * @param[in] endMz endMz
			 */
			void setMzRange( const double startMz, const double endMz );

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
			 * @fn void setIntensity( const double intensity )
			 * @brief sets intensity
			 * @param[in] intensity intensity
			 */
			void setIntensity( const double intensity );

			/**
			 * @fn double getIntensity()
			 * @brief gets intensity 
			 * @return intensity
			 */
			double getIntensity();

		public:
			/**
			 * @fn void setCharge( const int charge )
			 * @brief sets charge
			 * @param[in] charge charge
			 */
			void setCharge( const int charge );

			/**
			 * @fn int getCharge()
			 * @brief gets charge
			 * @return charge
			 */
			int getCharge();
		
		public:
			/**
			 * @fn void addPeak( Peak2DElement* peak )
			 * @brief adds peak
			 * @param[in] peak peak to be added
			 */
			void addPeak( Peak2DElement* peak );

			/**
			 * @fn void removePeak( Peak2DElement* peak )
			 * @brief removes peak
			 * @param[in] peak peak to be removed
			 */
			void removePeak( Peak2DElement* peak );

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
		
		protected:
			/**
			 * @fn void update
			 * @brief updates member values
			 */
			void update();
		};
	}
}

#endif		// __KOME_OBJECTS_PEAKS_2D_CLUSTER_H__

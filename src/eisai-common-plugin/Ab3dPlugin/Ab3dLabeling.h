/**
 * @file Ab3dLabeling.h
 * @brief interfaces of Ab3dLabeling class
 *
 * @author S.Tanaka
 * @date 2007.10.11
 * @data 2011.03.02 <Del> M.Aihara
 *       delete static Timer
 * @data 2011.03.02 <Add> M.Aihara
 *       add TBB parallel code
 * @data 2011.03.09 <Add> M.Aihara
 *       add TBB mutex code
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_AB3D_LABELING_H__
#define __KOME_AB3D_LABELING_H__

// @data 2011.03.02 <Add> M.Aihara ->
// add TBB parallel code
#ifdef  ENABLE_MASSPP_PARALLEL_TBB_MODE
#include "tbb/parallel_for.h"
#include "tbb/parallel_reduce.h"
#include "tbb/blocked_range.h"
#include "tbb/spin_mutex.h"
using namespace tbb;
#define PARALLEL_DETECT_TBB
#define PARALLEL_MERGE_TBB
#define PARALLEL_MERGE_REMOVE_TBB
// @data 2011.03.09 <Add> M.Aihara ->
// add TBB mutex code
namespace
{
	typedef spin_mutex AB3DH_ParallelMutexType;
	AB3DH_ParallelMutexType  AB3DH_ParallelMutex;
}
// <Add> M.Aihara <-
#endif // ENABLE_MASSPP_PARALLEL_TBB_MODE
// <Add> M.Aihara <-

namespace kome {
	namespace ab3d {

		/**
		 * @class Ab3dLabeling
		 * @brief AB3D labeling class
		 */
		class Ab3dLabeling {
		public:
			/**
			 * @fn Ab3dLabeling( kome::objects::SettingParameterValues* settings )
			 * @brief constructor
			 * @param settings setting parameter values object
			 */
			Ab3dLabeling( kome::objects::SettingParameterValues* settings );

			/** 
			 * @fn virtual ~Ab3dLabeling()
			 * @brief destructor
			 */
			virtual ~Ab3dLabeling();

			public:
			/**
			 * @typedef ThresholdType
			 * @brief intensity threshold type
			 */
			typedef enum {
				THRESHOLD_ABS,
				THRESHOLD_REL,
				THRESHOLD_AUTO
			} ThresholdType;

		protected:
			/** settings */
			kome::objects::SettingParameterValues* m_settings;

			/** spectrum smoothing */
			kome::plugin::PluginFunctionItem* m_specSmoothing;

			/** spectrum peak detector */
			kome::plugin::PluginFunctionItem* m_specPeakDetector;

			/** mass merge distance */
			double m_mzDistance;

			/** RT merge distance */
			double m_rtDistance;

			/** peak end height percentage */
			double m_endHeight;

			/** intensity threhsold type */
			ThresholdType m_intThresholdType;

			/** intensity threshold */
			double m_intThreshold;

			/** delete shoulder peaks flag */
			bool m_deleteShoulderFlag;

			/** shoulder m/z range */
			double m_shoulderMzRange;

			/** shoulder RT range */
			double m_shoulderRtRange;

			/** spectrum timer */
			// @data 2011.03.02 <Del> M.Aihara
			// kome::core::Timer* m_specTimer;

			/** spectrum peaks timer */
			// @data 2011.03.02 <Del> M.Aihara
			// kome::core::Timer* m_peaksTimer;

			/** merge timer */
			// @data 2011.03.02 <Del> M.Aihara
			// kome::core::Timer* m_mergeTimer;

		protected:
			/** minimum RT */
			double m_minRt;

			/** maximum RT */
			double m_maxRt;

			/** minimum m/z */
			double m_minMz;

			/** maximum m/z */
			double m_maxMz;

		public:
			/**
			 * @fn void setRange(
						const double minRt,
						const double maxRt,
						const double minMz,
						const double maxMz
					)
			 * @brief sets labeling range
			 * @param minRt minimum RT
			 * @param maxRt maximum RT
			 * @param minMz minimum m/z
			 * @param maxMz maximum m/z
			 */
			void setRange(
				const double minRt,
				const double maxRt,
				const double minMz,
				const double maxMz
			);

		public:
			/**
			 * @fn void executeLabeling(
					kome::objects::DataSet& dataSet,
					kome::objects::Peaks2D& peaks,
					kome::core::Progress& progress
				)
			 * @brief executes labeling
			 * @param spectra spectra
			 * @param peaks the object to store peak information
			 * @param progress the object to show progress
			 */
			void executeLabeling(
				kome::objects::DataSet& dataSet,
				kome::objects::Peaks2D& peaks,
				kome::core::Progress& progress
			);

		protected:
			/**
			 * @fn void detectPeaks(
					kome::objects::DataSet& dataSet,
					kome::objects::Peaks2D& peaks,
					kome::core::Progress& progress
				)
			 * @brief detects peaks
			 * @param spectra spectra
			 * @param peaks the object to store peak information
			 * @param progress the object to show progress
			 */
			void detectPeaks(
				kome::objects::DataSet& dataSet,
				kome::objects::Peaks2D& peaks,
				kome::core::Progress& progress
			);

			/**
			 * @fn void detectSpectrumPeaks(
					kome::objects::DataSet& dataSet,
					kome::core::XYData** pointsArray,
					kome::objects::Peaks* peaksArray,
					kome::core::Progress& progress
				)
			 * @brief detects spectrum peaks
			 * @param spectra spectra
			 * @param pointsArray the array to store spectrum data points
			 * @param peaksArray the array to store peaks information
			 * @param progress the object to show progress
			 */
			void detectSpectrumPeaks(
				kome::objects::DataSet& dataSet,
				kome::core::XYData** pointsArray,
				kome::objects::Peaks* peaksArray,
				kome::core::Progress& progress
			);

			/**
			 * @fn void getSpectrumData(
						kome::objects::Spectrum& spec,
						kome::core::XYData& xyData
				   )
			 * @brief gets spectrum data
			 * @param spec spectrum
			 * @param xyData the object to store spectrum data
			 */
			void getSpectrumData(
				kome::objects::Spectrum& spec,
				kome::core::XYData& xyData
			);

			/**
			 * @fn void detectSpectrumPeaks(
					kome::objects::Spectrum& spec,
					kome::core::XYData& xyData,
					kome::objects::Peaks& peaks
					const double minInt
				)
			 * @brief deltects spectrum peaks
			 * @param spec spectrum
			 * @param xyData spectrum data points
			 * @param peaks the object to store spectrum peaks
			 * @param minimum intensity
			 */
			void detectSpectrumPeaks(
				kome::objects::Spectrum& spec,
				kome::core::XYData& xyData,
				kome::objects::Peaks& peaks,
				const double minInt
			);

			/**
			 * @fn void mergePeaks(
					kome::objects::DataSet& dataSet,
					kome::objects::Peaks2D& peaks,
					kome::core::XYData** pointsArray,
					kome::objects::Peaks* peaksArray,
					kome::core::Progress& progress
				)
			 * @brief merges spectrum peaks
			 * @param spectra spectra
			 * @param peaks the object to store merged peaks
			 * @param pointsArray spectrum data points array
			 * @param peaksArray the array of spectrum peaks
			 * @param progress the object to show progress
			 */
			void mergePeaks(
				kome::objects::DataSet& dataSet,
				kome::objects::Peaks2D& peaks,
				kome::core::XYData** pointsArray,
				kome::objects::Peaks* peaksArray,
				kome::core::Progress& progress
			);

			/**
			 * @fn bool mergeSpectrumPeaks(
					kome::objects::Peaks* peaks,
					kome::objects::Peak2DElement* peak,
					const double specRt,
					const double maxInt
				)
			 * brief merges spectrum peaks
			 * @param peaks spectrum peaks
			 * @param peak 2D peak
			 * @param maxInt max intensity
			 * @param 
			 * @return If false, a peak whose intensity is more than maxInt is found.
			 */
			bool mergeSpectrumPeaks(
				kome::objects::Peaks* peaks,
				kome::objects::Peak2DElement* peak,
				const double specRt,
				const double maxInt
			);

			/**
			 * @fn double getMinIntensity( kome::objects::DataSet& dataSet )
			 * @brief gets minimum intensity
			 * @param spectra spectra
			 * @return minimum intensity
			 */
			double getMinIntensity( kome::objects::DataSet& dataSet );

		protected:
			/**
			 * @fn static bool lessRt( kome::objects::Peak2DElement* p0, kome::objects::Peak2DElement* p1 )
			 * @brief compares peaks to sort them by RT order
			 * @param p0 peak object to be compared
			 * @param p1 peak object to compare
			 * @return If true, p0 sorts before p1.
			 */
			static bool lessRt( kome::objects::Peak2DElement* p0, kome::objects::Peak2DElement* p1 );

			/**
			 * @fn static bool lessMz( kome::objects::Peak2DElement* p0, kome::objects::Peak2DElement* p1 )
			 * @brief compares peaks to sort them by m/z order
			 * @param p0 peak object to be compared
			 * @param p1 peak object to compare
			 * @return If true, p0 sorts before p1.
			 */
			static bool lessMz( kome::objects::Peak2DElement* p0, kome::objects::Peak2DElement* p1 );

// @data 2011.03.02 <Add> M.Aihara ->
// add TBB parallel code
#ifdef  ENABLE_MASSPP_PARALLEL_TBB_MODE
#ifdef  PARALLEL_DETECT_TBB
		public:
			/**
			 * @fn void parallel_detectSpectrumPeaks(
					Ab3dLabeling* ab3ptr,
					kome::objects::DataSet& dataSet,
					kome::core::XYData** pointsArray,
					kome::objects::Peaks* peaksArray,
					kome::core::Progress& progress
					double minInt,
					int i,
					int &pos
				)
			 * @brief parallel detects spectrum peaks
			 * @param Ab3dLabeling instance points
			 * @param spectra spectra
			 * @param pointsArray the array to store spectrum data points
			 * @param peaksArray the array to store peaks information
			 * @param progress the object to show progress
			 * @param minInt  minimum intensity
			 * @param i loop current number
			 * @param pos set position number
			 */
			void parallel_detectSpectrumPeaks(
				Ab3dLabeling* ab3ptr,
				kome::objects::DataSet& dataSet,
				kome::core::XYData** pointsArray,
				kome::objects::Peaks* peaksArray,
				kome::core::Progress& progress,
				double minInt,
				int i,
				int &pos
			);
#endif //  PARALLEL_DETECT_TBB
#ifdef  PARALLEL_MERGE_TBB
		public:
			/**
			 * @fn void parallel_mergePeaks(
					Ab3dLabeling* ab3ptr,
					kome::objects::DataSet& dataSet,
					kome::objects::Peaks2D& peaks,
					kome::core::XYData** pointsArray,
					kome::objects::Peaks* peaksArray,
					kome::core::Progress& progress
					int i,
					int &pos
				)
			 * @brief parallel detects spectrum peaks
			 * @param Ab3dLabeling instance points
			 * @param peaks the object to store merged peaks
			 * @param pointsArray spectrum data points array
			 * @param peaksArray the array of spectrum peaks
			 * @param progress the object to show progress
			 * @param i loop current number
			 * @param pos set position number
			 */
			void parallel_mergePeaks(
				Ab3dLabeling* ab3ptr,
				kome::objects::DataSet& dataSet,
				kome::objects::Peaks2D& peaks,
				kome::core::XYData** pointsArray,
				kome::objects::Peaks* peaksArray,
				kome::core::Progress& progress,
				kome::core::Progress* pgrs0,
				int i,
				int &pos,
				std::vector<kome::objects::Peak2DElement*>& m_savePeaks);
#endif //  PARALLEL_MERGE_TBB
#endif //  ENABLE_MASSPP_PARALLEL_TBB_MODE
// <Add> M.Aihara
		};

// @data 2011.03.02 <Add> M.Aihara ->
// add TBB parallel code
#ifdef  ENABLE_MASSPP_PARALLEL_TBB_MODE
#ifdef PARALLEL_DETECT_TBB
		/**
		 * @class ApplyParallel_detectSpectrumPeaks
		 * @brief Parallel TBB detectSpectrumPeaks class
		 */
		class ApplyParallel_detectSpectrumPeaks {
			Ab3dLabeling*           m_ab3ptr;
			kome::objects::DataSet& m_dataSet;
			kome::core::XYData**    m_pointsArray;
			kome::objects::Peaks*   m_peaksArray;
			kome::core::Progress&   m_progress;
			double                  m_minInt;
			int&                    m_pos;
		public:
			void operator()(const blocked_range<size_t>& range ) const {
				for( int i = (int)range.begin(); i < (int)range.end(); i++ ){
					m_ab3ptr->parallel_detectSpectrumPeaks(
						m_ab3ptr,
						m_dataSet,
						m_pointsArray,
						m_peaksArray,
						m_progress,
						m_minInt,
						i,
						m_pos);
				}
			}
			ApplyParallel_detectSpectrumPeaks(
				Ab3dLabeling* ab3ptr,
				kome::objects::DataSet& dataSet,
				kome::core::XYData** pointsArray,
				kome::objects::Peaks* peaksArray,
				kome::core::Progress& progress,
				double minInt,
				int &pos
				) : 
				m_ab3ptr(ab3ptr),
				m_dataSet(dataSet),
				m_pointsArray(pointsArray),
				m_peaksArray(peaksArray),
				m_progress(progress),
				m_minInt(minInt),
				m_pos(pos)
				{}
		};
#endif //PARALLEL_DETECT_TBB
#ifdef PARALLEL_MERGE_TBB
		/**
		 * @class ApplyParallel_mergePeaks
		 * @brief Parallel TBB mergePeaks class
		 */
		class ApplyParallel_mergePeaks {
			Ab3dLabeling*           m_ab3ptr;
			kome::objects::DataSet& m_dataSet;
			kome::objects::Peaks2D& m_peaks;
			kome::core::XYData**    m_pointsArray;
			kome::objects::Peaks*   m_peaksArray;
			kome::core::Progress&   m_progress;
			kome::core::Progress*   m_pgrs0;
			int&                    m_pos;
		public:
			std::vector<kome::objects::Peak2DElement*> m_savePeak;

			ApplyParallel_mergePeaks(
				Ab3dLabeling* ab3ptr,
				kome::objects::DataSet& dataSet,
				kome::objects::Peaks2D& peaks,
				kome::core::XYData** pointsArray,
				kome::objects::Peaks* peaksArray,
				kome::core::Progress& progress,
				kome::core::Progress* pgrs0,
				int &pos
				) : 
			    m_ab3ptr(ab3ptr),
				m_dataSet(dataSet),
				m_peaks(peaks),
				m_pointsArray(pointsArray),
				m_peaksArray(peaksArray),
				m_progress(progress),
				m_pgrs0(pgrs0),
				m_pos(pos)
				{
					m_savePeak.clear();
				}
			ApplyParallel_mergePeaks(
				ApplyParallel_mergePeaks& A,
				split
				) : 
			    m_ab3ptr(A.m_ab3ptr),
				m_dataSet(A.m_dataSet),
				m_peaks(A.m_peaks),
				m_pointsArray(A.m_pointsArray),
				m_peaksArray(A.m_peaksArray),
				m_progress(A.m_progress),
				m_pgrs0(A.m_pgrs0),
				m_pos(A.m_pos)
				{
					m_savePeak.clear();
				}
			void operator()(const blocked_range<size_t>& range ) {
				for( int i = (int)range.begin(); i < (int)range.end(); i++ ){
					m_ab3ptr->parallel_mergePeaks(
						m_ab3ptr,
						m_dataSet,
						m_peaks,
						m_pointsArray,
						m_peaksArray,
						m_progress,
						m_pgrs0,
						i,
						m_pos,
						m_savePeak);
				}
			}
			void join(ApplyParallel_mergePeaks& A){
				for(std::vector<kome::objects::Peak2DElement*>::iterator it=A.m_savePeak.begin();
					it != A.m_savePeak.end();
					++it){
					m_savePeak.push_back(*it);
				}
			}
		};
#endif //PARALLEL_MERGE_TBB
#ifdef PARALLEL_MERGE_REMOVE_TBB
		/**
		 * @class ApplyParallel_mergePeaksCollectRemoveElems
		 * @brief Parallel TBB mergePeaks remove Elements class
		 */
        class ApplyParallel_mergePeaksCollectRemoveElems {
        private:
            kome::objects::Peaks2D& peaks;
            int                     num;
            double                  m_shoulderMzRange;
            double                  m_shoulderRtRange;
			kome::core::Progress*   m_progress;
			int&                    m_pos;
        public:
            std::vector<int> removeElements;
            
            ApplyParallel_mergePeaksCollectRemoveElems(
                kome::objects::Peaks2D& _peaks,
                int                     _num,
                double                  _m_shoulderMzRange,
                double                  _m_shoulderRtRange,
				kome::core::Progress*   _m_progress,
				int&                    _m_pos
                )
                : peaks(_peaks),
                num(_num),
                m_shoulderMzRange(_m_shoulderMzRange),
                m_shoulderRtRange(_m_shoulderRtRange),
				m_progress(_m_progress),
				m_pos(_m_pos)
                {
                    removeElements.clear();
                }
            ApplyParallel_mergePeaksCollectRemoveElems(
                ApplyParallel_mergePeaksCollectRemoveElems& A,
                split)
                : peaks(A.peaks),
                num(A.num),
                m_shoulderMzRange(A.m_shoulderMzRange),
                m_shoulderRtRange(A.m_shoulderRtRange),
				m_progress(A.m_progress),
				m_pos(A.m_pos)
                {
                    removeElements.clear();
                }
            void operator()(const blocked_range<int>& range)
            {
                for(int i=range.begin(); i!=range.end(); ++i){
                    Kernel(peaks,
                           num,
                           i,
                           m_shoulderMzRange,
                           m_shoulderRtRange,
						   m_progress,
						   m_pos,
                           removeElements);
                }
            }
            void join(ApplyParallel_mergePeaksCollectRemoveElems& A)
            {
                for(std::vector<int>::iterator it=A.removeElements.begin();
                    it != A.removeElements.end();
                    ++it){
                    removeElements.push_back(*it);
                }
            }
        private:
            void Kernel(
                kome::objects::Peaks2D& peaks,
                int                     num,
                int                     i,
                double                  m_shoulderMzRange,
                double                  m_shoulderRtRange,
				kome::core::Progress*   pgrs1,
				int&                    pos,
                std::vector<int>&       removeElements)
            {
                kome::objects::Peak2DElement* peak = peaks.getPeak(i);

                for(int j=i+1; j<num; ++j){
                    kome::objects::Peak2DElement* tmp = peaks.getPeak(j);

                    if(tmp->getRt() >= peak->getStartRt() - m_shoulderRtRange
                       && tmp->getRt() <= peak->getEndRt()   + m_shoulderRtRange
                       && tmp->getMz() >= peak->getStartMz() - m_shoulderMzRange
                       && tmp->getMz() <= peak->getEndMz()   + m_shoulderMzRange){
                        removeElements.push_back(j);
                    }
                }
				{
					// @data 2011.03.09 <Add> M.Aihara ->
					// add TBB mutex code
					AB3DH_ParallelMutexType::scoped_lock lock( AB3DH_ParallelMutex );
					// <Add> M.Aihara
					pos += 1;
					pgrs1->setPosition( pos );
				}
            }
        };
#endif PARALLEL_MERGE_REMOVE_TBB
#endif // ENABLE_MASSPP_PARALLEL_TBB_MODE
// <Add> M.Aihara

	}
}

#endif		// __KOME_AB3D_LABELING_H__

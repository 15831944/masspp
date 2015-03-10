/**
 * @file Ab3dSample.h
 * @brief interfaces of Ab3dSample class
 *
 * @author S.Tanaka
 * @date 2008.01.17
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_AB3D_SAMPLE_H__
#define __KOME_AB3D_SAMPLE_H__


#include "Ab3dComposition.h"
#include "Ab3dCompositionCluster.h"


namespace kome {
	namespace ab3d {

		class Ab3dSampleSet;
		class Ab3dSpectrum;

		/**
		 * @class Ab3dSample
		 * @brief composition data object class
		 */
		class Ab3dSample : public kome::objects::Sample {
		public:
			/**
			 * @fn Ab3dSample(
						Ab3dSampleSet* sampleSet,
						kome::operation::Alignment& alignment,
						kome::core::Progress& progress
					)
			 * @brief constructor
			 * @param sampleSet sample set object
			 * @param alignment alignment
			 * @param progress
			 */
			Ab3dSample(
				Ab3dSampleSet* sampleSet,
				kome::operation::Alignment& alignment,
				kome::core::Progress& progress
			);

			/**
			 * @fn virtual ~Ab3dSample()
			 * @brief destructor
			 */
			virtual ~Ab3dSample();

		protected:
			/** sample set */
			Ab3dSampleSet* m_sampleSet;

			/** alignment */
			kome::operation::Alignment& m_alignment;

			/** progress */
			kome::core::Progress& m_progress;

		protected:
			/** control chromatogram */
			kome::objects::SpectraChromatogram m_ctrlChrom;

			/** treatment chromatogram */
			kome::objects::SpectraChromatogram m_trmtChrom;

			/** treatment scale */
			double m_treatmentScale;

			/** control peak */
			kome::objects::Peaks2D m_ctrlPeaks;

			/** treatment peak */
			kome::objects::Peaks2D m_trmtPeaks;

			/** clusters */
			std::vector< Ab3dCompositionCluster > m_clusters;

		public:
			/**
			 * @fn Ab3dSampleSet* getAb3dSampleSet()
			 * @brief gets AB3D sample set object
			 * @return AB3D sample set object
			 */
			Ab3dSampleSet* getAb3dSampleSet();

		public:
			/**
			 * @fn kome::objects::Chromatogram& getControlChromatogram()
			 * @brief gets control chromatogram
			 * @return control chromatogram
			 */
			kome::objects::Chromatogram& getControlChromatogram();

			/**
			 * @fn kome::objects::Chromatogram& getTreatmentChromatogram()
			 * @brief gets treatment chromatogram
			 * @return treatment chromatogram
			 */
			kome::objects::Chromatogram& getTreatmentChromatogram();

			/**
			 * @fn double getTreatmentScale()
			 * @brief gets treatment scale
			 * @return treatment scale
			 */
			double getTreatmentScale();

		public:
			/**
			 * @fn unsigned int getNumberOfClusters()
			 * @brief gets the number of clusters
			 * @return the number of clusters
			 */
			unsigned int getNumberOfClusters();

			/**
			 * @fn Ab3dCompositionCluster* getCluster( const unsigned int index )
			 * @brief gets cluster information
			 * @param index cluster index
			 * @return cluster information object (If NULL, the index is illegal.)
			 */
			Ab3dCompositionCluster* getCluster( const unsigned int index );

		protected:
			/**
			 * @fn void setProperties()
			 * @brief sets properties
			 */
			void setProperties();

			/**
			 * @fn void pickPeaks(
					kome::objects::DataGroupNode* group,
					kome::objects::Peaks2D* peaks,
					kome::core::Progress* progress
				)
			 * @brief pick peaks
			 * @param group spectrum group
			 * @param peaks peaks object to store peaks
			 * @param progress progress object to show progress
			 */
			void pickPeaks(
				kome::objects::DataGroupNode* group,
				kome::objects::Peaks2D* peaks,
				kome::core::Progress* progress
			);

			/**
			 * @fn void detectClusters(
					kome::objects::DataGroupNode* group,
					kome::objects::Peaks2D* peaks,
					kome::core::Progress* progress
				)
			 * @brief detects clusters
			 * @param group spectrum group
			 * @param peaks peaks information
			 * @param progress progress object to show progress
			 */
			void detectClusters(
				kome::objects::DataGroupNode* group,
				kome::objects::Peaks2D* peaks,
				kome::core::Progress* progress
			);

			/**
			 * @fn void executeSmoothing(
					kome::objects::Peaks2D* ctrlPeaks,
					kome::objects::Peaks2D* trmtPeaks,
					kome::core::Progress* progress
				)
			 * @brief executes smoothing
			 * @param ctrlPeaks control peaks
			 * @param trmtPeaks treatment peaks
			 * @param progress progress object to show progress
			 */
			void executeSmoothing(
				kome::objects::Peaks2D* ctrlPeaks,
				kome::objects::Peaks2D* trmtPeaks,
				kome::core::Progress* progress
			);

			/**
			 * @fn void createSpectra(
					kome::objects::Peaks2D* ctrlPeaks,
					kome::objects::Peaks2D* trmtPeaks,
					kome::core::Progress* progress
				)
			 * @brief creates spectra
			 * @param ctrlPeaks control peaks
			 * @param trmtPeaks treatment peaks
			 * @param progress progress object to show progress
			 */
			void createSpectra(
				kome::objects::Peaks2D* ctrlPeaks,
				kome::objects::Peaks2D* trmtPeaks,
				kome::core::Progress* progress
			);

			/**
			 * @fn Ab3dSpectrum* getSpectrum( kome::objects::Spectrum* ctrlSpec, kome::objects::Spectrum* trmtSpec )
			 * @brief gets AB3D spectrum object
			 * @param ctrlSpec control spectrum
			 * @param trmtSpec treatment spectrum
			 * @return spectrum object
			 */
			Ab3dSpectrum* getSpectrum( kome::objects::Spectrum* ctrlSpec, kome::objects::Spectrum* trmtSpec );

		protected:
			/**
			 * @fn virtual bool onOpenSample( DataGroupNode* rootGroup )
			 * @brief This method is called by openTreatment method. (override method)
			 * @param rootGroup root spectrum group
			 * @return If true, it succeeded to open this treatment.
			 */
			virtual bool onOpenSample( kome::objects::DataGroupNode* rootGroup, kome::core::Progress* progress = NULL );

			/**
			 * @fn virtual bool onCloseSample()
			 * @brief This method is called by closeTreatment method. (override method)
			 * @return If true, it succeeded to close this treatment.
			 */
			virtual bool onCloseSample();
		};
	}
}


#endif		// __KOME_AB3D_SAMPLE_H__

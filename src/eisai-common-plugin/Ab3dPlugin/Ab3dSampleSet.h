/**
 * @file Ab3dSampleSet.h
 * @brief interfaces of Ab3dSampleSet class
 *
 * @author S.Tanaka
 * @date 2008.01.17
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_AB3D_SAMPLE_SET_H__
#define __KOME_AB3D_SAMPLE_SET_H__


#include "Ab3dComposition.h"
#include "Ab3dCompositionCluster.h"


namespace kome {
	namespace ab3d {

		class Ab3dSpectrum;

		/**
		 * @class Ab3dSampleSet
		 * @brief composition data object class
		 */
		class Ab3dSampleSet : public kome::objects::SampleSet {
		public:
			/**
			 * @fn Ab3dSampleSet(
					kome::objects::DataSet& ctrlSpectra,
					kome::objects::DataSet& trmtSpectra,
					kome::objects::SettingParameterValues* settings,
					kome::operation::Alignment* alignment,
					kome::core::Progress& progress
				)
			 * @brief constructor
			 * @param ctrlSpectra control spectra
			 * @param trmtSpectra treatment spectra
			 * @param settings setting parameter values
			 * @param alignment alignment
			 * @param progress progress object to show progress
			 */
			Ab3dSampleSet(
				kome::objects::DataSet& ctrlSpectra,
				kome::objects::DataSet& trmtSpectra,
				kome::objects::SettingParameterValues* settings,
				kome::operation::Alignment* alignment,
				kome::core::Progress& progress
			);

			/**
			 * @fn virtual ~Ab3dSampleSet()
			 * @brief destructor
			 */
			virtual ~Ab3dSampleSet();

		protected:
			/** control spectra */
			kome::objects::DataSet& m_ctrlSpectra;

			/** treatment spectra */
			kome::objects::DataSet& m_trmtSpectra;

			/** setting values */
			kome::objects::SettingParameterValues* m_settings;

			/** peak detector */
			kome::plugin::PluginFunctionItem* m_peakDetector;

			/** peak settings */
			kome::objects::SettingParameterValues m_peakSettings;

			/** cluster detector */
			kome::plugin::PluginFunctionItem* m_clusterDetector;

			/** cluster settings */
			kome::objects::SettingParameterValues m_clusterSettings;

			/** intensity type */
			Ab3dComposition::IntensityType m_intType;

			/** operation */
			Ab3dComposition::Operation m_operation;

			/** m/z tolerance */
			double m_mzTol;

			/** RT tolerance */
			double m_rtTol;

			/** alignment */
			kome::operation::Alignment* m_alignment;

			/** progress */
			kome::core::Progress& m_progress;

		public:
			/**
			 * @fn kome::objects::DataSet& getControlDataSet()
			 * @brief gets control spectra
			 * @return control spectra
			 */
			kome::objects::DataSet& getControlDataSet();

			/**
			 * @fn kome::objects::DataSet& getTreatmentSpectra()
			 * @brief gets treatment spectra
			 * @return treatment spectra
			 */
			kome::objects::DataSet& getTreatmentSpectra();

			/**
			 * @fn kome::plugin::PluginFunctionItem* getPeakDetector()
			 * @brief gets peak detector
			 * @return peak detector
			 */
			kome::plugin::PluginFunctionItem* getPeakDetector();

			/**
			 * @fn kome::objects::SettingParameterValues* getPeakSettings()
			 * @brief gets peak settings
			 * @return peak setting values object
			 */
			kome::objects::SettingParameterValues* getPeakSettings();

			/**
			 * @fn kome::plugin::PluginFunctionItem* getClusterDetector()
			 * @brief gets cluster detector
			 * @return cluster detector
			 */
			kome::plugin::PluginFunctionItem* getClusterDetector();

			/**
			 * @fn kome::objects::SettingParameterValues* getClusterSettings()
			 * @briefg gets cluster setting values
			 * @return cluster setting values object
			 */
			kome::objects::SettingParameterValues* getClusterSettings();

			/**
			 * @fn Ab3dComposition::IntensityType getIntensityType()
			 * @brief gets intensity type
			 * @return intensity type
			 */
			Ab3dComposition::IntensityType getIntensityType();

			/**
			 * @fn Ab3dComposition::Operation getOperation()
			 * @brief gets data operation type
			 * @return operation type
			 */
			Ab3dComposition::Operation getOperation();

			/**
			 * @fn double getMzTolerance()
			 * @brief gets m/z tolerance
			 * @return m/z tolerance
			 */
			double getMzTolerance();

			/**
			 * @fn double getRtTolerance()
			 * @brief gets RT tolerance
			 * @return RT tolerance
			 */
			double getRtTolerance();

		protected:
			/**
			 * @fn void createData()
			 * @brief creates data
			 */
			void createData();

		protected:
			/**
			 * @fn virtual bool onOpenFile( const char* path )
			 * @brief This method is called by openFile method. (override method)
			 * @param path file path
			 * @param progress
			 * @return If true, it succeeded to open the file
			 */
			virtual bool onOpenFile( const char* path, kome::core::Progress* progress = NULL );


			/**
			 * @fn virtual bool onCloseFile()
			 * @brief This method is called by closeFile method. (override method)
			 * @param path file path
			 * @return If true, it succeeded to close the file
			 */
			virtual bool onCloseFile();

		public:
			/**
			 * @fn virtual bool onOpenSample( kome::objects::Sample* sample )
			 * @brief This method is called by openTreatment method. (override method)
			 * @param treatment treatment object to be opened (If NULL, opens all treatments.)
			 * @param progress
			 * @return If true, it succeeded to open the treatment.
			 */
			virtual bool onOpenSample( kome::objects::Sample* sample, kome::core::Progress* progress = NULL ); 

			/**
			 * @fn virtual bool onCloseSample( kome::objects::Sample* sample )
			 * @brief This method is called by closeTreatment method. (override method)
			 * @param treatment treatment object to be closed (If NULL, closes all treatments.)
			 * @return If true, it succeeded to close the treatment.
			 */
			virtual bool onCloseSample( kome::objects::Sample* sample );
		};
	}
}


#endif		// __KOME_AB3D_SAMPLE_SET_H__

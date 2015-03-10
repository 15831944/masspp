/**
 * @file LabelingAndMergePeakPosition.h
 * @brief interfaces of LabelingAndMergePeakPosition class
 *
 * @author S.Tanaka
 * @date 2008.11.28
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_POSITION_LABELING_AND_MERGE_H__
#define __KOME_POSITION_LABELING_AND_MERGE_H__


namespace kome {
	namespace position {
		namespace merge {

			/**
			 * @class LabelingAndMergePeakPosition
			 * @brief weighted average position class
			 */
			class LabelingAndMergePeakPosition {
			public:
				/**
				 * @fn LabelingAndMergePeakPosition()
				 * @brief constructor
				 */
				LabelingAndMergePeakPosition( kome::objects::SettingParameterValues* settings, kome::ident::SampleGetter* getter );

				/**
				 * @fn virtual ~LabelingAndMergePeakPosition()
				 * @brief destructor
				 */
				virtual ~LabelingAndMergePeakPosition();

			protected:
				/**
				 * @struct PeakInfo
				 * @brief peak information
				 */
				struct PeakInfo {
					double rt;
					double mz;
					double intensity;
					int charge;
					int id;		// @date 2014.01.10 <Add> M.Izumi 
				};


			protected:
				/** setting parameters */
				kome::objects::SettingParameterValues* m_settings;

				/** sample getter */
				kome::ident::SampleGetter* m_getter;

			protected:
				/** peak detector */
				kome::plugin::PluginFunctionItem* m_peakDetector;

				/** peak settings */
				kome::objects::SettingParameterValues* m_peakSettings;

				/** RT tolerance */
				double m_rtTol;

				/** m/z tolerance */
				double m_mzTol;

			public:
				/**
				 * @fn void execute( kome::objects::Peaks2D* peaks, kome::core::Progress& progress )
				 * @brief gets peak positions
				 * @param peaks[out] the object to store peak positions.
				 * @param progress the object to show progress
				 */
				void execute( kome::objects::Peaks2D* peaks, kome::core::Progress& progress );

			protected:
				/**
				 * @fn void setParameters()
				 * @brief sets parametes from settings object
				 */
				void setParameters();

				/**
				 * @fn void detectPeaks( kome::objects::Sample* sample, kome::objects::Peaks2D& peaks, kome::core::Progress& progress )
				 * @brief detects 2D peaks
				 * @param[in] sample sample
				 * @param[out] peaks the object to store peaks.
				 * @param[in,out] progress the object to show the progress
				 */
				void detectPeaks( kome::objects::Sample* sample, kome::objects::Peaks2D& peaks, kome::core::Progress& progress );

			protected:
				/**
				 * @fn static void getSpectra( kome::objects::Sample* sample, kome::objects::DataSet& spectra )
				 * @brief gets spectra contained in the specified sample
				 * @param[in] sample sample
				 * @param[out] spectra the object to store spectra
				 */
				static void getSpectra( kome::objects::Sample* sample, kome::objects::DataSet& spectra );

				/**
				 * @fn static bool lessPeak( std::pair< long long, PeakInfo >& p0, std::pair< long long, PeakInfo >& p1 )
				 * @brief compares peaks to sort 
				 * @parma[in] p0 peak to be compared
				 * @param[in] p1 peak to compare
				 * @return If true, p0 sorts before p1.
				 */
				static bool lessPeak( std::pair< long long, PeakInfo >& p0, std::pair< long long, PeakInfo >& p1 );

				/**
				 * @fn static bool lessCluster( kome::objects::PeaksCluster2D* c0, kome::objects::PeaksCluster2D* c1 )
				 * @brief compares clusters to sort
				 * @param[in] c0 cluster to be compared
				 * @param[in] c1 cluster to compare
				 * @return If true, c0 sorts before c1.
				 */
				static bool lessCluster( kome::objects::PeaksCluster2D* c0, kome::objects::PeaksCluster2D* c1 );

			};
		}
	}
}

#endif	// __KOME_POSITION_LABELING_AND_MERGE_H__

/**
 * @file NeighborPeaks.h
 * @brief interfaces of NeighborPeaks class
 *
 * @author S.Tanaka
 * @date 2008.10.10
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_SEARCH_NEIGHBOR_PEAKS_H__
#define __KOME_SEARCH_NEIGHBOR_PEAKS_H__


namespace kome {
	namespace search {
		namespace neighbor {

			/**
			 * @class NeighborPeaks
			 * @brief Neighbor Peaks class
			 */
			class NeighborPeaks {
			public:
				/**
				 * @fn NeighborPeaks(
							kome::objects::SettingParameterValues* settings
						)
				 * @brief constructor
				 * @param settings setting parameter values
				 */
				NeighborPeaks(
					kome::objects::SettingParameterValues* settings
				);

				/**
				 * @fn virtual ~NeighborPeaks()
				 * @brief destructor
				 */
				virtual ~NeighborPeaks();

			public:
				/**
				 * @typedef PeakValue
				 * @brief peak value type definition
				 */
				typedef enum {
					VALUE_INTENSITY,
					VALUE_AREA
				} PeakValue;

				/**
				 * @typedef PeakSelection
				 * @brief peak selection type definition
				 */
				typedef enum {
					SEL_NEAREST,
					SEL_MAX,
					SEL_SCORE,
					SEL_SCORE2,
					SEL_SUM,
					SEL_AVERAGE
				} PeakSelection;

			public:
				/**
				 * @struct PeakInfo
				 * @brief peak information
				 */
				struct PeakInfo {
					double rt;
					double mz;
					int idx;
				};

			protected:
				/** settings */
				kome::objects::SettingParameterValues* m_settings;

			protected:
				/** spectra filter */
				unsigned char m_filter;

				/** peak detector */
				kome::plugin::PluginFunctionItem* m_peakDetector;

				/** peak settings */
				kome::objects::SettingParameterValues* m_peakSettings;

				/** peak value type */
				PeakValue m_peakValue;

				/** peak selection type */
				PeakSelection m_peakSel;

				/** RT tolerance */
				double m_rtTol;

				/** m/z tolerance */
				double m_mzTol;

			protected:
				/**
				 * @fn void setParameters()
				 * @breif set members from setting parameters
				 */
				void setParameters();

				/**
				 * @fn void getSpectra( kome::objects::Sample* sample, kome::objects::DataSet& spectra, const bool hasChrom )
				 * @brief gets spectra
				 * @param[in] sample sample
				 * @parma[in] spectra the object to store spectra
				 * @parma[in] hasChrom If true, chromatogram is required.
				 */
				void getSpectra( kome::objects::Sample* sample, kome::objects::DataSet& spectra, const bool hasChrom );

			protected:
				/**
				 * @fn static PeakSelection getPeakSelection( const char* s )
				 * @brief gets peak selection type from character string
				 * @param s character string
				 * @return peak selection type
				 */
				static PeakSelection getPeakSelection( const char* s );
				
				/**
				 * @fn static bool lessRt( PeakInfo& peak0, PeakInfo& peak1 )
				 * @brief compares peaks to sort
				 * @param[in] peak0 first peak
				 * @param[in] peak1 second peak
				 * @return If true, peak0 sorts before peak1.
				 */
				static bool lessRt( PeakInfo& peak0, PeakInfo& peak1 );
				
				/**
				 * @fn static bool lessMz( PeakInfo& peak0, PeakInfo& peak1 )
				 * @brief compares peaks to sort
				 * @param[in] peak0 first peak
				 * @param[in] peak1 second peak
				 * @return If true, peak0 sorts before peak1.
				 */
				static bool lessMz( PeakInfo& peak0, PeakInfo& peak1 );

			};
		}
	}
}

#endif	// __KOME_SEARCH_NEIGHBOR_PEAKS_H__

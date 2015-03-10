/**
 * @file MergedSpectrumPeaks.h
 * @brief interfaces of MergedSpectrumPeaks class
 *
 * @author S.Tanaka
 * @date 2009.07.30
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_POSITION_MERGED_SPECTRUM_PEAKS_H__
#define __KOME_POSITION_MERGED_SPECTRUM_PEAKS_H__


namespace kome {
	namespace position {
		namespace merged {

			/**
			 * @class MergedSpectrumPeaks
			 * @brief weighted average position class
			 */
			class MergedSpectrumPeaks {
			public:
				/**
				 * @fn MergedSpectrumPeaks()
				 * @brief constructor
				 */
				MergedSpectrumPeaks( kome::objects::SettingParameterValues* settings, kome::ident::SampleGetter* getter );

				/**
				 * @fn virtual ~MergedSpectrumPeaks()
				 * @brief destructor
				 */
				virtual ~MergedSpectrumPeaks();

			protected:
				/**
				 * @typedef ValueType
				 * @brief peak value type
				 */
				typedef enum {
					VALUE_INT,
					VALUE_AREA
				} ValueType;

				/**
				 * @typedef MassUnit
				 * @brief mass unit
				 */
				typedef enum {
					UNIT_DA,
					UNIT_PPM
				} MassUnit;

			protected:
				/**
				 * @struct PeakPosition
				 * @brief peak position
				 */
				struct PeakPosition {
					kome::objects::PeakElement* peak;
					int smplIdx;
					int peakId;
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

				/** peak filter */
				std::string m_peakFilter;

				/** value type */
				ValueType m_type;

				/** start RT */
				double m_startRt;

				/** end RT */
				double m_endRt;

				/** mass unit */
				MassUnit m_unit;

				/** mass tolerance */
				double m_massTol;

			public:
				/**
				 * @fn void execute( kome::objects::Peaks2D* peaks, kome::core::Progress& progress );
				 * @brief gets peak positions
				 * @param[out] peaks the object to store peaks
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
				 * @fn void detectPeaks( kome::objects::Sample* sample, kome::objects::Peaks& peaks )
				 * @brief detect peaks
				 * @param[in] sample sample
				 * @param[out] peaks the object to store peaks
				 * @return profile ID
				 */
				int detectPeaks( kome::objects::Sample* sample, kome::objects::Peaks& peaks );

			};
		}
	}
}


#endif	// __KOME_POSITION_MERGED_SPECTRUM_PEAKS_H__

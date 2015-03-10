/**
 * @file SimpleChargeDetect2.h
 * @brief interfaces of SimpleChargeDetect class (Rewrite Version)
 *
 * @author M.Fukuda
 * @date 2010.06.08
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#ifndef __KOME_CHARGE_SIMPLE2_CHARGE_DETECT_H__
#define __KOME_CHARGE_SIMPLE2_CHARGE_DETECT_H__

#define MZ_TOLERANCE "mz_tolerance"
#define MAX_CHARGE "max_charge"

namespace kome {
	namespace charge {
		namespace simple2 {
			/**
			 * @class SimpleChargeDetect
			 * @brief simple charge detect class
			 */
			class SimpleChargeDetect {
			
			public:
				/**
				 * @fn SimplwChargeDetect()
				 * @brief constructor
				 * @param params parameters object
				 */
				SimpleChargeDetect(kome::objects::Parameters* params);
				
				/**
				 * @fn ~SimpleChargeDetect()
				 * @brief destructor
				 */
				virtual ~SimpleChargeDetect();
			protected:
				//spectrum information
				kome::objects::Spectrum* m_spectrum;

				//peaks object to store charge
				kome::objects::Peaks* m_peaks;

				//setting object
				kome::objects::SettingParameterValues* m_settings;

				// m/z tolerance
				double m_mzTolerance;

				// precursor tolerance
				double m_precursorTol;

				// max charge
				int m_maxCharge;

				// Natural
				double m_N;

				// parent peak detection
				kome::plugin::PluginFunctionItem* m_peakDetector;

				// parent peak settins
				kome::objects::SettingParameterValues* m_peakSettings;
			
			public:				
				/**
				 * @fn execute()
				 * @brief
				 */
				void getCharge();

				/**
				 * @fn bool checkStatus()
				 * @brief check status
				 * @return return bool
				 */
				bool checkStatus();

			protected:
				/**
				 * @fn setStatus()
				 * @brief set status
				 */
				void setStatus();
			};
		}
	}
}

#endif //__KOME_CHARGE_SIMPLE2_CHARGE_DETECT_H__

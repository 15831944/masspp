/**
 * @file IntensityRateChargeDetect.h
 * @brief interfaces of IntensityRateChargeDetect class
 *
 * @author S.Tanaka
 * @date 2006.10.15
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_CHARGE_IR_INTENSITY_RATE_CHARGE_DETECT_H__
#define __KOME_CHARGE_IR_INTENSITY_RATE_CHARGE_DETECT_H__


namespace kome {
	namespace charge {
		namespace ir {
			/**
			 * @class IntensityRateChargeDetect
			 * @brief intensity rate charge detect class
			 */
			class IntensityRateChargeDetect {
			public:
				/**
				 * @fn IntensityRateChargeDetect()
				 * @brief constructor
				 */
				IntensityRateChargeDetect();

				/**
				 * @fn virtual ~IntensityRateChargeDetect()
				 * @brief destructor
				 */
				virtual ~IntensityRateChargeDetect();
            
            protected:
				/** relative position of borderline from precursor */
				double m_border;
				/** rate threshold */
				double m_rateThreshold;
				/** max charge */
				int m_maxCharge;
			
            public:
				/**
				 * @fn void getCharge(
						kome::core::XYData& xyData,
						kome::objects::Peaks& peaks
					)
				 * @brief detects charge
				 * @param xyData data points
				 * @param peaks peaks object to store charge
				 */
				void getCharge(
					kome::core::XYData& xyData,
					kome::objects::Peaks& peaks
				);
			
			};
		}
	}
}

#endif	// __KOME_CHARGE_IR_INTENSITY_RATE_CHARGE_DETECT_H__

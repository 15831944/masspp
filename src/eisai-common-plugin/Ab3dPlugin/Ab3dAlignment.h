/**
 * @file Ab3dAlignment.h
 * @brief interfaces of Ab3dAlignmentclass
 *
 * @author S.Tanaka
 * @date 2008.08.11
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_AB3D_ALIGNMENT_H__
#define __KOME_AB3D_ALIGNMENT_H__


namespace kome {
	namespace ab3d {

		/**
		 * @class Ab3dAlignment
		 * @brief AB3D alignment class
		 */
		class Ab3dAlignment : public kome::operation::Alignment {
		public:
			/**
			 * @fn Ab3dAlignment( kome::objects::SettingParameterValues* settings )
			 * @brief constructor
			 * @param settings setting parameter values
			 */
			Ab3dAlignment( kome::objects::SettingParameterValues* settings );

			/**
			 * @fn virtual ~Ab3dAlignment()
			 * @brief destructor
			 */
			virtual ~Ab3dAlignment();

		public:
			/**
			 * @typedef FittingMethod
			 * @brief fitting method type
			 */
			typedef enum {
				FITTING_LINEAR_INTERPOLATION,
				FITTING_SPLINE_INTERPOLATION,
				FITTING_LEAST_SQUARE_LINEAR,
				FITTING_LEAST_SQUARE_CUBIC,
				FITTING_LOGISTIC_REGRESSION
			} FittingMethod;

		protected:
			/** fitting method */
			FittingMethod m_fittingMethod;

			/** settings */
			kome::objects::SettingParameterValues* m_settings;

		public:
			/**
			 * @fn bool getCorrectionPoints( kome::core::XYData& corrPts, kome::core::XYData& tbPts )
			 * @brief gets correction points
			 * @param corrPts the data points object to store correction points (unit. min)
			 * @param tbPts traceback information object (unit. sec)
			 * @return If true, it succeeded to get correction points.
			 */
			bool getCorrectionPoints( kome::core::XYData& corrPts, kome::core::XYData& tbPts );

		protected:
			/**
			 * @fn bool executeLinearInterpolation( kome::core::XYData& corrPts, kome::core::XYData& tbPts, double* rts, const unsigned int num )
			 * @brief executes linear interpolation
			 * @param corrPts the data points object to store correction points (unit. min)
			 * @param tbPts traceback information objects (unit. sec)
			 * @param rts retention time array
			 * @param num the size of retention time array
			 * @return If true, it succeeded to get correction points.
			 */
			bool executeLinearInterpolation( kome::core::XYData& corrPts, kome::core::XYData& tbPts, double* rts, const unsigned int num );

			/**
			 * @fn bool executeSplineInterpolation( kome::core::XYData& corrPts, kome::core::XYData& tbPts, double* rts, const unsigned int num )
			 * @brief executes spline interpolation
			 * @param corrPts the data points object to store correction points (unit. min)
			 * @param tbPts traceback information objects (unit. sec)
			 * @param rts retention time array
			 * @param num the size of retention time array
			 * @return If true, it succeeded to get correction points.
			 */			
			bool executeSplineInterpolation( kome::core::XYData& corrPts, kome::core::XYData& tbPts, double* rts, const unsigned int num );

			/**
			 * @fn bool executeLeastSquareLinear( kome::core::XYData& corrPts, kome::core::XYData& tbPts, double* rts, const unsigned int num )
			 * @brief executes least square method to detect linear polynomial
			 * @param corrPts the data points object to store correction points (unit. min)
			 * @param tbPts traceback information objects (unit. sec)
			 * @param rts retention time array
			 * @param num the size of retention time array
			 * @return If true, it succeeded to get correction points.
			 */			
			bool executeLeastSquareLinear( kome::core::XYData& corrPts, kome::core::XYData& tbPts, double* rts, const unsigned int num );

			/**
			 * @fn bool executeLeastSquareCubic( kome::core::XYData& corrPts, kome::core::XYData& tbPts, double* rts, const unsigned int num )
			 * @brief executes least square method to detect cubic polynomial
			 * @param corrPts the data points object to store correction points (unit. min)
			 * @param tbPts traceback information objects (unit. sec)
			 * @param rts retention time array
			 * @param num the size of retention time array
			 * @return If true, it succeeded to get correction points.
			 */			
			bool executeLeastSquareCubic( kome::core::XYData& corrPts, kome::core::XYData& tbPts, double* rts, const unsigned int num );

			/**
			 * @fn bool executeLogisticRegression( kome::core::XYData& corrPts, kome::core::XYData& tbPts, double* rts, const unsigned int num )
			 * @brief executes logistic regression fitting
			 * @param corrPts the data points object to store correction points (unit. min)
			 * @param tbPts traceback information objects (unit. sec)
			 * @param rts retention time array
			 * @param num the size of retention time array
			 * @return If true, it succeeded to get correction points.
			 */			
			bool executeLogisticRegression( kome::core::XYData& corrPts, kome::core::XYData& tbPts, double* rts, const unsigned int num );

		protected:
			/**
			 * @fn void getSpectra( kome::objects::Sample* sample, kome::objects::DataSet& spectra )
			 * @brief gets the spectra
			 * @param[in] sample sample
			 * @param[out] spectra data set object to store spectra
			 */
			void getSpectra( kome::objects::Sample* sample, kome::objects::DataSet& spectra );

		protected:
			/**
			 * @fn static double getSumOfDifferentialSquareOfLogistic( double* vertex, kome::core::XYData* dps )
			 * @brief gets sum of differential square of logistic regression function
			 * @param vertex parameters
			 * @param dps traceback points
			 * @return the sum of differential square of logistic regression function
			 */
			static double getSumOfDifferentialSquareOfLogistic( double* vertex, kome::core::XYData* dps );
		};
	}
}


#endif	// __KOME_AB3D_ALIGNMENT_H__

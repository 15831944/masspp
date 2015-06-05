/**
 * @file MsbackadjBaselineDetectionManager.h
 * @brief interfaces of MsbackadjBaselineDetectionManager class
 *
 * @author A.Ozaki
 * @date 2013.08.01
 * 
 * Copyright(C) 2013-2013 Shimadzu Corporation. All rights reserved.
 */


#ifndef __KOME_BASELINE_MSBACKADJ_MANAGER_H__
#define __KOME_BASELINE_MSBACKADJ_MANAGER_H__


namespace kome {
	namespace baseline {
		namespace msbackadj {

			/**
			 * @class AbcBaselineDetectionManager
			 * @brief weighted average baseline class
			 */
			class MsbackadjBaselineDetectionManager {
			protected:
				/**
				 * @fn MsbackadjBaselineDetectionManager( void )
				 * @brief constructor
				 */
				MsbackadjBaselineDetectionManager( void );

				/**
				 * @fn virtual ~MsbackadjBaselineDetectionManager( void )
				 * @brief destructor
				 */
				virtual ~MsbackadjBaselineDetectionManager( void );

			protected:
				/** window size */
				double	m_dWindowSize;

				/** step size */
				double	m_dStepSize;

				/** regression method */
				int	m_nRegressionMethod;

				/** estimation method */
				int	m_nEstimationMethod;

				/** smoothing method */
				int	m_nSmoothingMethod;

				/** quantile */
				double	m_dQuantile;

			public:
				/**
				 * @fn void setWindowSize( const double dSize )
				 * @brief sets window size
				 * @param dSize window size
				 */
				void	setWindowSize( const double dSize );

				/**
				 * @fn double getWindowSize( void )
				 * @brief gets window size
				 * @return window size
				 */
				double	getWindowSize( void );

				/**
				 * @fn void setStepSize( const double dSize )
				 * @brief sets step size
				 * @param dSize step size
				 */
				void	setStepSize( const double dSize );

				/**
				 * @fn double getStepSize( void )
				 * @brief gets step size
				 * @return step size
				 */
				double	getStepSize( void );

				/**
				 * @fn void setRegressionMethod( const ind nMethodValue )
				 * @brief sets regression method
				 * @param nMethodValue number of regression method
				 */
				void	setRegressionMethod( const int nMethodValue );

				/**
				 * @fn double getRegressionMethod( void )
				 * @brief gets regression method
				 * @return number of regresstion method
				 */
				int	getRegressionMethod( void );

				/**
				 * @fn void setEstimationMethod( const ind nMethodValue )
				 * @brief sets estimation method
				 * @param nMethodValue number of estimation method
				 */
				void	setEstimationMethod( const int nMethodValue );

				/**
				 * @fn double getEstimationMethod( void )
				 * @brief gets estimation method
				 * @return number of estimation method
				 */
				int	getEstimationMethod( void );

				/**
				 * @fn void setSmoothingMethod( const ind nMethodValue )
				 * @brief sets smoothing method
				 * @param nMethodValue number of smoothing method
				 */
				void	setSmoothingMethod( const int nMethodValue );

				/**
				 * @fn double getSmoothingMethod( void )
				 * @brief gets smoothing method
				 * @return number of smoothing method
				 */
				int	getSmoothingMethod( void );

				/**
				 * @fn void setSQuantile( const double dQuantile )
				 * @brief sets quantile
				 * @param dQuantile quantile
				 */
				void setQuantile( const double dQuantile );

				/**
				 * @fn int getQuantile( void )
				 * @brief gets quantile
				 * @return quantile
				 */
				double	getQuantile( void );

			public:
				/**
				 * @fn void getMsbackadj( kome::core::XYData& src, kome::core::XYData& baseline )
				 * @brief gets baseline
				 * @param src source data points
				 * @param baseline the object to store baseline
				 */
				void getMsbackadj( kome::core::XYData& src, kome::core::XYData& baseline );

				/**
				 * @fn void releaseMatlab( void )
				 * @brief release MATLAB application
				 */
				void	releaseMatlab( void );

			protected:
				/**
				 * @fn double getNoiseThreshold( kome::core::XYData& src )
				 * @brief gets noise threshold
				 * @param src source data points
				 * @return noise threshold
				 */				 
				double getNoiseThreshold( kome::core::XYData& src );

			private:
				/** Progress */
				kome::window::DialogProgress	*m_pProgress;

			private:
				/** initialize application */
				bool	initializeApplication( void );

			public:
				/**
				 * @fn static MsbackadjBaselineDetectionManager& getInstance( void )
				 * @brief gets MsbackadjBaselineDetectionManager object
				 * @return AbcBaselineDetectionManager object (This is the only object.)
				 */
				static MsbackadjBaselineDetectionManager& getInstance( void );
			};
		}
	}
}

#endif	// __KOME_BASELINE_MSBACKADJ_MANAGER_H__

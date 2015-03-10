/**
 * @file MspeaksDetectionManager.h
 * @brief interfaces of MspeaksDetectionManager class
 *
 * @author A.Ozaki
 * @date 2013.07.31
 * 
 * Copyright(C) 2013-2013 Shimadzu Corporation All rights reserved.
 */


#ifndef __KOME_PEAKS_MSPEAKS_DETECTION_MANAGER_H__
#define __KOME_PEAKS_MSPEAKS_DETECTION_MANAGER_H__


namespace kome {
	namespace peaks {
		namespace mspeaks {

			/**
			 * @class MspeaksDetectionManager
			 * @brief MspeaksDetectionManager class
			 */
			class MspeaksDetectionManager
			{
			protected:
				/**
				 * @fn MspeaksDetectionManager( void )
				 * @brief constructor
				 */
				MspeaksDetectionManager( void );

				/**
				 * @fn virtual ~MspeaksDetectionManager( void )
				 * @brief destructor
				 */
				virtual ~MspeaksDetectionManager( void );

			protected:
				/** bsse */
				int	m_nBase;

				/** levels */
				int m_nLevels;

				/** noise */
				int	m_nNoise;

				/** threshold multipliner */
				double	m_dMultipliner;

				/** denoising */
				int	m_nDenoising;

				/** peak location */
				double	m_dPeakLocation;

				/** FWHM filter*/
				double	m_dFWHMFilter;

				/** over segmentation */
				double	m_dOverSegmentation;

			public:
				/**
				 * @fn void setBase( const int nBase )
				 * @brief sets base
				 * @param dBase base
				 */
				void	setBase( const int nBase );

				/**
				 * @fn int getBase( void )
				 * @brief gets base
				 * @return base
				 */
				int	getBase( void );

				/**
				 * @fn void setLevels( const int nLevels )
				 * @brief sets levels
				 * @param nLevels levels
				 */
				void	setLevels( const int nLevels );

				/**
				 * @fn int getLevels( void )
				 * @brief gets levels
				 * @return levels
				 */
				int	getLevels( void );

				/**
				 * @fn void setNoise( const int nNoise)
				 * @brief sets noise
				 * @param nNoise noise
				 */
				void	setNoise( const int nNoise );

				/**
				 * @fn dint getNoise( void )
				 * @brief gets noise
				 * @return noise
				 */
				int	getNoise( void );

				/**
				 * @fn void setMulipliner( const double dMultipliner )
				 * @brief sets multipliner
				 * @param dMultipliner multipliner
				 */
				void	setMultipliner( const double dMultipliner );

				/**
				 * @fn double getMultipliner( void )
				 * @brief gets multi pliner
				 * @return multi pliner
				 */
				double	getMultipliner( void );

				/**
				 * @fn void setDenoising( const int nDenoising )
				 * @brief sets denoising
				 * @param nDenoising denoising
				 */
				void	setDenoising( const int nDenoising );

				/**
				 * @fn int getDenoising( void )
				 * @brief gets denoising
				 * @return denoising
				 */
				int	getDenoising( void );

				/**
				 * @fn void setPeakLocation( const double dPeakLocation )
				 * @brief sets peaklocation
				 * @param dPeakLocation peak location
				 */
				void	setPeakLocation( const double dPeakLocation );

				/**
				 * @fn double getPeakLocation( void )
				 * @brief gets peak location
				 * @return PeakLocation
				 */
				double	getPeakLocation( void );

				/**
				 * @fn void setFWHMFilter( const double dFWHMFilter )
				 * @brief sets FWHMFilter
				 * @param dFWHMFilter FWHM filter
				 */
				void	setFWHMFilter( const double dFWHMFilter );

				/**
				 * @fn double getFWHMFilter( void )
				 * @brief gets FWHMFilter
				 * @return FWHM Filter
				 */
				double	getFWHMFilter( void );

				/**
				 * @fn void setOverSegmentation( const couble nOverSegmentation )
				 * @brief sets over segmentation
				 * @param dOverSegmentation over segmentation
				 */
				void	setOverSegmentation( const double dOverSegmentation );

				/**
				 * @fn double getOverSegmentation( void )
				 * @brief gets over segmentaion
				 * @return over segmentaion
				 */
				double	getOverSegmentation( void );

			public:
				/**
				 * @fn void detection( kome::core::XYData& xydata, kome::objects::Peaks& peaks )
				 * @brief detect peaks
				 * @param[in] xyData data points
				 * @pram[out] peaks the object to store peaks
				 */
				void	detection( kome::core::XYData& refXydata,
									kome::objects::Peaks& refPeaks );

				/**
				 * @fn void releaseMatlab( void )
				 * @brief release MATLAB application
				 */
				void	releaseMatlab( void );

			private:
				/** Progress */
				kome::window::DialogProgress	*m_pProgress;

			private:
				/** initialize application */
				bool	initializeApplication( void );

			public:
				/**
				 * @fn static MspeaksDetectionManager& getInstance()
				 * @brief gets object
				 * @return MspeaksDetectionManager object. (This is the only object.)
				 */
				static MspeaksDetectionManager& getInstance( void );
			};
		}
	}
}

#endif	// __KOME_PEAKS_MSPEAKS_DETECTION_MANAGER_H__

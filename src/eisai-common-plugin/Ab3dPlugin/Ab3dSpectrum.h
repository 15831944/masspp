/**
 * @file Ab3dSpectrum.h
 * @brief interfaces of Ab3dSpectrum class
 *
 * @author S.Tanaka
 * @date 2007.09.10
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_AB3D_SPECTRUM_H__
#define __KOME_AB3D_SPECTRUM_H__


#include <vector>


namespace kome {
	namespace ab3d {

		class Ab3dSample;

		/**
		 * @class Ab3dSpectrum
		 * @brief spectrum of composition sample class
		 */
		class Ab3dSpectrum : public kome::objects::Spectrum {
		public:
			/**
			 * @fn Ab3dSpectrum( Ab3dSample& sample, kome::objects::Spectrum* ctrlSpec, kome::objects::Spectrum* trmtSpec )
			 * @brief constructor
			 * @param sample composition sample object
			 * @param ctrlSpec control spectrum
			 * @param trmtSpec sample spectrum
			 */
			Ab3dSpectrum( Ab3dSample& sample, kome::objects::Spectrum* ctrlSpec, kome::objects::Spectrum* trmtSpec );

			/**
			 * @fn virtual ~Ab3dSpectrum()
			 * @brief destructor
			 */
			virtual ~Ab3dSpectrum();

		public:
			/**
			 * @struct PointInfo
			 * @brief point information
			 */
			struct PointInfo {
				double mz;
				double intensity;
			};

		protected:
			/** sample object */
			Ab3dSample& m_ab3dSample;

			/** control spectrum */
			kome::objects::Spectrum* m_ctrlSpec;

			/** sample spectrum */
			kome::objects::Spectrum* m_trmtSpec;

			/** data points */
			std::vector< PointInfo > m_points;

		public:
			/**
			 * @fn kome::objects::Spectrum* getCtrlSpectrum()
			 * @brief gets control spectrum
			 * @return control spectrum
			 */
			kome::objects::Spectrum* getCtrlSpectrum();

			/**
			 * @fn kome::objects::Spectrum* getSmplSpectrum()
			 * @brief gets sample spectrum
			 * @return sample spectrum
			 */
			kome::objects::Spectrum* getSmplSpectrum();

			/**
			 * @fn void addPeakInfo(
					const double rt,
					const double mz,
					const double intensity
				)
			 * @brief adds peak information
			 * @param rt RT
			 * @param mz m/z
			 * @param intensity intensity
			 */
			void addPeakInfo(
				const double rt,
				const double mz,
				const double intensity
			);

			/**
			 * @fn void sort()
			 * @brief sorts data
			 */
			void sort();

			/**
			 * @fn unsigned int getNumberOfPoints()
			 * @brief gets the number of points
			 * @return the number of points
			 */
			unsigned int getNumberOfPoints();

			/**
			 * @fn PointInfo* getPoint( const unsigned int index )
			 * @brief gets point information
			 * @param index point index
			 * @return point information (If NULL, the index is illegal.)
			 */
			PointInfo* getPoint( const unsigned int index );

		protected:
			/**
			 * @fn virtual void onGetXYData( kome::core::XYData* const xyData, const double minX, const double maxX )
			 * @brief This method is called by getXYData method. (override method)
			 * @param xyData the object to store data points
			 * @param minX minimum x value. (If minX is negative number, minimum x value is not unbounded.)
			 * @param maxX maximum x value. (If maxX is negative number, maximum x value is not unbounded.)
			 */
			virtual void onGetXYData( kome::core::XYData* const xyData, const double minX, const double maxX );

			/**
			 * @fn virtual void onGetXRange( double* minX, double* maxX )
			 * @brief This method is called by getMinX or getMaxX method. (override method)
			 * @param minX the pointer to store minimum x value
			 * @param maxX the pointer to store maximum x value
			 */
			virtual void onGetXRange( double* minX, double* maxX );

			/**
			 * @fn virtual double onGetTotalIntensity( const double minX, const double maxX )
			 * @brief This method is called by getTotalIntensity method. (override method)
			 * @param minX minimum x of range (If minX is negative number, minimum x value is unbounded.)
			 * @param maxX maximum x of range (If maxX is negative number, maximum x value is unbounded.)
			 * @return total intensity
			 */
			virtual double onGetTotalIntensity( const double minX, const double maxX );

			/**
			 * @fn virtual double onGetMaxIntensity( const double minX, const double maxX )
			 * @brief This method is called by getMaxIntensity method. (override method)
			 * @param minX minimum x of range (If minX is negative number, minimum x value is unbounded.)
			 * @param maxX maximum x of range (If maxX is negative number, maximum x value is unbounded.)
			 * @return max intensity
			 */
			virtual double onGetMaxIntensity( const double minX, const double maxX );

			// >>>>>> @Date:2013/09/05	<Add>	A.Ozaki
			//
			/**
			 * @fn virtual  bool onSetRequestLoadData( void )
			 * @brief This method is called by setRequestLoadData method. (abstract method)
			 */
			virtual void	onSetRequestLoadData( void );
			/**
			 * @fn virtual  bool onResetRequestLoadData( void )
			 * @brief This method is called by resetRequestLoadData method. (abstract method)
			 */
			virtual void	onResetRequestLoadData( void );
			
			/**
			 * @fn virtual  bool onIsRequestLoadData( void )
			 * @brief This method is called by isRequestLoadData method. (abstract method)
			 * @return If true, file read request is valid.
			 */
			virtual bool	onIsRequestLoadData( void );

			/**
			 * @fn virtual  bool onSetFirstAccess( void )
			 * @brief This method is called by setFirstAccess method. (abstract method)
			 */
			virtual void	onSetFirstAccess( void );

			/**
			 * @fn virtual  bool onResetFirstAccess( void )
			 * @brief This method is called by resetFirstAccess method. (abstract method)
			 */
			virtual void	onResetFirstAccess( void );
			
			/**
			 * @fn virtual  bool onIsFirstAccess( void )
			 * @brief This method is called by isFirstAccess method. (abstract method)
			 * @return If true, the first accessing.
			 */
			virtual bool	onIsFirstAccess( void );
			
			/**
			 * @fn virtual  bool onLoadData( void )
			 * @brief This method is called by loadData method. (abstract method)
			 * @return If true, file reading success.
			 */
			virtual bool	onLoadData( void );
			//
			// <<<<<< @Date:2013/09/05	<Add>	A.Ozaki

		protected:
			/**
			 * @fn static bool lessPoint( PointInfo p0, PointInfo p1 )
			 * @brief compares to sort point information
			 * @param p0 point information to be compared
			 * @param p1 point information to compare
			 * @return If true, p0 sorts before p1
			 */
			static bool lessPoint( PointInfo p0, PointInfo p1 );
		};
	}
}


#endif		// __KOME_AB3D_SPECTRUM_H__

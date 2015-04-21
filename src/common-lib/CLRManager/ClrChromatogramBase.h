/**
 * @file ClrChromatogramBase.h
 * @brief interfaces of ClrChromatogramBase class
 *
 * @author S.Tanaka
 * @date 2008.04.14
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_CLR_CHROMATOGRAM_BASE_H__
#define __KOME_CLR_CHROMATOGRAM_BASE_H__


#include "ChromatogramWrapper.h"


namespace kome {
	namespace clr {

		ref class SampleWrapper;
		ref class XYDataWrapper;

		/**
		 * @class ClrChromatogramBase
		 * @brief abstract class of spectrum information on CLR environment
		 */
#ifndef _MANAGED	// (for doxygen)
		public ref class ClrChromatogramBase : public ChromatogramWrapper {
#else
		public ref class ClrChromatogramBase abstract : public ChromatogramWrapper {
#endif	// _MANAGED
		public:
			/**
			 * @fn ClrChromatogramBase( SampleWrapper^ sample )
			 * @brief constructor
			 * @param[in] sample sample object that has this spectrum
			 */
			ClrChromatogramBase( SampleWrapper^ sample );

			/**
			 * @fn virtual ~ClrChromatogramBase()
			 * @brief destructor
			 */
			virtual ~ClrChromatogramBase();

		public:
			/**
			 * @fn virtual void onGetXYData( XYDataWrapper^ xyData )
			 * @brief This method is called by getXYData method. (abstract method)
			 * @param[out] xyData the object to store data points
			 */
			virtual void onGetXYData( XYDataWrapper^ xyData ) = 0;

			/**
			 * @fn virtual void onGetSpectra(
						DataSetWrapper^ dataSet,
						double startRt,
						double endRt,
						SearchType startSearch,
						SearchType endSearch
				)
			 * @brief get spectra in specified RT range (abstract method)
			 * @param[out] dataSet data set object to store spectra
			 * @param[in] startRt start RT
			 * @param[in] endRt end RT
			 * @param[in] startSearch start search type
			 * @param[in] endSearch end search type
			 */
			virtual void onGetSpectra(
					DataSetWrapper^ dataSet,
					double startRt,
					double endRt,
					SearchType startSearch,
					SearchType endSearch
			) = 0;

			/**
			 * @fn virtual double onGetMass( unsigned int index ) = 0
			 * @brief This method is called by getMass method (abstract method)
			 * @param[in] index data index
			 * @return mass
			 */
			virtual double onGetMass( unsigned int index ) = 0;

			/**
			 * @fn virtual int onGetMsStage( unsigned int index ) = 0
			 * @brief This method is called by getMsStage method (abstract method)
			 * @param[in] index data index
			 * @return MS stage.
			 */
			virtual int onGetMsStage( unsigned int index ) = 0;

			/**
			 * @fn virtual double onGetPrecursor( unsigned int index ) = 0
			 * @brief This method is called by getPrecursor method (abstract method)
			 * @param[in] index data index
			 * @return precursor mass
			 */
			virtual double onGetPrecursor( unsigned int index ) = 0;
		};
	}
}

#endif		// __KOME_CLR_CHROMATOGRAM_BASE_H__

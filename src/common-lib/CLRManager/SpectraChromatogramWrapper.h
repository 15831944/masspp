/**
 * @file SpectraChromatogramWrapper.h
 * @brief interfaces of SpectraChromatogramWrapper class
 *
 * @author S.Tanaka
 * @date 2008.04.14
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_CLR_GROUP_CHROMATOGRAM_WRAPPER_H__
#define __KOME_CLR_GROUP_CHROMATOGRAM_WRAPPER_H__


#include "ChromatogramWrapper.h"


namespace kome {
	namespace clr {

		/**
		 * @class SpectraChromatogramWrapper
		 * @brief SpectraChromatogram object wrapper class to use on CLR environment
		 */
		public ref class SpectraChromatogramWrapper : public ChromatogramWrapper {
		public:
			/**
			 * @fn SpectraChromatogramWrapper( kome::objects::SpectraChromatogram& chromatogram )
			 * @brief constructor
			 * @param[in] chromatogram chromatogram object
			 */
			SpectraChromatogramWrapper( kome::objects::SpectraChromatogram& chromatogram );

			/**
			 * @fn virtual ~SpectraChromatogramWrapper()
			 * @brief destructor
			 */
			virtual ~SpectraChromatogramWrapper();

		protected:
			/** SpectraChromatogram object */
			kome::objects::SpectraChromatogram& m_groupChrom;

		public:
			/**
			 * @fn kome::objects::SpectraChromatogram& getSpectraChromatogram()
			 * @brief gets chromatogram object
			 * @return chromatogram object
			 */
			kome::objects::SpectraChromatogram& getSpectraChromatogram();

		public:
			/**
			 * @fn DataSetWrapper^ getMemberSpectra()
			 * @brief gets spectra
			 * @return spectra
			 */
			DataSetWrapper^ getMemberSpectra();

			/**
			 * @fn void setMzRange( double minMz, double maxMz )
			 * @brief sets m/z range
			 * @param[in] minMz minimum value of m/z range (If minMz is negative number, m/z range doesn't have lower bound.)
			 * @param[in] maxMz maximum value of m/z range (If minMz is negative number, m/z range doesn't have upper bound.)
			 */
			void setMzRange( double minMz, double maxMz );

			/**
			 * @fn void setMinMz( double mz )
			 * @brief sets minimum value of m/z range
			 * @param[in] mz minimum value of m/z range (If minMz is negative number, m/z range doesn't have lower bound.)
			 */
			void setMinMz( double mz );

			/**
			 * @fn void setMaxMz( double mz )
			 * @brief sets maximum value of m/z range
			 * @param[in] mz maximum value of m/z range (If maxMz is negative number, m/z range doesn't have upper bound.)
			 */
			void setMaxMz( double mz );

			/**
			 * @fn double getMinMz()
			 * @brief gets minimum value of m/z range
			 * @return minimum value of m/z range (If m/z range doesn't have lower bound, this method returns negative number.)
			 */
			double getMinMz();

			/**
			 * @fn double getMaxMz()
			 * @brief gets maximum value of m/z range
			 * @return maximum value of m/z range (If m/z range doesn't have upper bound, this method returns negative number.)
			 */
			double getMaxMz();

			/**
			 * @fn void setRtRange( double minRt, double maxRt )
			 * @brief sets RT range
			 * @param[in] minRt minimum value of RT range (If minRt is negative number, RT range doesn't have lower bound.)
			 * @param[in] maxRt maximum value of RT range (If maxRt is negative number, RT range doesn't have upper bound.)
			 */
			void setRtRange( double minRt, double maxRt );

			/**
			 * @fn void setMinRt( double rt )
			 * @brief sets minimum value of RT range
			 * @param[in] rt minimum value of RT range (If minRt is negative number, RT range doesn't have lower bound.)
			 */
			void setMinRt( double rt );

			/**
			 * @fn void setMaxRt( double rt )
			 * @brief sets maximum value of RT range
			 * @param[in] rt maximum value of RT range (If maxRt is negative number, RT range doesn't have upper bound.)
			 */
			void setMaxRt( double rt );

			/**
			 * @fn double getMinRt()
			 * @brief gets minimum value of RT range
			 * @return minimum value of RT range (If RT range doesn't have lower bound, this method returns negative number.)
			 */
			double getMinRt();

			/**
			 * @fn double getMaxRt()
			 * @brief gets maximum value of RT range
			 * @return maximum value of RT range (If RT range doesn't have upper bound, this method returns negative number.)
			 */
			double getMaxRt();

			/**
			 * @fn void setFilter( unsigned char filter )
			 * @brief sets filter
			 * @param[in] filter 
					  - The least significant bit is the ms1 filter.
					  - The second least significant bit is the ms2 filter.
					  - ...
					  - The most least significant bit is the ms8 or more filter.
			 */
			void setFilter( unsigned char filter );

			/**
			 * @fn unsigned char getFilter()
			 * @brief gets filter
			 * @return filter
					  - The least significant bit is the ms1 filter.
					  - The second least significant bit is the ms2 filter.
					  - ...
					  - The most least significant bit is the ms8 or more filter.
			 */
			unsigned char getFilter();

			/**
			 * @fn void setTIC()
			 * @brief sets Total Ion SpectraChromatogram mode
			 */
			void setTIC();

			/**
			 * @fn void setBPC()
			 * @brief sets Base Peak SpectraChromatogram mode
			 */
			void setBPC();

			/**
			 * @fn bool isTIC()
			 * @brief judges whether chromatogram mode is TIC (Total Ion SpectraChromatogram)
			 * @return whether chromatogram mode is TIC (Total Ion SpectraChromatogram)
			 */
			bool isTIC();

			/**
			 * @fn bool isBPC()
			 * @brief judges whether chromatogram mode is BPC (Base Peak SpectraChromatogram)
			 * @return whether chromatogram mode is BPC (Base Peak SpectraChromatogram)
			 */
			bool isBPC();
		};
	}
}

#endif	// __KOME_CLR_GROUP_CHROMATOGRAM_WRAPPER_H__

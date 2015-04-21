/**
 * @file PeaksManagerWrapper.h
 * @brief interfaces of PeaksManagerWrapper class
 *
 * @author S.Tanaka
 * @date 2008.01.07
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_CLR_PEAKS_MANAGER_WRAPPER_H__
#define __KOME_CLR_PEAKS_MANAGER_WRAPPER_H__


#include "DataManagerWrapper.h"


namespace kome { 
	namespace clr {

		ref class SpectrumWrapper;
		ref class ChromatogramWrapper;
		ref class DataGroupNodeWrapper;
		ref class PeaksWrapper;
		ref class Peaks2DWrapper;
		ref class PeakElementWrapper;

		/**
		 * @class PeaksManagerWrapper
		 * @brief peaks manager wrapper class to use on CLR environment
		 */
		public ref class PeaksManagerWrapper : public DataManagerWrapper {
		protected:
			/**
			 * @fn PeaksManagerWrapper( kome::objects::PeaksManager& mgr )
			 * @brief constructor
			 * @param[in] mgr data manager object
			 */
			PeaksManagerWrapper( kome::objects::PeaksManager& mgr );

			/**
			 * @fn virtual ~PeaksManagerWrapper()
			 * @brief destructor
			 */
			virtual ~PeaksManagerWrapper();

		protected:
			/** data manager object */
			kome::objects::PeaksManager& m_mgr;

		public:
			/**
			 * @fn kome::objects::PeaksManager& getPeaksManager()
			 * @brief gets data manager object
			 * @return data manager object
			 */
			kome::objects::PeaksManager& getPeaksManager();

		public:
			/**
			 * @fn PeaksWrapper^ getPeaks( SpectrumWrapper^ spec )
			 * @brief gets peaks of specified spectrum
			 * @param[in] spec spectrum
			 * @return peaks object (If NULL, peaks object is not created yet.)
			 */
			PeaksWrapper^ getPeaks( SpectrumWrapper^ spec );

			/**
			 * @fn PeaksWrapper^ getPeaks( ChromatogramWrapper^ chrom )
			 * @brief gets peaks of specified chromatogram
			 * @param[in] chrom chromatogram
			 * @return peaks object (If NULL, peaks object is not created yet.)
			 */
			PeaksWrapper^ getPeaks( ChromatogramWrapper^ chrom );

			/**
			 * @fn Peaks2DWrapper^ getPeaks( DataGroupNodeWrapper^ group )
			 * @brief gets peak of specified spectrum group
			 * @param[in] group spectrum group
			 * @return peaks object (If NULL, peaks object is not created yet.)
			 */
			Peaks2DWrapper^ getPeaks( DataGroupNodeWrapper^ group );

			/**
			 * @fn PeaksWrapper^ createPeaks( SpectrumWrapper^ spec )
			 * @brief creates peaks of specified spectrum
			 * @param[in] spec spectrum
			 * @return peaks object
			 */
			PeaksWrapper^ createPeaks( SpectrumWrapper^ spec );

			/**
			 * @fn PeaksWrapper^ createPeaks( ChromatogramWrapper^ chrom )
			 * @brief creates peaks of specified chromatogram
			 * @param[in] chrom chromatogram
			 * @return peaks object
			 */
			PeaksWrapper^ createPeaks( ChromatogramWrapper^ chrom );

			/**
			 * @fn Peaks2DWrapper^ createPeaks( DataGroupNodeWrapper^ group )
			 * @brief creates peaks of specified spectrum group
			 * @param[in] group spectrum group
			 * @return peak object
			 */
			Peaks2DWrapper^ createPeaks( DataGroupNodeWrapper^ group );

			/**
			 * @fn bool hasPeaks( SpectrumWrapper^ spec )
			 * @brief judges whether peaks object of specified spectrum exists
			 * @param[in] spec spectrum
			 * @return If true, peaks object of specified spectrum exists.
			 */
			bool hasPeaks( SpectrumWrapper^ spec );

			/**
			 * @fn bool hasPeaks( ChromatogramWrapper^ chrom )
			 * @brief judges whether peaks object of specified chromatogram exists
			 * @param[in] chrom chromatogram
			 * @return If true, peaks object of specified chromatogtram exists.
			 */
			bool hasPeaks( ChromatogramWrapper^ chrom );

			/**
			 * @fn bool hasPeaks( DataGroupNodeWrapper^ group )
			 * @brief judges whether peaks object of specified spectrum group exists
			 * @param[in] group spectrum group
			 * @return If true, peaks object of specified spectrum group exists.
			 */
			bool hasPeaks( DataGroupNodeWrapper^ group );

			/**
			 * @fn void deletePeaks( SpectrumWrapper^ spec )
			 * @brief deletes peaks of specified spectrum
			 * @param[in] spec spectrum
			 */
			void deletePeaks( SpectrumWrapper^ spec );

			/**
			 * @fn void deletePeaks( ChromatogramWrapper^ chrom )
			 * @brief deletes peaks of specified chromatogram
			 * @param[in] chrom chromatogram
			 */
			void deletePeaks( ChromatogramWrapper^ chrom );

			/**
			 * @fn void deletePeaks( DataGroupNodeWrapper^ group )
			 * @brief deletes peaks of specified spectrum group
			 * @param[in] group spectrum group
			 */
			void deletePeaks( DataGroupNodeWrapper^ group );

			/**
			 * @fn void updatePeaks( SpectrumWrapper^ spec, PeaksWrapper^ peaks, cli::array< PeakElementWrapper^ >^ addPeaks )
			 * @brief update peaks
			 * @param[in] spec spectrum object
			 * @param[in] peaks peaks object
			 * @param[in] addPeaks peak element object to add
			 */
			void updatePeaks( SpectrumWrapper^ spec, PeaksWrapper^ peaks, cli::array< PeakElementWrapper^ >^ addPeaks );

		public:
			/**
			 * @fn static PeaksManagerWrapper^ getInstance()
			 * @brief get peaks manager object (This is the only object.)
			 * @return peaks manager object
			 */
			static PeaksManagerWrapper^ getInstance();
		};
	}
}

#endif		// __KOME_CLR_PEAKS_MANAGER_WRAPPER_H__

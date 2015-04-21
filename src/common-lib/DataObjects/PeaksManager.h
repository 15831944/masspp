/**
 * @file PeaksManager.h
 * @brief interfaces of PeaksManager class
 *
 * @author S.Tanaka
 * @date 2007.12.25
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_OBJECTS_PEAKS_MANAGER_H__
#define __KOME_OBJECTS_PEAKS_MANAGER_H__


#include "DefaultDataManager.h"

#include "Peaks.h"
#include "Peaks2D.h"
#include "PeakElement.h"


#include <map>
#include <vector>


namespace kome {

	namespace objects {

		class Sample;
		class Spectrum;
		class Chromatogram;
		class DataGroupNode;
		class DataMapInfo;

		/**
		 * @class PeaksManager
		 * @brief peaks management class
		 */
		class DATA_OBJECTS_CLASS PeaksManager : public DefaultDataManager {
		protected:
			/**
			 * @fn PeaksManager()
			 * @brief constructor
			 */
			PeaksManager();

			/**
			 * @fn virtual ~PeaksManager()
			 * @brief destructor
			 */
			virtual ~PeaksManager();

		protected:
			/** spectrum peaks */
			std::map< Spectrum*, Peaks > m_specPeaksMap;

			/** chromatogram peaks */
			std::map< Chromatogram*, Peaks > m_chromPeaksMap;

			/** 2D peaks */
			std::map< DataGroupNode*, Peaks2D > m_2dPeaksMap;

		public:
			/**
			 * @fn Peaks* getPeaks( Spectrum* spec )
			 * @brief gets peaks of specified spectrum
			 * @param[in] spec spectrum
			 * @return peaks object (If NULL, peaks object is not created yet.)
			 */
			Peaks* getPeaks( Spectrum* spec );

			/**
			 * @fn Peaks* getPeaks( Chromatogram* chrom )
			 * @brief gets peaks of specified chromatogram
			 * @param[in] chrom chromatogram
			 * @return peaks object (If NULL, peaks object is not created yet.)
			 */
			Peaks* getPeaks( Chromatogram* chrom );

			/**
			 * @fn Peaks2D* getPeaks( DataGroupNode* group )
			 * @brief gets peak of specified spectrum group
			 * @param[in] group spectrum group
			 * @return peaks object (If NULL, peaks object is not created yet.)
			 */
			Peaks2D* getPeaks( DataGroupNode* group );

			/**
			 * @fn Peaks* createPeaks( Spectrum* spec )
			 * @brief creates peaks of specified spectrum
			 * @param[in] spec spectrum
			 * @return peaks object
			 */
			Peaks* createPeaks( Spectrum* spec );

			/**
			 * @fn Peaks* createPeaks( Chromatogram* chrom )
			 * @brief creates peaks of specified chromatogram
			 * @param[in] chrom chromatogram
			 * @return peaks object
			 */
			Peaks* createPeaks( Chromatogram* chrom );

			/**
			 * @fn Peaks2D* createPeaks( DataGroupNode* group )
			 * @brief creates peaks of specified spectrum group
			 * @param[in] group spectrum group
			 * @return peak object
			 */
			Peaks2D* createPeaks( DataGroupNode* group );

			/**
			 * @fn bool hasPeaks( Spectrum* spec )
			 * @brief judges whether peaks object of specified spectrum exists
			 * @param[in] spec spectrum
			 * @return If true, peaks object of specified spectrum exists.
			 */
			bool hasPeaks( Spectrum* spec );

			/**
			 * @fn bool hasPeaks( Chromatogram* chrom )
			 * @brief judges whether peaks object of specified chromatogram exists
			 * @param[in] chrom chromatogram
			 * @return If true, peaks object of specified chromatogtram exists.
			 */
			bool hasPeaks( Chromatogram* chrom );

			/**
			 * @fn bool hasPeaks( DataGroupNode* group )
			 * @brief judges whether peaks object of specified spectrum group exists
			 * @param[in] group spectrum group
			 * @return If true, peaks object of specified spectrum group exists.
			 */
			bool hasPeaks( DataGroupNode* group );

			/**
			 * @fn void deletePeaks( Spectrum* spec )
			 * @brief deletes peaks of specified spectrum
			 * @param[in] spec spectrum
			 */
			void deletePeaks( Spectrum* spec );

			/**
			 * @fn void deletePeaks( Chromatogram* chrom )
			 * @brief deletes peaks of specified chromatogram
			 * @param[in] chrom chromatogram
			 */
			void deletePeaks( Chromatogram* chrom );

			/**
			 * @fn void deletePeaks( DataGroupNode* group )
			 * @brief deletes peaks of specified spectrum group
			 * @param[in] group spectrum group
			 */
			void deletePeaks( DataGroupNode* group );

			 /**
			  * @fn void updatePeaks( Spectrum& spec, Peaks* peaks, std::vector<PeakElement>& addPeaks )
			  * @brief update peaks
			  * @param[in] spec spectrum object
			  * @param[in] peaks peaks object
			  * @param[in] addPeaks peak element object to add
			  */
			void updatePeaks( Spectrum& spec, Peaks* peaks, std::vector<PeakElement>& addPeaks );

		protected:
			/**
			 * @fn virtual void onCloseSample( Sample* sample, const bool deleting )
			 * @brief This method is called when a sample is closed. (override method)
			 * @param[in] sample sample object to be closed
			 * @param[in] deleting If true, the specified object is being deleted now.
			 */
			virtual void onCloseSample( Sample* sample, const bool deleting );

		public:
			/**
			 * @fn static PeaksManager& getInstance()
			 * @brief get peaks manager object (This is the only object.)
			 * @return peaks manager object
			 */
			static PeaksManager& getInstance();
		};
	}
}


#endif		// __KOME_OBJECTS_PEAKS_MANAGER_H__

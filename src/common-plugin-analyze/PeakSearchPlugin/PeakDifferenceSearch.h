/**
 * @file PeakDifferenceSearch.h
 * @brief interfaces of PeakDifferenceSearch class
 *
 * @author S.Tanaka
 * @date 2008.10.17
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_PEAK_DIFFERENCE_SEARCH_H__
#define __KOME_PEAK_DIFFERENCE_SEARCH_H__



namespace kome {
	namespace search {

		/**
		 * @class PeakDifferenceSearch
		 * @brief peak difference search class
		 */
		class PeakDifferenceSearch : public kome::objects::DefaultDataManager {
		protected:
			/**
			 * @fn PeakDifferenceSearch()
			 * @brief constructor
			 */
			PeakDifferenceSearch();

			/**
			 * @fn virtual ~PeakDifferenceSearch()
			 * @brief destructor
			 */
			virtual ~PeakDifferenceSearch();

		protected:
			/** m/z difference */
			double m_mzDiff;
			/** m/z tolerance */
			double m_mzTol;
			/** min stage */
			int m_minStage;
			/** max stage */
			int m_maxStage;
			/** min RT */
			double m_minRt;
			/** max RT */
			double m_maxRt;
			/** min m/z */
			double m_minMz;
			/** max m/z */
			double m_maxMz;
			/** min intensity */
			double m_minInt;
			/** max intensity */
			double m_maxInt;

		protected:
			/**
			 * @struct PeakInfo
			 * @brief peak information
			 */
			struct PeakInfo {
				kome::objects::Spectrum* spec;
				double lowMz;
				double lowInt;
				double highMz;
				double highInt;
				double diff;
			};

			/** hit peaks */
			std::vector< PeakInfo > m_hitPeaks;

			/** search group */
			kome::objects::DataGroupNode* m_group;

			/** labeled spectra count */
			int m_lblSpecCnt;

			/** not labeled spectra count */
			int m_notLblSpecCnt;

		protected:
			/** dialog */
			kome::window::GridListDialog* m_peakDiffDlg;

		public:
			/**
			 * @fn void resetParameters()
			 * @brief resets parameters
			 */
			void resetParameters();

			/**
			 * @fn clearResult()
			 * @brief clears result
			 */
			void clearResult();

			/**
			 * @fn void reset()
			 * @brief resets
			 */
			void reset();

		public:
			/**
			 * @fn void setMzDifference( const double diff )
			 * @brief ses m/z difference
			 * @param diff m/z difference
			 */
			void setMzDifference( const double diff );

			/**
			 * @fn double getMzDifference()
			 * @brief gets m/z difference
			 * @return m/z difference
			 */
			double getMzDifference();

			/**
			 * @fn void setMzTolerance( const double tol )
			 * @brief sets m/z tolerance
			 * @param tol m/z tolerance
			 */
			void setMzTolerance( const double tol );

			/**
			 * @fn double getMzTolerance()
			 * @brief gets m/z tolerance
			 * @return m/z tolerance
			 */
			double getMzTolerance();

			/**
			 * @fn void setStageRange( const int minStage, const int maxStage )
			 * @brief sets stage range
			 * @param minStage minimum stage
			 * @param maxStage maximum stage
			 */
			void setStageRange( const int minStage, const int maxStage );

			/**
			 * @fn int getMinStage()
			 * @brief gets min stage
			 * @return minimum stage
			 */
			int getMinStage();

			/**
			 * @fn int getMaxStage()
			 * @brief gets maximum stage
			 * @return maximum stage
			 */
			int getMaxStage();

			/**
			 * @fn void setRtRange( const double minRt, const double maxRt )
			 * @brief sets RT range
			 * @param minRt minimum RT
			 * @param maxRt maximum RT
			 */
			void setRtRange( const double minRt, const double maxRt );

			/**
			 * @fn double getMinRt()
			 * @brief gets min RT
			 * @return min RT
			 */
			double getMinRt();

			/** 
			 * @fn double getMaxRt()
			 * @brief gets max RT
			 * @return max RT
			 */
			double getMaxRt();

			/**
			 * @fn void setMzRange( const double minMz, const double maxMz )
			 * @brief sets m/z range
			 * @param minMz min m/z
			 * @param maxMz max m/z
			 */
			void setMzRange( const double minMz, const double maxMz );

			/**
			 * @fn double getMinMz()
			 * @brief gets min m/z
			 * @return min m/z
			 */
			double getMinMz();

			/**
			 * @fn double getMaxMz()
			 * @brief gets max m/z
			 * @return max m/z
			 */
			double getMaxMz();

			/**
			 * @fn void setIntRange( const double minInt, const double maxInt )
			 * @brief sets intensity range
			 * @param minInt min intensity
			 * @param maxInt max intensity
			 */
			void setIntRange( const double minInt, const double maxInt );

			/**
			 * @fn double getMinIntensity()
			 * @brief gets min intensity
			 * @return min intensity
			 */
			double getMinIntensity();

			/**
			 * @fn double getMaxIntensity()
			 * @brief gets max intensity
			 * @return max intensity
			 */
			double getMaxIntensity();

		public:
			/**
			 * @fn unsigned int getNumberOfPeaks()
			 * @brief gets the number of hit peaks
			 * @return the number of hit peaks
			 */
			unsigned int getNumberOfPeaks();

			/**
			 * @fn kome::objects::Spectrum* getPeakSpectrum( const unsigned int index )
			 * @brief gets peak spectrum
			 * @param index peak index
			 * @return peak spectrum
			 */
			kome::objects::Spectrum* getPeakSpectrum( const unsigned int index );

			/**
			 * @fn double getPeakRt( const unsigned int index )
			 * @brief gets peak RT
			 * @param index peak index
			 * @return peak RT
			 */
			double getPeakRt( const unsigned int index );

			/**
			 * @fn double getLowPeakMz( const unsigned int index )
			 * @brief gets low peak m/z
			 * @param index peak index
			 * @return low peak m/z
			 */
			double getLowPeakMz( const unsigned int index );

			/**
			 * @fn double getLowPeakIntensity( const unsigned int index )
			 * @brief gets low peak intensity
			 * @param index peak index
			 * @return low peak intensity
			 */
			double getLowPeakIntensity( const unsigned int index );

			/**
			 * @fn double getHighPeakMz( const unsigned int index )
			 * @brief gets high peak m/z
			 * @param index peak index
			 * @return high peak m/z
			 */
			double getHighPeakMz( const unsigned int index );

			/**
			 * @fn double getHighPeakIntensity( const unsigned int index )
			 * @brief gets high peak intensity
			 * @param index peak index
			 * @return high peak intensity
			 */
			double getHighPeakIntensity( const unsigned int index );

			/**
			 * @fn double getPeakDiff( const unsigned int index )
			 * @brief gets peak difference
			 * @param index peak index
			 * @return peak difference
			 */
			double getPeakDiff( const unsigned int index );

			/**
			 * @fn kome::objects::DataGroupNode* getTargetGroup()
			 * @brief gets target group
			 * @return target group
			 */
			kome::objects::DataGroupNode* getTargetGroup();

			/**
			 * @fn int getNumberOfLabeledSpectra()
			 * @brief gets the number of labeled spectra
			 * @return the number of labeled spectra
			 */
			int getNumberOfLabeledSpectra();

			/**
			 * @fn int getNumberOfNotLabeledSpectra()
			 * @brief gets the number of not labeled spectra
			 * @return the number of not labeled spectra
			 */
			int getNumberOfNotLabeledSpectra();

		public:
			/**
			 * @fn bool search(
						kome::objects::DataGroupNode& group,
						kome::core::Progress* progress
					)
			 * @brief search peaks
			 * @param group group
			 * @param progress the object to show progress
			 * @param If true, it succeeded to search peaks.
			 */
			bool search(
				kome::objects::DataGroupNode& group,
				kome::core::Progress* progress
			);

		public:
			/**
			 * @fn void releaseDialog()
			 * @brief releases matrix dialog
			 */
			void releaseDialog();

			/**
			 * @fn void togglenDialog()
			 * @brief toggle visibility of the matrix dialog
			 */
			void toggleDialog();

			/**
			 * @fn bool isShownDialog()
			 * @brief judges the matirx dialog is opened
			 * @return If true, the matrix dialog is opened.
			 */
			bool isShownDialog();

			/**
			 * @fn void updateDialog()
			 * @brief updates dialog
			 */
			void updateDialog();

			/**
			 * @fn void closeDialog()
			 * @brief close dialog
			 */
			void closeDialog();

		protected:
			/**
			 * @fn virtual void onCloseSample( kome::objects::Sample* sample, const bool deleting )
			 * @brief This method is called when a sample is closed. (override method)
			 * @param[in] sample sample object to be closed
			 * @param[in] deleting If true, the specified object is being deleted now.
			 */
			virtual void onCloseSample( kome::objects::Sample* sample, const bool deleting );

		public:
			/**
			 * @fn static PeakDifferenceSearch& getInstance()
			 * @brief gets instance
			 * @return PeakDifferenceSearch object (This is the only object.)
			 */
			static PeakDifferenceSearch& getInstance();
		};
	}
}


#endif	// __KOME_PEAK_DIFFERENCE_SEARCH_H__

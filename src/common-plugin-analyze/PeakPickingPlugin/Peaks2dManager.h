/**
 * @file Peaks2dManager.h
 * @brief interfaces of Peaks2dManager class
 *
 * @author S.Tanaka
 * @date 2007.10.22
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_LABELING_PEAKS_2D_MANAGER_H__
#define __KOME_LABELING_PEAKS_2D_MANAGER_H__


namespace kome {
	namespace labeling {

		class PeaksListCtrl;

		/**
		 * @class Peaks2dManager
		 * @brief peak picking management class
		 */
		class Peaks2dManager : public kome::objects::DefaultDataManager {
		protected:
			/**
			 * @fn Peaks2dManager()
			 * @brief constructor
			 */
			Peaks2dManager();

			/**
			 * @fn virtual ~Peaks2dManager()
			 * @brief destructor
			 */
			virtual ~Peaks2dManager();

		protected:
			/** peaks list control */
			PeaksListCtrl* m_listCtrl;

			/** spectrum group */
			kome::objects::DataGroupNode* m_group;

			/** selected peaks */
			std::set< kome::objects::Peak2DElement* > m_selectedPeaks;

			/** selected clusters */
			std::set< kome::objects::PeaksCluster2D* > m_selectedClusters;

		public:
			/**
			 * @fn void detectPeaks(
					kome::objects::DataGroupNode& group,
					kome::objects::DataSet& dataSet,
					kome::objects::Peaks2D& peaks,
					kome::plugin::PluginFunctionItem* peakDetector,
					kome::objects::SettingParameterValues* peakSettings,
					kome::core::Progress& progress,
					const double startRt = -1.0,
					const double endRt = -1.0,
					const double startMz = -1.0,
					const double endMz = -1.0
				)
			 * @brief detects peaks
			 * @param group spectrum group
			 * @param spectra spectra
			 * @param peaks peak object to store peaks information
			 * @param peakDetector peak detector
			 * @param peakSettings peak settings
			 * @param progress progress object to show progress
			 * @param startRt start RT
			 * @param endRt end RT
			 * @param startMz start m/z
			 * @param endMz end m/z
			 */
			void detectPeaks(
				kome::objects::DataGroupNode& group,
				kome::objects::DataSet& dataSet,
				kome::objects::Peaks2D& peaks,
				kome::plugin::PluginFunctionItem* peakDetector,
				kome::objects::SettingParameterValues* peakSettings,
				kome::core::Progress& progress,
				const double startRt = -1.0,
				const double endRt = -1.0,
				const double startMz = -1.0,
				const double endMz = -1.0
			);

		public:
			/**
			 * @fn void createPeaksView()
			 * @brief creates peaks view
			 */
			void createPeaksView();

			/**
			 * @fn void releaseView()
			 * @brief releases view
			 */
			void releaseView();

			/**
			 * @fn void toggleVisibilityOfPeakListView()
			 * @brief toggles visibility of the peak list view
			 */
			void toggleVisibilityOfPeakListView();

			/**
			 * @fn bool isVisiblePeakListView()
			 * @brief checks the peak list view is visible or not
			 * @return If true, the peak list view is visible.
			 */
			bool isVisiblePeakListView();

			/**
			 * @fn void updatePeakListView( kome::objects::DataGroupNode* group )
			 * @brief updates peaks view
			 * @param group updated spectrum group
			 */
			void updatePeakListView( kome::objects::DataGroupNode* group );

		public:
			/**
			 * @fn void selectGroup( kome::objects::DataGroupNode* group )
			 * @brief selects spectrum group
			 * @param group spectrum group
			 */
			void selectGroup( kome::objects::DataGroupNode* group );

			/**
			 * @fn kome::objects::DataGroupNode* getSelectedGroup()
			 * @brief gets selected group
			 * @return selected group
			 */
			kome::objects::DataGroupNode* getSelectedGroup();

		public:
			/**
			 * @fn void clearSelectedPeaks()
			 * @brief clears selected peaks
			 */
			void clearSelectedPeaks();

			/**
			 * @fn void addSelectedPeaks( kome::objects::Peak2DElement* peak )
			 * @brief adds selected peaks
			 * @param peak adds selected peak
			 */
			void addSelectedPeaks( kome::objects::Peak2DElement* peak );

			/**
			 * @fn void drawPeaks(
					kome::objects::DataMapInfo& dataMap,
					kome::img::Graphics& g
				)
			 * @brief draws peaks
			 * @param dataMap data map object
			 * @param g graphics object to draw
			 */
			void drawPeaks(
				kome::objects::DataMapInfo& dataMap,
				kome::img::Graphics& g
			);

			/**
			 * @fn void drawClusters(
					kome::objects::DataMapInfo& dataMap,
					kome::img::Graphics& g
				)
			 * @brief draws peaks
			 * @param dataMap data map object
			 * @param g graphics object to draw
			 */
			void drawClusters(
				kome::objects::DataMapInfo& dataMap,
				kome::img::Graphics& g
			);

		public:
			/**
			 * @fn bool savePeaks(
						kome::objects::Peaks2D& peaks,
						const char* filePath,
						kome::core::Progress& progress
					)
			 * @brief saves peaks
			 * @param peaks peaks object
			 * @param filePath output file path
			 * @param progress the object to show progress
			 * @return If true, it succeeded to save file.
			 */
			bool savePeaks(
				kome::objects::Peaks2D& peaks,
				const char* filePath,
				kome::core::Progress& progress
			);

		protected:
			 /* @fn void drawPosition(
					kome::img::Graphics& g,
					const double startRt,
					const double endRt,
					const double startMz,
					const double endMz
				)
			 * @brief draws position
			 * @param startRt start RT
			 * @param endRt end RT
			 * @param startMz start m/z
			 * @param endMz end m/z
			 */
			void drawPosition(
				kome::img::Graphics& g,
				const double startRt,
				const double endRt,
				const double startMz,
				const double endMz
			);

			/**
			 * @fn virtual void onCloseSample( kome::objects::Sample* sample, const bool deleting )
			 * @brief This method is called when a sample is closed. (override method)
			 * @param[in] sample sample object to be closed
			 * @param[in] deleting If true, the specified object is being deleted now.
			 */
			virtual void onCloseSample( kome::objects::Sample* sample, const bool deleting );

		public:
			/**
			 * @fn static Peaks2dManager& getInstance()
			 * @brief gets Peaks2dManager object
			 * @return Peaks2dManager object. (This is the only object.)
			 */
			static Peaks2dManager& getInstance();
		};
	}
}

#endif	// __KOME_LABELING_PEAKS_2D_MANAGER_H__

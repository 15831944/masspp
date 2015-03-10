/**
 * @file XYDataViewManager.h
 * @brief interfaces of XYDataViewManager class
 *
 * @author S.Tanaka
 * @date 2007.11.12
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */

#ifndef __KOME_VIEW_XY_DATA_VIEW_MANAGER_H__
#define __KOME_VIEW_XY_DATA_VIEW_MANAGER_H__

namespace kome {
	namespace view {

		class XYData2DListCtrl;	// @date 2011/05/25 <Add> OKADA

		/**
		 * @class XYDataViewManager
		 * @brief spectra view management class
		 */
		class XYDataViewManager : public kome::objects::DefaultDataManager {
		protected:
			/**
			 * @fn XYDataViewManager()
			 * @brief constructor
			 */
			XYDataViewManager();

			/**
			 * @fn virtual ~XYDataViewManager()
			 * @brief destructor
			 */
			virtual ~XYDataViewManager();

		protected:
			/** data grid view */
			XYData2DListCtrl* m_dataXYData;	// @date 2011/05/25 <Add> OKADA
			
			/** peaks grid view */
			XYData2DListCtrl* m_peaksXYData;	// @date 2011/05/25 <Add> OKADA

			/** current sample */
			kome::objects::Spectrum* m_currentSpec;

			/** current sample */
			kome::objects::Chromatogram* m_currentChrom;
		protected:
			/** spectrum group */
			kome::objects::DataGroupNode* m_group;

			/** selected peaks */
			std::set< kome::objects::Peak2DElement* > m_selectedPeaks;

			/** selected clusters */
			std::set< kome::objects::PeaksCluster2D* > m_selectedClusters;

		public:
			/**
			 * @fn kome::objects::Sample* getCurrentSample()
			 * @brief gets current sample
			 * @return current sample
			 */
			kome::objects::Sample* getCurrentSample();

			/**
			 * @fn kome::objects::Spectrum* getCurrentSpectrum()
			 * @brief gets current spectrum
			 * @return current spectrum
			 */
			kome::objects::Spectrum* getCurrentSpectrum();

			/**
			 * @fn kome::objects::Chromatogram* getCurrentChromatogram()
			 * @brief gets current chromatogram
			 * @return current chromatoram
			 */
			kome::objects::Chromatogram* getCurrentChromatogram();

		public:
			/**
			 * @fn void createPointsView()
			 * @brief creates data grid view
			 */
			void createPointsView();

			/**
			 * @fn void createPeaksView()
			 * @brief creates peaks grid view
			 */
			void createPeaksView();
		
			/**
			 * @fn void selectSpectrum( kome::objects::Spectrum* spec )
			 * @brief selects spectrum
			 * @param spec selected spectrum
			 */
			void selectSpectrum( kome::objects::Spectrum* spec );

			/**
			 * @fn void selectChromatogram( kome::objects::Chromatogram* chrom )
			 * @brief selects chromatogram
			 * @param chrom selected chromatogram
			 */
			void selectChromatogram( kome::objects::Chromatogram* chrom );

			/**
			 * @fn void toggleDataVisibility()
			 * @brief toggles the visibility of the data grid view
			 */
			void toggleDataVisibility();

			/**
			 * @fn void togglePeaksVisibility()
			 * @brief toggles the visibility of the peaks grid view
			 */
			void togglePeaksVisibility();

			/**
			 * @fn bool isVisiblePointsXYPointsView()
			 * @brief checks the visibility of the data grid view
			 * @return If true, the data grid view is visible.
			 */
			bool isVisiblePointsXYPointsView();

			/**
			 * @fn bool isVisiblePeaksXYPointsView()
			 * @brief checks the visibility of the peaks grid view
			 * @return If true, the peaks grid view is visible.
			 */
			bool isVisiblePeaksXYPointsView();

			/**
			 * @fn void updateSpectrumXYPointsView( kome::objects::Spectrum* spec );
			 * @brief updates spectrum grid view
			 * @param spec updated spectrum
			 */
			void updateSpectrumXYPointsView( kome::objects::Spectrum* spec );

			/**
			 * @fn void updateChromatogramXYPointsView( kome::objects::Chromatogram* chrom )
			 * @brief updates chromatogram grid view
			 * @param chrom updated chromatogram
			 */
			void updateChromatogramXYPointsView( kome::objects::Chromatogram* chrom );			

			/**
			 * @fn void releaseView()
			 * @brief releases view
			 */
			void releaseView();

		protected:
			/**
			 * @fn virtual void onCloseSample( kome::objects::Sample* sample, const bool deleting )
			 * @brief This method is called when a sample is closed. (override method)
			 * @param sample sample object to be closed
			 * @param[in] deleting If true, the specified object is being deleted now.
			 */
			virtual void onCloseSample( kome::objects::Sample* sample, const bool deleting );

			/**
			 * @fn virtual void onCloseSpectrum( kome::objects::Spectrum* spec, const bool deleting )
			 * @brief This method is called when a spectrum is closed. (override method)
			 * @param spec spectrum object to be opened
			 * @param[in] deleting If true, the specified object is being deleted now.
			 */
			virtual void onCloseSpectrum( kome::objects::Spectrum* spec, const bool deleting );

			/**
			 * @fn virtual void onCloseChromatogram( kome::objects::Chromatogram* chrom, const bool deleting )
			 * @brief This method is called when a chromatogram is closed. (override method)
			 * @param chrom chromatogram object to be closed
			 * @param[in] deleting If true, the specified object is being deleted now.
			 */
			virtual void onCloseChromatogram( kome::objects::Chromatogram* chrom, const bool deleting );

		public:
			/**
			 * @fn static XYDataViewManager& getInstance()
			 * @brief gets spectra view management object
			 * @return spectra view management object (This is the only object.)
			 */
			static XYDataViewManager& getInstance();

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
			 * @fn void createPeaksView2D()
			 * @brief creates peaks view
			 */
			void createPeaksView2D();

			/**
			 * @fn void selectGroup( kome::objects::DataGroupNode* group )
			 * @brief selects spectrum group
			 * @param group spectrum group
			 */
			void selectGroup( kome::objects::DataGroupNode* group );

			/**
			 * @fn void updatePeakListView( kome::objects::DataGroupNode* group )
			 * @brief updates peaks view
			 * @param group updated spectrum group
			 */
			void updatePeakListView( kome::objects::DataGroupNode* group );

		protected:
			/**
			 * @fn virtual void onCloseDataMap( DataMapInfo* dataMap );
			 * @brief This method is called when a sample is closed. (override method)
			 * @param dataMap dataMap object to be closed
			 */
			virtual void onCloseDataMap( kome::objects::DataMapInfo* dataMap );

			/** frame */
			kome::window::MainFrame* m_mainFrameUnite;	// @date 2011/05/16 >Add> OKADA

		public:
			/**
			 * @fn void SetPeaksVisibility( int iFlgShowHide );
			 * @brief This method is set Peaks Visibility
			 * @param iFlgShowHide 0:hide 1:show -1:not change
			 */
			void SetPeaksVisibility( int iFlgShowHide );

			/**
			 * @fn void SetVisibilityOfPeakListView( int iFlgShowHide );
			 * @brief This method is set Visibility of PeakListView
			 * @param iFlgShowHide 0:hide 1:show -1:not change
			 */
			void SetVisibilityOfPeakListView( int iFlgShowHide );

			/**
			 * @fn void togglePeakVisibility()
			 * @brief toggles the visibility of the peak grid view
			 * @param kome::objects::Parameters* para parameter
			 */
			void XYDataViewManager::togglePeakVisibility( kome::objects::Parameters* param );

			/**
			 * @fn bool isVisibleXY2DPeaksView()
			 * @brief checks the peak list view is visible or not
			 * @return If true, the peak list view is visible.
			 */
			bool XYDataViewManager::isVisibleXY2DPeaksView();
	  
	   // Peaks2dManagerÇ©ÇÁà⁄êA	@date 2011.09.16 <Add> M.Izumi ->
 	   public:
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

		 protected:
			/**
			 * @fn void drawPosition(
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
		// @date 2011.09.16 <Add> M.Izumi <-
	
		};
	}
}


#endif		// __KOME_VIEW_XY_DATA_VIEW_MANAGER_H__

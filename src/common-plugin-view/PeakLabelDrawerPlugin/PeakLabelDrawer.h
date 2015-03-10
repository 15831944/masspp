/**
 * @file PeakLabelDrawer.h
 * @brief interfaces of PeakLabelDrawer class
 *
 * @author S.Tanaka
 * @date 2007.03.01
 * 
 * Copyright(C) 2006-2014 Eisai Co., Ltd. All rights reserved.
 */


#ifndef __KOME_VIEW_PEAK_LABEL_DRAWER_H__
#define __KOME_VIEW_PEAK_LABEL_DRAWER_H__


#include <list>
#include <string>

namespace kome {
	namespace view {
		/**
		 * @class PeakLabelDrawer
		 * @brief class to draw profile
		 */
		class PeakLabelDrawer : public kome::objects::DefaultDataManager {
		protected:
			/**
			 * @fn PeakLabelDrawer()
			 * @brief constructor
			 */
			PeakLabelDrawer();

			/**
			 * @fn virtual ~PeakLabelDrawer()
			 * @brief destructor
			 */
			virtual ~PeakLabelDrawer();

		protected:
			/** font */
			kome::img::Font m_font;

			/** spectrum drawing data map */
			std::map< kome::objects::Spectrum*, kome::img::DrawingXYData* > m_specDrawingMap;

			/** chromatogram drawing data map */
			std::map< kome::objects::Chromatogram*, kome::img::DrawingXYData* > m_chromDrawingMap;
			
			/** changing data */
			struct {				
				bool leftFlg;				
				bool draggingFlg;
				int  endPos;
				wxWindow* canvas;
			} m_changing;
			
			/** 閾値(spectrum) */
			double m_specThreshold;

			/** 閾値(chromatogram) */
			double m_chromThreshold;
			
			/** y軸で閾値設定を行うか判定 */
			bool m_bflg;

			/** select peak label */
			double m_selPeakLabelSpec;

			/** select peak label */
			double m_selPeakLabelChrom;
			
			/* Rect List Map */
			std::map< kome::objects::Spectrum* , std::list< std::pair< kome::core::Rect<int>, double > > > m_specRectListMap;
			
			/* Rect List Map */
			std::map< kome::objects::Chromatogram* , std::list< std::pair< kome::core::Rect<int>, double > > > m_chromRectListMap;
			
			/* Rect List */
			std::list< std::pair< kome::core::Rect< int >, double > >m_rectList;
			
		public:
			/**
			 * @fn kome::img::DrawingXYData* getDrawingData( kome::objects::Spectrum* spec )
			 * @brief gets drawing data object to draw label
			 * @param spec spectrum
			 * @return drawing data object
			 */
			kome::img::DrawingXYData* getDrawingData( kome::objects::Spectrum* spec );

			/**
			 * @fn kome::img::DrawingXYData* getDrawingData( kome::objects::Chromatogram* chrom )
			 * @brief gets drawing data object to draw label
			 * @param chrom chromatogram
			 * @return drawing data object
			 */
			kome::img::DrawingXYData* getDrawingData( kome::objects::Chromatogram* chrom );

			/**
			 * @fn void drawLabel(
					kome::objects::Spectrum* spec,
					kome::img::Graphics* g
				)
			 * @brief draws label
			 * @param spec spectrum
			 * @param g graphics object
			 */
			void drawLabel(
				kome::objects::Spectrum* spec,
				kome::img::Graphics* g
			);

			/**
			 * @fn void drawLabel(
					kome::objects::Chromatogram* chrom,
					kome::img::Graphics* g
				)
			 * @brief draws label
			 * @param chrom chromatogram
			 * @param g graphics object
			 */
			void drawLabel(
				kome::objects::Chromatogram* chrom,
				kome::img::Graphics* g
			);

			/**
			 * @fn void drawLabel(
					kome::img::DrawingXYData* drawing,
					const int accuracy,
					kome::objects::Peaks* peaks,
					kome::core::XYData* xyData,
					kome::img::Graphics* g,
					bool bchromflg
				)
			 * @brief draws label
			 * @param drawing drawing data
			 * @param accuracy accuracy
			 * @param peaks peaks
			 * @param xyData data points
			 * @param g graphics object
			 * @param bchromflg true:chromatogaram / false:spectrum
			 */
			void drawLabel(
				kome::img::DrawingXYData* drawing,
				const int accuracy,
				kome::objects::Peaks* peaks,
				kome::core::XYData* xyData,
				kome::img::Graphics* g,
				bool bchromflg			 // @date 2011.03.29 <Add> M.Izumi
			);

			/**
			 * @fn void prepareDraw(
					kome::objects::Spectrum* spec,
					kome::img::Graphics* g
				)
			 * @brief prepare to draw
			 * @param spec spectrum
			 * @param g graphics object
			 */
			void prepareDraw(
				kome::objects::Spectrum* spec,
				kome::img::Graphics* g
			);

			/**
			 * @fn void prepareDraw(
					kome::objects::Chromatogram* chrom,
					kome::img::Graphics* g
				)
			 * @brief prepare to draw
			 * @param chrom chromatogram
			 * @param g graphics object
			 */
			void prepareDraw(
				kome::objects::Chromatogram* chrom,
				kome::img::Graphics* g
			);

			/**
			 * @fn void deleteDrawData( kome::objects::Spectrum* spec )
			 * @brief deletes drawing data of specified spectrum
			 * @param spec spectrum
			 */
			void deleteDrawData( kome::objects::Spectrum* spec );

			/**
			 * @fn void deleteDrawData( kome::objects::Chromatogram* chrom )
			 * @brief deletes drawing data of specified chromatogram
			 * @param chrom chromatogram
			 */
			void deleteDrawData( kome::objects::Chromatogram* chrom );

			/**
			 * @fn void deleteRectListMap( kome::objects::Spectrum* spec )
			 * @brief delete rect list map 
			 * @param spec spectrum objects
			 */
			void deleteRectListMap( kome::objects::Spectrum* spec );

			/**
			 * @fn void deleteRectListMap( kome::objects::Chromatogram* chrom )
			 * @brief delete rect list map
			 * @param chrom chromatogram objects
			 */
			void deleteRectListMap( kome::objects::Chromatogram* chrom );
		protected:
			/**
			 * @fn virtual void onCloseSample( kome::objects::Sample* sample, const bool deleting  )
			 * @brief This method is called when a sample is closed. (override method)
			 * @param sample sample object to be closed
			 * @param[in] deleting If true, the specified object is being deleted now.
			 */
			virtual void onCloseSample( kome::objects::Sample* sample, const bool deleting  );

			/**
			 * @fn virtual void onCloseSpectrum( kome::objects::Spectrum* spec, const bool deleting  )
			 * @brief This method is called when a spectrum is closed. (override method)
			 * @param spec spectrum object to be opened
			 * @param[in] deleting If true, the specified object is being deleted now.
			 */
			virtual void onCloseSpectrum( kome::objects::Spectrum* spec, const bool deleting  );

			/**
			 * @fn virtual void onCloseChromatogram( kome::objects::Chromatogram* chrom, const bool deleting  )
			 * @brief This method is called when a chromatogram is closed. (override method)
			 * @param chrom chromatogram object to be closed
			 * @param[in] deleting If true, the specified object is being deleted now.
			 */
			virtual void onCloseChromatogram( kome::objects::Chromatogram* chrom, const bool deleting  );

		protected:
			/**
			 * @fn static bool lessPeak( std::pair< double, double > p0, std::pair< double, double > p1 )
			 * @brief compares to sort peak data
			 * @param p0 peak data to be compared
			 * @param p1 peak data to compare
			 * @return If true, p0 sorts before p1.
			 */
			static bool lessPeak( std::pair< double, double > p0, std::pair< double, double > p1 );

			/**
			 * @fn static int getAccuracy( const char* key )
			 * @brief gets accuracy
			 * @param key parameter key
			 * @return accuracy
			 */
			static int getAccuracy( const char* key );
			
			/** 
			 * @fn static std::string getLabelType( const char* key )
			 * @brief gets label type
			 * @param kye parameter key
			 * @return label type
			 */
			static std::string getLabelType( const char* key );

			// @date 2011.03.29 <Add> M.Izumi ->
		public:
			/**
			 * @fn bool downButton(
					kome::objects::Spectrum& spec,
					kome::evt::MouseEvent& evt,
					kome::img::Graphics& graphics,
					wxWindow* canvas
				)
			 * @brief button down event　(spectrum)
			 * @param spec spectrum object
			 * @param evt mouse event information object
			 * @param graphics graphics object
			 * @param canvas canvas ID
			 * @return refresh flag
			 */
			bool downButton(
					kome::objects::Spectrum& spec,
					kome::evt::MouseEvent& evt,
					kome::img::Graphics& graphics,
					wxWindow* canvas
			);

			/**
			 * @fn bool downButton(
					kome::objects::Chromatogram& chrom,
					kome::evt::MouseEvent& evt,
					kome::img::Graphics& graphics,
					wxWindow* canvas
				)
			 * @brief button down event (chromatogram)
			 * @param chrom chromatogram object
			 * @param evt mouse event information object
			 * @param graphics graphics object
			 * @param canvas canvas ID
			 * @return refresh flag
			 */
			bool downButton(
					kome::objects::Chromatogram& chrom,
					kome::evt::MouseEvent& evt,
					kome::img::Graphics& graphics,
					wxWindow* canvas
			);
			
			/**
			 * @fn bool moveMouse(
					kome::objects::Spectrum& spec,
					kome::evt::MouseEvent& evt,
					kome::img::Graphics& graphics,
					wxWindow* canvas 
				)
			 * @brief mouse move event (spectrum)
			 * @param spec spectrum object
			 * @param evt mouse event information object
			 * @param graphics graphics object
			 * @return refresh flag
			 */
			bool moveMouse(
					kome::objects::Spectrum& spec,
					kome::evt::MouseEvent& evt,
					kome::img::Graphics& graphics,
					wxWindow* canvas 
			);

			/**
			 * @fn bool moveMouse(
					kome::objects::Chromatogram& chrom,
					kome::evt::MouseEvent& evt,
					kome::img::Graphics& graphics,
					wxWindow* canvas
				)
			 * @brief mouse move event (chromatogram)
			 * @param chrom chromatogram object
			 * @param evt mouse event information object
			 * @param graphics graphics object
			 * @param canvas canvas ID
			 * @return refresh flag
			 */
			bool moveMouse(
					kome::objects::Chromatogram& chrom,
					kome::evt::MouseEvent& evt,
					kome::img::Graphics& graphics,
					wxWindow* canvas
			);
			
			/**
			 * @fn bool upButton(
					kome::evt::MouseEvent& evt,
					kome::img::Graphics& graphics,
					bool bchromflg
				)
			 * @brief button up event
			 * @param evt mouse event information object
			 * @param graphics graphics object
			 * @param bchromflg true:chromatogaram / false:spectrum 
			 */					
			bool upButton(
					kome::evt::MouseEvent& evt,
					kome::img::Graphics& graphics,
					bool bchromflg
			);
	
			/**
			 * @fn bool dblClick( 
					kome::evt::MouseEvent& evt,
					kome::img::Graphics& graphics,
					bool bchromflg
				)
			 * @brief double click event
			 * @param evt mouse event information object
			 * @param graphics graphics object
			 * @param bchromflg true:chromatogaram / false:spectrum 
			 */
			bool dblClick( 
					kome::evt::MouseEvent& evt,
					kome::img::Graphics& graphics,
					bool bchromflg
			);
			
			/**
			 * @fn void drawForeground(
					kome::objects::Spectrum& spec,
					kome::img::Graphics& graphics, 
					wxWindow* canvas
				)
			 * @brief draws spectrum foreground
			 * @param spec spectrum object
			 * @param graphics graphics object
			 * @param canvas canvas ID
			 */
			void drawForeground(
					kome::objects::Spectrum& spec,
					kome::img::Graphics& graphics, 
					wxWindow* canvas
			);

			/**
			 * @fn void drawForeground(
					kome::objects::Chromatogram& chrom,
					kome::img::Graphics& graphics,
					wxWindow* canvas
				)
			 * @brief draws chromatogram foreground
			 * @param chrom chromatogram object
			 * @param graphics graphics object
			 * @param canvas canvas ID
			 */
			void drawForeground(
					kome::objects::Chromatogram& chrom,
					kome::img::Graphics& graphics,
					wxWindow* canvas
			);

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
		protected:	
			/**
			 * @fn bool setDownButtonInfo( 
					kome::evt::MouseEvent& evt,	
					kome::img::Graphics& graphics, 
					bool bchromflg
				)
			 * @brief sets button down information
			 * @param evt mouse event information object
			 * @param graphics graphics object
			 * @param bchromflg true:chromatogaram / false:spectrum 
			 * @return refresh flag
			 */
			bool setDownButtonInfo( 
					kome::evt::MouseEvent& evt,	
					kome::img::Graphics& graphics, 
					bool bchromflg,
					std::list< std::pair< kome::core::Rect<int>, double > >
			);

			/**
			 * @fn bool setMoveMouseInfo( 
					kome::evt::MouseEvent& evt,
					kome::img::Graphics& graphics,
					wxWindow* canvas
				)
			 * @brief sets mouse move information
			 * @param evt mouse event information object
			 * @param graphics graphics object
			 * @param canvas canvas ID
			 * @return refresh flag
			 */
			bool setMoveMouseInfo( 
					kome::evt::MouseEvent& evt, 
					kome::img::Graphics& graphics, 
					wxWindow* canvas
			);
			
			/**
			 * @fn bool setUpButtonInfo( 
					kome::evt::MouseEvent& evt, 
					kome::img::Graphics& graphics,
					bool bchromflg
				)
			 * @brief sets button up information
			 * @param evt mouse event information object
			 * @param graphics graphics object
			 * @param @param bchromflg true:chromatogaram / false:spectrum 
			 * @return refresh flag
			 */
			bool setUpButtonInfo(
					kome::evt::MouseEvent& evt,
					kome::img::Graphics& graphics,
					bool bchromflg 
			);			
			
			/**
			 * @fn void drawForeground( kome::img::Graphics& graphics, bool bchromflg )
			 * @brief draws foreground
			 * @param graphics graphics object
			 * @param bchromflg true:chromatogaram / false:spectrum 
			 */
			void drawForeground( kome::img::Graphics& graphics, bool bchromflg );

			/**
			 * @fn bool hasPeaks( kome::objects::Spectrum& spec )
			 * @brief has peaks
			 * @param spec spectrum object
			 * @return ピークの有無
			 */
			bool hasPeaks( kome::objects::Spectrum& spec );

			/**
			 * @fn bool hasPeaks( kome::objects::Chromatogram& chrom )
			 * @brief has peaks
			 * @param chrom chromatogram object
			 * @return ピークの有無
			 */
			bool hasPeaks( kome::objects::Chromatogram& chrom );
		
		private:
			/**
			 * @fn void setThreshold( 
					kome::img::Graphics& grapicsh,
					bool initialPos,
					bool& refresh, 
					bool bchromflg
				)
			 * @bfief 閾値セット
			 * @param graphics graphics object
			 * @param initialPos 初期位置フラグ
			 * @param refresh return refresh flag
			 * @param bchromflg true:chromatogaram / false:spectrum 
			 */
			void setThreshold( 
					kome::img::Graphics& graphics,
					bool bflg,
					bool& refresh, 
					bool bchromflg
			);

			/**
			 * @fn void setSettings( int endPos, bool& refresh, bool draggingFlg )
			 * @brief 各種設定を行う
			 * @param endPos マウスの終了位置(y軸)
			 * @param refresh return refresh flag
			 */
			void setSettings( int endPos, bool& refresh );
		// @date 2011.03.29 <Add> M.Izumi <-

		public:
			/**
			 * @fn static PeakLabelDrawer& getInstance()
			 * @brief gets instance. (This is the only object.)
			 * @return PeakLabelDrawer object
			 */
			static PeakLabelDrawer& getInstance();
		};
	}
}

#endif		//  __KOME_VIEW_PEAK_LABEL_DRAWER_H__
